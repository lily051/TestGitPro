// ComboTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ComboTreeCtrl.h"

#include "TreeComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrl
IMPLEMENT_DYNAMIC(CComboTreeCtrl, CTreeCtrl)


CComboTreeCtrl::CComboTreeCtrl()
	:m_pCombo(NULL)
	,m_pActrueParent(NULL)
{
}

CComboTreeCtrl::~CComboTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CComboTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CComboTreeCtrl)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
	ON_MESSAGE(TREECTRL_CHECK_STATE_CHANGE, OnTreeCtrlCheckStateChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrl message handlers

void CComboTreeCtrl::Display(CRect rect)
{
	if(NULL != GetSafeHwnd())
	{
		SetWindowPos(&wndNoTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		CWnd* pTopParent = GetParent()->GetParentOwner();
		if(NULL != pTopParent)
		{
			pTopParent->SendMessage(WM_NCACTIVATE, TRUE);
			pTopParent->SetRedraw(TRUE);
		}
	}
}

BOOL CComboTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->hwnd == m_hWnd && WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
		return TRUE;

	if(pMsg->hwnd == m_hWnd && WM_KEYDOWN == pMsg->message &&
		(VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam))
	{
		::PostMessage(m_pCombo->m_hWnd, WMU_CLOSE_CONTROL, (WPARAM)0, (LPARAM)0);
		return TRUE;
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CComboTreeCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	::PostMessage(m_pCombo->m_hWnd, WMU_CLOSE_CONTROL, (WPARAM)0, (LPARAM)0);
}

LRESULT CComboTreeCtrl::OnTreeCtrlCheckStateChange(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItemChanged = (HTREEITEM)lParam;
	BOOL bCheckState = GetCheck(hItemChanged);
	SelectItem(hItemChanged);

	if(! bCheckState)
		UncheckChilds(hItemChanged,TRUE);
	else
	{
		while(hItemChanged = GetParentItem(hItemChanged))
			SetCheck(hItemChanged);
	}

	return 1;
}

void CComboTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	UINT nFlags;
	HTREEITEM hItem;
	// verify that we have a mouse click in the check box area 
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	ScreenToClient(&point);
	hItem = HitTest(point, &nFlags);
	if(hItem && (nFlags & TVHT_ONITEMSTATEICON))
		PostMessage(TREECTRL_CHECK_STATE_CHANGE, 0, (LPARAM)hItem);

	*pResult = 0;
}

void CComboTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(VK_SPACE == pTVKeyDown->wVKey)
	{
		HTREEITEM hItem = GetSelectedItem();
		if(NULL != hItem)
		{
			// handle state change here or post message to another handler
			// Post message state has changed
			PostMessage(TREECTRL_CHECK_STATE_CHANGE, 0, (LPARAM)hItem);
		}
	}

	*pResult = 0;
}

int CComboTreeCtrl::UncheckChilds(HTREEITEM hItem/* = NULL*/, BOOL bRecurse/* = TRUE*/)
{
	int nCount = 0;
	if(NULL == hItem)
		GetSelectedItem();
	if(ItemHasChildren(hItem))
	{
		hItem = GetNextItem(hItem, TVGN_CHILD);
		while(hItem)
		{
			nCount++;
			SetCheck(hItem, FALSE);
			if(bRecurse)
				nCount += UncheckChilds(hItem, bRecurse);
			hItem = GetNextItem(hItem, TVGN_NEXT);
		}
	}

	return nCount;
}

void CComboTreeCtrl::GetTreeHierarchy(HTREEITEM hItem, CString& sTreeHierarchy, const BOOL bTopToBottom)
{
	HTREEITEM hRoot = GetParentItem(hItem);
	if(NULL != hRoot)
	{
		sTreeHierarchy += GetItemText(hItem) + _T("\\");
		return;
	}

	if(! bTopToBottom)
		sTreeHierarchy += GetItemText(hItem) + _T("\\");
	GetTreeHierarchy(hRoot, sTreeHierarchy, bTopToBottom);
	if(bTopToBottom)
		sTreeHierarchy += GetItemText(hItem) + _T("\\");
}

HTREEITEM CComboTreeCtrl::FindItem(const CString& sName, HTREEITEM hRoot)
{
	// check whether the current item is the searched one
	CString sText = GetItemText(hRoot);
	if(0 == sText.Compare(sName))
		return hRoot;

	// get a handle to the first child item
	HTREEITEM hSub = GetChildItem(hRoot);
	// iterate as long a new item is found
	while(hSub)
	{
		// check the children of the current item
		HTREEITEM hFound = FindItem(sName, hSub);
		if(hFound)
			return hFound;
		// get the next sibling of the current item
		hSub = GetNextSiblingItem(hSub);
	}

	return NULL;
}
