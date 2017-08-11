/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       AppLogo.cpp
*   Description:    the control to show the application name and version
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/
//lint -library

// AppLogo.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "../MainDialog.h"
#include "AppLogo.h"
#include "MemDCAla.h"
#include "../XMLFunction.h"

// CAppLogo

IMPLEMENT_DYNAMIC(CAppLogo, CStatic)
CAppLogo::CAppLogo()
{
    m_logoName = XML_GetLogoName();
}

CAppLogo::~CAppLogo()
{
}


BEGIN_MESSAGE_MAP(CAppLogo, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CAppLogo message handlers


void CAppLogo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CMemDCAla memDC(&dc, this);
	CDC * pMemDC = memDC.GetDC();

	if (GetParent()->IsKindOf(RUNTIME_CLASS(CMainDialog)))
	{
		((CSkinDialog *)GetParent())->TransparentBkgnd(pMemDC, this);
	}

	Graphics graph(pMemDC->m_hDC);
	
	FontFamily fontFamily(L"Arial");

	graph.SetTextRenderingHint(TextRenderingHintAntiAlias );

    // draw LOGO name
    // white
	SolidBrush solidBrush(Color(255, 255, 255, 255));
	//English,fontApp字体，PointF位置
	Gdiplus::Font fontApp(&fontFamily, 22, FontStyleBold, UnitPixel);

	CStringW strAppName = CA2W((LPCTSTR)m_logoName);
	graph.DrawString(strAppName.GetBuffer(0), strAppName.GetLength(), &fontApp, PointF(10, 10), &solidBrush);
	strAppName.ReleaseBuffer(0);

// 	//Chinese
// 	Gdiplus::Font fontApp2(&fontFamily, 14, FontStyleBold, UnitPixel);
// 	CStringW strAppName2 = CA2W("报警服务器V3.0");
// 	graph.DrawString(strAppName2.GetBuffer(0), strAppName2.GetLength(), &fontApp2, PointF(38, 40), &solidBrush);
// 	strAppName2.ReleaseBuffer(0);

	// Do not call CStatic::OnPaint() for painting messages
}
