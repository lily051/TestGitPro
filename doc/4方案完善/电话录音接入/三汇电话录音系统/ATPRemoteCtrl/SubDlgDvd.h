#if !defined(AFX_SUBDLGDVD_H__F85DC9D7_6B85_4539_B5B6_C586A0D5456E__INCLUDED_)
#define AFX_SUBDLGDVD_H__F85DC9D7_6B85_4539_B5B6_C586A0D5456E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgDvd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDvd dialog

class CSubDlgDvd : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgDvd(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgDvd)
	enum { IDD = IDD_DIALOG_DVD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgDvd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgDvd)
	afx_msg void OnBUTTONCheckMediaConnected();
	afx_msg void OnBUTTONCheckMediaAvailable();
	afx_msg void OnBUTTONWriteDVD();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGDVD_H__F85DC9D7_6B85_4539_B5B6_C586A0D5456E__INCLUDED_)
