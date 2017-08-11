#pragma once


// CRecNameInfoDlg¼��ʱ��ʾ���� �Ի���

class CRecNameInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecNameInfoDlg)

public:
	CRecNameInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecNameInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RECNAMEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
