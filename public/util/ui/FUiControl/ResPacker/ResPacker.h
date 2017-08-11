
// ResPacker.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CResPackerApp:
// 有关此类的实现，请参阅 ResPacker.cpp
//

class CResPackerApp : public CWinAppEx
{
public:
	CResPackerApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

int SuperMkDirs(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
char *GenRandomString(char *pStr, int nLen);
void myDeleteDirectory(CString strDelPath);
void myCopyDirectory(CString strSrc, CString strDst);

extern CResPackerApp theApp;