#pragma once

/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			CustomDrawBlackWnd.h
Description:  		自画窗口
					1.黑色背景
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-3-26     qmx    创建
*****************************************************/
// CCustomDrawGrayWnd

class CCustomDrawBlackWnd : public CWnd
{
	DECLARE_DYNAMIC(CCustomDrawBlackWnd)

public:
	CCustomDrawBlackWnd();
	virtual ~CCustomDrawBlackWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	CBrush m_bakeBrush;
	void Wnd_SetBakeColor(COLORREF crBakeColor);
	virtual void Wnd_DrawBK(CDC * pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


