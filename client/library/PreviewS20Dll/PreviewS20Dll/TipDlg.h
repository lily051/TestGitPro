#pragma once
#include "TipHeader.h"
#include "ui/UIDialog.h"


// CTipDlg 对话框

class CTipDlg : public UIDialog
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTipDlg();

// 对话框数据
	enum { IDD = IDD_DLG_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnClose();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    LRESULT afx_msg OnMsgSetWndPos(WPARAM wParam, LPARAM lParam);
    LRESULT afx_msg OnMsgSetWndAlpha(WPARAM wParam, LPARAM lParam);
    LRESULT afx_msg OnMsgShowWnd(WPARAM wParam, LPARAM lParam);
    LRESULT afx_msg OnMsgTipShowEnd(WPARAM wParam, LPARAM lParam);

	void ShowDlg();
	void BlinkDlg();
	void HideDlg();

	void ShowDlgFromButtom();
    void ShowDlgFromRight();
    void DrawTextInRect(CString strText, CRect rcTemp, COLORREF clr);

    BOOL m_bLive;
	RECT m_rcDesktop;
	CRect m_rcDlg;
	BOOL m_bMouseHover;
	CBrush m_brush;
	HANDLE m_hMoveDlg;

	int m_nDlgIndex;
    int m_nDlgUpTime;
    int m_nDlgBlinkTime;
    int m_nAlphaMax, m_nAlphaMin;
	int m_nHideTime;
	int m_nShowMode;
    int m_nMsgType;
    int m_nAlphaSlowly;

    CString m_strTips;
    CRect m_rcTips;
    LOGFONT m_stLF;

public:
    void ShowTips(CString strCaption, CString strTip, const int nDlgIndex, const TIP_DLG_PARAM &tdp);
    void DestoryTips();
};
