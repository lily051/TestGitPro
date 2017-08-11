
// Updater.cpp : ����Ӧ�ó��������Ϊ��
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


// ȫ���ַ������������飨�����ʼ����InitStringTable()�����н��У�
struct StringStru g_String[STRING_BOTTOM];

// ȫ�����Դ��루���������޸���ֵ������ᵼ�³����쳣����
enum enLANGUAGE g_LanguageID;


// CUpdaterApp

BEGIN_MESSAGE_MAP(CUpdaterApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpdaterApp ����

CUpdaterApp::CUpdaterApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_bSilenceMode = FALSE;
	m_UpdateOk = TRUE;
}


// Ψһ��һ�� CUpdaterApp ����

CUpdaterApp theApp;


// CUpdaterApp ��ʼ��

BOOL CUpdaterApp::InitInstance()
{

	// ����������
	if (!ParseCommandLine())
	{
		return FALSE;
	}

	// �������Ƿ��Ѿ������У�������ֱ���˳�
	if (IsAppRunning())
	{
		return FALSE;
	}

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
	
	// ��ʾ������
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

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

// ����������
// ����ֵΪ TRUE ��ʾҪ�������к���ĳ���Ϊ FALSE ��ʾ��ֹ��������
BOOL CUpdaterApp::ParseCommandLine()
{
	CString sParameter = _T("");
	char cParameter[MAX_PATH] = {0};
	// ��������в���
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

	m_sAppName = _T("Tool"); //��������

	if (m_UploadClientName.IsEmpty()|| m_FMSClientName.IsEmpty())
	{
		UPDATER_LOG_ERROR("exe 's  name is empty!");
		m_UpdateOk = FALSE;
		return FALSE;
	}

	// ������������ļ����ж��Ƿ����°汾���������
	if (CheckUpdate())
	{

		if (!m_bSilenceMode)
		{
			//CString strCaption = m_sAppName + _T("Updater");
			CString strCaption = _T("��������...");
			CString strText = _T("��ǰ�и��°汾��������ã��Ƿ�����������");
			if (IDOK == MessageBox(NULL, strText, strCaption, MB_ICONQUESTION|MB_OKCANCEL))
			{
				// �����������˳������򣬱�֤��ȫ˳������
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
				UPDATER_LOG_INFO("Cancel update��delete the ini files");
				//ȡ������ɾ�������ļ�
				//DeleteFile(file_info::GetAppDirectory()+_T("LocalFiles.ini")); 
				DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
				return FALSE;
			}
		}
		else if (m_bSilenceMode == 1)
		{
			//��Ĭ����
			m_bSilenceMode = 1;

			UPDATER_LOG_INFO("Start to Update��Close program");
			// �����������˳������򣬱�֤��ȫ˳������
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
		UPDATER_LOG_INFO("the newest��delete the ini files");
		//ɾ�������ļ� 
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/*        ֮ǰ��·���ȶ������������ļ����������ʧ�ܣ���Ӷ�ʱ���                                                              */
/************************************************************************/


// �������վ�������������ļ�������Ƿ����°汾���������
// ����ֵΪ TRUE ��ʾ���°汾���������
BOOL CUpdaterApp::CheckUpdate()
{
	char cIniName[MAX_PATH] = {0};
	CString sConfigFilename = file_info::GetAppDirectory() + UPDATE_CONFIG_FILENAME;

    StringChange::CStringToChar(UPDATE_CONFIG_FILENAME,cIniName);
	UPDATER_LOG_INFO("Start to Download %s",cIniName);
	// ��ָ����URL�������������ļ�
	httpdownload hdl;
	if (!hdl.DownloadFile(CSCT::Tstring2Ansi(m_sURL.GetString()),  CSCT::Unicode2Ansi(sConfigFilename.GetString())))
	{
		// ��¼�����ļ�ʧ����־
		UPDATER_LOG_ERROR("Download ini %s failed",cIniName);
		m_UpdateOk = FALSE;
		return FALSE;
	}
	//��ȡ���������ļ�������ʱ��
	UPDATER_LOG_INFO("Read LocalConfig.ini");
	theApp.strLocalPackageTime = _T("");
	ReadLocalInI(strLocalPackageTime);
	
	//��ȡ���ص������ļ�
	UPDATER_LOG_INFO("Read UpdaterConfig.ini");
	theApp.strURLPackageTime = _T("");
	int nUpdateNumber = 0;

	ReadUrlInI(strURLPackageTime, nUpdateNumber);

	//�ļ�����Ϊ 0 ���� ����������ʱ�� >= Զ��������ʱ��
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
	// ��������������ֹͬʱ�����������ʵ��
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
		CString strTmp = info->szExeFile;     //ָ���������   
		//strcmp�ַ����ȽϺ���ͬҪ������ͬ    
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//���̴���
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
	HANDLE handle; //����CreateToolhelp32Snapshotϵͳ���վ�� 
	HANDLE handle1; //����Ҫ�������̾�� 
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ�� 
	PROCESSENTRY32 *info; //����PROCESSENTRY32�ṹ��ָ 
	//PROCESSENTRY32 �ṹ�� dwSize ��Ա���ó� sizeof(PROCESSENTRY32) 

	info = new PROCESSENTRY32; 
	info->dwSize = sizeof(PROCESSENTRY32); 
	//����һ�� Process32First �������ӿ����л�ȡ�����б� 
	Process32First(handle, info); 
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ 
	while(Process32Next(handle, info) != FALSE) 
	{   
		CString strTmp = info->szExeFile;     //ָ���������   
		//strcmp�ַ����ȽϺ���ͬҪ������ͬ   
		//if(strcmp(c, info->szExeFile) == 0 )   
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info->th32ProcessID=����ID    
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID); 
			//�������� 
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

	//��ȡ������ʱ��
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
	//	//�����ļ���Ϣ�����
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


	//��ȡ������ʱ��
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

		//��Ŀ¼
		strKey = _T("Subcatalog");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(strSection, strKey, _T(""), acBuffer, BUFFER_SIZE, szIniPath);
		filestru->Subcatalog = acBuffer;

		//Url�ļ���Ϣ�����
		m_UrlFilesList.push_back(*filestru);
	}
}

int CUpdaterApp::UpdateFilesNumber(int &number)
{
	//Ѱ����Ҫ���µ��ļ�������ȡ��װĿ¼
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
	// ȡ�ù����ļ������ļ���С�ܺ�
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
 *  @brief    ��������
 *  @param    (CString) cmd [IN] : ShellExecute������
 *            (CString) DefaultDir [IN] : Ĭ�ϵ�����Ŀ¼
 *  @return:  void
 */
BOOL CUpdaterApp::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // ��־
	CString strDebug = _T("");
	strDebug.Format(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);
	OutputDebugString(strDebug);

	  // ִ������
    //HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, NULL, NULL, SW_RESTORE);
	//int a = (int)hInstance;

	UINT hInstance = WinExec("Wscript.exe //H:Wscript restart.vbs ",SW_SHOW);


    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        AfxMessageBox(_T("�ڴ治��"));
        break;
   /* case ERROR_FILE_NOT_FOUND:
       AfxMessageBox(_T("�ļ�����·��������"));
        break;
    case ERROR_PATH_NOT_FOUND:
        AfxMessageBox(_T("�ļ�����·��������"));
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
	//��������
	UINT hInstance = WinExec("Wscript.exe //H:Wscript restart.vbs ",SW_SHOW);
	//RunCmd(runDir, runDir);
}

/** @fn       void CMainDialog::OnIconnotifyStop()
 *  @brief    ֹͣ
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
	//��������
	UINT hInstance = WinExec("Wscript.exe //H:Wscript stop.vbs ",SW_SHOW);
	//RunCmd(runDir, runDir);
}