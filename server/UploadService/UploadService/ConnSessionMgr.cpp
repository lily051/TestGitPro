/**	@file ClientConnSessionMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief ���ӻỰ����
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#include "StdAfx.h"
#include "ConnSessionMgr.h"
#include "../util/Guard.h"
#include <process.h>
#include "MsgLayer.h"

#include "center//Records.pb.h"
#include "center/Prosaverecord.pb.h"
#include "center/StorageConfig.pb.h"
#include "center/Collectconfig.pb.h"
#include "center/Server.pb.h"
#include "server/ReqServerData.pb.h"
#include "server/RspServerData.pb.h"
#include "verify/ReqServerLogin.pb.h"
#include "verify/RspServerLogin.pb.h"
#include "verify/MsgServerLogout.pb.h"

//////////////////////////////////////////////////////////////////////////
//<�������Ľ�������
WrapIf* _CreateMessageByCmdId(HPR_INT32 cmdId)
{
    ULSERVER_LOG_INFO("CreateMessageByCmdId ...cmdID:%d...",cmdId);
    WrapIf* p_wrap = NULL;	
    switch (cmdId)
    {
    case cms_8120::CMD_RSP_SERVER_LOGIN:
        {
            p_wrap = PBWrapFactory<cms_8120::RspServerLogin>::CreatePBWrap();
        }
        break;
    case cms_8120::CMD_DT_RECORDS_LIST:
        {
            p_wrap = PBWrapFactory<cms_8120::RecordsList>::CreatePBWrap();
        }
        break;
	case cms_8120::CMD_DT_RECORDS_LIST_EX:
		{
			p_wrap = PBWrapFactory<cms_8120::RecordsList>::CreatePBWrap();
		}
		break;
    case cms_8120::CMD_MDT_RECORDS_STATUS:
        {
            p_wrap = PBWrapFactory<cms_8120::RspServerData>::CreatePBWrap();
        }
        break;
    case cms_8120::CMD_DT_STORAGE_INFO:
        {
            p_wrap = PBWrapFactory<cms_8120::RspStorageConfig>::CreatePBWrap();
        }
        break;
    default:
        p_wrap = NULL;
    }
    return p_wrap;
}

/**	@fn	    _OnConnectionComplete_Client
*	@brief	������ɻص�����
*	@param  [in] p_conn_session_,�Ựָ��
*	@return	
*/
HPR_INT32 _OnConnectionComplete_Client(ConnSession* p_conn_session_)
{
    ULSERVER_LOG_INFO("_OnConnectionComplete_Client...");

    CConnSessionMgr::GetSingletonPtr()->AddConnSession(p_conn_session_);

    return 0;
}


/**	@fn	    _OnConnectionClosed_Client
*	@brief	���ӹر�
*	@param  [in] p_conn_session_,�Ựָ��
*	@return	
*/
HPR_INT32 _OnConnectionClosed_Client(ConnSession* p_conn_session_)
{
    ULSERVER_LOG_INFO("%s...in",__FUNCTION__);

    CConnSessionMgr::GetSingletonPtr()->CloseConnSession(p_conn_session_);

    ULSERVER_LOG_INFO("%s...out",__FUNCTION__);

    return 0;
}

/**	@fn	    _OnCheckTimeout
*	@brief	��ʱ���
*	@param  [in] p_conn_session_,�Ựָ��
*	@return	
*/
HPR_BOOL _OnCheckTimeout_Client(ConnSession* pConnSession)
{
    return HPR_FALSE;
}

/** @fn OnConnectionError_Client(HPR_VOIDPTR p_user_data_);
*   @brief �ͻ������ӻὨ������ص�����
*	@param p_user_data_ �û��Զ�������
*   @return CString
*/
HPR_INT32 _OnConnectionError_Client(HPR_VOIDPTR p_user_data_)
{
    ULSERVER_LOG_INFO("_OnConnectionError_Client...");
    return 0;
}

HPR_BOOL _OnCheckTimeout(ConnSession* pConnSession)
{
    return HPR_FALSE;
}

HPR_INT32 _OnSocketClosed(ConnSession* pConnSession)
{
    ULSERVER_LOG_INFO("_OnSocketClosed...in");

    if (pConnSession)
    {
        std::string sClientIP = pConnSession->GetSockRemoteIp();
        ULSERVER_LOG_INFO("Client ip-%s, closed.", sClientIP.c_str());
        pConnSession->StopService();
    }

    ULSERVER_LOG_INFO("_OnSocketClosed...out");
    return HPR_OK;
}

//���յ����ӵĻص�����
HPR_INT32 _OnAcceptClient(ConnSession* pConnSession)
{
    ULSERVER_LOG_INFO("_OnAcceptClient...in");
    //���ûص�����������
    if (pConnSession)
    {
        std::string sClientIP = pConnSession->GetSockRemoteIp();
        pConnSession->SetServiceType(cms_8120::MT_SERVER_TRANSPORT);
        pConnSession->SetSocketClosedCallBack(_OnSocketClosed);
        pConnSession->SetTimeOutCallBack(_OnCheckTimeout);
        if(pConnSession->StartService(SERVICE_NORMAL_PRI) != HPR_OK)
        {
            ULSERVER_LOG_ERROR("Client IP-%s,StartService fail!",sClientIP.c_str());
        }
        else
        {
            ULSERVER_LOG_INFO("Client IP-%s, StartService succ!.", sClientIP.c_str());
        }
    }

    ULSERVER_LOG_INFO("_OnAcceptClient...out");
    return HPR_OK;
}

//////////////////////////////////////////////////////////////////////////

SINGLETON_IMPLEMENT(CConnSessionMgr)///<�������ʵ��

CConnSessionMgr::CConnSessionMgr(void)
:m_bInit(false)
,m_bStart(false)
,m_hHppHandle(NULL)
,m_hReconnectThread(NULL)
,m_bStopWorkerThread(true)
,m_hServerHandle(NULL)
{
    InitializeCriticalSection(&m_csConnSesssionLock);
}

CConnSessionMgr::~CConnSessionMgr(void)
{
    try
    {
        Fini();
        DeleteCriticalSection(&m_csConnSesssionLock);

        if (m_hReconnectThread)
        {
            CloseHandle(m_hReconnectThread);
            m_hReconnectThread = NULL;
        }

        if (m_hServerHandle)
        {
            HPP_StopLocalServer(m_hHppHandle,m_hServerHandle);
            m_hServerHandle = NULL;
        }

        if (m_hHppHandle)
        {
            HPP_Close(m_hHppHandle);
            m_hHppHandle = NULL;
        }
    }
    catch (...)
    {
    }
}
/**	@fn	    FormatKey
*	@brief	�γ�����
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	std::string ����
*/
std::string CConnSessionMgr::FormatKey(const std::string & ip_, int port_)
{
    char buff[256] = {0};
    sprintf_s(buff,256,"%s_%d",ip_.c_str(), port_);
    return std::string(buff);
}

/**	@fn	    Init
*	@brief	��ʼ������
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Init()
{
    ULSERVER_LOG_INFO("CConnSessionMgr::Init ...in");
    if (m_bInit)
    {
        ULSERVER_LOG_ERROR("Has Already Init");
        return HPR_ERROR;
    }

    //��ʼ��HPP��
    if (NULL == m_hHppHandle)
    {
        //��ʼ��HPP��,����(�����߳�,���������ĳ�ʱʱ��)
        m_hHppHandle = HPP_Init(HPP_WORKER_THREAD, HPP_TIMEOUT);  
        if (NULL == m_hHppHandle)
        {
            return HPR_ERROR;
        }
    }

    //����HPP(Protocol BufferЭ��)�������ɻص�����,
    HPP_SetPbMsgCreateFun(m_hHppHandle,_CreateMessageByCmdId);

    m_bInit = true;

    ULSERVER_LOG_INFO("CConnSessionMgr::Init ...out");
    return HPR_OK;
}
/**	@fn	    Fini
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Fini()
{
    ULSERVER_LOG_INFO("CConnSessionMgr::Init ...in");
    if (!m_bInit)
    {
        ULSERVER_LOG_ERROR("Has no Init");
        return HPR_ERROR;
    }

    Stop();

    //�ͷ�HPP��
    if (m_hHppHandle)
    {
        HPP_Close(m_hHppHandle);
        m_hHppHandle = NULL;
    }

    m_bInit = false;

    ULSERVER_LOG_INFO("CConnSessionMgr::Fini ...out");
    return HPR_OK;
}

/**	@fn	    _ReconnectThreadFun
*	@brief	��ȡ�����̺߳���
*	@param  [in] pArgumentsָ��CCmsClient����
*	@return	
*/
unsigned __stdcall _ReconnectThreadFun( void* pArguments)
{
    CConnSessionMgr::GetSingletonPtr()->RunCycleReconnect();
    _endthreadex(0);
    return 0;
}

/**	@fn	    Start
*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Start()
{
    ULSERVER_LOG_INFO("CConnSessionMgr::Start ...in");
    if (!m_bInit || m_bStart)
    {
        ULSERVER_LOG_ERROR("Has no Init or Already Start");
        return HPR_ERROR;
    }

    //���������߳�
    m_bStopWorkerThread = false;
    m_bStart = true;

    if (NULL == m_hReconnectThread)
    {
        unsigned threadID;
        m_hReconnectThread = (HANDLE)_beginthreadex( NULL, 0, &_ReconnectThreadFun, this, 0, &threadID );
    }

    ULSERVER_LOG_INFO("CConnSessionMgr::Start ...out");

    return HPR_OK;
}

/**	@fn	    Stop
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Stop()
{
    ULSERVER_LOG_INFO("CConnSessionMgr::Stop ...in");
    if (!m_bInit || !m_bStart)
    {
        ULSERVER_LOG_ERROR("Already Stop or has no Init");
        return HPR_ERROR;
    }

    m_bStopWorkerThread = true;
    if (m_hReconnectThread)
    {
        WaitForSingleObject(m_hReconnectThread,INFINITE);
        CloseHandle(m_hReconnectThread);
        m_hReconnectThread = NULL;
    }

    ClearConnect();

    m_bStart = false;

    ULSERVER_LOG_INFO("CConnSessionMgr::Stop ...out");
    return HPR_OK;
}

/**	@fn	    Connect
*	@brief	����
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Connect(const std::string & ip_, int port_)
{
    if (!m_bInit || !m_bStart || NULL == m_hHppHandle)
    {
        return HPR_ERROR;
    }

    std::string sKey = FormatKey(ip_,port_);

    do 
    {
        CGuard guard(&m_csConnSesssionLock);
        if (m_mapConnSessionInfo.find(sKey) != m_mapConnSessionInfo.end())
        {
            ULSERVER_LOG_ERROR("Key:%s Already Exist!",sKey.c_str());
            return HPR_ERROR;
        }

        CConnSessionInfo connSessiongInfo ;
        connSessiongInfo.m_sIP = ip_;
        connSessiongInfo.m_nPort = port_;
        connSessiongInfo.m_StartConnectTime = time(NULL);
        connSessiongInfo.m_pConnSession = NULL;
        m_mapConnSessionInfo[sKey] = connSessiongInfo;
    } while (0);

    //��һ������
    //����ģʽ
    //int nConnectResult= HPP_ConnRemoteService(m_hHppHandle,
    //    ip_.c_str(),(HPR_UINT16)port_, 
    //    _OnConnectionComplete_Client,NULL,0,
    //    ConnSession::PROTOCOL_TYPE_HPP, NULL,SERVICE_NORMAL_PRI);

    ////������ģʽ
    int nConnectResult = HPP_ConnRemoteServiceNoBlock(m_hHppHandle
        , ip_.c_str(), (HPR_UINT16)port_, _OnConnectionComplete_Client
        , NULL, 0, NULL, SERVICE_NORMAL_PRI, _OnConnectionError_Client);

    ULSERVER_LOG_INFO("Connect %s......Result-%d......",sKey.c_str(),nConnectResult);

    return HPR_OK;
}

/**	@fn	    Disconnect
*	@brief	�Ͽ�����
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::Disconnect(const std::string & ip_, int port_)
{
    if (!m_bInit || !m_bStart)
    {
        return HPR_ERROR;
    }

    std::string sKey = FormatKey(ip_,port_);

    CGuard guard(&m_csConnSesssionLock);
    ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
    if (itFind != m_mapConnSessionInfo.end())
    {
        if (itFind->second.m_pConnSession)
        {
            itFind->second.m_pConnSession->StopService();
            itFind->second.m_pConnSession = NULL;
        }
        m_mapConnSessionInfo.erase(itFind);
        ULSERVER_LOG_INFO("Disconnect %s......",sKey.c_str());
    }

    return HPR_OK;
}

/**	@fn	    IsConnect
*	@brief	�ж��Ƿ�������
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	true���ӣ�falseû������
*/
bool CConnSessionMgr::IsConnect(const std::string & ip_, int port_)
{
    if (!m_bInit || !m_bStart)
    {
        return false;
    }

    std::string sKey = FormatKey(ip_,port_);

    CGuard guard(&m_csConnSesssionLock);
    ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
    if (itFind != m_mapConnSessionInfo.end())
    {
        if (itFind->second.m_pConnSession)
        {
            return true;
        }
    }

    return false;
}


/**	@fn	    ClearConnect
*	@brief	������е�������Ϣ
*	@param  [in] ��
*	@param  [out] ��
*	@return	std::string ����
*/
void CConnSessionMgr::ClearConnect()
{
    CGuard guard(&m_csConnSesssionLock);
    for (ConnSessionInfoIterator it = m_mapConnSessionInfo.begin()
        ; it != m_mapConnSessionInfo.end(); it++)
    {
        if (it->second.m_pConnSession)
        {
            it->second.m_pConnSession->StopService();
            it->second.m_pConnSession = NULL;
        }
    }
    m_mapConnSessionInfo.clear();
}

/**	@fn	    AddConnSession
*	@brief	��ӻỰSession
*	@param  [in] pConnSession_ 
*	@param  [out] ��
*	@return	std::string ����
*/
void CConnSessionMgr::AddConnSession(ConnSession * pConnSession_)
{
    if (NULL == pConnSession_)
    {
        return;
    }
    std::string ip = pConnSession_->GetSockRemoteIp();
    int nPort = pConnSession_->GetSockRemotePort();

    std::string sKey = FormatKey(ip,nPort);

    CGuard guard(&m_csConnSesssionLock);
    ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
    if (itFind != m_mapConnSessionInfo.end())
    {
        pConnSession_->SetSocketClosedCallBack(_OnConnectionClosed_Client);
        if (HPR_OK != pConnSession_->StartService(SERVICE_NORMAL_PRI))
        {
            ULSERVER_LOG_INFO("Key:%s StartService fail",sKey.c_str());
        }
        else
        {
            if (itFind->second.m_pConnSession)
            {
                itFind->second.m_pConnSession->StopService();
                itFind->second.m_pConnSession = NULL;
            }
            itFind->second.m_pConnSession = pConnSession_;
            ULSERVER_LOG_INFO("Key:%s StartService succ!",sKey.c_str());
        }
    }
}

/**	@fn	    CloseConnSession
*	@brief	�رջỰ
*	@param  [in] pConnSession_ 
*	@param  [out] ��
*	@return	std::string ����
*/
void CConnSessionMgr::CloseConnSession(ConnSession * pConnSession_)
{
    if (!m_bInit || !m_bStart)
    {
        return;
    }

    std::string ip = pConnSession_->GetSockRemoteIp();
    int nPort = pConnSession_->GetSockRemotePort();
    std::string sKey = FormatKey(ip,nPort);
    CGuard guard(&m_csConnSesssionLock);
    ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
    if (itFind != m_mapConnSessionInfo.end())
    {
        if (itFind->second.m_pConnSession)
        {
            itFind->second.m_pConnSession->StopService();
            itFind->second.m_pConnSession = NULL;
            ULSERVER_LOG_INFO("Key:%s StopService!",sKey.c_str());
        }
    }
}
/**	@fn	    RunCycleReconnect
*	@brief	�����̺߳���
*	@param  [in]  
*	@param  [out] ��
*	@return	
*/
void CConnSessionMgr::RunCycleReconnect()
{
    ULSERVER_LOG_INFO("RunCycleReconnect......in");
    while(!m_bStopWorkerThread)
    {
        for (int i=0;i<RECONNECT_OVER_TIME*2 && !m_bStopWorkerThread;++i)
        {
            Sleep(500);
        }

        do 
        {
            CGuard guard(&m_csConnSesssionLock);
            for (ConnSessionInfoIterator it = m_mapConnSessionInfo.begin()
                ; it != m_mapConnSessionInfo.end() && !m_bStopWorkerThread; ++it)
            {
                if (it->second.m_pConnSession == NULL
                    && time(NULL)-it->second.m_StartConnectTime > RECONNECT_OVER_TIME)
                {
                    ULSERVER_LOG_INFO("Reconnect %s,%d......",it->second.m_sIP.c_str(),it->second.m_nPort);
                    it->second.m_StartConnectTime = time(NULL);
                    HPP_ConnRemoteServiceNoBlock(m_hHppHandle
                        , it->second.m_sIP.c_str(), (HPR_UINT16)it->second.m_nPort, _OnConnectionComplete_Client
                        , NULL, 0, NULL, SERVICE_NORMAL_PRI, _OnConnectionError_Client);
                }
            }
        } while (0);
    }
    ULSERVER_LOG_INFO("RunCycleReconnect......out");
}
/**	@fn	    SendMsg
*	@brief	������Ϣ
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::SendMsg(const std::string & ip_,int port_, cms_8120::CMD cmd_, void * pMsg)
{
    if (!m_bInit || !m_bStart || NULL == m_hHppHandle)
    {
        return HPR_ERROR;
    }

    std::string sKey = FormatKey(ip_,port_);

    CConnSessionInfo connSessionInfo;

    do 
    {
        CGuard guard(&m_csConnSesssionLock);
        ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
        if (itFind != m_mapConnSessionInfo.end())
        {
            connSessionInfo = itFind->second; 
            if (NULL == connSessionInfo.m_pConnSession)
            {
                ULSERVER_LOG_ERROR("Key:%s not connect!",sKey.c_str());
                return HPR_ERROR;
            }
        }
        else
        {
            ULSERVER_LOG_ERROR("Key:%s not exist!",sKey.c_str());
            return HPR_ERROR;
        }
    } while (0);

    ConnSession* pConnSession = connSessionInfo.m_pConnSession;
    if (NULL == pConnSession)
    {
        return HPR_ERROR;
    }

    //��������
    switch(cmd_)
    {
    case cms_8120::CMD_REQ_SERVER_LOGIN:    //��¼��Ϣ
        {
            PBWrap<cms_8120::ReqServerLogin>* p_msg_wrap = PBWrapFactory<cms_8120::ReqServerLogin>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap fail ...");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(cms_8120::CMD_REQ_SERVER_LOGIN);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NEED_RSP);//��Ҫ��Ӧ
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            cms_8120::ReqServerLogin& r_req_msg = p_msg_wrap->GetMsg();
            r_req_msg = *(cms_8120::ReqServerLogin*)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r fail ...");
                PBWrapFactory<cms_8120::ReqServerLogin>::DestroyPBWrap(p_msg_wrap);
                return HPR_ERROR;
            }
        }
        break;
    case cms_8120::CMD_MSG_SERVER_LOGOUT:    //�ǳ���Ϣ
        {
            PBWrap<cms_8120::MsgServerLogout>* p_msg_wrap = PBWrapFactory<cms_8120::MsgServerLogout>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap fail ...");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(cms_8120::CMD_MSG_SERVER_LOGOUT);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);//����Ҫ��Ӧ
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            cms_8120::MsgServerLogout& r_req_msg = p_msg_wrap->GetMsg();
            r_req_msg = *(cms_8120::MsgServerLogout*)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r fail ...");
                PBWrapFactory<cms_8120::MsgServerLogout>::DestroyPBWrap(p_msg_wrap);
                return HPR_ERROR;
            }
        }
        break;  
    case cms_8120::CMD_MDT_SERVER_STATUS:    //������Ϣ
        {
            PBWrap<cms_8120::ReqServer>* p_msg_wrap = PBWrapFactory<cms_8120::ReqServer>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap fail ...");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(cms_8120::CMD_MDT_SERVER_STATUS);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);//��Ҫ��Ӧ
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            cms_8120::ReqServer& r_req_msg = p_msg_wrap->GetMsg();
            r_req_msg = *(cms_8120::ReqServer*)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r fail ...");
                PBWrapFactory<cms_8120::ReqServer>::DestroyPBWrap(p_msg_wrap);
                return HPR_ERROR;
            }
        }
        break;
    case cms_8120::CMD_DT_RECORDS_LIST://¼����Ϣ
	case cms_8120::CMD_DT_RECORDS_LIST_EX://��ȡ���ϴ�¼����Ϣ
    case cms_8120::CMD_MDT_RECORDS_STATUS://����¼��״̬
        {
            PBWrap<cms_8120::ReqRecords>* p_msg_wrap = PBWrapFactory<cms_8120::ReqRecords>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap fail ...");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(cmd_);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NEED_RSP);//��Ҫ��Ӧ
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            cms_8120::ReqRecords& r_req_msg = p_msg_wrap->GetMsg();
            r_req_msg = *(cms_8120::ReqRecords*)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r fail ...");
                PBWrapFactory<cms_8120::ReqRecords>::DestroyPBWrap(p_msg_wrap);
                return HPR_ERROR;
            }
        }
        break;
    case cms_8120::CMD_DT_STORAGE_INFO://�洢�豸��Ϣ
        {
            PBWrap<cms_8120::ReqStorageConfig>* p_msg_wrap = PBWrapFactory<cms_8120::ReqStorageConfig>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap fail ...");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(cms_8120::CMD_DT_STORAGE_INFO);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NEED_RSP);
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            cms_8120::ReqStorageConfig& r_msg = p_msg_wrap->GetMsg();
            r_msg = *(cms_8120::ReqStorageConfig*)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r fail ...");
                PBWrapFactory<cms_8120::ReqStorageConfig>::DestroyPBWrap(p_msg_wrap);
                return  HPR_ERROR;
            }
        }
        break;
    default:
        break;
    }

    //////////////////////////////////////////////////////////////////////////
    return HPR_OK;
}
/**	@fn	    StartListen
*	@brief	��������
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::StartListen(const std::string & ip_, int port_)
{
    ULSERVER_LOG_INFO("CConnSessionMgr::StartListen ...in");

    if (!m_bInit || !m_bStart || NULL == m_hHppHandle || NULL != m_hServerHandle)
    {
        ULSERVER_LOG_ERROR("param error!");
        return HPR_ERROR;
    }  

    m_hServerHandle = HPP_StartLocalServer(m_hHppHandle, ip_.c_str(), (HPR_UINT16)port_, _OnAcceptClient,ConnSession::PROTOCOL_TYPE_HPP);	

    if(NULL == m_hServerHandle)					//���ط��񴴽�ʧ��
    {
        ULSERVER_LOG_ERROR("HPP_StartLocalServer ip-%s,port-%d,fail! ",ip_.c_str(), port_);
        return HPR_ERROR;
    }
    else
    {
        ULSERVER_LOG_INFO("HPP_StartLocalServer ip-%s,port-%d,succ! ",ip_.c_str(), port_);
    }

    ULSERVER_LOG_INFO("CConnSessionMgr::StartListen ...out");
    return HPR_OK;
}

/**	@fn	    StopListen
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::StopListen()
{
    ULSERVER_LOG_INFO("CConnSessionMgr::StopListen ...in");

    if (!m_bInit || !m_bStart || NULL == m_hHppHandle || NULL == m_hServerHandle)
    {
        ULSERVER_LOG_ERROR("param error!");
        return HPR_ERROR;
    }

    HPP_StopLocalServer(m_hHppHandle, m_hServerHandle);

    m_hServerHandle = NULL;

    ULSERVER_LOG_INFO("CConnSessionMgr::StopListen ...out");
    return HPR_OK;
}

/**	@fn	    SendAlarmMsg
*	@brief	���ͱ�����Ϣ
*	@param  [in] ip_ ip
*	@param  [in] port_ �˿�
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CConnSessionMgr::SendAlarmMsg(const std::string & ip_, int port_, AlarmMsg::AlarmId cmd_, void * pMsg)
{
    if (!m_bInit || !m_bStart || NULL == m_hHppHandle)
    {
        return HPR_ERROR;
    }

    std::string sKey = FormatKey(ip_,port_);

    CConnSessionInfo connSessionInfo;

    do 
    {
        CGuard guard(&m_csConnSesssionLock);
        ConnSessionInfoIterator itFind = m_mapConnSessionInfo.find(sKey);
        if (itFind != m_mapConnSessionInfo.end())
        {
            connSessionInfo = itFind->second; 
            if (NULL == connSessionInfo.m_pConnSession)
            {
                ULSERVER_LOG_ERROR("Key:%s not connect!",sKey.c_str());
                return HPR_ERROR;
            }
        }
        else
        {
            ULSERVER_LOG_ERROR("Key:%s not exist!",sKey.c_str());
            return HPR_ERROR;
        }
    } while (0);

    ConnSession* pConnSession = connSessionInfo.m_pConnSession;
    if (NULL == pConnSession)
    {
        return HPR_ERROR;
    }
    //��������
    switch(cmd_)
    {
    case AlarmMsg::AUTHORIZE_REQ://������Ϣ
        {
            PBWrap<AlarmMsg::AuthorizeReq>* p_msg_wrap = PBWrapFactory<AlarmMsg::AuthorizeReq>::CreatePBWrap();
            if (p_msg_wrap == NULL)
            {
                ULSERVER_LOG_ERROR("CreatePBWrap Fail........");
                return HPR_ERROR;
            }
            //����HPPЭ��ͷ
            p_msg_wrap->SetCommandId(AlarmMsg::AUTHORIZE_REQ);
            p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
            p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);
            p_msg_wrap->SetWrapDstId(pConnSession->GetSessionId());
            //������Ϣ����
            AlarmMsg::AuthorizeReq &r_msg = p_msg_wrap->GetMsg();
            r_msg = *(AlarmMsg::AuthorizeReq *)pMsg;

            if (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, pConnSession->GetSessionId()))
            {
                ULSERVER_LOG_ERROR("SendMessage_r Fail ... .");
                PBWrapFactory<AlarmMsg::AuthorizeReq>::DestroyPBWrap(p_msg_wrap);
                return HPR_ERROR;
            }
        }
        break;
    }
    return HPR_OK;
}

