#pragma once

#include "./ui/ChildDlgOwerDraw.h"
#include "./ui/Scope.h"

// CRealWaveDlg 对话框

class CRealWaveDlg : public CChildDlgOwerDraw
{
	DECLARE_DYNAMIC(CRealWaveDlg)

public:
	CRealWaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRealWaveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REAL_WAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();	

	DECLARE_MESSAGE_MAP()

public:
    CScope m_scope;
	CScope m_scopePhone;

private:
    void MoveCtrl();

    BOOL m_bInitFlag;
};
