#pragma once
#include "FuiBase.h"
#include "FEdit.h"

// CFComboBox

class AFX_EXT_CLASS CFComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CFComboBox)

public:
	CFComboBox();
	virtual ~CFComboBox();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCbnKillfocus();
    afx_msg void OnCbnSetfocus();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
    // 子类化的输入框
    CFEdit m_edit;
    // 设置字体
    std::map<int, CString> m_mapFont;
    // 边框颜色：常态和有焦点状态
    std::map<int, COLORREF> m_mapBorderColor;
    // 文字颜色
    std::map<int, COLORREF> m_mapTextColor;
    // 下拉按钮的图片（目前没有实现）
    Image *m_pImgArrow;
    // 下拉按钮的宽度和高度
    int m_nArrowWidth, m_nArrowHeight;
    // 控件为错误输入状态
    BOOL m_bError;
    // 控件是否有焦点
    BOOL m_bFocus;
    // 控件名称
    CString m_strObjectName;

    //
    CRect m_rcArrow;
    // 
    BOOL m_bMouseTrack;
    // 鼠标hover按钮
    BOOL m_bHoverBtn;

public:
    // 鼠标按下按钮
    BOOL m_bPressBtn;

	//是否弹出对话框
	BOOL m_bPopupDlg;

	// 提示文字
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
private:
    // 控件边框
    void DrawBorder(CDC* pDC, CRect& rcWnd);
    // 画下拉按钮
    void DrawArrowBtn(CDC* pDC, CRect& rcArrow);

public:

    void SetObjectName(LPCTSTR szObjectName);
    // 设置显示字体
    // SetItemHeight 需要放在该接口之后调用，否则会在设置字体后高度被重置
    void SetTextFont(int nState, const CString& strFontName);
    // 设置边框颜色
    void SetBoderColor(int nState, COLORREF color);
    // 设置提示文字
    void SetDefaultText(LPCTSTR szDefaultText);
    // 设置显示文字颜色
    void SetTextColor(int nState, COLORREF color);
    // 获取文字（控件自带的不能再调用）
    void GetText(LPTSTR szText);
    // 设置文字（控件自带的不能再调用）
    void SetText(LPCTSTR szText);
    // 设置是否为输入错误状态
    void SetState(BOOL bError = FALSE);
    // 清空文字
    void ClearText();
	// 设置为不能编辑模式
	void SetReadOnly(BOOL bReadOnly = TRUE);

	void SetPopupDlg(BOOL bPopup = TRUE);

	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


