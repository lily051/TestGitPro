#include "stdafx.h"
#include "./TransButton.h"
#include "DownloadingDlg.h"
//lint -library
#include "DownSuccessDlg.h"
#include "DownFailedDlg.h"


IMPLEMENT_DYNAMIC(CTransButton, CButton)
CTransButton::CTransButton()
{
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;
	m_bLock = FALSE;
	m_bSelect = FALSE;

	m_pImage = NULL;
	m_pImageHoverBk = NULL;
	m_pImageSelBk = NULL;

	m_pFontText = NULL;
	m_pSolidBrush = NULL;

	m_bInit = FALSE;

	m_nBtnID = 0;

};

CTransButton::~CTransButton()
{
	try
	{
		if (m_pImage)
		{
			delete m_pImage;
		}
		if (m_pImageHoverBk)
		{
			delete m_pImageHoverBk;
		}
		if (m_pImageSelBk)
		{
			delete m_pImageSelBk;
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

BEGIN_MESSAGE_MAP(CTransButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	/*ON_CONTROL_REFLECT(BN_CLICKED, &CTransButton::OnBnClicked)*/
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CTransButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	REAL nLeftOffset = 2;
	REAL nTopOffset = 4;
	UINT state = lpDrawItemStruct->itemState;

	RectF rcF(nLeftOffset, nTopOffset, (REAL)m_pImage->GetWidth(), (REAL)m_pImage->GetHeight());
	CRect rcWnd;
	GetClientRect(&rcWnd);

	if (GetParent()->IsKindOf(RUNTIME_CLASS(CDownloadingDlg)))
	{
		((CDownloadingDlg *)GetParent())->TransparentBk(&memDc, this);
	}
	else if (GetParent()->IsKindOf(RUNTIME_CLASS(CDownSuccessDlg)))
	{
		((CDownSuccessDlg *)GetParent())->TransparentBk(&memDc, this);
	}
	else if (GetParent()->IsKindOf(RUNTIME_CLASS(CDownFailedDlg)))
	{
		((CDownFailedDlg *)GetParent())->TransparentBk(&memDc, this);
	}

	Graphics graph(memDc.m_hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	Image * pImage = m_pImage;

	//文字位置>.
	int nLeft = 20;
	if (m_pImage)
	{
		nLeft = m_pImage->GetWidth() + 4 + 4;
	}
	TEXTMETRIC tm;
	pDc-> GetTextMetrics(&tm); 
	int nTop = (rcWnd.Height() - (tm.tmHeight + tm.tmExternalLeading)) / 2;

	if (state & ODS_SELECTED)
	{
		if (m_pImageSelBk)
		{
			graph.DrawImage(m_pImageSelBk, 1, 1, 80, 24);
		}
		nTop = nTop + 1;
		nLeft = nLeft + 1;
	}
	else
	{
		if (m_bHover)
		{
			if (m_pImageHoverBk)
			{
				graph.DrawImage(m_pImageHoverBk, 1, 1, 80, 24);
			}
		}
	}

	if (m_pImage)
	{
		graph.DrawImage(m_pImage, rcF);
	}

	CString strText;
	GetWindowText(strText);
	graph.DrawString(strText.GetBuffer(), strText.GetLength(), m_pFontText, PointF((Gdiplus::REAL)nLeft, (Gdiplus::REAL)nTop), m_pSolidBrush);

	pDc->BitBlt(0, 0, nWidth, nHeight, &memDc, 0, 0, SRCCOPY);

}

void CTransButton::OnMouseMove(UINT nFlags, CPoint point)
{
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

	CButton::OnMouseMove(nFlags, point);
}

void CTransButton::OnMouseLeave()
{
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;
	Invalidate();

	CButton::OnMouseLeave();
}

void CTransButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bHover = TRUE;

	Invalidate();

	CButton::OnMouseHover(nFlags, point);
}

BOOL CTransButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;
	}

	return CButton::PreTranslateMessage(pMsg);
}

void CTransButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

//加载图片、文字
void CTransButton::Load(UINT nIDResource)
{
	m_pImage = ::LoadImageFromResource(nIDResource);

	if (!m_bInit)
	{
		Gdiplus::Font * pFontText = NULL;
		FontFamily fontFamily(_T("Arial"));
		m_pFontText = new Gdiplus::Font(&fontFamily, 11, FontStyleRegular, UnitPixel);

		m_pSolidBrush = new SolidBrush(/*Color(128, 128, 0)*/Color(25, 25, 25));

		m_bInit = TRUE;
	}
}

//加载选中背景图片
void CTransButton::LoadSelectBkgnd(UINT nIDHover, UINT nIDSelect)
{
	m_pImageHoverBk = ::LoadImageFromResource(nIDHover);
	m_pImageSelBk = ::LoadImageFromResource(nIDSelect);
}

int CTransButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	Gdiplus::Font * pFontText = NULL;
	FontFamily fontFamily(_T("Arial"));
	m_pFontText = new Gdiplus::Font(&fontFamily, 11, FontStyleRegular, UnitPixel);

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
BOOL CTransButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;

	//return CButton::OnEraseBkgnd(pDC);
}

//锁定状态
void CTransButton::LockState()
{
	m_bLock = TRUE;

}

//设置按钮选中状态
void CTransButton::SetSelect(BOOL bSelect)
{
	if (m_bSelect != bSelect)
	{
		m_bSelect = bSelect;
		Invalidate();
	}
}