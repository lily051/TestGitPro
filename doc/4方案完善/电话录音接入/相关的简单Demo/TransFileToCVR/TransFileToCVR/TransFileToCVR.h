
// TransFileToCVR.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTransFileToCVRApp:
// �йش����ʵ�֣������ TransFileToCVR.cpp
//

class CTransFileToCVRApp : public CWinAppEx
{
public:
	CTransFileToCVRApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTransFileToCVRApp theApp;