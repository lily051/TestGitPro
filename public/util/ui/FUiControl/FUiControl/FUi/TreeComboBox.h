#if !defined(AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_)
#define AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_


#include "ComboTreeCtrlExt.h"
#include "FuiBase.h"
#include "FEdit.h"


// 编辑框左边距离外边框的距离
#define EDIT_LEFT_M 3

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeComboBox window

class AFX_EXT_CLASS CTreeComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CTreeComboBox)

// Construction
public:
	CTreeComboBox();

// Attributes
public:

// Operations
public:
	void DisplayTree(CString strText);
	void SetTitle(const CString sTitle);
	BOOL IsAlertBkg() const {return m_bAlertBkg;}
	BOOL IsAlertText() const {return m_bAlertText;}
	CComboTreeCtrlExt& GetTreeCtrl(){return m_Tree;}
	BOOL IsControlActive() const {return m_bControlActive;}
	int GetDroppedWidth() const {return m_nDroppedWidth;}
	int GetDroppedHeight() const {return m_nDroppedHeight;}
	void SetDroppedWidth(const int nWidth){m_nDroppedWidth = nWidth;}
	void SetDroppedHeight(const int nHeight){m_nDroppedHeight = nHeight;}
	void AlertBkg(const BOOL bAlert = TRUE, BOOL bRedraw = FALSE)
	{
		m_bAlertBkg = bAlert;
		if(bRedraw)
			Invalidate();
	}
	void AlertText(const BOOL bAlert = TRUE, BOOL bRedraw = FALSE)
	{
		m_bAlertText = bAlert;
		if(bRedraw)
			Invalidate();
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbnSetfocus();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:
	void TreeCtrlDone();
	virtual ~CTreeComboBox();
	void SetAlertColorBkg(const COLORREF crColor);
	COLORREF GetAlertColorBkg() const {return m_crAlertBkg;}
	void SetAlertColorText(const COLORREF crColor){m_crAlertText = crColor;}
	COLORREF GetAlertColorText() const {return m_crAlertText;}

protected:
	BOOL m_bAlertBkg;
	BOOL m_bAlertText;
	BOOL m_bControlActive;
	int m_nDroppedWidth;
	int m_nDroppedHeight;
	CBrush m_BrushAlert;
	COLORREF m_crAlertBkg;
	COLORREF m_crAlertText;
	CComboTreeCtrlExt m_Tree;

public:
	void SetActrueParent(CWnd* pParent);
	//CWnd* m_pActrueParent;      // 实际的父窗口

// Generated message map functions
protected:
	//{{AFX_MSG(CTreeComboBox)
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnCloseControl(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

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
	// 鼠标按下按钮
	BOOL m_bPressBtn;

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
	void GetText(CString& szText);
	// 设置文字（控件自带的不能再调用）
	void SetText(LPCTSTR szText);
	// 设置是否为输入错误状态
	void SetState(BOOL bError = FALSE);
	// 清空文字
	void ClearText();
	// 设置为不能编辑模式
	void SetReadOnly(BOOL bReadOnly = TRUE);
	//设置tips提示
	void SetTipText(LPCTSTR szTipText);

    HTREEITEM FindItem(HTREEITEM hRoot, CString strText);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_)
