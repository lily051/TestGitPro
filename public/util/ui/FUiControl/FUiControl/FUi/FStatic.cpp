// FStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "FStatic.h"

// CFStatic

IMPLEMENT_DYNAMIC(CFStatic, CStatic)

CFStatic::CFStatic()
: m_bgColor(GetSysColor(COLOR_WINDOW))
, m_clrText(GetSysColor(COLOR_WINDOWTEXT))
, m_hCursor(NULL)
, m_bTransparent(TRUE)
, m_nTextAlign(DT_LEFT | DT_VCENTER)
, m_strTextFontName(YAHEI_20_N)
{
}

CFStatic::~CFStatic()
{
}


BEGIN_MESSAGE_MAP(CFStatic, CStatic)
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// 设置鼠标悬浮时的指针样式
void CFStatic::SetHoverCursor(HCURSOR hCursor)
{
    m_hCursor = hCursor;

    ModifyStyle(0, SS_NOTIFY);
    PostMessage(WM_SETCURSOR, NULL, NULL);
}

// 设置文字样式
void CFStatic::SetTextFont(const CString& strFontName)
{
    m_strTextFontName = strFontName;
    UpdateFace();
}

// 设置控件的背景色
void CFStatic::SetBgcolor(BOOL bTransparent, COLORREF color)
{
    m_bTransparent = bTransparent;
    m_bgColor = color;
    UpdateFace();
}

// 设置文字颜色
void CFStatic::SetTextColor(COLORREF color)
{
    m_clrText = color;
    UpdateFace();
}

BOOL CFStatic::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CFStatic::PreSubclassWindow()
{
    ModifyStyle(0, SS_OWNERDRAW);

    CStatic::PreSubclassWindow();
}


BOOL CFStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hCursor)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CFStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//TRACE("55555555\n");
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
   
    CRect rcClient(lpDrawItemStruct->rcItem);

    if (!m_bTransparent)
    {
        pDC->FillSolidRect(rcClient, m_bgColor);
        RenderEngine->DrawColor(pDC, rcClient, m_bgColor);
    }

    CString strText;
    GetWindowText(strText);

    CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strTextFontName));
    RenderEngine->DrawTextX(pDC, strText, rcClient, m_clrText, m_nTextAlign);
    pDC->SelectObject(pOld);
}

void CFStatic::SetText(LPCTSTR szText)
{
    SetWindowText(szText);
    UpdateFace();
}

void CFStatic::UpdateFace()
{
    CRect rcClient;
	LONG lstyle = ::GetWindowLong(GetParent()->GetSafeHwnd(),GWL_STYLE);
	if (!m_bTransparent && (lstyle & WS_CLIPCHILDREN))
	{
		GetClientRect(rcClient);
		InvalidateRect(rcClient);
	}
	else
	{
		GetWindowRect(rcClient);
		GetParent()->ScreenToClient(rcClient);
		GetParent()->InvalidateRect(rcClient);
	}
}

void CFStatic::SetTextAlign(int nAlign)
{
    m_nTextAlign = nAlign;
    UpdateFace();
}

void CFStatic::SetTipText(LPCTSTR szTipText)
{
	_tcscpy_s(m_szTipText, MAX_LEN_TOOLTIP, szTipText);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP, (WPARAM)this, (LPARAM)m_szTipText);
}

LRESULT CFStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message >= WM_MOUSEFIRST &&message <= WM_MOUSELAST ) //VM_MOUSEMOVE
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP_EVENT, (WPARAM)this->GetSafeHwnd(), message);
	}
	return CStatic::WindowProc(message, wParam, lParam);
}
