/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       AppStat.h
*   Description:    the control to show the application's statistics
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

#pragma once

#include "WorkThreadBase.h"


// CAppStat

class CAppStat : public CStatic
{
	DECLARE_DYNAMIC(CAppStat)

public:
	CAppStat();
	virtual ~CAppStat();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	//
    int m_nDays;
	int m_nHours;
	int m_nMinutes;
	int m_nSeconds;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

    // 设置皮肤的种类
    void SetFlags(BOOL bFlags);

private:
    BOOL m_bIsBlackAndRed;

protected:
	virtual void PreSubclassWindow();
};


