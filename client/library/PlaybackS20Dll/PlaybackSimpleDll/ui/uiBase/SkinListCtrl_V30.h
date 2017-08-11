/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl_V30.h
*   Description:    the list ctrl has an owner draw header control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
        2.0         2010-04-08      zhiwenhuan      modify  增加了排序功能
**************************************************************************/

#pragma once

#include "skinheaderctrl_V30.h"
#ifdef _DEBUG
#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG

// CSkinListCtrl_V30

class CSkinListCtrl_V30 : public CListCtrl
{
	DECLARE_DYNAMIC(CSkinListCtrl_V30)

public:
	CSkinListCtrl_V30();
	virtual ~CSkinListCtrl_V30();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSkinHeaderCtrl_V30 m_skinHeaderCtrl;
protected:
	virtual void PreSubclassWindow();
public:
	//afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void Sort( int iColumn, BOOL bAscending );
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	int m_iNumColumns;
	int m_iSortColumn;
	CImageList * m_pDragImage;
	BOOL m_bSortAscending;

};


