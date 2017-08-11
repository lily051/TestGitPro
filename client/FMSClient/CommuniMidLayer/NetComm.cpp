#include "stdafx.h"
#include "NetComm.h"
#include <Shlwapi.h>
#include "./ConnSessionMgr.h"
#include "./PbWrapRelative.h"
//#include <msg/AlarmClientSync.pb.h>


const TCHAR* CLIENT_CONFIG_FILE = _T("CentralClient.ini");

/** @fn GetComponentID
*   @brief ��ȡĿ�����ID
*   @param[in] nCmdID: ��Ϣ������
*   @param NULL
*   @return ���ID
*/
int CNetComm::GetComponentID(int nCmdID)
{
	int nComId = MENUID_INVALID;
// 	switch (nCmdID)
// 	{
// 	case cms_8100::CMD_RSP_CLIENT_LOGIN:    //��¼
// 		{
// 			nComId = 0;
// 		}
// 		break;
// 	case cms_8100::CMD_MSG_ACTION:          //�յ��Ӵ�����Ϣ
//     case cms_8100::CMD_MSG_ALARM_CLIENTSYNC: // �յ�ͬ����Ϣ
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
*   @brief ��ʱ��Ϣ����߳�
*   @param[in] lpvoid: �Զ������
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
//     //��ʱ�������еĳ�ʱ��Ϣ
//     vector<ASYNRSP_CONTEXT>::iterator itTimeout = vTimeoutMsg.begin();
//     while (itTimeout != vTimeoutMsg.end())
//     {
//         CModuleDllMgr::SnglPtr()->OnRspTimeoutNotify(itTimeout->nComponentID, itTimeout->nCmdID);
//         itTimeout++;
//     }
}

//////////////////////////////////////////////////////////////////////////
// CNetComm

SINGLETON_IMPLEMENT(CNetComm)///<�������ʵ��

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
*   @brief �����ʼ��
*   @param[in] pMsgNeedRsp: �ص�����
*   @param[in] lpvoid: �û�����
*   @return NULL
*/
BOOL CNetComm::Init()
{
    TCHAR szDir[MAX_PATH] = {0};
    GetModuleFileName(NULL, szDir, MAX_PATH);
    PathRemoveFileSpec(szDir);
    TCHAR szPath[MAX_PATH] = {0};
    PathCombine(szPath, szDir, CLIENT_CONFIG_FILE);

    // ��Ϣ��ʱʱ��
    m_nMsgTimeout = GetPrivateProfileInt(_T("CentralClient"), _T("MsgTimeout"), 30, szPath);
    if (m_nMsgTimeout < 5
        || m_nMsgTimeout > 60)
    {
        m_nMsgTimeout = 30;
    }

    //�¼��س�ʼ��
	m_poolEvent.SetEventNum(20);
	m_poolEvent.SetEventTime(m_nMsgTimeout+5);
	m_poolEvent.InitAllEvent();

	//��ʱ��Ϣ����߳�
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

    //��Ϣ�齨
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

    //���ദ��
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

            {   // ����Ϣ�ŵ��ȴ�������
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

            // ��Ϣ����
            const HPR_INT32 nRet = ConnSession::SendMessage_r(pWrap, nConnSessionId);

            if (HPR_OK != nRet)
            {
                COMM_ERROR("SendMessage failed(%d,%u,%u)", nConnSessionId, nSeq, nCmdID);
                delete pWrap;
                pWrap = NULL;
            }else
            {
                COMM_TRACE("SendMessage (%d,%u,%u)", nConnSessionId, nSeq, nCmdID);
                // ���ͳɹ����ȴ�������Ϣ�����HPP_TIMEOUT��
                WaitForSingleObject(hEvent,  dwTimeout);
            }

            BOOL bRspGot = FALSE;   // �Ƿ��յ�������Ϣ
            {
                // ע�⣺��������Ϣ�����ں�������ǰ�Ƴ�����Ϊһ�����أ�
                // SYNCRSP_CONTEXT.pMsg����Ϊ��Чָ�롣֮���յ��÷���
                // ��Ϣʱ���ܻ���ʸ���Чָ��
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
                            // ��ʱ��δ�ȵ�������Ϣ��δ���ͳɹ�
	                           m_poolEvent.SetSingleEvent(itor->hEvent);
                        }else
                        {
                            // ��ȡ������Ϣ������
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

            // <stAsynRsp�����첽�ȴ�����>������<�յ�������Ϣ>ǰ
            // <������Ϣ>һ��������<��Ϣ����>�ɹ���
            // ���ԣ�<stAsynRsp�����첽�ȴ�����>�������<��Ϣ����>ǰ
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
//                 if (HPR_OK != ConnSession::SendMessage_r(pWrap, nConnSessionId)) // ���﷢��һ��ȱ�ݣ����pWrapTmp == NULL���ͻ����
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
*   @brief �������ķ�����
*   @param[in] stServerInfo
*   @param NULL
*   @return TRUE�ɹ���FALSEʧ��
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
//             // ������Ƶ�֪ͨ��ϢȺ����Ԥ��,�طźͽӾ�
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
//             // ��ͼ����������ͼ���
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
    {   //����Ƿ��ڵȴ���Ϣ�б���
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

    {   //�첽��Ϣ�б���
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

    //��Ϣ�޷�ʶ�𣬿����ǳ�ʱ��Ϣ
    COMM_ERROR("Response not Matched, possibly a timeout msg(%d,%u,%u)"
        , nSrcID, nSeq, nCommand);
}

// ����Ƕ����������ͻ�������
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