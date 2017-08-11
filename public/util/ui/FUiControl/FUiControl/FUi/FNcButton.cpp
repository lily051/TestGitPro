#include "StdAfx.h"
#include "FNcButton.h"
#include "FuiBase.h"

// 系统按钮的ID
//#define SYSBTN_CLOSE 500
//#define SYSBTN_MAX   501
//#define SYSBTN_MIN   502

CFNcButton::CFNcButton(void)
: m_pParent(NULL)
, m_bHover(FALSE)
, m_bPressed(FALSE)
, m_bDisabled(FALSE)
, m_clrText(FUi::White)
, m_nID(0)
, m_pImgBkg(NULL)
, m_nTextAlign(DT_CENTER)
, m_strTextFont(YAHEI_20_N)
, m_bAlwHover(FALSE)
{
}

CFNcButton::~CFNcButton(void)
{
    m_pImgBkg = NULL;
    m_pParent = NULL;
}

void CFNcButton::DrawButton(CDC* pDC)
{
    if (NULL == m_pImgBkg)
    {
        return ;
    }

	RectF rcfWnd((REAL)m_rcRect.left, (REAL)m_rcRect.top, 
		(REAL)m_rcRect.Width(), (REAL)m_rcRect.Height());
	
	// normal, hover, pressed, disabled
	int nOffset = 0;
	if (m_bAlwHover)
	{
		nOffset = 1;
	}
	else
	{
		if (m_bHover)
		{
			nOffset = 1;
		}
		if (m_bPressed)
		{
			nOffset = 2;
		}
		if (m_bDisabled)
		{
			nOffset = 3;
		}
	}
	

	int nBkgWidth = m_pImgBkg->GetWidth();
	int nBkgHeight = m_pImgBkg->GetHeight();

    RenderEngine->DrawImageX(pDC, m_pImgBkg, rcfWnd, 0, (REAL)nBkgHeight * nOffset / 4, 
        (REAL)nBkgWidth, (REAL)nBkgHeight / 4);

	// text
    CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strTextFont));
    RenderEngine->DrawTextX(pDC, m_strText, m_rcRect, m_clrText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | m_nTextAlign);
    pDC->SelectObject(pOld);
}

void CFNcButton::SetObjectName(const CString& strObjectName)
{
	Image* pImg = CResMgr::GetInstance()->GetRes(strObjectName);
	if (pImg)
	{
		m_strObjectName = strObjectName;
		m_pImgBkg = pImg;
        m_rcRect = CRect(0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight() / 4);
	}
}

BOOL CFNcButton::CreateButton(const CString& strText, CRect rcRect, CWnd* pParent, int nID)
{
	if (NULL == pParent)
	{
		return FALSE;
	}
	m_strText = strText;
	m_rcRect = rcRect;
	m_pParent = pParent;
	m_nID = nID;

	return TRUE;
}

void CFNcButton::EnableButton(BOOL bEnable)
{
	m_bDisabled = bEnable;
}

int CFNcButton::GetControlID()
{
	return m_nID;
}

void CFNcButton::SetTextFont(const CString& strFontName)
{
    m_strTextFont = strFontName;
}

void CFNcButton::GetRect(CRect& rcRect)
{
	rcRect = m_rcRect;
}

void CFNcButton::MoveWindow(CRect rcRect)
{
	m_rcRect = rcRect;
}

void CFNcButton::LButtonDown()
{
    if(!m_bDisabled)
    {
        m_bPressed = TRUE;
    }
}

void CFNcButton::LButtonUp()
{
    if (!m_bDisabled)
    {
        if(m_bPressed == TRUE)
        {
            m_bHover = FALSE;
            m_bPressed = FALSE;
            if (m_pParent)
            {
                m_pParent->PostMessage(WM_COMMAND, m_nID);
                m_pParent->PostMessage(WM_NCMOUSEMOVE, m_nID, 0);
            }
        }
    }
}

void CFNcButton::MouseHover()
{
    if(!m_bDisabled)
    {
        m_bHover = TRUE;
    }
	if (m_pParent)
	{
		m_pParent->PostMessage(WM_CTRL_TOOLTIP_EVENT, (WPARAM)m_pParent->GetSafeHwnd(), WM_MOUSEFIRST);
	}
}

void CFNcButton::MouseLeave()
{
    if(!m_bDisabled)
    {
        m_bPressed = FALSE;
        m_bHover = FALSE;
    }
}

void CFNcButton::SetToolTip(const CString& strToolTip)
{
    m_strToolTip = strToolTip;
}

CString CFNcButton::GetToolTip()
{
    return m_strToolTip;
}

void CFNcButton::SetFixedWidth(const int nWidth)
{
    m_rcRect.right = nWidth;
}

void CFNcButton::LockHover(BOOL bLock /*= TRUE*/)
{
	m_bAlwHover = bLock;
//	Invalidate();
}