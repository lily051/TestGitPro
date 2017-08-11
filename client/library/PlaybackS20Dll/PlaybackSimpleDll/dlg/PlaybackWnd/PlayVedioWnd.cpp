#include "stdafx.h"
#include "PlayVedioWnd.h"
#include "../../ui/uiCtrl/memdcex.h"
//lint -library
IMPLEMENT_DYNAMIC(CPlayVedioWnd, CWnd)
CPlayVedioWnd::CPlayVedioWnd()
{
	m_clrBk = UI_PLAYWND_COLOR;
	m_clrText = RGB(195, 25, 25);
	m_bShowStatePic = FALSE;
	m_emShowType = PLAYVEDIO_SHOWTYPE_NULL;
	m_nState = 0;
	m_bShowText = FALSE;
	m_nTextCount = 0;
	m_strOnWnd.Empty();
}

CPlayVedioWnd::~CPlayVedioWnd()
{
}


BEGIN_MESSAGE_MAP(CPlayVedioWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

int CPlayVedioWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码、
	m_fTextFont.CreateFont(16, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_MODERN, _T("Arial"));

	return 0;
}

void CPlayVedioWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CWnd::OnPaint()
}

void CPlayVedioWnd::OnRButtonDown(UINT nFlags, CPoint point)
{

	::PostMessage(this->GetParent()->GetSafeHwnd(), WM_RBUTTONDOWN, 0, 0);

	CWnd::OnRButtonDown(nFlags, point);
}


//设置显示文字
void CPlayVedioWnd::SetTextShow(CString strText, BOOL bErase)
{
	if (m_bShowStatePic)
	{
		KillTimer(TIMER_SHOW_PIC);
		m_bShowStatePic = FALSE;
	}

	m_strText = strText;
    if (bErase)
    {
        Invalidate();
    }
	if (!m_bShowText)
	{
		m_bShowText = TRUE;
		m_emShowType = PLAYVEDIO_SHOWTYPE_TEXT;
		SetTimer(TIMER_SHOW_TEXT, 1000, NULL);
	}
}

void CPlayVedioWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_SHOW_TEXT:
		{
			m_nTextCount++;
			if (m_nTextCount >= TIME_SHOW_VALUE)
			{
				KillTimer(TIMER_SHOW_TEXT);
				m_nTextCount = 0;
				m_bShowText = FALSE;
				m_strText = _T("");
				Invalidate(TRUE);
			}
		}
		break;

	case TIMER_SHOW_PIC:
		{
			if (++m_nState >= 8)
			{
				m_nState = 0;
			}

			if (m_nState < 4)
			{
				Invalidate(TRUE);
			}
		}
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CPlayVedioWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CPlayVedioWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	::PostMessage(this->GetParent()->GetSafeHwnd(), WM_LBUTTONDOWN, 0, 0);

	CWnd::OnLButtonDown(nFlags, point);
}

void CPlayVedioWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	::PostMessage(this->GetParent()->GetSafeHwnd(), WM_LBUTTONDBLCLK, 0, 0);

	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CPlayVedioWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (m_emShowType)
	{
	case PLAYVEDIO_SHOWTYPE_NULL:
		{
			DrawCleanRect(pDC);
		}
		break;

	case PLAYVEDIO_SHOWTYPE_PIC:
		{
			DrawStatePic(pDC);
			DrawTextOnVedio(pDC);
		}
		break;

	case PLAYVEDIO_SHOWTYPE_TEXT:
		{
			DrawStateText(pDC);
		}
		break;
	}

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CPlayVedioWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_rcClient);
}

void CPlayVedioWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT       tme;       
	tme.cbSize=sizeof(tme);       
	tme.dwFlags=TME_LEAVE;             
	tme.hwndTrack=m_hWnd;       
	if(!_TrackMouseEvent(&tme))       
	{
		return;
	}

	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MOUSEEVENT_MINITOOLBAR_MOVE, 0, 0);

	CWnd::OnMouseMove(nFlags, point);
}

void CPlayVedioWnd::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MOUSEEVENT_MINITOOLBAR_LEAVE, 0, 0);

	CWnd::OnMouseLeave();
}

/**   @fn          DrawStateText
 *    @brief   	   写状态文字.
 *    @param[in]   pDC:设备上下文指针.
 *    @param[in]   
 *    @return      
 */
void CPlayVedioWnd::DrawStateText(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);

	pDC->SetTextColor(m_clrText);
	pDC->SelectObject(&m_fTextFont);
	CRect rcText(rcClient);
	rcText.top = rcClient.Height()/2 - 20;
	pDC->DrawText(m_strText, rcText, /*DT_SINGLELINE | DT_VCENTER | */DT_CENTER);
}

/**   @fn          DrawStatePic
 *    @brief   	   画状态图.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayVedioWnd::DrawStatePic(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	CMemDCEX memDC(pDC,rcClient);

	CBitmap bmp;
	switch (m_nState)
	{
		// 扬声器图标1.
	case 0:
		{
			bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_AUDIO_ONE, Color(GetRValue(m_clrBk),GetGValue(m_clrBk),GetBValue(m_clrBk))));
		}
		break;

		// 扬声器图标2.
	case 1:
		{
			bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_AUDIO_TWO, Color(GetRValue(m_clrBk),GetGValue(m_clrBk),GetBValue(m_clrBk))));
		}
		break;

		// 扬声器图标3.
	case 2:
		{
			bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_AUDIO_THREE, Color(GetRValue(m_clrBk),GetGValue(m_clrBk),GetBValue(m_clrBk))));
		}
		break;

		// 扬声器图标4.
	case 3:
		{
			bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_AUDIO_FORE, Color(GetRValue(m_clrBk),GetGValue(m_clrBk),GetBValue(m_clrBk))));
		}
		break;

	default:
		{
			bmp.Attach(RESOURCELOAD_PNG_EX_HBMP(IDB_PNG_AUDIO_ONE, Color(GetRValue(m_clrBk),GetGValue(m_clrBk),GetBValue(m_clrBk))));
		}
		break;
	}
	
	CBitmap* pOldBmp = dcCompatible.SelectObject(&bmp);

	int nPicWidth = 96;
	int nPicHeight = 96;
	int nLeft = 0;
	int nTop = 0;
	nLeft = (rcClient.Width() -  nPicWidth) / 2;
	nTop = (rcClient.Height() - nPicHeight) / 2;
	

	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(rcClient, m_clrBk);
	memDC.BitBlt(nLeft, nTop, nPicWidth ,nPicHeight, &dcCompatible, 0, 0, SRCCOPY);

	dcCompatible.SelectObject(pOldBmp);
	pOldBmp = NULL;
	
	dcCompatible.DeleteDC();
}

/**   @fn          DrawCleanRect
 *    @brief   	   清空客户端区域.
 *    @param[in]   pDC:设备上下文指针
 *    @param[in]   
 *    @return      
 */
void CPlayVedioWnd::DrawCleanRect(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrBk);
}

/**   @fn          SetStatePicShow
 *    @brief   	   设置显示状态图标.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */

void CPlayVedioWnd::DrawTextOnVedio( CDC *pDC )
{
	//绘制叠加文字
	if (pDC && !m_strOnWnd.IsEmpty())
	{
		CRect rt(0,0,0,0);
		GetClientRect(&rt);
		rt.left += rt.Width() / 2;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->DrawText(m_strOnWnd, rt, DT_LEFT |DT_EDITCONTROL | DT_WORDBREAK);
	}
}
void CPlayVedioWnd::SetStatePicShow()
{
	if (m_bShowText)
	{
		KillTimer(TIMER_SHOW_TEXT);
		m_bShowText = FALSE;
	}

	if (!m_bShowStatePic)
	{
		SetTimer(TIMER_SHOW_PIC, 300, NULL);
		m_bShowStatePic = TRUE;
	}

	m_emShowType = PLAYVEDIO_SHOWTYPE_PIC;
}

/**   @fn          SetStatePicHide
 *    @brief   	   设置状态图标隐藏.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayVedioWnd::SetStatePicHide()
{
	if (m_bShowStatePic)
	{
		KillTimer(TIMER_SHOW_PIC);
		m_bShowStatePic = FALSE;
	}
	if (m_bShowText)
	{
		KillTimer(TIMER_SHOW_TEXT);
		m_bShowText = FALSE;
	}

	m_emShowType = PLAYVEDIO_SHOWTYPE_NULL;
	Invalidate(TRUE);
}

