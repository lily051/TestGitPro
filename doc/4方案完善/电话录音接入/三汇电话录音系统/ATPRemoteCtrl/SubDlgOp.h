#if !defined(AFX_SUBDLGOP_H__3CE6B012_6B1B_4EBF_A7D8_F540DEC3431D__INCLUDED_)
#define AFX_SUBDLGOP_H__3CE6B012_6B1B_4EBF_A7D8_F540DEC3431D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgOp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgOp dialog

class CSubDlgOp : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgOp(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgOp)
	enum { IDD = IDD_DIALOG_OP };
	int		m_nCh;
	CString	m_szMonitor;
	int		m_nRebootMode;
	CString	m_szDelFile;
	CString	m_szSaveFile;
	CString	m_szUDFile;
	int		m_nStart;
	int		m_nEnd;
	CString	m_szStartDate;
	CString	m_szStopDate;
	CString	m_szStartTime;
	CString	m_szStopTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgOp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgOp)
	afx_msg void OnBUTTONStartMonitor();
	afx_msg void OnBUTTONStopMonitor();
	afx_msg void OnBUTTONRebootDevice();
	afx_msg void OnBUTTONDelFile();
	afx_msg void OnBUTTONDelFiles();
	afx_msg void OnBUTTONStopDelFile();
	afx_msg void OnBUTTONUploadFile();
	afx_msg void OnBUTTONDownloadFile();
	afx_msg void OnBUTTONStartUploadFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGOP_H__3CE6B012_6B1B_4EBF_A7D8_F540DEC3431D__INCLUDED_)
