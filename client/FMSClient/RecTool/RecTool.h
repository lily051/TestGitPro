
// RecTool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CRecToolApp:
// 有关此类的实现，请参阅 RecTool.cpp
//

class CRecToolApp : public CWinAppEx
{
public:
	CRecToolApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	// 获取程序配置相关信息
	void GetConfigInfo();

	BOOL ReadConfig(CString& m_strAppCapPath);


public:
	CString m_strAppPath;		//程序运行路径
	CString m_strAppName;
	CString m_strAppVer;
	CString m_strDefaultCapPath;
	int m_nLanguage;
	int m_nSkinType;
	CString m_sCustomTitle;
	CString m_sCopyright;
	CString m_strAppCapPath;
	CString m_strNewFileName;
};

extern CRecToolApp theApp;