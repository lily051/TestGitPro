/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	7:1:2008   17:19
	filename: 	c:\guoll\Working\controlcenterNew\code\Controlcenter\Button_Paste.h
	file path:	c:\guoll\Working\controlcenterNew\code\Controlcenter
	file base:	Button_Paste
	file ext:	h
	author:		Angle Pig
	
	purpose:	这是一个基于CButtonST封装的类，用于绘制按钮的底图
*********************************************************************/

#pragma once
#include "./DrawFillRectFun.h"
#include "ButtonST.h"

#define WM_BTN_CLICKED WM_USER + 8161
// CButton_Paste

class CButton_Paste : public CButtonST
{
//	DECLARE_DYNAMIC(CButton_Paste)

public:
	CButton_Paste();
	virtual ~CButton_Paste();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
public:
	void EableClickButton(BOOL bEnable = TRUE){	m_bClickEnable = bEnable; };
	void DrawBackGround(CDC * pDC, LPRECT pRect,BOOL bFocus);
	virtual void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);

	//load绘制背景的图片
	void Loadbakebmp( UINT nMid,UINT nMidSel = 0);
	//load绘制背景的图片
	void Loadbakebmp( HBITMAP hMid,HBITMAP hMidSel = NULL);
	//设置按钮是否是选中状态
	void BtnPaset_SetSelect(bool bSelect);
	
	void SetTextColorEx(COLORREF textColor,COLORREF textColorSel);

	void SetFont(long lfWeight,long lfHeight); // yxn20090716

	void SetCaptionStyle(int iStyle);      // yxn20090716
	// 创建字体
	BOOL CreateTextFont(void);
protected:
	BOOL m_bClickEnable;	//修改是否容许左键点击
	int  m_iStyle;      // yxn20090716 字体所在矩形框
	BOOL m_bFontDefault;  // yxn20090716  字体是否默认
	long m_lFontWeight;   // yxn20090716  字体大小
	long m_lFontHight;    // yxn20090716  字体高度
	BOOL m_bMulState;
	BOOL m_bSelected;
	CDrawFillRectFun m_DrawStrip_Normal;
	CDrawFillRectFun m_DrawStrip_Select;
	CFont m_font;

	COLORREF m_TextColor;               //文字颜色
	COLORREF m_TextColor_sel;           //选中时文字颜色
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


