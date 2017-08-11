// Wnd_AP.cpp : implementation file
//
//lint -library
#include "stdafx.h"
//#include "PTZCtrlDemo.h"
#include "Wnd_AP.h"
#include ".\wnd_ap.h"


// CWnd_AP

IMPLEMENT_DYNAMIC(CWnd_AP, CWnd)
CWnd_AP::CWnd_AP()
{
}

CWnd_AP::~CWnd_AP()
{
}


BEGIN_MESSAGE_MAP(CWnd_AP, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// Draw Window Background


void CWnd_AP::Wnd_DrawBK(CDC * pDC)
{
}

BOOL CWnd_AP::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	Wnd_DrawBK(pDC);

	return CWnd::OnEraseBkgnd(pDC);
}

void CWnd_AP::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	Wnd_DrawBK(&dc);
}


