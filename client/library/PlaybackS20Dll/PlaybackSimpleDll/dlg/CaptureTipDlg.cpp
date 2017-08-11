// CaptureTipDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "CaptureTipDlg.h"
#include "../ui/uiCtrl/memdcex.h"

// CCaptureTipDlg 对话框

IMPLEMENT_DYNAMIC(CCaptureTipDlg, UIDialog)

CCaptureTipDlg::CCaptureTipDlg(CWnd* pParent /*=NULL*/)
	: UIDialog(CCaptureTipDlg::IDD, pParent)
{
	m_bIsInit = FALSE;
	m_bCount = TRUE;
	m_bTimer = FALSE;
	m_nLeaveTime = 0;

	m_bkBrush.CreateSolidBrush(UI_TREE_BKCOLOR);
}

CCaptureTipDlg::~CCaptureTipDlg()
{
	try
	{
		m_bkBrush.DeleteObject();
	}
	catch (...)
	{

	}
}

void CCaptureTipDlg::DoDataExchange(CDataExchange* pDX)
{
	UIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATH, m_sttPath);
}


BEGIN_MESSAGE_MAP(CCaptureTipDlg, UIDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCaptureTipDlg 消息处理程序

BOOL CCaptureTipDlg::OnInitDialog()
{
	UIDialog::OnInitDialog();

	InitPos();

	m_bIsInit = TRUE;

	SetTimer(IDT_CAPTUREDLGCLOSE_TIME, 1000, NULL);
	m_bTimer = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCaptureTipDlg::OnDestroy()
{
	UIDialog::OnDestroy();

	m_pic.FreePicture();

	if (m_bTimer)
	{
		KillTimer(IDT_CAPTUREDLGCLOSE_TIME);
		m_bTimer = FALSE;
	}
}

BOOL CCaptureTipDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMemDCEX memDC(pDC,m_rcPic);
	
	m_pic.DrawPicture(memDC->m_hDC,0, 0,m_rcPic.Width(),m_rcPic.Height());

	return UIDialog::OnEraseBkgnd(pDC);
}

void CCaptureTipDlg::LoadPic(TCHAR * szPicPath)
{
	char * szPic;
	int nLength;
	CP936_W2A(szPicPath, wcslen(szPicPath)+1, szPic, nLength);
	m_pic.LoadPicture(szPic);
	CodeSet_Free(szPic);

	m_sttPath.SetWindowText(szPicPath);
}


void CCaptureTipDlg::InitPos()
{
	CRect rcWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);

	CRect rcClient;
	GetClientRect(rcClient);

	int nWidth = 300;
	int nHeight = 240;
	MoveWindow(rcWorkArea.right - nWidth,
		rcWorkArea.bottom - nHeight, nWidth, nHeight);

	GetClientRect(rcClient);
	m_rcPic.left = 3;
	m_rcPic.top = 3;
	m_rcPic.right = m_rcPic.left + 290;
	m_rcPic.bottom = m_rcPic.top + 190;

	m_rcPath.left = m_rcPic.left;
	m_rcPath.top = m_rcPic.bottom + 2;
	m_rcPath.right = m_rcPath.left + 290;
	m_rcPath.bottom = m_rcPath.top + 40;
	m_sttPath.MoveWindow(m_rcPath);
}

void CCaptureTipDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//if (m_rcPath.PtInRect(point))
	{
		SetCursor(LoadCursor(NULL,IDC_HAND)); 	
	}

	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
	mouseEvent.hwndTrack = m_hWnd;
	mouseEvent.dwHoverTime = 200;
	::TrackMouseEvent(&mouseEvent);

	UIDialog::OnMouseMove(nFlags, point);
}

void CCaptureTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if (m_rcPath.PtInRect(point))
	{
		CString strPic;
		m_sttPath.GetWindowText(strPic);

		CString strParam;
		strParam.Format(_T("/select,%s"),  strPic);
		ShellExecute(NULL, _T("open"), _T("Explorer.exe"), strParam.GetBuffer(), NULL, SW_SHOW);
	}

	UIDialog::OnLButtonDown(nFlags, point);
}

void CCaptureTipDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UIDialog::OnTimer(nIDEvent);

	if (IDT_CAPTUREDLGCLOSE_TIME == nIDEvent)
	{
		if (!m_bCount)
		{
			return;
		}

		if (m_nLeaveTime >= 5)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			m_nLeaveTime++;
		}
	}
}

void CCaptureTipDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bCount = FALSE;
	m_nLeaveTime = 0;

	UIDialog::OnMouseHover(nFlags, point);
}

void CCaptureTipDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bCount = TRUE;

	UIDialog::OnMouseLeave();
}

HBRUSH CCaptureTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = UIDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(UI_TREE_BKCOLOR);
		pDC->SetTextColor(UI_TEXT_COLOR);
		hbr = m_bkBrush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CCaptureTipDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	if (m_bTimer)
	{
		KillTimer(IDT_CAPTUREDLGCLOSE_TIME);
		m_bTimer = FALSE;
	}
	//UIDialog::OnClose();
}
