#pragma once
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "fui\fbutton.h"
#include "fui\fbutton.h"


// CChangePasswordDlg �Ի���

class CChangePasswordDlg : public CFDialog
{
	DECLARE_DYNAMIC(CChangePasswordDlg)

public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangePasswordDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CHANGEPWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//�Ƿ��Ѿ���¼
	bool m_bIsLogin;
	bool GetLoginState(void);
	void SetLoginState(bool bLogin);
};
