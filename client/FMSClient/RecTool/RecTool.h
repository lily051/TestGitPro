
// RecTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRecToolApp:
// �йش����ʵ�֣������ RecTool.cpp
//

class CRecToolApp : public CWinAppEx
{
public:
	CRecToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	// ��ȡ�������������Ϣ
	void GetConfigInfo();

	BOOL ReadConfig(CString& m_strAppCapPath);


public:
	CString m_strAppPath;		//��������·��
	CString m_strAppName;
	CString m_strAppVer;
	CString m_strDefaultCapPath;
	int m_nLanguage;
	int m_nSkinType;
	CString m_sCustomTitle;
	CString m_sCopyright;
	CString m_strAppCapPath;
	CString m_strNewFileName;
};

extern CRecToolApp theApp;