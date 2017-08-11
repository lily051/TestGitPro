
// RecTool.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "RecTool.h"
#include "RecToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* CLIENT_CONFIG_FILENAME = _T("FMSClient.ini");
const TCHAR* MODULE_CONFIG_PATH  = _T("Config\\ModuleConfig.xml");      // 模块配置文件位置
const TCHAR* FUI_RES_PASSWORD = _T("Hik_Financial_8100");               // Fui资源密码

// CRecToolApp

BEGIN_MESSAGE_MAP(CRecToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRecToolApp 构造

CRecToolApp::CRecToolApp()
:m_nSkinType(1)
,m_nLanguage(0)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRecToolApp 对象

CRecToolApp theApp;


// CRecToolApp 初始化

BOOL CRecToolApp::InitInstance()
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

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strAppName = szPath;

	//获取版本
	//GetVerValue(_T("FileVersion"), m_strAppVer);

	//工作目录
	(_tcsrchr(szPath, _T('\\')))[0] = 0;
	m_strAppPath = szPath;
	m_strDefaultCapPath = m_strAppPath + _T("\\RecordFiles\\");

	 FUiInit();

	ReadConfig(m_strAppCapPath);

	 GetConfigInfo();

	CRecToolDlg dlg;
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

int CRecToolApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	FUiCleanup();

	return CWinAppEx::ExitInstance();
}


void CRecToolApp::GetConfigInfo()
{
	//配置文件路径
	CString strConfigPath = m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	//初始化皮肤风格
	m_nSkinType = GetPrivateProfileInt(_T("FMSClient"), _T("SkinScheme"), 1, strConfigPath);
	CString strImagePath;
	strImagePath.Format(_T("\\image\\%d.rc"), m_nSkinType);
	FUiSetRes(m_strAppPath + strImagePath, FUI_RES_PASSWORD);

	TCHAR szCustomTitle[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("CustomTitle"), _T("理财风险管理系统"), szCustomTitle, MAX_PATH, strConfigPath);
	m_sCustomTitle = szCustomTitle;

	TCHAR szCopyright[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("Copyright"), _T(""), szCopyright, MAX_PATH, strConfigPath);
	m_sCopyright = szCopyright;
}

BOOL CRecToolApp::ReadConfig(CString& m_strAppCapPath)
{
	TCHAR szDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szDir, MAX_PATH);
	PathRemoveFileSpec(szDir); //将路径末尾的文件名和反斜杠去掉
	TCHAR szPath[MAX_PATH] = {0};
	PathCombine(szPath, szDir, CLIENT_CONFIG_FILENAME);
	TCHAR szBuf[1024] = {0};
	GetPrivateProfileString(_T("LocalConfig"), _T("CapPath"), theApp.m_strDefaultCapPath, szBuf, 1024, szPath);
	m_strAppCapPath.Format(_T("%s"), szBuf);

	return TRUE;
}