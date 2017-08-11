#if !defined(AFX_SUBDLGINIT_H__D1770E43_7B2F_47D9_9E41_27DDE2D6CC47__INCLUDED_)
#define AFX_SUBDLGINIT_H__D1770E43_7B2F_47D9_9E41_27DDE2D6CC47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgInit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgInit dialog

class CSubDlgInit : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgInit(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgInit)
	enum { IDD = IDD_DIALOG_INIT };
	CString	m_szUser;
	CString	m_szPassword;
	UINT	m_wLocalPort;
	CString	m_szLocalIP;
	CString	m_szServerIP;
	CString	m_szSetParam1;
	UINT	m_uWebPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgInit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgInit)
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonSoapDest();
	afx_msg void OnBUTTONSetNetTimeout();
	afx_msg void OnButtonSetwebport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGINIT_H__D1770E43_7B2F_47D9_9E41_27DDE2D6CC47__INCLUDED_)
