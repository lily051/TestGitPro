/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinButton.h
*   Description:    5.2各服务器基本按钮类
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History: 
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
*    <version>      <time>          <author>        <desc>
*       1.01        2009-01-17      GuoLinlin       Modified Load Function
**************************************************************************/

#pragma once


// CSkinButton

class CSkinButton : public CButton
{
	DECLARE_DYNAMIC(CSkinButton)

public:
	CSkinButton();
	virtual ~CSkinButton();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	Image * m_pImage; // 按钮图片
	Image * m_pImageSel;

	BOOL m_bMouseTrack;
	BOOL m_bHover;
	CFont m_font;

	COLORREF m_clrText;

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

public:
	void Load(UINT uID, BOOL bLargeFont = FALSE);
	void Load(Image * pImage, BOOL bLargeFont = FALSE);
	void Load(Image * pImage, Image * pImageSel,BOOL bLargeFont = FALSE);
	void LiteIcon(HICON hIcon);

	HICON m_hIcon;
};



