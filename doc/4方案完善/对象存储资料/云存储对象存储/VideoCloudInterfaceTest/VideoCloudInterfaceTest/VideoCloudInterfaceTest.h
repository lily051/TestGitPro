
// VideoCloudInterfaceTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoCloudInterfaceTestApp:
// �йش����ʵ�֣������ VideoCloudInterfaceTest.cpp
//

class CVideoCloudInterfaceTestApp : public CWinAppEx
{
public:
	CVideoCloudInterfaceTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoCloudInterfaceTestApp theApp;