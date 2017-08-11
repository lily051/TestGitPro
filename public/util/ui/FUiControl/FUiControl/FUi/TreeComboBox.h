#if !defined(AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_)
#define AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_


#include "ComboTreeCtrlExt.h"
#include "FuiBase.h"
#include "FEdit.h"


// �༭����߾�����߿�ľ���
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
	//CWnd* m_pActrueParent;      // ʵ�ʵĸ�����

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
	// ���໯�������
	CFEdit m_edit;
	// ��������
	std::map<int, CString> m_mapFont;
	// �߿���ɫ����̬���н���״̬
	std::map<int, COLORREF> m_mapBorderColor;
	// ������ɫ
	std::map<int, COLORREF> m_mapTextColor;
	// ������ť��ͼƬ��Ŀǰû��ʵ�֣�
	Image *m_pImgArrow;
	// ������ť�Ŀ�Ⱥ͸߶�
	int m_nArrowWidth, m_nArrowHeight;
	// �ؼ�Ϊ��������״̬
	BOOL m_bError;
	// �ؼ��Ƿ��н���
	BOOL m_bFocus;
	// �ؼ�����
	CString m_strObjectName;

	//
	CRect m_rcArrow;
	// 
	BOOL m_bMouseTrack;
	// ���hover��ť
	BOOL m_bHoverBtn;
	// ��갴�°�ť
	BOOL m_bPressBtn;

	// ��ʾ����
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
private:
	// �ؼ��߿�
	void DrawBorder(CDC* pDC, CRect& rcWnd);
	// ��������ť
	void DrawArrowBtn(CDC* pDC, CRect& rcArrow);

public:

	void SetObjectName(LPCTSTR szObjectName);
	// ������ʾ����
	// SetItemHeight ��Ҫ���ڸýӿ�֮����ã�����������������߶ȱ�����
	void SetTextFont(int nState, const CString& strFontName);
	// ���ñ߿���ɫ
	void SetBoderColor(int nState, COLORREF color);
	// ������ʾ����
	void SetDefaultText(LPCTSTR szDefaultText);
	// ������ʾ������ɫ
	void SetTextColor(int nState, COLORREF color);
	// ��ȡ���֣��ؼ��Դ��Ĳ����ٵ��ã�
	void GetText(CString& szText);
	// �������֣��ؼ��Դ��Ĳ����ٵ��ã�
	void SetText(LPCTSTR szText);
	// �����Ƿ�Ϊ�������״̬
	void SetState(BOOL bError = FALSE);
	// �������
	void ClearText();
	// ����Ϊ���ܱ༭ģʽ
	void SetReadOnly(BOOL bReadOnly = TRUE);
	//����tips��ʾ
	void SetTipText(LPCTSTR szTipText);

    HTREEITEM FindItem(HTREEITEM hRoot, CString strText);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECOMBOBOX_H__B9544E92_7215_4D8B_9DF2_67F232BA4014__INCLUDED_)
