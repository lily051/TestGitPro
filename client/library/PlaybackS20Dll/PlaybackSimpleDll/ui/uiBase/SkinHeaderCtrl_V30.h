/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinHeaderCtrl_V30.h
*   Description:    the header control for list control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
        2.0         2010-04-08      zhiwenhuan      modify  增加了排序功能的小箭头，
		但是由于响应ONPAINT消息，掩盖了小箭头
**************************************************************************/

#pragma once


// CSkinHeaderCtrl_V30

class CSkinHeaderCtrl_V30 : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CSkinHeaderCtrl_V30)

public:
	CSkinHeaderCtrl_V30();
	virtual ~CSkinHeaderCtrl_V30();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT );
	afx_msg void OnPaint();
	//CBitmap m_bitmapBkgnd;
protected:
	virtual void PreSubclassWindow();

		virtual void Serialize(CArchive& ar);

	CFont m_headerFont;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetSortArrow( const int iColumn, const BOOL bAscending );
	int m_iSortColumn;
	BOOL m_bSortAscending;
};


