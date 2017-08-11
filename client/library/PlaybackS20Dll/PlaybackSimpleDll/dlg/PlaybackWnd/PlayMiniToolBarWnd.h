#pragma once
#include "../ui/uiBase/ButtonST_Skin.h"

//以上按钮个数
#define PVWND_TOOLBAR_BTN_MAXNUM    8

// CRealWndToolBar 对话框

class CPlayMiniToolBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayMiniToolBarWnd)

public:
	CPlayMiniToolBarWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayMiniToolBarWnd();

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnToolBarBtnClkPlay();
	afx_msg void OnToolBarBtnClkStop();
	afx_msg void OnToolBarBtnClkSlow();
	afx_msg void OnToolBarBtnClkFast();
	afx_msg void OnToolBarBtnClkFrame();
    afx_msg void OnToolBarBtnClkCapture();
    afx_msg void OnToolBarBtnClkSave();
    afx_msg void OnToolBarBtnClkSound();
	afx_msg void OnToolBarBtnClkDownload();
	afx_msg void OnToolBarBtnClkExpand();
    
	virtual void PreSubclassWindow();

private:
	CButtonST_Skin m_btnPlay;
	CButtonST_Skin m_btnStop;
	CButtonST_Skin m_btnSlow;
	CButtonST_Skin m_btnFast;
	CButtonST_Skin m_btnFrame;
	CButtonST_Skin m_btnDownload;
    CButtonST_Skin m_btnCapture;
    CButtonST_Skin m_btnSave;
    CButtonST_Skin m_btnSound;
	CButtonST_Skin m_btnExpand;
   
    CBitmap m_hBkImage;                 ///< 背景图
    CDC m_dcBuffer;
	BOOL m_bReverse;					// 是否是倒放状态.
	BOOL m_bPause;						// 是否是暂停状态.
	BOOL m_bOpenSound;					// 是否打开了声音.
	BOOL m_bSave;						// 是否正在剪辑.
	BOOL m_bIsSingle;					// 是否是单帧回放.

public:
	afx_msg void OnPaint();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void CreateChildWnd();
	void InitCtrl();
	void InitPos();
	void OnBtnClick(UINT nID);
    void SetToolBarBtnTips();

    /** @fn         DrawBkImage
     *  @brief      绘制背景图片，被OnPaint调用
     *  @return     void
     */
    void DrawBkImage();

	// 获取音频按钮的窗口位置.
	void GetSoundBtnRect(CRect& rcSound);
	// 更新按钮状态.
	void UpdateBtnState(BOOL bPause, BOOL bOpenSound, BOOL bSave, BOOL bIsSingleFrame, BOOL bSingleSound = FALSE);
	// 更新按钮tips.
	void UpdateButtonTips();
	// 设置倒放状态.
	void SetReverseState(BOOL bReverse);


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

public:
	HRESULT OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
};
