/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	2:4:2008   21:42
	filename: 	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0\DrawFillRectFun.cpp
	file path:	c:\guoll\Working\Emap_Version3.0\Emap_Version3.0
	file base:	DrawFillRectFun
	file ext:	cpp
	author:		Angle Pig
	
	purpose:	�����ε���
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
	//ɾ��ͼƬ���
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;
	}
}

/*************************************************
Function:    	DrawFillRect_Loadbmp
Description:	Load��ͼ��ͼƬ
Input:			nBakebmp ͼƬ��ID
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFillRect_Loadbmp(UINT nBakebmp)
{	
	//ɾ��ͼƬ���
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;
	}

	//LoadͼƬ
	m_Pastebmp.LoadBitmap(nBakebmp);
}

/*************************************************
Function:    	DrawFillRect_Loadbmp
Description:	Load��ͼ��ͼƬ
Input:			hBakebmp ͼƬ�ľ��
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFillRect_Loadbmp(HBITMAP hBakebmp)
{
	//ɾ��ͼƬ���
	if(m_Pastebmp.m_hObject)
	{
		DeleteObject(m_Pastebmp);
		m_Pastebmp.m_hObject;

		m_Pastebmp.Detach();
	}

	//LoadͼƬ
	m_Pastebmp.Attach(hBakebmp);
}

/*************************************************
Function:    	DrawFiiiRect_DrawFun
Description:	��ָ������
Input:			pDC ��ͼ���   
				rectBoundary ָ���Ļ�ͼ����  
				nDrawStyle ��ͼ����
Output:      	
Return:			
*************************************************/
void CDrawFillRectFun::DrawFiiiRect_DrawFun(CDC * pDC, LPRECT rectBoundary, UINT nDrawStyle)
{
	//ASSERT(m_Pastebmp.m_hObject);

	//���ͼƬ���Ϊ��,����
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
