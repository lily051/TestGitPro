// PlayBarTimeCtrl.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "PlayBarTimeCtrl.h"
#include ".\playbartimectrl.h"
#include "memdcex.h"



#define MAX_WIDTH 2048
#define MAX_HEIGHT 1536
// CPlayBarTimeCtrl
#define SIZE_BIG  1
#define SIZE_NORMAL8   2
#define SIZE_SMALL    3

#define ONEHOUR       1
#define HALFHOUR      2
#define QUARTERHOUR   3
#define TENMINUTES    4
#define FIVEMINUTES   5
#define ONEMINUTE     6
#define HALFMINUTE    7
#define QUARTERMINUTE 8 
#define TENSECONDS    9
#define ONESECONDS   10

#define CELLLENTYPE_1 1
#define CELLLENTYPE_2 2
#define CELLLENTYPE_3 3
#define CELLLENTYPE_4 4
#define CELLLENTYPE_5 5

#define COLOR_RECORD_BLACK RGB(0xff,0xff,0xff)
#define COLOR_WNDNAMETEXT RGB(32,35,39)
#define COLOR_WNDNAMEBK RGB(30,90,150)
#define COLOR_BORDER RGB(104,104,104)

UINT  g_PlayBarMove(LPVOID pParam)
{
//	TRACE()
	int i = 0;
	CPlayBarTimeCtrl * pCtrl = (CPlayBarTimeCtrl *)pParam;
	while(TRUE)
	{
		for(i = 0; i < 2;i++)
		{
			if(pCtrl->m_BtnbClk[i])
			{
				TRACE("m_BtnbClk  i = %d",i);
				//				CStatic_PlayTimeBar_Control::GetCurLen(i);
				if(i == 0)
					pCtrl->GetCurLen(-60);
				else
					pCtrl->GetCurLen(60);

				break;
			}
		}

		if(WaitForSingleObject(pCtrl->m_hEvent,100) == WAIT_OBJECT_0)
		{
			ResetEvent(pCtrl->m_hEvent);
			break;
		}
	}
	return TRUE;
}

HBITMAP CreateGradientBMP(HDC hDC, COLORREF cl1, COLORREF cl2, int nWidth, int nHeight, int nDir, int nNumColors)
{
	if(nNumColors > 256)
		nNumColors = 256;

	COLORREF		PalVal[256];
	memset(PalVal, 0, sizeof(COLORREF)*256);

	int nIndex;
	BYTE peRed=0,peGreen=0,peBlue=0;

	int r1=GetRValue(cl1);
	int r2=GetRValue(cl2);
	int g1=GetGValue(cl1);
	int g2=GetGValue(cl2);
	int b1=GetBValue(cl1);
	int b2=GetBValue(cl2);

	for (nIndex = 0; nIndex < nNumColors; nIndex++)
	{
		peRed = (BYTE) (r1 + MulDiv((r2-r1),nIndex,nNumColors-1));
		peGreen = (BYTE) (g1 + MulDiv((g2-g1),nIndex,nNumColors-1));
		peBlue = (BYTE) (b1 + MulDiv((b2-b1),nIndex,nNumColors-1));

		PalVal[nIndex]=(peRed << 16) | (peGreen << 8) | (peBlue);
	}

	int x,y,w,h;
	w=nWidth;
	h=nHeight;

	LPDWORD			pGradBits;
	BITMAPINFO		GradBitInfo;

	pGradBits=(DWORD*) malloc(w*h*sizeof(DWORD));
	memset(&GradBitInfo,0,sizeof(BITMAPINFO));

	GradBitInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	GradBitInfo.bmiHeader.biWidth=w;
	GradBitInfo.bmiHeader.biHeight=h;
	GradBitInfo.bmiHeader.biPlanes=1;
	GradBitInfo.bmiHeader.biBitCount=32;
	GradBitInfo.bmiHeader.biCompression=BI_RGB;

	if(nDir==0) 
	{
		for(y=0;y<h;y++) 
		{
			for(x=0;x<w;x++) 
			{
				*(pGradBits+(y*w)+x)=PalVal[MulDiv(nNumColors,y,h)];
			}
		}
	}
	else if(nDir==1) 
	{
		for(y=0;y<h;y++)
		{
			int l,r;
			l=MulDiv((nNumColors/2),y,h);
			r=l+(nNumColors/2)-1;
			for(x=0;x<w;x++)
			{
				*(pGradBits+(y*w)+x)=PalVal[l+MulDiv((r-l),x,w)];
			}
		}
	}
	else if(nDir==2)
	{
		for(x=0;x<w;x++)
		{
			for(y=0;y<h;y++)
			{
				*(pGradBits+(y*w)+x)=PalVal[MulDiv(nNumColors,x,w)];
			}
		}
	}
	else if(nDir==3)
	{
		for(y=0;y<h;y++)
		{
			int l,r;
			r=MulDiv((nNumColors/2),y,h);
			l=r+(nNumColors/2)-1;
			for(x=0;x<w;x++)
			{
				*(pGradBits+(y*w)+x)=PalVal[l+MulDiv((r-l),x,w)];
			}
		}
	}else if(nDir == 4)
	{
		int j = 0;
		for(j=0;j<h;j++) 
		{
			y = h*2/5+ j;

			if(y >= h)
				y -= h;

			for(x=0;x<w;x++) 
			{
				*(pGradBits+(j*w)+x)=PalVal[MulDiv(nNumColors,y,h)];
			}
		}		
	}


	HBITMAP hBmp = CreateDIBitmap(hDC,&GradBitInfo.bmiHeader,CBM_INIT,
		pGradBits,&GradBitInfo,DIB_RGB_COLORS);

	free(pGradBits);

	return hBmp;
}
IMPLEMENT_DYNAMIC(CPlayBarTimeCtrl, CWnd)
CPlayBarTimeCtrl::CPlayBarTimeCtrl()
: m_iSizeStyle(0)
, m_pList(NULL)
, m_iLineHeit(0)
, m_iCellNormalWid(0)
, m_lCellNum(0)
, m_iDaySpan(1)
, m_iTimeType(ONEHOUR)
, m_iCellLenType(CELLLENTYPE_2)
, m_lRealLen(0)
, m_iCellWid(0)
, m_lCurLen(0)
, m_lSelectPos(0)
, m_iListStart(0)
, m_iElementNum(0)
, m_bTrack(FALSE)
, m_hThread(NULL)
, m_hEvent(NULL)
, m_bBackup(FALSE)//wgf
{
	CTime ctNow = CTime::GetCurrentTime();

	CTime ctStart = CTime(ctNow.GetYear(),ctNow.GetMonth(),ctNow.GetDay(),0,0,0);
	m_Start = ctStart.GetTime();

	CTimeSpan span(1,0,0,0);

	CTime ctEnd = ctStart + span;

	m_End = ctEnd.GetTime();

	int i = 0;

	for(i = 0; i < 6;i++)
	{
		m_BtnbHover[i] = m_BtnbClk[i] = FALSE;
	}

}

CPlayBarTimeCtrl::~CPlayBarTimeCtrl()
{
	SetEvent(m_hEvent);
	Sleep(100);
	m_hThread = NULL;
	if(m_pList)
		delete m_pList;

	m_pList = NULL;

//	ClearDC();
}


BEGIN_MESSAGE_MAP(CPlayBarTimeCtrl, CWnd)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_MESSAGE(EVENT_SELECT, OnEventSelect)

END_MESSAGE_MAP()



// CPlayBarTimeCtrl message handlers

LRESULT CPlayBarTimeCtrl::OnEventSelect(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->m_hWnd,EVENT_SELECT,wParam,lParam);
	return 0;

}

BOOL CPlayBarTimeCtrl::PlayBar_Create(const RECT & rect, CWnd * pParentWnd, UINT nID,int nSizeStyle)
{
	CWnd* pWnd = this;
	m_iSizeStyle = nSizeStyle;

	return pWnd->Create(NULL, _T("PlayBarTimeCtrl"), WS_VISIBLE|WS_CHILD|WS_TABSTOP, rect, pParentWnd, nID);

	return 0;
}

void CPlayBarTimeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ChageSize(cx,cy);

	m_pList->MoveWindow(&m_ListRect);

	m_lCurLen = cx - m_iListStart;

	Invalidate(TRUE);

	// TODO: Add your message handler code here
}

void CPlayBarTimeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlayBarTimeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPlayBarTimeCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class


	CWnd::PreSubclassWindow();
}

void CPlayBarTimeCtrl::InitDC(void)
{
	GetFontSize();

	for(int i = 0; i < 3;i++)
	{
		m_Font[i].CreateFont( TextSize[i].cy,TextSize[i].cx,0,0,FW_BOLD,0,0,0,\
			DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
	}

	m_Pen[0].CreatePen(PS_SOLID,1,COLOR_RECORD_BLACK);
	m_Pen[1].CreatePen(PS_SOLID,1,RGB(255,255,255)/*COLOR_WNDNAMETEXT*/);
	m_Pen[2].CreatePen(PS_SOLID,1,COLOR_WNDNAMEBK);
	m_Pen[3].CreatePen(PS_SOLID,1,COLOR_BORDER);

	m_Brush[0].CreateSolidBrush(COLOR_RECORD_BLACK);
	m_Brush[1].CreateSolidBrush(COLOR_WNDNAMETEXT);
	m_Brush[2].CreateSolidBrush(COLOR_WNDNAMEBK);
	m_Brush[3].CreateSolidBrush(RGB(32,35,39)/*COLOR_BORDER*/);
	m_Brush[4].CreateSolidBrush(COLOR_PLAN_RECORD);
	m_Brush[5].CreateSolidBrush(COLOR_MANUAL_RECORD);
	m_Brush[6].CreateSolidBrush(COLOR_ALARMR_ECORD);

	if(m_hEvent == NULL)
	{
		m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

		ResetEvent(m_hEvent);
	}

	m_hThread = AfxBeginThread(g_PlayBarMove,this);


}

void CPlayBarTimeCtrl::GetFontSize(void)
{
	switch(m_iSizeStyle)
	{
	case SIZE_BIG:
		TextSize[0].cx = 8;
		TextSize[0].cy = 24;

		TextSize[1].cx = 5;
		TextSize[1].cy = 14;

		TextSize[2].cx = 6;
		TextSize[2].cy = 15;

		break;
	case SIZE_NORMAL8:
		TextSize[0].cx = 6;
		TextSize[0].cy = 18;

		TextSize[1].cx = 5;
		TextSize[1].cy = 14;

		TextSize[2].cx = 5;
		TextSize[2].cy = 15;
		break;
	case SIZE_SMALL:
		TextSize[0].cx = 5;
		TextSize[0].cy = 16;

		TextSize[1].cx = 4;
		TextSize[1].cy = 12;

		TextSize[2].cx = 4;
		TextSize[2].cy = 10;		
		break;
	default:
		break;
	}
}

void CPlayBarTimeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	DrawUI(&dc);

	// Do not call CWnd::OnPaint() for painting messages
}

void CPlayBarTimeCtrl::ChageSize(int cx,int cy)
{
	CDC* pDC = GetDC();
	ASSERT(pDC);

	CFont * pOldFont;
	int iExternal = 8;
	int iCheckBoxSize = 16;

	TEXTMETRIC tm;
	int textHeit[3],textWid[3];

	for(int i = 0; i < 3;i++)
	{
		pOldFont = pDC->SelectObject(&m_Font[i]);
		pDC->GetTextMetrics(&tm);

		textWid[i] = tm.tmAveCharWidth;

		textHeit[i] = tm.tmHeight + tm.tmExternalLeading;
	}

	pDC->SelectObject(pOldFont);

	m_ListRect.top = textHeit[0] + textHeit[2] + textHeit[2] / 3;

	m_ListRect.left = 0;

	if (m_bBackup)//wgf
	{
		m_iListStart = textWid[1] * 15 + iCheckBoxSize;
	} 
	else
	{
		m_iListStart = textWid[1] * 12 ;//+ iCheckBoxSize;
	}

	m_iListStart = 8;

	m_ListRect.bottom = cy - textHeit[1] - textHeit[1] / 3;

	m_ListRect.right = cx;

	m_iLineHeit = textHeit[1] + textHeit[1] / 3;


	int iCellNormal = 0;

	iCellNormal = (cx - m_iListStart) / 24 + 1;

	if(iCellNormal <  textWid[2] * 6)
		iCellNormal = textWid[2] * 6;

	m_iCellNormalWid = iCellNormal;

//	m_iCellNormalWid = textWid[2] * 7;


	m_BtnRect[0].top = 5;
	m_BtnRect[0].bottom = textHeit[0] - 5;
	m_BtnRect[0].left = 5;/*m_iListStart + 5;*/
	m_BtnRect[0].right = m_BtnRect[0].left + m_BtnRect[0].Height() * 3/2;

	m_BtnRect[1] = m_BtnRect[0];
	m_BtnRect[1].right = cx - 5;
	m_BtnRect[1].left = m_BtnRect[1].right - m_BtnRect[0].Width();

	m_BtnRect[5].top = m_ListRect.bottom + ((cy - m_ListRect.bottom) - textHeit[1])/2;
	m_BtnRect[5].bottom = m_ListRect.bottom + textHeit[2] + 2;

	m_BtnRect[5].right = cx - 8;
	m_BtnRect[5].left = m_BtnRect[5].right - m_BtnRect[5].Height() * 5;

	m_BtnRect[4] = m_BtnRect[5];

	m_BtnRect[4].OffsetRect(-(m_BtnRect[5].Width() + 5),0);

	m_BtnRect[3] = m_BtnRect[4];

	m_BtnRect[3].OffsetRect(-(m_BtnRect[5].Width() + 5),0);

	m_BtnRect[2] = m_BtnRect[3];

	m_BtnRect[2].OffsetRect(-(m_BtnRect[5].Width() + 5),0);	

	ReleaseDC(pDC);
}

void CPlayBarTimeCtrl::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

int CPlayBarTimeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitDC();

	ChageSize(lpCreateStruct->cx,lpCreateStruct->cy);

	m_pList = new CPlayBarTimeList;

	m_pList->Create(m_ListRect,this,m_iListStart,m_iLineHeit);


	m_lCellNum = GetCellNum();
	m_iCellWid = GetCellLen();
	m_lRealLen = GetRealLen();
	m_lCurLen = lpCreateStruct->cx - m_iListStart;
	m_lSelectPos = 0;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CPlayBarTimeCtrl::ClearDC(void)
{
	int i = 0;


}
//wgf
BOOL CPlayBarTimeCtrl::PlayBar_SetBackupFlag(BOOL bFlag)
{
	m_bBackup=bFlag;
	return bFlag;
}

void CPlayBarTimeCtrl::DrawUI(CDC* pDC)
{
	CRect rect(0,0,0,0);
	CRect rectText(0,0,0,0);
	CString strText = _T("");
	int i = 0;

	CBitmap *pOldBmp = NULL;
	CFont * pOldFont = NULL;
	CPen * pOldPen = NULL;
	CBrush *pOldBrush = NULL;

	CDC bitmapDC;
	HDC hDC = pDC->GetSafeHdc();
	CBitmap  bmp;
	CBitmap bmpBake;
	bitmapDC.CreateCompatibleDC(pDC);

	//设置文本颜色 ljg add 2010-8-3 11:37:23
	bitmapDC.SetTextColor(COLOR_TEXT);

	GetClientRect(rect);
	CMemDCEX memDC(pDC,rect);

	bmpBake.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBmp = bitmapDC.SelectObject(&bmpBake);

	pOldBrush = bitmapDC.SelectObject(&m_Brush[3]);
    //pOldPen = bitmapDC.SelectObject(&m_Pen[3]);  //上下文看这里多余

	bitmapDC.Rectangle(&rect);

	memDC.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&bitmapDC,0,0,SRCCOPY);

	CRect rectTitle(0,0,0,0);
	rectTitle = rect;
	rectTitle.bottom = m_ListRect.top;

	//背景色修改 ljg 2010-8-3 11:18:00
	COLORREF clrTitleBk = UI_PLAYBAR_TITLEBK;
	HBITMAP	bmpBar = CreateGradientBMP(
		hDC,clrTitleBk/*RGB(0xff,0xff,0xff)*/, clrTitleBk/*RGB(30,90,150)*/,
		rectTitle.Width(),rectTitle.Height(),
		1, 256);

	TEXTMETRIC tm;
	bmp.Attach(bmpBar);

	pOldBmp = bitmapDC.SelectObject(&bmp);

	bitmapDC.SetBkMode(TRANSPARENT);

	rectText = rectTitle;

	rectText.left = m_iListStart;

	pOldFont = bitmapDC.SelectObject(&m_Font[0]);
	bitmapDC.GetTextMetrics(&tm);

	rectText.bottom = rectText.top + tm.tmHeight + tm.tmExternalLeading;

	if(m_iDaySpan == 1)
	{
		CTime ctTime(m_Start);
		strText = ctTime.Format(_T("%Y - %m - %d  "));
		CString strWeek = _T("");
		switch(ctTime.GetDayOfWeek())
		{
		case 1:
			strWeek.Format(_T("星期日"));
			break;
		case 2:
			strWeek.Format(_T("星期一"));
			break;
		case 3:
			strWeek.Format(_T("星期二"));
			break;
		case 4:
			strWeek.Format(_T("星期三"));
			break;
		case 5:
			strWeek.Format(_T("星期四"));
			break;
		case 6:
			strWeek.Format(_T("星期五"));
			break;
		case 7:
			strWeek.Format(_T("星期六"));
			break;
		}

		strText += strWeek;
		//bitmapDC.DrawTextEx(strText.GetBuffer(0),strText.GetLength(),&rectText,DT_CENTER | DT_VCENTER, 0);
		bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectText,DT_CENTER | DT_VCENTER);
	}

    if(m_BtnbClk[0] || m_BtnbHover[0])
    {
        pOldPen = bitmapDC.SelectObject(&m_Pen[2]);
    }
    else
    {
        pOldPen = bitmapDC.SelectObject(&m_Pen[1]);
    }
		
	int ix,iy_1,iy_2,iTangleWid;

	ix = m_BtnRect[0].left;
	iTangleWid = m_BtnRect[0].Height() / 2;

	int k = 0;
	while(k < 2)
	{
		iy_1 = m_BtnRect[0].top + iTangleWid;
		iy_2 = iy_1 + 1;

		while(ix < m_BtnRect[0].left + (k + 1) * iTangleWid)
		{
			bitmapDC.MoveTo(ix,iy_1);
			bitmapDC.LineTo(ix,iy_2);

			ix++;
			iy_1--;
			iy_2++;
		}
		k++;
	}

	if(m_BtnbClk[1] || m_BtnbHover[1])
    {
        pOldPen = bitmapDC.SelectObject(&m_Pen[2]);
    }
	else
    {
        pOldPen = bitmapDC.SelectObject(&m_Pen[1]);
    }
	ix = m_BtnRect[1].right;
	iTangleWid = m_BtnRect[1].Height() / 2;

	k = 0;
	while(k < 2)
	{
		iy_1 = m_BtnRect[1].top + iTangleWid;
		iy_2 = iy_1 + 1;

		while(ix > m_BtnRect[1].right - (k + 1) * iTangleWid)
		{
			bitmapDC.MoveTo(ix,iy_1);
			bitmapDC.LineTo(ix,iy_2);

			ix--;
			iy_1--;
			iy_2++;
		}
		k++;
	}

	int iCellNum,iCellLeft;
	long lLeft = 0;
	int xPos = 0;
	CRect rectTemp(0,0,0,0);
	lLeft = m_lCurLen - (rect.Width() - m_iListStart);

	iCellNum = lLeft / m_iCellWid;
	iCellLeft = lLeft % m_iCellWid;

	xPos -= iCellLeft;
	xPos += rectText.left;

	pOldFont = bitmapDC.SelectObject(&m_Font[1]);
	//pOldPen = bitmapDC.SelectObject(&m_Pen[2]);  //上下文看这里如果执行之前和之后的赋值都是扯淡
	bitmapDC.GetTextMetrics(&tm);

	rectText.top = rectText.bottom;
	rectText.bottom = rectTitle.bottom;

	time_t ct;

	while(xPos < rectTitle.right)
	{
		rectText.left = xPos;
		rectText.right = rectText.left + m_iCellWid;

		bitmapDC.MoveTo(rectText.left,rectText.top);
		bitmapDC.LineTo(rectText.left,rectText.bottom);

		if(GetText(iCellNum,ct))
		{
			if(m_iDaySpan == 1)
			{
				CTime cTime(ct);
	
				strText = cTime.Format(_T("%H:%M"));
			
				rectTemp = rectText;
				rectTemp.top = rectTemp.bottom - (tm.tmExternalLeading + tm.tmHeight);
				rectTemp.left += 2;
//				bitmapDC.DrawTextEx(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
//					DT_CENTER | DT_BOTTOM, 0);
				bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
					DT_LEFT | DT_BOTTOM);
			}else
			{
				CTime cTime(ct);
				rectTemp = rectText;
				rectTemp.top  = rectText.bottom -  2 * (tm.tmExternalLeading + tm.tmHeight);
				rectTemp.bottom = rectTemp.top + (tm.tmExternalLeading + tm.tmHeight);
				strText = cTime.Format("%y-%m-%d");
//				bitmapDC.DrawTextEx(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
//					DT_CENTER | DT_BOTTOM, 0);
				bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
					DT_CENTER | DT_BOTTOM);

				rectTemp.top = rectTemp.bottom;
				rectTemp.bottom = rectText.bottom;
				strText = cTime.Format(_T("%H:%M:%S"));
//				bitmapDC.DrawTextEx(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
//					DT_CENTER | DT_BOTTOM, 0);
				bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectTemp,\
					DT_CENTER | DT_BOTTOM);
			}		
		}

		iCellNum++;

		xPos += m_iCellWid;
	}

	memDC.BitBlt(rectTitle.left,rectTitle.top,rectTitle.Width(),rectTitle.Height(),
		&bitmapDC,0,0,SRCCOPY);

	CRect rectRC(0,0,0,0);

	rectRC.top = m_ListRect.bottom + (rect.bottom - m_ListRect.bottom - 16) / 2;
	rectRC.bottom = rectRC.top + 16;
	rectRC.left = 10;
	rectRC.right = 26;
	
	memDC.SelectStockObject(BLACK_PEN);
	pOldBrush = memDC.SelectObject(&m_Brush[4]);

	memDC.Rectangle(&rectRC);

	pOldFont = memDC.SelectObject(&m_Font[2]);
	memDC.SetTextColor(UI_PLAYBAR_RECORDBK);
	memDC.SetBkMode(TRANSPARENT);
	rectRC.OffsetRect(20,0);
	rectRC.right += 70;
	strText.Format(_T("定时录像"));
	memDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectRC,\
		DT_LEFT | DT_BOTTOM);

	//去除 移动侦测  界面提示 ljg 2010-8-3 11:14:52
	//rectRC.OffsetRect(80,0);
	//rectRC.right = rectRC.left + 16;
	//memDC.SelectStockObject(BLACK_PEN);
	//pOldBrush = memDC.SelectObject(&m_Brush[5]);

	//memDC.Rectangle(&rectRC);

	//rectRC.OffsetRect(20,0);
	//rectRC.right += 70;
	//strText.Format("移动侦测");
	//memDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectRC,\
	//	DT_LEFT | DT_BOTTOM);

	rectRC.OffsetRect(80,0);
	rectRC.right = rectRC.left + 16;
	memDC.SelectStockObject(BLACK_PEN);
	pOldBrush = memDC.SelectObject(&m_Brush[6]);

	memDC.Rectangle(&rectRC);

	rectRC.OffsetRect(20,0);
	rectRC.right += 70;
	strText.Format(_T("报警录像"));
	memDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectRC,\
		DT_LEFT | DT_BOTTOM);

	//增加提示信息显示
	CRect rectInfo;
	rectInfo.left = rectRC.right + 10;
	rectInfo.top = rectRC.top;
	rectInfo.bottom = rect.bottom;
	rectInfo.right = rectInfo.left+280;
	if (!m_strShowInfo.IsEmpty())
	{
		memDC.DrawText(m_strShowInfo.GetBuffer(0),m_strShowInfo.GetLength(),&rectInfo,\
			DT_LEFT | DT_BOTTOM);
	}

	for(int j = 2; j < 6;j++)
	{
		if(m_BtnbHover[j] || m_BtnbClk[j])
		{
			pOldPen = memDC.SelectObject(&m_Pen[1]);
			pOldBrush = memDC.SelectObject(&m_Brush[0]);
			memDC.SetTextColor(RGB(0,0,0));
		}else
		{
			pOldPen = memDC.SelectObject(&m_Pen[2]);
			pOldBrush = memDC.SelectObject(&m_Brush[3]);
			memDC.SetTextColor(RGB(255,255,255));
		}

		switch(j)
		{
		case 2:
			strText.Format(_T("时间间隔+"));
			break;
		case 3:
			strText.Format(_T("时间间隔-"));
			break;
		case 4:
			strText.Format(_T("单元格+"));
			break;
		case 5:
			strText.Format(_T("单元格-"));
			break;
		}
		memDC.Rectangle(&m_BtnRect[j]);
		memDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&m_BtnRect[j],\
			DT_CENTER | DT_BOTTOM);
	}

	if(pOldBmp)
		bitmapDC.SelectObject(pOldBmp);

	if(pOldFont)
		bitmapDC.SelectObject(pOldFont);

	if(pOldPen)
		bitmapDC.SelectObject(pOldPen);

	if(pOldBrush)
		bitmapDC.SelectObject(pOldBrush);


	if(bmpBar)
		DeleteObject(bmpBar);
}

BOOL CPlayBarTimeCtrl::PlayBar_InitTimeSpan(WORD wStartYear, BYTE bStartMonth, BYTE bStartDay, WORD wEndYear,BYTE bEndMonth,BYTE bEndDay)
{

	CTime ctStart = CTime(wStartYear,bStartMonth,bStartDay,0,0,0);
	CTime ctEnd = CTime(wEndYear,bEndMonth,bEndDay,0,0,0);

	if(ctStart >= ctEnd)
		return FALSE;

	CTimeSpan ctSpan = ctEnd - ctStart;

	m_iDaySpan = (int)ctSpan.GetDays();

	m_Start = ctStart.GetTime();
	m_End = ctEnd.GetTime();

	m_lCellNum = GetCellNum();

	m_lRealLen = GetRealLen();

	CRect rect(0,0,0,0);
	GetClientRect(&rect);

	m_lCurLen = rect.Width() - m_iListStart;
	m_lSelectPos = 0;

	rect.bottom = m_ListRect.top;

	InvalidateRect(&rect);

	if(m_pList)
		m_pList->InvalidateList();
	return 1;
}

long CPlayBarTimeCtrl::GetCellNum(void)
{
	long lCellNum = 0;
	switch(m_iTimeType)
	{
	case ONEHOUR:
		lCellNum = 24 * m_iDaySpan;
		break;
	case HALFHOUR:
		lCellNum = 48 * m_iDaySpan;
		break;
	case QUARTERHOUR:
		lCellNum = 96 * m_iDaySpan;
		break;
	case TENMINUTES:
		lCellNum = 144 * m_iDaySpan;
		break;
	case FIVEMINUTES:
		lCellNum = 288 * m_iDaySpan;
		break;
	case ONEMINUTE:
		lCellNum = 1440 * m_iDaySpan;
		break;
	case HALFMINUTE:
		lCellNum = 2880 * m_iDaySpan;
		break;
	case QUARTERMINUTE:
		lCellNum = 5760 * m_iDaySpan;
		break;
	case TENSECONDS:
		lCellNum = 8640 * m_iDaySpan;
		break;
	case ONESECONDS:
		lCellNum = 86400 * m_iDaySpan;
		break;
	default:
		break;
	}
	return lCellNum;
}

int CPlayBarTimeCtrl::GetCellLen(void)
{
	int iCellWid = 0;
	switch(m_iCellLenType)
	{
	case CELLLENTYPE_1:
		iCellWid = m_iCellNormalWid / 2;
		break;
	case CELLLENTYPE_2:
		iCellWid = m_iCellNormalWid;
		break;
	case CELLLENTYPE_3:
		iCellWid = m_iCellNormalWid * 3/2;
		break;
	case CELLLENTYPE_4:
		iCellWid = m_iCellNormalWid * 2;
		break;
	case CELLLENTYPE_5:
		iCellWid = m_iCellNormalWid * 3;
		break;
	default:
		break;
	}
	return iCellWid;
}

long CPlayBarTimeCtrl::GetRealLen(void)
{
	long lRealLen = m_iCellWid * m_lCellNum;

	return lRealLen;
}

BOOL CPlayBarTimeCtrl::GetText(long lCellNum, time_t & ctTime)
{
	long lCell = lCellNum;
	int iHour = 0,iMinute = 0,iSecond = 0,iDay = 0;

	switch(m_iTimeType)
	{
	case ONEHOUR:
		iDay = lCell / 24;
		iHour = lCell % 24;
		break;
	case HALFHOUR:
		iDay = lCell / 48;
		iHour   = (lCell % 48) / 2;
		iMinute = (lCell % 2) * 30;
		break;
	case QUARTERHOUR:
		iDay = lCell / 96;
		iHour   = (lCell % 96) / 4;
		iMinute = (lCell % 4) * 15;
		break;
	case TENMINUTES:
		iDay = lCell / 144;
		iHour   = (lCell % 144) / 6;
		iMinute = (lCell % 6) * 10;
		break;
	case FIVEMINUTES:
		iDay = lCell / 288;
		iHour   = (lCell % 288) / 12;
		iMinute = (lCell % 12) * 5;
		break;
	case ONEMINUTE:
		iDay = lCell / 1440;
		iHour   = (lCell % 1440) / 60;
		iMinute = (lCell % 60);
		break;
	case HALFMINUTE:
		iDay = lCell / 2880;
		iHour   = (lCell % 2880) / 120;
		iMinute = (lCell % 120) / 2;
		iSecond = ((lCell % 120) % 2) * 30;
		break;
	case QUARTERMINUTE:
		iDay = lCell / 5760;
		iHour   = (lCell % 5760) / 240;
		iMinute = (lCell % 240) / 4;
		iSecond = ((lCell % 240) % 4) * 15;
		break;
	case TENSECONDS:
		iDay = lCell / 8640;
		iHour   = (lCell % 8640) / 360;
		iMinute = (lCell % 360) / 6;
		iSecond = ((lCell % 360) % 6) * 10;
		break;
	case ONESECONDS:
		iDay = lCell / 86400;
		iHour   = (lCell % 86400) / 3600;
		iMinute = (lCell % 3600) / 60;
		iSecond = ((lCell % 3600) % 60);
		break;
	default:
		break;
	}

	ctTime = m_Start + iDay * 86400 + iHour * 3600 + iMinute * 60 + iSecond;

	if(ctTime > m_End)
		return FALSE;

	return BOOL(TRUE);
}

void CPlayBarTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	for(int i = 0; i < 6;i++)
	{
		if(m_BtnRect[i].PtInRect(point))
		{
			m_BtnbClk[i] = TRUE;
			m_BtnRect[i].OffsetRect(1,1);

			InvalidateRect(&m_BtnRect[i]);
			SetCapture();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CPlayBarTimeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(GetCapture() == this)
	{
		ReleaseCapture();

		for(int i = 0; i < 6;i++)
		{
			if(m_BtnbClk[i])
			{
				m_BtnbClk[i] = FALSE;

				m_BtnRect[i].OffsetRect(-1,-1);
				switch(i)
				{
				case 2:
					if(m_iTimeType < 10)
					{
						SetTimeLenType(m_iTimeType + 1);
					}
					break;
				case 3:
					if(m_iTimeType > 1)
					{
						SetTimeLenType(m_iTimeType - 1);
					}
					break;
				case 4:
					if(m_iCellLenType < 5)
					{
						SetCellLenType(m_iCellLenType + 1);
					}
					break;
				case 5:
					if(m_iCellLenType > 2)
					{
						SetCellLenType(m_iCellLenType - 1);
					}
					break;
				}
			

				InvalidateRect(&m_BtnRect[i]);
			}
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CPlayBarTimeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(!m_bTrack)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 1;
		m_bTrack = _TrackMouseEvent(&tme);
	}	

	int i = 0;

	BOOL bIn = FALSE;


	for(i = 0; i < 6;i++)
	{
		if(m_BtnRect[i].PtInRect(point))
		{
			TRACE("BIn");
			bIn = m_BtnbHover[i] = TRUE;
			InvalidateRect(&m_BtnRect[i]);
		}else
		{
			if(m_BtnbHover[i])
			{
				m_BtnbHover[i] = FALSE;
				InvalidateRect(&m_BtnRect[i]);
			}
		}

	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CPlayBarTimeCtrl::PlayBar_AddElement(int iCameraID)
{
	if(m_pList)
		return m_pList->AddElement(iCameraID);

	return 0;
}

BOOL CPlayBarTimeCtrl::PlayBar_RemoveAllElement(void)
{
	if(m_pList)
	{
		m_pList->RemoveALlElement();
		Invalidate();
	}

	return 0;
}

BOOL CPlayBarTimeCtrl::PlayBar_RemoveElement(int iCameraID)
{
	if(m_pList)
	{

	}
	return 0;
}

BOOL CPlayBarTimeCtrl::PlayBar_SetRecordSlice(PSECTIONINFO pSegInfo,int iCameraID)
{
	if(m_pList)
		m_pList->SetRecordSlice(pSegInfo,iCameraID);

	PSECTIONINFO pSegTemp = NULL;

	while(pSegInfo)
	{
		pSegTemp = pSegInfo->pNext;

		delete pSegInfo;

		pSegInfo = pSegTemp;
	}

	return 0;
}

time_t CPlayBarTimeCtrl::GetStartTime()
{
	return m_Start;
}
time_t CPlayBarTimeCtrl::GetEndTime()
{
	return m_End;
}

long CPlayBarTimeCtrl::GetRealLenForElement(void)
{
	return m_lRealLen;
}

long CPlayBarTimeCtrl::GetCurLenForElement(void)
{
	return  m_lCurLen;
}

BOOL CPlayBarTimeCtrl::SetSelect(BOOL bSelect, int nCameraID)
{
	if(m_pList)
		return m_pList->SetSelect(bSelect,nCameraID);

	return 0;
}

BOOL CPlayBarTimeCtrl::IsSelect(int nCameraID)
{
	if(m_pList)
		return m_pList->IsSelect(nCameraID);

	return 0;
}

int CPlayBarTimeCtrl::GetCellWidForElement(void)
{
	return m_iCellWid;
}

int CPlayBarTimeCtrl::GetWid(void)
{
	CRect rect(0,0,0,0);

	GetClientRect(&rect);

	return rect.Width();
	return 0;
}

long CPlayBarTimeCtrl::GetSelPosForElement(void)
{
	return m_lSelectPos;
}

BOOL CPlayBarTimeCtrl::SetSelPos(long lSelPos)
{
	if(lSelPos < 0 || lSelPos > m_lRealLen)
		return 0;

	m_lSelectPos = lSelPos;

	if(m_pList)
		m_pList->InvalidateList();

	::PostMessage(GetParent()->m_hWnd,EVENT_SETTIME,NULL,NULL);

	return 1;
	return 0;
}

void CPlayBarTimeCtrl::GetCurLen(int iLen)
{
	long lOldCurLen = m_lCurLen;

	lOldCurLen += iLen;

	if(lOldCurLen < m_ListRect.Width() - m_iListStart)
		lOldCurLen = m_ListRect.Width() - m_iListStart;
	else if(lOldCurLen > m_lRealLen)
		lOldCurLen = m_lRealLen;
	

	if(lOldCurLen!= m_lCurLen)
	{
		m_lCurLen = lOldCurLen;

		CRect rect(0,0,0,0);
		rect = m_ListRect;
		rect.bottom = m_ListRect.top;
		rect.top = 0;

		InvalidateRect(&rect);

//		Invalidate();

		if(m_pList)
			m_pList->InvalidateList();
	}
}
void CPlayBarTimeCtrl::OnDestroy()
{
	if(m_hThread)
	{
		SetEvent(m_hEvent);

		WaitForSingleObject(m_hThread->m_hThread,INFINITE);

		m_hThread = NULL;
	}
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}

time_t CPlayBarTimeCtrl::PlayBar_GetSelTime(void)
{

	time_t tNow = m_Start + (m_lSelectPos * (m_End - m_Start)) / m_lRealLen;

	if(tNow >= m_Start && tNow <= m_End)
		return tNow;

	return 0;
}

BOOL CPlayBarTimeCtrl::PlayBar_SetTime(time_t ct)
{
	if(ct < m_Start)
		return FALSE;

	if(ct > m_End)
		return FALSE;

	long lSel = (long)((ct - m_Start) * m_lRealLen / (m_End - m_Start));

	m_lSelectPos = lSel;

	if(m_pList)
		m_pList->InvalidateList();
	return 0;
}

void CPlayBarTimeCtrl::SetCellLenType(int iType)
{
	if(iType < 2 || iType > 5)
		return;
	if(m_iCellLenType == iType)
		return;

	m_iCellLenType = iType;
	time_t tNow = m_Start + (m_lSelectPos * (m_End - m_Start)) / m_lRealLen;

	long lOldReal = m_lRealLen;

	m_iCellWid = GetCellLen();

	m_lRealLen = GetRealLen();

	long lLeft = m_lCurLen - (m_ListRect.Width() - m_iListStart);
	long lSelLeft = m_lSelectPos - lLeft;
	long lSelPos = (long)((tNow - m_Start) * m_lRealLen / (m_End - m_Start));

	if(lSelPos > m_lRealLen)
		lSelPos = m_lRealLen;
	else if(lSelPos < 0)
		lSelPos = 0;

	lLeft = lSelPos - lSelLeft;
	long lCurLen = lLeft + (m_ListRect.Width() - m_iListStart);

	if(lCurLen > m_lRealLen)
		lCurLen = m_lRealLen;
	if(lCurLen < m_ListRect.Width() - m_iListStart)
		lCurLen = m_ListRect.Width() - m_iListStart;

	m_lSelectPos = lSelPos;

	m_lCurLen = lCurLen;

	CRect rect(0,0,0,0);
	rect = m_ListRect;
	rect.bottom = m_ListRect.top;
	rect.top = 0;

	InvalidateRect(&rect);

//		Invalidate();

	if(m_pList)
	{
		m_pList->RebuildList();
		m_pList->InvalidateList();
	}
}

void CPlayBarTimeCtrl::SetTimeLenType(int iType)
{
	if(iType < 1 || iType > 5)
		return;

	if(m_iTimeType == iType)
		return;

	time_t tNow = m_Start + (m_lSelectPos * (m_End - m_Start)) / m_lRealLen;

	m_iTimeType = iType;

	long lOldReal = m_lRealLen;

	m_lCellNum = GetCellNum();

	m_lRealLen = GetRealLen();
	
	long lLeft = m_lCurLen - (m_ListRect.Width() - m_iListStart);
	long lSelLeft = m_lSelectPos - lLeft;
	long lSelPos = (long)((tNow - m_Start) * m_lRealLen / (m_End - m_Start));

	if(lSelPos > m_lRealLen)
		lSelPos = m_lRealLen;
	else if(lSelPos < 0)
		lSelPos = 0;

	lLeft = lSelPos - lSelLeft;
	long lCurLen = lLeft + (m_ListRect.Width() - m_iListStart);

	if(lCurLen > m_lRealLen)
		lCurLen = m_lRealLen;
	if(lCurLen < m_ListRect.Width() - m_iListStart)
		lCurLen = m_ListRect.Width() - m_iListStart;

	m_lSelectPos = lSelPos;

	m_lCurLen = lCurLen;

	CRect rect(0,0,0,0);
	rect = m_ListRect;
	rect.bottom = m_ListRect.top;
	rect.top = 0;

	InvalidateRect(&rect);

//	Invalidate();

	if(m_pList)
	{
		m_pList->RebuildList();
		m_pList->InvalidateList();
	}
}

void CPlayBarTimeCtrl::SetShowMsg(CString strMsg)
{
	if (m_strShowInfo == strMsg)
	{
		return;
	}
	else
	{
		m_strShowInfo = strMsg;
		Invalidate();
	}
}
