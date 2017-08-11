// WaitWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WaitWnd.h"
#include "FMemDCEx.h"
#include "fuilib.h"

// �ؼ���϶
const int CTRL_SPACE = 20;

const int WND_HEIGHT = 80;
const int IMAGE_WIDTH = 32;

unsigned _stdcall WT_UpdateImage(void* pParam)
{
    CWaitWnd* pWnd = (CWaitWnd*)pParam;
    if (NULL == pWnd)
    {
        return 2;
    }

    return pWnd->UpdateImage();
}
// CWaitWnd

IMPLEMENT_DYNAMIC(CWaitWnd, CWnd)

CWaitWnd::CWaitWnd()
: m_pImgWait(NULL)
, m_pImgBkg(NULL)
, m_nImageOffset(0)
, m_hExit(NULL)
, m_hThread(NULL)
{
    m_hExit = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CWaitWnd::~CWaitWnd()
{
}

BEGIN_MESSAGE_MAP(CWaitWnd, CWnd)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CWaitWnd ��Ϣ�������



BOOL CWaitWnd::CreateWnd(CWnd* pParent)
{
    m_pImgWait = CResMgr::GetInstance()->GetRes(_T("wait"));
	if (NULL == m_pImgWait)
	{
		return FALSE;
	}
    m_pImgBkg = CResMgr::GetInstance()->GetRes(_T("wait_bkg"));
	if (NULL == m_pImgBkg)
	{
		return FALSE;
	}

    CRect rcImage(0, 0, m_pImgBkg->GetWidth(), m_pImgBkg->GetHeight());
    
    std::vector<int> vecMargin;
    SetMargin(vecMargin, 3);
    SepRect(rcImage, m_vecRcfBkg, vecMargin);

    CRect rcWnd(0, 0, 50, 50);
    int nID = 888;
    DWORD dwStyle = WS_EX_TOOLWINDOW & (~WS_VISIBLE);
    BOOL bRet = CreateEx(dwStyle, NULL, _T("WaitWnd"), WS_CHILD, rcWnd, pParent, nID);

    return bRet;
}

BOOL CWaitWnd::ShowWaitWnd(LPCTSTR szText)
{
    m_strText = szText;

    CDC* pDC = GetDC();
    if (NULL == pDC)
    {
        return FALSE;
    }

    CFont *pFont = RenderEngine->GetFont(YAHEI_20_N);
    CFont *pOld = pDC->SelectObject(pFont);

    CRect rcText(0, 0, 0, 0);
    pDC->DrawText(m_strText, rcText, DT_LEFT| DT_SINGLELINE | DT_CALCRECT);
    pDC->SelectObject(pOld);
    ReleaseDC(pDC);

    // ͼƬ����
    m_rcImage = CRect(24, 24, 24 + IMAGE_WIDTH, 24 + IMAGE_WIDTH);

    // ��������
    int nTopInc = (WND_HEIGHT - rcText.Height()) / 2;
    m_rcText = rcText;
    m_rcText.InflateRect(-(CTRL_SPACE * 2 + IMAGE_WIDTH), -nTopInc, (CTRL_SPACE * 2 + IMAGE_WIDTH), nTopInc);

    // ���ڴ�С
    m_rcWnd = CRect(0, 0, rcText.Width() + CTRL_SPACE * 3 + IMAGE_WIDTH, WND_HEIGHT);

    std::vector<int> vecMargin;
    SetMargin(vecMargin, 3);
    SepRect(m_rcWnd, m_vecRcfWnd, vecMargin);

    MoveWindow(m_rcWnd);
    CenterWindow();
    ShowWindow(SW_SHOW);

    ResetEvent(m_hExit);
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, WT_UpdateImage, this, 0, NULL);

    return TRUE;
}

void CWaitWnd::OnDestroy()
{
    CWnd::OnDestroy();

    if (m_hExit)
    {
        SetEvent(m_hExit);
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        CloseHandle(m_hExit);
    }
}

BOOL CWaitWnd::OnEraseBkgnd(CDC* pDC)
{
    FUi::CMemDCEX memDC(pDC, m_rcWnd);
    
    DrawBkg(&memDC);

    return TRUE;
}

void CWaitWnd::SplashStep(CDC* pDC)
{
    if (NULL == pDC)
    {
        return ;
    }
    if (m_nImageOffset >= 12)
    {
        m_nImageOffset = 0;
    }
    // ��ˢ�±���
    DrawBkg(pDC);

#pragma warning(push)
#pragma warning(disable:4244)
    // �ٻ�ͼƬ
    RectF rcfDst(m_rcImage.left, m_rcImage.top, IMAGE_WIDTH, IMAGE_WIDTH);
    RenderEngine->DrawImageX(pDC, m_pImgWait, rcfDst, m_nImageOffset++ * IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_WIDTH);
#pragma warning(pop)

    // ������
    CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(YAHEI_18_N));
    RenderEngine->DrawTextX(pDC, m_strText, m_rcText, FUi::Black, DT_LEFT | DT_SINGLELINE);
    pDC->SelectObject(pOld);
}

// ˢ�½���
unsigned CWaitWnd::UpdateImage()
{
    while(WAIT_OBJECT_0 != WaitForSingleObject(m_hExit, 50))
    {
        CDC* pDC = GetDC();
        {
            FUi::CMemDCEX memDC(pDC, m_rcWnd);
            SplashStep(&memDC);
        }
        ReleaseDC(pDC);
    }
    return 0;
}

// ������ͼ
void CWaitWnd::DrawBkg(CDC* pDC)
{
    for (size_t i = 0; i < m_vecRcfWnd.size(); ++i)
    {
        RenderEngine->DrawImageX(pDC, m_pImgBkg, m_vecRcfWnd[i], m_vecRcfBkg[i].X, m_vecRcfBkg[i].Y,
            m_vecRcfBkg[i].Width, m_vecRcfBkg[i].Height);
    }
}
