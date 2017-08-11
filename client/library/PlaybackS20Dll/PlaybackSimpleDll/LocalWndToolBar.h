#pragma once
#include "./ui/uiBase/xSkinButton.h"
#include "./ui/uiCtrl/BitmapSlider.h"     

// CLocalWndToolBar

class CLocalWndToolBar : public CWnd
{
	DECLARE_DYNAMIC(CLocalWndToolBar)

public:
	CLocalWndToolBar();
	virtual ~CLocalWndToolBar();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnToolBarBtnClk_Stop();
	afx_msg void OnToolBarBtnClk_Capture();
	afx_msg void OnToolBarBtnClk_FullScreen();
	afx_msg void OnToolBarBtnClk_OpenSound();
	afx_msg void OnToolBarBtnClk_Slow();
	afx_msg void OnToolBarBtnClk_Play();
	afx_msg void OnToolBarBtnClk_Fast();
	LRESULT OnMsgSliderClick(WPARAM wParam, LPARAM lParam);

protected:
	CFont m_fTextFont;
	CString m_strTime;
	CRect m_rcTime;
	BOOL m_bIsPlaying;

	CBitmapSlider m_sliderPos;
	CxSkinButton m_btnStop;
	CxSkinButton m_btnCapture;
	CxSkinButton m_btnSlow;
	CxSkinButton m_btnPlay;
	CxSkinButton m_btnFast;
	CxSkinButton m_btnSound;
	CxSkinButton m_btnFullScreen;
	CBitmapSlider m_valueSound;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	// 设置播放进度.
	void SetPlayPos(int nPos);
	// 刷新时间信息
	void SetTimeInfo(CString strTime);
	// 刷新按钮状态
	void UpdateBtnStatus(BOOL bIsPlaying, int nBtnType);

protected:
	void CreateCtrl();
	void InitCtrl();
	void MoveCtrl();
	void SetToolBarBtnTips();

private:
	// 是否在全屏中
	BOOL m_bIsFullScreening;
	// 是否音频开启中
	BOOL m_bIsSounding;
	// 是否在暂停中
	BOOL m_bIsPausing;

};


