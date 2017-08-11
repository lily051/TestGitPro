
// ConfigToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"
#include "Tinyxml/tinyxml.h"
#include "Tinyxml/tinyxmlutils.h"
#include <vector>
#pragma comment(lib,"Iphlpapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ICON_NOTIFY      (WM_USER + 102)
#define CONFIG_FILE			"ServiceConfig.xml"
#define TOP_DIRECTORY_CONFIG_FILE   "../ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

/*******************************************************************************
*    Function:      AppFun_GetLocalIPAddr 
*    Description:   获取本机IP
*    Input:         
*    Output:        CString & csIPAddr：得到IP
*    Return:	    成功返回TRUE,否则FALSE 
*******************************************************************************/
BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);   

    TRACE("@ljg-----Begin");
	if(0 == WSAStartup(wVersionRequested,&wsaData))   
	{   
		if(0 == gethostname(name,sizeof(name)))   
		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				csIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}   
		}   
		WSACleanup();
	}   
    TRACE("@ljg-----End");
	if (!csIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}

void AppFun_GetAllLocalIPAddr(std::vector<CString> & vecIP)   
{       
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    pIPAddrTable = (MIB_IPADDRTABLE*) GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));
    if ( NULL != pIPAddrTable ) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            GlobalFree( pIPAddrTable );
            pIPAddrTable = (MIB_IPADDRTABLE *) GlobalAlloc (GPTR, dwSize);
        }
    }
    else
    {
        OutputDebugString(_T("@ljg-Memory allocation failed."));
    }

    if ( NULL != pIPAddrTable ) 
    {
        // Make a second call to GetIpAddrTable to get the
        // actual data we want
        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == NO_ERROR ) 
        { 
            for (unsigned int i = 0; i < dwSize/sizeof(MIB_IPADDRTABLE); ++i)
            {
                if ( 0 != pIPAddrTable->table[i].dwAddr)
                {
                    in_addr in;
                    in.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                    CString tempIP;
                    tempIP = ::inet_ntoa(in);
                    vecIP.push_back(tempIP);
                }
				else
				{
					OutputDebugString(_T("@ljg-pIPAddrTable->table[i].dwAddr == 0."));
				}
            }
        }
        else 
        {
            OutputDebugString(_T("@ljg-Call to GetIpAddrTable failed."));
        }
    }

    if ( NULL != pIPAddrTable)
    {
        GlobalFree( pIPAddrTable );
    }
}


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


// CConfigToolDlg 对话框




CConfigToolDlg::CConfigToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigToolDlg::IDD, pParent)
    , m_sOldDCPort(_T("8849"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
}

void CConfigToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_DATA_PORT, m_editDCPort);
    DDX_Control(pDX, IDC_IP_DC, m_dcIPCtrl);
    DDX_Control(pDX, IDC_LOCAL_IP, m_localIPCtrl);
    DDX_Control(pDX, IDC_HISTORY_IP, m_historyIP);
}

BEGIN_MESSAGE_MAP(CConfigToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICON_NOTIFY, &CConfigToolDlg::OnNotifyIcon)
	ON_COMMAND(ID_ICONNOTIFY_SHOW_WINDOW, &CConfigToolDlg::OnIconnotifyShowWindow)
	ON_COMMAND(ID_ICONNOTIFY_HIDE_WINDOW, &CConfigToolDlg::OnIconnotifyHideWindow)
	ON_COMMAND(ID_ICONNOTIFY_EXIT, &CConfigToolDlg::OnIconnotifyExit)
	ON_COMMAND(ID_SERVICECONTROL_START, &CConfigToolDlg::OnIconnotifyStart)
	ON_COMMAND(ID_SERVICECONTROL_STOP, &CConfigToolDlg::OnIconnotifyStop)
	ON_COMMAND(ID_SERVICECONTROL_RESTART, &CConfigToolDlg::OnIconnotifyRestart)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CConfigToolDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CConfigToolDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CConfigToolDlg 消息处理程序

BOOL CConfigToolDlg::OnInitDialog()
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

	// 添加系统托盘。
	AddNotifyIcon();

	// TODO: 在此添加额外的初始化代码
    m_editDCPort.SetLimitText(6);
    AppFun_GetAllLocalIPAddr(m_vecLocalIP);
	ReadConfig();

    m_dcIPCtrl.SetWindowText(m_sOldDCIP);
    m_editDCPort.SetWindowText(m_sOldDCPort);
    m_localIPCtrl.SetWindowText(m_sOldLocalIP);

	// 获得桌面大小
	CRect rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);   

	// 获得对话框大小
	CRect rectDlg;
	GetWindowRect(&rectDlg);
	int nW = rectDlg.Width();
	int nH = rectDlg.Height();

	// 将窗口设置到右下角
	::SetWindowPos(GetSafeHwnd(),HWND_BOTTOM,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_NOZORDER);

	// 动画显示弹窗效果
	AnimateWindow(800,AW_SLIDE|AW_VER_NEGATIVE);

	//任务栏隐藏
	//ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConfigToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConfigToolDlg::OnPaint()
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
HCURSOR CConfigToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/** @fn       LRESULT CMainDialog::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_ICON_NOTIFY 消息响应
 *  @param    (WPARAM) wParam [IN] : 
 *            (LPARAM) lParam [IN] : 命令
 *  @return:  LRESULT：处理结果
 */
LRESULT CConfigToolDlg::OnNotifyIcon(WPARAM /*wParam*/, LPARAM lParam)
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

// return 1 or 0
int is_valid_ip(const char *ip_str) 
{
    char temp[31];
    int a,b,c,d; 
    if(sscanf(ip_str, "%d.%d.%d.%d ",&a,&b,&c,&d) == 4 
        &&   a>=0   &&   a<=255 
        &&   b>=0   &&   b<=255 
        &&   c>=0   &&   c<=255   
        &&   d>=0   &&   d<=255)    
    {    
        sprintf(temp, "%d.%d.%d.%d",a,b,c,d);    //把格式化的数据写入字符串temp     
        if(strcmp(temp,ip_str)==0)     
        {    
            return 1;     
        }     
        else    
        {    
            return 0;
        }    
    }    
    else     
    {    
        return 0; 
    }    
}

//检查参数合法性
BOOL CConfigToolDlg::CheckParamInvalid()
{
    //检查参数的合法性
    DWORD dwDCAddress = 0;
    int nRet = m_dcIPCtrl.GetAddress(dwDCAddress);
    if (4 != nRet) 
    {
		MessageBox(_T("The FSvcDC IP Address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }
    if (0 == dwDCAddress)
    {
		MessageBox(_T("The FSvcDC IP Address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }

    DWORD dwLocalAddress = 0;
    nRet = m_localIPCtrl.GetAddress(dwLocalAddress);
    if (4 != nRet) 
    {
		MessageBox(_T("The machine IP address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }
    if (0 == dwLocalAddress)
    {
		MessageBox(_T("The machine IP address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }

    CString csDCIP,csDCPort,csLocalIP;
    m_dcIPCtrl.GetWindowText(csDCIP);
    csDCIP.Trim();
    m_editDCPort.GetWindowText(csDCPort);
    csDCPort.Trim();
    m_localIPCtrl.GetWindowText(csLocalIP);
    csLocalIP.Trim();

    if (!is_valid_ip(CStringA(csDCIP)))
    {
        MessageBox(_T("The FSvcDC IP Address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }

    if (csDCPort.IsEmpty())
    {
		MessageBox(_T("The FSvcDC Port cannot be empty!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_editDCPort.SetFocus();
        m_editDCPort.SetSel(0, -1);
        return FALSE;
    }

    int nDCPort = _tstoi(csDCPort);
    if (nDCPort<1024 || nDCPort >65535)
    {
		MessageBox(_T("The FSvcDC Port range is 1024~65535!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_editDCPort.SetFocus();
        m_editDCPort.SetSel(0, -1);
        return FALSE;
    }

    if (!is_valid_ip(CStringA(csLocalIP)))
    {
        MessageBox(_T("The machine IP address is not valid!"),_T("WARNING"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }

    return TRUE;
}

// 保存
void CConfigToolDlg::OnBnClickedBtnSave()
{
    //检查参数合法性
    if (!CheckParamInvalid())
    {
        return;
    }

    //////////////////////////////////////
	SaveConfig();
    m_dcIPCtrl.GetWindowText(m_sOldDCIP);
    m_editDCPort.GetWindowText(m_sOldDCPort);
    m_localIPCtrl.GetWindowText(m_sOldLocalIP);

	//CDialog::OnCancel();
	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	// 重启
	runDir.Format(_T("%srestart.bat"), rootPath);
	RunCmd(runDir, runDir);

	//MessageBox(_T("Successfully saved!"),_T("INFORMATION"),MB_ICONINFORMATION|MB_OK);
}

void CConfigToolDlg::OnBnClickedBtnExit()
{
	CConfigToolDlg::OnCancel();
}

//读取配置文件
void CConfigToolDlg::ReadConfig()
{
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(CONFIG_FILE))
	{
        if (!xmlDoc.LoadFile(TOP_DIRECTORY_CONFIG_FILE))
        {
            return;
        }
	}

	TiXmlHandle  docHandle(&xmlDoc);

	//本地参数
	do 
	{
		TiXmlElement * pLocalNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_LOCAL_PARAM).ToElement();
		if (pLocalNode)
        {
            std::_tstring sIP = CSC::UTF82Tstring(pLocalNode->Attribute("ip"));
            m_sOldLocalIP = sIP.c_str();
		}
	} while (0);

	//数据中心
	do 
	{
		TiXmlElement * pDataCenterNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_DATA_CENTER).ToElement();
		if (pDataCenterNode)
		{
            m_sOldDCIP = CSC::UTF82Tstring(pDataCenterNode->Attribute("ip")).c_str();
			m_sOldDCPort = CSC::UTF82Tstring(pDataCenterNode->Attribute("port")).c_str();
        }
	} while (0);
}

/*lint -e429*/ 
//保存配置文件
void CConfigToolDlg::SaveConfig()
{
    CString csDCIP,csDCPort,csLocalIP;
    m_dcIPCtrl.GetWindowText(csDCIP);
    m_editDCPort.GetWindowText(csDCPort);
    m_localIPCtrl.GetWindowText(csLocalIP);

    //构建报文
	TiXmlDocument *pDoc = new TiXmlDocument;
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","UTF-8","");  
	pDoc->LinkEndChild(pDeclaration);

	TiXmlElement *pRootNode = new TiXmlElement(NODE_ROOT);  
	pDoc->LinkEndChild(pRootNode);  

	//本地参数
	do 
	{
		TiXmlElement *pLocalNode = new TiXmlElement(NODE_LOCAL_PARAM);  
		pRootNode->LinkEndChild(pLocalNode);  

		std::_tstring sLocalIP(csLocalIP.GetBuffer());
		csLocalIP.ReleaseBuffer();
		pLocalNode->SetAttribute("ip", CSC::Tstring2UTF8(sLocalIP));

// 		pLocalNode->SetAttribute("loginname", "admin");
// 		pLocalNode->SetAttribute("loginpwd", "888");
	} while (0);

	//数据中心
	do 
	{
		TiXmlElement *pDataCenterNode = new TiXmlElement(NODE_DATA_CENTER);  
		pRootNode->LinkEndChild(pDataCenterNode);  

		std::_tstring sDataCenterIP(csDCIP.GetBuffer());
		csDCIP.ReleaseBuffer();
		pDataCenterNode->SetAttribute("ip", CSC::Tstring2UTF8(sDataCenterIP));

		std::_tstring sDataCenterPort(csDCPort.GetBuffer());
		csDCPort.ReleaseBuffer();
		pDataCenterNode->SetAttribute("port", CSC::Tstring2UTF8(sDataCenterPort));
	} while (0);

	pDoc->SaveFile(CONFIG_FILE);
	delete pDoc;
}

/*lint +e429*/ 

BOOL CConfigToolDlg::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            if (VK_F4 == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CConfigToolDlg::OnCancel()
{
  //  CString sCMSIP,sCMSPort,sLocalIP;
  //  m_dcIPCtrl.GetWindowText(sCMSIP);
  //  m_editDCPort.GetWindowText(sCMSPort);
  //  m_localIPCtrl.GetWindowText(sLocalIP);

  //  if (m_sOldDCIP != sCMSIP
  //      || m_sOldDCPort != sCMSPort
  //      || m_sOldLocalIP != sLocalIP)
  //  {
		//if (IDYES == MessageBox(_T("Do you want to save it?"),_T("INFORMATION"),MB_YESNO|MB_ICONQUESTION))
  //      {
  //          //检查参数合法性
  //          if (!CheckParamInvalid())
  //          {
  //              return;
  //          }

  //          SaveConfig();
  //      }
  //  }

  //  CDialog::OnCancel();
	ShowWindow(SW_HIDE);
}

void CConfigToolDlg::IPHistoryDropdown(void)
{
    int ipCount = m_vecLocalIP.size();
    if (ipCount <= 0)
    {
        return;
    }

    CMenu IPListMenu;
    IPListMenu.CreatePopupMenu();	// create menu

    for (int i = 0; i < ipCount; ++i)
    {
        if (!m_vecLocalIP[i].IsEmpty())
        {
            //设置的ID值在TPM_RETURNCMD中会返回，尽量不要设置为0，因为未选中时返回的也是0
            IPListMenu.AppendMenu(MF_STRING | MF_ENABLED, i+1, m_vecLocalIP[i]);
        }
    }

    POINT curPos;
    GetCursorPos(&curPos);
    SetForegroundWindow();

    // show the menu
    int nSel = IPListMenu.TrackPopupMenu(
        TPM_LEFTBUTTON | TPM_RIGHTALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
        curPos.x, curPos.y + 10, this);

    if (nSel > 0)
    {
        // fill the IP address control with the selection in the menu
        m_localIPCtrl.SetWindowText(m_vecLocalIP[nSel-1]);
        ::PostMessage(m_hWnd, 0, 0, 0);
    }


    // destroy pop up menu
    IPListMenu.DestroyMenu();
}

/** @fn       void CConfigToolDlg::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    运行命令
 *  @param    (CString) cmd [IN] : ShellExecute的命令
 *            (CString) DefaultDir [IN] : 默认的运行目录
 *  @return:  void
 */
BOOL CConfigToolDlg::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // 日志
	CString strDebug = _T("");
	strDebug.Format(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);
	OutputDebugString(strDebug);

    // 执行命令
    HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, DefaultDir, NULL, SW_RESTORE);
	//ShellExecute(NULL, _T("open"), runDir, rootPath, NULL, SW_RESTORE);

    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        MessageBox(_T("内存不足"), _T("Error"));
        break;
    case ERROR_FILE_NOT_FOUND:
        MessageBox(_T("文件名或路径名错误"), _T("Error"));
        break;
    case ERROR_PATH_NOT_FOUND:
        MessageBox(_T("文件名或路径名错误"), _T("Error"));
        break;
    case ERROR_BAD_FORMAT:
        MessageBox(_T("ERROR_BAD_FORMAT"), _T("Error"));
        break;
    case SE_ERR_ASSOCINCOMPLETE:
        MessageBox(_T("SE_ERR_ASSOCINCOMPLETE"), _T("Error"));
        break;
    case SE_ERR_ACCESSDENIED:
        MessageBox(_T("SE_ERR_ACCESSDENIED"), _T("Error"));
        break;
    case SE_ERR_DDEBUSY:
        MessageBox(_T("SE_ERR_DDEBUSY"), _T("Error"));
        break;
    case SE_ERR_DDEFAIL:
        MessageBox(_T("SE_ERR_DDEFAIL"), _T("Error"));
        break;
    case SE_ERR_DDETIMEOUT:
        MessageBox(_T("SE_ERR_DDETIMEOUT"), _T("Error"));
        break;
    case SE_ERR_DLLNOTFOUND:
        MessageBox(_T("SE_ERR_DLLNOTFOUND"), _T("Error"));
        break;
    case SE_ERR_NOASSOC:
        MessageBox(_T("SE_ERR_NOASSOC"), _T("Error"));
        break;
    case SE_ERR_OOM:
        MessageBox(_T("SE_ERR_OOM"), _T("Error"));
        break;
    case SE_ERR_SHARE:
        MessageBox(_T("SE_ERR_SHARE"), _T("Error"));
        break;
    default:
        bRet = TRUE;
        break;
    }

    OutputDebugString(_T("---------------------> RunCmd end.\n"));
    return bRet;
}

/** @fn       void CMainDialog::AddNotifyIcon(void)
 *  @brief    添加系统托盘图标
 *  @param    void
 *  @return:  void
 */
void CConfigToolDlg::AddNotifyIcon(void)
{
    // 对NOTIFYICONDATA结构各参数初始化
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd   = m_hWnd;
    m_nid.hIcon  = m_hIcon;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;

    m_nid.uCallbackMessage = WM_ICON_NOTIFY;

    //CString appName(_T("Service Manager Tray"));

    TCHAR szServerNameBuff[MAX_PATH] = {0};
    //MultiLanRes_LoadString(IDS_SERVICE_NAME, szServerNameBuff, _countof(szServerNameBuff));

    lstrcpyn(m_nid.szTip, _T("理财上传客户端"), ARRAYSIZE(m_nid.szTip));

    m_nid.uTimeout = 10;

    lstrcpyn(m_nid.szInfoTitle, szServerNameBuff, ARRAYSIZE(m_nid.szInfoTitle));

    lstrcpyn(m_nid.szInfo, szServerNameBuff, ARRAYSIZE(m_nid.szInfo));

    Shell_NotifyIcon(NIM_ADD, &m_nid);
}
/** @fn       void CMainDialog::RemoveNotifyIcon(void)
 *  @brief    删除系统托盘图标
 *  @param    void
 *  @return:  void
 */
void CConfigToolDlg::RemoveNotifyIcon(void)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
/** @fn       void CMainDialog::OnIconnotifyShowWindow()
 *  @brief    显示窗口菜单项响应
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyShowWindow()
{
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();
}

/** @fn       void CMainDialog::OnIconnotifyHideWindow()
 *  @brief    隐藏窗口菜单项响应
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyHideWindow()
{
    ShowWindow(SW_HIDE);
}

/** @fn       void CMainDialog::OnIconnotifyExit()
 *  @brief    退出菜单项响应
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyExit()
{
    CDialog::OnCancel();
}

/** @fn       void CMainDialog::OnIconnotifyStart()
 *  @brief    运行
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyStart()
{
    //CDialog::OnCancel();
	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	runDir.Format(_T("%sstart.bat"), rootPath);

	//运行命令
	RunCmd(runDir, runDir);
}

/** @fn       void CMainDialog::OnIconnotifyStop()
 *  @brief    停止
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyStop()
{
    //CDialog::OnCancel();
	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	runDir.Format(_T("%sstop.bat"), rootPath);

	//运行命令
	RunCmd(runDir, runDir);
}

/** @fn       void CMainDialog::OnIconnotifyRestart()
 *  @brief    重启
 *  @param
 *  @return:  void
 */
void CConfigToolDlg::OnIconnotifyRestart()
{
    //CDialog::OnCancel();
	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}
	CString runDir;
	// 重启
	runDir.Format(_T("%srestart.bat"), rootPath);
	RunCmd(runDir, runDir);
}