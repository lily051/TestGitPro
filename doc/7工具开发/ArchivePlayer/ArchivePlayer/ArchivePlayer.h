
// ArchivePlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArchivePlayerApp:
// �йش����ʵ�֣������ ArchivePlayer.cpp
//

class CArchivePlayerApp : public CWinAppEx
{
public:
	CArchivePlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CArchivePlayerApp theApp;