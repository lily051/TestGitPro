
// UpdaterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpdateThread.h"
#include "Updater.h"
#include "UpdaterDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpdaterDlg �Ի���




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


// CUpdaterDlg ��Ϣ�������

BOOL CUpdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
	}


	//SetWindowText(m_sAppName + _T("��������"));
	SetWindowText(_T("��������..."));
	m_btnUpgrade.SetWindowText(_T("��ʼ����"));
	m_btnCancel.SetWindowText(_T("ȡ������"));
	m_sPrompt = _T("����׼������!");
	UpdateData(FALSE);
	if (m_bSilenceMode)
	{
		ShowWindow(SW_HIDE);
		m_btnCancel.EnableWindow(FALSE);
		OnOK();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpdaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	// �ػ������
	if (m_fTotalDownloadPercent != 0.0)
	{
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//ͻȻ��������������������
	if (theApp.m_UpdateOk == TRUE)
	{
		// ִ����������
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
	}//����ʧ��
	else if (theApp.m_UpdateOk == FALSE)
	{
		//������ʼ�ĳ���
		theApp.OnIconnotifyStart();
		
	}
	
	CDialog::OnCancel();
}

void CUpdaterDlg::OnOK() 
{
	// �����ʼ������ť��ִ������
	if (!SetConfigFile(file_info::GetAppDirectory() + UPDATE_CONFIG_FILENAME))
	{
		MessageBox(_T("�������ļ�ʧ�� ��"));
	}
	else
	{
		// ��ʼ���������ð�ť�������ظ����
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		//����ȡ��������ť
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		//����ʧ���Ժ� ��������
		theApp.m_UpdateOk = TRUE;
		DoUpdate();
	}

	//	CDialog::OnOK();
}

// �������������ļ���
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
	// ���������߳�
	m_pUpdateThread = new CUpdateThread;
	m_pUpdateThread->m_bAutoDelete = TRUE;
	m_pUpdateThread->m_sConfigFilename = m_sConfigFilename;
	m_pUpdateThread->m_hProgressWindow = m_hWnd;
	m_pUpdateThread->CreateThread();

	return TRUE;
}

// �����û���Ϣ
LRESULT CUpdaterDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case NOTIFY_DOWNLOAD_INFO:			// ֪ͨҪ���ص��ļ�״��
		m_TotalFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = 0;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOADED_INFO:		// ֪ͨ�����ص��ļ�״��
		m_FinishedFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize) / m_TotalFileInfo.iFileSize;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOAD_PROGRESS:		// ֪ͨ���ص����ļ�����
		{
			DOWNLOAD_PROGRESS_STRU* pDownloadProgress = (DOWNLOAD_PROGRESS_STRU*)lParam;
			m_fDownloadPercent = float(pDownloadProgress->iCurrentFileFinishedSize) / pDownloadProgress->iCurrentFileSize;
			m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize + pDownloadProgress->iCurrentFileFinishedSize) / m_TotalFileInfo.iFileSize;
			UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
				, m_fTotalDownloadPercent, m_fDownloadPercent);
		}
		break;
	case NOTIFY_DOWNLOADING_FILENAME:	// ֪ͨ�������ص��ļ���
		if (lParam != 0)
		{
			m_sPrompt.Format(_T("���������ļ� %s."), (wchar_t*)lParam);
		}
		else
		{
			m_sPrompt = _T("���������ļ� :");
		}
		m_sProgressBar = _T("");
		UpdateData(FALSE);
		break;
	case NOTIFY_DOWNLOAD_FILE_FAIL:		// ֪ͨ�����ļ�ʧ��
		m_sPrompt.Format(_T("�����ļ� %s ʧ�ܣ�"), (wchar_t*)lParam);
		m_sProgressBar = _T("");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_VERIFY_FILE_FAIL:		// ֪ͨУ���ļ�ʧ��
		m_sPrompt.Format(_T("У���ļ� %s ʧ�ܣ�"), (wchar_t*)lParam);
		m_sProgressBar = _T("");

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_UPDATE_FILE_FAIL:		// ֪ͨ�����ļ�ʧ��
		m_sPrompt.Format(_T("�����ļ� %s ʧ�ܣ�"), (wchar_t*)lParam);
		m_sProgressBar = _T("");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	case NOTIFY_FINISH_UPDATE:			// ֪ͨ���������Ϣ
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
			{  	//������ļ���ôɾ���ļ�
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

// ������ʾ�����ļ�����
void CUpdaterDlg::UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent)
{
	if (m_hWnd == NULL /*|| m_bSilenceMode*/ || iTotalFileCount == 0)
	{
		return;
	}

	// ���ƽ�����
	m_sProgressBar.Format(_T("%.0f%%"), fPercent*100);
	m_ProCurCtrl.SetPos(fPercent*100);

	m_sTotalProgressBar.Format(_T("%.0f%%"), fTotalPercent*100);
	m_ProTotalCtrl.SetPos(fTotalPercent*100);

	m_sPromptTotalProgress.Format(_T("�����ܽ��� %d / %d")
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
// ��������ִ�����
void CUpdaterDlg::OnNotifyUpdateFinish(BOOL bSuccess)
{
	m_pUpdateThread = NULL;

	if (m_bSilenceMode && bSuccess)
	{
		m_sPrompt = _T("������ϣ�");
		m_btnCancel.SetWindowText(_T("�������"));
		UPDATER_LOG_INFO("Update is Over");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		//����������ʱ��
		WriteIni();

		UPDATER_LOG_INFO("Exit the Update Programma,Delete ini Files and Backup");
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Backup"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Update"));
		
		UPDATER_LOG_INFO("Run Upload sever");
		theApp.OnIconnotifyStart();//�����ϴ�����
		UPDATER_LOG_INFO("Run FMSClient");
		theApp.RunFMSClient();     //����CS�ͻ���

		Sleep(1000);
		CDialog::OnOK();
	}
	else if (!bSuccess)
	{
		m_sPrompt = _T("������ϣ�");
		m_btnCancel.SetWindowText(_T("����ʧ��"));
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		UPDATER_LOG_INFO("Run Upload sever");
		theApp.OnIconnotifyStart();//�����ϴ�����
		UPDATER_LOG_INFO("Run FMSClient");
		theApp.RunFMSClient();     //����CS�ͻ���

		Sleep(1000);
		CDialog::OnOK();

	}
	else if (bSuccess)
	{
		UPDATER_LOG_INFO("Exit the Update Programma,Delete ini Files and Backup");
		DeleteFile(file_info::GetAppDirectory()+_T("UpdateConfig.ini"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Backup"));
		DeleteDirectory(file_info::GetAppDirectory() + _T("Update"));
		//����������ʱ��
		WriteIni();

		m_sPrompt = _T("������ϣ�");
		m_btnCancel.SetWindowText(_T("�������"));
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
