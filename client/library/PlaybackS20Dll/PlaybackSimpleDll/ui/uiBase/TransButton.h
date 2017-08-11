#pragma once
/**	@file    TransButton.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   透明背景按钮
*
*	@author	 yudan
*	@date	 2012/08/20
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/08/20 by yudan
*/


class CTransButton : public CButton
{
	DECLARE_DYNAMIC(CTransButton)

public:
	CTransButton();
	~CTransButton();

private:
	BOOL m_bMouseTrack;
	BOOL m_bHover;
	BOOL m_bLock;
	BOOL m_bSelect;

	BOOL m_bInit;

	Image * m_pImage;
	Image * m_pImageHoverBk;
	Image * m_pImageSelBk;

	CBitmap m_bitmapBk;


public:
	Gdiplus::Font* m_pFontText;  //字体
	SolidBrush* m_pSolidBrush;  //刷子
	int m_nBtnID;  //按钮的表示ID，与组件相对应

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//加载图片、文字
	void Load(UINT nIDResource);
	//加载选中背景图片
	void LoadSelectBkgnd(UINT nIDHover, UINT nIDSelect);
	//锁定状态
	void LockState();
	//设置按钮选中状态
	void SetSelect(BOOL bSelect);

protected:
	virtual void PreSubclassWindow();

public:
	afx_msg void OnBnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
