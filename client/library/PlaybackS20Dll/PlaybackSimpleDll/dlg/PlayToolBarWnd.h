#pragma once
#include "../ui/uiBase/DrawStripFun.h"
#include "../ui/uiBase/ButtonST_Msg.h"
#include "../ui/uiBase/ButtonST_Skin.h"
#include "../ui/uiCtrl/BitmapSlider.h"
#include "CToolTips.h"
#include <vector>

// CPlayToolBarDlg 对话框

class CPlayToolBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayToolBarWnd)

public:
	CPlayToolBarWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayToolBarWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	BOOL m_bPlayState;

public:
	CButtonST_Skin m_btnGroupCfg;

	CButtonST_Skin m_btnStopAll;
	CButtonST_Skin m_btnSound;
	CButtonST_Skin m_btnPlay;
    CButtonST_Skin m_btnAllCapture;
	CButtonST_Skin m_btnFullScreen;
	CDrawStripFun m_DrawBakeFun;

private:
	//创建子控件
	void CreateChildCtrl();
	//初始化子控件
	void InitCtrl();
	//移动控件
	void MoveCtrl();

	//按钮点击消息传出
	void OnClkToolBarBtn(WPARAM wParam, LPARAM lParam);

	//设置按钮提示符
	void SetButtonTips();
	

public:
	//刷新按钮状态
	void UpdateBtnState(LPVOID lpPlayWnd);
	//控制全屏按钮状态
	void SetFullScreenOn(BOOL bFull);
	//播放按钮状态
	void UpdatePlayState(BOOL bState);
	// 更新按钮提示符.
	void UpdateButtonTips();

public:
	//按钮点击事件
	afx_msg void OnBnClickPlay();
	afx_msg void OnBnClickReversePlay();
	afx_msg void OnBnClickStopAll();
	afx_msg void OnBnClickVolumn();
	afx_msg void OnBnClickFullScreen();
	afx_msg void OnBnClickPlayGroupCfg();
	afx_msg void OnBnClickSound();
    afx_msg void OnBnClickAllCapture();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
