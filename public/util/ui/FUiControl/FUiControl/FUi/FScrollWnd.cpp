// FScrollWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "FScrollWnd.h"

#define ID_SCROLL_VERT 1000
#define ID_SCROLL_HORZ 1001
#define ID_SCROLL_LIMIT 1002
#define MSG_DESTMOVE (WM_USER + 100)
#define PADDING_WIDTH 2 // 和窗口的边距


LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CFScrollWnd* pScrollWnd = (CFScrollWnd*)GetWindowLong(hwnd, GWL_USERDATA);
    if (WM_DESTROY == msg)
    {
        WNDPROC procOld = pScrollWnd->m_funOldProc;
        UnskinWndScroll(CWnd::FromHandle(hwnd));

        return ::CallWindowProc(procOld, hwnd, msg, wParam, lParam);
    }

    LRESULT lResult = ::CallWindowProc(pScrollWnd->m_funOldProc, hwnd, msg, wParam, lParam);
    if(pScrollWnd->m_bOpering)
    {
        return lResult;
    }

    switch (msg)
    {
    case WM_ERASEBKGND:
        {
            SCROLLINFO si = {0};
            si.fMask = SIF_ALL;

            DWORD dwStyle=::GetWindowLong(hwnd, GWL_STYLE);
            if (dwStyle & WS_VSCROLL)
            {
                ::GetScrollInfo(hwnd, SB_VERT, &si);

                pScrollWnd->m_sbVert.SetScrollInfo(&si);
            }

            if (dwStyle & WS_HSCROLL)
            {
                ::GetScrollInfo(hwnd, SB_HORZ, &si);

                pScrollWnd->m_sbHorz.SetScrollInfo(&si);
            }

            break;
        }
    case WM_NCCALCSIZE:
        {
            if (NULL == wParam)
            {
                break;
            }

            LPNCCALCSIZE_PARAMS pNcCalcSizeParam = (LPNCCALCSIZE_PARAMS)lParam;
            DWORD dwStyle=::GetWindowLong(hwnd, GWL_STYLE);
            DWORD dwExStyle=::GetWindowLong(hwnd, GWL_EXSTYLE);

            BOOL bLeftScroll = dwExStyle & WS_EX_LEFTSCROLLBAR;
            CRect rcVert, rcHorz;

            int nOrgWidth=::GetSystemMetrics(SM_CXVSCROLL);
            int nWidth = pScrollWnd->m_nScrollWidth;

            if (bLeftScroll)
            {
                int nLeft = pNcCalcSizeParam->rgrc[0].left - (nOrgWidth - nWidth);
                int nBottom = pNcCalcSizeParam->rgrc[0].bottom;

                rcVert.right = nLeft;
                rcVert.left = nLeft - nWidth;
                rcVert.top = PADDING_WIDTH;
                rcVert.bottom = nBottom;
                rcHorz.left = nLeft;
                rcHorz.right = pNcCalcSizeParam->rgrc[0].right;
                rcHorz.top = nBottom;
                rcHorz.bottom = nBottom + nWidth;
            }
            else
            {
                int nRight = pNcCalcSizeParam->rgrc[0].right + (nOrgWidth - nWidth);
                int nBottom = pNcCalcSizeParam->rgrc[0].bottom;

                rcVert.left = nRight;
                rcVert.right = nRight + nWidth;
                rcVert.top = PADDING_WIDTH;
                rcVert.bottom = nBottom;
                rcHorz.left = PADDING_WIDTH;
                rcHorz.right = nRight;
                rcHorz.top = nBottom;
                rcHorz.bottom=nBottom + nWidth;
            }

            if (dwStyle & WS_VSCROLL)
            {
                pScrollWnd->m_sbVert.MoveWindow(&rcVert);
                pScrollWnd->m_sbVert.ShowWindow(SW_SHOW);
            }
            else
            {
                pScrollWnd->m_sbVert.ShowWindow(SW_HIDE);
            }

            if (dwStyle & WS_HSCROLL)
            {
                pScrollWnd->m_sbHorz.MoveWindow(&rcHorz);
                pScrollWnd->m_sbHorz.ShowWindow(SW_SHOW);
            }
            else
            {
                pScrollWnd->m_sbHorz.ShowWindow(SW_HIDE);
            }

            if (dwStyle & WS_VSCROLL && dwStyle & WS_HSCROLL)
            {
                pScrollWnd->m_nAngleType = bLeftScroll ? 1 : 2;
            }
            else
            {
                pScrollWnd->m_nAngleType = 0;
            }

            pScrollWnd->PostMessage(MSG_DESTMOVE, dwStyle & WS_VSCROLL, bLeftScroll);
            break;
        }
    default:
        break;
    }

    return lResult;
}

// CFScrollWnd

IMPLEMENT_DYNAMIC(CFScrollWnd, CWnd)

CFScrollWnd::CFScrollWnd()
: m_funOldProc(NULL)
, m_nScrollWidth(DEFAULT_SCROLL_WIDTH)
, m_nAngleType(0)
, m_bOpering(FALSE)
{

}

CFScrollWnd::~CFScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CFScrollWnd, CWnd)
    ON_MESSAGE(MSG_DESTMOVE, CFScrollWnd::OnDestMove)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CFScrollWnd 消息处理程序

void CFScrollWnd::SkinScroll(CWnd *pWnd)
{
    CWnd* pParent = pWnd->GetParent();
    if (!pParent)
    {
        return;
    }

    CRect rcScroll, rcWnd;
    pWnd->GetWindowRect(&rcScroll);
    pParent->ScreenToClient(&rcScroll);

    rcWnd = rcScroll;
    OffsetRect(&rcWnd,-rcWnd.left,-rcWnd.top);

    UINT uID = pWnd->GetDlgCtrlID();

    this->Create(AfxRegisterWndClass(NULL), _T("FScrollWndFrame"),
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rcScroll, pParent, uID);

    m_wndLimit.Create(NULL, _T("FScrollWndLimit"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        CRect(0, 0, rcScroll.right, rcScroll.bottom), this, ID_SCROLL_LIMIT);

    m_sbVert.Create(WS_CHILD | SBS_VERT, CRect(0, 0, 0, 0), this, ID_SCROLL_VERT);
    m_sbHorz.Create(WS_CHILD | SBS_HORZ, CRect(0, 0, 0, 0), this, ID_SCROLL_HORZ);

    pWnd->SetParent(&m_wndLimit);
    SetWindowLong(pWnd->m_hWnd, GWL_USERDATA, (LONG)this);
    pWnd->MoveWindow(&rcWnd);

    m_funOldProc = (WNDPROC)SetWindowLong(pWnd->m_hWnd, GWL_WNDPROC, (LONG)HookWndProc);
}

LRESULT CFScrollWnd::OnDestMove(WPARAM wParam, LPARAM lParam)
{
    m_bOpering = TRUE;

    BOOL bVScroll = wParam;
    BOOL bLeftScroll = lParam;
    CRect rcClient;

    GetClientRect(&rcClient);

    CRect rcLimit,rcWnd;
    rcWnd = rcClient;
    rcLimit = rcClient;

    if(GetWindowLong(m_sbHorz.m_hWnd,GWL_STYLE) & WS_VISIBLE)
    {
        rcLimit.bottom -= m_nScrollWidth;
    }

    if (bLeftScroll)
    {
        if (bVScroll)
        {
            rcLimit.left += m_nScrollWidth;
            OffsetRect(&rcWnd, -m_nScrollWidth, 0);
        }
    }
    else
    {
        if (bVScroll)
        {
            rcLimit.right -= m_nScrollWidth;
        }
    }

    CWnd* pWnd = m_wndLimit.GetWindow(GW_CHILD);
    m_wndLimit.MoveWindow(&rcLimit);
    pWnd->SetWindowPos(NULL,rcWnd.left, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    m_bOpering = FALSE;
    return 0;
}

void CFScrollWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (1 == m_nAngleType)
    {   // 左下组合
        CRect rcClient;
        GetClientRect(&rcClient);
        rcClient.top = rcClient.bottom - m_sbHorz.GetScrollWidth() - PADDING_WIDTH;
        rcClient.left = PADDING_WIDTH;
        rcClient.bottom = rcClient.bottom - PADDING_WIDTH;
        rcClient.right = rcClient.left + m_sbHorz.GetScrollWidth();

        RenderEngine->DrawColor(&dc, rcClient, RGB(242, 242, 242));
    }
    else if (2 == m_nAngleType)
    {   // 右下组合
        CRect rcClient;
        GetClientRect(&rcClient);
        rcClient.top = rcClient.bottom - m_sbHorz.GetScrollWidth() - PADDING_WIDTH;
        rcClient.left = rcClient.right - m_sbHorz.GetScrollWidth() - PADDING_WIDTH;
        rcClient.bottom = rcClient.bottom - PADDING_WIDTH;
        rcClient.right = rcClient.right - PADDING_WIDTH;

        RenderEngine->DrawColor(&dc, rcClient, RGB(242, 242, 242));
    }
}

CFScrollWnd* SkinWndScroll(CWnd *pWnd)
{
    CFScrollWnd *pScrollWnd = new CFScrollWnd;
    if (!pScrollWnd)
    {
        return NULL;
    }

    pScrollWnd->SkinScroll(pWnd);
    return pScrollWnd;
}

BOOL UnskinWndScroll(CWnd *pWnd)
{
    CFScrollWnd* pScrollWnd = (CFScrollWnd*)GetWindowLong(pWnd->m_hWnd, GWL_USERDATA);
    if(!pScrollWnd)
    {
        return FALSE;
    }

    CRect rcClient;
    CWnd *pParent = pScrollWnd->GetParent();
    pScrollWnd->GetWindowRect(&rcClient);
    pParent->ScreenToClient(&rcClient);

    SetWindowLong(pWnd->m_hWnd, GWL_WNDPROC, (LONG)pScrollWnd->m_funOldProc);
    SetWindowLong(pWnd->m_hWnd, GWL_USERDATA, 0);

    pWnd->SetParent(pParent);
    pWnd->MoveWindow(&rcClient);

    pScrollWnd->DestroyWindow();
    delete pScrollWnd;
    pScrollWnd = NULL;

    return TRUE;
}

void CFScrollWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (0 == cx || 0 == cy)
    {
        return;
    }

    if (m_wndLimit.m_hWnd)
    {
        CWnd *pWnd = m_wndLimit.GetWindow(GW_CHILD);
        pWnd->MoveWindow(0, 0, cx, cy);
    }
}

void CFScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CWnd* pWnd = m_wndLimit.GetWindow(GW_CHILD);
    pWnd->SendMessage(WM_VSCROLL, MAKELONG(nSBCode, nPos), NULL);

    if (SB_THUMBTRACK == nSBCode)
    {
        return;
    }

    SCROLLINFO si={0};
    si.cbSize=sizeof(si);
    si.fMask = SIF_ALL;
    pWnd->GetScrollInfo(SB_VERT, &si);
    pScrollBar->SetScrollInfo(&si, TRUE);
}


void CFScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CWnd* pWnd = m_wndLimit.GetWindow(GW_CHILD);
    pWnd->SendMessage(WM_HSCROLL, MAKELONG(nSBCode, nPos), NULL);

    if (SB_THUMBTRACK == nSBCode)
    {
        return;
    }

    SCROLLINFO si={0};
    si.cbSize=sizeof(si);
    si.fMask = SIF_ALL;
    pWnd->GetScrollInfo(SB_HORZ, &si);
    pScrollBar->SetScrollInfo(&si, TRUE);
}
