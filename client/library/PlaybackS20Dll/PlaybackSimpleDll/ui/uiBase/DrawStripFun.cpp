/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	5:4:2008   13:53
	filename: 	DrawStripFun.cpp
	file base:	DrawStripFun
	file ext:	cpp
	author:		GuoLinlin
	
	purpose:	»­¾ØÐÎµÄÀà
*********************************************************************/
//lint -library
#include "StdAfx.h"
#include "DrawStripFun.h"

CDrawStripFun::CDrawStripFun(void)
{
	//³õÊ¼»¯Í¼Æ¬¾ä±ú
	for(int i = PASTEBMP_LEFT;i <= PASTEBMP_RIGHT;i++)
	{
		m_gPasteBmp[i].m_hObject = NULL;
	}
}

CDrawStripFun::~CDrawStripFun(void)
{
	//É¾³ýÍ¼Æ¬
	for(int i = PASTEBMP_LEFT;i <= PASTEBMP_RIGHT;i++)
	{
		if(m_gPasteBmp[i].m_hObject)
		{
			DeleteObject(m_gPasteBmp[i]);
			m_gPasteBmp[i].m_hObject = NULL;
		}
	}
}

/*************************************************
Function:    	DrawStrip_Loadbmp
Description:	LoadÍ¼Æ¬
Input:			nLeft  ×ó±ßÍ¼Æ¬ID
nMid   ÖÐ¼äÍ¼Æ¬ID
nRight ÓÒ±ßÍ¼Æ¬ID
Output:      	
Return:			
*************************************************/
void CDrawStripFun::DrawStrip_Loadbmp(UINT nLeft, UINT nMid, UINT nRight)
{
	//É¾³ýÍ¼Æ¬
	for(int i = PASTEBMP_LEFT;i <= PASTEBMP_RIGHT;i++)
	{
		if(m_gPasteBmp[i].m_hObject)
		{
			DeleteObject(m_gPasteBmp[i]);
			m_gPasteBmp[i].m_hObject = NULL;
		}
	}

	//LoadÍ¼Æ¬
	m_gPasteBmp[PASTEBMP_LEFT].LoadBitmap(nLeft);
	m_gPasteBmp[PASTEBMP_MID].LoadBitmap(nMid);
	m_gPasteBmp[PASTEBMP_RIGHT].LoadBitmap(nRight);
}

/*************************************************
Function:    	DrawStrip_Loadbmp
Description:	LoadÍ¼Æ¬
Input:			hLeft  ×ó±ßÍ¼Æ¬¾ä±ú
				hMid   ÖÐ¼äÍ¼Æ¬¾ä±ú
				hRight ÓÒ±ßÍ¼Æ¬¾ä±ú
Output:      	
Return:			
*************************************************/
void CDrawStripFun::DrawStrip_Loadbmp(HBITMAP hLeft, HBITMAP hMid, HBITMAP hRight)
{
	//É¾³ýÍ¼Æ¬
	for(int i = PASTEBMP_LEFT;i <= PASTEBMP_RIGHT;i++)
	{
		if(m_gPasteBmp[i].m_hObject)
		{
			DeleteObject(m_gPasteBmp[i]);
			m_gPasteBmp[i].m_hObject = NULL;
		}
	}

	//LoadÍ¼Æ¬
	m_gPasteBmp[PASTEBMP_LEFT].Attach(hLeft);
	m_gPasteBmp[PASTEBMP_MID].Attach(hMid);
	m_gPasteBmp[PASTEBMP_RIGHT].Attach(hRight);
}

/*************************************************
Function:    	DrawStrip_DrawFun
Description:	»­Ö¸¶¨ÇøÓò
Input:			pDC »­Í¼µÄDC
				rectBoundary Ö¸¶¨µÄ¾ØÐÎÇøÓò
Output:      	
Return:			
*************************************************/
void CDrawStripFun::DrawStrip_DrawFun(CDC * pDC, LPRECT rectBoundary)
{
	//ASSERT(m_gPasteBmp[PASTEBMP_LEFT].m_hObject);
	//ASSERT(m_gPasteBmp[PASTEBMP_MID].m_hObject);
	//ASSERT(m_gPasteBmp[PASTEBMP_RIGHT].m_hObject);

	if(m_gPasteBmp[PASTEBMP_RIGHT].m_hObject == NULL
		|| m_gPasteBmp[PASTEBMP_MID].m_hObject == NULL
		|| m_gPasteBmp[PASTEBMP_RIGHT].m_hObject == NULL)
	{
		return;
	}

	CDC MemDC;
	BITMAP bmp;
	CBitmap *pOldBMP = NULL;
	int iWid[2],iLeftTemp = 0,iTopTemp = 0,iWidTemp = 0,iHeitTemp = 0;

	MemDC.CreateCompatibleDC(pDC);

//...................»­×ó±ß
	pOldBMP = (CBitmap*)MemDC.SelectObject(&m_gPasteBmp[PASTEBMP_LEFT]);

	m_gPasteBmp[PASTEBMP_LEFT].GetBitmap(&bmp);

	iWid[0] = bmp.bmWidth;

	iLeftTemp = rectBoundary->left;
	iTopTemp = rectBoundary->top;
	iHeitTemp = rectBoundary->bottom - rectBoundary->top;
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);
	pDC->StretchBlt(iLeftTemp,iTopTemp,bmp.bmWidth,iHeitTemp,&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);

	if(NULL != pOldBMP)
	{
		MemDC.SelectObject(pOldBMP);
		//pOldBMP = NULL;   //Õâ²»ÊÇÄÖ×ÅÍæÂð¡£
	}
//...................»­ÓÒ±ß
	MemDC.SelectObject(&m_gPasteBmp[PASTEBMP_RIGHT]);

	m_gPasteBmp[PASTEBMP_RIGHT].GetBitmap(&bmp);

	iWid[1] = bmp.bmWidth;

	iLeftTemp = rectBoundary->right - iWid[1];
	iTopTemp = rectBoundary->top;
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);
	pDC->StretchBlt(iLeftTemp,iTopTemp,bmp.bmWidth,iHeitTemp,&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);

	if(NULL != pOldBMP)
	{
		MemDC.SelectObject(pOldBMP);
		//pOldBMP = NULL;
	}

//...................»­ÖÐ¼ä
	MemDC.SelectObject(&m_gPasteBmp[PASTEBMP_MID]);

	m_gPasteBmp[PASTEBMP_MID].GetBitmap(&bmp);

	iLeftTemp = rectBoundary->left + iWid[0];
	iTopTemp = rectBoundary->top;

	iWidTemp = rectBoundary->right - rectBoundary->left - iWid[0] - iWid[1];
	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetBrushOrg(0,0);
	pDC->StretchBlt(iLeftTemp,iTopTemp,iWidTemp,iHeitTemp,&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);

	if(NULL != pOldBMP)
	{
		MemDC.SelectObject(pOldBMP);
        DeleteObject(pOldBMP);
		pOldBMP = NULL;
	}
}
