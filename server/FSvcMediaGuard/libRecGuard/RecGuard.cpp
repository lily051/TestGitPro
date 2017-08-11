#include "StdAfx.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "RecGuard.h"
#include <fclib/FCThreadHelper.h>
#include "CVR_Udd.h"
#include "Config.h"
#include <SvcGeneralDef.h>
#include "Cloud_Udd.h"
#include "DBDataMgr.h"
#include "lib/curl/curl.h"
#include "lib/jsoncpp/include/json.h"
#include "../util/StringConverter.h"
#include "hak.h"
#include "type/DevType.pb.h"
#include "ConnSessionMgr.h"
using namespace fclib::fcthread;

typedef unsigned int bn_handle_t;
//#define IP "10.64.60.80"
//#define Port 8008 
//#define username "web_admin"
//#define passwd   "123"

typedef int cloud_result_t;
typedef unsigned int cloud_handle_t;

#define CLOUD_RESULT_ERROR -1
#define CLOUD_RESULT_OK 0

#define OBJECT_KEY_NOT_EXIST   13017

using namespace std;

namespace {
// 管理线程的时间间隔，用来控制传输线程的开始与停止
DWORD const MANAGE_WORK_INTERVAL = 30000;
//// CVR磁盘满
//HPR_INT32 const CVR_NEARLLY_FULL = 60101; 
//// CVR登录失败
//HPR_INT32 const CVR_CANNOT_LOGIN = 60102;
//// CVR 某些操作失败
//HPR_INT32 const CVR_OPERATION_FAIL = 60103;
// void CALLBACK MsgError(unsigned int playhandle,DWORD user,int ErrorCode);
// void CALLBACK CB_GetGroupResult(int rvID,
//     const char *rvName,
//     int activeDevices,
//     int totalDevices,
//     unsigned int gSizeHigh,
//     unsigned int gSizeLow,
//     unsigned int UsedHigh,
//     unsigned int UsedLow,
//     int wdays,
//     int dPolicy,
//     NVR_DWORD pUserData);
// bn_handle_t LoginCVR(MsgCallback pPlayMsgCB,NVR_DWORD pUserData,  const cvr_info_t &cvrInfo);

//// Cloud磁盘满
//HPR_INT32 const CLOUD_NEARLLY_FULL = 60101; 
//// Cloud登录失败
//HPR_INT32 const CLOUD_CANNOT_LOGIN = 60102;
//// Cloud某些操作失败
//HPR_INT32 const CLOUD_OPERATION_FAIL = 60103;
//// 获取录像池超时
//HPR_INT32 const CLOUD_GET_POOLLIST_FAIL = 60104;

//HPR_INT32 const MAX_WAIT_POOLLIST_TIMES = 200; 

HPR_INT32 const MAX_DELETE_CLIENTS_ID = 150; 

//int PoolsListCb(CloudPoolInfoEx info, int Count, int Flat, void* pUser);

//int UserMsgError(int LogicType,int ErrorCode,void* pUser);

//cloud_result_t LoginCloud(CUserMessage pUserMsgCB, void* pUserData, const cloud_info_t &cloudInfo);

} //~ end of anonymous namespace

// 片段头
static size_t const SEG_HEAD_LEN = 1024;

CRecGuard::CRecGuard(void)
: m_hThreadManage(NULL)
, m_hThreadHeartBeat(NULL)
, m_hThreadCVRGuard(NULL)
, m_hThreadRecGuard(NULL)
, m_hRecordEvent(NULL)
, m_hOverRecordEvent(NULL)
, m_hStorageDevEvent(NULL)
, m_hWorkParamEvent(NULL)
, m_bStopWorker(false)
, m_bStopServer(false)
{
    ZeroMemory(&m_struStorage, sizeof(m_struStorage));
    ZeroMemory(&m_struWorkParam, sizeof(m_struWorkParam));
}

CRecGuard::~CRecGuard(void)
{
    try
    {
		std::map<std::string, HCSClient *>::iterator iterHCSClient;
		for (iterHCSClient = m_mapHCSClient.begin(); iterHCSClient != m_mapHCSClient.end(); ++iterHCSClient)
		{
			ClientFinalize(iterHCSClient->second);
			TPLOG_INFO("ClientFinalize  succ,IP(%s)", iterHCSClient->first.c_str());

		}
		m_mapHCSClient.clear();

        CDBDataMgr::GetSingletonPtr()->FreeSingleton();
        CConnSessionMgr::FreeSingleton();
    }
    catch (...)
    {
    	
    }
}

int CRecGuard::Init(void)
{
    //初始化会话层
    NET_DVR_Init();
    //云存储初始化
    int nRes = HCS_SDK_Init();
    if (HCS_OK != nRes)
    {
        TPLOG_ERROR("call HCS_SDK_Init fail.error[%d]",
            nRes);
        return FALSE; 
    }
    //初始化curl库
    curl_global_init(CURL_GLOBAL_ALL);
	m_mapHCSClient.clear();
    CConnSessionMgr::GetSingletonPtr()->Init();
    //初始化数据库交互层
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->Init())
    {
        TPLOG_ERROR("init CDBDataMgr failed");
        return HPR_ERROR;
    }
    return HPR_OK;
}
int CRecGuard::Fini(void)
{
    NET_DVR_Cleanup();
    HCS_SDK_Deinit();
    //反初始化curl库
    curl_global_cleanup();
	std::map<std::string, HCSClient *>::iterator iterHCSClient;
	for (iterHCSClient = m_mapHCSClient.begin(); iterHCSClient != m_mapHCSClient.end(); ++iterHCSClient)
	{
		ClientFinalize(iterHCSClient->second);
		TPLOG_INFO("ClientFinalize  succ,IP(%s)", iterHCSClient->first.c_str());
	}
	m_mapHCSClient.clear();
    //会话层
    CConnSessionMgr::GetSingletonPtr()->Fini();
    CDBDataMgr::GetSingletonPtr()->Fini();
    return HPR_OK;
}
int CRecGuard::Start(void)
{
	m_mapHCSClient.clear();
    m_bStopServer = false;
    m_bStopWorker = false;
    m_hWorkParamEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hRecordEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hOverRecordEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hStorageDevEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    //会话层开始
    CConnSessionMgr::GetSingletonPtr()->Start();
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->Start())
    {
        TPLOG_ERROR("CDBDataMgr Start Fail!!!!");
        return -1;
    } 
    if (NULL == m_hThreadManage)
    {
        m_hThreadManage = fcFancyCreateThread(WT_Manage, this);
        if (NULL == m_hThreadManage)
        {
            TPLOG_ERROR("create manage thread failed:%d", ::GetLastError());
			Msger_NotifyMsg(ALL_USER, _T("创建管理线程失败，错误码:%d"), ::GetLastError());
            return -1;
        }
    }
    if (NULL == m_hThreadHeartBeat)
    {
        m_hThreadHeartBeat = fcFancyCreateThread(WT_HeartBeat, this);
        if (NULL == m_hThreadHeartBeat)
        {
            TPLOG_ERROR("create HeartBeat thread failed:%d", ::GetLastError());
			Msger_NotifyMsg(ALL_USER, _T("创建心跳线程失败，错误码:%d"), ::GetLastError());
            m_bStopServer = true;
            m_bStopWorker = true;
            if (NULL != m_hThreadCVRGuard)
            {
                fcWaitAndCloseThread(m_hThreadCVRGuard);
                m_hThreadCVRGuard = NULL;
            }
            if (NULL != m_hThreadRecGuard)
            {
                fcWaitAndCloseThread(m_hThreadRecGuard);
                m_hThreadRecGuard = NULL;
            }
            if (NULL != m_hThreadManage)
            {
                fcWaitAndCloseThread(m_hThreadManage);
                m_hThreadManage = NULL;
            }
            return -1;
        }
    }
    return NO_ERROR;
}
int CRecGuard::Stop(void)
{
    m_bStopServer = true;
    m_bStopWorker = true;

    //数据库交互层停止
    CDBDataMgr::GetSingletonPtr()->Stop();
    //会话层停止
    CConnSessionMgr::GetSingletonPtr()->Stop();
    if (NULL != m_hThreadManage)
    {
        fcWaitAndCloseThread(m_hThreadManage);
        m_hThreadManage = NULL;
    }
    if (NULL != m_hThreadCVRGuard)
    {
        fcWaitAndCloseThread(m_hThreadCVRGuard);
        m_hThreadCVRGuard = NULL;
    }
    if (NULL != m_hThreadRecGuard)
    {
        fcWaitAndCloseThread(m_hThreadRecGuard);
        m_hThreadRecGuard = NULL;
    }
    if (NULL != m_hThreadHeartBeat)
    {
        fcWaitAndCloseThread(m_hThreadHeartBeat);
        m_hThreadHeartBeat = NULL;
    }	    
    if (NULL != m_hRecordEvent)
    {
        CloseHandle(m_hRecordEvent);
        m_hRecordEvent = NULL;
    }
    if (NULL != m_hOverRecordEvent)
    {
        CloseHandle(m_hOverRecordEvent);
        m_hOverRecordEvent = NULL;
    }
    if (NULL != m_hStorageDevEvent)
    {
        CloseHandle(m_hStorageDevEvent);
        m_hStorageDevEvent = NULL;
    }
    if (NULL != m_hWorkParamEvent)
    {
        CloseHandle(m_hWorkParamEvent);
        m_hWorkParamEvent = NULL;
    }
	std::map<std::string, HCSClient *>::iterator iterHCSClient;
	for (iterHCSClient = m_mapHCSClient.begin(); iterHCSClient != m_mapHCSClient.end(); ++iterHCSClient)
	{
		ClientFinalize(iterHCSClient->second);
		TPLOG_INFO("ClientFinalize  succ,IP(%s)", iterHCSClient->first.c_str());
	}
	m_mapHCSClient.clear();
    return 0;
}

DWORD WINAPI CRecGuard::WT_Manage(LPVOID lpParam)
{
    CRecGuard *pInstance = reinterpret_cast<CRecGuard*>(lpParam);
    return pInstance->ManageProc();
}

DWORD CRecGuard::ManageProc(void)
{
    do
    {
        if (IsWorkingTime()
            && (NULL == m_hThreadRecGuard))
        {
            StartWorker();
        }
        else if(!IsWorkingTime()
            && (NULL != m_hThreadRecGuard))
        {
            StopWorker();
        }

    } while (DelayBySleep(MANAGE_WORK_INTERVAL, m_bStopServer), !m_bStopServer);

    return 0;
}

DWORD WINAPI CRecGuard::WT_HeartBeat(LPVOID lpParam)
{
    CRecGuard *pInstance = reinterpret_cast<CRecGuard*>(lpParam);
    return pInstance->HeartBeatProc();
}

DWORD CRecGuard::HeartBeatProc(void)
{
    do
    {
        CDBDataMgr::GetSingletonPtr()->SendHeartBeat();
    } while (DelayBySleep(MANAGE_WORK_INTERVAL, m_bStopServer), !m_bStopServer);
    return 0;
}

// DWORD WINAPI CRecGuard::WT_CVRGuard(LPVOID lpParam)
// {
//     CRecGuard *pInstance = reinterpret_cast<CRecGuard*>(lpParam);
//     return pInstance->CVRGuardProc();
// }
// 主要监测cvr的容量和在线状态
// DWORD CRecGuard::CVRGuardProc(void)
// {
//     // 10 分钟来一次
//     DWORD const CVR_WORK_INTERVAL = 10 * 60 * 1000;
//     do 
//     {
// 		work_param_t workParam;
// 		if (!GetSysConfig(workParam))
// 		{
// 			continue;
// 		}
// 
// 		if (CENTER_STORAGE_TYPE_CVR == workParam.storageType)
// 		{
// 			vector<cvr_info_t> cvrInfos;
// 			if (!GetAllCVR(cvrInfos))
// 			{
// 				continue;
// 			}
// 
// 			for (vector<cvr_info_t>::size_type i = 0;
// 				i < cvrInfos.size(); ++i)
// 			{
// 				CheckCVR(cvrInfos[i]);
// 			}
// 		}
// 		else if (CENTER_STORAGE_TYPE_CLOUD == workParam.storageType)
// 		{
// 			vector<cloud_info_t> cloudInfos;
// 			if (!GetAllClouds(cloudInfos))
// 			{
// 				continue;
// 			}
// 
// 			for (vector<cloud_info_t>::size_type i = 0;
// 				i < cloudInfos.size(); ++i)
// 			{
// 				CheckCloud(cloudInfos[i]);
// 			}
// 		}
// 
//     } while (DelayBySleep(CVR_WORK_INTERVAL, m_bStopWorker), !m_bStopWorker);
// 
//     return 0;
// }

DWORD WINAPI CRecGuard::WT_RecGuard(LPVOID lpParam)
{
    CRecGuard *pInstance = reinterpret_cast<CRecGuard*>(lpParam);
    return pInstance->RecGuardProc();
}
DWORD CRecGuard::RecGuardProc(void)
{
    //获取任务间隔为m_nTaskInteval秒
    DWORD const DEL_WORK_INTERVAL = CConfig::GetSingletonPtr()->m_nTaskInteval * 1000;
    DWORD delInterval = DEL_WORK_INTERVAL;
    do 
    {
        delInterval = DEL_WORK_INTERVAL;
        vector<bz_record_t> bzRecords;
        if (!GetOldBzRecord(bzRecords))
        {
            delInterval = 10 * 1000;
            continue;
        }

        if (!bzRecords.empty())
        {
            // 如果有要删除的录像，就间隔10秒钟
            delInterval = 10 * 1000;
        }

        for (vector<bz_record_t>::size_type i = 0;
            i < bzRecords.size(); ++i)
        {
            bz_record_t &bzRecord = bzRecords[i];
            if (ProcessOldBzRecord(bzRecord))
            {
                Msger_NotifyMsg(ALL_USER, _T("单号：%s 时间：%s 关联的所有录像已被删除"),
                    bzRecord.bzID.c_str(),
                    bzRecord.bzTime.c_str());
                //标记过期记录
                MarkOldBzRecords(bzRecord.recordID);
            }
        }
//         if (!bzRecordsToDel.empty())
//         {
//             DelOldBzRecordByIDs(bzRecordsToDel);
//         }

        //MarkOldBzRecords();

    } while (DelayBySleep(delInterval, m_bStopWorker), !m_bStopWorker);

    return 0;
}

bool CRecGuard::StartWorker(void)
{
    m_bStopWorker = false;

    //存储设备容量检测转移到系统巡检服务
//     if (NULL == m_hThreadCVRGuard)
//     {
//         m_hThreadCVRGuard = fcFancyCreateThread(WT_CVRGuard, this);
//         if (NULL == m_hThreadCVRGuard)
//         {
//             const DWORD dwError = ::GetLastError();
//             TPLOG_ERROR("create cvr guard thread failed:%d", dwError);
//             Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//                 ALL_USER,
//                 ALL_CHANNEL,
//                 ALARM_OCCUR,
//                 1,
//                 _T("创建存储守卫线程失败，错误码:%d"), dwError);
//             return false;
//         }
//     }
//     else
//     {
//         TPLOG_WARN("cvr guard thread already run");
//     }

    if (NULL == m_hThreadRecGuard)
    {
        m_hThreadRecGuard = fcFancyCreateThread(WT_RecGuard, this);
        if (NULL == m_hThreadRecGuard)
        {
            const DWORD dwError = ::GetLastError();
            TPLOG_ERROR("create rec guard thread failed:%d", dwError);

			Msger_NotifyMsg(ALL_USER, _T("创建录像守卫线程失败，错误码:%d"), dwError);
            return false;
        }
    }
    else
    {
        TPLOG_WARN("rec guard thread already run");
    }

    return true;
}
bool CRecGuard::StopWorker(void)
{
    m_bStopWorker = true;
    if (NULL != m_hThreadCVRGuard)
    {
        fcWaitAndCloseThread(m_hThreadCVRGuard);
    }
    if (NULL != m_hThreadRecGuard)
    {
        fcWaitAndCloseThread(m_hThreadRecGuard);
    }

    return true;
}

void CRecGuard::DelayBySleep(DWORD dwMilliseconds,
    volatile bool &stopFlag,
    DWORD dwInterval/* = 500*/)
{
    if (0 == dwInterval)
    {
        dwInterval = 500;
    }
    DWORD const nTimes = dwMilliseconds / dwInterval;
    for (DWORD i = 0; !stopFlag && i < nTimes; ++i)
    {
        Sleep(dwInterval);
    }
}

bool CRecGuard::GetOldBzRecord(std::vector<bz_record_t> &bzRecords)
{
    ::ResetEvent(m_hOverRecordEvent);
    m_vecBzRecord.clear();
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetOldBzRecord())
    {
        TPLOG_ERROR("failed to get old record info");
//         Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//             ALL_USER,
//             ALL_CHANNEL,
//             ALARM_OCCUR,
//             1,
//             _T("获取过期记录信息失败"));
        return false;
    }
    if (WAIT_OBJECT_0 != WaitForSingleObject(m_hOverRecordEvent, 1000*10))
    {
        TPLOG_ERROR("WaitForSingleObject overtime."); 
//         Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//             ALL_USER,
//             ALL_CHANNEL,
//             ALARM_OCCUR,
//             1,
//             _T("获取过期记录信息失败"));
        return false;
    }
    bzRecords = m_vecBzRecord;

    return true;
}
bool CRecGuard::GetRecRecord(
    const HPR_INT32 recordID,
    std::vector<rec_record_t> &recRecords)
{
    ::ResetEvent(m_hRecordEvent);
    m_vecRecRecord.clear();
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetRecRecord(recordID))
    {
        TPLOG_ERROR("failed to get rec record info");
//         Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//             ALL_USER,
//             ALL_CHANNEL,
//             ALARM_OCCUR,
//             1,
//             _T("获取记录信息失败"));
        return false;
    }
    if (WAIT_OBJECT_0 != WaitForSingleObject(m_hRecordEvent, 1000*10))
    {
        TPLOG_ERROR("failed to get rec record info");
//         Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//             ALL_USER,
//             ALL_CHANNEL,
//             ALARM_OCCUR,
//             1,
//             _T("获取记录信息失败"));
        return false;
    }
    recRecords = m_vecRecRecord;
    return true;
}
bool CRecGuard::ProcessOldBzRecord(const bz_record_t &bzRecord)
{
    vector<rec_record_t> recRecords;
    if (!GetRecRecord(bzRecord.recordID, recRecords))
    {
        TPLOG_ERROR("failed to get recRecord for %d", bzRecord.recordID);
        return false;
    }

    bool bRet = true;
    for (vector<rec_record_t>::size_type i = 0;
        i < recRecords.size(); ++i)
    {
        rec_record_t &recRecord = recRecords[i];
        if (ProcessRecRecord(recRecord))
        {
            Msger_NotifyMsg(ALL_USER, _T("cloud：%d 删除录像 %s_%s_%s"),
                recRecord.cvrID,
                recRecord.cameraID.c_str(),
                recRecord.startTime.c_str(),
                recRecord.stopTime.c_str());
			TPLOG_INFO("cloud：%d 删除录像 %s_%s_%s", 
				recRecord.cvrID,
				(LPCSTR)fcT2A(recRecord.cameraID.c_str()),
				(LPCSTR)fcT2A(recRecord.startTime.c_str()),
				(LPCSTR)fcT2A(recRecord.stopTime.c_str()));
            if (!DelRecRecordByIDs(recRecord.ID))
            {
                bRet = false;
            }           
        }
        else
        {
            Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
                ALL_USER,
                recRecord.recordID,
                ALARM_OCCUR,
                1,
                _T("storage：%d 删除录像 %s_%s_%s 失败"),
                recRecord.cvrID,
                recRecord.cameraID.c_str(),
                recRecord.startTime.c_str(),
                recRecord.stopTime.c_str());
			TPLOG_ERROR("storage：%d 删除录像 %s_%s_%s 失败", 
				recRecord.cvrID,
				(LPCSTR)fcT2A(recRecord.cameraID.c_str()),
				(LPCSTR)fcT2A(recRecord.startTime.c_str()),
				(LPCSTR)fcT2A(recRecord.stopTime.c_str()));
            bRet = false;
        }
    }

    return bRet;
}
bool CRecGuard::ProcessRecRecord(const rec_record_t &recRecord)
{
// 	work_param_t workParam;
//     if (!GetSysConfig(workParam))
// 	{
// 		return false;
// 	}

    storage_info_t struStorageInfo;
    //如果该录像未上传，删除过期的录像记录
    if (!recRecord.bUpload)
    {
        TPLOG_INFO("record %s not upload",
            (LPCSTR)fcT2A(recRecord.cameraID.c_str()));
        return true;
    }
	if (0 == CConfig::GetSingletonPtr()->m_nHaveCenterStorage) //无中心存储模式
	{
		if (!GetStorageInfo(recRecord.devId, struStorageInfo))
		{
			TPLOG_ERROR("get dvr info failed by %d", recRecord.devId);
			return false;
		}
	}
    else if (!GetStorageInfo(recRecord.cvrID, struStorageInfo))
    {
        TPLOG_ERROR("get storage info failed by %d", recRecord.cvrID);
        return false;
    }
    //CVR设备
    if (cms_8120::DEV_TYPE_CVR == struStorageInfo.nType)
	{
//		storage_info_t cvrInfo;
// 		if (!GetStorageInfo(recRecord.cvrID, cvrInfo))
// 		{
// 			TPLOG_ERROR("get cvr info failed by %s", recRecord.cvrID.c_str());
// 			return false;
// 		}

		NET_DVR_TIME beginTime = {0};
		NET_DVR_TIME endTime = {0};
		if (!GetNetTimeFromTimeString(recRecord.startTime.c_str(), beginTime)
			|| !GetNetTimeFromTimeString(recRecord.stopTime.c_str(), endTime))
		{
			return false;
		}

		LONG lLoginID = LoginCVR(struStorageInfo);
		if (STORAGE_LOGIN_FAILED == lLoginID)
		{
			return false;
		}

		bool bResult = LockFileInCVR(lLoginID,
			fcT2A(recRecord.cameraID.c_str()), beginTime, endTime);
		NET_DVR_Logout(lLoginID);
		if (!bResult)
		{
			TPLOG_ERROR("del record %s:%s:%s",
				(LPCSTR)fcT2A(recRecord.cameraID.c_str()),
				(LPCSTR)fcT2A(recRecord.startTime.c_str()),
				(LPCSTR)fcT2A(recRecord.stopTime.c_str()));
			return false;
		}

		return true;
	}
    else if (cms_8120::DEV_TYPE_CLOUD_STORAGE == struStorageInfo.nType)
	{
//         storage_info_t cloudInfo;
//         if (!GetStorageInfo(recRecord.cvrID, cloudInfo))
// 		{
// 			TPLOG_ERROR("get cloud info failed by %s", recRecord.cvrID.c_str());
// 			return false;
// 		}

        HCS_Time beginTime = {0};
		HCS_Time endTime = {0};
		if (!GetHcsTimeFromTimeString(recRecord.startTime.c_str(), beginTime)
			|| !GetHcsTimeFromTimeString(recRecord.stopTime.c_str(), endTime))
		{
			return false;
		}

        unsigned long long lLoginID;
		int nRes = HCS_SDK_Login(fcT2A(struStorageInfo.host.c_str()), struStorageInfo.nPort, fcT2A(struStorageInfo.username.c_str()),
            fcT2A(struStorageInfo.password.c_str()), &lLoginID);
		if (HCS_OK != nRes)
		{
			TPLOG_ERROR("login cloud %s:%d:%s failed(%d)",
				(LPCSTR)fcT2A(struStorageInfo.host.c_str()),
				struStorageInfo.nPort,
				(LPCSTR)fcT2A(struStorageInfo.username.c_str()),
				nRes);
			return false;
		}

		nRes = HCS_SDK_RecDelete(lLoginID, 
			fcT2A(recRecord.cameraID.c_str()), 
			"65535",
			beginTime, endTime);
		HCS_SDK_Logout(lLoginID); 
		if (HCS_OK != nRes)
		{
			TPLOG_ERROR("del record %s:%s:%s failed(%d)",
				(LPCSTR)fcT2A(recRecord.cameraID.c_str()),
				(LPCSTR)fcT2A(recRecord.startTime.c_str()),
				(LPCSTR)fcT2A(recRecord.stopTime.c_str()),
				nRes);
			return false;
		}

		return true;
	}
    else if (cms_8120::DEV_TYPE_KMS == struStorageInfo.nType)
    {
//         storage_info_t kmsInfo;
//         if (!GetStorageInfo(recRecord.cvrID, kmsInfo))
//         {
//             TPLOG_ERROR("get kms info failed by %s", recRecord.cvrID.c_str());
//             return false;
//         }
        std::string strToken;
        if (!GetHttpToken(struStorageInfo, strToken))
        {
            TPLOG_ERROR("GetHttpToken failed.");
//             Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//                 ALL_USER,
//                 ALL_CHANNEL,
//                 ALARM_OCCUR,
//                 1,
//                 _T("获取KMS的token认证信息失败"));
            return false;
        }
        if (!DeleteFileInKMS(struStorageInfo, strToken, recRecord.cameraID))
        {
            TPLOG_ERROR("del record %s on kms:%d failed",
                (LPCSTR)fcT2A(recRecord.cameraID.c_str()),
                recRecord.cvrID);
//             Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
//                 ALL_USER,
//                 ALL_CHANNEL,
//                 ALARM_OCCUR,
//                 1,
//                 _T("删除KMS的录像文件失败"));
            return false;
        }
        return true;
    }
	else if (cms_8120::DEV_TYPE_OBJECTCLOUD == struStorageInfo.nType)
	{
		std::string strObjectCloudIp = fcT2A(struStorageInfo.host.c_str());
		int nObjectCloudPort         = struStorageInfo.nPort;
		std::string strAccessKey     = fcT2A(struStorageInfo.strAccessKey.c_str());
		std::string strSecretKey     = fcT2A(struStorageInfo.strSecretKey.c_str());
		std::string strBucket        = fcT2A(recRecord.strBucket.c_str());
		std::string strObjectKey     = fcT2A(recRecord.cameraID.c_str());
		HCSClient * pHCSClient = NULL;
		std::stringstream strFormat ;
		strFormat << strObjectCloudIp << "_" << nObjectCloudPort;
		std::map<std::string, HCSClient *>::iterator iterHCSClient = m_mapHCSClient.find(strFormat.str());
		if (iterHCSClient == m_mapHCSClient.end())
		{
			char endpoint[30]={0};
			HCSClient* pClient = NULL;
			sprintf(endpoint, "%s:%d", strObjectCloudIp.c_str(), nObjectCloudPort);
			pClient = ClientInitialize(strAccessKey.c_str(), strSecretKey.c_str(), endpoint);
			//调用初始化信息函数
			if (NULL == pClient)
			{
				TPLOG_ERROR("ClientInitialize  fail,IP(%s),Port(%d)", strObjectCloudIp.c_str(), nObjectCloudPort);
				return  false;
			}
			TPLOG_INFO("ClientInitialize  succ,IP(%s),Port(%d)", strObjectCloudIp.c_str(), nObjectCloudPort);
			m_mapHCSClient.insert(make_pair(strFormat.str(), pClient));
			pHCSClient = pClient;
		}
		else
		{
			pHCSClient = iterHCSClient->second;
		}
        int result = DeleteObjectByName(pHCSClient, strBucket.c_str(), strObjectKey.c_str(), NULL);
		if (0 != result && OBJECT_KEY_NOT_EXIST != result)
		{
			TPLOG_ERROR("ObjectCloud del record %s:%s:%s failed(%d)",
				(LPCSTR)fcT2A(recRecord.cameraID.c_str()),
				(LPCSTR)fcT2A(recRecord.startTime.c_str()),
				(LPCSTR)fcT2A(recRecord.stopTime.c_str()),
				result);
			return false;
		}

		return true;
	}
	else if (cms_8120::DEV_TYPE_DVR == struStorageInfo.nType || cms_8120::DEV_TYPE_NVR == struStorageInfo.nType)
	{
		bool bResult = UnLockDvrRecord(struStorageInfo, recRecord);
		if (!bResult)
		{
			TPLOG_ERROR("del record fail:%d_%d_%s_%s", 
				recRecord.devId,
				recRecord.devChannel,
				fcT2A(recRecord.startTime.c_str()),
				fcT2A(recRecord.stopTime.c_str()));
			return false;
		}
		return true;
	}

	return false;
}

bool CRecGuard::UnLockDvrRecord(const storage_info_t& struStorageInfo, const rec_record_t &recRecord)
{
	NET_DVR_TIME beginTime = {0};
	NET_DVR_TIME endTime = {0};
	if (!GetNetTimeFromTimeString(recRecord.startTime.c_str(), beginTime)
		|| !GetNetTimeFromTimeString(recRecord.stopTime.c_str(), endTime))
	{
		TPLOG_ERROR("解锁录像失败 格式化时间格式失败:%d_%d_%s_%s", 
			recRecord.devId,
			recRecord.devChannel,
			fcT2A(recRecord.startTime.c_str()),
			fcT2A(recRecord.stopTime.c_str()));
		Msger_NotifyMsg(ALL_USER, _T("解锁录像失败 格式化时间格式失败: %d_%d_%s_%s "),
			recRecord.devId,
			recRecord.devChannel,
			recRecord.startTime.c_str(),
			recRecord.stopTime.c_str());
		return false;
	}
	LONG lLoginID = LoginCVR(struStorageInfo);
	if (STORAGE_LOGIN_FAILED == lLoginID)
	{
		return false;
	}

	//按时间解锁
	//查找文件
	NET_DVR_FILECOND_V40 struFileCond={0};
	struFileCond.dwFileType = 0xFF;
	struFileCond.lChannel = recRecord.devChannel + 1; //通道号，需要加上起始通道号
	struFileCond.dwIsLocked = 0xff; //解锁文件
	struFileCond.dwUseCardNo = 0;

	struFileCond.struStartTime = beginTime; //开始时间
	struFileCond.struStopTime  = endTime; //结束时间
	//查找录像文件 
	int lFindHandle = NET_DVR_FindFile_V40(lLoginID, &struFileCond);
	if (lFindHandle < 0)
	{
		DWORD ulErrorCode = NET_DVR_GetLastError();
		TPLOG_ERROR("按时间查找文件失败(%d)%d_%d_%s_%s",
			ulErrorCode,
			recRecord.devId,
			recRecord.devChannel,
			fcT2A(recRecord.startTime.c_str()),
			fcT2A(recRecord.stopTime.c_str()));
		Msger_NotifyMsg(ALL_USER, _T("按时间查找文件失败(%d)%d_%d_%s_%s"),
			ulErrorCode,
			recRecord.devId,
			recRecord.devChannel,
			recRecord.startTime.c_str(),
			recRecord.stopTime.c_str());
		//注销用户
		NET_DVR_Logout(lLoginID);
		return false;
	}

	TPLOG_INFO("按时间查找文件开始：%d_%d_%s_%s",
		recRecord.devId,
		recRecord.devChannel,
		fcT2A(recRecord.startTime.c_str()),
		fcT2A(recRecord.stopTime.c_str()));

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
				TPLOG_ERROR("按时间查找文件超时%d_%d_%s_%s",
					recRecord.devId,
					recRecord.devChannel,
					fcT2A(recRecord.startTime.c_str()),
					fcT2A(recRecord.stopTime.c_str()));
				Msger_NotifyMsg(ALL_USER, _T("按时间查找文件超时%d_%d_%s_%s"),
					recRecord.devId,
					recRecord.devChannel,
					recRecord.startTime.c_str(),
					recRecord.stopTime.c_str());
				NET_DVR_FindClose_V30(lFindHandle);
				NET_DVR_Logout(lLoginID);
				return false;
			}
			continue;
		}
		else if (NET_DVR_FILE_SUCCESS == iResult) //获取文件信息成功，将其解锁
		{ 
			beginFind = time(NULL);
			TPLOG_INFO("查找到文件,%d-%d-%d %d:%d:%d_%d-%d-%d %d:%d:%d",
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
			if (0 == struFileData.byLocked)
			{
				continue;
			}
			if (TRUE != NET_DVR_UnlockFileByName(lLoginID, (char*)(struFileData.sFileName)))
			{
				DWORD ulErrorCode = NET_DVR_GetLastError();
				TPLOG_ERROR("解锁录像失败(错误码%d) %d_%d_%s_%s",
					ulErrorCode,
					recRecord.devId,
					recRecord.devChannel,
					fcT2A(recRecord.startTime.c_str()),
					fcT2A(recRecord.stopTime.c_str()));
				Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
					ALL_USER,
					recRecord.devChannel,
					ALARM_OCCUR,
					1,
					_T("解锁录像失败(错误码%d) %d_%d_%s_%s "),
					ulErrorCode,
					recRecord.devId,
					recRecord.devChannel,
					recRecord.startTime.c_str(),
					recRecord.stopTime.c_str());
				NET_DVR_FindClose_V30(lFindHandle);
				NET_DVR_Logout(lLoginID);
				return false;
			}
		}
		else if(NET_DVR_FILE_NOFIND == iResult)
		{
			TPLOG_ERROR("查找到零个文件 %d_%d_%s_%s",
				recRecord.devId,
				recRecord.devChannel,
				fcT2A(recRecord.startTime.c_str()),
				fcT2A(recRecord.stopTime.c_str()));
			Msger_NotifyMsg(ALL_USER, _T("查找到零个文件%d_%d_%s_%s"),
				recRecord.devId,
				recRecord.devChannel,
				(recRecord.startTime.c_str()),
				(recRecord.stopTime.c_str()));
			NET_DVR_FindClose_V30(lFindHandle);
			NET_DVR_Logout(lLoginID);
			return true;
		}
		else if (NET_DVR_NOMOREFILE == iResult)
		{
			TPLOG_INFO("NET_DVR_NOMOREFILE");
			break;
		}
		else
		{
			DWORD ulErrorCode = NET_DVR_GetLastError();
			TPLOG_ERROR("查找文件异常(错误码%d)%d_%d_%s_%s",
				ulErrorCode,
				recRecord.devId,
				recRecord.devChannel,
				fcT2A(recRecord.startTime.c_str()),
				fcT2A(recRecord.stopTime.c_str()));
			Msger_NotifyMsg(ALL_USER, _T("查找文件异常(错误码%d)%s_%d_%d_%s_%s"),
				ulErrorCode,
				recRecord.devId,
				recRecord.devChannel,
				(recRecord.startTime.c_str()),
				(recRecord.stopTime.c_str()));
			NET_DVR_FindClose_V30(lFindHandle);
			NET_DVR_Logout(lLoginID);
			return false;
		}
	}

	TPLOG_INFO("按时间查找文件结束：%d_%d_%s_%s",
		recRecord.devId,
		recRecord.devChannel,
		fcT2A(recRecord.startTime.c_str()),
		fcT2A(recRecord.stopTime.c_str()));
	NET_DVR_FindClose_V30(lFindHandle);
	NET_DVR_Logout(lLoginID);
	return true;
}

/**	@fn	    getDvrTimeStamp
*	@brief	得到时间戳
*	@param  [in]  stDvrTimeSt -- 时间
*	@param  [out] iTime -- 时间戳
*	@return	BOOL
*/
void CRecGuard::getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime)
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

bool CRecGuard::DelOldBzRecordByIDs(
    const std::vector<HPR_INT32> &bzRecordIDs)
{
	if (bzRecordIDs.empty())
	{
		return true;
	}
	stringstream ss;
	ss << bzRecordIDs[0];
	// 每次最多删除150条，超过150条剩余的下次再删
	for (vector<HPR_INT32>::size_type i = 1; i < bzRecordIDs.size() && i < MAX_DELETE_CLIENTS_ID; ++i)
	{
		ss << "," << bzRecordIDs[i];
	}
	const string strRecordIDs = ss.str();

    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->DelClientRecords(strRecordIDs))
    {
        TPLOG_ERROR("failed to DelOldBzRecordByIDs");
        Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
            ALL_USER,
            ALL_CHANNEL,
            ALARM_OCCUR,
            1,
            _T("删除过期记录信息失败"));
        return false;
    }

    return true;
}

bool CRecGuard::GetTimeFromTimeString(
    LPCTSTR timeString,
    __time32_t &t)
{
    if (NULL == timeString)
    {
        return false;
    }

    COleDateTime tim;
    if (!tim.ParseDateTime(timeString))
    {
        TPLOG_ERROR("parse time %s failed", fcT2A(timeString));
        return false;
    }

    t = (__time32_t)CTime(tim.GetYear(), tim.GetMonth(), tim.GetDay(),
        tim.GetHour(), tim.GetMinute(), tim.GetSecond()).GetTime();

    if (-1 == t)
    {
        TPLOG_ERROR("bad time %s to parse", fcT2A(timeString));
        return false;
    }

    return true;    
}

bool CRecGuard::MarkOldBzRecords(const int &nClientID)
{
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->MarkOldBzRecords(nClientID))
    {
        TPLOG_ERROR("failed to MarkOldBzRecords");
        Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
            ALL_USER,
            ALL_CHANNEL,
            ALARM_OCCUR,
            1,
            _T("标记过期记录失败"));
        return false;
    }

    return true;
}

bool CRecGuard::IsWorkingTime(void)
{
    work_param_t workParam;
    if (!GetSysConfig(workParam))
    {
        return false;
    }

    COleDateTime startTime;
    COleDateTime stopTime;
    if (!ConvertToCTime(workParam, startTime, stopTime))
    {
        TPLOG_ERROR("failed to convert time");
        return false;
    }

    COleDateTime nowTime = COleDateTime::GetCurrentTime();
    startTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        startTime.GetHour(), startTime.GetMinute(), startTime.GetSecond());
    stopTime.SetDateTime(nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(),
        stopTime.GetHour(), stopTime.GetMinute(), stopTime.GetSecond());

    // 结束时间比开始时间大
    if (startTime < stopTime)
    {
        if (startTime <= nowTime && nowTime <= stopTime)
        {
            return true;
        }
        return false;
    }
    else
    {
        if (stopTime < nowTime && nowTime < startTime)
        {
            return false;
        }
        return true;
    }
}

bool CRecGuard::ConvertToCTime(const work_param_t &workParam,
                    COleDateTime &startTime,
                    COleDateTime &stopTime)
{
    if (!startTime.ParseDateTime(workParam.startTime.c_str()))
    {
        TPLOG_ERROR("parse start time failed");
        return false;
    }
    if (!stopTime.ParseDateTime(workParam.stopTime.c_str()))
    {
        TPLOG_ERROR("parse stop time failed");
        return false;
    }

    return true;
}

/**	@fn	    GetSysConfig
*	@brief	获取中心系统信息
*	@param  [in] stWorkParam -- 工作参数
*	@param  [out] stWorkParam -- 工作参数
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
bool CRecGuard::GetSysConfig(work_param_t &stWorkParam)
{
    ::ResetEvent(m_hWorkParamEvent);
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetSysConfig())
    {
        TPLOG_ERROR("GetSysConfig failed.");
        return false;
    }
    if (WAIT_OBJECT_0 != WaitForSingleObject(m_hWorkParamEvent, 1000*10))
    {
        TPLOG_ERROR("WaitForSingleObject overtime."); 
        return false;
    }
    stWorkParam = m_struWorkParam;
    return true;
}

/**	@fn	    HandleStorage
*	@brief	处理中心参数信息
*	@param  [in] sysconfigList -- 存储设备信息
*   @param  [in] bFinish -- 是否结束
*	@return	
*/
void CRecGuard::HandleSysConfig(const cms_8120::SysconfigList &sysconfigList, BOOL bFinish /*= true*/)
{
    if (bFinish)
    {
        TPLOG_ERROR("sysconfigList size:%d", sysconfigList.sysconfig_size());
        for (int i = 0; i < sysconfigList.sysconfig_size(); ++i)
        {
            cms_8120::Sysconfig msgSysconfig = sysconfigList.sysconfig(i);
            switch(msgSysconfig.nkey())
            {
//             case WP_STORAGE_TYPE:
//                 m_struWorkParam.storageType = atoi(CSCT::Utf82Ansi(msgSysconfig.strvalue()).c_str());
//                 break;
            case WP_TIP_DAY:
                m_struWorkParam.tipDays = atoi(CSCT::Utf82Ansi(msgSysconfig.strvalue()).c_str());
                break;
            case WP_STARTTIME:
                m_struWorkParam.startTime = CSCT::Utf82Unicode(msgSysconfig.strvalue());
                break;
            case WP_STOPTIME:
                m_struWorkParam.stopTime = CSCT::Utf82Unicode(msgSysconfig.strvalue());
                break;
            default:
                break;
            }
        }
        ::SetEvent(m_hWorkParamEvent);
    }
    else
    {
        TPLOG_ERROR("AddRecordsList....one time not send all records!");
    }
}

/**	@fn	    HandleOverRecord
*	@brief	处理过期录像信息
*	@param  [in] overtimeRecord -- 过期录像信息
*   @param  [in] bFinish -- 是否结束
*	@return	
*/
void CRecGuard::HandleOverRecord(const cms_8120::RspOvertimeRecord &overRecordList, BOOL bFinish /*= true*/)
{
    if (bFinish)
    {
        TPLOG_ERROR("overRecordList size:%d", overRecordList.overtimerecord_size());
        for (int i = 0; i < overRecordList.overtimerecord_size(); ++i)
        {
            cms_8120::OvertimeRecord msgOverRecord = overRecordList.overtimerecord(i);
            bz_record_t bzRecord;
            bzRecord.recordID = msgOverRecord.id();
            bzRecord.bzID = CSCT::Utf82Unicode(msgOverRecord.proid());
            bzRecord.bzTime = CSCT::Utf82Unicode(msgOverRecord.businesstime());
            //bzRecord.productName = CSCT::Utf82Unicode(msgOverRecord.strvalue());
            //bzRecord.tipFlag = msgOverRecord.tipflag();
            m_vecBzRecord.push_back(bzRecord);
        }
        ::SetEvent(m_hOverRecordEvent);
    }
    else
    {
        TPLOG_ERROR("overRecordList....one time not send all records!");
    }
    
}

/**	@fn	    HandleRecord
*	@brief	处理录像信息
*	@param  [in] prosaverecordList -- 录像信息
*   @param  [in] bFinish -- 是否结束
*	@return	
*/
void CRecGuard::HandleRecord(const cms_8120::ProsaverecordList &prosaverecordList, BOOL bFinish /*= true*/)
{
    if (bFinish)
    {
        TPLOG_ERROR("prosaverecordList size:%d", prosaverecordList.prosaverecord_size());
        for (int i = 0; i < prosaverecordList.prosaverecord_size(); ++i)
        {
            cms_8120::Prosaverecord msgRecord = prosaverecordList.prosaverecord(i);
            rec_record_t recRecord;
            recRecord.ID = msgRecord.id();
            recRecord.recordID = msgRecord.clientid();
            recRecord.cameraID = CSCT::Utf82Unicode(msgRecord.streamno());
            recRecord.cvrID = msgRecord.cvr();
			recRecord.strBucket = CSCT::Utf82Unicode(msgRecord.strbucket());
            recRecord.startTime = CSCT::Utf82Unicode(msgRecord.starttime());
            recRecord.stopTime = CSCT::Utf82Unicode(msgRecord.endtime());
			recRecord.devId = msgRecord.devid();
			recRecord.devChannel = msgRecord.devchannel();
            if (1 == msgRecord.upload())
            {
                recRecord.bUpload = true;
            }
            else
            {
                recRecord.bUpload = false;
            }
            m_vecRecRecord.push_back(recRecord);
        }
        ::SetEvent(m_hRecordEvent);
    }
    else
    {
        TPLOG_ERROR("prosaverecordList....one time not send all records!");
    } 
}

/**	@fn	    HandleStorageInfo
*	@brief	获取存储器信息
*	@param  [in] storage -- 存储器信息
*	@return	void
*/
void CRecGuard::HandleStorageInfo(const cms_8120::DeviceList &storage)
{
    if (storage.device_size() <= 0)
    {
        TPLOG_ERROR("the storage not exist");
        return;
    }
    cms_8120::Device device = storage.device(0);
    m_struStorage.nID = device.id();
    m_struStorage.nType = device.ntype();
    m_struStorage.host = CSCT::Utf82Unicode(device.strip());
    m_struStorage.nPort = device.nport();
    m_struStorage.username = CSCT::Utf82Unicode(device.struser());
    m_struStorage.password = CSCT::Utf82Unicode(device.strpassword());
    char aes[1024] = { '\0' };
    int aes_len = 1024;
    //如果加密了 则解密
    if (Hak9(fcT2A(m_struStorage.password.c_str()), NULL, NULL))
    {
        char* out = aes;
        char* outl = (char*)&aes_len;
        Hak7(out, fcT2A(m_struStorage.password.c_str()), outl);
        m_struStorage.password = fcA2T(out);
    }
    m_struStorage.regionID = device.nregionid();

	if (cms_8120::DEV_TYPE_OBJECTCLOUD  == m_struStorage.nType)
	{
		m_struStorage.strAccessKey = CSCT::Utf82Unicode(device.straccesskey());
		char aesAccessKey[1024] = { '\0' };
		int aesAccessKey_len = 1024;
		//如果加密了 则解密
		if (Hak9(fcT2A(m_struStorage.strAccessKey.c_str()), NULL, NULL))
		{
			char* out = aesAccessKey;
			char* outl = (char*)&aesAccessKey_len;
			Hak7(out, fcT2A(m_struStorage.strAccessKey.c_str()), outl);
			m_struStorage.strAccessKey = fcA2T(out);
		}

		m_struStorage.strSecretKey = CSCT::Utf82Unicode(device.strsecretkey());
		char aesSecretKey[1024] = { '\0' };
		int aesSecretKey_len = 1024;
		//如果加密了 则解密
		if (Hak9(fcT2A(m_struStorage.strSecretKey.c_str()), NULL, NULL))
		{
			char* out = aesSecretKey;
			char* outl = (char*)&aesSecretKey_len;
			Hak7(out, fcT2A(m_struStorage.strSecretKey.c_str()), outl);
			m_struStorage.strSecretKey = fcA2T(out);
		}
	}
    ::SetEvent(m_hStorageDevEvent);
}

/**	@fn	    GetStorageInfo
*	@brief	获取存储信息
*	@param  [in] storageID -- 存储器ID
*	@param  [out] storage -- 存储器信息
*	@return	bool
*/
bool CRecGuard::GetStorageInfo(const HPR_INT32 &storageID, storage_info_t &storage)
{
    ::ResetEvent(m_hStorageDevEvent);
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->GetStorageInfo(storageID))
    {
        TPLOG_ERROR("GetStorageInfo failed.stroageID:%d", storageID);
        return false;
    }
    if (WAIT_OBJECT_0 != WaitForSingleObject(m_hStorageDevEvent, 1000*10))
    {
        TPLOG_ERROR("WaitForSingleObject overtime."); 
        return false;
    }
    storage = m_struStorage;
    return true;
}

bool CRecGuard::GetHcsTimeFromTimeString(LPCTSTR timeString, HCS_Time &t)
{
    if (NULL == timeString)
    {
        return false;
    }

    COleDateTime tim;
    if (!tim.ParseDateTime(timeString))
    {
        TPLOG_ERROR("parse time %s failed", fcT2A(timeString));
        return false;
    }

    t.year = tim.GetYear();
    t.month = tim.GetMonth();
    t.day = tim.GetDay();
    t.hour = tim.GetHour();
    t.minute = tim.GetMinute();
    t.second = tim.GetSecond();
    t.millisecond = 0;

    return true;  
}

/**	@fn	    LoginCVR
 *	@brief	登录CVR
 *	@param  [in]struCVRInfo -- CVR参数
 *	@return	LONG -- 登录句柄
 */
LONG CRecGuard::LoginCVR(const storage_info_t& struCVRInfo)
{
    LONG lLoginID = STORAGE_LOGIN_FAILED;
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
    memcpy_s(struLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, fcT2A(struCVRInfo.host.c_str()), strlen(fcT2A(struCVRInfo.host.c_str())));
    struLoginInfo.wPort = struCVRInfo.nPort;
    memcpy_s(struLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, fcT2A(struCVRInfo.username.c_str()), strlen(fcT2A(struCVRInfo.username.c_str())));
    memcpy_s(struLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, fcT2A(struCVRInfo.password.c_str()), strlen(fcT2A(struCVRInfo.password.c_str())));
    struLoginInfo.cbLoginResult = NULL;
    struLoginInfo.pUser = NULL;
    struLoginInfo.bUseAsynLogin = 0;
    lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if (STORAGE_LOGIN_FAILED == lLoginID)
    {
        TPLOG_ERROR("call NET_DVR_Login_V40 fail. ip:%s, port:%d, error[%d]",
            (LPCSTR)fcT2A(struCVRInfo.host.c_str()), struCVRInfo.nPort, NET_DVR_GetLastError());
        return lLoginID;
    }
    TPLOG_INFO("Login succeed. ip:%s, port:%d",
        (LPCSTR)fcT2A(struCVRInfo.host.c_str()), struCVRInfo.nPort);
    return lLoginID;
}

bool CRecGuard::GetNetTimeFromTimeString(LPCTSTR timeString, NET_DVR_TIME &t)
{
    if (NULL == timeString)
    {
        return false;
    }

    COleDateTime tim;
    if (!tim.ParseDateTime(timeString))
    {
        TPLOG_ERROR("parse time %s failed", fcT2A(timeString));
        return false;
    }

    t.dwYear = tim.GetYear();
    t.dwMonth = tim.GetMonth();
    t.dwDay = tim.GetDay();
    t.dwHour = tim.GetHour();
    t.dwMinute = tim.GetMinute();
    t.dwSecond = tim.GetSecond();

    return true;  
}

/**	@fn	    LockFileInCVR
 *	@brief	锁定CVR上的录像文件
 *	@param  [in]lLoginID -- 登录句柄
 *	@param  [in]strCameraID -- 流ID
 *	@param  [in]struStartTime -- 录像开始时间
 *	@param  [in]struStopTime -- 录像结束时间
 *	@return	bool
 */
bool CRecGuard::LockFileInCVR(const LONG& lLoginID, const std::string& strCameraID, const NET_DVR_TIME& struStartTime, const NET_DVR_TIME& struStopTime)
{
    NET_DVR_STREAM_TIME_LOCK struUnLock = {0};
    struUnLock.dwSize = sizeof(NET_DVR_STREAM_TIME_LOCK);//大小
    struUnLock.strBeginTime = struStartTime;    //录像开始时间
    struUnLock.strEndTime = struStopTime;       //录像结束时间
    //流信息
    struUnLock.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO); //流信息大小
    memcpy_s(struUnLock.struStreamInfo.byID, STREAM_ID_LEN, strCameraID.c_str(), strlen(strCameraID.c_str()));  //流ID
    struUnLock.struStreamInfo.dwChannel = 0xffffffff;   //通道号
    struUnLock.dwRecordType = 10;   //录像类型（回传录像）
    struUnLock.dwLockDuration = 0;  //锁定持续时间，0表示解锁
    NET_DVR_LOCK_RETURN struLockReturn = {0};
    struLockReturn.dwSize = sizeof(NET_DVR_LOCK_RETURN);
    if (!NET_DVR_UnlockStreamFileByTime(lLoginID, &struUnLock, &struLockReturn))
    {
        TPLOG_ERROR("call NET_DVR_UnlockStreamFileByTime failed.stream id:%s, error[%d]",
            strCameraID.c_str(), NET_DVR_GetLastError());
        return false;
    }
    TPLOG_INFO("unlock file succeed.stream id:%s", strCameraID.c_str());
    return true;
}

//回调函数：获取返回结果
size_t OnCallBackGetHttpInfo(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        TPLOG_ERROR("str or buffer is NULL...");
        return 0;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);

    return nmemb * size;   
}

/** @fn GetHttpToken
*  @brief  向KMS获取Token认证信息
*  @param [in]struKMSInfo KMS的配置信息
*  @param [out]strToken 获取到的token信息
*  @return bool
*/
bool CRecGuard::GetHttpToken(const storage_info_t& struKMSInfo, std::string& strToken)
{
    std::string strResponse;
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        TPLOG_ERROR("curl is null...");
        return false;  
    }  

    char strURL[200] = {0};
    sprintf_s(strURL, sizeof(strURL), "http://%s:%d/kms/services/rest/operationService/getTokenInfo?username=%s&&password=%s", 
        fcT2U(struKMSInfo.host.c_str()), struKMSInfo.nPort, fcT2U(struKMSInfo.username.c_str()), fcT2U(struKMSInfo.password.c_str()));
    curl_easy_setopt(curl, CURLOPT_URL, strURL);
    /*curl_easy_setopt(curl, CURLOPT_URL, "http://10.16.38.27:8080/kms/services/rest/operationService/getTokenInfo?username=administrator&&password=hik12345_");*/
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL); 
    //设置回调
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);      
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        TPLOG_ERROR("curl_easy_perform failed.res:%d", (int)res);
        return false;
    }
    TPLOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //{"node":"10.16.38.27:8080","ret":0,"msg":"操作成功","tokenError":false,"token":"807d6bf90000014f60ecfe434f42a54555"}
    //调用jsoncpp库解析返回的内容
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            strToken = root["token"].asString();
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            TPLOG_ERROR("getToken failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return false;
        }
    }
    else
    {
        TPLOG_ERROR("parse token info failed...");
        return false;
    }
    TPLOG_INFO("strToken is %s", fcU2A(strToken.c_str())); 
    return true;
}

/** @fn DeleteFileInKMS
*  @brief  向KMS获取Token认证信息
*  @param [in]struKMSInfo KMS的配置信息
*  @param [in]strToken token信息
*  @param [in]strFileId 文件ID
*  @return bool
*/
bool CRecGuard::DeleteFileInKMS(const storage_info_t& struKMSInfo, const std::string& strToken, const std::_tstring& strFileId)
{
    std::string strResponse;
    CURL* curl = curl_easy_init();
    if(NULL == curl)  
    {  
        TPLOG_ERROR("curl is null...");
        return false;  
    }  
    CURLcode res; 
    char szURL[200] = {0};
    sprintf_s(szURL, sizeof(szURL), "http://%s:%d/kms/services/rest/deleteService/deleteFiles?id=%s&&token=%s", 
        fcT2U(struKMSInfo.host.c_str()), struKMSInfo.nPort, fcT2U(strFileId.c_str()), strToken.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, szURL);

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&strResponse ); 
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, OnCallBackGetHttpInfo); 

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform( curl ); 
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        TPLOG_ERROR("curl_easy_perform failed.");
        return false;
    }
    TPLOG_INFO("strResponse is %s", fcU2A(strResponse.c_str()));
    //     {
    //         "ret": 0,
    //             "msg": "操作成功"
    //     }

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(strResponse.c_str(), root))
    {
        int nRet = root["ret"].asInt();
        if (0 == nRet)
        {
            return true;
        }
        else
        {
            std::string strMsg = root["msg"].asString();
            TPLOG_ERROR("deleteFiles failed.ErrorMsg:%s", fcU2A(strMsg.c_str()));
            return false;
        }
    }
    else
    {
        TPLOG_ERROR("parse upload info failed...");
        return false;
    }
}

bool CRecGuard::DelRecRecordByIDs(const int &nRecordID)
{   
//     if (recRecordIDs.empty())
//     {
//         return true;
//     }
//     stringstream ss;
//     ss << recRecordIDs[0];
//     for (vector<HPR_INT32>::size_type i = 1;
//         i < recRecordIDs.size(); ++i)
//     {
//         ss << "," << recRecordIDs[i];
//     }
//     std::string strRecordIDs;
//     ss >> strRecordIDs;
    if (HPR_OK != CDBDataMgr::GetSingletonPtr()->DelCenterRecords(nRecordID))
    {
        TPLOG_ERROR("failed to DelRecRecordByIDs");
        Msger_AlarmMsg(WORK_TYPE_GUARD_SVC_FAIL,
            ALL_USER,
            ALL_CHANNEL,
            ALARM_OCCUR,
            1,
            _T("删除录像信息失败"));
        return false;
    }

    return true;
}

