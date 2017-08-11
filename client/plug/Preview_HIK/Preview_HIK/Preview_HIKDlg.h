
// Preview_HIKDlg.h : 头文件
//

#pragma once
#include "DlgPic.h"


// CPreview_HIKDlg 对话框
class CPreview_HIKDlg : public CDialog
{
// 构造
public:
	CPreview_HIKDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PREVIEW_HIK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
    CDlgPic * m_pPicDlg;
    CBrush m_bkBrush;
    NET_DVR_DEVICEINFO_V30 m_devInfo;
    LONG m_lLoginID;
    LONG m_lPlayHandle;
private:
    void StartPlay(int nChannel);
    void StopPlay();
    void CapturePicture();
    void SetPicDlgShowPos();
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    virtual void OnCancel();
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
