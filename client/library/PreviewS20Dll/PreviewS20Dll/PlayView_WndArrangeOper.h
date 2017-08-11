/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	31:7:2008   10:54
	filename: 	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0\PlayView_WndArrangeOper.h
	file path:	c:\guoll\Mydemo\5.2\Playback_Version3.0\Playback_Version3.0
	file base:	PlayView_WndArrangeOper
	file ext:	h
	author:		Angle Pig
	
	purpose:	播放视图画面分割模式管理
*********************************************************************/
//#include "PlayViewStruct.h"
#include "Layoutctrl.h"
#pragma once

class CPlayView_WndArrangeOper
{
public:
	CPlayView_WndArrangeOper(void);
	virtual ~CPlayView_WndArrangeOper(void);

public:
	/*************************************************
	Function:    	PlayView_WndArrangeOper_GetMaxWndNum
	Description:	获取最大窗口数
	Input:		   
	Output:      	
	Return:		    返回最大窗口数	
	*************************************************/
	unsigned int PlayView_WndArrangeOper_GetMaxWndNum(void);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetMaxWndNum
	Description:	设置最大窗口数
	Input:		    uiMaxWndNum  最大的窗口数
	Output:      	
	Return:			是否设置成功
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetMaxWndNum(unsigned int uiMaxWndNum);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_AssignPlayWnd
	Description:	注册窗口指针
	Input:		    pWnd 窗口指针 
					uiWndIndex 窗口索引
	Output:      	
	Return:			是否注册成功
	*************************************************/
	BOOL PlayView_WndArrangeOper_AssignPlayWnd(CWnd * pWnd, unsigned int uiWndIndex);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetZoom
	Description:	设置焦点窗口
	Input:			pWnd 窗口指针
	Output:      	
	Return:			是否设置成功
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetZoom(CWnd * pWnd);

	/*************************************************
	Function:    	PlayView_WndArrangeOper_SetZoom
	Description:	设置焦点窗口
	Input:			uiWndIndex 窗口索引
	Output:      	
	Return:			是否设置成功
	*************************************************/
	BOOL PlayView_WndArrangeOper_SetZoom(unsigned int uiWndIndex);
protected:
	BOOL _UIFun_Update_OutputPose();
	/*************************************************
	Function:    	PlayView_WndArrangeOper_UnAssignAllOutput
	Description:	注销所有注册过的窗口指针
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_UnAssignAllOutput(void);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetWndArrMode
	Description:	设置播放视图的画面分割模式
	Input:			nMode 画面分割参数
	Output:      	
	Return:			是否设置成功
	*************************************************/
	BOOL _PlayView_WndArrangeOper_SetWndArrMode(WORD wMode);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetSize
	Description:	设置播放视图的大小
	Input:		    rect 大小
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_SetSize(RECT rect);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_WndArrModeChange
	Description:	
	Input:		
	Output:      	
	Return:			
	*************************************************/
	virtual void _PlayView_WndArrangeOper_WndArrModeChange();

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_SetEnlargeWnd
	Description:	将选中窗口单画面放大
	Input:		
	Output:      	
	Return:			是否成功
	*************************************************/
	BOOL _PlayView_WndArrangeOper_SetEnlargeWnd(void);

	/*************************************************
	Function:    	_PlayView_WndArrangeOper_DrawOutputBorder
	Description:	画播放窗口的边框线
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void _PlayView_WndArrangeOper_DrawOutputBorder(CDC * pDC);
protected:
	unsigned int m_uiShowWndNum;           //显示窗口的个数
	unsigned int m_uiWndNum;               //最大播放窗口数					
	CWnd * m_pPlayWnd[MAX_LAYOUT_WND_NUM];//已注册的窗口指针
	CWnd * m_pZoomWnd;	                   //焦点窗口的指针
	
	CPen m_BorderPen[2];				   //画窗口边框的画笔
	
	CLayoutctrl m_LayoutCtrl;
private:
	/*************************************************
	Function:    	__IsAddWnd
	Description:	是否是已注册的窗口
	Input:			pWnd 窗口指针
	Output:      	
	Return:			返回窗口的索引
	*************************************************/
	int __IsAddWnd(CWnd * pWnd);

	/*************************************************
	Function:    	__UIFun_IniPose_SetOutputMode
	Description:	设置画面分割模式
	Input:		
	Output:      	
	Return:			
	*************************************************/
	BOOL __UIFun_IniPose_SetOutputMode(WORD wMode);

	/*************************************************
	Function:    	__IsValidWndIndex
	Description:	判断是否是有效的窗口索引号
	Input:			uiWndIndex 窗口索引号
	Output:      	
	Return:			是否索引号有效
	*************************************************/
	BOOL __IsValidWndIndex(unsigned int uiWndIndex);
public:
	/*************************************************
	Function:    	PlayView_WndArrangeOper_GetWndArrMode
	Description:	获取当前的画面分割模式
	Input:		
	Output:      	iArrangeMode 画面分割模式
	Return:			是否获取成功
	*************************************************/
	BOOL PlayView_WndArrangeOper_GetWndArrMode(WORD & wArrangeMode);
	int PlayView_WndArrangeOper_GetZoom();

	UINT PlayView_WndArrangeOper_GetWndNum(void);
	void Fun_Init(void);

	public:
	CWnd* PlayView_WndArrangeOper_GetWnd(int iIndex);
	int PalyView_WndArrangeOper_GetWndIndex(CPoint &pt);
	BOOL PlayView_WndArrangeOper_ChangeWnd(int iSourceWnd, int iPurposeWnd);

};
