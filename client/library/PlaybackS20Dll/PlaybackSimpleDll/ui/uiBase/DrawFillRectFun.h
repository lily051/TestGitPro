/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	2:4:2008   21:40
	filename: 	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0\DrawFillRectFun.h
	file path:	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0
	file base:	DrawFillRectFun
	file ext:	h
	author:		Angle Pig
	
	purpose:	�����ε���
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
	Description:	Load��ͼ��ͼƬ
	Input:			nBakebmp ͼƬ��ID
	Output:      	
	Return:			
	*************************************************/
	void DrawFillRect_Loadbmp(UINT nBakebmp);

	/*************************************************
	Function:    	DrawFillRect_Loadbmp
	Description:	Load��ͼ��ͼƬ
	Input:			hBakebmp ͼƬ�ľ��
	Output:      	
	Return:			
	*************************************************/
	void DrawFillRect_Loadbmp(HBITMAP hBakebmp);

	/*************************************************
	Function:    	DrawFiiiRect_DrawFun
	Description:	��ָ������
	Input:			pDC ��ͼ���   rectBoundary ָ���Ļ�ͼ����  nDrawStyle ��ͼ����
	Output:      	
	Return:			
	*************************************************/
	void DrawFiiiRect_DrawFun(CDC * pDC, LPRECT rectBoundary, UINT nDrawStyle);
private:
	CBitmap m_Pastebmp;
};
