
// DemoWSInterfece.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemoWSInterfeceApp:
// �йش����ʵ�֣������ DemoWSInterfece.cpp
//

class CDemoWSInterfeceApp : public CWinAppEx
{
public:
	CDemoWSInterfeceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemoWSInterfeceApp theApp;