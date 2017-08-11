/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinButton.cpp
*   Description:    5.2各服务器基本按钮类
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/

// SkinButton.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "SkinButton.h"
#include "utils.h"
#include "MainDialog.h"

#define COLER_RED       RGB(255, 0, 0)
#define COLER_GREEN     RGB(0, 255, 0)

#define COLER_SELECTED_BLUE         RGB(52, 52, 92)
#define COLER_HOVER_BLUE            RGB(0, 0, 0)
#define COLER_TEXT_DEFAULT_BLUE     RGB(52, 52, 0)


#define COLER_TEXT_DEFAULT_RED_BLACK    RGB(192, 192, 192)

// CSkinButton

IMPLEMENT_DYNAMIC(CSkinButton, CButton)
CSkinButton::CSkinButton()
{
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;

    m_bIsBlackAndRed = FALSE;

	//m_clrText = RGB(52, 52, 0);

	m_hIcon = NULL;
    m_pImage = NULL;
}

CSkinButton::~CSkinButton()
{
    try
    {
        if (m_pImage)
        {
            delete m_pImage;
        }
    }
    catch (...)
    {
    }
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()



// CSkinButton message handlers

void CSkinButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	int nWidth = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBitmap);

	if (GetParent()->IsKindOf(RUNTIME_CLASS(CMainDialog)))
	{
		((CSkinDialog *)GetParent())->TransparentBkgnd(&memDC, this);
	}
	else
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);
		memDC.FillSolidRect(0, 0, rc.right - rc.left, rc.bottom - rc.top, RGB(203, 203, 207));
	}

	Graphics graph(memDC.m_hDC);

	RectF rcF(0, 0, (REAL)nWidth, (REAL)nHeight);

	UINT state = lpDrawItemStruct->itemState;

	COLORREF clrText;

	int nStart = 0;

    if (TRUE == m_bIsBlackAndRed)
    {
        clrText = COLER_TEXT_DEFAULT_RED_BLACK;
        if (state & ODS_SELECTED)
        {
            nStart = 2;
            clrText = GetSysColor(COLOR_BTNFACE);
        }
        else if (state & ODS_DISABLED)
        {
            nStart = 3;
            clrText = COLER_TEXT_DEFAULT_RED_BLACK;
        }
        else if (m_bHover)
        {
            nStart = 1;
            clrText = GetSysColor(COLOR_BTNFACE);
        }
    }
    else
    {
        clrText = COLER_TEXT_DEFAULT_BLUE;
        if (state & ODS_SELECTED)
        {
            nStart = 2;
            clrText = COLER_SELECTED_BLUE;
        }
        else if (state & ODS_DISABLED)
        {
            nStart = 3;
            clrText = COLER_TEXT_DEFAULT_BLUE;
        }
        else if (m_bHover)
        {
            nStart = 1;
            clrText = COLER_HOVER_BLUE;
        }
    }

	graph.DrawImage(m_pImage, rcF, (REAL)(nStart * nWidth), (REAL)(0), (REAL)(nWidth), (REAL)(nHeight), UnitPixel);
	
	// draw text 
	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		memDC.SetBkMode(TRANSPARENT);
		memDC.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		CRect rcText(&(lpDrawItemStruct->rcItem));

		if (state & ODS_SELECTED)
		{
			rcText.OffsetRect(1, 1);
		}

		memDC.SelectObject(&m_font);
		
		memDC.SetTextColor(clrText);

		memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		if (m_hIcon)
		{
			DrawIconEx(memDC.m_hDC, rcText.right - 14, rcText.top + 9, m_hIcon, 9, 9, 0, NULL, DI_NORMAL | DI_COMPAT); 
		}
	}

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
}

BOOL CSkinButton::OnEraseBkgnd(CDC* /*pDC*/)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;

//	return CButton::OnEraseBkgnd(pDC);
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bMouseTrack)
	{
		Invalidate();

		TRACKMOUSEEVENT eventTrack;
		eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		eventTrack.hwndTrack = m_hWnd;
		eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
		eventTrack.dwHoverTime = 1;
		m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CSkinButton::OnMouseHover(WPARAM /*wparam*/, LPARAM /*lparam*/) 
{
	// 鼠标移入按钮
	m_bHover = TRUE;
	
	Invalidate();

	return 0;
}

LRESULT CSkinButton::OnMouseLeave(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	// 鼠标离开
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return 0;
}


BOOL CSkinButton::PreTranslateMessage(MSG* pMsg)
{
	// Add your specialized code here and/or call the base class

	// change double click to click
	// if do not, when double click the button quickly,
	// the button pressed state will not be right
	// so that the button press state bitmap will not be drawn right.
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;	
	}

	return CButton::PreTranslateMessage(pMsg);
}

/**************************************************************************
*   Function:		Load
*   Description:    load button background by resource id 
*   Input:          UINT uID: the resource id
*					BOOL bSizeToContent: move the button size as the background bitmap size
*   Output:         (null)
*   Return:			return void
**************************************************************************/
void CSkinButton::Load(UINT uID, BOOL bLargeFont)
{
	//m_pImage = LoadImageFromResource(uID);
    m_pImage = new Bitmap(AfxGetResourceHandle(), MAKEINTRESOURCEW(uID));

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	rc.right = rc.left + m_pImage->GetWidth() / 4;
	rc.bottom = rc.top + m_pImage->GetHeight();

	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0);

	LOGFONT lf = {0};
	lf.lfWeight = bLargeFont?FW_SEMIBOLD:FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, "Arial");

	m_font.CreateFontIndirect(&lf);
}

void CSkinButton::LiteIcon(HICON hIcon)
{
	m_hIcon = hIcon;

	Invalidate();
}

void CSkinButton::SetFlags(BOOL bIsBlackAndRed)
{
    m_bIsBlackAndRed = bIsBlackAndRed;
}
