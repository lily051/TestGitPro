/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       AppLogo.h
*   Description:    the control to show the application name and version
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/
//lint -library

#pragma once

// CAppLogo

class CAppLogo : public CStatic
{
	DECLARE_DYNAMIC(CAppLogo)

public:
	CAppLogo();
	virtual ~CAppLogo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
    CString m_logoName;
};


