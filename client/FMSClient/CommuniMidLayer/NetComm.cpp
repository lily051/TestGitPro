#include "stdafx.h"
#include "NetComm.h"
#include <Shlwapi.h>
#include "./ConnSessionMgr.h"
#include "./PbWrapRelative.h"
//#include <msg/AlarmClientSync.pb.h>


const TCHAR* CLIENT_CONFIG_FILE = _T("CentralClient.ini");

/** @fn GetComponentID
*   @brief 获取目标组件ID
*   @param[in] nCmdID: 消息命令码
*   @param NULL
*   @return 组件ID
*/
int CNetComm::GetComponentID(int nCmdID)
{
	int nComId = MENUID_INVALID;
// 	switch (nCmdID)
// 	{
// 	case cms_8100::CMD_RSP_CLIENT_LOGIN:    //登录
// 		{
// 			nComId = 0;
// 		}
// 		break;
// 	case cms_8100::CMD_MSG_ACTION:          //收到接处警消息
//     case cms_8100::CMD_MSG_ALARM_CLIENTSYNC: // 收到同步消息
// 		{
// 			nComId = MENUID_ALARMCENTER;
// 		}
// 		break;
//     case cms_8100::CMD_BW_NOTICE_PRE:
//         {
//             nComId = MENUID_PREVIEW;
//         }
//         break;
//     case cms_8100::CMD_BW_NOTICE_PB:
//         {
//             nComId = MENUID_PLAYBACK;
//         }
//         break;
// 	default:
// 		break;
// 	}

	return nComId;
}

/** @fn CheckTimeoutMsg
*   @brief 超时消息检查线程
*   @param[in] lpvoid: 自定义参数
*   @param NULL
*   @return NULL
*/
void CNetComm::CheckTimeoutMsg(void)
{
//     LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
//     vector<ASYNRSP_CONTEXT> vTimeoutMsg;
//     {
//         CGuard guard(&m_lckAsynRspContext);
//         vector<ASYNRSP_CONTEXT>::iterator itor = m_vAsynRspContext.begin();
//         while (itor != m_vAsynRspContext.end())
//         {
//             if (itor->lWaitTime + m_nMsgTimeout < lCurTime)
//             {
//                 vTimeoutMsg.push_back(*itor);
//                 itor = m_vAsynRspContext.erase(itor);
//                 continue;
//             }
//             itor++;
//         }
//     }
// 
//     //此时处理所有的超时消息
//     vector<ASYNRSP_CONTEXT>::iterator itTimeout = vTimeoutMsg.begin();
//     while (itTimeout != vTimeoutMsg.end())
//     {
//         CModuleDllMgr::SnglPtr()->OnRspTimeoutNotify(itTimeout->nComponentID, itTimeout->nCmdID);
//         itTimeout++;
//     }
}

//////////////////////////////////////////////////////////////////////////
// CNetComm

SINGLETON_IMPLEMENT(CNetComm)///<单例相关实现

CNetComm::CNetComm()
: m_nMsgTimeout(30)
{
    InitializeCriticalSection(&m_lckSyncRspContext);
    InitializeCriticalSection(&m_lckAsynRspContext);
}

CNetComm::~CNetComm()
{
	DeleteCriticalSection(&m_lckSyncRspContext);
    DeleteCriticalSection(&m_lckAsynRspContext);
}

/** @fn Init
*   @brief 网络初始化
*   @param[in] pMsgNeedRsp: 回调函数
*   @param[in] lpvoid: 用户参数
*   @return NULL
*/
BOOL CNetComm::Init()
{
    TCHAR szDir[MAX_PATH] = {0};
    GetModuleFileName(NULL, szDir, MAX_PATH);
    PathRemoveFileSpec(szDir);
    TCHAR szPath[MAX_PATH] = {0};
    PathCombine(szPath, szDir, CLIENT_CONFIG_FILE);

    // 消息超时时间
    m_nMsgTimeout = GetPrivateProfileInt(_T("CentralClient"), _T("MsgTimeout"), 30, szPath);
    if (m_nMsgTimeout < 5
        || m_nMsgTimeout > 60)
    {
        m_nMsgTimeout = 30;
    }

    //事件池初始化
	m_poolEvent.SetEventNum(20);
	m_poolEvent.SetEventTime(m_nMsgTimeout+5);
	m_poolEvent.InitAllEvent();

	//超时消息检查线程
    if (!m_CheckThread.Start()
        || !m_CheckThread.SetTimer(AsyncMsgCheckThread::CHECK_TIMER
                                , AsyncMsgCheckThread::CHECK_INTERVAL))
	{
        m_CheckThread.Stop();
        return FALSE;
	}
	return TRUE;
}

void CNetComm::Fini()
{
	//thread
    CLIENT_INFO("Stop CheckThread...");
	m_CheckThread.Stop();
    CLIENT_INFO("ReleaseAllEvent...");
	m_poolEvent.ReleaseAllEvent();
    m_mapCmdSessionId.clear();
}

//lint -e438
int CNetComm::TransmitCompntMsgEx(string& sReq
                                 , HPR_UINT32 nCmdID
                                 , int emReqMsgType
                                 , SERVER_INFO stServerInfo
                                 , int nComponentID
                                 , HPR_UINT32& nCmdRsp
                                 , string& sRsp
                                 , int nTimeout /*= 0*/)
{
    int nConnSessionId = ConnSessionMgr::SnglPtr()->GetConnSessionID(stServerInfo);
//     if (-1 == nConnSessionId && nCmdID != cms_8100::CMD_MSG_ALARM_CLIENTSYNC)
//     {
//         string sIp = CP936_W2A(stServerInfo.szServerIp);
//         COMM_ERROR("No Connection to %s,%d"
//             , sIp.c_str()
//             , stServerInfo.nServerPort);
//         return TRANSMIT_NO_CONN;
//     }

    //消息组建
    HPR_UINT32 nMsgType = HPP_PACK_HEAD::REQ_MSG_NEED_RSP;
    if (REQ_MSG_NOT_NEED_RSP == emReqMsgType)
    {
        nMsgType = HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP;
    }
    HPR_UINT32 nSeq = HPP_GetUnRepeatSeq();
    FiPBWrap * pWrap = CreateWrapEx(nCmdID
        , nSeq
        , nConnSessionId
        , nMsgType
        , sReq);
    if (NULL == pWrap)
    {
        COMM_ERROR("CreateWrapEx failed, nCmdID=%u", nCmdID);
        return TRANSMIT_ERROR;
    }

    //分类处理
    switch (emReqMsgType)
    {
    case REQ_MSG_WAITING_RSP:
        {
            const HANDLE hEvent = m_poolEvent.GetFreeEvent();
            if (hEvent == NULL)
            {
                COMM_ERROR("GetFreeEvent failed(%d,%u,%u)"
                    , nConnSessionId, nSeq, nCmdID);
                delete pWrap;
                pWrap = NULL;
                return TRANSMIT_ERROR;
            }
            SYNCRSP_CONTEXT stWaitRspEvent;
            stWaitRspEvent.nSeq = nSeq;
            stWaitRspEvent.nConnSessionID = nConnSessionId;
            stWaitRspEvent.hEvent = hEvent;
            stWaitRspEvent.nCmdRsp = nCmdRsp;
            stWaitRspEvent.psMsg = &sRsp;

            {   // 该消息放到等待队列里
                CGuard guard(&m_lckSyncRspContext);
                m_vSyncRspContext.push_back(stWaitRspEvent);
            }

            // Timeout
            DWORD dwTimeout;
            if (nTimeout <= 0)
            {
                dwTimeout = m_nMsgTimeout * 1000;
                int nSec = sReq.size() / 1024 / 1024;
                if (nSec > m_nMsgTimeout)
                {
                    dwTimeout = nSec * 1000;
                }
            }else if (nTimeout > m_nMsgTimeout)
            {
                dwTimeout = m_nMsgTimeout * 1000;
            }else
            {
                dwTimeout = nTimeout * 1000;
            }

            DWORD dwTimeStart = GetTickCount();

            // 消息发送
            const HPR_INT32 nRet = ConnSession::SendMessage_r(pWrap, nConnSessionId);

            if (HPR_OK != nRet)
            {
                COMM_ERROR("SendMessage failed(%d,%u,%u)", nConnSessionId, nSeq, nCmdID);
                delete pWrap;
                pWrap = NULL;
            }else
            {
                COMM_TRACE("SendMessage (%d,%u,%u)", nConnSessionId, nSeq, nCmdID);
                // 发送成功，等待返回消息，最多HPP_TIMEOUT秒
                WaitForSingleObject(hEvent,  dwTimeout);
            }

            BOOL bRspGot = FALSE;   // 是否收到返回消息
            {
                // 注意：该请求消息必须在函数返回前移除，因为一旦返回，
                // SYNCRSP_CONTEXT.pMsg将成为无效指针。之后收到该返回
                // 消息时可能会访问该无效指针
                CGuard guard(&m_lckSyncRspContext);
                std::vector<SYNCRSP_CONTEXT>::iterator itor = m_vSyncRspContext.begin();
                for (;itor != m_vSyncRspContext.end(); itor++)
                {
                    if (itor->nSeq == nSeq
                        && itor->nConnSessionID == nConnSessionId)
                    {
                        bRspGot = itor->bGet;
                        if (FALSE == bRspGot)
                        {
                            // 超时，未等到返回消息或未发送成功
	                           m_poolEvent.SetSingleEvent(itor->hEvent);
                        }else
                        {
                            // 获取返回消息命令码
                            nCmdRsp = itor->nCmdRsp;
                        }
                        m_vSyncRspContext.erase(itor);
                        break;
                    }
                }
            }

            DWORD dwTimeSpan = GetTickCount() - dwTimeStart;
            if (dwTimeSpan > 1000)
            {
                COMM_ERROR("TransitMsg Too Long %u: \n%u", dwTimeSpan, nCmdID);
            }

            if (HPR_OK != nRet)
            {
                return TRANSMIT_FAIL;
            }else
            {
                return (bRspGot == TRUE ? TRANSMIT_SUCC : TRANSMIT_TIMEOUT);
            }            
        }
    case REQ_MSG_ASYN_RSP:
        {
            CLIENT_WARN("REQ_MSG_ASYN_RSP unsupported");
            DebugBreak();

            // <stAsynRsp加入异步等待队列>必须在<收到返回消息>前
            // <返回消息>一定发生在<消息发送>成功后
            // 所以，<stAsynRsp加入异步等待队列>必须放在<消息发送>前
            ASYNRSP_CONTEXT stAsynRsp;
            stAsynRsp.nSeq = nSeq;
            stAsynRsp.nCmdID = nCmdID;
            stAsynRsp.nComponentID = nComponentID;
            {
                CGuard guard(&m_lckAsynRspContext);
                m_vAsynRspContext.push_back(stAsynRsp);
            }

            if (HPR_OK != ConnSession::SendMessage_r(pWrap, nConnSessionId))
            {
                COMM_ERROR("SendMessage(Async response) failed(%d, %u, %u)"
                    , nConnSessionId, nSeq, nCmdID);
                delete pWrap;
                pWrap = NULL;
                return TRANSMIT_FAIL;
            }

            return TRANSMIT_SUCC;
        }
    default:
        {
//             if (cms_8100::CMD_MSG_ALARM_CLIENTSYNC == nCmdID && m_mapCmdSessionId.find(cms_8100::CMD_MSG_ACTION) != m_mapCmdSessionId.end())
//             {
//                 nConnSessionId = m_mapCmdSessionId[cms_8100::CMD_MSG_ACTION];
// 
//                 if (HPR_OK != ConnSession::SendMessage_r(pWrap, nConnSessionId)) // 这里发现一个缺陷，如果pWrapTmp == NULL，就会崩溃
//                 {
//                     COMM_ERROR("SendMessage to linkage server(without response) failed(%d,%u,%u)"
//                         , nConnSessionId, nSeq, nCmdID);
//                     delete pWrap;
//                     pWrap = NULL;
//                     return TRANSMIT_FAIL;
//                 }
//             }
//             else 
				if (HPR_OK != ConnSession::SendMessage_r(pWrap, nConnSessionId))
            {
                COMM_ERROR("SendMessage(without response) failed(%d,%u,%u)"
                    , nConnSessionId, nSeq, nCmdID);
                delete pWrap;
                pWrap = NULL;
                return TRANSMIT_FAIL;
            }
            return TRANSMIT_SUCC;
        }
    }
}
//lint +e438

/** @fn ConnectCms
*   @brief 连接中心服务器
*   @param[in] stServerInfo
*   @param NULL
*   @return TRUE成功，FALSE失败
*/
BOOL CNetComm::ConnectCms(SERVER_INFO stServerInfo)
{
    if (-1 == ConnSessionMgr::SnglPtr()->GetConnSessionID(stServerInfo))
    {
        return FALSE;
    }
    return TRUE;
}

void CNetComm::RecievingTransmissionEx(string& sPbmsg, const HPR_UINT32 nCommand)
{
    int nCmpntID = GetComponentID(nCommand);
//     switch (nCommand)
//     {
//     case cms_8100::CMD_BW_CONNECT:
//         {
//             // 带宽控制的通知消息群发给预览,回放和接警
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_PREVIEW);
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_PLAYBACK);
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_ALARMCENTER);
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_VIEWTALK);
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_VIEWWALL);
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, MENUID_EMAP);
//         }
//         break;
//     case cms_8100::CMD_MSG_ACTION:
//         {
//             // 地图联动发给地图组件
//             cms_8100::MsgAction pbmsgAction;
//             pbmsgAction.ParseFromString(sPbmsg);
//             if (pbmsgAction.action_type() == cms_8100::ACTION_TYPE_EMAP_LINK)
//             {
//                 COMM_TRACE("Emap_Link MsgAction received");
//                 nCmpntID = MENUID_EMAP;
//             }
//             else if (pbmsgAction.action_type() == cms_8100::ACTION_TYPE_COFFER)
//             {
//                 COMM_TRACE("Coffer MsgAction received");
//                 nCmpntID = MENUID_COFFER;
//             }
//             else if (pbmsgAction.action_type() == cms_8100::ACTION_TYPE_TERMINAL_VIEW)
//             {
//                 COMM_TRACE("viewwall MsgAction received");
//                 nCmpntID = MENUID_VIEWWALL;
//             }
//             else if (pbmsgAction.action_type() == cms_8100::ACTION_TYPE_VIDEO_TALK)
//             {
//                 COMM_TRACE("viewtalk MsgAction received");
//                 nCmpntID = MENUID_VIEWTALK;
//             }
//             else if (pbmsgAction.action_type() == cms_8100::ACTION_TYPE_AC_SYS)
//             {
//                 COMM_TRACE("acsystem MsgAction received");
//                 nCmpntID = MENUID_AC;
//             }
//             std::string s = pbmsgAction.DebugString();
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, nCmpntID);
//         }
//         break;
//     default:
//         {
//             CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, nCmpntID);
//         }
//         break;
//     }
}

void CNetComm::RecievingResponseEx(string& sPbmsg
                                 , const HPR_INT32 nSrcID
                                 , const HPR_UINT32 nSeq
                                 , const HPR_UINT32 nCommand)
{
    {   //检查是否在等待信息列表中
        CGuard guard(&m_lckSyncRspContext);
        vector<SYNCRSP_CONTEXT>::iterator itor = m_vSyncRspContext.begin();
        for (;itor != m_vSyncRspContext.end(); itor++)
        {
            if (nSeq == itor->nSeq
                && nSrcID == itor->nConnSessionID)
            {
                itor->nCmdRsp = nCommand;
                itor->psMsg->assign(sPbmsg);
                itor->bGet = TRUE;
                m_poolEvent.SetSingleEvent(itor->hEvent);
                return;
            }
        }
    }

    {   //异步消息列表检查
        CGuard guard(&m_lckAsynRspContext);
        vector<ASYNRSP_CONTEXT>::iterator itAsynor = m_vAsynRspContext.begin();
        for (;itAsynor != m_vAsynRspContext.end(); itAsynor++)
        {
#pragma message("======== TODO: Check ConnSessionId")
            if (itAsynor->nSeq == (int)nSeq)
            {
                const int nComponent = itAsynor->nComponentID;
                m_vAsynRspContext.erase(itAsynor);
               // CModuleDllMgr::SnglPtr()->OnMsgTransformEx(sPbmsg, nCommand, nComponent);
                return;
            }
        }
    }

    //消息无法识别，可能是超时消息
    COMM_ERROR("Response not Matched, possibly a timeout msg(%d,%u,%u)"
        , nSrcID, nSeq, nCommand);
}

// 如果是多个联动服务就会有问题
BOOL CNetComm::AddSessionId(int nCmdID, int nSessionId)
{
//     if (cms_8100::CMD_MSG_ACTION != nCmdID)
//     {
//         return FALSE;
//     }

    map<int, int>::iterator it = m_mapCmdSessionId.find(nCmdID);
    if (it == m_mapCmdSessionId.end())
    {
        m_mapCmdSessionId[nCmdID] = nSessionId;
        return TRUE;
    }

    return FALSE;
}