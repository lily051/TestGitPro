#include "StdAfx.h"
#include <time.h>
#include "Msger.h"
#include <fclib/fcTchar.h>
#include <HPR_Utils.h>
#include <HPR_Netdb.h>
#include "ConnSessionMgr.h"
#include "AlarmMsg.pb.h"
#include "Config.h"
#include "type/ModuleType.pb.h"
#include "libRecGuard.h"

namespace {
    std::string g_strLocalIp;
    CMsger *g_pMsger = NULL;
    TCHAR *MakeTimeString(TCHAR *buf, DWORD nSize);
    void AddMsgV(TCHAR *buf, DWORD nSize, LPCTSTR pszFormat, va_list args);
}

CMsger::CMsger(CMsger *pMsger/* = NULL*/)
: m_pOtherMsger(pMsger)
{
}

CMsger::~CMsger(void)
{
    try
    {
	    if (NULL != m_pOtherMsger)
	    {
	        delete m_pOtherMsger;
	        m_pOtherMsger = NULL;
	    }
    }
    catch (...)
    {
    }
}

void CMsger::AddMsg(LPCTSTR pszFormat, ...)
{
    va_list arglist;
    va_start(arglist, pszFormat);
    AddMsgV(pszFormat, arglist);
    va_end(arglist);
}

void CMsger::AddMsgV(LPCTSTR pszFormat, va_list args)
{
    TCHAR buf[BUFF_SIZE] = {0};

    __time64_t long_time;
    _time64(&long_time);
    struct tm *newtime = _localtime64(&long_time);
    _stprintf(buf, _T("%04d-%02d-%02d %02d:%02d:%02d "),
        newtime->tm_year + 1900,
        newtime->tm_mon + 1,
        newtime->tm_mday,
        newtime->tm_hour,
        newtime->tm_min,
        newtime->tm_sec);

    size_t const timeinfolen = _tcslen(buf);

    _vsntprintf(buf + timeinfolen, _countof(buf) - 1 - timeinfolen,
        pszFormat, args);

    Action(buf);
}

// 用于debugview
class CMsgerDbgView : public CMsger
{
public:
    CMsgerDbgView(CMsger *pMsger = NULL)
        : CMsger(pMsger)
    {}
private:
    // override
    void doAction(LPCTSTR lpMsg)
    {
        OutputDebugString(lpMsg);
        if (NULL == _tcsstr(lpMsg, _T("\n")))
        {
            OutputDebugString(_T("\n"));
        }
    }
};

// 用于控制台
class CMsgerConsole : public CMsger
{
public:
    CMsgerConsole(CMsger *pMsger = NULL)
        : CMsger(pMsger)
    {}
private:
    // override
    void doAction(LPCTSTR lpMsg)
    {
        DWORD dwCharWrite = 0;
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpMsg,
            _tcslen(lpMsg), &dwCharWrite, NULL);
        if (NULL == _tcsstr(lpMsg, _T("\n")))
        {
            WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), _T("\n"),
                1, &dwCharWrite, NULL);
        }
    }
};

// 用于服务模式
class CMsgerCallback : public CMsger
{
public:
    CMsgerCallback(CMsger *pMsger = NULL)
        : CMsger(pMsger)
    {}
private:
    // override
    void doAction(LPCTSTR lpMsg)
    {
        std::string strMsg = std::string(fcT2U(lpMsg));
        _gShowMsgStr(strMsg);
    }
};

bool GetLocalIp(std::string &strIp)
{
    char local[255] = {0};
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (ph == NULL)
    {
        return false;
    }
    // 这里仅获取第一个ip
    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
    std::string localIP;
    localIP.assign(inet_ntoa(addr));

    strIp = localIP.c_str();

    return true;
}

void Msger_Init(void)
{
    if (NULL != g_pMsger)
    {
        delete[] g_pMsger;
        g_pMsger = NULL;
    }
    LPCTSTR lpCmdline = GetCommandLine();
    // 命令行参数带有 -svc 则为服务模式
    //if (NULL != _tcsstr(lpCmdline, _T("-svc")))
    //{
    //    g_pMsger = new CMsgerCallback(g_pMsger);
    //}
    //else
    if (NULL == _tcsstr(lpCmdline, _T("-svc")))
    {
        g_pMsger = new CMsgerConsole(g_pMsger);
    }
    else
    {
        g_pMsger = new CMsgerDbgView(g_pMsger);
    }

    GetLocalIp(g_strLocalIp);
}
void Msger_Fini(void)
{
    if (NULL != g_pMsger)
    {
        delete g_pMsger;
        g_pMsger = NULL;
    }
}

void Msger_AddMsg(LPCTSTR pszFormat, ...)
{
    if (NULL == g_pMsger)
    {
        return;
    }
    va_list arglist;
    va_start(arglist, pszFormat);
    g_pMsger->AddMsgV(pszFormat, arglist);
    va_end(arglist);
}

void Msger_NotifyMsg(const DWORD userid, LPCTSTR pszFormat, ...)
{
    TCHAR buf[4096] = {0};
    va_list arglist;
    va_start(arglist, pszFormat);
    AddMsgV(buf, _countof(buf), pszFormat, arglist);
    va_end(arglist);
    std::string strMsg = std::string(fcT2U(buf));
    _gShowMsgStr(strMsg);
}

void Msger_AlarmMsg(const DWORD alarmType,
					const DWORD userid, 
					const DWORD channel, 
					const DWORD alarmStatus,
					const DWORD dwRev,
					LPCTSTR pszFormat, ...)
{
    TCHAR buf[4096] = {0};
    va_list arglist;
    va_start(arglist, pszFormat);
    AddMsgV(buf, _countof(buf), pszFormat, arglist);
    va_end(arglist);

    std::string strMsg = std::string(fcT2U(buf));
    _gShowMsgStr(strMsg);
    
    //报警信息
    AlarmMsg::Alarm msgAlarm;
    char szBuff[10] = {0};

    //模块
    sprintf_s(szBuff, sizeof(szBuff), "%d", SVC_RECGUARD);
    msgAlarm.set_svctype(fcA2U(szBuff));

    //消息类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", MSG_TYPE_ALARM);
    msgAlarm.set_dwmsgtype(fcA2U(szBuff));

    //服务类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", alarmType);
    msgAlarm.set_alarmtype(fcA2U(szBuff));

    //用户
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", userid);
    msgAlarm.set_userid(fcA2U(szBuff));

    //通道
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", channel);
    msgAlarm.set_channel(fcA2U(szBuff));

    //报警状态 1报警 0 非报警
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", alarmStatus);
    msgAlarm.set_alarmstatus(fcA2U(szBuff));

    //导致报警操作类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", dwRev);
    msgAlarm.set_dwrev(fcA2U(szBuff));

    //报警信息
    char   szAlarmInfo[SVCMSG_CONTENT_LEN] = {0};
    strncpy(szAlarmInfo, fcT2A(pszFormat), SVCMSG_CONTENT_LEN - 1);
    szAlarmInfo[SVCMSG_CONTENT_LEN - 1] = 0;
    msgAlarm.set_alarminfo(fcA2U(szAlarmInfo));

    //报警ip地址
    msgAlarm.set_alarmhost(fcA2U(CConfig::GetSingletonPtr()->m_strLocalIP.c_str()));

    CConnSessionMgr::GetSingletonPtr()->SendAlarmMsg(CConfig::GetSingletonPtr()->m_strATServerIP, 
        CConfig::GetSingletonPtr()->m_nATServerPort, AlarmMsg::AUTHORIZE_REQ, &msgAlarm);
}

namespace {
TCHAR *MakeTimeString(TCHAR *buf, DWORD nSize)
{
    __time64_t long_time;
    _time64(&long_time);
    struct tm *newtime = _localtime64(&long_time);
    _stprintf(buf, _T("%04d-%02d-%02d %02d:%02d:%02d "),
        newtime->tm_year + 1900,
        newtime->tm_mon + 1,
        newtime->tm_mday,
        newtime->tm_hour,
        newtime->tm_min,
        newtime->tm_sec);

    return buf;
}
void AddMsgV(TCHAR *buf, DWORD nSize, LPCTSTR pszFormat, va_list args)
{
    _vsntprintf(buf, nSize - 1,
        pszFormat, args);
}
}


