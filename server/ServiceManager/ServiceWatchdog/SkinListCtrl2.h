//lint -library
/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinListCtrl2.h
*   Description:    the list ctrl has an owner draw header control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/
//lint -library

#pragma once

#include "skinheaderctrl.h"


// CSkinListCtrl2

class CSkinListCtrl2 : public CListCtrl
{
	DECLARE_DYNAMIC(CSkinListCtrl2)

public:
	CSkinListCtrl2();
	virtual ~CSkinListCtrl2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSkinHeaderCtrl m_skinHeaderCtrl;
    int m_nItemHeight;
protected:
	virtual void PreSubclassWindow();
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    void SetItemHeight(UINT nHeight);
    void SetHeaderStyle(void);
    afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};


