
// SetNTP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetNTPApp:
// �йش����ʵ�֣������ SetNTP.cpp
//

class CSetNTPApp : public CWinAppEx
{
public:
	CSetNTPApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSetNTPApp theApp;