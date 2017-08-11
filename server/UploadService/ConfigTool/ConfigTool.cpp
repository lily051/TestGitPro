
// ConfigTool.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void InitWorkPath()
{
	TCHAR path[256] = {0};
	GetModuleFileName(NULL, path, 256);
	for (int i = (int)_tcslen(path); i >= 0; --i)
	{
		if (path[i] == _T('\\'))
		{
			path[i]=0;
			break;
		}
	}
	SetCurrentDirectory(path);
}


// CConfigToolApp

BEGIN_MESSAGE_MAP(CConfigToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConfigToolApp 构造

CConfigToolApp::CConfigToolApp()
{
	m_hMutex = NULL;
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CConfigToolApp 对象

CConfigToolApp theApp;


// CConfigToolApp 初始化

BOOL CConfigToolApp::InitInstance()
{
	// 同一个程序只运行一个实例
	CString appMutexName(_T("Global\\Upload Client"));
	m_hMutex = CreateMutex(NULL, FALSE, appMutexName);
	if (NULL == m_hMutex)
	{
		MessageBox(NULL, _T("理财上传客户端启动失败！"), _T("提示"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("理财上传客户端只能启动一个！"), _T("提示"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}

	CString moduleFileDir;
	GetModuleFileName(NULL, moduleFileDir.GetBuffer(MAX_PATH), MAX_PATH - 1);
	moduleFileDir.ReleaseBuffer(MAX_PATH - 1);
	int dotPos = moduleFileDir.ReverseFind('\\');
	if (dotPos > 0)
	{
		moduleFileDir = moduleFileDir.Left(dotPos + 1);
		// 设置当前运行目录为看门狗可执行程序所在目录。
		SetCurrentDirectory(moduleFileDir);
	}
	else
	{
		moduleFileDir.Empty();
	}

	// 写入注册表，开机自运行
	WCHAR szAppName[MAX_PATH];
	GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppPath(szAppName);
	LPCTSTR lpszKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	CRegKey regeditKey;
	if (ERROR_SUCCESS == regeditKey.Open(HKEY_LOCAL_MACHINE, lpszKey))
	{
		// LONG lrtn = 
		regeditKey.SetStringValue(_T("Upload Client"), strAppPath);
		regeditKey.Close();
	}
	else
	{
		MessageBox(NULL, _T("注册表打开失败！"), _T("提示"), MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("The application wizard generates a local application"));

	InitWorkPath();

	CConfigToolDlg *dlg = new CConfigToolDlg;
/*lint -e1414*/
	m_pMainWnd = dlg;
/*lint +e1414*/ 
	INT_PTR nResponse = dlg->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	delete dlg;
	return FALSE;
}
