#if !defined(AFX_SUBDLGQUERY_H__354F50AB_4AE9_4C9B_BDF6_D8F7A8BE1F50__INCLUDED_)
#define AFX_SUBDLGQUERY_H__354F50AB_4AE9_4C9B_BDF6_D8F7A8BE1F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgQuery dialog

class CSubDlgQuery : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgQuery(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgQuery)
	enum { IDD = IDD_DIALOG_QUERY };
	CString	m_szMsg;
	CTime	m_tStart;
	CTime	m_tEnd;
	int		m_nCalcMode;
	int		m_nCalcMonth;
	CString	m_szCalcYear;
	int		m_nEndCh;
	int		m_nStartCh;
	int		m_nStartIndex;
	CTime	m_tEndDate;
	CTime	m_tEndTime;
	CTime	m_tStartDate;
	CTime	m_tStartTime;
	int		m_nMediaType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgQuery)
	afx_msg void OnBUTTONExecuteSQL();
	afx_msg void OnBUTTONCalcData();
	afx_msg void OnRadioCalcdataMode();
	afx_msg void OnRadioCalcdataMode2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONQueryRecord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGQUERY_H__354F50AB_4AE9_4C9B_BDF6_D8F7A8BE1F50__INCLUDED_)
