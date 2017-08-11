#pragma once

/*****************************************************************
*   Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayVedioWnd.h
*   Description:    回放视频控件类， 仅用于接受视频
*   Author:         yudan
*   Date:			2011-1-30 08:54
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-27 16:33   yudan          create
*****************************************************************/

#define TIMER_SHOW_TEXT       WM_USER + 5001
#define TIMER_SHOW_PIC       WM_USER + 5002
#define TIME_SHOW_VALUE       60

typedef enum _EnumShowType
{
	PLAYVEDIO_SHOWTYPE_NULL = 0,
	PLAYVEDIO_SHOWTYPE_TEXT,
	PLAYVEDIO_SHOWTYPE_PIC,
}EnumShowType;

class CPlayVedioWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayVedioWnd)

public:
	CPlayVedioWnd();
	virtual ~CPlayVedioWnd();

private:
	COLORREF m_clrBk;  //背景色
	COLORREF m_clrText;  //文字颜色
	CFont m_fTextFont;  //字体
	CString m_strText;    //显示的文字
	CString m_strOnWnd;   //单音频播放时的文字
	CRect m_rcClient;	// 客户区大小.
	BOOL m_bShowStatePic;	// 显示状态图标定时器是否正在跑.
	BOOL m_bShowText;		// 显示文字定时器是否正在跑.
	EnumShowType m_emShowType;	// 当前显示方式.
	int m_nState;		// 当前图标状态.
	int m_nTextCount;		// 显示文字计数.

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	//设置显示文字
	void SetTextShow(CString strText, BOOL bErase);
	//设置单音频播放时的文字
	void SetTextOnWnd(const CString &strText) {m_strOnWnd = strText;};
	// 设置显示状态图标.
	void SetStatePicShow();
	// 设置状态图标隐藏.
	void SetStatePicHide();
	// 回调窗口上叠加文字
	void DrawTextOnVedio(CDC *pDC);

private:
	// 写状态文字.
	void DrawStateText(CDC* pDC);
	// 画状态图.
	void DrawStatePic(CDC* pDC);
	// 清空显示区域.
	void DrawCleanRect(CDC* pDC);


public:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
