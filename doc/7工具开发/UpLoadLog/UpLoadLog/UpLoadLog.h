
// UpLoadLog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUpLoadLogApp:
// �йش����ʵ�֣������ UpLoadLog.cpp
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
// ��д
	public:
	virtual BOOL InitInstance();
	BOOL IsAppRunning();
	BOOL ParseCommandLine();
	void CloseProgram(CString strProgram);
// ʵ��

	DECLARE_MESSAGE_MAP()
};
extern CUpLoadLogApp theApp;