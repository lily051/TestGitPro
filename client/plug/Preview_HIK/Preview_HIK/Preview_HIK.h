
// Preview_HIK.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

typedef struct _tagCmdParam
{
    int nParam;//��������
    CString sIP;
    int nPort;
    CString sUser;
    CString sPWD;
    int nChannel;
    _tagCmdParam()
    {
        nParam = 0;
        nChannel = -1;
    }
}CMD_PARAM;


// CPreview_HIKApp:
// �йش����ʵ�֣������ Preview_HIK.cpp
//

class CPreview_HIKApp : public CWinAppEx
{
public:
	CPreview_HIKApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
public:
    CMD_PARAM m_cmdParam;
    CString m_strAppPath;
    CString m_strPicPath;
private:
    BOOL GetCommandLineParam();
};

extern CPreview_HIKApp theApp;