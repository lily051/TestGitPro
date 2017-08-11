// PlayBarTimeELement.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "PlayBarTimeELement.h"
#include ".\playbartimeelement.h"
#include "memdcex.h"
#include "PlayBarTimeCtrl.h"



// CPlayBarTimeELement

IMPLEMENT_DYNAMIC(CPlayBarTimeELement, CWnd)
CPlayBarTimeELement::CPlayBarTimeELement()
: m_nCameraID(0)
, m_pParent(NULL)
, m_iLineStart(0)
, m_bHighLight(FALSE)
, m_pRecordCell(NULL)
, m_pSection(NULL)
, m_bSelect(FALSE)
{
}

CPlayBarTimeELement::~CPlayBarTimeELement()
{
	ClearSection();

	ClearRecordCell();
}


BEGIN_MESSAGE_MAP(CPlayBarTimeELement, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CPlayBarTimeELement message handlers


void CPlayBarTimeELement::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	DrawUI(&dc);
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CPlayBarTimeELement::Create(const RECT & rect, CWnd * pParentWnd, int nChannelID,int iLineStart)
{
	m_nCameraID = nChannelID;

	m_iLineStart = iLineStart;

	m_pParent = pParentWnd;

	CWnd* pWnd = this;

	if(pWnd->Create(NULL, _T("PlayBarTimeELement"), WS_VISIBLE|WS_CHILD|WS_TABSTOP, rect, pParentWnd, 0xfff0))
	{
		m_SelectRect.top = (rect.bottom - rect.top - 13)/2;
		m_SelectRect.bottom = m_SelectRect.top + 13;
		m_SelectRect.left = 3;
		m_SelectRect.right = 16;
		return 1;
	}

	return 0;
}

void CPlayBarTimeELement::DrawUI(CDC * pDC)
{
	CRect rect(0,0,0,0);

	GetClientRect(&rect);


	CMemDCEX memDC(pDC,rect);

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(pDC);
	CBitmap bmpBake;
	CBitmap * pOldBmp = NULL;
	CBrush * pOldBrush = NULL;
	CPen * pOldPen = NULL;
	CFont * pOldFont = NULL;

	bmpBake.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBmp = bitmapDC.SelectObject(&bmpBake);

	pOldBrush = bitmapDC.SelectObject(&m_Brush[7]);

	bitmapDC.Rectangle(&rect);

	if(m_bHighLight)
	{
		pOldBrush = bitmapDC.SelectObject(&m_Brush[4]);
		bitmapDC.SetTextColor(COLOR_WNDNAMETEXT);
	}
	else
	{
		pOldBrush = bitmapDC.SelectObject(&m_Brush[6]);
		bitmapDC.SetTextColor(COLOR_WNDNAMETEXT);
	}

	bitmapDC.SelectStockObject(NULL_PEN);

	CRect rectScreen = rect;
	CRect rectText;
	rectScreen.right = m_iLineStart;

	bitmapDC.Rectangle(&rectScreen);

	CString strText = _T("");

	strText.Format(_T("窗口%02d"),m_nCameraID + 1);

	pOldFont = bitmapDC.SelectObject(&m_Font);

	bitmapDC.SetBkMode(TRANSPARENT);

	rectText = rectScreen;
//	rectText.left += 4;
//	rectText.top += 4;

	//ljg 注释掉的，不显示名称 2010-8-3 16:07:18
	//bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectText,\
	//	DT_CENTER | DT_BOTTOM);
	//ljg 注释掉的结束

//	bitmapDC.DrawTextEx(strText.GetBuffer(0),strText.GetLength(),&rectText,\
//		DT_CENTER | DT_BOTTOM, 0);

	CRect rectList(0,0,0,0);

	rectList = rect;
	rectList.left = m_iLineStart;
	rectList.bottom -= 1;

	if(m_bHighLight)   //这里必须执行一个分支，那么在这之前对pOldPen的赋值依然扯淡
	{
		pOldBrush = bitmapDC.SelectObject(&m_Brush[6]);
		pOldPen = bitmapDC.SelectObject(&m_pen[6]);
	}
    else
	{
		pOldBrush = bitmapDC.SelectObject(&m_Brush[4]);
		pOldPen = bitmapDC.SelectObject(&m_pen[4]);
	}

	bitmapDC.Rectangle(&rectList);

	PRECORD_CELL pRecord = m_pRecordCell;

	CRect rectCell(0,0,0,0);
	long lShowEnd = GetCurLen();
	long lShowStart = lShowEnd - rect.Width() + m_iLineStart;
	int iDataType = 0;
	int iSize = (rect.Height() - 5)/4;

	while(pRecord)
	{
		if(pRecord->m_lEndPoint > lShowStart && pRecord->m_lStartPoint < lShowEnd)
		{
			iDataType = pRecord->m_iDataType;

			rectCell = rect;

			rectCell.bottom--;

			rectCell.left = pRecord->m_lStartPoint - lShowStart + m_iLineStart;

			if(rectCell.left < m_iLineStart)
				rectCell.left = m_iLineStart;

			rectCell.right = pRecord->m_lEndPoint - lShowStart + m_iLineStart;

			if(rectCell.right > rect.right)
				rectCell.right = rect.right;

			pOldBrush = bitmapDC.SelectObject(&m_Brush[iDataType]);
			pOldPen = bitmapDC.SelectObject(&m_pen[iDataType]);

			bitmapDC.Rectangle(&rectCell);
		}

		pRecord = pRecord->m_pNext;
	}

	int iCellWid = GetCellWid();

	int iLeft = lShowStart % iCellWid;

	int x = iCellWid - iLeft + m_iLineStart;

	//pOldPen = bitmapDC.SelectObject(&m_pen[7]);  //根据上下文逻辑，这里是必须执行的，那之前的所有赋值都成了扯淡

	while( x < rect.right)
	{
		//中间隔开录像
		//bitmapDC.MoveTo(x,rect.top);
		//bitmapDC.LineTo(x,rect.bottom);
		x += iCellWid;
	}

	long lSelPos = GetSelPos();

	int xSel = lSelPos - lShowStart + m_iLineStart;

	if(xSel >= m_iLineStart || xSel <= rect.right)
	{
//		if(m_bHighLight)
		    //滑动条颜色
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(188, 112, 237));
		pOldPen = bitmapDC.SelectObject(&pen);
//		else
//			pOldPen = bitmapDC.SelectObject(&m_pen[6]);

		bitmapDC.MoveTo(xSel,rect.top);
		bitmapDC.LineTo(xSel,rect.bottom - 1);

//		if(m_bHighLight)
		{
			int iPointTop = rect.top;
			int iHeit = 3;

			while(iHeit > 0)
			{
				bitmapDC.MoveTo(xSel - iHeit,iPointTop);
				bitmapDC.LineTo(xSel + iHeit + 1,iPointTop);

				iPointTop++;
				iHeit--;
			}

			iHeit = 3;

			iPointTop = rect.bottom - 2;

			while(iHeit > 0)
			{
				bitmapDC.MoveTo(xSel - iHeit,iPointTop);
				bitmapDC.LineTo(xSel + iHeit + 1,iPointTop);

				iPointTop--;
				iHeit--;
			}
		}		
	}
	
	memDC.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&bitmapDC,0,0,SRCCOPY);

//	pOldBmp = bitmapDC.SelectObject(&m_SelBmp[m_bSelect]);

//	memDC.BitBlt(m_SelectRect.left,m_SelectRect.top,m_SelectRect.Width(),m_SelectRect.Height(),&bitmapDC,0,0,SRCCOPY);

	if(pOldBmp)
		bitmapDC.SelectObject(pOldBmp);

	if(pOldBrush)
		bitmapDC.SelectObject(pOldBrush);

	if(pOldFont)
		bitmapDC.SelectObject(pOldFont);

	if(pOldPen)
		bitmapDC.SelectObject(pOldPen);


}

void CPlayBarTimeELement::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	m_pen[0].CreatePen(PS_SOLID,1,UI_PLAYBAR_RECORDTIMEING);
	m_pen[1].CreatePen(PS_SOLID,1,UI_PLAYBAR_RECORDMANUAL);
	m_pen[2].CreatePen(PS_SOLID,1,UI_PLAYBAR_RECORDALARM);
	m_pen[3].CreatePen(PS_SOLID,1,COLOR_NOR8);
	m_pen[4].CreatePen(PS_SOLID,1,COLOR_RECORD_BLACK);
	m_pen[5].CreatePen(PS_SOLID,1,COLOR_WNDNAMETEXT);
	m_pen[6].CreatePen(PS_SOLID,1,COLOR_WNDNAMEBK);
	m_pen[7].CreatePen(PS_SOLID,1,COLOR_BORDER);
	m_pen[8].CreatePen(PS_SOLID,1,COLOR_NOR9);

	m_Brush[0].CreateSolidBrush(UI_PLAYBAR_RECORDTIMEING);
	m_Brush[1].CreateSolidBrush(UI_PLAYBAR_RECORDMANUAL);
	m_Brush[2].CreateSolidBrush(UI_PLAYBAR_RECORDALARM);
	m_Brush[3].CreateSolidBrush(COLOR_NOR8);
	m_Brush[4].CreateSolidBrush(COLOR_RECORD_BLACK);
	m_Brush[5].CreateSolidBrush(COLOR_WNDNAMETEXT);
	m_Brush[6].CreateSolidBrush(COLOR_WNDNAMEBK);
	m_Brush[7].CreateSolidBrush(COLOR_BORDER);


//	m_SelBmp[0].LoadBitmap(IDB_BITMAP_NOSELLIST);
//	m_SelBmp[1].LoadBitmap(IDB_BITMAP_SELLIST);

	m_Font.CreateFont( 15,6,0,0,FW_BOLD,0,0,0,\
	DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
	
	CWnd::PreSubclassWindow();
}

BOOL CPlayBarTimeELement::SetbHighLight(bool bHighLight)
{
	m_bHighLight = bHighLight;
	return 0;
}

BOOL CPlayBarTimeELement::SetRecordList(PSECTIONINFO pSegInfo)
{
	ClearSection();

	PSECTIONINFO pTemp = NULL,pSeg = NULL,pPre = NULL;

	time_t ctStart = GetStartTime();

	time_t ctEnd = GetEndTime();

	time_t ctS,ctE;

	if(pSegInfo)
	{
		pTemp= pSegInfo;

		while(pTemp)
		{
			if(pTemp->iCameraID == m_nCameraID)
			{
				ctS = pTemp->tTimeStart;
				ctE = pTemp->tTimeEnd;

				if(ctS < ctStart)
					ctS = ctStart;

				if(ctE > ctEnd)
					ctE = ctEnd;

				if(ctE <= ctS)
				{
					pTemp = pTemp->pNext;
					continue;
				}
				
				pSeg = new SCTIOININFO;

				memcpy(pSeg,pTemp,sizeof(SCTIOININFO));

				pSeg->tTimeStart = (unsigned int)ctS;

				pSeg->tTimeEnd = (unsigned int)ctE;

				pSeg->pNext = NULL;

				if(m_pSection == NULL)
					m_pSection = pSeg;
			
				if(pPre)
					pPre->pNext = pSeg;

				pPre = pSeg;
			}

			pTemp = pTemp->pNext;
		}
	}

	BuildRecordCell();

	return 0;
}

void CPlayBarTimeELement::ClearSection(void)
{
	PSECTIONINFO pTemp = NULL,
		pList = NULL;

	pList = m_pSection;

	while(pList)
	{
		pTemp = pList->pNext;

		delete pList;

		pList = pTemp;
	}

	m_pSection = NULL;
}

time_t CPlayBarTimeELement::GetStartTime()
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetStartTime();

}

time_t CPlayBarTimeELement::GetEndTime()
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetEndTime();
}

void CPlayBarTimeELement::ClearRecordCell(void)
{
	PRECORD_CELL pRecord = NULL,pTemp = NULL;

	pRecord = m_pRecordCell;

	while(pRecord)
	{
		pTemp = pRecord->m_pNext;

		delete pRecord;

		pRecord = pTemp;
	}

	m_pRecordCell = NULL;
}

BOOL CPlayBarTimeELement::BuildRecordCell(void)
{
	ClearRecordCell();

	PRECORD_CELL pRecordTemp = NULL,pRecorList = NULL,pPre = NULL;

	PSECTIONINFO pSeg = m_pSection;

	long lRealLen = GetRealLen();
	time_t ctStart = GetStartTime();
	time_t ctEnd = GetEndTime();
	long lStart = 0,lEnd = 0;


	while(pSeg)
	{
		lStart = (long)((pSeg->tTimeStart - ctStart) * lRealLen / (ctEnd - ctStart));
		lEnd = (long)((pSeg->tTimeEnd - ctStart) * lRealLen / (ctEnd - ctStart));

		if(lStart >= lEnd)
			lEnd = lStart + 1;

		if(lStart < 0 || lStart > lRealLen || lEnd < 0 || lEnd > lRealLen)
		{
			pSeg = pSeg->pNext;

			continue;
		}

		pRecordTemp = new RECORD_CELL;

		pRecordTemp->m_iDataType = pSeg->iDataType;

		pRecordTemp->m_lStartPoint = lStart;

		pRecordTemp->m_lEndPoint = lEnd;

		pRecordTemp->m_pNext = NULL;

		if(pRecorList == NULL)
			pRecorList = pRecordTemp;

		if(pPre)
			pPre->m_pNext = pRecordTemp;

		pPre = pRecordTemp;

		pSeg = pSeg->pNext;
	}

	m_pRecordCell = pRecorList;

	Invalidate(TRUE);

	return 0;
}

long CPlayBarTimeELement::GetRealLen(void)
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetRealLenForElement();
}

long CPlayBarTimeELement::GetCurLen(void)
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetCurLenForElement();
}

void CPlayBarTimeELement::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rect(0,0,0,0);

	GetClientRect(&rect);

	CRect rectSel = rect;

	rectSel.right = m_iLineStart;

/*	if(rectSel.PtInRect(point))
	{
		m_bSelect = !m_bSelect;

//		int * pSelect = new int(m_bSelect);
//		int * pCameraID = new int(m_nCameraID);

		CPlayBarTimeCtrl * pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();


		::PostMessage(pParent->m_hWnd,EVENT_SELECT,(WPARAM)(&m_bSelect),(LPARAM)(&m_nCameraID));

		m_bHighLight = m_bSelect;

		Invalidate();
	}else*/
	{
		if(point.x >= m_iLineStart && point.x <= rect.right)
		{
			long lCurLen = GetCurLen();

			long lShowStart = lCurLen - rect.Width() + m_iLineStart;

			long lSelPos = lShowStart + point.x - m_iLineStart;

			CPlayBarTimeCtrl * pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

			pParent->SetSelPos(lSelPos);

			Invalidate();
		}

	
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CPlayBarTimeELement::SetSelect(BOOL bSelect)
{
	m_bSelect = bSelect;

	m_bHighLight = bSelect;

	Invalidate();
}

BOOL CPlayBarTimeELement::IsSelected(void)
{
	return m_bSelect;
}

int CPlayBarTimeELement::GetCellWid(void)
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetCellWidForElement();
	return 0;
}

long CPlayBarTimeELement::GetSelPos(void)
{
	CPlayBarTimeCtrl *pParent = (CPlayBarTimeCtrl *)GetParent()->GetParent();

	return pParent->GetSelPosForElement();
	return 0;
}

int CPlayBarTimeELement::GetID()
{
	return m_nCameraID;
}
