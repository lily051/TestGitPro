// EditFileNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "EditFileNameDlg.h"


// CEditFileNameDlg 对话框

IMPLEMENT_DYNAMIC(CEditFileNameDlg, CFDialog)

CEditFileNameDlg::CEditFileNameDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CEditFileNameDlg::IDD, pParent)
	,m_strOldFileName(_T(""))
	,m_strNewFileName(_T(""))
	,m_strNamePostfix(_T(""))
{

}

CEditFileNameDlg::~CEditFileNameDlg()
{
}

void CEditFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFileName);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CEditFileNameDlg, CFDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CEditFileNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditFileNameDlg 消息处理程序


BOOL CEditFileNameDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();

	MoveCtrl();

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEditFileNameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}

void CEditFileNameDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));

	CRect rcClient(EDIT_FILENAME_WND_SIZE);
	MoveWindow(rcClient);
	CenterWindow();

	m_staticFileName.SetTextFont(YAHEI_18_N);
	m_staticFileName.SetTextAlign(DT_RIGHT);
	m_editFileName.LimitText(45);	//数据库Records表的strRecordName字段限制50，输入后加上扩展名(.mp4/.jpg)不超过50，否则提交异常
	m_editFileName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editFileName.SetFocus();

	m_btnOK.SetObjectName(_T("btn"));
	m_btnOK.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, FUi::fui_color::White);
	m_btnOK.SetTextAlign(DT_CENTER);
	m_btnOK.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnCancel.SetObjectName(_T("cancel"));
	m_btnCancel.SetTextAlign(DT_CENTER);
	m_btnCancel.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
}

void CEditFileNameDlg::SetDlgData(CString strFileName)
{
	m_strOldFileName = strFileName;
}

void CEditFileNameDlg::InitData()
{
	//提取文件名
	CString strRecName = m_strOldFileName.Mid(m_strOldFileName.ReverseFind('\\') + 1);
	int npos = strRecName.ReverseFind('\.');
	CString strTmp = strRecName.Mid(0, npos);
	m_editFileName.SetWindowText(strTmp);
	m_strNamePostfix = strRecName.Mid(npos);  //获取后缀名
}

void CEditFileNameDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcStaticFileName(rcClient);
	rcStaticFileName.left = rcStaticFileName.left + 30;
	rcStaticFileName.top = rcStaticFileName.top + 33;
	rcStaticFileName.right = rcStaticFileName.left + 60;
	rcStaticFileName.bottom = rcStaticFileName.top + 26;
	m_staticFileName.MoveWindow(rcStaticFileName);

	CRect rcEditFileName(rcStaticFileName);
	rcEditFileName.left = rcEditFileName.right + 10;
	rcEditFileName.top = rcClient.top + 30;
	rcEditFileName.right = rcEditFileName.left + 200;
	rcEditFileName.bottom = rcEditFileName.top + 26;
	m_editFileName.MoveWindow(rcEditFileName);

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

}
void CEditFileNameDlg::OnBnClickedOk()
{
	//提取文件名
	CString strPath = m_strOldFileName.Mid(0, m_strOldFileName.ReverseFind('\\') + 1);
	CString strFileName;
	m_editFileName.GetWindowText(strFileName);
	if (_T("") == strFileName)
	{
		FMessageBox::Show(_T("文件名不能为空！"), _T("提示"), MB_OK | MB_ICONERROR);
		m_editFileName.SetErrorState(TRUE);
		return;
	}
	else
	{
		m_strNewFileName = strPath + strFileName + m_strNamePostfix;
		CLIENT_INFO("Before rename");
		try
		{
			CFile::Rename(m_strOldFileName, m_strNewFileName);
		}
		catch(CFileException* pEx )
		{
			TRACE(_T("File %20s not found, cause = %d\n"), m_strOldFileName, 
				pEx->m_cause);
			pEx->Delete();
			return;
		}
		CLIENT_INFO("After rename");
		theApp.m_strNewFileName = m_strNewFileName.Mid(m_strNewFileName.ReverseFind('\\') + 1);
	}
	CDialog::OnOK();
}

BOOL CEditFileNameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CEdit * m_edit = (CEdit*)GetDlgItem(IDC_EDIT_FILENAME);
	if (pMsg->message == WM_NCLBUTTONDBLCLK)  ////屏蔽标题栏双击事件(最大化/还原)
	{
		return TRUE;
	}
	else if ((GetFocus() == m_edit) && (pMsg->message == WM_CHAR))  //限制特殊字符输入
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
