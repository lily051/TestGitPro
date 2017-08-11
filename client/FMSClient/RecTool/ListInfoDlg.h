#pragma once
#include "Fui/fstatic.h"
#include "Fui/fbutton.h"


// CListInfoDlg �Ի���

class CListInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CListInfoDlg)

public:
	CListInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_LISTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
