// Button_Paste.cpp : implementation file
//
//lint -library
#include "stdafx.h"

#include "Button_Paste.h"
#include ".\button_paste.h"


// CButton_Paste

//IMPLEMENT_DYNAMIC(CButton_Paste, CButtonST)
CButton_Paste::CButton_Paste()
: m_bSelected(FALSE)
,m_bMulState(FALSE)
{
	m_TextColor     = RGB(136,140,162);
	m_TextColor_sel = RGB(188,188,188);

	m_bFontDefault  = TRUE; // yxn20090716
	m_bClickEnable  = TRUE;
    m_iStyle        = 0;          // yxn20090716
    m_lFontWeight   = 0;
    m_lFontHight    = 0;
}

CButton_Paste::~CButton_Paste()
{ 
	
	if(m_font.m_hObject)
	{

		DeleteObject(m_font.m_hObject);
		m_font.m_hObject = NULL;
	}
}


BEGIN_MESSAGE_MAP(CButton_Paste, CButtonST)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CButton_Paste message handlers


void CButton_Paste::DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/)
{
	CDC*	pDC = CDC::FromHandle(lpDIS->hDC);


	m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);

	// If there is a menu and it's displayed, draw the button as pressed
	if (
#ifdef	BTNST_USE_BCMENU
		m_menuPopup.m_hMenu 
#else
		m_hMenu 
#endif
		&& m_bMenuDisplayed)	m_bIsPressed = TRUE;

	m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	pDC->SetBkMode(TRANSPARENT);
	BOOL bFocusState = FALSE;
	if(m_bIsFocused)
		bFocusState = TRUE;

	if(m_bIsPressed)
		bFocusState = TRUE;

	//if(m_bSelected)
	//	bFocusState = TRUE;

	if(m_bMouseOnButton)
	{

		bFocusState = TRUE;
	}

	DrawBackGround(pDC, &itemRect,bFocusState);
	CString sTitle;
	GetWindowText(sTitle);

	CRect captionRect = lpDIS->rcItem;

	// Draw the icon
	if (m_csIcons[0].hIcon)
	{
		DrawTheIcon(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	if (m_csBitmaps[0].hBitmap)
	{
		pDC->SetBkColor(RGB(255,255,255));
		//DrawTheBitmap(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
		DrawTheBitmap(pDC, FALSE, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{

		if(bFocusState)
		{

			pDC->SetTextColor(RGB(136,140,162));
		}else
		{

			pDC->SetTextColor(RGB(101,101,101));
		}
		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if
/*	if (sTitle.IsEmpty() == FALSE)
	{
		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if
*/
	if (m_bIsFlat == FALSE || (m_bIsFlat && m_bDrawFlatFocus))
	{
		// Draw the focus rect
		if (m_bIsFocused)
		{
			CRect focusRect = itemRect;
			focusRect.DeflateRect(3, 3);
			pDC->DrawFocusRect(&focusRect);
		} // if
	} // if
	// TODO:  Add your code to draw the specified item
}

void CButton_Paste::DrawBackGround(CDC * pDC, LPRECT pRect,BOOL bFocus)
{
	CRect rect = *pRect;

//	rect.DeflateRect(0,0,1,1);
	if(bFocus && m_bMulState)
	{
		m_DrawStrip_Select.DrawFiiiRect_DrawFun(pDC,&rect,0);
	}else
	{
		m_DrawStrip_Normal.DrawFiiiRect_DrawFun(pDC,&rect,0);
	}
}

void CButton_Paste::Loadbakebmp( UINT nMid,UINT nMidSel)
{
	m_DrawStrip_Normal.DrawFillRect_Loadbmp(nMid);

	if(nMidSel > 0)
	{
		m_bMulState = TRUE;

		m_DrawStrip_Select.DrawFillRect_Loadbmp(nMidSel);
	}
}	

void CButton_Paste::Loadbakebmp( HBITMAP hMid,HBITMAP hMidSel)
{
	m_DrawStrip_Normal.DrawFillRect_Loadbmp(hMid);

	if(hMidSel != NULL)
	{
		m_bMulState = TRUE;

		m_DrawStrip_Select.DrawFillRect_Loadbmp(hMidSel);
	}
}


void CButton_Paste::BtnPaset_SetSelect(bool bSelect)
{
	m_bSelected = bSelect;
	if(m_hWnd)
	{
		Invalidate();
	}
}

void CButton_Paste::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	//设置按钮的有效区域
	//CRgn rgn;
	//CRect rc;
	//GetClientRect(&rc);

	////有效区域为一个角半径为5的圆角矩形
	//rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);

	//SetWindowRgn(rgn,TRUE);

	//rgn.DeleteObject();
	CreateTextFont();

	CButtonST::PreSubclassWindow();
}

// 创建字体
BOOL CButton_Paste::CreateTextFont(void)
{
	LOGFONT lf = {0};
	lf.lfHeight = 85;
	lf.lfWeight = 0;
	::lstrcpy(lf.lfFaceName, _T("Tahoma"));
	m_font.CreatePointFontIndirect(&lf);	

	return true;
}

void CButton_Paste::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	// Draw the button's title
	// If button is pressed then "press" title also
	*rpCaption = *rpItem;
	if (m_bIsPressed && m_bIsCheckBox == FALSE)
		rpCaption->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);

	// ONLY FOR DEBUG 
	//CBrush brBtnShadow(RGB(255, 0, 0));
	//pDC->FrameRect(rCaption, &brBtnShadow);

	CFont * pOldFont = NULL;
	CFont font;

	// 修改按钮字体 yxn20090716
	if(m_bFontDefault == FALSE)
	{
		LOGFONT lf;
		::ZeroMemory(&lf,sizeof(lf));
		lf.lfHeight = m_lFontHight;
		lf.lfWeight = m_lFontWeight;

		::lstrcpy(lf.lfFaceName,_T("Tahoma"));

		font.CreatePointFontIndirect(&lf);
		pOldFont = pDC->SelectObject(&font);
	}
	else
	{
		pOldFont = pDC->SelectObject(&m_font);
	}

	// 文字靠右 yxn20090716
	if(m_iStyle == 2)
	{
		CRect centerRect = rpCaption;
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())/2, (centerRect.Height() - rpCaption->Height())/2);

		rpCaption->left += 10;
		rpCaption->right += 20;
	}
	else if(m_iStyle == 1)  // yxn2009106
	{
		rpCaption->left += 8;
		rpCaption->right += 8;
	}
	else
	{
		// Center text
		CRect centerRect = rpCaption;
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())/2, (centerRect.Height() - rpCaption->Height())/2);
	}
	
	//rpCaption->OffsetRect(8, (centerRect.Height() - rpCaption->Height())/2);
	/* RFU
	rpCaption->OffsetRect(0, (centerRect.Height() - rpCaption->Height())/2);
	rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())-4, (centerRect.Height() - rpCaption->Height())/2);
	*/

	pDC->SetBkMode(TRANSPARENT);
	/*
	pDC->DrawState(rCaption->TopLeft(), rCaption->Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
	TRUE, 0, (CBrush*)NULL);
	*/

	//CFont * pFont = GetParent()->GetFont();

	//// 修改按钮字体 yxn20090716
	//if(m_bFontDefault == FALSE)
	//{
	//	LOGFONT lf;
	//	::ZeroMemory(&lf,sizeof(lf));
	//	lf.lfHeight = m_lFontHight;
	//	lf.lfWeight = m_lFontWeight;

	//	::lstrcpy(lf.lfFaceName,_T("Tahoma"));

	//	CFont font;
	//	font.CreatePointFontIndirect(&lf);
	//	pDC->SelectObject(&font);
	//}
	//else
	//{
	//	pDC->SelectObject(pFont);
	//}

	
	if (m_bIsDisabled)
	{
		//rpCaption->OffsetRect(1, 1);
		////pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		//pDC->SetTextColor(RGB(55,55,55));
		//pDC->DrawText(lpszText, -1, rpCaption, DT_SINGLELINE | DT_LEFT);
		//rpCaption->OffsetRect(-1, -1);
		//pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
		pDC->SetTextColor(RGB(55,55,55));
		pDC->DrawText(lpszText, -1, rpCaption, DT_SINGLELINE | DT_LEFT);
	} // if
	else
	{
		if (m_bMouseOnButton || m_bIsPressed) 
		{
			//pDC->SetTextColor(RGB(136,140,162));
			pDC->SetTextColor(m_TextColor);                          //myl 0603设置字体颜色
			pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
		} // if
		else 
		{
			if (m_bIsFocused)
			{ 
				//pDC->SetTextColor(RGB(188,188,188));
				pDC->SetTextColor(m_TextColor_sel);                  //myl 0603设置字体颜色
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_FOCUS]); 
			} // if
			else 
			{
				//pDC->SetTextColor(RGB(188,188,188));
				pDC->SetTextColor(m_TextColor_sel);                  //myl 0603设置字体颜色  
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]); 
			} // else
		} // else
		pDC->DrawText(lpszText, rpCaption, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	} // if

	if(font.m_hObject)
	{

		font.DeleteObject();
	}

	if(pOldFont)
	{

		pDC->SelectObject(pOldFont);
		pOldFont = NULL;
	}
}


void CButton_Paste::SetTextColorEx(COLORREF textColor,COLORREF textColorSel)
{
	m_TextColor = textColor;
	m_TextColor_sel = textColorSel;
}

// 设置按钮字体 yxn20090716 
void CButton_Paste::SetFont(long lfWeight,long lfHeight)
{
	m_bFontDefault = FALSE;
	m_lFontWeight  = lfWeight;
	m_lFontHight   = lfHeight;
}

// 设置按钮字体位置 yxn20090716
// iStyle 0-居中 1-居左 2-偏右
void CButton_Paste::SetCaptionStyle(int iStyle)
{
	m_iStyle = iStyle;
}


void CButton_Paste::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bClickEnable)//[23:12:2009]wangchao TODO:如果不容许点击
	{
		return;
	}

	CButtonST::OnLButtonDown(nFlags, point);
}
