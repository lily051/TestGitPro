
// FUiDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFUiDemoApp:
// �йش����ʵ�֣������ FUiDemo.cpp
//

class CFUiDemoApp : public CWinAppEx
{
public:
	CFUiDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CFUiDemoApp theApp;