#pragma once
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "fui\fbutton.h"
#include "fui\fbutton.h"


// CChangePasswordDlg 对话框

class CChangePasswordDlg : public CFDialog
{
	DECLARE_DYNAMIC(CChangePasswordDlg)

public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePasswordDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CHANGEPWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitCtrl(void);
	void MoveCtrl(void);
	virtual BOOL OnInitDialog();
	CFStatic m_staticNowPwd;
	CFEdit m_editNowPwd;
	CFStatic m_staticNewPwd;
	CFEdit m_editNewPwd;
	CFStatic m_staticNewPwd2;
	CFEdit m_editNewPwd2;
	CFButton m_btnOK;
	CFButton m_btnCancel;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	//是否已经登录
	bool m_bIsLogin;
	bool GetLoginState(void);
	void SetLoginState(bool bLogin);
};
