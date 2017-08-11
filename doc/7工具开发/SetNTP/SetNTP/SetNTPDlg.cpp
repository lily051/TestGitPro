
// SetNTPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetNTP.h"
#include "SetNTPDlg.h"

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


// CSetNTPDlg �Ի���




CSetNTPDlg::CSetNTPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetNTPDlg::IDD, pParent)
	, m_csIP(_T("127.0.0.1"))
	, m_iPort(3306)
	, m_csUser(_T("root"))
	, m_csPwd(_T("888"))
	, m_csNtpIp(_T("127.0.0.1"))
	, m_iNtpPort(123)
	, m_iTime(10)
	, m_csAction(_T("ivms8120v2"))
	, m_strDataBaseIp(_T("127.0.0.1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_cDataBaseIp[CHAR_LENGTH]=NULL;
	m_csDataBaseAction[CHAR_LENGTH]=NULL;
	m_cDataBaseUser[CHAR_LENGTH]=NULL;
	m_cDataBasePwd[CHAR_LENGTH]=NULL;
	m_cNtpIp[CHAR_LENGTH]=NULL;
	m_iTotalNumber = 0;
	m_iSeccessful = 0;;
	mysql_init(&m_MySql);
	//��ʼ�����ܽ��ܿ�
	Hak1((char*)103, "DATA", NULL);
	NET_DVR_Init();
}

void CSetNTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_csNtpIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPwd);
	DDX_Text(pDX, IDC_EDIT_NTP_IP, m_csNtpIp);
	DDX_Text(pDX, IDC_EDIT_NTP_PORT, m_iNtpPort);
	DDX_Text(pDX, IDC_EDIT_TIME, m_iTime);
	DDX_Text(pDX, IDC_EDIT_ACTION, m_csAction);
	DDX_Text(pDX, IDC_EDIT_IP, m_strDataBaseIp);
}

BEGIN_MESSAGE_MAP(CSetNTPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CSetNTPDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CSetNTPDlg ��Ϣ�������

BOOL CSetNTPDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    UpdateData(FALSE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSetNTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSetNTPDlg::OnPaint()
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
HCURSOR CSetNTPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSetNTPDlg::GetData()
{
	UpdateData(TRUE);
	m_iSeccessful = 0; 
	m_iTotalNumber = 0;
	
	StringChange::CStringToChar(m_strDataBaseIp,m_cDataBaseIp);
	StringChange::CStringToChar(m_csUser,m_cDataBaseUser);
	StringChange::CStringToChar(m_csPwd,m_cDataBasePwd);
	StringChange::CStringToChar(m_csNtpIp,m_cNtpIp);
	StringChange::CStringToChar(m_csAction,m_csDataBaseAction);
}

BOOL  CSetNTPDlg::ConnectDataBase()
{
	UpdateData(TRUE);
	if(!mysql_real_connect(&m_MySql,m_cDataBaseIp,m_cDataBaseUser,
	   m_cDataBasePwd,m_csDataBaseAction,m_iPort,NULL,0))
	{
		NTP_LOG_ERROR("��¼���ݿ�ʵ��%sʧ��",m_csDataBaseAction);
		return FALSE;
	}
	else
	{
		NTP_LOG_INFO("��¼���ݿ�ʵ��%s�ɹ�",m_csDataBaseAction);
		return TRUE;
	}
}

void CSetNTPDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_BTN_START,_T("����Уʱ.."));
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);  
	GetData();
	if(ConnectDataBase() == TRUE)
	{
		PushQueue();
		m_iTotalNumber = m_DvrList.size();
		PopQueue();
		PrintInfo();
		AfxMessageBox(_T("Уʱ����!"));
	}
	else
	{
		AfxMessageBox(_T("Уʱʧ��!"));
	}
	SetDlgItemText(IDC_BTN_START,_T("��ʼУʱ"));
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE); 
}

void CSetNTPDlg::PushQueue()
{
	mysql_query(&m_MySql,"SET NAMES 'GB2312'");
	char szSql[300] = "select * from device a where a.nType = 50000 or a.nType = 50012";
	int res=mysql_query(&m_MySql,szSql);
	if(!res) 
	{ 
		m_Result=mysql_store_result(&m_MySql);//�����ѯ�������ݵ�result 
		if(m_Result) 
		{ 
			while(m_Sql_row=mysql_fetch_row(m_Result))//��ȡ��������� 
			{ 
				DVRStruct dvrstruct = {0};
				sprintf_s(dvrstruct.cDvrIp,"%s",m_Sql_row[3]);
				dvrstruct.iPort = atoi(m_Sql_row[4]);
				sprintf_s(dvrstruct.cDvrUser,"%s",m_Sql_row[5]);

				int dst_len = 1024;
				char* outl = (char*)&dst_len;
				Hak7(dvrstruct.cDvrPwd, m_Sql_row[6], outl);// ����
				m_DvrList.push(dvrstruct);
			}
		}
	}
}

void CSetNTPDlg::PopQueue()
{
	while(m_DvrList.size()>0)
	{
		TCHAR temp[CHAR_LENGTH]={0};
		StringChange::CharToTchar(m_DvrList.front().cDvrIp,temp);
		CString strShow(_T("����Уʱ���豸IP:"));
		strShow+=temp;
		SetDlgItemText(IDC_STATIC_SHOW,strShow);
		SetDevNTPConfig(m_DvrList.front());
		m_DvrList.pop();
	}
}

void CSetNTPDlg::PrintInfo()
{
    NTP_LOG_INFO("==========================================");
	NTP_LOG_INFO("һ��Уʱ%d̨�豸,�ɹ�%d̨,ʧ��%d̨",m_iTotalNumber,m_iSeccessful,m_iTotalNumber-m_iSeccessful);
	while(m_FaildList.size()>0)
	{
		NTP_LOG_INFO("NTPУʱʧ�ܵ��豸IP:%s",m_FaildList.front().cDvrIp);
		m_FaildList.pop();
	}
	NTP_LOG_INFO("==========================================");
}
BSTR CSetNTPDlg::SetDevNTPConfig(DVRStruct &funParam)
{
	CString strResult;//���صĽ���ַ���
	USES_CONVERSION;//��Ҫд��ѭ������
	//�����豸
	NET_DVR_DEVICEINFO_V30 devInfoV30;//�豸��Ϣ
	memset(&devInfoV30, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID =  NET_DVR_Login_V30(funParam.cDvrIp,
		funParam.iPort,
		funParam.cDvrUser,
		funParam.cDvrPwd, 
		&devInfoV30);
	if (lLoginID == -1)
	{
		int err = NET_DVR_GetLastError();
	    NTP_LOG_ERROR("NET_DVR_Login_V30 falid, error = %d", err);
		CString strMsg;
		strMsg.Format(_T("��¼ʧ�ܡ�������:%ld"), err);
		OutputDebugString(strMsg);
		strResult.Format(_T("��¼�豸ʧ��!"));
		//��¼�豸��Ϣ�����
		m_FaildList.push(funParam);
		return strResult.AllocSysString();
	}

	NET_DVR_NTPPARA nNtpConfigInfo;
	memset(&nNtpConfigInfo, 0, sizeof(NET_DVR_NTPPARA));
	strcpy((char*)nNtpConfigInfo.sNTPServer, m_cNtpIp);
	nNtpConfigInfo.wNtpPort = m_iNtpPort;
	nNtpConfigInfo.wInterval = m_iTime ;
	nNtpConfigInfo.byEnableNTP = 1;
	nNtpConfigInfo.cTimeDifferenceH += 8;    //ʱ��+8����ʱ���ı䣬���Ǳ���ʱ��
	if(!NET_DVR_SetDVRConfig(lLoginID, NET_DVR_SET_NTPCFG, 0xFFFFFFFF, &nNtpConfigInfo, sizeof(NET_DVR_NTPPARA)))
	{
		//�ǳ��豸
		int err = NET_DVR_GetLastError();
		NTP_LOG_ERROR("NET_DVR_GetDVRConfig falid, error = %d", err);
		NET_DVR_Logout_V30(lLoginID);
		CString strMsg;
		strMsg.Format(_T("����NTPУʱʧ�ܡ�������:%ld"), err);
		OutputDebugString(strMsg);
		strResult.Format(_T("����NTPУʱʧ��!"));
		//Уʱʧ�ܵ��豸��Ϣ�����
		m_FaildList.push(funParam);
		return strResult.AllocSysString();
	}
	m_iSeccessful++;
	NTP_LOG_INFO("Уʱ�ɹ����豸IP:%s",funParam.cDvrIp);
	strResult = _T("1");
	//�ǳ��豸
	NET_DVR_Logout_V30(lLoginID);
	return strResult.AllocSysString();
}
