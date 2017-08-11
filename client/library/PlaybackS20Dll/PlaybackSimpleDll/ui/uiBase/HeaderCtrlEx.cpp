/**************************************************************************
*   Copyright 2008-2009 Hikvision Digital Technology Co., Ltd.
*   FileName:       HearderCtrlEx.cpp
*   Description     自定义列表控件
*   Author:         Mayonglai
*   Date:           2009-05-10
**************************************************************************/
// HeaderCtrlEx.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "HeaderCtrlEx.h"
#include "../uiCtrl/memdcex.h"


// CHeaderCtrlEx

IMPLEMENT_DYNAMIC(CHeaderCtrlEx, CHeaderCtrl)
CHeaderCtrlEx::CHeaderCtrlEx()
{
	
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
	if(m_bmpLeft.m_hObject)
	{

		DeleteObject(m_bmpLeft.Detach());
		m_bmpLeft.m_hObject = NULL;
	}

	if(m_bmpMid.m_hObject)
	{

		DeleteObject(m_bmpMid.Detach());
		m_bmpMid.m_hObject = NULL;
	}

	if(m_bmpRight.m_hObject)
	{

		DeleteObject(m_bmpRight.Detach());
		m_bmpRight.m_hObject = NULL;
	}

}

HBITMAP CHeaderCtrlEx::CopyBitmap(HBITMAP  hSourceHbitmap)
{
	HBITMAP   hbmResult;
	hbmResult = (HBITMAP)CopyImage(hSourceHbitmap,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	return   hbmResult; 
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayOut)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CHeaderCtrlEx message handlers


void CHeaderCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CHeaderCtrl::OnPaint() for painting messages

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);

	CRect clientRc;
	GetClientRect(&clientRc);
	CMemDCEX memDC(&dc, &clientRc);
	//CMemDC_ForSlider memDC(&dc, &clientRc);

	BITMAP btLeft,btMid,btRight;
	CRect leftRC,midRC,rightRC;

	
	//leftBmp.Attach(RESOURCELOAD_BMP(IDB_BITMAP_DEVBROADCAST_LISTHEADLEFT));
	//midBmp.Attach(RESOURCELOAD_BMP(IDB_BITMAP_DEVBROADCAST_LISTHEADMID));
	//rightBmp.Attach(RESOURCELOAD_BMP(IDB_BITMAP_DEVBRAODCAST_LISTHEADRIGHT));
	//leftBmp.Attach(RESOURCELOAD_PNG_EX_HBMP((IDR_PNG_LSITHEADER),Color(0,0,0,180)));
	//midBmp.Attach(RESOURCELOAD_PNG_EX_HBMP((IDR_PNG_LSITHEADER),Color(0,0,0,180)));
	//rightBmp.Attach(RESOURCELOAD_PNG_EX_HBMP((IDR_PNG_LSITHEADER),Color(0,0,0,180)));
	BOOL bPaintHead = FALSE;
	if (m_bmpLeft.m_hObject && m_bmpMid.m_hObject && m_bmpRight.m_hObject)
	{
		m_bmpLeft.GetBitmap(&btLeft);
		m_bmpMid.GetBitmap(&btMid);
		m_bmpRight.GetBitmap(&btRight);
		bPaintHead = TRUE;

		//绘制表头背景
		bitmapDC.SelectObject(m_bmpMid);
		memDC.StretchBlt(clientRc.left,clientRc.top,clientRc.Width(),clientRc.Height(),&bitmapDC,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);
	}
	

	
	
	

	CRect itemRC;
	int iItem = GetItemCount();
	for (int i = 0; i < iItem; i++)
	{
		//表头贴图
		GetItemRect(i,&itemRC);
		if (bPaintHead)
		{
			if (i == 0)                                         //第一列
			{
				//左边
				leftRC.left = itemRC.left;
				leftRC.right = leftRC.left + btLeft.bmWidth;
				leftRC.top = itemRC.top;
				leftRC.bottom = itemRC.bottom;
				bitmapDC.SelectObject(m_bmpLeft);
				memDC.StretchBlt(leftRC.left,leftRC.top,leftRC.Width(),leftRC.Height(),&bitmapDC,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);

				//右边
				rightRC.right = itemRC.right;
				rightRC.left = rightRC.right - btRight.bmWidth;
				rightRC.top = itemRC.top;
				rightRC.bottom = itemRC.bottom;
				bitmapDC.SelectObject(m_bmpRight);
				memDC.StretchBlt(rightRC.left,rightRC.top,rightRC.Width(),rightRC.Height(),&bitmapDC,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

				//中间
				midRC.left = leftRC.right;
				midRC.right = rightRC.left;
				midRC.top = itemRC.top;
				midRC.bottom = itemRC.bottom;
				bitmapDC.SelectObject(m_bmpMid);
				memDC.StretchBlt(midRC.left,midRC.top,midRC.Width(),midRC.Height(),&bitmapDC,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);
			}
			else
			{
				//右边
				rightRC.right = itemRC.right;
				rightRC.left = rightRC.right - btRight.bmWidth;
				rightRC.top = itemRC.top;
				rightRC.bottom = itemRC.bottom;
				bitmapDC.SelectObject(m_bmpRight);
				memDC.StretchBlt(rightRC.left,rightRC.top,rightRC.Width(),rightRC.Height(),&bitmapDC,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

				//中间
				midRC.left = itemRC.left;
				midRC.right = rightRC.left;
				midRC.top = itemRC.top;
				midRC.bottom = itemRC.bottom;
				bitmapDC.SelectObject(m_bmpMid);
				memDC.StretchBlt(midRC.left,midRC.top,midRC.Width(),midRC.Height(),&bitmapDC,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

			}

		}
		
		//绘制表头字符
		//CRect hdRect;
		TCHAR buf[256];
		HD_ITEM hdItem;

		hdItem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hdItem.pszText = buf;
		hdItem.cchTextMax = 255;
		GetItem( i, &hdItem );

		//GetItemRect(i, &hdRect);
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS|DT_VCENTER ;
		memDC.SetTextColor(RGB(0,0,0));
		memDC.SetBkMode(TRANSPARENT);
		
		CFont* pOldFont = memDC.SelectObject(GetFont());
		memDC.DrawText(buf,&itemRC,uFormat);
		memDC.SelectObject(pOldFont);
	}
	
	//绘制列线
	CPen myPen(PS_SOLID,1,RGB(204,204,204));
	CPen *pOldPen;
	pOldPen = memDC.SelectObject(&myPen);
	for (int i = 0; i < iItem; i++)
	{
		GetItemRect(i,&itemRC);
		memDC.MoveTo(itemRC.right,itemRC.top);
		memDC.LineTo(itemRC.right,itemRC.bottom);

	}
	memDC.SelectObject(pOldPen);
	//DeleteObject(leftBmp.Detach());
	//DeleteObject(midBmp.Detach());
	//DeleteObject(rightBmp.Detach());
}


LRESULT CHeaderCtrlEx::OnLayOut(WPARAM wParam, LPARAM lParam) 
{ 
	// 调用缺省的处理
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 

	// 获取表头信息
	HD_LAYOUT &hdl = *(HD_LAYOUT *)lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	// 修改表头高度
	//int iHeight = pwpos->cy + 7; 
	int iHeight = 31; 
	pwpos->cy = iHeight; 
	prc->top = iHeight; 

	return lResult; 
}

void CHeaderCtrlEx::SetListTitleBMP(HBITMAP hLeft, HBITMAP hMid, HBITMAP hRight)
{
	if(m_bmpLeft.m_hObject)
	{

		DeleteObject(m_bmpLeft.Detach());
		m_bmpLeft.m_hObject = NULL;
	}

	if(m_bmpMid.m_hObject)
	{

		DeleteObject(m_bmpMid.Detach());
		m_bmpMid.m_hObject = NULL;
	}

	if(m_bmpRight.m_hObject)
	{

		DeleteObject(m_bmpRight.Detach());
		m_bmpRight.m_hObject = NULL;
	}

	
	m_bmpLeft.Attach(CopyBitmap(hLeft));
	m_bmpMid.Attach(CopyBitmap(hMid));
	m_bmpRight.Attach(CopyBitmap(hRight));
}


BOOL CHeaderCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CHeaderCtrl::OnEraseBkgnd(pDC);
}
