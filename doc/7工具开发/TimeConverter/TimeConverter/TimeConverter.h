
// TimeConverter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTimeConverterApp:
// �йش����ʵ�֣������ TimeConverter.cpp
//

class CTimeConverterApp : public CWinAppEx
{
public:
	CTimeConverterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTimeConverterApp theApp;