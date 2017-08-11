//lint -library
#include "stdafx.h"
#include "./IconStatic.h"


#define STATIC_BK_COLOR RGB(255, 255, 255)
#define STATIC_BKON_COLOR RESOURCELOAD_COLOR(IDS_STATIC_BKON_COLOR)
#define STATIC_BKSEL_COLOR RESOURCELOAD_COLOR(IDS_STATIC_BKSEL_COLOR)
#define STATIC_TEXT_COLOR RGB(0, 0, 0)


IMPLEMENT_DYNAMIC(CIconStatic, CStatic)
CIconStatic::CIconStatic()
{
	m_pImage = NULL;
	m_bHove = FALSE;
	m_bMouseTrack = FALSE;
	m_bSelect = FALSE;

	LOGFONT lf = {0};
	lf.lfWeight = FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, _T("Arial"));
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);
	m_strText = _T("");
// 	LOGFONT lfSel = {0};
// 	lfSel.lfWeight = FW_SEMIBOLD;
// 	lfSel.lfHeight = 14;
// 	::lstrcpy(lfSel.lfFaceName, _T("Arial"));
// 	m_fontSel.DeleteObject();
// 	m_fontSel.CreateFontIndirect(&lfSel);;

}

CIconStatic::~CIconStatic()
{
	try
	{
		if (m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}

		m_font.DeleteObject();
	}
	catch (...)
	{

	}
}

void CIconStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	DrawItem(&dc);

	// 不为绘图消息调用 CStatic::OnPaint()
}

void CIconStatic::DrawItem(CDC * pDC)
{

	if (m_pImage == NULL)
	{
		return;
	}

	CRect rcStatic;
	GetClientRect(&rcStatic);

	//     HRGN hRgn = CreateRoundRectRgn(rcStatic.left, rcStatic.top, rcStatic.right, rcStatic.bottom, 5, 5);
	//     CRgn * pRgn = CRgn::FromHandle(hRgn);
	//     CBrush bkBrush;
	//     bkBrush.DeleteObject();

	int nWidth = rcStatic.Width();
	int nHeight = rcStatic.Height();

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&memBitmap);

	Graphics graph(memDC.m_hDC);

	//Draw pic
	int nImageWidth = m_pImage->GetWidth();
	int nImageHeight = m_pImage->GetHeight();
	int nLeft = 4;
	int nTop = (nHeight - nImageHeight)/2;
	Rect rcImage(nLeft, nTop, nWidth, nHeight);

	int nSpace = 4;
	CRect rcText(nLeft+nImageWidth+nSpace, 0, nWidth-nImageWidth-nLeft-nSpace, nHeight);

	if (m_bSelect)
	{
		memDC.FillSolidRect(rcStatic, STATIC_BKSEL_COLOR);
	}
	else
	{
		if (m_bHove)
		{
			memDC.FillSolidRect(rcStatic, STATIC_BKON_COLOR);
			rcText.DeflateRect(2, 2, 0, 0);
		}
		else
		{
			memDC.FillSolidRect(rcStatic, STATIC_BK_COLOR);
		}
	}

	if (m_pImage)
	{
		graph.DrawImage(m_pImage, rcImage, 0, 0, nWidth, nHeight, UnitPixel);
	}

	//Draw text
	if (!m_strText.IsEmpty())
	{
		memDC.SelectObject(m_font);
		memDC.SetTextColor(STATIC_TEXT_COLOR);
		memDC.DrawText(m_strText, rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);

}

BEGIN_MESSAGE_MAP(CIconStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


LRESULT CIconStatic::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bHove = TRUE;

	Invalidate();

	return 0;
}

LRESULT CIconStatic::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bHove = FALSE;

	m_bMouseTrack = FALSE;

	Invalidate();

	return 0;
}

void CIconStatic::Load(UINT nID)
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	m_pImage = LoadImageFromResource(nID);
}

void CIconStatic::LoadImage(Image * pImage)
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	m_pImage = pImage;
}

//设置static是否处于选中状态
void CIconStatic::SetStaticSel(BOOL bSelect)
{
	m_bSelect = bSelect;
	Invalidate();
}

void CIconStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (!m_bMouseTrack)
	{
		Invalidate();

		TRACKMOUSEEVENT eventTrack;
		eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		eventTrack.hwndTrack = m_hWnd;
		eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
		eventTrack.dwHoverTime = 1;
		m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CIconStatic::SetText(const CString& strText)
{
	m_strText = strText;
}