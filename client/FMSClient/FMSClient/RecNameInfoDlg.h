#pragma once


// CRecNameInfoDlg录像时显示名称 对话框

class CRecNameInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecNameInfoDlg)

public:
	CRecNameInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecNameInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RECNAMEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CFStatic m_staticRecName;

public:
	void InitCtrl(void);
	void MoveCtrl(void);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	LRESULT OnStartRecord(WPARAM wparam, LPARAM lparam);
	LRESULT OnStopRecord(WPARAM wparam, LPARAM lparam);
	BOOL StopRecord(CString & strRecFileName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
