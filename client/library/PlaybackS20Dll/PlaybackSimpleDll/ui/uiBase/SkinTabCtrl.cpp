/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinTabCtrl.cpp
*   Description:    a owner draw tab control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinTabCtrl.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "SkinTabCtrl.h"
#include "../../PlaybackSimpleDll.h"
#include "MemDC.h"


// CSkinTabCtrl

IMPLEMENT_DYNAMIC(CSkinTabCtrl, CTabCtrl)
CSkinTabCtrl::CSkinTabCtrl()
{
	m_fontBarText.CreateFont(   
		12,                                                 //   nHeight   
		0,                                                   //   nWidth   
		0,                                                   //   nEscapement   
		0,                                                   //   nOrientation   
		/*FW_NORMAL*/FW_SEMIBOLD,                                   //   nWeight   
		FALSE,                                           //   bItalic   
		FALSE,                                           //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily   
		_T("ËÎÌו")); 
}

CSkinTabCtrl::~CSkinTabCtrl()
{
	m_fontBarText.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinTabCtrl message handlers


void CSkinTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	dc.SelectObject(GetFont());

	CRect rcClient;
	GetClientRect(&rcClient);
	
	CRect rcPage = rcClient;
	AdjustRect(FALSE, rcPage);

	//dc.ExcludeClipRect(&rcPage);

	// draw the tab control rect
	dc.FillSolidRect(rcClient,UI_COLOR_BKGND);

	rcClient.top = rcPage.top - 1;

    CPen penBlack;
    penBlack.CreatePen(PS_SOLID, 1, RGB(126, 129, 134));
    /*CPen* pOldPen = */dc.SelectObject(&penBlack);

  /*  dc.MoveTo(rcClient.left, rcClient.top - 1);
    dc.LineTo(rcClient.right, rcClient.top - 1);*/
	dc.Draw3dRect(rcClient, RGB(126, 129, 134), RGB(126, 129, 134));

	dc.SetBkMode(TRANSPARENT);

	// draw the tab border and text

	int nCount = GetItemCount();

	CRect rcItem;
	TCHAR  lpBuffer[256] = {0};

	TCITEM ti;
	ti.mask = TCIF_TEXT;
	ti.pszText = lpBuffer;
	ti.cchTextMax = 255;

	int nSel = GetCurSel();

	int i = 0;

	for (i = 0; i < nCount; ++i)
	{
		if (i != nSel)
		{
			GetItemRect(i, &rcItem);
			GetItem(i, &ti);

            rcItem.top += 2;

            rcItem.DeflateRect(2, 0);

			DrawTab(&dc, rcItem);
		
            rcItem.OffsetRect(0, 1);
			dc.DrawText(ti.pszText, rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}

	dc.SelectObject(&m_fontSel);

	// draw the current selected tab

	if (nSel != -1)
	{
		GetItemRect(nSel, &rcItem);
		GetItem(nSel, &ti);

        rcItem.DeflateRect(2, 0);

        rcItem.left -= 1;
		DrawTab(&dc, rcItem, TRUE);

        rcItem.OffsetRect(0, 2);
		dc.DrawText(ti.pszText, rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	if (!m_strShowInfo.IsEmpty())
	{
		int nOldMode = dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(&m_fontBarText);
		dc.SetTextColor(RGB(255,0,0));

		CRect rcSpeedInfo;
		GetClientRect(rcSpeedInfo);
		rcSpeedInfo.top += 2;
		rcSpeedInfo.bottom = rcSpeedInfo.top + 18;
		CSize sz;
		sz = dc.GetTextExtent(m_strShowInfo);
		rcSpeedInfo.left = rcClient.right - sz.cx;
		dc.DrawText(m_strShowInfo, rcSpeedInfo, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
	}
	// Do not call CTabCtrl::OnPaint() for painting messages
}

BOOL CSkinTabCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;

//	return CTabCtrl::OnEraseBkgnd(pDC);
}

/**************************************************************************
*   Function:		DrawTab
*   Description:    draw the tab item's in control
*   Input:          CDC * pDC: the CDC pointer
*					CRect & rcItem: the item rect
*					BOOL bSelected: the tab is selected or not
*   Output:         (null)
*   Return:			(null)
**************************************************************************/
void CSkinTabCtrl::DrawTab(CDC * pDC , const CRect &rcItem, BOOL bSelected)
{
    CDC bufferDC;
    bufferDC.CreateCompatibleDC(pDC);
    bufferDC.SelectObject(m_bitmap);

    if (!bSelected)
    {
        pDC->StretchBlt(rcItem.left,rcItem.top,4,rcItem.Height(),&bufferDC,0,0,4,22,SRCCOPY);
        pDC->StretchBlt(rcItem.left + 4,rcItem.top,rcItem.Width()-4-4,rcItem.Height(),&bufferDC,4,0,4,22,SRCCOPY);
        pDC->StretchBlt(rcItem.right - 4,rcItem.top,4,rcItem.Height(),&bufferDC,8,0,4,22,SRCCOPY);
    }
    else
    {
        pDC->StretchBlt(rcItem.left,rcItem.top,4,rcItem.Height(),&bufferDC,0,22,4,22,SRCCOPY);
        pDC->StretchBlt(rcItem.left + 4,rcItem.top,rcItem.Width()-4-4,rcItem.Height(),&bufferDC,4,22,4,22,SRCCOPY);
        pDC->StretchBlt(rcItem.right - 4,rcItem.top,4,rcItem.Height(),&bufferDC,8,22,4,22,SRCCOPY);

        CRect rc(rcItem.left,rcItem.bottom,rcItem.right,rcItem.bottom + 1);
        pDC->FillSolidRect(rc,RGB(183,183,183));
    }
}

void CSkinTabCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
    SetItemSize(CSize(22, 24));
    SetMinTabWidth(80);

	CFont * pFont = GetFont();

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfWeight = 550;

	m_fontSel.CreateFontIndirect(&lf);

	
    m_bitmap.Attach(RESOURCELOAD_PNG_EX_HBMPE(IDB_PNG_TAB_CONTROL));

	CTabCtrl::PreSubclassWindow();
}
