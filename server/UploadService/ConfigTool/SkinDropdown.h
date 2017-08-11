/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinDropdown.h
*   Description:    ip地址控件的历史下拉按钮
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/

#pragma once


// CSkinDropdown

class CSkinDropdown : public CStatic
{
	DECLARE_DYNAMIC(CSkinDropdown)

public:
	CSkinDropdown();
	virtual ~CSkinDropdown();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


