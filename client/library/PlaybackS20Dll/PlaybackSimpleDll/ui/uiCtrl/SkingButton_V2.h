#pragma once

#include "../uiBase/ButtonST.h"


class CSkingButton_V2 : public CButtonST
{
	DECLARE_DYNAMIC(CSkingButton_V2)

public:
	CSkingButton_V2();
	virtual ~CSkingButton_V2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

	void Load(Image * pImage, Image * pImageSel = NULL, Image * pImageDisabled = NULL);
	void SetBtnBkColor(COLORREF color) {m_clbk = color;};

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	Image * m_pImage; // °´Å¥Í¼Æ¬
	Image * m_pImageSel;
	Image * m_pImageDisabled;

	BOOL m_bMouseTrack;
	BOOL m_bHover;
	CFont m_font;

	COLORREF m_clrText;
	COLORREF m_clbk;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


