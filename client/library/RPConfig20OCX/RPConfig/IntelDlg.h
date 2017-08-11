#pragma once
#include "afxcmn.h"

#define DIALOG_BK_COLOR	        RGB(175,175,175)  //背景颜色RGB(175,175,175)

// CIntelDlg 对话框

class CIntelDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntelDlg)

public:
	CIntelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntelDlg();

// 对话框数据
	enum { IDD = IDD_DLG_INTEL };
public:
    FUN_PARAM m_struFunParam;
private:
    CBrush m_brBackGray;
    CTreeCtrl m_treeCtrl;
    CImageList m_imTree;
    // 是否支持智能
    BOOL m_bIntelligent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTvnSelchangedTreeDev(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
