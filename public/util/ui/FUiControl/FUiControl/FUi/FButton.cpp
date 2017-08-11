// FButton.cpp : 实现文件
//

#include "stdafx.h"
#include "FButton.h"
#include "FMemDCEx.h"


// FButton

// 图标左右留空
#define FBUTTON_CHECK_MARGIN 3
#define FBUTTON_ICON_LEFT 24

IMPLEMENT_DYNAMIC(CFButton, CButton)

CFButton::CFButton()
: m_pImgBkg(NULL)
, m_bMouseTrack(FALSE)
, m_bMouseHover(FALSE)
, m_pImgIcon(NULL)
, m_pImgCheckBox(NULL)
, m_hCursor(LoadCursor(NULL, IDC_ARROW))
, m_bgColor(FUi::White)
, m_nTextAlign(DT_LEFT)
, m_nTextMargin(10)
, m_nTextTopMargin(10)
, m_nSubNameMargin(33)
, m_nIconLeftMargin(-1)
, m_nIconTopMargin(-1)
, m_bCheckBox(FALSE)
, m_isInListCtrl(FALSE)
, m_nChecked(0)
, m_nIconIndex(-1)
, m_nIconTotal(-1)
, m_bAlwHover(FALSE)
, m_bUserMargin(FALSE)
{
    m_mapFont[FUi::Normal] = YAHEI_18_N;
    m_mapFont[FUi::Hovered] = YAHEI_18_N;
    m_mapFont[FUi::Pressed] = YAHEI_18_N;
    m_mapFont[FUi::Disabled] = YAHEI_18_N;

    m_mapTextColor[FUi::Normal] = FUi::Black;
    m_mapTextColor[FUi::Hovered] = FUi::Black;
    m_mapTextColor[FUi::Pressed] = FUi::Black;
    m_mapTextColor[FUi::Disabled] = FUi::Black;
}

CFButton::~CFButton()
{
}


BEGIN_MESSAGE_MAP(CFButton, CButton)
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
    ON_WM_SETCURSOR()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CFButton::SetIcon(LPCTSTR szIconName, int nIconTotal)
{
    Image *pImg = CResMgr::GetInstance()->GetRes(szIconName);
    if (NULL == pImg)
    {
        return ;
    }

	m_nIconTotal = nIconTotal;
    m_pImgIcon = pImg;
    m_strIconName = szIconName;
}

// 按状态设置字体
void CFButton::SetTextFont(int nState, const CString& strFontName)
{
    if (nState & FUi::Normal)
    {
        m_mapFont[FUi::Normal] = strFontName;
    }
    if (nState & FUi::Hovered)
    {
        m_mapFont[FUi::Hovered] = strFontName;
    }
    if (nState & FUi::Pressed)
    {
        m_mapFont[FUi::Pressed] = strFontName;
    }
    if (nState & FUi::Disabled)
    {
        m_mapFont[FUi::Disabled] = strFontName;
    }
    Invalidate();
}

// 设置鼠标指针
void CFButton::SetHoverCursor(HCURSOR hCursor)
{
    m_hCursor = hCursor;
    PostMessage(WM_SETCURSOR, NULL, NULL);
}

// 设置字体颜色
void CFButton::SetTextColor(int nState, COLORREF clrText)
{
    if (nState & FUi::Normal)
    {
        m_mapTextColor[FUi::Normal] = clrText;
    }
    if (nState & FUi::Hovered)
    {
        m_mapTextColor[FUi::Hovered] = clrText;
    }
    if (nState & FUi::Pressed)
    {
        m_mapTextColor[FUi::Pressed] = clrText;
    }
    if (nState & FUi::Disabled)
    {
        m_mapTextColor[FUi::Disabled] = clrText;
    }
    Invalidate();
}

// 设置按钮背景色，目前使用双缓冲和GDI+的方式绘图，
// 无法把图片中的透明部分画出来，需要上层设置一个颜色相似的背景色
void CFButton::SetBgColor(COLORREF color)
{
    m_bgColor = color;
    Invalidate();
}

BOOL CFButton::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

BOOL CFButton::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_LBUTTONDBLCLK)
    {
        pMsg->message = WM_LBUTTONDOWN;
    }

    return CButton::PreTranslateMessage(pMsg);
}

void CFButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

void CFButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseTrack)
    {
        TRACKMOUSEEVENT eventTrack;
        eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
        eventTrack.hwndTrack = m_hWnd;
        eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
        eventTrack.dwHoverTime = 1;
        m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
    }

    if (m_ptrCBtn.get())
    {
        if (CornerBtnHit(point))
        {
            m_ptrCBtn->MouseHover();
        }
        else
        {
            m_ptrCBtn->MouseLeave();
        }
        Invalidate();
    }

    CButton::OnMouseMove(nFlags, point);
}

void CFButton::OnMouseHover(UINT nFlags, CPoint point)
{
    // 鼠标移入按钮
    m_bMouseHover = TRUE;

    Invalidate();

    CButton::OnMouseHover(nFlags, point);
}

void CFButton::OnMouseLeave()
{
    // 鼠标离开
    m_bMouseTrack = FALSE;
    m_bMouseHover = FALSE;
    if (m_ptrCBtn.get())
    {
        m_ptrCBtn->MouseLeave();
    }
    Invalidate();

    CButton::OnMouseLeave();
}

#pragma warning(push)
#pragma warning(disable:4244)
void CFButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC);

	CRect rcItem(lpDrawItemStruct->rcItem);
    FUi::CMemDCEX memDC(pDC, rcItem);
	Graphics graphic(memDC->GetSafeHdc());

	UINT uItemState = lpDrawItemStruct->itemState;

    int nCtrlState = FUi::Normal;
	int nStart = 0;
	if (m_bAlwHover)
	{
		nStart = 2;
		nCtrlState = FUi::Pressed;
	}
	else
	{
		if ((uItemState & ODS_SELECTED) /*|| (uItemState & ODS_FOCUS)*/)
		{
			nStart = 2;
			nCtrlState = FUi::Pressed;
		}
		else if (uItemState & ODS_DISABLED)
		{
			nStart = 3;
			nCtrlState = FUi::Disabled;
		}
		else if (m_bMouseHover)
		{
			nStart = 1;
			nCtrlState = FUi::Hovered;
		}
	}

    // bg-color
    RenderEngine->DrawColor(&memDC, rcItem, m_bgColor);

    // image
    if (NULL != m_pImgBkg)
    {
        RectF rcfDst(0, 0, (REAL)rcItem.Width(), (REAL)rcItem.Height());
        RenderEngine->DrawImageX(&memDC, m_pImgBkg, rcfDst, 0, nStart * (REAL)m_pImgBkg->GetHeight() / 4,
            (REAL)m_pImgBkg->GetWidth(), (REAL)m_pImgBkg->GetHeight() / 4);
    }

	// checkbox
	int nTextLeft = m_nTextMargin;                  // 文字左侧边距
    int nTextRight = rcItem.right - m_nTextMargin;  // 文字右侧边距

    if (m_bCheckBox && m_pImgCheckBox)
    {
        int nOffset = 0;
        if (GetCheck()) // 选中
        {
            nOffset = 3;
        }
        else        // 未选中
        {
            nOffset = 0;
        }
        // 鼠标Hover
        if (m_bMouseHover)
        {
            nOffset += 1;
        }
        // disable
        if (uItemState & ODS_DISABLED)
        {
            nOffset += 2;
        }

        // 画图标
        int nIconW = m_pImgCheckBox->GetWidth();
        int nIconH = m_pImgCheckBox->GetHeight() / 6;

        RectF rcfIcon(FBUTTON_CHECK_MARGIN, (rcItem.Height() - nIconH) / 2.0,
            (Gdiplus::REAL)nIconW, (Gdiplus::REAL)nIconH);

        // 如果用在列表中，边距为0
        if (m_isInListCtrl)
        {
            rcfIcon.X = 0;
        }

        RenderEngine->DrawImageX(&memDC, m_pImgCheckBox, rcfIcon, 2, nIconH * nOffset, 
            (REAL)nIconW, (REAL)nIconH);

        nTextLeft = FBUTTON_CHECK_MARGIN + m_pImgCheckBox->GetWidth() + m_nTextMargin;
    }

    
    // 图标和文字间隔10px ：FBUTTON_TEXT_LEFT
    // 根据设置进行LEFT,CENTER,RIGHT布局

    CString strText;
    GetWindowText(strText);

    // 计算文字宽度
    CRect rcTmp(0,0,0,0);
    pDC->SelectObject(RenderEngine->GetFont(m_mapFont[nCtrlState]));
    pDC->DrawText(strText, rcTmp, DT_CALCRECT | DT_SINGLELINE);

    // 总计可用大小
    CRect rcIconText(rcItem);
    rcIconText.left = nTextLeft;
    rcIconText.right = nTextRight;

    CRect rcText(rcIconText);

    int nIconW = 0;
    if (m_pImgIcon)
    {
        nIconW = m_pImgIcon->GetWidth();
    }

	if (nIconW != 0 && rcTmp.Width() != 0)
	{
		nIconW += m_nTextMargin;
	}

    int nLeft = nTextLeft;
    if (m_nTextAlign & DT_CENTER)
    {
        nLeft = rcIconText.left + (rcIconText.Width() - nIconW - rcTmp.Width()) / 2;
    }
    else if (m_nTextAlign & DT_RIGHT)
    {
        nLeft = rcIconText.right - nIconW - rcTmp.Width();
    }
    int nTextAlign = m_nTextAlign & (~DT_CENTER);
    nTextAlign &= (~DT_RIGHT);
    nTextAlign |= DT_LEFT;

    rcText.left = nLeft + nIconW;

    // icon
    if (m_pImgIcon)
    {
		int nIndex = 0;
		if (m_nIconTotal == 4)
		{
			nIndex = nStart;
		}
		if (m_nIconIndex != -1)
		{
			nIndex = m_nIconIndex;
		}
		
		int nHeight = m_pImgIcon->GetHeight() / m_nIconTotal;

		if (!m_bUserMargin)
		{
			m_nIconLeftMargin = nLeft;
		}

        if (-1 == m_nIconTopMargin)
        {
            m_nIconTopMargin = (rcItem.Height() - nHeight) / 2.0;
        }

        RectF rcfIcon(m_nIconLeftMargin, m_nIconTopMargin,
            (Gdiplus::REAL)m_pImgIcon->GetWidth(), (Gdiplus::REAL)nHeight);

        RenderEngine->DrawImageX(&memDC, m_pImgIcon, rcfIcon, 0, nIndex *  nHeight,
            (REAL)m_pImgIcon->GetWidth(),  (REAL)nHeight);
    }

	// text

    // 说明文字
    if (m_strSubName.IsEmpty())
    {
        // 
        UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_END_ELLIPSIS | nTextAlign;
        memDC->SelectObject(RenderEngine->GetFont(m_mapFont[nCtrlState]));
        RenderEngine->DrawTextX(&memDC, strText, rcText, m_mapTextColor[nCtrlState], uFormat);
    }
    else
    {
        rcText.top = m_nTextTopMargin;
        rcText.bottom = m_nTextTopMargin + 14;
        UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_END_ELLIPSIS | nTextAlign;
        memDC->SelectObject(RenderEngine->GetFont(m_mapFont[nCtrlState]));
        RenderEngine->DrawTextX(&memDC, strText, rcText, m_mapTextColor[nCtrlState], uFormat);

        rcText.top = m_nSubNameMargin;
        rcText.bottom = rcItem.bottom;
        uFormat = DT_WORD_ELLIPSIS | DT_WORDBREAK | DT_LEFT;
        memDC->SelectObject(RenderEngine->GetFont(m_mapSubNameFont[nCtrlState]));
        RenderEngine->DrawTextX(&memDC, m_strSubName, rcText, m_mapSubNameColor[nCtrlState], uFormat);
    }

    // 按钮
    if (m_ptrCBtn.get() && m_bMouseHover)
    {
        m_ptrCBtn->DrawButton(&memDC);
    }
}
#pragma warning(pop)

BOOL CFButton::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if (m_ptrCBtn.get())
    {
        if ((int)wParam == m_ptrCBtn->GetControlID())
        {
            GetParent()->PostMessage(WM_COMMAND, wParam, lParam);
        }
    }

    return CButton::OnCommand(wParam, lParam);
}

BOOL CFButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hCursor)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CButton::OnSetCursor(pWnd, nHitTest, message);
}


void CFButton::SetTextAlign(int nAlign)
{
    m_nTextAlign = nAlign;
}

void CFButton::SetTextMargin(int nMargin)
{
    m_nTextMargin = nMargin;
}

void CFButton::SetTextTopMargin(int nTopMargin, int nSubMargin)
{
    m_nTextTopMargin = nTopMargin;
    m_nSubNameMargin = nSubMargin;
}

void CFButton::SetTipText(LPCTSTR szTipText)
{
    _tcscpy_s(m_szTipText, MAX_LEN_TOOLTIP, szTipText);

	::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP, (WPARAM)this, (LPARAM)m_szTipText);
}

void CFButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bCheckBox)
    {
        m_nChecked = !m_nChecked;
        ::PostMessage(GetParent()->GetSafeHwnd(), MSG_CHECKBOX_CLICK, (WPARAM)m_nChecked, 0);
    }

    if (m_ptrCBtn.get())
    {
        if (CornerBtnHit(point))
        {
            m_ptrCBtn->LButtonUp();
        }
    }

    CButton::OnLButtonUp(nFlags, point);
}

void CFButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_ptrCBtn.get())
    {
        if (CornerBtnHit(point))
        {
            m_ptrCBtn->LButtonDown();
			return ;
        }
    }

    CButton::OnLButtonDown(nFlags, point);
}

BOOL CFButton::CornerBtnHit(CPoint pt)
{
    if (m_ptrCBtn.get())
    {
        CRect rcBtn;
        m_ptrCBtn->GetRect(rcBtn);

        if (rcBtn.PtInRect(pt))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void CFButton::SetObjectName(LPCTSTR szObjectName)
{
	Image* pImg = CResMgr::GetInstance()->GetRes(szObjectName);
	if (pImg)
    {
        m_strObjectName = szObjectName;
		m_pImgBkg = pImg;
		SetWindowPos(NULL, 0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight() / 4, SWP_NOMOVE);
	}
}

void CFButton::SetCheckBox(BOOL bSet /*= TRUE*/)
{
    m_bCheckBox = bSet;

    Image *pImg = CResMgr::GetInstance()->GetRes(m_strObjectName + _T("_checkbox"));
    if (pImg)
    {
        m_pImgCheckBox = pImg;
    }
}

void CFButton::SetCheck(int nCheck)
{
    if (m_bCheckBox)
    {
        m_nChecked = nCheck;
        Invalidate();
    }
}

int CFButton::GetCheck()
{
    if (m_bCheckBox)
    {
        return m_nChecked;
    }
    return 0;
}

void CFButton::SetFixedWidth(const int nWidth)
{
    CRect rc;
    GetClientRect(rc);
    SetWindowPos(NULL, 0, 0, nWidth, rc.Height(), SWP_NOMOVE);
    if (m_ptrCBtn.get())
    {
        CRect rcBtn;
        m_ptrCBtn->GetRect(rcBtn);

        CRect rcBtnPos(rcBtn);
        rcBtnPos.left = nWidth - rcBtn.Width();
        rcBtnPos.right = nWidth;
        m_ptrCBtn->MoveWindow(rcBtnPos);
    }
    Invalidate();
}

void CFButton::AddCornerBtn(const int nBtnID, const CString& strCornerBtnName)
{
    if (m_ptrCBtn.get() == NULL)
    {
        // 
        CRect rcBase;
        GetClientRect(rcBase);

        // 
        int nW = 16;
        int nH = 16;
        Image *pImg = CResMgr::GetInstance()->GetRes(strCornerBtnName);
        if (pImg)
        {
            nW = pImg->GetWidth();
            nH = pImg->GetHeight() / 4;
        }
        CRect rcBtn(rcBase.right - nW, 0, rcBase.right, nH);

        CFNcButton *pBtn = new CFNcButton;
        pBtn->CreateButton(_T(""), rcBtn, this, nBtnID);
        pBtn->SetObjectName(strCornerBtnName);
        pBtn->MoveWindow(rcBtn);
        m_ptrCBtn.reset(pBtn);
    }
    else
    {
        m_ptrCBtn->SetObjectName(strCornerBtnName);
    }
}

void CFButton::RemoveCornerBtn()
{
    m_ptrCBtn.reset();
}

void CFButton::SetInListCtrl(BOOL bSet /*= TRUE*/)
{
    m_isInListCtrl = bSet;
}

void CFButton::SetSubName(const CString& strSubName)
{
    m_strSubName = strSubName;
    Invalidate();
}
void CFButton::SetSubNameFont(int nState, const CString& strFontName)
{
    if (nState & FUi::Normal)
    {
        m_mapSubNameFont[FUi::Normal] = strFontName;
    }
    if (nState & FUi::Hovered)
    {
        m_mapSubNameFont[FUi::Hovered] = strFontName;
    }
    if (nState & FUi::Pressed)
    {
        m_mapSubNameFont[FUi::Pressed] = strFontName;
    }
    if (nState & FUi::Disabled)
    {
        m_mapSubNameFont[FUi::Disabled] = strFontName;
    }
    Invalidate();
}
void CFButton::SetSubNameColor(int nState, COLORREF clrText)
{
    if (nState & FUi::Normal)
    {
        m_mapSubNameColor[FUi::Normal] = clrText;
    }
    if (nState & FUi::Hovered)
    {
        m_mapSubNameColor[FUi::Hovered] = clrText;
    }
    if (nState & FUi::Pressed)
    {
        m_mapSubNameColor[FUi::Pressed] = clrText;
    }
    if (nState & FUi::Disabled)
    {
        m_mapSubNameColor[FUi::Disabled] = clrText;
    }
    Invalidate();
}

void CFButton::LockHover(BOOL bLock /*= TRUE*/)
{
	m_bAlwHover = bLock;
	Invalidate();
}

void CFButton::SetIconIndex(int nIndex)
{
	m_nIconIndex = nIndex;
	Invalidate();
}

void CFButton::SetIconMargin(int nLeftMargin, int nTopMargin)
{
	m_bUserMargin = TRUE;
    m_nIconLeftMargin = nLeftMargin;
    m_nIconTopMargin = nTopMargin;
}
LRESULT CFButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message >= WM_MOUSEFIRST &&message <= WM_MOUSELAST ) //VM_MOUSEMOVE
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_CTRL_TOOLTIP_EVENT, (WPARAM)this->GetSafeHwnd(), message);
	}
	return CButton::WindowProc(message, wParam, lParam);
}
