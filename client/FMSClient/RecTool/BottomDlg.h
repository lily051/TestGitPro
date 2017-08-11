#pragma once


// CBottomDlg 对话框

class CBottomDlg : public CDialog
{
	DECLARE_DYNAMIC(CBottomDlg)

public:
	CBottomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBottomDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BOTTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	void InitCtrl(void);
	virtual BOOL OnInitDialog();
};
