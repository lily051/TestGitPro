#if !defined(AFX_COMBOTREECTRL_H__FE1D96F3_B70F_4E65_8203_B37AE69DB82D__INCLUDED_)
#define AFX_COMBOTREECTRL_H__FE1D96F3_B70F_4E65_8203_B37AE69DB82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboTreeCtrl.h : header file
//

#define TREECTRL_CHECK_STATE_CHANGE			(WM_USER + 64)
#define WMU_CLOSE_CONTROL					(WM_USER + 65)
/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrl window

class CTreeComboBox;

class AFX_EXT_CLASS CComboTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CComboTreeCtrl)
// Construction
public:
	CComboTreeCtrl();

// Attributes
protected:
	CTreeComboBox* m_pCombo;

// Operations
protected:
	virtual int UncheckChilds(HTREEITEM hItem = NULL, BOOL bRecurse = TRUE);
	virtual void GetTreeHierarchy(HTREEITEM hItem, CString& sTreeHierarchy, const BOOL bTopToBottom);
	virtual HTREEITEM FindItem(const CString& sName, HTREEITEM hRoot);

// Attributes
public:

// Operations
public:
	virtual void Display(CRect rect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init(CTreeComboBox* pCombo){m_pCombo = pCombo;};
	virtual ~CComboTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboTreeCtrl)
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnTreeCtrlCheckStateChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	   void SetActrueParent(CWnd* pParent){ m_pActrueParent = pParent; }
	   CWnd* m_pActrueParent;      // 实际的父窗口
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOTREECTRL_H__FE1D96F3_B70F_4E65_8203_B37AE69DB82D__INCLUDED_)
