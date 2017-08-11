#pragma once
#include "fui\fstatic.h"
#include "fui\fbutton.h"
#include "fui\fedit.h"


// CWordTemplateDlg 对话框

class CWordTemplateDlg : public CFDialog
{
	DECLARE_DYNAMIC(CWordTemplateDlg)

public:
	CWordTemplateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWordTemplateDlg();

// 对话框数据
	enum { IDD = IDD_DLG_WORDTEMPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void LoadSkin(void);
	void InitCtrl(void);
	void MoveCtrl(void);
	void InitData(void);
	void SetDlgData(CString& strProductName, CString& strWordTemplate);
	afx_msg void OnPaint();
	CFStatic m_staticProductName1;
	CFStatic m_staticProductName2;
	CFStatic m_staticProductWord1;
	CFButton m_btnClose;
	CFEdit m_editProductWord;
	CString m_strProductName;
	CString m_strWordTemplate;
	CFStatic m_staticBlank1;
};
