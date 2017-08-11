/**
*   @file   SkinTabCtrl.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*  
*   @author mazhentong
*   @date   2012/11/03
*
*   @note
*
*   @warning
*/

#include "stdafx.h"
#include "ServiceWatchdog.h"

#include "SkinTabCtrl.h"

IMPLEMENT_DYNAMIC(CSkinTabCtrl, CTabCtrl)
CSkinTabCtrl::CSkinTabCtrl()
{
}

CSkinTabCtrl::~CSkinTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CSkinTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	dc.SelectObject(GetFont());

	CRect rcClient;
	GetClientRect(&rcClient);
	
	CRect rcPage = rcClient;
	AdjustRect(FALSE, rcPage);

	dc.ExcludeClipRect(&rcPage);

	// draw the tab control rect

    dc.FillSolidRect(rcClient, RGB(200, 220, 255));

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
 
             rcItem.top += 1;
 
             rcItem.DeflateRect(1, 0);
 
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

}


BOOL CSkinTabCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    // TODO: Add your message handler code here and/or call default
    return FALSE;

    //	return CTabCtrl::OnEraseBkgnd(pDC);
}


void CSkinTabCtrl::DrawTab(CDC * pDC , const CRect & rcItem, BOOL bSelected)
{
    CDC bufferDC;
    bufferDC.CreateCompatibleDC(pDC);
    bufferDC.SelectObject(m_bitmap);

    if (!bSelected)
    {
        pDC->StretchBlt(rcItem.left, rcItem.top, 4, rcItem.Height(), &bufferDC, 0, 0, 4, 22,SRCCOPY);
        pDC->StretchBlt(rcItem.left + 4, rcItem.top, rcItem.Width()-4-4, rcItem.Height(), &bufferDC, 4, 0, 4, 22, SRCCOPY);
        pDC->StretchBlt(rcItem.right - 4, rcItem.top, 4, rcItem.Height(), &bufferDC, 8, 0, 4, 22, SRCCOPY);
    }
    else
    {
        pDC->StretchBlt(rcItem.left, rcItem.top, 4, rcItem.Height(), &bufferDC, 0, 22, 4, 22, SRCCOPY);
        pDC->StretchBlt(rcItem.left + 4, rcItem.top, rcItem.Width()-4-4, rcItem.Height(), &bufferDC, 4, 22, 4, 22, SRCCOPY);
        pDC->StretchBlt(rcItem.right - 4, rcItem.top, 4, rcItem.Height(), &bufferDC, 8, 22, 4, 22, SRCCOPY);

        CRect rc(rcItem.left,rcItem.bottom,rcItem.right,rcItem.bottom + 1);
        pDC->FillSolidRect(rc,RGB(183,183,183));
    }
}

void CSkinTabCtrl::PreSubclassWindow()
{
    SetItemSize(CSize(22, 24));
    SetMinTabWidth(90);

    CFont * pFont = GetFont();

    LOGFONT lf;
    pFont->GetLogFont(&lf);
    lf.lfWeight = 550;

    m_fontSel.CreateFontIndirect(&lf);

    m_bitmap.LoadBitmap(IDB_BITMAP_TAB_CONTROL);

    CTabCtrl::PreSubclassWindow();
}