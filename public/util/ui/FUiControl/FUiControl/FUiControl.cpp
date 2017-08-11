// FUiControl.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "FUiControl.h"
#include "FUi/ResMgr.h"

#include "FUi/CalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCalendarDlg *g_pCalendarDlg = NULL;

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

static ULONG_PTR fui_token = NULL;
FUICONTROL_API void FUiInit()
{
	GdiplusStartupInput input;
	GdiplusStartup(&fui_token, &input, NULL);
	CResMgr::GetInstance()->InitResMgr();
}

FUICONTROL_API void FUiCleanup()
{
	GdiplusShutdown(fui_token);

    if (g_pCalendarDlg)
    {
        g_pCalendarDlg->DestroyWindow();
        delete g_pCalendarDlg;
        g_pCalendarDlg = NULL;
    }
	CResMgr::GetInstance()->UninitResMgr();
}

FUICONTROL_API void FUiSetRes(LPCTSTR lpszRes, LPCTSTR lpszPassword)
{
	CResMgr::GetInstance()->SetRes(lpszRes, lpszPassword);
}

FUICONTROL_API Image* FUiGetRes(LPCTSTR lpszResName)
{
    return CResMgr::GetInstance()->GetRes(lpszResName);
}
