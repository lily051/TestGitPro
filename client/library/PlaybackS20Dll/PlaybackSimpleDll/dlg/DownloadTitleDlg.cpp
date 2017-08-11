// DownloadTitleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadTitleDlg.h"


// CDownloadTitleDlg 对话框

IMPLEMENT_DYNAMIC(CDownloadTitleDlg, CDialog)

CDownloadTitleDlg::CDownloadTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadTitleDlg::IDD, pParent)
{
	m_bMove = FALSE;
	m_bkBrush.CreateSolidBrush(UI_TREE_BKCOLOR);

	m_nDowningCount = 0;
	m_nDownSuccessCount = 0;
	m_nDownFailedCount = 0;
}

CDownloadTitleDlg::~CDownloadTitleDlg()
{
	try
	{
		m_bkBrush.DeleteObject();
	}
	catch (...)
	{

	}

}

void CDownloadTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DOWNTITLE, m_btnTitle);
	DDX_Control(pDX, IDC_STATIC_DOWNLOADING, m_staDownloading);
	DDX_Control(pDX, IDC_STATIC_DOWNSUCCESS, m_staDownSuccess);
	DDX_Control(pDX, IDC_STATIC_DOWNFAILED, m_staDownFailed);
}


BEGIN_MESSAGE_MAP(CDownloadTitleDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_DOWNLOADING, &CDownloadTitleDlg::OnStnClickedStaticDownloading)
	ON_STN_CLICKED(IDC_STATIC_DOWNSUCCESS, &CDownloadTitleDlg::OnStnClickedStaticDownsuccess)
	ON_STN_CLICKED(IDC_STATIC_DOWNFAILED, &CDownloadTitleDlg::OnStnClickedStaticDownfailed)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDownloadTitleDlg 消息处理程序

BOOL CDownloadTitleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDownloadTitleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}

}

HBRUSH CDownloadTitleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_bkBrush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

/** @fn InitCtrl
*   @brief 初始化控件
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::InitCtrl()
{
	m_btnTitle.SetWindowText(_T("下载列表"));
	m_btnTitle.Load(IDB_PNG_TITLE);

	m_staDownloading.Load(IDB_PNG_ONBTN_DOWNLOAD);
	m_staDownSuccess.Load(IDB_PNG_DOWNSUCCESS);
	m_staDownFailed.Load(IDB_PNG_ONBTN_DELETE);

	m_staDownloading.SetStaticSel(TRUE);

	SetStaText();

}

/** @fn MoveCtrl
*   @brief 移动控件
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nSpace = 1;
	CRect rcBtnTitle;
	rcBtnTitle.left = rcClient.left + nSpace;
	rcBtnTitle.top = rcClient.top + nSpace;
	rcBtnTitle.right = rcClient.right - 1;
	rcBtnTitle.bottom = rcBtnTitle.top + 26;
	m_btnTitle.MoveWindow(rcBtnTitle);

	int nStaticHeight = 32;

	CRect rcStatic(rcBtnTitle);
	rcStatic.top = rcBtnTitle.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownloading.MoveWindow(rcStatic);

	rcStatic.top = rcStatic.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownSuccess.MoveWindow(rcStatic);

	rcStatic.top = rcStatic.bottom;
	rcStatic.bottom = rcStatic.top + nStaticHeight;
	m_staDownFailed.MoveWindow(rcStatic);

}

/** @fn SetStaText 
*   @brief 设置static控件文字
*   @param[in]  NULL
*   @param[out] NULL
*   @return NULL
*/
void CDownloadTitleDlg::SetStaText()
{
	CString staText;

	staText.Format(_T("正在下载(%d)"), m_nDowningCount);
	m_staDownloading.SetText(staText);

	staText.Format(_T("已下载(%d)"), m_nDownSuccessCount);
	m_staDownSuccess.SetText(staText);

	staText.Format(_T("下载失败(%d)"), m_nDownFailedCount);
	m_staDownFailed.SetText(staText);

	Invalidate();
	UpdateWindow();
}

void CDownloadTitleDlg::OnStnClickedStaticDownloading()
{
	m_staDownloading.SetStaticSel(TRUE);
	m_staDownSuccess.SetStaticSel(FALSE);
	m_staDownFailed.SetStaticSel(FALSE);

	::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNLOADING, 0);
}

void CDownloadTitleDlg::OnStnClickedStaticDownsuccess()
{
	m_staDownloading.SetStaticSel(FALSE);
	m_staDownSuccess.SetStaticSel(TRUE);
	m_staDownFailed.SetStaticSel(FALSE);

	::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNSUCCESS, 0);
}

void CDownloadTitleDlg::OnStnClickedStaticDownfailed()
{
	m_staDownloading.SetStaticSel(FALSE);
	m_staDownSuccess.SetStaticSel(FALSE);
	m_staDownFailed.SetStaticSel(TRUE);

	::PostMessage(this->GetParent()->GetSafeHwnd(), MSG_DOWNLIST_SELECT, IDC_STATIC_DOWNFAILED, 0);
}


void CDownloadTitleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_TREE_BKCOLOR);

}
