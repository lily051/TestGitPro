// libAdjustTime.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libAdjustTime.h"
#include <fclib/fcTchar.h>
//#include "fclib/fcMySQL.h"
#include <HPR_Hpr.h>
#include "AdjustTime.h"
#include "Config.h"
#include "hlogConfig.h"
#include "Msger.h"
#include <hak.h>
#include "ServiceFactory.h"
#include "type/ModuleType.pb.h"
#include "../util/StringConverter.h"

static pTransMsgBack g_pMsgBack = NULL;
pTransMsgBackEx g_pMsgBackEx = NULL;

static HANDLE g_hServiceThread = NULL;
static HANDLE g_hEvent = NULL;

static const int ADJUSTTIME_SVC_ERROR_BASIC = -1;

/**	@fn	    InitWorkPath
*	@brief	设置工作路径
*	@param  [in] 无
*	@param  [out] 无
*	@return	
*/
void InitWorkPath()
{
	TCHAR path[256] = {0};
	GetModuleFileName(NULL, path, 256);
	for (int i = (int)_tcslen(path); i >= 0; --i)
	{
		if (path[i] == _T('\\'))
		{
			path[i]=0;
			break;
		}
	}
	SetCurrentDirectory(path);
}

DWORD WINAPI ServiceThread(LPVOID /*lpPrame*/)
{
	TPLOG_INFO("ServiceThread..........IN");

	int nStartResult = ADJUST_ERROR_FAIL;
	bool bWaitOK = false;

	do 
	{
		//服务启动
		nStartResult = GetServiceFactoryPtr()->Start();

		if (ADJUST_ERROR_SUCC == nStartResult)
		{
			break;
		}

		bWaitOK = ::WaitForSingleObject(g_hEvent, 5000) == WAIT_OBJECT_0;
		if (bWaitOK)
		{
			TPLOG_INFO("Get Exit Signal......");
			break;
		}
	} while (ADJUST_ERROR_SUCC != nStartResult);

	TPLOG_INFO("ServiceThread............OUT");
	return 0;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_Init(void)
{
	//设置工作路径
	InitWorkPath();
	//初始化
	GetServiceFactoryPtr()->Init(); 
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_Fini(void)
{
	if (g_hServiceThread)
	{
		libAdjustTime_Stop();
	}
	//释放
	GetServiceFactoryPtr()->Fini();
	GetServiceFactoryPtr()->free();
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_Start(void)
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

	return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_Stop(void)
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

		GetServiceFactoryPtr()->Stop();
	}
	return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_ImmExec(void)
{
    //GetAdjustPtr()->ImmAdjustOnce();
    //TPLOG_INFO(" immediately adjust time once");
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API DWORD WINAPI libAdjustTime_GetVersion(void)
{
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_GetLastError(void)
{
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallback(const pTransMsgBack msgfun)
{
    g_pMsgBack = msgfun;
    return ADJUSTTIME_SVC_NO_ERROR;
}

LIBADJUSTTIME_API int WINAPI libAdjustTime_SetMsgCallbackEx(const pTransMsgBackEx msgfun)
{
    g_pMsgBackEx = msgfun;
    return ADJUSTTIME_SVC_NO_ERROR;
}
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
		LOG_INFO("g_pMsgBack == NULL");
		return;
	}

	BASIC_MESSAGE_EX msgEx;
	//需要根据具体的服务器类型修改下面的参数
	msgEx.dwSvcType = cms_8120::MT_SERVER_ADJUSTTIME;
	msgEx.dwMsgType = MSG_TYPE_NOTIFY;
	std::string sMsg = CSCT::Ansi2Utf8(_gGetCurTimeStr())+ sMsg_;
	strcpy_s(msgEx.chContent, SVCMSG_CONTENT_LEN, sMsg.c_str());
	strcpy_s(msgEx.chResult, SVCMSG_RESULT_LEN, "");
	g_pMsgBack(&msgEx);
}
//void main()
//{
//    int iRet = 0;
//    iRet = libAdjustTime_Init();
//    if(iRet)
//    {
//        OutputDebugStringA("init failed\n");
//        return ;
//    }
//    iRet = libAdjustTime_Start();
//    if(iRet)
//    {
//        OutputDebugStringA("start failed\n");
//        return ;
//    }
//    while (1)
//    {
//        if(0)
//        {
//            libAdjustTime_Stop();
//        }
//        Sleep(1000);
//    }
//}
