/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       AppStat.cpp
*   Description:    the control to show the application's statistics
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

// AppStat.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "AppStat.h"
#include "MemDC.h"
#include "MainDialog.h"
#include "utils.h"
//#include "Lang/MultiLanRes.h"

// CAppStat

IMPLEMENT_DYNAMIC(CAppStat, CStatic)
CAppStat::CAppStat()
: m_nHours(0)
, m_nMinutes(0)
, m_nSeconds(0)
, m_nDays(0)
, m_bIsBlackAndRed(FALSE)
{

}

CAppStat::~CAppStat()
{
}


BEGIN_MESSAGE_MAP(CAppStat, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
//	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CAppStat message handlers


void CAppStat::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CMemDC memDC(&dc, this);
	CDC * pMemDC = memDC.GetDC();

	if (GetParent()->IsKindOf(RUNTIME_CLASS(CMainDialog)))
	{
		((CSkinDialog *)GetParent())->TransparentBkgnd(pMemDC, this);
	}

	Graphics graph(pMemDC->m_hDC);

	FontFamily fontFamily(L"Arial");

	
	Font font(&fontFamily, 11, FontStyleRegular, UnitPixel);

    // ¼ÓÔØ×ÊÔ´
    wchar_t szRunTimeBuff[MAX_PATH] = {0};
    MultiLanRes_LoadStringW(IDS_RUNTIME, szRunTimeBuff, _countof(szRunTimeBuff));

    wchar_t szDayBuff[MAX_PATH] = {0};
    MultiLanRes_LoadStringW(IDS_DAY, szDayBuff, _countof(szDayBuff));

	// the statistics
    CStringW strInfo;

    if (m_nDays >= 1)
    {
        strInfo.Format(L"%s %d %s %02d : %02d : %02d", szRunTimeBuff, m_nDays, szDayBuff, m_nHours, m_nMinutes, m_nSeconds);
    }
    else
    {
        strInfo.Format(L"%s %02d : %02d : %02d", szRunTimeBuff, m_nHours, m_nMinutes, m_nSeconds);
    }

    if (TRUE == m_bIsBlackAndRed)
    {
        SolidBrush solidBrush(Color(255, 169, 162, 162));
        graph.DrawString(strInfo.GetBuffer(0), strInfo.GetLength(), &font, PointF(0, 17), &solidBrush);
    }
    else
    {
        SolidBrush solidBrush(Color(255, 64, 0, 0));
	    graph.DrawString(strInfo.GetBuffer(0), strInfo.GetLength(), &font, PointF(0, 17), &solidBrush);
    }

	// Do not call CStatic::OnPaint() for painting messages
}

void CAppStat::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	COleDateTimeSpan span = COleDateTime::GetCurrentTime() - theApp.m_startTime;

    m_nDays = span.GetDays();
	m_nHours = span.GetHours();
	m_nMinutes = span.GetMinutes();
	m_nSeconds = span.GetSeconds();

	Invalidate();

	CStatic::OnTimer(nIDEvent);
}

void CAppStat::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: Add your message handler code here

	KillTimer(0);
}

void CAppStat::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	SetTimer(0, 500, NULL);

	CStatic::PreSubclassWindow();
}

void CAppStat::SetFlags(BOOL bFlags)
{
    m_bIsBlackAndRed = bFlags;
}
