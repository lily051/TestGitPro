
// FUiDemoDlg.h : 头文件
//

#pragma once

#include "../FUiControl/FUiControl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
using namespace std;

// CFUiDemoDlg 对话框
class CFUiDemoDlg : public CFDialog
{
    // 构造
public:
    CFUiDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CFUiDemoDlg();

    // 对话框数据
    enum { IDD = IDD_FUIDEMO_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


    // 实现
protected:
    DECLARE_MESSAGE_MAP()

    HICON m_hIcon;

    CFStatic m_fStatic;
    CFEdit m_fEdit;
    CFEdit m_fMEdit;
    CFComboBox m_fComboBox;
    CFButton m_fBtn;
    CFButton m_fCheckBox;
    CFButton m_fCheckBox2;
    CFButton m_fRadio;
    CFButton m_fRadio2;
    CFButton m_btnSS;
    CFButton m_btnM;
    CFSlider m_fSlider1;
    CFSlider m_fSlider2;
    CFProgress m_fProgress;
    CFListCtrl m_fList;
    CFTreeCtrlEx m_fTreeEx;
    CFScrollBar m_scVert;

    CImageList m_ImageList;
    vector<PTreeNode> m_vcTreeDatas;
    CFDateTimeCtrl m_fDataTime;  // 必须是短日期样式的才能调用到日历控件
    CCalendarDlg* m_pCalDlg;

    void InitDlg();
    void InitCtrl();

    //清除树上的数据
    void ClearTreeData();

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedAddsysbtn();
    afx_msg void OnBnClickedDelsysbtn();
    afx_msg void OnBnClickedBtnMenu();
    afx_msg void OnBnClickedBtnCalendar();
    afx_msg void OnBnClickedBtnWait();
    afx_msg void OnBnClickedBtnMsgbox();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtn();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
