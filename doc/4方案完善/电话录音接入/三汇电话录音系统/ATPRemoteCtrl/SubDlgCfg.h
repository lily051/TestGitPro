#if !defined(AFX_SUBDLGCFG_H__F46A899F_2344_4DE8_B3B0_880687930B0E__INCLUDED_)
#define AFX_SUBDLGCFG_H__F46A899F_2344_4DE8_B3B0_880687930B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgCfg dialog

class CSubDlgCfg : public CDialog
{
// Construction
public:
	void OnTargetChanged(int nTarget);
	CSubDlgCfg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnOK(){};
	virtual void OnCancel(){};

// Dialog Data
	//{{AFX_DATA(CSubDlgCfg)
	enum { IDD = IDD_DIALOG_CFG };
	int		m_nCompress;
	int		m_nFileFormat;
	int		m_nStartCh;
	int		m_nTotalCh;
	CString	m_szRebootTime;
	CString	m_szRecPath;
	int		m_nCh;
	int		m_nRecType;
	int		m_nRecDir;
	int		m_nMaxRecLen;
	int		m_nRcvDTMFTime;
	CString	m_szDNS;
	CString	m_szGateway;
	CString	m_szIPAddr;
	CString	m_szMask;
	CString	m_szDateTime;
	CString	m_szNewPwd;
	CString	m_szOldPwd;
	int		m_nLogLevel;
	int		m_nRebootHour;
	int		m_nRebootMinute;
	BOOL	m_bAutoReboot;
	int		m_nRecInt;
	CString	m_szStartDtmf;
	CString	m_szStopDtmf;
	CString	m_szBindPhone;
	UINT	m_uPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgCfg)
	afx_msg void OnBUTTONGetGlobalCfg();
	afx_msg void OnBUTTONSetGlobalCfg();
	afx_msg void OnBUTTONGetChannelCfg();
	afx_msg void OnBUTTONSetChannelCfg();
	afx_msg void OnBUTTONGetNetCfg();
	afx_msg void OnBUTTONSetNetCfg();
	afx_msg void OnBUTTONSetDateTime();
	afx_msg void OnBUTTONSetAdminPassword();
	afx_msg void OnBUTTONEnableDebugMsg();
	afx_msg void OnButtonSetWebport();
	afx_msg void OnButtonGetWebport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGCFG_H__F46A899F_2344_4DE8_B3B0_880687930B0E__INCLUDED_)
