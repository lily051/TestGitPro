#pragma once
#include "afxcmn.h"

#define DIALOG_BK_COLOR	        RGB(175,175,175)  //������ɫRGB(175,175,175)

// CIntelDlg �Ի���

class CIntelDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntelDlg)

public:
	CIntelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntelDlg();

// �Ի�������
	enum { IDD = IDD_DLG_INTEL };
public:
    FUN_PARAM m_struFunParam;
private:
    CBrush m_brBackGray;
    CTreeCtrl m_treeCtrl;
    CImageList m_imTree;
    // �Ƿ�֧������
    BOOL m_bIntelligent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTvnSelchangedTreeDev(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
