/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl_V30.cpp
*   Description:    the list ctrl has an owner draw header control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinListCtrl_V30.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "SkinListCtrl_V30.h"


// CSkinListCtrl_V30

IMPLEMENT_DYNAMIC(CSkinListCtrl_V30, CListCtrl)
CSkinListCtrl_V30::CSkinListCtrl_V30()
: m_iNumColumns(0)
, m_iSortColumn(-1)
, m_bSortAscending(TRUE)
, m_pDragImage(NULL)
{
} 

CSkinListCtrl_V30::~CSkinListCtrl_V30()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl_V30, CListCtrl)
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()



// CSkinListCtrl_V30 message handlers


void CSkinListCtrl_V30::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl * pHeaderCtrl = GetHeaderCtrl();

	if (pHeaderCtrl)
	{
		m_skinHeaderCtrl.SubclassWindow(pHeaderCtrl->m_hWnd);
	}

    SetBkColor(RGB(241, 241, 241));

	// set  the list control full row select and grid line
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

    ModifyStyle(0, LVS_OWNERDRAWFIXED, 0);

	CListCtrl::PreSubclassWindow();
}
#if 0
void CSkinListCtrl_V30::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lpLVCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	switch(lpLVCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
		{
			lpLVCustomDraw->clrTextBk = (0 == lpLVCustomDraw->nmcd.dwItemSpec % 2)?LIST_CTRL_ITEM2_CLR:LIST_CTRL_ITEM1_CLR;
		}

		break;

	default:
		break;    
	}

	*pResult = 0;

	/**pResult |= CDRF_NOTIFYPOSTPAINT;*/
	*pResult |= CDRF_NOTIFYITEMDRAW;
}
#endif


void CSkinListCtrl_V30::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // draw the specified item
    LV_ITEM lvi;
    _TCHAR szBuff[MAX_PATH] = {0};

    int nItem = lpDrawItemStruct->itemID;
    CRect rcItem(lpDrawItemStruct->rcItem);

    lvi.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM | LVIF_IMAGE; 
    lvi.iItem = nItem;
    lvi.iSubItem = 0;
    lvi.pszText = szBuff;
    lvi.cchTextMax = sizeof(szBuff);
    lvi.stateMask = 0xFFFF;	
    GetItem(&lvi);

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    ASSERT(pDC);

 /*   CRect rcClient;
    GetClientRect(&rcClient);
    rcClient.left = rcItem.left;
    rcClient.top = rcItem.top;
    rcClient.bottom = rcItem.bottom;*/
    pDC->FillSolidRect(rcItem/*rcClient*/, (nItem % 2 == 0)?UI_LISTITEMS_COLOR:UI_LISTITEMF_COLOR);

    BOOL bSelected = FALSE;

    // draw the effect of selected item

    if ((lpDrawItemStruct->itemState & ODS_SELECTED))
    {
        CRect rcSel(rcItem);
        rcSel.DeflateRect(1, 1);
        --rcSel.top;
        ++rcSel.right;

        CPen pen(PS_SOLID, 1, RGB(10,36,106));
        CBrush brush(UI_LISTSELECT_COLOR);
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

		pDC->FillSolidRect(rcItem, UI_LISTSELECT_COLOR);

        bSelected = TRUE;
    }

	CImageList * pImagelist = GetImageList(LVSIL_SMALL);

	BOOL bIsHaveIcon = FALSE;
    if (pImagelist)
    {
        CRect rcIcon; 
        GetItemRect(lpDrawItemStruct->itemID, &rcIcon, LVIR_ICON);
        POINT pt = {rcIcon.left, rcIcon.top};
        pImagelist->Draw(pDC, lvi.iImage, pt, ILD_TRANSPARENT);
		bIsHaveIcon = TRUE;
    }

    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH;

    int nOldClr = pDC->SetTextColor(UI_TEXT_COLOR/*bSelected?RGB(255,255,255):RGB(0,0,0)*/);

    // draw item text
    for (int nColumn = 0; GetColumn(nColumn, &lvc); ++nColumn)
    {
        CRect rcSubItem;
        GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rcSubItem);
        rcSubItem.left += 5;
		if (bIsHaveIcon && 0 == nColumn)
		{
			rcSubItem.left += 15;
		}
        pDC->DrawText(GetItemText(nItem, nColumn), -1, rcSubItem, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
    }

    pDC->SetTextColor(nOldClr);

#if 0   
    TCHAR  lpBuffer[256];   

	int iItem =  lpDrawItemStruct->itemID;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CImageList* pImagelist = GetImageList(LVSIL_SMALL );

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp;
	CRect rc;
	BITMAP btSize;

	CRect rcClient;
	GetClientRect(&rcClient);

	//绘制行颜色
	CRect itemRC;
	GetItemRect(iItem, &itemRC, LVIR_BOUNDS);
	COLORREF itemColor;
	itemColor = (iItem % 2 == 1)?LIST_CTRL_ITEM1_CLR:LIST_CTRL_ITEM2_CLR;;
	itemRC.right = rcClient.right;
	pDC->FillSolidRect(&itemRC,itemColor);

	LV_ITEM lvi;   

	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;   
	lvi.iItem = iItem ;     
	lvi.iSubItem = 0;   
	lvi.pszText = lpBuffer ;   
	lvi.cchTextMax = sizeof(lpBuffer);   
	VERIFY(GetItem(&lvi));   

	LV_COLUMN lvc, lvcprev ;   
	::ZeroMemory(&lvc, sizeof(lvc));   
	::ZeroMemory(&lvcprev, sizeof(lvcprev));   
	lvc.mask = LVCF_WIDTH | LVCF_FMT;   
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;   

	//判断是否被选中，原先这段代码在for语句中，但
	//如果列项超过1个，则相当于FillSolidRect了多次
	//这样前面的文字会被覆盖掉 -- 090919
	if ( lpDrawItemStruct->itemState & ODS_SELECTED )   
	{   
		//pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT)) ;  
		CRect rtItem;
		GetItemRect(iItem,&rtItem,LVIR_BOUNDS);
		pDC->FillSolidRect(&rtItem, GetSysColor(COLOR_HIGHLIGHT)) ;   
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;   
	}   
	else  
	{    
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)) ;    
	}
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT)); 

	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)   
	{   
		if ( nCol > 0 )    
		{   
			// Get Previous Column Width in order to move the next display item   
			GetColumn(nCol-1, &lvcprev) ;   
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;   
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left ;    
		}

		// Get the text    
		::ZeroMemory(&lvi, sizeof(lvi));   
		lvi.iItem = iItem;   
		lvi.mask = LVIF_TEXT | LVIF_PARAM;   
		lvi.iSubItem = nCol;   
		lvi.pszText = lpBuffer;   
		lvi.cchTextMax = sizeof(lpBuffer);   
		VERIFY(GetItem(&lvi));   

		//绘制图标
		CRect rcIcon; 
		GetItemRect(lpDrawItemStruct->itemID,&rcIcon, LVIR_ICON);
		if (!rcIcon.IsRectEmpty())
		{
			GetSubItemRect(iItem,nCol,LVIR_LABEL,rc);
			if (lvi.iImage != -1 )
			{
				CBitmap bmp;

				if (!GetImageFromList(pImagelist,lvi.iImage,&bmp))
				{
					return;
				}

				bmp.GetBitmap(&btSize);

				CRect tempRC; 
				tempRC = rcIcon;

				int y = (rc.Height() - 16)/2;
				tempRC.DeflateRect(0,y);             //移到中间

				pOldBmp = bitmapDC.SelectObject(&bmp);
				pDC->StretchBlt(tempRC.left,tempRC.top,btSize.bmWidth,btSize.bmHeight,&bitmapDC,0,0,btSize.bmWidth,btSize.bmHeight,SRCCOPY);

				bitmapDC.SelectObject(pOldBmp);
				bmp.DeleteObject();
			}
		}

		CRect rcItem;   
		GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem);  

		CRect rcText = rcItem;
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcText.OffsetRect(0,ofst/2);
		rcText.left +=1;
		UINT  uFormat    = DT_LEFT ;  

		::DrawText(lpDrawItemStruct->hDC, lpBuffer, (int)strlen(lpBuffer),    
			&rcText, uFormat) ; 
	}   
	pDC->SelectStockObject(SYSTEM_FONT) ;   
#endif
}
void CSkinListCtrl_V30::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;

	// if it's a second click on the same column then reverse the sort order,
	// otherwise sort the new column in ascending order.
	Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );

	*pResult = 0;
}
void CSkinListCtrl_V30::Sort( int iColumn, BOOL bAscending )
{
	m_iSortColumn = iColumn;
	m_bSortAscending = bAscending;

	// show the appropriate arrow in the header control.
	m_skinHeaderCtrl.SetSortArrow( m_iSortColumn, m_bSortAscending );

	VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}

bool IsNumber( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;

	return true;
}


int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	ASSERT_VALID_STRING( pszNumber1 );
	ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = _wtoi( pszNumber1 );
	const int iNumber2 = _wtoi( pszNumber2 );

	if( iNumber1 < iNumber2 )
		return -1;

	if( iNumber1 > iNumber2 )
		return 1;

	return 0;
}


bool IsDate( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	// format should be 99/99/9999.

	if( lstrlen( pszText ) != 10 )
		return false;

	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& pszText[ 2 ] == _T('/')
		&& _istdigit( pszText[ 3 ] )
		&& _istdigit( pszText[ 4 ] )
		&& pszText[ 5 ] == _T('/')
		&& _istdigit( pszText[ 6 ] )
		&& _istdigit( pszText[ 7 ] )
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}


int DateCompare( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = _wtoi( strDate1.Mid( 6, 4 ) );
	const int iYear2 = _wtoi( strDate2.Mid( 6, 4 ) );

	if( iYear1 < iYear2 )
		return -1;

	if( iYear1 > iYear2 )
		return 1;

	const int iMonth1 = _wtoi( strDate1.Mid( 3, 2 ) );
	const int iMonth2 = _wtoi( strDate2.Mid( 3, 2 ) );

	if( iMonth1 < iMonth2 )
		return -1;

	if( iMonth1 > iMonth2 )
		return 1;

	const int iDay1 = _wtoi( strDate1.Mid( 0, 2 ) );
	const int iDay2 = _wtoi( strDate2.Mid( 0, 2 ) );

	if( iDay1 < iDay2 )
		return -1;

	if( iDay1 > iDay2 )
		return 1;

	return 0;
}

int CALLBACK CSkinListCtrl_V30::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CSkinListCtrl_V30* pListCtrl = reinterpret_cast<CSkinListCtrl_V30*>( lParamData );
	ASSERT( pListCtrl->IsKindOf( RUNTIME_CLASS( CListCtrl ) ) );

	LVFINDINFO info;
	info.flags = LVFI_PARAM;
	info.lParam = lParam1;

	int nDex1 = pListCtrl->FindItem(&info);
	CString pszText1 = pListCtrl->GetItemText(nDex1,pListCtrl->m_iSortColumn);
	info.lParam = lParam2;
	int nDex2 = pListCtrl->FindItem(&info);
	CString pszText2 = pListCtrl->GetItemText(nDex2,pListCtrl->m_iSortColumn);



	//ItemData* pid1 = reinterpret_cast<ItemData*>( lParam1 );
	//ItemData* pid2 = reinterpret_cast<ItemData*>( lParam2 );

	//ASSERT( pid1 );
	//ASSERT( pid2 );


	//LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_iSortColumn ];
	//LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_iSortColumn ];


	ASSERT_VALID_STRING( pszText1 );
	ASSERT_VALID_STRING( pszText2 );

	if( IsNumber( pszText1 ) )
		return pListCtrl->m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
	else if( IsDate( pszText1 ) )
		return pListCtrl->m_bSortAscending ? DateCompare( pszText1, pszText2 ) : DateCompare( pszText2, pszText1 );
	else
		// text.
		return pListCtrl->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
}
