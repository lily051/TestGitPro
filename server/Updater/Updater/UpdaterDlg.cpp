
// UpdaterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpdateThread.h"
#include "Updater.h"
#include "UpdaterDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpdaterDlg 对话框




CUpdaterDlg::CUpdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdaterDlg::IDD, pParent)
{
	m_sPrompt = _T("");
	m_sPromptTotalProgress = _T("");
	m_sProgressBar = _T("");
	m_sTotalProgressBar = _T("");
	m_TotalFileInfo.iFileCount = 0;
	m_TotalFileInfo.iFileSize = 0;
	m_FinishedFileInfo.iFileCount = 0;
	m_FinishedFileInfo.iFileSize = 0;
	m_fDownloadPercent = 0.0;
	m_bSilenceMode = FALSE;
	m_bUserBreak = FALSE;
	m_pUpdateThread = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnUpgrade);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CURRENT_PROGRESS, m_ProCurCtrl);
	DDX_Control(pDX, IDC_TOTAL_PROGRESS, m_ProTotalCtrl);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_sPrompt);
	DDX_Text(pDX, IDC_STATIC_PROMPT_TOTAL_PROGRESS, m_sPromptTotalProgress);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_sProgressBar);
	DDX_Text(pDX, IDC_STATIC_TOTAL_PROGRESS, m_sTotalProgressBar);
}

BEGIN_MESSAGE_MAP(CUpdaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER, OnUserMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CUpdaterDlg 消息处理程序

BOOL CUpdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
	}


	//SetWindowText(m_sAppName + _T("升级工具"));
	SetWindowText(_T("正在升级..."));
	m_btnUpgrade.SetWindowText(_T("开始升级"));
	m_btnCancel.SetWindowText(_T("取消升级"));
	m_sPrompt = _T("升级准备就绪!");
	UpdateData(FALSE);
	if (m_bSilenceMode)
	{
		ShowWindow(SW_HIDE);
		m_btnCancel.EnableWindow(FALSE);
		OnOK();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpdaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	// 重绘进度条
	if (m_fTotalDownloadPercent != 0.0)
	{
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUpdaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpdaterDlg::OnCancel() 
{
	m_bUserBreak = TRUE;
	if (m_pUpdateThread != NULL)
	{
		HANDLE hThread = m_pUpdateThread->m_hThread;
		m_pUpdateThread->m_bUserBreak = TRUE;
		WaitForSingleObject(hThread, 1000);
	}
	//突然不想升级或者升级结束
	if (theApp.m_UpdateOk == TRUE)
	{
		// 执行升级程序
		const int BUFFER_SIZE = 512;
		wchar_t acBuffer[BUFFER_SIZE];
		CString strRunApp,strIniPath;
		CString sKey = _T("RunAfterDownload");
		memset(acBuffer, 0, BUFFER_SIZE);
		strIniPath =  file_info::GetAppDirectory()+_T("UpdateConfig.ini");
		GetPrivateProfileString(SECTION_UPDATE, sKey, _T(""), (wchar_t*)acBuffer, BUFFER_SIZE, strIniPath);
		strRunApp = (wchar_t*)acBuffer;
		if (!strRunApp.IsEmpty())
		{
			char temp[MAX_PATH] = {0};
			StringChange::CStringToChar(strRunApp,temp);
			UPDATER_LOG_INFO("Open program %s",temp);
			theApp.OnIconnotifyStart();
		}
		else
		{
			UPDATER_LOG_ERROR("Program name is empty ,open is faild");
		}

		UPDATER_LOG_INFO("Exit Program,delete Ini files");
		//DeleteFile(file_info::GetAppDirectory()+_T("LocalFiles.ini")); 
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Backup"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Update"));
	}//升级失败
	else if (theApp.m_UpdateOk == FALSE)
	{
		//启动开始的程序
		theApp.OnIconnotifyStart();
		
	}
	
	CDialog::OnCancel();
}

void CUpdaterDlg::OnOK() 
{
	// 点击开始升级按钮，执行升级
	if (!SetConfigFile(file_info::GetAppDirectory() + UPDATE_CONFIG_FILENAME))
	{
		MessageBox(_T("打开配置文件失败 ！"));
	}
	else
	{
		// 开始升级，禁用按钮，避免重复点击
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		//禁用取消升级按钮
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		//升级失败以后 重新升级
		theApp.m_UpdateOk = TRUE;
		DoUpdate();
	}

	//	CDialog::OnOK();
}

// 设置升级配置文件名
BOOL CUpdaterDlg::SetConfigFile(CString &sFilename)
{
	CFileFind FileFinder;

	if (FileFinder.FindFile(sFilename))
	{
		FileFinder.FindNextFile();
		if (!FileFinder.IsDirectory())
		{
			m_sConfigFilename = sFilename;
			FileFinder.Close();
			return TRUE;
		}
		FileFinder.Close();
	}

	return FALSE;
}

BOOL CUpdaterDlg::DoUpdate(void)
{
	UPDATER_LOG_INFO("Start the new thread");
	// 启动升级线程
	m_pUpdateThread = new CUpdateThread;
	m_pUpdateThread->m_bAutoDelete = TRUE;
	m_pUpdateThread->m_sConfigFilename = m_sConfigFilename;
	m_pUpdateThread->m_hProgressWindow = m_hWnd;
	m_pUpdateThread->CreateThread();

	return TRUE;
}

// 处理用户消息
LRESULT CUpdaterDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case NOTIFY_DOWNLOAD_INFO:			// 通知要下载的文件状况
		m_TotalFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = 0;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOADED_INFO:		// 通知已下载的文件状况
		m_FinishedFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize) / m_TotalFileInfo.iFileSize;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOAD_PROGRESS:		// 通知下载单个文件进度
		{
			DOWNLOAD_PROGRESS_STRU* pDownloadProgress = (DOWNLOAD_PROGRESS_STRU*)lParam;
			m_fDownloadPercent = float(pDownloadProgress->iCurrentFileFinishedSize) / pDownloadProgress->iCurrentFileSize;
			m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize + pDownloadProgress->iCurrentFileFinishedSize) / m_TotalFileInfo.iFileSize;
			UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
				, m_fTotalDownloadPercent, m_fDownloadPercent);
		}
		break;
	case NOTIFY_DOWNLOADING_FILENAME:	// 通知正在下载的文件名
		if (lParam != 0)
		{
			m_sPrompt.Format(_T("正在下载文件 %s."), (wchar_t*)lParam);
		}
		else
		{
			m_sPrompt = _T("正在下载文件 :");
		}
		m_sProgressBar = _T("");
		UpdateData(FALSE);
		break;
	case NOTIFY_DOWNLOAD_FILE_FAIL:		// 通知下载文件失败
		m_sPrompt.Format(_T("下载文件 %s 失败！"), (wchar_t*)lParam);
		m_sProgressBar = _T("");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_VERIFY_FILE_FAIL:		// 通知校验文件失败
		m_sPrompt.Format(_T("校验文件 %s 失败！"), (wchar_t*)lParam);
		m_sProgressBar = _T("");

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_UPDATE_FILE_FAIL:		// 通知更新文件失败
		m_sPrompt.Format(_T("更新文件 %s 失败！"), (wchar_t*)lParam);
		m_sProgressBar = _T("");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_FINISH_UPDATE:			// 通知升级完毕消息
		OnNotifyUpdateFinish((BOOL)lParam);
		break;
	default:
		break;
	}

	return TRUE;
}
BOOL CUpdaterDlg::DeleteDirectory(CString strDirName)
{

	CFileFind tempFind;  
	CString strPath;
	strPath += strDirName + _T("\\*.*");
	BOOL IsFinded = tempFind.FindFile(strPath);  
	while (IsFinded)  
	{ 
		IsFinded = tempFind.FindNextFile();  
		if (!tempFind.IsDots())  
		{  
			CString sFoundFileName = tempFind.GetFileName();
			if (tempFind.IsDirectory())  
			{  	
				CString sTempDir = strDirName + _T("\\") + sFoundFileName;
				DeleteDirectory(sTempDir);
			}  
			else  	
			{  	//如果是文件那么删除文件
				CString sTempFileName = strDirName + _T("\\") + sFoundFileName;
				DeleteFile(sTempFileName);  
			}  
		}  
	}  
	tempFind.Close();  
	if(!RemoveDirectory(strDirName))  
	{  
		return FALSE;  	
	}  
	return TRUE;  
}

// 更新显示下载文件进度
void CUpdaterDlg::UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent)
{
	if (m_hWnd == NULL /*|| m_bSilenceMode*/ || iTotalFileCount == 0)
	{
		return;
	}

	// 绘制进度条
	m_sProgressBar.Format(_T("%.0f%%"), fPercent*100);
	m_ProCurCtrl.SetPos(fPercent*100);

	m_sTotalProgressBar.Format(_T("%.0f%%"), fTotalPercent*100);
	m_ProTotalCtrl.SetPos(fTotalPercent*100);

	m_sPromptTotalProgress.Format(_T("升级总进度 %d / %d")
		, iFinishedFileCount >= iTotalFileCount ? iTotalFileCount : iFinishedFileCount + 1
		, iTotalFileCount);

	UpdateData(FALSE);

	return;
}

void CUpdaterDlg::WriteIni()
{
	CString szIniPath = file_info::GetAppDirectory() + TEXT("LocalFiles.ini");  
	::WritePrivateProfileString(TEXT("COMMON"), TEXT("Time"), theApp.strURLPackageTime, szIniPath); 
}
// 升级程序执行完毕
void CUpdaterDlg::OnNotifyUpdateFinish(BOOL bSuccess)
{
	m_pUpdateThread = NULL;

	if (m_bSilenceMode && bSuccess)
	{
		m_sPrompt = _T("升级完毕！");
		m_btnCancel.SetWindowText(_T("完成升级"));
		UPDATER_LOG_INFO("Update is Over");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		//更新升级包时间
		WriteIni();

		UPDATER_LOG_INFO("Exit the Update Programma,Delete ini Files and Backup");
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Backup"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Update"));
		
		UPDATER_LOG_INFO("Run Upload sever");
		theApp.OnIconnotifyStart();//启动上传服务
		UPDATER_LOG_INFO("Run FMSClient");
		theApp.RunFMSClient();     //启动CS客户端

		Sleep(1000);
		CDialog::OnOK();
	}
	else if (!bSuccess)
	{
		m_sPrompt = _T("升级完毕！");
		m_btnCancel.SetWindowText(_T("升级失败"));
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		UPDATER_LOG_INFO("Run Upload sever");
		theApp.OnIconnotifyStart();//启动上传服务
		UPDATER_LOG_INFO("Run FMSClient");
		theApp.RunFMSClient();     //启动CS客户端

		Sleep(1000);
		CDialog::OnOK();

	}
	else if (bSuccess)
	{
		UPDATER_LOG_INFO("Exit the Update Programma,Delete ini Files and Backup");
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Backup"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Update"));
		//更新升级包时间
		WriteIni();

		m_sPrompt = _T("升级完毕！");
		m_btnCancel.SetWindowText(_T("完成升级"));
		UPDATER_LOG_INFO("Update is Over");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		Sleep(1000);
		CDialog::OnOK();
	}
	else
	{
		m_sPromptTotalProgress = "";
		m_sTotalProgressBar = "";
	}

	m_sProgressBar = "";
	UpdateData(FALSE);
	//GetDlgItem(IDOK)->EnableWindow();
}
