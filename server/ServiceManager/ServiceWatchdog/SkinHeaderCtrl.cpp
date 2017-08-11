/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinHeaderCtrl.cpp
*   Description:    the header control for list control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "SkinHeaderCtrl.h"
#include "MemDC.h"

#define HEADER_HIGHT 29
#define HEADER_BORDER_TOP 1
#define HEADER_BORDER_BOTTOM 3

// CSkinHeaderCtrl

IMPLEMENT_DYNAMIC(CSkinHeaderCtrl, CHeaderCtrl)
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	m_bitmapBkgnd.LoadBitmap(IDB_BITMAP_LIST_HEADER);
}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
    ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()



// CSkinHeaderCtrl message handlers


void CSkinHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// draw the specified item
}

void CSkinHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CMemDC memDC(&dc, this);
	CDC * pDC = memDC.GetDC();
	
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap * pOldbitmap = dcMem.SelectObject(&m_bitmapBkgnd);
	pDC->StretchBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, 1, 29, SRCCOPY);
	dcMem.SelectObject(pOldbitmap);

	pDC->SetBkMode(TRANSPARENT);

	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	pDC->SetTextColor(RGB(0, 0, 0));

	CRect rcItem;
	int nItemCount = GetItemCount();

	HDITEM hdi;
	TCHAR  lpBuffer[256] = {0};

	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 255;
	
    CPen linPen, linPen2;
    linPen.CreatePen(PS_SOLID, 1, RGB(202, 202, 203));
    linPen2.CreatePen(PS_SOLID, 1, RGB(243, 243, 243));
    
    for (int i = 0; i < nItemCount; ++i)
	{
		GetItem(i, &hdi);
		GetItemRect(i, &rcItem);

        if (i >= 1)
        {
            pDC->SelectObject(&linPen2);
            pDC->MoveTo(rcItem.left - 1, rcItem.top - HEADER_BORDER_TOP);
            pDC->LineTo(rcItem.left - 1, rcItem.bottom - HEADER_BORDER_BOTTOM);
        }

        pDC->SelectObject(&linPen);
        pDC->MoveTo(rcItem.left, rcItem.top - HEADER_BORDER_TOP);
        pDC->LineTo(rcItem.left, rcItem.bottom - HEADER_BORDER_BOTTOM);

		//pDC->DrawText(lpBuffer, (int)strlen(lpBuffer), &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        rcItem.left += 6;
        pDC->DrawText(lpBuffer, (int)strlen(lpBuffer), &rcItem, DT_SINGLELINE | DT_VCENTER);
	}

	// draw last item border
    pDC->SelectObject(&linPen2);
    pDC->MoveTo(rcItem.right - 1, rcItem.top - HEADER_BORDER_TOP);
    pDC->LineTo(rcItem.right - 1, rcItem.bottom - HEADER_BORDER_BOTTOM);
    pDC->SelectObject(&linPen);
    pDC->MoveTo(rcItem.right, rcItem.top - HEADER_BORDER_TOP);
    pDC->LineTo(rcItem.right, rcItem.bottom - HEADER_BORDER_BOTTOM);

	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

void CSkinHeaderCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_headerFont.CreatePointFont(120, "MS Serif");
	
	SetFont(&m_headerFont);

	CHeaderCtrl::PreSubclassWindow();
}

BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	// do not erase the background.
	return FALSE;
}

LRESULT CSkinHeaderCtrl::OnLayout(WPARAM /*wParam*/, LPARAM lParam)
{
    LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
    HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam;
    RECT *prc = hdl.prc;
    WINDOWPOS *pwpos = hdl.pwpos;

    pwpos->cy = HEADER_HIGHT;
    prc->top = HEADER_HIGHT;

    return lResult;
}
