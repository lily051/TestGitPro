#pragma once
#include "afxcmn.h"


// BusinessDialg �Ի���

class BusinessDialg : public CDialog
{
	DECLARE_DYNAMIC(BusinessDialg)

public:
	BusinessDialg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BusinessDialg();
    
// �Ի�������
	enum { IDD = IDD_DIALOG_BUSINESS };
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
