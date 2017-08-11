
// UpLoadLogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpLoadLog.h"
#include "UpLoadLogDlg.h"
#include "zlib.h"
#include "ZipFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUpLoadLogDlg �Ի���




CUpLoadLogDlg::CUpLoadLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpLoadLogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	zipfile[MAX_PATH] = NULL;
	file[MAX_PATH]    = NULL;
}

void CUpLoadLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUpLoadLogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCPAINT()
	ON_MESSAGE(WM_USER,UserMessage)
END_MESSAGE_MAP()


// CUpLoadLogDlg ��Ϣ�������

BOOL CUpLoadLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	SetUploadInfo();
	UPLOAD_LOG_INFO("Start to Rar");
	if (!PathFileExists(theApp.m_strlogpath)) 
	{
		UPLOAD_LOG_ERROR("Log Path is error:%s",theApp.m_strlogpath.GetBuffer());
		return TRUE;
	}
	DWORD zResult = ZipUtils::CompressDirToZip(theApp.m_sLogPath,theApp.m_filename,NULL);
	if(zResult == ZR_OK)
	{
		UpLoad();
	}else
	{
		UPLOAD_LOG_ERROR("Rar file failed");
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CUpLoadLogDlg::OnNcPaint()
{
	static int i = 2;
	if(i > 0)
	{
		i--;
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnNcPaint();
	}
}
void CUpLoadLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpLoadLogDlg::OnPaint()
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
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUpLoadLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CUpLoadLogDlg::SetUploadInfo()
{
	GetExePath();
	theApp.m_csLogPath+=_T("\\");
	theApp.m_csLogPath+=_T("Log");
	theApp.m_strlogpath = theApp.m_csLogPath;
	CTime tm=CTime::GetCurrentTime();
	CString date = tm.Format("%Y%m%d%H%M%S.zip");
	theApp.m_csLogPath+=date;
	theApp.m_filename+= _T("Log");
	theApp.m_filename+= date;

	StringChange::CStringToChar(theApp.m_sLogPath,theApp.m_localpath);
	StringChange::CStringToChar(theApp.m_sUrl,theApp.m_url);
	StringChange::CStringToChar(theApp.m_csLogPath,theApp.m_logpath);
	StringChange::CStringToChar(theApp.m_filename,theApp.m_cfilename);
	UPLOAD_LOG_INFO("Ready to Rar Log's Local Path: %s",theApp.m_localpath);
	UPLOAD_LOG_INFO("The Uploaded URL : %s",theApp.m_url);
	UPLOAD_LOG_INFO("Ready to Upload Rar Log's Local Path: %s",theApp.m_logpath);
	UPLOAD_LOG_INFO("The Rar Log's Name: %s",theApp.m_cfilename);
}
void CUpLoadLogDlg::GetExePath()
{
	// �õ�exeִ��·��.  
	TCHAR tcExePath[MAX_PATH] = {0};  
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);  
	// ����ini·����exeͬһĿ¼��  
	#ifndef CONFIG_FILE  
	#define CONFIG_FILE     (TEXT("Config.ini"))  
	#endif  
	//_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');  
	*pFind = '\0';  
	theApp.m_csLogPath = tcExePath;
}
void CUpLoadLogDlg::UpLoad()
{
	m_upthread = (UploadThread *)AfxBeginThread(RUNTIME_CLASS(UploadThread));
	m_upthread->m_hwnd = this->m_hWnd;
	strcpy(m_upthread->m_url,theApp.m_url);
	strcpy(m_upthread->m_localpath ,theApp.m_logpath);

	strcpy(m_upthread->m_filename,theApp.m_cfilename);
	if (theApp.m_iFlag == _T("1"))
	{
		//strcpy(m_upthread->userpwd , "FTP:1111");
		strcpy(m_upthread->userpwd , ":");
		strcat(m_upthread->m_url,theApp.m_cfilename);
		UPLOAD_LOG_INFO("FTP Type to Upload");
		m_upthread->PostThreadMessage(WM_FTPUPLOAD,0,0);
	}
	else
	{
		UPLOAD_LOG_INFO("HTTP Type to Upload");
		m_upthread->PostThreadMessage(WM_HTTPUPLOAD,0,0);
	}
}
LRESULT CUpLoadLogDlg::UserMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case WM_HTTPLOAD_OK:
		{
			////ɾ���ļ�
			if (PathFileExists(theApp.m_csLogPath) == TRUE) 
			{ 
				DeleteFile(theApp.m_csLogPath);
			}
			OnCancel();
		}break;
	case WM_FTPUPLOAD_OK:
		{
			if (PathFileExists(theApp.m_csLogPath) == TRUE) 
			{ 
				DeleteFile(theApp.m_csLogPath);
			}
			OnCancel();
		}break;
	case WM_FTPUOLOAD_FAILD:
		{
			//FTP�ϴ�ʧ��
			OnCancel();
		}break;
	case WM_HTTPUOLOAD_FAILD:
		{
			//HTTP�ϴ�ʧ��
			OnCancel();
		}break;
	}
	return 0;
}
void CUpLoadLogDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}
