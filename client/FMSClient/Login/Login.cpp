// Login.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int /*argc*/, TCHAR* /*argv[]*/, TCHAR* /*envp[]*/)
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
	//coverityҪ����ָ�룬���ö���
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
