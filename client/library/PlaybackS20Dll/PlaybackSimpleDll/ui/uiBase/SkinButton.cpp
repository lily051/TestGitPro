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
//lint -library
#include "stdafx.h"
#include "SkinButton.h"
#include "../util/utils.h"


// CSkinButton

IMPLEMENT_DYNAMIC(CSkinButton, CButton)
CSkinButton::CSkinButton()
{
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;

	m_clrText = RGB(192, 192, 192);

	m_pImage = NULL;
	m_pImageSel = NULL;
	m_hIcon = NULL;
}

CSkinButton::~CSkinButton()
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	if(m_pImageSel)
	{

		delete m_pImageSel;
		m_pImageSel = NULL;
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
	if(m_pImage == NULL)
	{
		return;
	}
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	int nWidth = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBitmap);

	//if (GetParent()->IsKindOf(RUNTIME_CLASS(CDBConfigDialog))
	//	|| GetParent()->IsKindOf(RUNTIME_CLASS(CLoginDlg))
	//	/*|| GetParent()->IsKindOf(RUNTIME_CLASS(CStreamServerDlg))*/)
	//{
	//	((CSkinDialog *)GetParent())->TransparentBkgnd(&memDC, this);
	//}
	//else
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);
		memDC.FillSolidRect(0, 0, rc.right - rc.left, rc.bottom - rc.top, RGB(203, 203, 207));
	}

	Graphics graph(memDC.m_hDC);

	Rect rcF(0, 0, nWidth, nHeight);

	UINT state = lpDrawItemStruct->itemState;

	COLORREF clrText = m_clrText;



	Image * pImage = NULL;
	int nStart = 0;

	if (state & ODS_SELECTED)
	{
		nStart = 2;
		clrText = RGB(64,64,64);//GetSysColor(COLOR_GRAYTEXT);

		if(m_pImageSel)
		{

			pImage = m_pImageSel;
		}else
		{

			pImage = m_pImage;
		}
	}
	else if (state & ODS_DISABLED)
	{
		nStart = 3;
		//clrText = GetSysColor(COLOR_GRAYTEXT);
		clrText = RGB(255, 255, 255);
		pImage = m_pImage;
	}
	else if (m_bHover)
	{
		nStart = 1;
		clrText = RGB(64,64,64);

		if(m_pImageSel)
		{

			pImage = m_pImageSel;
		}else
		{

			pImage = m_pImage;
		}
	}else
	{
		clrText = GetSysColor(COLOR_BTNTEXT);

		pImage = m_pImage;
	}

	if(pImage)
	{

		graph.DrawImage(pImage, rcF, 0, 0, nWidth, nHeight, UnitPixel);
	}

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

		// 如果需要画icon，写窗口文字时留出icon的位置 [6/28/2009 guolinlin]
		if (m_hIcon)
		{
			rcText.right -= 14;
			memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			DrawIconEx(memDC.m_hDC, rcText.right, rcText.top + 9, m_hIcon, 9, 9, 0, NULL, DI_NORMAL | DI_COMPAT); 
		}else
		{
			memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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
	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	m_pImage = LoadImageFromResource(uID);

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	rc.right = rc.left + m_pImage->GetWidth() / 4;
	rc.bottom = rc.top + m_pImage->GetHeight();

	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0);

	LOGFONT lf = {0};
	lf.lfWeight = bLargeFont?FW_SEMIBOLD:FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, _T("Arial"));

	m_font.DeleteObject();

	m_font.CreateFontIndirect(&lf);
}

/**************************************************************************
*   Function:		Load//guolinlin
*   Description:    load button background by resource id 
*   Input:          pImage: the resource
*					BOOL bSizeToContent: move the button size as the background bitmap size
*   Output:         (null)
*   Return:			return void
**************************************************************************/
void CSkinButton::Load(Image * pImage, BOOL bLargeFont)
{
	if(pImage == NULL)
	{
		return;
	}

	if(m_pImage)
	{

		delete m_pImage;
		m_pImage = NULL;
	}// 删除原图片 [6/28/2009 guolinlin]

	m_pImage = pImage;

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	rc.right = rc.left + m_pImage->GetWidth();// / 4;
	rc.bottom = rc.top + m_pImage->GetHeight();

	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0);

	LOGFONT lf = {0};
	lf.lfWeight = bLargeFont?FW_SEMIBOLD:FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, _T("Arial"));

	m_font.DeleteObject();

	m_font.CreateFontIndirect(&lf);

}

void CSkinButton::Load(Image * pImage, Image * pImageSel,BOOL bLargeFont)
{
	if(pImage == NULL)
	{

		return;
	}

	if(pImageSel == NULL)
	{

		return;
	}

	if(m_pImage)
	{

		delete m_pImage;
		m_pImage = NULL;
	}// 删除原图片 [6/28/2009 guolinlin]

	if(m_pImageSel)
	{
		delete m_pImageSel;
		m_pImageSel = NULL;
	}

	m_pImage = pImage;
	m_pImageSel = pImageSel;

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	rc.right = rc.left + m_pImage->GetWidth();
	rc.bottom = rc.top + m_pImage->GetHeight();

	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0);

	LOGFONT lf = {0};
	lf.lfWeight = bLargeFont?FW_SEMIBOLD:FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, _T("Arial"));

	m_font.DeleteObject();

	m_font.CreateFontIndirect(&lf);

}

void CSkinButton::LiteIcon(HICON hIcon)
{
	m_hIcon = hIcon;

	Invalidate();
}

