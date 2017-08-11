
// UpLoadLog.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "UpLoadLog.h"
#include "UpLoadLogDlg.h"
#include "tlhelp32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpLoadLogApp

BEGIN_MESSAGE_MAP(CUpLoadLogApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpLoadLogApp 构造

CUpLoadLogApp::CUpLoadLogApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	g_nCount = 0;
	m_url[MAX_PATH] = NULL;
	m_localpath[MAX_PATH] = NULL;
	m_cfilename[MAX_PATH] = NULL;
}


// 唯一的一个 CUpLoadLogApp 对象

CUpLoadLogApp theApp;


// CUpLoadLogApp 初始化

BOOL CUpLoadLogApp::InitInstance()
{
	
	// 检查程序是否已经在运行，如是则直接退出
	if (IsAppRunning())
	{
		return FALSE;
	}
	if(ParseCommandLine() == FALSE)
	{
		return FALSE;
	}
	char temp[MAX_PATH] = {0};
	//关闭调用该程序的进程
	StringChange::CStringToChar(m_sAppName+_T(".exe"),temp);
	UPLOAD_LOG_INFO("Close Progress %s",temp);
	CloseProgram(m_sAppName+_T(".exe"));

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

	CUpLoadLogDlg dlg;
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

BOOL CUpLoadLogApp::IsAppRunning()
{
	char temp[MAX_PATH] = {0};
	//互斥体
	CString m_strMutex = _T("UpLoadMutex");
	if ((m_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, m_strMutex)))
	{
		
		StringChange::CStringToChar(_T("UpLoadMutex"),temp);
		UPLOAD_LOG_INFO("Progress %s is exist",temp);
		return TRUE;
	}
	else
	{
		StringChange::CStringToChar(_T("UpLoadMutex"),temp);
		UPLOAD_LOG_INFO("Create Progress %s",temp);
		::CreateMutex(0, FALSE, m_strMutex);
		return FALSE;
	}
}
// 处理命令行
// 返回值为 TRUE 表示要继续运行后面的程序，为 FALSE 表示终止程序运行
BOOL CUpLoadLogApp::ParseCommandLine()
{
	CString sParameter = _T("");
	// 检查命令行参数
	for (int i = 1; i < __argc; i++)
	{
		sParameter = __wargv[i];
		if(sParameter.Left(wcslen(PARA_KEY_CHECLOGPATH)).CompareNoCase(PARA_KEY_CHECLOGPATH) == 0)
		{
			m_sLogPath = sParameter.Mid(wcslen(PARA_KEY_CHECLOGPATH));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECKURL)).CompareNoCase(PARA_KEY_CHECKURL) == 0)
		{
			m_sUrl = sParameter.Mid(wcslen(PARA_KEY_CHECKURL));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECFLAG)).CompareNoCase(PARA_KEY_CHECFLAG) == 0)
		{
			m_iFlag = sParameter.Mid(wcslen(PARA_KEY_CHECFLAG));
		}
		else if(sParameter.Left(wcslen(PARA_KEY_CHECAPPNAME)).CompareNoCase(PARA_KEY_CHECAPPNAME) == 0)
		{
			m_sAppName = sParameter.Mid(wcslen(PARA_KEY_CHECAPPNAME));
		}
		else
		{
			char temp[MAX_PATH] = {0};
			StringChange::CStringToChar(sParameter,temp);
			UPLOAD_LOG_ERROR("Invalid parameter : %s", temp);
			return FALSE;
		}
	}
	return TRUE;
}

void CUpLoadLogApp::CloseProgram(CString strProgram)
{
	HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄 
	HANDLE handle1; //定义要结束进程句柄 
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄 
	PROCESSENTRY32 *info; //定义PROCESSENTRY32结构字指 
	//PROCESSENTRY32 结构的 dwSize 成员设置成 sizeof(PROCESSENTRY32) 

	info = new PROCESSENTRY32; 
	info->dwSize = sizeof(PROCESSENTRY32); 
	//调用一次 Process32First 函数，从快照中获取进程列表 
	Process32First(handle, info); 
	//重复调用 Process32Next，直到函数返回 FALSE 为止 
	while(Process32Next(handle, info) != FALSE) 
	{   
		CString strTmp = info->szExeFile;     //指向进程名字   
		//strcmp字符串比较函数同要结束相同   
		//if(strcmp(c, info->szExeFile) == 0 )   
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//PROCESS_TERMINATE表示为结束操作打开,FALSE=可继承,info->th32ProcessID=进程ID    
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID); 
			//结束进程 
			TerminateProcess(handle1, 0);    
		}   
	}
	delete info;
	CloseHandle(handle);
}