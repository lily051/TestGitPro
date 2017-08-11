// WordTemplateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "WordTemplateDlg.h"


// CWordTemplateDlg 对话框

IMPLEMENT_DYNAMIC(CWordTemplateDlg, CFDialog)

CWordTemplateDlg::CWordTemplateDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CWordTemplateDlg::IDD, pParent)
	,m_strProductName(_T(""))
	,m_strWordTemplate(_T(""))
{

}

CWordTemplateDlg::~CWordTemplateDlg()
{
}

void CWordTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PRODUCTNAME1, m_staticProductName1);
	DDX_Control(pDX, IDC_STATIC_PRODUCTNAME2, m_staticProductName2);
	DDX_Control(pDX, IDC_STATIC_PRODUCTWORD, m_staticProductWord1);
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_EDIT_PRODUCTWORD, m_editProductWord);
	DDX_Control(pDX, IDC_STATIC_BLANK1, m_staticBlank1);
}


BEGIN_MESSAGE_MAP(CWordTemplateDlg, CFDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWordTemplateDlg 消息处理程序

BOOL CWordTemplateDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	LoadSkin();

	MoveWindow(WORD_TEMPLATE_WND_SIZE);
	CenterWindow();

	InitCtrl();

	MoveCtrl();

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWordTemplateDlg::LoadSkin(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));
}

void CWordTemplateDlg::InitCtrl(void)
{
	int nState = FUi::Normal | FUi::Hovered | FUi::Pressed | FUi::Focus | FUi::Disabled | FUi::Default;
	m_staticProductName1.SetTextFont(YAHEI_18_N);
	m_staticProductName2.SetTextFont(YAHEI_18_N);
	m_staticProductName2.SetHoverCursor(LoadCursor(NULL, IDC_ARROW));	 //为tips提示做准备，否则无法显示tips
	m_staticProductWord1.SetTextFont(YAHEI_18_N);
	m_editProductWord.SetTextFont(nState, YAHEI_18_N);
	//m_editProductWord.EnableWindow(FALSE);
	m_editProductWord.SetReadOnly(TRUE);
	m_editProductWord.SetDefaultText(_T(""));
	m_editProductWord.SetTextColor(nState, FUi::Black);
	m_editProductWord.SetBorderColor(nState, FUi::White);
	m_btnClose.SetObjectName(_T("cancel"));
	m_btnClose.SetTextFont(nState, YAHEI_18_SB);
	m_btnClose.SetTextAlign(DT_CENTER);
	GetDlgItem(IDC_STATIC_BLANK1)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_EDIT_PRODUCTWORD)->ShowScrollBar(SB_VERT, TRUE);
}

void CWordTemplateDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcTmp(rcClient);

	rcTmp.top = 25;
	rcTmp.left = 30;
	m_staticProductName1.SetWindowPos(NULL, rcTmp.left, rcTmp.top, 0, 0, SWP_NOSIZE);

	//rcTmp.top -= 3;
	rcTmp.left = 100;
	//m_staticProductName2.SetWindowPos(NULL, rcTmp.left, rcTmp.top, 300, 25, SWP_NOSIZE);
	m_staticProductName2.MoveWindow(rcTmp.left, rcTmp.top, rcTmp.left + 285, rcTmp.top + 15);

	rcTmp.top += 35;
	rcTmp.left = 30;
	m_staticProductWord1.SetWindowPos(NULL, rcTmp.left, rcTmp.top, 0, 0, SWP_NOSIZE);

	rcTmp.top -= 3;
	rcTmp.left = 100;
	//m_editProductWord.SetWindowPos(NULL, rcTmp.left, rcTmp.top, 400, 200, SWP_NOSIZE);
	m_editProductWord.MoveWindow(rcTmp.left, rcTmp.top, rcTmp.left + 300, rcTmp.top + 120);

	rcTmp.top = rcTmp.bottom - 40;
	rcTmp.left += 330;
	m_btnClose.SetWindowPos(NULL, rcTmp.left, rcTmp.top, 90, 30, SWP_NOSIZE);

	//底部横线
	CRect rcStaticBlank1(rcClient);
	rcStaticBlank1.top = rcClient.top + 243;
	rcStaticBlank1.bottom = rcStaticBlank1.top + 1;
	m_staticBlank1.MoveWindow(rcStaticBlank1);
}

void CWordTemplateDlg::InitData(void)
{
	m_staticProductName2.SetText(m_strProductName);
	m_staticProductName2.SetTipText(m_strProductName);
	m_editProductWord.SetWindowText(m_strWordTemplate);
}

void CWordTemplateDlg::SetDlgData(CString& strProductName, CString& strWordTemplate)
{
	m_strProductName = strProductName;
	m_strWordTemplate = strWordTemplate;
}


void CWordTemplateDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(240, 240, 240));
	CPen* pOld = dc.SelectObject(&linePen);

	//底部横线
	CRect rc1;
	GetDlgItem(IDC_STATIC_BLANK1)->GetWindowRect(&rc1);
	ScreenToClient(rc1);

	dc.MoveTo(rc1.left, rc1.top + rc1.Height() / 2);
	dc.LineTo(rc1.right, rc1.top + rc1.Height() / 2);
}
