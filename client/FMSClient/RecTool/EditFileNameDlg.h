#pragma once
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "fui\fbutton.h"


// CEditFileNameDlg�޸�¼������ �Ի���

class CEditFileNameDlg : public CFDialog
{
	DECLARE_DYNAMIC(CEditFileNameDlg)

public:
	CEditFileNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditFileNameDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_EDIT_FILENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFStatic m_staticFileName;
	CFEdit m_editFileName;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitCtrl(void);
	CFButton m_btnOK;
	CFButton m_btnCancel;
	void SetDlgData(CString strFileName);
	CString m_strOldFileName;
	CString m_strNewFileName;
	CString m_strNamePostfix;  //�ļ���׺��
	void InitData(void);
	void MoveCtrl(void);
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
