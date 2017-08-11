#pragma once

// CLocalWndView

class CLocalWndView : public CWnd
{
	DECLARE_DYNAMIC(CLocalWndView)

public:
	CLocalWndView();
	virtual ~CLocalWndView();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

protected:
	// 背景色.
	COLORREF m_clrBk;  

private:
	// 是否在播放中
	BOOL m_bIsPlaying;
	// 是否在暂停中
	BOOL m_bIsPausing;
	// 音频是否开启中
	BOOL m_bIsSounding;
	// 是否全屏中
	BOOL m_bIsFullScreening;
	// 右键弹出菜单
	void PopupMenu(BOOL bJustClose = FALSE);
	// 执行菜单
	void ExcuteSelectOption(int nCmd);

public:
	// 修改播放状态
	void UpdatePlayStatus(BOOL bPlay);
	// 修改菜单状态
	void UpdateMenuStatus(int nBtnType, BOOL bStatus);

};


