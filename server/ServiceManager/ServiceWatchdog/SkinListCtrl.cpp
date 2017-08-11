/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl.cpp
*   Description:    the list ctrl has an owner draw header control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "SkinListCtrl.h"

#define LIST_CTRL_BK_CLR RGB(245, 245, 247)
#define LIST_CTRL_BK_CLR2 RGB(/*230*/192, /*230*/192, /*232*/192)

// CSkinListCtrl

IMPLEMENT_DYNAMIC(CSkinListCtrl, CListCtrl)
CSkinListCtrl::CSkinListCtrl()
{
    m_nItemHeight = 20;
}

CSkinListCtrl::~CSkinListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



// CSkinListCtrl message handlers


void CSkinListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl * pHeaderCtrl = GetHeaderCtrl();

	if (pHeaderCtrl)
	{
		m_skinHeaderCtrl.SubclassWindow(pHeaderCtrl->m_hWnd);
	}

    // 获得原有风格。
    DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
    dwStyle &= ~(LVS_TYPEMASK);
    dwStyle &= ~(LVS_EDITLABELS);

    // 设置新风格。
    ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED);

    // 设置扩展风格。
    dwStyle = GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES;
    SetExtendedStyle(dwStyle);

	SetBkColor(LIST_CTRL_BK_CLR);
	SetTextColor(RGB(0, 0, 0));

	CListCtrl::PreSubclassWindow();
}

void CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    int nItem = lpDrawItemStruct->itemID;
    CRect rcItem(lpDrawItemStruct->rcItem);

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    ASSERT(pDC);

    pDC->FillSolidRect(rcItem, (nItem % 2 == 0) ? LIST_CTRL_BK_CLR : LIST_CTRL_BK_CLR2);

    BOOL bSelected = FALSE;
    if ((lpDrawItemStruct->itemState & ODS_SELECTED))
    {
        CRect rcSel(rcItem);
        rcSel.DeflateRect(1, 1);
        --rcSel.top;
        ++rcSel.right;

        CPen pen(PS_SOLID, 1, RGB(10,36,106));
        CBrush brush(RGB(193, 210, 238));
        CPen * pOldPen = pDC->SelectObject(&pen);
        CBrush * pOldBrush = pDC->SelectObject(&brush);

        //pDC->RoundRect(rcSel, CPoint(2, 2));

        rcSel.DeflateRect(1,1);

        COLORREF rgbSelectTop = RGB(10,36,106);
        COLORREF rgbSelectBottom = RGB(0,0,128);

        GRADIENT_RECT grdRect = { 0, 1 };
        TRIVERTEX triVertext[ 2 ] = {
            rcSel.left,
            rcSel.top,
            GetRValue( rgbSelectTop) << 8,
            GetGValue( rgbSelectTop) << 8,
            GetBValue( rgbSelectTop) << 8,
            0x0000,			
            rcSel.right,
            rcSel.bottom,
            GetRValue(rgbSelectBottom) << 8,
            GetGValue(rgbSelectBottom) << 8,
            GetBValue(rgbSelectBottom) << 8,
            0x0000
        };

        pDC->GradientFill( triVertext, 2, &grdRect, 1, GRADIENT_FILL_RECT_V );

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);

        bSelected = TRUE;
    }

    COLORREF nOldClr = pDC->SetTextColor(bSelected ? RGB(255,255,255) : RGB(0,0,0));

    CImageList * pImagelist = GetImageList(LVSIL_SMALL);
    LV_ITEM lvi;

    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH;

    for (int nColumn = 0; GetColumn(nColumn, &lvc); ++nColumn)
    {
        lvi.mask = LVIF_IMAGE; 
        lvi.iItem = nItem;
        lvi.iSubItem = nColumn;
        GetItem(&lvi);

        CRect rcSubItem;
        GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rcSubItem);

        rcSubItem.left += 6;

        if (lvi.iImage >= 0)
        {
            CRect rcIcon; 
            GetSubItemRect(nItem, nColumn, LVIR_ICON, rcIcon);
            POINT pt = { rcIcon.left, rcIcon.top };
            pImagelist->Draw(pDC, lvi.iImage, pt, ILD_TRANSPARENT);

            rcSubItem.left += 16;
        }

        pDC->DrawText(GetItemText(nItem, nColumn), -1, rcSubItem, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
    }

    pDC->SetTextColor(nOldClr);
}

void CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if( m_nItemHeight > 0 )
    {
        lpMeasureItemStruct->itemHeight = m_nItemHeight;
    }
}

void CSkinListCtrl::SetItemHeight(UINT nHeight)
{
    m_nItemHeight = nHeight;

    CRect rcWin;
    GetWindowRect(&rcWin);
    WINDOWPOS wp;
    wp.hwnd = m_hWnd;
    wp.cx = rcWin.Width();
    wp.cy = rcWin.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

    SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}


void CSkinListCtrl::SetHeaderStyle(void)
{
    CHeaderCtrl * pHeaderCtrl = GetHeaderCtrl();

    if (pHeaderCtrl)
    {
        m_skinHeaderCtrl.SubclassWindow(pHeaderCtrl->m_hWnd);
    }

    // 获得原有风格。
    DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
    dwStyle &= ~(LVS_TYPEMASK);
    dwStyle &= ~(LVS_EDITLABELS);

    // 设置新风格。
    ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED);

    // 设置扩展风格
    dwStyle = GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES;
    SetExtendedStyle(dwStyle);

    SetBkColor(LIST_CTRL_BK_CLR);
    SetTextColor(RGB(0, 0, 0));
}
