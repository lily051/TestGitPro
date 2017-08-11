// PanelMenu.cpp : implementation file
//

#include "stdafx.h"
#include <vector>
#include "FTreeCtrlEx.h"
#include "ResMgr.h"

typedef struct CONTROLCENTER_TreenodePtr
{
	int				m_nUserData;          //树的节点类型
	Image*			m_pImage;
	BOOL			m_bFav;				  //收藏该节点
	BOOL			m_bEnable;				//节点是否可用
	CONTROLCENTER_TreenodePtr()
		: m_nUserData(-1)
		, m_bFav(FALSE)
		, m_pImage(NULL)
		, m_bEnable(TRUE)
	{
	}
}TreeNode,*PTreeNode;

// CCustomDrawTreeCtrl

IMPLEMENT_DYNAMIC(CFTreeCtrlEx, CTreeCtrl)

CFTreeCtrlEx::CFTreeCtrlEx()
	: m_clOrange(RGB(235,76,81))
	, m_clGray(RGB(137, 137, 137))
	, m_clWhite(RGB(255,255,255))
	, m_clBlack(RGB(0,0,0))
	, m_bHover(FALSE)
	, m_hCurHotItem(NULL)
	, m_dwDragStart(0)
	, m_pDragImage(NULL)
	, m_hItemDragS(NULL)
	, m_hItemDragD(NULL)
	, m_hInsertAfter(NULL)
	, m_nScrollTimerID(0)
	, m_nHoverTimerID(0)
	, m_bDragging(FALSE)
	, m_bDragAble(FALSE)
	, m_bTracking(FALSE)
	, m_bHadIcon(FALSE)
	, m_bHoverHot(FALSE)
	, m_wLastKayDown(0)
{
	InitCtrl();
}

CFTreeCtrlEx::~CFTreeCtrlEx()
{
	try
	{
		//CloseHandle(m_hCSHand);
		//CloseHandle(m_hCSArrow);
		//m_fontText.DeleteObject();
		for (int i = 0; i < TREE_IMAGE_ALL; i++)
		{
			if (m_pImageArray[i] != NULL)
			{
				delete m_pImageArray[i];
				m_pImageArray[i] = NULL;
			}
		}
		ClearTreeData();
	}
	catch (...)
	{
	}
}

BEGIN_MESSAGE_MAP(CFTreeCtrlEx, CTreeCtrl)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, &CFTreeCtrlEx::OnNMKillfocus)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CFTreeCtrlEx::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, &CFTreeCtrlEx::OnTvnKeydown)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CFTreeCtrlEx::OnNMDblclk)
    ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CCustomDrawTreeCtrl message handlers


void CFTreeCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	NMTVCUSTOMDRAW *pTVCD = (NMTVCUSTOMDRAW*)pNMHDR;
	CRect rcItem(pTVCD->nmcd.rc);
	POINT ptItem;
	ptItem.x = rcItem.left + 1;
	ptItem.y = rcItem.top + 1;
	UINT uFlags;   
	HTREEITEM hItem = HitTest(ptItem, &uFlags);  // Item 句柄
	if (NULL == hItem)
	{
		return ;
	}
	// The rect for the cell gives correct left and right values.

	switch (pTVCD->nmcd.dwDrawStage)
	{
		// First stage (for the whole control)
	case  CDDS_PREPAINT: 
		{		
			*pResult = CDRF_NOTIFYITEMDRAW;
			//OutputDebugString(_T("Draw Icon Stage 1. CDDS_PREPAINT \n"));
		}
		break;

		// Stage three (called for each subitem of the focused item)
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM: 
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
			//OutputDebugString(_T("Draw Icon Stage 3. CDDS_ITEMPREPAINT | CDDS_SUBITEM \n"));
		}
		break;

		// Stage four (called for each subitem of the focused item)
	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM: 
		{
			*pResult = CDRF_SKIPDEFAULT;	
			//OutputDebugString(_T("Draw Icon Stage 4. CDDS_ITEMPOSTPAINT | CDDS_SUBITEM \n"));
		}
		break;

	default:

		if (IsItemEnable(hItem))
		{
			// Stage two handled here. (called for each item)
			if (pTVCD->nmcd.uItemState & CDIS_HOT)
			{
				pTVCD->clrTextBk = m_clOrange;
				pTVCD->clrText = RGB(0, 0, 0);

				// Tell the control that to draw it again.
				*pResult = CDRF_NOTIFYPOSTPAINT;
				//OutputDebugString(_T("Draw Icon Stage 2. CDIS_HOT \n"));
			}

			if (pTVCD->nmcd.uItemState & CDIS_SELECTED)
			{
				pTVCD->clrTextBk = m_clOrange;
				pTVCD->clrText = m_clWhite;
				*pResult = AddItemIcon(pTVCD);
				//OutputDebugString(_T("Draw Icon Stage 2. CDIS_SELECTED \n"));
			}

			if ((pTVCD->nmcd.uItemState & CDIS_FOCUS))
			{
				pTVCD->clrTextBk = m_clOrange;
				pTVCD->clrText = m_clWhite;
				*pResult = CDRF_NOTIFYPOSTPAINT;
				//OutputDebugString(_T("Draw Icon Stage 2. CDIS_FOCUS \n"));
			}	

			if ((pTVCD->nmcd.uItemState & CDIS_CHECKED))
			{
				*pResult = CDRF_DODEFAULT;
				//OutputDebugString(_T("Draw Icon Stage 2. CDIS_CHECKED \n"));
			}
		}
		else
		{
			pTVCD->clrTextBk = m_clWhite;
			pTVCD->clrText = m_clGray;
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}

		break;
	}
}

LRESULT CFTreeCtrlEx::AddItemIcon(NMTVCUSTOMDRAW* pTVCD)
{
	CRect rcItem(pTVCD->nmcd.rc);
	POINT ptItem;
	ptItem.x = rcItem.left + 1;
	ptItem.y = rcItem.top + 1;
	UINT uFlags;   
	HTREEITEM hItem = HitTest(ptItem, &uFlags);  // Item 句柄
	if (hItem == NULL)
	{
		return CDRF_DODEFAULT;
	}
	CRect rcClient;
	GetClientRect(&rcClient);
	rcItem.left = 0;
	rcItem.right = rcClient.right;
	rcItem.NormalizeRect();

	bool bSelect = false;
	bool bFocus = false;

	if (GetItemState(hItem, TVIS_SELECTED) == TVIS_SELECTED)
	{
		bSelect = true;
		if (GetFocus() == this)
		{
			bFocus = true;
		}
	}
	CString strText;
	strText = GetItemText(hItem);

	HDC  hdcItem = pTVCD->nmcd.hdc;  //  Item的dc
	CDC dc;
	dc.Attach(hdcItem);
	int nSave = dc.SaveDC();

	DrawIcon(&dc, rcItem, hItem);

	dc.RestoreDC(nSave);
	dc.Detach();

	return CDRF_NOTIFYPOSTPAINT;
}

void CFTreeCtrlEx::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ptHover.SetPoint(-1,-1);
	m_bHover = FALSE;
	m_bTracking = FALSE;
	if (!m_rcOldHover.IsRectEmpty())
	{
		InvalidateRect(m_rcOldHover);
	}
	m_rcOldHover.SetRectEmpty();
	//鼠标离开树控件后，恢复箭头
	SetCurrentCursor(m_hCSArrow, NULL);
	Base::OnMouseLeave();
}

void CFTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = GetSafeHwnd();
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	MouseMoveOfDrag(nFlags, point);

	m_ptHover = point;
	m_bHover = TRUE;
	BOOL bHoverSelItem = FALSE;
	HTREEITEM hSelItem = NULL;
	HTREEITEM hHoverItem = NULL;
	UINT uFlags = 0;
	hHoverItem = HitTest(point, &uFlags);
	hSelItem = GetSelectedItem();
	bHoverSelItem = (hHoverItem == hSelItem);
	if (hHoverItem != NULL)
	{
		PTreeNode pData = (PTreeNode)Base::GetItemData(hHoverItem);
		if (!pData)
		{
			return Base::OnMouseMove(nFlags, point);
		}
		CDC* pDC = GetDC();
		int nSave = pDC->SaveDC();
		CRect rcItem;
		if (GetItemRect(hHoverItem, rcItem, FALSE))
		{
			BOOL bSetHot = FALSE;
			CRect rcTarget(rcItem);
			rcTarget.top = rcItem.top + (rcItem.Height() - TREE_IMAGE_WIDTH)/2;
			rcTarget.bottom = rcTarget.top + TREE_IMAGE_WIDTH;
			rcTarget.right = rcItem.right - TREE_MARGIN_10;
			rcTarget.left = rcTarget.right - TREE_IMAGE_WIDTH;
			if (rcTarget.PtInRect(point) && m_bHadIcon && pData->m_pImage && (bHoverSelItem || m_bHoverHot))
			{
				bSetHot = TRUE;
				SetCurrentCursor(m_hCSHand, hHoverItem);
			}
			else
			{
				SetCurrentCursor(m_hCSArrow, NULL);
			}

			if (!rcItem.EqualRect(&m_rcOldHover))
			{
				if (!m_rcOldHover.IsRectEmpty())
				{
					InvalidateRect(m_rcOldHover, FALSE);
				}
				m_rcOldHover = rcItem;
				InvalidateRect(rcItem, FALSE);
			}
			if (bSetHot)
			{
				DrawIcon(pDC, rcItem, hHoverItem);
			}
		}
		pDC->RestoreDC(nSave);
	}
	else
	{
		SetCurrentCursor(m_hCSArrow, NULL);
		if (!m_rcOldHover.IsRectEmpty())
		{
			InvalidateRect(m_rcOldHover, FALSE);
		}
		m_rcOldHover.SetRectEmpty();
	}

	SetCursor(m_hCSCurrent);

	Base::OnMouseMove(nFlags, point);
}

void CFTreeCtrlEx::LoadImageRes(Image * pImageIcon, Image * pImageDIcon)
{
	if (pImageIcon || pImageDIcon)
	{
		m_bHadIcon = TRUE;
	}
	if (m_pImageArray[TREE_IMAGE_ICON] != NULL)
	{
		delete m_pImageArray[TREE_IMAGE_ICON];
		m_pImageArray[TREE_IMAGE_ICON] = NULL;
	}
	m_pImageArray[TREE_IMAGE_ICON] = pImageIcon;

	if (m_pImageArray[TREE_IMAGE_DICON])
	{
		delete m_pImageArray[TREE_IMAGE_DICON];
		m_pImageArray[TREE_IMAGE_DICON] = NULL;
	}
	m_pImageArray[TREE_IMAGE_DICON] = pImageDIcon;
}

void CFTreeCtrlEx::LoadImageRes(LPCTSTR lpszIconPath, LPCTSTR lpszDIconPath)
{
	Image* pIcon = CResMgr::GetInstance()->GetRes(lpszIconPath);
	Image* pDIcon = CResMgr::GetInstance()->GetRes(lpszDIconPath);
	LoadImageRes(pIcon, pDIcon);
}

void CFTreeCtrlEx::InitCtrl()
{
	m_ptHover.SetPoint(-1,-1);
	for (int i = 0; i < TREE_IMAGE_ALL; i++)
	{
		m_pImageArray[i] = NULL;
	}
	m_hCSHand = ::LoadCursor(NULL, IDC_HAND);
	m_hCSArrow = ::LoadCursor(NULL, IDC_ARROW);
	m_hCSCurrent = m_hCSArrow;
}

void CFTreeCtrlEx::DrawIcon(CDC *pDC, CRect &rcItem, HTREEITEM hItem)
{
	Graphics graph(pDC->GetSafeHdc());

	PTreeNode pItemData = (PTreeNode)Base::GetItemData(hItem);
	if (pItemData == NULL)
	{
		return;
	}
	Image* pImage = NULL;

	if (pItemData->m_bFav)
	{
		pImage = m_pImageArray[TREE_IMAGE_ICON];
	}
	else
	{
		pImage = m_pImageArray[TREE_IMAGE_DICON];
	}

	if (pImage != NULL)
	{
		int nSize = min(pImage->GetWidth(), rcItem.Height());
		CRect rcImage(rcItem);
		if (rcImage.Height() < rcItem.Height())
		{
			rcImage.top = rcImage.top + (rcItem.Height() - rcImage.Height()) / 2;
		}
		rcImage.right = rcImage.right - TREE_MARGIN_10;
		rcImage.left = rcImage.right - rcImage.Height();

		graph.DrawImage(pImage, rcImage.left, rcImage.top, nSize, nSize);
		pItemData->m_pImage = pImage;
	}
}

void CFTreeCtrlEx::OnItemIconClick(HTREEITEM hItem)
{
	PTreeNode pItemData = (PTreeNode)Base::GetItemData(hItem);
	if (pItemData->m_bFav)
	{
		pItemData->m_bFav = FALSE;
	}
	else
	{
		pItemData->m_bFav = TRUE;
	}
	Invalidate(FALSE);
}

void CFTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT flags = 0;
	HTREEITEM hItem = HitTest(point, &flags);  // Item 句柄

	if (hItem && !IsItemEnable(hItem))
	{
		if (flags & TVHT_ONITEMBUTTON)
		{
			if(TVIS_EXPANDED & GetItemState(hItem, TVIS_EXPANDED))
			{
				Expand(hItem, TVE_COLLAPSE);
			}
			else
			{
				Expand(hItem, TVE_EXPAND);
			}
		}
		return;
	}

	if (m_bDragAble)
	{
		m_dwDragStart = GetTickCount();
	}
	if (NULL != m_hCurHotItem)
	{
		OnItemIconClick(m_hCurHotItem);
	}
	Base::OnLButtonDown(nFlags, point);
}

//OnMouseMove每次结束后都会调用。若在OnMouseMove方法中调用SetCursor会导致指针频闪。
BOOL CFTreeCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

void CFTreeCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Base::OnLButtonUp(nFlags, point);  

	if (!m_bDragAble)
	{
		return;
	}
	if( m_bDragging )  
	{  
		m_bDragging = FALSE;  
		CImageList::DragLeave( this );  
		CImageList::EndDrag();  
		ReleaseCapture();  
		delete m_pDragImage;  

		SelectDropTarget( NULL );  

		if( m_hItemDragS == m_hItemDragD )  
		{  
			KillTimer( m_nScrollTimerID );  
			return;  
		}  

		Expand( m_hItemDragD,TVE_EXPAND );  

		HTREEITEM  hParent = m_hItemDragD;  
		while( (hParent = GetParentItem(hParent)) != NULL )  
		{  
			if( hParent == m_hItemDragS )  
			{  
/*				HTREEITEM  htiNewTemp = CopyBranch( m_hItemDragS,NULL,TVI_LAST );  
				HTREEITEM  htiNew = CopyBranch( htiNewTemp,m_hItemDragD,TVI_LAST );  
				DeleteItem( htiNewTemp ); */ 
				SelectItem( m_hItemDragS );  
				KillTimer( m_nScrollTimerID );  
				return;  
			}  
		}  

		HTREEITEM  hNew = CopyBranch( m_hItemDragS, m_hItemDragD, m_hInsertAfter);  
		DeleteItem( m_hItemDragS );  
		SelectItem( hNew );  
		KillTimer( m_nScrollTimerID );  
	}  

}

void CFTreeCtrlEx::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bDragAble == FALSE)
	{
		return;
	}
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//处理无意拖拽
	if (GetTickCount() - m_dwDragStart < DRAG_DELAY)
	{
		return ;
	}

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	m_hItemDragD = NULL;

	//拖拽过程显示的图片
	m_pDragImage = CreateDragImage(m_hItemDragS);
	if (!m_pDragImage)
	{
		return;
	}

	m_bDragging = TRUE;
	m_pDragImage->BeginDrag(0,CPoint(8,8));
	CPoint pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(this, pt);
	SetCapture();

}


void CFTreeCtrlEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bDragAble)
	{
		Base::OnTimer(nIDEvent);
		return;
	}
	
	//鼠标敏感节点  
	if( nIDEvent == m_nHoverTimerID )  
	{  
		KillTimer( m_nHoverTimerID );  
		m_nHoverTimerID = 0;  
		HTREEITEM  trItem = 0;  
		UINT  uFlag = 0;  
		trItem = HitTest( m_HoverPoint,&uFlag );  
		if( trItem && m_bDragging )  
		{  
			SelectItem( trItem );  
			Expand( trItem,TVE_EXPAND );  
		}  
	}  
	//处理拖曳过程中的滚动问题  
	else if( nIDEvent == m_nScrollTimerID )  
	{  
		m_TimerTicks++;  
		CPoint  pt;  
		GetCursorPos( &pt );  
		CRect  rect;  
		GetClientRect( &rect );  
		ClientToScreen( &rect );  

		HTREEITEM  hItem = GetFirstVisibleItem();  

		if( pt.y < rect.top +10 )  
		{  
			//向上滚动  
			int  slowscroll = 6 - (rect.top + 10 - pt.y )/20;  
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )  
			{  
				CImageList::DragShowNolock ( false );  
				SendMessage( WM_VSCROLL,SB_LINEUP );  
				SelectDropTarget( hItem );  
				m_hItemDragD = hItem;
				CImageList::DragShowNolock ( true );  
			}  
		}  
		else if( pt.y > rect.bottom - 10 )  
		{  
			//向下滚动  
			int  slowscroll = 6 - (pt.y - rect.bottom + 10)/20;  
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )  
			{  
				CImageList::DragShowNolock ( false );  
				SendMessage( WM_VSCROLL,SB_LINEDOWN );  
				int  nCount = GetVisibleCount();  
				for( int i=0 ; i<nCount-1 ; i++ )  
					hItem = GetNextVisibleItem( hItem );  
				if( hItem )  
					SelectDropTarget( hItem );  
				m_hItemDragD = hItem;  
				CImageList::DragShowNolock ( true );  
			}  
		}  
	}  
	else  
		Base::OnTimer(nIDEvent);
}

//拷贝条目  
HTREEITEM CFTreeCtrlEx::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)  
{  
	TV_INSERTSTRUCT  tvstruct;  
	HTREEITEM        hNewItem;  
	CString          sText;  

	//得到源条目的信息  
	tvstruct.item.hItem = hItem;  
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;  
	GetItem( &tvstruct.item );  
	sText = GetItemText( hItem );  
	tvstruct.item.cchTextMax = sText.GetLength ();  
	tvstruct.item.pszText    = sText.LockBuffer ();  

	//将条目插入到合适的位置  
	tvstruct.hParent         = htiNewParent;  
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;  
	hNewItem = InsertItem( &tvstruct );  
	sText.ReleaseBuffer ();  

	//限制拷贝条目数据和条目状态  
	SetItemData( hNewItem,GetItemData(hItem) );  
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);  

	return hNewItem;  
}  

//拷贝分支  
HTREEITEM CFTreeCtrlEx::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)  
{  
	if (htiAfter == NULL)
	{
		htiAfter = TVI_LAST;
	}
	HTREEITEM  hChild;  
	HTREEITEM  hNewItem = CopyItem( htiBranch,htiNewParent,htiAfter );  
	hChild = GetChildItem( htiBranch );  

	while( hChild != NULL )  
	{  
		CopyBranch( hChild,hNewItem,htiAfter );  
		hChild = GetNextSiblingItem( hChild );  
	}  

	return  hNewItem;  
}  

void CFTreeCtrlEx::SetDragAble(BOOL bVale)
{
	m_bDragAble = bVale;
}

void CFTreeCtrlEx::MouseMoveOfDrag(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM  hItem;  
	UINT       flags;  


	if (!m_bDragAble)
	{
		return;
	}
	//检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时  
	if( m_nHoverTimerID )  
	{  
		KillTimer( m_nHoverTimerID );  
		m_nHoverTimerID = 0;  
	}  
	m_nHoverTimerID = SetTimer( 1,800,NULL );  //定时为 0.8 秒则自动展开  
	m_HoverPoint = point;  

	if( m_bDragging )  
	{  
		CPoint  pt = point;  
		CImageList::DragMove( pt );  

		//鼠标经过时高亮显示  
		CImageList::DragShowNolock( false );  //避免鼠标经过时留下难看的痕迹  
		if( (hItem = HitTest(point,&flags)) != NULL )  
		{  
			SelectDropTarget( hItem );  
			m_hItemDragD = hItem; 
		}  
		CImageList::DragShowNolock( true );  

		//当条目被拖曳到左边缘时，将条目放在根下 
		CRect rcRoot;
		CRect rcItem;

		CRgn rgnRoot;	//纵向客户区更新区域
		CRgn rgnItem;	//横向客户区更新区域
		bool bInRoot = false;
		CRect rcDraw(0,0,0,0);

		GetClientRect( &rcRoot );
		rcRoot.right = rcRoot.left + 20;
		GetItemRect(m_hItemDragD, rcItem, FALSE);
		rcItem.top = rcItem.bottom - 8;

		if( rcRoot.PtInRect(point) ) 
		{
			bInRoot = true;
			rcDraw = rcRoot;
			m_hItemDragD = NULL;
		}
		else
		{
			//当条目拖动到目标条目下方时，作为下一个兄弟节点插入
			if (rcItem.PtInRect(point))
			{
				rcDraw = rcItem;
				m_hInsertAfter = m_hItemDragD;
				m_hItemDragD = GetParentItem(m_hItemDragD);
			}
			else
			{
				InvalidateRect(rcItem, FALSE);
			}
		}
		if (!rcDraw.IsRectEmpty())
		{
			rgnItem.CreateRectRgn(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
			rgnRoot.CreateRectRgn(rcRoot.left, rcRoot.top, rcRoot.right, rcRoot.bottom);
			if (bInRoot)
			{
				rgnItem.CombineRgn(&rgnItem, &rgnRoot, RGN_DIFF);
				InvalidateRgn(&rgnItem, FALSE);
			}
			else
			{
				rgnRoot.CombineRgn(&rgnRoot, &rgnItem, RGN_DIFF);
				InvalidateRgn(&rgnRoot, TRUE);
			}
			rgnItem.DeleteObject();
			rgnRoot.DeleteObject();
			DrawRect(GetDC(), rcDraw);
		}
	}  

}

void CFTreeCtrlEx::SetCurrentCursor(HCURSOR curSor, HTREEITEM hotItem)
{
	if (m_hCSCurrent != curSor)
	{
		CRect rcSel;
		HTREEITEM hItem = GetSelectedItem();
		GetItemRect(hItem, rcSel, FALSE);
		InvalidateRect(rcSel);
	}
	m_hCSCurrent = curSor;
	m_hCurHotItem = hotItem;
}

void CFTreeCtrlEx::OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL CFTreeCtrlEx::SetItemData(HTREEITEM hItem, int nUserData)
{
	PTreeNode pTreeNode = (PTreeNode)Base::GetItemData(hItem);
	if (NULL == pTreeNode)
	{
		pTreeNode = new TreeNode;
		m_vcTreeDatas.push_back(pTreeNode);
	}
	pTreeNode->m_nUserData = nUserData;

	return CTreeCtrl::SetItemData(hItem, (DWORD)pTreeNode);
}

int CFTreeCtrlEx::GetItemData(HTREEITEM hItem)
{
	PTreeNode pTreeNode = (PTreeNode)Base::GetItemData(hItem);
	if (NULL == pTreeNode)
	{
		return -1;
	}
	return pTreeNode->m_nUserData;
}

void CFTreeCtrlEx::SetItemFavState(HTREEITEM hItem, BOOL bFav)
{
	PTreeNode pItemData = (PTreeNode)Base::GetItemData(hItem);
	if (pItemData == NULL)
	{
		return;
	}
	pItemData->m_bFav = bFav;
	Invalidate(FALSE);
}

void CFTreeCtrlEx::ClearTreeData()
{
	PTreeNode pNode = NULL;
	std::vector<PTreeNode>::iterator vcIter;
	while(!m_vcTreeDatas.empty())
	{
		vcIter = m_vcTreeDatas.begin();
		pNode = *vcIter;
		if (pNode != NULL)
		{
			delete pNode;
			pNode = NULL;
		}
		m_vcTreeDatas.erase(vcIter);
	}
}

void CFTreeCtrlEx::DrawRect(CDC *pDC, CRect &rcTag)
{
	pDC->FillSolidRect(rcTag, FUi::Blue);
}

void CFTreeCtrlEx::SetHoverHot(BOOL bValue)
{
	m_bHoverHot = bValue;
}

void CFTreeCtrlEx::SetItemEnable(HTREEITEM hItem, BOOL bEnable)
{
	PTreeNode pItemData = (PTreeNode)Base::GetItemData(hItem);
	if (pItemData == NULL)
	{
		return;
	}
	pItemData->m_bEnable = bEnable;
	Invalidate(FALSE);
}

BOOL CFTreeCtrlEx::IsItemEnable(HTREEITEM hItem)
{
	if(!hItem)
	{
		return FALSE;
	}
	PTreeNode pItemData = (PTreeNode)Base::GetItemData(hItem);
	if (pItemData == NULL)
	{
		return TRUE;
	}
	return pItemData->m_bEnable;
}


void CFTreeCtrlEx::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM& hNewItem = pNMTreeView->itemNew.hItem;
	HTREEITEM& hOldItem = pNMTreeView->itemOld.hItem;
	HTREEITEM hWatToItem = hNewItem;

	if (pNMTreeView->action == TVC_BYKEYBOARD)
	{
		while(hWatToItem && !IsItemEnable(hWatToItem))
		{
			HTREEITEM hItemTmp = NULL;
			switch(m_wLastKayDown)
			{
			case VK_RIGHT:
			case VK_DOWN:
				{
					hItemTmp = GetNextSiblingItem(hWatToItem);
					if (NULL == hItemTmp)
					{
						hWatToItem = GetChildItem(hWatToItem);
					}
					else
					{
						hWatToItem = hItemTmp;
					}
					break;
				}
			case VK_LEFT:
			case VK_UP:
				{
					hItemTmp = GetPrevSiblingItem(hWatToItem);
					if (NULL == hItemTmp)
					{
						hWatToItem = GetParentItem(hWatToItem);
					}
					else
					{
						hWatToItem = hItemTmp;
					}
					break;
				}
			default:
				break;
			}
		}

		if (NULL == hWatToItem)
		{
			hNewItem = hOldItem;
		}
		else
		{
			hNewItem = hWatToItem;
		}
	}
	else
	{
		if (hNewItem && !IsItemEnable(hNewItem))
		{
			hNewItem = hOldItem;
		}
	}
	Select(hNewItem, TVGN_CARET);

	*pResult = 0;
}

void CFTreeCtrlEx::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_wLastKayDown = pTVKeyDown->wVKey;
	*pResult = 0;
}

BOOL CFTreeCtrlEx::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = GetSelectedItem();
	if (hItem && !IsItemEnable(hItem))
	{
		if (!GetChildItem(hItem))
		{
			return FALSE;
		}
		else
		{
			Expand(hItem, TVE_EXPAND);
		}
	}
	*pResult = 0;

	return FALSE;
}

HTREEITEM CFTreeCtrlEx::GetNextEnableItem(HTREEITEM hItem /*= NULL*/)
{
	HTREEITEM hFind = hItem;
	if (!hFind)
	{
		hFind = GetRootItem();
	}
	
	if (hFind && IsItemEnable(hFind))
	{
		return hFind;
	}

	HTREEITEM hChild = GetChildItem(hItem);
	if (hChild)
	{
		HTREEITEM hFindChild = GetNextEnableItem(hChild);
		if (hFindChild)
		{
			return hFindChild;
		}
	}
	HTREEITEM hNextSilbing = GetNextSiblingItem(hItem);
	if (hNextSilbing)
	{
		HTREEITEM hFindNextSilbing = GetNextEnableItem(hNextSilbing);
		if (hFindNextSilbing)
		{
			return hFindNextSilbing;
		}
	}
	HTREEITEM hPreSilbing = GetPrevSiblingItem(hItem);
	if (hPreSilbing)
	{
		HTREEITEM hFindPreSilbing = GetNextEnableItem(hPreSilbing);
		if (hFindPreSilbing)
		{
			return hFindPreSilbing;
		}
	}

	return NULL;
}

void CFTreeCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // 响应树控件垂直滚动条拖动
    if(SB_THUMBTRACK == nSBCode)
    {
        SetScrollPos(SB_VERT, nPos);
    }

    CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
