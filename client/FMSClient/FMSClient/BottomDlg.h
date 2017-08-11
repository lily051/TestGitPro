#pragma once
#include ".\fui\fstatic.h"
#include ".\fui\fbutton.h"


// CBottomDlg 对话框
enum {
	APP_NORMAL,
	APP_WARN,
	APP_ERROR
};

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
	virtual BOOL OnInitDialog();
	void InitCtrl(void);
	void MoveCtrl(void);
	void SetAppState(int nAppState);
	int GetAppState();
	CFStatic m_staticCopyright;
	CFStatic m_staticVersion;
	CFButton m_btnState;
	int m_nAppState;
};
