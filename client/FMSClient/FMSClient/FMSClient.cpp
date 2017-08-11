
// FMSClient.cpp : ����Ӧ�ó��������Ϊ��
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
const TCHAR* MODULE_CONFIG_PATH  = _T("Config\\ModuleConfig.xml");      // ģ�������ļ�λ��
const TCHAR* FUI_RES_PASSWORD = _T("Hik_Financial_8100");               // Fui��Դ����

#define CMD_ARGS_NUM 8      // �����в�������
#define ID_MODULE_WND 1001  // ģ�鴰��ID

// �����¼�߼�
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


// CFMSClientApp ����

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
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFMSClientApp ����

CFMSClientApp theApp;


// CFMSClientApp ��ʼ��

BOOL CFMSClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//��ȡ����·��������
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strAppName = szPath;

	//��ȡ�汾
	GetVerValue(_T("FileVersion"), m_strAppVer);
    //������Ϣ
    GetVerValue(_T("FileDescription"), m_strFileDescription);

	//����Ŀ¼
	(_tcsrchr(szPath, _T('\\')))[0] = 0;
	m_strAppPath = szPath;
	m_strDefaultCapPath = m_strAppPath + _T("\\RecordFiles\\");

	InitRunConfig();

	BOOL bLoadModule = FALSE;  //�Ƿ���ص�����ģ��
	BOOL bReLogin = FALSE;         //�ͻ����Ƿ���������
	BOOL bForceLogoff = FALSE;   //�ͻ����Ƿ�ǿ������

	//�����в���
	int nArgs = 0;
	LPWSTR *szArgsList = CommandLineToArgvW(GetCommandLine(), &nArgs);
	CString strLibConfig;  //Ԥ���������ļ���
	STU_COMMAND_PARAM strCommandParam;
	if (CMD_ARGS_NUM == nArgs)
	{
		bLoadModule = TRUE;  //���������в��������ص�����ģ��
		//......
	}
	else
	{
		//ͨ���������ж��Ƿ�ͻ����������̻�ǿ������
		if (1 == nArgs && 0 == _tcscmp(szArgsList[0], _T("reboot")))
		{
			bReLogin = TRUE;
		}
		else if (1 == nArgs && 0 == _tcscmp(szArgsList[0], _T("forcelogoff")))
		{
			bReLogin = TRUE;
			bForceLogoff = TRUE;
		}

		//�ͻ��˿�ܼ����˿�
		m_nListenPort = GetPrivateProfileInt(_T("FMSClient"), _T("ListenPort"), 27200, szPath);

		TCHAR szPreload[MAX_PATH] = {0};
		GetPrivateProfileString(_T("FMSClient"), _T("PreloadFile"), _T("Config\\preload.xml"), szPreload, MAX_PATH, szPath);
		strLibConfig = szPreload;
	}

	//Ԥ���ؿ��ļ�
	CPublicLibraryMgr publicLibMgr(strLibConfig);

	//��ʼ����
	InitCom();

	// ģ��EXE����
	if (bLoadModule)
	{
// 		m_pProcessWnd = new CProcessWnd(stuCommandParam);
// 		m_pMainWnd = m_pProcessWnd;
// 
// 		// ��ȡ��ܵĴ��ھ��
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
		//������Ψһ
		//m_bClientPlatform = TRUE;
		if (FALSE == CheckInstance(bReLogin))
		{
			//��ʾ�������Ĵ���
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
		//�����ָ����´β���Ч
		::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));

		//��¼ƽ̨
		if (IDOK != ShowLoginDlg(LoginEvent, this, nReboot, m_nSkinType, m_nLanguage))
		{
			return FALSE;
		}
		
		CFMSClientDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȷ�������رնԻ���Ĵ���
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȡ�������رնԻ���Ĵ���
		}
	}
	
	UnInitCom();
	CLIENT_INFO("UninitCom Success");
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CFMSClientApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	return CWinAppEx::ExitInstance();
}

BOOL CFMSClientApp::InitRunConfig()
{
	//��ȡ��������·��
	//AppFun_GetAppPath();
	ReadConfig(m_strAppCapPath);
	//��ȡ�ͻ��˰汾��Ϣ
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CModuleVerInfo ModuleVer(szPath);
	ModuleVer.GetVerValue(_T("FileVersion"), m_strAppVer);

	//�Զ�����
// 	CString strHttp;
// 	strHttp.Format(_T("AppName=FMSClient.exe CurrentVersion=%s CheckURL=http://10.16.36.110:9001/HTTP�ϴ�/UpdateConfig.ini NotifyWindow= NotifyWindowTitle= NotifyFinish= Silence=0"),
// 		m_strAppVer);
// 	ShellExecute(NULL,_T("open"), _T("Updater.exe"), strHttp, NULL, SW_SHOWNORMAL);

	return TRUE;
}

BOOL CFMSClientApp::ReadConfig(CString& m_strAppCapPath)
{
	TCHAR szDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szDir, MAX_PATH);
	PathRemoveFileSpec(szDir); //��·��ĩβ���ļ����ͷ�б��ȥ��
	TCHAR szPath[MAX_PATH] = {0};
	PathCombine(szPath, szDir, CLIENT_CONFIG_FILENAME);
	TCHAR szBuf[1024] = {0};
	GetPrivateProfileString(_T("LocalConfig"), _T("CapPath"), theApp.m_strDefaultCapPath, szBuf, 1024, szPath);
	m_strAppCapPath.Format(_T("%s"), szBuf);

	return TRUE;
}

void CFMSClientApp::InitCom()
{
	//��ʼ��FUiControl
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
	//�����ļ�·��
	CString strConfigPath = m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	//��ʼ������
	m_nLanguage = GetPrivateProfileInt(_T("FMSClient"), _T("Language"), 0, strConfigPath);
	LANG_INIT("CentralClient", m_nLanguage);

	//��ʼ��Ƥ�����
	m_nSkinType = GetPrivateProfileInt(_T("FMSClient"), _T("SkinScheme"), 1, strConfigPath);
	CString strImagePath;
	strImagePath.Format(_T("\\image\\%d.rc"), m_nSkinType);
	FUiSetRes(m_strAppPath + strImagePath, FUI_RES_PASSWORD);

	TCHAR szCustomTitle[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("CustomTitle"), _T("��Ʒ��չ���ϵͳ"), szCustomTitle, MAX_PATH, strConfigPath);
	m_sCustomTitle = szCustomTitle;

	TCHAR szCopyright[MAX_PATH] = {0};
	GetPrivateProfileString(_T("FMSClient"), _T("Copyright"), _T(""), szCopyright, MAX_PATH, strConfigPath);
	m_sCopyright = szCopyright;

	//�Ƿ�Ϊ���ε�¼�����ε�¼�޸�Ĭ������Ϊ��ǰ�������е�·����
	//�ǳ��ε�¼����ȡ�����ļ��������õ�·����Ϣ��
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

	//Ĭ�����
	m_nCamIndex = GetPrivateProfileInt(_T("LocalConfig"), _T("CamIndex"), 0, strConfigPath);
	FMSCLIENT_INFO("APP Cfg:m_nCamIndex:%d", m_nCamIndex);
}

BOOL CFMSClientApp::ShowWndAlreadyExist()
{
	CWnd * pClientWnd = NULL;

	//1.��¼����
	pClientWnd = CWnd::FindWindow(NULL, LOGIN_TITLE);
	if (pClientWnd != NULL)
	{
		pClientWnd->ShowWindow(SW_RESTORE);
		pClientWnd->SetForegroundWindow();
		return TRUE;
	}

	//2.�Ѿ���¼�ɹ�����ʾ������
	pClientWnd = CWnd::FindWindow(NULL, m_sCustomTitle);
	if (pClientWnd != NULL)
	{
		pClientWnd->ShowWindow(SW_RESTORE);
		pClientWnd->SetForegroundWindow();
		return TRUE;
	}

	//������ʹ�ð�ĺ�׺
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

	// ���û�������URL����
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
