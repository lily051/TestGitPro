/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl.cpp
*   Description:    the list ctrl has an owner draw header control
*   Author:         Qimengxing (Qimx@hikvision.com)
*   Date:           2010-4-12
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2010-4-12      Qimengxing       created
**************************************************************************/

// SkinListCtrl.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "../resource.h"
#include "SkinListCtrl.h"



// CSkinListCtrl
IMPLEMENT_DYNAMIC(CSkinListCtrl, CListCtrl)
CSkinListCtrl::CSkinListCtrl(UINT nitemHeight/* = 31*/, int nShowProSubItem /*= -1*/)
: m_iNumColumns( 0 )
, m_bSortAscending( TRUE )
, m_nitemHeight(nitemHeight)
, m_nShowProSubItem(nShowProSubItem)
{
}

CSkinListCtrl::~CSkinListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_ERASEBKGND()
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

    SetBkColor(RGB(255, 255, 255));

	// set the list control full row select and grid line
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    ModifyStyle(0, LVS_OWNERDRAWFIXED, 0);

	//列表头部
	HBITMAP hLeft,hMid,hRight;
	hLeft = RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_LISTTITLE_LEFT,RGB(255,255,255));
	hMid = RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_LISTTITLE_MID,RGB(255,255,255));
	hRight = RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_LISTTITLE_RIGHT,RGB(255,255,255));
	m_skinHeaderCtrl.SetListTitleBMP(hMid,hMid,hMid);
	DeleteObject(hLeft);
	DeleteObject(hMid);
	DeleteObject(hRight);
	CListCtrl::PreSubclassWindow();
}

#define PRO_AREA_LEFT		RGB(31,176,19)		//List内进度条左边颜色
#define PRO_AREA_RIGHT		RGB(255,255,255)	//List内进度条右边颜色
#define PRO_TEXT_LEFT		RGB(255,255,255)	//List内进度条左边字体颜色
#define PRO_TEXT_RIGHT		RGB(0,0,0)			//List内进度条右边字体颜色
#define PRO_FRAME			RGB(238,238,238)	//List内进度条边框颜色
#define LIST_ITEMS			RGB(246,246,246)	//奇数行颜色
#define LIST_ITEMF			RGB(255,255,255)	//偶数行颜色
void CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
    pDC->FillSolidRect(rcItem/*rcClient*/, (nItem % 2 == 0)?LIST_ITEMS:LIST_ITEMF);

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

	int nOldClr = pDC->SetTextColor(bSelected ? 0x00ffffff - UI_LISTSELECT_COLOR:UI_TEXT_COLOR/*bSelected?RGB(255,255,255):RGB(0,0,0)*/);

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
		if (m_nShowProSubItem  != nColumn)
		{
			 pDC->DrawText(GetItemText(nItem, nColumn), -1, rcSubItem, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
		}
    }

    pDC->SetTextColor(nOldClr);

	if(m_nShowProSubItem != -1)//进度条实现
	{
		CRect rtSubItem(0,0,0,0);
		GetSubItemRect(nItem, m_nShowProSubItem, LVIR_BOUNDS, rtSubItem);
		CBrush brush((nItem % 2 == 0) ? LIST_ITEMS : LIST_ITEMF);
		pDC->FillRect(&rtSubItem, &brush);
		brush.DeleteObject();
		rtSubItem.DeflateRect(3,3);
		CRect rcLeft(0,0,0,0), rcRight(0,0,0,0);
		rcLeft = rcRight = rtSubItem;
		CString strItem = GetItemText(nItem, m_nShowProSubItem);
		rcLeft.right = rcLeft.left + MulDiv(_ttoi(strItem), rtSubItem.Width(), 100);
		rcRight.left = rcLeft.right;
		pDC->FillSolidRect(rcLeft, PRO_AREA_LEFT);
		pDC->FillSolidRect(rcRight, PRO_AREA_RIGHT);
		CRgn rgn;
		rgn.CreateRectRgnIndirect(rcLeft);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(PRO_TEXT_LEFT);
		pDC->DrawText(strItem, rtSubItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

		rgn.SetRectRgn(rcRight);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(PRO_TEXT_RIGHT);
		pDC->DrawText(strItem, rtSubItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		pDC->SelectClipRgn(NULL);
		pDC->SelectStockObject(NULL_BRUSH);
		CPen pen(PS_SOLID, 1, PRO_FRAME);
		CPen *pOldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(rtSubItem);
		pDC->SelectObject(pOldPen);
	}
}

void CSkinListCtrl::Init()
{
	InitializeFlatSB(this->m_hWnd);

	CWnd* pParent = GetParent();

	CRect windowRect;
	GetWindowRect(&windowRect);

	int nTitleBarHeight = 0;

	if(pParent->GetStyle() & WS_CAPTION)
		nTitleBarHeight = GetSystemMetrics(SM_CYSIZE);


	int nDialogFrameHeight = 0;
	int nDialogFrameWidth = 0;
	if((pParent->GetStyle() & WS_BORDER))
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}

	if(pParent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight+=1;
		nDialogFrameWidth+=1;
	}
}
void CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMeasureItemStruct->itemHeight = m_nitemHeight;
}
BOOL CSkinListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect(0,0,0,0),rectTemp(0,0,0,0);
	GetClientRect(rect);
	rectTemp = rect;
	CBrush brush0(LIST_ITEMS);
	CBrush brush1(LIST_ITEMF);
	CHeaderCtrl*pHead = GetHeaderCtrl();
	
	int ntCountPerPage = GetCountPerPage();
	for (int i=0;i<=ntCountPerPage;i++)
	{
		rectTemp.top = i * m_nitemHeight;
		        rectTemp.bottom =rectTemp.top + m_nitemHeight;
		pDC->FillRect(&rectTemp,i % 2 ? &brush0 : &brush1);

	}
	brush0.DeleteObject();
	brush1.DeleteObject();

	/*if (pHead)
	{
		CPoint pt1,pt2;
		pt1 = rect.TopLeft();
		pt2 = rect.BottomRight();
		pt2.x = pt1.x;
		int nItemCount = pHead->GetItemCount();
		CPen pen(PS_SOLID, 1, RGB(206, 206, 206));
		CPen *pOldPen = pDC->SelectObject(&pen);
		for(int i = 0; i <nItemCount; i++)
		{
			int nColumnWidth = GetColumnWidth(i);
			pt1.x = pt1.x + nColumnWidth;
			pt2.x = pt2.x + nColumnWidth;
			pDC->MoveTo(pt1);
			pDC->LineTo(pt2);
		}
		pDC->SelectObject(pOldPen);
	}*/
	//return CListCtrl::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSkinListCtrl::SetItemHeight( UINT nHeight )
{
	m_nitemHeight = nHeight;
	CRect rcwin;
	GetWindowRect(rcwin);
	WINDOWPOS wp;
	wp.hwnd= m_hWnd;
	wp.cx = rcwin.Width();
	wp.cy = rcwin.Height();
	wp.flags = SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER;  
	SendMessage(WM_WINDOWPOSCHANGED,0,(LPARAM)&wp);  
}
