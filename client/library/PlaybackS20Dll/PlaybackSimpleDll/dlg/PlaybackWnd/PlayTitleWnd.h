#pragma once

/*****************************************************************
*   Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayTitleWnd.h
*   Description:    回放视频控件类， 仅用于接受视频
*   Author:         yudan
*   Date:			2011-1-30 09:10
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-30 09:10   yudan          create
*****************************************************************/


class CPlayTitleWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayTitleWnd)

public:
	CPlayTitleWnd();
	virtual ~CPlayTitleWnd();

private:
	COLORREF m_clrBk;  //背景色
	CString m_strText;  //标题文字
	COLORREF m_clrText;  //标题文字颜色
	CFont m_fTextFont;  //标题文字字体
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
	//设置文本
	void SetText(CString strText);
	//设置标题文本颜色
	void SetTextColour(COLORREF clrText);
	//设置文本字体
	void SetTextFont(CFont* fTextFont);
	//刷新
	void UpdateSurface();
	//获取标题文字
	CString GetText();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
