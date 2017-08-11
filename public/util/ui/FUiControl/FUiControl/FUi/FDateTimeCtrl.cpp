// FDateTimeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FDateTimeCtrl.h"
#include "FMemDCEx.h"
#include "CalendarDlg.h"

// CFDateTimeCtrl

CCalendarDlg* g_pCalWnd = NULL;

#define TEXT_MARGIN 10   // 时间文字的边距
#define TIME_WIDTH  15   // 单个时间单位宽度

#define COLOR_BORDER       RGB(210, 210, 210)     // 边框颜色
#define COLOR_BG_SELECT    RGB(150, 210, 255)     // 选中时间的背景颜色

IMPLEMENT_DYNAMIC(CFDateTimeCtrl, CDateTimeCtrl)

CFDateTimeCtrl::CFDateTimeCtrl()
: m_pImgCal(NULL)
, m_pImgTime(NULL)
, m_pImgTranBkg(NULL)
, m_isSelTime(false)
, m_bEnable(true)
, m_nStyle(FDTS_TIME)
, m_nCalStyle(CS_DATE)
, m_nSelTimeRect(ST_HOUR)
, m_rcMask(CRect(0,0,0,0))
, m_rcCal(CRect(0,0,0,0))
, m_rcTime(CRect(0,0,0,0))
, m_rcHour(CRect(0,0,0,0))
, m_rcMini(CRect(0,0,0,0))
, m_rcSecd(CRect(0,0,0,0))
{
}

CFDateTimeCtrl::~CFDateTimeCtrl()
{
    if (g_pCalWnd)
    {
        delete g_pCalWnd;
        g_pCalWnd = NULL;
    }
}


BEGIN_MESSAGE_MAP(CFDateTimeCtrl, CDateTimeCtrl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_ENABLE()
    ON_MESSAGE(WM_CALDLG_CLOSE, &CFDateTimeCtrl::OnMsgCaldlgClose)
END_MESSAGE_MAP()


// CFDateTimeCtrl 消息处理程序
void CFDateTimeCtrl::PreSubclassWindow()
{
    // 默认样式
    SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT);

    CDateTimeCtrl::PreSubclassWindow();
}

int CFDateTimeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDateTimeCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 默认样式
    SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT);

    return 0;
}


void CFDateTimeCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient;
    GetClientRect(&rcClient);
    FUi::CMemDCEX memDC(&dc, rcClient);

    // 计算屏蔽系统日历的区域
    if (CRect(0,0,0,0) == m_rcMask)
    {
        m_rcMask = rcClient;
        m_rcMask.left = rcClient.right - rcClient.Height() * 3 / 2;
    }

    // 计算选中时间的区域
    if (FDTS_TIME == m_nStyle && CRect(0,0,0,0) == m_rcHour)
    {
        m_rcSecd = rcClient;
        m_rcSecd.top = rcClient.top + 2;
        m_rcSecd.bottom = rcClient.bottom - 2;
        m_rcHour = m_rcMini = m_rcSecd;

        m_rcHour.left += TEXT_MARGIN - 1;
        m_rcHour.right = m_rcHour.left + TIME_WIDTH;

        m_rcMini.left = m_rcHour.right + 2;
        m_rcMini.right = m_rcMini.left + TIME_WIDTH;

        m_rcSecd.left = m_rcMini.right + 3;
        m_rcSecd.right = m_rcSecd.left + TIME_WIDTH;
    }

    // 读取图片
    if (FDTS_CAL == m_nStyle && NULL == m_pImgCal)
    {
        m_pImgCal = CResMgr::GetInstance()->GetRes(_T("datetime_cal"));
    }

    if (FDTS_TIME == m_nStyle && NULL == m_pImgTime)
    {
        m_pImgTime = CResMgr::GetInstance()->GetRes(_T("datetime_time"));
    }

    if (!m_pImgTranBkg)
    {
        m_pImgTranBkg = CResMgr::GetInstance()->GetRes(_T("calendar_transparent_bg"));
    }

    RenderEngine->DrawColor(&memDC, rcClient, FUi::White);
    RenderEngine->DrawRect(&memDC, rcClient, 1, COLOR_BORDER);

    // 画选中的时间背景
    if (m_isSelTime)
    {
        switch (m_nSelTimeRect)
        {
        case ST_HOUR:
            {
                RenderEngine->DrawColor(&memDC, m_rcHour, COLOR_BG_SELECT);
                break;
            }
        case ST_MINI:
            {
                RenderEngine->DrawColor(&memDC, m_rcMini, COLOR_BG_SELECT);
                break;
            }
        case ST_SECD:
            {
                RenderEngine->DrawColor(&memDC, m_rcSecd, COLOR_BG_SELECT);
                break;
            }
        default:
            break;
        }
    }

    // 获取时间字符串，当小时小于10时前面面补0
    CString strWindow, strTemp;
    GetWindowText(strTemp);
    if (strTemp.GetLength() >= 8)
    {
        strWindow = strTemp;
    }
    else
    {
        strWindow = _T("0") + strTemp;
    }

    CRect rcText(rcClient);
    CFont* pOldFont = memDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
    rcText.left += TEXT_MARGIN;
    RenderEngine->DrawTextX(&memDC, strWindow, rcText, FUi::Black, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    memDC->SelectObject(pOldFont);

    if (FDTS_CAL == m_nStyle && m_pImgCal)
    {
        m_rcCal = rcClient;
        int deflateLen = rcClient.Height()/ 5;
        m_rcCal.left = m_rcCal.right - m_rcCal.Height();
        m_rcCal.DeflateRect(deflateLen, deflateLen, deflateLen, deflateLen);

        RectF rcfDst((REAL)m_rcCal.left, (REAL)m_rcCal.top,
            (REAL)m_rcCal.Width(), (REAL)m_rcCal.Height());
        RenderEngine->DrawImageX(&memDC, m_pImgCal, rcfDst, 0, 0,
            (REAL)m_pImgCal->GetWidth(), (REAL)m_pImgCal->GetHeight());
    }
    else if (m_pImgTime)
    {
        m_rcTime = rcClient;
        int deflateLen = rcClient.Height()/ 5;
        m_rcTime.left = m_rcTime.right - m_rcTime.Height();
        m_rcTime.DeflateRect(deflateLen, deflateLen, deflateLen, deflateLen);

        RectF rcfDst((REAL)m_rcTime.left, (REAL)m_rcTime.top,
            (REAL)m_rcTime.Width(), (REAL)m_rcTime.Height());
        RenderEngine->DrawImageX(&memDC, m_pImgTime, rcfDst, 0, 0,
            (REAL)m_pImgTime->GetWidth(), (REAL)m_pImgTime->GetHeight());
    }

    if (!m_bEnable && m_pImgTranBkg)
    {
        CRect rcTran(rcClient);

        RectF rcfDst((REAL)rcTran.left, (REAL)rcTran.top, (REAL)rcTran.Width(), (REAL)rcTran.Height());
        RenderEngine->DrawImageX(&memDC, m_pImgTranBkg, rcfDst, 0, 0, 
            (REAL)m_pImgTranBkg->GetWidth(), (REAL)m_pImgTranBkg->GetHeight());
    }
}

void CFDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (FDTS_CAL == m_nStyle && m_rcCal.PtInRect(point))
    {
        if (NULL != g_pCalWnd)
        {
            delete g_pCalWnd;
            g_pCalWnd = NULL;
        }
        else
        {
            g_pCalWnd = new CCalendarDlg;
            g_pCalWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, 1000);
            g_pCalWnd->SetStyle(m_nCalStyle);  // 日历的样式

            CRect rcClient;
            GetClientRect(&rcClient);
            ClientToScreen(&rcClient);

            g_pCalWnd->SetWindowPos(NULL, rcClient.left, rcClient.bottom + 5, 0, 0, SWP_NOSIZE);
            g_pCalWnd->ShowWindow(SW_SHOW); 
        }
    }
    else if (m_rcTime.PtInRect(point))
    {
        CRect rcUp(m_rcTime);
        CRect rcDown(m_rcTime);
        rcUp.bottom = (m_rcTime.top + m_rcTime.bottom) / 2;
        rcDown.top = (m_rcTime.top + m_rcTime.bottom) / 2;

        // 改变时间
        SYSTEMTIME timeNow;
        GetTime(&timeNow);
        if (rcUp.PtInRect(point))
        {
            switch (m_nSelTimeRect)
            {
            case ST_HOUR:
                {
                    timeNow.wHour++;
                    if (timeNow.wHour > 23)
                    {
                        timeNow.wHour = 0;
                    }
                    break;
                }
            case ST_MINI:
                {
                    timeNow.wMinute++;
                    if (timeNow.wMinute > 59)
                    {
                        timeNow.wMinute = 0;
                    }
                    break;
                }
            case ST_SECD:
                {
                    timeNow.wSecond++;
                    if (timeNow.wSecond > 59)
                    {
                        timeNow.wSecond = 0;
                    }
                    break;
                }
            default:
                break;
            }
        }
        else if(rcDown.PtInRect(point))
        {
            switch (m_nSelTimeRect)
            {
            case ST_HOUR:
                {
                    if (0 == timeNow.wHour)
                    {
                        timeNow.wHour = 23;
                    }
                    else
                    {
                        timeNow.wHour--;
                    }

                    break;
                }
            case ST_MINI:
                {
                    if (0 == timeNow.wMinute)
                    {
                        timeNow.wMinute = 59;
                    }
                    else
                    {
                        timeNow.wMinute--;
                    }
                    break;
                }
            case ST_SECD:
                {
                    if (0 == timeNow.wSecond)
                    {
                        timeNow.wSecond = 59;
                    }
                    else
                    {
                        timeNow.wSecond--;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SetTime(timeNow);

        CRect rcReflash(m_rcHour);
        rcReflash.right = m_rcSecd.right;
        InvalidateRect(rcReflash);

        m_isSelTime = true;
    }

    if (m_rcMask.PtInRect(point))
    {
        return;
    }

    // 改变选中的时间项
    if (m_rcHour.PtInRect(point))
    {
        m_nSelTimeRect = ST_HOUR;
    }
    else if (m_rcMini.PtInRect(point))
    {
        m_nSelTimeRect = ST_MINI;
    }
    else if (m_rcSecd.PtInRect(point))
    {
        m_nSelTimeRect = ST_SECD;
    }
    m_isSelTime = true;
    ::PostMessage(GetParent()->GetSafeHwnd(), WM_SELTIME, m_nSelTimeRect, NULL);

    CRect rcReflash(m_rcHour);
    rcReflash.right = m_rcSecd.right;
    InvalidateRect(rcReflash);

    CDateTimeCtrl::OnLButtonDown(nFlags, point);
}

BOOL CFDateTimeCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CFDateTimeCtrl::OnSize(UINT nType, int cx, int cy)
{
    CDateTimeCtrl::OnSize(nType, cx, cy);
}

LRESULT CFDateTimeCtrl::OnMsgCaldlgClose(WPARAM wParam, LPARAM /*lParam*/)
{
    if (WPARAM_SELECT == wParam)
    {
        SYSTEMTIME selectTime;
        g_pCalWnd->GetSelectTime(selectTime);
        SetTime(&selectTime);
    }

    if (NULL != g_pCalWnd)
    {
        delete g_pCalWnd;
        g_pCalWnd = NULL;
    }

    Invalidate();
    return true;
}

void CFDateTimeCtrl::SetCalStyle(int nCalStyle)
{
    m_nCalStyle = nCalStyle;
}

void CFDateTimeCtrl::SetStyle(int nStyle)
{
    m_nStyle  = nStyle;

    if (FDTS_CAL == nStyle)
    {
        ModifyStyle(DTS_TIMEFORMAT, DTS_SHORTDATEFORMAT);
    }
    else
    {
        ModifyStyle(DTS_SHORTDATEFORMAT, DTS_TIMEFORMAT);
    }
}

void CFDateTimeCtrl::OnEnable(BOOL bEnable)
{
    CDateTimeCtrl::OnEnable(bEnable);

    m_bEnable = bEnable;
}
