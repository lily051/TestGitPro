
// SetNTPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetNTP.h"
#include "SetNTPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSetNTPDlg 对话框




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
	//初始化加密解密库
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


// CSetNTPDlg 消息处理程序

BOOL CSetNTPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    UpdateData(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSetNTPDlg::OnPaint()
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		NTP_LOG_ERROR("登录数据库实例%s失败",m_csDataBaseAction);
		return FALSE;
	}
	else
	{
		NTP_LOG_INFO("登录数据库实例%s成功",m_csDataBaseAction);
		return TRUE;
	}
}

void CSetNTPDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_BTN_START,_T("正在校时.."));
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);  
	GetData();
	if(ConnectDataBase() == TRUE)
	{
		PushQueue();
		m_iTotalNumber = m_DvrList.size();
		PopQueue();
		PrintInfo();
		AfxMessageBox(_T("校时结束!"));
	}
	else
	{
		AfxMessageBox(_T("校时失败!"));
	}
	SetDlgItemText(IDC_BTN_START,_T("开始校时"));
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE); 
}

void CSetNTPDlg::PushQueue()
{
	mysql_query(&m_MySql,"SET NAMES 'GB2312'");
	char szSql[300] = "select * from device a where a.nType = 50000 or a.nType = 50012";
	int res=mysql_query(&m_MySql,szSql);
	if(!res) 
	{ 
		m_Result=mysql_store_result(&m_MySql);//保存查询到的数据到result 
		if(m_Result) 
		{ 
			while(m_Sql_row=mysql_fetch_row(m_Result))//获取具体的数据 
			{ 
				DVRStruct dvrstruct = {0};
				sprintf_s(dvrstruct.cDvrIp,"%s",m_Sql_row[3]);
				dvrstruct.iPort = atoi(m_Sql_row[4]);
				sprintf_s(dvrstruct.cDvrUser,"%s",m_Sql_row[5]);

				int dst_len = 1024;
				char* outl = (char*)&dst_len;
				Hak7(dvrstruct.cDvrPwd, m_Sql_row[6], outl);// 解密
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
		CString strShow(_T("正在校时的设备IP:"));
		strShow+=temp;
		SetDlgItemText(IDC_STATIC_SHOW,strShow);
		SetDevNTPConfig(m_DvrList.front());
		m_DvrList.pop();
	}
}

void CSetNTPDlg::PrintInfo()
{
    NTP_LOG_INFO("==========================================");
	NTP_LOG_INFO("一共校时%d台设备,成功%d台,失败%d台",m_iTotalNumber,m_iSeccessful,m_iTotalNumber-m_iSeccessful);
	while(m_FaildList.size()>0)
	{
		NTP_LOG_INFO("NTP校时失败的设备IP:%s",m_FaildList.front().cDvrIp);
		m_FaildList.pop();
	}
	NTP_LOG_INFO("==========================================");
}
BSTR CSetNTPDlg::SetDevNTPConfig(DVRStruct &funParam)
{
	CString strResult;//返回的结果字符串
	USES_CONVERSION;//不要写在循环体呢
	//登入设备
	NET_DVR_DEVICEINFO_V30 devInfoV30;//设备信息
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
		strMsg.Format(_T("登录失败。错误码:%ld"), err);
		OutputDebugString(strMsg);
		strResult.Format(_T("登录设备失败!"));
		//登录设备信息入队列
		m_FaildList.push(funParam);
		return strResult.AllocSysString();
	}

	NET_DVR_NTPPARA nNtpConfigInfo;
	memset(&nNtpConfigInfo, 0, sizeof(NET_DVR_NTPPARA));
	strcpy((char*)nNtpConfigInfo.sNTPServer, m_cNtpIp);
	nNtpConfigInfo.wNtpPort = m_iNtpPort;
	nNtpConfigInfo.wInterval = m_iTime ;
	nNtpConfigInfo.byEnableNTP = 1;
	nNtpConfigInfo.cTimeDifferenceH += 8;    //时区+8否则时区改变，不是北京时间
	if(!NET_DVR_SetDVRConfig(lLoginID, NET_DVR_SET_NTPCFG, 0xFFFFFFFF, &nNtpConfigInfo, sizeof(NET_DVR_NTPPARA)))
	{
		//登出设备
		int err = NET_DVR_GetLastError();
		NTP_LOG_ERROR("NET_DVR_GetDVRConfig falid, error = %d", err);
		NET_DVR_Logout_V30(lLoginID);
		CString strMsg;
		strMsg.Format(_T("设置NTP校时失败。错误码:%ld"), err);
		OutputDebugString(strMsg);
		strResult.Format(_T("设置NTP校时失败!"));
		//校时失败的设备信息入队列
		m_FaildList.push(funParam);
		return strResult.AllocSysString();
	}
	m_iSeccessful++;
	NTP_LOG_INFO("校时成功的设备IP:%s",funParam.cDvrIp);
	strResult = _T("1");
	//登出设备
	NET_DVR_Logout_V30(lLoginID);
	return strResult.AllocSysString();
}
