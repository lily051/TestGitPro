
// FUiDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FUiDemo.h"
#include "FUiDemoDlg.h"

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


// CFUiDemoDlg 对话框




CFUiDemoDlg::CFUiDemoDlg(CWnd* pParent /*=NULL*/)
: CFDialog(CFUiDemoDlg::IDD, pParent)
, m_pCalDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    TCHAR szPath[256] = {0};
    GetModuleFileName(NULL, szPath, 256);
    CString strAppPath = szPath;
    szPath[strAppPath.ReverseFind(_T('\\'))] = _T('\0');
    strAppPath = szPath;

    CString s = strAppPath + _T("\\image\\1.rc");
    FUiSetRes(s, _T("Hik_Financial_8100"));
}

void CFUiDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CFDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_1, m_fStatic);
    DDX_Control(pDX, IDC_EDIT1, m_fEdit);
    DDX_Control(pDX, IDC_EDIT2, m_fMEdit);
    DDX_Control(pDX, IDC_COMBO1, m_fComboBox);
    DDX_Control(pDX, IDC_BTN, m_fBtn);
    DDX_Control(pDX, IDC_CHECK1, m_fCheckBox);
    DDX_Control(pDX, IDC_CHECK2, m_fCheckBox2);
    DDX_Control(pDX, IDC_SLIDER1, m_fSlider1);
    DDX_Control(pDX, IDC_SLIDER2, m_fSlider2);
    DDX_Control(pDX, IDC_PROGRESS1, m_fProgress);
    DDX_Control(pDX, IDC_CHECK3, m_fRadio);
    DDX_Control(pDX, IDC_CHECK4, m_fRadio2);
    DDX_Control(pDX, IDC_LIST1, m_fList);
    DDX_Control(pDX, IDC_BUTTON1, m_btnSS);
    DDX_Control(pDX, IDC_TREE1, m_fTreeEx);
    DDX_Control(pDX, IDC_BUTTON2, m_btnM);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_fDataTime);
    DDX_Control(pDX, IDC_SCROLLBAR2, m_scVert);
}

BEGIN_MESSAGE_MAP(CFUiDemoDlg, CFDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ADDSYSBTN, &CFUiDemoDlg::OnBnClickedAddsysbtn)
    ON_BN_CLICKED(IDC_DELSYSBTN, &CFUiDemoDlg::OnBnClickedDelsysbtn)
    ON_BN_CLICKED(IDC_BTN_WAIT, &CFUiDemoDlg::OnBnClickedBtnWait)
    ON_BN_CLICKED(IDC_BTN_MSGBOX, &CFUiDemoDlg::OnBnClickedBtnMsgbox)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_MENU, &CFUiDemoDlg::OnBnClickedBtnMenu)
    ON_BN_CLICKED(IDC_BTN_CALENDAR, &CFUiDemoDlg::OnBnClickedBtnCalendar)
    ON_BN_CLICKED(22222, &CFUiDemoDlg::OnBnClickedBtnCalendar)
	ON_BN_CLICKED(IDC_BUTTON1, &CFUiDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN, &CFUiDemoDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDC_BUTTON3, &CFUiDemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFUiDemoDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

// CFUiDemoDlg 消息处理程序

BOOL CFUiDemoDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

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

	InitDlg();

    InitCtrl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFUiDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFUiDemoDlg::OnPaint()
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
        CFDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFUiDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFUiDemoDlg::InitDlg(void)
{
    // 采用默认大小时，右侧可能会出现一条黑线，建议改变一下对话框的大小
    MoveWindow(0, 0, 1280, 800);

    SetObjectName(_T("dialog\\dialog2"));

    std::vector<int> vecMargin;
    vecMargin.push_back(3);
    vecMargin.push_back(35);
    vecMargin.push_back(3);
    vecMargin.push_back(28);
    SetMargin(vecMargin);
}

void CFUiDemoDlg::OnBnClickedAddsysbtn()
{
    AddCustomBtnToSysBtn(_T("custom"), _T(""), _T("<b><font color=red>User</font></b>:Admin...\nAge:45\nRole:Administrator"), 601);
    AddCustomBtnToSysBtn(_T("btn"), _T("Admin_w"), _T("User:Admin_w..."), 602, 100);
}

void CFUiDemoDlg::OnBnClickedDelsysbtn()
{
    DeleteCustomBtn(601);
}

BOOL CFUiDemoDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if (wParam == 809 || wParam == 601 || wParam == 602)
    {
        CString str;
        str.Format(_T("Clicked %d"), wParam);
        MessageBox(str);
    }

    // 响应列表中多按钮点击
    if (MSG_MULTIBTN_CLICK == wParam)
    {
        // 按钮信息的结构体
        STRU_MULTIBTN_INFO* multibtnInfo = (STRU_MULTIBTN_INFO*)lParam;

        // 行号和序号都从0开始计数
        CString strCheck;
        strCheck.Format(_T("第%d行, 第%d个按钮"), multibtnInfo->nItem + 1, multibtnInfo->nNumber + 1);
        MessageBox(strCheck, _T("列表多按钮点击"));

        // 需要释放内存, 并返回TRUE
        delete multibtnInfo;
        return TRUE;
    }

    return CFDialog::OnCommand(wParam, lParam);
}

void CFUiDemoDlg::InitCtrl()
{
    LOGFONT lf = {0};
    _tcscpy_s(lf.lfFaceName, 32, _T("微软雅黑"));
    lf.lfHeight = 20;
    
    // 静态文本
    m_fStatic.SetTextColor(FUi::Red);
    m_fStatic.SetTextFont(SONG_12_SB);
    m_fStatic.SetHoverCursor(LoadCursor(NULL, IDC_HAND));
    m_fStatic.SetBgcolor(0, FUi::RGB_204);
    m_fStatic.SetTextAlign(DT_VCENTER | DT_SINGLELINE);

    // 文本框
    m_fEdit.SetDefaultText(_T("单行文本框"));
	m_fEdit.SetWindowPos(NULL, 0, 0, 175, 34, SWP_NOMOVE);
	m_fEdit.SetPasswordMode(TRUE);

    m_fMEdit.SetDefaultText(_T("多行文本框"));

    // 下拉框
    m_fComboBox.SetObjectName(_T("combo"));
	m_fComboBox.SetWindowPos(NULL, 0, 0, 175, 34, SWP_NOMOVE);
	m_fComboBox.SetItemHeight(-1, 34);
    m_fComboBox.SetDefaultText(_T("下拉框"));
   // m_fComboBox.SetTextFont(FUi::Normal, SONG_12_N);
    //m_fComboBox.SetState(1);
    m_fComboBox.SetBoderColor(FUi::Focus, RGB(0,255,0));
    m_fComboBox.SetTextColor(FUi::Normal, FUi::Red);

    m_fComboBox.AddString(L"测试项1");
    m_fComboBox.AddString(L"测试项2");
    m_fComboBox.AddString(L"测试项3");
	//m_fComboBox.SetCurSel(2);		// SetCurSel无法触发Edit的EnChange消息（好像是这样的，没有深究）
	m_fComboBox.SetWindowText(_T("下拉框提示文字"));	// 无法触发Edit的EnChange消息（好像是这样的
	//m_fComboBox.SetReadOnly();
	TRACE("SetCurSel\n");

    /* 按钮 */
    // button
    m_fBtn.SetObjectName(_T("btn"));
    m_fBtn.SetIcon(_T("icon"));
    m_fBtn.SetTextAlign(DT_CENTER);
    m_fBtn.SetHoverCursor(LoadCursor(NULL, IDC_HAND));
    m_fBtn.SetTextColor(FUi::Normal, FUi::White);
	m_fBtn.SetTipText(_T("1111111"));
	m_fBtn.SetTipText(_T("22222"));

    m_btnSS.SetObjectName(_T("btnSS"));
    m_btnSS.SetIcon(_T("icon"));
	m_btnSS.SetTextColor(FUi::Normal, FUi::White);
	m_btnSS.SetTipText(_T("44444444444"));
    m_btnSS.AddCornerBtn(809, _T("cclose"));

	LOGFONT lf1 = {0};
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 20;
	lf1.lfWeight = FW_SEMIBOLD;
	RenderEngine->AddFont(_T("MFont"), lf1);

	lf1.lfHeight = 18;
	lf1.lfWeight = FW_NORMAL;
	RenderEngine->AddFont(_T("MFontSub"), lf1);

	m_btnM.SetObjectName(_T("m"));
	m_btnM.SetIcon(_T("icon_modular_set"));
	m_btnM.SetTextColor(FUi::Normal | FUi::Hovered, FUi::Black);
	m_btnM.SetTextColor(FUi::Hovered, FUi::White);
	m_btnM.SetTextFont(FUi::Normal | FUi::Hovered, _T("MFont"));
	m_btnM.SetSubName(_T("说明文字说明文字说明文字"));
	m_btnM.SetSubNameFont(FUi::Normal | FUi::Hovered, _T("MFontSub"));
	m_btnM.SetSubNameColor(FUi::Normal | FUi::Hovered, FUi::Gray);
	m_btnM.SetSubNameColor(FUi::Hovered, FUi::BurlyWood);
	m_btnM.SetTipText(_T("33333"));
	//m_btnM.LockHover(TRUE);

    // checkbox
    m_fCheckBox.SetObjectName(_T("cbtn"));
    m_fCheckBox.SetCheckBox();

    m_fCheckBox2.SetObjectName(_T("cbtn"));
    m_fCheckBox2.SetCheckBox();
    m_fCheckBox2.SetIcon(_T("icon"));
    m_fCheckBox2.SetFixedWidth(150);

    // radio
    m_fRadio.SetObjectName(_T("radio"));
    m_fRadio.SetCheckBox();

    m_fRadio2.SetObjectName(_T("radio"));
    m_fRadio2.SetCheckBox();
    m_fRadio2.SetIcon(_T("icon"));
    m_fRadio2.SetFixedWidth(150);


    // 滑块
    m_fSlider1.SetObjectName(_T("slider"));
    m_fSlider1.SetRange(0, 100);

    m_fSlider2.SetObjectName(_T("slider"));
    m_fSlider2.SetRange(0, 100);

    // 进度条
    m_fProgress.SetObjectName(_T("progress"));
    m_fProgress.SetRange(0, 100);
    m_fProgress.SetPos(0);
    m_fProgress.SetWindowPos(NULL, 0, 0, 175, 16, SWP_NOMOVE);
    SetTimer(1, 100, NULL);

    // DateTimeCtrl带日历
    m_fDataTime.SetStyle(FDTS_CAL);  // 样式选择: FDTS_CAL-日历样式, FDTS_TIME-时间样式
    m_fDataTime.SetCalStyle(CS_DATETIME);  // 日历样式选择: CS_DATE-日期样式, CS_DATETIME-日期时间样式

    // 列表

    // 使用Fui的滚动条
    SkinWndScroll(&m_fList);
    // 启用滚动条后父窗口改变，控件中如果使用到父窗口指针，需要提前设置
    m_fList.SetActrueParent(this);

    //m_fList.SetStyle(FLS_LINE);     // 样式选择,FLS_ZEBRA-斑马纹，FLS_LINE-线性
    m_fList.SetHeadHeight(26); // 设置表头高度
    //m_fList.SetHeadBgColor(FUi::Red); // 设置表头背景色
    m_fList.SetHeight(34);
    m_fList.SetCheckBox();     // checkbox开关
    m_fList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
    m_fList.InsertColumn(1, _T("筛选条件"), LVCFMT_LEFT, 100);
    m_fList.InsertColumn(2, _T("内容"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(3, _T("状态"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(4, _T("操作"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(5, _T("多按钮"), LVCFMT_LEFT, 80);

    //m_fList.SetHeadLock(-1);      // 锁定所有
    m_fList.SetHeadLock(0);         // 锁定列表头开关
    m_fList.SetHeadLock(1, false);
    m_fList.SetHeadSort(1);         // 排序功能开关
    m_fList.SetHeadSort(2, false);

     // 图标列表
    int nImage1 = m_fList.SetImageList(_T("list_ok_ico"));
    int nImage2 =  m_fList.SetImageList(_T("list_error_ico"));

    // 多按钮无文字，每项相同, 第一个参数为按钮数，后面为不定参数，为zip中的图片名
    // 只需要调用一次，响应点击消息查看WM_COMMAND中的MSG_MULTIBTN_CLICK
    m_fList.SetButton(5, 3, _T("btn_test"), _T("btn_test"), _T("btn_test"));
	m_fList.SetButtonToolTip(3, _T("btn_test1"), _T("btn_test2"), _T("btn_test3"));
	m_fList.SetToolTipEnable(TRUE);
    
	m_fList.InsertItem(0, _T("10"));
    //m_fList.SetItemText(0, 1, _T("海康01"));
    m_fList.SetItemText(0, 2, _T("海康编码设备列表01"));
    m_fList.SetItem(0, 3, _T("在线"), nImage1); // 设置Item，其中图标需要SetImageList
    m_fList.SetButton(0, 4 , _T("0000"), 22222); // ID用于响应按钮点击

     CRect rcSubItem;
    m_fList.GetSubItemRect(0, 1, LVIR_LABEL, rcSubItem);
   // m_fList.DrawTextColor(0, 1, _T("海康0000"), FUi::Red);

    m_fList.InsertItem(1, _T("02"));
    m_fList.SetItemText(1, 1, _T("海康02"));
    m_fList.SetItem(1, 2, _T("海康编码设备列表02"), nImage1);
    m_fList.SetItem(1, 3, _T("在线"), nImage1);
    m_fList.SetButton(1, 4 , _T("1111"), 1501);

    m_fList.InsertItem(2, _T("03"));
    m_fList.SetItemText(2, 1, _T("海康03"));
    m_fList.SetItemText(2, 2, _T("海康编码设备列表03"));
    m_fList.SetItem(2, 3, _T(""), nImage1);
    m_fList.SetButton(2, 4 , _T("2222"), 1502);

    m_fList.InsertItem(3, _T("04"));
    m_fList.SetItem(3, 1, _T("海康04"), nImage2);
    m_fList.SetItemText(3, 2, _T("海康编码设备列表04"));
    m_fList.SetItem(3, 3, _T("离线"), nImage2);
    m_fList.SetButton(3, 4 , _T("3333"), 1503);

    m_fList.InsertItem(4, _T("05"));
    m_fList.SetItemText(4, 1, _T("海康05"));
    m_fList.SetItemText(4, 2, _T("海康编码设备列表05"));
    m_fList.SetItem(4, 3, _T(""), nImage1);
    m_fList.SetButton(4, 4 , _T("4444"), 1504);

    m_fList.InsertItem(2, _T("***"));
    m_fList.SetItemText(2, 1, _T("***"));
    m_fList.SetItemText(2, 2, _T("***"));
    m_fList.SetItem(2, 3, _T(""), nImage1);
    m_fList.SetButton(2, 4 , _T("***"), 1504);


    for (int i = 0; i < 1; i++)
    {
        m_fList.InsertItem(5, _T("05"));
        m_fList.SetItemText(5, 1, _T("海康05"));
        m_fList.SetItemText(5, 2, _T("海康编码设备列表05"));
        m_fList.SetItem(5, 3, _T(""), nImage1);
	}

    SkinWndScroll(&m_fTreeEx);

    // 树
	m_fTreeEx.LoadImageRes(_T("list_ok_ico"), _T("list_error_ico"));
	m_ImageList.Create(IDB_BITMAP1, 16, 16, RGB(255,0,0));
	m_fTreeEx.SetImageList(&m_ImageList, 0);
	m_fTreeEx.SetItemHeight(16);
	HTREEITEM hRoot;
	HTREEITEM hParent;
	HTREEITEM hSon;

	hRoot = m_fTreeEx.InsertItem(_T("中国"), 1, 1);
	m_fTreeEx.SetItemData(hRoot, 0);
	
	hParent = m_fTreeEx.InsertItem(_T("唐"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 1);

	hSon = m_fTreeEx.InsertItem(_T("南唐"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 2);

	hSon = m_fTreeEx.InsertItem(_T("北唐"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 3);

	hParent = m_fTreeEx.InsertItem(_T("宋"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 4);

	hSon = m_fTreeEx.InsertItem(_T("南宋"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 5);

	hSon = m_fTreeEx.InsertItem(_T("北宋"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 6);


	hParent = m_fTreeEx.InsertItem(_T("元"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 7);

	hParent = m_fTreeEx.InsertItem(_T("明"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 8);
	
	hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("清清清清清清清清清清清清清清清清清清清清清清清清清清清清清清清清"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

	m_fTreeEx.Expand(hRoot,TVE_EXPAND);
	//默认选中根节点，可避免第一次点击出现MFC默认的选中效果
	hRoot = m_fTreeEx.GetRootItem();
	if(hRoot)
	{
		m_fTreeEx.SelectItem(hRoot);
	}
	m_fTreeEx.SetItemHeight(20);
	m_fTreeEx.SetDragAble(TRUE);
}

void CFUiDemoDlg::OnBnClickedBtnWait()
{
    CFBusyWait w(this);
    w.ShowBusyWait(_T("我的心在等待, 在呀么在等待...."));
    Sleep(3000);
}

void CFUiDemoDlg::OnBnClickedBtnMsgbox()
{
    CString s = _T("也就是说，在函数堆栈结构中，\r\n（针对当前DLL）只需要在最外层入\r\n口的地方使用一次即可。并且使用了 \r\n这个宏的接口，最好不要再嵌套调用，\
那样就相当于上\r\n面说的 “告诉系统，程序进入这个函\r\n数的时候，就从当前D\r\nLL去查找资源” 执行了多次");
    int ret = FMessageBox::Show(s, _T("内容很长内容很长内容很长内容很长内容很长"), MB_YESNOCANCEL | MB_ICONINFORMATION);

    FMessageBox::Show(_T("登录平台失败，连接服务器超时"), _T("登录失败"), MB_RETRYCANCEL/* | MB_ICONERROR*/);

    MessageBox(_T("登录平台失败，连接服务器超时"), _T("登录失败"), MB_OK | MB_ICONERROR);
}

int pos = 0, nCount = 0;
void CFUiDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        m_fProgress.SetPos(++pos);
        if (pos >= 105)
        {
            pos = 1;
        }
    }
	else if (nIDEvent == 2)
	{
		time_t t1 = time(NULL);
		int nImage1 = m_fList.SetImageList(_T("list_ok_ico"));
		CString s;
		s.Format(_T("%03d"), nCount++);
		m_fList.InsertItem(0, s);
		m_fList.SetItemText(0, 1, _T("海康") + s);
		m_fList.SetItemText(0, 2, _T("海康编码设备列表05"));
		m_fList.SetItem(0, 3, _T(""), 0);
		//m_fList.SetButton(0, 4 , s, 1504);

		time_t t2 = time(NULL);
		TRACE(s + _T(":%d\n"), t2-t1);
	}
    CFDialog::OnTimer(nIDEvent);
}

void CFUiDemoDlg::OnBnClickedBtnMenu()
{
    CFMenu fMenu;
    fMenu.SetObjectName(_T("menu"));
    fMenu.CreatePopupMenu();

    fMenu.SetTextColor(FUi::Black, FUi::RGB_204);

    fMenu.AddMenu(MF_STRING, 10, _T("系统设置"), _T("menu_set"));
    fMenu.AddMenu(MF_SEPARATOR,0);
    fMenu.AddMenu(MF_STRING| MF_GRAYED, 11, _T("修改密码"), _T("menu_pwd"));
    fMenu.AddMenu(MF_STRING, 12, _T("交班日志"), _T("menu_shift"));
    fMenu.AddMenu(MF_STRING, 13, _T("重新登录"), _T("menu_logout"));
    fMenu.AddMenu(MF_STRING, 14, _T("锁定系统"), _T("menu_lock"));
    fMenu.AddMenu(MF_STRING, 15, _T("关于我们"), _T("menu_item"));
    fMenu.AddMenu(MF_STRING, 16, _T("退出系统"), _T("menu_exit"));

    CFMenu subMenu;
    subMenu.SetObjectName(_T("menu"));
    subMenu.CreatePopupMenu();
    subMenu.AddMenu(MF_STRING, 21, _T("子菜单3-1"));
    subMenu.AddMenu(MF_SEPARATOR, 0);
    subMenu.AddMenu(MF_STRING | MF_GRAYED, 22, _T("子菜单3-2"));
    subMenu.AddMenu(MF_STRING | MF_DISABLED, 23, _T("子菜单3-3"));

    //fMenu.AddMenu(MF_POPUP, (UINT_PTR)subMenu.GetSafeHmenu(), _T("很长很长很长很长很长的文本"));

    CRect rcBtn;
    GetDlgItem(IDC_BTN_MENU)->GetWindowRect(&rcBtn);
    int nCmd = (int)fMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD, \
        rcBtn.left,rcBtn.bottom, this);

    if (nCmd > 0)
    {
        CString str;str.Format(_T("%d"), nCmd);
        MessageBox(str);
    }
}

void CFUiDemoDlg::OnBnClickedBtnCalendar()
{
    // 日历

    CRect rc;
    GetDlgItem(IDC_BTN_CALENDAR)->GetWindowRect(rc);

    if (NULL != m_pCalDlg)
    {
        delete m_pCalDlg;
        m_pCalDlg = NULL;
    }
    else
    {
        m_pCalDlg = new CCalendarDlg;
        m_pCalDlg->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1000);
        m_pCalDlg->SetStyle(CS_DATE);  // 日历的样式
        m_pCalDlg->SetWindowPos(NULL, rc.left, rc.bottom + 5, 0, 0, SWP_NOSIZE);
        m_pCalDlg->ShowWindow(SW_SHOW);
    }
}

CFUiDemoDlg::~CFUiDemoDlg()
{
}

static BOOL bLock = TRUE;
void CFUiDemoDlg::OnBnClickedButton1()
{

	//m_fComboBox.SetCurSel(2);

	m_fComboBox.SetWindowText(_T("ac"));
}

void CFUiDemoDlg::OnBnClickedBtn()
{
// 	m_btnSS.LockHover(bLock);
// 	bLock = !bLock;

    CString s;
    m_fEdit.GetPasswordText(s);
    MessageBox(s);

    m_fEdit.GetWindowText(s);
    MessageBox(s);
}

BOOL g_b30 = FALSE;
void CFUiDemoDlg::OnBnClickedButton3()
{
	if (g_b30)
	{
		KillTimer(2);
	}
	else
	{
		SetTimer(2, 33, NULL);
	}
	g_b30 = !g_b30;
}

void CFUiDemoDlg::OnBnClickedButton4()
{
	m_fList.SetRedraw(FALSE);
	for (int i = 100; i < 200; ++i)
	{
		time_t t1 = time(NULL);
		int nImage1 = m_fList.SetImageList(_T("list_ok_ico"));
		CString s;
		s.Format(_T("%03d"), i);
		m_fList.InsertItem(0, s);
		m_fList.SetItemText(0, 1, _T("海康") + s);
		m_fList.SetItemText(0, 2, _T("海康编码设备列表05"));
		m_fList.SetItem(0, 3, _T(""), 0);
		//m_fList.SetButton(0, 4 , _T("配置"), 1504);

		time_t t2 = time(NULL);
		TRACE(s + _T(":%d\n"), t2-t1);
	}
	m_fList.SetRedraw(TRUE);
}
