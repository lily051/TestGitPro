
// FUiDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FUiDemo.h"
#include "FUiDemoDlg.h"

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


// CFUiDemoDlg �Ի���




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

// CFUiDemoDlg ��Ϣ�������

BOOL CFUiDemoDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

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

	InitDlg();

    InitCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFUiDemoDlg::OnPaint()
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
HCURSOR CFUiDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFUiDemoDlg::InitDlg(void)
{
    // ����Ĭ�ϴ�Сʱ���Ҳ���ܻ����һ�����ߣ�����ı�һ�¶Ի���Ĵ�С
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

    // ��Ӧ�б��жఴť���
    if (MSG_MULTIBTN_CLICK == wParam)
    {
        // ��ť��Ϣ�Ľṹ��
        STRU_MULTIBTN_INFO* multibtnInfo = (STRU_MULTIBTN_INFO*)lParam;

        // �кź���Ŷ���0��ʼ����
        CString strCheck;
        strCheck.Format(_T("��%d��, ��%d����ť"), multibtnInfo->nItem + 1, multibtnInfo->nNumber + 1);
        MessageBox(strCheck, _T("�б�ఴť���"));

        // ��Ҫ�ͷ��ڴ�, ������TRUE
        delete multibtnInfo;
        return TRUE;
    }

    return CFDialog::OnCommand(wParam, lParam);
}

void CFUiDemoDlg::InitCtrl()
{
    LOGFONT lf = {0};
    _tcscpy_s(lf.lfFaceName, 32, _T("΢���ź�"));
    lf.lfHeight = 20;
    
    // ��̬�ı�
    m_fStatic.SetTextColor(FUi::Red);
    m_fStatic.SetTextFont(SONG_12_SB);
    m_fStatic.SetHoverCursor(LoadCursor(NULL, IDC_HAND));
    m_fStatic.SetBgcolor(0, FUi::RGB_204);
    m_fStatic.SetTextAlign(DT_VCENTER | DT_SINGLELINE);

    // �ı���
    m_fEdit.SetDefaultText(_T("�����ı���"));
	m_fEdit.SetWindowPos(NULL, 0, 0, 175, 34, SWP_NOMOVE);
	m_fEdit.SetPasswordMode(TRUE);

    m_fMEdit.SetDefaultText(_T("�����ı���"));

    // ������
    m_fComboBox.SetObjectName(_T("combo"));
	m_fComboBox.SetWindowPos(NULL, 0, 0, 175, 34, SWP_NOMOVE);
	m_fComboBox.SetItemHeight(-1, 34);
    m_fComboBox.SetDefaultText(_T("������"));
   // m_fComboBox.SetTextFont(FUi::Normal, SONG_12_N);
    //m_fComboBox.SetState(1);
    m_fComboBox.SetBoderColor(FUi::Focus, RGB(0,255,0));
    m_fComboBox.SetTextColor(FUi::Normal, FUi::Red);

    m_fComboBox.AddString(L"������1");
    m_fComboBox.AddString(L"������2");
    m_fComboBox.AddString(L"������3");
	//m_fComboBox.SetCurSel(2);		// SetCurSel�޷�����Edit��EnChange��Ϣ�������������ģ�û�����
	m_fComboBox.SetWindowText(_T("��������ʾ����"));	// �޷�����Edit��EnChange��Ϣ��������������
	//m_fComboBox.SetReadOnly();
	TRACE("SetCurSel\n");

    /* ��ť */
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
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
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
	m_btnM.SetSubName(_T("˵������˵������˵������"));
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


    // ����
    m_fSlider1.SetObjectName(_T("slider"));
    m_fSlider1.SetRange(0, 100);

    m_fSlider2.SetObjectName(_T("slider"));
    m_fSlider2.SetRange(0, 100);

    // ������
    m_fProgress.SetObjectName(_T("progress"));
    m_fProgress.SetRange(0, 100);
    m_fProgress.SetPos(0);
    m_fProgress.SetWindowPos(NULL, 0, 0, 175, 16, SWP_NOMOVE);
    SetTimer(1, 100, NULL);

    // DateTimeCtrl������
    m_fDataTime.SetStyle(FDTS_CAL);  // ��ʽѡ��: FDTS_CAL-������ʽ, FDTS_TIME-ʱ����ʽ
    m_fDataTime.SetCalStyle(CS_DATETIME);  // ������ʽѡ��: CS_DATE-������ʽ, CS_DATETIME-����ʱ����ʽ

    // �б�

    // ʹ��Fui�Ĺ�����
    SkinWndScroll(&m_fList);
    // ���ù������󸸴��ڸı䣬�ؼ������ʹ�õ�������ָ�룬��Ҫ��ǰ����
    m_fList.SetActrueParent(this);

    //m_fList.SetStyle(FLS_LINE);     // ��ʽѡ��,FLS_ZEBRA-�����ƣ�FLS_LINE-����
    m_fList.SetHeadHeight(26); // ���ñ�ͷ�߶�
    //m_fList.SetHeadBgColor(FUi::Red); // ���ñ�ͷ����ɫ
    m_fList.SetHeight(34);
    m_fList.SetCheckBox();     // checkbox����
    m_fList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);
    m_fList.InsertColumn(1, _T("ɸѡ����"), LVCFMT_LEFT, 100);
    m_fList.InsertColumn(2, _T("����"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(3, _T("״̬"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(4, _T("����"), LVCFMT_LEFT, 80);
    m_fList.InsertColumn(5, _T("�ఴť"), LVCFMT_LEFT, 80);

    //m_fList.SetHeadLock(-1);      // ��������
    m_fList.SetHeadLock(0);         // �����б�ͷ����
    m_fList.SetHeadLock(1, false);
    m_fList.SetHeadSort(1);         // �����ܿ���
    m_fList.SetHeadSort(2, false);

     // ͼ���б�
    int nImage1 = m_fList.SetImageList(_T("list_ok_ico"));
    int nImage2 =  m_fList.SetImageList(_T("list_error_ico"));

    // �ఴť�����֣�ÿ����ͬ, ��һ������Ϊ��ť��������Ϊ����������Ϊzip�е�ͼƬ��
    // ֻ��Ҫ����һ�Σ���Ӧ�����Ϣ�鿴WM_COMMAND�е�MSG_MULTIBTN_CLICK
    m_fList.SetButton(5, 3, _T("btn_test"), _T("btn_test"), _T("btn_test"));
	m_fList.SetButtonToolTip(3, _T("btn_test1"), _T("btn_test2"), _T("btn_test3"));
	m_fList.SetToolTipEnable(TRUE);
    
	m_fList.InsertItem(0, _T("10"));
    //m_fList.SetItemText(0, 1, _T("����01"));
    m_fList.SetItemText(0, 2, _T("���������豸�б�01"));
    m_fList.SetItem(0, 3, _T("����"), nImage1); // ����Item������ͼ����ҪSetImageList
    m_fList.SetButton(0, 4 , _T("0000"), 22222); // ID������Ӧ��ť���

     CRect rcSubItem;
    m_fList.GetSubItemRect(0, 1, LVIR_LABEL, rcSubItem);
   // m_fList.DrawTextColor(0, 1, _T("����0000"), FUi::Red);

    m_fList.InsertItem(1, _T("02"));
    m_fList.SetItemText(1, 1, _T("����02"));
    m_fList.SetItem(1, 2, _T("���������豸�б�02"), nImage1);
    m_fList.SetItem(1, 3, _T("����"), nImage1);
    m_fList.SetButton(1, 4 , _T("1111"), 1501);

    m_fList.InsertItem(2, _T("03"));
    m_fList.SetItemText(2, 1, _T("����03"));
    m_fList.SetItemText(2, 2, _T("���������豸�б�03"));
    m_fList.SetItem(2, 3, _T(""), nImage1);
    m_fList.SetButton(2, 4 , _T("2222"), 1502);

    m_fList.InsertItem(3, _T("04"));
    m_fList.SetItem(3, 1, _T("����04"), nImage2);
    m_fList.SetItemText(3, 2, _T("���������豸�б�04"));
    m_fList.SetItem(3, 3, _T("����"), nImage2);
    m_fList.SetButton(3, 4 , _T("3333"), 1503);

    m_fList.InsertItem(4, _T("05"));
    m_fList.SetItemText(4, 1, _T("����05"));
    m_fList.SetItemText(4, 2, _T("���������豸�б�05"));
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
        m_fList.SetItemText(5, 1, _T("����05"));
        m_fList.SetItemText(5, 2, _T("���������豸�б�05"));
        m_fList.SetItem(5, 3, _T(""), nImage1);
	}

    SkinWndScroll(&m_fTreeEx);

    // ��
	m_fTreeEx.LoadImageRes(_T("list_ok_ico"), _T("list_error_ico"));
	m_ImageList.Create(IDB_BITMAP1, 16, 16, RGB(255,0,0));
	m_fTreeEx.SetImageList(&m_ImageList, 0);
	m_fTreeEx.SetItemHeight(16);
	HTREEITEM hRoot;
	HTREEITEM hParent;
	HTREEITEM hSon;

	hRoot = m_fTreeEx.InsertItem(_T("�й�"), 1, 1);
	m_fTreeEx.SetItemData(hRoot, 0);
	
	hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 1);

	hSon = m_fTreeEx.InsertItem(_T("����"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 2);

	hSon = m_fTreeEx.InsertItem(_T("����"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 3);

	hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 4);

	hSon = m_fTreeEx.InsertItem(_T("����"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 5);

	hSon = m_fTreeEx.InsertItem(_T("����"), 1, 1, hParent);
	m_fTreeEx.SetItemData(hSon, 6);


	hParent = m_fTreeEx.InsertItem(_T("Ԫ"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 7);

	hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 8);
	
	hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
	m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("��"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

    hParent = m_fTreeEx.InsertItem(_T("����������������������������������������������������������������"), 1, 1, hRoot);
    m_fTreeEx.SetItemData(hParent, 9);

	m_fTreeEx.Expand(hRoot,TVE_EXPAND);
	//Ĭ��ѡ�и��ڵ㣬�ɱ����һ�ε������MFCĬ�ϵ�ѡ��Ч��
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
    w.ShowBusyWait(_T("�ҵ����ڵȴ�, ��ѽô�ڵȴ�...."));
    Sleep(3000);
}

void CFUiDemoDlg::OnBnClickedBtnMsgbox()
{
    CString s = _T("Ҳ����˵���ں�����ջ�ṹ�У�\r\n����Ե�ǰDLL��ֻ��Ҫ���������\r\n�ڵĵط�ʹ��һ�μ��ɡ�����ʹ���� \r\n�����Ľӿڣ���ò�Ҫ��Ƕ�׵��ã�\
�������൱����\r\n��˵�� ������ϵͳ��������������\r\n����ʱ�򣬾ʹӵ�ǰD\r\nLLȥ������Դ�� ִ���˶��");
    int ret = FMessageBox::Show(s, _T("���ݺܳ����ݺܳ����ݺܳ����ݺܳ����ݺܳ�"), MB_YESNOCANCEL | MB_ICONINFORMATION);

    FMessageBox::Show(_T("��¼ƽ̨ʧ�ܣ����ӷ�������ʱ"), _T("��¼ʧ��"), MB_RETRYCANCEL/* | MB_ICONERROR*/);

    MessageBox(_T("��¼ƽ̨ʧ�ܣ����ӷ�������ʱ"), _T("��¼ʧ��"), MB_OK | MB_ICONERROR);
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
		m_fList.SetItemText(0, 1, _T("����") + s);
		m_fList.SetItemText(0, 2, _T("���������豸�б�05"));
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

    fMenu.AddMenu(MF_STRING, 10, _T("ϵͳ����"), _T("menu_set"));
    fMenu.AddMenu(MF_SEPARATOR,0);
    fMenu.AddMenu(MF_STRING| MF_GRAYED, 11, _T("�޸�����"), _T("menu_pwd"));
    fMenu.AddMenu(MF_STRING, 12, _T("������־"), _T("menu_shift"));
    fMenu.AddMenu(MF_STRING, 13, _T("���µ�¼"), _T("menu_logout"));
    fMenu.AddMenu(MF_STRING, 14, _T("����ϵͳ"), _T("menu_lock"));
    fMenu.AddMenu(MF_STRING, 15, _T("��������"), _T("menu_item"));
    fMenu.AddMenu(MF_STRING, 16, _T("�˳�ϵͳ"), _T("menu_exit"));

    CFMenu subMenu;
    subMenu.SetObjectName(_T("menu"));
    subMenu.CreatePopupMenu();
    subMenu.AddMenu(MF_STRING, 21, _T("�Ӳ˵�3-1"));
    subMenu.AddMenu(MF_SEPARATOR, 0);
    subMenu.AddMenu(MF_STRING | MF_GRAYED, 22, _T("�Ӳ˵�3-2"));
    subMenu.AddMenu(MF_STRING | MF_DISABLED, 23, _T("�Ӳ˵�3-3"));

    //fMenu.AddMenu(MF_POPUP, (UINT_PTR)subMenu.GetSafeHmenu(), _T("�ܳ��ܳ��ܳ��ܳ��ܳ����ı�"));

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
    // ����

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
        m_pCalDlg->SetStyle(CS_DATE);  // ��������ʽ
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
		m_fList.SetItemText(0, 1, _T("����") + s);
		m_fList.SetItemText(0, 2, _T("���������豸�б�05"));
		m_fList.SetItem(0, 3, _T(""), 0);
		//m_fList.SetButton(0, 4 , _T("����"), 1504);

		time_t t2 = time(NULL);
		TRACE(s + _T(":%d\n"), t2-t1);
	}
	m_fList.SetRedraw(TRUE);
}
