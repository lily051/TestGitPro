// BusinessInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "BusinessInfoDlg.h"


// CBusinessInfoDlg 对话框

IMPLEMENT_DYNAMIC(CBusinessInfoDlg, CFDialog)

CBusinessInfoDlg::CBusinessInfoDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CBusinessInfoDlg::IDD, pParent)
	, m_strBusinessId(_T(""))
	, m_pPlaybackDlg(NULL)
	, m_treeProductType(NULL)
	, m_hRoot(NULL)
	, m_treeRegion(NULL)
	, m_nRegionId(-1)
	, m_pRegionTree(NULL)
	, m_nProductId(-1)
	,m_nLastHover(-1)
{
	m_ToolTips.Create(this); 
	m_ToolTips.SetTipBkColor(RGB(255, 255, 225));
	m_ToolTips.SetDelayTime(1000);
}

CBusinessInfoDlg::~CBusinessInfoDlg()
{
	if (NULL != m_pPlaybackDlg)
	{
		delete m_pPlaybackDlg;
		m_pPlaybackDlg = NULL;
	}
}

void CBusinessInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BUSINESSINFO, m_btnBusinessInfo);
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
	DDX_Control(pDX, IDC_BTN_PRODUCTINFO, m_btnProductInfo);
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
	DDX_Control(pDX, IDC_BTN_RECORDINFO, m_btnRecordInfo);
	DDX_Control(pDX, IDC_STATIC_RECORDINFO, m_staticRecordInfo);
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnAddRecord);
	DDX_Control(pDX, IDC_LIST_RECORDS, m_listRecords);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_BUSINESSID, m_strBusinessId);
}


BEGIN_MESSAGE_MAP(CBusinessInfoDlg, CFDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CBusinessInfoDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CBusinessInfoDlg::OnBnClickedBtnUpload)
	ON_CBN_DROPDOWN(IDC_COMBO_PRODUCTNAME, &CBusinessInfoDlg::OnCbnDropdownComboProductname)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCTNAME, &CBusinessInfoDlg::OnCbnSelchangeComboProductname)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CBusinessInfoDlg 消息处理程序

BOOL CBusinessInfoDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();

	MoveCtrl();

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBusinessInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}

void CBusinessInfoDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));

	MoveWindow(0, 0, 480, 620);
	CenterWindow();

	//业务信息
	m_btnBusinessInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticBusinessInfo.SetTextFont(YAHEI_18_B);
	m_staticBusinessId.SetTextFont(YAHEI_18_N);
	m_editBusinessId.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled,YAHEI_18_N);
	m_editBusinessId.SetReadOnly(TRUE);
	m_comboBusinessLocation.SetItemHeight(0, 0);
	m_staticBusinessLocation.SetTextFont(YAHEI_18_N);
	m_comboBusinessLocation.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_comboBusinessLocation.SetObjectName(_T("combo1"));
	m_comboBusinessLocation.ShowDropDown(FALSE);
	m_comboBusinessLocation.SetActrueParent(GetDlgItem(IDC_COMBO_BUSINESSLOCATION));	//实际的父窗口（用来屏蔽区域下拉选择消息）
	m_staticCustomerName.SetTextFont(YAHEI_18_N);
	m_editCustomerName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editCustomerName.LimitText(32);
	m_staticCustomerPhone.SetTextFont(YAHEI_18_N);
	m_editCustomerPhone.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editCustomerPhone.LimitText(16);
	m_staticIDType.SetTextFont(YAHEI_18_N);
	m_comboIDType.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_staticIDNumber.SetTextFont(YAHEI_18_N);
	m_editIDNumber.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editIDNumber.LimitText(32);
	m_comboIDType.SetObjectName(_T("combo1"));
	m_comboIDType.SetReadOnly(TRUE);

	//产品信息
	m_btnProductInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticProductInfo.SetTextFont(YAHEI_18_B);
	m_staticProductType.SetTextFont(YAHEI_18_N);
	m_comboProductType.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_staticProductName.SetTextFont(YAHEI_18_N);
	m_comboProductName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_staticProductCode.SetTextFont(YAHEI_18_N);
	m_editProductCode.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editProductCode.LimitText(32);
	m_staticIssuer.SetTextFont(YAHEI_18_N);
	m_editIssuer.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editIssuer.SetReadOnly(TRUE);
	m_staticPurchaseAmount.SetTextFont(YAHEI_18_N);
	m_editPurchaseAmount.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editPurchaseAmount.LimitText(10);
	m_editPurchaseAmount.SetDefaultText(_T("单位：元"));
	m_comboProductType.SetObjectName(_T("combo1"));
	m_comboProductName.SetObjectName(_T("combo1"));
	m_comboProductName.SetReadOnly(TRUE);

	//录像信息
	m_btnRecordInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticRecordInfo.SetTextFont(YAHEI_18_B);
	m_btnAddRecord.SetHoverCursor(LoadCursor(NULL, IDC_HAND));
	m_btnAddRecord.SetTextColor(FUi::Normal, RGB(0x3F, 0x85, 0xE4));
	m_btnAddRecord.ShowWindow(SW_HIDE);

	m_listRecords.SetToolTipEnable(true);
	m_listRecords.SetHeadHeight(30);  //设置表头高度(不显示)
	m_listRecords.SetHeadBgColor(RGB(158, 167, 178));
	m_listRecords.SetHeight(25);
	m_listRecords.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
	m_listRecords.InsertColumn(1, _T("文件路径"), LVCFMT_LEFT, 160);
	m_listRecords.InsertColumn(2, _T("上传状态"), LVCFMT_LEFT, 80);
	m_listRecords.InsertColumn(3, _T("状态说明"), LVCFMT_LEFT, 80);
	m_listRecords.InsertColumn(4, _T("操作"), LVCFMT_LEFT, 45);
	//ListCtrl item项操作图标
	m_listRecords.SetButton(4, 1, _T("play"));
	m_listRecords.SetButtonToolTip(1, _T("查看"));
	m_listRecords.SetHeadLock(-1, TRUE);	//设置表头锁定

	//确定按钮
	m_btnSave.SetObjectName(_T("btn"));
	m_btnSave.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, YAHEI_18_N);
	m_btnSave.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnSave.SetTextAlign(DT_CENTER);

	//取消按钮
	m_btnCancel.SetObjectName(_T("cancel"));
	m_btnCancel.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, YAHEI_18_N);
	m_btnCancel.SetTextAlign(DT_CENTER);


}

void CBusinessInfoDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	/********************业务信息Begin********************/
	//业务信息
	CRect rcBtnBusinsessInfo(rcClient);
	rcBtnBusinsessInfo.left = rcClient.left + 10;
	rcBtnBusinsessInfo.top = rcClient.top + 23;
	rcBtnBusinsessInfo.right = rcBtnBusinsessInfo.left + 12;
	rcBtnBusinsessInfo.bottom = rcBtnBusinsessInfo.top + 10;
	m_btnBusinessInfo.MoveWindow(rcBtnBusinsessInfo);

	CRect rcStaticBusinessInfo(rcClient);
	rcStaticBusinessInfo.left = rcBtnBusinsessInfo.right + 6;
	rcStaticBusinessInfo.top = rcClient.top + 20;
	rcStaticBusinessInfo.right = rcStaticBusinessInfo.left + 80;
	rcStaticBusinessInfo.bottom = rcStaticBusinessInfo.top + 26;
	m_staticBusinessInfo.MoveWindow(rcStaticBusinessInfo);

	//业务单号(40+static+edit+btn=400)
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
	m_editBusinessId.MoveWindow(rcEditBusinessId);

	//办理网点(40+static+combobox=400)
	CRect rcStaticBusinessLocation(rcStaticBusinessId);
	rcStaticBusinessLocation.top = rcStaticBusinessId.bottom + 14 + 8;
	rcStaticBusinessLocation.bottom = rcStaticBusinessLocation.top + 26;
	m_staticBusinessLocation.MoveWindow(rcStaticBusinessLocation);

	CRect rcComboBusinessLocation(rcEditBusinessId);
	rcComboBusinessLocation.top = rcEditBusinessId.bottom + 14;
	rcComboBusinessLocation.right = rcEditBusinessId.right;
	rcComboBusinessLocation.bottom = rcComboBusinessLocation.top + 26;
	m_comboBusinessLocation.MoveWindow(rcComboBusinessLocation);
	m_comboBusinessLocation.SetItemHeight(-1, 22);  //需要先MoveWindow,再SetItemHeight

	//客户姓名
	CRect rcStaticCustomerName(rcStaticBusinessLocation);
	rcStaticCustomerName.top = rcComboBusinessLocation.bottom + 14 + 8;
	rcStaticCustomerName.bottom = rcStaticCustomerName.top + 28;
	m_staticCustomerName.MoveWindow(rcStaticCustomerName);

	CRect rcEditCustomerName(rcComboBusinessLocation);
	rcEditCustomerName.top = rcComboBusinessLocation.bottom + 14 + 2;
	rcEditCustomerName.right = rcEditCustomerName.left + 135;
	rcEditCustomerName.bottom = rcEditCustomerName.top + 28;
	m_editCustomerName.MoveWindow(rcEditCustomerName);

	//联系方式
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

	//证件类型
	CRect rcStaticIDType(rcStaticCustomerName);
	rcStaticIDType.top = rcStaticIDType.bottom + 14 + 2;
	rcStaticIDType.bottom = rcStaticIDType.top + 26;
	m_staticIDType.MoveWindow(rcStaticIDType);

	CRect rcComboIDType(rcEditCustomerName);
	rcComboIDType.top = rcEditPhoneNum.bottom + 14;
	rcComboIDType.bottom = rcComboIDType.top + 26;
	m_comboIDType.MoveWindow(rcComboIDType);
	m_comboIDType.SetItemHeight(-1, 22);  //需要先MoveWindow,再SetItemHeight

	//证件号码
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
	/********************业务信息End********************/

	/********************产品信息Begin********************/
	//产品信息
	CRect rcBtnProductInfo(rcClient);
	rcBtnProductInfo.left = rcClient.left + 10;
	rcBtnProductInfo.top = rcClient.top + 218;
	rcBtnProductInfo.right = rcBtnProductInfo.left + 12;
	rcBtnProductInfo.bottom = rcBtnProductInfo.top + 10;
	m_btnProductInfo.MoveWindow(rcBtnProductInfo);

	CRect rcStaticProductInfo(rcClient);
	rcStaticProductInfo.left = rcBtnProductInfo.right + 6;
	rcStaticProductInfo.top = rcClient.top + 215;
	rcStaticProductInfo.right = rcStaticProductInfo.left + 50;
	rcStaticProductInfo.bottom = rcStaticProductInfo.top + 26;
	m_staticProductInfo.MoveWindow(rcStaticProductInfo);

	//产品类型
	CRect rcStaticProductType(rcClient);
	rcStaticProductType.left = rcClient.left + 40;
	rcStaticProductType.top = rcStaticProductInfo.bottom + 6;
	rcStaticProductType.right = rcStaticProductType.left + 60;
	rcStaticProductType.bottom = rcStaticProductType.top + 20;
	m_staticProductType.MoveWindow(rcStaticProductType);

	CRect rcComboProductType(rcClient);
	rcComboProductType.left = rcStaticProductType.right;
	rcComboProductType.top = rcStaticProductInfo.bottom;
	rcComboProductType.right = rcComboProductType.left + 135;
	rcComboProductType.bottom = rcComboProductType.top + 26;
	m_comboProductType.MoveWindow(rcComboProductType);
	m_comboProductType.SetItemHeight(-1, 22);  //需要先MoveWindow,再SetItemHeight

	//产品名称
	CRect rcStaticProductName(rcStaticProductType);
	rcStaticProductName.left = rcComboProductType.right + 30;
	rcStaticProductName.right = rcStaticProductName.left + 60;
	m_staticProductName.MoveWindow(rcStaticProductName);

	CRect rcComboProductName(rcStaticProductName);
	rcComboProductName.left = rcComboProductName.right;
	rcComboProductName.top = rcComboProductName.top - 6;
	rcComboProductName.right = rcComboProductName.left + 135;
	m_comboProductName.MoveWindow(rcComboProductName);
	m_comboProductName.SetItemHeight(-1, 22);  //需要先MoveWindow,再SetItemHeight

// 	//产品编码
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
// 	//发行机构
// 	CRect rcStaticIssuer(rcStaticProductName);
// 	rcStaticIssuer.top = rcStaticIssuer.bottom + 20;
// 	rcStaticIssuer.bottom = rcStaticIssuer.top + 26;
// 	m_staticIssuer.MoveWindow(rcStaticIssuer);
// 
// 	CRect rcEditIssuer(rcComboProductName);
// 	rcEditIssuer.top = rcEditIssuer.bottom + 14 + 2;
// 	rcEditIssuer.bottom = rcEditIssuer.top + 28;
// 	m_editIssuer.MoveWindow(rcEditIssuer);

	//发行机构
	CRect rcStaticProductCode(rcStaticProductType);
	rcStaticProductCode.top = rcStaticProductCode.bottom + 22;
	rcStaticProductCode.bottom = rcStaticProductCode.top + 26;
	m_staticIssuer.MoveWindow(rcStaticProductCode);

	CRect rcEditProductCode(rcComboProductType);
	rcEditProductCode.top = rcEditProductCode.bottom + 14 + 2;
	rcEditProductCode.bottom = rcEditProductCode.top + 28;
	m_editIssuer.MoveWindow(rcEditProductCode);

	//产品编码
	CRect rcStaticIssuer(rcStaticProductName);
	rcStaticIssuer.top = rcStaticIssuer.bottom + 20;
	rcStaticIssuer.bottom = rcStaticIssuer.top + 26;
	m_staticProductCode.MoveWindow(rcStaticIssuer);

	CRect rcEditIssuer(rcComboProductName);
	rcEditIssuer.top = rcEditIssuer.bottom + 14 + 2;
	rcEditIssuer.bottom = rcEditIssuer.top + 28;
	m_editProductCode.MoveWindow(rcEditIssuer);

	//购买金额
	CRect rcStaticPurchaseAmount(rcStaticProductCode);
	rcStaticPurchaseAmount.top = rcStaticPurchaseAmount.bottom + 16;
	rcStaticPurchaseAmount.bottom = rcStaticPurchaseAmount.top + 26;
	m_staticPurchaseAmount.MoveWindow(rcStaticPurchaseAmount);

	CRect rcEditPurchaseAmount(rcEditProductCode);
	rcEditPurchaseAmount.top = rcEditPurchaseAmount.bottom + 14;
	rcEditPurchaseAmount.bottom = rcEditPurchaseAmount.top + 28;
	m_editPurchaseAmount.MoveWindow(rcEditPurchaseAmount);
	/********************产品信息End********************/

	/********************录像信息Begin********************/
	//录像信息
	CRect rcBtnRecordInfo(rcClient);
	rcBtnRecordInfo.left = rcClient.left + 10;
	rcBtnRecordInfo.top = rcClient.top + 368;
	rcBtnRecordInfo.right = rcBtnRecordInfo.left + 12;
	rcBtnRecordInfo.bottom = rcBtnRecordInfo.top + 10;
	m_btnRecordInfo.MoveWindow(rcBtnRecordInfo);

	CRect rcStaticRecordInfo(rcClient);
	rcStaticRecordInfo.left = rcBtnProductInfo.right + 6;
	rcStaticRecordInfo.top = rcClient.top + 365;
	rcStaticRecordInfo.right = rcStaticRecordInfo.left + 50;
	rcStaticRecordInfo.bottom = rcStaticRecordInfo.top + 20;
	m_staticRecordInfo.MoveWindow(rcStaticRecordInfo);

	//上传本地视频
	CRect rcRecUpload(rcStaticRecordInfo);
	rcRecUpload.left = rcClient.right - 120;
	rcRecUpload.right = rcClient.right - 28;
	m_btnAddRecord.MoveWindow(rcRecUpload);

	CRect rcRecordsList(rcClient);
	rcRecordsList.left = rcStaticPurchaseAmount.left;
	rcRecordsList.top = rcStaticRecordInfo.bottom + 5;
	rcRecordsList.right = rcEditIssuer.right;
	rcRecordsList.bottom = rcRecordsList.top + 145;
	m_listRecords.MoveWindow(rcRecordsList);
	/********************录像信息End********************/

	CRect rcBtnSave(rcClient);
	rcBtnSave.left = rcBtnSave.left + 270;
	rcBtnSave.top = rcBtnSave.bottom - 40;
	rcBtnSave.right = rcBtnSave.left + 90;
	rcBtnSave.bottom = rcBtnSave.top + 30;
	m_btnSave.MoveWindow(rcBtnSave);

	CRect rcBtnCancel(rcBtnSave);
	rcBtnCancel.left = rcBtnCancel.right + 10;
	rcBtnCancel.right = rcBtnCancel.left + 90;
	m_btnCancel.MoveWindow(rcBtnCancel);	
}

void CBusinessInfoDlg::InitData(void)
{
	LoadIDType();

	//业务单号
	m_editBusinessId.SetWindowText( UTF8_A2MFCSTRW(m_businessInfo.nproid()));
	//区域
	m_comboBusinessLocation.SetText(UTF8_A2MFCSTRW(m_businessInfo.inputregionname()));
	//客户姓名
	m_editCustomerName.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.custname()));
	//联系方式
	m_editCustomerPhone.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.strcreditphone()));
	//证件类型
	CString strIDType;
	if (m_IDMap.Lookup(m_businessInfo.creditid(), strIDType))
	{
		int nIndex = m_comboIDType.FindString(-1, strIDType);
		if (nIndex >= 0)
		{
			m_comboIDType.SetCurSel(nIndex);
		}
	}
	//证件号码
	m_editIDNumber.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.creditcode()));
	//产品类型
	m_comboProductType.SetText(UTF8_A2MFCSTRW(m_businessInfo.producttypename()));
	//产品名称
// 	m_comboProductName.AddString(UTF8_A2MFCSTRW(m_businessInfo.productname()));
// 	m_comboProductName.SetCurSel(0);
	m_comboProductName.SetText(UTF8_A2MFCSTRW(m_businessInfo.productname()));
	//产品名称Id
	m_nProductId = m_businessInfo.productid();
	//产品编码
	m_editProductCode.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.productcode()));
	//发行机构
	m_editIssuer.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.strgrade()));
	//购买金额
	m_editPurchaseAmount.SetWindowText(UTF8_A2MFCSTRW(m_businessInfo.strbuymoney()));
	//录像列表
	for (int i = m_businessInfo.recordsdetailinfo_size() - 1; i >= 0; i--)
	{
		int nIndex = m_listRecords.GetItemCount();
		CString strIndex;
		strIndex.Format(_T("%d"), nIndex + 1);

		m_listRecords.InsertItem(nIndex, strIndex);
		m_listRecords.SetItemText(nIndex, 1, UTF8_A2MFCSTRW(m_businessInfo.recordsdetailinfo(i).strurl()));
		m_listRecords.SetItemText(nIndex, 2, UTF8_A2MFCSTRW(m_businessInfo.recordsdetailinfo(i).strrecordstatus()));
		m_listRecords.SetItemText(nIndex, 3, UTF8_A2MFCSTRW(m_businessInfo.recordsdetailinfo(i).errormsg()));
	}

	//加载区域
	m_treeRegion = &(m_comboBusinessLocation.GetTreeCtrl());
	m_treeRegion->DeleteAllItems();
	m_treeRegion->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
	m_comboBusinessLocation.SetReadOnly(TRUE);
	LoadRegion(TVI_ROOT, 0);

	//加载产品类型
	m_comboProductType.SetActrueParent(this);
	m_treeProductType = &(m_comboProductType.GetTreeCtrl());
	m_treeProductType->DeleteAllItems();
	m_treeProductType->ModifyStyle(NULL,  TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT);
	m_comboProductType.SetReadOnly(TRUE);
	LoadProductType(TVI_ROOT, 0);

	if (m_businessInfo.lockclient() == 2) //退回
	{
		SetEditEnable(TRUE);
	}
	else //未审核或通过
	{
		SetEditEnable(FALSE);
	}

}

void CBusinessInfoDlg::SetDialogData(const cms_8120::RecordsInfo& recordInfo)
{
	m_businessInfo = recordInfo;
}

//设置页面是否可编辑
void CBusinessInfoDlg::SetEditEnable(BOOL bEnable)
{
	m_comboBusinessLocation.EnableWindow(bEnable);
	m_editCustomerName.EnableWindow(bEnable);
	m_editCustomerPhone.EnableWindow(bEnable);
	m_comboIDType.EnableWindow(bEnable);
	m_editIDNumber.EnableWindow(bEnable);
	m_comboProductType.EnableWindow(bEnable);
	m_comboProductName.EnableWindow(bEnable);
	m_editProductCode.EnableWindow(bEnable);
	m_editIssuer.EnableWindow(bEnable);
	m_editPurchaseAmount.EnableWindow(bEnable);
}

//保存
void CBusinessInfoDlg::OnBnClickedBtnSave()
{
	UpdateData(TRUE);

	if (!GetRegionId())
	{
		m_comboBusinessLocation.SetState(TRUE);
		return;
	}
	else
	{
		m_comboBusinessLocation.SetState(FALSE);
	}

	// 客户姓名
	CString strCustomerName;
	m_editCustomerName.GetWindowText(strCustomerName);
	if (strCustomerName == _T(""))
	{
		m_editCustomerName.SetErrorState(TRUE);
		return;
	}

	//客户电话
	CString strPhoneNum;
	m_editCustomerPhone.GetWindowText(strPhoneNum);

	//证件号
	CString strIDNum;
	m_editIDNumber.GetWindowText(strIDNum);
	if (strIDNum == _T(""))
	{
		m_editIDNumber.SetErrorState(TRUE);
		return;
	}

	//证件类型
	CString strIDName;
	m_comboIDType.GetWindowText(strIDName);
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

	if (nIDType < 0)
	{
		FMessageBox::Show(_T("证件类型错误"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}

	//产品类型
	CString strProductType;
	m_comboProductType.GetWindowText(strProductType);
	if(strProductType == _T(""))
	{
		m_comboProductType.DisplayTree(strProductType);
		//m_comboProductType.SetState(TRUE);
		return;
	}

	//产品名称
	CString strProductName;
	m_comboProductName.GetWindowText(strProductName);
	if (strProductName == _T("") || strProductName == _T("暂无可选产品"))
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

	//发行机构
	CString strIssuer;
	m_editIssuer.GetWindowText(strIssuer);
	if (strIssuer == _T(""))
	{
		m_editIssuer.SetErrorState(TRUE);
		m_editIssuer.SetFocus();
		return;
	}

	//产品编码
	CString strCode;
	m_editProductCode.GetWindowText(strCode);
	if (strCode == _T(""))
	{
		m_editProductCode.SetErrorState(TRUE);
		return;
	}

	//购买金额
	CString strBuyNum;
	m_editPurchaseAmount.GetWindowText(strBuyNum);
	//购买金额校验
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
	if (nTime > 1)	//多个小数点
	{
		m_editPurchaseAmount.SetErrorState(TRUE);
		return;
	}

	//业务单信息
	cms_8120::CSClientInput clientInput;
	clientInput.set_id(m_businessInfo.id());  //更新用业务记录的id(主键)
	clientInput.set_inputregionid(m_nRegionId);
	clientInput.set_inputuser(theApp.m_nUserId);
	clientInput.set_nproid(UTF8_MFCSTRW2A(m_strBusinessId));
	clientInput.set_name(UTF8_MFCSTRW2A(strCustomerName));
	clientInput.set_creditcode(UTF8_MFCSTRW2A(strIDNum));
	clientInput.set_creditid(nIDType);
	clientInput.set_productid(m_nProductId);
	clientInput.set_strbuymoney(UTF8_MFCSTRW2A(strBuyNum));
	clientInput.set_strcreditphone(UTF8_MFCSTRW2A(strPhoneNum));

	CArray<cms_8120::CSRecords> recordsArray;
	cms_8120::RspCsClientRecord rspMsg;
	if (!CCMSMgr::SnglPtr()->UpdateBusiness(clientInput, recordsArray, rspMsg) || rspMsg.result() != 0)
	{
		CString str = UTF8_A2MFCSTRW(rspMsg.result_info());
		FMSCLIENT_ERROR("UpdateBusiness Failed:%s", fcT2A(str));
        FMessageBox::Show(str, _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		CLIENT_INFO("UpdateBusiness Succeed!%d", m_businessInfo.id());
		FMessageBox::Show(_T("更新成功！"), _T("提示"), MB_OK);
		this->OnOK();
	}

}

void CBusinessInfoDlg::LoadIDType(void)
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
}
void CBusinessInfoDlg::OnBnClickedBtnUpload()
{
	m_importFileArray.SetSize(0);
	CString strFileFilter = _T("视频文件(*.mp4;*.ps)|*.mp4;*.ps||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFileFilter);
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
		recordInfo.lFileSize = 500;

		if (strFileName.Find(_T(".mp4")) > 0)
		{
			InsertRec(recordInfo);
		}
		else
		{
			//InsertPic(recordInfo);
		}
	}
}

BOOL CBusinessInfoDlg::InsertRec(const RECOED_FILE_INFO &recordFile)
{
	//序号
	int nIndex = m_listRecords.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	m_listRecords.InsertItem(nIndex, strIndex);
	//文件名
	//int nPos = recordFile.strFileName.ReverseFind('\\');
	//CString strFileName = recordFile.strFileName.Mid(recordFile.strFileName.ReverseFind('\\') + 1);
	m_listRecords.SetItemText(nIndex, 1, recordFile.strFileName);
	//录像时长
// 	CString strFileDuration;
// 	int nFileDuration = recordFile.lEndTime - recordFile.lStartTime;
// 	if (0 == nFileDuration)
// 	{
// 		strFileDuration = _T("--");
// 		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
// 	}
// 	else
// 	{
// 		strFileDuration.Format(_T("%02d:%02d:%02d"), nFileDuration / 3600, nFileDuration % 3600 / 60, nFileDuration % 3600 % 60);		
// 		m_listRecords.SetItemText(nIndex, 2, strFileDuration);
// 	}

	CString strUploadStat = _T("等待上传");
	m_listRecords.SetItemText(nIndex, 2, strUploadStat);

	CString strComment = _T("等待上传");
	m_listRecords.SetItemText(nIndex, 3, strComment);

	//文件大小
// 	CString strFileSize;
// 	strFileSize.Format(_T("%.2fM"),  (double)recordFile.lFileSize / 1024 / 1024);
// 	/*double dFileSize = (double)recordFile.lFileSize / 1024 / 1024;*/
// 	m_listRecords.SetItemText(nIndex, 3, strFileSize);

	//添加到录像列表
	//m_RecFileArray.Add(recordFile);

	return TRUE;
}

BOOL CBusinessInfoDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MSG_MULTIBTN_CLICK == wParam)  
	{
		STRU_MULTIBTN_INFO * multiBtnInfo = (STRU_MULTIBTN_INFO*)lParam;
		CString strFileName = m_listRecords.GetItemText(multiBtnInfo->nItem, 1);
		int nItem = multiBtnInfo->nItem;
		if (-1 == nItem)
		{
			FMSCLIENT_ERROR("未选中任何项!\n");
			//return FALSE;
		}

		//回放
		if (multiBtnInfo->nNumber == 0)
		{
			
			if (NULL == m_pPlaybackDlg)
			{
				try
				{
					m_pPlaybackDlg = new CPlaybackCtrlDlg(this);
				}
				catch (...)
				{
					return FALSE;
				}
				if (NULL != m_pPlaybackDlg)
				{
					m_pPlaybackDlg->Create(IDD_DLG_PLAYBACK_CTRL, this);
				}
			}
			if (NULL != m_pPlaybackDlg)
			{
				m_pPlaybackDlg->ShowWindow(SW_SHOW);
				m_pPlaybackDlg->StartPlayBack(0, strFileName);
			}
		}	
	}
	else if (MSG_COMBO_SELCHANGE == wParam)  //产品类型选择改变消息
	{
		m_comboProductName.ResetContent();
		m_editProductCode.SetWindowText(_T(""));
		m_editIssuer.SetWindowText(_T(""));
	}

	return CFDialog::OnCommand(wParam, lParam);
}

void CBusinessInfoDlg::LoadRegion(HTREEITEM hParentItem, int nParentId)
{
	//网点列表
	RegionList regionList;
	if (!CCMSMgr::SnglPtr()->GetRegionList(nParentId, regionList) || regionList.region_size() == 0)
	{
		if (TVI_ROOT == hParentItem && nParentId == 0)
		{
			HTREEITEM hItem = m_treeRegion->InsertItem(_T("控制单元"), 0);
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
			m_hRoot= m_treeRegion->InsertItem(_T("控制单元"), 0);
		}

		//插入节点
		InsertNode(TREE_DLG_TYPE_REGION, m_hRoot, 1);
		ExpandBranch(m_hRoot, *m_treeRegion, TRUE);
	}

}

void CBusinessInfoDlg::InsertNode(int nType, HTREEITEM hParantNode, int nPId)
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
					if (strRegionName.IsEmpty())
					{
						m_treeRegion->SelectItem(m_treeRegion->GetRootItem());
					}
					if (strRegionName == m_ArrayRegionName.GetAt(i))
					{
						m_treeRegion->SelectItem(hItem);
					}
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

void CBusinessInfoDlg::ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand /*=TRUE*/)  	  
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

//获取产品类型
void CBusinessInfoDlg::LoadProductType(HTREEITEM hParentItem, int nParentId)
{
	//产品类型列表
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
			m_hRoot = m_treeProductType->InsertItem(_T("产品类型列表"), 0);
		}

		//插入节点
		InsertNode(TREE_DLG_TYPE_PRODUCTTYPE, m_hRoot, 1);

		//展开
		ExpandBranch(m_hRoot, *m_treeProductType, TRUE);
	}
}

//加载产品名称
void CBusinessInfoDlg::OnCbnDropdownComboProductname()
{
	// 清空原有数据
	m_ArrayProductId.SetSize(0);
	m_ArrayProductPId.SetSize(0);
	m_ArrayProductName.SetSize(0);

	CString strTempName;
	m_comboProductName.GetWindowText(strTempName);

	m_comboProductName.SetState(FALSE);

	CString strProductType;
	m_comboProductType.GetText(strProductType);
	if (strProductType == _T(""))		//未选择产品类型时点击产品名称，类型标红
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
		FMSCLIENT_ERROR("GetProductTypeIdByName Failed！");
		return;
	}

	//加载产品
	LoadProduct(strTempName, TVI_ROOT, nProductTypeId);
}

void CBusinessInfoDlg::OnCbnSelchangeComboProductname()
{
	// TODO: 在此添加控件通知处理程序代码
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
		FMSCLIENT_ERROR("Get Product Id Failed:%s", strProductName);
		return;
	}


	//获取产品信息
	cms_8120::ProductInfo productInfo;
	if (!CCMSMgr::SnglPtr()->GetProductInfo(productInfo, nProductId) || productInfo.result() != 0)
	{
		FMSCLIENT_ERROR("GetProductInfo Failed!%s", fcT2A(UTF8_A2MFCSTRW(productInfo.result_info())));
		return;
	}
	else
	{
		cms_8120::Product product = productInfo.product();
		CString strCode = UTF8_A2MFCSTRW(product.strcode());   //产品编码
		CString strGrade = UTF8_A2MFCSTRW(product.strgrade()); //发行机构
		m_editProductCode.SetWindowText(strCode);
		m_editIssuer.SetWindowText(strGrade);
		m_nProductId = nProductId;
	}

	return;
}

//获取产品名称
void CBusinessInfoDlg::LoadProduct(CString & strTempName, HTREEITEM hParentItem, int nParentId)
{
	m_comboProductName.ResetContent();  //清空
	//产品列表
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
			m_comboProductName.SetText(_T("暂无可选产品"));
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

BOOL CBusinessInfoDlg::GetRegionId()
{
	CString strRegionName = _T("");
	m_comboBusinessLocation.GetText(strRegionName);
	int i = 0;
	for(i = 0; i < m_ArrayRegionName.GetSize(); i++)  //查询区域ID
	{
		if (strRegionName == m_ArrayRegionName.GetAt(i))
		{
			m_nRegionId = _ttoi(m_ArrayRegionId.GetAt(i));
			break;
		}
	}

	//查询区域ID失败
	if (i == m_ArrayRegionName.GetSize())
	{
		FMessageBox::Show(_T("获取网点编号失败，请重新选择网点"), _T("错误"), MB_OK | MB_ICONWARNING);
		FMSCLIENT_ERROR("Find Region Id Failed:%s", fcT2A(strRegionName));
		return FALSE;
	}

	return TRUE;
}

BOOL CBusinessInfoDlg::GetProductId()
{
	CString strProductName = _T("");
	int nProductId;
	m_comboProductName.GetWindowText(strProductName);
	int i = 0;
	for(i = 0; i < m_ArrayProductName.GetSize(); i++)  //查询产品ID
	{
		if (strProductName == m_ArrayProductName.GetAt(i))
		{
			nProductId = _ttoi(m_ArrayProductId.GetAt(i));
			break;
		}
	}

	//查询区域ID失败
	if (i == m_ArrayProductName.GetSize())
	{
		FMSCLIENT_ERROR("Find Product Id Failed:%s", fcT2A(strProductName));
		return FALSE;
	}

	return TRUE;
}

int CBusinessInfoDlg::GetProductIdByName(CString& strProductName)
{
	// 清空原有数据
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
		FMSCLIENT_ERROR("GetProductTypeIdByName Failed！");
		return -1;
	}

	//产品列表
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
	for(nIndex = 0; nIndex < m_ArrayProductName.GetSize(); nIndex++)  //查询产品ID
	{
		if (strProductName == m_ArrayProductName.GetAt(nIndex))
		{
			return _ttoi(m_ArrayProductId.GetAt(nIndex));
			break;
		}
	}

	//查询产品ID失败
	if (nIndex == m_ArrayProductName.GetSize())
	{
		FMSCLIENT_ERROR("Find Region Id Failed:%s", fcT2A(strProductName));
		return -1;
	}

	return -1;
}

int CBusinessInfoDlg::GetProductTypeIdByName(CString& strProductTypeName)
{
	int nProductTypeId = -1;
	strProductTypeName.Trim();
	for (int i = 0; i < m_ArrayProductTypeName.GetSize(); i++)  //查找类型ID
	{
		if (strProductTypeName == m_ArrayProductTypeName.GetAt(i))
		{
			nProductTypeId = _ttoi(m_ArrayProductTypeId.GetAt(i));
			break;
		}
	}

	if (0 >= nProductTypeId) //没有找到
	{
		FMSCLIENT_ERROR("Find ProductType Id Failed!");
		return -1;
	}

	return nProductTypeId;
}
BOOL CBusinessInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	 if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))  
	{
		return TRUE;
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PURCHASEAMOUNT)) && (pMsg->message == WM_CHAR))  //只能输入数字、小数点等
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
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_CUSTOMERNAME)) &&  (pMsg->message == WM_CHAR))	//客户姓名特殊字符约束
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
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_IDNUMBER)) &&  (pMsg->message == WM_CHAR))	//证件号码
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
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PRODUCTCODE)) &&  (pMsg->message == WM_CHAR))	//产品编码
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
	else if(pMsg->message == WM_MOUSEMOVE)
	{
		m_ToolTips.RelayEvent(pMsg); 
	}
		
	return CFDialog::PreTranslateMessage(pMsg);
}

void CBusinessInfoDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFDialog::OnMouseHover(nFlags, point);
}

void CBusinessInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	  //区域
	  CString strRegion;
	  m_comboBusinessLocation.GetWindowText(strRegion);
	  CRect rcBusinessLocation;
	  m_comboBusinessLocation.GetWindowRect(rcBusinessLocation);
	  ScreenToClient(rcBusinessLocation);
	  if (rcBusinessLocation.PtInRect(point))
	  {
			//m_comboBusinessLocation.OnMouseHover(nFlags, point);
		  m_nLastHover = 1;
		  m_ToolTips.AddTool(GetDlgItem(IDC_COMBO_BUSINESSLOCATION), strRegion); 
		  m_ToolTips.Popup();
	  }

	  //客户姓名
	  CString strConsumerName;
	  m_editCustomerName.GetWindowText(strConsumerName);
	  CRect rcConsumerName;
	  m_editCustomerName.GetWindowRect(rcConsumerName);
	  ScreenToClient(rcConsumerName);
	  if (rcConsumerName.PtInRect(point))
	  {
		  m_nLastHover = 2;
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_CUSTOMERNAME), strConsumerName); 
		  m_ToolTips.Popup();
	  }

	  //客户电话
	  CString strConsumerPhone;
	  m_editCustomerPhone.GetWindowText(strConsumerPhone);
	  CRect rcConsumePhone;
	  m_editCustomerPhone.GetWindowRect(rcConsumePhone);
	  ScreenToClient(rcConsumePhone);
	  if (rcConsumePhone.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_CUSTOMERPHONE), strConsumerPhone); 
		  m_ToolTips.Popup();
	  }

	  //证件类型
	  CString strIDType;
	  m_comboIDType.GetWindowText(strIDType);
	  CRect rcIDType;
	  m_comboIDType.GetWindowRect(rcIDType);
	  ScreenToClient(rcIDType);
	  if (rcIDType.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_COMBO_IDTYPE), strIDType); 
		  m_ToolTips.Popup();
	  }

	  //证件号码
	  CString strIDNumber;
	  m_editIDNumber.GetWindowText(strIDNumber);
	  CRect rcIDNumber;
	  m_editIDNumber.GetWindowRect(rcIDNumber);
	  ScreenToClient(rcIDNumber);
	  if (rcIDNumber.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_IDNUMBER), strIDNumber); 
		  m_ToolTips.Popup();
	  }

	  //******产品信息******
	  //产品类型
	  CString strProductType;
	  m_comboProductType.GetWindowText(strProductType);
	  CRect rcProductType;
	  m_comboProductType.GetWindowRect(rcProductType);
	  ScreenToClient(rcProductType);
	  if (rcProductType.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_COMBO_PRODUCTTYPE), strProductType); 
		  m_ToolTips.Popup();
	  }

	  //产品名称
	  CString strProductName;
	  m_comboProductName.GetWindowText(strProductName);
	  CRect rcProductName;
	  m_comboProductName.GetWindowRect(rcProductName);
	  ScreenToClient(rcProductName);
	  if (rcProductName.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_COMBO_PRODUCTNAME), strProductName); 
		  m_ToolTips.Popup();
	  }

	  //发行机构
	  CString strIssuer;
	  m_editIssuer.GetWindowText(strIssuer);
	  CRect rcIssuer;
	  m_editIssuer.GetWindowRect(rcIssuer);
	  ScreenToClient(rcIssuer);
	  if (rcIssuer.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_ISSUER), strIssuer); 
		  m_ToolTips.Popup();
	  }

	  //产品编码
	  CString strCode;
	  m_editProductCode.GetWindowText(strCode);
	  CRect rcCode;
	  m_editProductCode.GetWindowRect(rcCode);
	  ScreenToClient(rcCode);
	  if (rcCode.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_PRODUCTCODE), strCode); 
		  m_ToolTips.Popup();
	  }

	  //购买金额
	  CString strMoney;
	  m_editPurchaseAmount.GetWindowText(strMoney);
	  CRect rcMoney;
	  m_editPurchaseAmount.GetWindowRect(rcMoney);
	  ScreenToClient(rcMoney);
	  if (rcMoney.PtInRect(point))
	  {
		  m_ToolTips.AddTool(GetDlgItem(IDC_EDIT_PURCHASEAMOUNT), strMoney); 
		  m_ToolTips.Popup();
	  }

	CFDialog::OnMouseMove(nFlags, point);
}

void CBusinessInfoDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFDialog::OnMouseLeave();
}
