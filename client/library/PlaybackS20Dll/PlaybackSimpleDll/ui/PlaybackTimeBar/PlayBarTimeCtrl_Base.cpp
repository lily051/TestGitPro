/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	22:4:2008   0:24
	filename: 	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo\PlayBarTimeCtrl_Base.cpp
	file path:	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo
	file base:	PlayBarTimeCtrl_Base
	file ext:	cpp
	author:		Angle Pig
	
	purpose:	回放录像文件列表显示控件（基类）
*********************************************************************/
// PlayBarTimeCtrl_Base.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "PlayBarTimeCtrl_Base.h"
#include ".\playbartimectrl_base.h"


// CPlayBarTimeCtrl_Base

IMPLEMENT_DYNAMIC(CPlayBarTimeCtrl_Base, CWnd_AP)
CPlayBarTimeCtrl_Base::CPlayBarTimeCtrl_Base()
{
	// 时间初始化
	CTime ctNow = CTime::GetCurrentTime();
	m_timePosSel.ctTime =  (time_t)ctNow.GetTime();
	m_ctStart = TIMESPAN_MIN;
	m_ctEnd   = TIMESPAN_MAX;

	for (int i = 0; i < 16; i++)
	{
		m_ctElemStart[i] = TIMESPAN_MIN;
		m_ctElemEnd[i] = TIMESPAN_MAX;
		m_bInit[i] = FALSE;
	}

	// 鼠标控制
	m_bDown = false;
	m_bMove = false;
	m_bTrack = false;
	m_hCursor = NULL;
	m_hDragCursor = NULL; 

	// 显示尺寸
	m_uiSpanType = SPAN_ONEDAY;
	m_uiListStartPos = 0;
	m_uiCellNormalWid = 0;
	m_uiCellSeconds = 0;
	m_iMoreCellNum = 0; 
	m_bChgBack = TRUE;
	m_bChgLeft = TRUE;
	m_bChgTop  = TRUE;
	m_iBackWidth = 0;
	m_iBackHeight = 0;
	m_iElemHeight = 20;
	m_iElemSpanHeight = 1;
	m_iShowTop = 0;
	m_iMoreSpan = 0;

	// 片段选择
	m_bSelect = false;
	m_pSelectSections = NULL;

	// 通道选择
	m_iSelElement = 0;
	m_iElemSelectStart = 0;
	m_iElemSelectEnd   = 0;

	// 放大缩小时间轴按钮
	m_bClkLongTime = false;
	m_bHoverLongTime = false;
	m_bClkShortTime = false;
	m_bHoverShortTime = false;

	//  默认为同步模式
	m_bSynchronize = true;  

	// 拖拽时间线的标志 yxn20091203
	m_bDragLine = false;

	//多彩条循环展示
	m_nTopElem = 0;
	m_nCycleElem = 1;
	m_nCycleSpace = 1;
	
	m_pScrollBar = NULL;
	m_nScrollBarWidth = 16;

    m_bShowMidLine  = false;
    m_bDrawBigImage = false;
    m_iRealHeight = 0;
    m_iElemSelectStart = 0;
    m_iElemSelectEnd = 0;
    m_commanCallback = NULL;
    m_timeSelectStart = 0;
    m_timeSelectEnd = 0;
    m_lCommandUser = NULL;
}

CPlayBarTimeCtrl_Base::~CPlayBarTimeCtrl_Base()
{
	// 删除文件列表
	_ClearSection(); 

	// 删除文件类型
	_ClearFileType();

	// 删除选中片段
	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	PSECTIONINFO_FOR9000 pElemSections = NULL ,pElemSecTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;

		pElemSections = pTemp->pFileSections;
		while(pElemSections)
		{
			//pElemSecTemp = pElemSecTemp;
			pElemSections = pElemSections->pNext;

			//delete pElemSecTemp;
		}
		delete pTemp;
        pTemp = NULL;
	}
	m_pSelectSections = NULL;

	// 删除锁定片段
	POSITION pos = m_mapElemLockInfo.GetStartPosition();
	WORD wKey = 0;
	PPLAYBAR_LOCK_SECTION pTempLock = NULL;
	while(pos != NULL)
	{
		m_mapElemLockInfo.GetNextAssoc(pos,wKey,(void * &)pTempLock);

		delete pTempLock;
		pTempLock = NULL;
	}
	m_mapElemLockInfo.RemoveAll();

	// 删除标注片段
	pos = m_mapElemRemarkInfo.GetStartPosition();
	wKey = 0;
	PPLAYBAR_REMARK_SECTION pTempRemark = NULL;
	while(pos != NULL)
	{
		m_mapElemRemarkInfo.GetNextAssoc(pos,wKey,(void * &)pTempRemark);

		delete pTempRemark;
		pTempRemark = NULL;
	}
	m_mapElemRemarkInfo.RemoveAll();
}


BEGIN_MESSAGE_MAP(CPlayBarTimeCtrl_Base, CWnd_AP)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNeedText)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CPlayBarTimeCtrl_Base message handlers


bool CPlayBarTimeCtrl_Base::PlayBar_Create(const RECT & rect, CWnd * pParentWnd, UINT nID,int nSizeStyle,\
										   FCHK_CALLBACK_PLAYBAR callbackfn,void* lUser)
{
	CWnd* pWnd = this;
	//m_iSizeStyle = nSizeStyle;

	if(pWnd->Create(NULL, _T("PlayBarTimeCtrl_Base"), WS_VISIBLE|WS_CHILD|WS_TABSTOP|WS_CLIPCHILDREN/*|WS_BORDER*/, rect, pParentWnd, nID))
	{
		m_lCommandUser = lUser;
		m_commanCallback = callbackfn;
		return true;
	}

	return 0;
}

void CPlayBarTimeCtrl_Base::OnSize(UINT nType, int cx, int cy)
{
	CWnd_AP::OnSize(nType, cx, cy);

	if(cx < 100)
	{
		return;
	}

	PlayBar_ChageSize();
}

void CPlayBarTimeCtrl_Base::PlayBar_ChageSize(void)
{
	// 计算List的左上角坐标
	PlayBar_GetListrect();

	// 获取单元格长度
	PlayBar_GetCellLen();

	// 获取单元格的秒数和左右边界点坐标
	PlayBar_GetPerCellSeconds();

	// 计算背景图片长度
	PlayBar_CalBackLen(); // yxn20090915

	// 设置滚动条位置.
	PlayBar_GetScrollRect();

	// 确定按钮的位置
	int nTop = m_rectList.top -20;
	int nLeft = (m_uiListStartPos - 2*43 -10)/2;
	m_rcLongTime = CRect(nLeft,nTop,nLeft+43,nTop+12);
	nLeft = nLeft + 43 + 10;
	m_rcShortTime = CRect(nLeft,nTop,nLeft+43,nTop+12);

	// 改变中间背景图片
	m_bChgBack = true;

	// 改变时间刻度图片
	m_bChgTop = true;
}

int CPlayBarTimeCtrl_Base::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_AP::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	// 初始化DC
	PlayBar_InitDC();

	PlayBar_ChageSize();

	EnableToolTips(TRUE);

	m_toolTipCtrl.Create(this);
	m_toolTipCtrl.SetMaxTipWidth(200);
	m_toolTipCtrl.AddTool(this);

	m_pScrollBar = new CScrollBar;
	if (m_pScrollBar != NULL)
	{
		m_pScrollBar->Create(SBS_VERT | SBS_LEFTALIGN | WS_CHILD | 
			WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_SCROLLBARCTRL);
	}

	return 0;
}

void CPlayBarTimeCtrl_Base::PlayBar_InitDC(void)
{
	_InitDC();
}

void CPlayBarTimeCtrl_Base::_InitDC(void)
{
}


// 获取当前单元格长度
void CPlayBarTimeCtrl_Base::PlayBar_GetCellLen(void)
{
	_GetCellLen();
}

void CPlayBarTimeCtrl_Base::_GetCellLen(void)
{
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	unsigned int uiListwid = 0;
	unsigned int uiCellNormal = 0;
	uiListwid = rectClient.Width() - m_uiListStartPos;

	switch(m_uiSpanType)
	{
	case SPAN_THREEDAY:
		uiCellNormal = uiListwid / 12 ;//每6小时一格

		// yxn20090915
		m_iMoreCellNum = 12; 
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_ONEDAY:
		uiCellNormal = uiListwid / 12;//每2小时一格
		m_iMoreCellNum = 12; // yxn20090915
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_HALFOFDAY:
		uiCellNormal = uiListwid /12;//每小时一格
		m_iMoreCellNum = 12; // yxn20090915
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_SIXHOUR:
		uiCellNormal = uiListwid / 12;//每半小时一格
		m_iMoreCellNum = 12; // yxn20090915
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_FOURHOUR:
		uiCellNormal = uiListwid / 8;//每半小时一格
		m_iMoreCellNum = 8; // yxn20090915
		m_iMoreSpan = uiListwid%8;
		break;
	case SPAN_TWOHOUR:
		uiCellNormal = uiListwid / 12;//每１０分钟一格
		m_iMoreCellNum = 12; // yxn20090915
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_ONEHOUR:
		uiCellNormal = uiListwid /12;//每５分钟一格
		m_iMoreCellNum = 12; // yxn20090915
		m_iMoreSpan = uiListwid%12;
		break;
	case SPAN_HALFHOUR:
		uiCellNormal = uiListwid / 6;//每５分钟一格
		m_iMoreCellNum = 6; // yxn20090915
		m_iMoreSpan = uiListwid%6;
		break;
	default:
		break;
	}

	m_uiCellNormalWid = max(1,uiCellNormal);
}

// 获取当前单元格秒数和左右边界点坐标
void CPlayBarTimeCtrl_Base::PlayBar_GetPerCellSeconds(void)
{
	_GetPerCellSeconds();

	_GetNowTimeCellLeft();
}

void CPlayBarTimeCtrl_Base::_GetPerCellSeconds(void)
{
	switch(m_uiSpanType)
	{
	case SPAN_THREEDAY:
		m_uiCellSeconds = 21600;
		break;
	case SPAN_ONEDAY:
		m_uiCellSeconds = 7200;
		break;
	case SPAN_HALFOFDAY:
		m_uiCellSeconds = 3600;
		break;
	case SPAN_SIXHOUR:
	case SPAN_FOURHOUR:
		m_uiCellSeconds = 1800;
		break;
	case SPAN_TWOHOUR:
		m_uiCellSeconds = 600;
		break;
	case SPAN_ONEHOUR:
	case SPAN_HALFHOUR:
		m_uiCellSeconds = 300;
		break;
	default:
		break;
	}
}

// 获取list的左上角坐标
void CPlayBarTimeCtrl_Base::PlayBar_GetListrect(void)
{
	_GetListRect();
}

// 获取滚动条的位置.
void CPlayBarTimeCtrl_Base::PlayBar_GetScrollRect(void)
{
	CRect rcScroll(m_rectList);
	rcScroll.left = m_rectList.right;
	rcScroll.right = rcScroll.left + m_nScrollBarWidth;
	if (m_pScrollBar != NULL)
	{
		m_pScrollBar->MoveWindow(rcScroll);
	}
}

void CPlayBarTimeCtrl_Base::_GetListRect(void)
{
	// 获取每种字体的大小
	CDC *pDC = GetDC();
    if (NULL == pDC)
    {
        return;
    }
	TEXTMETRIC tm;
	CFont *pOldFont = NULL;
	int textHeit[2],textWid[2];
	for(int i = 0; i < 2;i++)
	{
		pOldFont = pDC->SelectObject(&m_font[i]);
		pDC->GetTextMetrics(&tm);

		//记住字体的高和宽
		textWid[i] = tm.tmAveCharWidth;
		textHeit[i] = tm.tmHeight + tm.tmExternalLeading;

		//恢复字体
		if(pOldFont)
		{
			pDC->SelectObject(pOldFont);
			pOldFont = NULL;
		}
	}
	pDC->SelectObject(pOldFont);

	// 计算list的坐标
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);
	m_rectList = rectClient;
	m_rectList.right -= m_nScrollBarWidth;

	// list上面留出写时间刻度的位置
	m_rectList.top = textHeit[0] * 2+ 5;
	
	// 左边留出写Element信息的位置
	unsigned int uiImageSize = 16;
	m_uiListStartPos = textWid[1] * 8 + uiImageSize;

	// 计算中间点的位置
	m_timePosSel.uiTimePos = m_uiListStartPos + (rectClient.Width() - m_uiListStartPos) / 2;

	if(NULL != pDC)
	{
		ReleaseDC(pDC);
		pDC = NULL;
	}
}

// 获取当前时间一格当中的剩余像素
unsigned int CPlayBarTimeCtrl_Base::_GetNowTimeCellLeft(void)
{
	// 计算定位点的绝对时间（单位为秒）
	CTime ctNow(m_timePosSel.ctTime);

	/*unsigned */int uiHour = ctNow.GetHour();
	/*unsigned */int uiMinutes = ctNow.GetMinute();
	/*unsigned */int uiSeconds = ctNow.GetSecond();

	/*unsigned */int uiTotalSeconds = uiHour * 3600 + uiMinutes * 60 + uiSeconds;
	/*unsigned */int uiTemp1 = 0,uiTemp2 = 0;
	/*unsigned */int uiCellLeft = 0;

	// 计算距离左边最近时间刻度的秒数和长度
	uiTemp1 = uiTotalSeconds % m_uiCellSeconds;
	uiCellLeft = uiTemp1 * m_uiCellNormalWid / m_uiCellSeconds;
	uiTemp2 = (uiTemp1 * m_uiCellNormalWid) % m_uiCellSeconds;

	if(uiTemp2 > (m_uiCellSeconds / 2))
	{
		uiCellLeft++;
	}

	// 计算左边最近时间刻度的时间和坐标
	time_t ctTime = 0;
	ctTime = m_timePosSel.ctTime - uiTemp1;
	if(ctTime < TIMESPAN_MIN)
	{
		ctTime += m_uiCellSeconds;
		m_timePosNearSel.ctTime = ctTime;
		m_timePosNearSel.uiTimePos = m_timePosSel.uiTimePos - uiCellLeft + m_uiCellNormalWid;
	}else
	{
		m_timePosNearSel.ctTime = ctTime;
		m_timePosNearSel.uiTimePos = m_timePosSel.uiTimePos - uiCellLeft;
	}
	//m_uiSelCellLeft = uiCellLeft;

	// 计算定位点距离左边界点的秒数
	/*unsigned */int uiLeftLen = m_timePosSel.uiTimePos - m_uiListStartPos;
	/*unsigned */int uiLeftTime = 0;

	uiLeftTime = (int)((float)((float)((float)m_uiCellSeconds * (float)uiLeftLen) / (float)m_uiCellNormalWid) + 0.5);

	// 计算左边界点的时间
	ctTime = m_timePosSel.ctTime - uiLeftTime;
	if(ctTime < TIMESPAN_MIN)
	{
		ctTime = TIMESPAN_MIN;
		uiLeftTime = (int)(m_timePosSel.ctTime - TIMESPAN_MIN);

		uiLeftLen = (int)((float)((float)((float)uiLeftTime * (float)m_uiCellNormalWid) / (float)m_uiCellSeconds) + 0.5);
	}

	// 记录左边界点的坐标和时间
	m_timePosStart.uiTimePos = m_timePosSel.uiTimePos - uiLeftLen;
	m_timePosStart.ctTime = ctTime;

	// 计算定位点距离右边界点的秒数
	uiLeftLen = m_rectList.right - m_timePosSel.uiTimePos;
	uiLeftTime = (int)((float)((float)((float)m_uiCellSeconds * (float)uiLeftLen) / (float)m_uiCellNormalWid) + 0.5);
	ctTime = m_timePosSel.ctTime + uiLeftTime;

	// 计算右边界点的时间
	if(ctTime > TIMESPAN_MAX)
	{
		ctTime = TIMESPAN_MAX;
		uiLeftTime = (int)(TIMESPAN_MAX - m_timePosSel.ctTime);

		uiLeftLen = (int)((float)((float)((float)uiLeftTime * (float)m_uiCellNormalWid) / (float)m_uiCellSeconds) + 0.5);
	}

	// 记录右边界点的坐标和时间
	m_timePosEnd.uiTimePos = m_timePosSel.uiTimePos + uiLeftLen;
	m_timePosEnd.ctTime = ctTime;

	Invalidate();

	return uiCellLeft;
}

void CPlayBarTimeCtrl_Base::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bDown = true;
	m_bMove = false;

	// yxn20090916
	if(MK_CONTROL&nFlags)
	{
		m_bSelect = true;
		m_bDown = false;
		SetCapture();

		// 获取点对应的横坐标——时间
		unsigned int uiPos = point.x;
		time_t timeTemp;
		if(!_GetTimeByPos(uiPos,timeTemp))
		{
			m_timeSelectStart = m_timePosStart.ctTime;
		}
		else
		{
			m_timeSelectStart = timeTemp;
		}

		// 获取点对应的纵坐标——单元格序号
		m_iElemSelectStart = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight;
	}
	else
	{
		SetCapture();
		m_OldPoint = point;
		m_pointLBDown = point;

		if(point.y > m_rectList.top)
		{
			// 获取点对应的纵坐标——单元格序号
			int iSelElem = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight;
			if(iSelElem >= 0 && iSelElem != m_iSelElement)
			{
				PlayBar_SetSelectElem(iSelElem);
				//::PostMessage(GetParent()->m_hWnd,WM_PB_PLAYBAR,PLAYBAR_COMMAND_CHGSELWND,iSelElem);
				Invalidate();
			}
		}
		else if(m_rcLongTime.PtInRect(point))
		{
			m_bClkLongTime = TRUE;
			m_bDown = false;
			m_rcLongTime.OffsetRect(1,1);
			InvalidateRect(&m_rcLongTime);
		}
		else if(m_rcShortTime.PtInRect(point))
		{
			m_bClkShortTime = TRUE;
			m_bDown = false;
			m_rcShortTime.OffsetRect(1,1);
			InvalidateRect(&m_rcShortTime);
		}
	}

	SetFocus();


	CWnd_AP::OnLButtonDown(nFlags, point);
}

void CPlayBarTimeCtrl_Base::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDown)
	{
		m_bDown = false;
	}

	if(GetCapture() == this)
	{
		ReleaseCapture();
	}

	// yxn20090916
	bool bChgScale = false;
	if(m_bSelect)
	{
		m_bSelect = false;

		// 获取点对应的横坐标——时间
		unsigned int uiPos = point.x;
		time_t timeTemp;
		if(!_GetTimeByPos(uiPos,timeTemp))
		{
			m_timeSelectEnd = m_timePosEnd.ctTime;
		}
		else
		{
			m_timeSelectEnd = timeTemp;
		}

		// 获取点对应的纵坐标——单元格序号
		m_iElemSelectEnd = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight + m_nTopElem;

		// 获取选中区域中的片段列表
		_GetSelectSections();

	}
	else if(m_bMove)
	{
// 		if (!theApp.m_struClientLocalInfo.bSetPosClick)
// 		{
// 			return;
// 		}
		if(m_bSynchronize == true)
		{
			// 拖动完成，发送时间改变消息
			::PostMessage(GetParent()->m_hWnd,WM_PB_PLAYBAR,PLAYBARCTRL_COMMAND_TIMECHANGE,(LPARAM)m_timePosSel.ctTime);
		}
		else  // yxn20091203
		{
			if(m_bDragLine == true)   
			{
				m_bDragLine = false;

				// 发送改变时间消息
				//::PostMessage(GetParent()->m_hWnd,WM_PB_SINGLE_TIMECHG,(WPARAM)m_iSelElement,(LPARAM)m_timeSingleElem[m_iSelElement]);
			}
		}
	}
	else if(m_bClkLongTime) // 判断按钮是否点下
	{
		m_bClkLongTime = false;
		m_rcLongTime.OffsetRect(-1,-1);
		InvalidateRect(m_rcLongTime);

		if(m_uiSpanType < SPAN_HALFHOUR)
		{
			m_uiSpanType++;
			bChgScale = true;
		}
	}
	else if(m_bClkShortTime)
	{
		m_bClkShortTime = false;
		m_rcShortTime.OffsetRect(-1,-1);
		InvalidateRect(m_rcShortTime);

		if(m_uiSpanType > SPAN_THREEDAY)
		{
			m_uiSpanType--;
			bChgScale = true;
		}
	}

	if (!m_bMove)
	{
// 		if (!theApp.m_struClientLocalInfo.bSetPosClick)
// 		{
// 			return;
// 		}
		if(point.x >= (long)m_timePosStart.uiTimePos && point.x <= (long)m_timePosEnd.uiTimePos)
		{
			// 获取点对应的纵坐标——单元格序号
			if(point.x > m_rectList.top)
			{
				int iSelElem = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight + m_nTopElem;
				// 时间轴不动
				time_t tSelTime;
				if(m_bInit[iSelElem]  &&  true == _GetTimeByPos(point.x,tSelTime))
				{
					//if(tSelTime >= m_ctStart && tSelTime <= m_ctEnd)
					if (tSelTime >= m_ctElemStart[iSelElem]  &&  tSelTime <= m_ctElemEnd[iSelElem]  
					&&  tSelTime >= m_ctStart  &&  tSelTime <= m_ctEnd)
					{
						m_timeSingleElem[iSelElem] = tSelTime;
						//::PostMessage(GetParent()->m_hWnd,WM_PB_SINGLE_TIMECHG,(WPARAM)iSelElem,(LPARAM)tSelTime);
					}
				}	
			}		
		}
	}

	if(bChgScale == true)
	{
		PlayBar_GetCellLen();
		PlayBar_GetPerCellSeconds();
		PlayBar_CalBackLen();
		PlayBar_RepaintBack();
		PlayBar_RepaintTop();
		Invalidate();
	}

	CWnd_AP::OnLButtonUp(nFlags, point);
}

void CPlayBarTimeCtrl_Base::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_bDown)
	{
		int cx = m_OldPoint.x - point.x;

		if(!m_bMove)
		{
			if(abs(cx) > 1)
			{
				m_bMove = true;
			}
		}

		if(m_bMove)
		{
			if(m_hDragCursor)
			{
				SetCursor(m_hDragCursor);
			}

			m_OldPoint = point;

			if(m_bDragLine == false)    // yxn20091203
			{
				_GetPointMoveToTime(cx);
			}
		}
	}

	if(point.x >= (long)m_timePosStart.uiTimePos && point.x < (long)m_timePosEnd.uiTimePos)
	{
		unsigned int uiPos = point.x;
		time_t ctTime = 0;			
		m_timePosHover.uiTimePos = point.x;

		if(_GetTimeByPos(uiPos,ctTime))
		{
			m_timePosHover.uiTimePos = uiPos;
			m_timePosHover.ctTime = ctTime;

			if(m_bDragLine == true) // yxn20091203
			{
				if(m_timePosHover.ctTime >= m_ctStart && m_timePosHover.ctTime <= m_ctEnd)
				{
					m_timeSingleElem[m_iSelElement] = m_timePosHover.ctTime;
				}	
			}

			Invalidate();
		}

		m_toolTipCtrl.Activate(FALSE);
	}
	else if(point.x <= (long)m_uiListStartPos && point.y >= m_rectList.top)  // yxn20091104
	{
		if(m_bHoverLongTime)
		{
			m_bHoverLongTime = FALSE;
			InvalidateRect(&m_rcLongTime);
		}

		if(m_bHoverShortTime)
		{
			m_bHoverShortTime = FALSE;
			InvalidateRect(&m_rcShortTime);
		}

		// 获取点对应的纵坐标——单元格序号
		int iElemID = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight + m_nTopElem;

		// 获取该单元格名称
		if(iElemID >= 0 && iElemID<m_elementCollect.GetCount())
		{
			PLAYBAR_ELEMENT_FILELIST ElementNew = m_elementCollect.GetAt(iElemID);
			CString strName;
			strName.Format(_T("%s"),ElementNew.ElementInfo.czElementName);
			m_toolTipCtrl.Activate(TRUE);
			m_toolTipCtrl.UpdateTipText(strName,this);
		}

	}
	else if(m_rcLongTime.PtInRect(point))  // 判断鼠标是否移上按钮
	{
		m_bHoverLongTime = TRUE;
		InvalidateRect(&m_rcLongTime);
		m_toolTipCtrl.Activate(TRUE);
		m_toolTipCtrl.UpdateTipText(_T("拉长"),this);
	}
	else if(m_rcShortTime.PtInRect(point))
	{
		m_bHoverShortTime = TRUE;
		InvalidateRect(&m_rcShortTime);
		m_toolTipCtrl.Activate(TRUE);
		m_toolTipCtrl.UpdateTipText(_T("缩短"),this);
	}

	if(!m_bTrack)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 1;
		m_bTrack = _TrackMouseEvent(&tme);
	}	

	CWnd_AP::OnMouseMove(nFlags, point);
}

void CPlayBarTimeCtrl_Base::_GetPointMoveToTime(int cx) // yxn20091009
{
	/*unsigned */int uiCx = (int)abs(cx);
	/*unsigned */int uiSeconds = (int)(((float)((float)uiCx * (float)m_uiCellSeconds) / (float)m_uiCellNormalWid) + 0.5);
	time_t ctNow = m_timePosSel.ctTime;

	if(uiSeconds != 0)
	{
		if(cx > 0)
		{
			ctNow += uiSeconds;
		}else
		{
			ctNow -= uiSeconds;
		}

		if(ctNow < TIMESPAN_MIN)
		{
			ctNow = TIMESPAN_MIN;
		}else if(ctNow > TIMESPAN_MAX)
		{
			ctNow = TIMESPAN_MAX;
		}

		if(m_ctStart != 0)
		{
			ctNow = max(m_ctStart,ctNow);
		}

		if(m_ctEnd != 0)
		{
			ctNow = min(m_ctEnd,ctNow);	
		}

		m_timePosSel.ctTime = ctNow;
		_GetNowTimeCellLeft();
	}
}

bool CPlayBarTimeCtrl_Base::PlayBar_SetSpantype(unsigned int uiSpantype)
{
	if(m_uiSpanType != uiSpantype)
	{
		if(uiSpantype >= SPAN_THREEDAY && uiSpantype <= SPAN_HALFHOUR)
		{
			m_uiSpanType = uiSpantype;

			PlayBar_GetCellLen();

			PlayBar_GetPerCellSeconds();

			PlayBar_CalBackLen(); // yxn20090915

			return true;
		}
	}

	return false;
}

unsigned int CPlayBarTimeCtrl_Base::PlayBar_GetSpantype(void)
{
	return m_uiSpanType;
}

time_t CPlayBarTimeCtrl_Base::PlayBar_GetTime(void)
{
	return m_timePosSel.ctTime;
}
void CPlayBarTimeCtrl_Base::PlayBar_ChgElement(int iElementID,CString strName)
{
	if(!_IsElementHas(iElementID))
	{
		return;
	}

	_ClearSection(iElementID);

	PLAYBAR_ELEMENT_FILELIST ElementNew;
	ElementNew.ElementInfo.iElementID = iElementID;
	wsprintf(ElementNew.ElementInfo.czElementName,_T("%s"),strName);
	ElementNew.pSectionList = NULL;
	ElementNew.pNext = NULL;
	m_elementCollect.SetAt(iElementID,ElementNew);

	m_bChgLeft = TRUE;
}

bool CPlayBarTimeCtrl_Base::PlayBar_AddElement(PPLAYBAR_ELEMENT_INFO pElement)
{
	ASSERT(pElement);

	if(_IsElementHas(pElement->iElementID))
	{
		return false;
	}

	PLAYBAR_ELEMENT_FILELIST ElementNew;

	memcpy(&(ElementNew.ElementInfo),pElement,sizeof(PLAYBAR_ELEMENT_INFO));
	ElementNew.pSectionList = NULL;
	ElementNew.pNext = NULL;
	m_elementCollect.Add(ElementNew);

	// yxn20090916
	int nCount = (int)m_elementCollect.GetCount();
	m_iBackHeight = m_iElemHeight*nCount;
	return true;
}

bool CPlayBarTimeCtrl_Base::_IsElementHas(int iElementID)
{
	unsigned int uiCount = (unsigned int)m_elementCollect.GetCount();
	PLAYBAR_ELEMENT_FILELIST ElementFileTemp;

	while(uiCount > 0)
	{
		uiCount --;

		ElementFileTemp = m_elementCollect.GetAt(uiCount);

		if(ElementFileTemp.ElementInfo.iElementID == iElementID)
		{
			return true;
		}
	}
	return false;
}

bool CPlayBarTimeCtrl_Base::_GetRectPosByFileTime(time_t ctStart, time_t ctEnd, unsigned int &uiStartPos, unsigned int &uiEndPos)
{
	if(ctEnd < ctStart)
	{
		return false;
	}

	// 将相对坐标改为绝对坐标 yxn20090916
	if(ctEnd < m_ctStart)
	{
		return false;		
	}

	if(ctStart > m_ctEnd)
	{
		return false;
	}

	if(ctStart <= m_ctStart)
	{
		uiStartPos = m_iMoreCellNum*m_uiCellNormalWid/2 + m_iMoreSpan;
	}
	else
	{
		uiStartPos = (unsigned int)((float)((float)((float)(ctStart - m_ctStart) * (float)m_uiCellNormalWid) / (float)m_uiCellSeconds) + 0.5 + m_iMoreCellNum*m_uiCellNormalWid/2 + m_iMoreSpan);
	}

	if(ctEnd >= m_ctEnd)
	{
		uiEndPos = m_iBackWidth - m_iMoreCellNum*m_uiCellNormalWid/2;
	}else
	{
		uiEndPos = (unsigned int)((float)((float)((float)(ctEnd - m_ctStart) * (float)m_uiCellNormalWid) / (float)m_uiCellSeconds) + 0.5 + m_iMoreCellNum*m_uiCellNormalWid/2 + m_iMoreSpan);
	}

	uiStartPos = max((unsigned int)(m_iMoreCellNum*m_uiCellNormalWid/2+ m_iMoreSpan),uiStartPos);
	uiEndPos = min((unsigned int)(m_iBackWidth - m_iMoreCellNum*m_uiCellNormalWid/2 - m_iMoreSpan),uiEndPos);
	return true;
}

bool CPlayBarTimeCtrl_Base::PlayBar_AddfileType(int iTypeID, COLORREF crColor)
{
	PPLAYBAR_FILETYPE_COLOR pTemp = NULL;

	if(m_FiletypeColormap.Lookup(iTypeID,(void * &)pTemp))
	{
		pTemp->crfileColor = crColor;
		return true;
	}else
	{
		pTemp = new PLAYBAR_FILETYPE_COLOR;
		if(pTemp)
		{
			pTemp->iDataType = iTypeID;
			pTemp->crfileColor = crColor;

			m_FiletypeColormap[iTypeID] = pTemp;

			return true;
		}
	}
	return false;
}

bool CPlayBarTimeCtrl_Base::_GetColorOfFileType(int iDataType,COLORREF & crFileColor)
{
	PPLAYBAR_FILETYPE_COLOR ptemp = NULL;

	if(m_FiletypeColormap.Lookup(iDataType,(void * &)ptemp))
	{
		crFileColor = ptemp->crfileColor;
		return true;
	}

	return false;
}

bool CPlayBarTimeCtrl_Base::PlayBar_SetTime(time_t ctSelTime)
{
	if(ctSelTime < TIMESPAN_MIN || ctSelTime > TIMESPAN_MAX)
	{
		return false;
	}
	else if(ctSelTime < m_ctStart || ctSelTime > m_ctEnd)
	{
		return false;
	}

	m_timePosSel.ctTime = ctSelTime;
	
	_GetNowTimeCellLeft();

	return true;
}

void CPlayBarTimeCtrl_Base::PlayBar_SetCursor(unsigned int uiCursorID)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	} // if

	// Load cursor
	if (uiCursorID)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(uiCursorID), RT_GROUP_CURSOR);
		// Load cursor resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(uiCursorID), IMAGE_CURSOR, 0, 0, 0);
		// Repaint the button
		// If something wrong
		if (m_hCursor == NULL)
		{
			return;
		}
	} // if

	return;
}

BOOL CPlayBarTimeCtrl_Base::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	if(m_hCursor)
	{
		SetCursor(m_hCursor);
		return TRUE;
	}

	return CWnd_AP::OnSetCursor(pWnd, nHitTest, message);
}

bool CPlayBarTimeCtrl_Base::PlayBar_InitTimeSpan(time_t ctStart,time_t ctEnd)
{
	if(ctEnd <= ctStart)
	{
		return false;
	}

	if(ctStart < TIMESPAN_MIN)
	{
		return false;
	}

	if(ctEnd > TIMESPAN_MAX)
	{
		return false;
	}

	CTime cStart(ctStart);
	CTime cEnd(ctEnd);
	m_ctStart = ctStart;
	m_ctEnd = ctEnd;

	for(int i = 0;i<16;i++)
	{
		m_timeSingleElem[i] = ctStart;
	}

	m_timePosSel.ctTime = m_ctStart / 2 + m_ctEnd / 2;

	_GetNowTimeCellLeft();

	PlayBar_CalBackLen(); // yxn20090915

	m_bChgTop = TRUE;
	m_bChgBack = TRUE;

	return true;
}

bool CPlayBarTimeCtrl_Base::PlayBar_InitElemTimeSpan(int nElem, time_t ctStart, time_t ctEnd)
{
	if(ctEnd <= ctStart)
	{
		return false;
	}

	if(ctStart < TIMESPAN_MIN)
	{
		return false;
	}

	if(ctEnd > TIMESPAN_MAX)
	{
		return false;
	}

	if (nElem >= 16)
	{
		return false;
	}

	m_ctElemStart[nElem] = ctStart;
	m_ctElemEnd[nElem] = ctEnd;
	m_bInit[nElem] = TRUE;

	return true;
}

BOOL CPlayBarTimeCtrl_Base::GetElemTimeSpan(int nElem, PB_TIME& tStart, PB_TIME& tEnd)
{
	if (nElem < 0  || nElem >= 16)
	{
		return FALSE;
	}

	time_t ttStart = m_ctElemStart[nElem];
	time_t ttEnd = m_ctElemEnd[nElem];

	COleDateTime ctStart(ttStart);
	tStart.dwYear = ctStart.GetYear();
	tStart.dwMonth = ctStart.GetMonth();
	tStart.dwDay = ctStart.GetDay();
	tStart.dwHour = ctStart.GetHour();
	tStart.dwMinute = ctStart.GetMinute();
	tStart.dwSecond = ctStart.GetSecond();

	COleDateTime ctStop(ttEnd);
	tEnd.dwYear = ctStop.GetYear();
	tEnd.dwMonth = ctStop.GetMonth();
	tEnd.dwDay = ctStop.GetDay();
	tEnd.dwHour = ctStop.GetHour();
	tEnd.dwMinute = ctStop.GetMinute();
	tEnd.dwSecond = ctStop.GetSecond();

	return TRUE;
}

void CPlayBarTimeCtrl_Base::PlayBar_SetDragCursor(unsigned char uiCursorID)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_hDragCursor)
	{
		::DestroyCursor(m_hDragCursor);
		m_hDragCursor = NULL;
	} // if

	// Load cursor
	if (uiCursorID)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(uiCursorID), RT_GROUP_CURSOR);
		// Load cursor resource
		m_hDragCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(uiCursorID), IMAGE_CURSOR, 0, 0, 0);
		// Repaint the button
		// If something wrong
		if (m_hDragCursor == NULL)
		{
			return;
		}
	} // if

	return;
}

void CPlayBarTimeCtrl_Base::PlayBar_AddSection(PSECTIONINFO_FOR9000 pSection,int iElementID)
{
	_ClearSection(iElementID);

	if(iElementID >= 0)
	{
		_AddSection(pSection,iElementID);
		return;
	}

	unsigned int uiCount = (unsigned int)m_elementCollect.GetCount();
	PLAYBAR_ELEMENT_FILELIST FileList;

	while(uiCount > 0)
	{
		uiCount--;

		FileList = m_elementCollect.GetAt(uiCount);

		_AddSection(pSection,FileList.ElementInfo.iElementID);
	}

}

void CPlayBarTimeCtrl_Base::PlayBar_ClearSection(int iElementID)
{
	m_ctElemStart[iElementID] = TIMESPAN_MIN;
	m_ctElemEnd[iElementID] = TIMESPAN_MAX;
	m_bInit[iElementID] = FALSE;

	_ClearSection(iElementID);

	Invalidate();
}

void CPlayBarTimeCtrl_Base::_AddSection(PSECTIONINFO_FOR9000 pSection, int iElementID)
{
	ASSERT(iElementID >= 0);

	unsigned int uiCount = (unsigned int)m_elementCollect.GetCount();
	PLAYBAR_ELEMENT_FILELIST FileList;
	PSECTIONINFO_FOR9000 pSectionTemp = NULL;
	PSECTIONINFO_FOR9000 pSectionNext = NULL;
	PSECTIONINFO_FOR9000 pSectionPre = NULL;
	PSECTIONINFO_FOR9000 pSectionList = NULL;

	while(uiCount > 0)
	{
		uiCount--;
		FileList = m_elementCollect.GetAt(uiCount);

		if(FileList.ElementInfo.iElementID == iElementID)
		{
			pSectionTemp = FileList.pSectionList;

			while(pSectionTemp)
			{
				pSectionNext = pSectionTemp->pNext;
				delete pSectionTemp;
				pSectionTemp = pSectionNext;
			}

			pSectionList = pSection;

			while(pSectionList)
			{
				if(pSectionList->iElementID != iElementID)
				{
					pSectionList = pSectionList->pNext;
					continue;
				}

                pSectionTemp = new(std::nothrow) SECTIONINFO_FOR9000;
   
				if(NULL != pSectionTemp)
				{
					memcpy(pSectionTemp, pSectionList, sizeof(SECTIONINFO_FOR9000));

					if(FileList.pSectionList == NULL)
					{
						FileList.pSectionList = pSectionTemp;
					}
                    //TODO: (makexue) 
					if(NULL != pSectionPre)   //两个指针操作同一块内存，没用完之前，释放任何一个都会出现问题；
					{
						pSectionPre->pNext = pSectionTemp;
					}

					pSectionPre = pSectionTemp;
				}

				pSectionList = pSectionList->pNext;
			}	
		}
		m_elementCollect.SetAt(uiCount,FileList);

	}
}

void CPlayBarTimeCtrl_Base::_ClearSection(int iElementID)
{
	PLAYBAR_ELEMENT_FILELIST FileList;
	PSECTIONINFO_FOR9000 pSectionTemp = NULL,pSectionNext = NULL;

	unsigned int uiCount = 0;

	uiCount = (unsigned int)m_elementCollect.GetCount();

	while(uiCount > 0)
	{
		uiCount--;

		FileList = m_elementCollect.GetAt(uiCount);

		if(iElementID >= 0)
		{
			if(FileList.ElementInfo.iElementID != iElementID)
			{
				continue;
			}
		}

		pSectionTemp = FileList.pSectionList;
		FileList.pSectionList = NULL;
		m_elementCollect.SetAt(uiCount,FileList);

		while(pSectionTemp)
		{
			pSectionNext = pSectionTemp->pNext;
			delete pSectionTemp;
			pSectionTemp = pSectionNext;
		}
	}
}

bool CPlayBarTimeCtrl_Base::_GetTimeByPos(unsigned int uiPos,time_t & ctTime)
{
	if(uiPos > m_timePosEnd.uiTimePos)
	{
		return false;
	}

	if(uiPos <= m_timePosStart.uiTimePos)
	{
		return false;
	}

	time_t ct = 0;

	ct = m_timePosStart.ctTime + (int)(((float)((float)((uiPos - m_timePosStart.uiTimePos) * m_uiCellSeconds) / (float)m_uiCellNormalWid) + 0.5));
	ctTime = ct;

	return true;
}

// yxn20090918
bool CPlayBarTimeCtrl_Base::_GetPosByTime(time_t ctTime,unsigned int &uiPos)
{
	if(ctTime < m_timePosStart.ctTime)
	{
		return false;
	}
	else if(ctTime > m_timePosEnd.ctTime)
	{
		return false;
	}
	else
	{
		uiPos = (unsigned int)((float)((float)((float)(ctTime - m_timePosStart.ctTime) * (float)m_uiCellNormalWid) / (float)m_uiCellSeconds) + 0.5);
		uiPos += m_timePosStart.uiTimePos;
	}

	return true;
}

LRESULT CPlayBarTimeCtrl_Base::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTrack = FALSE;

	m_timePosHover.uiTimePos = 0;
	m_timePosHover.ctTime = 0;

	Invalidate();
	return 0;
}

void CPlayBarTimeCtrl_Base::_ClearFileType(void)
{
	POSITION pos = m_FiletypeColormap.GetStartPosition();
	WORD wKey = 0;
	PPLAYBAR_FILETYPE_COLOR pTemp = NULL;

	while(pos != NULL)
	{
		m_FiletypeColormap.GetNextAssoc(pos,wKey,(void * &)pTemp);

		delete pTemp;
		pTemp = NULL;
	}

	m_FiletypeColormap.RemoveAll();
}

void CPlayBarTimeCtrl_Base::PlayBar_DelElement(int iElementID)
{
	if(iElementID >= 0)
	{
		if(!_IsElementHas(iElementID))
		{
			return;
		}
		
		_ClearSection(iElementID);

		unsigned int uiCount = (unsigned int)m_elementCollect.GetSize();
		PLAYBAR_ELEMENT_FILELIST FileList;

		while(uiCount > 0)
		{
			uiCount--;

			FileList = m_elementCollect.GetAt(uiCount);

			if(FileList.ElementInfo.iElementID == iElementID)
			{
				m_elementCollect.RemoveAt(uiCount);
				Invalidate();
				return;
			}
		}
	}else
	{
		_ClearSection(iElementID);
		m_elementCollect.RemoveAll();
	}

	Invalidate();
}

// yxn20090915
void CPlayBarTimeCtrl_Base::PlayBar_RepaintBack()
{
	m_bChgBack = TRUE;
}

// yxn20090916
void CPlayBarTimeCtrl_Base::PlayBar_RepaintLeft()
{
	m_bChgLeft = TRUE;
}

// yxn20090916
void CPlayBarTimeCtrl_Base::PlayBar_RepaintTop()
{
	m_bChgTop = TRUE;
}

// yxn20090916
void CPlayBarTimeCtrl_Base::PlayBar_CalBackLen()
{
	m_iBackWidth = (int)(((m_ctEnd - m_ctStart)*m_uiCellNormalWid)/m_uiCellSeconds + m_iMoreCellNum*m_uiCellNormalWid + m_iMoreSpan*2);
}

// 根据选中区域修改锁定片段
void CPlayBarTimeCtrl_Base::PlayBar_LockSelSections()
{
	PPLAYBAR_SELECT_SECTION pSelectTemp = m_pSelectSections;
	PPLAYBAR_LOCK_SECTION pLockSection = NULL,pLockTemp = NULL,pNewLockSection = NULL,pNewLockTemp = NULL,pNewLockNext = NULL;
	PSECTIONINFO_FOR9000 pSelectFiles = NULL,pLockFiles = NULL;
	WORD wKey = 0;
	while(pSelectTemp)
	{
		wKey = pSelectTemp->iElementID;

		// 选中的文件链表
		pSelectFiles = pSelectTemp->pFileSections;
		pLockSection = NULL;
		pNewLockSection = NULL;
		if(m_mapElemLockInfo.Lookup(wKey,(void*&)pLockSection))
		{
			// 合并两个链表
			while(pLockSection && pSelectFiles)
			{
				if(pLockSection->timeStart <= pSelectFiles->tTimeStart)
				{
					pNewLockNext = pLockSection;

					pLockSection = pLockSection->pNext;
				}
				else if(pLockSection->timeStart > pSelectFiles->tTimeStart)
				{
					pNewLockNext = new PLAYBAR_LOCK_SECTION;
					pNewLockNext->timeStart = pSelectFiles->tTimeStart;
					pNewLockNext->timeEnd   = pSelectFiles->tTimeEnd;

					pSelectFiles = pSelectFiles->pNext;
				}

				if(pNewLockSection == NULL)
				{
					pNewLockSection = pNewLockTemp = pNewLockNext;
				}
				else
				{
					pNewLockTemp->pNext = pNewLockNext;
					pNewLockTemp = pNewLockNext;
				}
			}

			while(pLockSection)
			{
				pNewLockTemp->pNext = pLockSection;
				pLockSection = pLockSection->pNext;
			}

			while(pSelectFiles)
			{
				pNewLockNext = new PLAYBAR_LOCK_SECTION;
				pNewLockNext->timeStart = pSelectFiles->tTimeStart;
				pNewLockNext->timeEnd   = pSelectFiles->tTimeEnd;

				pSelectFiles = pSelectFiles->pNext;

				if(pNewLockSection == NULL)
				{
					pNewLockSection = pNewLockTemp = pNewLockNext;
				}
				else
				{
					pNewLockTemp->pNext = pNewLockNext;
					pNewLockTemp = pNewLockNext;
				}
			}

			m_mapElemLockInfo[wKey] = pNewLockSection;
		}
		else
		{
			while(pSelectFiles)
			{
				pNewLockNext = new PLAYBAR_LOCK_SECTION;
				pNewLockNext->timeStart = pSelectFiles->tTimeStart;
				pNewLockNext->timeEnd   = pSelectFiles->tTimeEnd;

				pSelectFiles = pSelectFiles->pNext;

				if(pNewLockSection == NULL)
				{
					pNewLockSection = pNewLockTemp = pNewLockNext;
				}
				else
				{
					pNewLockTemp->pNext = pNewLockNext;
					pNewLockTemp = pNewLockNext;
				}
			}

			m_mapElemLockInfo[wKey] = pNewLockSection;
		}

		pSelectTemp = pSelectTemp->pNext;
	}

	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}
	m_pSelectSections = NULL;

	PlayBar_RepaintBack();
}



// 根据选中区域解锁锁定片段
void CPlayBarTimeCtrl_Base::PlayBar_UnLockSelSections()
{
	PPLAYBAR_SELECT_SECTION pSelectTemp = m_pSelectSections;
	PPLAYBAR_LOCK_SECTION pHeadLock = NULL;
	PSECTIONINFO_FOR9000 pSelectFiles = NULL,pLockFiles = NULL;
	WORD wKey = 0;
	while(pSelectTemp)
	{
		wKey = pSelectTemp->iElementID;
		if(m_mapElemLockInfo.Lookup(wKey,(void*&)pHeadLock))
		{
			PPLAYBAR_LOCK_SECTION pLockPre = NULL,pTempLock = pHeadLock;
			while(pTempLock)
			{
				if(pTempLock->timeStart >= pSelectTemp->timeStart && pTempLock->timeEnd <= pSelectTemp->timeEnd)  // 要删除的节点
				{
					// 如果既是头又是尾
					if(pLockPre == NULL && pTempLock->pNext == NULL)
					{
						pHeadLock = NULL;
						delete pTempLock;
						pTempLock = NULL;
					}
					else if(pLockPre == NULL )// 如果是头节点
					{
						pHeadLock = pTempLock->pNext;
						delete pTempLock;
						pTempLock = pHeadLock;
					}
					else if(pTempLock->pNext == NULL)
					{
						// 如果是尾节点
						pLockPre->pNext = NULL;
						delete pTempLock;
						pTempLock = NULL;
					}
					else
					{
						// 其他
						pLockPre->pNext = pTempLock->pNext;
						delete pTempLock;
						pTempLock = pLockPre->pNext;
					}
				}
				else
				{
					if(pLockPre == NULL)
					{
						pHeadLock = pLockPre = pTempLock;
					}

					pTempLock = pTempLock->pNext;
				}
			}
		}

		if(pHeadLock == NULL)
		{
			m_mapElemLockInfo.RemoveKey(wKey);
		}
		else
		{
			m_mapElemLockInfo[wKey] = pHeadLock;
		}

		pSelectTemp = pSelectTemp->pNext;
	}

	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}

	m_pSelectSections = NULL;

	PlayBar_RepaintBack();
}

// 给选中的片段加标注
void CPlayBarTimeCtrl_Base::PlayBar_RemarkSelSections(CString strRemark)
{
	PPLAYBAR_SELECT_SECTION pSelectTemp = m_pSelectSections;
	PPLAYBAR_REMARK_SECTION pRemarkSection = NULL,pNewRemark = NULL;
	WORD wKey = 0;
	while(pSelectTemp)
	{
		pNewRemark = new PLAYBAR_REMARK_SECTION;
		pNewRemark->timeStart = pSelectTemp->timeStart;
		pNewRemark->timeEnd = pSelectTemp->timeEnd;
		pNewRemark->strRemark = strRemark;

		wKey = pSelectTemp->iElementID;
		if(m_mapElemRemarkInfo.Lookup(wKey,(void*&)pRemarkSection))
		{
			// 加入到链表头部
			pNewRemark->pNext = pRemarkSection;
			m_mapElemRemarkInfo[wKey] = pNewRemark;
		}
		else
		{
			m_mapElemRemarkInfo[wKey] = pNewRemark;
		}

		pSelectTemp = pSelectTemp->pNext;
	}

	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}
	m_pSelectSections = NULL;
	m_bChgBack = TRUE;
}

// 删除选中片段的标注
void CPlayBarTimeCtrl_Base::PlayBar_DelSelRemarks()
{
	PPLAYBAR_SELECT_SECTION pSelectTemp = m_pSelectSections;
	PPLAYBAR_REMARK_SECTION pHeadRemark = NULL,pRemarkPre = NULL,pTempRemark = NULL;
	WORD wKey = 0;
	while(pSelectTemp)  // 遍历选中片段
	{
		wKey = pSelectTemp->iElementID;
		if(m_mapElemRemarkInfo.Lookup(wKey,(void*&)pHeadRemark))
		{
			pTempRemark = pHeadRemark;
			pRemarkPre = NULL;
			while(pTempRemark)  // 遍历标注片段
			{
				if(pTempRemark->timeStart >= pSelectTemp->timeStart && pTempRemark->timeEnd <= pSelectTemp->timeEnd)  // 要删除的节点
				{
					// 如果既是头又是尾
					if(pRemarkPre == NULL && pTempRemark->pNext == NULL)
					{
						pHeadRemark = NULL;
						delete pTempRemark;
						pTempRemark = NULL;
					}
					else if(pRemarkPre == NULL )// 如果是头节点
					{
						pHeadRemark = pTempRemark->pNext;
						delete pTempRemark;
						pTempRemark = pHeadRemark;
					}
					else if(pTempRemark->pNext == NULL)
					{
						// 如果是尾节点
						pRemarkPre->pNext = NULL;
						delete pTempRemark;
						pTempRemark = NULL;
					}
					else
					{
						// 其他
						pRemarkPre->pNext = pTempRemark->pNext;
						delete pTempRemark;
						pTempRemark = pRemarkPre->pNext;
					}
				}
				else
				{
					if(pRemarkPre == NULL)
					{
						pHeadRemark = pRemarkPre = pTempRemark;
					}

					pTempRemark = pTempRemark->pNext;
				}
			}

			if(pHeadRemark == NULL)
			{
				m_mapElemRemarkInfo.RemoveKey(wKey);
			}
			else
			{
				m_mapElemRemarkInfo[wKey] = pHeadRemark;
			}
		}

		pSelectTemp = pSelectTemp->pNext;
	}

	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}
	m_pSelectSections = NULL;

	m_bChgBack = TRUE;
}

// 获取选中区域
void CPlayBarTimeCtrl_Base::_GetSelectSections()
{
	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}
	m_pSelectSections = NULL;

	// 遍历各通道的文件列表，获取选中的时间段列表
	PLAYBAR_ELEMENT_FILELIST struElemFileList;
	PSECTIONINFO_FOR9000 pElemFiles = NULL,pSelectFiles = NULL,pSelectFilesTemp = NULL,pSelectFilesNext = NULL;
	PPLAYBAR_SELECT_SECTION pSelectTemp = NULL,pSelectNext = NULL;
	int iElementID = 0;
	time_t timeElemStart,timeElemEnd;
	/*unsigned */int uiCount = (int)m_elementCollect.GetCount();
	unsigned int uiSelectDateLen = 0;
	unsigned int uiTotalDataLen = 0;
	for(int i = 0;i<uiCount;i++)
	{
		// 取出通道的文件列表
		struElemFileList = m_elementCollect.GetAt(i);

		// 判断该通道是否处在选中区域
		iElementID = struElemFileList.ElementInfo.iElementID;
		if(iElementID >= m_iElemSelectStart && iElementID <= m_iElemSelectEnd)
		{
			pElemFiles = struElemFileList.pSectionList;

			timeElemStart = 0;
			timeElemEnd = 0;
			bool bHead = true;
			pSelectFiles = NULL;
			uiSelectDateLen = 0;
			while(pElemFiles)
			{
				// 记录选中片段
				if(pElemFiles->tTimeStart >= m_timeSelectEnd || pElemFiles->tTimeEnd <= m_timeSelectStart)
				{

				}
				else
				{
					pSelectFilesNext = new SECTIONINFO_FOR9000;
					memcpy(pSelectFilesNext,pElemFiles,sizeof(SECTIONINFO_FOR9000));
					pSelectFilesNext->pNext = NULL;

					// 计算截取的长度
					if(pElemFiles->tTimeStart >= m_timeSelectStart && pElemFiles->tTimeEnd <= m_timeSelectEnd)
					{
						uiSelectDateLen += pElemFiles->videoDataLen;
					}
					else if(pElemFiles->tTimeStart < m_timeSelectStart)  // 右边相交
					{
						uiSelectDateLen += (unsigned int)((float)(pElemFiles->tTimeEnd - m_timeSelectStart)*(pElemFiles->videoDataLen)/(float)(pElemFiles->tTimeEnd - pElemFiles->tTimeStart));
					}
					else if(pElemFiles->tTimeEnd > m_timeSelectEnd)      // 左边相交
					{
						uiSelectDateLen += (unsigned int)((float)(m_timeSelectEnd - pElemFiles->tTimeStart)*(pElemFiles->videoDataLen)/(float)(pElemFiles->tTimeEnd - pElemFiles->tTimeStart));
					}


					if(pSelectFiles == NULL)
					{
						pSelectFiles = pSelectFilesTemp = pSelectFilesNext;

						//记录开始时间和结束时间
						timeElemStart = pElemFiles->tTimeStart;
						timeElemEnd = pElemFiles->tTimeEnd;
					}
					else
					{
						pSelectFilesTemp->pNext = pSelectFilesNext;
						pSelectFilesTemp = pSelectFilesNext;

						// 比较是否是最早的开始时间和最晚结束时间
						if(timeElemStart > pElemFiles->tTimeStart)
						{
							timeElemStart = pElemFiles->tTimeStart;
						}

						if(timeElemEnd < pElemFiles->tTimeEnd)
						{
							timeElemEnd = pElemFiles->tTimeEnd;
						}
					}
				}

				pElemFiles = pElemFiles->pNext;
			}

			// 取得选中区域与文件列表的交集
			if(pSelectFiles != NULL)
			{
				pSelectNext = new PLAYBAR_SELECT_SECTION;
				pSelectNext->iElementID = iElementID;
				pSelectNext->timeStart = max(timeElemStart,m_timeSelectStart);
				pSelectNext->timeEnd   = min(timeElemEnd,m_timeSelectEnd);
				wsprintf(pSelectNext->szElementName, _T("%s"),struElemFileList.ElementInfo.czElementName);
				pSelectNext->iTotalDataLen = uiSelectDateLen;
				pSelectNext->pFileSections = pSelectFiles;

				uiTotalDataLen += uiSelectDateLen;

				if(m_pSelectSections == NULL)
				{
					m_pSelectSections = pSelectTemp = pSelectNext;
				}
				else
				{
					pSelectTemp->pNext = pSelectNext;
					pSelectTemp = pSelectNext;
				}
			}
		}
	}

	::PostMessage(GetParent()->m_hWnd,WM_PB_PLAYBAR_SELECT,(WPARAM)uiTotalDataLen,0);
}

void CPlayBarTimeCtrl_Base::PlayBar_SelectAll()
{
	// 清空当前列表
	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;
		delete pTemp;
	}
	m_pSelectSections = NULL;


	// 遍历各通道的文件列表，获取选中的时间段列表
	PLAYBAR_ELEMENT_FILELIST struElemFileList;
	PSECTIONINFO_FOR9000 pElemFiles = NULL,pSelectFiles = NULL,pSelectFilesTemp = NULL,pSelectFilesNext = NULL;
	PPLAYBAR_SELECT_SECTION pSelectTemp = NULL,pSelectNext = NULL;
	int iElementID = 0;
	time_t timeElemStart,timeElemEnd;
	/*unsigned */int uiCount = (int)m_elementCollect.GetCount();
	unsigned int uiSelectDateLen = 0;
	unsigned int uiTotalDataLen = 0;
	for(int i = 0;i<uiCount;i++)
	{
		// 取出通道的文件列表
		struElemFileList = m_elementCollect.GetAt(i);

		// 判断该通道是否处在选中区域
		iElementID = struElemFileList.ElementInfo.iElementID;
		pElemFiles = struElemFileList.pSectionList;

		timeElemStart = 0;
		timeElemEnd = 0;
		pSelectFiles = NULL;
		uiSelectDateLen = 0;
		while(pElemFiles)
		{
			pSelectFilesNext = new SECTIONINFO_FOR9000;
			memcpy(pSelectFilesNext,pElemFiles,sizeof(SECTIONINFO_FOR9000));
			pSelectFilesNext->pNext = NULL;

			uiSelectDateLen += pElemFiles->videoDataLen;

			if(pSelectFiles == NULL)
			{
				pSelectFiles = pSelectFilesTemp = pSelectFilesNext;

				//记录开始时间和结束时间
				timeElemStart = pElemFiles->tTimeStart;
				timeElemEnd = pElemFiles->tTimeEnd;
			}
			else
			{
				pSelectFilesTemp->pNext = pSelectFilesNext;
				pSelectFilesTemp = pSelectFilesNext;

				// 比较是否是最早的开始时间和最晚结束时间
				if(timeElemStart > pElemFiles->tTimeStart)
				{
					timeElemStart = pElemFiles->tTimeStart;
				}

				if(timeElemEnd < pElemFiles->tTimeEnd)
				{
					timeElemEnd = pElemFiles->tTimeEnd;
				}
			}

			pElemFiles = pElemFiles->pNext;
		}

		// 取得选中区域与文件列表的交集
		if(pSelectFiles != NULL)
		{
			pSelectNext = new PLAYBAR_SELECT_SECTION;
			pSelectNext->iElementID = iElementID;
			pSelectNext->timeStart = timeElemStart;
			pSelectNext->timeEnd   = timeElemEnd;
			wsprintf(pSelectNext->szElementName,_T("%s"),struElemFileList.ElementInfo.czElementName);
			pSelectNext->iTotalDataLen = uiSelectDateLen;
			pSelectNext->pFileSections = pSelectFiles;

			uiTotalDataLen += uiSelectDateLen;

			if(m_pSelectSections == NULL)
			{
				m_pSelectSections = pSelectTemp = pSelectNext;
			}
			else
			{
				pSelectTemp->pNext = pSelectNext;
				pSelectTemp = pSelectNext;
			}
		}
	}

	Invalidate();

	::PostMessage(GetParent()->m_hWnd,WM_PB_PLAYBAR_SELECT,(WPARAM)uiTotalDataLen,0);
}

void CPlayBarTimeCtrl_Base::PlayBar_UnSelectAll()
{
	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	PSECTIONINFO_FOR9000 pElemSections = NULL ,pElemSecTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;

		pElemSections = pTemp->pFileSections;
		while(pElemSections)
		{
			//pElemSecTemp = pElemSecTemp;    //这是为什么呢？谁写的？
			pElemSections = pElemSections->pNext;

			//delete pElemSecTemp;
		}

		delete pTemp;
        pTemp = NULL;
	}
	m_pSelectSections = NULL;

	// 重绘
	Invalidate();
}

BOOL CPlayBarTimeCtrl_Base::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	bool bChange = false;
	ScreenToClient(&pt);
	if (pt.x < m_uiListStartPos  ||  pt.x > m_rectList.right)
	{
		if (zDelta < 0 && m_nTopElem < m_elementCollect.GetCount()-m_nCycleElem)
		{
			m_nTopElem += m_nCycleSpace;
			if (m_nTopElem > m_elementCollect.GetCount() - m_nCycleElem)
			{
				m_nTopElem = m_elementCollect.GetCount() - m_nCycleElem;
			}
			bChange = true;
		}
		else if (zDelta > 0 && m_nTopElem > 0)
		{
			m_nTopElem -= m_nCycleSpace;
			if (m_nTopElem < 0)
			{
				m_nTopElem = 0;
			}
			bChange = true;
		}
		// 设置滚动条位置.
		SCROLLINFO si;  
		si.cbSize = sizeof(si);  
		m_pScrollBar->GetScrollInfo(&si, SIF_POS|SIF_PAGE|SIF_RANGE);  
		si.fMask = SIF_POS;  
		si.nPos = m_nTopElem / m_nCycleSpace;  
		m_pScrollBar->SetScrollInfo(&si, TRUE);  

	}
	else
	{
		if(zDelta < 0 && m_uiSpanType > SPAN_THREEDAY)
		{
			m_uiSpanType--;
			bChange = true;
		}
		else if(zDelta > 0 && m_uiSpanType < SPAN_HALFHOUR)
		{
			m_uiSpanType++;
			bChange = true;
		}
	}

	if(bChange == true)
	{
		if(m_bSynchronize == false)  // 异步的模式下，以当前选中的中线为中心 yxn20091203
		{
			m_timePosSel.ctTime = m_timeSingleElem[m_iSelElement];
		}

		PlayBar_GetCellLen();
		PlayBar_GetPerCellSeconds();
		PlayBar_CalBackLen();
		PlayBar_RepaintBack();
		PlayBar_RepaintTop();
		PlayBar_RepaintLeft();
		Invalidate();
	}

	return CWnd_AP::OnMouseWheel(nFlags, zDelta, pt);
}

void CPlayBarTimeCtrl_Base::PlayBar_ShowMiddleLine(bool bShow)
{
	m_bShowMidLine = bShow;
}

void CPlayBarTimeCtrl_Base::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	CWnd_AP::OnRButtonDown(nFlags, point);
}

void CPlayBarTimeCtrl_Base::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(point.x >= (long)m_timePosStart.uiTimePos && point.x <= (long)m_timePosEnd.uiTimePos)
	{
		if(m_bSynchronize == true)
		{
			// 计算位移
			int cx = (int)point.x - (int)m_timePosSel.uiTimePos;

			// 移动时间轴
			_GetPointMoveToTime(cx);

			// 发送改变时间消息
			::PostMessage(GetParent()->m_hWnd,WM_PB_PLAYBAR,PLAYBARCTRL_COMMAND_TIMECHANGE,(LPARAM)m_timePosSel.ctTime);
		}
		else    // 异步模式 yxn20091203
		{
// 			if (theApp.m_struClientLocalInfo.bSetPosClick)
// 			{
// 				return;
// 			}
			// 获取点对应的纵坐标——单元格序号
			if(point.x > m_rectList.top)
			{
				int iSelElem = (point.y - m_rectList.top + m_iShowTop)/m_iElemHeight + m_nTopElem;

				// 时间轴不动
				time_t tSelTime;
				if(m_bInit[iSelElem]  &&  true == _GetTimeByPos(point.x,tSelTime))
				{
					//if(tSelTime >= m_ctStart && tSelTime <= m_ctEnd)
					if (tSelTime >= m_ctElemStart[iSelElem]  &&  tSelTime <= m_ctElemEnd[iSelElem]  
					&&  tSelTime >= m_ctStart  &&  tSelTime <= m_ctEnd)
					{
						m_timeSingleElem[iSelElem] = tSelTime;
						// Invalidate();

						// 发送改变时间消息 yxn20091201
						OutputDebugString(_T("yxn20091205........send message"));
						LPRecordBarDbClickData pData = new RecordBarDbClickData;
						if (pData != NULL)
						{
// 							pData->tCur = tSelTime;  
// 							pData->tStart = m_ctElemStart[iSelElem];  //传入查到的文件的时间是不对的，需要传入需要播放的时间段
// 							pData->tStop = m_ctElemEnd[iSelElem];
							pData->tCur = tSelTime;       //查询结束时间后未停止回放
							pData->tStart = m_ctStart;	//开始时间为业务单录像的开始时间，不是查询到的录像文件的开始时间
							pData->tStop = m_ctEnd;		//结束时间为业务单录像的结束时间，不是查询到的录像文件的结束时间
							::PostMessage(GetParent()->m_hWnd,WM_PB_SINGLE_TIMECHG,(WPARAM)iSelElem,(LPARAM)pData);
						}
					}

				}	
			}		
		}
	}

	CWnd_AP::OnLButtonDblClk(nFlags, point);
}

void CPlayBarTimeCtrl_Base::PlayBar_SetSelectElem(int iElementID)
{
	m_iSelElement = iElementID;

	// 计算当前元素位置，如果不可见，使之可见
	int iSelElemTop = m_iSelElement * m_iElemHeight - m_iShowTop + m_rectList.top;
	if(iSelElemTop + m_iElemHeight >= m_rectList.bottom)
	{
		int iShowTop = (m_iSelElement+1) * m_iElemHeight - m_rectList.Height() + 1;	
	}
	else if(iSelElemTop < m_rectList.top)
	{
		int iShowTop = m_iSelElement * m_iElemHeight;
	}
}

void CPlayBarTimeCtrl_Base::PlayBar_ClearList()
{
	PlayBar_ClearSection();

	CString strTemp;
	for(int i = 0; i < 16; i++)
	{
		strTemp.Format(_T("窗口%02d"), i+1);
		PlayBar_ChgElement(i, strTemp);
	}

	// yxn20090918
	PPLAYBAR_SELECT_SECTION pTemp = NULL;
	PSECTIONINFO_FOR9000 pElemSections = NULL ,pElemSecTemp = NULL;
	while(m_pSelectSections)
	{
		pTemp = m_pSelectSections;
		m_pSelectSections = m_pSelectSections->pNext;

		pElemSections = pTemp->pFileSections;
		while(pElemSections)
		{
			//pElemSecTemp = pElemSecTemp;    //空指针
			pElemSections = pElemSections->pNext;

			//delete pElemSecTemp;
		}

		delete pTemp;
        pTemp = NULL;
	}
	m_pSelectSections = NULL;

	POSITION pos = m_mapElemLockInfo.GetStartPosition();
	WORD wKey = 0;
	PPLAYBAR_LOCK_SECTION pTempLock = NULL;
	while(pos != NULL)
	{
		m_mapElemLockInfo.GetNextAssoc(pos,wKey,(void * &)pTempLock);

		delete pTempLock;
		pTempLock = NULL;
	}
	m_mapElemLockInfo.RemoveAll();

	pos = m_mapElemRemarkInfo.GetStartPosition();
	wKey = 0;
	PPLAYBAR_REMARK_SECTION pTempRemark = NULL;
	while(pos != NULL)
	{
		m_mapElemRemarkInfo.GetNextAssoc(pos,wKey,(void * &)pTempRemark);

		delete pTempRemark;
		pTempRemark = NULL;
	}
	m_mapElemRemarkInfo.RemoveAll();

	m_bChgBack = TRUE;

	Invalidate();
}

BOOL CPlayBarTimeCtrl_Base::OnToolTipNeedText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult)
{
	OutputDebugString(_T("yxn20091104......test1"));
	return TRUE;
}

BOOL CPlayBarTimeCtrl_Base::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_toolTipCtrl.m_hWnd)
	{
		m_toolTipCtrl.RelayEvent(pMsg);
	}
	return CWnd_AP::PreTranslateMessage(pMsg);
}

bool CPlayBarTimeCtrl_Base::PlayBar_SetTimeByElem(int iElemID,time_t ctSelTime)  // yxn20091201
{
	if(ctSelTime < TIMESPAN_MIN || ctSelTime > TIMESPAN_MAX)
	{
		return false;
	}
	else if(ctSelTime < m_ctStart || ctSelTime > m_ctEnd)
	{
		return false;
	}

	m_timeSingleElem[iElemID] = ctSelTime;

	Invalidate();

	return true;
}

void CPlayBarTimeCtrl_Base::PlayBar_SetCycleElem(int nCycleElem, int nCycleSpace)
{
	if (nCycleElem > 0 && nCycleElem < 65)
	{
		m_nCycleElem = nCycleElem;
	}

	if (nCycleSpace > 0)
	{
		m_nCycleSpace = nCycleSpace;
	}
}

bool CPlayBarTimeCtrl_Base::PlayBar_MoveBar(time_t tMoveTime)
{
	// 计算位移
	unsigned int uiPos;
	if (_GetPosByTime(tMoveTime,uiPos))
	{
		int cx = uiPos - (int)m_timePosStart.uiTimePos - 100;
		// 移动时间轴
		_GetPointMoveToTime(cx);
	}
	return TRUE;
}

void CPlayBarTimeCtrl_Base::OnDestroy()
{
	CWnd_AP::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pScrollBar != NULL)
	{
		m_pScrollBar->DestroyWindow();
		delete m_pScrollBar;
		m_pScrollBar = NULL;
	}
}

void CPlayBarTimeCtrl_Base::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bool bChange = FALSE;

	if (NULL == m_pScrollBar)
	{
		return;
	}

	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	m_pScrollBar->GetScrollInfo(&si, SIF_POS|SIF_PAGE|SIF_RANGE);  
	int nVscroll = si.nPos;  

	switch(nSBCode)  
	{  
	case SB_LINEDOWN:  
		{
			nVscroll += si.nPage;   
			if (nVscroll > (si.nMax - si.nMin - si.nPage ))  
			{  
				nVscroll = si.nMax - si.nMin - si.nPage;  
			}  
			bChange = TRUE;
		}
		break;  

	case SB_LINEUP: 
		{
			nVscroll -= si.nPage;  
			if (nVscroll < si.nMin)  
			{  
				nVscroll = 0;  
			}  
			bChange = TRUE;
		}
		break;  
	case SB_PAGEDOWN:  
		{
			nVscroll += si.nPage;   
			if (nVscroll > (si.nMax - si.nMin - si.nPage))  
			{  
				nVscroll = si.nMax - si.nMin - si.nPage;  
			}  
			bChange = TRUE;
		}
		break;  

	case SB_PAGEUP:  
		{
			nVscroll -= si.nPage;  
			if (nVscroll < si.nMin)  
			{  
				nVscroll = 0;  
			}  
			bChange = TRUE;
		}
		break;  
	case  SB_THUMBTRACK: 
		{
			nVscroll = nPos;  
			if (nVscroll > (si.nMax - si.nMin - si.nPage))  
			{  
				nVscroll = si.nMax - si.nMin - si.nPage;  
			} 
			if (nVscroll < si.nMin)  
			{  
				nVscroll = 0;  
			} 
			bChange = TRUE;
		}
		break;
	}  

	//ScrollWindow(0, -(nVscroll -si.nPos) , NULL ,NULL);  

	si.fMask = SIF_POS;  
	si.nPos = nVscroll;  
	m_pScrollBar->SetScrollInfo(&si, TRUE);  

	m_nTopElem = nVscroll * m_nCycleSpace;
	if (m_nTopElem > m_elementCollect.GetCount() - m_nCycleElem)
	{
		m_nTopElem = m_elementCollect.GetCount() - m_nCycleElem;
	}
	if (m_nTopElem < 0)
	{
		m_nTopElem = 0;
	}

	if(bChange == TRUE)
	{
		if(m_bSynchronize == FALSE)  // 异步的模式下，以当前选中的中线为中心 yxn20091203
		{
			m_timePosSel.ctTime = m_timeSingleElem[m_iSelElement];
		}

		PlayBar_GetCellLen();
		PlayBar_GetPerCellSeconds();
		PlayBar_CalBackLen();
		PlayBar_RepaintBack();
		PlayBar_RepaintTop();
		PlayBar_RepaintLeft();
		Invalidate();
	}

	CWnd_AP::OnVScroll(nSBCode, nPos, pScrollBar);
}

/**   @fn          PlayBar_InitScroll
 *    @brief   	   初始化滚动条.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayBarTimeCtrl_Base::PlayBar_InitScroll()
{
	SCROLLINFO si;  
	si.cbSize = sizeof(SCROLLINFO);; 
	si.nMin = 0;  
	si.nMax = m_nCycleSpace;  
	si.nPage = 1 ;  
	si.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;  
	m_pScrollBar->SetScrollInfo(&si, TRUE);  

	if ((1 == m_nCycleElem  ||  4 == m_nCycleElem)  &&  (1 == m_nCycleSpace))
	{
		TRACE("@z PlayBar_InitScroll show, m_nCycleElem:%d", m_nCycleElem);
		m_pScrollBar->ShowWindow(SW_HIDE);
	}
	else
	{
		TRACE("@z PlayBar_InitScroll hide, m_nCycleElem:%d", m_nCycleElem);
		m_pScrollBar->ShowWindow(SW_SHOW);
	}
}