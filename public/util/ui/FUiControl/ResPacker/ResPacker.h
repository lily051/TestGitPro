
// ResPacker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CResPackerApp:
// �йش����ʵ�֣������ ResPacker.cpp
//

class CResPackerApp : public CWinAppEx
{
public:
	CResPackerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

int SuperMkDirs(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
char *GenRandomString(char *pStr, int nLen);
void myDeleteDirectory(CString strDelPath);
void myCopyDirectory(CString strSrc, CString strDst);

extern CResPackerApp theApp;