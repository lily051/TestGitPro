#include "stdafx.h"
#include "PlayTitleWnd.h"

//lint -library
IMPLEMENT_DYNAMIC(CPlayTitleWnd, CWnd)
CPlayTitleWnd::CPlayTitleWnd()
{
	m_clrBk = UI_PLAYWND_TITLE_COLOR;
	m_clrText = RGB(212, 212, 212);
	m_strText = _T("");

	//设置标题字体
	m_fTextFont.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, _T("Arial"));
}

CPlayTitleWnd::~CPlayTitleWnd()
{
	try
	{
		m_fTextFont.DeleteObject();
	}
	catch (...)
	{
		
	}
}


BEGIN_MESSAGE_MAP(CPlayTitleWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CPlayTitleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPlayTitleWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	
	// 不为绘图消息调用 CWnd::OnPaint()
}

//设置文本
void CPlayTitleWnd::SetText(CString strText)
{
	m_strText = strText;
	UpdateSurface();
}

//设置标题文本颜色
void CPlayTitleWnd::SetTextColour(COLORREF clrText)
{
	m_clrText = clrText;
}

//设置文本字体
void CPlayTitleWnd::SetTextFont(CFont* fTextFont)
{
	memcpy(&m_fTextFont, fTextFont, sizeof(CFont));
}

//获取标题文字
CString CPlayTitleWnd::GetText()
{
	return m_strText;
}

//刷新
void CPlayTitleWnd::UpdateSurface()
{
	CRect rc;
	GetWindowRect(rc);
	RedrawWindow();

	Invalidate(TRUE);
	UpdateWindow();
}

BOOL CPlayTitleWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	pDC->SetTextColor(m_clrText);
	pDC->SelectObject(&m_fTextFont);
	pDC->DrawText(m_strText, rcClient, NULL);
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}