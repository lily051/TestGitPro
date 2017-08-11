/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	21:4:2008   23:23
	filename: 	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo\PlayBarTimeCtrl_Base.h
	file path:	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo
	file base:	PlayBarTimeCtrl_Base
	file ext:	h
	author:		Angle Pig
	
	purpose:	回放录像文件列表显示控件（基类）
*********************************************************************/
#pragma once

// CPlayBarTimeCtrl_Base
#include "Wnd_AP.h"
#include "memdcex.h"
#include "PlayBarStruct.h"

#define WM_PB_PLAYBAR_SELECT (WM_USER+94)
#define WM_PB_PLAYBAR        (WM_USER+99)
#define WM_PB_SINGLE_TIMECHG (WM_USER+100)

// 滚动条ID.
#define IDC_SCROLLBARCTRL 9721

// 录像时间条点击消息结构体.
typedef struct _RecordBarDbClickData
{
	time_t tStart;
	time_t tStop;
	time_t tCur;
}RecordBarDbClickData, *LPRecordBarDbClickData;

class CPlayBarTimeCtrl_Base : public CWnd_AP
{
	DECLARE_DYNAMIC(CPlayBarTimeCtrl_Base)

public:
	CPlayBarTimeCtrl_Base();
	virtual ~CPlayBarTimeCtrl_Base();

public:
//调用接口....................................................begin
	/*************************************************
	Function:    	PlayBar_Create
	Description:	创建控件
	Input:			rect	   控件的大小
					pParentWnd 父窗口
					nID        控件ID
					nSizeStyle 预留（分辨率自适应时用）
					callbackfn 消息回调函数
					lUser      自定义参数
	Output:      	
	Return:			是否创建成功
	*************************************************/
	virtual bool PlayBar_Create(const RECT & rect, CWnd * pParentWnd, UINT nID,int nSizeStyle,\
		FCHK_CALLBACK_PLAYBAR callbackfn = NULL,void*  lUser = NULL);

	/*************************************************
	Function:    	PlayBar_ClearSection
	Description:	清空文件列表,并在控件上刷新显示
	Input:			iElementID 要清空的Elment的ID iElment = -1表示清空所有
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_ClearSection(int iElementID = 0);

	/*************************************************
	Function:    	PlayBar_AddSection
	Description:	传入文件链表,在控件上显示
	Input:			pSection   传入的文件链表（每个录像文件的开始时间，结束时间，录像类型）
					iElementID 文件对应的通道ID (如果iElementID = -1 表示对应所有的通道)
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_AddSection(PSECTIONINFO_FOR9000 pSection,int iElementID = -1);

	/*************************************************
	Function:    	PlayBar_SetCursor
	Description:	设置鼠标移到控件上的光标(如果不调用,使用系统默认的光标)
	Input:			uiCursorID 光标的ID
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_SetCursor(unsigned int uiCursorID);

	/*************************************************
	Function:    	PlayBar_SetDragCursor
	Description:	设置鼠标拖动控件时的光标(如果不调用,使用系统默认的光标)
	Input:			uiCursorID 光标的ID
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_SetDragCursor(unsigned char uiCursorID);

	/*************************************************
	Function:    	PlayBar_InitTimeSpan
	Description:	设置控件显示的时间范围(如果不调用,那么起始时间和结束时间为默认的时间最小值及最大值)
	Input:			ctStart 起始时间 ctEnd 结束时间
	Output:      	
	Return:			是否设置成功
	*************************************************/
	bool PlayBar_InitTimeSpan(time_t ctStart,time_t ctEnd);

	/*************************************************
	Function:    	PlayBar_InitElemTimeSpan
	Description:	设置控件元素显示的时间范围(如果不调用,那么起始时间和结束时间为默认的时间最小值及最大值)
	Input:			ctStart 起始时间 ctEnd 结束时间 nElem 元素ID
	Output:      	
	Return:			是否设置成功
	*************************************************/
	bool PlayBar_InitElemTimeSpan(int nElem, time_t ctStart, time_t ctEnd);

	/*************************************************
	Function:    	PlayBar_GetSpantype
	Description:	获取显示的时间范围类型
	Input:		
	Output:      	
	Return:			类型ID
	*************************************************/
	unsigned int PlayBar_GetSpantype(void);

	/*************************************************
	Function:    	PlayBar_SetSpantype
	Description:	设置显示的时间范围的类型
	Input:			uiSpantype 类型ID
	Output:      	
	Return:			是否设置成功
	*************************************************/
	bool PlayBar_SetSpantype(unsigned int uiSpantype);

	/*************************************************
	Function:    	PlayBar_GetTime
	Description:	获取当前光标所指时间
	Input:		
	Output:      	
	Return:			返回光标所指时间
	*************************************************/
	time_t PlayBar_GetTime(void);

	/*************************************************
	Function:    	PlayBar_SetTime
	Description:	设置当前光标所指时间
	Input:			ctSelTime 设置时间
	Output:      	
	Return:			
	*************************************************/
	bool PlayBar_SetTime(time_t ctSelTime);

	bool PlayBar_SetTimeByElem(int iElemID,time_t ctSelTime);  // yxn20091201

	void PlayBar_SetCycleElem(int nCycleElem, int nCycleSpace = 1);

	//移动时间轴
	bool PlayBar_MoveBar(time_t tMoveTime);//ljg add 2010-9-29 14:23:27

	/*************************************************
	Function:    	PlayBar_AddElement
	Description:	添加一条Element
	Input:			pElement Element的信息 (名称,ID)
	Output:      	
	Return:			是否添加成功
	*************************************************/
	bool PlayBar_AddElement(PPLAYBAR_ELEMENT_INFO pElement);

	/*************************************************
	Function:    	PlayBar_AddfileType
	Description:	添加文件的类型以及表示颜色
	Input:			iTypeID 类型ID 
					crColor 表示颜色
	Output:      	
	Return:			是否添加成功
	*************************************************/
	bool PlayBar_AddfileType(int iTypeID, COLORREF crColor);

	/*************************************************
	Function:    	PlayBar_InitScroll
	Description:	初始化滚动条.
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_InitScroll();

	/*************************************************
	Function:    	GetElemTimeSpan
	Description:	回去元素时间范围.
	Output:      	tStart:开始时间.
	Output:			tEnd:结束时间.
	Input:			nElem:元素ID.
	Return:			
	*************************************************/
	BOOL GetElemTimeSpan(int nElem, PB_TIME& tStart, PB_TIME& tEnd);
//调用接口....................................................end


	
public:
	/*************************************************
	Function:    	PlayBar_ChageSize
	Description:	当控件大小变化是调用,调整位置
	Input:			
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_ChageSize(void);

	/*************************************************
	Function:    	PlayBar_InitDC
	Description:	初始化画图所需资源
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_InitDC(void);

	/*************************************************
	Function:    	PlayBar_GetPerCellSeconds
	Description:	根据显示范围的类型计算每一格的时间间隔
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetPerCellSeconds(void);

	/*************************************************
	Function:    	PlayBar_GetCellLen
	Description:	根据控件的大小以及显示范围的类型计算每一格的长度
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetCellLen(void);

	/*************************************************
	Function:    	PlayBar_GetListrect
	Description:	获取显示List的区域大小
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetListrect(void);

	/*************************************************
	Function:    	PlayBar_GetScrollRect
	Description:	获取滚动条的区域大小
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetScrollRect(void);

	/*************************************************
	Function:    	PlayBar_RepaintBack
	Description:	重绘背景 yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintBack();

	/*************************************************
	Function:    	PlayBar_CalBackLen
	Description:	计算背景长度 yxn20090916
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_CalBackLen();

	/*************************************************
	Function:    	PlayBar_RepaintLeft
	Description:	重绘左边区域 yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintLeft();

	/*************************************************
	Function:    	PlayBar_RepaintTop
	Description:	重绘上边区域 yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintTop();

	// 获取选中片段列表
	PPLAYBAR_SELECT_SECTION PlayBar_GetSelSections(){return m_pSelectSections;}

	// 锁定选中区域
	void PlayBar_LockSelSections();

	// 解锁选中区域
	void PlayBar_UnLockSelSections();

	// 给选中区域加标注
	void PlayBar_RemarkSelSections(CString strRemark);

	// 删除选中区域的标注
	void PlayBar_DelSelRemarks();

	void PlayBar_SetElemRemarkInfo(int iElemID,PPLAYBAR_REMARK_SECTION pElemRemarkInfo);

	void PlayBar_SetElemLockInfo(int iElemID,PPLAYBAR_LOCK_SECTION pElemLockInfo);

	int  PlayBar_GetListTopPos(){return m_rectList.top;}

	int  PlayBar_GetListLeftPos(){return m_uiListStartPos;}

	int  PlayBar_GetRealHeight(){return m_iBackHeight;}

	void PlayBar_SetShowTop(int iShowTop){m_iShowTop = iShowTop;}

	void PlayBar_SelectAll();

	void PlayBar_UnSelectAll();
	
	void PlayBar_ShowMiddleLine(bool bShow);

	time_t PlayBar_GetSelectStartTime(){return m_timeSelectStart;}

	time_t PlayBar_GetSelectEndTime(){return m_timeSelectEnd;}

	int PlayBar_GetSelectStartElem(){return m_iElemSelectStart;}

	int PlayBar_GetSelectEndElem(){return m_iElemSelectEnd;}

	void PlayBar_ChgElement(int iElementID,CString strName);

	void PlayBar_SetSelectElem(int iElementID);

	void PlayBar_ClearList();

	// 异步控制
	bool m_bSynchronize;
	void PlayBar_SetSynState(bool bSynchronize){m_bSynchronize = bSynchronize;}

protected:
	DECLARE_MESSAGE_MAP()
protected:

	bool m_bDragLine;  // yxn20091203

	CToolTipCtrl m_toolTipCtrl;
	int m_iSelElement;

	//缩小放大时间轴图片控制
	CBitmap m_bmpLongTime;
	CBitmap m_bmpShortTime;
	CBitmap m_bmpLongTimeSel;
	CBitmap m_bmpShortTimeSel;
	bool m_bClkLongTime;
	bool m_bHoverLongTime;
	bool m_bClkShortTime;
	bool m_bHoverShortTime;
	CRect m_rcLongTime;
	CRect m_rcShortTime;

	bool m_bShowMidLine;
	CMapWordToPtr   m_mapElemLockInfo;          // 通道的锁定信息
	CMapWordToPtr   m_mapElemRemarkInfo;        // 通道的标注信息
	PPLAYBAR_SELECT_SECTION   m_pSelectSections;      // 选中的时间段信息     yxn20090917
	int  m_iElemSelectStart;
	int  m_iElemSelectEnd;
	time_t m_timeSelectStart;
	time_t m_timeSelectEnd;
	bool m_bSelect;
	int  m_iShowTop;       // 显示区域的顶点 
	int  m_iMoreCellNum;   // yxn20090915
	int  m_iMoreSpan;    
	int  m_iElemHeight;    // 每一行的高度
	int  m_iElemSpanHeight;// 每一行之间的间隔
	int  m_iBackWidth;     // 底图宽度 yxn20090915
	int  m_iBackHeight;    // 底图高度 yxn20090916
	bool m_bChgBack;       // yxn20090915
	bool m_bChgLeft;       // yxn20090915
	bool m_bChgTop;        // yxn20090915
	bool m_bDrawBigImage;  // yxn20091010
	int m_iRealHeight;     // 画局部图时的高度

	CScrollBar* m_pScrollBar;	// 滚动条.
	int m_nScrollBarWidth;	// 滚动条宽度.
	int m_nTopElem;    //最上面的录像条序号
	int m_nCycleElem;  //循环展示集容量
	int m_nCycleSpace;  //展示间隔


	BOOL m_bTrack;  // yxn20091022
	time_t m_ctStart;
	time_t m_ctEnd;
	time_t m_ctElemStart[16];		
	time_t m_ctElemEnd[16];
	BOOL m_bInit[16];
	/*unsigned */int m_uiSpanType;
	/*unsigned */int m_uiCellNormalWid;
	/*unsigned */int m_uiListStartPos;
	/*unsigned */int m_uiCellSeconds;
	PLATBAR_TIMEPOS m_timePosHover;
	PLATBAR_TIMEPOS m_timePosStart;
	PLATBAR_TIMEPOS m_timePosEnd;
	PLATBAR_TIMEPOS m_timePosSel;
	PLATBAR_TIMEPOS m_timePosNearSel;
	time_t m_timeSingleElem[16];
	bool m_bMove;
	bool m_bDown;
	CRect m_rectList;
	CFont m_font[2];
	CPoint m_OldPoint;
	CPoint m_pointLBDown; // yxn20091101
	void* m_lCommandUser;  // yxn20091022
	HCURSOR m_hCursor;
	HCURSOR m_hDragCursor;
	CArray<PLAYBAR_ELEMENT_FILELIST,PLAYBAR_ELEMENT_FILELIST>  m_elementCollect;
	CMapWordToPtr m_FiletypeColormap;
	//消息回调指针
	FCHK_CALLBACK_PLAYBAR	m_commanCallback;
protected:
	void _GetSelectSections();  // 获取选中的时间段信息 yxn20090916
	void _AddSection(PSECTIONINFO_FOR9000 pSection, int iElementID);
	void _ClearSection(int iElementID = -1);
	bool _GetTimeByPos(unsigned int uiPos,time_t & ctTime);
	bool _GetPosByTime(time_t ctTime,unsigned int &uiPos); // yxn20090917
	bool _IsElementHas(int iElementID);
	bool _GetRectPosByFileTime(time_t ctStart, time_t ctEnd, unsigned int &uiStartPos, unsigned int &uiEndPos);
	bool _GetColorOfFileType(int iDataType,COLORREF & crFileColor);
	void _GetPointMoveToTime(int cx);  // yxn20091009
	// virtual void _ChageSize(void);
	virtual void _InitDC(void);
	virtual void _GetPerCellSeconds(void);
	virtual void _GetCellLen(void);
	virtual void _GetListRect(void);
	virtual unsigned int _GetNowTimeCellLeft(void);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	void _ClearFileType(void);
public:
	void PlayBar_DelElement(int iElementID);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	BOOL OnToolTipNeedText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


