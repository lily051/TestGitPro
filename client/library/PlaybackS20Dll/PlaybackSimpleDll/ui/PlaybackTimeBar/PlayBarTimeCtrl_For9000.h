#pragma once

#include "PlayBarTimeCtrl_Base.h"
// CPlayBarTimeCtrl_For9000

class CPlayBarTimeCtrl_For9000 : public CPlayBarTimeCtrl_Base
{
	DECLARE_DYNAMIC(CPlayBarTimeCtrl_For9000)

public:

	CPlayBarTimeCtrl_For9000();
	virtual ~CPlayBarTimeCtrl_For9000();

protected:
	void DrawBigImage(CDC *pDC);  // »­´óÍ¼ yxn20091010
	void DrawSelectSection(CDC *pDC);
	void Wnd_DrawBK(CDC * pDC);
	void _InitDC(void);

	void CreateTopBitmap(CDC *pDC);
	void CreateLeftBitmap(CDC *pDC);
	void CreateBackBitmapByElem(CDC *pDC);
private:
	CPen  m_Pen[5];
	CPen m_MinPen;//ÖÐ¼ä±Ê
	CBrush m_Brush[5]; 
	CBitmap m_bmpBackByElement[16];
	CBitmap m_bmpLeft;
	CBitmap m_bmpTop;
 
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


