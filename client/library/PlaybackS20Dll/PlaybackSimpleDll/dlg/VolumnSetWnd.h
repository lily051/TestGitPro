#pragma once
/**	@file    VolumnSetWnd.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   音量设置显示
*
*	@author	 yudan
*	@date	 2012/07/24
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/07/24 by yudan
*/

#include "../ui/uiCtrl/CustomDrawBlackWnd.h"
#include "../ui/uiCtrl/BitmapSlider.h"
#include "../ui/uiBase/DrawStripFun.h"


class CVolumnSetWnd : public CWnd
{
public:
	CVolumnSetWnd();
	virtual ~CVolumnSetWnd();

	DECLARE_MESSAGE_MAP()

	
private:
	BOOL m_bMove;
	CBrush m_bkBrush;
	CFont m_font;
	UINT m_uiCloseTimer;//时钟句柄
	UINT m_uiIdleTime;
	BOOL m_bVoice;      //音频是否打开
	
	CWnd * m_pParentWnd;  //父窗口
	CDC m_dcMem;

	CDrawStripFun m_DrawBakeFun;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //背景图片宽度
	int m_nBitmapHeight;  //背景图片高度

	CBitmapSlider * m_pSliderVolumn;
private:
	//初始化控件
	void InitCtrl();
	//移动控件
	void MoveCtrl();
	//开启定时器
	BOOL SetCloseTimer(void);
	//关闭时钟
	void KillColseTimer(void);

public:
	//创建
	BOOL CreateFloatWindow(CWnd *pParentWnd);
	//设置初始化进度
	void SetInitPos(int nVolumn);
	//进度条点击
	LRESULT OnMsgSliderClick(WPARAM wParam, LPARAM lParam);
	//透明绘图
	void TransparentBk(CDC * pDc, CWnd * pWnd);

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
