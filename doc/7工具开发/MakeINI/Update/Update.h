
// Update.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
#include "stdafx.h"
#include "resource.h"		// ������

// CUpdateApp:
// �йش����ʵ�֣������ Update.cpp
//

class CUpdateApp : public CWinAppEx
{
public:
	int file_number;
	CString szIniPath;
	queue<CString> download_list;
	CUpdateApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpdateApp theApp;