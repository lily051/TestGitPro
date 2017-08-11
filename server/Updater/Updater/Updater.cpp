
// Updater.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "httpdownload.h"
#include "Updater.h"
#include "UpdaterDlg.h"
#include "MakeIni.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 全局字符串常量表数组（数组初始化在InitStringTable()函数中进行）
struct StringStru g_String[STRING_BOTTOM];

// 全局语言代码（不可随意修改其值，否则会导致程序异常！）
enum enLANGUAGE g_LanguageID;


// CUpdaterApp

BEGIN_MESSAGE_MAP(CUpdaterApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpdaterApp 构造

CUpdaterApp::CUpdaterApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_bSilenceMode = FALSE;
	m_UpdateOk = TRUE;
}


// 唯一的一个 CUpdaterApp 对象

CUpdaterApp theApp;


// CUpdaterApp 初始化

BOOL CUpdaterApp::InitInstance()
{

	// 处理命令行
	if (!ParseCommandLine())
	{
		return FALSE;
	}

	// 检查程序是否已经在运行，如是则直接退出
	if (IsAppRunning())
	{
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
	
	// 显示主窗口
	CUpdaterDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_sAppName = m_sAppName;
	dlg.m_bSilenceMode = m_bSilenceMode;
	
	int nResponse = dlg.DoModal();
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

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

// 处理命令行
// 返回值为 TRUE 表示要继续运行后面的程序，为 FALSE 表示终止程序运行
BOOL CUpdaterApp::ParseCommandLine()
{
	CString sParameter = _T("");
	char cParameter[MAX_PATH] = {0};
	// 检查命令行参数
	for (int i = 1; i < __argc; i++)
	{
		sParameter = __wargv[i];
		if (sParameter.Left(wcslen(PARA_KEY_UPLOADCLIENT_NAME)).CompareNoCase(PARA_KEY_UPLOADCLIENT_NAME) == 0)
		{
			m_UploadClientName = sParameter.Mid(wcslen(PARA_KEY_UPLOADCLIENT_NAME));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_FMSCLIENT_NAME)).CompareNoCase(PARA_KEY_FMSCLIENT_NAME) == 0)
		{
			m_FMSClientName = sParameter.Mid(wcslen(PARA_KEY_FMSCLIENT_NAME));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_CURRENTVERSION)).CompareNoCase(PARA_KEY_CURRENTVERSION) == 0)
		{
			m_sVersion = sParameter.Mid(wcslen(PARA_KEY_CURRENTVERSION));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_CHECKURL)).CompareNoCase(PARA_KEY_CHECKURL) == 0)
		{
			m_sURL = sParameter.Mid(wcslen(PARA_KEY_CHECKURL));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_NOTIFYWINDOW)).CompareNoCase(PARA_KEY_NOTIFYWINDOW) == 0)
		{
			m_iNotifyWindow = (DWORD)_wtoi(sParameter.Mid(wcslen(PARA_KEY_NOTIFYWINDOW)));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_NOTIFYWINDOWTITLE)).CompareNoCase(PARA_KEY_NOTIFYWINDOWTITLE) == 0)
		{
			m_sNotifyWindowTitle = sParameter.Mid(wcslen(PARA_KEY_NOTIFYWINDOWTITLE));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_NOTIFYFINISH)).CompareNoCase(PARA_KEY_NOTIFYFINISH) == 0)
		{
			m_iNotifyFinish = (DWORD)_wtoi(sParameter.Mid(wcslen(PARA_KEY_NOTIFYFINISH)));
		}
		else if (sParameter.Left(wcslen(PARA_KEY_SILENCE)).CompareNoCase(PARA_KEY_SILENCE) == 0)
		{
			m_bSilenceMode = (sParameter.Mid(wcslen(PARA_KEY_SILENCE)) == "1");
		}
		else
		{
			StringChange::CStringToChar(sParameter,cParameter);
			UPDATER_LOG_ERROR("Geting the Cmd is failed,Cmd is %s",cParameter);
			m_UpdateOk = FALSE;
			return FALSE;
		}
	}

	m_UploadClientName.TrimLeft();
	m_UploadClientName.TrimRight();
	m_FMSClientName.TrimLeft();
	m_FMSClientName.TrimRight();

	m_sAppName = _T("Tool"); //工具名称

	if (m_UploadClientName.IsEmpty()|| m_FMSClientName.IsEmpty())
	{
		UPDATER_LOG_ERROR("exe 's  name is empty!");
		m_UpdateOk = FALSE;
		return FALSE;
	}

	// 检查升级配置文件，判断是否有新版本的软件可用
	if (CheckUpdate())
	{

		if (!m_bSilenceMode)
		{
			//CString strCaption = m_sAppName + _T("Updater");
			CString strCaption = _T("正在升级...");
			CString strText = _T("当前有更新版本的软件可用，是否立即升级？");
			if (IDOK == MessageBox(NULL, strText, strCaption, MB_ICONQUESTION|MB_OKCANCEL))
			{
				// 进入升级，退出主程序，保证安全顺利升级
				OnIconnotifyStop();
				char temp[MAX_PATH] = {0};
				CloseProgram(m_UploadClientName + _T(".exe"));
				StringChange::CStringToChar(m_UploadClientName + _T(".exe"),temp);
				UPDATER_LOG_INFO("Close UploadClient progress %s ",temp);

				while(IsHaveProgram(m_UploadClientName+_T(".exe")))
				{
					Sleep(1000);
				}
				UPDATER_LOG_INFO("Close UploadClient progress is successful");


				CloseProgram(m_FMSClientName + _T(".exe"));
				StringChange::CStringToChar(m_FMSClientName + _T(".exe"),temp);
				UPDATER_LOG_INFO("Close FMSClient progress %s ",temp);
				while(IsHaveProgram(m_FMSClientName+_T(".exe")))
				{
					Sleep(1000);
				}
				UPDATER_LOG_INFO("Close FMSClientName progress is successful");
				return TRUE;
			}
			else
			{
				UPDATER_LOG_INFO("Cancel update，delete the ini files");
				//取消升级删除配置文件
				//DeleteFile(file_info::GetAppDirectory()+_T("LocalFiles.ini")); 
				DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
				return FALSE;
			}
		}
		else if (m_bSilenceMode == 1)
		{
			//静默升级
			m_bSilenceMode = 1;

			UPDATER_LOG_INFO("Start to Update，Close program");
			// 进入升级，退出主程序，保证安全顺利升级
			OnIconnotifyStop();
			char temp[MAX_PATH] = {0};
			CloseProgram(m_UploadClientName + _T(".exe"));
			StringChange::CStringToChar(m_UploadClientName + _T(".exe"),temp);
			UPDATER_LOG_INFO("Close UploadClient progress %s ",temp);

			while(IsHaveProgram(m_UploadClientName+_T(".exe")))
			{
				Sleep(1000);
			}
			UPDATER_LOG_INFO("Close UploadClient progress is successful");


			CloseProgram(m_FMSClientName + _T(".exe"));
			StringChange::CStringToChar(m_FMSClientName + _T(".exe"),temp);
			UPDATER_LOG_INFO("Close FMSClient progress %s ",temp);
			while(IsHaveProgram(m_FMSClientName+_T(".exe")))
			{
				Sleep(1000);
			}
			UPDATER_LOG_INFO("Close FMSClientName progress is successful");

		}
	}
	else
	{
		UPDATER_LOG_INFO("the newest，delete the ini files");
		//删除配置文件 
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/*        之前网路不稳定，下载配置文件，检测升级失败，添加定时检测                                                              */
/************************************************************************/


// 从软件网站下载升级配置文件，检查是否有新版本的软件可用
// 返回值为 TRUE 表示有新版本的软件可用
BOOL CUpdaterApp::CheckUpdate()
{
	char cIniName[MAX_PATH] = {0};
	CString sConfigFilename = file_info::GetAppDirectory() + UPDATE_CONFIG_FILENAME;

    StringChange::CStringToChar(UPDATE_CONFIG_FILENAME,cIniName);
	UPDATER_LOG_INFO("Start to Download %s",cIniName);
	// 从指定的URL下载升级配置文件
	httpdownload hdl;
	if (!hdl.DownloadFile(CSCT::Tstring2Ansi(m_sURL.GetString()),  CSCT::Unicode2Ansi(sConfigFilename.GetString())))
	{
		// 记录下载文件失败日志
		UPDATER_LOG_ERROR("Download ini %s failed",cIniName);
		m_UpdateOk = FALSE;
		return FALSE;
	}
	//读取本地配置文件升级包时间
	UPDATER_LOG_INFO("Read LocalConfig.ini");
	theApp.strLocalPackageTime = _T("");
	ReadLocalInI(strLocalPackageTime);
	
	//读取下载的配置文件
	UPDATER_LOG_INFO("Read UpdaterConfig.ini");
	theApp.strURLPackageTime = _T("");
	int nUpdateNumber = 0;

	ReadUrlInI(strURLPackageTime, nUpdateNumber);

	//文件个数为 0 或者 本地升级包时间 >= 远程升级包时间
	if ( 0 == nUpdateNumber || 
		strURLPackageTime.CompareNoCase(strLocalPackageTime) <= 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CUpdaterApp::IsAppRunning()
{
	char cMutexName[MAX_PATH] = {0};
	// 创建互斥量，防止同时启动多个程序实例
	CString sMutex = _T("AutoUpdateMutex");
	//CString sMutex = m_sAppName + _T("AutoUpdateMutex");
	StringChange::CStringToChar(sMutex,cMutexName);
	if (::OpenMutex(MUTEX_ALL_ACCESS, FALSE, sMutex))
	{
		UPDATER_LOG_INFO("%s has been Opened",cMutexName);
		return TRUE;
	}
	else
	{
		UPDATER_LOG_INFO("Create Mutex %s",cMutexName);
		::CreateMutex(0, FALSE, sMutex);
		return FALSE;
	}
}
void CUpdaterApp::RunFMSClient()
{

	ShellExecute(NULL, _T("open"), m_FMSClientName,
		NULL, NULL, SW_SHOWNORMAL);
}
BOOL CUpdaterApp::IsHaveProgram(CString strProgram)
{
	HANDLE handle; 
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 *info;
	info = new PROCESSENTRY32; 
	info->dwSize = sizeof(PROCESSENTRY32); 
	Process32First(handle, info); 
	while(Process32Next(handle, info) != FALSE) 
	{   
		CString strTmp = info->szExeFile;     //指向进程名字   
		//strcmp字符串比较函数同要结束相同    
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//进程存在
			UPDATER_LOG_INFO("progress is running");

			return TRUE;
		}   
	}
	delete info;
	CloseHandle(handle);
	return FALSE;
}
void CUpdaterApp::CloseProgram(CString strProgram)
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
			UPDATER_LOG_INFO("Start to Stop progress");
			TerminateProcess(handle1, 0); 
		}   
	}
	delete info;

	CloseHandle(handle);
}

void CUpdaterApp::ReadLocalInI(CString& strLocalPackageTime)
{
	CString szIniPath = file_info::GetAppDirectory()+_T("LocalFiles.ini");  
	CString strSection;
	CString strKey;
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];

	//读取升级包时间
	strKey = _T("Time");
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(_T("COMMON"), strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	strLocalPackageTime = acBuffer;
	//FileInfo *filestru;
	//int iFileCount = GetPrivateProfileInt(SECTION_COMMON, _T("FileCount"), 0, szIniPath);
	//for (int i = 1; i <= iFileCount; i++)
	//{
	//	filestru = new FileInfo;
	//	strSection.Format(_T("CommonFile%d"), i);

	//	filestru->nFileCount = i;

	//	strKey = _T("Name");
	//	memset(acBuffer, 0, BUFFER_SIZE);
	//	GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	//	filestru->strName = acBuffer;

	//	strKey = _T("Version");
	//	memset(acBuffer, 0, BUFFER_SIZE);
	//	GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	//	filestru->strVersion = acBuffer;

	//	strKey = _T("Path");
	//	memset(acBuffer, 0, BUFFER_SIZE);
	//	GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	//	filestru->strPath = acBuffer;
	//	//本地文件信息入队列
	//	m_LocalFilesList.push_back(*filestru);
	//}
}
void CUpdaterApp::ReadUrlInI(CString& strTime, int& nUpdateNumber)
{
	CString szIniPath = file_info::GetAppDirectory()+_T("UpdateConfig.ini");

	CString strSection;
	CString strKey;
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];


	//读取升级包时间
	strKey = _T("Time");
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(_T("UPDATE"), strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
	strTime = acBuffer;


	FileInfo *filestru;
	int iFileCount = GetPrivateProfileInt(SECTION_COMMON, _T("FileCount"), 0, szIniPath);
	nUpdateNumber = iFileCount;

	for (int i = 1; i <= iFileCount; i++)
	{
		filestru = new FileInfo;
		strSection.Format(_T("CommonFile%d"), i);
		filestru->nFileCount = i;

		strKey = _T("Name");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->strName = acBuffer;

		/*strKey = _T("Version");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->strVersion = acBuffer;*/

		strKey = _T("URL");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->Url = acBuffer;

		strKey = _T("Size");
		memset(acBuffer, 0, BUFFER_SIZE);
		filestru->size = GetPrivateProfileInt(strSection, strKey, 0, szIniPath);

		strKey = _T("Hash");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->Hash = acBuffer;

		//子目录
		strKey = _T("Subcatalog");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->Subcatalog = acBuffer;

		//Url文件信息入队列
		m_UrlFilesList.push_back(*filestru);
	}
}

int CUpdaterApp::UpdateFilesNumber(int &number)
{
	//寻找需要更新的文件，并获取安装目录
	for(int i = 0;i < theApp.m_LocalFilesList.size();i++)
	{
		ULONGLONG size;
		CString strLocalName,strLocalVersion;
		strLocalName = theApp.m_LocalFilesList.at(i).strName;
		strLocalVersion = theApp.m_LocalFilesList.at(i).strVersion;
		size = theApp.m_LocalFilesList.at(i).size;
		for(int j = 0;j < theApp.m_UrlFilesList.size(); j++)
		{
			CString strName,strVersion;
			strName = theApp.m_UrlFilesList.at(j).strName;
			strVersion = theApp.m_UrlFilesList.at(j).strVersion;

			char temp[MAX_PATH],ctemp[MAX_PATH];
			

			if (strLocalName.CompareNoCase(strName) == 0)
			{

				StringChange::CStringToChar(strLocalName,ctemp);
				StringChange::CStringToChar(strName,temp);
				UPDATER_LOG_INFO("-----------------------------------------");
				UPDATER_LOG_INFO("Local'file %s %s(newest)",ctemp,temp);
				
				StringChange::CStringToChar(strLocalVersion,ctemp);
				StringChange::CStringToChar(strVersion,temp);
				if(strLocalVersion.CompareNoCase(strVersion) > 0)
				{
					UPDATER_LOG_INFO("Local'version %s > %s",ctemp,temp);
				}else if(strLocalVersion.CompareNoCase(strVersion) == 0)
				{
					UPDATER_LOG_INFO("Local'version %s = %s",ctemp,temp);
				}else if(strLocalVersion.CompareNoCase(strVersion) < 0)
				{

					UPDATER_LOG_INFO("Local'version %s < %s",ctemp,temp);
				}
			}


			if ( (strLocalName.CompareNoCase(strName) == 0
				&& strLocalVersion.CompareNoCase(strVersion) < 0))
				/*||
				(strLocalName.CompareNoCase(strName) == 0
				&& strLocalVersion.CompareNoCase(strVersion) == 0 
				&& size < theApp.m_UrlFilesList.at(j).size))*/
			{
				StringChange::CStringToChar(strName,ctemp);
				UPDATER_LOG_INFO("Need Update %s ",ctemp);
				theApp.m_UrlFilesList.at(j).nFlag = NEED_UPDATE;
				theApp.m_UrlFilesList.at(j).Subcatalog = theApp.m_LocalFilesList.at(i).strPath;
			}
		}
	}
	// 取得公共文件数及文件大小总和
	for(int i = 0;i < theApp.m_UrlFilesList.size(); i++)
	{
		if(theApp.m_UrlFilesList.at(i).nFlag == NEED_UPDATE)
		{
			number++;
		}
	}
	return number;
}

/** @fn       void CConfigToolDlg::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    运行命令
 *  @param    (CString) cmd [IN] : ShellExecute的命令
 *            (CString) DefaultDir [IN] : 默认的运行目录
 *  @return:  void
 */
BOOL CUpdaterApp::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // 日志
	CString strDebug = _T("");
	strDebug.Format(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);
	OutputDebugString(strDebug);

	  // 执行命令
    //HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, NULL, NULL, SW_RESTORE);
	//int a = (int)hInstance;

	UINT hInstance = WinExec("Wscript.exe //H:Wscript restart.vbs ",SW_SHOW);


    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        AfxMessageBox(_T("内存不足"));
        break;
   /* case ERROR_FILE_NOT_FOUND:
       AfxMessageBox(_T("文件名或路径名错误"));
        break;
    case ERROR_PATH_NOT_FOUND:
        AfxMessageBox(_T("文件名或路径名错误"));
        break;*/
    case ERROR_BAD_FORMAT:
        AfxMessageBox(_T("ERROR_BAD_FORMAT"));
        break;
    case SE_ERR_ASSOCINCOMPLETE:
        AfxMessageBox(_T("SE_ERR_ASSOCINCOMPLETE"));
        break;
    case SE_ERR_ACCESSDENIED:
        AfxMessageBox(_T("SE_ERR_ACCESSDENIED"));
        break;
    case SE_ERR_DDEBUSY:
        AfxMessageBox(_T("SE_ERR_DDEBUSY"));
        break;
    case SE_ERR_DDEFAIL:
       AfxMessageBox(_T("SE_ERR_DDEFAIL"));
        break;
    case SE_ERR_DDETIMEOUT:
        AfxMessageBox(_T("SE_ERR_DDETIMEOUT"));
        break;
    case SE_ERR_DLLNOTFOUND:
        AfxMessageBox(_T("SE_ERR_DLLNOTFOUND"));
        break;
    case SE_ERR_NOASSOC:
        AfxMessageBox(_T("SE_ERR_NOASSOC"));
        break;
    case SE_ERR_OOM:
        AfxMessageBox(_T("SE_ERR_OOM"));
        break;
    case SE_ERR_SHARE:
        AfxMessageBox(_T("SE_ERR_SHARE"));
        break;
    default:
        bRet = TRUE;
        break;
    }

    OutputDebugString(_T("---------------------> RunCmd end.\n"));
    return bRet;
}
void CUpdaterApp::OnIconnotifyStart()
{
    //CDialog::OnCancel();
	CString rootPath;
	char temp[MAX_PATH] = {0};
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	runDir.Format(_T("%srestart.vbs"), rootPath);
	StringChange::CStringToChar(runDir,temp);
	UPDATER_LOG_INFO("UploadClient path %s ",temp);
	//运行命令
	UINT hInstance = WinExec("Wscript.exe //H:Wscript restart.vbs ",SW_SHOW);
	//RunCmd(runDir, runDir);
}

/** @fn       void CMainDialog::OnIconnotifyStop()
 *  @brief    停止
 *  @param
 *  @return:  void
 */
void CUpdaterApp::OnIconnotifyStop()
{
    //CDialog::OnCancel();
	CString rootPath;
	char temp[MAX_PATH] = {0};
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	runDir.Format(_T("%srunStop.vbs"), rootPath);
	StringChange::CStringToChar(runDir,temp);
	UPDATER_LOG_INFO("UploadClient path %s ",temp);
	//运行命令
	UINT hInstance = WinExec("Wscript.exe //H:Wscript stop.vbs ",SW_SHOW);
	//RunCmd(runDir, runDir);
}