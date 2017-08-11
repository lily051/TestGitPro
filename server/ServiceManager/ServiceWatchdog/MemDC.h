/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       MemDC.h
*   Description:    a simple memory dc lick famous CMemDC
*   Author:         JiangTianjing(jiangtj@hikvision.com)
*   Date:           13/11/2008
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         13/11/2008      JiangTianjing   created
**************************************************************************/


#pragma once

class CMemDC
{
public:
	CMemDC (CDC * pDC, CWnd * pWnd)
		: m_pDC(pDC)
		, m_pOldBitmap(NULL)
	{
		pWnd->GetClientRect(&m_rect);
		m_dcMem.CreateCompatibleDC(pDC);
		m_bitmapMem.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
		m_pOldBitmap = m_dcMem.SelectObject(&m_bitmapMem);
	}

	virtual ~CMemDC()
	{
		CRect rcClip;

		int nClipType = m_pDC->GetClipBox(&rcClip);

		if (nClipType != NULLREGION)
		{
			if (nClipType != SIMPLEREGION)
			{
				rcClip = m_rect;
			}

			m_pDC->BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
				&m_dcMem, rcClip.left, rcClip.top, SRCCOPY);
		}

		m_dcMem.SelectObject(m_pOldBitmap);
	}

	CDC * GetDC() {return &m_dcMem;};

protected:
	CDC * m_pDC;	// the dc to draw
	CDC m_dcMem;	// the memory dc
	CBitmap m_bitmapMem;	// the memory bitmap
	CBitmap * m_pOldBitmap;
	CRect m_rect;
};
