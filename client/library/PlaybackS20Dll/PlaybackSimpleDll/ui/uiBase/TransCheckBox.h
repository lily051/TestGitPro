#pragma once  
/**	@file    CTransCheckBox.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   透明checkBox
*
*	@author	 yudan
*	@date	 2012/08/11
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/08/11 by yudan
*/


class CTransCheckBox : public CButton
{
	DECLARE_DYNAMIC(CTransCheckBox)

public:
	CTransCheckBox();
	~CTransCheckBox();


	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	Gdiplus::Font* m_pFontText;  //字体
	SolidBrush* m_pSolidBrush;  //刷子

	BOOL m_bCheck;

protected:
	virtual void PreSubclassWindow();

public:
	//设置选中状态
	void SetCheck(BOOL bCheck);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
