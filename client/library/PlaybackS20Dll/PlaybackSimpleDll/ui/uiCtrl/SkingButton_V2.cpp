// SkingButton_V2.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "SkingButton_V2.h"
#include ".\skingbutton_v2.h"


// CSkingButton_V2

IMPLEMENT_DYNAMIC(CSkingButton_V2, CButtonST)
CSkingButton_V2::CSkingButton_V2()
{
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;

	m_clrText = RGB(192, 192, 192);
	m_pImage = NULL;
	m_pImageSel = NULL;
	m_clbk = RGB(203, 203, 207);
	m_pImageDisabled = NULL;
}

CSkingButton_V2::~CSkingButton_V2()
{
	if (m_pImage)
	{
		delete m_pImage;
	}

	if(m_pImageSel)
	{

		delete m_pImageSel;
	}

	if (m_pImageDisabled)
	{
		delete m_pImageDisabled;
		m_pImageDisabled =NULL;
	}
}


BEGIN_MESSAGE_MAP(CSkingButton_V2, CButtonST)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()



// CSkingButton_V2 message handlers


BOOL CSkingButton_V2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CButtonST::OnEraseBkgnd(pDC);
}

void CSkingButton_V2::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
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

	CButtonST::OnMouseMove(nFlags, point);
}

BOOL CSkingButton_V2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;	
	}


	return CButtonST::PreTranslateMessage(pMsg);
}

void CSkingButton_V2::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);

	CButtonST::PreSubclassWindow();
}

void CSkingButton_V2::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	UINT state = lpDrawItemStruct->itemState;

	COLORREF clrText = m_clrText;

	Graphics graph(memDC.m_hDC);
	CRect rcImage = CRect(0, 0, nWidth, nHeight);
	if (state & ODS_SELECTED)
	{
		rcImage.OffsetRect(1, 1);
	}

	Rect rcF(rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height());


	int nStart = 0;

	if (state & ODS_SELECTED)
	{
		nStart = 2;
		clrText = GetSysColor(COLOR_BTNFACE);
	}
	else if (state & ODS_DISABLED)
	{
		nStart = 3;
		clrText = GetSysColor(COLOR_GRAYTEXT);
	}
	else if (m_bHover)
	{
		nStart = 1;
		clrText = GetSysColor(COLOR_BTNFACE);
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

		memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	// TODO:  Add your code to draw the specified item
}
LRESULT CSkingButton_V2::OnMouseHover(WPARAM /*wparam*/, LPARAM /*lparam*/) 
{
	// 鼠标移入按钮
	m_bHover = TRUE;

	Invalidate();

	return 0;
}

LRESULT CSkingButton_V2::OnMouseLeave(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	// 鼠标离开
	m_bMouseTrack = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return 0;
}

/**************************************************************************
*   Function:		Load//guolinlin
*   Description:    load button background by resource id 
*   Input:          pImage: the resource
*					BOOL bSizeToContent: move the button size as the background bitmap size
*   Output:         (null)
*   Return:			return void
**************************************************************************/
void CSkingButton_V2::Load(Image * pImage, Image * pImageSel, Image * pImageDisabled)
{
	if(pImage == NULL)
	{

		return;
	}
	if(m_pImage)
	{

		delete m_pImage;
		m_pImage = NULL;
	}
	m_pImage = pImage;

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	if(m_pImage == NULL)
	{

		return;
	}

	rc.right = rc.left + m_pImage->GetWidth();
	rc.bottom = rc.top + m_pImage->GetHeight();

	//SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0);
	if(m_pImageSel)
	{

		delete m_pImageSel;
		m_pImageSel = NULL;
	}

	if(pImageSel)
	{
		m_pImageSel = pImageSel;
	}

	if (m_pImageDisabled)
	{
		delete m_pImageDisabled;
		m_pImageDisabled = NULL;
	}
	if (pImageDisabled)
	{
		m_pImageDisabled = pImageDisabled;
	}

	LOGFONT lf = {0};
	lf.lfWeight = FW_NORMAL;
	lf.lfHeight = 14;
	::lstrcpy(lf.lfFaceName, _T("Arial"));

	m_font.CreateFontIndirect(&lf);

}