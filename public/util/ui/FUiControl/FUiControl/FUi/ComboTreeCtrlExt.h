#if !defined(AFX_COMBOTREECTRLEXT_H__8C84BCCC_2D78_47B8_AD2D_6E6FC8218087__INCLUDED_)
#define AFX_COMBOTREECTRLEXT_H__8C84BCCC_2D78_47B8_AD2D_6E6FC8218087__INCLUDED_

#include "ComboTreeCtrl.h"

#define MSG_COMBO_SELCHANGE (WM_USER + 206) // comboÑ¡Ôñ¸Ä±ä

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboTreeCtrlExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboTreeCtrlExt window

class AFX_EXT_CLASS CComboTreeCtrlExt : public CComboTreeCtrl
{
	DECLARE_DYNAMIC(CComboTreeCtrlExt)
// Construction
public:
	CComboTreeCtrlExt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboTreeCtrlExt)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboTreeCtrlExt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboTreeCtrlExt)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOTREECTRLEXT_H__8C84BCCC_2D78_47B8_AD2D_6E6FC8218087__INCLUDED_)
