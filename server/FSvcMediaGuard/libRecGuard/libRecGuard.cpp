// libRecGuard.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "libRecGuard.h"
#include <fclib/fcNetComm.h>
#include <fclib/fcTchar.h>
//#include <hpr/include/HPR_Hpr.h>
#include "RecGuard.h"
#include "Config.h"
#include <SvcGeneralDef.h>
#include <hak.h>
#include "DBDataMgr.h"
#include "type/ModuleType.pb.h"
#include "../util/StringConverter.h"

static pTransMsgBack g_pMsgBack = NULL;
//pTransMsgBackEx g_pMsgBack = NULL;
static HANDLE g_hServiceThread = NULL;
static HANDLE g_hEvent = NULL;

static const int LIBRECGUARD_ERROR_BASIC = -1;

// This is an example of an exported function.
LIBRECGUARD_API int fnlibRecGuard(void)
{
	return 42;
}

DWORD WINAPI ServiceThread(LPVOID /*lpPrame*/)
{
    TPLOG_INFO("ServiceThread..........IN");

    int nStartResult = HPR_ERROR;
    bool bWaitOK = false;

    do 
    {
        //服务启动
        nStartResult = GetRecGuardPtr()->Start();

        if (NO_ERROR == nStartResult)
        {
            break;
        }

        bWaitOK = ::WaitForSingleObject(g_hEvent, 5000) == WAIT_OBJECT_0;
        if (bWaitOK)
        {
            TPLOG_INFO("Get Exit Signal......");
            break;
        }
    } while (NO_ERROR != nStartResult);

    TPLOG_INFO("ServiceThread............OUT");
    return 0;
}

LIBRECGUARD_API int WINAPI libRecGuard_Init(void)
{
    //HLOG_INIT_LOG4CXX_SYNC("libRecGuard");

    if (HPR_OK != HPR_Init())
    {
        TPLOG_ERROR("init hpr failed");
		Msger_NotifyMsg(ALL_USER, _T("初始化HPR库失败"));
        return LIBRECGUARD_ERROR_BASIC;
    }

    if (HPR_OK != CConfig::GetSingletonPtr()->LoadConfigFile())
    {
        TPLOG_ERROR("init config failed");
		Msger_NotifyMsg(ALL_USER, _T("加载配置文件失败"));
        return LIBRECGUARD_ERROR_BASIC;
    }

    if (NO_ERROR != GetRecGuardPtr()->Init())
    {
        TPLOG_ERROR("init rec guard failed");
        return LIBRECGUARD_ERROR_BASIC;
    }
    
    // 初始化库。
    if (Hak1((char*)103, "DATA", NULL))
    {
        TPLOG_ERROR("初始化海康加密库失败");
        return LIBRECGUARD_ERROR_BASIC;
    }

    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API int WINAPI libRecGuard_Fini(void)
{
    GetRecGuardPtr()->Fini();
    GetRecGuardPtr()->free();
    HPR_Fini();
    //Msger_Fini();
    Hak2(NULL, NULL, NULL);
    //HLOG_FINI();
    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API int WINAPI libRecGuard_Start(void)
{
    if (!g_hServiceThread)
    {
        g_hEvent = ::CreateEvent(NULL,FALSE,FALSE,NULL);
        if (NULL == g_hEvent)
        {
            return -1;
        }
        g_hServiceThread = ::CreateThread(NULL, 0, ServiceThread, NULL, 0, NULL);
    } 
    
    TPLOG_INFO("录像守卫服务启动");
    Msger_NotifyMsg(ALL_USER, _T("录像守卫服务启动"));
    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API int WINAPI libRecGuard_Stop(void)
{
    if (g_hServiceThread)
    {
        ::SetEvent(g_hEvent);
        ::WaitForSingleObject(g_hServiceThread, INFINITE);
        ::CloseHandle(g_hServiceThread);
        g_hServiceThread = NULL;

        if (NULL != g_hEvent)
        {
            ::CloseHandle(g_hEvent);
            g_hEvent = NULL;
        }

        GetRecGuardPtr()->Stop();
    }
    
    Msger_NotifyMsg(ALL_USER, _T("录像守卫服务停止"));
    TPLOG_INFO("录像守卫服务停止");
    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API DWORD WINAPI libRecGuard_GetVersion(void)
{
    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API int WINAPI libRecGuard_GetLastError(void)
{
    return libRecGuard_NO_ERROR;
}

LIBRECGUARD_API int WINAPI libRecGuard_SetMsgCallback(const pTransMsgBack msgfun)
{
    g_pMsgBack = msgfun;
    return libRecGuard_NO_ERROR;
}
// LIBRECGUARD_API int WINAPI libRecGuard_SetMsgCallbackEx(const pTransMsgBackEx msgfun)
// {
//  //   g_pMsgBack = msgfun;
//     return libRecGuard_NO_ERROR;
// }

/** @fn     _gGetCurTimeStr
* @brief 获取当前时间的字符串
* @param  [in] 无
* @param  [out] 无
* @return 时间字符串， 格式：%Y%m%d %H:%M:%S--
*/
std::string _gGetCurTimeStr()
{ 
    time_t t = time(NULL); 
    char buff[128]; 
    strftime(buff, sizeof(buff), "%Y%m%d %H:%M:%S--", localtime(&t));
    return std::string(buff); 
}

//注意参数为UTF-8编码
void _gShowMsgStr(const std::string  & sMsg_)
{
    if (NULL == g_pMsgBack)
    {
        TPLOG_INFO("g_pMsgBack == NULL");
        return;
    }

    BASIC_MESSAGE_EX msgEx;
    //需要根据具体的服务器类型修改下面的参数
    msgEx.dwSvcType = cms_8120::MT_SERVER_MEDIAGUARD;
    msgEx.dwMsgType = MSG_TYPE_NOTIFY;
    std::string sMsg = CSCT::Ansi2Utf8(_gGetCurTimeStr())+ sMsg_;
    strcpy_s(msgEx.chContent, SVCMSG_CONTENT_LEN, sMsg.c_str());
    strcpy_s(msgEx.chResult, SVCMSG_RESULT_LEN, "");
    g_pMsgBack(&msgEx);
}
