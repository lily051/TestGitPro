// FScrollBar.cpp : 实现文件
//

#include "stdafx.h"
#include "FScrollBar.h"
#include "FMemDCEx.h"

HWND g_hwndScrollBar = NULL;

#define ID_BTN_UP   1000
#define ID_BTN_DOWN 1001

// 规定最小尺寸，防止数据量过大时，滚动条尺寸太小
#define SLIDER_MIN_SIZE 15  // 滚动条最小尺寸

enum _e_mouse_pos
{
    POS_NONE      = 0, // 无
    POS_BTN_UP    = 1, // 向上按钮
    POS_SLIDER    = 2, // 滚动条位置
    POS_BTN_DOWN  = 3, // 向上按钮
    POS_SLIDER_UP   = 4, // 滚动条上方
    POS_SLIDER_DOWN = 5, // 滚动条下方
};

// CFScrollBar

IMPLEMENT_DYNAMIC(CFScrollBar, CScrollBar)

CFScrollBar::CFScrollBar()
: m_pImgUp(NULL)
, m_pImgDown(NULL)
, m_nScrollStyle(SBS_HORZ)
, m_nHover(POS_NONE)
, m_nClick(POS_NONE)
, m_nUseless(POS_NONE)
, m_nScrollWidth(DEFAULT_SCROLL_WIDTH)
, m_nSliderBegin(0)
, m_nSliderEnd(0)
, m_nSliderPos(0)
, m_nSliderHeight(0)
, m_bTrace(FALSE)
, m_nTraceBegin(0)
, m_nTraceMove(0)
, m_nTracePos(0)
, m_bLeaveTrace(FALSE)
, m_nBtnImgWidth(0)
, m_bHoverBtn(FALSE)
{
    memset(&m_siCur, 0, sizeof(SCROLLINFO));
}

CFScrollBar::~CFScrollBar()
{
}


BEGIN_MESSAGE_MAP(CFScrollBar, CScrollBar)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CFScrollBar 消息处理程序

void CFScrollBar::PreSubclassWindow()
{
    DWORD dwStyle = GetStyle();
    SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | WS_CLIPSIBLINGS);

    if (dwStyle & SBS_VERT)
    {
        m_nScrollStyle = SBS_VERT;

        m_pImgUp = CResMgr::GetInstance()->GetRes(_T("scroll_up"));
        m_pImgDown = CResMgr::GetInstance()->GetRes(_T("scroll_down"));
        m_nBtnImgWidth = m_pImgUp->GetWidth();
    }
    else
    {
        m_nScrollStyle = SBS_HORZ;

        m_pImgUp = CResMgr::GetInstance()->GetRes(_T("scroll_left"));
        m_pImgDown = CResMgr::GetInstance()->GetRes(_T("scroll_right"));
        m_nBtnImgWidth = m_pImgUp->GetWidth();
    }

    CRect rcClient;
    GetClientRect(&rcClient);
    SetWindowPos(NULL, rcClient.left, rcClient.top, m_nScrollWidth, rcClient.Height(), SWP_NOMOVE);

    CalSliderPos();

    CScrollBar::PreSubclassWindow();
}


int CFScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CScrollBar::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetWindowLong(GetSafeHwnd(), GWL_STYLE, lpCreateStruct->style | WS_CLIPSIBLINGS);

    if (lpCreateStruct->style & SBS_VERT)
    {
        m_nScrollStyle = SBS_VERT;
        m_pImgUp = CResMgr::GetInstance()->GetRes(_T("scroll_up"));
        m_pImgDown = CResMgr::GetInstance()->GetRes(_T("scroll_down"));

        m_nBtnImgWidth = m_pImgUp->GetWidth();
    }
    else
    {
        m_nScrollStyle = SBS_HORZ;

        m_pImgUp = CResMgr::GetInstance()->GetRes(_T("scroll_left"));
        m_pImgDown = CResMgr::GetInstance()->GetRes(_T("scroll_right"));
        m_nBtnImgWidth = m_pImgUp->GetWidth();
    }

    CRect rcClient;
    GetClientRect(&rcClient);
    SetWindowPos(NULL, rcClient.left, rcClient.top, m_nScrollWidth, rcClient.Height(), SWP_NOMOVE);

    CalSliderPos();

    return 0;
}

void CFScrollBar::OnDestroy()
{
    CScrollBar::OnDestroy();
}

LRESULT CFScrollBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDBLCLK:
        {
            return CScrollBar::WindowProc(WM_LBUTTONDOWN, wParam, lParam);;
        }
    case SBM_SETSCROLLINFO:
        {
            if (m_bTrace)
            {
                return TRUE;
            }

            BOOL bRedraw = wParam;
            LPSCROLLINFO lpScrollInfo=(LPSCROLLINFO)lParam;

            if (lpScrollInfo->fMask & SIF_PAGE)
                m_siCur.nPage = lpScrollInfo->nPage;
            if (lpScrollInfo->fMask & SIF_POS)
                m_siCur.nPos = lpScrollInfo->nPos;

            if (lpScrollInfo->fMask & SIF_RANGE)
            {
                m_siCur.nMin = lpScrollInfo->nMin;
                m_siCur.nMax = lpScrollInfo->nMax;
            }

            if (bRedraw)
            {
                UpdateSlider();
            }
            return TRUE;
        }
    case SBM_GETSCROLLINFO:
        {
            LPSCROLLINFO lpScrollInfo = (LPSCROLLINFO)lParam;
            int nMask = lpScrollInfo->fMask;

            if(nMask & SIF_PAGE)
                lpScrollInfo->nPage = m_siCur.nPage;
            if(nMask & SIF_POS)
                lpScrollInfo->nPos = m_siCur.nPos;
            if(nMask & SIF_TRACKPOS)
                lpScrollInfo->nTrackPos = m_siCur.nTrackPos;
            if(nMask & SIF_RANGE)
            {
                lpScrollInfo->nMin = m_siCur.nMin;
                lpScrollInfo->nMax = m_siCur.nMax;
            }

            return TRUE;
        }
    default:
        break;
    }

    return CScrollBar::WindowProc(message, wParam, lParam);
}


void CFScrollBar::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient;
    GetClientRect(&rcClient);
    FUi::CMemDCEX memDC(&dc, rcClient);

    DrawBg(&memDC);

    DrawBtn(&memDC);

    DrawSlider(&memDC);
}

void CFScrollBar::DrawBg(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    // 背景
    RenderEngine->DrawColor(pDC, rcClient, RGB(242, 242, 242));
}

void CFScrollBar::DrawBtn(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    int nPadding = m_nScrollWidth - m_nBtnImgWidth;
    if (nPadding < 0)
    {
        nPadding = 0;
    }

    // 画上下按钮
    if (SBS_VERT == m_nScrollStyle)
    {
        CRect rcBtnUp(0, 0, m_nScrollWidth, m_nScrollWidth);

        if (POS_BTN_UP == m_nUseless)
        {
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 3 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if (POS_BTN_UP == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcBtnUp, RGB(180, 180, 180));
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 2 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if(POS_BTN_UP == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcBtnUp, RGB(154, 154, 154));
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else
        {
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 0, m_nBtnImgWidth, m_nBtnImgWidth);
        }

        CRect rcBtnDown(0, rcClient.bottom - m_nScrollWidth, m_nScrollWidth, rcClient.bottom);
        if (POS_BTN_DOWN == m_nUseless)
        {
            RenderEngine->DrawImageX(pDC, m_pImgDown, nPadding, rcClient.bottom - (m_nScrollWidth - nPadding),
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 3 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if (POS_BTN_DOWN == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcBtnDown, RGB(180, 180, 180));
            RenderEngine->DrawImageX(pDC, m_pImgDown, nPadding, rcClient.bottom - (m_nScrollWidth - nPadding),
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 2 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if(POS_BTN_DOWN == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcBtnDown, RGB(154, 154, 154));
            RenderEngine->DrawImageX(pDC, m_pImgDown, nPadding, rcClient.bottom - (m_nScrollWidth - nPadding),
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else
        {
            RenderEngine->DrawImageX(pDC, m_pImgDown, nPadding, rcClient.bottom - (m_nScrollWidth - nPadding),
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 0, m_nBtnImgWidth, m_nBtnImgWidth);
        }
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        CRect rcBtnUp(0, 0, m_nScrollWidth, m_nScrollWidth);
        if (POS_BTN_UP == m_nUseless)
        {
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 3 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if (POS_BTN_UP == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcBtnUp, RGB(180, 180, 180));
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 2 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if(POS_BTN_UP == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcBtnUp, RGB(154, 154, 154));
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else
        {
            RenderEngine->DrawImageX(pDC, m_pImgUp, nPadding, nPadding, m_nScrollWidth - 2 * nPadding,
                m_nScrollWidth - 2 * nPadding, 0, 0, m_nBtnImgWidth, m_nBtnImgWidth);
        }

        CRect rcBtnDown(rcClient.right - m_nScrollWidth, 0, rcClient.right, m_nScrollWidth);
        if (POS_BTN_DOWN == m_nUseless)
        {
            RenderEngine->DrawImageX(pDC, m_pImgDown, rcClient.right - (m_nScrollWidth - nPadding), nPadding,
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 3 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if (POS_BTN_DOWN == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcBtnDown, RGB(180, 180, 180));
            RenderEngine->DrawImageX(pDC, m_pImgDown, rcClient.right - (m_nScrollWidth - nPadding), nPadding,
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 2 * m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else if(POS_BTN_DOWN == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcBtnDown, RGB(154, 154, 154));
            RenderEngine->DrawImageX(pDC, m_pImgDown, rcClient.right - (m_nScrollWidth - nPadding), nPadding,
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, m_nBtnImgWidth, m_nBtnImgWidth, m_nBtnImgWidth);
        }
        else
        {
            RenderEngine->DrawImageX(pDC, m_pImgDown, rcClient.right - (m_nScrollWidth - nPadding), nPadding,
                m_nScrollWidth - 2 * nPadding, m_nScrollWidth - 2 * nPadding, 0, 0, m_nBtnImgWidth, m_nBtnImgWidth);
        }
    }
}

void CFScrollBar::DrawSlider(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    if (SBS_VERT == m_nScrollStyle)
    {
        CRect rcDraw(rcClient.left, m_nSliderBegin,
            rcClient.right, m_nSliderEnd);

        if (POS_SLIDER == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(153, 153, 153));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(128, 128, 128));
        }
        else if(POS_SLIDER == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(179, 179, 179));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(153, 153, 153));
        }
        else
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(204, 204, 204));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(179, 179, 179));
        }
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        CRect rcDraw(m_nSliderBegin, rcClient.top,
            m_nSliderEnd, rcClient.bottom);

        if (POS_SLIDER == m_nClick)
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(153, 153, 153));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(128, 128, 128));
        }
        else if(POS_SLIDER == m_nHover)
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(179, 179, 179));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(153, 153, 153));
        }
        else
        {
            RenderEngine->DrawColor(pDC, rcDraw, RGB(204, 204, 204));
            RenderEngine->DrawRect(pDC, rcDraw, 1, RGB(179, 179, 179));
        }
    }
}

void CFScrollBar::OnSize(UINT nType, int cx, int cy)
{
    CScrollBar::OnSize(nType, cx, cy);


}

BOOL CFScrollBar::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CFScrollBar::OnMouseMove(UINT nFlags, CPoint point)
{
    int nHit = HitTest(point);

    // 注册TME_LEAVE消息
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
    mouseEvent.hwndTrack = GetSafeHwnd();
    mouseEvent.dwHoverTime = 1;
    _TrackMouseEvent(&mouseEvent);

    if (!m_bTrace)
    {
        CalSliderPos();
        m_nHover = nHit;

        Invalidate();
    }
    else
    {
        MoveSlider(point);
    }

    CScrollBar::OnMouseMove(nFlags, point);
}

void CFScrollBar::OnMouseHover(UINT nFlags, CPoint point)
{
    if (m_bHoverBtn)
    {
        int nHit = HitTest(point);

        if (POS_BTN_UP == nHit)
        {
            if (m_siCur.nPos > 0)
            {
                GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL, MAKELONG(SB_LINEUP, 0), (LPARAM)m_hWnd);
            }
        }
        else if (POS_BTN_DOWN == nHit)
        {
            if (m_siCur.nPos < (m_siCur.nMax - m_siCur.nPage + 1))
            {
                GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL, MAKELONG(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
            }
        }
    }

    CScrollBar::OnMouseHover(nFlags, point);
}

void CFScrollBar::OnMouseLeave()
{
    m_nHover = POS_NONE;
    if (!m_bTrace)
    {
        m_nClick = POS_NONE;
    }

    CScrollBar::OnMouseLeave();
}

void CFScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    int nHit = HitTest(point);
    m_nClick = nHit;

    switch (nHit)
    {
    case POS_SLIDER:
        {
            m_bTrace = TRUE;
            m_nTraceMove = m_nSliderPos;
            SetCapture();
            if (SBS_VERT == m_nScrollStyle)
            {
                m_nTraceBegin = point.y;
            }
            else if (SBS_HORZ == m_nScrollStyle)
            {
                m_nTraceBegin = point.x;
            }

            break;
        }
    case POS_BTN_UP:
        {
            m_bHoverBtn = TRUE;
            if (m_siCur.nPos > 0)
            {
                GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL, MAKELONG(SB_LINEUP, 0), (LPARAM)m_hWnd);
            }
            break;
        }
    case POS_BTN_DOWN:
        {
            m_bHoverBtn = TRUE;
            if (m_siCur.nPos < (m_siCur.nMax - m_siCur.nPage + 1))
            {
                GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL, MAKELONG(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
            }
            break;
        }
    case POS_SLIDER_UP:
        {
            GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL,
                MAKELONG(SB_PAGEUP, 0), (LPARAM)m_hWnd);
        }
        break;
    case POS_SLIDER_DOWN:
        {
            GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL,
                MAKELONG(SB_PAGEDOWN, 0), (LPARAM)m_hWnd);
        }
        break;
    default:
        break;
    }

    // 屏蔽左键消息，防止原来的滑动条重绘
    //CScrollBar::OnLButtonDown(nFlags, point);
}

void CFScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_nClick = POS_NONE;
    m_bHoverBtn = FALSE;

    int nHit = HitTest(point);
    InvalidateArea(nHit);

    // 重新计算滚动条位置/鼠标悬停位置
    if (m_bTrace)
    {
        nHit = HitTest(point);
        m_nHover = nHit;
        m_bTrace = FALSE;
        ReleaseCapture();

        m_siCur.nPos += m_nTracePos;
        m_nTracePos = 0;

        UpdateSlider();
    }

    CScrollBar::OnLButtonUp(nFlags, point);
}

int CFScrollBar::HitTest(const CPoint& point)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    int nTestPos = -1;
    int nEnd = 0;
    if (SBS_VERT == m_nScrollStyle)
    {
        nTestPos = point.y;
        nEnd = rcClient.bottom;
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        nTestPos = point.x;
        nEnd = rcClient.right;
    }

    if (nTestPos < 0 || nTestPos > nEnd)
    {
        return POS_NONE;
    }

    if (nTestPos < m_nScrollWidth)
    {
        return POS_BTN_UP;
    }
    else if (nTestPos > nEnd - m_nScrollWidth)
    {
        return POS_BTN_DOWN;
    }

    if (nTestPos > m_nScrollWidth && nTestPos < m_nSliderBegin)
    {
        return POS_SLIDER_UP;
    }

    if (m_nSliderBegin < nTestPos && nTestPos < m_nSliderEnd)
    {
        return POS_SLIDER;
    }

    if (nTestPos > m_nSliderEnd && nTestPos < nEnd)
    {
        return POS_SLIDER_DOWN;
    }

    return POS_NONE;
}

void CFScrollBar::InvalidateArea(int nPos)
{
    CRect rcInval;
    GetClientRect(&rcInval);

    switch (nPos)
    {
    case POS_BTN_UP:
        {
            rcInval.bottom = m_nScrollWidth;
            break;
        }
    case POS_BTN_DOWN:
        {
            rcInval.top = rcInval.bottom - m_nScrollWidth;
            break;
        }
    case POS_SLIDER:
    case POS_SLIDER_UP:
    case POS_SLIDER_DOWN:
        {
            if (SBS_VERT == m_nScrollStyle)
            {
                rcInval.top = m_nScrollWidth;
                rcInval.bottom = rcInval.bottom - m_nScrollWidth;
            }
            else if (SBS_HORZ == m_nScrollStyle)
            {
                rcInval.left = m_nScrollWidth;
                rcInval.right = rcInval.right - m_nScrollWidth;
            }

            break;
        }
    default:
        break;
    }

    InvalidateRect(rcInval);
}

void CFScrollBar::CalSliderPos()
{
    SCROLLINFO si;
    GetCurScrollInfo(si);

    if (0 == si.nPage)
    {
        m_nSliderBegin = m_nScrollWidth;
        m_nSliderEnd = m_nScrollWidth + SLIDER_MIN_SIZE + 1;
        return;
    }

    CRect rcClient;
    GetClientRect(&rcClient);

    int nScrollHeight = 0;

    if (SBS_VERT == m_nScrollStyle)
    {
        nScrollHeight = rcClient.Height() - 2 * m_nScrollWidth;
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        nScrollHeight = rcClient.Width() - 2 * m_nScrollWidth;
    }

    if (si.nMax >= si.nPage)
    {
        // 使用最小尺寸时，滚动条位置使用不同的计算模型和方法
        if (SLIDER_MIN_SIZE == m_nSliderHeight)
        {
            float fSliserPos = (float)si.nPos / (si.nMax + 1 - si.nPage) * (nScrollHeight - 2 * SLIDER_MIN_SIZE / 2);

            m_nSliderPos = fSliserPos + (SLIDER_MIN_SIZE + 1) / 2;

            // 保证从位置0到位置1会有位移（显示效果）
            if (0 != si.nPos && (SLIDER_MIN_SIZE + 1) / 2 == m_nSliderPos)
            {
                ++m_nSliderPos;
            }

            m_nSliderBegin = m_nScrollWidth + m_nSliderPos - (SLIDER_MIN_SIZE + 1) / 2;
            m_nSliderEnd = m_nSliderBegin + SLIDER_MIN_SIZE;
        }
        else
        {
            // 根据滚动条信息计算滚动条位置
            float fSliderHeight = (float)si.nPage / (si.nMax + 1) * nScrollHeight;
            float fSliserBegin = (float)si.nPos / (si.nMax + 1) * nScrollHeight;

            m_nSliderBegin = m_nScrollWidth + fSliserBegin;
            m_nSliderEnd = m_nScrollWidth + fSliderHeight + fSliserBegin;

            // 限制滚功条
            //if (m_nSliderEnd > rcClient.bottom - m_nScrollWidth)
            //{
            //    m_nSliderEnd = rcClient.bottom - m_nScrollWidth;
            //}

            if (m_nSliderEnd - m_nSliderBegin < SLIDER_MIN_SIZE)
            {
                m_nSliderEnd = m_nSliderBegin + SLIDER_MIN_SIZE;
            }

            m_nSliderPos = m_nSliderBegin + (m_nSliderEnd - m_nSliderBegin + 1) / 2;
        }
    }

    m_nSliderHeight = m_nSliderEnd - m_nSliderBegin;
}

void CFScrollBar::MoveSlider(const CPoint& point)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    // 计算位移
    int nMove = 0;
    int nEnd = 0;
    if (SBS_VERT == m_nScrollStyle)
    {
        nMove = point.y - m_nTraceBegin;
        nEnd = rcClient.bottom;
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        nMove = point.x - m_nTraceBegin;
        nEnd = rcClient.right;
    }

    m_nTraceBegin += nMove;
    m_nSliderBegin += nMove;
    m_nSliderEnd += nMove;

    if (m_nSliderBegin < m_nScrollWidth)
    {
        int nSliderWidth = m_nSliderEnd - m_nSliderBegin;
        m_nSliderBegin = m_nScrollWidth;
        m_nSliderEnd = m_nSliderBegin + nSliderWidth;

        m_nTraceBegin -= nMove;
    }
    else if (m_nSliderEnd > nEnd - m_nScrollWidth)
    {
        int nSliderWidth = m_nSliderEnd - m_nSliderBegin;
        m_nSliderEnd = nEnd - m_nScrollWidth;
        m_nSliderBegin = m_nSliderEnd - nSliderWidth;

        m_nTraceBegin -= nMove;
    }

    m_nSliderPos = m_nSliderBegin + (m_nSliderEnd - m_nSliderBegin + 1) / 2;
    if (SLIDER_MIN_SIZE == m_nSliderHeight)
    {
        m_nSliderPos -= m_nScrollWidth;
    }

    // 拖动后直接刷新，保证拖动不卡顿
    CDC* pDC = GetDC();
    FUi::CMemDCEX memDC(pDC, rcClient);
    DrawBg(&memDC);
    DrawBtn(&memDC);
    DrawSlider(&memDC);

    OnSlider();
}

void CFScrollBar::OnSlider()
{
    CRect rcClient;
    GetClientRect(&rcClient);
    int nSliderHeight = m_nSliderHeight;
    int nScrollHeight = 0;

    if (SBS_VERT == m_nScrollStyle)
    {
        nScrollHeight = rcClient.Height() - 2 * m_nScrollWidth;
    }
    else if (SBS_HORZ == m_nScrollStyle)
    {
        nScrollHeight = rcClient.Width() - 2 * m_nScrollWidth;
    }

    // 计算滚动条位置移动
    int nPosChange = 0;
    int nMove = m_nSliderPos - m_nTraceMove;

    if (SLIDER_MIN_SIZE == nSliderHeight)
    {
        float lPosChange = ((float)nMove / (nScrollHeight - 2 * SLIDER_MIN_SIZE / 2)) * (m_siCur.nMax - m_siCur.nPage + 1);

        // 进行四舍五入
        if (lPosChange > 0)
        {
            nPosChange = lPosChange + 0.5;
        }
        else if (lPosChange < 0)
        {
            nPosChange = lPosChange - 0.5;
        }

        // 拖动到开头，位置置为0
        if (m_nSliderBegin == m_nScrollWidth)
        {
            nPosChange = -m_siCur.nPos;
        }

        CString strDbg;
        strDbg.Format(_T("nMove = %d, lPosChange = %f\n"), nMove, lPosChange);
        OutputDebugString(strDbg);
    }
    else
    {
        float lPosChange = ((float)nMove / (nScrollHeight - nSliderHeight)) * (m_siCur.nMax - m_siCur.nPage + 1);

        // 进行四舍五入
        if (lPosChange > 0)
        {
            nPosChange = lPosChange + 0.5;
        }
        else if (lPosChange < 0)
        {
            nPosChange = lPosChange - 0.5;
        }
    }

    GetParent()->SendMessage((SBS_VERT == m_nScrollStyle) ? WM_VSCROLL : WM_HSCROLL,
        MAKELONG(SB_THUMBTRACK, m_siCur.nPos + nPosChange), (LPARAM)m_hWnd);

    // 记录位置移动
    m_nTracePos = nPosChange;
}

void CFScrollBar::SetCurScrollInfo(const SCROLLINFO& si)
{
    m_siCur = si;

    UpdateSlider();
}

void CFScrollBar::GetCurScrollInfo(SCROLLINFO& si)
{
    si = m_siCur;
}

void CFScrollBar::SetScrollWidth(const int nWidth)
{
    m_nScrollWidth = nWidth;
}

int CFScrollBar::GetScrollWidth()
{
    return m_nScrollWidth;
}


void CFScrollBar::UpdateSlider()
{
    CalSliderPos();

    if (0 == m_siCur.nPos)
    {
        m_nUseless = POS_BTN_UP;
    }
    else if (m_siCur.nPos + m_siCur.nPage == m_siCur.nMax + 1)
    {
        m_nUseless = POS_BTN_DOWN;
    }
    else
    {
        m_nUseless = POS_NONE;
    }

    Invalidate();
}
