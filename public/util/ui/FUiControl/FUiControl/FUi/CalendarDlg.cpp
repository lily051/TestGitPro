// FCalendarDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalendarDlg.h"
#include "FMemDCEx.h"

#define DIALOG_WIDTH  234   // ���ڿ��
#define DIALOG_HEIGHT 288   // ���ڸ߶�
#define HEADER_HEIGHT 34    // ͷ���߶�
#define BOTTOM_HEIGHT 38    // �ײ��߶�
#define WEEK_HEIGHT   29    // �������߶�

#define COLOR_HEADER         RGB(63, 133, 228)     // ͷ����ɫ
#define COLOR_SELECT         RGB(63, 133, 228)     // ѡ����ɫ
#define COLOR_LINE_NORMAL    RGB(250, 250, 250)    // �����ָ�����ɫ
#define COLOR_LINE_HOVER     RGB(63, 133, 228)     // �����ָ�����ɫ
#define COLOR_TEXT_NORMAL    RGB(30, 30, 30)       // ����������ɫ
#define COLOR_TEXT_UNUSE     RGB(204, 204, 204)    // ����������ɫ
#define COLOR_TEXT_TODAY     RGB(63, 133, 228)     // ����������ɫ
#define COLOR_TEXT_SELECT    RGB(63, 133, 228)     // ѡ��������ɫ

#define ID_BTN_PRE_MONTH    1000
#define ID_BTN_NEXT_MONTH   1001
#define ID_BTN_YEAR         1002
#define ID_BTN_MONTH        1003
#define ID_BTN_TODAY        1004
#define ID_BTN_CURTIME      1005
#define ID_BTN_OK           1006
#define ID_DATETIME_TIME    1007
#define ID_WND_SELTIME      1008

// CFCalendarDlg �Ի���

IMPLEMENT_DYNAMIC(CCalendarDlg, CWnd)

CCalendarDlg::CCalendarDlg()
: m_isInit(false)
, m_bEnable(true)
, m_bAllEnable(false)
, m_pbtnPreMonth(NULL)
, m_pbtnNextMonth(NULL)
, m_pbtnYear(NULL)
, m_pbtnMonth(NULL)
, m_pbtnToday(NULL)
, m_pbtnCurTime(NULL)
, m_pbtnOk(NULL)
, m_pdtTime(NULL)
, m_pParent(NULL)
, m_pCalselWnd(NULL)
, m_pImgBkg(NULL)
, m_pImgTranBkg(NULL)
, m_nStyle(CS_DATE)
, m_rcDate(CRect(0,0,0,0))
, m_ptHover(CPoint(DATE_ROW_MAX, DATE_COL_MAX))
, m_ptSelect(CPoint(DATE_ROW_MAX, DATE_COL_MAX))
{
    GetLocalTime(&m_timeToday);
    m_nCurYear = m_timeToday.wYear;
    m_nCurMonth = m_timeToday.wMonth;

    m_strClassName = AfxRegisterWndClass(0);
    ZeroMemory(&m_timeSelect, sizeof(m_timeSelect));
	GetLocalTime(&m_timeSelect);
}

CCalendarDlg::~CCalendarDlg()
{

}

BEGIN_MESSAGE_MAP(CCalendarDlg, CWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONUP()
    ON_WM_ACTIVATE()
    ON_WM_MOUSEHOVER()
    ON_WM_DESTROY()
    ON_BN_CLICKED(ID_BTN_OK, &CCalendarDlg::OnBnClickedBtnOk)
    ON_BN_CLICKED(ID_BTN_PRE_MONTH, &CCalendarDlg::OnBnClickedBtnPreMonth)
    ON_BN_CLICKED(ID_BTN_NEXT_MONTH, &CCalendarDlg::OnBnClickedBtnNextMonth)
    ON_BN_CLICKED(ID_BTN_YEAR, &CCalendarDlg::OnBnClickedBtnYear)
    ON_BN_CLICKED(ID_BTN_MONTH, &CCalendarDlg::OnBnClickedBtnMonth)
    ON_BN_CLICKED(ID_BTN_TODAY, &CCalendarDlg::OnBnClickedBtnToday)
    ON_BN_CLICKED(ID_BTN_CURTIME, &CCalendarDlg::OnBnClickedBtnCurtime)
    ON_MESSAGE(WM_SELTIME, &CCalendarDlg::OnMsgSeltime)
    ON_MESSAGE(WM_CALSELWND_CLOSE, &CCalendarDlg::OnMsgCalselWndClose)
END_MESSAGE_MAP()


// CFCalendarDlg ��Ϣ�������

BOOL CCalendarDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                          const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    // �������ڲ��ܱ��游���ڣ���Ҫ�ֶ�����
    m_pParent = pParentWnd;

    // �Ը������ڵ���ʽ������WS_POPUP��
    lpszClassName = m_strClassName;
    dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	//dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    nID = 0;

    return CWnd::CreateEx(0, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CCalendarDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    InitDlg();
    InitBtn();

	// UpdataDate(m_nCurYear, m_nCurMonth, m_timeToday.wDay);
	// Ĭ��ѡ�пؼ�ʱ�䣬�����ǵ�ǰʱ��
	UpdataDate(m_timeSelect.wYear, m_timeSelect.wMonth, m_timeSelect.wDay);
    CalDateRect();

    return 0;
}

void CCalendarDlg::OnDestroy()
{
    CWnd::OnDestroy();

    if (m_pbtnPreMonth)
    {
        delete m_pbtnPreMonth;
    }
    if (m_pbtnNextMonth)
    {
        delete m_pbtnNextMonth;
    }
    if (m_pbtnYear)
    {
        delete m_pbtnYear;
    }
    if (m_pbtnMonth)
    {
        delete m_pbtnMonth;
    }
    if (m_pbtnToday)
    {
        delete m_pbtnToday;
    }
    if (m_pbtnCurTime)
    {
        delete m_pbtnCurTime;
    }
    if (m_pbtnOk)
    {
        delete m_pbtnOk;
    }
    if (m_pdtTime)
    {
        delete m_pdtTime;
    }
    if (m_pCalselWnd)
    {
        delete m_pCalselWnd;
    }
}

BOOL CCalendarDlg::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CCalendarDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

    CRect rcWnd;
    GetClientRect(rcWnd);
    FUi::CMemDCEX memDC(&dc, rcWnd);

    DrawWnd(&memDC);
}

void CCalendarDlg::InitDlg()
{
    SetWindowText(_T("Calendar"));
    MoveWindow(0, 0, DIALOG_WIDTH, DIALOG_HEIGHT);

    m_pImgBkg = CResMgr::GetInstance()->GetRes(_T("calendar"));
    m_pImgTranBkg = CResMgr::GetInstance()->GetRes(_T("calendar_transparent_bg"));
}

void CCalendarDlg::InitBtn()
{
    m_pbtnPreMonth = new CFButton;
    m_pbtnPreMonth->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_PRE_MONTH);
    m_pbtnPreMonth->SetObjectName(_T("pre_month"));

    m_pbtnNextMonth = new CFButton;
    m_pbtnNextMonth->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_NEXT_MONTH);
    m_pbtnNextMonth->SetObjectName(_T("next_month"));

    int nCtrlState = FUi::Normal | FUi::Hovered | FUi::Pressed | FUi::Disabled;

    m_pbtnToday = new CFButton;
    m_pbtnToday->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_TODAY);
    m_pbtnToday->SetObjectName(_T("calendar_t"));
    m_pbtnToday->SetTextFont(nCtrlState, SONG_12_N);
    m_pbtnToday->SetTextColor(nCtrlState, FUi::Black);
    m_pbtnToday->SetTextAlign(DT_CENTER);

    m_pbtnOk = new CFButton;
    m_pbtnOk->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_OK);
    m_pbtnOk->SetObjectName(_T("calendar_btn_ok"));
    m_pbtnOk->SetTextFont(nCtrlState, SONG_12_N);
    m_pbtnOk->SetTextColor(nCtrlState, FUi::White);
    m_pbtnOk->SetTextAlign(DT_CENTER);
    m_pbtnOk->SetTextMargin(0);

    m_pbtnCurTime = new CFButton;
    m_pbtnCurTime->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_CURTIME);
    m_pbtnCurTime->SetObjectName(_T("calendar_ct"));
    m_pbtnCurTime->SetTextFont(nCtrlState, SONG_12_N);
    m_pbtnCurTime->SetTextColor(nCtrlState, FUi::Black);
    m_pbtnCurTime->SetTextColor(FUi::Disabled, RGB(200,200,200));
    m_pbtnCurTime->SetTextAlign(DT_CENTER);
    m_pbtnCurTime->SetTextMargin(0);

    m_pbtnYear = new CFButton;
    m_pbtnYear->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_YEAR);
    m_pbtnYear->SetObjectName(_T("calendar_btn"));
    m_pbtnYear->SetTextFont(nCtrlState, SONG_12_SB);
    m_pbtnYear->SetTextColor(nCtrlState, FUi::White);
    m_pbtnYear->SetTextAlign(DT_CENTER);
    m_pbtnYear->SetTextMargin(0);

    m_pbtnMonth = new CFButton;
    m_pbtnMonth->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_BTN_MONTH);
    m_pbtnMonth->SetObjectName(_T("calendar_btn"));
    m_pbtnMonth->SetTextFont(nCtrlState, SONG_12_SB);
    m_pbtnMonth->SetTextColor(nCtrlState, FUi::White);
    m_pbtnMonth->SetTextAlign(DT_CENTER);
    m_pbtnMonth->SetTextMargin(0);

    m_pdtTime = new CFDateTimeCtrl;
    m_pdtTime->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_DATETIME_TIME);
    m_pdtTime->SetStyle(FDTS_TIME);
	m_pdtTime->SetTime(m_timeSelect);

    m_isInit = true;

    // ��ť��λ��
    CRect rcClient;
    GetClientRect(&rcClient);

    CPoint ptPos(12, 0);
    m_pbtnPreMonth->MoveWindow(ptPos.x, ptPos.y, 16, HEADER_HEIGHT);

    ptPos = CPoint(rcClient.right - 28, 0);
    m_pbtnNextMonth->MoveWindow(ptPos.x, ptPos.y, 16, HEADER_HEIGHT);

    ptPos = CPoint(75, 0);
    m_pbtnYear->MoveWindow(ptPos.x, ptPos.y, 50, HEADER_HEIGHT);

    ptPos = CPoint(125, 0);
    m_pbtnMonth->MoveWindow(ptPos.x, ptPos.y, 35, HEADER_HEIGHT);

    ptPos = CPoint(8, rcClient.bottom - 31);
    m_pbtnToday->MoveWindow(ptPos.x, ptPos.y, 218, 24);

    ptPos = CPoint(8, rcClient.bottom - 31);
    m_pbtnCurTime->MoveWindow(ptPos.x, ptPos.y, 66, 24);

    ptPos = CPoint(rcClient.right - 48, rcClient.bottom - 31);
    m_pbtnOk->MoveWindow(ptPos.x, ptPos.y, 40, 24);

    ptPos = CPoint(rcClient.right - 153, rcClient.bottom - 31);
    m_pdtTime->MoveWindow(ptPos.x, ptPos.y, 100, 24);


    // ��ť����
    CString strYear, strMonth;
//     strYear.Format(_T("%d��"), m_nCurYear);
//     strMonth.Format(_T("%d��"), m_nCurMonth);
 strYear.Format(_T("%d��"),  m_timeSelect.wYear);
 strMonth.Format(_T("%d��"),  m_timeSelect.wMonth);
    m_pbtnYear->SetWindowText(strYear);
    m_pbtnMonth->SetWindowText(strMonth);

    m_pbtnToday->SetWindowText(_T("����"));
    m_pbtnCurTime->SetWindowText(_T("��ǰʱ��"));
    m_pbtnOk->SetWindowText(_T("ȷ��"));

    // Ĭ��Ϊ��������ʽ
    m_pbtnToday->ShowWindow(SW_SHOW);
    m_pbtnCurTime->ShowWindow(SW_HIDE);
    m_pbtnOk->ShowWindow(SW_HIDE);
}

void CCalendarDlg::CalDateRect()
{
    // ������ʾ���ڵ�����, ����Ӧ���������
    CRect rcClient;
    GetClientRect(&rcClient);
    m_rcDate = rcClient;
    m_rcDate.top = HEADER_HEIGHT + WEEK_HEIGHT;
    m_rcDate.bottom = rcClient.bottom - BOTTOM_HEIGHT;
    m_rcDate.DeflateRect(1, 0, 1, 0);

    for (int i = 0; i < DATE_ROW_MAX; i++)
    {
        CRect rcText;
        rcText.top = m_rcDate.top + m_rcDate.Height() * i / DATE_ROW_MAX;
        rcText.bottom = rcText.top + m_rcDate.Height() / DATE_ROW_MAX + 1;

        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            rcText.left = m_rcDate.left + m_rcDate.Width() * j / DATE_COL_MAX;
            rcText.right = rcText.left + m_rcDate.Width() / DATE_COL_MAX + 1;

            m_strArrDate[i][j].rect = rcText;
        }
    }
}

void CCalendarDlg::DrawWnd(CDC *pDC)
{
    if (NULL == pDC || NULL == m_pImgBkg)
    {
        return ;
    }

     // ������
    CRect rcClient;
    GetClientRect(&rcClient);
    RectF rcfDst((REAL)rcClient.left, (REAL)rcClient.top, (REAL)rcClient.Width(), (REAL)rcClient.Height());
    RenderEngine->DrawImageX(pDC, m_pImgBkg, rcfDst, 0, 0, 
        (REAL)m_pImgBkg->GetWidth(), (REAL)m_pImgBkg->GetHeight());

    // ��ͷ��
    CRect rcHeader(rcClient);
    rcHeader.bottom = rcHeader.top + HEADER_HEIGHT;
    RenderEngine->DrawColor(pDC, rcHeader, COLOR_HEADER);

    // ������һ��
    CRect rcWeek(rcClient);
    TCHAR strWeek[DATE_COL_MAX][2] = {_T("��"), _T("һ"), _T("��"), _T("��"), _T("��"), _T("��"), _T("��")};
    for (int i= 0; i < DATE_COL_MAX; i++)
    {
        rcWeek.left = rcClient.Width() * i / DATE_COL_MAX;
        rcWeek.right = rcWeek.left + rcClient.Width() / DATE_COL_MAX;
        rcWeek.top = HEADER_HEIGHT;
        rcWeek.bottom = rcWeek.top + WEEK_HEIGHT;
        CFont* pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
        RenderEngine->DrawTextX(pDC, strWeek[i], rcWeek, FUi::Black, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
    }

    // ����������
    DrawDate(pDC);

    if (!m_bEnable && m_pImgTranBkg)
    {
        CRect rcTran(rcClient);
        rcTran.top = HEADER_HEIGHT;

        if (!m_bAllEnable)
        {
            rcTran.bottom = rcTran.bottom - BOTTOM_HEIGHT;
        }

        RectF rcfDst((REAL)rcTran.left, (REAL)rcTran.top, (REAL)rcTran.Width(), (REAL)rcTran.Height());
        RenderEngine->DrawImageX(pDC, m_pImgTranBkg, rcfDst, 0, 0, 
            (REAL)m_pImgTranBkg->GetWidth(), (REAL)m_pImgTranBkg->GetHeight());
    }
}

void CCalendarDlg::DrawDate(CDC *pDC)
{
    // �����ڱ���
    RenderEngine->DrawColor(pDC, m_rcDate, FUi::White);
    for (int i = 1; i < DATE_ROW_MAX; i++)
    {
        CRect rcLine(m_rcDate);
        rcLine.top = m_rcDate.top + m_rcDate.Height() * i / DATE_ROW_MAX;
        rcLine.bottom = rcLine.top;
        RenderEngine->DrawLine(pDC, rcLine, 1, COLOR_LINE_NORMAL, PS_SOLID);
    }

    for (int i = 1; i < DATE_COL_MAX; i++)
    {
        CRect rcLine(m_rcDate);
        rcLine.left = m_rcDate.left + m_rcDate.Width() * i / DATE_COL_MAX;
        rcLine.right = rcLine.left;
        RenderEngine->DrawLine(pDC, rcLine, 1, COLOR_LINE_NORMAL, PS_SOLID);
    }

    // ���������ֺͱ߿�
    CFont* pOldFont = NULL;
    for (int i = 0; i < DATE_ROW_MAX; i++)
    {
        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            CRect rcText(m_strArrDate[i][j].rect);

            if (DATE_STATE_SELECT & m_strArrDate[i][j].nState)
            {
                RenderEngine->DrawColor(pDC, rcText, COLOR_SELECT);
                CString strText;
                strText.Format(_T("%d"), m_strArrDate[i][j].nDate);
                pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
                RenderEngine->DrawTextX(pDC, strText, rcText, FUi::White, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                if (DATE_STATE_TODAY & m_strArrDate[i][j].nState)
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

            if (DATE_STATE_HOVER & m_strArrDate[i][j].nState)
            {
                RenderEngine->DrawRect(pDC, rcText, 1, COLOR_LINE_HOVER);
            }

            if (DATE_STATE_UNUSE & m_strArrDate[i][j].nState)
            {
                CString strText;
                strText.Format(_T("%d"), m_strArrDate[i][j].nDate);
                pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
                RenderEngine->DrawTextX(pDC, strText, rcText, COLOR_TEXT_UNUSE, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            else if(DATE_STATE_TODAY & m_strArrDate[i][j].nState)
            {
                CString strText;
                strText.Format(_T("%d"), m_strArrDate[i][j].nDate);
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
                strText.Format(_T("%d"), m_strArrDate[i][j].nDate);
                pOldFont = pDC->SelectObject(RenderEngine->GetFont(SONG_12_N));
                RenderEngine->DrawTextX(pDC, strText, rcText, COLOR_TEXT_NORMAL, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
    }
    pDC->SelectObject(pOldFont);
}

void CCalendarDlg::OnBnClickedBtnOk()
{
    // ��ȷ�������õ�ǰʱ�䣬������Ϣ��������
    SYSTEMTIME timeDtctrl;
    m_pdtTime->GetTime(&timeDtctrl);
    m_timeSelect.wHour = timeDtctrl.wHour;
    m_timeSelect.wMinute = timeDtctrl.wMinute;
    m_timeSelect.wSecond = timeDtctrl.wSecond;

    ::PostMessage(m_pParent->GetSafeHwnd(), WM_CALDLG_CLOSE, WPARAM_SELECT, NULL);
}

void CCalendarDlg::OnBnClickedBtnPreMonth()
{
    if (1 == m_nCurMonth)
    {
        m_nCurYear--;
        m_nCurMonth = 12;
    }
    else
    {
        m_nCurMonth--;
    }

    UpdataDate(m_nCurYear, m_nCurMonth, m_timeToday.wDay);
    InvalidateRect(m_rcDate);

    CString strYear, strMonth;
    strYear.Format(_T("%d��"), m_nCurYear);
    strMonth.Format(_T("%d��"), m_nCurMonth);
    m_pbtnYear->SetWindowText(strYear);
    m_pbtnMonth->SetWindowText(strMonth);

    m_ptSelect = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
    m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
}

void CCalendarDlg::OnBnClickedBtnNextMonth()
{
    if (12 == m_nCurMonth)
    {
        m_nCurYear++;
        m_nCurMonth = 1;
    }
    else
    {
        m_nCurMonth++;
    }

    UpdataDate(m_nCurYear, m_nCurMonth, m_timeToday.wDay);
    InvalidateRect(m_rcDate);

    CString strYear, strMonth;
    strYear.Format(_T("%d��"), m_nCurYear);
    strMonth.Format(_T("%d��"), m_nCurMonth);
    m_pbtnYear->SetWindowText(strYear);
    m_pbtnMonth->SetWindowText(strMonth);

    m_ptSelect = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
    m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
}

void CCalendarDlg::OnBnClickedBtnYear()
{
    SetEnable(FALSE, TRUE);

    CreateSelWnd(SWS_YEAR);

    Invalidate();
}

void CCalendarDlg::OnBnClickedBtnMonth()
{
    SetEnable(FALSE, TRUE);

    CreateSelWnd(SWS_MONTH);

    Invalidate();
}

void CCalendarDlg::OnBnClickedBtnToday()
{
    SYSTEMTIME curTime;
    GetLocalTime(&curTime);
    UpdataDate(curTime.wYear, curTime.wMonth, curTime.wDay);

    CString strYear, strMonth;
    strYear.Format(_T("%d��"), m_nCurYear);
    strMonth.Format(_T("%d��"), m_nCurMonth);
    m_pbtnYear->SetWindowText(strYear);
    m_pbtnMonth->SetWindowText(strMonth);

    m_ptSelect = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
    m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);

    if (m_pCalselWnd)
    {
        SetEnable(TRUE, TRUE);
        Invalidate();
        delete m_pCalselWnd;
        m_pCalselWnd = NULL;
    }
    else
    {
        InvalidateRect(m_rcDate);
    }
}

void CCalendarDlg::OnBnClickedBtnCurtime()
{
    SYSTEMTIME curTime;
    GetLocalTime(&curTime);
    UpdataDate(curTime.wYear, curTime.wMonth, curTime.wDay);

    CString strYear, strMonth;
    strYear.Format(_T("%d��"), m_nCurYear);
    strMonth.Format(_T("%d��"), m_nCurMonth);
    m_pbtnYear->SetWindowText(strYear);
    m_pbtnMonth->SetWindowText(strMonth);
	
	GetLocalTime(&m_timeSelect);
    m_pdtTime->SetTime(curTime);

    m_ptSelect = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
    m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);

    if (m_pCalselWnd)
    {
        SetEnable(TRUE, TRUE);
        Invalidate();
        delete m_pCalselWnd;
        m_pCalselWnd = NULL;
    }
    else
    {
        InvalidateRect(m_rcDate);
    }
}

void CCalendarDlg::SetStyle(int nStyle)
{
    m_nStyle = nStyle;
    if (!m_isInit)
    {
        return;
    }

    if (CS_DATE == m_nStyle)
    {
        m_pbtnToday->ShowWindow(SW_SHOW);
        m_pbtnCurTime->ShowWindow(SW_HIDE);
        m_pbtnOk->ShowWindow(SW_HIDE);
        m_pdtTime->ShowWindow(SW_HIDE);
    }
    else if(CS_DATETIME == m_nStyle)
    {
        m_pbtnToday->ShowWindow(SW_HIDE);
        m_pbtnCurTime->ShowWindow(SW_SHOW);
        m_pbtnOk->ShowWindow(SW_SHOW);
        m_pdtTime->ShowWindow(SW_SHOW);
    }
}

void CCalendarDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // ���ڲ�����ʱ��������������������Ϣ
    if (!m_bEnable && m_rcDate.PtInRect(point))
    {
        return;
    }

    // ע������뿪��Ϣ����ͣ��Ϣ
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
    mouseEvent.hwndTrack = GetSafeHwnd();
    mouseEvent.dwHoverTime = 1;
    _TrackMouseEvent(&mouseEvent);

    CWnd::OnMouseMove(nFlags, point);
}

void CCalendarDlg::OnMouseHover(UINT nFlags, CPoint point)
{
    // ��겻�����������ڣ������ͣ״̬������
    if (!m_rcDate.PtInRect(point))
    {
        if (m_ptHover != CPoint(DATE_ROW_MAX, DATE_COL_MAX))
        {
            m_strArrDate[m_ptHover.x][m_ptHover.y].nState &= 0xFB;
            InvalidateRect(m_strArrDate[m_ptHover.x][m_ptHover.y].rect);
            m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);
        }

        CWnd::OnMouseHover(nFlags, point);
        return;
    }

    // ��������ͣ����һ��
    for (int i = 0; i < DATE_ROW_MAX; i++)
    {
        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            if (!m_strArrDate[i][j].rect.PtInRect(point))
            {
                continue;
            }

            // ��ͣ��ı䣬ˢ����ͣ���ԭ������ͣ��
            if (m_ptHover != CPoint(i, j))
            {
                m_strArrDate[i][j].nState |= DATE_STATE_HOVER;
                m_strArrDate[m_ptHover.x][m_ptHover.y].nState &= 0xFB;
                InvalidateRect(m_strArrDate[i][j].rect);
                InvalidateRect(m_strArrDate[m_ptHover.x][m_ptHover.y].rect);
            }

            m_ptHover = CPoint(i, j);
            CWnd::OnMouseHover(nFlags, point);
            return;
        }
    }

    CWnd::OnMouseHover(nFlags, point);
}

void CCalendarDlg::OnMouseLeave()
{
    // ����뿪��ȡ����ͣ״̬
    m_strArrDate[m_ptHover.x][m_ptHover.y].nState &= 0xFB;
    InvalidateRect(m_strArrDate[m_ptHover.x][m_ptHover.y].rect);
    m_ptHover = CPoint(DATE_ROW_MAX, DATE_COL_MAX);

    CWnd::OnMouseLeave();
}

void CCalendarDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // ������ѡ�񴰿�ʱ�ر�֮
    if (m_pCalselWnd)
    {
        SetEnable(TRUE, TRUE);
        Invalidate();
        delete m_pCalselWnd;
        m_pCalselWnd = NULL;
        return;
    }

    // ���ڲ�����ʱ��������������������Ϣ
    if (!m_bEnable && m_rcDate.PtInRect(point))
    {
        return;
    }

    if (!m_rcDate.PtInRect(point))
    {
        CWnd::OnLButtonDown(nFlags, point);
        return;
    }

    // ������������һ��
    for (int i = 0; i < DATE_ROW_MAX; i++)
    {
        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            if (!m_strArrDate[i][j].rect.PtInRect(point))
            {
                continue;
            }

            if (m_ptSelect != CPoint(i, j))
            {
                if (!(m_strArrDate[i][j].nState & DATE_STATE_UNUSE))
                {
                    m_strArrDate[i][j].nState |= DATE_STATE_SELECT;
                    m_strArrDate[m_ptSelect.x][m_ptSelect.y].nState &= 0xEF;
                    InvalidateRect(m_strArrDate[i][j].rect);
                    InvalidateRect(m_strArrDate[m_ptSelect.x][m_ptSelect.y].rect);

                    // ��¼ѡ�е�ʱ��
                    m_timeSelect.wYear = (WORD)m_nCurYear;
                    m_timeSelect.wMonth = (WORD)m_nCurMonth;
                    m_timeSelect.wDay = (WORD)m_strArrDate[i][j].nDate;
                }
                m_ptSelect = CPoint(i, j);
            }

            CWnd::OnLButtonDown(nFlags, point);
            return;
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CCalendarDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // ���ڲ�����ʱ��������������������Ϣ
    if (!m_bEnable && m_rcDate.PtInRect(point))
    {
        return;
    }

    if (!m_rcDate.PtInRect(point))
    {
        CWnd::OnLButtonUp(nFlags, point);
        return;
    }

    // �����ʽΪ��ʱ�䡢���ſ�ʱ������������
    // ����ѡ����ʱ���رմ���
    if (CS_DATE == m_nStyle && m_ptSelect != CPoint(DATE_ROW_MAX, DATE_COL_MAX))
    {
        ::PostMessage(m_pParent->GetSafeHwnd(), WM_CALDLG_CLOSE, WPARAM_SELECT, NULL);
    }

    // ���������״̬�����ڣ���ת��ǰһ���»��һ����
    if (CS_DATETIME == m_nStyle && (DATE_STATE_UNUSE & m_strArrDate[m_ptSelect.x][m_ptSelect.y].nState))
    {
        if (0 == m_ptSelect.x)
        {
            OnBnClickedBtnPreMonth();
        }
        else
        {
            OnBnClickedBtnNextMonth();
        }
    }

    CWnd::OnLButtonUp(nFlags, point);
}

void CCalendarDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);

    // ʧȥ����ʱ�ر�
    if (WA_INACTIVE == nState)
    {
        ::PostMessage(m_pParent->GetSafeHwnd(), WM_CALDLG_CLOSE, WPARAM_UNSELECT, NULL);
    }
}

void CCalendarDlg::SetSelectTime(const SYSTEMTIME& timeSelect)
{
	memcpy(&m_timeSelect, &timeSelect, sizeof(SYSTEMTIME));
}

void CCalendarDlg::GetSelectTime(SYSTEMTIME& timeSelect)
{
    memcpy(&timeSelect, &m_timeSelect, sizeof(SYSTEMTIME));
}

void CCalendarDlg::SetEnable(BOOL bEnable, BOOL bAllEnable)
{
    m_bEnable = bEnable;
    m_bAllEnable = bAllEnable;

    if (bAllEnable)
    {
        m_pdtTime->EnableWindow(bEnable);
        m_pbtnCurTime->EnableWindow(bEnable);
        m_pbtnOk->EnableWindow(bEnable);
    }
}

void CCalendarDlg::CreateSelWnd(int nStyle)
{
    if (m_pCalselWnd)
    {
        delete m_pCalselWnd;
        m_pCalselWnd = NULL;
    }

    m_pCalselWnd = new CCalSelectWnd;
    m_pCalselWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
        CRect(0, 0, 0, 0), this, ID_WND_SELTIME);
    m_pCalselWnd->SetStyle(nStyle);

    if (SWS_YEAR == nStyle)
    {
        m_pCalselWnd->SetCurYear(m_nCurYear);
    }

    CRect rcClient;
    GetClientRect(&rcClient);

    // ��ͬ��ʽ�Ĵ�С����ʾλ�ò�ͬ
    if (SWS_YEAR == nStyle || SWS_MONTH == nStyle)
    {
        m_pCalselWnd->MoveWindow(rcClient.left, HEADER_HEIGHT, rcClient.Width(), 90);
    }
    else if (SWS_MINI == nStyle || SWS_SECD == nStyle)
    {
        m_pCalselWnd->MoveWindow(rcClient.left, rcClient.bottom - (BOTTOM_HEIGHT + 90) + 1, rcClient.Width(), 90);
    }
    else if (SWS_HOUR == nStyle)
    {
        m_pCalselWnd->MoveWindow(rcClient.left, rcClient.bottom - (BOTTOM_HEIGHT + 154) + 1, rcClient.Width(), 154);
    }

    m_pCalselWnd->ShowWindow(SW_SHOW);
}

LRESULT CCalendarDlg::OnMsgSeltime(WPARAM wParam, LPARAM /*lParam*/)
{
    SetEnable(FALSE, FALSE);
    // ���ݲ�ͬ����Ϣ������ͬ��ʱ��ѡ�񴰿�
    if (ST_HOUR == wParam)
    {
        CreateSelWnd(SWS_HOUR);
    }
    else if (ST_MINI == wParam)
    {
        CreateSelWnd(SWS_MINI);
    }
    else if (ST_SECD == wParam)
    {
        CreateSelWnd(SWS_SECD);
    }

    Invalidate();

     return TRUE;
}

LRESULT CCalendarDlg::OnMsgCalselWndClose(WPARAM wParam, LPARAM lParam)
{
    switch ((int)wParam)
    {
    case SWS_YEAR:
        {
            CString strYear;
            strYear.Format(_T("%d��"), (int)lParam);
            m_pbtnYear->SetWindowText(strYear);

            m_nCurYear = (int)lParam;
            UpdataDate(m_nCurYear, m_nCurMonth, m_timeToday.wDay);
            break;
        }
    case SWS_MONTH:
        {
            CString strMonth;
            strMonth.Format(_T("%d��"), (int)lParam);
            m_pbtnMonth->SetWindowText(strMonth);

            m_nCurMonth = (int)lParam;
            UpdataDate(m_nCurYear, m_nCurMonth, m_timeToday.wDay);
            break;
        }
    case SWS_HOUR:
        {
            SYSTEMTIME time;
            m_pdtTime->GetTime(&time);
            time.wHour = (WORD)lParam;
            m_pdtTime->SetTime(time);
            break;
        }
    case SWS_MINI:
        {
            SYSTEMTIME time;
            m_pdtTime->GetTime(&time);
            time.wMinute = (WORD)lParam;
            m_pdtTime->SetTime(time);
            break;
        }
    case SWS_SECD:
        {
            SYSTEMTIME time;
            m_pdtTime->GetTime(&time);
            time.wSecond = (WORD)lParam;
            m_pdtTime->SetTime(time);
            break;
        }
    default:
        break;
    }

    SetEnable(TRUE, TRUE);
    Invalidate();
    if (m_pCalselWnd)
    {
        delete m_pCalselWnd;
        m_pCalselWnd = NULL;
    }

    return TRUE;
}

/*--���ڼ���--*/

int CCalendarDlg::GetDaysOfMonth(int nYear, int nMonth)
{
    switch (nMonth)
    {
    case 2:
        {
            if (IsLeapYear(nYear))
            {
                return 29;
            }
            return 28;
        }
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        return 31;
    }
}

bool CCalendarDlg::IsLeapYear(int nYear)
{
    if (0 == nYear % 4 && 0 != nYear % 100)
    {
        return true;
    }

    if (0 == nYear % 400)
    {
        return true;
    }

    return false;
}

void CCalendarDlg::UpdataDate(int nYear, int nMonth, int nToday)
{
    m_nCurYear = nYear;
    m_nCurMonth = nMonth;
    for (int i = 0; i < DATE_ROW_MAX; i++)
    {
        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            m_strArrDate[i][j].nDate = 0;
        }
    }

    int monthDays = GetDaysOfMonth(nYear, nMonth);
    bool firstTime = true;
    int nWeekNum = 0;    // ���µĵڼ�������
    for (int i = 1; i <= monthDays; ++i)
    {
        COleDateTime time;
        time.SetDate(nYear, nMonth, i);
        int nWeekDay = time.GetDayOfWeek();

        // �����һ�λ�ȡ�ڼ������1���ʹӵڶ��п�ʼ
        if (1 == nWeekDay && firstTime)
        {
            nWeekNum++;
        }
        m_strArrDate[nWeekNum][nWeekDay - 1].nDate = i;

        m_strArrDate[nWeekNum][nWeekDay - 1].nState = DATE_STATE_NORMAL;
        if (i == nToday && nYear == m_timeToday.wYear
            && nMonth == m_timeToday.wMonth)
        {
            m_strArrDate[nWeekNum][nWeekDay - 1].nState = DATE_STATE_TODAY;
        }

        if (7 == nWeekDay)
        {
            nWeekNum++;
        }

        firstTime = false;
    }

    // �����ϸ��µ���������䱾������ǰ�Ŀհ�
    int preMonthDays = GetDaysOfMonth(nYear, nMonth - 1);
    for (int i = DATE_COL_MAX - 1; i >= 0; i--)
    {
        if (0 == m_strArrDate[0][i].nDate)
        {
            m_strArrDate[0][i].nDate = preMonthDays--;
            m_strArrDate[0][i].nState = DATE_STATE_UNUSE;
        }
    }

    // ��䱾�����ں�Ŀհ�
    int nextMonthDay = 1;
    for(int i = DATE_ROW_MAX - 2; i < DATE_ROW_MAX; i++)
    {
        for (int j = 0; j < DATE_COL_MAX; j++)
        {
            if (0 == m_strArrDate[i][j].nDate)
            {
                m_strArrDate[i][j].nDate = nextMonthDay++;
                m_strArrDate[i][j].nState = DATE_STATE_UNUSE;
            }
        }
    }
}

/*--���ڼ���end--*/
