
// DBSimulateToolForTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDBSimulateToolForTestApp:
// �йش����ʵ�֣������ DBSimulateToolForTest.cpp
//

class CDBSimulateToolForTestApp : public CWinAppEx
{
public:
	CDBSimulateToolForTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDBSimulateToolForTestApp theApp;