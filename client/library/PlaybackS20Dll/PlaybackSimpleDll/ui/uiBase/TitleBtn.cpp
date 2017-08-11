//lint -library
#include "stdafx.h"
#include "./TitleBtn.h"


IMPLEMENT_DYNAMIC(CTitleButton, CButton)
CTitleButton::CTitleButton()
{
	m_pImage = NULL;

	m_pFontText = NULL;
	m_pSolidBrush = NULL;

	m_nFontSize = 14;

	m_bInit = FALSE;

	m_bCenter = FALSE;

};

CTitleButton::~CTitleButton()
{
	try
	{
		if (m_pImage)
		{
			delete m_pImage;
		}
		if (m_pFontText)
		{
			delete m_pFontText;
		}
		if (m_pSolidBrush)
		{
			delete m_pSolidBrush;
		}

	}
	catch (...)
	{

	}

}

BEGIN_MESSAGE_MAP(CTitleButton, CButton)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CTitleButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (NULL == m_pImage)
	{
		return;
	}

	CDC * pDc = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nWidth = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDc, nWidth, nHeight);

	CDC memDc;
	memDc.CreateCompatibleDC(pDc);
	memDc.SelectObject(memBitmap);

	CRect rcWnd;
	GetClientRect(&rcWnd);

	Graphics graph(memDc.m_hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	if (m_pImage)
	{
		graph.DrawImage(m_pImage, 0, 0, nWidth*2, nHeight);
	}

	CString strText;
	GetWindowText(strText);
	TEXTMETRIC tm;
	int nLeft = 4;
	if (m_bCenter)
	{
		nLeft = (rcWnd.Width() - m_nFontSize*strText.GetLength()) / 2;
	}
	pDc-> GetTextMetrics(&tm); 
	int nTop = (rcWnd.Height() - (tm.tmHeight + tm.tmExternalLeading)) / 2;
	graph.DrawString(strText.GetBuffer(), strText.GetLength(), m_pFontText, PointF((Gdiplus::REAL)nLeft, (Gdiplus::REAL)nTop), m_pSolidBrush);

	pDc->BitBlt(0, 0, nWidth, nHeight, &memDc, 0, 0, SRCCOPY);

}

BOOL CTitleButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;
	}

	return CButton::PreTranslateMessage(pMsg);
}

void CTitleButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

//加载图片
void CTitleButton::Load(UINT nIDResource)
{
	m_pImage = ::LoadImageFromResource(nIDResource);

	if (!m_bInit)
	{
		Gdiplus::Font * pFontText = NULL;
		FontFamily fontFamily(_T("宋体"));
		m_pFontText = new Gdiplus::Font(&fontFamily, (Gdiplus::REAL)m_nFontSize, FontStyleRegular, UnitPixel);

		m_pSolidBrush = new SolidBrush(/*Color(128, 128, 0)*/Color(25, 25, 25));

		m_bInit = TRUE;
	}
}

int CTitleButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	Gdiplus::Font * pFontText = NULL;
	FontFamily fontFamily(_T("宋体"));
	m_pFontText = new Gdiplus::Font(&fontFamily, (Gdiplus::REAL)m_nFontSize, FontStyleRegular, UnitPixel);

	m_pSolidBrush = new SolidBrush(/*Color(128, 128, 0)*/Color(25, 25, 25));

	m_bInit = TRUE;

	return 0;
}

/** @fn 
*   @brief 此函数重写是为了避免在鼠标快速移动下btn背景白色闪烁
*   @param NULL
*   @param NULL
*   @return NULL
*/
BOOL CTitleButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;

	//return CButton::OnEraseBkgnd(pDC);
}

//文字居中
void CTitleButton::CenterText(BOOL bCenter)
{
	m_bCenter = bCenter;
	Invalidate();
}
