/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinHeaderCtrl.h
*   Description:    the header control for list control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/
//lint -library

#pragma once


// CSkinHeaderCtrl

class CSkinHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CSkinHeaderCtrl)

public:
	CSkinHeaderCtrl();
	virtual ~CSkinHeaderCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnPaint();

	CBitmap m_bitmapBkgnd;
protected:
	virtual void PreSubclassWindow();

	CFont m_headerFont;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
};


