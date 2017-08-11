/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	2:4:2008   21:42
	filename: 	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0\DrawFillRectFun.cpp
	file path:	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0
	file base:	DrawFillRectFun
	file ext:	cpp
	author:		Angle Pig
	
	purpose:	»­¾ØÐÎµÄÀà
*********************************************************************/
//lint -library
#include "StdAfx.h"
#include ".\drawfillrectfun.h"

CDrawFillRectFun::CDrawFillRectFun(void)
{
	m_Pastebmp.m_hObject = NULL;
}

CDrawFillRectFun::~CDrawFillRectFun(void)
{
	//É¾³ýÍ¼Æ¬¾ä±ú
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;
	}
}

/*************************************************
Function:    	DrawFillRect_Loadbmp
Description:	Load»­Í¼µÄÍ¼Æ¬
Input:			nBakebmp Í¼Æ¬µÄID
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFillRect_Loadbmp(UINT nBakebmp)
{	
	//É¾³ýÍ¼Æ¬¾ä±ú
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;
	}

	//LoadÍ¼Æ¬
	m_Pastebmp.LoadBitmap(nBakebmp);
}

/*************************************************
Function:    	DrawFillRect_Loadbmp
Description:	Load»­Í¼µÄÍ¼Æ¬
Input:			hBakebmp Í¼Æ¬µÄ¾ä±ú
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFillRect_Loadbmp(HBITMAP hBakebmp)
{
	//É¾³ýÍ¼Æ¬¾ä±ú
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;

		m_Pastebmp.Detach();
	}

	//LoadÍ¼Æ¬
	m_Pastebmp.Attach(hBakebmp);
}

/*************************************************
Function:    	DrawFiiiRect_DrawFun
Description:	»­Ö¸¶¨ÇøÓò
Input:			pDC »­Í¼¾ä±ú   
				rectBoundary Ö¸¶¨µÄ»­Í¼ÇøÓò  
				nDrawStyle »­Í¼ÀàÐÍ
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFiiiRect_DrawFun(CDC * pDC, LPRECT rectBoundary, UINT nDrawStyle)
{
	//ASSERT(m_Pastebmp.m_hObject);

	//Èç¹ûÍ¼Æ¬¾ä±úÎª¿Õ,·µ»Ø
	if(m_Pastebmp.m_hObject == NULL)
	{
		return;
	}

	CDC MemDC;
	BITMAP bmp;
	CBitmap *pOldBMP = NULL;
	int iLeftTemp = 0;
	int iTopTemp = 0;
	int iWidTemp = 0;
	int iHeitTemp = 0;

	MemDC.CreateCompatibleDC(pDC);
	pOldBMP = (CBitmap*)MemDC.SelectObject(&m_Pastebmp);
	m_Pastebmp.GetBitmap(&bmp);

	switch(nDrawStyle)
	{
	case DRAWFILLRECT_STYLE_CONTENT:
		iLeftTemp = rectBoundary->left;
		iTopTemp = rectBoundary->top;
		iWidTemp = rectBoundary->right - rectBoundary->left;
		iHeitTemp = rectBoundary->bottom - rectBoundary->top;

		pDC->StretchBlt(iLeftTemp,iTopTemp,/*bmp.bmWidth*/iWidTemp,iHeitTemp,&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		break;
	default:
		break;
	}

	if(pOldBMP)
	{
		MemDC.SelectObject(pOldBMP);
		pOldBMP = NULL;
	}
}
