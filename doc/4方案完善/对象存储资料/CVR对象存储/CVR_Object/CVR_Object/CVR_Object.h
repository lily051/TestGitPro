// CVR_Object.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCVR_ObjectApp:
// �йش����ʵ�֣������ CVR_Object.cpp
//

class CCVR_ObjectApp : public CWinApp
{
public:
	CCVR_ObjectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCVR_ObjectApp theApp;