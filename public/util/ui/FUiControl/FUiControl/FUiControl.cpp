// FUiControl.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FUiControl.h"
#include "FUi/ResMgr.h"

#include "FUi/CalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCalendarDlg *g_pCalendarDlg = NULL;

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
