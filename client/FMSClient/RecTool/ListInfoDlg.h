#pragma once
#include "Fui/fstatic.h"
#include "Fui/fbutton.h"


// CListInfoDlg 对话框

class CListInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CListInfoDlg)

public:
	CListInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_LISTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitCtrl(void);
	void MoveCtrl();
	CFStatic m_staticListInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOpenfolder();
	CFButton m_btnOpenFolder;
};
