
// HlogDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHlogDemoApp:
// �йش����ʵ�֣������ HlogDemo.cpp
//

class CHlogDemoApp : public CWinAppEx
{
public:
	CHlogDemoApp();
    ~CHlogDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHlogDemoApp theApp;