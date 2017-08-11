#include "stdafx.h"
#include "FMemDCEx.h"
#include "CalSelectWnd.h"

#define ID_BTN_PRE      1000
#define ID_BTN_NEXT     1001

#define WND_MARGIN_TOP  6  // �������±߾�
#define WND_MARGIN_LEFT 5  // �������ұ߾�
#define BTN_WIDTH       16   // ǰ��ť�Ĵ�С

#define COLOR_BORDER         RGB(188, 188, 188)     // �߿���ɫ
#define COLOR_HEADER         RGB(63, 133, 228)     // ͷ����ɫ
#define COLOR_SELECT         RGB(63, 133, 228)     // ѡ����ɫ
#define COLOR_LINE_NORMAL    RGB(240, 240, 240)    // �����ָ�����ɫ
#define COLOR_LINE_HOVER     RGB(63, 133, 228)     // �����ָ�����ɫ
#define COLOR_TEXT_NORMAL    RGB(30, 30, 30)       // ����������ɫ
#define COLOR_TEXT_UNUSE     RGB(204, 204, 204)    // ����������ɫ
#define COLOR_TEXT_TODAY     RGB(63, 133, 228)     // ����������ɫ
#define COLOR_TEXT_SELECT    RGB(63, 133, 228)     // ѡ��������ɫ

// CCalSelectWnd

IMPLEMENT_DYNAMIC(CCalSelectWnd, CWnd)

CCalSelectWnd::CCalSelectWnd()
: m_nStyle(-1)
, m_nPreStyle(-1)
, m_nCurYear(-1)
, m_nHover(60)
, m_nSelect(60)
, m_nSelTime(-1)
, m_rcDraw(CRect(0,0,0,0))
, m_pbtnPre(NULL)
, m_pbtnNext(NULL)
{
}

CCalSelectWnd::~CCalSelectWnd()
{
    if (m_pbtnPre)
    {
        delete m_pbtnPre;
    }
    if (m_pbtnNext)
    {
        delete m_pbtnNext;
    }
}


BEGIN_MESSAGE_MAP(CCalSelectWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(ID_BTN_PRE, &CCalSelectWnd::OnBnClickedBtnPre)
    ON_BN_CLICKED(ID_BTN_NEXT, &CCalSelectWnd::OnBnClickedBtnNext)
END_MESSAGE_MAP()



// CCalSelectWnd ��Ϣ�������

void CCalSelectWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient;
    GetClientRect(rcClient);
    FUi::CMemDCEX memDC(&dc, rcClient);

    CRect rcBg(rcClient);
    RenderEngine->DrawRect(&memDC, rcBg, 1, COLOR_BORDER);

    UpdataTimeInfo(m_nStyle);
    switch (m_nStyle)
    {
    case SWS_YEAR:
        {
            rcBg.DeflateRect(1, 0, 1, 1);
            RenderEngine->DrawColor(&memDC, rcBg, FUi::White);
            DrawForStyle(&memDC, m_nStyle);
            break;
        }
    case SWS_MONTH:
        {
            rcBg.DeflateRect(1, 0, 1, 1);
            RenderEngine->DrawColor(&memDC, rcBg, FUi::White);
            DrawForStyle(&memDC, m_nStyle);
            break;
        }
    case SWS_HOUR:
        {
            rcBg.DeflateRect(1, 1, 1, 1);
            RenderEngine->DrawColor(&memDC, rcBg, FUi::White);
            DrawForStyle(&memDC, m_nStyle);
            break;
        }
    case SWS_MINI:
        {
            rcBg.DeflateRect(1, 1, 1, 1);
            RenderEngine->DrawColor(&memDC, rcBg, FUi::White);
            DrawForStyle(&memDC, m_nStyle);
            break;
        }
    case SWS_SECD:
        {
            rcBg.DeflateRect(1, 1, 1, 1);
            RenderEngine->DrawColor(&memDC, rcBg, FUi::White);
            DrawForStyle(&memDC, m_nStyle);
            break;
        }
    default:
        break;
    }
}

void CCalSelectWnd::SetStyle(int nStyle)
{
    m_nStyle = nStyle;
    Invalidate();
}

void CCalSelectWnd::SetCurYear(int nYear)
{
    m_nCurYear = nYear;
}

void CCalSelectWnd::DrawForStyle(CDC *pDC, int nStyle)
{
    // ����ť
    if (SWS_YEAR == nStyle && !m_pbtnPre && !m_pbtnNext)
    {
        CRect rcClient;
        GetClientRect(&rcClient);

        m_pbtnPre = new CFButton;
        m_pbtnPre->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_PRE);
        m_pbtnPre->SetObjectName(_T("calsel_pre"));
        m_pbtnPre->MoveWindow(rcClient.left + WND_MARGIN_LEFT,
            rcClient.bottom / 2 - BTN_WIDTH / 2, BTN_WIDTH, BTN_WIDTH);

        m_pbtnNext = new CFButton;
        m_pbtnNext->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_NEXT);
        m_pbtnNext->SetObjectName(_T("calsel_next"));
        m_pbtnNext->MoveWindow(rcClient.right - WND_MARGIN_LEFT - BTN_WIDTH,
            rcClient.bottom / 2 - BTN_WIDTH / 2, BTN_WIDTH, BTN_WIDTH);
    }
    else if (SWS_YEAR != nStyle && m_pbtnPre && m_pbtnNext)
    {
        delete m_pbtnPre;
        delete m_pbtnNext;
        m_pbtnPre = NULL;
        m_pbtnNext = NULL;
    }

    // ��ʱ����Ϣ���ֺͱ߿�
    CFont* pOldFont = NULL;
    for (int i = 0; i < m_nTimeNum; i++)
    {
        RenderEngine->DrawRect(pDC, m_strArrayTime[i].rect, 1, COLOR_LINE_NORMAL);
    }
    RenderEngine->DrawRect(pDC, m_rcDraw, 1, FUi::White);

    for (int i = 0; i < m_nTimeNum; i++)
    {
        CRect rcText(m_strArrayTime[i].rect);

        if (TIME_STATE_SELECT & m_strArrayTime[i].nState)
        {
            RenderEngine->DrawColor(pDC, rcText, COLOR_SELECT);
            CString strText;
            strText.Format(_T("%d"), m_strArrayTime[i].nTime);
            pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
            RenderEngine->DrawTextX(pDC, strText, rcText, FUi::White, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            if (TIME_STATE_CUR & m_strArrayTime[i].nState)
            {
                CRect rcPoint;
                rcPoint.left = (rcText.left + rcText.right) / 2 - 1;
                rcPoint.right = (rcText.left + rcText.right) / 2 + 1;
                rcPoint.top = (rcText.top + rcText.bottom) / 2 + 8;
                rcPoint.bottom = rcPoint.top + 2;
                RenderEngine->DrawColor(pDC, rcPoint, FUi::White);
            }
            continue;;
        }

        if (TIME_STATE_HOVER & m_strArrayTime[i].nState)
        {
            RenderEngine->DrawRect(pDC, rcText, 1, COLOR_LINE_HOVER);
        }

        if(TIME_STATE_CUR & m_strArrayTime[i].nState)
        {
            CString strText;
            strText.Format(_T("%d"), m_strArrayTime[i].nTime);
            pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_SB));
            RenderEngine->DrawTextX(pDC, strText, rcText, COLOR_TEXT_TODAY, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            CRect rcPoint;
            rcPoint.left = (rcText.left + rcText.right) / 2 - 1;
            rcPoint.right = (rcText.left + rcText.right) / 2 + 1;
            rcPoint.top = (rcText.top + rcText.bottom) / 2 + 8;
            rcPoint.bottom = rcPoint.top + 2;
            RenderEngine->DrawColor(pDC, rcPoint, COLOR_TEXT_TODAY);
        }
        else
        {
            CString strText;
            strText.Format(_T("%d"), m_strArrayTime[i].nTime);
            pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
            RenderEngine->DrawTextX(pDC, strText, rcText, COLOR_TEXT_NORMAL, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
    pDC->SelectObject(pOldFont);
}

void CCalSelectWnd::UpdataTimeInfo(int nStyle)
{
    if (m_nPreStyle == nStyle)
    {
        return;
    }
    else
    {
        m_nPreStyle = nStyle;
    }

    // ��ͬ��ʽ��ʱ����Ϣ������ͬ
    if (SWS_YEAR == nStyle)
    {
        m_nTimeNum = 10;
    }
    else if (SWS_MONTH == nStyle || SWS_MINI == nStyle || SWS_SECD == nStyle)
    {
        m_nTimeNum = 12;
    }
    else if (SWS_HOUR == nStyle)
    {
        m_nTimeNum = 24;
    }

    SYSTEMTIME timeNow;
    GetLocalTime(&timeNow);

    CRect rcClient;
    GetClientRect(&rcClient);
    m_rcDraw = rcClient;
    m_rcDraw.DeflateRect(WND_MARGIN_LEFT, WND_MARGIN_TOP, WND_MARGIN_LEFT + 1, WND_MARGIN_TOP + 1);

    switch (nStyle)
    {
    case SWS_YEAR:
        {
            // ����ʽ������Ҫ�а�ť��λ��
            m_rcDraw.DeflateRect(m_rcDraw.Width() / 12, 0, m_rcDraw.Width() / 12 + 1, 0);
            for (int i = 0; i < m_nTimeNum; i++)
            {
                m_strArrayTime[i].nTime = i + m_nCurYear - 2;
                m_strArrayTime[i].nState = TIME_STATE_NORMAL;
                if (timeNow.wYear == m_strArrayTime[i].nTime)
                {
                    m_strArrayTime[i].nState = TIME_STATE_CUR;
                }

                // ����������Ϣ
                CRect rcTime;
                rcTime.top = m_rcDraw.top + m_rcDraw.Height() * (i / 5) / 2;
                rcTime.bottom = rcTime.top + m_rcDraw.Height() / 2 + 1;
                rcTime.left = m_rcDraw.left + m_rcDraw.Width() * (i % 5) / 5;
                rcTime.right = rcTime.left + m_rcDraw.Width() / 5 + 1;

                m_strArrayTime[i].rect = rcTime;
            }
            break;
        }
    case SWS_MONTH:
        {
            for (int i = 0; i < m_nTimeNum; i++)
            {
                m_strArrayTime[i].nTime = i + 1;
                m_strArrayTime[i].nState = TIME_STATE_NORMAL;
                if (timeNow.wMonth == i + 1)
                {
                    m_strArrayTime[i].nState = TIME_STATE_CUR;
                }

                // ����������Ϣ
                CRect rcTime;
                rcTime.top = m_rcDraw.top + m_rcDraw.Height() * (i / 6) / 2;
                rcTime.bottom = rcTime.top + m_rcDraw.Height() / 2 + 1;
                rcTime.left = m_rcDraw.left + m_rcDraw.Width() * (i % 6) / 6;
                rcTime.right = rcTime.left + m_rcDraw.Width() / 6 + 1;

                m_strArrayTime[i].rect = rcTime;
            }
            break;
        }
    case SWS_HOUR:
        {
            for (int i = 0; i < m_nTimeNum; i++)
            {
                m_strArrayTime[i].nTime = i;
                m_strArrayTime[i].nState = TIME_STATE_NORMAL;
                if (timeNow.wHour == i)
                {
                    m_strArrayTime[i].nState = TIME_STATE_CUR;
                }

                // ����������Ϣ
                CRect rcTime;
                rcTime.top = m_rcDraw.top + m_rcDraw.Height() * (i / 6) / 4;
                rcTime.bottom = rcTime.top + m_rcDraw.Height() / 4 + 1;
                rcTime.left = m_rcDraw.left + m_rcDraw.Width() * (i % 6) / 6;
                rcTime.right = rcTime.left + m_rcDraw.Width() / 6 + 1;

                m_strArrayTime[i].rect = rcTime;
            }
            break;
        }
    case SWS_MINI:
    case SWS_SECD:
        {
            for (int i = 0; i < m_nTimeNum; i++)
            {
                m_strArrayTime[i].nTime = i * 5;
                m_strArrayTime[i].nState = TIME_STATE_NORMAL;

                // ����������Ϣ
                CRect rcTime;
                rcTime.top = m_rcDraw.top + m_rcDraw.Height() * (i / 6) / 2;
                rcTime.bottom = rcTime.top + m_rcDraw.Height() / 2 + 1;
                rcTime.left = m_rcDraw.left + m_rcDraw.Width() * (i % 6) / 6;
                rcTime.right = rcTime.left + m_rcDraw.Width() / 6 + 1;

                m_strArrayTime[i].rect = rcTime;
            }
            break;
        }
    default:
        break;
    }
}

void CCalSelectWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // ע������뿪��Ϣ����ͣ��Ϣ
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
    mouseEvent.hwndTrack = GetSafeHwnd();
    mouseEvent.dwHoverTime = 1;
    _TrackMouseEvent(&mouseEvent);

    CWnd::OnMouseMove(nFlags, point);
}

void CCalSelectWnd::OnMouseLeave()
{
    // ����뿪��ȡ����ͣ״̬��ѡ��״̬
    if (m_nHover != 60)
    {
        m_strArrayTime[m_nHover].nState &= 0xFB;
        InvalidateRect(m_strArrayTime[m_nHover].rect);
        m_nHover = 60;
    }

    if (m_nSelect != 60)
    {
        m_strArrayTime[m_nSelect].nState &= 0xEF;
        InvalidateRect(m_strArrayTime[m_nSelect].rect);
        m_nSelect = 60;
    }

    CWnd::OnMouseLeave();
}

void CCalSelectWnd::OnMouseHover(UINT nFlags, CPoint point)
{
    // ��겻�����������ڣ�ȡ����ͣ״̬��ѡ��״̬
    if (!m_rcDraw.PtInRect(point))
    {
        if (m_nHover != 60)
        {
            m_strArrayTime[m_nHover].nState &= 0xFB;
            InvalidateRect(m_strArrayTime[m_nHover].rect);
            m_nHover = 60;
        }

        if (m_nSelect != 60)
        {
            m_strArrayTime[m_nSelect].nState &= 0xEF;
            InvalidateRect(m_strArrayTime[m_nSelect].rect);
            m_nSelect = 60;
        }

        CWnd::OnMouseHover(nFlags, point);
        return;
    }

    // ��������ͣ����һ��
    for (int i = 0; i < m_nTimeNum; i++)
    {
        if (!m_strArrayTime[i].rect.PtInRect(point))
        {
            continue;
        }

        // ��ͣ��ı䣬ˢ����ͣ���ԭ������ͣ��
        if (m_nHover != i)
        {
            m_strArrayTime[i].nState |= TIME_STATE_HOVER;
            m_strArrayTime[m_nHover].nState &= 0xFB;
            InvalidateRect(m_strArrayTime[i].rect);
            InvalidateRect(m_strArrayTime[m_nHover].rect);
        }

        m_nHover = i;
        break;
    }

    CWnd::OnMouseHover(nFlags, point);
}

void CCalSelectWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // û�е������Ч������
    if (!m_rcDraw.PtInRect(point))
    {
        m_nSelect = 60;
        CWnd::OnLButtonDown(nFlags, point);
        return;
    }
    // ������������һ��
    for (int i = 0; i < m_nTimeNum; i++)
    {
        if (!m_strArrayTime[i].rect.PtInRect(point))
        {
            continue;
        }

        if (m_nSelect != i)
        {
            m_strArrayTime[i].nState |= TIME_STATE_SELECT;
            m_strArrayTime[m_nSelect].nState &= 0xEF;
            InvalidateRect(m_strArrayTime[i].rect);
            InvalidateRect(m_strArrayTime[m_nSelect].rect);

            // ��¼ѡ�е�ʱ��
            m_nSelTime = m_strArrayTime[i].nTime;
        }

        m_nSelect = i;
        break;
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CCalSelectWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // ��ѡ����
    if (m_nSelect != 60)
    {
        ::PostMessage(GetParent()->GetSafeHwnd(), WM_CALSELWND_CLOSE, m_nStyle, m_nSelTime);
    }

    CWnd::OnLButtonUp(nFlags, point);
}

void CCalSelectWnd::OnBnClickedBtnPre()
{
    // �ı�����ʽʱ��ֵ
    SYSTEMTIME timeNow;
    GetLocalTime(&timeNow);

    for (int i = 0; i < m_nTimeNum; i++)
    {
        m_strArrayTime[i].nTime -= 10;
        m_strArrayTime[i].nState = TIME_STATE_NORMAL;
        if (timeNow.wYear == m_strArrayTime[i].nTime)
        {
            m_strArrayTime[i].nState = TIME_STATE_CUR;
        }
    }
    InvalidateRect(m_rcDraw);
}

void CCalSelectWnd::OnBnClickedBtnNext()
{
    // �ı�����ʽʱ��ֵ
    SYSTEMTIME timeNow;
    GetLocalTime(&timeNow);

    for (int i = 0; i < m_nTimeNum; i++)
    {
        m_strArrayTime[i].nTime += 10;
        m_strArrayTime[i].nState = TIME_STATE_NORMAL;
        if (timeNow.wYear == m_strArrayTime[i].nTime)
        {
            m_strArrayTime[i].nState = TIME_STATE_CUR;
        }
    }
    InvalidateRect(m_rcDraw);
}
