
// VideoCollect_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoCollect_Client.h"
#include "VideoCollect_ClientDlg.h"
#include "SQLiteHelper.h"
//#include "MySqlDBLayer.h"
//#include "SimpleHttp.h"

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


// CVideoCollect_ClientDlg 对话框



CVideoCollect_ClientDlg::CVideoCollect_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoCollect_ClientDlg::IDD, pParent),m_bLogin(false),m_bPlaying(false),
    m_bPlayingBack(false),m_bPlayingBackPause(false),m_lPlayHandle(-1),m_lPlayBackHandle(-1),
    m_iCurChanIndex(-1),m_strDevIP(_T("127.0.0.1")),m_strDevPort(_T("8000")),
    m_strDevUser(_T("admin")),m_strDevPassword(_T("12345")),m_hThreadVideoPlayerBack(NULL),m_hThreadVoicePlayerBack(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_GFBANK);
}
CVideoCollect_ClientDlg::~CVideoCollect_ClientDlg()
{
    try
    {
        if(NULL != m_hThreadVideoPlayerBack)
        {
            ::CloseHandle(m_hThreadVideoPlayerBack);
            m_hThreadVideoPlayerBack = NULL;
        }
        if(NULL != m_hThreadVoicePlayerBack)
        {
            ::CloseHandle(m_hThreadVoicePlayerBack);
            m_hThreadVoicePlayerBack = NULL;
        }
        
    }
    catch (...)
    {
    	
    }

}
void CVideoCollect_ClientDlg::OnOK()
{
    return;
}
BOOL CVideoCollect_ClientDlg::PreTranslateMessage(MSG* pMsg)
{
    //屏蔽ESC关闭窗体/
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return TRUE;
    //屏蔽回车关闭窗体,但会导致回车在窗体上失效.
    //if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) return TRUE;
    else
        return CDialog::PreTranslateMessage(pMsg);
}
//
void CVideoCollect_ClientDlg::OnCancel()
{
    // 停止播放-退出设备
    if(m_bPlaying)
    {
        StopPlay(m_lPlayHandle);
    }
    //
    if(m_bPlayingBack)
    {
        StopPlayBack(m_lPlayBackHandle);
    }
    if(-1 != m_struDeviceInfo.lLoginID)
    {
        if(!NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID))
        {
            wchar_t ErrExplain[40];
            wsprintf(ErrExplain,_T("登出失败(错误码:%d)"),NET_DVR_GetLastError());
            MessageBox(ErrExplain,_T("提示"),MB_ICONWARNING);
            return;
        }
        m_ctrlTreeChan.DeleteAllItems();
        m_struDeviceInfo.lLoginID = -1;
        m_bLogin = false;
        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登录"));
    }

    SaveParamLogin();
    CDialog::OnCancel();
}
void CVideoCollect_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_CHAN, m_ctrlTreeChan);
    DDX_Control(pDX, IDC_IPADDRESS_DEV, m_ctrlDevIp);
    DDX_Control(pDX, IDC_EDIT_PORT, m_editDevPort);
    DDX_Control(pDX, IDC_EDIT_USER, m_editDevUserName);
    DDX_Control(pDX, IDC_EDIT_PWD, m_editDevPassWord);

    DDX_Control(pDX, IDC_TIME_START, m_TimeStart);
    DDX_Control(pDX, IDC_TIME_END, m_TimeEnd);
    DDX_Control(pDX, IDC_DATE_END, m_DateEnd);
    DDX_Control(pDX, IDC_DATE_START, m_DateStart);

    DDX_Control(pDX, IDC_OCX1, m_Player);//绑定播放器控件
    //DDX_Control(pDX, IDC_OCX1, m_control);
}

BEGIN_MESSAGE_MAP(CVideoCollect_ClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CVideoCollect_ClientDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CVideoCollect_ClientDlg::OnBnClickedButtonPlay)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CHAN, &CVideoCollect_ClientDlg::OnClinkSelectTreeChan)//单击选中通道映射
    ON_NOTIFY(NM_DBLCLK, IDC_TREE_CHAN, &CVideoCollect_ClientDlg::OnDoubleHitSelectTreeChan)
    ON_BN_CLICKED(IDC_BUTTON_PLAYBACK, &CVideoCollect_ClientDlg::OnBnClickedButtonPlayback)
    ON_BN_CLICKED(IDC_BUTTON_PLAYBACK_OUT, &CVideoCollect_ClientDlg::OnBnClickedButtonPlaybackOut)
END_MESSAGE_MAP()
// CVideoCollect_ClientDlg 消息处理程序

unsigned int _stdcall VideoPlayBack(LPVOID lpParam)
{
    CVideoCollect_ClientDlg* pVideo = static_cast<CVideoCollect_ClientDlg*>(lpParam);
    if (NULL == pVideo)
    {
        return 1;
    }
    pVideo ->StartPlayBack();
    return 0;
}
unsigned int _stdcall VoicePlayBack(LPVOID lpParam)
{
    CVideoCollect_ClientDlg* pVoice = static_cast<CVideoCollect_ClientDlg*>(lpParam);
    if (NULL == pVoice)
    {
        return 1;
    }
    pVoice ->StartPlayBackVoice();
    return 0;
}

//
BOOL CVideoCollect_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    SetWindowText(_T("音频视频回放客户端"));
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

    CButton *pButton;
    pButton = (CButton *)GetDlgItem(IDC_BUTTON_PLAYBACK_OUT);
    pButton->SetIcon(m_hStopDisableIcon);
    pButton->EnableWindow(FALSE);
	// TODO: 在此添加额外的初始化代码
    CTime timeDataStart(2000,1,1,0,0,0); 
    //CDateTimeCtrl* pCtrlData = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_START);
    //ASSERT(pCtrlData != NULL);
    //VERIFY(pCtrlData->SetTime(&timeData));
    CDateTimeCtrl* pCtrlTimeStart = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_START);
    ASSERT(pCtrlTimeStart != NULL);
    VERIFY(pCtrlTimeStart->SetTime(&timeDataStart));

    CTime timeDataStop(2000,1,1,23,59,59); 
    CDateTimeCtrl* pCtrlTimeStop = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_END);
    ASSERT(pCtrlTimeStop != NULL);
    VERIFY(pCtrlTimeStop->SetTime(&timeDataStop));
    UpdateData(TRUE);
    // SDK标准初始化
    NET_DVR_Init();
    BuildDefaultFile();
    InitConfig();
    //m_ctrlDevIp.SetAddress(10,16,37,105);

    m_control=static_cast<CWMPControls>(m_Player.get_controls());
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoCollect_ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoCollect_ClientDlg::OnPaint()
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
HCURSOR CVideoCollect_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//登录设备
bool CVideoCollect_ClientDlg::DoLogin()
{
    UpdateData(TRUE);
    CString sDeviceIP = _T("");
    CString sDevicePort = _T("");
    CString sDeviceUserNmae = _T("");
    CString sDevicePassWord = _T("");
    m_ctrlDevIp.GetWindowText(sDeviceIP);
    sDeviceIP.Trim();
    m_editDevPort.GetWindowText(sDevicePort);
    sDevicePort.Trim();
    m_editDevUserName.GetWindowText(sDeviceUserNmae);
    sDeviceUserNmae.Trim();
    m_editDevPassWord.GetWindowText(sDevicePassWord);
    sDevicePassWord.Trim();
    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
    memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
    USES_CONVERSION;
    LONG lLoginID = NET_DVR_Login_V30(T2A(sDeviceIP.GetBuffer(sDeviceIP.GetLength())),\
        atoi(T2A(sDevicePort.GetBuffer(sDevicePort.GetLength()))),T2A(sDeviceUserNmae.GetBuffer(sDeviceUserNmae.GetLength())),\
        T2A(sDevicePassWord.GetBuffer(sDevicePassWord.GetLength())),&DeviceInfoTmp);
    if(0 != lLoginID)
    {
        wchar_t ErrExplain[40];
        wsprintf(ErrExplain,_T("登录失败(err= %d)"),NET_DVR_GetLastError());
        MessageBox(ErrExplain,_T("提示"),MB_ICONWARNING);
        return false;
    }
    m_struDeviceInfo.lLoginID = lLoginID;
    m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;
    m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;
    m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;
    m_struDeviceInfo.iIPStartChan  = DeviceInfoTmp.byStartDChan;
    return true;
}
//设备资源获取
void CVideoCollect_ClientDlg::DoGetDeviceResoureCfg()
{
    DWORD  dwReturned;
    NET_DVR_IPPARACFG_V40 IpAccessCfg;
    memset(&IpAccessCfg,0,sizeof(IpAccessCfg));
    m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG_V40,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG_V40),&dwReturned);
    if(!m_struDeviceInfo.bIPRet)   //不支持ip接入,9000以下设备不支持禁用模拟通道
    {
        for(int i = 0; i < MAX_ANALOG_CHANNUM; i++)
        {
            if (i < m_struDeviceInfo.iDeviceChanNum)
            {
                sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camf%d",i + m_struDeviceInfo.iStartChan);
                m_struDeviceInfo.struChanInfo[i].iChanIndex = i + m_struDeviceInfo.iStartChan;  //通道号
                m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
            }
            else
            {
                m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
                m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
                sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
            }
        }
    }
    else        //支持IP接入，9000设备
    {
        for(int i = 0; i < MAX_ANALOG_CHANNUM; i++)  //模拟通道
        {
            if (i < m_struDeviceInfo.iDeviceChanNum)
            {
                sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"模拟通道%d",i + m_struDeviceInfo.iStartChan);
                m_struDeviceInfo.struChanInfo[i].iChanIndex = i + m_struDeviceInfo.iStartChan;
                if(IpAccessCfg.byAnalogChanEnable[i])
                {
                    m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
                }
                else
                {
                    m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
                }
            }
            else//clear the state of other channel
            {
                m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
                m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
                sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
            }		
        }
        //数字通道
        for(int i = 0; i < MAX_IP_CHANNEL; i++)
        {
            if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ip通道在线
            {
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = TRUE;
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = i + IpAccessCfg.dwStartDChan;
                sprintf(m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].chChanName,"数字通道 %d",i+1);
            }
            else
            {
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = FALSE;
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = -1;
            }
        }
    }
}
//创建通道树
void CVideoCollect_ClientDlg::CreateDeviceTree()
{
    USES_CONVERSION;
    HTREEITEM ChanItem;
    m_hDevItem = m_ctrlTreeChan.InsertItem(_T("通道列表"));
    m_ctrlTreeChan.SetItemData(m_hDevItem,DEVICETYPE * 1000);
    for(int i = 0; i < MAX_CHANNUM_V30; i++)            //MAX_CHANNUM_V30最大支持的通道数 最大模拟加上最大IP支持64
    {
        if(m_struDeviceInfo.struChanInfo[i].bEnable)    //通道有效，插入通道树
        {
            ChanItem = m_ctrlTreeChan.InsertItem(A2T(m_struDeviceInfo.struChanInfo[i].chChanName),m_hDevItem);
            m_ctrlTreeChan.SetItemData(ChanItem,CHANNELTYPE * 1000 + i);   //Data对应通道在数组中的索引
        }
    }
    m_ctrlTreeChan.Expand(m_hDevItem,TVE_EXPAND);
}
//单击选中通道
void CVideoCollect_ClientDlg::OnClinkSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();//检索当前选中的项的树视图控件
    if(NULL == hSelected)
        return;
    DWORD itemData = m_ctrlTreeChan.GetItemData(hSelected);//获取之前插入对应行索引值
    //确定设备类型下通道索引
    int itype = itemData/1000;
    int iIndex = itemData%1000;
    switch(itype)
    {
    case DEVICETYPE:
        m_iCurChanIndex = -1;
        break;
    case CHANNELTYPE:
        m_iCurChanIndex = iIndex;
        TRACE("select chan index: %d\n",iIndex);
        break;
    default:
        break;
    }
    *pResult = 0;
}
//双击选中通道且实现播放
void CVideoCollect_ClientDlg::OnDoubleHitSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();//检索当前选中的项的树视图控件
    if(NULL == hSelected)
        return;
    DWORD itemData = m_ctrlTreeChan.GetItemData(hSelected);//获取之前插入对应行索引值
    //确定设备类型下通道索引
    int itype = itemData/1000;
    int iIndex = itemData%1000;
    switch(itype)
    {
    case DEVICETYPE:
        m_iCurChanIndex = -1;
        break;
    case CHANNELTYPE:
        {
            m_iCurChanIndex = iIndex;
            TRACE("select chan index: %d\n",iIndex);
            if (!m_bPlaying)
                StartPlay(m_iCurChanIndex);
            else
            {
                StopPlay(m_lPlayHandle);
                StartPlay(m_iCurChanIndex);
            }
        }
        break;
    default:
        break;
    }
    *pResult = 0;
}
//开始预览
void CVideoCollect_ClientDlg::StartPlay(int& iChanIndex)
{
    NET_DVR_CLIENTINFO ClientInfo;
    ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;
    ClientInfo.lChannel     = m_iCurChanIndex + 1;
    ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
    TRACE("Channel number:%d\n",ClientInfo.lChannel);
    m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
    if(-1 == m_lPlayHandle)
    {
        CString m_csErr;
        m_csErr.Format(_T("播放失败(错误代码:%d)"),NET_DVR_GetLastError());
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    m_bPlaying = true;
    GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("停止预览"));
}
//停止预览
void CVideoCollect_ClientDlg::StopPlay(LONG& m_lPlayHandle)
{
    if(-1 != m_lPlayHandle)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle = -1;
        m_bPlaying = false;
        GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("预览"));
        GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////
void CVideoCollect_ClientDlg::OnBnClickedButtonLogin()
{
    // TODO: 在此添加控件通知处理程序代码
    if(!m_bLogin)
    {
        //设备登陆参数保存
        SaveParamLogin();
        //设备登录注册
        if(!DoLogin())
            return;
        //获取设备资源与创建通道树
        DoGetDeviceResoureCfg();
        CreateDeviceTree();
        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("退出"));
        m_bLogin = true;
    }
    else
    {
        // 判断是否还在播放
        if(m_bPlaying || m_bPlayingBack)
        {
            MessageBox(_T("请先停止播放或退出回放!"),_T("提示"),MB_ICONWARNING);
            return;
        }
        //登出操作
        if(!NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID))
        {
            wchar_t ErrExplain[40];
            wsprintf(ErrExplain,_T("登出失败(错误码:%d)"),NET_DVR_GetLastError());
            MessageBox(ErrExplain,_T("提示"),MB_ICONWARNING);
            return;
        }
        m_ctrlTreeChan.DeleteAllItems();
        m_struDeviceInfo.lLoginID = -1;
        m_bLogin = false;
        m_iCurChanIndex = -1;
        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登录"));
    }
}

void CVideoCollect_ClientDlg::OnBnClickedButtonPlay()
{
    // TODO: 在此添加控件通知处理程序代码
    //播放通道号确定
    if (!m_bLogin)
    {
        MessageBox(_T("请先登录再播放!"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if (-1 == m_iCurChanIndex)
    {
        MessageBox(_T("请选择一个需要的通道播放"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if(m_bPlayingBack)
    {
        MessageBox(_T("请退出回放再预览"),_T("提示"),MB_ICONWARNING);
        return;
    }
    //确定是否播放
    if (!m_bPlaying)
        StartPlay(m_iCurChanIndex);
    else
        StopPlay(m_lPlayHandle);
}

void CVideoCollect_ClientDlg::StartVideoLinkCheck()
{
}
//////////////////////////////////////////////////////////////////////////回放
void CVideoCollect_ClientDlg::GetStartStopTime(NET_DVR_TIME& StartSearchTime,NET_DVR_TIME& StopSearchTime)
{
    CTime StartDay,StartTime, StopDay,StopTime;
    UpdateData(TRUE);
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);

    StartSearchTime.dwYear = StartDay.GetYear();
    StartSearchTime.dwMonth = StartDay.GetMonth();
    StartSearchTime.dwDay = StartDay.GetDay();
    StartSearchTime.dwHour = StartTime.GetHour();
    StartSearchTime.dwMinute = StartTime.GetMinute();
    StartSearchTime.dwSecond = StartTime.GetSecond();

    StopSearchTime.dwYear = StopDay.GetYear();
    StopSearchTime.dwMonth = StopDay.GetMonth();
    StopSearchTime.dwDay = StopDay.GetDay();
    StopSearchTime.dwHour = StopTime.GetHour();
    StopSearchTime.dwMinute = StopTime.GetMinute();
    StopSearchTime.dwSecond = StopTime.GetSecond();
    return;
}
void CVideoCollect_ClientDlg::StartPlayBack()
{
    NET_DVR_TIME StartSearchTime,StopSearchTime;
    GetStartStopTime(StartSearchTime,StopSearchTime);
    //播放窗口句柄
    HWND hPlayWnd = GetDlgItem(IDC_STATIC_PLAY)->GetSafeHwnd();//GetDlgItem(IDC_STATIC_PLAY)->m_hWnd
    m_lPlayBackHandle = NET_DVR_PlayBackByTime(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,
        &StartSearchTime,&StopSearchTime,hPlayWnd);
    if(-1 == m_lPlayBackHandle)
    {
        CString m_csErr;
        m_csErr.Format(_T("回放失败(错误代码:%d)"),NET_DVR_GetLastError());
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    if(!NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYSTART,0,NULL))
    {
        CString m_csErr;
        m_csErr.Format(_T("回放控制失败(错误代码:%d)"),NET_DVR_GetLastError());
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    m_bPlayingBack = true;
    GetDlgItem(IDC_BUTTON_PLAYBACK)->SetWindowText(_T("暂停回放"));
    SetPlayState(PLAY_NORMAL);
}
void CVideoCollect_ClientDlg::StopPlayBack(LONG& m_lPlayBackHandle)
{
    if(!m_bPlayingBackPause)
    {
        m_control.pause();//暂停
        NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYPAUSE,0,NULL);
        m_bPlayingBackPause = true;
        GetDlgItem(IDC_BUTTON_PLAYBACK)->SetWindowText(_T("继续回放"));
    }
    else
    {
        m_control.play();//继续播放 
        //速度控制省略,以后需要再加
        bool ret = NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYRESTART,0,NULL);
        if(!ret)
        {
            CString m_csErr;
            m_csErr.Format(_T("回放暂停失败(错误代码:%d)"),NET_DVR_GetLastError());
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        m_bPlayingBackPause = false;
        GetDlgItem(IDC_BUTTON_PLAYBACK)->SetWindowText(_T("暂停回放"));
    }
}
void CVideoCollect_ClientDlg::StartPlayBackVoice()
{
    //m_Player.put_URL(_T("P:\\2016_04_14_007_20160414163233_007_Out_64699_2007.wav"));
    //m_Player.put_URL(_T("http:\\\\10.16.38.19:80\\video\\20160415_173901378_20___64697_1.wav"));
    //m_Player.put_URL(_T("http:\\\\10.16.38.19:80\\video\\2016_04_14_007_20160414163233_007_Out_64699_2007.wav"));
    //m_Player.put_URL(_T("http:\\\\10.16.36.12:80\\download\\20160419_192144601_22_test1_64699_65998_1.wav"));

    //ftp参考：http://cache.baiducontent.com/c?m=9d78d513d99907e91afa950e1a66d271685597124bc0a16668d5e31cd5264c40377192cc30531613a3b46b1376bb0e1cb4ff6c34714137b6e8d5950f98fe852858d37c672c4bc30005d36efe961938897acb0cbaf204a4fda773d5f58f8fc85425ca50017a8be78b2d414e8c30ae&p=9839c64ad48415f109bd9b7d0d1285&newp=ce62d31a80904ead42bd9b7d0d1293231610db2151d4d01165&user=baidu&fm=sc&query=http%B7%C3%CE%CAftp%CE%C4%BC%FE&qid=dc712ef4000026e4&p1=3
    m_Player.put_URL(_T("ftp://admin:admin@10.16.36.5/hd1/Record/Record20160422/20160422_145758659_22___65998_1.wav"));
    MessageBox(_T("ftp://10.16.36.5/hd1/Record/Record20160422/20160422_145758659_22___65998_1.wav"));

}
void CVideoCollect_ClientDlg::OnBnClickedButtonPlayback()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLogin)
    {
        MessageBox(_T("请先登录再回放!"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if (-1 == m_iCurChanIndex)
    {
        MessageBox(_T("请选择一个需要的通道回放"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if (m_bPlaying)
    {
        MessageBox(_T("请先停止预览再回放"),_T("提示"),MB_ICONWARNING);
        return;
    }
    //确定是否播放
    if (!m_bPlayingBack)
    {
        //创建回放线程
        if(NULL == m_hThreadVideoPlayerBack)
        {
            m_hThreadVideoPlayerBack = (HANDLE)_beginthreadex(NULL,0,VideoPlayBack,(LPVOID)this,0,NULL);
            if(NULL == m_hThreadVideoPlayerBack)
            {
                CString m_csErr;
                m_csErr.Format(_T("创建录像回放线程失败,错误码:%d"),NET_DVR_GetLastError());
                MessageBox(m_csErr);
            }
        }
        //创建语音回放线程
        //if(NULL == m_hThreadVoicePlayerBack)
        //{
        //    m_hThreadVoicePlayerBack = (HANDLE)_beginthreadex(NULL,0,VoicePlayBack,(LPVOID)this,0,NULL);
        //    if(NULL == m_hThreadVoicePlayerBack)
        //    {
        //        CString m_csErr;
        //        m_csErr.Format(_T("创建语音回放线程失败,错误码:%d"),NET_DVR_GetLastError());
        //        MessageBox(m_csErr);
        //    }
        //}
        StartPlayBackVoice();
    }
    else
    {
        StopPlayBack(m_lPlayBackHandle);
    }
}
void CVideoCollect_ClientDlg::SetPlayState(int iState)
{
    CButton *pButton;
    switch(iState)
    {
    case PLAY_STOP:
        pButton = (CButton *)GetDlgItem(IDC_BUTTON_PLAYBACK_OUT);
        pButton->SetIcon(m_hStopDisableIcon);
        pButton->EnableWindow(FALSE);
        break;
    case PLAY_NORMAL:
        pButton = (CButton *)GetDlgItem(IDC_BUTTON_PLAYBACK_OUT);
        pButton->SetIcon(m_hStopEnableIcon);
        pButton->EnableWindow(TRUE);
    default:
        break;
    }
}
void CVideoCollect_ClientDlg::OnBnClickedButtonPlaybackOut()
{
    m_control.stop();

    if(NULL != m_hThreadVideoPlayerBack)
    {
        ::CloseHandle(m_hThreadVideoPlayerBack);
        m_hThreadVideoPlayerBack = NULL;
    }
    if(NULL != m_hThreadVoicePlayerBack)
    {
        ::CloseHandle(m_hThreadVoicePlayerBack);
        m_hThreadVoicePlayerBack = NULL;
    }
    // TODO: 在此添加控件通知处理程序代码
    if(m_lPlayBackHandle >= 0)
    {
        //停止播放
        bool ret = NET_DVR_StopPlayBack(m_lPlayBackHandle);
        if(!ret)
        {
            CString m_csErr;
            m_csErr.Format(_T("退出回放失败(错误代码:%d)"),NET_DVR_GetLastError());
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        m_lPlayBackHandle = -1;
        m_bPlayingBack = false;
        GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
        GetDlgItem(IDC_BUTTON_PLAYBACK)->SetWindowText(_T("回放"));

        SetPlayState(PLAY_STOP);
    }
}
//////////////////////////////////////////////////////////////////////////配置参数
void CVideoCollect_ClientDlg::DelayBySleep(DWORD dwMilliseconds,
                             volatile bool &stopFlag,
                             DWORD dwInterval/* = 500*/)
{
    if (0 == dwInterval)
    {
        dwInterval = 500;
    }
    DWORD const nTimes = dwMilliseconds / dwInterval;
    for (DWORD i = 0; !stopFlag && i < nTimes; ++i)
    {
        Sleep(dwInterval);
    }
}
void CVideoCollect_ClientDlg::SaveParamLogin()
{
    //检查保存信息合法性-IP
    CString sDeviceIP = _T("");
    m_ctrlDevIp.GetWindowText(sDeviceIP);
    sDeviceIP.Trim();
    if (sDeviceIP.IsEmpty())
    {
        MessageBox(_T("设备IP不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ctrlDevIp.SetFocus();
        return;
    }
    //获取参数保存
    CString sDevicePort = _T("");
    CString sDeviceUserNmae = _T("");
    CString sDevicePassWord = _T("");
    m_editDevPort.GetWindowText(sDevicePort);
    sDevicePort.Trim();
    m_editDevUserName.GetWindowText(sDeviceUserNmae);
    sDeviceUserNmae.Trim();
    m_editDevPassWord.GetWindowText(sDevicePassWord);
    sDevicePassWord.Trim();

    CConfigParser myConfig(CONFIG_FILE);
    myConfig.SetSection(INI_SEC_AUTH);
    myConfig.SetStringValue(KEY_DEVICE_IP, sDeviceIP);
    myConfig.SetStringValue(KEY_DEVICE_PORT, sDevicePort);
    myConfig.SetStringValue(KEY_DEVICE_USERNAME, sDeviceUserNmae);
    myConfig.SetStringValue(KEY_DEVICE_PASSWORD, sDevicePassWord);
}
void CVideoCollect_ClientDlg::InitConfig()
{
    std::_tstring m_appDir;
    std::_tstring m_strCfgFile;
    //
    char szAppPath[MAX_PATH] = {0};
    if (HPR_OK != HPR_GetCurExePath(szAppPath, _countof(szAppPath)))
    {
        TPLOG_ERROR("get app path failed(%d)", HPR_GetLastError());
        return ;
    }
    char *pEnd = NULL;
    if ((pEnd = ::strrchr(szAppPath, '\\')) != NULL
        || (pEnd = ::strrchr(szAppPath, '/')) != NULL)
    {
        if (pEnd - szAppPath + 1 < (int)(_countof(szAppPath)))
        {
            *(pEnd + 1) = 0;
        }
        else
        {
            return;
        }
    }
    else
    {
        TPLOG_ERROR("failed to get splash for: %s", szAppPath);
        return;
    }
    TPLOG_INFO("current dir is : %s", szAppPath);
    if (FALSE == ::SetCurrentDirectoryA(szAppPath))
    {
        TPLOG_ERROR("set current dir failed(%d)", ::GetLastError());
        return;
    }
    m_appDir = fcA2T(szAppPath);

    //本地配置文件路径
    m_strCfgFile = m_appDir + my::INI_CFG_FILE_NAME;

    CConfigParser myini(m_strCfgFile.c_str());
    myini.SetSection(my::INI_SEC_DEVICE_PARAM);
    m_strDevIP = myini.GetStringValue(my::INI_SEC_DEVICE_IP, my::DEFAULT_SEC_DEVICE_IP);
    m_strDevPort = myini.GetStringValue(my::INI_SEC_DEVICE_PORT, my::DEFAULT_SEC_DEVICE_PORT);
    m_strDevUser = myini.GetStringValue(my::INI_SEC_DEVICE_USERNAME, my::DEFAULT_SEC_DEVICE_USERNAME);
    m_strDevPassword = myini.GetStringValue(my::INI_SEC_DEVICE_PASSWORD, my::DEFAULT_SEC_DEVICE_PASSWORD);
    //

    m_ctrlDevIp.SetWindowText(m_strDevIP);
    m_editDevPort.SetWindowText(m_strDevPort);
    m_editDevUserName.SetWindowText(m_strDevUser);
    m_editDevPassWord.SetWindowText(m_strDevPassword);

}

void CVideoCollect_ClientDlg::BuildDefaultFile()
{
    CConfigParser myConfig(CONFIG_FILE);
    assert(!myConfig.IsExist());
    if (myConfig.IsExist())
    {
        return;
    }
    myConfig.SetSection(INI_SEC_AUTH);
    myConfig.SetStringValue(KEY_DEVICE_IP, DEFAULT_DEVICE_IP);
    myConfig.SetStringValue(KEY_DEVICE_PORT, DEFAULT_DEVICE_PORT);
    myConfig.SetStringValue(KEY_DEVICE_USERNAME, DEFAULT_DEVICE_USERNAME);
    myConfig.SetStringValue(KEY_DEVICE_PASSWORD, DEFAULT_DEVICE_PASSWORD);
}

//下面是验证本地录音文件上传CVR
