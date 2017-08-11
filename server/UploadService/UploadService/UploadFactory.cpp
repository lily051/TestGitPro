#include "StdAfx.h"
#include <stdio.h>
#include "UploadFactory.h"
#include "DBDataMgr.h"
#include "Msger.h"
#include "Config.h"
#include "Storage/StorageMgr.h"
#include "../util/fclib/fcTchar.h"
#include "../util/StringConverter.h"
#include "type/DevType.pb.h"
#include <hak.h>
#include <HPR_Config.h>


SINGLETON_IMPLEMENT(CUploadFactory)//<单例相关实现

#define WORKTIME_IN         1   //在工作时间内
#define WORKTIME_OUT        0   //不在工作时间内
#define WORKTIME_ERROR      -1  //转换时间出错

#define MAX_FAIL_TIME       20  //最大失败次数

#define HEARTBEAT_WORK_INTERVAL    30 //心跳间隔

CUploadFactory::CUploadFactory(void)
{
    m_bInit = false;
    m_bStart = false;
    m_bStopWorkerThread = false;
    m_hHeartBeatThread = NULL;
    m_hUploadThread = NULL;
    m_hGetStatusThread = NULL;
    m_hGetRecordsThread = NULL;
    m_hStorageEvent = NULL;
	m_bStorageReturn = FALSE;
    //m_hGetUsersEvent = NULL;
    m_mapRecordStatus.clear();
    InitializeCriticalSection(&m_csListRecords);
    InitializeCriticalSection(&m_csMapRecordStatus);
}

CUploadFactory::~CUploadFactory(void)
{
    try
    {
        DeleteCriticalSection(&m_csListRecords);
        DeleteCriticalSection(&m_csMapRecordStatus);
    }
    catch (...)
    {
        
    }
    
}

/**	@fn	    _UploadThreadFun
*	@brief	上传录像线程函数
*	@param  [in] pArguments指向CUploadFactory对象
*	@return	
*/
unsigned __stdcall _UploadThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleUpload();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _GetStatusThreadFun
*	@brief	获取上传状态线程
*	@param  [in] pArguments指向CUploadFactory对象
*	@return	
*/
unsigned __stdcall _GetStatusThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleGetStatus();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _GetRecordsThreadFun
*	@brief	获取录像信息线程
*	@param  [in] pArguments指向CUploadFactory对象
*	@return	
*/
unsigned __stdcall _GetRecordsThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleGetRecords();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _HeartBeatThreadFun
*	@brief	心跳线程
*	@param  [in] pArguments指向CUploadFactory对象
*	@return	
*/
unsigned __stdcall _HeartBeatThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleHeartBeat();
    _endthreadex(0);
    return 0;
}

/**	@fn	    Init
*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CUploadFactory::Init()
{
    ULSERVER_LOG_INFO("CUploadFactory::Init In.........");
    if (m_bInit)
    {
        //已经初始化过了
        ULSERVER_LOG_ERROR("CUploadFactory::Init Already");
        return HPR_ERROR;
    }
    
    for (int i = 0; i < STORAGE_TYPE_MAX; ++i)
    {
        if (!CStorageMgr::GetSingletonPtr()->Init((ENUM_STORAGE_TYPE)i))
        {
            ULSERVER_LOG_ERROR("CStorage init failed.");
            return HPR_ERROR;
        }
    }
    m_bInit = true;
    ULSERVER_LOG_INFO("CUploadFactory::Init Out.......");
    return HPR_OK;
}

/**	@fn	    Fini
*	@brief	反初始化函数，负责资源释放
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CUploadFactory::Fini()
{
    ULSERVER_LOG_INFO("CUploadFactory::Fini............In");
    if (!m_bInit)
    {
        //已经释放
        ULSERVER_LOG_ERROR("CUploadFactory::Fini Already");
        return HPR_ERROR;
    }

	m_bInit = false;
    //释放存储设备内存
    CStorageMgr::GetSingletonPtr()->Fini();
    Stop();

    ULSERVER_LOG_INFO("CUploadFactory::Fini.............Out");
    return HPR_OK;
}

/**	@fn	    Start
*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CUploadFactory::Start()
{
    ULSERVER_LOG_INFO("CNetworkModuleMgr::Start in...........");
    if (!m_bInit || m_bStart)
    {
        ULSERVER_LOG_ERROR("CNetworkModuleMgr has no Init or Already Start");
        return HPR_ERROR;
    }
    m_bStopWorkerThread = false;
    unsigned threadID = 0;
    m_hStorageEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    //启用IPC模式才启动心跳线程
    if (1 == CConfig::GetSingletonPtr()->m_nMode)
    {
        if (NULL == m_hHeartBeatThread)
        {
            m_hHeartBeatThread = (HANDLE)_beginthreadex(NULL, 0, &_HeartBeatThreadFun, this, 0, &threadID);
            if (NULL == m_hHeartBeatThread)
            {
                ULSERVER_LOG_ERROR("Create _HeartBeatThreadFun Fail!!!");
                return HPR_ERROR;
            }
        }
    }
    //获取录像线程
    if (NULL == m_hGetRecordsThread)
    {
        m_hGetRecordsThread = (HANDLE)_beginthreadex(NULL, 0, &_GetRecordsThreadFun, this, 0, &threadID);
        if (NULL == m_hGetRecordsThread)
        {
            ULSERVER_LOG_ERROR("Create _GetRecordsThreadFun Fail!!!");
            return HPR_ERROR;
        }
    }

    //上传线程
    if (NULL == m_hUploadThread)
    {
        m_hUploadThread = (HANDLE)_beginthreadex(NULL, 0, &_UploadThreadFun, this, 0, &threadID);
        if (NULL == m_hUploadThread)
        {
            ULSERVER_LOG_ERROR("Create _UploadThreadFun Fail!!!");
            return HPR_ERROR;
        }
    }

    //获取上传状态线程
    if (NULL == m_hGetStatusThread)
    {
        m_hGetStatusThread = (HANDLE)_beginthreadex(NULL, 0, &_GetStatusThreadFun, this, 0, &threadID);
        if (NULL == m_hGetStatusThread)
        {
            ULSERVER_LOG_ERROR("Create _GetStatusThreadFun Fail!!!");
            return HPR_ERROR;
        }   
    }
 
	//启用USB模式才删除录像
	if (2 == CConfig::GetSingletonPtr()->m_nMode/* && CConfig::GetSingletonPtr()->m_isAutoDelFile*/)
	{
		ULSERVER_LOG_INFO("request to get uploaded record to delete");
		CDBDataMgr::GetSingletonPtr()->GetUploadedRecords();//获取已上传录像，删除本地录像文件
	}
    ULSERVER_LOG_INFO("CConnSessionMgr::Start ...Out");
    m_bStart = true;
    return HPR_OK;
}

/**	@fn	    Stop
*	@brief	停止函数
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CUploadFactory::Stop()
{
    m_bStopWorkerThread = true;
    if (m_hGetRecordsThread != NULL)
    {
        WaitForSingleObject(m_hGetRecordsThread, INFINITE);
        CloseHandle(m_hGetRecordsThread);
        m_hGetRecordsThread = NULL;
    }
    if (m_hGetStatusThread != NULL)
    {
        WaitForSingleObject(m_hGetStatusThread, INFINITE);
        CloseHandle(m_hGetStatusThread);
        m_hGetStatusThread = NULL;
    }
    if (m_hUploadThread != NULL)
    {
        WaitForSingleObject(m_hUploadThread, INFINITE);
        CloseHandle(m_hUploadThread);
        m_hUploadThread = NULL;
    }
    if (m_hHeartBeatThread != NULL)
    {
        WaitForSingleObject(m_hHeartBeatThread, INFINITE);
        CloseHandle(m_hHeartBeatThread);
        m_hHeartBeatThread = NULL;
    }
    if (m_hStorageEvent != NULL)
    {
        CloseHandle(m_hStorageEvent);
        m_hStorageEvent = NULL;
    }
    {
        CGuard guard(&m_csListRecords);
        m_listRecords.clear();
    }
    {
        CGuard guard(&m_csMapRecordStatus);
        m_mapRecordStatus.clear();
    }

    return HPR_OK;
}

/**	@fn	    AddRecordsList
*	@brief	添加录像信息列表
*	@param  [in] recordsList -- 录像信息列表
*   @param  [in] bFinish -- 是否结束
*	@return	
*/
void CUploadFactory::AddRecordsList(const cms_8120::RecordsList& recordsList, BOOL bFinish /*= true*/)
{
    if (bFinish)
    {
        ULSERVER_LOG_INFO("RecordsList size:%d", recordsList.record_size());
        int nSize = 0;
        for (int i = 0; i < recordsList.record_size() && !m_bStopWorkerThread; ++i)
        {
            const cms_8120::Records& recordInfo = recordsList.record(i);
            {
                //std::list<cms_8120::Records> listRecords;
                {
                    CGuard guard(&m_csListRecords);
                    std::list<cms_8120::Records>::iterator iter = m_listRecords.begin();
                    std::list<cms_8120::Records>::iterator iterE = m_listRecords.end();
                    BOOL bFind = FALSE;
                    //查找该条录像是否正在执行回传任务，如果在执行不存入map
                    for (; iter != iterE; ++iter)
                    {
                        if (iter->id() == recordInfo.id())
                        {
                            bFind = TRUE;
                            break;
                        }  
                        
                    }
                    if (!bFind)
                    { 
                        //判断正在执行的任务数是否达到上限
                        if (m_listRecords.size() >= (CConfig::GetSingletonPtr()->m_nTaskCount))
                        {
                            ULSERVER_LOG_ERROR("There are so many task uploading.");
                            return;
                        }
                        m_listRecords.push_back(recordInfo);
                    }

                }

            }                     
        }
    }
    else
    {
        ULSERVER_LOG_INFO("AddRecordsList....one time not send all records!");
    }
}

/**	@fn	    DeleteLocalRecordFiles
*	@brief	删除本地录像文件
*	@param  [in] recordsList -- 录像信息列表
*	@return	
*/
void CUploadFactory::DeleteLocalRecordFiles(const cms_8120::RecordsList& recordsList)
{
	ULSERVER_LOG_INFO("DeleteLocalRecordFiles-RecordsList size:%d", recordsList.record_size());
	for (int i = 0; i < recordsList.record_size() && !m_bStopWorkerThread; ++i)
	{
		const cms_8120::Records& recordInfo = recordsList.record(i);
		
		std::string         strRecordName;   //记录名称
		std::string         strFilePath;     //本地文件路径
		strRecordName = CSCT::Utf82Ansi(recordInfo.strrecordname());
		strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());  
		if (0 != remove(strFilePath.c_str()))  //删除失败
		{
			ULSERVER_LOG_ERROR("delete record fail(%d), recordName:%s, recordPath:%s", GetLastError(), strRecordName.c_str(), strFilePath.c_str());
		}
		else
		{
			ULSERVER_LOG_INFO("delete record succ, recordName:%s, recordPath:%s", strRecordName.c_str(), strFilePath.c_str());
		}
	}
}

/**	@fn	    RunCycleUpload[线程函数]
*	@brief	上传录像
*	@param  [in] 无
*	@return	
*/
void CUploadFactory::RunCycleUpload()
{
    ULSERVER_LOG_INFO("RunCycleUpload........IN");

    int nFailTime = 0;
    while (!m_bStopWorkerThread)
    {
        BOOL bHasRecord = FALSE;
        cms_8120::Records recordInfo;
        do
        {
            CGuard guard(&m_csListRecords);
            if (!m_listRecords.empty())
            {
                recordInfo = m_listRecords.front();
                bHasRecord = TRUE;
            }
        }while(0);

        //没找到数据
        if (!bHasRecord)
        {
            Sleep(500);
            continue;
        }
		ULSERVER_LOG_INFO("record info(recordName:%s, id:%d, recordStorageType:%d, streamNo:%s, startTime:%s, stopTime:%s, filePath:%s)", 
            CSCT::Utf82Ansi(recordInfo.strrecordname()).c_str(), recordInfo.id(), recordInfo.nrecordfiletype(), CSCT::Utf82Ansi(recordInfo.streamno()).c_str(), 
            CSCT::Utf82Ansi(recordInfo.starttime()).c_str(), CSCT::Utf82Ansi(recordInfo.endtime()).c_str(),
            CSCT::Utf82Ansi(recordInfo.strurl()).c_str());
        Msger_NotifyMsg(ALL_USER, _T("获取录像信息,录像ID:%d, 录像名称:%s"),
            recordInfo.id(), CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
        //录像信息转换
        Struct_RecordInfo struRecord;
        convertRecordInfo(recordInfo, struRecord);
		if (0 == CConfig::GetSingletonPtr()->m_nHaveCenterStorage) //无中心存储只锁定前端录像
		{
			if (!lockDvrRecord(struRecord))
			{
				lockFailManage(recordInfo, "锁定录像失败(提示:请检查该录像是否存在)", nFailTime);   
				ULSERVER_LOG_ERROR("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], lock record failed.nFailTime:%d", 
					struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
					struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
					struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
					struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
					struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond,
					nFailTime);
				Msger_NotifyMsg(ALL_USER, _T("锁定录像失败,录像名称:%s"),
					CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
				Sleep(1000);
				continue;
			}
			else
			{
				ULSERVER_LOG_INFO("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], lock record succeed.", 
					struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
					struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
					struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
					struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
					struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
				Msger_NotifyMsg(ALL_USER, _T("锁定录像成功, 录像名称:%s"),
					CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());

				BOOL bOver = uploadOver(recordInfo.id(), struRecord);
				if (bOver)
				{
					ULSERVER_LOG_INFO("lock record over.record id:%d, record name:%s", recordInfo.id(), struRecord.strRecordName.c_str());
				}  
				{
					CGuard guard(&m_csListRecords);
					m_listRecords.pop_front();
				}
				//完成锁定重置失败次数
				nFailTime = 0;
				//锁定任务时间间隔m_nUploadInteval*1000秒
				for (int i = 0; i < CConfig::GetSingletonPtr()->m_nUploadInteval; ++i)
				{
					Sleep(1000);
				}
				continue;
			}
		}  
        //本地文件上传
        if (RECORD_STORAGETYPE_LOCAL_KMS == struRecord.recordStorageType || RECORD_STORAGETYPE_OBJECT_CLOUD == struRecord.recordStorageType)
        {
            //判断本地录像文件是否存在
            if (!PathFileExists(fcA2T(struRecord.strFilePath.c_str())))
            {
                //CGuard guard(&m_csListRecords);
                //m_listRecords.pop_front();
				uploadFailManage(recordInfo, "录像文件不存在", nFailTime);
				ULSERVER_LOG_ERROR("the file is not exist.filename:%s,filepath:%s", 
					struRecord.strRecordName.c_str(),struRecord.strFilePath.c_str());
				Msger_NotifyMsg(ALL_USER, _T("录像文件不存在, 文件名:%s,文件路径:%s"), 
                    CSCT::Ansi2Unicode(struRecord.strRecordName).c_str(),
					CSCT::Ansi2Unicode(struRecord.strFilePath).c_str());
                continue;
            }
			ULSERVER_LOG_INFO("find the file.filename:%s,filepath:%s", struRecord.strRecordName.c_str(),struRecord.strFilePath.c_str());
        }
        //DVR录像需要根据DVR的ip获取存储设备  
        if (RECORD_STORAGETYPE_NORMAL == struRecord.recordStorageType)
        {
            //获取存储信息
            ::ResetEvent(m_hStorageEvent);
            if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetStorageInfo(struRecord.strDvrIp, struRecord.nDvrPort))
            {
                ULSERVER_LOG_ERROR("GetStorageInfo failed.");
            }
            if (WAIT_OBJECT_0 != WaitForSingleObject(m_hStorageEvent, 1000*10))
            {
                //获取存储信息失败处理
                uploadFailManage(recordInfo, "获取存储设备信息超时", nFailTime);
                ULSERVER_LOG_ERROR("DVR ip:%s, port:%d, get storage info failed.nFailTime:%d", 
                    struRecord.strDvrIp.c_str(), struRecord.nDvrPort, nFailTime);
                Msger_NotifyMsg(ALL_USER, _T("获取存储设备信息超时，采集设备IP:%s, 端口:%d"), 
                    CSCT::Ansi2Unicode(struRecord.strDvrIp).c_str(), struRecord.nDvrPort);
                continue;
            }
            if (FALSE == m_bStorageReturn)
            {
                //获取存储信息失败处理
                uploadFailManage(recordInfo, "获取存储设备信息失败", nFailTime);
                ULSERVER_LOG_ERROR("DVR ip:%s, port:%d, get storage info failed.nFailTime:%d", 
                    struRecord.strDvrIp.c_str(), struRecord.nDvrPort, nFailTime);
                Msger_NotifyMsg(ALL_USER, _T("获取存储设备信息失败，采集设备IP:%s, 端口:%d"), 
                    CSCT::Ansi2Unicode(struRecord.strDvrIp).c_str(), struRecord.nDvrPort);
                continue;
            }
            struRecord.struStorage = m_struStorage;
        }
           
        //存储器登录句柄
        LONG lLoginID = STORAGE_LOGIN_FAILED;
        lLoginID = CStorageMgr::GetSingletonPtr()->LoginManage(struRecord.struStorage);
        if (STORAGE_LOGIN_FAILED == lLoginID)
        {
            uploadFailManage(recordInfo, "登录存储设备失败", nFailTime);              
            ULSERVER_LOG_ERROR("Storage ip:%s, port:%d, login failed.nFailTime:%d", 
                struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, nFailTime);
            Msger_NotifyMsg(ALL_USER, _T("登录存储设备失败,存储器IP:%s, 端口:%d"), 
                CSCT::Ansi2Unicode(struRecord.struStorage.strIp).c_str(), struRecord.struStorage.nPort);
            continue;
        }
        //在上传录像前先获取文件的上传状态，如果能获取到则为已上传录像，跳过
        if ((RECORDSTAT_UPLOADING == struRecord.recordStatus) || (CStorageMgr::GetSingletonPtr()->GetUploadStatus(lLoginID, struRecord)))
        {
            //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
            //存入上传状态MAP      
            {
                CGuard guard(&m_csMapRecordStatus);
                std::map<int, Struct_RecordInfo>::iterator iter = m_mapRecordStatus.find(recordInfo.id());
                if (iter == m_mapRecordStatus.end())
                {
                    m_mapRecordStatus[recordInfo.id()] = struRecord;
                }               
            }
            {
                CGuard guard(&m_csListRecords);
                m_listRecords.pop_front();
            }
            ULSERVER_LOG_ERROR("record id:%d, camera id:%s, the task maybe exist.", 
                recordInfo.id(), struRecord.strCameraID.c_str());
            Msger_NotifyMsg(ALL_USER, _T("该录像正在上传,录像名称:%s"), 
                CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
			//重置失败次数
			nFailTime = 0;
            continue;
        }
        //录像上传
        if (!CStorageMgr::GetSingletonPtr()->Upload(lLoginID, struRecord))
        {
            //上传失败登出设备
            CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
            uploadFailManage(recordInfo, "上传失败(提示:请检查存储设备容量)", nFailTime);   
            ULSERVER_LOG_ERROR("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], upload failed.nFailTime:%d", 
                struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
                struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
                struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
                struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
                struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond,
                nFailTime);
            Msger_NotifyMsg(ALL_USER, _T("设置上传任务失败,录像名称:%s"),
                CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
            continue;
        } 
        ULSERVER_LOG_INFO("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], set upload task succeed.", 
            struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
            struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
            struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
            struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
            struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
        Msger_NotifyMsg(ALL_USER, _T("设置上传任务成功, 录像名称:%s"),
            CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
        //KMS不获取上传状态，如果存在直接设置为上传成功
        if (STORAGE_TYPE_KMS == struRecord.struStorage.storageType)
        {
            BOOL bOver = uploadOver(recordInfo.id(), struRecord);
            if (bOver)
            {
                ULSERVER_LOG_INFO("KMS Upload record over.record id:%d, camera id:%s, filepath:%s, filesize:%d",
                    recordInfo.id(), struRecord.strCameraID.c_str(), 
                    struRecord.strFilePath.c_str(),
                    struRecord.nFileSize);
                {
                    CGuard guard(&m_csListRecords);
                    m_listRecords.pop_front();
                }
				//完成上传重置失败次数
				nFailTime = 0;
                continue;
            }  
        }
		else if (STORAGE_TYPE_OBJECT_CLOUD == struRecord.struStorage.storageType)
		{
			BOOL bOver = uploadOver(recordInfo.id(), struRecord);
			if (bOver)
			{
				ULSERVER_LOG_INFO("ObjectCloud Upload record over.record name:%s,record id:%d, object key:%s, filepath:%s, startTime:%s, endTime:%s",
					struRecord.strRecordName.c_str(),
					recordInfo.id(), 
					struRecord.strCameraID.c_str(), 
					struRecord.strFilePath.c_str(),
					struRecord.strStartTime,
					struRecord.strEndTime);
				{
					CGuard guard(&m_csListRecords);
					m_listRecords.pop_front();
				}
				//完成上传重置失败次数
				nFailTime = 0;
				continue;
			}  
		}
        //存入上传状态MAP  
        else
        {
            //将录像上传状态置为上传中
            CDBDataMgr::GetSingletonPtr()->SetRecordStatus(recordInfo.id(), RECORDSTAT_UPLOADING, struRecord.nTaskID, 0);
            CGuard guard(&m_csMapRecordStatus);
            m_mapRecordStatus[recordInfo.id()] = struRecord;
        }
        {
            CGuard guard(&m_csListRecords);
            m_listRecords.pop_front();
        }
        
        //上传完成登出设备
        //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
        //完成上传重置失败次数
        nFailTime = 0;
        //上传任务时间间隔m_nUploadInteval*1000秒
        for (int i = 0; i < CConfig::GetSingletonPtr()->m_nUploadInteval; ++i)
        {
            Sleep(1000);
        }
    }
    ULSERVER_LOG_INFO("RunCycleUpload........OUT");
}

/**	@fn	    RunCycleGetStatus[线程函数]
*	@brief	获取状态
*	@param  [in] 无
*	@return	
*/
void CUploadFactory::RunCycleGetStatus()
{
    ULSERVER_LOG_INFO("RunCycleGetStatus........IN");

    while (!m_bStopWorkerThread)
    {    
        if (m_mapRecordStatus.empty())
        {
            Sleep(500);
            continue;
        }
        CGuard guard(&m_csMapRecordStatus);
        std::map<int, Struct_RecordInfo>::iterator iter = m_mapRecordStatus.begin();
        std::map<int, Struct_RecordInfo>::iterator iterE = m_mapRecordStatus.end();
        for (; iter != iterE; /*++iter*/)
        {
            BOOL bDelRecord = FALSE;    //是否从MAP中删除录像记录
            int nRecordID = iter->first;
            Struct_RecordInfo struRecord = iter->second;
            LONG lLoginID = STORAGE_LOGIN_FAILED;
            lLoginID = CStorageMgr::GetSingletonPtr()->LoginManage(struRecord.struStorage);
            if (STORAGE_LOGIN_FAILED == lLoginID)
            {
                //删除map中的录像信息
                m_mapRecordStatus.erase(iter++);
                ULSERVER_LOG_ERROR("Login Storage failed.ip:%s, port:%d", 
                    struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort);
                continue;
            }
            //获取录像状态
            if (!CStorageMgr::GetSingletonPtr()->GetUploadStatus(lLoginID, struRecord))
            {
                CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
                //删除map中的录像信息
                m_mapRecordStatus.erase(iter++);
                ULSERVER_LOG_ERROR("GetUploadStatus failed.record id:%d", nRecordID);
                Msger_NotifyMsg(ALL_USER, _T("获取录像状态失败, 录像名称:%s"), 
                    CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
                continue;
            }
            //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
            //上传完成录像处理
            if (RECORDSTAT_OVER == struRecord.recordStatus)
            {
				Struct_UploadRecordParam struRecordParam;
				//获取已上传录像信息
				if (!CStorageMgr::GetSingletonPtr()->GetUploadRecordInfo(lLoginID, struRecord, struRecordParam))
				{
					CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
					//删除map中的录像信息
					m_mapRecordStatus.erase(iter++);
					ULSERVER_LOG_ERROR("GetUploadRecordInfo failed.record id:%d", nRecordID);
					Msger_NotifyMsg(ALL_USER, _T("获取已上传录像信息失败, 录像名称:%s"), 
						CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
					continue;
				}
				if (struRecordParam.dwRecordNum <= 0)
				{
					ULSERVER_LOG_ERROR("find no record in storage device.record id:%d, record name:%s, camera id:%s, startTime:%d-%d-%d %d:%d:%d, stopTime:%d-%d-%d %d:%d:%d",
						nRecordID, struRecord.strRecordName.c_str(), struRecord.strCameraID.c_str(),
						struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay, 
						struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
						struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay, 
						struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
					std::string strErrorMsg;
					strErrorMsg = "存储设备无录像";
					if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordFail(nRecordID, strErrorMsg))
					{
						ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d, status:fail", nRecordID);
					}
					else
					{
						Msger_AlarmMsg(WORK_TYPE_TRANSPORT_SVC_FAIL,
							ALL_USER,
							ALL_CHANNEL,
							ALARM_OCCUR,
							TP_ALARM_STORAGE,
							_T("%s上传失败"),
							CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
					}
				}
                else
                {
					std::string strTips;
					setUserTips(struRecord, struRecordParam, strTips);
					if (uploadOver(nRecordID, struRecord, strTips))
					{
						ULSERVER_LOG_INFO("Upload record over.record id:%d, record name:%s, camera id:%s, startTime:%d-%d-%d %d:%d:%d, stopTime:%d-%d-%d %d:%d:%d",
							nRecordID, struRecord.strRecordName.c_str(), struRecord.strCameraID.c_str(), 
							struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay, 
							struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
							struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay, 
							struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
						Msger_NotifyMsg(ALL_USER,_T("上传完成, 录像名称:%s"), 
							CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
					}
                }              
                //删除map中的录像信息
                m_mapRecordStatus.erase(iter++);  
            }
            //上传中录像处理
            else if (RECORDSTAT_UPLOADING == struRecord.recordStatus)
            {
                if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordStatus(nRecordID, RECORDSTAT_UPLOADING, struRecord.nTaskID, struRecord.nUploadPercent))
                {
                    ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d, status:uploading", nRecordID);
                }
                ++iter;
            }
            //上传失败录像处理
            else if (RECORDSTAT_FAIL == struRecord.recordStatus)
            {
				ULSERVER_LOG_ERROR("Upload fail.record id:%d, record name:%s, camera id:%s, startTime:%d-%d-%d %d:%d:%d, stopTime:%d-%d-%d %d:%d:%d",
					nRecordID, struRecord.strRecordName.c_str(), struRecord.strCameraID.c_str(),
					struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay, 
					struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
					struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay, 
					struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
                std::string strErrorMsg;
                strErrorMsg = "执行回传任务失败";
                if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordFail(nRecordID, strErrorMsg))
                {
                    ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d, status:fail", nRecordID);
                }
                else
                {
                    Msger_AlarmMsg(WORK_TYPE_TRANSPORT_SVC_FAIL,
                        ALL_USER,
                        ALL_CHANNEL,
                        ALARM_OCCUR,
                        TP_ALARM_STORAGE,
                        _T("%s上传失败"),
                        CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
                }
                //删除map中的录像信息
                m_mapRecordStatus.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
        //获取状态时间间隔m_nUploadInteval*1000秒
        for (int i = 0; i < CConfig::GetSingletonPtr()->m_nStatusInteval; ++i)
        {
            Sleep(1000);
        }
    }
    ULSERVER_LOG_INFO("RunCycleGetStatus........OUT");
}

/**	@fn	    RunCycleGetRecords[线程函数]
*	@brief	获取录像
*	@param  [in] 无
*	@return	
*/
void CUploadFactory::RunCycleGetRecords()
{
    ULSERVER_LOG_INFO("RunCycleGetRecords........IN");

    time_t lastGetDataTime = time(NULL) - CConfig::GetSingletonPtr()->m_nUpdateDataTime;
    while (!m_bStopWorkerThread)
    {
        int nRes = isWorkingTime(CConfig::GetSingletonPtr()->m_strStartTime, CConfig::GetSingletonPtr()->m_strStopTime);
        if (WORKTIME_OUT == nRes || WORKTIME_ERROR == nRes)
        {
            //未到工作时间或者工作时间转换错误，间隔1s再重新执行
            ULSERVER_LOG_ERROR("It is not work time or convert time errro.nRes:%d", nRes);
            Sleep(1000);
            continue;
        }
        
        time_t currentTime = time(NULL);
        if (lastGetDataTime < currentTime)
        {            
            CDBDataMgr::GetSingletonPtr()->GetRecordByStatus();
            Msger_NotifyMsg(ALL_USER, _T("请求获取录像信息"));
            ULSERVER_LOG_INFO("request to get record");
            lastGetDataTime = CConfig::GetSingletonPtr()->m_nUpdateDataTime + time(NULL);
        }
        Sleep(100);
        
    }
    ULSERVER_LOG_INFO("RunCycleGetRecords........OUT");
}

/**	@fn	    HandleStorage
*	@brief	处理存储设备信息
*	@param  [in] storage -- 存储设备信息
*   @param  [in] bFinish -- 是否结束
*	@return	
*/
void CUploadFactory::HandleStorage(const cms_8120::RspStorageConfig& storage, BOOL bFinish /*= true*/)
{
    m_bStorageReturn = TRUE;
    //DC返回失败结果函数将直接返回
    if (-1 == storage.result())
    {
        m_bStorageReturn = FALSE;
        ::SetEvent(m_hStorageEvent);
        return;
    }
    else
    {
        m_bStorageReturn = TRUE;
    }   
    m_struStorage.strIp = CSCT::Utf82Ansi(storage.storageip());
    m_struStorage.nPort = storage.storageport();
    m_struStorage.strUser = CSCT::Utf82Ansi(storage.storageusername());
    std::string strPwd = CSCT::Utf82Ansi(storage.storagepwd());
    CConfig::GetSingletonPtr()->Deciphering(strPwd);
    m_struStorage.strPwd = strPwd;
    switch(storage.type())
    {
    case cms_8120::DEV_TYPE_CVR:    //CVR
        {
            m_struStorage.storageType = STORAGE_TYPE_CVR;
        }
        break;
    case cms_8120::DEV_TYPE_CLOUD_STORAGE:  //云存储
        {
            m_struStorage.storageType = STORAGE_TYPE_CLOUD;
        }
        break;
    case cms_8120::DEV_TYPE_KMS:    //KMS
        {
            m_struStorage.storageType = STORAGE_TYPE_KMS;
        }
        break;
    default:
        {
            ULSERVER_LOG_ERROR("the storage type not support!type:%d", storage.type());
        }
        break;
    }
    m_struStorage.nPoolId = storage.poolid();
    ::SetEvent(m_hStorageEvent);
}

/**	@fn	    isWorkingTime
*	@brief	判断是否在工作时间内
*	@param  [in] strStartTime -- 开始时间
*   @param  [in] strStopTime -- 结束时间
*	@return	int, -1表示错误，0表示不在工作时间内，1表示在工作时间内
*/
int CUploadFactory::isWorkingTime(const std::string &strStartTime, const std::string &strStopTime)
{
    COleDateTime startTime;
    COleDateTime stopTime;
    if (!convertToCTime(strStartTime, strStopTime, startTime, stopTime))
    {
        Msger_AlarmMsg(WORK_TYPE_TRANSPORT_SVC_FAIL,
            ALL_USER,
            ALL_CHANNEL,
            ALARM_OCCUR,
            TP_ALARM_DB,
            _T("检查是否是工作时间时转换时间失败"));
        return WORKTIME_ERROR;
    }

    COleDateTime nowTime = COleDateTime::GetCurrentTime();
    startTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        startTime.GetHour(), startTime.GetMinute(), startTime.GetSecond());
    stopTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        stopTime.GetHour(), stopTime.GetMinute(), stopTime.GetSecond());

    // 认定时间一样为全天工作
    if (startTime == stopTime)
    {
        return WORKTIME_IN;
    }
    else if (startTime < stopTime)
    {
        if (startTime <= nowTime && nowTime <= stopTime)
        {
            return WORKTIME_IN;
        }
        return WORKTIME_OUT;
    }
    else
    {
        if (stopTime < nowTime && nowTime < startTime)
        {
            return WORKTIME_OUT;
        }
        return WORKTIME_IN;
    }      
}

/**	@fn	    convertToCTime
*	@brief	转换时间类型（string-COleDateTime）
*	@param  [in] strStartTime -- 开始时间
*   @param  [in] strStopTime -- 结束时间
*	@param  [out] startTime -- 开始时间
*   @param  [out] stopTime -- 结束时间
*	@return	BOOL
*/
BOOL CUploadFactory::convertToCTime(const std::string &strStartTime, const std::string &strStopTime, COleDateTime &startTime, COleDateTime &stopTime)
{
    std::_tstring strTemp = fcA2T(strStartTime.c_str());
    if (!startTime.ParseDateTime(strTemp.c_str()))
    {
        ULSERVER_LOG_ERROR("parse start time failed: %s", 
            strStartTime.c_str());
        return FALSE;
    }
    strTemp = fcA2T(strStopTime.c_str());
    if (!stopTime.ParseDateTime(strTemp.c_str()))
    {
        ULSERVER_LOG_ERROR("parse stop time failed: %s", 
            strStopTime.c_str());
        return FALSE;
    }

    return TRUE;
}

/**	@fn	    setRecordTime
*	@brief	设置时间信息
*	@param  [in] nTime -- 时间戳
*	@param  [out] struTime -- 时间信息
*	@return	BOOL
*/
BOOL CUploadFactory::setRecordTime(const std::string &strTime, Struct_RecordTime &struTime)
{
    sscanf(strTime.c_str(), "%d-%02d-%02d %02d:%02d:%02d", &struTime.dwYear, &struTime.dwMonth, &struTime.dwDay,
        &struTime.dwHour, &struTime.dwMinute, &struTime.dwSecond);

    return TRUE;
}

/**	@fn	    ConvertRecordInfo
*	@brief	转换录像信息
*	@param  [in] recordInfo -- 原录像信息
*   @param  [out] struRecord -- 转换后的录像信息
*	@return	BOOL
*/
BOOL CUploadFactory::convertRecordInfo(const cms_8120::Records &recordInfo, Struct_RecordInfo &struRecord)
{
    struRecord.recordStatus = (ENUM_RECORD_STATUS)recordInfo.nrecordstatus();
    /*struRecord.nUserID = recordInfo.nuserid();*/
    struRecord.recordStorageType = (ENUM_RECORD_STORAGE_TYPE)recordInfo.nrecordfiletype();
    struRecord.strRecordName = CSCT::Utf82Ansi(recordInfo.strrecordname());
    //DVR录像文件设置采集DVR设备信息
    if (RECORD_STORAGETYPE_NORMAL == struRecord.recordStorageType)
    {
        struRecord.strDvrIp = CSCT::Utf82Ansi(recordInfo.strcollectip());
        struRecord.nDvrPort = recordInfo.dvrport();
        struRecord.nChannelID = recordInfo.ncollectchan() + 1;
        struRecord.strDvrUser = CSCT::Utf82Ansi(recordInfo.dvrusername());
        std::string strPwd = CSCT::Utf82Ansi(recordInfo.dvrpwd());
        CConfig::GetSingletonPtr()->Deciphering(strPwd);
        struRecord.strDvrPwd = strPwd;
        struRecord.strCameraID = CSCT::Utf82Ansi(recordInfo.streamno());
        //录像开始时间
        setRecordTime(recordInfo.starttime(), struRecord.struRecStartTime);
        //录像结束时间
        setRecordTime(recordInfo.endtime(), struRecord.struRecStopTime);
        struRecord.nTaskID = recordInfo.ntaskid();
    }
    //本地录像文件设置KMS存储设备信息
    else if (RECORD_STORAGETYPE_LOCAL_KMS == struRecord.recordStorageType)
    {
        struRecord.strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());  //本地文件路径
        struRecord.struStorage.storageType = STORAGE_TYPE_KMS;          //存储设备类型
        struRecord.struStorage.strIp = CSCT::Utf82Ansi(recordInfo.strcollectip());  //存储设备IP
        struRecord.struStorage.nPort = recordInfo.dvrport();                        //存储设备端口
        struRecord.struStorage.strUser = CSCT::Utf82Ansi(recordInfo.dvrusername()); //存储设备用户名
        std::string strPwd = CSCT::Utf82Ansi(recordInfo.dvrpwd());                  //存储设备密码
        CConfig::GetSingletonPtr()->Deciphering(strPwd);
        struRecord.struStorage.strPwd = strPwd; 
    }
	//本地录像文件设置云存储信息
	else if (RECORD_STORAGETYPE_OBJECT_CLOUD == struRecord.recordStorageType)
	{
		struRecord.nID = recordInfo.id();                                                  //数据库记录ID，用作Key
		struRecord.strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());                     //本地文件路径
		struRecord.strHost = CSCT::Utf82Ansi(recordInfo.strip());                          //本地文件所在主机IP地址,用作Key
		struRecord.struStorage.strIp =  CSCT::Utf82Ansi(recordInfo.strcloudip());          //存储设备IP
		struRecord.struStorage.nPort = recordInfo.ncloudport();                            //存储设备端口
		std::string strAccessKey = CSCT::Utf82Ansi(recordInfo.straccesskey());             //对象存储AccessKey
		CConfig::GetSingletonPtr()->Deciphering(strAccessKey);
		struRecord.struStorage.strAccessKey = strAccessKey;
		std::string strSecretKey = CSCT::Utf82Ansi(recordInfo.strsecretkey());             //对象存储SecretKey
		CConfig::GetSingletonPtr()->Deciphering(strSecretKey);
		struRecord.struStorage.strSecretKey = strSecretKey;
		struRecord.strBucket = CSCT::Utf82Ansi(recordInfo.strbucket());                    //对象存储Bucket
		struRecord.fileType = FILE_TYPE_VIDEO;                                             //文件类型，决定对象存储上传方式
		struRecord.struStorage.storageType = STORAGE_TYPE_OBJECT_CLOUD;                    //存储设备类型	                     
		struRecord.strStartTime =  CSCT::Utf82Ansi(recordInfo.starttime());                //本地文件开始时间,用作Key
		struRecord.strEndTime =    CSCT::Utf82Ansi(recordInfo.endtime());                  //本地文件结束时间,用作Key
		//录像开始时间
		setRecordTime(recordInfo.starttime(), struRecord.struRecStartTime);
		//录像结束时间
		setRecordTime(recordInfo.endtime(), struRecord.struRecStopTime);
	}
	else
	{
		ULSERVER_LOG_ERROR("unknown struRecord.recordStorageType");
	}
    
    return TRUE;
}

/**	@fn	    uploadFailManage
*	@brief	上传失败处理
*   @param  [in] recordInfo -- 录像信息
*   @param  [out] nFailTimes -- 上传失败次数
*	@return	void
*/
void CUploadFactory::uploadFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes)
{
    //失败次数小于MAX_FAIL_TIME，加入list重新上传
    ++nFailTimes;
    ULSERVER_LOG_ERROR("upload fail.nFailTime:%d", nFailTimes);
    //失败次数大于MAX_FAIL_TIME，设置录像状态为失败
    if (nFailTimes >= MAX_FAIL_TIME)
    {
		ULSERVER_LOG_ERROR("upload fail,record info(id:%d, recordName:%s, recordStorageType:%d, startTime:%s, stopTime:%s, filePath:%s)", 
			recordInfo.id(), CSCT::Utf82Ansi(recordInfo.strrecordname()).c_str(), recordInfo.nrecordfiletype(),
			CSCT::Utf82Ansi(recordInfo.starttime()).c_str(), CSCT::Utf82Ansi(recordInfo.endtime()).c_str(),
			CSCT::Utf82Ansi(recordInfo.strurl()).c_str());
        if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordFail(recordInfo.id(), strErrorMsg))
        {
            ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", recordInfo.id());            
        }
        Msger_AlarmMsg(WORK_TYPE_TRANSPORT_SVC_FAIL,
            ALL_USER,
            ALL_CHANNEL,
            ALARM_OCCUR,
            TP_ALARM_STORAGE,
            _T("%s上传失败"),
            CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
        //上传失败重置失败次数
        nFailTimes = 0;
        CGuard guard(&m_csListRecords);
        m_listRecords.pop_front();
    }
}

/**	@fn	    lockFailManage
*	@brief	锁定失败处理
*   @param  [in] recordInfo -- 录像信息
*   @param  [out] nFailTimes -- 锁定失败次数
*	@return	void
*/
void CUploadFactory::lockFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes)
{
	//失败次数小于MAX_FAIL_TIME，加入list重新锁定
	++nFailTimes;
	ULSERVER_LOG_ERROR("lock fail.nFailTime:%d", nFailTimes);
	//失败次数大于MAX_FAIL_TIME，设置录像状态为失败
	if (nFailTimes >= MAX_FAIL_TIME)
	{
		ULSERVER_LOG_ERROR("lock fail,record info(id:%d, recordName:%s, recordStorageType:%d, startTime:%s, stopTime:%s, filePath:%s)", 
			recordInfo.id(), CSCT::Utf82Ansi(recordInfo.strrecordname()).c_str(), recordInfo.nrecordfiletype(),
			CSCT::Utf82Ansi(recordInfo.starttime()).c_str(), CSCT::Utf82Ansi(recordInfo.endtime()).c_str(),
			CSCT::Utf82Ansi(recordInfo.strurl()).c_str());
		if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordFail(recordInfo.id(), strErrorMsg))
		{
			ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", recordInfo.id());            
		}
		Msger_AlarmMsg(WORK_TYPE_TRANSPORT_SVC_FAIL,
			ALL_USER,
			ALL_CHANNEL,
			ALARM_OCCUR,
			TP_ALARM_STORAGE,
			_T("%s锁定失败"),
			CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
		//锁定失败重置失败次数
		nFailTimes = 0;
		CGuard guard(&m_csListRecords);
		m_listRecords.pop_front();
	}
}

/**	@fn	    uploadOver
*	@brief	上传完成处理
*	@param  [in] struRecordInfo -- 录像信息
*	@param  [in] strTip-- 提示信息
*	@return	BOOL
*/
BOOL CUploadFactory::uploadOver(const int &nRecordID, const Struct_RecordInfo &struRecordInfo, std::string strTip)
{
    //KMS完成录像上传处理
    if (STORAGE_TYPE_KMS == struRecordInfo.struStorage.storageType)
    {
        if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetKmsRecordStatus(nRecordID, RECORDSTAT_OVER, 
            struRecordInfo.strCameraID, struRecordInfo.strRtspPath, struRecordInfo.nFileSize, struRecordInfo.strStartTime, struRecordInfo.strEndTime))
        {
            ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", nRecordID);
            return FALSE;
        }
        if (CConfig::GetSingletonPtr()->m_isAutoDelFile)
        {
            if (0 != remove(struRecordInfo.strFilePath.c_str()))
            {
				ULSERVER_LOG_ERROR("remove file failed.record id:%d", nRecordID);
            }
        }
        return TRUE;
    }
	else if (STORAGE_TYPE_OBJECT_CLOUD == struRecordInfo.struStorage.storageType)
	{
		if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetObjectCloudRecordStatus(nRecordID, RECORDSTAT_OVER, struRecordInfo.strCameraID, struRecordInfo.strStartTime, struRecordInfo.strEndTime))
		{
			ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", nRecordID);
			return FALSE;
		}
		if (CConfig::GetSingletonPtr()->m_isAutoDelFile)
		{
			if (0 != remove(struRecordInfo.strFilePath.c_str()))
			{
				ULSERVER_LOG_ERROR("remove file failed.record id:%d", nRecordID);
			}
		}
		return TRUE;
	}
    else
    {
        if (!CConfig::GetSingletonPtr()->m_isAutoDelRecord)
        {
            if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordStatus(nRecordID, RECORDSTAT_OVER, 0, 0, strTip))
            {
                ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", nRecordID);
                return FALSE;
            }
        }
        else
        {
            if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordStatus(nRecordID, RECORDSTAT_DELETE, 0, 0, strTip))
            {
                ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d", nRecordID);
                return FALSE;
            }
        }
        return TRUE;
    }

    return TRUE;
}

/**	@fn	    RunCycleHeartBeat[线程函数]
*	@brief	心跳
*	@param  [in] 无
*	@return	
*/
void CUploadFactory::RunCycleHeartBeat()
{
    ULSERVER_LOG_INFO("RunCycleHeartBeat........IN");
    while (!m_bStopWorkerThread)
    {
        CDBDataMgr::GetSingletonPtr()->SendHeartBeat();
        for (int i = 0; i < HEARTBEAT_WORK_INTERVAL && !m_bStopWorkerThread; ++i)
        {
            Sleep(1000);
        }
    }
    ULSERVER_LOG_INFO("RunCycleHeartBeat........IN");
}

/**	@fn	    loginDvr
*	@brief	登录Dvr
*	@param  [in] struStorage -- 设备信息
*	@return	LONG -- 登录句柄
*/
LONG CUploadFactory::loginDvr(const Struct_RecordInfo &struRecordInfo)
{
	LONG lLoginID = STORAGE_LOGIN_FAILED;
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
	memcpy_s(struLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, struRecordInfo.strDvrIp.c_str(), strlen(struRecordInfo.strDvrIp.c_str()));
	struLoginInfo.wPort = struRecordInfo.nDvrPort;
	memcpy_s(struLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, struRecordInfo.strDvrUser.c_str(), strlen(struRecordInfo.strDvrUser.c_str()));
	memcpy_s(struLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, struRecordInfo.strDvrPwd.c_str(), strlen(struRecordInfo.strDvrPwd.c_str()));
	struLoginInfo.cbLoginResult = NULL;
	struLoginInfo.pUser = NULL;
	struLoginInfo.bUseAsynLogin = 0;
	lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
	if (STORAGE_LOGIN_FAILED == lLoginID)
	{
		ULSERVER_LOG_ERROR("call NET_DVR_Login_V40 fail. ip:%s, port:%d, error[%d]",
			struRecordInfo.strDvrIp.c_str(), struRecordInfo.nDvrPort, NET_DVR_GetLastError());
		return lLoginID;
	}
	ULSERVER_LOG_INFO("Login succeed. ip:%s, port:%d",
		struRecordInfo.strDvrIp.c_str(), struRecordInfo.nDvrPort);
	return lLoginID;
}

/**	@fn	    lockDvrRecord
*	@brief	锁定DVR录像
*	@param  [in] struRecordInfo -- 录像信息
*	@return	BOOL
*/
BOOL CUploadFactory::lockDvrRecord(const Struct_RecordInfo &struRecordInfo)
{
	LONG lLoginID = STORAGE_LOGIN_FAILED;
	lLoginID = loginDvr(struRecordInfo);
	if (STORAGE_LOGIN_FAILED == lLoginID)
	{            
		return FALSE;
	}
	//按时间锁定
	//查找文件
	NET_DVR_FILECOND_V40 struFileCond={0};
	struFileCond.dwFileType = 0xFF;
	struFileCond.lChannel = struRecordInfo.nChannelID; //通道号
	struFileCond.dwIsLocked = 0xff; //所有文件
	struFileCond.dwUseCardNo = 0;

	struFileCond.struStartTime.dwYear = struRecordInfo.struRecStartTime.dwYear;
	struFileCond.struStartTime.dwMonth = struRecordInfo.struRecStartTime.dwMonth;
	struFileCond.struStartTime.dwDay = struRecordInfo.struRecStartTime.dwDay;
	struFileCond.struStartTime.dwHour = struRecordInfo.struRecStartTime.dwHour;
	struFileCond.struStartTime.dwMinute = struRecordInfo.struRecStartTime.dwMinute;
	struFileCond.struStartTime.dwSecond = struRecordInfo.struRecStartTime.dwSecond;
	//录像结束时间 
	struFileCond.struStopTime.dwYear = struRecordInfo.struRecStopTime.dwYear;
	struFileCond.struStopTime.dwMonth = struRecordInfo.struRecStopTime.dwMonth;
	struFileCond.struStopTime.dwDay = struRecordInfo.struRecStopTime.dwDay;
	struFileCond.struStopTime.dwHour = struRecordInfo.struRecStopTime.dwHour;
	struFileCond.struStopTime.dwMinute = struRecordInfo.struRecStopTime.dwMinute;
	struFileCond.struStopTime.dwSecond = struRecordInfo.struRecStopTime.dwSecond;

	//查找录像文件 
	int lFindHandle = NET_DVR_FindFile_V40(lLoginID, &struFileCond);
	if (lFindHandle < 0)
	{
		DWORD ulErrorCode = NET_DVR_GetLastError();
		ULSERVER_LOG_ERROR("按时间查找文件失败(%d)%s_%d_%d_%s",
			ulErrorCode,
			struRecordInfo.strDvrIp.c_str(),
			struRecordInfo.nDvrPort,
			struRecordInfo.nChannelID,
			struRecordInfo.strRecordName.c_str());
		Msger_NotifyMsg( ALL_USER, _T("按时间查找文件失败(%d)%s_%d_%d_%s"),
			ulErrorCode,
			CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
			struRecordInfo.nDvrPort,
			struRecordInfo.nChannelID,
			CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
		//注销用户
		NET_DVR_Logout(lLoginID);
		return FALSE;
	}
	ULSERVER_LOG_INFO("按时间查找文件开始：%s_%d_%d_%s",
		struRecordInfo.strDvrIp.c_str(),
		struRecordInfo.nDvrPort,
		struRecordInfo.nChannelID,
		struRecordInfo.strRecordName.c_str());
	NET_DVR_FINDDATA_V40 struFileData = {0};
	time_t beginFind = time(NULL);
	while(true)
	{
		//逐个获取查找到的文件信息
		int iResult = NET_DVR_FindNextFile_V40(lFindHandle, &struFileData);
		if (NET_DVR_ISFINDING == iResult)
		{
			// 这里有可能会无限循环 加个超时判断。
			//如果搜索时间大于30秒 判断为超时。
			if (abs(difftime(beginFind, time(NULL))) > 30)
			{
				ULSERVER_LOG_ERROR("按时间查找文件超时：%s_%d_%d_%s",
					struRecordInfo.strDvrIp.c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					struRecordInfo.strRecordName.c_str());
				Msger_NotifyMsg( ALL_USER, _T("按时间查找文件超时：%s_%d_%d_%s"),
					CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
				NET_DVR_FindClose_V30(lFindHandle);
				NET_DVR_Logout(lLoginID);
				return FALSE;
			}
			continue;
		}
		else if (NET_DVR_FILE_SUCCESS == iResult) //获取文件信息成功
		{
			beginFind = time(NULL);
			ULSERVER_LOG_INFO("查找到文件,%d-%d-%d %d:%d:%d_%d-%d-%d %d:%d:%d",
				struFileData.struStartTime.dwYear,
				struFileData.struStartTime.dwMonth,
				struFileData.struStartTime.dwDay,
				struFileData.struStartTime.dwHour,
				struFileData.struStartTime.dwMinute,
				struFileData.struStartTime.dwSecond,
				struFileData.struStopTime.dwYear,
				struFileData.struStopTime.dwMonth,
				struFileData.struStopTime.dwDay,
				struFileData.struStopTime.dwHour,
				struFileData.struStopTime.dwMinute,
				struFileData.struStopTime.dwSecond);
			//去除只交叉几秒的文件。
			time_t iFileBeginTime = 0;
			time_t iFileEndTime = 0;
			time_t iRecBeginTime = 0;
			time_t iRecEndTime = 0;
			//去除只交叉几秒的文件。
			getDvrTimeStamp(struFileData.struStartTime, iFileBeginTime);
			getDvrTimeStamp(struFileData.struStopTime, iFileEndTime);
			getDvrTimeStamp(struFileCond.struStartTime, iRecBeginTime);
			getDvrTimeStamp(struFileCond.struStopTime, iRecEndTime);
			if(iRecEndTime - 2 <= iFileBeginTime || iRecBeginTime + 2 >= iFileEndTime)
			{
				continue;
			}
			if (1 == struFileData.byLocked)
			{
				continue;
			}
			if (TRUE != NET_DVR_LockFileByName(lLoginID, (char*)(struFileData.sFileName)))
			{
				DWORD ulErrorCode = NET_DVR_GetLastError();
				ULSERVER_LOG_ERROR("DVR锁定录像失败(错误码%d)%s_%d_%d_%s", 
					ulErrorCode,
					struRecordInfo.strDvrIp.c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					struRecordInfo.strRecordName.c_str());
				Msger_NotifyMsg(ALL_USER, _T("DVR锁定录像失败(错误码%d)%s_%d_%d_%s "),
					ulErrorCode,
					CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
				NET_DVR_FindClose_V30(lFindHandle);
				NET_DVR_Logout(lLoginID);
				return FALSE;
			}
		}
		else if(NET_DVR_FILE_NOFIND == iResult)
		{
			ULSERVER_LOG_ERROR("查找到零个文件：%s_%d_%d_%s", 
				struRecordInfo.strDvrIp.c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				struRecordInfo.strRecordName.c_str());
			Msger_NotifyMsg(ALL_USER, _T("查找到零个文件：%s_%d_%d_%s"),
				CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
			NET_DVR_FindClose_V30(lFindHandle);
			NET_DVR_Logout(lLoginID);
			for (int i = 0; i < 10 && !m_bStopWorkerThread; ++i) //录完像立即锁定可能查不到录像（一般两分钟生成录像），等待一段时间下次再锁定
			{
				Sleep(1000);
			}
			return FALSE;
		}
		else if (NET_DVR_NOMOREFILE == iResult)
		{
			ULSERVER_LOG_INFO("NET_DVR_NOMOREFILE");
			break;
		}
		else
		{
			DWORD ulErrorCode = NET_DVR_GetLastError();
			ULSERVER_LOG_ERROR("查找文件异常(错误码%d)%s_%d_%d_%s", 
				ulErrorCode,
				struRecordInfo.strDvrIp.c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				struRecordInfo.strRecordName.c_str());
			Msger_NotifyMsg(ALL_USER, _T("查找文件异常(错误码%d)%s_%d_%d_%s"),
				ulErrorCode,
				CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
			NET_DVR_FindClose_V30(lFindHandle);
			NET_DVR_Logout(lLoginID);
			return FALSE;
		}
	}

	ULSERVER_LOG_INFO("按时间查找文件结束：%s_%d_%d_%s",
		struRecordInfo.strDvrIp.c_str(),
		struRecordInfo.nDvrPort,
		struRecordInfo.nChannelID,
		struRecordInfo.strRecordName.c_str());
	NET_DVR_FindClose_V30(lFindHandle);
	NET_DVR_Logout(lLoginID);
	return TRUE;

}

/**	@fn	    getDvrTimeStamp
*	@brief	得到时间戳
*	@param  [in]  stDvrTimeSt -- 时间
*	@param  [out] iTime -- 时间戳
*	@return	void
*/
void CUploadFactory::getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime)
{
	struct tm  stTime = {0};
	stTime.tm_year  = stDvrTimeSt.dwYear - 1900;
	stTime.tm_mon   = stDvrTimeSt.dwMonth - 1;
	stTime.tm_mday  =  stDvrTimeSt.dwDay;
	stTime.tm_hour  =  stDvrTimeSt.dwHour;
	stTime.tm_min   =  stDvrTimeSt.dwMinute;
	stTime.tm_sec   =  stDvrTimeSt.dwSecond;

	iTime = mktime(&stTime);
}

/**	@fn	    getRecordTimeStamp
*	@brief	得到录像时间戳
*	@param  [in]  stRecordTime -- 录像时间
*	@param  [out] iTime -- 时间戳
*	@return	void
*/
void CUploadFactory::getRecordTimeStamp(const Struct_RecordTime &stRecordTime, time_t &iTime)
{
	struct tm  stTime = {0};
	stTime.tm_year  = stRecordTime.dwYear - 1900;
	stTime.tm_mon   = stRecordTime.dwMonth - 1;
	stTime.tm_mday  =  stRecordTime.dwDay;
	stTime.tm_hour  =  stRecordTime.dwHour;
	stTime.tm_min   =  stRecordTime.dwMinute;
	stTime.tm_sec   =  stRecordTime.dwSecond;

	iTime = mktime(&stTime);
}

/**	@fn	    setUserTips
*	@brief	设置提示
*	@param  [in] struRecordInfo -- 录像信息
*	@param  [in] struRecordParam -- 已上传到存储设备中的录像信息
*	@param  [out] strTip-- 提示信息
*	@return	BOOL
*/
BOOL CUploadFactory::setUserTips(const Struct_RecordInfo &struRecordInfo, const Struct_UploadRecordParam &struRecordParam, std::string &strTip)
{
	char szTime[50] = {0};
	sprintf(szTime, "%4d-%02d-%02d %2d:%02d:%02d,%4d-%02d-%02d %2d:%02d:%02d",     
		struRecordParam.struRecStartTime.dwYear,
		struRecordParam.struRecStartTime.dwMonth,
		struRecordParam.struRecStartTime.dwDay,
		struRecordParam.struRecStartTime.dwHour,
		struRecordParam.struRecStartTime.dwMinute,
		struRecordParam.struRecStartTime.dwSecond,
		struRecordParam.struRecStopTime.dwYear,
		struRecordParam.struRecStopTime.dwMonth,
		struRecordParam.struRecStopTime.dwDay,
		struRecordParam.struRecStopTime.dwHour,
		struRecordParam.struRecStopTime.dwMinute,
		struRecordParam.struRecStopTime.dwSecond);

	ULSERVER_LOG_INFO("已上传录像时间:%s", szTime);

	time_t nRecordStartTime;
	time_t nRecordStopTime;
	time_t nQueryStartTime;
	time_t nQueryStopTime;
	getRecordTimeStamp(struRecordInfo.struRecStartTime, nRecordStartTime);
	getRecordTimeStamp(struRecordInfo.struRecStopTime, nRecordStopTime);
	getRecordTimeStamp(struRecordParam.struRecStartTime, nQueryStartTime);
	getRecordTimeStamp(struRecordParam.struRecStopTime, nQueryStopTime);

	if (difftime(nQueryStartTime, nRecordStartTime) >= CConfig::GetSingletonPtr()->m_nOffsetStartTime || 
		difftime(nRecordStopTime, nQueryStopTime)   >= CConfig::GetSingletonPtr()->m_nOffsetStopTime)
	{
		ULSERVER_LOG_ERROR("上传录像不完整");
		strTip = szTime;
	}
	return TRUE;
}


