// Cloud_Object.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCloud_ObjectApp:
// �йش����ʵ�֣������ Cloud_Object.cpp
//

class CCloud_ObjectApp : public CWinApp
{
public:
	CCloud_ObjectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCloud_ObjectApp theApp;