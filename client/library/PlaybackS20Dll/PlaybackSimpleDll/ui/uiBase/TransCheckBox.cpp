//lint -library
#include "StdAfx.h"
#include "TransCheckBox.h"
#include "VolumnSetWnd.h"


IMPLEMENT_DYNAMIC(CTransCheckBox, CButton)
CTransCheckBox::CTransCheckBox()
{
	m_pFontText = NULL;
	m_pSolidBrush = NULL;

	m_bCheck = FALSE;
}

CTransCheckBox::~CTransCheckBox()
{
	try
	{
		if (m_pFontText)
		{
			delete m_pFontText;
			m_pFontText = NULL;
		}
		if (m_pSolidBrush)
		{
			delete m_pSolidBrush;
			m_pSolidBrush = NULL;
		}
	}
	catch (...)
	{
		
	}
	
}

BEGIN_MESSAGE_MAP(CTransCheckBox, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CTransCheckBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}

void CTransCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC * pDc = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nWidth = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDc, nWidth, nHeight);

	CDC memDc;
	memDc.CreateCompatibleDC(pDc);
	memDc.SelectObject(memBitmap);

	//if (GetParent()->IsKindOf(RUNTIME_CLASS(CVolumnSetWnd)))
	{
		((CVolumnSetWnd *)GetParent())->TransparentBk(&memDc, this);
	}

	CRect rcButton;
	GetClientRect(&rcButton);

	//绘制选择框
	CRect rcCheckBox(rcButton);
	rcCheckBox.left = 4;
	rcCheckBox.top = 6;
	rcCheckBox.right = rcCheckBox.left + 14;
	rcCheckBox.bottom = rcCheckBox.top + 14;
	memDc.DrawFrameControl(rcCheckBox, DFC_BUTTON, m_bCheck?DFCS_CHECKED :0);

	rcCheckBox.OffsetRect(16, 0);

	Graphics graph(memDc.m_hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	//绘制文字
	CString strWndText;
	GetWindowText(strWndText);
	graph.DrawString(strWndText.GetBuffer(), strWndText.GetLength(), m_pFontText, PointF((Gdiplus::REAL)rcCheckBox.left, (Gdiplus::REAL)rcCheckBox.top), m_pSolidBrush);

	//内存图片copy
	pDc->BitBlt(0, 0, nWidth, nHeight, &memDc, 0, 0, SRCCOPY);
}

void CTransCheckBox::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

int CTransCheckBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//Gdiplus::Font * pFontText = NULL;
	FontFamily fontFamily(_T("Arial"));
	m_pFontText = new Gdiplus::Font(&fontFamily, 11, FontStyleRegular, UnitPixel);

	m_pSolidBrush = new SolidBrush(/*Color(128, 128, 0)*/Color(25, 25, 25));

	return 0;
}

//设置选中状态
void CTransCheckBox::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
}
