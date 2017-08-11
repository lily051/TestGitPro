// ButtonST_Skin.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "ButtonST_Skin.h"
#include "memdcex.h"


// CButtonST_Skin

//IMPLEMENT_DYNAMIC(CButtonST_Skin, CButtonST)
CButtonST_Skin::CButtonST_Skin()
{
	m_bMulState = TRUE;
	m_bDown = FALSE;

	m_pBakeImage = NULL;
	m_pCenterImage = NULL;
	m_pBakeImageSel = NULL;
	m_pCenterImageSel = NULL;


	m_pBakeImageLeft = NULL;
	m_pBakeImageMid = NULL;
	m_pBakeImageRight = NULL;

	m_pBakeImageSelLeft = NULL;
	m_pBakeImageSelMid = NULL;
	m_pBakeImageSelRight = NULL;

	m_bSingleMode = TRUE;

	m_nCenterImageLeft = -1;
	m_nCenterImageTop = -1;

	m_bClickEnable = TRUE;
    m_iCount = -1;
}

CButtonST_Skin::~CButtonST_Skin()
{

	FreeImage();
}


BEGIN_MESSAGE_MAP(CButtonST_Skin, CButtonST)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CButtonST_Skin message handlers


void CButtonST_Skin::DrawItem(LPDRAWITEMSTRUCT lpDIS)
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
	//if(m_bIsFocused)
	//	bFocusState = TRUE;

	if(m_bIsPressed)
		bFocusState = TRUE;

	//if(m_bSelected)
	//	bFocusState = TRUE;

	if(m_bMouseOnButton)
	{

		bFocusState = TRUE;
	}

	CRect rcDC = itemRect;
	if(m_iCount > 0)
	{
		rcDC.right = itemRect.right * m_iCount;
	}

	CMemDCEX memDC(pDC,itemRect);

	Graphics graph(memDC.m_hDC);


	if(m_bSingleMode)
	{
		if(m_bMulState && m_pBakeImageSel  == NULL && m_iCount > 0)
		{
			if(m_pBakeImage)
			{
				Rect rcF(0, 0, itemRect.Width(), itemRect.Height());
				int iX = m_pBakeImage->GetWidth() / m_iCount;
				if(bFocusState)
				{
					// »­¶¯Ì¬ÉÁË¸Í¼Æ¬
					graph.DrawImage(m_pBakeImage, rcF, iX , 0, itemRect.Width(),  itemRect.Height(), UnitPixel);

				}
				else
				{
					// »­¶¯Ì¬ÉÁË¸Í¼Æ¬
					graph.DrawImage(m_pBakeImage, rcF,0 , 0,itemRect.Width(),  itemRect.Height(), UnitPixel);

				}
			}

		}else
		{
			//ASSERT(m_pBakeImage);
			if(bFocusState && m_bMulState && m_pBakeImageSel)
			{

				// »­±³¾°Í¼Æ¬
				graph.DrawImage(m_pBakeImageSel, itemRect.left, itemRect.top, itemRect.Width(), itemRect.Height());

			}else if(m_pBakeImage)
			{
				// »­±³¾°Í¼Æ¬
				graph.DrawImage(m_pBakeImage, itemRect.left, itemRect.top, itemRect.Width(), itemRect.Height());
			}
		}

	}else
	{
		//ASSERT(m_pBakeImageLeft && m_pBakeImageMid && m_pBakeImageRight);

		int iLeftWid = 0;
		int iRightWid = 0;
		int iMidWid = 0;

		if(bFocusState && m_bMulState && m_pBakeImageSelLeft && m_pBakeImageSelMid && m_pBakeImageSelRight)
		{
			iLeftWid = m_pBakeImageSelLeft->GetWidth();

			// »­±³¾°Í¼Æ¬
			graph.DrawImage(m_pBakeImageSelLeft, itemRect.left, itemRect.top, iLeftWid, itemRect.Height());

			iRightWid = m_pBakeImageSelRight->GetWidth();


			iMidWid = m_pBakeImageSelMid->GetWidth();

			while(iLeftWid < itemRect.right)
			{

				// »­±³¾°Í¼Æ¬
				graph.DrawImage(m_pBakeImageSelMid, itemRect.left + iLeftWid, itemRect.top, iMidWid, itemRect.Height());
				iLeftWid += iMidWid;
			}

			graph.DrawImage(m_pBakeImageSelRight, itemRect.right - iRightWid, itemRect.top, iRightWid, itemRect.Height());
		}else if(m_pBakeImageLeft && m_pBakeImageMid && m_pBakeImageRight)
		{
			iLeftWid = m_pBakeImageLeft->GetWidth();

			// »­±³¾°Í¼Æ¬
			graph.DrawImage(m_pBakeImageLeft, itemRect.left, itemRect.top, iLeftWid, itemRect.Height());

			iRightWid = m_pBakeImageRight->GetWidth();


			iMidWid = m_pBakeImageMid->GetWidth();

			while(iLeftWid < itemRect.right)
			{

				// »­±³¾°Í¼Æ¬
				graph.DrawImage(m_pBakeImageMid, itemRect.left + iLeftWid, itemRect.top, iMidWid, itemRect.Height());
				iLeftWid += iMidWid;
			}

			graph.DrawImage(m_pBakeImageRight, itemRect.right - iRightWid, itemRect.top, iRightWid, itemRect.Height());
		}
	}


	CString sTitle;
	GetWindowText(sTitle);

	CRect captionRect = lpDIS->rcItem;
	if(m_pCenterImage)
	{
		//DrawTheIcon();

		BYTE		byIndex		= 0;

		Image * pImageIcon = NULL;

		// Select the icon to use

		if(m_bMulState && m_pCenterImageSel)
		{
			if ((m_bIsCheckBox && m_bIsPressed) || (!m_bIsCheckBox && (m_bIsPressed || m_bMouseOnButton)))
				pImageIcon = m_pCenterImageSel;
			else
				pImageIcon = m_pCenterImage;
		}else
		{

			pImageIcon = m_pCenterImage;
		}

		if(pImageIcon)
		{
			CRect	rImage;
			PrepareImageRect(!sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, pImageIcon->GetWidth(), pImageIcon->GetHeight(), &rImage);

			if(m_nCenterImageLeft != -1)
			{
				rImage.left = m_nCenterImageLeft;
			}

			if(m_nCenterImageTop != -1)
			{
				rImage.top = m_nCenterImageTop;
			}

			graph.DrawImage(pImageIcon, rImage.left, rImage.top,  pImageIcon->GetWidth(),pImageIcon->GetHeight());
		}
	}

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{
		LOGFONT lf = {0};
		lf.lfHeight = 85;
		lf.lfWeight = 0;
		::lstrcpy(lf.lfFaceName, _T("Tahoma"));
		CFont font;
		CFont * pOldFont = NULL;
		font.CreatePointFontIndirect(&lf);

		//if(bFocusState)
		//{

		//	memDC.SetTextColor(RGB(136,140,162));
		//}else
		//{

		//	memDC.SetTextColor(RGB(101,101,101));
		//}

		pOldFont = memDC.SelectObject(&font);

		DrawTheText(&memDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);

		font.DeleteObject();

		if(pOldFont)
		{
			memDC.SelectObject(pOldFont);
			pOldFont = NULL;
		}
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
			memDC.DrawFocusRect(&focusRect);
		} // if
	} // if
//
//	//DrawBackGround(pDC, &itemRect,bFocusState);
//
//	CString sTitle;
//	GetWindowText(sTitle);
//
//	CRect captionRect = lpDIS->rcItem;
//
//	// Draw the icon
//	if (m_csIcons[0].hIcon)
//	{
//		DrawTheIcon(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
//	} // if
//
//	if (m_csBitmaps[0].hBitmap)
//	{
//		pDC->SetBkColor(RGB(255,255,255));
//		//DrawTheBitmap(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
//		DrawTheBitmap(pDC, FALSE, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
//	} // if
//
//	// Write the button title (if any)
//	if (sTitle.IsEmpty() == FALSE)
//	{
//
//		if(bFocusState)
//		{
//
//			pDC->SetTextColor(RGB(136,140,162));
//		}else
//		{
//
//			pDC->SetTextColor(RGB(101,101,101));
//		}
//		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
//	} // if
//	/*	if (sTitle.IsEmpty() == FALSE)
//	{
//	DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
//	} // if
//	*/
//	if (m_bIsFlat == FALSE || (m_bIsFlat && m_bDrawFlatFocus))
//	{
//		// Draw the focus rect
//		if (m_bIsFocused)
//		{
//			CRect focusRect = itemRect;
//			focusRect.DeflateRect(3, 3);
//			pDC->DrawFocusRect(&focusRect);
//		} // if
//	} // if
	// TODO:  Add your code to draw the specified item
}


void CButtonST_Skin::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	// Draw the button's title
	// If button is pressed then "press" title also
	*rpCaption = *rpItem;
	if (m_bIsPressed && m_bIsCheckBox == FALSE)
		rpCaption->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);

	// Center text
	CRect centerRect = rpCaption;
	centerRect.DeflateRect(2,0,2,0);
	
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS|DT_VCENTER ;

	pDC->SetBkMode(TRANSPARENT);

	CFont *pOldFont = NULL;
	if(m_pFontText != NULL && m_pFontText->m_hObject)
	{
		pOldFont = pDC->SelectObject(m_pFontText);
	}

	if (m_bIsDisabled)
	{
		pDC->SetTextColor(RGB(100,100,100));
		pDC->DrawText(lpszText, centerRect, uFormat);
	}
	else
	{
		if (m_bMouseOnButton || m_bIsPressed)   
		{
			pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_IN]);
			pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
		} // if
		else 
		{
			if (m_bIsFocused)
			{ 
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_FOCUS]); 
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_FOCUS]); 
			} // if
			else 
			{
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_OUT]); 
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]); 
			} // else
		} // else

		pDC->DrawText(lpszText, centerRect, uFormat);
	} // if

	pDC->SelectObject(pOldFont);
}

void CButtonST_Skin::DrawBackGround(CDC * pDC, LPRECT pRect,BOOL bFocus)
{

	CRect rect = *pRect;

	//	rect.DeflateRect(0,0,1,1);
	if(bFocus && m_bMulState)
	{
		
	}else
	{
		
	}
}

void CButtonST_Skin::LoadbakeImage( Image * pBakeImage,Image * pCenterImage,Image * pBakeImageSel ,Image * pCenterImageSel)
{
	FreeImage();

	m_pBakeImage = pBakeImage;
	m_pBakeImageSel = pBakeImageSel;
	m_pCenterImage = pCenterImage;
	m_pCenterImageSel = pCenterImageSel;

	if(pBakeImageSel || pCenterImageSel)
	{

		m_bMulState = TRUE;
	}else
	{

		m_bMulState = FALSE;
	}

	m_bSingleMode = TRUE;

	if(GetSafeHwnd() &&  IsWindowVisible())
	{

		Invalidate();
	}
}

void CButtonST_Skin::SetBakeImage(Image * pBakeImage)
{
	if(m_pBakeImage)
	{
		delete m_pBakeImage;
		m_pBakeImage = NULL;
	}

	m_pBakeImage = pBakeImage;

	Invalidate();
}

void CButtonST_Skin::LoadbakeImage_WholeBake( Image * pBakeImage,Image * pCenterImage,int iCount)
{
	FreeImage();

	m_pBakeImage = pBakeImage;
	m_pCenterImage = pCenterImage;


	if(iCount > 0)
	{

		m_bMulState = TRUE;
	}
	else
	{

		m_bMulState = FALSE;
	}

	m_iCount = iCount;

	m_bSingleMode = TRUE;

	if(GetSafeHwnd() &&  IsWindowVisible())
	{

		Invalidate();
	}
}

void CButtonST_Skin::LoadbakeImageEx( Image * pBakeImageLeft,Image * pBakeImageMid,Image * pBakeImageRight,Image * pCenterImage,\
									 Image * pBakeImageSelLeft ,Image * pBakeImageSelMid,Image * pBakeImageSelRight,Image * pCenterImageSel)
{
	FreeImage();

	m_pBakeImageLeft = pBakeImageLeft;
	m_pBakeImageRight = pBakeImageRight;
	m_pBakeImageMid = pBakeImageMid;

	m_pBakeImageSelLeft = pBakeImageSelLeft;
	m_pBakeImageSelMid = pBakeImageSelMid;
	m_pBakeImageSelRight = pBakeImageSelRight;

	m_pCenterImage = pCenterImage;
	m_pCenterImageSel = pCenterImageSel;


	if(pBakeImageSelLeft)
	{

		m_bMulState = TRUE;
	}else
	{

		m_bMulState = FALSE;
	}

	m_bSingleMode = FALSE;

	if(GetSafeHwnd() && IsWindowVisible())
	{

		Invalidate();
	}
}

void CButtonST_Skin::FreeImage(void)
{
	if(m_pBakeImage)
	{

		delete m_pBakeImage;
		m_pBakeImage = NULL;
	}

	if(m_pBakeImageSel)
	{

		delete m_pBakeImageSel;
		m_pBakeImageSel = NULL;
	}

	if(m_pCenterImage)
	{

		delete m_pCenterImage;
		m_pCenterImage = NULL;
	}

	if(m_pCenterImageSel)
	{

		delete m_pCenterImageSel;
		m_pCenterImageSel = NULL;
	}

	if(m_pBakeImageSelLeft)
	{

		delete m_pBakeImageSelLeft;
		m_pBakeImageSelLeft = NULL;
	}

	if(m_pBakeImageSelMid)
	{

		delete m_pBakeImageSelMid;
		m_pBakeImageSelMid = NULL;
	}

	if(m_pBakeImageSelRight)
	{

		delete m_pBakeImageSelRight;
		m_pBakeImageSelRight = NULL;
	}

	if(m_pBakeImageLeft)
	{

		delete m_pBakeImageLeft;
		m_pBakeImageLeft = NULL;
	}

	if(m_pBakeImageMid)
	{

		delete m_pBakeImageMid;
		m_pBakeImageMid = NULL;
	}

	if(m_pBakeImageRight)
	{

		delete m_pBakeImageRight;
		m_pBakeImageRight = NULL;
	}

	m_iCount = 0;
	m_bSingleMode = FALSE;
	m_bMulState = FALSE;
}

void CButtonST_Skin::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class


	CButtonST::PreSubclassWindow();
}

void CButtonST_Skin::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bClickEnable)
	{
		return;
	}

	CButtonST::OnLButtonDown(nFlags, point);
}

void CButtonST_Skin::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CButtonST::OnLButtonUp(nFlags, point);
}


void CButtonST_Skin::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::PostMessage(GetParent()->GetSafeHwnd(),WM_RBTNDOWNPRESETBTN,(WPARAM)GetDlgCtrlID(),NULL);

	CButtonST::OnRButtonDown(nFlags, point);
}


void CButtonST_Skin::SetCenterImagePos(int nLeft,int nTop)
{
	m_nCenterImageLeft = nLeft;
	m_nCenterImageTop = nTop;

	if(GetSafeHwnd() &&  IsWindowVisible())
	{
		Invalidate();
	}
}


void CButtonST_Skin::OnDestroy()
{
	FreeImage();
}
