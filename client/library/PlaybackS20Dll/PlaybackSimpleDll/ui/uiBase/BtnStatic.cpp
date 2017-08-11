//lint -library
#include "stdafx.h"
#include "./BtnStatic.h"


#define STATIC_BK_COLOR RGB(175, 175, 175)
#define STATIC_BKON_COLOR RGB(150, 150, 150)
#define STATIC_BKSEL_COLOR RGB(125, 125, 125)
#define STATIC_TEXT_COLOR RGB(23, 23, 23)


//IMPLEMENT_DYNAMIC(CBtnStatic, CStatic)
CBtnStatic::CBtnStatic()
{
    m_pImage = NULL;
    m_pImageSel = NULL;
    m_bHove = FALSE;
    m_bMouseTrack = FALSE;
    m_bSelect = FALSE;

    LOGFONT lf = {0};
    lf.lfWeight = FW_NORMAL;
    lf.lfHeight = 14;
    ::lstrcpy(lf.lfFaceName, L"Arial");
    m_font.DeleteObject();
    m_font.CreateFontIndirect(&lf);

    LOGFONT lfSel = {0};
    lfSel.lfWeight = FW_SEMIBOLD;
    lfSel.lfHeight = 14;
    ::lstrcpy(lfSel.lfFaceName, L"Arial");
    m_fontSel.DeleteObject();
    m_fontSel.CreateFontIndirect(&lfSel);;

}

CBtnStatic::~CBtnStatic()
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }

    if (m_pImageSel)
    {
        delete m_pImageSel;
        m_pImageSel = NULL;
    }

}

void CBtnStatic::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码

    DrawItem(&dc);


    // 不为绘图消息调用 CStatic::OnPaint()
}

void CBtnStatic::DrawItem(CDC * pDC)
{

    if (m_pImage == NULL)
    {
        return;
    }

    CRect rcStatic;
    GetClientRect(&rcStatic);

//     HRGN hRgn = CreateRoundRectRgn(rcStatic.left, rcStatic.top, rcStatic.right, rcStatic.bottom, 5, 5);
//     CRgn * pRgn = CRgn::FromHandle(hRgn);
//     CBrush bkBrush;
//     bkBrush.DeleteObject();

    int nWidth = rcStatic.Width();
    int nHeight = rcStatic.Height();

    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    memDC.SelectObject(&memBitmap);

    Graphics graph(memDC.m_hDC);

    CFont font;

    //Draw pic
    Image * pImage = m_pImage;

    if (m_bSelect)
    {
//         bkBrush.CreateSolidBrush(STATIC_BKSEL_COLOR);
//         memDC.FillRgn(pRgn, &bkBrush);
        memDC.FillSolidRect(rcStatic, STATIC_BKSEL_COLOR);

        if (m_pImageSel)
        {
            pImage = m_pImageSel;
        }
        LOGFONT lf = {0};
        lf.lfWeight = FW_SEMIBOLD;
        lf.lfHeight = 14;
        ::lstrcpy(lf.lfFaceName, L"Arial");
        font.DeleteObject();
        font.CreateFontIndirect(&lf);
    }
    else
    {
        if (m_bHove)
        {
//             bkBrush.CreateSolidBrush(STATIC_BKON_COLOR);
//             memDC.FillRgn(pRgn, &bkBrush);
            memDC.FillSolidRect(rcStatic, STATIC_BKON_COLOR);

            if (m_pImageSel)
            {
                pImage = m_pImageSel;
            }
            LOGFONT lf = {0};
            lf.lfWeight = FW_SEMIBOLD;
            lf.lfHeight = 14;
            ::lstrcpy(lf.lfFaceName, L"Arial");
            font.DeleteObject();
            font.CreateFontIndirect(&lf);
        }
        else
        {
//             bkBrush.CreateSolidBrush(STATIC_BK_COLOR);
//             memDC.FillRgn(pRgn, &bkBrush);
            memDC.FillSolidRect(rcStatic, STATIC_BK_COLOR);

            LOGFONT lf = {0};
            lf.lfWeight = FW_NORMAL;
            lf.lfHeight = 14;
            ::lstrcpy(lf.lfFaceName, _T("Arial"));
            font.DeleteObject();
            font.CreateFontIndirect(&lf);
        }
    }

	Rect rcImage( 4, (nHeight-pImage->GetHeight())/2, nWidth, nHeight);

    if (pImage)
    {
        graph.DrawImage(pImage, rcImage, 0, 0, nWidth, nHeight, UnitPixel);
    }

    //Draw text
    CString strText;
    GetWindowText(strText);
    if (!strText.IsEmpty())
    {
        memDC.SelectObject(font);
        memDC.SetTextColor(STATIC_TEXT_COLOR);
        CRect rcText(pImage->GetWidth()+ 12, 0, nWidth, nHeight);
        memDC.DrawText(strText, rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    }

    pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);

}

BEGIN_MESSAGE_MAP(CBtnStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

LRESULT CBtnStatic::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    m_bHove = TRUE;

    Invalidate();

    return 0;
}

LRESULT CBtnStatic::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    m_bHove = FALSE;

    m_bMouseTrack = FALSE;

    Invalidate();

    return 0;
}

void CBtnStatic::PreSubclassWindow()
{
    // TODO: 在此添加专用代码和/或调用基类
    
    //SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetStyle()|SS_OWNERDRAW);

    ModifyStyle(0, SS_OWNERDRAW, TRUE);

    CStatic::PreSubclassWindow();
}

void CBtnStatic::Load(UINT nID, UINT nIDSel)
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }
    m_pImage = LoadImageFromResource(nID);

    if (m_pImageSel)
    {
        delete m_pImageSel;
        m_pImageSel = NULL;
    }
    m_pImageSel = LoadImageFromResource(nIDSel);

}

void CBtnStatic::LoadImage(Image * pImage, Image * pImageSel)
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }
    m_pImage = pImage;

    if (m_pImageSel)
    {
        delete m_pImageSel;
        m_pImageSel = NULL;
    }
    m_pImageSel = pImageSel;

}

//设置static是否处于选中状态
void CBtnStatic::SetStaticSel(BOOL bSelect)
{
    m_bSelect = bSelect;
    Invalidate();
}

void CBtnStatic::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    if (!m_bMouseTrack)
    {
        Invalidate();

        TRACKMOUSEEVENT eventTrack;
        eventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
        eventTrack.hwndTrack = m_hWnd;
        eventTrack.dwFlags = TME_LEAVE | TME_HOVER;
        eventTrack.dwHoverTime = 1;
        m_bMouseTrack = ::_TrackMouseEvent(&eventTrack);
    }

    CStatic::OnMouseMove(nFlags, point);
}

