// UpdateThread.cpp : implementation file
//

#include "stdafx.h"
#include "Updater.h"
#include "httpdownload.h"
#include "UpdateThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread

IMPLEMENT_DYNCREATE(CUpdateThread, CWinThread)

CUpdateThread::CUpdateThread()
{
	m_bSilenceMode = FALSE;
	m_bUserBreak = FALSE;
	m_fPercent = 0;
	m_hProgressWindow = NULL;
}

CUpdateThread::~CUpdateThread()
{
}

BOOL CUpdateThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CUpdateThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	m_bUserBreak = TRUE;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUpdateThread, CWinThread)
	//{{AFX_MSG_MAP(CUpdateThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread message handlers

int CUpdateThread::Run() 
{
	BOOL bresult = DoUpdate();
	if (bresult)
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)1);
		}
		return 1;
	}
	else
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)0);
		}
		return 0;
	}

//	return CWinThread::Run();
}
// 执行升级
BOOL CUpdateThread::DoUpdate()
{
	if (m_sConfigFilename.IsEmpty())
	{
		return FALSE;
	}
	UINT iCommonFileCount = 0;
	UINT iLanguageFileCount = 0;
	DOWNLOAD_INFO_STRU DownloadInfo = {0};
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];
	CString sSection;
	CString sKey;
	CString sFilename;
	CString sHash;
	CString sURL;
	// 创建保存升级文件的目录
	CreateDirectory(file_info::GetAppDirectory() + _T("Update"), NULL);

	// 取得公共文件数及文件大小总和
	for(int i = 0;i < theApp.m_UrlFilesList.size(); i++)
	{
		//if(theApp.m_UrlFilesList.at(i).nFlag == NEED_UPDATE)
		{
			iCommonFileCount++;
			DownloadInfo.iFileSize += theApp.m_UrlFilesList.at(i).size;
		}
	}
	DownloadInfo.iFileCount += iCommonFileCount;
    
	// 将文件总数和文件大小总和上报到界面
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_INFO, (LPARAM)&DownloadInfo);
		UPDATER_LOG_INFO("Read to update %d files，size is %d(B)",DownloadInfo.iFileCount,DownloadInfo.iFileSize);
	}

	// 下载所有文件
	memset(&DownloadInfo, 0, sizeof(DOWNLOAD_INFO_STRU));
	
	for(int i = 0;i < theApp.m_UrlFilesList.size(); i++)
	{
		//if(theApp.m_UrlFilesList.at(i).nFlag == NEED_UPDATE)
		{
			if (m_bUserBreak)
			{
				return FALSE;
			}

			sSection.Format(_T("CommonFile%d"), theApp.m_UrlFilesList.at(i).nFileCount);
			if (!DownloadFile(sSection))
			{
				// 文件下载失败，升级失败
				if (!m_bSilenceMode && m_hProgressWindow != NULL)
				{
					sKey = _T("Name");
					memset(acBuffer, 0, BUFFER_SIZE);
					GetPrivateProfileString(sSection, sKey, _T(""), acBuffer, BUFFER_SIZE, m_sConfigFilename);
					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
					
				}
				//UPDATER_LOG_INFO("Start sever");
				////启动开始的程序
				//theApp.OnIconnotifyStart();

				//UPDATER_LOG_INFO("Run Upload sever");
				//theApp.OnIconnotifyStart();//启动上传服务
				//UPDATER_LOG_INFO("Run FMSClient");
				//theApp.RunFMSClient();     //启动CS客户端
				/*CDialog::OnOK();*/
				return FALSE;
			}

			// 将升级进度上报到界面
			DownloadInfo.iFileCount++;
			DownloadInfo.iFileSize += GetPrivateProfileInt(sSection, _T("Size"), 0, m_sConfigFilename);
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADED_INFO, (LPARAM)&DownloadInfo);
			}
		}
	}
	// 下载完毕后校验文件
	for(int i = 0;i < theApp.m_UrlFilesList.size(); i++)
	{
		//if(theApp.m_UrlFilesList.at(i).nFlag == NEED_UPDATE)
		{
			if (m_bUserBreak)
			{
				return FALSE;
			}
			char temp[MAX_PATH] = {0};
			StringChange::CStringToChar(theApp.m_UrlFilesList.at(i).strName,temp);
			sSection.Format(_T("CommonFile%d"), theApp.m_UrlFilesList.at(i).nFileCount);
			UPDATER_LOG_INFO("Start to VerifyFile %s",temp);
			if (!VerifyFile(sSection))
			{
				// 文件校验不通过，升级失败
				if (!m_bSilenceMode && m_hProgressWindow != NULL)
				{
					sKey = _T("Name");
					memset(acBuffer, 0, BUFFER_SIZE);
					GetPrivateProfileString(sSection, sKey, _T(""), acBuffer, BUFFER_SIZE, m_sConfigFilename);
					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_VERIFY_FILE_FAIL, (LPARAM)acBuffer);
					UPDATER_LOG_ERROR("VerifyFile %s is failed",temp);
					theApp.m_UpdateOk = FALSE;
				}
				//UPDATER_LOG_INFO("Start sever");
				////启动开始的程序
				//theApp.OnIconnotifyStart();
				//UPDATER_LOG_INFO("Run Upload sever");
				//theApp.OnIconnotifyStart();//启动上传服务
				//UPDATER_LOG_INFO("Run FMSClient");
				//theApp.RunFMSClient();     //启动CS客户端
				//CDialog::OnOK();
				return FALSE;
			}
			else
			{
				UPDATER_LOG_INFO("VerifyFile %s succefully",temp);
			}
		}
	}

	// 复制、更新文件
	// 创建备份文件目录
	CreateDirectory(file_info::GetAppDirectory() + _T("Backup"), NULL);
	// 循环更新所有文件
	for(int i = 0;i < theApp.m_UrlFilesList.size(); i++)
	{
		//if(theApp.m_UrlFilesList.at(i).nFlag == NEED_UPDATE)
		{
			if (m_bUserBreak)
			{
				return FALSE;
			}
			char temp[MAX_PATH] = {0};
			StringChange::CStringToChar(theApp.m_UrlFilesList.at(i).strName,temp);
			sSection.Format(_T("CommonFile%d"), theApp.m_UrlFilesList.at(i).nFileCount);
			if (!UpdateFile(theApp.m_UrlFilesList.at(i)))
			{
				// 文件更新不成功，升级失败
				if (!m_bSilenceMode && m_hProgressWindow != NULL)
				{
					sKey = _T("Name");
					memset(acBuffer, 0, BUFFER_SIZE);
					GetPrivateProfileString(sSection, sKey, _T(""), acBuffer, BUFFER_SIZE, m_sConfigFilename);
					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_UPDATE_FILE_FAIL, (LPARAM)acBuffer);
					UPDATER_LOG_ERROR("Update %s is Failed",temp);
					theApp.m_UpdateOk = FALSE;
				}
				//UPDATER_LOG_INFO("Start sever");
				////启动开始的程序
				//theApp.OnIconnotifyStart();
				//UPDATER_LOG_INFO("Run Upload sever");
				//theApp.OnIconnotifyStart();//启动上传服务
				//UPDATER_LOG_INFO("Run FMSClient");
				//theApp.RunFMSClient();     //启动CS客户端
				//CDialog::OnOK();
				return FALSE;
			}
			else
			{
				//UPDATER_LOG_INFO("Start sever");
				//启动开始的程序
				//theApp.OnIconnotifyStart();
				UPDATER_LOG_INFO("Update %s is successful",temp);
			}
		}
	}

	// 通知界面升级完毕
	/*if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, 0);
	}*/

	return TRUE;
}
void CUpdateThread::WriteIni()
{
	CString szIniPath = file_info::GetAppDirectory() + TEXT("LocalFiles.ini");  
	::WritePrivateProfileString(TEXT("COMMON"), TEXT("Time"), theApp.strURLPackageTime, szIniPath); 
}
// 检查并下载一个文件
BOOL CUpdateThread::DownloadFile(CString &sFileSection)
{
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];
	char temp[MAX_PATH] = {0};
	CString sFilename;
	CString sHash;
	CString sURL;
	CString sKey;
	
	// 比较文件是否已经下载了，如是则跳过
	if (VerifyFile(sFileSection))
	{
		return TRUE;
	}

	// 取得文件名
	sKey = _T("Name");
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection, sKey, _T(""), acBuffer, BUFFER_SIZE, m_sConfigFilename);
	sFilename = (wchar_t*)acBuffer;
	
	// 取得文件URL
	sKey = "URL";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection, sKey, _T(""), acBuffer, BUFFER_SIZE, m_sConfigFilename);
	sURL = (wchar_t*)acBuffer;

	StringChange::CStringToChar(sFilename,temp);
	UPDATER_LOG_INFO("Download %s",temp);

	// 更新显示正在下载的文件
	// 下载文件
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADING_FILENAME, (LPARAM)sFilename.GetBuffer(0));
	}
	httpdownload hdl;
	if (!hdl.DownloadFile(CSCT::Tstring2Ansi(sURL.GetString()), CSCT::Tstring2Ansi((file_info::GetAppDirectory() + _T("Update\\") + sFilename).GetString()), 1024, m_hProgressWindow))
	{
		UPDATER_LOG_ERROR("Download %s failed",temp);
		//更新是否成功标志
		theApp.m_UpdateOk = FALSE;
		return FALSE;
	}

	UPDATER_LOG_INFO("Download %s successfully",temp);
	return TRUE;
}

// 校验文件
BOOL CUpdateThread::VerifyFile(CString &sFileSection)
{
	const int BUFFER_SIZE = 512;
	wchar_t acBuffer[BUFFER_SIZE];
	char temp[MAX_PATH] = {0};
	CString sFilename;
	CString sLocalPath;
	ULONGLONG iFileSize;
	CString sHash;
	CString sKey;

	// 取得文件名
	sKey = _T("Name");
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection, sKey, _T(""), (wchar_t*)acBuffer, BUFFER_SIZE, m_sConfigFilename);
	sFilename = (wchar_t*)acBuffer;

	// 取得文件大小
	sKey = _T("Size");
	iFileSize = GetPrivateProfileInt(sFileSection, sKey, 0, m_sConfigFilename);
    
	StringChange::CStringToChar(sFilename,temp);

	sLocalPath = (file_info::GetAppDirectory() + _T("Update\\") + sFilename);
	sLocalPath.Replace(_T("/"), _T("\\"));
	UINT LocalFileSize = file_info::GetFileSize(sLocalPath);
	if ( LocalFileSize == iFileSize)
	{
		sKey = _T("Hash");
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(sFileSection, sKey, _T(""), (wchar_t*)acBuffer, BUFFER_SIZE, m_sConfigFilename);
		sHash = (wchar_t*)acBuffer;

		// 计算文件的Hash码以进行比较
		unsigned char acMD5Digest[16];
		CalculateMD5(CSCT::Tstring2Ansi((file_info::GetAppDirectory() + _T("Update\\") + sFilename).GetString()).c_str(),acMD5Digest);

		CString strMD5 = MD5toString(acMD5Digest);
		if (sHash.CompareNoCase(MD5toString(acMD5Digest)) == 0)
		{
			return TRUE;
		}
		else
		{
			StringChange::CStringToChar(strMD5,temp);
			UPDATER_LOG_ERROR("VerifyFile %s HASH is Failed",temp);
			theApp.m_UpdateOk = FALSE;
		}
	}
	/*else if(NOT_DOWNLOAD_FILE == LocalFileSize)
	{
		UPDATER_LOG_INFO("Localfile is not exist, need to download");
	}
	else
	{
		UPDATER_LOG_ERROR("LocalFile size %d !=  %d",LocalFileSize, iFileSize);
	}*/
	return FALSE;
}

bool FolderExists(CString s)
{
	DWORD attr; 
	attr = GetFileAttributes(s); 
	return (attr != (DWORD)(-1) ) &&
		( attr & FILE_ATTRIBUTE_DIRECTORY); 
}


static bool SuperMkDir(CString P)
{
	int len=P.GetLength();
	if ( len <2 ) return false; 

	if('\\'==P[len-1])
	{
		P=P.Left(len-1);
		len=P.GetLength();
	}
	if ( len <=0 ) return false;

	if (len <=3) 
	{
		if (FolderExists(P))return true;
		else return false; 
	}

	if (FolderExists(P))return true;



	CString Parent;
	Parent=P.Left(P.ReverseFind('\\') );

	if(Parent.GetLength()<=0)return false; 

	bool Ret=SuperMkDir(Parent); 

	if(Ret) 
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor=NULL;
		sa.bInheritHandle=0;
		Ret=(CreateDirectory(P,&sa)==TRUE);
		return Ret;
	}
	else
		return false;
}

// 更新文件
BOOL CUpdateThread::UpdateFile(FileInfo &filestru)
{
	CString sFilename;
	CString strFlord;
	CString sDestFilename;
	CString sBackupFilename;
	char temp[MAX_PATH] = {0};
	char tempPath[MAX_PATH] = {0};
	sFilename = filestru.strName;
	sDestFilename = filestru.Subcatalog;


	if(_T("") == sDestFilename)
	{
		sDestFilename = file_info::GetAppDirectory() + sFilename;
	}
	else
	{
		sDestFilename = file_info::GetAppDirectory() + filestru.Subcatalog + _T("\\") + sFilename;
		strFlord = file_info::GetAppDirectory() + filestru.Subcatalog;
	}

	StringChange::CStringToChar(sFilename,temp);
	UPDATER_LOG_INFO("Start to Update %s",temp);
	// 备份原文件
	StringChange::CStringToChar(sFilename,temp);
	UPDATER_LOG_INFO("BackUp %s",temp);

 	sBackupFilename = file_info::GetAppDirectory() + _T("Backup\\") + sFilename;
	if (file_info::GetFileSize(sDestFilename) > 0)
	{
		wchar_t acBuffer[MAX_PATH] = {0};

		// 取得本自动升级程序的文件全路径
		GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));
		if (sFilename.CompareNoCase(file_info::GetFilename(acBuffer)) == 0)
		{
			// 要更新的文件是本自动升级程序，须换名更新
			CopyFile(sDestFilename, sBackupFilename, FALSE);
			// 复制新文件，新文件名加上 .new 后缀，由主程序来将其更名
			sDestFilename += _T(".new");
			return CopyFile(file_info::GetAppDirectory() + _T("Update\\") + sFilename, sDestFilename, FALSE);
		}
		else
		{
			MoveFile(sDestFilename, sBackupFilename);
		}
	}
	// 复制新文件
	StringChange::CStringToChar(sFilename,temp);
	StringChange::CStringToChar(sDestFilename,tempPath);
	UPDATER_LOG_INFO("Copy the newest files %s to %s",temp,tempPath);
 
	if(_T("") != strFlord)
	{
		strFlord.Replace(_T("/"), _T("\\"));
		if (!PathIsDirectory(strFlord) ) 
		{ 
			UPDATER_LOG_ERROR("Folder %s is not found ",tempPath);
			if(FALSE == SuperMkDir(strFlord))
			{
				int error = GetLastError();
				UPDATER_LOG_ERROR("Create Folder %s failed ",tempPath);
				return FALSE;
			}
			/*if (FALSE == CreateDirectory(strFlord, NULL)) 
			{ 
				int error = GetLastError();
				UPDATER_LOG_ERROR("Create Folder %s failed ",tempPath);
				return FALSE;
			}*/
		}
	}
	return CopyFile(file_info::GetAppDirectory() + _T("Update\\") + sFilename, sDestFilename, FALSE);
}
