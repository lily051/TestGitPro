#include "stdafx.h"
#include "ConnSessionMgr.h"
#include <Shlwapi.h>
#include "PbWrapRelative.h"

//////////////////////////////////////////////////////////////////////////
// ConnSessionMgr

SINGLETON_IMPLEMENT(ConnSessionMgr)///<单例相关实现

ConnSessionMgr::ConnSessionMgr(void)
: m_hHppInit(NULL)
, m_hHppSvr(NULL)
, m_hEventConnect(NULL)
, m_nLastSessionId(-1)
, m_nPort(27200)
, m_nHppThreadNum(2)
, m_nHppContextTimeout(30)
, m_nConnectTimeout(3)
{
    InitializeCriticalSection(&m_lckSpecifiedConns);
    InitializeCriticalSection(&m_lckConns);
    InitializeCriticalSection(&m_lckIncomingConns);
    InitializeCriticalSection(&m_lckConnect);
    m_hEventConnect = CreateEvent(NULL, TRUE, FALSE, NULL);
}

ConnSessionMgr::~ConnSessionMgr(void)
{
    try
    {
        DeleteCriticalSection(&m_lckSpecifiedConns);
        DeleteCriticalSection(&m_lckConns);
        DeleteCriticalSection(&m_lckIncomingConns);
        DeleteCriticalSection(&m_lckConnect);
        CloseHandle(m_hEventConnect);
        m_hHppInit = NULL;
        m_hHppSvr = NULL;
        m_hEventConnect = NULL;
    }
    catch (...)
    {
    }
}

string ConnSessionMgr::FormatKey(const string & sIP, int nPort)
{
    ostringstream stream;
    stream << "," << nPort;
    return sIP + stream.str();
}

void ConnSessionMgr::PrintHppConns(void)
{
    {
        map<int, string> mpSpecifiedConns;
        {
            CGuard guard(&m_lckSpecifiedConns);
            mpSpecifiedConns = m_mpSpecifiedConns;
        }
        ostringstream stream;
        map<int, string>::iterator it = mpSpecifiedConns.begin();
        for (; it != mpSpecifiedConns.end(); it++)
        {
            stream << std::endl << it->first << " <=> "
                << it->second.c_str();
        }
        COMM_INFO("Specified ConnSessions:%s", stream.str().c_str());
    }

    {
        map<string, ConnInfo> mpTempConns;
        {
            CGuard guard(&m_lckConns);
            mpTempConns = m_mpConns;
        }
        ostringstream stream;
        map<string, ConnInfo>::iterator it = mpTempConns.begin();
        for (; it != mpTempConns.end(); it++)
        {
            stream << std::endl << "HppConn: " << it->first.c_str() << " <=> ";
            ConnSession* pConn = HPP_GetHppSessionById(it->second.nSessionId);
            if (pConn)
            {
                const char *pRemoteIp = pConn->GetSockRemoteIp();
                string sRemoteIp = (pRemoteIp == NULL ? "null" : pRemoteIp);
                stream << it->second.nSessionId << " " << sRemoteIp.c_str() << "," << pConn->GetSockRemotePort();
            }else
            {
                stream << it->second.nSessionId << " Can't be found";
            }
        }
        COMM_INFO("Temporary ConnSessions:%s", stream.str().c_str());
    }

    {
        map<HPR_INT32, string> mpInConns;
        {
            CGuard guard(&m_lckIncomingConns);
            mpInConns = m_mpIncomingConns;
        }
        ostringstream stream;
        map<HPR_INT32, string>::iterator it = mpInConns.begin();
        for (; it != mpInConns.end(); it++)
        {
            stream << std::endl << "HppConn: " << it->first << " <=> " << it->second;
        }
        COMM_INFO("Incoming Conns:%s", stream.str().c_str());
    }
}

/** @fn GetConnSessionID
*   @brief 获取指定服务器的hpp SessionId值
*   @param[in] stSvrInfo: 服务器信息
*   @param NULL
*   @return id值，-1表示获取失败
*/
int ConnSessionMgr::GetConnSessionID(SERVER_INFO& stSvrInfo)
{
    string sKey;    // "<ip>,<port>"
    if (stSvrInfo.nServerID > 0) // Specified ConnSession
    {
        CGuard guard(&m_lckSpecifiedConns);
        // Find in SpecifiedConns
        map<int, string>::iterator it
            = m_mpSpecifiedConns.find(stSvrInfo.nServerID);
        if (it != m_mpSpecifiedConns.end())
        {
            std::string sIp = CP936_W2A(stSvrInfo.szServerIp);            
            if (sIp.empty() || sIp == "0.0.0.0")
            {
                sKey = it->second;
            }else
            {
                sKey = FormatKey(sIp, stSvrInfo.nServerPort);
                it->second = sKey;
            }
        }else
        {
            // Bookkeep in SpecifiedConns
            std::string sIp = CP936_W2A(stSvrInfo.szServerIp);
            sKey = FormatKey(sIp, stSvrInfo.nServerPort);
            m_mpSpecifiedConns[stSvrInfo.nServerID] = sKey;
        }
    }else
    {
        std::string sIp = CP936_W2A(stSvrInfo.szServerIp);
        sKey = FormatKey(sIp, stSvrInfo.nServerPort);
    }

    {
        CGuard guard(&m_lckConns);
        // Find in Conns
        map<string, ConnInfo>::const_iterator it = m_mpConns.find(sKey);
        if (it != m_mpConns.end())
        {
            if (it->second.bOnline)
            {
                return it->second.nSessionId;
            }else
            {
                return -1;
            }
        }
    }
    // Not found, establish in Conns
    ConnInfo stConnInfo;
    stConnInfo.stSvrInfo = stSvrInfo;
    Connect(stConnInfo, sKey);

    return stConnInfo.nSessionId;
}

BOOL ConnSessionMgr::Init(const int nPort)
{
    TCHAR szDir[MAX_PATH] = {0};
    GetModuleFileName(NULL, szDir, MAX_PATH);
    PathRemoveFileSpec(szDir);
    TCHAR szPath[MAX_PATH] = {0};
    //PathCombine(szPath, szDir, CLIENT_CONFIG_FILE);

    // 监听端口
    if (nPort < 1 || nPort > 65535)
    {
        m_nPort = 27200;
    }
    else
    {
        m_nPort = nPort;
    }

    // HPP线程数
    m_nHppThreadNum = GetPrivateProfileInt(_T("CentralClient"), _T("HppThreadNum"), 2, szPath);
    if (m_nHppThreadNum < 2
        || m_nHppThreadNum > 10)
    {
        m_nHppThreadNum = 2;
    }
    // HPP上下文超时时间
    m_nHppContextTimeout = GetPrivateProfileInt(_T("CentralClient"), _T("HppContextTimeout"), 30, szPath);
    if (m_nHppContextTimeout < 10
        || m_nHppContextTimeout > 90)
    {
        m_nHppContextTimeout = 30;
    }
    // HPP连接超时时间
    m_nConnectTimeout = GetPrivateProfileInt(_T("CentralClient"), _T("ConnectTimeout"), 5, szPath);
    if (m_nConnectTimeout < 5
        || m_nConnectTimeout > 30)
    {
        m_nConnectTimeout = 5;
    }

    if (m_hHppInit)
    {
        COMM_ERROR("The hpp service has been initialized");
        return FALSE;
    }
    //初始化
    m_hHppInit = HPP_Init(m_nHppThreadNum, m_nHppContextTimeout);
    if (NULL == m_hHppInit)
    {
        COMM_ERROR("Init hpp service error");
        return FALSE;
    }

    //设置连接回调函数
    HPP_SetPbMsgCreateFun(m_hHppInit, CreateMsgByCmdIdEx);

    return TRUE;
}

BOOL ConnSessionMgr::Fini(void)
{
    if (m_hHppInit)
    {
        CLIENT_INFO("HPP_Close...");
        HPP_Close(m_hHppInit);
        m_hHppInit = NULL;
        COMM_INFO("hpp release");
    }

    return TRUE;
}


HPR_INT32 ConnSessionMgr::OnConnectFailed(HPR_VOIDPTR hpr_voidPtr)
{
    ConnSessionMgr::SnglPtr()->m_nLastSessionId = -1;
    //通知可以获取连接的id了
    COMM_ERROR("Connect failed, %d"
        , ConnSessionMgr::SnglPtr()->m_nLastSessionId);
    SetEvent(ConnSessionMgr::SnglPtr()->m_hEventConnect);

    return HPR_OK;
}

HPR_INT32 ConnSessionMgr::OnConnectSuccess(ConnSession* pConnSession)
{
    pConnSession->SetSocketClosedCallBack(OnServerSocketClosed);
    pConnSession->SetTimeOutCallBack(OnServerCheckTimeOut);
    if (HPR_OK != pConnSession->StartService(SERVICE_NORMAL_PRI))
    {
        return HPR_ERROR;
    }
    ConnSessionMgr::SnglPtr()->m_nLastSessionId = pConnSession->GetSessionId();

    const char *pRemoteIp = pConnSession->GetSockRemoteIp();
    string sRemoteIp = (pRemoteIp == NULL ? "null" : pRemoteIp);
    COMM_INFO("Connect to %s,%d succeed, %d"
        , sRemoteIp.c_str()
        , pConnSession->GetSockRemotePort()
        , ConnSessionMgr::SnglPtr()->m_nLastSessionId);

    SetEvent(ConnSessionMgr::SnglPtr()->m_hEventConnect);
    return HPR_OK;
}

BOOL ConnSessionMgr::StartListening(int nPort)
{
    if (m_hHppSvr)
    {
        COMM_ERROR("The hpp service has been started");
        return FALSE;
    }
    if (NULL == m_hHppInit)
    {
        COMM_ERROR("The hpp must be initialized first");
        return FALSE;
    }

    string sListenIp = "0.0.0.0";
    m_hHppSvr = HPP_StartLocalServer(m_hHppInit, sListenIp.c_str(), (HPR_UINT16)nPort, OnAcceptIncomingConn, ConnSession::PROTOCOL_TYPE_HPP);
    if (NULL == m_hHppSvr)
    {
        COMM_ERROR("HPP_StartLocalServer Failed. ip=%s, port=%d", sListenIp.c_str(), nPort);
        return FALSE;
    }
    COMM_INFO("HPP_StartLocalServer %d Succeeded", (LONG)m_hHppSvr);

    return TRUE;
}

void ConnSessionMgr::StopListening()
{
    if (m_hHppSvr)
    {
        COMM_INFO("The hpp service %d close", (LONG)m_hHppSvr);
        HPP_StopLocalServer(m_hHppInit, m_hHppSvr);
        m_hHppSvr = NULL;
    }
}

BOOL ConnSessionMgr::AddIncomingConn(ConnSession* pConnSession)
{
    CGuard guard(&m_lckIncomingConns);
    HPR_INT32 nSessionId = pConnSession->GetSessionId();
    map<HPR_INT32, string>::iterator it = m_mpIncomingConns.find(nSessionId);
    if (it == m_mpIncomingConns.end())
    {
        ostringstream stream;
        stream << "," << pConnSession->GetSockRemotePort();
        const char *pRemoteIp = pConnSession->GetSockRemoteIp();
        string sRemoteIp = (pRemoteIp == NULL ? "null" : pRemoteIp);
        m_mpIncomingConns[nSessionId] = sRemoteIp + stream.str();
    }else
    {
        COMM_WARN("ConnSession[%d] already exists!", nSessionId);
        return FALSE;
    }
    return TRUE;
}

BOOL ConnSessionMgr::RemoveIncomingConn(HPR_INT32 nSessionId)
{
    CGuard guard(&m_lckIncomingConns);
    map<HPR_INT32, string>::iterator it = m_mpIncomingConns.find(nSessionId);
    if (it != m_mpIncomingConns.end())
    {
        m_mpIncomingConns.erase(it);
        return TRUE;
    }else
    {
        COMM_WARN("ConnSession[%d] doesn't exist!", nSessionId);
    }
    return TRUE;
}

HPR_INT32 ConnSessionMgr::OnServerCheckTimeOut(ConnSession* pConnSession)
{
    return HPR_FALSE;
}

HPR_INT32 ConnSessionMgr::OnServerSocketClosed(ConnSession* pConnSession)
{
    ConnInfo stConnInfo;
    const char *pRemoteIp = pConnSession->GetSockRemoteIp();
    string sRemoteIp = (pRemoteIp == NULL ? "null" : pRemoteIp);
    wstring wsIp = CP936_A2W(sRemoteIp);
    wcscpy_s(stConnInfo.stSvrInfo.szServerIp, 16, wsIp.c_str());
    stConnInfo.stSvrInfo.nServerPort = pConnSession->GetSockRemotePort();
    stConnInfo.bOnline = FALSE;
    stConnInfo.nSessionId = -1;
    ConnSessionMgr::SnglPtr()->UpdateConnStatus(stConnInfo);
    string sKey = FormatKey(sRemoteIp, pConnSession->GetSockRemotePort());
    COMM_INFO("ConnSession[%d](%s) Closed and update",pConnSession->GetSessionId(),sKey.c_str());
    return HPR_OK;
}

HPR_INT32 ConnSessionMgr::OnAcceptIncomingConn(ConnSession* pConnSession)
{
    pConnSession->SetServiceType(0x1020);
    pConnSession->SetSocketClosedCallBack(OnIncomingConnClosed);
    pConnSession->SetTimeOutCallBack(OnIncomingConnTimeOut);
    if (pConnSession->StartService(SERVICE_NORMAL_PRI) != HPR_OK)
    {
        return HPR_ERROR;
    }

    // 记录连接
    ConnSessionMgr::SnglPtr()->AddIncomingConn(pConnSession);

    const char *pRemoteIp = pConnSession->GetSockRemoteIp();
    string sRemoteIp = (pRemoteIp == NULL ? "null" : pRemoteIp);
    COMM_INFO("Incoming Conn %d <=> %s,%d"
        , pConnSession->GetSessionId()
        , sRemoteIp.c_str()
        , pConnSession->GetSockRemotePort());
    return HPR_OK;
}

HPR_INT32 ConnSessionMgr::OnIncomingConnTimeOut(ConnSession* pConnSession)
{
    return HPR_FALSE;
}

HPR_INT32 ConnSessionMgr::OnIncomingConnClosed(ConnSession* pConnSession)
{
    // 删除记录
    HPR_INT32 nSessionId = pConnSession->GetSessionId();
    ConnSessionMgr::SnglPtr()->RemoveIncomingConn(nSessionId);
    COMM_INFO("Incoming Conn %d Closed", pConnSession->GetSessionId());
    return HPR_OK;
}

BOOL ConnSessionMgr::UpdateConnStatus(const ConnInfo& stConnInfo)
{
    string sIP = CP936_W2A(stConnInfo.stSvrInfo.szServerIp);
    string sKey = FormatKey(sIP, stConnInfo.stSvrInfo.nServerPort);
    CGuard guard(&m_lckConns);
    map<string, ConnInfo>::iterator it = m_mpConns.find(sKey);
    if (it != m_mpConns.end())
    {
        it->second = stConnInfo;
        COMM_INFO("ConnSession[%d](%s) Status updated(%d)"
            , stConnInfo.nSessionId
            , sKey.c_str()
            , stConnInfo.bOnline);
        return TRUE;
    }else
    {
        COMM_ERROR("ConnSession %s not found", sKey.c_str());
        return FALSE;
    }
}

void ConnSessionMgr::MaintainConnSessions(const BOOL *pAvailable)
{
    // Get ConnSessions state snapshot
    map<string, ConnInfo> mpConns;
    {
        CGuard guard(&m_lckConns);
        mpConns = m_mpConns;
    }

    map<string, ConnInfo>::iterator it = mpConns.begin();
    for (; it != mpConns.end(); it++)
    {
        if (FALSE == *pAvailable)
        {
            break;
        }

        if (it->second.bOnline)
        {
            continue;
        }

        if (Connect(it->second, "", TRUE))
        {
            UpdateConnStatus(it->second);
        }

        if (FALSE == *pAvailable)
        {
            break;
        }
    }
}

/**	@fn	    Connect
 *	@brief	与服务器建立HPP栈连接
 *	@param  [in][out]stConnInfo 服务器信息，连接结果
 *  @param  [in]strKey 连接键
 *  @param  [in]bUpdateConn 是否用于更新连接
 *	@return	连接成功与否
 */
BOOL ConnSessionMgr::Connect(ConnInfo & stConnInfo, const string &strKey, BOOL bUpdateConn/* = FALSE*/)
{
    string sIp = CP936_W2A(stConnInfo.stSvrInfo.szServerIp);
    CGuard guardConn(&m_lckConnect);
    if (!bUpdateConn)
    {
        // 再次检查连接
        {
            CGuard guard(&m_lckConns);
            map<string, ConnInfo>::const_iterator it = m_mpConns.find(strKey);
            if (it != m_mpConns.end())
            {
                stConnInfo = it->second;
                return it->second.bOnline;
            }
        }
    }
    ResetEvent(m_hEventConnect);
    COMM_TRACE("Connecting %s,%d!"
        , sIp.c_str()
        , stConnInfo.stSvrInfo.nServerPort);
    HPR_INT32 nResult =
        HPP_ConnRemoteServiceNoBlock(m_hHppInit
        , sIp.c_str()
        , (HPR_UINT16)stConnInfo.stSvrInfo.nServerPort
        , OnConnectSuccess
        , NULL, 0
        //, ConnSession::PROTOCOL_TYPE_HPP
        , NULL
        , SERVICE_NORMAL_PRI
        , OnConnectFailed);
    stConnInfo.bOnline = TRUE;
    if (nResult != HPR_OK)
    {
        stConnInfo.bOnline = FALSE;
        stConnInfo.nSessionId = -1;
        COMM_ERROR("Connect %s,%d error!"
            , sIp.c_str()
            , stConnInfo.stSvrInfo.nServerPort);
    }else
    {
        if (WAIT_OBJECT_0 != WaitForSingleObject(m_hEventConnect, (m_nConnectTimeout*1000+100)))
        {
            stConnInfo.bOnline = FALSE;
            stConnInfo.nSessionId = -1;
            COMM_ERROR("Connect %s,%d timeout!"
                , sIp.c_str()
                , stConnInfo.stSvrInfo.nServerPort);
        }else if (m_nLastSessionId == -1)
        {
            stConnInfo.bOnline = FALSE;
            stConnInfo.nSessionId = -1;
            COMM_ERROR("Connect %s,%d failed, %d"
                , sIp.c_str()
                , stConnInfo.stSvrInfo.nServerPort
                , m_nLastSessionId);
        }else
        {
            // Set SessionId ,no error occurred
            stConnInfo.bOnline = TRUE;
            stConnInfo.nSessionId = m_nLastSessionId;
            COMM_TRACE("Connect %s,%d Succeed!, %d"
                , sIp.c_str()
                , stConnInfo.stSvrInfo.nServerPort
                , m_nLastSessionId);
        }
    }

    if (!strKey.empty())
    {
        CGuard guard(&m_lckConns);
        m_mpConns[strKey] = stConnInfo;
    }

    return stConnInfo.bOnline;
}

BOOL ConnSessionMgr::StartOutgoing(void)
{
    if (NULL == m_hHppInit)
    {
        FMSCLIENT_ERROR("ConnSessionMgr not init'ed");
        return FALSE;
    }

    if (!RspDispatcher::SnglPtr()->Init())
    {
        FMSCLIENT_ERROR("RspDispatcher not init'ed");
        return FALSE;
    }

    BOOL bRet = m_TimerThread.Start();
    bRet &= m_TimerThread.SetTimer(TimerThread::CONN_PRINT_TIMER
                                  , TimerThread::CONN_PRINT_INTERVAL);
    bRet &= m_TimerThread.SetTimer(TimerThread::CONN_CHECK_TIMER
                                  , TimerThread::CONN_CHECK_INTERVAL);

    if (!bRet)
    {
        FMSCLIENT_ERROR("Start Reconnect Thread failed");
        m_TimerThread.Stop();
        RspDispatcher::SnglPtr()->Fini();
        return FALSE;
    }

    return TRUE;
}


BOOL ConnSessionMgr::StopOutgoing(void)
{
    if (NULL == m_hHppInit)
    {
        COMM_ERROR("ConnSessionMgr not init'ed");
        return FALSE;
    }

    CLIENT_INFO("Stop TimerThread...");
    m_TimerThread.Stop();

#pragma message("======== TODO: stop connecting request, sending messages")
    // 暂时要求StopOutgoing调用之前调用CModuleDllMgr::FlushPendingMsgAndShutdown();

    // 关闭并清空连接
    {
        CGuard guard(&m_lckConns);
        map<string, ConnInfo>::iterator it = m_mpConns.begin();
        for (; it != m_mpConns.end(); it++)
        {
            ConnSession* pConn = HPP_GetHppSessionById(it->second.nSessionId);
            if (!pConn)
            {
                continue;
            }
            if(HPR_OK == pConn->StopService())
            {
                COMM_INFO("StopService [%d] succeeded", pConn->GetSessionId());
            }else
            {
                COMM_WARN("StopService [%d] failed", pConn->GetSessionId());
            }
        }
    }

    {
        CGuard guard(&m_lckConns);
        m_mpConns.clear();
    }

    {
        CGuard guard(&m_lckSpecifiedConns);
        m_mpSpecifiedConns.clear();
    }

    RspDispatcher::SnglPtr()->Fini();

    return TRUE;
}

BOOL ConnSessionMgr::StartIncoming(void)
{
    if (NULL == m_hHppInit)
    {
        COMM_ERROR("ConnSessionMgr not init'ed");
        return FALSE;
    }

    if (!PbMsgDispatcher::SnglPtr()->Init())
    {
        return FALSE;
    }

    if (!StartListening(m_nPort))
    {
        PbMsgDispatcher::SnglPtr()->Fini();
        return FALSE;
    }

    return TRUE;
}

BOOL ConnSessionMgr::StopIncoming(void)
{
    if (NULL == m_hHppInit)
    {
        COMM_ERROR("ConnSessionMgr not init'ed");
        return FALSE;
    }

    // 停止监听
    CLIENT_INFO("StopListening...");
    StopListening();

    {
        CGuard guard(&m_lckIncomingConns);
        map<HPR_INT32, string>::iterator it = m_mpIncomingConns.begin();
        for (; it != m_mpIncomingConns.end(); it++)
        {
            ConnSession* pConn = HPP_GetHppSessionById(it->first);
            if (!pConn)
            {
                continue;
            }
            if(HPR_OK == pConn->StopService())
            {
                COMM_INFO("StopService [%d] succeeded", pConn->GetSessionId());
            }else
            {
                COMM_WARN("StopService [%d] failed", pConn->GetSessionId());
            }
        }
    }

    {
        CGuard guard(&m_lckIncomingConns);
        m_mpIncomingConns.clear();
    }

    PbMsgDispatcher::SnglPtr()->Fini();

    return TRUE;
}
