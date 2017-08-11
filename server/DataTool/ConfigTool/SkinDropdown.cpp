/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinDropdown.cpp
*   Description:    5.2服务器中ip地址控件的下拉按钮
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Friday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/

// SkinDropdown.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SkinDropdown.h"
#include "ConfigToolDlg.h"


// CSkinDropdown

IMPLEMENT_DYNAMIC(CSkinDropdown, CStatic)
CSkinDropdown::CSkinDropdown()
{
}

CSkinDropdown::~CSkinDropdown()
{
}


BEGIN_MESSAGE_MAP(CSkinDropdown, CStatic)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSkinDropdown message handlers


void CSkinDropdown::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    CRect rcWnd;
    GetWindowRect(&rcWnd);
    ScreenToClient(&rcWnd);

    // draw border
    COLORREF clrBk =  dc.GetBkColor();

    dc.FillSolidRect(rcWnd, clrBk);
    
    COLORREF clrHiLite	 =	GetSysColor(COLOR_BTNHIGHLIGHT);
	COLORREF clrShadow	 =	GetSysColor(COLOR_BTNSHADOW);
	COLORREF clrDarkShadow =	GetSysColor(COLOR_3DDKSHADOW);
	COLORREF clrButton	 =	GetSysColor(COLOR_3DLIGHT);
   
    dc.Draw3dRect (&rcWnd, clrShadow, clrHiLite);
    rcWnd.DeflateRect (1,1);
    //dc.Draw3dRect (&rcWnd, clrDarkShadow, clrButton);

    CRect rcButton = rcWnd;
    rcButton.DeflateRect(1, 1);
    rcButton.OffsetRect(1, 0);

	// 画倒三角形按钮
    POINT point[3];
    point[0].x = rcButton.left + (rcButton.Width() - 5) / 2;
    point[0].y = rcButton.top + (rcButton.Height() - 1) / 2;
  
    point[1].x = point[0].x + 4;
    point[1].y = point[0].y;
  
    point[2].x = point[0].x + 2;
    point[2].y = point[0].y + 2;

    dc.SelectObject(GetStockObject(BLACK_BRUSH));

    dc.Polygon(point, 3);

    rcWnd.SetRect(rcWnd.left - 1, rcWnd.top + 1, 2, rcWnd.Height());
    dc.FillSolidRect(&rcWnd, clrBk);

    dc.SetPixel(rcWnd.left, rcWnd.top - 1, clrDarkShadow);
    dc.SetPixel(rcWnd.left, rcWnd.bottom, clrButton);
    // Do not call CStatic::OnPaint() for painting messages
}

void CSkinDropdown::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Add your message handler code here and/or call default
    CConfigToolDlg * pDLG = (CConfigToolDlg * )GetParent();

	// 点击按钮告知parent需要改变ip地址
    pDLG->IPHistoryDropdown();

    CStatic::OnLButtonDown(nFlags, point);
}
