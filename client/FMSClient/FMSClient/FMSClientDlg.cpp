
// FMSClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMSClient.h"
#include "FMSClientDlg.h"
#include "Splash.h"
#include "MultiLang/lClient.h"
#include "ChangePasswordDlg.h"

#define WM_ICON_NOTIFY      (WM_USER + 102)

#define TIMER_OPEN_SOUND	1


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


// CFMSClientDlg �Ի���




CFMSClientDlg::CFMSClientDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CFMSClientDlg::IDD, pParent)
	,m_lpBusinessManageDlg(NULL)
	,m_lpBusinessListDlg(NULL)
	,m_lpConfigDlg(NULL)
	,m_lpBottomDlg(NULL)
	,m_pBtnBusinessMgr(NULL)
	,m_pBtnBusinessList(NULL)
	,m_pBtnBusinessCfg(NULL)
	,m_pBtnMenu(NULL)
	,m_hThread(NULL)
{
	this->SetDrawTitleIcon(FALSE);		//����ҪFDialog�Ի����ͼ�꣨��Ϊ���Լ��У�
	this->SetTitleLeftMargin(20);	//������ͼ����߾�20

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == m_hEvent)
	{
		FMSCLIENT_ERROR("CreateEvent Failed!");
	}
 	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
 	SetIcon(m_hIcon, TRUE);

	memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
}

CFMSClientDlg::~CFMSClientDlg()
{
	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
	}

	if (NULL != m_lpBottomDlg)
	{
		delete m_lpBottomDlg;
		m_lpBottomDlg = NULL;
	}

	if (NULL != m_lpBusinessManageDlg)
	{
		delete m_lpBusinessManageDlg;
		m_lpBusinessManageDlg = NULL;
	}

	if (NULL != m_lpBusinessListDlg)
	{
		delete m_lpBusinessListDlg;
		m_lpBusinessListDlg = NULL;
	}

	if (NULL != m_lpConfigDlg)
	{
		delete m_lpConfigDlg;
		m_lpConfigDlg = NULL;
	}
}

void CFMSClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMSClientDlg, CFDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ICON_NOTIFY, &CFMSClientDlg::OnNotifyIcon)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ICONNOTIFY_SHOW_WINDOW, &CFMSClientDlg::OnIconnotifyShowWindow)
	ON_COMMAND(ID_ICONNOTIFY_EXIT, &CFMSClientDlg::OnIconnotifyExit)
	ON_WM_DESTROY()
	 ON_MESSAGE(WM_ON_ALARM_NET_DISCONNT, OnNetDisconnect)
	 ON_MESSAGE(MSG_FORCE_LOGOFF, OnMsgRebootApp)
	 ON_WM_TIMER()
END_MESSAGE_MAP()

DWORD WINAPI CFMSClientDlg::WorkStateThread(LPVOID pParam)
{
	CFMSClientDlg * lpThis = (CFMSClientDlg*)pParam;
	if (!lpThis)
	{
		FMSCLIENT_ERROR("CFMSClientDlg::WorkStateThread lpThis is NULL.");
		return -1;
	}
	std::string strip = UTF8_MFCSTRW2A(CCMSMgr::SnglPtr()->GetClientIP());
	int nPort = theApp.m_nListenPort;
	cms_8120::RspClientInfo rspClientInfo;

	BOOL bFirst = TRUE;
	time_t ttStart = time(NULL);
	while(WaitForSingleObject(lpThis->m_hEvent, 10 * 1000) != WAIT_OBJECT_0) //ÿ��10sдһ������
	{
		time_t ttNow = time(NULL);
		if (ttNow - ttStart < 10)
		{
			continue;
		}
		ttStart = ttNow;
		BOOL bNetState = FALSE;
		if (bFirst)
		{
			bNetState = CCMSMgr::SnglPtr()->WriteWorkState(1, strip, nPort, rspClientInfo);
			bFirst = FALSE;
		}
		else
		{
			bNetState = CCMSMgr::SnglPtr()->WriteWorkState(0, strip, nPort, rspClientInfo);
		}
		int nLogoff = rspClientInfo.ismodifypwd();
		if (nLogoff == 1)
		{
			CString str = UTF8_A2MFCSTRW(rspClientInfo.result_info());
			FMSCLIENT_ERROR("Client Force Logoff:%d", nLogoff);
			lpThis->PostMessage(MSG_FORCE_LOGOFF, nLogoff);
		}
		lpThis->PostMessage(WM_ON_ALARM_NET_DISCONNT, bNetState);
	}

	return 0;
}

// CFMSClientDlg ��Ϣ�������

BOOL CFMSClientDlg::OnInitDialog()
{
	//�޸ı�����ϵͳ��ť���Ĵ��������ڻ���OnInitDialogǰ�����
	ModifyStyle(NULL, WS_MAXIMIZEBOX | WS_MINIMIZEBOX);

	CFDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	InitData();

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
	//InitDlg();

	//��ʾ���ؽ���
// 	CSplashDlg *pSplashDlg = new CSplashDlg;
// 	if (NULL == pSplashDlg)
// 	{
// 		FMSCLIENT_ERROR("New splash dlg failed");
// 	}
	//CSplash splash(pSplashDlg);
	//pSplashDlg->SetText(_T("��ȡ�û���Ϣ"));

	//���ô�������
	InitMainWnd();

	//����ģ�鴰��:�Ӵ���
	InitChildWnd();

	//�ƶ��Ӵ���
	MoveChildWnd();

	//Ԥ��������ʱ��
	SetTimer(TIMER_OPEN_SOUND, 2 * 1000, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFMSClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CFDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFMSClientDlg::OnPaint()
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
		CFDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFMSClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFMSClientDlg::InitMainWnd(void)
{
	LoadSkin();

	MoveWindow(DEFAULT_WND_SIZE);

	CenterWindow();

	//SetWindowText(theApp.m_sMainTitle);
	SetWindowText(theApp.m_sCustomTitle);
	this->SetTitleFont(YAHEI_24_B);
	//this->SetTitleFont(YAHEI_26_SB);

	

	//��������ť
	//....
}

void CFMSClientDlg::LoadSkin(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT);
	vecMargin.push_back(MARGIN_TOP);
	vecMargin.push_back(MARGIN_RIGHT);
	vecMargin.push_back(MARGIN_BOTTOM);
	SetMargin(vecMargin);

	SetObjectName(_T("main"));
}

void CFMSClientDlg::InitChildWnd(void)
{
// 	//Navibar
// 	if (NULL == m_pNavibarDlg.get())
// 	{
// 		CNavibarDlg * pDlg = new CNavibarDlg;
// 		if (NULL == pDlg)
// 		{
// 			FMSCLIENT_ERROR("Create Navi Fail:%d\n", GetLastError());
// 			return;
// 		}
// 		m_pNavibarDlg.reset(pDlg);
// 		m_pNavibarDlg->Create(CNavibarDlg::IDD, this);
// 		m_pNavibarDlg->ShowWindow(SW_SHOW);
// 	}
// 
// 	//BaseLayer
// 	if (NULL == m_pBaseLayer.get())
// 	{
// 		CBaseLayer *pDlg = new CBaseLayer;
// 		if (NULL == pDlg)
// 		{
// 			FMSCLIENT_ERROR("Create BaseLayer Fail:%d\n", GetLastError());
// 			return;
// 		}
// 		m_pBaseLayer.reset(pDlg);
// 		m_pBaseLayer->Create(NULL, _T("BaseLayer"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 1, 1), this, BASE_LAYER_ID);
// 	}
// 
// 	//BottomDlg
// 	if (NULL == m_pBottomDlg.get())
// 	{
// 		CBottomDlg *pDlg = new CBottomDlg;
// 		if (NULL == pDlg)
// 		{
// 			FMSCLIENT_ERROR("Create BottomDlg Fail:%d\n", GetLastError());
// 			return;
// 		}
// 		m_pBottomDlg.reset(pDlg);
// 		m_pBottomDlg->Create(CBottomDlg::IDD, this);
// 		m_pBottomDlg->ShowWindow(SW_SHOW);
// 	}


	m_pBtnMenu = AddCustomBtnToSysBtn(_T("custom"), _T(""), _T(""), SYS_BTN_MENU);
	CString strComment = GernateUserComment();
	AddCustomBtnToSysBtn(_T("btn_head"), fcU2T(theApp.m_pbmsgReqLogin.user_name().c_str()), strComment, SYS_BTN_USER, 75);
	m_pBtnBusinessCfg = AddCustomBtnToSysBtn(_T("btn_head"), _T("��������"), _T("��������"), SYS_BTN_CONFIG, 80);
	m_pBtnBusinessList = AddCustomBtnToSysBtn(_T("btn_head"), _T("ҵ���¼"), _T("ҵ���¼"), SYS_BTN_LIST, 80);
	m_pBtnBusinessMgr = AddCustomBtnToSysBtn(_T("btn_head"), _T("ҵ�����"), _T("ҵ�����"), SYS_BTN_BUSINESS, 80);
	if (NULL != m_pBtnBusinessMgr)
	{
		m_pBtnBusinessMgr->m_bAlwHover = TRUE;
	}

	//�������ô���
	if (NULL == m_lpConfigDlg)
	{
		m_lpConfigDlg = new CConfigDlg;
		if (NULL == m_lpConfigDlg)
		{
			FMSCLIENT_ERROR("Create ConfigDlg Fail:%d\n", GetLastError());
			return;
		}
		m_lpConfigDlg->Create(IDD_DLG_CONFIG);
		m_lpConfigDlg->ShowWindow(SW_HIDE);
	}

	//ҵ�������
	if (NULL == m_lpBusinessManageDlg)
	{
		m_lpBusinessManageDlg = new CBusinessManageDlg;
		if (NULL == m_lpBusinessManageDlg)
		{
			FMSCLIENT_ERROR("Create BusinessManageDlg Fail:%d\n", GetLastError());
			return;
		}
		m_lpBusinessManageDlg->Create(IDD_DLG_BUSINESSMANAGE, this);
		m_lpBusinessManageDlg->ShowWindow(SW_SHOW);
	}

	//ҵ���¼����
	if (NULL == m_lpBusinessListDlg)
	{
		m_lpBusinessListDlg = new CBusinessListDlg;
		if (NULL == m_lpBusinessListDlg)
		{
			FMSCLIENT_ERROR("Create BusinessListDlg Fail:%d\n", GetLastError());
			return;
		}
		m_lpBusinessListDlg->Create(IDD_DLG_BUSINESSLIST, this);
		m_lpBusinessListDlg->ShowWindow(SW_HIDE);
	}

	//״̬��
	if (NULL == m_lpBottomDlg)
	{
		m_lpBottomDlg = new CBottomDlg;
		if (NULL == m_lpBottomDlg)
		{
			FMSCLIENT_ERROR("Create BottomDlg Fail:%d\n", GetLastError());
			return;
		}
		m_lpBottomDlg->Create(IDD_DLG_BOTTOM);
		m_lpBottomDlg->ShowWindow(SW_SHOW);
	}

	return;
}

void CFMSClientDlg::MoveChildWnd(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcBusinessMgr(rcClient);
	rcBusinessMgr.bottom = rcClient.bottom - 28;
	if (NULL != m_lpBusinessManageDlg)
	{
		m_lpBusinessManageDlg->MoveWindow(rcBusinessMgr);
	}

	CRect rcBusinessListDlg(rcBusinessMgr);
	if (NULL != m_lpBusinessListDlg)
	{
		m_lpBusinessListDlg->MoveWindow(rcBusinessListDlg);
	}

	CRect rcConfigDlg(rcBusinessMgr);
	if (NULL != m_lpConfigDlg)
	{
		m_lpConfigDlg->MoveWindow(rcConfigDlg);
	}

	CRect rcBottom(rcClient);
	rcBottom.top = rcBusinessMgr.bottom;
	if (NULL != m_lpBottomDlg)
	{
		m_lpBottomDlg->MoveWindow(rcBottom);
	}



// 	if (m_pNavibarDlg)
// 	{
// 		CRect rcNavibar(rcClient);
// 		rcNavibar.top += 10;
// 		rcNavibar.bottom = rcNavibar.top + NAVIBAR_HEIGHT;
// 		m_pNavibarDlg->MoveWindow(rcNavibar);
// 	}
// 
// 	if (m_pBaseLayer)
// 	{
// 		CRect rcHomeDlg(rcClient);
// 		rcHomeDlg.top += NAVIBAR_HEIGHT;
// 		rcHomeDlg.bottom -= BOTTOM_HEIGHT;
// 		m_pBaseLayer->MoveWindow(rcHomeDlg);
// 	}
// 
// 	if (m_pBottomDlg)
// 	{
// 		CRect rcBottom(rcClient);
// 		rcBottom.top = rcBottom.bottom - BOTTOM_HEIGHT;
// 		m_pBottomDlg->MoveWindow(rcBottom);
// 	}
}



BOOL CFMSClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_NCLBUTTONDBLCLK)  ////���α�����˫���¼�(���/��ԭ)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)  
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:  //����Enter
			return TRUE;
		case VK_ESCAPE:   //����ESC��
			return TRUE;
		default:
			return CFDialog::PreTranslateMessage(pMsg);
		}
	}

	return CFDialog::PreTranslateMessage(pMsg);
}

BOOL CFMSClientDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.bottom = rcClient.bottom - 28;
	if (wParam == SYS_BTN_BUSINESS)
	{
		if (NULL != m_pBtnBusinessMgr && NULL != m_pBtnBusinessCfg && NULL != m_pBtnBusinessList)
		{
			m_pBtnBusinessCfg->m_bAlwHover = FALSE;
			m_pBtnBusinessList->m_bAlwHover = FALSE;
			m_pBtnBusinessMgr->m_bAlwHover = TRUE;		
		}
		m_lpBusinessManageDlg->ShowWindow(SW_SHOW);
		m_lpBusinessManageDlg->SetFocus();	//���ý���رյ����������ؼ���comboTree��
		m_lpBusinessListDlg->ShowWindow(SW_HIDE);
		m_lpConfigDlg->ShowWindow(SW_HIDE);
	}
	else if (wParam == SYS_BTN_LIST)
	{
		if (NULL != m_pBtnBusinessMgr && NULL != m_pBtnBusinessCfg && NULL != m_pBtnBusinessList)
		{
			m_pBtnBusinessCfg->m_bAlwHover = FALSE;
			m_pBtnBusinessList->m_bAlwHover = TRUE;
			m_pBtnBusinessMgr->m_bAlwHover = FALSE;		
		}
		m_lpBusinessManageDlg->ShowWindow(SW_HIDE);
		m_lpBusinessListDlg->MoveWindow(rcClient);
		m_lpBusinessListDlg->QueryBusiness(FALSE);
		m_lpBusinessListDlg->ShowWindow(SW_SHOW);
		m_lpBusinessListDlg->SetFocus();
		m_lpConfigDlg->ShowWindow(SW_HIDE);
	}
	else if (wParam == SYS_BTN_CONFIG)
	{
		if (NULL != m_pBtnBusinessMgr && NULL != m_pBtnBusinessCfg && NULL != m_pBtnBusinessList)
		{
			m_pBtnBusinessCfg->m_bAlwHover = TRUE;
			m_pBtnBusinessList->m_bAlwHover = FALSE;
			m_pBtnBusinessMgr->m_bAlwHover = FALSE;		
		}
		m_lpBusinessManageDlg->ShowWindow(SW_HIDE);
		m_lpBusinessListDlg->ShowWindow(SW_HIDE);
		m_lpConfigDlg->MoveWindow(rcClient);
		m_lpConfigDlg->InitData();
		m_lpConfigDlg->ShowWindow(SW_SHOW);
		m_lpConfigDlg->SetFocus();
	}
	else if (wParam == SYS_BTN_MENU)
	{
		this->SetFocus();
		CFMenu sysMenu;
		sysMenu.SetObjectName(_T("menu"));
		sysMenu.SetItemHeight(26);
		sysMenu.CreatePopupMenu();
		sysMenu.SetTextColor(FUi::Black, FUi::RGB_204);

		sysMenu.AddMenu(MF_STRING, SYS_MENU_MODPWD, _T("�޸�����"), _T("menu_pwd"));
		sysMenu.AddMenu(MF_STRING, SYS_MENU_LOGOUT, _T("ע����¼"), _T("menu_logout"));
		//sysMenu.AddMenu(MF_STRING, SYS_MENU_ABOUT, _T("��������"), _T("menu_item"));
		sysMenu.AddMenu(MF_STRING, SYS_MENU_EXIT, _T("�˳�ϵͳ"), _T("menu_exit"));

		CRect rect;
		if (NULL != m_pBtnMenu)
		{
			rect = m_pBtnMenu->m_rcRect;
		}
		ClientToScreen(rect);
// 		CPoint pt;
// 		GetCursorPos(&pt);

		int nCmd = (int)sysMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD, \
			rect.left, rect.top - 8, this);
		DoMenuCmd(nCmd);
	}
	else if (wParam == SYS_BTN_USER)
	{
		this->SetFocus();
	}

	return CFDialog::OnCommand(wParam, lParam);
}

void CFMSClientDlg::DoMenuCmd(int nCmd)
{
	switch (nCmd)
	{
	case SYS_MENU_MODPWD:
		{
// 			CChangePasswordDlg dlg;
// 			dlg.SetLoginState(true);
// 			dlg.DoModal();
			CString strUrl = theApp.GernateUrl();
			if (strUrl.IsEmpty())
			{
				FMessageBox::Show(_T("�������Ĳ�����"), _T("����"), MB_OK | MB_ICONERROR);
				return;
			}

			OpenUrl(strUrl);
			break;
		}
	case SYS_MENU_LOGOUT:
		{
			if (IDYES != FMessageBox::Show(APP_RELOGIN_CONFIRM, MSGBOX_TIPS, MB_YESNO | MB_ICONQUESTION))
			{
				return ;
			}

			TCHAR szPath[256] = {0};
			GetModuleFileName(NULL, szPath, 256);
			BeforeExit();
			theApp.m_bReboot = true;
			RebootApp(szPath);
			break;
		}
	case SYS_MENU_ABOUT:
		{
			CAboutDlg dlg;
			dlg.DoModal();
			break;
		}
	case SYS_MENU_EXIT:
		{
			if (IDYES != FMessageBox::Show(APP_EXIT_CONFIRM, MSGBOX_TIPS, MB_YESNO | MB_ICONQUESTION))
			{
				return;
			}
			// �ж��Ƿ��Զ���¼
			if (GetAutoLogin())
			{
				::WriteProfileString(_T("reboot"), _T("switch"), _T("1"));
			}
			else
			{
				::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));
			}
			BeforeExit();
			CDialog::OnCancel();
			break;
		}
	default:
		break;
	}
}

void CFMSClientDlg::OnCancel()
{
	if (theApp.m_bReboot)
	{
		//BeforeExit();
		CDialog::OnCancel();
		return;
	}

	// �ж��Ƿ��Զ���¼
	if (GetAutoLogin())
	{
		::WriteProfileString(_T("reboot"), _T("switch"), _T("1"));
	}
	else
	{
		::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));
	}

	//�����ļ�·��
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	int nClose = GetPrivateProfileInt(_T("LocalConfig"), _T("Close"), 1, strConfigPath);
	if (2 != nClose) //�رճ���
	{
		if (IDYES != FMessageBox::Show(APP_EXIT_CONFIRM, MSGBOX_TIPS, MB_YESNO | MB_ICONQUESTION))
		{
			return;
		}
		BeforeExit();
		CDialog::OnCancel();
	}
	else if (2 == nClose) //��С����ϵͳ����
	{
		AddNotifyIcon();
		ShowWindow(SW_HIDE);
		return;
	}
	
	return;
}

void CFMSClientDlg::BeforeExit()
{
	CFBusyWait busywait(theApp.m_pMainWnd);
	busywait.ShowBusyWait(_T("�����˳�"));

	ConnSessionMgr::SnglPtr()->StopIncoming();
	
	FMSCLIENT_INFO("Before Logout...");
	if (!CCMSMgr::SnglPtr()->ClientLogout())
	{
		FMSCLIENT_ERROR("ClientLogout Failed.");
		return;
	}
}


/** @fn       void CMainDialog::AddNotifyIcon(void)
 *  @brief    ���ϵͳ����ͼ��
 *  @param    void
 *  @return:  void
 */
void CFMSClientDlg::AddNotifyIcon(void)
{
    // ��NOTIFYICONDATA�ṹ��������ʼ��
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd   = m_hWnd;
    m_nid.hIcon  = m_hIcon;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;

    m_nid.uCallbackMessage = WM_ICON_NOTIFY;

    //CString appName(_T("Service Manager Tray"));

    TCHAR szServerNameBuff[MAX_PATH] = {0};
    //MultiLanRes_LoadString(IDS_SERVICE_NAME, szServerNameBuff, _countof(szServerNameBuff));

    lstrcpyn(m_nid.szTip, _T("˫¼�ͻ���"), ARRAYSIZE(m_nid.szTip));

    m_nid.uTimeout = 10;

    lstrcpyn(m_nid.szInfoTitle, szServerNameBuff, ARRAYSIZE(m_nid.szInfoTitle));

    lstrcpyn(m_nid.szInfo, szServerNameBuff, ARRAYSIZE(m_nid.szInfo));

    Shell_NotifyIcon(NIM_ADD, &m_nid);
}
/** @fn       void CMainDialog::RemoveNotifyIcon(void)
 *  @brief    ɾ��ϵͳ����ͼ��
 *  @param    void
 *  @return:  void
 */
void CFMSClientDlg::RemoveNotifyIcon(void)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

/** @fn       LRESULT CMainDialog::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_ICON_NOTIFY ��Ϣ��Ӧ
 *  @param    (WPARAM) wParam [IN] : 
 *            (LPARAM) lParam [IN] : ����
 *  @return:  LRESULT��������
 */
LRESULT CFMSClientDlg::OnNotifyIcon(WPARAM /*wParam*/, LPARAM lParam)
{
    UINT uMouseMsg = (UINT)lParam;

    switch (uMouseMsg)
    {
    case WM_LBUTTONDBLCLK:
        {
			ShowWindow(SW_RESTORE);
			SetForegroundWindow();
        }
        break;
    case WM_RBUTTONUP:
        {
            CPoint point;
            GetCursorPos(&point);
            SetForegroundWindow();

            CMenu popMenu;
            popMenu.LoadMenu(IDR_MENU_ICON_NOTIFY);
            CMenu *pSubMenu = (CMenu *)popMenu.GetSubMenu(0);

            pSubMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

            popMenu.DestroyMenu();
        }
        break;

    default:
        break;
    }

    return 0;
}


void CFMSClientDlg::OnIconnotifyShowWindow()
{
	// TODO: �ڴ���������������
	ShowWindow(SW_SHOW);
	SetForegroundWindow();
}

void CFMSClientDlg::OnIconnotifyExit()
{
	// TODO: �ڴ���������������
	BeforeExit();	
	 CDialog::OnCancel();
}


BOOL CFMSClientDlg::GetAutoLogin()
{
	CString strPath = theApp.GetAppPath() + LOGIN_FILE;
	std::string strPathA = fcT2A(strPath.GetBuffer());

	TiXmlDocument doc;
	if (!doc.LoadFile(strPathA))
	{
		return FALSE;
	}

	TiXmlElement *pRootNode = doc.FirstChildElement();	// root
	if (!pRootNode)
	{
		return FALSE;
	}

	TiXmlElement* pAutoLogin = pRootNode->FirstChildElement("auto_login");
	if (!pAutoLogin)
	{
		return FALSE;
	}

	BOOL bAutoLogin = (BOOL)atoi(pAutoLogin->GetText());

	return bAutoLogin;
}
void CFMSClientDlg::InitData(void)
{
	//��ʱˢ������״̬
	FMSCLIENT_INFO("InitData CurrentPID:%d", GetCurrentProcessId());
	ResetEvent(m_hEvent);
	m_hThread = CreateThread(NULL, 0, CFMSClientDlg::WorkStateThread, this, 0, NULL);
	if (NULL == m_hThread)
	{
		FMSCLIENT_ERROR("_beginthreadex WorkState Failed!%d", ::GetLastError());
		return;
	}

	return;
}

void CFMSClientDlg::UnitData(void)
{
	SetEvent(m_hEvent);
	if (NULL != m_hThread)
	{
		if (WaitForSingleObject(m_hThread, 3000) != WAIT_OBJECT_0)
		{
			FMSCLIENT_ERROR("Stop Thead Failed!");
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return;
}

void CFMSClientDlg::OnDestroy()
{
	CFDialog::OnDestroy();

	UnitData();
}

//������ʾ
LRESULT CFMSClientDlg::OnNetDisconnect(WPARAM wParam, LPARAM lParam) 
{
	//ָ��ʹ��ǰ���ж��Ƿ�Ϊ��
	if (NULL == m_lpBottomDlg)
	{
		return -1;
	}
	if (NULL == m_lpBusinessManageDlg)
	{
		return -1;
	}
	if (NULL == m_lpBusinessListDlg)
	{
		return -1;
	}

	BOOL bNetState = wParam;
	if (bNetState)
	{
		m_lpBottomDlg->SetAppState(APP_NORMAL);
	}
	else
	{
		if (m_lpBottomDlg->GetAppState() != APP_ERROR)
		{
			m_lpBottomDlg->SetAppState(APP_ERROR);
			//FMessageBox::Show(_T("�޷����ӵ�����ƽ̨�������������ӣ�"), _T("����"), MB_OK | MB_ICONERROR);	//��Ҫ��
		}		
	}
	m_lpBusinessManageDlg->EnableWindow(bNetState);
	m_lpBusinessListDlg->EnableWindow(bNetState);
	return 0;
}

//Ԥ��������ʱ��
void CFMSClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_OPEN_SOUND)
	{
		if (NULL != m_lpBusinessManageDlg)
		{
			int bRet  = m_lpBusinessManageDlg->StartSound();
			if (bRet)
			{
				KillTimer(nIDEvent);
			}
		}
	}

	CFDialog::OnTimer(nIDEvent);
}

CString CFMSClientDlg::GernateUserComment()
{
	CString strTemp;
	strTemp.AppendFormat(_T("�û�����%s\n"), fcU2T(theApp.m_pbmsgReqLogin.user_name().c_str()));
	strTemp.AppendFormat(_T("��ʵ������%s\n"), theApp.m_strSalesmanName);
	strTemp.AppendFormat(_T("���ţ�%s\n"), theApp.m_strSalesmanCode);
	strTemp.AppendFormat(_T("���ĵ�ַ��%s:%d\n"), theApp.m_strServerIp, theApp.m_nServerPort);

	return strTemp;
}

LRESULT CFMSClientDlg::OnMsgRebootApp(WPARAM wParam, LPARAM lParam)
{
	//BeforeExit();
	theApp.m_bReboot = true;
	TCHAR szPath[256] = {0};
	GetModuleFileName(NULL, szPath, 256);
	RebootApp(szPath, TRUE);

	return TRUE;
}