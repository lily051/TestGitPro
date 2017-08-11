#pragma once
#include "afxcmn.h"


// BusinessDialg 对话框

class BusinessDialg : public CDialog
{
	DECLARE_DYNAMIC(BusinessDialg)

public:
	BusinessDialg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BusinessDialg();
    
// 对话框数据
	enum { IDD = IDD_DIALOG_BUSINESS };
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    
	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_listdata;

    int getWeight();
private:
    CRect m_rect;
    int m_curItem;
public:
    afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
     void setItemColour(int item);
};
