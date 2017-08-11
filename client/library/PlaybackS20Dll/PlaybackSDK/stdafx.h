// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//lint -library
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


#ifdef PLAYBACKSDK_API_EXPORT
#define PLAYBACKSDK_API_EXPORT
#endif

#pragma comment(lib, "imagehlp.lib")

#include "PlaybackSDKApi.h"
#include <imagehlp.h>
#include "hlog.h"
#include "hlogex.h"
#include "PBLog.h"
#include "Guard.h"
#include "Markup.h"
#include "HPR_String.h"
#include "HPR_Socket.h"
#include "HPR_Netdb.h"
#include <math.h>
/*
#include "HPR_Utils.h"
#include "HPR_Time.h"*/

/** @fn HANDLE Create_File
*   @brief 创建文件函数
*   @param[in] strFileName：文件名
*   @param null
*   @return 文件句柄
*/
HANDLE Create_File(char * szFileName);

/** @fn AppFun_GetDllPath
*   @brief 获取dll路径
*   @param[in]  NULL
*   @param[out] strPathName: 路径
*   @return NULL
*/
void AppFun_GetDllPath(CString &strPathName);

/**   @fn          WaitForWindowThread
 *    @brief   	   等待窗口线程.
 *    @param[in]   hThread:线程句柄.
 *    @param[in]   
 *    @return      
 */
void WaitForWindowThread(HANDLE hThread);

#if (defined _WIN32 || defined _WIN64)
void PLAYBACKSDK_PRINT(HLOG_LEVEL nLogType, const TCHAR *fmt, ...);
#else
void PLAYBACKSDK_PRINT(HLOG_LEVEL nLogType, const TCHAR *fmt, args...)
#endif 

// 打印调试信息.
void OutputDebugStrEx(const TCHAR *fmt, ...);
// 获取本机IP.
BOOL AppFun_GetLocalIPAddr(CString & strIPAddr);
// 获取UDP端口.
int GetUdpPort(const CString& strLocalIP, LONG& nPort);
// 判断PB_TIME是都有效
BOOL IsPBTimeCorrect(const PB_TIME& pbTime);

// 设置全局错误码.
extern void PB_SetLastError(DWORD dwLastError);