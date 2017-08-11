/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	5:4:2008   13:53
	filename: 	DrawStripFun.h
	file base:	DrawStripFun
	file ext:	h
	author:		GuoLinlin
	purpose:	»­¾ØÐÎµÄÀà
*********************************************************************/
#pragma once
#define PASTEBMP_LEFT  0 //×ó±ß
#define PASTEBMP_MID   1 //ÖÐ¼ä
#define PASTEBMP_RIGHT 2 //ÓÒ±ß

class CDrawStripFun
{
public:
	CDrawStripFun(void);
	~CDrawStripFun(void);
	/*************************************************
	Function:    	DrawStrip_Loadbmp
	Description:	LoadÍ¼Æ¬
	Input:			nLeft  ×ó±ßÍ¼Æ¬ID
					nMid   ÖÐ¼äÍ¼Æ¬ID
					nRight ÓÒ±ßÍ¼Æ¬ID
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_Loadbmp(UINT nLeft, UINT nMid, UINT nRight);

	/*************************************************
	Function:    	DrawStrip_DrawFun
	Description:	»­Ö¸¶¨ÇøÓò
	Input:			pDC »­Í¼µÄDC
					rectBoundary Ö¸¶¨µÄ¾ØÐÎÇøÓò
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_DrawFun(CDC * pDC, LPRECT rectBoundary);

	/*************************************************
	Function:    	DrawStrip_Loadbmp
	Description:	LoadÍ¼Æ¬
	Input:			hLeft  ×ó±ßÍ¼Æ¬¾ä±ú
					hMid   ÖÐ¼äÍ¼Æ¬¾ä±ú
					hRight ÓÒ±ßÍ¼Æ¬¾ä±ú
	Output:      	
	Return:			
	*************************************************/
	void DrawStrip_Loadbmp(HBITMAP hLeft, HBITMAP hMid, HBITMAP hRight);
private:
	CBitmap m_gPasteBmp[3];
};
