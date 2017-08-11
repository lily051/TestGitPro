/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinHeaderCtrl_V30.cpp
*   Description:    the header control for list control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinHeaderCtrl_V30.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "../../Resource.h"
#include "SkinHeaderCtrl_V30.h"
#include "ListMemDC.h"



// CSkinHeaderCtrl_V30

IMPLEMENT_DYNAMIC(CSkinHeaderCtrl_V30, CHeaderCtrl)
CSkinHeaderCtrl_V30::CSkinHeaderCtrl_V30()
: m_bSortAscending(FALSE)
, m_iSortColumn(0)
{
}

CSkinHeaderCtrl_V30::~CSkinHeaderCtrl_V30()
{
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl_V30, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinHeaderCtrl_V30 message handlers
void CSkinHeaderCtrl_V30::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	m_iSortColumn = iSortColumn;
	m_bSortAscending = bSortAscending;

	// change the item to owner drawn.
	HD_ITEM hditem;

	hditem.mask = HDI_FORMAT;
	VERIFY( GetItem( iSortColumn, &hditem ) );
	hditem.fmt |= HDF_OWNERDRAW;
	VERIFY( SetItem( iSortColumn, &hditem ) );

	// invalidate the header control so it gets redrawn
	Invalidate();
}


void CSkinHeaderCtrl_V30::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// draw the specified item
	// attath to the device context.
	CDC dc;
	VERIFY( dc.Attach( lpDrawItemStruct->hDC ) );

	// save the device context.
	const int iSavedDC = dc.SaveDC();

	// get the column rect.
	CRect rc( lpDrawItemStruct->rcItem );

	// set the clipping region to limit drawing within the column.
	CRgn rgn;
	VERIFY( rgn.CreateRectRgnIndirect( &rc ) );
	(void)dc.SelectObject( &rgn );
	VERIFY( rgn.DeleteObject() );

	// draw the background,
	CBrush brush( GetSysColor( COLOR_3DFACE ) );
	dc.FillRect( rc, &brush );

	// get the column text and format.
	TCHAR szText[ 256 ];
	HD_ITEM hditem;

	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = szText;
	hditem.cchTextMax = 255;

	VERIFY( GetItem( lpDrawItemStruct->itemID, &hditem ) );

	// determine the format for drawing the column label.
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS ;

	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;

	// adjust the rect if the mouse button is pressed on it.
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rc.left++;
		rc.top += 2;
		rc.right++;
	}

	CRect rcIcon( lpDrawItemStruct->rcItem );
	const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;

	// adjust the rect further if the sort arrow is to be displayed.
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
		rc.right -= 3 * iOffset;

	rc.left += iOffset;
	rc.right -= iOffset;

	// draw the column label.
	if( rc.left < rc.right )
		(void)dc.DrawText( szText, -1, rc, uFormat );

	// draw the sort arrow.
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
	{
		// set up the pens to use for drawing the arrow.
		CPen penLight( PS_SOLID, 1, /*GetSysColor( COLOR_3DHILIGHT )*/ RGB(255,255,255));
		CPen penShadow( PS_SOLID, 1, /*GetSysColor( COLOR_3DSHADOW )*/  RGB(255,255,255));
		CPen* pOldPen = dc.SelectObject( &penLight );

		if( m_bSortAscending )
		{
			// draw the arrow pointing upwards.
			dc.MoveTo( rcIcon.right - 2 * iOffset, iOffset);
			dc.LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);		
		}
		else
		{
			// draw the arrow pointing downwards.
			dc.MoveTo( rcIcon.right - iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
			dc.LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - iOffset - 1, iOffset );		
		}

		// restore the pen.
		(void)dc.SelectObject( pOldPen );
	}

	// restore the previous device context.
	VERIFY( dc.RestoreDC( iSavedDC ) );

	// detach the device context before returning.
	(void)dc.Detach();


}

void CSkinHeaderCtrl_V30::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CLsitMemDC memDC(&dc, this);
	CDC * pDC = memDC.GetDC();
	
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	CRect rcClient;
	GetClientRect(&rcClient);

	pDC->FillSolidRect(rcClient, UI_LISTHEADER_COLOR);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
// 	CBitmap * pOldbitmap = dcMem.SelectObject(&m_bitmapBkgnd);
// 	Graphics graph(dcMem.m_hDC);
// 	Rect rc(0,0,rcClient.Width(),rcClient.Height());
// 	//graph.DrawImage(m_pBitmap, rc);
// 	pDC->StretchBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, 1, 21, SRCCOPY);
// 	dcMem.SelectObject(pOldbitmap);

	pDC->SetBkMode(TRANSPARENT);

	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	pDC->SetTextColor(UI_TEXT_COLOR);

	CRect rcItem;
	int nItemCount = GetItemCount();

	HDITEM hdi;
	TCHAR  lpBuffer[256] = {0};

	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 255;

    CPen linPen;
    linPen.CreatePen(PS_SOLID, 1, RGB(145, 145, 145));
    pDC->SelectObject(&linPen);
	
	for (int i = 0; i < nItemCount; ++i)
	{
		GetItem(i, &hdi);
		GetItemRect(i, &rcItem);

		pDC->MoveTo(rcItem.left, rcItem.top + 2);
		pDC->LineTo(rcItem.left, rcItem.bottom - 2);
		pDC->DrawText(lpBuffer, (int)wcslen(lpBuffer), &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

	// draw last item border
	pDC->MoveTo(rcItem.right, rcItem.top + 2);
	pDC->LineTo(rcItem.right, rcItem.bottom - 2);

	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

void CSkinHeaderCtrl_V30::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_headerFont.CreatePointFont(120, _T("MS Serif"));
	
	SetFont(&m_headerFont);

	CHeaderCtrl::PreSubclassWindow();
}
void CSkinHeaderCtrl_V30::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		const int iItemCount = GetItemCount();
		if( iItemCount != -1 )
		{
			ar << iItemCount;

			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				VERIFY( GetItem( i, &hdItem ) );
				ar << hdItem.cxy;
			}
		}
	}
	else
	{
		int iItemCount;
		ar >> iItemCount;

		if( GetItemCount() != iItemCount )
			TRACE0("Different number of columns in registry.");
		else
		{
			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				ar >> hdItem.cxy;
				VERIFY( SetItem( i, &hdItem ) );
			}
		}
	}
}

BOOL CSkinHeaderCtrl_V30::OnEraseBkgnd(CDC* /*pDC*/)
{
	// do not erase the background.
	return FALSE;
}
