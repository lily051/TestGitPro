// ComboTreeCtrlExt.cpp : implementation file
//

#include "stdafx.h"
#include "ComboTreeCtrlExt.h"

#include "TreeComboBox.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrlExt
IMPLEMENT_DYNAMIC(CComboTreeCtrlExt, CComboTreeCtrl)

CComboTreeCtrlExt::CComboTreeCtrlExt()
{
}

CComboTreeCtrlExt::~CComboTreeCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CComboTreeCtrlExt, CComboTreeCtrl)
	//{{AFX_MSG_MAP(CComboTreeCtrlExt)
	//ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClickTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrlExt message handlers

void CComboTreeCtrlExt::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if(NULL != hItem)
	{
		//TRACE1(_T("Item selected: %s\n"), GetItemText(hItem));
		m_pCombo->SetText(GetItemText(hItem));
	}
	//m_pCombo->SetFocus();
		

	*pResult = 0;
}

void CComboTreeCtrlExt::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	UINT uFlags;
	HTREEITEM hCurrentItem;
	hCurrentItem = HitTest(pt, &uFlags);
	if (uFlags & TVHT_ONITEM)  //鼠标点击在Item标签上
	{
		HTREEITEM parentItem = GetParentItem(hCurrentItem);
		CString strItemName = GetItemText(hCurrentItem);
		OutputDebugString(strItemName);

	
		if(NULL != hCurrentItem)
		{
			//TRACE1(_T("Item selected: %s\n"), GetItemText(hCurrentItem));
			m_pCombo->SetText(GetItemText(hCurrentItem));
		}

	//	if (!ItemHasChildren(hCurrentItem))
		{
			this->ShowWindow(SW_HIDE);
		}

	}

// 	CWnd* pParent = GetParent();
// 	if(pParent != NULL)
// 	{
// 		WPARAM wParam = MAKEWPARAM(GetDlgCtrlID(), CBN_CLOSEUP);
// 		pParent->SendMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
// 	}

	CWnd* pParent = GetParent();
	if (m_pActrueParent)
	{
		pParent = m_pActrueParent;
	}
	::PostMessage(pParent->GetSafeHwnd(), WM_COMMAND, MSG_COMBO_SELCHANGE, (LPARAM)pParent->GetSafeHwnd());


	*pResult = 0;

	return;
}