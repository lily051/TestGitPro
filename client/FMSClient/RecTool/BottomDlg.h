#pragma once


// CBottomDlg �Ի���

class CBottomDlg : public CDialog
{
	DECLARE_DYNAMIC(CBottomDlg)

public:
	CBottomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBottomDlg();

// �Ի�������
	enum { IDD = IDD_DLG_BOTTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	void InitCtrl(void);
	virtual BOOL OnInitDialog();
};
