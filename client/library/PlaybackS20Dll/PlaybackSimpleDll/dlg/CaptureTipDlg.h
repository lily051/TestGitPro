#pragma once
#include "Picture.h"
#include "afxwin.h"
#include "../ui/uiCtrl/UiDialog.h"



// CCaptureTipDlg 对话框

class CCaptureTipDlg : public UIDialog
{
	DECLARE_DYNAMIC(CCaptureTipDlg)

public:
	CCaptureTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCaptureTipDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CAPTURE_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CBrush m_bkBrush;

	CStatic m_sttPath;
	BOOL m_bIsInit;
	CPicture m_pic;
	CRect m_rcPic;
	CRect m_rcPath;

	BOOL m_bTimer;     //定时器是否运行中
	BOOL m_bCount;	   // 是否需要计时.
	int m_nLeaveTime;  //鼠标离开时间

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	void LoadPic(TCHAR *szPicPath);	
	void InitPos();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};
