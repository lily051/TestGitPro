
// Preview_HIK.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

typedef struct _tagCmdParam
{
    int nParam;//参数个数
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
// 有关此类的实现，请参阅 Preview_HIK.cpp
//

class CPreview_HIKApp : public CWinAppEx
{
public:
	CPreview_HIKApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

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