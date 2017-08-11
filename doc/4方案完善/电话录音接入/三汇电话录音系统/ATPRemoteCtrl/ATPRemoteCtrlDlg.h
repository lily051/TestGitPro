// ATPRemoteCtrlDlg.h : header file
//

#if !defined(AFX_ATPREMOTECTRLDLG_H__552C2756_E7D2_43CA_B189_F336E3040ED2__INCLUDED_)
#define AFX_ATPREMOTECTRLDLG_H__552C2756_E7D2_43CA_B189_F336E3040ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SubDlgInit.h"
#include "SubDlgCfg.h"
#include "SubDlgStatus.h"
#include "SubDlgOp.h"
#include "SubDlgQuery.h"
#include "SubDlgDvd.h"
/////////////////////////////////////////////////////////////////////////////
// CATPRemoteCtrlDlg dialog

#define ENUM_SUBDLG(x) VIEW_##x
#define SubDlg(x) m_dlg##x
#define SUBDLG_VAR(x) x SubDlg(x)

class CATPRemoteCtrlDlg : public CDialog
{
// Construction
public:
	void ChangeWebPort(WORD uPort);
	void AppendResult(CString& msg);
	CATPRemoteCtrlDlg(CWnd* pParent = NULL);	// standard constructor
	enum{
		ENUM_SUBDLG(CSubDlgInit),
		ENUM_SUBDLG(CSubDlgCfg),
		ENUM_SUBDLG(CSubDlgStatus),
		ENUM_SUBDLG(CSubDlgOp),
		ENUM_SUBDLG(CSubDlgQuery),
		ENUM_SUBDLG(CSubDlgDvd),
		VIEW_MAX
	};
	HWND m_hWndViews[VIEW_MAX];
	CfnXwebApiBindingProxy * m_pSynapiProxy;
	CStringArray m_arrServer;
	CWordArray m_arrPort;
	CStringArray m_arrEndPoint;
	int m_nServerIndex;

// Dialog Data
	//{{AFX_DATA(CATPRemoteCtrlDlg)
	enum { IDD = IDD_ATPREMOTECTRL_DIALOG };
	CComboBox	m_cbServerList;
	CTabCtrl	m_tabMain;
	int		m_nTarget;
	CString	m_szResult;
	//}}AFX_DATA
	SUBDLG_VAR(CSubDlgInit);
	SUBDLG_VAR(CSubDlgCfg);
	SUBDLG_VAR(CSubDlgStatus);
	SUBDLG_VAR(CSubDlgOp);
	SUBDLG_VAR(CSubDlgQuery);
	SUBDLG_VAR(CSubDlgDvd);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATPRemoteCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CATPRemoteCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioMode();
	afx_msg void OnRadioMode2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboServerList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline CATPRemoteCtrlDlg * TopDlg(CWnd * pWnd)
{
	return (CATPRemoteCtrlDlg*)pWnd;
}
extern CString tmpMsg;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPREMOTECTRLDLG_H__552C2756_E7D2_43CA_B189_F336E3040ED2__INCLUDED_)
