
// FMSClient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "AppDef.h"
#include "FMSClient.h"
#include "FMSClientDlg.h"
#include "PublicLibraryMgr.h"
#include "LoginDlg.h"
#include "MultiLang/lClient.h"
#include "../Login/Login.h"
#include "LoginCMS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* CLIENT_CONFIG_FILENAME = _T("FMSClient.ini");
const TCHAR* MODULE_CONFIG_PATH  = _T("Config\\ModuleConfig.xml");      // 模块配置文件位置
const TCHAR* FUI_RES_PASSWORD = _T("Hik_Financial_8100");               // Fui资源密码

#define CMD_ARGS_NUM 8      // 命令行参数个数
#define ID_MODULE_WND 1001  // 模块窗口ID

// 处理登录逻辑
BOOL _stdcall LoginEvent(LIB_LOGIN_USER_INFO& loginUserInfo, void *pUser)
{
	CLoginCMS login;
	return login.DoLogin(loginUserInfo);
	return TRUE;
}

// CFMSClientApp

BEGIN_MESSAGE_MAP(CFMSClientApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFMSClientApp 构造

CFMSClientApp::CFMSClientApp()
: m_strAppPath(_T(""))
, m_strAppCapPath(_T(""))
, m_strAppVer(_T(""))
, m_sCustomTitle(_T(""))
, m_nSkinType(0)
, m_nLanguage(0)
, m_nListenPort(27200)
, m_sCopyright(_T(""))
, m_nMonitorCount(0)
, m_nLoginType(0)
, m_nServerPort(0)
, m_strAppIp(_T(""))
, m_sMainTitle(_T(""))
,m_nUserId(-1)
, m_strSalesmanName(_T(""))
, m_strSalesmanCode(_T(""))
,m_strRegionName(_T(""))
,m_strServerIp(_T(""))
, m_strNewFileName(_T(""))
,m_bReboot(false)
,m_hMutex(0)
,m_strDefaultCapPath(_T(""))
, m_nMaxRecordTime(30)
,m_nCamIndex(0)
,m_nFirstLogin(0)
,m_strFileDescription(_T(""))
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFMSClientApp 对象

CFMSClientApp theApp;


// CFMSClientApp 初始化

BOOL CFMSClientApp::InitInstance()
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

	AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	//获取程序路径和名称
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strAppName = szPath;

	//获取版本
	GetVerValue(_T("FileVersion"), m_strAppVer);
    //描述信息
    GetVerValue(_T("FileDescription"), m_strFileDescription);

	//工作目录
	(_tcsrchr(szPath, _T('\\')))[0] = 0;
	m_strAppPath = szPath;
	m_strDefaultCapPath = m_strAppPath + _T("\\RecordFiles\\");

	InitRunConfig();

	BOOL bLoadModule = FALSE;  //是否加载单独的模块
	BOOL bReLogin = FALSE;         //客户端是否重启进程
	BOOL bForceLogoff = FALSE;   //客户端是否被强制下线

	//命令行参数
	int nArgs = 0;
	LPWSTR *szArgsList = CommandLineToArgvW(GetCommandLine(), &nArgs);
	CString strLibConfig;  //预加载配置文件名
	STU_COMMAND_PARAM strCommandParam;
	if (CMD_ARGS_NUM == nArgs)
	{
		bLoadModule = TRUE;  //发现命令行参数，加载单独的模块
		//......
	}
	else
	{
		//通过命令行判断是否客户端重启进程或被强制下线
		if (1 == nArgs && 0 == _tcscmp(szArgsList[0], _T("reboot")))
		{
			bReLogin = TRUE;
		}
		else if (1 == nArgs && 0 == _tcscmp(szArgsList[0], _T("forcelogoff")))
		{
			bReLogin = TRUE;
			bForceLogoff = TRUE;
		}

		//客户端框架监听端口
		m_nListenPort = GetPrivateProfileInt(_T("FMSClient"), _T("ListenPort"), 27200, szPath);

		TCHAR szPreload[MAX_PATH] = {0};
		GetPrivateProfileString(_T("FMSClient"), _T("PreloadFile"), _T("Config\\preload.xml"), szPreload, MAX_PATH, szPath);
		strLibConfig = szPreload;
	}

	//预加载库文件
	CPublicLibraryMgr publicLibMgr(strLibConfig);

	//初始化库
	InitCom();

	// 模块EXE流程
	if (bLoadModule)
	{
// 		m_pProcessWnd = new CProcessWnd(stuCommandParam);
// 		m_pMainWnd = m_pProcessWnd;
// 
// 		// 获取框架的窗口句柄
// 		m_hMainWnd = GetFrameHwnd(stuCommandParam.strFileMapName);
// 		if (NULL == m_hMainWnd)
// 		{
// 			FMSCLIENT_ERROR("GetFrameHwnd failed");
// 			return FALSE;
// 		}
// 
// 		CWnd wndParent;
// 		if (!wndParent.Attach(m_hMainWnd))
// 		{
// 			CLIENT_DEBUG("Attach framewnd failed");
// 			return FALSE;
// 		}
// 		CLIENT_DEBUG("Attach framewnd success");
// 
// 		if (!m_pProcessWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), &wndParent, ID_MODULE_WND))
// 		{
// 			FMSCLIENT_ERROR("Create CInsertWnd failed");
// 			return FALSE;
// 		}
// 		m_pProcessWnd->ShowWindow(SW_SHOWNORMAL);
// 
// 		wndParent.Detach();
		return TRUE;
	}
	else
	{
		//检查进程唯一
		//m_bClientPlatform = TRUE;
		if (FALSE == CheckInstance(bReLogin))
		{
			//显示已启动的窗口
			if (!ShowWndAlreadyExist())
			{
				FMessageBox::Show(APP_ERROR1, MSGBOX_TIPS, MB_OK);
			}
			return FALSE;
		}

		if (bForceLogoff)
		{
			FMessageBox::Show(LOGIN_TIP_FORCELOGOFF, MSGBOX_TIPS, MB_ICONINFORMATION | MB_OK);
		}

		int nReboot = ::GetProfileInt(_T("reboot"), _T("switch"), 1);
		//立即恢复，下次不生效
		::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));

		//登录平台
		if (IDOK != ShowLoginDlg(LoginEvent, this, nReboot, m_nSkinType, m_nLanguage))
		{
			return FALSE;
		}
		
		CFMSClientDlg dlg;
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
	}
	
	UnInitCom();
	CLIENT_INFO("UninitCom Success");
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CFMSClientApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	return CWinAppEx::ExitInstance();
}

BOOL CFMSClientApp::InitRunConfig()
{
	//获取程序运行路径
	//AppFun_GetAppPath();
	ReadConfig(m_strAppCapPath);
	//获取客户端版本信息
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CModuleVerInfo ModuleVer(szPath);
	ModuleVer.GetVerValue(_T("FileVersion"), m_strAppVer);

	//自动升级
// 	CString strHttp;
// 	strHttp.Format(_T("AppName=FMSClient.exe CurrentVersion=%s CheckURL=http://10.16.36.110:9001/HTTP上传/UpdateConfig.ini NotifyWindow= NotifyWindowTitle= NotifyFinish= Silence=0"),
// 		m_strAppVer);
// 	ShellExecute(NULL,_T("open"), _T("Updater.exe"), strHttp, NULL, SW_SHOWNORMAL);

	return TRUE;
}

BOOL CFMSClientApp::ReadConfig(CString& m_strAppCapPath)
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

void CFMSClientApp::InitCom()
{
	//初始化FUiControl
	FUiInit();

	GetConfigInfo();

	Hak1((char*)103, "DATA", NULL);

	if (!ConnSessionMgr::SnglPtr()->Init(m_nListenPort))
	{
		FMSCLIENT_ERROR("ConnSessionMgr Init Failed");
	}

	if (!CNetComm::SnglPtr()->Init())
	{
		FMSCLIENT_ERROR("CNetComm Init Failed");
	}

	if (!ConnSessionMgr::SnglPtr()->StartOutgoing())
	{
		FMSCLIENT_ERROR("ConnSessionMgr StartOutgoing Failed");
	}
}

void CFMSClientApp::UnInitCom()
{
	CLIENT_INFO("UninitCom start...");
	Hak2(NULL, NULL, NULL);

	FUiCleanup();
}

BOOL CFMSClientApp::CheckInstance(BOOL bReboot)
{
	do 
	{
		m_hMutex = CreateMutex(NULL, FALSE, MUTEX_INSTANCE_NAME);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (m_hMutex)
			{
				CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
		}
		else if (NULL != m_hMutex)
		{
			return TRUE;
		}

		Sleep(500);
	} while (bReboot);

	return FALSE;
}

void CFMSClientApp::GetConfigInfo()
{
	//配置文件路径
	CString strConfigPath = m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	//初始化语言
	m_nLanguage = GetPrivateProfileInt(_T("FMSClient"), _T("Language"), 0, strConfigPath);
	LANG_INIT("CentralClient", m_nLanguage);

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

	//是否为初次登录：初次登录修改默认配置为当前程序运行的路径；
	//非初次登录，读取配置文件中已配置的路径信息。
	m_nFirstLogin = GetPrivateProfileInt(_T("LocalConfig"), _T("firstLogin"), 1, strConfigPath);
	if (m_nFirstLogin == 1)
	{
		int bRet = WritePrivateProfileString(_T("LocalConfig"), _T("firstLogin"), _T("0"), strConfigPath);
		if (!bRet)
		{
			DWORD dwError = GetLastError();
			FMSCLIENT_ERROR("Save FirstLogin failed[%u]", dwError);
		}
		bRet = WritePrivateProfileString(_T("LocalConfig"), _T("CapPath"), m_strDefaultCapPath, strConfigPath);
		if (!bRet)
		{
			DWORD dwError = GetLastError();
			FMSCLIENT_ERROR("Save FirstLogin CapPath failed[%u]", dwError);
		}
	}

	//默认相机
	m_nCamIndex = GetPrivateProfileInt(_T("LocalConfig"), _T("CamIndex"), 0, strConfigPath);
	FMSCLIENT_INFO("APP Cfg:m_nCamIndex:%d", m_nCamIndex);
}

BOOL CFMSClientApp::ShowWndAlreadyExist()
{
	CWnd * pClientWnd = NULL;

	//1.登录窗口
	pClientWnd = CWnd::FindWindow(NULL, LOGIN_TITLE);
	if (pClientWnd != NULL)
	{
		pClientWnd->ShowWindow(SW_RESTORE);
		pClientWnd->SetForegroundWindow();
		return TRUE;
	}

	//2.已经登录成功，显示主界面
	pClientWnd = CWnd::FindWindow(NULL, m_sCustomTitle);
	if (pClientWnd != NULL)
	{
		pClientWnd->ShowWindow(SW_RESTORE);
		pClientWnd->SetForegroundWindow();
		return TRUE;
	}

	//可能有使用版的后缀
	CString strTrialTitle = m_sCustomTitle;
	strTrialTitle.Append(TITLEBAR_TITLE_EXT);
	pClientWnd = CWnd::FindWindow(NULL, strTrialTitle);
	if (pClientWnd != NULL)
	{
		pClientWnd->ShowWindow(SW_RESTORE);
		pClientWnd->SetForegroundWindow();
		return TRUE;
	}

	return FALSE;
}

CString CFMSClientApp::GernateUrl()
{
	if (m_strServerIp.IsEmpty())
	{
		return _T("");
	}

	// 对用户名进行URL编码
	CString strUser = fcU2T(theApp.m_pbmsgReqLogin.user_name().c_str());

	int nLength = WideCharToMultiByte(CP_UTF8, 0, strUser, -1, NULL, 0, NULL, NULL);
	TCHAR* strEncode= new TCHAR[nLength * 3];

	int nRet = URLEncode(strUser, strEncode, nLength * 3);
	if (0 == nRet)
	{
		CLIENT_WARN("URLEncode failed");
	}

	strUser = strEncode;
	delete[] strEncode;
	strEncode = NULL;

	//http://10.16.38.22/web/csToChangePwd.action?username=zhoujj 
	CString strUrl;
	strUrl.Format(_T("http://%s/web/csToChangePwd.action?username=%s"), m_strServerIp, strUser);

	return strUrl;
}
