
// ServiceWatchdog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "MainDialog.h"
#include "./Utilities/CrashAPI.h"

#include "Folder.h"
#include "Markup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServiceWatchdogApp

BEGIN_MESSAGE_MAP(CServiceWatchdogApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

static void crash_callback()
{
    SVCMANAGERLOG_ERROR(_T("Service Manager dump is happened."));
}

// 提升权限。
BOOL EnableDebugPrivilege(BOOL bEnable) 
{ 
    BOOL fOK = FALSE;
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) //打开进程访问令牌
    { 
        //试图修改“调试”特权
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
        tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
        AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        fOK = (GetLastError() == ERROR_SUCCESS);
        CloseHandle(hToken); 
    } 
    return fOK; 
}

// CServiceWatchdogApp construction

CServiceWatchdogApp::CServiceWatchdogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

    m_hMutex = NULL;

    gdiplusToken = 0;

    m_nListenPort = 0;
    //m_nDCPort = 0;

    m_nMsgMaxCount = 0;
    m_nRestartInterval = 0;
	m_nAlarmProcessor = 0;

    //m_bCorrTimeFlag = FALSE;
}

// The one and only CServiceWatchdogApp object

CServiceWatchdogApp theApp;

//// 与CMS之间的Session
//ConnSession* g_pCMSConnSession = NULL;


/** @fn       __int64 GetFileTimeInterval(const FILETIME&  PreTime, const FILETIME& CurTime)
 *  @brief    计算两个FILETIME之间的间隔(100ns)
 *  @param
 *  @return:  __int64：返回的时间间隔(100ns)
 */
__int64 GetFileTimeInterval(const FILETIME& PreTime, const FILETIME& CurTime)
{
    __int64 pre = (__int64) ( ((__int64)PreTime.dwHighDateTime<<32) | PreTime.dwLowDateTime);
    __int64 cur = (__int64) ( ((__int64)CurTime.dwHighDateTime<<32) | CurTime.dwLowDateTime);
    return  (cur - pre);
}

// CServiceWatchdogApp initialization

BOOL CServiceWatchdogApp::InitInstance()
{
    // 同一个程序只运行一个实例
    CStringA appMutexName("Global\\Service Manager");
    m_hMutex = CreateMutex(NULL, FALSE, appMutexName.GetBuffer(0));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        AfxMessageBox(_T("服务管理中心只能启动一个"));
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

    //HLOG_INIT_SYNC_TVWALL();
//     BLOCK_LOG_OBJECT(SVCMANAGERLOG_OBJECT_NAME);
//     BLOCK_LOG_OBJECT(SVCCONTROLLOG_OBJECT_NAME);
//     BLOCK_LOG_OBJECT(VIEWMSGLOG_OBJECT_NAME);

    SVCMANAGERLOG_INFO("Start!");
    SVCCONTROLLOG_INFO("Start!");
    VIEWMSGLOG_INFO("Start!");

    // 提升权限。
    if (FALSE == EnableDebugPrivilege(TRUE))
    {
        SVCMANAGERLOG_WARN(_T("EnableDebugPrivilege failed."));
    }

    // 写入注册表，开机自运行
    CStringA exeFileName = CFolder::GetExecutionFileName();
    LPCTSTR lpszKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    CRegKey regeditKey;
    if (ERROR_SUCCESS == regeditKey.Open(HKEY_LOCAL_MACHINE, lpszKey))
    {
       // LONG lrtn = 
        regeditKey.SetStringValue("Service Manager", exeFileName.GetBuffer(0));
        regeditKey.Close();
    }

    GdiplusStartupInput gdiplusStartupInput;
    /*ULONG_PTR gdiplusToken;*/
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    m_startTime = COleDateTime::GetCurrentTime();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // Init local DB config
    //InitLocalDBConfig();
// 	if (!fcDB_MySQL_Init())
// 	{
// 		AfxMessageBox(_T("初始化MYSQL库失败!"));
// 		return FALSE;
// 	}

    // Init MsgMaxCount、RestartInterval
    InitParamInfo();
    SVCMANAGERLOG_INFO("MsgMaxCount:%d ; RestartInterval:%d", m_nMsgMaxCount, m_nRestartInterval);

    /* crash dump */
    if (CrashAPI_Init())
    {
        CString exeFileDir = CFolder::GetExecutionRoot();
        exeFileDir.AppendFormat(_T("dumpfile\\"));

        CrashAPI_SetDumpPath((LPTSTR)(LPCTSTR)exeFileDir);
        CrashAPI_SetCallBack(crash_callback);
    }
//lint -e1414
	//CServiceWatchdogDlg dlg;
    CMainDialog dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
//lint +e1414
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CServiceWatchdogApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    SVCMANAGERLOG_INFO("-------------------------------------Stop-----------------------------------------");
    SVCCONTROLLOG_INFO("-------------------------------------Stop-----------------------------------------");
    VIEWMSGLOG_INFO("-------------------------------------Stop-----------------------------------------");

	//fcDB_MySQL_Fini();

    GdiplusShutdown(gdiplusToken);

    // 同一个程序只运行一个实例
    if (NULL != m_hMutex)
    {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }

    CrashAPI_Uninit();

    //::google::protobuf::ShutdownProtobufLibrary();

    //HLOG_FINI();
    return CWinApp::ExitInstance();
}

///** @fn       BOOL CServiceWatchdogApp::InitCMSAndLocal(void)
// *  @brief    初始化中心服务和本地的IP及端口
// *  @param    void
// *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
// */
//BOOL CServiceWatchdogApp::InitLocalDBConfig(void)
//{
//    CMarkup xmlInfo;
//    xmlInfo.SetDoc(CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + SVC_DBCFG_FILE));
//
//    if (xmlInfo.FindElem(XML_DB_ROOT) && xmlInfo.IntoElem())
//    {
//        // DB Host
//        if (xmlInfo.FindElem(XML_DB_SERVER))
//        {
//            m_localDBhost = xmlInfo.GetData();
//        }
//
//        // DB Port
//        if (xmlInfo.FindElem(XML_DB_SERVER_PORT))
//        {
//            m_localDBPort = atoi(xmlInfo.GetData());
//        }
//
//        // DB user
//        if (xmlInfo.FindElem(XML_DB_SERVER_USER))
//        {
//            m_localDBUser = xmlInfo.GetData();
//        }
//
//        // DB password
//        if (xmlInfo.FindElem(XML_DB_SERVER_PASSWORD))
//        {
//			m_localDBPassword = xmlInfo.GetData();
//        }
//
//		// DB schema
//		if (xmlInfo.FindElem(XML_DB_SERVER_SCHEMA))
//		{
//			m_localDBSchema = xmlInfo.GetData();
//		}
//    }
//
//    return TRUE;
//}

/** @fn       BOOL CServiceWatchdogApp::InitParamInfo(void)
 *  @brief    获取MsgMaxCount和RestartInterval
 *  @param    void
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CServiceWatchdogApp::InitParamInfo(void)
{
    m_nMsgMaxCount = 500;   // 默认为500条信息
    m_nRestartInterval = 5; // 默认关机超时时间为5s

    CMarkup xmlInfo;
    CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");
    if (strcmp(fileContent, _T("")) == 0)
    {
        return TRUE;
    }
 
    if (xmlInfo.SetDoc(fileContent))
    {
        if (xmlInfo.FindElem(_T("Config")) && xmlInfo.IntoElem())
        {
            // 查找MSM MAX COUNT元素
            if (xmlInfo.FindElem(_T("MSM_MAX_COUNT")))
            {
                m_nMsgMaxCount = atoi(xmlInfo.GetData());
            }

            if (xmlInfo.FindElem(_T("SHUT_DOWN_INTERVAL")))
            {
                m_nRestartInterval = atoi(xmlInfo.GetData());
            }

			if (xmlInfo.FindElem(_T("Processor")))
			{
				m_nAlarmProcessor = atoi(xmlInfo.GetData());
			}
        }
    }

    /* 消息显示的条数10~3000 */
    if (m_nMsgMaxCount < 10)
    {
        m_nMsgMaxCount = 10;
    }
    else if (m_nMsgMaxCount > 3000)
    {
        m_nMsgMaxCount = 3000;
    }

    /* 等待超时时间为1s~60s */
    if (m_nRestartInterval < 1)
    {
        m_nRestartInterval = 1;
    }
    else if (m_nRestartInterval > 60)
    {
        m_nRestartInterval = 60;
    }
//     CString csMsg;
//     csMsg.Format(_T("m_nMsgMaxCount[%d] m_nRestartInterval[%d] m_bCheckTimeFlag[%d]\n"), m_nMsgMaxCount, m_nRestartInterval, m_bCorrTimeFlag);
//     OutputDebugString(csMsg);
    return TRUE;
}
