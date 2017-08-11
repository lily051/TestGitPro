// BusinessManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMSClient.h"
#include "BusinessManageDlg.h"
#include "CMSDataOperate.h"
#include "EditFileNameDlg.h"
#include "WordTemplateDlg.h"
#include "ConfigDlg.h"

using namespace HTMLParse;

#define random(x) (rand()%x)

#define TIMER_ENABLE_BTN			1
#define TIMER_ENABLE_WORD		2

CBusinessManageDlg * g_pBusinessMgrDlg = NULL;


// CBusinessManageDlg �Ի���

IMPLEMENT_DYNAMIC(CBusinessManageDlg, CFDialog)

CBusinessManageDlg::CBusinessManageDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CBusinessManageDlg::IDD, pParent)
	,m_lpPreviewCtrlDlg(NULL)
	,m_lpRecNameInfoDlg(NULL)
	,m_lpPlaybackCtrlDlg(NULL)
	,m_pRegionTree(NULL)
	,m_pHistoryListDlg(NULL)
	,m_nRegionId(-1)
	,m_strBusinessId(_T(""))
	,m_nProductId(-1)
	,m_treeRegion(NULL)
	, m_bIsRecording(false)
	, m_bIsHistory(false)
	, m_nBusinessIndex(-1)
	,m_treeProductType(NULL)
	,m_hRoot(NULL) 
{
	g_pBusinessMgrDlg = this;
}

CBusinessManageDlg::~CBusinessManageDlg()
{
	if (NULL != m_lpPreviewCtrlDlg)
	{
		delete m_lpPreviewCtrlDlg;
		m_lpPreviewCtrlDlg = NULL;
	}

	if (NULL != m_lpRecNameInfoDlg)
	{
		delete m_lpRecNameInfoDlg;
		m_lpRecNameInfoDlg = NULL;
	}

	if (NULL != m_lpPlaybackCtrlDlg)
	{
		delete m_lpPlaybackCtrlDlg;
		m_lpPlaybackCtrlDlg = NULL;
	}

	if (NULL != m_pHistoryListDlg)
	{
		delete m_pHistoryListDlg;
		m_pHistoryListDlg = NULL;
	}
}

void CBusinessManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BUSINESSINFO, m_staticBusinessInfo);
	DDX_Control(pDX, IDC_STATIC_BUSINESSID, m_staticBusinessId);
	DDX_Control(pDX, IDC_EDIT_BUSINESSID, m_editBusinessId);
	DDX_Control(pDX, IDC_STATIC_BUSINESSLOCATION, m_staticBusinessLocation);
	DDX_Control(pDX, IDC_COMBO_BUSINESSLOCATION, m_comboBusinessLocation);
	DDX_Control(pDX, IDC_STATIC_CUSTOMERNAME, m_staticCustomerName);
	DDX_Control(pDX, IDC_EDIT_CUSTOMERNAME, m_editCustomerName);
	DDX_Control(pDX, IDC_STATIC_CUSTOMERPHONE, m_staticCustomerPhone);
	DDX_Control(pDX, IDC_EDIT_CUSTOMERPHONE, m_editCustomerPhone);
	DDX_Control(pDX, IDC_STATIC_IDTYPE, m_staticIDType);
	DDX_Control(pDX, IDC_COMBO_IDTYPE, m_comboIDType);
	DDX_Control(pDX, IDC_STATIC_IDNUMBER, m_staticIDNumber);
	DDX_Control(pDX, IDC_EDIT_IDNUMBER, m_editIDNumber);
	DDX_Control(pDX, IDC_STATIC_PRODUCTINFO, m_staticProductInfo);
	DDX_Control(pDX, IDC_STATIC_PRODUCTTYPE, m_staticProductType);
	DDX_Control(pDX, IDC_COMBO_PRODUCTTYPE, m_comboProductType);
	DDX_Control(pDX, IDC_STATIC_PRODUCTTYPE2, m_staticProductName);
	DDX_Control(pDX, IDC_COMBO_PRODUCTNAME, m_comboProductName);
	DDX_Control(pDX, IDC_STATIC_PRODUCTCODE, m_staticProductCode);
	DDX_Control(pDX, IDC_EDIT_PRODUCTCODE, m_editProductCode);
	DDX_Control(pDX, IDC_STATIC_ISSUER, m_staticIssuer);
	DDX_Control(pDX, IDC_EDIT_ISSUER, m_editIssuer);
	DDX_Control(pDX, IDC_STATIC_PURCHASEAMOUNT, m_staticPurchaseAmount);
	DDX_Control(pDX, IDC_EDIT_PURCHASEAMOUNT, m_editPurchaseAmount);
	DDX_Control(pDX, IDC_STATIC_SALESMANINFO, m_staticSalesmanInfo);
	DDX_Control(pDX, IDC_STATIC_SALESMANNAME1, m_staticSalesmanName1);
	DDX_Control(pDX, IDC_STATIC_SALESMANNAME2, m_staticSalesmanName2);
	DDX_Control(pDX, IDC_STATIC_SALESMANID1, m_staticSalesmanId1);
	DDX_Control(pDX, IDC_STATIC_SALESMANID2, m_staticSalesmanId2);
	DDX_Control(pDX, IDC_LIST_RECORDS, m_listRecords);
	DDX_Control(pDX, IDC_STATIC_RECLIST, m_staticRecList);
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnUpLoad);
	DDX_Control(pDX, IDC_BTN_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_SUBMIT, m_btnSubmit);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_OSD, m_btnShowOSD);
	DDX_Control(pDX, IDC_BTN_BUSINESSINFO, m_btnBusinessInfo);
	DDX_Control(pDX, IDC_BTN_PRODUCTINFO, m_btnProductInfo);
	DDX_Control(pDX, IDC_BTN_SALESMANINFO, m_btnSalesmanInfo);
	DDX_Control(pDX, IDC_STATIC_BLANK1, m_staticBlank1);
	DDX_Control(pDX, IDC_STATIC_BLANK2, m_staticBlank2);
	DDX_Control(pDX, IDC_STATIC_BLANK3, m_staticBlank3);
	DDX_Control(pDX, IDC_BTN_HISTORYLIST, m_btnHistoryList);
	DDX_Control(pDX, IDC_BTN_WORDS, m_btnWords);
	DDX_Control(pDX, IDC_LISTBOX_PRODUCTCODE, m_listBoxProductCode);
	DDX_Control(pDX, IDC_STATIC_BLANK4, m_staticBlank4);
	DDX_Control(pDX, IDC_COMBO_BUSINESSID, m_comboBusinessId);
}


BEGIN_MESSAGE_MAP(CBusinessManageDlg, CFDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()	
	ON_MESSAGE(WM_SELECT_PRODUCT, &CBusinessManageDlg::OnSelectProduct)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CBusinessManageDlg::OnBnClickedBtnUpload)
	ON_CBN_DROPDOWN(IDC_COMBO_PRODUCTNAME, &CBusinessManageDlg::OnCbnDropdownComboProductname)
	ON_BN_CLICKED(IDC_BTN_HISTORYLIST, &CBusinessManageDlg::OnBnClickedBtnHistorylist)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCTNAME, &CBusinessManageDlg::OnCbnSelchangeComboProductname)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CBusinessManageDlg::OnBnClickedBtnSubmit)
	ON_BN_CLICKED(IDC_BTN_RESET, &CBusinessManageDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_WORDS, &CBusinessManageDlg::OnBnClickedBtnWords)
	ON_BN_CLICKED(IDC_BTN_PRINT, &CBusinessManageDlg::OnBnClickedBtnPrint)
	ON_EN_CHANGE(IDC_EDIT_PRODUCTCODE, &CBusinessManageDlg::OnEnChangeEditProductcode)
	ON_LBN_SELCHANGE(IDC_LISTBOX_PRODUCTCODE, &CBusinessManageDlg::OnLbnSelchangeListboxProductcode)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_CBN_DROPDOWN(IDC_COMBO_BUSINESSID, &CBusinessManageDlg::OnCbnDropdownComboBusinessid)
END_MESSAGE_MAP()


// CBusinessManageDlg ��Ϣ�������

BOOL CBusinessManageDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	InitCtrl();

	MoveCtrl();

	InitData();

	StartPreview();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBusinessManageDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	SetMargin(vecMargin);

	//Ԥ������
	if (NULL == m_lpPreviewCtrlDlg)
	{
		try
		{
			m_lpPreviewCtrlDlg = new CPreviewCtrlDlg(this);
		}
		catch (...)
		{
			return;
		}
		m_lpPreviewCtrlDlg->Create(IDD_DLG_PREVIEW_CRTL, this);
		m_lpPreviewCtrlDlg->ShowWindow(SW_SHOW);
	}

	//¼�����ƴ���
	if (NULL == m_lpRecNameInfoDlg)
	{
		try
		{
			m_lpRecNameInfoDlg = new CRecNameInfoDlg(this);
		}
		catch(...)
		{
			return;
		}
		m_lpRecNameInfoDlg->Create(IDD_DLG_RECNAMEINFO, this);
		m_lpRecNameInfoDlg->ShowWindow(SW_SHOW);
	}

	//�طŴ���
	if (NULL == m_lpPlaybackCtrlDlg)
	{
		try
		{
			m_lpPlaybackCtrlDlg = new CPlaybackCtrlDlg(this);
		}
		catch (...)
		{
			return;	
		}
		m_lpPlaybackCtrlDlg->Create(IDD_DLG_PLAYBACK_CTRL, GetDesktopWindow());
		m_lpPlaybackCtrlDlg->ShowWindow(SW_HIDE);
	}

	//ҵ����Ϣ
	m_comboBusinessId.SetObjectName(_T("combo3"));
	m_comboBusinessId.SetDroppedWidth(1);
	m_comboBusinessId.SetPopupDlg(TRUE);
	m_comboBusinessId.SetReadOnly(TRUE);
	m_comboBusinessId.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Pressed, YAHEI_18_N);
	m_btnBusinessInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticBusinessInfo.SetTextFont(YAHEI_18_B);
	m_staticBusinessId.SetTextFont(YAHEI_18_N);
	m_editBusinessId.SetTextFont(1, YAHEI_18_N);
	m_editBusinessId.EnableWindow(FALSE);
	m_editBusinessId.ShowWindow(SW_HIDE);
	m_btnHistoryList.SetObjectName(_T("btn"));
	m_btnHistoryList.SetIcon(_T("list"), 4);
	m_btnHistoryList.SetTipText(_T("ѡ����ʷҵ��"));
	m_btnHistoryList.SetFixedWidth(50);
	m_btnHistoryList.ShowWindow(SW_HIDE);
	m_comboBusinessLocation.SetItemHeight(0, 0);
	m_staticBusinessLocation.SetTextFont(YAHEI_18_N);
	m_comboBusinessLocation.SetTextFont(1, YAHEI_18_N);
	m_comboBusinessLocation.SetObjectName(_T("combo1"));
	m_comboBusinessLocation.ShowDropDown(FALSE);
	m_staticCustomerName.SetTextFont(YAHEI_18_N);
	m_editCustomerName.LimitText(32);
	m_editCustomerName.SetTextFont(1, YAHEI_18_N);
	m_editCustomerName.SetFocus();
	m_staticCustomerPhone.SetTextFont(YAHEI_18_N);
	m_editCustomerPhone.LimitText(16);
	m_editCustomerPhone.SetTextFont(1, YAHEI_18_N);
	m_staticIDType.SetTextFont(YAHEI_18_N);
	m_comboIDType.SetTextFont(1, YAHEI_18_N);
	m_staticIDNumber.SetTextFont(YAHEI_18_N);
	m_editIDNumber.LimitText(32);
	m_editIDNumber.SetTextFont(1, YAHEI_18_N);
	m_comboIDType.SetObjectName(_T("combo1"));
	m_comboIDType.SetReadOnly(TRUE);

	//��Ʒ��Ϣ
	m_staticProductType.SetTextFont(YAHEI_18_N);
	m_comboProductType.SetTextFont(1, YAHEI_18_N);
	m_comboProductType.SetBoderColor(FUi::Error, FUi::Red);
	m_staticProductName.SetTextFont(YAHEI_18_N);
	m_comboProductName.SetTextFont(1, YAHEI_18_N);
	m_staticProductCode.SetTextFont(YAHEI_18_N);
	m_editProductCode.LimitText(32);
	m_editProductCode.SetTextFont(1, YAHEI_18_N);
	m_staticIssuer.SetTextFont(YAHEI_18_N);
	m_editIssuer.SetTextFont(1, YAHEI_18_N);
	m_editIssuer.SetReadOnly(TRUE);  //��֧�ֱ༭�޸�
	m_editIssuer.LimitText(32);
	m_staticPurchaseAmount.SetTextFont(YAHEI_18_N);
	m_editPurchaseAmount.LimitText(10);
	m_editPurchaseAmount.SetTextFont(1, YAHEI_18_N);
	m_editPurchaseAmount.SetDefaultText(_T("��λ��Ԫ"));
	m_btnWords.SetTipText(_T("����"));
	m_btnWords.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Pressed, YAHEI_18_B);
	m_btnWords.SetHoverCursor(LoadCursor(NULL, IDC_HAND));
	m_btnWords.SetTextColor(FUi::Normal, RGB(0x3F, 0x85, 0xE4));
	m_btnWords.SetTextColor(FUi::Hovered | FUi::Pressed, RGB(0x2D, 0x5F, 0xA4));
	m_btnWords.SetTextColor(FUi::Disabled, RGB(0xCC, 0xCC, 0xCC));
	m_btnWords.SetFixedWidth(40);
	m_btnWords.SetTextAlign(DT_LEFT);
	m_comboProductType.SetObjectName(_T("combo1"));
	m_comboProductName.SetObjectName(_T("combo1"));
	m_comboProductName.SetReadOnly(TRUE);
	m_listBoxProductCode.ShowWindow(SW_HIDE);
	//m_btnWords.SetObjectName(_T("btn"));

	m_btnProductInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticProductInfo.SetTextFont(YAHEI_18_B);

	//ҵ��Ա��Ϣ
	m_btnSalesmanInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticSalesmanInfo.SetTextFont(YAHEI_18_B);
	m_staticSalesmanName1.SetTextFont(YAHEI_18_N);
	m_staticSalesmanName2.SetTextFont(YAHEI_18_N);
	m_staticSalesmanName2.SetHoverCursor(LoadCursor(NULL, IDC_ARROW));	 //Ϊtips��ʾ��׼���������޷���ʾtips
	m_staticSalesmanId1.SetTextFont(YAHEI_18_N);
	m_staticSalesmanId2.SetTextFont(YAHEI_18_N);
	m_staticSalesmanId2.SetHoverCursor(LoadCursor(NULL, IDC_ARROW)); //Ϊtips��ʾ��׼���������޷���ʾtips
	
	//¼���б�
	m_staticRecList.SetTextFont(YAHEI_18_B);
	m_btnUpLoad.SetHoverCursor(LoadCursor(NULL, IDC_HAND)); 
	m_btnUpLoad.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Pressed, YAHEI_18_N);
	m_btnUpLoad.SetTextColor(FUi::Normal, RGB(0x3F, 0x85, 0xE4));
	m_btnUpLoad.SetTextColor(FUi::Hovered | FUi::Pressed, RGB(0x2D, 0x5F, 0xA4));
	m_btnUpLoad.SetTextAlign(DT_RIGHT);

	m_listRecords.SetToolTipEnable(true);
	m_listRecords.SetHeadHeight(0);  //���ñ�ͷ�߶�(0-����ʾ)
	m_listRecords.SetHeight(25);
	//SkinWndScroll(&m_listRecords);
	m_listRecords.InsertColumn(0, _T("���"), LVCFMT_LEFT, 30);
	m_listRecords.InsertColumn(1, _T("�ļ���"), LVCFMT_LEFT, 165);
	m_listRecords.InsertColumn(2, _T("ʱ��"), LVCFMT_LEFT, 60);
	m_listRecords.InsertColumn(3, _T("��С"), LVCFMT_LEFT, 50);
	m_listRecords.InsertColumn(4, _T("����"), LVCFMT_LEFT, 80);

	//ListCtrl item�����ͼ��
	m_listRecords.SetButton(4, 3, _T("edit-(video)"), _T("view"), _T("delete"));
	m_listRecords.SetButtonToolTip(3, _T("�޸�����"), _T("�鿴"), _T("ɾ��"));

	m_btnPrint.SetObjectName(_T("btn"));
	m_btnPrint.SetIcon(_T("print"), 4);
	m_btnPrint.SetTextAlign(DT_CENTER);
	m_btnPrint.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, FUi::White);
	m_btnPrint.SetTipText(_T("��ӡ"));
	m_btnPrint.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_B);

	m_btnSubmit.SetObjectName(_T("btn"));
	m_btnSubmit.SetIcon(_T("submit"), 4);
	m_btnSubmit.SetTextAlign(DT_CENTER);
	m_btnSubmit.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, FUi::White);
	m_btnSubmit.SetTipText(_T("�ύ"));
	m_btnSubmit.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_B);

	m_btnReset.SetObjectName(_T("reset"));
	m_btnReset.SetIcon(_T("set"), 4);
	m_btnReset.SetTextAlign(DT_CENTER);
	m_btnReset.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::Blue);
	m_btnReset.SetTextColor(FUi::ctrl_state::Disabled, RGB(0xCC, 0xCC, 0xCC));
	m_btnReset.SetTipText(_T("����"));
	m_btnReset.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_B);

	//����
	GetDlgItem(IDC_STATIC_BLANK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_BLANK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_BLANK3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_BLANK4)->ShowWindow(SW_HIDE);

	//������ʱ��
	SetTimer(TIMER_ENABLE_BTN, 500, NULL);
	SetTimer(TIMER_ENABLE_WORD, 1000, NULL);
}

void CBusinessManageDlg::MoveCtrl(void)
{

	CRect rcClient;
	GetClientRect(rcClient);

	/********************ҵ����ϢBegin********************/
	//ҵ����Ϣ
	CRect rcBtnBusinsessInfo(rcClient);
	rcBtnBusinsessInfo.left = rcClient.left + 10;
	rcBtnBusinsessInfo.top = rcClient.top + 33;
	rcBtnBusinsessInfo.right = rcBtnBusinsessInfo.left + 12;
	rcBtnBusinsessInfo.bottom = rcBtnBusinsessInfo.top + 10;
 	m_btnBusinessInfo.MoveWindow(rcBtnBusinsessInfo);

	CRect rcStaticBusinessInfo(rcClient);
	rcStaticBusinessInfo.left = rcBtnBusinsessInfo.right + 6;
	rcStaticBusinessInfo.top = rcClient.top + 30;
	rcStaticBusinessInfo.right = rcStaticBusinessInfo.left + 80;
	rcStaticBusinessInfo.bottom = rcStaticBusinessInfo.top + 26;
	m_staticBusinessInfo.MoveWindow(rcStaticBusinessInfo);

	//ҵ�񵥺�(40+static+edit+btn=400)
	CRect rcStaticBusinessId(rcClient);
	rcStaticBusinessId.left = rcClient.left + 40;
	rcStaticBusinessId.top = rcStaticBusinessInfo.bottom + 6;
	rcStaticBusinessId.right = rcStaticBusinessId.left + 50 + 10;
	rcStaticBusinessId.bottom = rcStaticBusinessId.top + 20;
	m_staticBusinessId.MoveWindow(rcStaticBusinessId);

	CRect rcEditBusinessId(rcStaticBusinessId);
	rcEditBusinessId.left = rcStaticBusinessId.right;
	rcEditBusinessId.top =  rcStaticBusinessInfo.bottom;
	rcEditBusinessId.right = rcEditBusinessId.left + 360;
	rcEditBusinessId.bottom = rcEditBusinessId.top + 28;
	//m_editBusinessId.MoveWindow(rcEditBusinessId);
	m_comboBusinessId.MoveWindow(rcEditBusinessId);
	m_comboBusinessId.SetItemHeight(-1, 22);  //��Ҫ��MoveWindow,��SetItemHeight

	CRect rcBtnHistoryList(rcEditBusinessId);
	rcBtnHistoryList.left = rcBtnHistoryList.right;
	rcBtnHistoryList.right = rcBtnHistoryList.left + 45;
	m_btnHistoryList.MoveWindow(rcBtnHistoryList);

	//��������(40+static+combobox=400)
	CRect rcStaticBusinessLocation(rcStaticBusinessId);
	rcStaticBusinessLocation.top = rcStaticBusinessId.bottom + 14 + 8;
	rcStaticBusinessLocation.bottom = rcStaticBusinessLocation.top + 26;
	m_staticBusinessLocation.MoveWindow(rcStaticBusinessLocation);

	CRect rcComboBusinessLocation(rcEditBusinessId);
	rcComboBusinessLocation.top = rcEditBusinessId.bottom + 14;
	rcComboBusinessLocation.right = rcEditBusinessId.right;
	rcComboBusinessLocation.bottom = rcComboBusinessLocation.top + 26;
	m_comboBusinessLocation.MoveWindow(rcComboBusinessLocation);
	m_comboBusinessLocation.SetItemHeight(-1, 22);  //��Ҫ��MoveWindow,��SetItemHeight

	//�ͻ�����
	CRect rcStaticCustomerName(rcStaticBusinessLocation);
	rcStaticCustomerName.top = rcComboBusinessLocation.bottom + 14 + 8;
	rcStaticCustomerName.bottom = rcStaticCustomerName.top + 28;
	m_staticCustomerName.MoveWindow(rcStaticCustomerName);

	CRect rcEditCustomerName(rcComboBusinessLocation);
	rcEditCustomerName.top = rcComboBusinessLocation.bottom + 14 + 2;
	rcEditCustomerName.right = rcEditCustomerName.left + 135;
	rcEditCustomerName.bottom = rcEditCustomerName.top + 28;
	m_editCustomerName.MoveWindow(rcEditCustomerName);

	//��ϵ��ʽ
	CRect rcStaticPhoneNum(rcStaticCustomerName);
	rcStaticPhoneNum.left = rcEditCustomerName.right + 30;
	rcStaticPhoneNum.right = rcStaticPhoneNum.left + 50 + 10;
	m_staticCustomerPhone.MoveWindow(rcStaticPhoneNum);

	CRect rcEditPhoneNum(rcStaticPhoneNum);
	rcEditPhoneNum.left = rcEditPhoneNum.right;
	rcEditPhoneNum.top = rcEditCustomerName.top;
	rcEditPhoneNum.right = rcEditPhoneNum.left + 135;
	rcEditPhoneNum.bottom = rcEditCustomerName.bottom;
	m_editCustomerPhone.MoveWindow(rcEditPhoneNum);

	//֤������
	CRect rcStaticIDType(rcStaticCustomerName);
	rcStaticIDType.top = rcStaticIDType.bottom + 14 + 2;
	rcStaticIDType.bottom = rcStaticIDType.top + 26;
	m_staticIDType.MoveWindow(rcStaticIDType);

	CRect rcComboIDType(rcEditCustomerName);
	rcComboIDType.top = rcEditPhoneNum.bottom + 14;
	rcComboIDType.bottom = rcComboIDType.top + 26;
	m_comboIDType.MoveWindow(rcComboIDType);
	m_comboIDType.SetItemHeight(-1, 22);  //��Ҫ��MoveWindow,��SetItemHeight

	//֤������
	CRect rcStaticIDNum(rcStaticPhoneNum);
	rcStaticIDNum.top = rcStaticIDNum.bottom + 14;
	rcStaticIDNum.bottom = rcStaticIDNum.top + 26;
	m_staticIDNumber.MoveWindow(rcStaticIDNum);

	CRect rcEditIDNum(rcStaticIDNum);
	rcEditIDNum.left = rcStaticIDNum.right;
	rcEditIDNum.top = rcComboIDType.top;
	rcEditIDNum.right = rcEditIDNum.left + 135;
	rcEditIDNum.bottom = rcComboIDType.bottom + 2;
	m_editIDNumber.MoveWindow(rcEditIDNum);
	/********************ҵ����ϢEnd********************/

	/********************��Ʒ��ϢBegin********************/
	//��Ʒ��Ϣ
	CRect rcBtnProductInfo(rcClient);
	rcBtnProductInfo.left = rcClient.left + 10;
	rcBtnProductInfo.top = rcClient.top + 238;
	rcBtnProductInfo.right = rcBtnProductInfo.left + 12;
	rcBtnProductInfo.bottom = rcBtnProductInfo.top + 10;
	m_btnProductInfo.MoveWindow(rcBtnProductInfo);

	CRect rcStaticProductInfo(rcClient);
	rcStaticProductInfo.left = rcBtnProductInfo.right + 6;
	rcStaticProductInfo.top = rcClient.top + 235;
	rcStaticProductInfo.right = rcStaticProductInfo.left + 50;
	rcStaticProductInfo.bottom = rcStaticProductInfo.top + 26;
	m_staticProductInfo.MoveWindow(rcStaticProductInfo);

	//����
	CRect rcBtnWords(rcStaticProductInfo);
	rcBtnWords.left = rcBtnWords.right;
	rcBtnWords.top = rcBtnWords.top - 8;
	rcBtnWords.right = rcBtnWords.left + 50;
	m_btnWords.MoveWindow(rcBtnWords);

	//��Ʒ����
	CRect rcStaticProductType(rcClient);
	rcStaticProductType.left = rcClient.left + 40;
	rcStaticProductType.top = rcClient.top + 276;
	rcStaticProductType.right = rcStaticProductType.left + 60;
	rcStaticProductType.bottom = rcStaticProductType.top + 20;
	m_staticProductType.MoveWindow(rcStaticProductType);

	CRect rcComboProductType(rcClient);
	rcComboProductType.left = rcStaticProductType.right;
	rcComboProductType.top = rcClient.top + 270;
	rcComboProductType.right = rcComboProductType.left + 135;
	rcComboProductType.bottom = rcComboProductType.top + 26;
	m_comboProductType.MoveWindow(rcComboProductType);
	m_comboProductType.SetItemHeight(-1, 22);  //��Ҫ��MoveWindow,��SetItemHeight

	//��Ʒ����
	CRect rcStaticProductName(rcStaticProductType);
	rcStaticProductName.left = rcComboProductType.right + 30;
	rcStaticProductName.right = rcStaticProductName.left + 60;
	m_staticProductName.MoveWindow(rcStaticProductName);

	CRect rcComboProductName(rcStaticProductName);
	rcComboProductName.left = rcComboProductName.right;
	rcComboProductName.top = rcComboProductName.top - 6;
	rcComboProductName.right = rcComboProductName.left + 135;
	m_comboProductName.MoveWindow(rcComboProductName);
	m_comboProductName.SetItemHeight(-1, 22);  //��Ҫ��MoveWindow,��SetItemHeight

// 	//��Ʒ����
// 	CRect rcStaticProductCode(rcStaticProductType);
// 	rcStaticProductCode.top = rcStaticProductCode.bottom + 22;
// 	rcStaticProductCode.bottom = rcStaticProductCode.top + 26;
// 	m_staticProductCode.MoveWindow(rcStaticProductCode);
// 
// 	CRect rcEditProductCode(rcComboProductType);
// 	rcEditProductCode.top = rcEditProductCode.bottom + 14 + 2;
// 	rcEditProductCode.bottom = rcEditProductCode.top + 28;
// 	m_editProductCode.MoveWindow(rcEditProductCode);
// 
// 	//���л���
// 	CRect rcStaticIssuer(rcStaticProductName);
// 	rcStaticIssuer.top = rcStaticIssuer.bottom + 20;
// 	rcStaticIssuer.bottom = rcStaticIssuer.top + 26;
// 	m_staticIssuer.MoveWindow(rcStaticIssuer);
// 
// 	CRect rcEditIssuer(rcComboProductName);
// 	rcEditIssuer.top = rcEditIssuer.bottom + 14 + 2;
// 	rcEditIssuer.bottom = rcEditIssuer.top + 28;
// 	m_editIssuer.MoveWindow(rcEditIssuer);

	//���л���
	CRect rcStaticProductCode(rcStaticProductType);
	rcStaticProductCode.top = rcStaticProductCode.bottom + 22;
	rcStaticProductCode.bottom = rcStaticProductCode.top + 26;
	m_staticIssuer.MoveWindow(rcStaticProductCode);

	CRect rcEditProductCode(rcComboProductType);
	rcEditProductCode.top = rcEditProductCode.bottom + 14 + 2;
	rcEditProductCode.bottom = rcEditProductCode.top + 28;
	m_editIssuer.MoveWindow(rcEditProductCode);

	//��Ʒ����
	CRect rcStaticIssuer(rcStaticProductName);
	rcStaticIssuer.top = rcStaticIssuer.bottom + 20;
	rcStaticIssuer.bottom = rcStaticIssuer.top + 26;
	m_staticProductCode.MoveWindow(rcStaticIssuer);

	CRect rcEditIssuer(rcComboProductName);
	rcEditIssuer.top = rcEditIssuer.bottom + 14 + 2;
	rcEditIssuer.bottom = rcEditIssuer.top + 28;
	m_editProductCode.MoveWindow(rcEditIssuer);

	//��Ʒ�����б�
	CRect rcListBox(rcEditIssuer);
	rcListBox.top = rcEditIssuer.bottom;
	rcListBox.bottom = rcListBox.top + 50;
	m_listBoxProductCode.MoveWindow(rcListBox);

	//������
	CRect rcStaticPurchaseAmount(rcStaticProductCode);
	rcStaticPurchaseAmount.top = rcStaticPurchaseAmount.bottom + 16;
	rcStaticPurchaseAmount.bottom = rcStaticPurchaseAmount.top + 26;
	m_staticPurchaseAmount.MoveWindow(rcStaticPurchaseAmount);

	CRect rcEditPurchaseAmount(rcEditProductCode);
	rcEditPurchaseAmount.top = rcEditPurchaseAmount.bottom + 14;
	rcEditPurchaseAmount.bottom = rcEditPurchaseAmount.top + 28;
	m_editPurchaseAmount.MoveWindow(rcEditPurchaseAmount);
	/********************��Ʒ��ϢEnd********************/

	/********************ҵ��Ա��ϢBegin********************/
	//ҵ��Ա��Ϣ
	CRect rcBtnSalesmanInfo(rcClient);
	rcBtnSalesmanInfo.left = rcClient.left + 10;
	rcBtnSalesmanInfo.top = rcClient.top + 413;
	rcBtnSalesmanInfo.right = rcBtnSalesmanInfo.left + 12;
	rcBtnSalesmanInfo.bottom = rcBtnSalesmanInfo.top + 10;
	m_btnSalesmanInfo.MoveWindow(rcBtnSalesmanInfo);

	CRect rcStaticSalesmanInfo(rcClient);
	rcStaticSalesmanInfo.left = rcBtnSalesmanInfo.right + 6;
	rcStaticSalesmanInfo.top = rcClient.top + 410;
	rcStaticSalesmanInfo.right = rcStaticSalesmanInfo.left + 80;
	rcStaticSalesmanInfo.bottom = rcStaticSalesmanInfo.top + 26;
	m_staticSalesmanInfo.MoveWindow(rcStaticSalesmanInfo);

	//ҵ��Ա����
	CRect rcStaticSalesmanName1(rcClient);
	rcStaticSalesmanName1.left = rcStaticSalesmanName1.left + 40;
	rcStaticSalesmanName1.top = rcStaticSalesmanName1.top + 410 + 26 + 14;
	rcStaticSalesmanName1.right = rcStaticSalesmanName1.left + 65;
	rcStaticSalesmanName1.bottom = rcStaticSalesmanName1.top + 20;
	m_staticSalesmanName1.MoveWindow(rcStaticSalesmanName1);

	CRect rcStaticSalesmanName2(rcStaticSalesmanName1);
	rcStaticSalesmanName2.left = rcStaticSalesmanName2.right + 5;
	rcStaticSalesmanName2.right = rcStaticSalesmanName2.left + 145;
	m_staticSalesmanName2.MoveWindow(rcStaticSalesmanName2);

	CRect rcStaticSalesmanId1(rcStaticSalesmanName2);
	rcStaticSalesmanId1.left = rcStaticSalesmanId1.right + 5;
	rcStaticSalesmanId1.right = rcStaticSalesmanId1.left + 65;
	m_staticSalesmanId1.MoveWindow(rcStaticSalesmanId1);

	CRect rcStaticSalesmanId2(rcStaticSalesmanId1);
	rcStaticSalesmanId2.left = rcStaticSalesmanId2.right + 5;
	rcStaticSalesmanId2.right = rcStaticSalesmanId2.left + 140;
	m_staticSalesmanId2.MoveWindow(rcStaticSalesmanId2);
	/********************ҵ��Ա��ϢEnd********************/

	/********************Ԥ������Start********************/
	CRect rcDlgRecName(rcClient);
	rcDlgRecName.left = rcDlgRecName.right - 420;
	rcDlgRecName.top = rcDlgRecName.top + 26;
	rcDlgRecName.right = rcDlgRecName.right - 20;
	rcDlgRecName.bottom = rcDlgRecName.top + 30;
	if (NULL != m_lpRecNameInfoDlg)
	{
		m_lpRecNameInfoDlg->MoveWindow(rcDlgRecName);
	}

	CRect rcPreview(rcClient);
	rcPreview.left = rcPreview.right - 420;
	rcPreview.top = rcDlgRecName.bottom;
	rcPreview.right = rcPreview.right - 20;
	rcPreview.bottom = rcPreview.top + 260;
	if (NULL != m_lpPreviewCtrlDlg)
	{
		m_lpPreviewCtrlDlg->MoveWindow(rcPreview);
	}
	/********************Ԥ������End********************/

	/********************¼���б���ϴ�Start********************/
	//�ѹ����ļ�
	CRect rcRecInfo(rcClient);
	rcRecInfo.left = rcRecInfo.right - 406;
	rcRecInfo.top = rcPreview.bottom + 4;
	rcRecInfo.right = (rcRecInfo.right - 20 - 210);
	rcRecInfo.bottom = rcRecInfo.top + 20;
	m_staticRecList.MoveWindow(rcRecInfo);

	//�ϴ�������Ƶ
	CRect rcRecUpload(rcClient);
	rcRecUpload.left = rcRecUpload.right - 110;
	rcRecUpload.top = rcPreview.bottom;
	rcRecUpload.right = (rcRecUpload.right - 25);
	rcRecUpload.bottom = rcRecUpload.top + 26;
	m_btnUpLoad.MoveWindow(rcRecUpload);

	//¼���б�
	CRect rcRecordsList(rcClient);
	rcRecordsList.left = rcPreview.left + 1;
	rcRecordsList.top = rcRecUpload.bottom + 3;
	rcRecordsList.right = rcPreview.right - 1;
	rcRecordsList.bottom = rcClient.bottom - 28;
	m_listRecords.MoveWindow(rcRecordsList);

	//������
	CRect rcStaticBlank2(rcClient);
	rcStaticBlank2.left = rcPreview.left;
	rcStaticBlank2.top = rcPreview.bottom;
	rcStaticBlank2.right = rcStaticBlank2.left + 1;
	rcStaticBlank2.bottom = rcRecordsList.bottom;
	m_staticBlank2.MoveWindow(rcStaticBlank2);

	//������
	CRect rcStaticBlank3(rcClient);
	rcStaticBlank3.left = rcPreview.right - 1;
	rcStaticBlank3.top = rcPreview.bottom;
	rcStaticBlank3.right = rcPreview.right;
	rcStaticBlank3.bottom = rcRecordsList.bottom;
	m_staticBlank3.MoveWindow(rcStaticBlank3);

	//¼���б�ͷ������
	CRect rcStaticBlank4(rcClient);
	rcStaticBlank4.left = rcStaticBlank2.right;
	rcStaticBlank4.top = rcRecordsList.top - 3;
	rcStaticBlank4.right = rcStaticBlank3.left;
	rcStaticBlank4.bottom = rcRecordsList.top - 2;
	m_staticBlank4.MoveWindow(rcStaticBlank4);
	/********************¼���б���ϴ�End********************/

	//��ӡ��ť
	CRect rcBtnPrint(rcClient);
	rcBtnPrint.left = rcClient.left + 590;
	rcBtnPrint.top = rcRecordsList.bottom + 11;
	rcBtnPrint.right = rcBtnPrint.left + 90;
	rcBtnPrint.bottom = rcBtnPrint.top + 30;
	m_btnPrint.MoveWindow(rcBtnPrint);

	//�ύ��ť
	CRect rcBtnSubmit(rcBtnPrint);
	rcBtnSubmit.left = rcBtnPrint.right + 10;
	rcBtnSubmit.right = rcBtnSubmit.left + 90;
	m_btnSubmit.MoveWindow(rcBtnSubmit);

	//���ð�ť
	CRect rcBtnReset(rcBtnPrint);
	rcBtnReset.left = rcBtnSubmit.right + 10;
	rcBtnReset.right = rcBtnReset.left + 90;
	m_btnReset.MoveWindow(rcBtnReset);

	//�ײ�����
	CRect rcStaticBlank1(rcClient);
	rcStaticBlank1.left = rcClient.left + 20;
	rcStaticBlank1.top = rcClient.top + 480;
	rcStaticBlank1.right = rcClient.left + 880;
	rcStaticBlank1.bottom = rcStaticBlank1.top + 3;
	m_staticBlank1.MoveWindow(rcStaticBlank1);
}

//��ʼԤ��
BOOL CBusinessManageDlg::StartPreview(void)
{
	CConfigDlg *pCfgDlg = new CConfigDlg;
	if (!pCfgDlg)
	{
		return FALSE;
	}
// 	if (dlg.GetUSBCamCount() <= 0) //δ��⵽USB���
// 	{
// 		FMessageBox::Show(_T("δ��⵽USB����������������Ӻ��ٿ���Ԥ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
// 		return FALSE;
// 	}
	if (pCfgDlg->GetUSBCamCount() < theApp.m_nCamIndex + 1)
	{
		FMessageBox::Show(_T("��⵽ѡ���Ĭ��USB������Ѳ�����\r\n�뵽��������������ѡ��USB�����������������"), _T("����"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	PREVIEW_CAMINFO stCameraInfo;
	stCameraInfo.nCamType = CAM_TYPE_USB;
	stCameraInfo.strCamName = _T("USB Camera00");
	stCameraInfo.strMicName = _T("USB Mic");
	stCameraInfo.lCamIndex = theApp.m_nCamIndex;
	stCameraInfo.lMicIndex = theApp.m_nCamIndex;
	stCameraInfo.nTimeout = theApp.m_nMaxRecordTime;
	stCameraInfo.nDelayTime = theApp.m_nMaxRecordTime;
	if (NULL !=m_lpPreviewCtrlDlg)
	{
		m_lpPreviewCtrlDlg->StartPreview(stCameraInfo);
	}

	if (NULL != pCfgDlg)
	{
		delete pCfgDlg;
		pCfgDlg = NULL;
	}
		

	return TRUE;
}

BOOL CBusinessManageDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));

	return TRUE;
}

void CBusinessManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFDialog::OnPaint(

	//�ײ�����
	CRect rc1;
	GetDlgItem(IDC_STATIC_BLANK1)->GetWindowRect(&rc1);
	ScreenToClient(rc1);
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(204, 204, 204));
	CPen * pOld = dc.SelectObject(&linePen);
	dc.MoveTo(rc1.left, rc1.top + rc1.Height() / 2);
	dc.LineTo(rc1.right, rc1.top + rc1.Height() / 2);

	//������
	CRect rc2;
	GetDlgItem(IDC_STATIC_BLANK2)->GetWindowRect(&rc2);
	ScreenToClient(rc2);
	dc.MoveTo(rc2.left + rc2.Width() / 2, rc2.top);
	dc.LineTo(rc2.left + rc2.Width() / 2, rc2.bottom);

	//������
	CRect rc3;
	GetDlgItem(IDC_STATIC_BLANK3)->GetWindowRect(&rc3);
	ScreenToClient(rc3);
	dc.MoveTo(rc3.left + rc3.Width() / 2, rc3.top);
	dc.LineTo(rc3.left + rc3.Width() / 2, rc3.bottom);

	//¼���б�ͷ������
	CRect rc4;
	GetDlgItem(IDC_STATIC_BLANK4)->GetWindowRect(&rc4);
	ScreenToClient(rc4);
	dc.MoveTo(rc4.left, rc4.top + rc4.Height() / 2);
	dc.LineTo(rc4.right, rc4.top + rc4.Height() / 2);

	dc.SelectObject(pOld);
	linePen.DeleteObject();
}

BOOL CBusinessManageDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_NCLBUTTONDBLCLK)  ////���α�����˫���¼�(���/��ԭ)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))  
	{
		return TRUE;
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PURCHASEAMOUNT)) && (pMsg->message == WM_CHAR))  //ֻ���������֡�С�����
	{
		if (pMsg->wParam <= '9' && pMsg->wParam >= '0' || pMsg->wParam == '.' || pMsg->wParam == VK_BACK)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PRODUCTCODE)) && (pMsg->wParam == VK_RETURN))
	{
		CString strCode;
		GetDlgItem(IDC_EDIT_PRODUCTCODE)->GetWindowText(strCode);
		if (strCode != _T(""))
		{
			GetProductInfoByCode(strCode);
			return FALSE;
		}
		return TRUE;
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_CUSTOMERNAME)) &&  (pMsg->message == WM_CHAR))	//�ͻ����������ַ�Լ��
	{
		if (pMsg->wParam == '\\' || pMsg->wParam == '/' || pMsg->wParam == ':' ||
			pMsg->wParam == '*' || pMsg->wParam == '*' || pMsg->wParam == '&' ||
			pMsg->wParam == '?' || pMsg->wParam == '\"' || pMsg->wParam == '<' ||
			pMsg->wParam == '|' || pMsg->wParam == '\'' || pMsg->wParam == '%' ||
			pMsg->wParam == '%' || pMsg->wParam == '>')
		{
			return TRUE;
		}
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_IDNUMBER)) &&  (pMsg->message == WM_CHAR))	//֤������
	{
		if (pMsg->wParam == '\\' || pMsg->wParam == '/' || pMsg->wParam == ':' ||
			pMsg->wParam == '*' || pMsg->wParam == '*' || pMsg->wParam == '&' ||
			pMsg->wParam == '?' || pMsg->wParam == '\"' || pMsg->wParam == '<' ||
			pMsg->wParam == '|' || pMsg->wParam == '\'' || pMsg->wParam == '%' ||
			pMsg->wParam == '%' || pMsg->wParam == '>')
		{
			return TRUE;
		}
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PRODUCTCODE)) &&  (pMsg->message == WM_CHAR))	//��Ʒ����
	{
		if (pMsg->wParam == '\\' || pMsg->wParam == '/' || pMsg->wParam == ':' ||
			pMsg->wParam == '*' || pMsg->wParam == '*' || pMsg->wParam == '&' ||
			pMsg->wParam == '?' || pMsg->wParam == '\"' || pMsg->wParam == '<' ||
			pMsg->wParam == '|' || pMsg->wParam == '\'' || pMsg->wParam == '%' ||
			pMsg->wParam == '%' || pMsg->wParam == '>')
		{
			return TRUE;
		}
	}
	

	return CFDialog::PreTranslateMessage(pMsg);
}

//����ҵ�񵥺�
BOOL CBusinessManageDlg::GenerateBusinessId(void)
{
	srand((int)time(0));  //���������
	CTime tmCurrent = CTime::GetCurrentTime();
	CString strTime;  //��ʽ��������ʱ����+5λ�����
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d%05d"), 
		tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 
		tmCurrent.GetHour(), tmCurrent.GetMinute(), tmCurrent.GetSecond(), random(100000));
	//m_editBusinessId.SetWindowText(strTime);
	m_comboBusinessId.SetText(strTime);
	m_strBusinessId = strTime;

	return TRUE;
}

void CBusinessManageDlg::InitData(void)
{
	//����ҵ�񵥺�
	GenerateBusinessId();

	 //Ĭ������
	m_comboBusinessLocation.SetText(theApp.m_strRegionName);

	//��������
	m_treeRegion = &(m_comboBusinessLocation.GetTreeCtrl());
	m_treeRegion->DeleteAllItems();
	m_treeRegion->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
	m_comboBusinessLocation.SetReadOnly(TRUE);
	LoadRegion(TVI_ROOT, 0);

	//����֤��
	LoadIDType();

	//���ز�Ʒ����
	m_comboProductType.SetActrueParent(this);	//ʵ�ʵĸ����ڣ�������Ϣ��
	m_treeProductType = &(m_comboProductType.GetTreeCtrl());
	m_treeProductType->DeleteAllItems();
	m_treeProductType->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
	m_comboProductType.SetReadOnly(TRUE);
	LoadProductType(TVI_ROOT, 0);

	m_staticSalesmanName2.SetWindowText(theApp.m_strSalesmanName);	//ҵ��Ա����
	m_staticSalesmanName2.SetTipText(theApp.m_strSalesmanName);
	m_staticSalesmanId2.SetWindowText(theApp.m_strSalesmanCode);		//ҵ��Ա����
	m_staticSalesmanId2.SetTipText(theApp.m_strSalesmanCode);

	return;
}

//��������
void CBusinessManageDlg::ResetData(void)
{
	m_bIsHistory = false;
	//����ҵ�񵥺�
	GenerateBusinessId();
	m_nBusinessIndex = -1;

	//��ʼ����
	m_comboBusinessLocation.SetText(theApp.m_strRegionName); //Ĭ������
	m_nRegionId = -1;

	//�ͻ���Ϣ
	m_editCustomerName.SetWindowText(_T(""));
	m_editCustomerPhone.SetWindowText(_T(""));
	if (m_comboIDType.GetCount() >= 0)
	{
		m_comboIDType.SetCurSel(0);
	}
	m_editIDNumber.SetWindowText(_T(""));

	//��Ʒ��Ϣ
	m_comboProductType.SetText(_T(""));
	m_comboProductName.ResetContent();	//��ղ�Ʒ�б�
	m_editProductCode.SetWindowText(_T(""));
	m_editIssuer.SetWindowText(_T(""));
	m_editPurchaseAmount.SetWindowText(_T(""));
	m_nProductId = -1;

	//¼����Ϣ
	m_listRecords.DeleteAllItems();
	m_RecFileArray.SetSize(0);
	m_importFileArray.SetSize(0);
	m_PicFileArray.SetSize(0);
	m_ArrayFileList.SetSize(0);

// 	//��������
// 	m_treeRegion = &(m_comboBusinessLocation.GetTreeCtrl());
// 	m_treeRegion->DeleteAllItems();
// 	m_treeRegion->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
// 	m_comboBusinessLocation.SetReadOnly(TRUE);
// 	LoadRegion(TVI_ROOT, 0);
// 
// 	//���ز�Ʒ����
// 	m_comboProductType.SetActrueParent(this);	//ʵ�ʵĸ����ڣ�������Ϣ��
// 	m_treeProductType = &(m_comboProductType.GetTreeCtrl());
// 	m_treeProductType->DeleteAllItems();
// 	m_treeProductType->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
// 	m_comboProductType.SetReadOnly(TRUE);
// 	LoadProductType(TVI_ROOT, 0);
}

void CBusinessManageDlg::LoadRegion(HTREEITEM hParentItem, int nParentId)
{
	m_ArrayRegionId.SetSize(0);
	m_ArrayRegionPId.SetSize(0);
	m_ArrayRegionName.SetSize(0);

	//�����б�
	RegionList regionList;
	if (!CCMSMgr::SnglPtr()->GetRegionList(nParentId, regionList) || regionList.region_size() == 0)
	{
		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			HTREEITEM hItem = m_treeRegion->InsertItem(_T("���Ƶ�Ԫ"), 0);
			return;
		}
	}
	else
	{
		for (int i = 0; i < regionList.region_size(); i++)
		{
			const cms_8120::Region& region = regionList.region(i);

			CString strId;
			strId.Format(_T("%d"), region.id());
			m_ArrayRegionId.Add(strId);

			strId.Format(_T("%d"), region.nparentid());
			m_ArrayRegionPId.Add(strId);

			CString strRegionName = UTF8_A2MFCSTRW(region.strname());
			m_ArrayRegionName.Add(strRegionName);
		}

		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			m_hRoot= m_treeRegion->InsertItem(_T("���Ƶ�Ԫ"), 0);
		}
	
		//����ڵ�
		InsertNode(TREE_DLG_TYPE_REGION, m_hRoot, 1);
		ExpandBranch(m_hRoot, *m_treeRegion, TRUE);
	}

}

void CBusinessManageDlg::InsertNode(int nType, HTREEITEM hParantNode, int nPId)
{
	HTREEITEM hItem;
	switch(nType)
	{
	case TREE_DLG_TYPE_REGION:
		{
			for(int i = 1; i < m_ArrayRegionId.GetSize(); i++)
			{
				if (nPId == _ttoi(m_ArrayRegionPId.GetAt(i)))
				{
					hItem = m_treeRegion->InsertItem(m_ArrayRegionName.GetAt(i), hParantNode);
					InsertNode(nType, hItem, _ttoi(m_ArrayRegionId.GetAt(i)));
					CString strRegionName;
					m_comboBusinessLocation.GetText(strRegionName);
// 					if (strRegionName.IsEmpty())
// 					{
// 						m_treeRegion->SelectItem(m_treeRegion->GetRootItem());
// 					}
// 					if (strRegionName == m_ArrayRegionName.GetAt(i))
// 					{
// 						m_treeRegion->SelectItem(hItem);
// 					}
				}
			}
		}
		break;
	case TREE_DLG_TYPE_PRODUCTTYPE:
		{
			for (int i = 1; i < m_ArrayProductTypeId.GetSize(); i++)
			{
				if (nPId == _ttoi(m_ArrayProductTypePId.GetAt(i)))
				{
					hItem = m_treeProductType->InsertItem(m_ArrayProductTypeName.GetAt(i), hParantNode);
					InsertNode(nType, hItem, _ttoi(m_ArrayProductTypeId.GetAt(i)));
					CString strTypeName;
					m_comboProductType.GetText(strTypeName);
					if (strTypeName.IsEmpty())
					{
						m_treeProductType->SelectItem(m_treeProductType->GetRootItem());
					}
					if (strTypeName == m_ArrayProductTypeName.GetAt(i))
					{
						m_treeProductType->SelectItem(hItem);
					}
				}
			}
		}
		break;
	default:
		break;
	}
	
	return;
}

void CBusinessManageDlg::ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand /*=TRUE*/)  	  
{
	if (tree.ItemHasChildren(hItem))
	{
		tree.Expand(hItem,bExpand?TVE_EXPAND:TVE_COLLAPSE);
		hItem=tree.GetChildItem(hItem);

		do 
		{
			ExpandBranch(hItem, tree,TRUE);
		} 
		while((hItem=tree.GetNextSiblingItem(hItem))!=NULL);
	}

	return;
}
void CBusinessManageDlg::LoadIDType(void)
{
	cms_8120::ParamDictionaryList IdTypeList;
	if (!CCMSMgr::SnglPtr()->GetIdTypeList(IdTypeList) || IdTypeList.paramdictionary_size() == 0)
	{
		int n = IdTypeList.paramdictionary_size();
		FMSCLIENT_ERROR("LoadIDType Failed!");
		return;
	}

	for (int i = 0; i < IdTypeList.paramdictionary_size(); i++)
	{
		cms_8120::ParamDictionary IdType = IdTypeList.paramdictionary(i);
		int nID = IdType.id();
		CString strIDType = UTF8_A2MFCSTRW(IdType.strname());
		m_IDMap.SetAt(nID, strIDType);
		//m_arrayIDType.Add(IdType.id());
		m_comboIDType.InsertString(i, strIDType);
	}
	if (m_comboIDType.GetCount() >= 0)
	{
		m_comboIDType.SetCurSel(0);
	}
}

//��ȡ��Ʒ����
void CBusinessManageDlg::LoadProductType(HTREEITEM hParentItem, int nParentId)
{
	m_ArrayProductTypeId.SetSize(0);
	m_ArrayProductTypePId.SetSize(0);
	m_ArrayProductTypeName.SetSize(0);
	//��Ʒ�����б�
	cms_8120::ProductTypeList productTypeList;
	if (!CCMSMgr::SnglPtr()->GetProductTypeList(productTypeList) || productTypeList.producttype_size() == 0)
	{
		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			FMSCLIENT_ERROR("LoadProductType Failed!");
			return;
		}
		return;
	}
	else
	{
		for (int i = 0; i < productTypeList.producttype_size(); i++)
		{
			cms_8120::ProductType productType = productTypeList.producttype(i);

			CString strId;
			strId.Format(_T("%d"), productType.id());
			m_ArrayProductTypeId.Add(strId);

			strId.Format(_T("%d"), productType.pid());
			m_ArrayProductTypePId.Add(strId);

			CString strProductTypeName = UTF8_A2MFCSTRW(productType.strname());
			m_ArrayProductTypeName.Add(strProductTypeName);
		}

		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			m_hRoot = m_treeProductType->InsertItem(_T("��Ʒ�����б�"), 0);
		}

		//����ڵ�
		InsertNode(TREE_DLG_TYPE_PRODUCTTYPE, m_hRoot, 1);

		//չ��
		ExpandBranch(m_hRoot, *m_treeProductType, TRUE);
	}
}

//��ȡ��Ʒ����
void CBusinessManageDlg::LoadProduct(CString & strTempName, HTREEITEM hParentItem, int nParentId)
{

	m_comboProductName.ResetContent();  //���
	//��Ʒ�б�
	cms_8120::ProductList productList;
	if (!CCMSMgr::SnglPtr()->GetProductList(productList, nParentId) || productList.product_size() == 0)
	{
		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			FMSCLIENT_ERROR("LoadProduct Failed!");
			return;
		}
		else
		{
			m_comboProductName.SetText(_T("���޿�ѡ��Ʒ"));
			return;
		}
		return;
	}
	else
	{
		for (int i = 0; i < productList.product_size(); i++)
		{
			cms_8120::Product product = productList.product(i);

			CString strId;
			strId.Format(_T("%d"), product.id());
			m_ArrayProductId.Add(strId);

			strId.Format(_T("%d"), product.pid());
			m_ArrayProductPId.Add(strId);

			CString strProductName = UTF8_A2MFCSTRW(product.strname());
			m_ArrayProductName.Add(strProductName);

			m_comboProductName.InsertString(i, strProductName);

			if (strTempName == strProductName)
			{
				m_comboProductName.SetWindowText(strTempName);
			}
		}
	}
}

void CBusinessManageDlg::GetProductInfoByCode(CString strCode)
{
	cms_8120::ProductInfo productInfo;
	if (!CCMSMgr::SnglPtr()->GetProductInfoByCode(productInfo, strCode) || productInfo.result() != 0)
	{
		CString strError = UTF8_A2MFCSTRW(productInfo.result_info());
		FMSCLIENT_ERROR("GetProductInfoByCode Failed!%s", fcT2A(strError));
		return;
	}
	else
	{
		cms_8120::Product product = productInfo.product();
		//��Ʒ����
		CString strProductType = UTF8_A2MFCSTRW(product.producttype());
		m_comboProductType.SetText(strProductType);
		//��Ʒ����
		CString strProductName = UTF8_A2MFCSTRW(product.strname());
		m_comboProductName.SetText(strProductName);
		//���л���
		CString strIssure = UTF8_A2MFCSTRW(product.strgrade());
		m_editIssuer.SetWindowText(strIssure);

		m_nProductId = product.id();
	}
}

//����ͼƬ���б�
BOOL CBusinessManageDlg::InsertPic(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.csPicPath;
	HANDLE hFileHandle = NULL;
	//���
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);  
	//�ļ���
	CString strFileName = recordFile.csPicPath.Mid(recordFile.csPicPath.ReverseFind('\\') + 1);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.csPicPath);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//¼��ʱ��
	CString strFileDuration = _T("--");
	m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	//�ļ���С
	hFileHandle = CreateFile(recordFile.csPicPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFileHandle)
	{
		FMSCLIENT_ERROR("CreateFile Failed![Error:%d]", GetLastError());
		return FALSE;
	}

	int nFileSize = (long)GetFileSize(hFileHandle, NULL);
	CloseHandle(hFileHandle);
	hFileHandle = NULL;
	CString strFileSize;
	strFileSize.Format(_T("%.2fM"),  (double)nFileSize / 1024 / 1024);
	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//��¼������ļ�
	m_PicFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.csPicPath);

	return TRUE;
}

BOOL CBusinessManageDlg::InsertRec(const RECOED_FILE_INFO &recordFile)
{
// 	ITEMDATA * pData = new ITEMDATA;
// 	pData->strFileName = recordFile.strFileName;
	//���
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//m_listRecords.SetItemData(nIndex, (DWORD)pData);
	//�ļ���
	//int nPos = recordFile.strFileName.ReverseFind('\\');
	CString strFileName = recordFile.strFileName.Mid(recordFile.strFileName.ReverseFind('\\') + 1);
	m_listRecords.SetItemText(nIndex, 1, strFileName);
	//m_listRecords.SetItemText(nIndex, 1, recordFile.strFileName);
	//¼��ʱ��
	CString strFileDuration;
	int nFileDuration = recordFile.lEndTime - recordFile.lStartTime;
	if (0 == nFileDuration)
	{
		strFileDuration = _T("--");
		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	}
	else
	{
		strFileDuration.Format(_T("%02d:%02d:%02d"), nFileDuration / 3600, nFileDuration % 3600 / 60, nFileDuration % 3600 % 60);		
		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
	}

	//�ļ���С
	CString strFileSize;
	strFileSize.Format(_T("%.2fM"),  (double)recordFile.lFileSize / 1024 / 1024);
	/*double dFileSize = (double)recordFile.lFileSize / 1024 / 1024;*/
	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//��ӵ�¼���б�
	m_RecFileArray.Add(recordFile);
	m_ArrayFileList.Add(recordFile.strFileName);

	return TRUE;
}

//����¼��������ʾ֪ͨ��Ϣ
void CBusinessManageDlg::NotifyRecordFileInfo(const RECOED_FILE_INFO &recordFile)
{
	BOOL bRet = FALSE;
	CString strFileName = recordFile.csPicPath;
	const RECOED_FILE_INFO * pRecordFileInfo = new RECOED_FILE_INFO(recordFile);
	RECOED_FILE_INFO recInfo = recordFile;
	
	if (0 == recordFile.nRecordStatus)  //��ʼ¼��(¼����)
	{
		InsertPic(recordFile);	//���뿪ʼ¼���ͼ
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STARTRECORD, WPARAM(pRecordFileInfo), 0);
		}
		//��ʼ¼����ð�ť
		m_btnPrint.EnableWindow(FALSE);
		m_btnSubmit.EnableWindow(FALSE);
		m_btnReset.EnableWindow(FALSE);
		m_bIsRecording = true;
	}
	else if (1 == recordFile.nRecordStatus)  //����¼��
	{
		InsertPic(recordFile);	//�������¼���ͼ
		if (NULL != m_lpRecNameInfoDlg)
		{
			bRet = ::SendMessage(m_lpRecNameInfoDlg->GetSafeHwnd(), WM_PV_STOPRECORD, WPARAM(pRecordFileInfo), 0);
			//m_lpRecNameInfoDlg->StopRecord(recInfo.strFileName);
			CLIENT_INFO("Before Rename:%s", recordFile.strFileName);
			//CFile::Rename(recordFile.strFileName, recInfo.strFileName);
			CLIENT_INFO("After Rename:%s", recInfo.strFileName);
		}
		InsertRec(recInfo);	//����¼���ļ�
		//����¼��ָ���ť�״̬
// 		m_btnPrint.EnableWindow(TRUE);
// 		m_btnSubmit.EnableWindow(TRUE);
//  		m_btnReset.EnableWindow(TRUE);
		m_bIsRecording = false;
	}
	delete pRecordFileInfo;
}

//������Ϣ����
BOOL CBusinessManageDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// �ఴť�����Ϣ
	if (MSG_MULTIBTN_CLICK == wParam)  
	{
		//��ť��Ϣ�Ľṹ��
		STRU_MULTIBTN_INFO * multiBtnInfo = (STRU_MULTIBTN_INFO*)lParam;
		//CString strFileName = m_listRecords.GetItemText(multiBtnInfo->nItem, 1);
// 		ITEMDATA * pData = (ITEMDATA *)m_listRecords.GetItemData(multiBtnInfo->nItem);
// 		if (!pData)
// 		{
// 			return FALSE;
// 		}
// 		CString strFileName = pData->strFileName;
		int nItem = multiBtnInfo->nItem;
		if (-1 == nItem)
		{
			FMSCLIENT_ERROR("δѡ���κ���!\n");
			//return FALSE;
		}
	
		CString strFileName = m_ArrayFileList.GetAt(nItem);
		if (multiBtnInfo->nNumber == 0)	// �޸��ļ�����ť
		{
			CEditFileNameDlg dlg;
			dlg.SetDlgData(strFileName);
			if (IDOK == dlg.DoModal())
			{
				m_listRecords.SetItemText(nItem, 1, theApp.m_strNewFileName);
				CString strText = strFileName.Mid(0, strFileName.ReverseFind('\\')) + _T('\\') + theApp.m_strNewFileName;
				m_ArrayFileList.SetAt(nItem, strText);
// 				pData->strFileName = strText;
// 				m_listRecords.SetItemData(nItem, (DWORD)pData);
				if (strFileName.Find(_T("mp4")) > 0)   //��Ƶ
				{
					for (int i = 0; i < m_RecFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_RecFileArray.GetAt(i);
						if (recInfo.strFileName.Find(strFileName) >= 0)
						{
							RECOED_FILE_INFO info = recInfo;
							info.strFileName = strText;
							m_RecFileArray.SetAt(i, info);
						}
					}
				}
				else
				{
					for (int i = 0; i < m_PicFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(i);
						if (recInfo.csPicPath.Find(strFileName) >= 0)
						{
							RECOED_FILE_INFO info = recInfo;
							info.csPicPath = strText;
							m_PicFileArray.SetAt(i, info);
						}
					}
				}
			}
		}
		else if (multiBtnInfo->nNumber == 1)		//�طŰ�ť
		{
			if (strFileName.Find(_T("mp4")) > 0)   //��Ƶ
			{
				m_lpPlaybackCtrlDlg->ShowWindow(SW_SHOW);
				m_lpPlaybackCtrlDlg->StartPlayBack(0, strFileName);
			}
			else		//ͼƬ
			{
				TCHAR szSystemDir[MAX_PATH];
				::GetSystemDirectory(szSystemDir, MAX_PATH);

				CString strRundll;
				strRundll.Format(_T("%s\\rundll32.exe"), szSystemDir);

				//�滻·����������ļ�ʧ��
				CString strTemp = strFileName;
				strTemp.Replace(_T("\\\\"), _T("\\"));
				
				CString strParam;
				strParam.Format(_T("%s\\shimgvw.dll imageview_fullscreen %s"), szSystemDir, strTemp);
				ShellExecute(NULL, _T("Open"), strRundll.GetBuffer(), strParam, NULL, SW_SHOW);
			}
			
		}
		else if (multiBtnInfo->nNumber == 2)  //ɾ����ť
		{
			if (IDYES == FMessageBox::Show(_T("ȷ��ɾ�����ļ���\r\nע�⣺�����ļ����ᱻɾ����"), _T("ɾ��"), MB_YESNO | MB_ICONQUESTION))
			{
				if (strFileName.Find(_T(".mp4")) > 0)
				{
					for (int i = 0; i < m_RecFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_RecFileArray.GetAt(i);
						if (recInfo.strFileName.Find(strFileName) >= 0)
						{
							m_RecFileArray.RemoveAt(i);
							m_listRecords.DeleteItem(nItem);
						}
					}
				}
				else
				{
					for (int i = 0; i < m_PicFileArray.GetSize(); i++)
					{
						RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(i);
						if (recInfo.csPicPath.Find(strFileName) >= 0)
						{
							m_PicFileArray.RemoveAt(i);
							m_listRecords.DeleteItem(nItem);
						}
					}
				}

				m_ArrayFileList.RemoveAt(nItem);
				
				if (!::DeleteFile(strFileName))
				{
					FMSCLIENT_ERROR("ɾ�������ļ�ʧ��[%d]��%s", GetLastError(), fcT2A(strFileName));
					delete multiBtnInfo;
					return FALSE;
				}
			}
		}

		//�ͷ��ڴ棬������TRUE
		delete multiBtnInfo;
		return TRUE;
	}
	else if (MSG_COMBO_SELCHANGE == wParam)  //��Ʒ����ѡ��ı���Ϣ
	{
		m_comboProductType.SetState(FALSE);
		m_comboProductName.ResetContent();
		m_editProductCode.SetWindowText(_T(""));
		m_editIssuer.SetWindowText(_T(""));
		m_nProductId = -1;
	}

	return CFDialog::OnCommand(wParam, lParam);
}


BOOL CBusinessManageDlg::GetRegionId()
{
	CString strRegionName = _T("");
	m_comboBusinessLocation.GetText(strRegionName);
	int i = 0;
	for(i = 0; i < m_ArrayRegionName.GetSize(); i++)  //��ѯ����ID
	{
		if (strRegionName == m_ArrayRegionName.GetAt(i))
		{
			m_nRegionId = _ttoi(m_ArrayRegionId.GetAt(i));
			break;
		}
	}

	//��ѯ����IDʧ��
	if (i == m_ArrayRegionName.GetSize())
	{
		FMessageBox::Show(_T("��ȡ������ʧ�ܣ�������ѡ������"), _T("����"), MB_OK | MB_ICONWARNING);
		FMSCLIENT_ERROR("Find Region Id Failed:%s", fcT2A(strRegionName));
		return FALSE;
	}

	return TRUE;
}

BOOL CBusinessManageDlg::GetProductId()
{
// 	int i = 0;
// 	for(i = 0; i < m_ArrayProductName.GetSize(); i++)  //��ѯ��ƷID
// 	{
// 		if (strProductName == m_ArrayProductName.GetAt(i))
// 		{
// 			m_nProductId = _ttoi(m_ArrayProductId.GetAt(i));
// 			break;
// 		}
// 	}
// 
// 	//��ѯ��ƷIDʧ��
// 	if (i == m_ArrayProductName.GetSize())
// 	{
// 		FMSCLIENT_ERROR("Find Region Id Failed:%s", strProductName);
// 		return FALSE;
// 	}

	return TRUE;
}

//���ز�Ʒ����
void CBusinessManageDlg::OnCbnDropdownComboProductname()
{
	// ���ԭ������
	m_ArrayProductId.SetSize(0);
	m_ArrayProductPId.SetSize(0);
	m_ArrayProductName.SetSize(0);

	CString strTempName;
	m_comboProductName.GetWindowText(strTempName);

	m_comboProductName.SetState(FALSE);

	CString strProductType;
	m_comboProductType.GetText(strProductType);
	if (strProductType == _T(""))		//δѡ���Ʒ����ʱ�����Ʒ���ƣ����ͱ��
	{
		m_comboProductType.SetState(TRUE);
		CRect rcRect;
		m_comboProductType.GetWindowRect(rcRect);
		ScreenToClient(rcRect);
		InvalidateRect(rcRect);
		return;
	}
	int nProductTypeId = GetProductTypeIdByName(strProductType);
	if (nProductTypeId < 0)
	{
		FMSCLIENT_ERROR("GetProductTypeIdByName Failed��");
		return;
	}
	
	//���ز�Ʒ
	LoadProduct(strTempName, TVI_ROOT, nProductTypeId);
}

//ѡ���Ʒ����
LRESULT CBusinessManageDlg::OnSelectProduct(WPARAM wparam, LPARAM lparam)
{
	TCHAR * strMsg = (TCHAR *)wparam;
	CString strProductName(strMsg);
	m_comboProductName.SetWindowText(strProductName);

	delete strMsg;
	strMsg = NULL;

	return 0;
}

//��Ʒ���Ʊ��
void CBusinessManageDlg::OnCbnSelchangeComboProductname()
{
	int nProductId = -1;
	CString strProductName;
	int nIndex = m_comboProductName.GetCurSel();
	if (nIndex < 0)
	{
		return;
	}
	m_comboProductName.GetLBText(nIndex, strProductName);
	int i = 0;
	for (i = 0; i < m_ArrayProductName.GetSize(); i++)
	{
		if (strProductName == m_ArrayProductName.GetAt(i))
		{
			nProductId = _ttoi(m_ArrayProductId.GetAt(i));
			break;
		}
	}

	if (i == m_ArrayProductId.GetSize())
	{
		FMSCLIENT_ERROR("Get Product Id Failed:%s", fcT2A(strProductName));
		return;
	}


	//��ȡ��Ʒ��Ϣ
	cms_8120::ProductInfo productInfo;
	if (!CCMSMgr::SnglPtr()->GetProductInfo(productInfo, nProductId) || productInfo.result() != 0)
	{
		FMSCLIENT_ERROR("GetProductInfo Failed!%s", fcT2A(UTF8_A2MFCSTRW(productInfo.result_info())));
		return;
	}
	else
	{
		cms_8120::Product product = productInfo.product();
		CString strCode = UTF8_A2MFCSTRW(product.strcode());   //��Ʒ����
		CString strGrade = UTF8_A2MFCSTRW(product.strgrade()); //���л���
		m_editProductCode.SetWindowText(strCode);
		m_editIssuer.SetWindowText(strGrade);
		m_nProductId = nProductId;
	}

	return;
}

//�ϴ�������Ƶ
void CBusinessManageDlg::OnBnClickedBtnUpload()
{
	m_importFileArray.SetSize(0);
	CString strFileFilter = _T("��Ƶ�ļ�(*.mp4)|*.mp4|ͼƬ�ļ�(*.jpg(*.jpeg);*.png)|*.jpg;*.jpeg;*.png||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, strFileFilter);
	if (fileDlg.DoModal() == IDOK)
	{
		CString strFilePath = _T("");
		POSITION pos = fileDlg.GetStartPosition();
		while(NULL != pos)
		{
			strFilePath = fileDlg.GetNextPathName(pos);
			strFilePath.Replace(_T("\\"), _T("\\\\"));
			m_importFileArray.Add(strFilePath);
		}
	}

	for (int i = 0; i < m_importFileArray.GetSize(); i++)
	{
		CString strFileName = m_importFileArray.GetAt(i);

		RECOED_FILE_INFO recordInfo;
		recordInfo.strFileName = strFileName;
		recordInfo.lStartTime = 0;
		recordInfo.lEndTime = 0;
		ULONGLONG size;  
		CFileStatus fileStatus;  
		if (CFile::GetStatus(strFileName, fileStatus))  
		{  
			recordInfo.lFileSize = fileStatus.m_size;  
		} 
		
		if (strFileName.Find(_T(".mp4")) > 0)
		{
			//��ȡ¼���ļ���
			CString strFileNameTemp = strFileName.Mid(strFileName.ReverseFind('\\') + 1);
			if (strFileNameTemp.GetLength() > 50)  //¼���ļ��ַ������
			{
				FMessageBox::Show(_T("��Ƶ�ļ������Ȳ��ܳ���50���ַ���"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				continue;
			}

			if (fileStatus.m_size > 1024 * 1024 * 1204)
			{
				FMessageBox::Show(_T("��Ƶ�ļ���С���ܳ���1024MB��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				continue;
			}
			InsertRec(recordInfo);
		}
		else
		{
			if (fileStatus.m_size > 2 * 1024 * 1024)
			{
				FMessageBox::Show(_T("ͼƬ�ļ���С���ܳ���2MB��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				continue;
			}
			recordInfo.csPicPath = strFileName;
			InsertPic(recordInfo);
		}
	}

}

//ѡ����ʷҵ��
void CBusinessManageDlg::OnBnClickedBtnHistorylist()
{
	if (NULL == m_pHistoryListDlg)
	{
		try
		{
			m_pHistoryListDlg = new CHistoryListDlg(this);
		}
		catch (...)
		{
			return;
		}
	}
	if (IDOK == m_pHistoryListDlg->DoModal())
	{
		m_bIsHistory = true;

		cms_8120::HisClientInput hisClientInput = theApp.m_hisClientInput;
		m_nBusinessIndex = hisClientInput.clientid();  //ҵ�񵥵�id�ţ��޸���
		m_editBusinessId.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.nproid()));	//ҵ�񵥺�
		m_comboBusinessLocation.SetText(UTF8_A2MFCSTRW(hisClientInput.inputregionname()));
		m_editCustomerName.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.name()));	//�ͻ�����
		m_editCustomerPhone.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strcreditphone())); //�ͻ���ϵ��ʽ
		int nIDType = hisClientInput.creditid();
		CString strIDTypeName;
		if (m_IDMap.Lookup(nIDType, strIDTypeName))
		{
			m_comboIDType.ResetContent();
			m_comboIDType.InsertString(0, strIDTypeName);
			if (m_comboIDType.GetCount() >= 0)
			{
				m_comboIDType.SetCurSel(0);
			}
		}
		m_editIDNumber.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.creditcode()));

		m_comboProductType.ResetContent();
		m_comboProductType.SetText(UTF8_A2MFCSTRW(hisClientInput.producttypename()));


		m_comboProductName.ResetContent();
		m_comboProductName.SetText(UTF8_A2MFCSTRW(hisClientInput.productname()));

		m_editProductCode.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.productcode()));
		m_editIssuer.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strgrade()));
		m_editPurchaseAmount.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strbuymoney()));		
	}
	
}

//�ύҵ���¼
void CBusinessManageDlg::OnBnClickedBtnSubmit()
{
	if (m_bIsRecording)
	{
		FMessageBox::Show(_T("����¼���޷��ύ��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (!GetRegionId())
	{
		m_comboBusinessLocation.SetState(TRUE);
		return;
	}
	else
	{
		m_comboBusinessLocation.SetState(FALSE);
	}

	// �ͻ�����
	CString strCustomerName;
	m_editCustomerName.GetWindowText(strCustomerName);
	if (strCustomerName == _T(""))
	{
		m_editCustomerName.SetErrorState(TRUE);
		return;
	}

	//�ͻ��绰
	CString strPhoneNum;
	m_editCustomerPhone.GetWindowText(strPhoneNum);
	//�ͻ��绰����Ϊ��
// 	if (strPhoneNum == _T(""))
// 	{
// 		m_editCustomerPhone.SetErrorState(TRUE);
// 		return;
// 	}

	//֤����
	CString strIDNum;
	m_editIDNumber.GetWindowText(strIDNum);
	if (strIDNum == _T(""))
	{
		m_editIDNumber.SetErrorState(TRUE);
		return;
	}

	//֤������
	//int nIDType = m_arrayIDType.GetAt(m_comboIDType.GetCurSel());
	CString strIDName;
	m_comboIDType.GetWindowText(strIDName);
	if (strIDName == _T(""))
	{
		m_comboIDType.SetState(TRUE);
		return;
	}
	int nIDType = -1;
	int nKey = -1;
	CString strValue;
	POSITION pos = m_IDMap.GetStartPosition();
	while(pos)
	{
		m_IDMap.GetNextAssoc(pos, nKey, strValue);
		if (strValue == strIDName)
		{
			nIDType = nKey;
			break;
		}
	}

	//��Ʒ����
	CString strProductType;
	m_comboProductType.GetWindowText(strProductType);
	if(strProductType == _T(""))
	{
		m_comboProductType.DisplayTree(strProductType);
		//m_comboProductType.SetState(TRUE);
		return;
	}

	//��Ʒ����
	CString strProductName;
	m_comboProductName.GetWindowText(strProductName);
	if (strProductName == _T("")  || strProductName == _T("���޿�ѡ��Ʒ"))
	{
		m_comboProductName.SetState(TRUE);
		m_comboProductName.SetFocus();
		return;
	}

// 	int nProductId = GetProductIdByName(strProductName);
// 	if (nProductId < 0)
// 	{
// 		FMSCLIENT_ERROR("Get GetProductId Failed!");
// 		m_comboProductName.SetState(TRUE);
// 		return;
// 	}

	if (m_nProductId < 0)
	{
		FMSCLIENT_ERROR("ProductId Error!%d", m_nProductId);
		m_comboProductName.SetState(TRUE);
		return;
	}

	//���л���
	CString strIssuer;
	m_editIssuer.GetWindowText(strIssuer);
	if (strIssuer == _T(""))
	{
		m_editIssuer.SetErrorState(TRUE);
		m_editIssuer.SetFocus();
		return;
	}

	//��Ʒ����
	CString strCode;
	m_editProductCode.GetWindowText(strCode);
	if (strCode == _T(""))
	{
		m_editProductCode.SetErrorState(TRUE);
		m_editProductCode.SetFocus();
		return;
	}

	//������
	CString strBuyNum;
	m_editPurchaseAmount.GetWindowText(strBuyNum);
	//������У��
	if (My_atoi(strBuyNum) >= 2147483647)
	{
		m_editPurchaseAmount.SetErrorState(TRUE);
		return;
	}
	int nTime = 0;
	for(int i = 0; i < strBuyNum.GetLength(); i++)
	{
		i = strBuyNum.Find('.', i);
		if (i != -1)
		{
			nTime++;
		}
		else
		{
			break;
		}
	}
	if (nTime > 1)	//���С����
	{
		m_editPurchaseAmount.SetErrorState(TRUE);
		return;
	}
	

	//ҵ����Ϣ
	cms_8120::CSClientInput clientInput;
	clientInput.set_id(m_nBusinessIndex);
	clientInput.set_inputregionid(m_nRegionId);
	clientInput.set_inputuser(theApp.m_nUserId);
	clientInput.set_nproid(UTF8_MFCSTRW2A(m_strBusinessId));
	clientInput.set_name(UTF8_MFCSTRW2A(strCustomerName));
	clientInput.set_creditcode(UTF8_MFCSTRW2A(strIDNum));
	clientInput.set_creditid(nIDType);
	clientInput.set_productid(m_nProductId);	//��Ʒid
	clientInput.set_strbuymoney(UTF8_MFCSTRW2A(strBuyNum));
	clientInput.set_strcreditphone(UTF8_MFCSTRW2A(strPhoneNum));

	//¼����Ϣ
	CArray<cms_8120::CSRecords> recordsArray;
	
	if(0 == m_RecFileArray.GetSize())
	{
		FMessageBox::Show(_T("û��¼���޷��ύ��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return;
	}
	for (int i = 0; i < m_RecFileArray.GetSize(); i++)
	{
		//��ȡ�ļ���
		CString strFileName = m_RecFileArray.GetAt(i).strFileName.Mid(m_RecFileArray.GetAt(i).strFileName.ReverseFind('\\') + 1);
		cms_8120::CSRecords record;
		record.set_strrecordname(UTF8_MFCSTRW2A(strFileName));
		record.set_inputuser(theApp.m_nUserId);
		record.set_inputregionid(m_nRegionId);
		record.set_nrecordfiletype(RECORD_TYPE_LOCAL); //1-NVR¼��2-����¼��
		if (0 == m_RecFileArray.GetAt(i).lStartTime)		//����¼�񣬿�ʼʱ�䴫���ַ���
		{
			record.set_starttime(UTF8_MFCSTRW2A(_T("")));
		}
		else
		{
			CTime tmStart = CTime(m_RecFileArray.GetAt(i).lStartTime);
			record.set_starttime(UTF8_MFCSTRW2A(tmStart.Format(_T("%Y-%m-%d %H:%M:%S"))));
		}
		if (0 == m_RecFileArray.GetAt(i).lEndTime)	//����¼�񣬽���ʱ�䴫���ַ���
		{
			record.set_endtime(UTF8_MFCSTRW2A(_T("")));
		}
		else
		{
			CTime tmEnd = CTime(m_RecFileArray.GetAt(i).lEndTime);
			record.set_endtime(UTF8_MFCSTRW2A(tmEnd.Format(_T("%Y-%m-%d %H:%M:%S"))));
		}
		record.set_strurl(UTF8_MFCSTRW2A(m_RecFileArray.GetAt(i).strFileName));
		record.set_nfilesize(m_RecFileArray.GetAt(i).lFileSize);
		CString strPicPath = _T("");
		if (m_PicFileArray.GetSize() <= 0)	//��ͼƬ
		{
			for (int j = 0; j < 2; j++)
			{
				strPicPath += _T("nopic,");
			}
		}
		else if (m_PicFileArray.GetSize() < 2)	 //ֻ��1��ͼƬ
		{
			for (int j = 0; j < 2; j++)
			{
				strPicPath += m_PicFileArray.GetAt(0).csPicPath;  //����ͼƬһ��
				strPicPath += _T(",");
			}
		}
		else if (m_PicFileArray.GetSize() >= 2)
		{
			for (int j = 0; j < 2 && j < m_PicFileArray.GetSize(); j++)
			{
				strPicPath += m_PicFileArray.GetAt(j).csPicPath;
				strPicPath += _T(",");			 
			}
		}
		
		record.set_screenshotsrc(UTF8_MFCSTRW2A(strPicPath));

		recordsArray.Add(record);
	}


	if (m_bIsHistory)  //��ʷҵ��
	{
// 		CFBusyWait busywait(this);
// 		busywait.ShowBusyWait(_T("�����ύ..."));

		cms_8120::RspCsClientRecord rspMsg;
		if (!CCMSMgr::SnglPtr()->UpdateBusiness(clientInput, recordsArray, rspMsg) || rspMsg.result() != 0)
		{
			CString str = UTF8_A2MFCSTRW(rspMsg.result_info());
			FMSCLIENT_ERROR("UpdateBusiness Failed:%s", fcT2A(str));
			FMessageBox::Show(str, _T("����"), MB_OK | MB_ICONWARNING);
			return;
		}
		else
		{
			CLIENT_INFO("UpdateBusiness Succeed!%d", m_nBusinessIndex);
			FMessageBox::Show(_T("���³ɹ���"), _T("��ʾ"), MB_OK);
			//��ʼ������
			ResetData();
		}
	}
	else	//�½�ҵ��
	{
// 		CFBusyWait busywait(this);
// 		busywait.ShowBusyWait(_T("�����ύ..."));

		cms_8120::RspCsClientRecord rspMsg;
		if (!CCMSMgr::SnglPtr()->SubmitBusiness(clientInput, recordsArray, rspMsg) || rspMsg.result() != 0)
		{
			CString str = UTF8_A2MFCSTRW(rspMsg.result_info());
			FMSCLIENT_ERROR("SubmitBusiness Failed:%s", fcT2A(str));
			FMessageBox::Show(str, _T("����"), MB_OK | MB_ICONERROR);
			return;
		}
		else
		{
			FMessageBox::Show(_T("�ύ�ɹ���"), _T("�ύ"), MB_OK);
			//��ʼ������
			ResetData();
		}
	}
	
	return;
}

//����
void CBusinessManageDlg::OnBnClickedBtnReset()
{
	if (m_bIsRecording)
	{
		FMessageBox::Show(_T("����¼���޷����ã�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES != FMessageBox::Show(_T("ȷ��������\r\nע�⣺���ý�����������ҵ����Ϣ�Ͳ�Ʒ��Ϣ��ͬʱ¼���б���\r\n�����ļ�Ҳ���ᱻɾ����"), _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	//ɾ��¼���б����ļ�
	int n = m_listRecords.GetItemCount();
	for (int i = 0; i < m_listRecords.GetItemCount(); i++)
	{
// 		ITEMDATA * pData = (ITEMDATA *)m_listRecords.GetItemData(i);
// 		CString strFileName = pData->strFileName;

		CString strFileName = m_ArrayFileList.GetAt(i);
	

		if (!::DeleteFile(strFileName))
		{
			FMSCLIENT_ERROR("ɾ�������ļ�ʧ��[%d]��%s", GetLastError(), fcT2A(strFileName));
			continue;
		}
	}

	//��������
	ResetData();

}

//��Ʒ����
void CBusinessManageDlg::OnBnClickedBtnWords()
{
	CString strProductName;
	m_comboProductName.GetWindowText(strProductName);
	if (strProductName == _T(""))
	{
		m_comboProductName.SetState(TRUE);
		m_comboProductName.SetFocus();
		return;
	}

	if (m_nProductId < 0)
	{
		FMessageBox::Show(_T("����ѡ���Ʒ��"), _T("��ʾ"), IDOK);
		return;
	}
	cms_8120::ProductInfo productInfo;
	if (!CCMSMgr::SnglPtr()->GetWordTemplate(productInfo, m_nProductId) || productInfo.result() != 0)
	{
		FMSCLIENT_ERROR("GetWordTemplate Failed!%s", fcT2A(UTF8_A2MFCSTRW(productInfo.result_info())));
		return;
	}
	else
	{
		Product product = productInfo.product();
		CString strWordTemplate = UTF8_A2MFCSTRW(product.wordtemplate());
		CString strProductName;
		m_comboProductName.GetWindowText(strProductName);

		CWordTemplateDlg dlg;
		dlg.SetDlgData(strProductName, strWordTemplate);
		dlg.DoModal();
	}
}

LRESULT CBusinessManageDlg::OnSelectHisClientInput(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//��ӡ
void CBusinessManageDlg::OnBnClickedBtnPrint()
{
	UpdateData(TRUE);

	CString strUrl = theApp.m_strAppPath + _T("\\printBusiness.html");

	HtmlParser htmlParser;
	MemBuffer memOutHtml;
	MemBuffer memHtml;
	int len =WideCharToMultiByte(CP_ACP,0,strUrl,-1,NULL,0,NULL,NULL);  
	char *ptxtTemp =new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,strUrl,-1,ptxtTemp,len,NULL,NULL );  
	char * fileName = "printBusiness.html";
	if (memHtml.loadFromFile(ptxtTemp))
	{
		MemBuffer outFileName;
		outFileName.appendText(fileName);
		outFileName.appendText(".notes.txt", -1, true);
		FILE *out = fopen((const char*)outFileName.getData(), "wb+");
		if (out == NULL)
		{
			FMSCLIENT_ERROR("Can't open output file:%s", outFileName.getData());
		}
		htmlParser.parseHtml((const char*)memHtml.getData(), true);
		htmlParser.dumpHtmlNodes(out);		//��������ļ�
		fclose(out);

		CString strTemp;
		//�ͻ�����
		m_editCustomerName.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_editCustomerName.SetErrorState(TRUE);
			return;
		}
		TCHAR * pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_CONSUMER_NAME, pTemp);

		//֤������
		m_comboIDType.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_comboProductType.SetState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ID_TYPE, pTemp);

		//֤������
		m_editIDNumber.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_editIDNumber.SetErrorState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ID_NUMBER, pTemp);

		//ҵ�񵥺�
		m_comboBusinessId.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_comboBusinessId.SetState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_BUSINESS_ID, pTemp);

		//����ʱ��
		CTime tmNow = CTime::GetCurrentTime();
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmNow.GetHour(), tmNow.GetMinute(), tmNow.GetSecond());
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_BUSINESS_TIME, pTemp);

		//���л���
		m_editIssuer.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_editIssuer.SetErrorState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ISSUER, pTemp);

		//��Ʒ����
		m_comboProductType.GetText(strTemp);
		if (_T("") == strTemp)
		{
			m_comboProductType.SetState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_PRODUCT_TYPE, pTemp);

		//��Ʒ����
		m_comboProductName.GetWindowText(strTemp);
		if (_T("") == strTemp)
		{
			m_comboProductName.SetState(TRUE);
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_PRODUCT_NAME, pTemp);

		//ҵ��Ա����
		m_staticSalesmanName2.GetWindowText(strTemp);
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_SALESMAN_NAME, pTemp);

		//ҵ��Ա���
		m_staticSalesmanId2.GetWindowText(strTemp);
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_SALESMAN_ID, pTemp);

		//ͼƬ
		if (m_bIsHistory)  //��ʷҵ��
		{
			CString strPic1 = UTF8_A2MFCSTRW(theApp.m_hisClientInput.picturestrart());
			if (strPic1 == _T(""))
			{
				pTemp = TEXT("nopic");
			}
			else
			{
				pTemp = strPic1.GetBuffer();
			}
			htmlParser.SetAttributeValue(INDEX_PIC1, pTemp);
	
			CString strPic2 = UTF8_A2MFCSTRW(theApp.m_hisClientInput.pictureend());
			if (strPic2 == _T(""))
			{
				pTemp = TEXT("nopic");
			}
			else
			{
				pTemp = strPic2.GetBuffer();
			}
			htmlParser.SetAttributeValue(INDEX_PIC2, pTemp);
		}
		else			//�½�ҵ��
		{
			if (m_PicFileArray.GetSize() <= 0)	//��ͼƬ
			{
				FMSCLIENT_ERROR("picture Array is empty.");
				for (int i = 0; i < 2; i++)
				{
					pTemp = TEXT("nopic");
					htmlParser.SetAttributeValue(INDEX_PIC1, pTemp);
					htmlParser.SetAttributeValue(INDEX_PIC2, pTemp);
				}
			}
			else if (m_PicFileArray.GetSize() < 2)	//ֻ��1��ͼƬ
			{
				RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(0);
				CString strPicPath = recInfo.csPicPath;
				if (strPicPath.IsEmpty())
				{
					FMSCLIENT_ERROR("picture is empty.");
					pTemp = TEXT("nopic");
				}
				else
				{
					strPicPath.Replace(_T("\\"), _T("//"));
					pTemp = strPicPath.GetBuffer();
				}
				FMSCLIENT_INFO("Picture 1 is:%s", fcT2A(strPicPath));
				htmlParser.SetAttributeValue(INDEX_PIC1, pTemp);
				FMSCLIENT_INFO("Picture 2 is empty!");
				pTemp = TEXT("nopic");
				htmlParser.SetAttributeValue(INDEX_PIC2, pTemp);
			}
			else if(m_PicFileArray.GetSize() >= 2) //2������ͼƬ
			{
				for (int i = 0; i < m_PicFileArray.GetSize() && i < 2; i++)
				{
					RECOED_FILE_INFO recInfo = m_PicFileArray.GetAt(i);
					CString strPicPath = recInfo.csPicPath;
					if (strPicPath.IsEmpty())
					{
						FMSCLIENT_ERROR("picture:%d is empty.", i);
						pTemp = TEXT("nopic");
					}
					else
					{
						strPicPath.Replace(_T("\\"), _T("//"));
						pTemp = strPicPath.GetBuffer();
					}
					FMSCLIENT_INFO("Picture %d is:%s", i, fcT2A(strPicPath));
					if (0 == i)
					{
						htmlParser.SetAttributeValue(INDEX_PIC1, pTemp);
					}
					else
					{
						htmlParser.SetAttributeValue(INDEX_PIC2, pTemp);
					}
				}
			}
		}

		memOutHtml.empty();
		htmlParser.outputHtml(memOutHtml);
		outFileName.empty();
		outFileName.appendText(fileName);
		memOutHtml.saveToFile((const char*)outFileName.getData());
	}
	else
	{
		FMSCLIENT_ERROR("Can't open print file:%s!", fileName);
		return;
	}

	//��ie�����
	HINSTANCE hinst = ShellExecute(NULL, _T("open"), _T("iexplore.exe"), strUrl, NULL, SW_SHOWMAXIMIZED);

	delete[] ptxtTemp;  

}


void CBusinessManageDlg::OnEnChangeEditProductcode()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CFDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// �û����룬�������״̬
	if (m_editProductCode.IsError())
	{
		m_editProductCode.SetErrorState(FALSE);
		SendMessage(WM_NCPAINT, 0, 0);
	}

	// ��ǰ������Ƿ�Ϊ��
	CString strText;
	if (m_editProductCode.IsPassWord())
	{
		m_editProductCode.GetPasswordText(strText);
	}
	else
	{
		GetWindowText(strText);
	}
	m_editProductCode.SetIsEmpty(strText.IsEmpty());

	//Invalidate();


	if ( (GetFocus() == GetDlgItem(IDC_EDIT_PRODUCTCODE)) )
	{
		m_listBoxProductCode.ShowWindow(SW_SHOW);
	}
	m_listBoxProductCode.ResetContent();
//	CString strText;
	GetDlgItem(IDC_EDIT_PRODUCTCODE)->GetWindowText(strText);
	if (strText == _T(""))
	{
		return;
	}

	m_editProductCode.SetTipText(strText); //������ʾ����

	//��Ʒ�б�
	cms_8120::ProductList productList;
	if (!CCMSMgr::SnglPtr()->GetProductList(productList, 0) || productList.product_size() == 0)
	{
		CString str = UTF8_A2MFCSTRW(productList.result_info());
		FMSCLIENT_ERROR("Get Product List failed!");
		return;
	}
	else
	{
		for (int i = 0; i < productList.product_size(); i++)
		{
			cms_8120::Product product = productList.product(i);
			CString strCode = UTF8_A2MFCSTRW(product.strcode());
			if (strCode.Find(strText) >= 0)
			{
				m_listBoxProductCode.AddString(strCode);
			}
		}
	}
}

//ѡ���Ʒ����ListBox���¼�
void CBusinessManageDlg::OnLbnSelchangeListboxProductcode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_listBoxProductCode.GetCurSel();
	if (LB_ERR == nIndex)
	{
		m_editProductCode.SetWindowText(_T(""));
		m_listBoxProductCode.ShowWindow(SW_HIDE);
		return;
	}
	CString strCode;
	m_listBoxProductCode.GetText(nIndex, strCode);
	m_editProductCode.SetWindowText(strCode);
	//��ѯ��Ʒ���ͺͲ�Ʒ����
	GetProductInfoByCode(strCode);
	m_listBoxProductCode.ShowWindow(SW_HIDE);
}

int CBusinessManageDlg::GetProductIdByName(CString& strProductName)
{
	// ���ԭ������
	int nProductId = 0;
	m_ArrayProductId.SetSize(0);
	m_ArrayProductPId.SetSize(0);
	m_ArrayProductName.SetSize(0);

	//m_comboProductName.ResetContent();
	m_comboProductName.SetState(FALSE);

	CString strProductType;
	m_comboProductType.GetText(strProductType);
	int nProductTypeId = GetProductTypeIdByName(strProductType);
	if (nProductTypeId < 0)
	{
		FMSCLIENT_ERROR("GetProductTypeIdByName Failed��");
		return -1;
	}

	//��Ʒ�б�
	cms_8120::ProductList productList;
	if (!CCMSMgr::SnglPtr()->GetProductList(productList, nProductTypeId) || productList.product_size() == 0)
	{
		FMSCLIENT_ERROR("LoadProduct Failed!");
		return -1;
	}
	else
	{
		for (int i = 0; i < productList.product_size(); i++)
		{
			cms_8120::Product product = productList.product(i);

			CString strId;
			strId.Format(_T("%d"), product.id());
			m_ArrayProductId.Add(strId);

			strId.Format(_T("%d"), product.pid());
			m_ArrayProductPId.Add(strId);

			CString strProductName = UTF8_A2MFCSTRW(product.strname());
			m_ArrayProductName.Add(strProductName);
		}
	}

	int nIndex = -1;
	for(nIndex = 0; nIndex < m_ArrayProductName.GetSize(); nIndex++)  //��ѯ��ƷID
	{
		if (strProductName == m_ArrayProductName.GetAt(nIndex))
		{
			return _ttoi(m_ArrayProductId.GetAt(nIndex));
			break;
		}
	}

	//��ѯ��ƷIDʧ��
	if (nIndex == m_ArrayProductName.GetSize())
	{
		FMSCLIENT_ERROR("Find Region Id Failed:%s", fcT2A(strProductName));
		return -1;
	}

	return -1;
}

int CBusinessManageDlg::GetProductTypeIdByName(CString& strProductTypeName)
{
	int nProductTypeId = -1;
	strProductTypeName.Trim();
	for (int i = 0; i < m_ArrayProductTypeName.GetSize(); i++)  //��������ID
	{
		if (strProductTypeName == m_ArrayProductTypeName.GetAt(i))
		{
			nProductTypeId = _ttoi(m_ArrayProductTypeId.GetAt(i));
			break;
		}
	}

	if (0 >= nProductTypeId) //û���ҵ�
	{
		FMSCLIENT_ERROR("Find ProductType Id Failed!");
		return -1;
	}

	return nProductTypeId;
}

BOOL CBusinessManageDlg::StartSound(void)
{
	if (NULL !=m_lpPreviewCtrlDlg)
	{
		m_lpPreviewCtrlDlg->StartSound();
	}
	return TRUE;
}

//�����л�
void CBusinessManageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//������հ����򣬴��ڻ�ȡ���㣬����ؼ�ʧȥ����
	//����Ʒ����༭���е����ݣ�����
	CWnd * pWndOld = this->SetFocus();
	if(GetDlgItem(IDC_EDIT_PRODUCTCODE) == pWndOld)
	{
		CString strText;
		GetDlgItem(IDC_EDIT_PRODUCTCODE)->GetWindowText(strText);
		//��Ʒ�б�
		cms_8120::ProductList productList;
		if (!CCMSMgr::SnglPtr()->GetProductList(productList, 0) || productList.product_size() == 0)
		{
			CString str = UTF8_A2MFCSTRW(productList.result_info());
			FMSCLIENT_ERROR("Get Product List failed!");
			return;
		}
		else
		{
			int i = 0;
			for (i = 0; i < productList.product_size(); i++)
			{
				cms_8120::Product product = productList.product(i);
				CString strCode = UTF8_A2MFCSTRW(product.strcode());
				if (strCode.Compare(strText) == 0)			//��ѯ������
				{
					m_editProductCode.SetWindowText(strCode);
					break;
				}
			}
			//û�в鵽�ò�Ʒ���룬��ձ༭������
			if (i == productList.product_size())
			{
				m_editProductCode.SetWindowText(_T(""));
			}
		}
		m_listBoxProductCode.ShowWindow(SW_HIDE);
	}
	else if (GetDlgItem(IDC_LISTBOX_PRODUCTCODE) == pWndOld)
	{
		m_editProductCode.SetWindowText(_T(""));
		m_listBoxProductCode.ShowWindow(SW_HIDE);
	}
	else if(GetDlgItem(IDC_EDIT_PURCHASEAMOUNT) == pWndOld)
	{
		m_editPurchaseAmount.SetErrorState(FALSE);
		CString strNum;
		m_editPurchaseAmount.GetWindowText(strNum);
		if (My_atoi(strNum) >= 2147483647)
		{
			m_editPurchaseAmount.SetErrorState(TRUE);
		}
	}


	CFDialog::OnLButtonDown(nFlags, point);
}

//��ʱ��
void CBusinessManageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (TIMER_ENABLE_BTN == nIDEvent)
	{
		//����
		CString strLocation;
		m_comboBusinessLocation.GetText(strLocation);
	
		//�ͻ�����
		CString strConsumerName;
		m_editCustomerName.GetWindowText(strConsumerName);

		//�ͻ��绰
		CString strConsumerPhone;
		m_editCustomerPhone.GetWindowText(strConsumerPhone);

		//֤������
		CString strIDType;
		m_comboIDType.GetWindowText(strIDType);

		//֤������
		CString strIDNum;
		m_editIDNumber.GetWindowText(strIDNum);

		//��Ʒ����
		CString strProductType;
		m_comboProductType.GetWindowText(strProductType);

		//��Ʒ����
		CString strProductName;
		m_comboProductName.GetWindowText(strProductName);

		//���л���
		CString strIssuer;
		m_editIssuer.GetWindowText(strIssuer);

		//��Ʒ����
		CString strProductCode;
		m_editProductCode.GetWindowText(strProductCode);

		//������
		CString strPurchaseAmount;
		m_editPurchaseAmount.GetWindowText(strPurchaseAmount);

		//���������������һ��Ϊ�գ��ûҰ�ť
		if (strLocation.IsEmpty() 
			|| strConsumerName.IsEmpty()
			|| strIDType.IsEmpty()
			|| strIDNum.IsEmpty()
			|| strProductType.IsEmpty()
			|| strProductName.IsEmpty()
			|| strIssuer.IsEmpty()
			|| strProductCode.IsEmpty()
			|| m_RecFileArray.IsEmpty()  //¼���б�
			)
		{
			//���ָ������ȫ��Ϊ��
			if (strConsumerName.IsEmpty()
				&& strIDNum.IsEmpty()
				&& strProductType.IsEmpty()
				&& strProductName.IsEmpty()
				&& strIssuer.IsEmpty()
				&& strProductCode.IsEmpty()
				&& m_RecFileArray.IsEmpty()  //¼���б�
				&& strConsumerPhone.IsEmpty()
				&& strPurchaseAmount.IsEmpty()
				)
			{
				m_btnReset.EnableWindow(FALSE);
			}
			else
			{
				if (!m_bIsRecording)	//������¼��
				{
					m_btnReset.EnableWindow(TRUE);
				}
			}
			m_btnPrint.EnableWindow(FALSE);
			m_btnSubmit.EnableWindow(FALSE);
		}
		else  //�����������
		{
			if (!m_bIsRecording)	//������¼��
			{
				m_btnPrint.EnableWindow(TRUE);
				m_btnSubmit.EnableWindow(TRUE);
				m_btnReset.EnableWindow(TRUE);
			}
		}
	}
	else if (TIMER_ENABLE_WORD == nIDEvent)
	{
		//��Ʒ����
		CString strProductName;
		m_comboProductName.GetWindowText(strProductName);

		if (strProductName.IsEmpty() || strProductName == _T("���޿�ѡ��Ʒ"))
		{
			m_btnWords.EnableWindow(FALSE);
		}
		else
		{
			m_btnWords.EnableWindow(TRUE);
		}
	}

	CFDialog::OnTimer(nIDEvent);
}

void CBusinessManageDlg::OnCbnDropdownComboBusinessid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_comboBusinessId.ShowDropDown(FALSE);
	if (NULL == m_pHistoryListDlg)
	{
		try
		{
			m_pHistoryListDlg = new CHistoryListDlg(this);
		}
		catch (...)
		{
			return;
		}
	}
	if (IDOK == m_pHistoryListDlg->DoModal())
	{
		m_bIsHistory = true;

		cms_8120::HisClientInput hisClientInput = theApp.m_hisClientInput;
		m_nBusinessIndex = hisClientInput.clientid();  //ҵ�񵥵�id�ţ��޸���
		m_nProductId = hisClientInput.productid();		//��Ʒid
		//m_editBusinessId.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.nproid()));	//ҵ�񵥺�
		m_comboBusinessId.SetText(UTF8_A2MFCSTRW(hisClientInput.nproid()));
		m_comboBusinessLocation.SetText(UTF8_A2MFCSTRW(hisClientInput.inputregionname()));
        m_comboBusinessLocation.SetState(FALSE);
		m_editCustomerName.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.name()));	//�ͻ�����
		m_editCustomerPhone.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strcreditphone())); //�ͻ��绰
		int nIDType = hisClientInput.creditid();
		CString strIDTypeName;
		if (m_IDMap.Lookup(nIDType, strIDTypeName))
		{
			int nIndex = m_comboIDType.FindStringExact(-1, strIDTypeName);
			if (nIndex >= 0)
			{
				m_comboIDType.SetCurSel(nIndex);
                m_comboIDType.SetState(FALSE);
			}
		}
		m_editIDNumber.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.creditcode()));

		m_comboProductType.ResetContent();
		m_comboProductType.SetText(UTF8_A2MFCSTRW(hisClientInput.producttypename()));
        m_comboProductType.SetState(FALSE);
        CRect rcRect;
        m_comboProductType.GetWindowRect(rcRect);
        ScreenToClient(rcRect);
        InvalidateRect(rcRect);

		m_comboProductName.ResetContent();
		m_comboProductName.SetText(UTF8_A2MFCSTRW(hisClientInput.productname()));
        m_comboProductName.SetState(FALSE);
  

		m_editProductCode.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.productcode()));
		m_editIssuer.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strgrade()));
		m_editPurchaseAmount.SetWindowText(UTF8_A2MFCSTRW(hisClientInput.strbuymoney()));		

// 		//��������
// 		m_treeRegion = &(m_comboBusinessLocation.GetTreeCtrl());
// 		m_treeRegion->DeleteAllItems();
// 		m_treeRegion->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
// 		m_comboBusinessLocation.SetReadOnly(TRUE);
// 		LoadRegion(TVI_ROOT, 0);
// 
// 		//���ز�Ʒ����
// 		m_comboProductType.SetActrueParent(this);	//ʵ�ʵĸ����ڣ�������Ϣ��
// 		m_treeProductType = &(m_comboProductType.GetTreeCtrl());
// 		m_treeProductType->DeleteAllItems();
// 		m_treeProductType->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
// 		m_comboProductType.SetReadOnly(TRUE);
// 		LoadProductType(TVI_ROOT, 0);
	}

	m_comboBusinessId.m_bPressBtn = FALSE;
}

void CBusinessManageDlg::SetEnable(BOOL bEnable)
{
	m_comboBusinessId.EnableWindow(bEnable);
	m_btnWords.EnableWindow(bEnable);
	m_comboProductName.EnableWindow(bEnable);
	m_btnPrint.EnableWindow(bEnable);
	m_btnSubmit.EnableWindow(bEnable);
	m_btnReset.EnableWindow(bEnable);
}