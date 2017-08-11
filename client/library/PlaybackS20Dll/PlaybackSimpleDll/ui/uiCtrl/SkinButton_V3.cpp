// SkinButton_V3.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "SkinButton_V3.h"
#include ".\skinbutton_v3.h"


// CSkinButton_V3

IMPLEMENT_DYNAMIC(CSkinButton_V3, CSkingButton_V2)
CSkinButton_V3::CSkinButton_V3()
{
}

CSkinButton_V3::~CSkinButton_V3()
{
}


BEGIN_MESSAGE_MAP(CSkinButton_V3, CSkingButton_V2)
END_MESSAGE_MAP()



// CSkinButton_V3 message handlers


void CSkinButton_V3::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_pImage == NULL)
	{
		return;
	}
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	int nWidth = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBitmap);

	//if (GetParent()->IsKindOf(RUNTIME_CLASS(CDecodeSingleCfgDlg))
	//	/*|| GetParent()->IsKindOf(RUNTIME_CLASS(CLoginDlg)*/
	//	/*|| GetParent()->IsKindOf(RUNTIME_CLASS(CStreamServerDlg))*/)
	//{
	//	((CSkinDialog *)GetParent())->TransparentBkgnd(&memDC, this);
	//}
	//else
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);
		memDC.FillSolidRect(0, 0, rc.right - rc.left, rc.bottom - rc.top, m_clbk);
	}

	Graphics graph(memDC.m_hDC);

	Rect rcF(0, 0, nWidth, nHeight);

	UINT state = lpDrawItemStruct->itemState;

	COLORREF clrText =  RGB(88,88,88);

	int nStart = 0;

	if (state & ODS_SELECTED)
	{
		nStart = 2;
		clrText = RGB(0,0,0);
	}
	else if (state & ODS_DISABLED)
	{
		nStart = 3;
		clrText = GetSysColor(COLOR_GRAYTEXT);
	}
	else if (m_bHover)
	{
		nStart = 1;
		clrText = RGB(0,0,0);
	}

	if (nStart == 3 && m_pImageDisabled)
	{
		graph.DrawImage(m_pImageDisabled, rcF,0, 0, nWidth, nHeight, UnitPixel);
	}
	else
	{
		if(m_bHover && m_pImageSel)
		{

			graph.DrawImage(m_pImageSel, rcF,0, 0, nWidth, nHeight, UnitPixel);
		}else
		{

			graph.DrawImage(m_pImage, rcF, 0, 0, nWidth, nHeight, UnitPixel);
		}
	}

	// draw text 
	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		memDC.SetBkMode(TRANSPARENT);
		memDC.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		CRect rcText(&(lpDrawItemStruct->rcItem));

		if (state & ODS_SELECTED)
		{
			rcText.OffsetRect(1, 1);
		}

		memDC.SelectObject(&m_font);

		memDC.SetTextColor(clrText);

		rcText.left += 16;

		memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	// TODO:  Add your code to draw the specified item
}
