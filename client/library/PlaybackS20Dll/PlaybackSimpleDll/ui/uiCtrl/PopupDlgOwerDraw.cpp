// PopupDlgOwerDraw.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "../../resource.h"
#include "PopupDlgOwerDraw.h"
#include "memdcex.h"

// CPopupDlgOwerDraw dialog

IMPLEMENT_DYNAMIC(CPopupDlgOwerDraw, CDialog)
//CPopupDlgOwerDraw::CPopupDlgOwerDraw(CWnd* pParent /*=NULL*/)
//	: CDialog(CPopupDlgOwerDraw::IDD, pParent)
//{
//}
CPopupDlgOwerDraw::CPopupDlgOwerDraw(UINT nIDTemplate, CWnd* pParentWnd) : 
CDialog(nIDTemplate,pParentWnd)
{
	m_hTopLeft = NULL;
	m_hTopMid = NULL;
	m_hTopRight = NULL;
	m_hMidLeft = NULL;	
	m_hMidRight = NULL;
	m_hBottomLeft = NULL;	
	m_hBottomMid = NULL;
	m_hBottomRight = NULL;	

	m_bSkinMode = FALSE;

	m_bakeBrush.m_hObject = NULL;

	//m_bakeBrush.CreateSolidBrush(RGB(207,207,207));
	m_bakeBrush.CreateSolidBrush(RGB(236, 241, 235));

}

CPopupDlgOwerDraw::~CPopupDlgOwerDraw()
{
	// 释放图片 [10/28/2009 GuoLinlin]
	FreeImage();

	// 释放画刷 [10/28/2009 GuoLinlin]
	FreeBrush();
}

void CPopupDlgOwerDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopupDlgOwerDraw, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPopupDlgOwerDraw message handlers


void CPopupDlgOwerDraw::_DrawDialog(CDC * pDC)
{
	CRect clientRC;
	GetClientRect(&clientRC);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	CMemDCEX memDC(pDC,clientRC);

	BITMAP btLeft,btMid,btRight;

	HBITMAP pOldBmp = NULL;
	//绘制标题栏
	//与设备对讲使用相同的标题栏
	////////////////////////////////////////////////////////////////////////////////////
	CBitmap titleLeftBmp, titleMidBmp, titleRightBmp;
	int iTitleHeight;

	titleLeftBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_TITLE_LEFT));
	titleLeftBmp.GetBitmap(&btLeft);
	iTitleHeight = btLeft.bmHeight;

	titleMidBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_TITLE_MIDDLE));
	titleMidBmp.GetBitmap(&btMid);

	titleRightBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_TITLE_RIGHT));
	titleRightBmp.GetBitmap(&btRight);

	CRect titleLeftRC(clientRC);
	titleLeftRC.right = btLeft.bmWidth;
	titleLeftRC.bottom = btLeft.bmHeight;

	CRect titleRightRC(clientRC);
	titleRightRC.left = clientRC.right- btRight.bmWidth;
	titleRightRC.bottom = btRight.bmHeight;

	CRect titleMidRC(clientRC);
	titleMidRC.left = titleLeftRC.right;
	titleMidRC.right = titleRightRC.left;
	titleMidRC.bottom = btMid.bmHeight;

	//绘制左边图片
	pOldBmp = (HBITMAP)dcCompatible.SelectObject(titleLeftBmp);
	memDC.StretchBlt(titleLeftRC.left,titleLeftRC.top,titleLeftRC.Width(),titleLeftRC.Height(),&dcCompatible,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);

	//绘制中间图片
	dcCompatible.SelectObject(titleMidBmp);
	memDC.StretchBlt(titleMidRC.left,titleMidRC.top,titleMidRC.Width(),titleMidRC.Height(),&dcCompatible,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

	//绘制右边图片
	dcCompatible.SelectObject(titleRightBmp);
	memDC.StretchBlt(titleRightRC.left,titleRightRC.top,titleRightRC.Width(),titleRightRC.Height(),&dcCompatible,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

	DeleteObject(titleLeftBmp.Detach());
	DeleteObject(titleMidBmp.Detach());
	DeleteObject(titleRightBmp.Detach());


	if(pOldBmp)
	{

		dcCompatible.SelectObject(pOldBmp);
		pOldBmp = NULL;
	}

	CRect rectTitleText = titleLeftRC;
	rectTitleText.right = clientRC.right;
	rectTitleText.left += 10;


	//绘制标题栏中的标题
	CString szCaption;
	GetWindowText(szCaption);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(255,255,255));

	CFont font;
	LOGFONT lf = {0};
	lf.lfHeight = 92;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Tahoma"));
	font.CreatePointFontIndirect(&lf);	

	CFont *pOldFont = memDC.SelectObject(&font);

	memDC.DrawText(szCaption,&rectTitleText,DT_SINGLELINE |  DT_VCENTER |DT_LEFT/*|DT_CENTER */);
	font.DeleteObject();
	memDC.SelectObject(pOldFont);
	pOldFont = NULL;

	//绘制背景图片
	/////////////////////////////////////////////////////

	CBitmap leftBmp,midBmp,rightBmp;

	leftBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_BKLEFT));
	leftBmp.GetBitmap(&btLeft);

	midBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_BKMID));
	midBmp.GetBitmap(&btMid);

	rightBmp.Attach(RESOURCELOAD_BMP(IDB_BMP_CUSTOMPOPUPDLG_BKRIGHT));
	rightBmp.GetBitmap(&btRight);


	CRect leftRC(clientRC);                 //左边区域
	leftRC.right = btLeft.bmWidth;
	leftRC.top = clientRC.top + iTitleHeight;

	CRect rightRC(&clientRC);              //右边区域
	rightRC.left = clientRC.right - btRight.bmWidth;
	rightRC.top = leftRC.top;


	CRect midRC(&clientRC);                 //中间区域
	midRC.left = leftRC.right;
	midRC.right = rightRC.left;
	midRC.top = leftRC.top;


	//绘制左边图片
	pOldBmp = (HBITMAP)dcCompatible.SelectObject(leftBmp);
	memDC.StretchBlt(leftRC.left,leftRC.top,leftRC.Width(),leftRC.Height(),&dcCompatible,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);

	//绘制中间图片
	dcCompatible.SelectObject(midBmp);
	memDC.StretchBlt(midRC.left,midRC.top,midRC.Width(),midRC.Height(),&dcCompatible,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

	//绘制右边图片
	dcCompatible.SelectObject(rightBmp);
	memDC.StretchBlt(rightRC.left,rightRC.top,rightRC.Width(),rightRC.Height(),&dcCompatible,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);

	DeleteObject(leftBmp.Detach());
	DeleteObject(midBmp.Detach());
	DeleteObject(rightBmp.Detach());

	if(pOldBmp)
	{

		dcCompatible.SelectObject(pOldBmp);
		pOldBmp = NULL;
	}

	_DrawCustom(&memDC);
	
	////横条中间
}

BOOL CPopupDlgOwerDraw::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bSkinMode)
	{

		_DrawDialog_Skin(pDC);
	}else
	{

		_DrawDialog(pDC);
	}
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CPopupDlgOwerDraw::_DrawCustom(CDC * pDC)
{
}

void CPopupDlgOwerDraw::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_SYSCOMMAND,0xF012,0);//可拖动	

	CDialog::OnLButtonDown(nFlags, point);
}


void CPopupDlgOwerDraw::MaxDlg()
{
	CRect wndRC;
	GetClientRect(&wndRC);
	CRgn wndRgn;
	wndRgn.CreateRoundRectRgn(wndRC.left,wndRC.top,wndRC.right,wndRC.bottom,10,10);
	SetWindowRgn(wndRgn,TRUE);
}

void CPopupDlgOwerDraw::MinDlg()
{
	CRect wndRC;
	GetClientRect(&wndRC);
	CRect minRect(wndRC.left,wndRC.top,wndRC.right,wndRC.top + 35);

	CRgn wndRgn;
	wndRgn.CreateRoundRectRgn(minRect.left,minRect.top,minRect.right,minRect.bottom,10,10);
	SetWindowRgn(wndRgn,TRUE);
}

void CPopupDlgOwerDraw::SetSkinImage(HBITMAP hTopLeft,HBITMAP hTopMid,HBITMAP hTopRight,HBITMAP hMidLeft,\
				  HBITMAP hMidRight,HBITMAP hBottomLeft,HBITMAP hBottomMid,HBITMAP hBottomRight)
{
	 // 释放图片 [10/28/2009 GuoLinlin]
	 FreeImage();

	 m_bSkinMode = TRUE;

	 m_hTopLeft = hTopLeft;
	 m_hTopMid = hTopMid;
	 m_hTopRight = hTopRight;
	 m_hMidLeft = hMidLeft;
	 m_hMidRight = hMidRight;
	 m_hBottomLeft = hBottomLeft;
	 m_hBottomMid = hBottomMid;
	 m_hBottomRight = hBottomRight;
}


/*******************************************************************************
*    Function:      FreeImage
*    Description:   释放图片
*    Input:         (NULL) 
*    Output:        (NULL)
*    Return:	    return void
*******************************************************************************/
void CPopupDlgOwerDraw::FreeImage(void)
{
	if(m_hTopLeft)
	{
		DeleteObject(m_hTopLeft);
		m_hTopLeft = NULL;
	}

	if(m_hTopMid)
	{
		DeleteObject(m_hTopMid);
		m_hTopMid = NULL;
	}

	if(m_hTopRight)
	{
		DeleteObject(m_hTopRight);
		m_hTopRight = NULL;
	}

	if(m_hMidLeft)
	{
		DeleteObject(m_hMidLeft);
		m_hMidLeft = NULL;
	}

	if(m_hMidRight)
	{
		DeleteObject(m_hMidRight);
		m_hMidRight = NULL;
	}

	if(m_hBottomLeft)
	{
		DeleteObject(m_hBottomLeft);
		m_hBottomLeft = NULL;
	}

	if(m_hBottomMid)
	{
		DeleteObject(m_hBottomMid);
		m_hBottomMid = NULL;
	}

	if(m_hBottomRight)
	{
		DeleteObject(m_hBottomRight);
		m_hBottomRight = NULL;
	}
}

void CPopupDlgOwerDraw::_DrawDialog_Skin(CDC * pDC)
{
	if(m_bSkinMode == FALSE)
	{

		return;
	}

	ASSERT(m_hTopLeft);
	ASSERT(m_hTopMid);
	ASSERT(m_hTopRight);
	ASSERT(m_hMidLeft);
	ASSERT(m_hMidRight);
	ASSERT(m_hBottomLeft);
	ASSERT(m_hBottomMid);
	ASSERT(m_hBottomRight);

	CRect clientRC;
	GetClientRect(&clientRC);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	CMemDCEX memDC(pDC,clientRC);

	memDC.FillRect(clientRC,&m_bakeBrush);

	BITMAP btLeft,btMid,btRight;

	HBITMAP pOldBmp = NULL;
	//绘制标题栏
	//与设备对讲使用相同的标题栏
	////////////////////////////////////////////////////////////////////////////////////
	CBitmap titleLeftBmp, titleMidBmp, titleRightBmp;
	int iTitleHeight;

	titleLeftBmp.Attach(m_hTopLeft);
	titleLeftBmp.GetBitmap(&btLeft);
	iTitleHeight = btLeft.bmHeight;

	titleMidBmp.Attach(m_hTopMid);
	titleMidBmp.GetBitmap(&btMid);

	titleRightBmp.Attach(m_hTopRight);
	titleRightBmp.GetBitmap(&btRight);

	CRect titleLeftRC(clientRC);
	titleLeftRC.right = btLeft.bmWidth;
	titleLeftRC.bottom = btLeft.bmHeight;

	CRect titleRightRC(clientRC);
	titleRightRC.left = clientRC.right- btRight.bmWidth;
	titleRightRC.bottom = btRight.bmHeight;

	CRect titleMidRC(clientRC);
	titleMidRC.left = titleLeftRC.right;
	titleMidRC.right = titleRightRC.left;
	titleMidRC.bottom = btMid.bmHeight;

	//绘制左边图片
	pOldBmp = (HBITMAP)dcCompatible.SelectObject(titleLeftBmp);
	memDC.StretchBlt(titleLeftRC.left,titleLeftRC.top,titleLeftRC.Width(),titleLeftRC.Height(),&dcCompatible,0,0,btLeft.bmWidth,btLeft.bmHeight,SRCCOPY);

	//绘制中间图片
	dcCompatible.SelectObject(titleMidBmp);
	memDC.StretchBlt(titleMidRC.left,titleMidRC.top,titleMidRC.Width(),titleMidRC.Height(),&dcCompatible,0,0,btMid.bmWidth,btMid.bmHeight,SRCCOPY);

	//绘制右边图片
	dcCompatible.SelectObject(titleRightBmp);
	memDC.StretchBlt(titleRightRC.left,titleRightRC.top,titleRightRC.Width(),titleRightRC.Height(),&dcCompatible,0,0,btRight.bmWidth,btRight.bmHeight,SRCCOPY);
	titleLeftBmp.Detach();
	titleMidBmp.Detach();
	titleRightBmp.Detach();


	CBitmap bottomLeftBmp, bottomMidBmp, bottomRightBmp;
	BITMAP btbottomLeft,btbottomMid,btbottomRight;

	bottomLeftBmp.Attach(m_hBottomLeft);
	bottomLeftBmp.GetBitmap(&btbottomLeft);

	bottomMidBmp.Attach(m_hBottomMid);
	bottomMidBmp.GetBitmap(&btbottomMid);

	bottomRightBmp.Attach(m_hBottomRight);
	bottomRightBmp.GetBitmap(&btbottomRight);

	CRect bottomLeftRC(clientRC);
	bottomLeftRC.right = btbottomLeft.bmWidth;
	bottomLeftRC.top = bottomLeftRC.bottom - btbottomLeft.bmHeight;

	CRect bottomRightRC(clientRC);
	bottomRightRC.left = clientRC.right- btbottomRight.bmWidth;
	bottomRightRC.top = bottomRightRC.bottom - btbottomRight.bmHeight;

	CRect bottomMidRC(clientRC);
	bottomMidRC.left = bottomLeftRC.right;
	bottomMidRC.right = bottomRightRC.left;
	bottomMidRC.top = bottomMidRC.bottom - btbottomMid.bmHeight;

	//绘制左边图片
	pOldBmp = (HBITMAP)dcCompatible.SelectObject(bottomLeftBmp);
	memDC.StretchBlt(bottomLeftRC.left,bottomLeftRC.top,bottomLeftRC.Width(),bottomLeftRC.Height(),&dcCompatible,0,0,btbottomLeft.bmWidth,btbottomLeft.bmHeight,SRCCOPY);

	//绘制中间图片
	dcCompatible.SelectObject(bottomMidBmp);
	memDC.StretchBlt(bottomMidRC.left,bottomMidRC.top,bottomMidRC.Width(),bottomMidRC.Height(),&dcCompatible,0,0,btbottomMid.bmWidth,btbottomMid.bmHeight,SRCCOPY);

	//绘制右边图片
	dcCompatible.SelectObject(bottomRightBmp);
	memDC.StretchBlt(bottomRightRC.left,bottomRightRC.top,bottomRightRC.Width(),bottomRightRC.Height(),&dcCompatible,0,0,btbottomRight.bmWidth,btbottomRight.bmHeight,SRCCOPY);

	bottomLeftBmp.Detach();
	bottomMidBmp.Detach();
	bottomRightBmp.Detach();


	CBitmap MidleftBmp;
	CBitmap MidrightBmp;
	BITMAP btMidLeft,btMidRight;

	MidleftBmp.Attach(m_hMidLeft);
	MidleftBmp.GetBitmap(&btMidLeft);
	CRect midLeftRc(clientRC);
	midLeftRc.top = titleLeftRC.bottom;
	midLeftRc.right = btMidLeft.bmWidth;
	midLeftRc.bottom = bottomLeftRC.top;


	MidrightBmp.Attach(m_hMidRight);
	MidrightBmp.GetBitmap(&btMidRight);
	CRect midRightRc(clientRC);

	midRightRc.top = titleRightRC.bottom;
	midRightRc.left = midRightRc.right - btMidRight.bmWidth;
	midRightRc.bottom = bottomRightRC.top;

	//绘制中间图片
	dcCompatible.SelectObject(MidleftBmp);
	memDC.StretchBlt(midLeftRc.left,midLeftRc.top,midLeftRc.Width(),midLeftRc.Height(),&dcCompatible,0,0,btMidLeft.bmWidth,btMidLeft.bmHeight,SRCCOPY);

	//绘制右边图片
	dcCompatible.SelectObject(MidrightBmp);
	memDC.StretchBlt(midRightRc.left,midRightRc.top,midRightRc.Width(),midRightRc.Height(),&dcCompatible,0,0,btMidRight.bmWidth,btMidRight.bmHeight,SRCCOPY);

	MidleftBmp.Detach();
	MidrightBmp.Detach();

	if(pOldBmp)
	{

		dcCompatible.SelectObject(pOldBmp);
		pOldBmp = NULL;
	}

	CRect rectTitleText = titleLeftRC;
	rectTitleText.top = rectTitleText.top - 2;
	rectTitleText.right = clientRC.right - 25;
	rectTitleText.left += 10;
	rectTitleText.bottom = rectTitleText.top + 32;


	//绘制标题栏中的标题
	CString szCaption;
	GetWindowText(szCaption);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(50, 50, 50));

	CFont font;
	LOGFONT lf = {0};
	lf.lfHeight = 92;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Tahoma"));
	font.CreatePointFontIndirect(&lf);	

	CFont *pOldFont = memDC.SelectObject(&font);

	memDC.DrawText(szCaption,&rectTitleText,DT_SINGLELINE |  DT_VCENTER |DT_LEFT | DT_END_ELLIPSIS);  
	font.DeleteObject();
	memDC.SelectObject(pOldFont);
	pOldFont = NULL;

	_DrawCustom(&memDC);
}

/*************************************************
Function:    	SetBakeColor
Description:	设置窗口的背景颜色
Input:			crBakeColor 设置背景颜色
Output:      	
Return:			
*************************************************/
void CPopupDlgOwerDraw::SetBakeColor(COLORREF crBakeColor)
{
	FreeBrush();

	m_bakeBrush.CreateSolidBrush(crBakeColor);
}

/*******************************************************************************
*    Function:      FreeBrush
*    Description:   释放画刷
*    Input:         (NULL) 
*    Output:        (NULL)
*    Return:	    return void
*******************************************************************************/
void CPopupDlgOwerDraw::FreeBrush(void)
{
	if( m_bakeBrush.m_hObject )
	{
		if( DeleteObject(m_bakeBrush.m_hObject) )
		{
			m_bakeBrush.m_hObject = NULL;
		}
	}

}

HBRUSH CPopupDlgOwerDraw::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor != CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_bakeBrush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CPopupDlgOwerDraw::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
			{
				//屏蔽ESC键
				if (VK_ESCAPE == pMsg->wParam)
				{
					return TRUE;
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				if (VK_F4 == pMsg->wParam)
				{
					return TRUE;
				}
			}
			break;
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPopupDlgOwerDraw::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetSkinImage(RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_UPLEFT,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_UPMID,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_UPRIGHT,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_LEFT,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_RIGHT,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_DOWNLEFT,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_DOWNMID,Color(0,0,0,180)),\
		RESOURCELOAD_PNG_EX_HBMP(IDR_PNG_CUSTOMPOPUPDLG_DOWNRIGHT,Color(0,0,0,180)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
