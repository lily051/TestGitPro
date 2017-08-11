// TipDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "TipDlg.h"
#include "PreviewS20Dll.h"

#define COLOR_TREE_222			RGB(222, 222, 222)
#define WM_MSG_SET_POS          (WM_USER + 221)
#define WM_MSG_SET_ALPHA        (WM_USER + 222)
#define WM_MSG_TIP_END          (WM_USER + 223)
#define WM_MSG_SHOW_WND         (WM_USER + 224)
#define TITLE_BAR_HEIGHT		32
#define DLG_BORDER_WIDTH		6

// CTipDlg 对话框

IMPLEMENT_DYNAMIC(CTipDlg, UIDialog)

CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
	: UIDialog(CTipDlg::IDD, pParent)
	, m_bMouseHover(FALSE)
	, m_hMoveDlg(NULL)
    , m_bLive(TRUE)
	, m_nDlgIndex(1)
    , m_nDlgUpTime(500)
    , m_nDlgBlinkTime(5)
    , m_nAlphaMax(255)
	, m_nAlphaMin(100)
	, m_nHideTime(2500)
	, m_nShowMode(0)
    , m_nMsgType(0)
    , m_nAlphaSlowly(255)
{
    memset(&m_stLF, 0, sizeof(LOGFONT));
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(m_stLF), &m_stLF);
    m_stLF.lfWeight = 500;
    m_brush.CreateSolidBrush(theApp.GetUIColor(UI_COLOR_TYPE_UIDIALOG));
}

CTipDlg::~CTipDlg()
{
	
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	UIDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTipDlg, UIDialog)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_MSG_SET_POS, &CTipDlg::OnMsgSetWndPos)
    ON_MESSAGE(WM_MSG_SET_ALPHA, &CTipDlg::OnMsgSetWndAlpha)
    ON_MESSAGE(WM_MSG_TIP_END, &CTipDlg::OnMsgTipShowEnd)
    ON_MESSAGE(WM_MSG_SHOW_WND, &CTipDlg::OnMsgShowWnd)
    ON_WM_CLOSE()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CTipDlg 消息处理程序

BOOL CTipDlg::OnInitDialog()
{
	UIDialog::OnInitDialog();

    // set wnd style: no taskbar icon
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

    // set wnd topmost
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // set wnd transparent
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_SYSMENU); 
	SetLayeredWindowAttributes(0, 200, LWA_ALPHA);

    // get system work area info
	SystemParametersInfo(SPI_GETWORKAREA, 0, &m_rcDesktop, 0);
	GetClientRect(&m_rcDlg);

    m_rcTips = m_rcDlg;
    m_rcTips.DeflateRect(12, 12);

	TRACE("work area on system's primary monitor info：%d-%d\n", m_rcDesktop.right, m_rcDesktop.bottom);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

DWORD WINAPI MoveDlg(LPVOID pParam)
{
	CTipDlg *pDlg = (CTipDlg *)pParam;
  
    pDlg->ShowDlg();
	if (pDlg->m_bLive)
	{
		pDlg->BlinkDlg();

		if (pDlg->m_bLive)
		{
			pDlg->HideDlg();
		}
	}
	
	if (pDlg->m_bLive)
	{
		PostMessage(pDlg->m_hWnd, WM_MSG_TIP_END, 0, 0);
	}

	return 0;
}

LRESULT CTipDlg::OnMsgSetWndPos(WPARAM wParam, LPARAM lParam)
{
    CRect *pRcPlace = (CRect *)wParam;
    if (pRcPlace == NULL)
    {
        return 0;
    }

    SetWindowPos(NULL, pRcPlace->left, pRcPlace->top, pRcPlace->right, pRcPlace->bottom, SWP_NOSIZE);

    delete pRcPlace;
    pRcPlace = NULL;

    DrawTextInRect(m_strTips, m_rcTips, RGB(0,0,0));

    return 0;
}

LRESULT CTipDlg::OnMsgSetWndAlpha(WPARAM wParam, LPARAM lParam)
{
    BYTE byAlpha = (BYTE)wParam;
    SetLayeredWindowAttributes(0, byAlpha, LWA_ALPHA);

    return 0;
}

LRESULT CTipDlg::OnMsgShowWnd(WPARAM wParam, LPARAM lParam)
{
    int nCmdShow = (int)wParam;
    ShowWindow(nCmdShow);

    return 0;
}

// 窗口自动关闭
LRESULT CTipDlg::OnMsgTipShowEnd(WPARAM wParam, LPARAM lParam)
{
    if (m_hMoveDlg)
    {
        WaitForSingleObject(m_hMoveDlg, INFINITE);
        CloseHandle(m_hMoveDlg);
        m_hMoveDlg = NULL;

        TRACE("* close thread handle, time %d\n", (int)time(NULL));
    }

    return 0;
}

void CTipDlg::ShowDlg()
{
    PostMessage(WM_MSG_SET_ALPHA, (WPARAM)m_nAlphaMax);
	// move dlg up from screen bottom
	if (m_nShowMode == 1)
	{
		ShowDlgFromButtom();
	}
	else if (m_nShowMode == 2)
	{
		ShowDlgFromRight();
	}
	else
	{
		if (m_nDlgIndex == 1)
		{
			ShowDlgFromButtom();
		}
		else
		{
			ShowDlgFromRight();
		}
	}
}
void CTipDlg::BlinkDlg()
{
	// blink dlg
	BOOL bShow = FALSE;
	BYTE bAlpha = 80;
	for (int i = 0; i < m_nDlgBlinkTime * 2; i++)
	{
		if (!m_bLive)
		{
			break;
		}

		if (m_bMouseHover)
		{
			break;  // skip blink
		}
		bAlpha = bShow? m_nAlphaMin:m_nAlphaMax;
        PostMessage(WM_MSG_SET_ALPHA, (WPARAM)bAlpha);
		bShow = !bShow;
		Sleep(500);
	}
}
void CTipDlg::HideDlg()
{
	// slowly hide dlg
	int nSleepTime = m_nHideTime / m_nAlphaMax;
	for (m_nAlphaSlowly = m_nAlphaMax; m_nAlphaSlowly > 1; m_nAlphaSlowly--)
	{
		if (!m_bLive)
		{
			break;
		}
		while(m_bMouseHover && m_bLive)
		{
			m_nAlphaSlowly = m_nAlphaMax;
			Sleep(50);
        }
        PostMessage(WM_MSG_SET_ALPHA, (WPARAM)m_nAlphaSlowly);
		Sleep(nSleepTime);
	}
    PostMessage(WM_MSG_SHOW_WND, SW_HIDE);
}

void CTipDlg::ShowDlgFromButtom()
{
	int nLeft = m_rcDesktop.right - m_rcDlg.Width() - DLG_BORDER_WIDTH * 2;
    CRect *pRect = new CRect(nLeft, m_rcDesktop.bottom, 0, 0);
    PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
    PostMessage(WM_MSG_SHOW_WND, SW_SHOW);
	// 100 steps
	float fStep = (float)((float)((m_rcDlg.Height() + 
		DLG_BORDER_WIDTH + TITLE_BAR_HEIGHT) * m_nDlgIndex) / 100.0);
	float fTop = 0.0;
	int nSleepTime = m_nDlgUpTime / 100;
	for (int i = 0; i < 100; i++)
	{
		if (!m_bLive) // destory
		{
			return;
		}

		fTop = (float)m_rcDesktop.bottom - (float)i * fStep;
        pRect = new CRect(nLeft, (int)fTop, 0, 0);
        BOOL bResult = PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
        if (!bResult)
        {
            delete pRect;
            pRect = NULL;
        }
		Sleep(nSleepTime);
	}
    pRect = new CRect(nLeft, m_rcDesktop.bottom - ((m_rcDlg.Height() + 
        DLG_BORDER_WIDTH + TITLE_BAR_HEIGHT) * m_nDlgIndex), 0, 0);
    BOOL bResult = PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
    if (!bResult)
        {
            delete pRect;
            pRect = NULL;
        }
}

void CTipDlg::ShowDlgFromRight()
{
	int nTop = m_rcDesktop.bottom - 
		(m_rcDlg.Height() + DLG_BORDER_WIDTH + TITLE_BAR_HEIGHT) * m_nDlgIndex;
    CRect *pRect = new CRect(m_rcDesktop.right, nTop, 0, 0);
    PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
    PostMessage(WM_MSG_SHOW_WND, SW_SHOW);
	// 100 steps
	float fStep = (float)((float)(m_rcDlg.Width() + DLG_BORDER_WIDTH * 2) / 100.0);
	float fLeft = 0.0;
	int nSleepTime = m_nDlgUpTime / 100;
	for (int i = 0; i < 100; i++)
	{
		if (!m_bLive) // destory
		{
			return;
		}

		fLeft = (float)m_rcDesktop.right - (float)i * fStep;
        CRect *pRect = new CRect((int)fLeft, nTop, 0, 0);
        BOOL bResult = PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
        if (!bResult)
        {
            delete pRect;
            pRect = NULL;
        }
		Sleep(nSleepTime);
	}
    pRect = new CRect(m_rcDesktop.right - (m_rcDlg.Width() + DLG_BORDER_WIDTH * 2), nTop, 0, 0);
    BOOL bResult = PostMessage(WM_MSG_SET_POS, (WPARAM)pRect);
    if (!bResult)
    {
        delete pRect;
        pRect = NULL;
    }
}

void CTipDlg::DrawTextInRect(CString strText, CRect rcTemp, COLORREF clr)
{
    CDC *pDC = GetDC();
    if (NULL == pDC)
    {
        return;
    }
    HFONT hFontNew, hFontOld;
    hFontNew = CreateFontIndirect(&m_stLF);
    hFontOld = (HFONT)pDC->SelectObject(hFontNew);
    pDC->SetTextColor(clr);
    pDC->SetBkColor(theApp.GetUIColor(UI_COLOR_TYPE_UIDIALOG));
    // DrawText多行输出：DT_WORDBREAK|DT_EDITCONTROL，截断省略显示：DT_END_ELLIPSIS
    pDC->DrawText(strText, strText.GetLength(), rcTemp, DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS);
    pDC->SelectObject(hFontOld);

    DeleteObject(hFontNew);
    ReleaseDC(pDC);
}

void CTipDlg::ShowTips(CString strCaption, CString strTip, const int nDlgIndex, const TIP_DLG_PARAM &tdp)
{
    m_bLive = TRUE;
    m_bMouseHover = FALSE;
	SetWindowText(strCaption);
    //SetDlgItemText(IDC_STATIC_TIP, strTip);
    DrawTextInRect(strTip, m_rcTips, RGB(0,0,0));
    m_strTips = strTip;

	m_nDlgIndex = nDlgIndex + 1;
    m_nDlgUpTime = tdp.uMoveSpeed;
    m_nDlgBlinkTime = tdp.uBlinkTime;
	m_nHideTime = tdp.uHideTime;
	m_nShowMode = tdp.byShowMode;
    m_nAlphaMax = tdp.byAlphaMax;
    m_nAlphaMin = tdp.byAlphaMin;
    m_nMsgType = tdp.uMsgType;

    m_nAlphaSlowly = m_nAlphaMax;

	SetLayeredWindowAttributes(0, m_nAlphaMax, LWA_ALPHA);

	if (!m_hMoveDlg)
    {
        m_hMoveDlg = ::CreateThread(NULL, 0, MoveDlg, this, 0, 0);
	}
}

// 窗口手动关闭
void CTipDlg::DestoryTips()
{
    m_bLive = FALSE;
    TRACE("...DestoryTips comming, time %d\n", (int)time(NULL));
	if (m_hMoveDlg)
	{
		WaitForSingleObject(m_hMoveDlg, INFINITE);
		CloseHandle(m_hMoveDlg);
		m_hMoveDlg = NULL;

		TRACE("* close thread handle, time %d\n", (int)time(NULL));
	}
    ShowWindow(SW_HIDE);
    TRACE("...DestoryTips leaving, time %d\n", (int)time(NULL));
}

void CTipDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
    TRACE("set wnd alpha:255\n");

	UIDialog::OnMouseHover(nFlags, point);
}

void CTipDlg::OnMouseLeave()
{
	m_bMouseHover = FALSE;

    SetLayeredWindowAttributes(0, m_nAlphaMax, LWA_ALPHA);
    TRACE("OnMouseLeave  --  set wnd alpha:%d\n", m_nAlphaMax);

	UIDialog::OnMouseLeave();
}

void CTipDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseHover)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme); 
		tme.hwndTrack = m_hWnd; 
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 10;
		::_TrackMouseEvent(&tme);

        TRACE("OnMouseMove, time=%d\n", (int)time(NULL));
		m_bMouseHover = TRUE;

        SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
	}

    if (m_nMsgType == TIP_MSG_HYPERLINKS && m_rcTips.PtInRect(point))
    {
        SetCursor(LoadCursor(NULL,IDC_HAND));
    }

	UIDialog::OnMouseMove(nFlags, point);
}

HBRUSH CTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = UIDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CTipDlg::OnDestroy()
{
    DestoryTips();

	UIDialog::OnDestroy();
}

void CTipDlg::OnClose()
{
    DestoryTips();
}

void CTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_nMsgType == TIP_MSG_HYPERLINKS)
    {
        ShellExecute(NULL, _T("open"), m_strTips, NULL, NULL, SW_SHOW);
    }

    UIDialog::OnLButtonDown(nFlags, point);
}
