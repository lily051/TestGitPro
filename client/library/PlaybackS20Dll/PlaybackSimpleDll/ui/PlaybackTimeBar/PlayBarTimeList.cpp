// PlayBarTimeList.cpp : implementation file
//
//lint -library
#include "stdafx.h"
//#include "ControlCenter.h"
#include "PlayBarTimeList.h"
#include ".\playbartimelist.h"
#include "PlayBarTimeCtrl.h"

// CPlayBarTimeList

IMPLEMENT_DYNAMIC(CPlayBarTimeList, CWnd)
CPlayBarTimeList::CPlayBarTimeList()
: m_iLineHeit(0)
, m_iListStart(0)
, m_lRealHeit(0)
, m_nVScrollPos(0)
{
}

CPlayBarTimeList::~CPlayBarTimeList()
{
	RemoveALlElement();
}


BEGIN_MESSAGE_MAP(CPlayBarTimeList, CWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPlayBarTimeList message handlers


BOOL CPlayBarTimeList::Create(const RECT & rect, CWnd * pParentWnd,int iListStart,int iLineHeit)
{
	CWnd* pWnd = this;

	m_iLineHeit = iLineHeit;
	m_iListStart = iListStart;

	return pWnd->Create(NULL, _T("PlayBarTimeList"), WS_VISIBLE|WS_CHILD|WS_TABSTOP|WS_VSCROLL, rect, pParentWnd,0xffff);

	return 0;
}

void CPlayBarTimeList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlayBarTimeList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int yOrig = GetScrollPos(SB_VERT);
	int nDelta = 0;

	switch (nSBCode)
	{
		/*case SB_TOP:
		y = 0;
		break;
		case SB_BOTTOM:
		y = INT_MAX;
		break;*/
	case SB_LINEUP:
		nDelta = -2;
		break;
	case SB_LINEDOWN:
		nDelta = 3;
		break;
	case SB_PAGEUP:
		nDelta = -30;
		break;
	case SB_PAGEDOWN:
		nDelta = 30;
		break;
	case SB_THUMBTRACK:
		nDelta = nPos - yOrig;
		break;
	default:
		return;
	}

	// adjust current x position
	int yMax = GetScrollLimit(SB_VERT);

	int y = yOrig;

	y += nDelta;

	if (y < 0)
	{
		y = 0;
	}
	else if (y > yMax)
	{
		y = yMax;
	}

	if (y == yOrig)
		return;

	// do scroll and update scroll positions
	m_nVScrollPos = y;

	ScrollWindow(0, -(y-yOrig));

	if (y != yOrig)
		SetScrollPos(SB_VERT, y, TRUE);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CPlayBarTimeList::AddElement(int iCameraID)
{
	CPlayBarTimeELement * pElement = NULL;

	if(m_ElementMap.Lookup(iCameraID,(void * &)pElement))
	{
		return FALSE;
	}

	int iSize = m_ElementMap.GetCount();

	CRect rectElement(0,0,0,0),rect(0,0,0,0);

	GetClientRect(&rect);
	pElement = new CPlayBarTimeELement;

	rectElement = rect;
	rectElement.right = rectElement.left + GetParentWid();
	rectElement.top = iSize * m_iLineHeit;
	rectElement.bottom = rectElement.top + m_iLineHeit;

	pElement->Create(rectElement,this,iCameraID,m_iListStart);

	m_ElementMap[iCameraID] = pElement;

	m_lRealHeit = (iSize + 1) * m_iLineHeit;

	if(m_hWnd)
		SetScroll();

	return 1;
}

BOOL CPlayBarTimeList::RemoveElement(int iCameraID)
{
	CPlayBarTimeELement * pElement = NULL;
	if(!m_ElementMap.Lookup(iCameraID,(void * &)pElement))
	{
		return FALSE;
	}

	if(pElement)
		delete pElement;

	pElement = NULL;

	m_ElementMap.RemoveKey(iCameraID);

	m_lRealHeit -= m_iLineHeit;

	if(m_hWnd)
		SetScroll();

	return 1;
}

void CPlayBarTimeList::RemoveALlElement(void)
{
	POSITION pos = NULL;
	CPlayBarTimeELement * pElement = NULL;
	WORD wID = 0;

	pos = m_ElementMap.GetStartPosition();

	while(pos != NULL)
	{
		m_ElementMap.GetNextAssoc(pos,wID,(void * &)pElement);

		delete pElement;

		pElement = NULL;
	}

	m_ElementMap.RemoveAll();

	m_lRealHeit = 0;

	if(m_hWnd)
		SetScroll();
}


void CPlayBarTimeList::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	SCROLLINFO si;

	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nPage = cy;

	if(cy > m_lRealHeit)
		si.nMax = cy - 1;
	else
		si.nMax = m_lRealHeit - 1;
		

	SetScrollInfo(SB_VERT, &si, TRUE);

	// TODO: Add your message handler code here
}

void CPlayBarTimeList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);

	dc.SetWindowOrg(rect.Width(), m_nVScrollPos);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = NULL;

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());

	pOldBitmap = memDC.SelectObject(&bmp);

	memDC.FillSolidRect(&rect,RGB(195,221,246));

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);

	// Do not call CWnd::OnPaint() for painting messages
}

void CPlayBarTimeList::SetScroll(void)
{
	CRect rect(0,0,0,0);

	GetClientRect(&rect);

	SCROLLINFO si;

	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;
	si.nMin = 0;
	si.nPos = 0;
	si.nPage = rect.Height();
	if(rect.Height() > m_lRealHeit)
		si.nMax = rect.Height() - 1;
	else
		si.nMax = m_lRealHeit - 1;
	SetScrollInfo(SB_VERT, &si, TRUE);

}

BOOL CPlayBarTimeList::SetRecordSlice(PSECTIONINFO pSegInfo,int iCameraID)
{
	POSITION pos = m_ElementMap.GetStartPosition();
	WORD wLsh = 0;

	CPlayBarTimeELement * pElement = NULL;

	while(pos != NULL)
	{
		m_ElementMap.GetNextAssoc(pos,wLsh,(void * &)pElement);

		if(pElement)
		{
			if(iCameraID >= 0)
			{
				if(pElement->GetID() == iCameraID)
				{
					pElement->SetRecordList(pSegInfo);
				}
			}else
				pElement->SetRecordList(pSegInfo);
		}
	}
	return 0;
}

BOOL CPlayBarTimeList::SetSelect(BOOL bSelect, int nCameraID)
{
	CPlayBarTimeELement * pElement = NULL;

	if(m_ElementMap.Lookup(nCameraID,(void * &)pElement))
	{
		pElement->SetSelect(bSelect);
		return 1;
	}
	return 0;
}

BOOL CPlayBarTimeList::IsSelect(int nCameraID)
{
	CPlayBarTimeELement * pElement = NULL;

	if(m_ElementMap.Lookup(nCameraID,(void * &)pElement))
	{
		return pElement->IsSelected();
	}

	return 0;
}

int CPlayBarTimeList::GetParentWid(void)
{
	CPlayBarTimeCtrl * pParent = (CPlayBarTimeCtrl *)GetParent();
	return pParent->GetWid();
}

void CPlayBarTimeList::InvalidateList(void)
{
	POSITION pos = m_ElementMap.GetStartPosition();
	WORD wLsh = 0;
	CPlayBarTimeELement * pElement = NULL;

	while(pos != NULL)
	{
		m_ElementMap.GetNextAssoc(pos,wLsh,(void  * &)pElement);

		if(pElement)
			pElement->Invalidate();
	}
}

void CPlayBarTimeList::RebuildList()
{
	POSITION pos = m_ElementMap.GetStartPosition();
	WORD wLsh = 0;
	CPlayBarTimeELement * pElement = NULL;

	while(pos != NULL)
	{
		m_ElementMap.GetNextAssoc(pos,wLsh,(void  * &)pElement);

		if(pElement)
			pElement->BuildRecordCell();
	}
}
