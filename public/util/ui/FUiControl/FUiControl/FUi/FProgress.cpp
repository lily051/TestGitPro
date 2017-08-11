// FProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "FProgress.h"
#include "FMemDCEx.h"

// CFProgress

IMPLEMENT_DYNAMIC(CFProgress, CProgressCtrl)

CFProgress::CFProgress()
: m_pImgBkg(NULL)
, m_clrText(FUi::White)
, m_strTextFontName(YAHEI_20_N)
{
}

CFProgress::~CFProgress()
{
}


BEGIN_MESSAGE_MAP(CFProgress, CProgressCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CFProgress 消息处理程序
void CFProgress::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (NULL == m_pImgBkg)
    {
        return ;
    }

    CRect rcClient;
    GetClientRect(rcClient);

    FUi::CMemDCEX memDC(&dc, rcClient);

    int nLower = 0, nUpper = 100;
    GetRange(nLower, nUpper);

    int nPos = GetPos() * rcClient.Width() / nUpper;


    RectF rcLeft((REAL)0, (REAL)0, (REAL)nPos, (REAL)rcClient.Height());
    RectF rcRight((REAL)nPos, (REAL)0, (REAL)rcClient.Width(), (REAL)rcClient.Height());

    //lint -e653
    // 进度
    RenderEngine->DrawImageX(&memDC, m_pImgBkg, rcLeft, 0, 0, 
        (REAL)(m_pImgBkg->GetWidth()), (REAL)(m_pImgBkg->GetHeight() / 2 - 1));

    RenderEngine->DrawImageX(&memDC, m_pImgBkg, rcRight, 0, (REAL)(m_pImgBkg->GetHeight() / 2 + 1), 
        (REAL)(m_pImgBkg->GetWidth()), (REAL)(m_pImgBkg->GetHeight() / 2 - 1));
    //lint +e653

    // 写字
    CFont *pOld = memDC->SelectObject(RenderEngine->GetFont(m_strTextFontName));
    int nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_WORD_ELLIPSIS;
    if (m_strText.IsEmpty())
    {
        CString strText;
        strText.Format(_T("%d"), GetPos() * 100 / nUpper);
        strText += _T("%");

        RenderEngine->DrawTextX(&memDC, strText, rcClient, m_clrText, nFormat);
    }
    else
    {
        RenderEngine->DrawTextX(&memDC, m_strText, rcClient, m_clrText, nFormat);
    }
    memDC->SelectObject(pOld);
}

void CFProgress::SetObjectName(LPCTSTR szObjectName)
{
    Image *pImg = CResMgr::GetInstance()->GetRes(szObjectName);
    if (pImg)
    {
        m_pImgBkg = pImg;
        m_strObjectName = szObjectName;
    }
    Invalidate();
}

void CFProgress::SetTextColor(COLORREF clrText)
{
    m_clrText = clrText;
    Invalidate();
}

void CFProgress::SetText(LPCTSTR szText)
{
    m_strText = szText;
    Invalidate();
}

void CFProgress::SetTextFont(const CString& strFontName)
{
    m_strTextFontName = strFontName;
}
