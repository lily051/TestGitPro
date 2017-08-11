/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinTabCtrl.h
*   Description:    a owner draw tab control
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

#pragma once


// CSkinTabCtrl

class CSkinTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CSkinTabCtrl)

public:
	CSkinTabCtrl();
	virtual ~CSkinTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawTab(CDC * pDC , const CRect & rcItem, BOOL bSelected = FALSE);
	void SetShowInfo(CString &strShowInfo){m_strShowInfo = strShowInfo;}

	CFont m_fontSel;
    CFont m_fontBarText;
    CBitmap m_bitmap;
	CString m_strShowInfo;
	
protected:
	virtual void PreSubclassWindow();
};


