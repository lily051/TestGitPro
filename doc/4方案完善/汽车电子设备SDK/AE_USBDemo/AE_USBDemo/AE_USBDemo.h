
// AE_USBDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAE_USBDemoApp:
// �йش����ʵ�֣������ AE_USBDemo.cpp
//

class CAE_USBDemoApp : public CWinAppEx
{
public:
	CAE_USBDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAE_USBDemoApp theApp;