// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//lint -library
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
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
*   @brief �����ļ�����
*   @param[in] strFileName���ļ���
*   @param null
*   @return �ļ����
*/
HANDLE Create_File(char * szFileName);

/** @fn AppFun_GetDllPath
*   @brief ��ȡdll·��
*   @param[in]  NULL
*   @param[out] strPathName: ·��
*   @return NULL
*/
void AppFun_GetDllPath(CString &strPathName);

/**   @fn          WaitForWindowThread
 *    @brief   	   �ȴ������߳�.
 *    @param[in]   hThread:�߳̾��.
 *    @param[in]   
 *    @return      
 */
void WaitForWindowThread(HANDLE hThread);

#if (defined _WIN32 || defined _WIN64)
void PLAYBACKSDK_PRINT(HLOG_LEVEL nLogType, const TCHAR *fmt, ...);
#else
void PLAYBACKSDK_PRINT(HLOG_LEVEL nLogType, const TCHAR *fmt, args...)
#endif 

// ��ӡ������Ϣ.
void OutputDebugStrEx(const TCHAR *fmt, ...);
// ��ȡ����IP.
BOOL AppFun_GetLocalIPAddr(CString & strIPAddr);
// ��ȡUDP�˿�.
int GetUdpPort(const CString& strLocalIP, LONG& nPort);
// �ж�PB_TIME�Ƕ���Ч
BOOL IsPBTimeCorrect(const PB_TIME& pbTime);

// ����ȫ�ִ�����.
extern void PB_SetLastError(DWORD dwLastError);