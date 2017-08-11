/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinDialog.cpp
*   Description:    a skin dialog has a background bitmap
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// SkinDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "SkinDialog.h"
#include "utils.h"
#include "MainDialog.h"


// CSkinDialog dialog

IMPLEMENT_DYNAMIC(CSkinDialog, CDialog)
//CSkinDialog::CSkinDialog(CWnd* pParent /*=NULL*/)
////    : CDialog(CSkinDialog::IDD, pParent)
//{
//}

CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParent /* = NULL */)
: CDialog(nIDTemplate, pParent)
{
    m_bExclude = FALSE;
}

CSkinDialog::~CSkinDialog()
{
}

BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CSkinDialog message handlers

BOOL CSkinDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    m_bExclude = FALSE;
    CRect rcClient;
    GetClientRect(rcClient);

    SetRegion(this, m_bitmap);

    GetClientRect(rcClient);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSkinDialog::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
//     if (m_bExclude)
//     {
//         CRect rcExclude(227, 39, 754, 496);
//         pDC->ExcludeClipRect(&rcExclude);    // the rect will not draw
//     }

    if (m_dcMem.m_hDC == NULL)
    {
        CDC dc;
        dc.CreateCompatibleDC(pDC);
        CBitmap *pOldBitmap = dc.SelectObject(&m_bitmap);

        CRect rc;
        GetClientRect(rc);

        BITMAP bitmap;
        m_bitmap.GetBitmap(&bitmap);
        m_scaleBitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());

        m_dcMem.CreateCompatibleDC(pDC);
        //m_dcMem.SetStretchBltMode(pDC->GetStretchBltMode());

        m_dcMem.SelectObject(&m_scaleBitmap);
        m_dcMem.StretchBlt(0, 0, rc.Width(), rc.Height(), 
            &dc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

        dc.SelectObject(pOldBitmap);
    }

    CRect rc;
    GetClientRect(&rc);

    pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &m_dcMem, 0, 0, SRCCOPY);

    return TRUE;

    //return CDialog::OnEraseBkgnd(pDC);
}

/**************************************************************************
*   Function:        LoadSkin
*   Description:    load dialog background by resource id 
*   Input:          UINT nIDResource: the resource id
*   Output:         (null)
*   Return:            return void
**************************************************************************/
void CSkinDialog::LoadSkin(UINT nIDResource)
{
    // load skin resource form PNG
    //
    // 2010-1-21 10:09:05 Hu Tonghai Remarks:
    //     原采用 PNG 图片格式，现改为 BITMAP 格式。
    //
    
//     Bitmap * pBitmap = ::LoadBitmapFromResource(nIDResource);
// 
//     HBITMAP hBitmap = NULL;
// 
//     pBitmap->GetHBITMAP(Color(0, 255, 0, 255), &hBitmap);
// 
//     // convert to CBitmap
//     m_bitmap.Attach(hBitmap);
// 
//     delete pBitmap;
    
    m_bitmap.LoadBitmap(nIDResource);

    // set window position
    
    BITMAP bm;
    m_bitmap.GetBitmap(&bm);
    CRect rc;
    GetClientRect(&rc);
    double wRate = rc.Width() / ((double)bm.bmWidth);
    double hRate = rc.Height() / ((double)bm.bmHeight);
    SetWindowPos(NULL, 0, 0, (int)(bm.bmWidth*wRate), (int)(bm.bmHeight*hRate), SWP_NOMOVE);

}

/**************************************************************************
*   Function:        TransparentBkgnd
*   Description:    if a child want transparent background
*                    must call this function
*   Input:          CDC * pChildDC: the child control dc
*                    CWnd * pChildWnd: the child window pointer
*   Output:         (null)
*   Return:            return void
**************************************************************************/
void CSkinDialog::TransparentBkgnd(CDC * pChildDC, CWnd * pChildWnd)
{
    CRect rc;
    pChildWnd->GetWindowRect(&rc);
    
    ScreenToClient(&rc);

    CClientDC dc(this);

    //pChildDC->BitBlt(0, 0, rc.right - rc.left, rc.bottom - rc.top, &m_dcMem, rc.left, rc.top, SRCCOPY);
    pChildDC->StretchBlt(0, 0, rc.right - rc.left, rc.bottom - rc.top, 
        &m_dcMem, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);


    //pDC->FillSolidRect(0, 0, rc.right - rc.left, rc.bottom - rc.top, RGB(203, 203, 207));
}

HBRUSH CSkinDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (CTLCOLOR_STATIC == nCtlColor)
    {
        /*int nCtrlID = */pWnd->GetDlgCtrlID();
        pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

LRESULT CSkinDialog::OnNcHitTest(CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    UINT uHitTest = CDialog::OnNcHitTest(point);

    if (uHitTest == HTCLIENT)
    {
        uHitTest = HTCAPTION;
    }

    return uHitTest;
}
