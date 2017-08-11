// ChangePasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "ChangePasswordDlg.h"
#include "CMSDataOperate.h"


// CChangePasswordDlg 对话框

IMPLEMENT_DYNAMIC(CChangePasswordDlg, CFDialog)

CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CChangePasswordDlg::IDD, pParent)
	, m_bIsLogin(false)
{

}

CChangePasswordDlg::~CChangePasswordDlg()
{
}

void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NOW_PWD, m_staticNowPwd);
	DDX_Control(pDX, IDC_EDIT_NOW_PWD, m_editNowPwd);
	DDX_Control(pDX, IDC_STATIC_NEW_PWD, m_staticNewPwd);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD, m_editNewPwd);
	DDX_Control(pDX, IDC_STATIC_NEW_PWD2, m_staticNewPwd2);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD2, m_editNewPwd2);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CChangePasswordDlg, CFDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CChangePasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangePasswordDlg 消息处理程序



BOOL CChangePasswordDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	InitCtrl();

	MoveCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CChangePasswordDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));

	CRect rcClient(CHANGE_PWD_WND_SIZE);
	MoveWindow(rcClient);
	CenterWindow();

	m_staticNowPwd.SetTextFont(YAHEI_18_N);
	m_staticNowPwd.SetTextAlign(DT_VCENTER | DT_RIGHT);
	m_staticNewPwd.SetTextFont(YAHEI_18_N);
	m_staticNewPwd.SetTextAlign(DT_VCENTER | DT_RIGHT);
	m_staticNewPwd2.SetTextFont(YAHEI_18_N);
	m_staticNewPwd2.SetTextAlign(DT_VCENTER | DT_RIGHT);
	m_editNowPwd.LimitText(50);
	m_editNowPwd.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editNewPwd.LimitText(50);
	m_editNewPwd.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editNewPwd2.LimitText(50);
	m_editNewPwd2.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editNowPwd.SetFocus();

	m_btnOK.SetObjectName(_T("btn"));
	m_btnOK.SetTextAlign(DT_CENTER);
	m_btnOK.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnCancel.SetObjectName(_T("cancel"));
	m_btnCancel.SetTextAlign(DT_CENTER);
	m_btnCancel.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
}

void CChangePasswordDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcStaticNowPwd(rcClient);
	rcStaticNowPwd.left = rcStaticNowPwd.left + 50;
	rcStaticNowPwd.top = rcStaticNowPwd.top + 33;
	rcStaticNowPwd.right = rcStaticNowPwd.left + 70;
	rcStaticNowPwd.bottom = rcStaticNowPwd.top + 20;
	m_staticNowPwd.MoveWindow(rcStaticNowPwd);

	CRect rcEditNowPwd(rcStaticNowPwd);
	rcEditNowPwd.left = rcEditNowPwd.right + 10;
	rcEditNowPwd.top = rcClient.top + 30;
	rcEditNowPwd.right = rcEditNowPwd.left + 150;
	rcEditNowPwd.bottom = rcEditNowPwd.top + 26;
	m_editNowPwd.MoveWindow(rcEditNowPwd);

	CRect rcStaticNewPwd(rcEditNowPwd);
	rcStaticNewPwd.left = rcStaticNowPwd.left;
	rcStaticNewPwd.top = rcStaticNewPwd.bottom + 17;
	rcStaticNewPwd.right = rcStaticNowPwd.right;
	rcStaticNewPwd.bottom = rcStaticNewPwd.top + 20;
	m_staticNewPwd.MoveWindow(rcStaticNewPwd);

	CRect rcEditNewPwd(rcEditNowPwd);
	rcEditNewPwd.top = rcEditNewPwd.bottom + 14;
	rcEditNewPwd.bottom = rcEditNewPwd.top + 26;
	m_editNewPwd.MoveWindow(rcEditNewPwd);

	CRect rcStaticNewPwd2(rcEditNewPwd);
	rcStaticNewPwd2.left = rcStaticNowPwd.left;
	rcStaticNewPwd2.top = rcStaticNewPwd2.bottom + 17;
	rcStaticNewPwd2.right = rcStaticNowPwd.right;
	rcStaticNewPwd2.bottom = rcStaticNewPwd2.top + 20;
	m_staticNewPwd2.MoveWindow(rcStaticNewPwd2);

	CRect rcEditNewPwd2(rcEditNewPwd);
	rcEditNewPwd2.top = rcEditNewPwd2.bottom + 14;
	rcEditNewPwd2.bottom = rcEditNewPwd2.top + 26;
	m_editNewPwd2.MoveWindow(rcEditNewPwd2);

	CRect rcBtnCancel(rcClient);
	rcBtnCancel.left = rcBtnCancel.right - 10 - 90;
	rcBtnCancel.top = rcBtnCancel.bottom - 10 - 30;
	rcBtnCancel.right = rcBtnCancel.left + 90;
	rcBtnCancel.bottom = rcBtnCancel.top + 30;
	m_btnCancel.MoveWindow(rcBtnCancel);

	CRect rcBtnOK(rcBtnCancel);
	rcBtnOK.right = rcBtnOK.left - 10;
	rcBtnOK.left = rcBtnOK.right - 90;
	m_btnOK.MoveWindow(rcBtnOK);

	return;
}

void CChangePasswordDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}

void CChangePasswordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strNowPwd;
	CString strNewPwd;
	CString strNewPwd2;
	m_editNowPwd.GetPasswordText(strNowPwd);
	m_editNewPwd.GetPasswordText(strNewPwd);
	m_editNewPwd2.GetPasswordText(strNewPwd2);
	if (strNowPwd == _T(""))
	{
		m_editNowPwd.SetErrorState(TRUE);
		return;
	}
	else if (strNewPwd == _T(""))
	{
		m_editNewPwd.SetErrorState(TRUE);
		return;
	}
	else if (strNewPwd2 == _T(""))
	{
		m_editNewPwd2.SetErrorState(TRUE);
		return;
	}

	if (strNewPwd != strNewPwd2)
	{
		FMessageBox::Show(_T("确认密码输入有误！"), _T("错误"), MB_OK | MB_ICONERROR);
		m_editNewPwd2.SetErrorState(TRUE);
		return;
	}

	cms_8120::RspClientLogin rspClientLogin;
	if (!CCMSMgr::SnglPtr()->ChangePassword(rspClientLogin, strNowPwd, strNewPwd) || rspClientLogin.result() != 0)
	{

		FMSCLIENT_ERROR("Change Password Failed!");
		CString strErrorInfo = UTF8_A2MFCSTRW(rspClientLogin.result_info());
		FMessageBox::Show(strErrorInfo, _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	else
	{
		CLIENT_INFO("User:%d修改密码成功！", theApp.m_nUserId);
		FMessageBox::Show(_T("修改密码成功，请重新登录！"), _T("提示"), MB_OK);
		if (m_bIsLogin)
		{
			theApp.m_bReboot = true;
			TCHAR szPath[256] = {0};
			GetModuleFileName(NULL, szPath, 256);
			RebootApp(szPath);
		}
		CDialog::OnOK();
	}
	
	CDialog::OnOK();
}

bool CChangePasswordDlg::GetLoginState(void)
{
	return m_bIsLogin;
}

void CChangePasswordDlg::SetLoginState(bool bLogin)
{
	m_bIsLogin = bLogin;
	return;
}
