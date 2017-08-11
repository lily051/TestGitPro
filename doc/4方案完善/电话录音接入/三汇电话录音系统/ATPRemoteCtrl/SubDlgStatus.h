#if !defined(AFX_SUBDLGSTATUS_H__2360E2B8_0926_4F96_B2DD_11F3AE0F4F48__INCLUDED_)
#define AFX_SUBDLGSTATUS_H__2360E2B8_0926_4F96_B2DD_11F3AE0F4F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgStatus dialog

class CSubDlgStatus : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgStatus(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgStatus)
	enum { IDD = IDD_DIALOG_STATUS };
	int		m_nCh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgStatus)
	afx_msg void OnBUTTONGetHardwareInfo();
	afx_msg void OnBUTTONGetChannelStatus();
	afx_msg void OnBUTTONSYGetDiskFreeSpace();
	afx_msg void OnBUTTONGetLineVol();
	afx_msg void OnBUTTONGetChannelRunningStatus();
	afx_msg void OnBUTTONGetDeviceTime();
	afx_msg void OnBUTTONGetDevID();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGSTATUS_H__2360E2B8_0926_4F96_B2DD_11F3AE0F4F48__INCLUDED_)
