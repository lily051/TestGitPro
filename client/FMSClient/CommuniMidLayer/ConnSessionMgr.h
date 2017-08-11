#pragma once

#include <map>
#include <hpp_hpp.h>
#include "./NetInfoApp.h"
#include "./TimerThread.h"

//////////////////////////////////////////////////////////////////////////
// ConnSessionMgr
//lint -e631
class ConnSessionMgr
{
    SINGLETON_DECLARE(ConnSessionMgr)   /// <单例相关声明
private:
    ConnSessionMgr(void);
    virtual ~ConnSessionMgr(void);

    HPP_HANDLE m_hHppInit;
    HPP_SERVER_HANDLE m_hHppSvr;
    int m_nPort;
    int m_nHppThreadNum;
    int m_nHppContextTimeout;
    int m_nConnectTimeout;

    typedef struct _ConnInfo
    {
        HPR_INT32 nSessionId;
        BOOL bOnline;
        SERVER_INFO stSvrInfo;
        _ConnInfo()
        {
            nSessionId = -1;
            bOnline = FALSE;
        }
    } ConnInfo;

    CRITICAL_SECTION    m_lckSpecifiedConns;
    std::map<int, string> m_mpSpecifiedConns;

    CRITICAL_SECTION    m_lckConns;
    std::map<string, ConnInfo> m_mpConns;

    CRITICAL_SECTION    m_lckIncomingConns;
    std::map<HPR_INT32, string> m_mpIncomingConns;

    CRITICAL_SECTION m_lckConnect;  //连接服务器信号量，同时正在连接树只允许为1
    HANDLE m_hEventConnect; //连接服务器事件
    int m_nLastSessionId;   //最近一次连接的服务id

    TimerThread m_TimerThread;

    BOOL UpdateConnStatus(const ConnInfo& stConnInfo);
    BOOL Connect(ConnInfo & stConnInfo, const string &strKey, BOOL bUpdateConn = FALSE);

    BOOL StartListening(int nPort);
    void StopListening();

public:
    void PrintHppConns(void);
    static string FormatKey(const string & sIP, int nPort);

    HPR_INT32 GetConnSessionID(SERVER_INFO& stServerInfo);
    void MaintainConnSessions(const BOOL *pAvailable);

    BOOL Init(const int nPort);
    BOOL Fini(void);

    BOOL StartOutgoing(void);
    BOOL StopOutgoing(void);

    BOOL StartIncoming(void);
    BOOL StopIncoming(void);

    BOOL AddIncomingConn(ConnSession* pConnSession);
    BOOL RemoveIncomingConn(HPR_INT32 nSessionId);

    static HPR_INT32 OnAcceptIncomingConn(ConnSession* pConnSession);
    static HPR_INT32 OnIncomingConnClosed(ConnSession* pConnSession);
    static HPR_INT32 OnIncomingConnTimeOut(ConnSession* pConnSession);

    static HPR_INT32 OnServerSocketClosed(ConnSession* pConnSession);
    static HPR_INT32 OnServerCheckTimeOut(ConnSession* pConnSession);
    static HPR_INT32 OnConnectSuccess(ConnSession* pConnSession);
    static HPR_INT32 OnConnectFailed(HPR_VOIDPTR hpr_voidPtr);
};

