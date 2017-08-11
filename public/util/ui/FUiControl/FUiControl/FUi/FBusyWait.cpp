#include "StdAfx.h"
#include "FBusyWait.h"

CFBusyWait::CFBusyWait(CWnd* pParent)
: m_pWaitWnd(NULL)
, m_pTranslucentWnd(NULL)
{
    // ÕÚµ²´°¿Ú
    if (0)
    {
        m_pTranslucentWnd = new CTranslucentWnd;
        if (m_pTranslucentWnd)
        {
            if (!m_pTranslucentWnd->CreateWnd(CWnd::GetDesktopWindow()))
            {
                TRACE("error");
            }
            SetWindowPos(m_pTranslucentWnd->GetSafeHwnd(), HWND_TOPMOST, 0, 0 , 0, 0, SWP_NOSIZE);

            CRect rcParent;
            pParent->GetWindowRect(rcParent);
            //int nFrameWidth = GetSystemMetrics(SM_CXFRAME) - 2;
            int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
            rcParent.top += nCaptionHeight;

            m_pTranslucentWnd->MoveWindow(rcParent);
        }
    }

    // µÈ´ý´°¿Ú
    m_pWaitWnd = new CWaitWnd;
    if (m_pWaitWnd)
    {
        if (!m_pWaitWnd->CreateWnd(CWnd::GetDesktopWindow()))
        {
            TRACE("error");
			m_pWaitWnd->DestroyWindow();
			delete m_pWaitWnd;
			m_pWaitWnd = NULL;
			return ;
        }
        SetWindowPos(m_pWaitWnd->GetSafeHwnd(), HWND_TOPMOST, 0, 0 , 0, 0, SWP_NOSIZE);
    }
}

CFBusyWait::~CFBusyWait(void)
{
    try
    {
        if (m_pWaitWnd)
        {
            m_pWaitWnd->DestroyWindow();
            delete m_pWaitWnd;
            m_pWaitWnd = NULL;
        }

        if (m_pTranslucentWnd)
        {
            m_pTranslucentWnd->DestroyWindow();
            delete m_pTranslucentWnd;
            m_pTranslucentWnd = NULL;
        }
    }
    catch (...)
    {
    }
}

void CFBusyWait::ShowBusyWait(LPCTSTR szText)
{
    if (m_pWaitWnd)
    {
        m_pWaitWnd->ShowWaitWnd(szText);
    }
}
