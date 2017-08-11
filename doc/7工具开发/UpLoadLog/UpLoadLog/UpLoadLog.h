
// UpLoadLog.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CUpLoadLogApp:
// 有关此类的实现，请参阅 UpLoadLog.cpp
//
class CUpLoadLogApp : public CWinAppEx
{
public:
	char m_url[MAX_PATH];
	char m_localpath[MAX_PATH];
	char m_logpath[MAX_PATH];
	char userpwd[MAX_PATH];
	char m_cfilename[MAX_PATH];
	int g_nCount;

	CString m_sUrl;
	CString m_sLogPath;
	CString m_iFlag;
	CString m_sAppName;

	CString m_filename;
	CString m_csLogPath;
	CString m_strlogpath;

	HANDLE m_hMutex;
public:
	CUpLoadLogApp();
// 重写
	public:
	virtual BOOL InitInstance();
	BOOL IsAppRunning();
	BOOL ParseCommandLine();
	void CloseProgram(CString strProgram);
// 实现

	DECLARE_MESSAGE_MAP()
};
extern CUpLoadLogApp theApp;