#pragma once
/**	@file    TitleBtn.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   标题按钮
*
*	@author	 yudan
*	@date	 2012/08/20
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/08/20 by yudan
*/


class CTitleButton : public CButton
{
	DECLARE_DYNAMIC(CTitleButton)

public:
	CTitleButton();
	~CTitleButton();

private:
	BOOL m_bInit;
	Image * m_pImage;
	CBitmap m_bitmapBk;
	int m_nFontSize;
	int m_bCenter;    //文字是否居中

public:
	Gdiplus::Font* m_pFontText;  //字体
	SolidBrush* m_pSolidBrush;  //刷子

public:

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//加载图片、文字
	void Load(UINT nIDResource);

	//文字居中
	void CenterText(BOOL bCenter = TRUE);

protected:
	virtual void PreSubclassWindow();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
