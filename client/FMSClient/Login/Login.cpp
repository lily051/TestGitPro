// Login.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int /*argc*/, TCHAR* /*argv[]*/, TCHAR* /*envp[]*/)
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	return nRetCode;
}

LOGIN_API INT_PTR ShowLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
                               int nUseAutoLogin, int nSkin, int nLanguage)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (NULL == pfunLoginEvent)
    {
        return NULL;
    }

  //  CLoginDlg loginDlg(pfunLoginEvent, pUser, nUseAutoLogin, nSkin, nLanguage);
	//coverity要求用指针，不用对象
	CLoginDlg *ploginDlg = new CLoginDlg(pfunLoginEvent, pUser, nUseAutoLogin, nSkin, nLanguage);
	if (!ploginDlg)
	{
		return IDCANCEL;
	}
	int nRet = ploginDlg->DoModal();
	if (ploginDlg)
	{
		delete ploginDlg;
		ploginDlg = NULL;
	}

	return nRet;
	
	
	//return loginDlg.DoModal();
}
