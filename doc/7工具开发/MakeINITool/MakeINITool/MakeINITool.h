
// MakeINITool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMakeINIToolApp:
// �йش����ʵ�֣������ MakeINITool.cpp
//

class CMakeINIToolApp : public CWinAppEx
{
public:
	CMakeINIToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMakeINIToolApp theApp;