#pragma once
#include "afxcmn.h"
//#include "NTPTimingdlg.h"

// DevinfoDialog 对话框

class DevinfoDialog : public CDialog
{
	DECLARE_DYNAMIC(DevinfoDialog)

public:
	DevinfoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DevinfoDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEVINFO };
    afx_msg void OnSize(UINT nType, int cx, int cy);    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
   // CTreeCtrl m_ctrlTreeDev;
     //NTPTimingdlg m_ntpdlg;
private:
     CRect m_rect;
    
public:
   // afx_msg void OnNMClickTreeDev(NMHDR *pNMHDR, LRESULT *pResult);
   // afx_msg void OnNMDblclkTreeDev(NMHDR *pNMHDR, LRESULT *pResult);

    void setNTPTiming(const int index);
    CListCtrl m_listDev;
    CListCtrl m_listchannel;
    void initDevList();
    void showDevList();

    void initChannalList();
     void showChannelList();
    void AdjustColumnWidth(CListCtrl& list);
    afx_msg void OnPaint();
    afx_msg void OnLvnItemchangedListDev(NMHDR *pNMHDR, LRESULT *pResult);
};
