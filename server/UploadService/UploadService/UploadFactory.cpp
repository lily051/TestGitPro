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


SINGLETON_IMPLEMENT(CUploadFactory)//<�������ʵ��

#define WORKTIME_IN         1   //�ڹ���ʱ����
#define WORKTIME_OUT        0   //���ڹ���ʱ����
#define WORKTIME_ERROR      -1  //ת��ʱ�����

#define MAX_FAIL_TIME       20  //���ʧ�ܴ���

#define HEARTBEAT_WORK_INTERVAL    30 //�������

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
*	@brief	�ϴ�¼���̺߳���
*	@param  [in] pArgumentsָ��CUploadFactory����
*	@return	
*/
unsigned __stdcall _UploadThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleUpload();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _GetStatusThreadFun
*	@brief	��ȡ�ϴ�״̬�߳�
*	@param  [in] pArgumentsָ��CUploadFactory����
*	@return	
*/
unsigned __stdcall _GetStatusThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleGetStatus();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _GetRecordsThreadFun
*	@brief	��ȡ¼����Ϣ�߳�
*	@param  [in] pArgumentsָ��CUploadFactory����
*	@return	
*/
unsigned __stdcall _GetRecordsThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleGetRecords();
    _endthreadex(0);
    return 0;
}

/**	@fn	    _HeartBeatThreadFun
*	@brief	�����߳�
*	@param  [in] pArgumentsָ��CUploadFactory����
*	@return	
*/
unsigned __stdcall _HeartBeatThreadFun( void* pArguments)
{
    CUploadFactory::GetSingletonPtr()->RunCycleHeartBeat();
    _endthreadex(0);
    return 0;
}

/**	@fn	    Init
*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CUploadFactory::Init()
{
    ULSERVER_LOG_INFO("CUploadFactory::Init In.........");
    if (m_bInit)
    {
        //�Ѿ���ʼ������
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
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CUploadFactory::Fini()
{
    ULSERVER_LOG_INFO("CUploadFactory::Fini............In");
    if (!m_bInit)
    {
        //�Ѿ��ͷ�
        ULSERVER_LOG_ERROR("CUploadFactory::Fini Already");
        return HPR_ERROR;
    }

	m_bInit = false;
    //�ͷŴ洢�豸�ڴ�
    CStorageMgr::GetSingletonPtr()->Fini();
    Stop();

    ULSERVER_LOG_INFO("CUploadFactory::Fini.............Out");
    return HPR_OK;
}

/**	@fn	    Start
*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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
    //����IPCģʽ�����������߳�
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
    //��ȡ¼���߳�
    if (NULL == m_hGetRecordsThread)
    {
        m_hGetRecordsThread = (HANDLE)_beginthreadex(NULL, 0, &_GetRecordsThreadFun, this, 0, &threadID);
        if (NULL == m_hGetRecordsThread)
        {
            ULSERVER_LOG_ERROR("Create _GetRecordsThreadFun Fail!!!");
            return HPR_ERROR;
        }
    }

    //�ϴ��߳�
    if (NULL == m_hUploadThread)
    {
        m_hUploadThread = (HANDLE)_beginthreadex(NULL, 0, &_UploadThreadFun, this, 0, &threadID);
        if (NULL == m_hUploadThread)
        {
            ULSERVER_LOG_ERROR("Create _UploadThreadFun Fail!!!");
            return HPR_ERROR;
        }
    }

    //��ȡ�ϴ�״̬�߳�
    if (NULL == m_hGetStatusThread)
    {
        m_hGetStatusThread = (HANDLE)_beginthreadex(NULL, 0, &_GetStatusThreadFun, this, 0, &threadID);
        if (NULL == m_hGetStatusThread)
        {
            ULSERVER_LOG_ERROR("Create _GetStatusThreadFun Fail!!!");
            return HPR_ERROR;
        }   
    }
 
	//����USBģʽ��ɾ��¼��
	if (2 == CConfig::GetSingletonPtr()->m_nMode/* && CConfig::GetSingletonPtr()->m_isAutoDelFile*/)
	{
		ULSERVER_LOG_INFO("request to get uploaded record to delete");
		CDBDataMgr::GetSingletonPtr()->GetUploadedRecords();//��ȡ���ϴ�¼��ɾ������¼���ļ�
	}
    ULSERVER_LOG_INFO("CConnSessionMgr::Start ...Out");
    m_bStart = true;
    return HPR_OK;
}

/**	@fn	    Stop
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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
*	@brief	���¼����Ϣ�б�
*	@param  [in] recordsList -- ¼����Ϣ�б�
*   @param  [in] bFinish -- �Ƿ����
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
                    //���Ҹ���¼���Ƿ�����ִ�лش����������ִ�в�����map
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
                        //�ж�����ִ�е��������Ƿ�ﵽ����
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
*	@brief	ɾ������¼���ļ�
*	@param  [in] recordsList -- ¼����Ϣ�б�
*	@return	
*/
void CUploadFactory::DeleteLocalRecordFiles(const cms_8120::RecordsList& recordsList)
{
	ULSERVER_LOG_INFO("DeleteLocalRecordFiles-RecordsList size:%d", recordsList.record_size());
	for (int i = 0; i < recordsList.record_size() && !m_bStopWorkerThread; ++i)
	{
		const cms_8120::Records& recordInfo = recordsList.record(i);
		
		std::string         strRecordName;   //��¼����
		std::string         strFilePath;     //�����ļ�·��
		strRecordName = CSCT::Utf82Ansi(recordInfo.strrecordname());
		strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());  
		if (0 != remove(strFilePath.c_str()))  //ɾ��ʧ��
		{
			ULSERVER_LOG_ERROR("delete record fail(%d), recordName:%s, recordPath:%s", GetLastError(), strRecordName.c_str(), strFilePath.c_str());
		}
		else
		{
			ULSERVER_LOG_INFO("delete record succ, recordName:%s, recordPath:%s", strRecordName.c_str(), strFilePath.c_str());
		}
	}
}

/**	@fn	    RunCycleUpload[�̺߳���]
*	@brief	�ϴ�¼��
*	@param  [in] ��
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

        //û�ҵ�����
        if (!bHasRecord)
        {
            Sleep(500);
            continue;
        }
		ULSERVER_LOG_INFO("record info(recordName:%s, id:%d, recordStorageType:%d, streamNo:%s, startTime:%s, stopTime:%s, filePath:%s)", 
            CSCT::Utf82Ansi(recordInfo.strrecordname()).c_str(), recordInfo.id(), recordInfo.nrecordfiletype(), CSCT::Utf82Ansi(recordInfo.streamno()).c_str(), 
            CSCT::Utf82Ansi(recordInfo.starttime()).c_str(), CSCT::Utf82Ansi(recordInfo.endtime()).c_str(),
            CSCT::Utf82Ansi(recordInfo.strurl()).c_str());
        Msger_NotifyMsg(ALL_USER, _T("��ȡ¼����Ϣ,¼��ID:%d, ¼������:%s"),
            recordInfo.id(), CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
        //¼����Ϣת��
        Struct_RecordInfo struRecord;
        convertRecordInfo(recordInfo, struRecord);
		if (0 == CConfig::GetSingletonPtr()->m_nHaveCenterStorage) //�����Ĵ洢ֻ����ǰ��¼��
		{
			if (!lockDvrRecord(struRecord))
			{
				lockFailManage(recordInfo, "����¼��ʧ��(��ʾ:�����¼���Ƿ����)", nFailTime);   
				ULSERVER_LOG_ERROR("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], lock record failed.nFailTime:%d", 
					struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
					struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
					struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
					struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
					struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond,
					nFailTime);
				Msger_NotifyMsg(ALL_USER, _T("����¼��ʧ��,¼������:%s"),
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
				Msger_NotifyMsg(ALL_USER, _T("����¼��ɹ�, ¼������:%s"),
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
				//�����������ʧ�ܴ���
				nFailTime = 0;
				//��������ʱ����m_nUploadInteval*1000��
				for (int i = 0; i < CConfig::GetSingletonPtr()->m_nUploadInteval; ++i)
				{
					Sleep(1000);
				}
				continue;
			}
		}  
        //�����ļ��ϴ�
        if (RECORD_STORAGETYPE_LOCAL_KMS == struRecord.recordStorageType || RECORD_STORAGETYPE_OBJECT_CLOUD == struRecord.recordStorageType)
        {
            //�жϱ���¼���ļ��Ƿ����
            if (!PathFileExists(fcA2T(struRecord.strFilePath.c_str())))
            {
                //CGuard guard(&m_csListRecords);
                //m_listRecords.pop_front();
				uploadFailManage(recordInfo, "¼���ļ�������", nFailTime);
				ULSERVER_LOG_ERROR("the file is not exist.filename:%s,filepath:%s", 
					struRecord.strRecordName.c_str(),struRecord.strFilePath.c_str());
				Msger_NotifyMsg(ALL_USER, _T("¼���ļ�������, �ļ���:%s,�ļ�·��:%s"), 
                    CSCT::Ansi2Unicode(struRecord.strRecordName).c_str(),
					CSCT::Ansi2Unicode(struRecord.strFilePath).c_str());
                continue;
            }
			ULSERVER_LOG_INFO("find the file.filename:%s,filepath:%s", struRecord.strRecordName.c_str(),struRecord.strFilePath.c_str());
        }
        //DVR¼����Ҫ����DVR��ip��ȡ�洢�豸  
        if (RECORD_STORAGETYPE_NORMAL == struRecord.recordStorageType)
        {
            //��ȡ�洢��Ϣ
            ::ResetEvent(m_hStorageEvent);
            if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetStorageInfo(struRecord.strDvrIp, struRecord.nDvrPort))
            {
                ULSERVER_LOG_ERROR("GetStorageInfo failed.");
            }
            if (WAIT_OBJECT_0 != WaitForSingleObject(m_hStorageEvent, 1000*10))
            {
                //��ȡ�洢��Ϣʧ�ܴ���
                uploadFailManage(recordInfo, "��ȡ�洢�豸��Ϣ��ʱ", nFailTime);
                ULSERVER_LOG_ERROR("DVR ip:%s, port:%d, get storage info failed.nFailTime:%d", 
                    struRecord.strDvrIp.c_str(), struRecord.nDvrPort, nFailTime);
                Msger_NotifyMsg(ALL_USER, _T("��ȡ�洢�豸��Ϣ��ʱ���ɼ��豸IP:%s, �˿�:%d"), 
                    CSCT::Ansi2Unicode(struRecord.strDvrIp).c_str(), struRecord.nDvrPort);
                continue;
            }
            if (FALSE == m_bStorageReturn)
            {
                //��ȡ�洢��Ϣʧ�ܴ���
                uploadFailManage(recordInfo, "��ȡ�洢�豸��Ϣʧ��", nFailTime);
                ULSERVER_LOG_ERROR("DVR ip:%s, port:%d, get storage info failed.nFailTime:%d", 
                    struRecord.strDvrIp.c_str(), struRecord.nDvrPort, nFailTime);
                Msger_NotifyMsg(ALL_USER, _T("��ȡ�洢�豸��Ϣʧ�ܣ��ɼ��豸IP:%s, �˿�:%d"), 
                    CSCT::Ansi2Unicode(struRecord.strDvrIp).c_str(), struRecord.nDvrPort);
                continue;
            }
            struRecord.struStorage = m_struStorage;
        }
           
        //�洢����¼���
        LONG lLoginID = STORAGE_LOGIN_FAILED;
        lLoginID = CStorageMgr::GetSingletonPtr()->LoginManage(struRecord.struStorage);
        if (STORAGE_LOGIN_FAILED == lLoginID)
        {
            uploadFailManage(recordInfo, "��¼�洢�豸ʧ��", nFailTime);              
            ULSERVER_LOG_ERROR("Storage ip:%s, port:%d, login failed.nFailTime:%d", 
                struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, nFailTime);
            Msger_NotifyMsg(ALL_USER, _T("��¼�洢�豸ʧ��,�洢��IP:%s, �˿�:%d"), 
                CSCT::Ansi2Unicode(struRecord.struStorage.strIp).c_str(), struRecord.struStorage.nPort);
            continue;
        }
        //���ϴ�¼��ǰ�Ȼ�ȡ�ļ����ϴ�״̬������ܻ�ȡ����Ϊ���ϴ�¼������
        if ((RECORDSTAT_UPLOADING == struRecord.recordStatus) || (CStorageMgr::GetSingletonPtr()->GetUploadStatus(lLoginID, struRecord)))
        {
            //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
            //�����ϴ�״̬MAP      
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
            Msger_NotifyMsg(ALL_USER, _T("��¼�������ϴ�,¼������:%s"), 
                CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
			//����ʧ�ܴ���
			nFailTime = 0;
            continue;
        }
        //¼���ϴ�
        if (!CStorageMgr::GetSingletonPtr()->Upload(lLoginID, struRecord))
        {
            //�ϴ�ʧ�ܵǳ��豸
            CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
            uploadFailManage(recordInfo, "�ϴ�ʧ��(��ʾ:����洢�豸����)", nFailTime);   
            ULSERVER_LOG_ERROR("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], upload failed.nFailTime:%d", 
                struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
                struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
                struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
                struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
                struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond,
                nFailTime);
            Msger_NotifyMsg(ALL_USER, _T("�����ϴ�����ʧ��,¼������:%s"),
                CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
            continue;
        } 
        ULSERVER_LOG_INFO("record name:%s, record id:%d, camera id:%s, recStartTime[%d-%d-%d %d:%d:%d], recStopTime[%d-%d-%d %d:%d:%d], set upload task succeed.", 
            struRecord.strRecordName.c_str(), recordInfo.id(), struRecord.strCameraID.c_str(), 
            struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay,
            struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
            struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay,
            struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
        Msger_NotifyMsg(ALL_USER, _T("�����ϴ�����ɹ�, ¼������:%s"),
            CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
        //KMS����ȡ�ϴ�״̬���������ֱ������Ϊ�ϴ��ɹ�
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
				//����ϴ�����ʧ�ܴ���
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
				//����ϴ�����ʧ�ܴ���
				nFailTime = 0;
				continue;
			}  
		}
        //�����ϴ�״̬MAP  
        else
        {
            //��¼���ϴ�״̬��Ϊ�ϴ���
            CDBDataMgr::GetSingletonPtr()->SetRecordStatus(recordInfo.id(), RECORDSTAT_UPLOADING, struRecord.nTaskID, 0);
            CGuard guard(&m_csMapRecordStatus);
            m_mapRecordStatus[recordInfo.id()] = struRecord;
        }
        {
            CGuard guard(&m_csListRecords);
            m_listRecords.pop_front();
        }
        
        //�ϴ���ɵǳ��豸
        //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
        //����ϴ�����ʧ�ܴ���
        nFailTime = 0;
        //�ϴ�����ʱ����m_nUploadInteval*1000��
        for (int i = 0; i < CConfig::GetSingletonPtr()->m_nUploadInteval; ++i)
        {
            Sleep(1000);
        }
    }
    ULSERVER_LOG_INFO("RunCycleUpload........OUT");
}

/**	@fn	    RunCycleGetStatus[�̺߳���]
*	@brief	��ȡ״̬
*	@param  [in] ��
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
            BOOL bDelRecord = FALSE;    //�Ƿ��MAP��ɾ��¼���¼
            int nRecordID = iter->first;
            Struct_RecordInfo struRecord = iter->second;
            LONG lLoginID = STORAGE_LOGIN_FAILED;
            lLoginID = CStorageMgr::GetSingletonPtr()->LoginManage(struRecord.struStorage);
            if (STORAGE_LOGIN_FAILED == lLoginID)
            {
                //ɾ��map�е�¼����Ϣ
                m_mapRecordStatus.erase(iter++);
                ULSERVER_LOG_ERROR("Login Storage failed.ip:%s, port:%d", 
                    struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort);
                continue;
            }
            //��ȡ¼��״̬
            if (!CStorageMgr::GetSingletonPtr()->GetUploadStatus(lLoginID, struRecord))
            {
                CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
                //ɾ��map�е�¼����Ϣ
                m_mapRecordStatus.erase(iter++);
                ULSERVER_LOG_ERROR("GetUploadStatus failed.record id:%d", nRecordID);
                Msger_NotifyMsg(ALL_USER, _T("��ȡ¼��״̬ʧ��, ¼������:%s"), 
                    CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
                continue;
            }
            //CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, FALSE);
            //�ϴ����¼����
            if (RECORDSTAT_OVER == struRecord.recordStatus)
            {
				Struct_UploadRecordParam struRecordParam;
				//��ȡ���ϴ�¼����Ϣ
				if (!CStorageMgr::GetSingletonPtr()->GetUploadRecordInfo(lLoginID, struRecord, struRecordParam))
				{
					CStorageMgr::GetSingletonPtr()->LogoutManage(struRecord.struStorage, TRUE);
					//ɾ��map�е�¼����Ϣ
					m_mapRecordStatus.erase(iter++);
					ULSERVER_LOG_ERROR("GetUploadRecordInfo failed.record id:%d", nRecordID);
					Msger_NotifyMsg(ALL_USER, _T("��ȡ���ϴ�¼����Ϣʧ��, ¼������:%s"), 
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
					strErrorMsg = "�洢�豸��¼��";
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
							_T("%s�ϴ�ʧ��"),
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
						Msger_NotifyMsg(ALL_USER,_T("�ϴ����, ¼������:%s"), 
							CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
					}
                }              
                //ɾ��map�е�¼����Ϣ
                m_mapRecordStatus.erase(iter++);  
            }
            //�ϴ���¼����
            else if (RECORDSTAT_UPLOADING == struRecord.recordStatus)
            {
                if (HPR_OK != CDBDataMgr::GetSingletonPtr()->SetRecordStatus(nRecordID, RECORDSTAT_UPLOADING, struRecord.nTaskID, struRecord.nUploadPercent))
                {
                    ULSERVER_LOG_ERROR("SetRecordStatus failed.record id:%d, status:uploading", nRecordID);
                }
                ++iter;
            }
            //�ϴ�ʧ��¼����
            else if (RECORDSTAT_FAIL == struRecord.recordStatus)
            {
				ULSERVER_LOG_ERROR("Upload fail.record id:%d, record name:%s, camera id:%s, startTime:%d-%d-%d %d:%d:%d, stopTime:%d-%d-%d %d:%d:%d",
					nRecordID, struRecord.strRecordName.c_str(), struRecord.strCameraID.c_str(),
					struRecord.struRecStartTime.dwYear, struRecord.struRecStartTime.dwMonth, struRecord.struRecStartTime.dwDay, 
					struRecord.struRecStartTime.dwHour, struRecord.struRecStartTime.dwMinute, struRecord.struRecStartTime.dwSecond, 
					struRecord.struRecStopTime.dwYear, struRecord.struRecStopTime.dwMonth, struRecord.struRecStopTime.dwDay, 
					struRecord.struRecStopTime.dwHour, struRecord.struRecStopTime.dwMinute, struRecord.struRecStopTime.dwSecond);
                std::string strErrorMsg;
                strErrorMsg = "ִ�лش�����ʧ��";
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
                        _T("%s�ϴ�ʧ��"),
                        CSCT::Ansi2Unicode(struRecord.strRecordName).c_str());
                }
                //ɾ��map�е�¼����Ϣ
                m_mapRecordStatus.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
        //��ȡ״̬ʱ����m_nUploadInteval*1000��
        for (int i = 0; i < CConfig::GetSingletonPtr()->m_nStatusInteval; ++i)
        {
            Sleep(1000);
        }
    }
    ULSERVER_LOG_INFO("RunCycleGetStatus........OUT");
}

/**	@fn	    RunCycleGetRecords[�̺߳���]
*	@brief	��ȡ¼��
*	@param  [in] ��
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
            //δ������ʱ����߹���ʱ��ת�����󣬼��1s������ִ��
            ULSERVER_LOG_ERROR("It is not work time or convert time errro.nRes:%d", nRes);
            Sleep(1000);
            continue;
        }
        
        time_t currentTime = time(NULL);
        if (lastGetDataTime < currentTime)
        {            
            CDBDataMgr::GetSingletonPtr()->GetRecordByStatus();
            Msger_NotifyMsg(ALL_USER, _T("�����ȡ¼����Ϣ"));
            ULSERVER_LOG_INFO("request to get record");
            lastGetDataTime = CConfig::GetSingletonPtr()->m_nUpdateDataTime + time(NULL);
        }
        Sleep(100);
        
    }
    ULSERVER_LOG_INFO("RunCycleGetRecords........OUT");
}

/**	@fn	    HandleStorage
*	@brief	����洢�豸��Ϣ
*	@param  [in] storage -- �洢�豸��Ϣ
*   @param  [in] bFinish -- �Ƿ����
*	@return	
*/
void CUploadFactory::HandleStorage(const cms_8120::RspStorageConfig& storage, BOOL bFinish /*= true*/)
{
    m_bStorageReturn = TRUE;
    //DC����ʧ�ܽ��������ֱ�ӷ���
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
    case cms_8120::DEV_TYPE_CLOUD_STORAGE:  //�ƴ洢
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
*	@brief	�ж��Ƿ��ڹ���ʱ����
*	@param  [in] strStartTime -- ��ʼʱ��
*   @param  [in] strStopTime -- ����ʱ��
*	@return	int, -1��ʾ����0��ʾ���ڹ���ʱ���ڣ�1��ʾ�ڹ���ʱ����
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
            _T("����Ƿ��ǹ���ʱ��ʱת��ʱ��ʧ��"));
        return WORKTIME_ERROR;
    }

    COleDateTime nowTime = COleDateTime::GetCurrentTime();
    startTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        startTime.GetHour(), startTime.GetMinute(), startTime.GetSecond());
    stopTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        stopTime.GetHour(), stopTime.GetMinute(), stopTime.GetSecond());

    // �϶�ʱ��һ��Ϊȫ�칤��
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
*	@brief	ת��ʱ�����ͣ�string-COleDateTime��
*	@param  [in] strStartTime -- ��ʼʱ��
*   @param  [in] strStopTime -- ����ʱ��
*	@param  [out] startTime -- ��ʼʱ��
*   @param  [out] stopTime -- ����ʱ��
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
*	@brief	����ʱ����Ϣ
*	@param  [in] nTime -- ʱ���
*	@param  [out] struTime -- ʱ����Ϣ
*	@return	BOOL
*/
BOOL CUploadFactory::setRecordTime(const std::string &strTime, Struct_RecordTime &struTime)
{
    sscanf(strTime.c_str(), "%d-%02d-%02d %02d:%02d:%02d", &struTime.dwYear, &struTime.dwMonth, &struTime.dwDay,
        &struTime.dwHour, &struTime.dwMinute, &struTime.dwSecond);

    return TRUE;
}

/**	@fn	    ConvertRecordInfo
*	@brief	ת��¼����Ϣ
*	@param  [in] recordInfo -- ԭ¼����Ϣ
*   @param  [out] struRecord -- ת�����¼����Ϣ
*	@return	BOOL
*/
BOOL CUploadFactory::convertRecordInfo(const cms_8120::Records &recordInfo, Struct_RecordInfo &struRecord)
{
    struRecord.recordStatus = (ENUM_RECORD_STATUS)recordInfo.nrecordstatus();
    /*struRecord.nUserID = recordInfo.nuserid();*/
    struRecord.recordStorageType = (ENUM_RECORD_STORAGE_TYPE)recordInfo.nrecordfiletype();
    struRecord.strRecordName = CSCT::Utf82Ansi(recordInfo.strrecordname());
    //DVR¼���ļ����òɼ�DVR�豸��Ϣ
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
        //¼��ʼʱ��
        setRecordTime(recordInfo.starttime(), struRecord.struRecStartTime);
        //¼�����ʱ��
        setRecordTime(recordInfo.endtime(), struRecord.struRecStopTime);
        struRecord.nTaskID = recordInfo.ntaskid();
    }
    //����¼���ļ�����KMS�洢�豸��Ϣ
    else if (RECORD_STORAGETYPE_LOCAL_KMS == struRecord.recordStorageType)
    {
        struRecord.strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());  //�����ļ�·��
        struRecord.struStorage.storageType = STORAGE_TYPE_KMS;          //�洢�豸����
        struRecord.struStorage.strIp = CSCT::Utf82Ansi(recordInfo.strcollectip());  //�洢�豸IP
        struRecord.struStorage.nPort = recordInfo.dvrport();                        //�洢�豸�˿�
        struRecord.struStorage.strUser = CSCT::Utf82Ansi(recordInfo.dvrusername()); //�洢�豸�û���
        std::string strPwd = CSCT::Utf82Ansi(recordInfo.dvrpwd());                  //�洢�豸����
        CConfig::GetSingletonPtr()->Deciphering(strPwd);
        struRecord.struStorage.strPwd = strPwd; 
    }
	//����¼���ļ������ƴ洢��Ϣ
	else if (RECORD_STORAGETYPE_OBJECT_CLOUD == struRecord.recordStorageType)
	{
		struRecord.nID = recordInfo.id();                                                  //���ݿ��¼ID������Key
		struRecord.strFilePath = CSCT::Utf82Ansi(recordInfo.strurl());                     //�����ļ�·��
		struRecord.strHost = CSCT::Utf82Ansi(recordInfo.strip());                          //�����ļ���������IP��ַ,����Key
		struRecord.struStorage.strIp =  CSCT::Utf82Ansi(recordInfo.strcloudip());          //�洢�豸IP
		struRecord.struStorage.nPort = recordInfo.ncloudport();                            //�洢�豸�˿�
		std::string strAccessKey = CSCT::Utf82Ansi(recordInfo.straccesskey());             //����洢AccessKey
		CConfig::GetSingletonPtr()->Deciphering(strAccessKey);
		struRecord.struStorage.strAccessKey = strAccessKey;
		std::string strSecretKey = CSCT::Utf82Ansi(recordInfo.strsecretkey());             //����洢SecretKey
		CConfig::GetSingletonPtr()->Deciphering(strSecretKey);
		struRecord.struStorage.strSecretKey = strSecretKey;
		struRecord.strBucket = CSCT::Utf82Ansi(recordInfo.strbucket());                    //����洢Bucket
		struRecord.fileType = FILE_TYPE_VIDEO;                                             //�ļ����ͣ���������洢�ϴ���ʽ
		struRecord.struStorage.storageType = STORAGE_TYPE_OBJECT_CLOUD;                    //�洢�豸����	                     
		struRecord.strStartTime =  CSCT::Utf82Ansi(recordInfo.starttime());                //�����ļ���ʼʱ��,����Key
		struRecord.strEndTime =    CSCT::Utf82Ansi(recordInfo.endtime());                  //�����ļ�����ʱ��,����Key
		//¼��ʼʱ��
		setRecordTime(recordInfo.starttime(), struRecord.struRecStartTime);
		//¼�����ʱ��
		setRecordTime(recordInfo.endtime(), struRecord.struRecStopTime);
	}
	else
	{
		ULSERVER_LOG_ERROR("unknown struRecord.recordStorageType");
	}
    
    return TRUE;
}

/**	@fn	    uploadFailManage
*	@brief	�ϴ�ʧ�ܴ���
*   @param  [in] recordInfo -- ¼����Ϣ
*   @param  [out] nFailTimes -- �ϴ�ʧ�ܴ���
*	@return	void
*/
void CUploadFactory::uploadFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes)
{
    //ʧ�ܴ���С��MAX_FAIL_TIME������list�����ϴ�
    ++nFailTimes;
    ULSERVER_LOG_ERROR("upload fail.nFailTime:%d", nFailTimes);
    //ʧ�ܴ�������MAX_FAIL_TIME������¼��״̬Ϊʧ��
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
            _T("%s�ϴ�ʧ��"),
            CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
        //�ϴ�ʧ������ʧ�ܴ���
        nFailTimes = 0;
        CGuard guard(&m_csListRecords);
        m_listRecords.pop_front();
    }
}

/**	@fn	    lockFailManage
*	@brief	����ʧ�ܴ���
*   @param  [in] recordInfo -- ¼����Ϣ
*   @param  [out] nFailTimes -- ����ʧ�ܴ���
*	@return	void
*/
void CUploadFactory::lockFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes)
{
	//ʧ�ܴ���С��MAX_FAIL_TIME������list��������
	++nFailTimes;
	ULSERVER_LOG_ERROR("lock fail.nFailTime:%d", nFailTimes);
	//ʧ�ܴ�������MAX_FAIL_TIME������¼��״̬Ϊʧ��
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
			_T("%s����ʧ��"),
			CSCT::Utf82Unicode(recordInfo.strrecordname()).c_str());
		//����ʧ������ʧ�ܴ���
		nFailTimes = 0;
		CGuard guard(&m_csListRecords);
		m_listRecords.pop_front();
	}
}

/**	@fn	    uploadOver
*	@brief	�ϴ���ɴ���
*	@param  [in] struRecordInfo -- ¼����Ϣ
*	@param  [in] strTip-- ��ʾ��Ϣ
*	@return	BOOL
*/
BOOL CUploadFactory::uploadOver(const int &nRecordID, const Struct_RecordInfo &struRecordInfo, std::string strTip)
{
    //KMS���¼���ϴ�����
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

/**	@fn	    RunCycleHeartBeat[�̺߳���]
*	@brief	����
*	@param  [in] ��
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
*	@brief	��¼Dvr
*	@param  [in] struStorage -- �豸��Ϣ
*	@return	LONG -- ��¼���
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
*	@brief	����DVR¼��
*	@param  [in] struRecordInfo -- ¼����Ϣ
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
	//��ʱ������
	//�����ļ�
	NET_DVR_FILECOND_V40 struFileCond={0};
	struFileCond.dwFileType = 0xFF;
	struFileCond.lChannel = struRecordInfo.nChannelID; //ͨ����
	struFileCond.dwIsLocked = 0xff; //�����ļ�
	struFileCond.dwUseCardNo = 0;

	struFileCond.struStartTime.dwYear = struRecordInfo.struRecStartTime.dwYear;
	struFileCond.struStartTime.dwMonth = struRecordInfo.struRecStartTime.dwMonth;
	struFileCond.struStartTime.dwDay = struRecordInfo.struRecStartTime.dwDay;
	struFileCond.struStartTime.dwHour = struRecordInfo.struRecStartTime.dwHour;
	struFileCond.struStartTime.dwMinute = struRecordInfo.struRecStartTime.dwMinute;
	struFileCond.struStartTime.dwSecond = struRecordInfo.struRecStartTime.dwSecond;
	//¼�����ʱ�� 
	struFileCond.struStopTime.dwYear = struRecordInfo.struRecStopTime.dwYear;
	struFileCond.struStopTime.dwMonth = struRecordInfo.struRecStopTime.dwMonth;
	struFileCond.struStopTime.dwDay = struRecordInfo.struRecStopTime.dwDay;
	struFileCond.struStopTime.dwHour = struRecordInfo.struRecStopTime.dwHour;
	struFileCond.struStopTime.dwMinute = struRecordInfo.struRecStopTime.dwMinute;
	struFileCond.struStopTime.dwSecond = struRecordInfo.struRecStopTime.dwSecond;

	//����¼���ļ� 
	int lFindHandle = NET_DVR_FindFile_V40(lLoginID, &struFileCond);
	if (lFindHandle < 0)
	{
		DWORD ulErrorCode = NET_DVR_GetLastError();
		ULSERVER_LOG_ERROR("��ʱ������ļ�ʧ��(%d)%s_%d_%d_%s",
			ulErrorCode,
			struRecordInfo.strDvrIp.c_str(),
			struRecordInfo.nDvrPort,
			struRecordInfo.nChannelID,
			struRecordInfo.strRecordName.c_str());
		Msger_NotifyMsg( ALL_USER, _T("��ʱ������ļ�ʧ��(%d)%s_%d_%d_%s"),
			ulErrorCode,
			CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
			struRecordInfo.nDvrPort,
			struRecordInfo.nChannelID,
			CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
		//ע���û�
		NET_DVR_Logout(lLoginID);
		return FALSE;
	}
	ULSERVER_LOG_INFO("��ʱ������ļ���ʼ��%s_%d_%d_%s",
		struRecordInfo.strDvrIp.c_str(),
		struRecordInfo.nDvrPort,
		struRecordInfo.nChannelID,
		struRecordInfo.strRecordName.c_str());
	NET_DVR_FINDDATA_V40 struFileData = {0};
	time_t beginFind = time(NULL);
	while(true)
	{
		//�����ȡ���ҵ����ļ���Ϣ
		int iResult = NET_DVR_FindNextFile_V40(lFindHandle, &struFileData);
		if (NET_DVR_ISFINDING == iResult)
		{
			// �����п��ܻ�����ѭ�� �Ӹ���ʱ�жϡ�
			//�������ʱ�����30�� �ж�Ϊ��ʱ��
			if (abs(difftime(beginFind, time(NULL))) > 30)
			{
				ULSERVER_LOG_ERROR("��ʱ������ļ���ʱ��%s_%d_%d_%s",
					struRecordInfo.strDvrIp.c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					struRecordInfo.strRecordName.c_str());
				Msger_NotifyMsg( ALL_USER, _T("��ʱ������ļ���ʱ��%s_%d_%d_%s"),
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
		else if (NET_DVR_FILE_SUCCESS == iResult) //��ȡ�ļ���Ϣ�ɹ�
		{
			beginFind = time(NULL);
			ULSERVER_LOG_INFO("���ҵ��ļ�,%d-%d-%d %d:%d:%d_%d-%d-%d %d:%d:%d",
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
			//ȥ��ֻ���漸����ļ���
			time_t iFileBeginTime = 0;
			time_t iFileEndTime = 0;
			time_t iRecBeginTime = 0;
			time_t iRecEndTime = 0;
			//ȥ��ֻ���漸����ļ���
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
				ULSERVER_LOG_ERROR("DVR����¼��ʧ��(������%d)%s_%d_%d_%s", 
					ulErrorCode,
					struRecordInfo.strDvrIp.c_str(),
					struRecordInfo.nDvrPort,
					struRecordInfo.nChannelID,
					struRecordInfo.strRecordName.c_str());
				Msger_NotifyMsg(ALL_USER, _T("DVR����¼��ʧ��(������%d)%s_%d_%d_%s "),
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
			ULSERVER_LOG_ERROR("���ҵ�����ļ���%s_%d_%d_%s", 
				struRecordInfo.strDvrIp.c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				struRecordInfo.strRecordName.c_str());
			Msger_NotifyMsg(ALL_USER, _T("���ҵ�����ļ���%s_%d_%d_%s"),
				CSCT::Ansi2Unicode(struRecordInfo.strDvrIp).c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				CSCT::Ansi2Unicode(struRecordInfo.strRecordName).c_str());
			NET_DVR_FindClose_V30(lFindHandle);
			NET_DVR_Logout(lLoginID);
			for (int i = 0; i < 10 && !m_bStopWorkerThread; ++i) //¼���������������ܲ鲻��¼��һ������������¼�񣩣��ȴ�һ��ʱ���´�������
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
			ULSERVER_LOG_ERROR("�����ļ��쳣(������%d)%s_%d_%d_%s", 
				ulErrorCode,
				struRecordInfo.strDvrIp.c_str(),
				struRecordInfo.nDvrPort,
				struRecordInfo.nChannelID,
				struRecordInfo.strRecordName.c_str());
			Msger_NotifyMsg(ALL_USER, _T("�����ļ��쳣(������%d)%s_%d_%d_%s"),
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

	ULSERVER_LOG_INFO("��ʱ������ļ�������%s_%d_%d_%s",
		struRecordInfo.strDvrIp.c_str(),
		struRecordInfo.nDvrPort,
		struRecordInfo.nChannelID,
		struRecordInfo.strRecordName.c_str());
	NET_DVR_FindClose_V30(lFindHandle);
	NET_DVR_Logout(lLoginID);
	return TRUE;

}

/**	@fn	    getDvrTimeStamp
*	@brief	�õ�ʱ���
*	@param  [in]  stDvrTimeSt -- ʱ��
*	@param  [out] iTime -- ʱ���
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
*	@brief	�õ�¼��ʱ���
*	@param  [in]  stRecordTime -- ¼��ʱ��
*	@param  [out] iTime -- ʱ���
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
*	@brief	������ʾ
*	@param  [in] struRecordInfo -- ¼����Ϣ
*	@param  [in] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
*	@param  [out] strTip-- ��ʾ��Ϣ
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

	ULSERVER_LOG_INFO("���ϴ�¼��ʱ��:%s", szTime);

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
		ULSERVER_LOG_ERROR("�ϴ�¼������");
		strTip = szTime;
	}
	return TRUE;
}


