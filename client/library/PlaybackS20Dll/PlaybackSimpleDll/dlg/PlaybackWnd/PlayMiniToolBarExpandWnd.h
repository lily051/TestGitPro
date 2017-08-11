#pragma once

#include "../ui/uiBase/ButtonST_Skin.h"

// CPlayMiniToolBarExpandWnd

class CPlayMiniToolBarExpandWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayMiniToolBarExpandWnd)

public:
	CPlayMiniToolBarExpandWnd(BOOL bOpenSound, BOOL bSaveData);
	virtual ~CPlayMiniToolBarExpandWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnToolBarBtnClkCapture();
	afx_msg void OnToolBarBtnClkSave();
	afx_msg void OnToolBarBtnClkSound();
	afx_msg void OnToolBarBtnClkDownload();

private:
	CButtonST_Skin m_btnDownload;
	CButtonST_Skin m_btnCapture;
	CButtonST_Skin m_btnSave;
	CButtonST_Skin m_btnSound;

	CPen m_penBorder;			// 边框画笔.
	int m_nBtnNum;				// 按钮个数.
	int m_nBtnWidth;			// 按钮宽度.
	int m_nBtnHeight;			// 按钮高度.
	int m_nBtnSpace;			// 按钮间隔.
	UINT m_uiIdleTime;			// 关闭时间.
	UINT m_uiCloseTimer;		//时钟句柄
	CWnd* m_pParentWnd;			// 父窗口指针.
	BOOL m_bOpenSound;			// 是否打开了声音.
	BOOL m_bSaveData;			// 是否已经保存了数据.

private:
	// 绘制边框.
	void DrawBorder(CDC* pDC);

	void OnBtnClick(UINT nID);
	void CreateChildWnd();
	void InitCtrl();
	void InitPos();
	void SetToolBarBtnTips();

public:
	// 获取窗口宽度.
	int GetWndWidth();
	// 获取窗口高度.
	int GetWndHeight();
	// 开启定时器.
	BOOL SetCloseTimer(void);
	// 关闭定时器.
	void KillColseTimer(void);
	// 创建窗口.
	BOOL CreateFloatWindow(CWnd *pParentWnd);
	// 更新按钮状态.
	void UpdateBtnState(BOOL bOpenSound, BOOL bSave);
	// 更新按钮tips.
	void UpdateButtonTips();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


