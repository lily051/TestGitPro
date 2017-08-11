
// ConfigClientUIDlg.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "UIDialog.h"
#include "mousepatch.h"
#include "UIDlgDefine.h"
#include "../../PlaybackSimpleDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// UIDialog dialog

IMPLEMENT_DYNAMIC(UIDialog, CDialog)

UIDialog::UIDialog(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCenter = FALSE;

    m_bitmap.Attach(m_pngMetrics.LoadPNG(UIDLG_PNG, UIDLG_COLOR_TRANSPARENT));
    m_dcBuffer.CreateCompatibleDC(NULL);
    m_dcBuffer.SelectObject(&m_bitmap);    
    m_pngMetrics.SetMode(0);
}

BEGIN_MESSAGE_MAP(UIDialog, CDialog)
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_NCACTIVATE()
	ON_WM_SIZE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_PAINT()
	ON_WM_NCCREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCRBUTTONUP()
    ON_WM_ERASEBKGND()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// UIDialog message handlers

BOOL UIDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	NcLButtonUpHook::SetHook(GetSafeHwnd());
	m_brushBkgnd.CreateSolidBrush(UIDLG_COLOR_BKGND);

    LOGFONT lf = {0};
    lf.lfHeight = UIDLG_FONT_BAR_HEIGHT;
    lf.lfWeight = UIDLG_FONT_BAR_WEIGHT;
    ::lstrcpy(lf.lfFaceName, UIDLG_FONT_BAR_NAME);

	m_font.CreatePointFontIndirect(&lf);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void UIDialog::DrawDialog(void)
{
	CWindowDC dc(this);
	if (NULL != dc.GetSafeHdc())
	{
		CRect rtWindow;
		GetWindowRect(rtWindow);
		ScreenToClient(rtWindow);

		CRect rtClient;
		GetClientRect(rtClient);

		rtClient.OffsetRect(-rtWindow.TopLeft());
		dc.ExcludeClipRect(rtClient);

		rtWindow.OffsetRect(-rtWindow.TopLeft());

		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
        CBitmap memBitmap;
		memBitmap.CreateCompatibleBitmap(&dc,rtWindow.Width(),rtWindow.Height());
		memDC.SelectObject(&memBitmap);

		memDC.FillSolidRect(rtWindow,RGB(255,255,255));

		int nOldMode = memDC.SetStretchBltMode(COLORONCOLOR);

		CSize szDlgFrame(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME));
		CRect rtCaption(rtWindow);
		rtCaption.bottom = rtCaption.top + m_pngMetrics.GetCapMid().Height();
				
        // draw caption
		int xSrc = m_pngMetrics.GetCapLeft(m_bActive).TopLeft().x;
		int ySrc = m_pngMetrics.GetCapLeft(m_bActive).TopLeft().y;
		int wSrc = m_pngMetrics.GetCapLeft(m_bActive).Width();
		int hSrc = m_pngMetrics.GetCapLeft(m_bActive).Height();
        memDC.StretchBlt(rtCaption.left,rtCaption.top,wSrc,hSrc,
            &m_dcBuffer,xSrc,ySrc,wSrc,hSrc,SRCCOPY);		

        xSrc = m_pngMetrics.GetCapMid(m_bActive).TopLeft().x;
        ySrc = m_pngMetrics.GetCapMid(m_bActive).TopLeft().y;
        wSrc = m_pngMetrics.GetCapMid(m_bActive).Width();
        hSrc = m_pngMetrics.GetCapMid(m_bActive).Height();
        memDC.StretchBlt(rtCaption.left + m_pngMetrics.GetCapLeft().Width()
                        ,rtCaption.top
                        ,rtCaption.Width() - m_pngMetrics.GetCapLeft().Width() - m_pngMetrics.GetCapRight().Width()
                        ,rtCaption.Height(),
            &m_dcBuffer,xSrc,ySrc,wSrc,hSrc,SRCCOPY);		

        xSrc = m_pngMetrics.GetCapRight(m_bActive).TopLeft().x;
        ySrc = m_pngMetrics.GetCapRight(m_bActive).TopLeft().y;
        wSrc = m_pngMetrics.GetCapRight(m_bActive).Width();
        hSrc = m_pngMetrics.GetCapRight(m_bActive).Height();
		memDC.StretchBlt(rtCaption.right - m_pngMetrics.GetCapRight().Width()
                        ,rtCaption.top
                        ,m_pngMetrics.GetCapRight().Width()
                        ,hSrc,
			&m_dcBuffer,xSrc,ySrc,wSrc,hSrc,SRCCOPY);

		// draw left
		CRect rcLeft(rtWindow);
		rcLeft.top = m_pngMetrics.GetCapMid().Height();
		rcLeft.right = m_pngMetrics.GetLeftBorder().Width();
        xSrc = m_pngMetrics.GetLeftBorder().TopLeft().x;
        ySrc = m_pngMetrics.GetLeftBorder().TopLeft().y;
        wSrc = m_pngMetrics.GetLeftBorder().Width();
        hSrc = m_pngMetrics.GetLeftBorder().Height();
		memDC.StretchBlt(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height(),
			&m_dcBuffer,xSrc,ySrc,wSrc,hSrc,SRCCOPY);

		// draw right
		CRect rcRight(rtWindow);
		rcRight.top = m_pngMetrics.GetCapMid().Height();
		rcRight.left = rcRight.right - m_pngMetrics.GetRightBorder().Width();
        xSrc = m_pngMetrics.GetRightBorder().TopLeft().x;
        ySrc = m_pngMetrics.GetRightBorder().TopLeft().y;
        wSrc = m_pngMetrics.GetRightBorder().Width();
        hSrc = m_pngMetrics.GetRightBorder().Height();
		memDC.TransparentBlt(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height(),
			&m_dcBuffer,xSrc,ySrc,wSrc,hSrc,UIDLG_COLOR_TRANSPARENT);

		// Draw bottom
		CRect rcBottom(rtWindow);
		//rcBottom.left = 0;
		//rcBottom.right = rcBottom.right;
		rcBottom.top = rcBottom.bottom - m_pngMetrics.GetBottomBorder().Height();
        xSrc = m_pngMetrics.GetBottomBorder().TopLeft().x;
        ySrc = m_pngMetrics.GetBottomBorder().TopLeft().y;
        wSrc = m_pngMetrics.GetBottomBorder().Width();
        hSrc = m_pngMetrics.GetBottomBorder().Height();
		memDC.TransparentBlt(rcBottom.left,rcBottom.top,rcBottom.Width(),rcBottom.Height(),
			&m_dcBuffer,xSrc,ySrc,wSrc,hSrc,UIDLG_COLOR_TRANSPARENT);

		memDC.SetStretchBltMode(nOldMode);

		if (m_hIcon)
		{
			// Draw icon
			CSize IconSize(::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON));

			long x = rtCaption.left + szDlgFrame.cx + szDlgFrame.cx - 1;
			long y = rtCaption.top + max(0,(rtCaption.Height() - IconSize.cy) / 2) + 1;

			::DrawIconEx(memDC.GetSafeHdc(),x,y,m_hIcon,16,16,0,NULL,DI_NORMAL | DI_COMPAT);
		}

		CString strCaption;
		GetWindowText(strCaption);

		nOldMode = memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(UIDLG_COLOR_TITLE);
		memDC.SelectObject(&m_font);
		
		UINT format = DT_SINGLELINE | DT_VCENTER;
		
		if (m_bCenter)
		{
			format |= DT_CENTER;
		}

		CRect rc(rtCaption);
		rc.left += 24;
		
		memDC.DrawText(strCaption,rc,format);

		memDC.SetBkMode(nOldMode);

		BOOL bMax = ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE)?TRUE:FALSE;

		for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
		{
			SystemButton * pSysButton = m_listSysButton.GetNext(pos);
			pSysButton->DrawButton(&memDC,&m_dcBuffer,bMax);
			ySrc += SystemButton::sysHeight;
		}

		dc.BitBlt(rtWindow.left,rtWindow.top,rtWindow.Width(),rtWindow.Height(),&memDC,0,0,SRCCOPY);
	}
}

void UIDialog::OnNcPaint()
{
	// TODO: Add your message handler code here
	DrawDialog();
	
	// Do not call CDialog::OnNcPaint() for painting messages
}

LRESULT UIDialog::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UINT hitTest = HTNOWHERE;

	CPoint pt(point);
	ScreenToClient(&pt);

	pt.Offset(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME) + ::GetSystemMetrics(SM_CYCAPTION));

	for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
	{
		SystemButton * pSysButton = m_listSysButton.GetNext(pos);

		pSysButton->SetHover(FALSE);

		if (pSysButton->PtInRect(pt))
		{
			pSysButton->SetHover(TRUE);
			hitTest = pSysButton->GetButtonHit();
		}
	}

	if (hitTest != HTNOWHERE)
	{
		DrawDialog();
		return hitTest;
	}

	if (m_rcHitCaption.PtInRect(pt))
	{
		return HTCAPTION;
	}

	if (m_rcHitIcon.PtInRect(pt))
	{
		return HTSYSMENU;
	}

	LRESULT result = CDialog::OnNcHitTest(point);

	if (result == HTCLOSE || result == HTMAXBUTTON || result == HTMINBUTTON)
	{
		DrawDialog();	
		return HTNOWHERE;
	}

	DrawDialog();
	return CDialog::OnNcHitTest(point);
}

void UIDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default
	UNUSED(bCalcValidRects);
	UNREFERENCED_PARAMETER(bCalcValidRects);

    //CRect rtWindow;
    //GetWindowRect(&rtWindow);

    //int n1 = ::GetSystemMetrics(SM_CYCAPTION);
    //int n2 = ::GetSystemMetrics(SM_CXDLGFRAME);
    //int n3 = ::GetSystemMetrics(SM_CYDLGFRAME);

	//lpncsp->rgrc[0].top += ::GetSystemMetrics(SM_CYCAPTION);

    RECT rc = lpncsp->rgrc[0];
    lpncsp->rgrc[0].top += m_pngMetrics.nCapHeight;
    lpncsp->rgrc[0].left += m_pngMetrics.nLeftBorderWidth;
    lpncsp->rgrc[0].right -= m_pngMetrics.nRightBorderWidth;
    lpncsp->rgrc[0].bottom -= m_pngMetrics.nBottomBorderHeight;
    RECT rc2 = lpncsp->rgrc[0];
    lpncsp->rgrc[0] = rc;

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
    lpncsp->rgrc[0] = rc2;
}

BOOL UIDialog::OnNcActivate(BOOL bActive)
{
	// TODO: Add your message handler code here and/or call default
	m_bActive = bActive;

	DrawDialog();

	return TRUE;
}

void UIDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rcCaption = GetCaptionRect();
	
	CRect rcClose(rcCaption);
	rcClose.OffsetRect(-8,2);

	SystemButton *pSysCloseButton = NULL;

	for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
	{
		if (NULL == pSysCloseButton)
		{
			pSysCloseButton = m_listSysButton.GetNext(pos);
			pSysCloseButton->SetButtonRect(rcClose);
		}
		else
		{
			CRect rcButton(rcClose);
			rcButton.right = pSysCloseButton->GetLeft();
			pSysCloseButton = m_listSysButton.GetNext(pos);
			pSysCloseButton->SetButtonRect(rcButton);
		}
	}

	int cxBorder = GetSystemMetrics(SM_CXDLGFRAME);
	int cyBorder = GetSystemMetrics(SM_CYDLGFRAME);
	int cxIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cyIcon = ::GetSystemMetrics(SM_CYSMICON);
	
	m_rcHitCaption = rcCaption;
	m_rcHitCaption.left = cxBorder + cxIcon;
	m_rcHitCaption.top = cyBorder;

	for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
	{
		SystemButton * pSysButton = m_listSysButton.GetNext(pos);
		m_rcHitCaption.right = pSysButton->GetLeft();
		break;
	}

	m_rcHitCaption.bottom -= cyBorder;

	m_rcHitIcon.left = cxBorder;
	m_rcHitIcon.top = cyBorder;
	m_rcHitIcon.right = cxIcon;
	m_rcHitIcon.bottom = rcCaption.bottom;

    CRect rtWindow;
    GetWindowRect(rtWindow);
    CRgn rgnUpper;
    rgnUpper.CreateRoundRectRgn(0, 0, rtWindow.Width() + 1, rtWindow.Height() - 4, 4, 4);
    CRgn rgnLower;
    rgnLower.CreateRectRgn(0, rtWindow.Height() - 10, rtWindow.Width() + 1, rtWindow.Height());
    CRgn rgnWnd;
    rgnUpper.CombineRgn(&rgnUpper, &rgnLower, RGN_OR);
    SetWindowRgn(rgnUpper, TRUE);

	DrawDialog();
}

CRect UIDialog::GetCaptionRect(void)
{
	CRect rtWindow;
	GetWindowRect(rtWindow);
	ScreenToClient(rtWindow);

	rtWindow.OffsetRect(-rtWindow.TopLeft());

	CRect rtCaption(rtWindow);
	CSize szDlgFrame(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME));

	rtCaption.right = rtCaption.right + szDlgFrame.cx;
	rtCaption.bottom = rtCaption.top + ::GetSystemMetrics(SM_CYCAPTION) + szDlgFrame.cy;

	return rtCaption;
}

void UIDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTCLOSE || nHitTest == HTMAXBUTTON || HTMINBUTTON == nHitTest)
	{
		for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
		{
			SystemButton *pSysButton = m_listSysButton.GetNext(pos);
			
			if (nHitTest == pSysButton->GetButtonHit())
			{
				pSysButton->SetLButtonDown(TRUE);
				SetCapture();
				break;
			}
			else
			{
				pSysButton->SetLButtonDown(FALSE);
			}
		}
		DrawDialog();
		return;
	}

	CPoint pt(point);
	ScreenToClient(&pt);

	pt.Offset(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME) + ::GetSystemMetrics(SM_CYCAPTION));


	if (m_rcHitCaption.PtInRect(pt))
	{
		nHitTest = HTCAPTION;
	}

	TRACE("[[[[[[[[[[[[%d]]]]]]]]]]]", nHitTest);

	//PostMessage(WS_SYSMENU); 
	DrawDialog();

	CDialog::OnNcLButtonDown(nHitTest, point);

    DrawDialog();
}

void UIDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UINT nSysCmd = 0;

	switch (nHitTest)
	{
	case HTCLOSE:
		nSysCmd = SC_CLOSE;
		break;
	case HTMAXBUTTON:
		nSysCmd = (GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE:SC_MAXIMIZE;
		break;
	case HTMINBUTTON:
		nSysCmd = IsIconic() ? SC_RESTORE:SC_MINIMIZE;
		break;
	}

	//
	CPoint pt(point);
	ScreenToClient(&pt);

	pt.Offset(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME) + ::GetSystemMetrics(SM_CYCAPTION));

	BOOL bSendCmd = FALSE;

	if (nSysCmd != 0)
	{
		for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
		{
			SystemButton *pSysButton = m_listSysButton.GetNext(pos);
			pSysButton->SetLButtonDown(FALSE);
			pSysButton->SetHover(FALSE);

			if (pSysButton->PtInRect(pt))
			{
				bSendCmd = TRUE;
			}
		}

		DrawDialog();
	}
	
	if (bSendCmd)
	{
		PostMessage(WM_SYSCOMMAND,nSysCmd,0);
	}

	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	return;
    
	//CDialog::OnNcLButtonUp(nHitTest, point);
}

void UIDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	DrawDialog();
}

void UIDialog::LoadSysButtons()
{
	CRect rcCaption = GetCaptionRect();

	CRect rcClose(rcCaption);
	rcClose.OffsetRect(-8,2);

	SystemButton *pSysButton = new SystemButton(HTCLOSE,rcClose,SystemButton::exitWidth);
	m_listSysButton.AddTail(pSysButton);

	const DWORD dwStyle = GetStyle();

	if ((dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX)
	{
		CRect rcMaxButton(rcClose);
		rcMaxButton.right = pSysButton->GetLeft();
		pSysButton = new SystemButton(HTMAXBUTTON,rcMaxButton,SystemButton::maxWidth);
		m_listSysButton.AddTail(pSysButton);
	}

	if ((dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX)
	{
		CRect rcMinButton(rcClose);
		rcMinButton.right = pSysButton->GetLeft();
		pSysButton = new SystemButton(HTMINBUTTON,rcMinButton,SystemButton::minWidth);
		m_listSysButton.AddTail(pSysButton);
	}
}

int UIDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	

	LoadSysButtons();

	return 0;
}

void UIDialog::OnDestroy()
{
	NcLButtonUpHook::UnHook(GetSafeHwnd());
	
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	for (POSITION pos = m_listSysButton.GetHeadPosition(); pos != NULL;)
	{
		delete m_listSysButton.GetNext(pos);
	}
}

void UIDialog::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt(point);
	ScreenToClient(&pt);

	pt.Offset(::GetSystemMetrics(SM_CXDLGFRAME),::GetSystemMetrics(SM_CYDLGFRAME) + ::GetSystemMetrics(SM_CYCAPTION));
	
	if (m_rcHitCaption.PtInRect(pt))
	{
		nHitTest = HTCAPTION;
	}

	DrawDialog();

	CDialog::OnNcMouseMove(nHitTest, point);
}

void UIDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	

	CDialog::OnLButtonUp(nFlags, point);
}

void UIDialog::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DrawDialog();

	CDialog::OnNcLButtonDblClk(nHitTest, point);
}


HBRUSH UIDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		//pDC->SetBkColor(DIALOG_BK_COLOR);
		pDC->SetBkMode(TRANSPARENT);
		return m_brushBkgnd;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void UIDialog::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here
	SendMessage(WM_NCPAINT);
}

void UIDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	DrawDialog();
	// Do not call CDialog::OnPaint() for painting messages
}

LRESULT UIDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class

    if (WM_NCPAINT == message || WM_NCACTIVATE == message)
    {
        TRACE("HTH\n");
        DrawDialog();
        return 1;
    }

    return CDialog::DefWindowProc(message, wParam, lParam);
}

void UIDialog::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(WS_CAPTION,0,0);

	CDialog::PreSubclassWindow();
}

BOOL UIDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreCreateWindow(cs);
}

BOOL UIDialog::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	ModifyStyle(WS_CAPTION,0,0);
	
	if (!CDialog::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}

void UIDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDown(nFlags, point);
}

void UIDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}

void UIDialog::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnNcRButtonDown(nHitTest, point);
}

void UIDialog::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnNcRButtonUp(nHitTest, point);
}

BOOL UIDialog::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return CDialog::OnEraseBkgnd(pDC);
}

void UIDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CRect rcMaximized;
    SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcMaximized, 0);
    //int cxBorder = GetSystemMetrics(SM_CXFRAME);
    //int cyBorder = GetSystemMetrics(SM_CYFRAME);
    //rcMaximized.InflateRect(cxBorder, cyBorder);
    lpMMI->ptMaxPosition.x = rcMaximized.left;
    lpMMI->ptMaxPosition.y = rcMaximized.top;
    lpMMI->ptMaxSize.x = rcMaximized.Width();
    lpMMI->ptMaxSize.y = rcMaximized.Height();
    //lpMMI->ptMinTrackSize = m_ptMinTrackSize;
    lpMMI->ptMaxTrackSize = lpMMI->ptMaxSize;

    CDialog::OnGetMinMaxInfo(lpMMI);
}


CString PngMetrics::sResFileName = UIDLG_RES_FILE;

HBITMAP PngMetrics::LoadPNG(UINT nID, DWORD cr)
{
    Gdiplus::Color crBak(GetRValue(cr), GetGValue(cr), GetBValue(cr));
    Gdiplus::Bitmap * pBitmap = NULL;
    HBITMAP hBitmap = NULL;

    // try file first
    WCHAR szDir[MAX_PATH] = {0};
    GetCurrentDirectoryW(MAX_PATH, szDir);
    CStringW wsDir(szDir);
    wsDir.Append(L"\\");
    wsDir.Append(ULDLG_PNG_FILE);
    pBitmap = Gdiplus::Bitmap::FromFile(wsDir);
    if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok)
    {
        goto Exit;
    }

    if (NULL == m_hResource)
    {
        m_hResource = theApp.m_hResInstance;/*LoadLibrary(sResFileName);*/
    }
    if (m_hResource == NULL)
    {
        OutputDebugString(_T("Load UIDialog PNG failed"));
        goto Exit;
    }

    HINSTANCE hinstRes = m_hResource;    

    HRSRC hResource = ::FindResource(hinstRes, MAKEINTRESOURCE(nID), L"PNG");   //   type 
    if (!hResource)
    {
        goto Exit;
    }

    DWORD imageSize = ::SizeofResource(hinstRes, hResource);
    if (!imageSize)
    {
        goto Exit;
    }

    const void* pResourceData = ::LockResource(::LoadResource(hinstRes, hResource));
    if (!pResourceData)
    {
        goto Exit;
    }

    void * hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);

    if (hBuffer)
    {
        void* pBuffer = ::GlobalLock(hBuffer);

        if (pBuffer)
        {
            CopyMemory(pBuffer, pResourceData, imageSize);

            IStream* pStream = NULL;
            if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
            {
                pBitmap = Gdiplus::Bitmap::FromStream(pStream);
                pStream->Release();
                if (pBitmap)
                { 
                    if (pBitmap->GetLastStatus() != Gdiplus::Ok)
                    {
                        delete pBitmap;
                        goto Exit;
                    }
                }
            }
            ::GlobalUnlock(hBuffer);
        }
        ::GlobalFree(hBuffer);
        hBuffer = NULL;
    }



Exit:    

    if(pBitmap)
    {
        pBitmap->GetHBITMAP(crBak, &hBitmap);
        delete pBitmap;
    }

    if (hBitmap)
    {
        BITMAP bm;
        ::GetObject(hBitmap, sizeof(BITMAP), &bm);
        nTotalHeight = bm.bmHeight;
        nTotalWidth = bm.bmWidth;
    }

    return hBitmap;
}

BOOL UIDialog::SystemButton::DrawButton(CDC * pDC, CDC * pBufferDC, BOOL bMax)
{
    int xSrc = 49;//31;
    int ySrc = 178; 

    int nStart = 0;

    if (m_bHover)
    {
        nStart = m_bLBDown?-1:1;
    }

    switch (m_nHit)
    {
    case HTMAXBUTTON:
        ySrc += sysHeight + 1;
        break;
    case HTMINBUTTON:
        ySrc += sysHeight + 1;
        ySrc += sysHeight + 1;
        break;
    case HTCLOSE:
        ySrc = 178;
        break;
    }

    if (m_nHit == HTMAXBUTTON && !bMax)
    {
        xSrc += ((maxWidth+1) * 3);
    }

    BOOL bOK = pDC->TransparentBlt(m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height(),
        pBufferDC, xSrc + nStart * (m_nWidth + 2),ySrc,m_rc.Width(),m_rc.Height(),UIDLG_COLOR_TRANSPARENT);

    return bOK;
}

void UIDialog::SetResFileName(CString sResFileName)
{
    PngMetrics::sResFileName = sResFileName;
}