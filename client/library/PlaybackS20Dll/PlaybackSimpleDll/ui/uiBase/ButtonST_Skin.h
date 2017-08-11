#pragma once

#include "ButtonST.h"
// CButtonST_Skin
#define WM_LBTNDOWN_TABCONTROL	WM_USER + 0x1900	//鼠标左键按下  

#define WM_RBTNDOWNPRESETBTN    WM_USER + 1351      //鼠标右键消息

class CButtonST_Skin : public CButtonST
{
	//DECLARE_DYNAMIC(CButtonST_Skin)

public:
	void EableClickButton(BOOL bEnable = TRUE){	m_bClickEnable = bEnable; };
	void SetCenterImagePos(int nLeft,int nTop);  
	int m_nCenterImageLeft;
	int m_nCenterImageTop;

	CButtonST_Skin();
	virtual ~CButtonST_Skin();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	void DrawBackGround(CDC * pDC, LPRECT pRect,BOOL bFocus);

	void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	/*******************************************************************************
	*    Function:      LoadbakeImage
	*    Description:   导入按钮图片
	*    Input:         
	*    Output:        (NULL)
	*    Return:	    return void
	*******************************************************************************/
	void LoadbakeImage( Image * pBakeImage,Image * pCenterImage,Image * pBakeImageSel = NULL,Image * pCenterImageSel = NULL);
	void SetBakeImage(Image * pBakeImage);
	void LoadbakeImage_WholeBake( Image * pBakeImage,Image * pCenterImage,int iCount = 2);
	void LoadbakeImageEx( Image * pBakeImageLeft,Image * pBakeImageMid,Image * pBakeImageRight,Image * pCenterImage,\
		Image * pBakeImageSelLeft = NULL ,Image * pBakeImageSelMid = NULL,Image * pBakeImageSelRight = NULL,Image * pCenterImageSel = NULL);
private:
	BOOL m_bClickEnable;//[23:12:2009]wangchao TODO:是否可以点击按钮
	BOOL m_bMulState;

	Image * m_pBakeImage;
	Image * m_pCenterImage;
	Image * m_pBakeImageSel;
	Image * m_pCenterImageSel;

	Image * m_pBakeImageLeft;
	Image * m_pBakeImageMid;
	Image * m_pBakeImageRight;

	Image * m_pBakeImageSelLeft;
	Image * m_pBakeImageSelMid;
	Image * m_pBakeImageSelRight;

	
	BOOL m_bSingleMode;
	int m_iCount; 

	void FreeImage(void);
protected:
	BOOL m_bDown;
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy(); 
};


