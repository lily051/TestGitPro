/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       SkinDialog.h
*   Description:    a skin dialog has a background bitmap
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

//lint -library

#pragma once


// CSkinDialog dialog

class CSkinDialog : public CDialog
{
	DECLARE_DYNAMIC(CSkinDialog)

public:
	//CSkinDialog(CWnd* pParent = NULL);   // standard constructor
	CSkinDialog(UINT nIDTemplate, CWnd * pParent = NULL);
	virtual ~CSkinDialog();

	DECLARE_MESSAGE_MAP()

protected:
	CDC m_dcMem;
	CBitmap m_bitmap;
    CBitmap m_scaleBitmap;
public:

	void LoadSkin(UINT nIDResource);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void TransparentBkgnd(CDC * pChildDC, CWnd * pChildWnd);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	BOOL m_bExclude;
};
