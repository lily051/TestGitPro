
// VideoCollect_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoCollect_ClientApp:
// �йش����ʵ�֣������ VideoCollect_Client.cpp
//

class CVideoCollect_ClientApp : public CWinAppEx
{
public:
	CVideoCollect_ClientApp();

// ��д
	public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoCollect_ClientApp theApp;