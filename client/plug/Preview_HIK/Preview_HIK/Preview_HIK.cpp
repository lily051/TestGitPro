
// Preview_HIK.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Preview_HIK.h"
#include "Preview_HIKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPreview_HIKApp

BEGIN_MESSAGE_MAP(CPreview_HIKApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPreview_HIKApp 构造

CPreview_HIKApp::CPreview_HIKApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPreview_HIKApp 对象

CPreview_HIKApp theApp;


// CPreview_HIKApp 初始化

BOOL CPreview_HIKApp::InitInstance()
{
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
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    if (!GetCommandLineParam())
    {
        AfxMessageBox(_T("参数不正确"));
        return FALSE;
    }

    AppFun_GetAppPath(m_strAppPath);
    m_strPicPath = m_strAppPath + _T("pic\\");
    SHCreateDirectoryEx(NULL,m_strPicPath,NULL);

    NET_DVR_Init();

	CPreview_HIKDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
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
	return FALSE;
}

int CPreview_HIKApp::ExitInstance()
{
    NET_DVR_Cleanup();

    return CWinAppEx::ExitInstance();
}

//获取命令行参数
BOOL CPreview_HIKApp::GetCommandLineParam()
{
    LPWSTR *szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (NULL == szArglist)
    {
        return FALSE;
    }

    BOOL bResult = TRUE;
    if (6 == nArgs)//IP PORT USER PWD CHANNEL-->按通道预览
    {
        m_cmdParam.nParam = nArgs-1;
        m_cmdParam.sIP = szArglist[1];
        m_cmdParam.nPort = _ttoi(szArglist[2]);
        m_cmdParam.sUser = szArglist[3];
        m_cmdParam.sPWD = szArglist[4];
        m_cmdParam.nChannel = _ttoi(szArglist[5]);
        
    }
    else if (5 == nArgs)//IP PORT USER PWD-->按设备预览
    {
        m_cmdParam.nParam = nArgs-1;
        m_cmdParam.sIP = szArglist[1];
        m_cmdParam.nPort = _ttoi(szArglist[2]);
        m_cmdParam.sUser = szArglist[3];
        m_cmdParam.sPWD = szArglist[4];
    }
    else
    {
        bResult = FALSE;
    }

    LocalFree(szArglist);
    return bResult;
}
