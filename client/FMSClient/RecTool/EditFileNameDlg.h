#pragma once
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "fui\fbutton.h"


// CEditFileNameDlg修改录像名称 对话框

class CEditFileNameDlg : public CFDialog
{
	DECLARE_DYNAMIC(CEditFileNameDlg)

public:
	CEditFileNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditFileNameDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_EDIT_FILENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CString m_strNamePostfix;  //文件后缀名
	void InitData(void);
	void MoveCtrl(void);
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
