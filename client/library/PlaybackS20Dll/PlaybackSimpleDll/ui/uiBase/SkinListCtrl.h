/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl.cpp
*   Description:    the list ctrl has an owner draw header control
*   Author:         Qimengxing (Qimx@hikvision.com)
*   Date:           2010-4-12
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2010-4-12      Qimengxing       created
**************************************************************************/


#pragma once

#include "HeaderCtrlEx.h"

#ifdef _DEBUG
#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG

// CSkinListCtrl

class CSkinListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSkinListCtrl)

public:
	CSkinListCtrl(UINT nitemHeight = 31, int nShowProSubItem = -1);
	virtual ~CSkinListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CHeaderCtrlEx m_skinHeaderCtrl;
protected:
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void Init();

	int m_iNumColumns;
	BOOL m_bSortAscending;
	int m_nShowProSubItem;//显示进度列，如果是-1表示不显示进度
	UINT m_nitemHeight;   //行高
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetItemHeight(UINT nHeight);
};


