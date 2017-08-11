#pragma once
#include "CustomLayoutData.h"


class CLayoutctrl
{
public:
	CLayoutctrl(void);
	~CLayoutctrl(void);
	BOOL GetArrangeModeParam(WORD wMode,PLAYOUT_PARAM pLayout);

	int	SetInfoFirst(RECT sRect);
	/*************************************************
	Function:    SetArrangeMode
	Description　设置预览模式 
	Input:       iMode　预览模式
	Output:      无
	Return:		 该模式下的区域数
	*************************************************/
	int	SetArrangeMode(WORD wMode);

	/*************************************************
	Function:    GetArrangeMode
	Description　获取预览模式 
	Input:       
	Output:      wMode　预览模式
	Return:		 该模式下的区域数
	*************************************************/
	int GetArrangeMode(WORD &wMode);

	/*************************************************
	Function:    GetRects
	Description　获取当前模式下预览窗口区域大小 
	Input:       iIndex　索引号
	　　
	  Output:      pRect　返回的区域大小
	  Return:		 0
	  *************************************************/
	int	GetRects(int iIndex, RECT * pRect);
	/*************************************************
	Function:    Zoom
	Description　全屏模式切换 
	Input:       无
	　　
	  Output:      无
	  Return:		 0
	  *************************************************/
	BOOL Zoom();

	BOOL IsZoom();
private:
private:
	BOOL	m_bInitialize; 			/*本模块是否初始化	*/
	RECT	m_sShowRect;			/*显示区域的位置大小*/
	WORD m_wCurLoyoutMode;
	BOOL m_bFullScreen;
	int m_iZoomIndex;
public:
	void Fun_Init(void);
	void Fun_UnInit(void);
};


