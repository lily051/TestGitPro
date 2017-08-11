/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	2:4:2008   21:40
	filename: 	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0\DrawFillRectFun.h
	file path:	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0
	file base:	DrawFillRectFun
	file ext:	h
	author:		Angle Pig
	
	purpose:	画矩形的类
*********************************************************************/

#pragma once

#define DRAWFILLRECT_STYLE_CONTENT 0

class CDrawFillRectFun
{
public:
	CDrawFillRectFun(void);
	~CDrawFillRectFun(void);
public:
	/*************************************************
	Function:    	DrawFillRect_Loadbmp
	Description:	Load画图的图片
	Input:			nBakebmp 图片的ID
	Output:      	
	Return:			
	*************************************************/
	void DrawFillRect_Loadbmp(UINT nBakebmp);

	/*************************************************
	Function:    	DrawFillRect_Loadbmp
	Description:	Load画图的图片
	Input:			hBakebmp 图片的句柄
	Output:      	
	Return:			
	*************************************************/
	void DrawFillRect_Loadbmp(HBITMAP hBakebmp);

	/*************************************************
	Function:    	DrawFiiiRect_DrawFun
	Description:	画指定区域
	Input:			pDC 画图句柄   rectBoundary 指定的画图区域  nDrawStyle 画图类型
	Output:      	
	Return:			
	*************************************************/
	void DrawFiiiRect_DrawFun(CDC * pDC, LPRECT rectBoundary, UINT nDrawStyle);
private:
	CBitmap m_Pastebmp;
};
