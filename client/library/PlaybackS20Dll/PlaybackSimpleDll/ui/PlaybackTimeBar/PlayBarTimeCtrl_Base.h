/********************************************************************
	Copyright:  2007-2008 Hikvision Digital Technology Co.,  Ltd. 
	created:	21:4:2008   23:23
	filename: 	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo\PlayBarTimeCtrl_Base.h
	file path:	c:\guoll\Working\9000\MonthCalendar_Version9000_Demo\CPlaybar_Version9000_Demo
	file base:	PlayBarTimeCtrl_Base
	file ext:	h
	author:		Angle Pig
	
	purpose:	�ط�¼���ļ��б���ʾ�ؼ������ࣩ
*********************************************************************/
#pragma once

// CPlayBarTimeCtrl_Base
#include "Wnd_AP.h"
#include "memdcex.h"
#include "PlayBarStruct.h"

#define WM_PB_PLAYBAR_SELECT (WM_USER+94)
#define WM_PB_PLAYBAR        (WM_USER+99)
#define WM_PB_SINGLE_TIMECHG (WM_USER+100)

// ������ID.
#define IDC_SCROLLBARCTRL 9721

// ¼��ʱ���������Ϣ�ṹ��.
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
//���ýӿ�....................................................begin
	/*************************************************
	Function:    	PlayBar_Create
	Description:	�����ؼ�
	Input:			rect	   �ؼ��Ĵ�С
					pParentWnd ������
					nID        �ؼ�ID
					nSizeStyle Ԥ�����ֱ�������Ӧʱ�ã�
					callbackfn ��Ϣ�ص�����
					lUser      �Զ������
	Output:      	
	Return:			�Ƿ񴴽��ɹ�
	*************************************************/
	virtual bool PlayBar_Create(const RECT & rect, CWnd * pParentWnd, UINT nID,int nSizeStyle,\
		FCHK_CALLBACK_PLAYBAR callbackfn = NULL,void*  lUser = NULL);

	/*************************************************
	Function:    	PlayBar_ClearSection
	Description:	����ļ��б�,���ڿؼ���ˢ����ʾ
	Input:			iElementID Ҫ��յ�Elment��ID iElment = -1��ʾ�������
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_ClearSection(int iElementID = 0);

	/*************************************************
	Function:    	PlayBar_AddSection
	Description:	�����ļ�����,�ڿؼ�����ʾ
	Input:			pSection   ������ļ�����ÿ��¼���ļ��Ŀ�ʼʱ�䣬����ʱ�䣬¼�����ͣ�
					iElementID �ļ���Ӧ��ͨ��ID (���iElementID = -1 ��ʾ��Ӧ���е�ͨ��)
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_AddSection(PSECTIONINFO_FOR9000 pSection,int iElementID = -1);

	/*************************************************
	Function:    	PlayBar_SetCursor
	Description:	��������Ƶ��ؼ��ϵĹ��(���������,ʹ��ϵͳĬ�ϵĹ��)
	Input:			uiCursorID ����ID
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_SetCursor(unsigned int uiCursorID);

	/*************************************************
	Function:    	PlayBar_SetDragCursor
	Description:	��������϶��ؼ�ʱ�Ĺ��(���������,ʹ��ϵͳĬ�ϵĹ��)
	Input:			uiCursorID ����ID
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_SetDragCursor(unsigned char uiCursorID);

	/*************************************************
	Function:    	PlayBar_InitTimeSpan
	Description:	���ÿؼ���ʾ��ʱ�䷶Χ(���������,��ô��ʼʱ��ͽ���ʱ��ΪĬ�ϵ�ʱ����Сֵ�����ֵ)
	Input:			ctStart ��ʼʱ�� ctEnd ����ʱ��
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	bool PlayBar_InitTimeSpan(time_t ctStart,time_t ctEnd);

	/*************************************************
	Function:    	PlayBar_InitElemTimeSpan
	Description:	���ÿؼ�Ԫ����ʾ��ʱ�䷶Χ(���������,��ô��ʼʱ��ͽ���ʱ��ΪĬ�ϵ�ʱ����Сֵ�����ֵ)
	Input:			ctStart ��ʼʱ�� ctEnd ����ʱ�� nElem Ԫ��ID
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	bool PlayBar_InitElemTimeSpan(int nElem, time_t ctStart, time_t ctEnd);

	/*************************************************
	Function:    	PlayBar_GetSpantype
	Description:	��ȡ��ʾ��ʱ�䷶Χ����
	Input:		
	Output:      	
	Return:			����ID
	*************************************************/
	unsigned int PlayBar_GetSpantype(void);

	/*************************************************
	Function:    	PlayBar_SetSpantype
	Description:	������ʾ��ʱ�䷶Χ������
	Input:			uiSpantype ����ID
	Output:      	
	Return:			�Ƿ����óɹ�
	*************************************************/
	bool PlayBar_SetSpantype(unsigned int uiSpantype);

	/*************************************************
	Function:    	PlayBar_GetTime
	Description:	��ȡ��ǰ�����ָʱ��
	Input:		
	Output:      	
	Return:			���ع����ָʱ��
	*************************************************/
	time_t PlayBar_GetTime(void);

	/*************************************************
	Function:    	PlayBar_SetTime
	Description:	���õ�ǰ�����ָʱ��
	Input:			ctSelTime ����ʱ��
	Output:      	
	Return:			
	*************************************************/
	bool PlayBar_SetTime(time_t ctSelTime);

	bool PlayBar_SetTimeByElem(int iElemID,time_t ctSelTime);  // yxn20091201

	void PlayBar_SetCycleElem(int nCycleElem, int nCycleSpace = 1);

	//�ƶ�ʱ����
	bool PlayBar_MoveBar(time_t tMoveTime);//ljg add 2010-9-29 14:23:27

	/*************************************************
	Function:    	PlayBar_AddElement
	Description:	���һ��Element
	Input:			pElement Element����Ϣ (����,ID)
	Output:      	
	Return:			�Ƿ���ӳɹ�
	*************************************************/
	bool PlayBar_AddElement(PPLAYBAR_ELEMENT_INFO pElement);

	/*************************************************
	Function:    	PlayBar_AddfileType
	Description:	����ļ��������Լ���ʾ��ɫ
	Input:			iTypeID ����ID 
					crColor ��ʾ��ɫ
	Output:      	
	Return:			�Ƿ���ӳɹ�
	*************************************************/
	bool PlayBar_AddfileType(int iTypeID, COLORREF crColor);

	/*************************************************
	Function:    	PlayBar_InitScroll
	Description:	��ʼ��������.
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_InitScroll();

	/*************************************************
	Function:    	GetElemTimeSpan
	Description:	��ȥԪ��ʱ�䷶Χ.
	Output:      	tStart:��ʼʱ��.
	Output:			tEnd:����ʱ��.
	Input:			nElem:Ԫ��ID.
	Return:			
	*************************************************/
	BOOL GetElemTimeSpan(int nElem, PB_TIME& tStart, PB_TIME& tEnd);
//���ýӿ�....................................................end


	
public:
	/*************************************************
	Function:    	PlayBar_ChageSize
	Description:	���ؼ���С�仯�ǵ���,����λ��
	Input:			
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_ChageSize(void);

	/*************************************************
	Function:    	PlayBar_InitDC
	Description:	��ʼ����ͼ������Դ
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_InitDC(void);

	/*************************************************
	Function:    	PlayBar_GetPerCellSeconds
	Description:	������ʾ��Χ�����ͼ���ÿһ���ʱ����
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetPerCellSeconds(void);

	/*************************************************
	Function:    	PlayBar_GetCellLen
	Description:	���ݿؼ��Ĵ�С�Լ���ʾ��Χ�����ͼ���ÿһ��ĳ���
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetCellLen(void);

	/*************************************************
	Function:    	PlayBar_GetListrect
	Description:	��ȡ��ʾList�������С
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetListrect(void);

	/*************************************************
	Function:    	PlayBar_GetScrollRect
	Description:	��ȡ�������������С
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_GetScrollRect(void);

	/*************************************************
	Function:    	PlayBar_RepaintBack
	Description:	�ػ汳�� yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintBack();

	/*************************************************
	Function:    	PlayBar_CalBackLen
	Description:	���㱳������ yxn20090916
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_CalBackLen();

	/*************************************************
	Function:    	PlayBar_RepaintLeft
	Description:	�ػ�������� yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintLeft();

	/*************************************************
	Function:    	PlayBar_RepaintTop
	Description:	�ػ��ϱ����� yxn20090915
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void PlayBar_RepaintTop();

	// ��ȡѡ��Ƭ���б�
	PPLAYBAR_SELECT_SECTION PlayBar_GetSelSections(){return m_pSelectSections;}

	// ����ѡ������
	void PlayBar_LockSelSections();

	// ����ѡ������
	void PlayBar_UnLockSelSections();

	// ��ѡ������ӱ�ע
	void PlayBar_RemarkSelSections(CString strRemark);

	// ɾ��ѡ������ı�ע
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

	// �첽����
	bool m_bSynchronize;
	void PlayBar_SetSynState(bool bSynchronize){m_bSynchronize = bSynchronize;}

protected:
	DECLARE_MESSAGE_MAP()
protected:

	bool m_bDragLine;  // yxn20091203

	CToolTipCtrl m_toolTipCtrl;
	int m_iSelElement;

	//��С�Ŵ�ʱ����ͼƬ����
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
	CMapWordToPtr   m_mapElemLockInfo;          // ͨ����������Ϣ
	CMapWordToPtr   m_mapElemRemarkInfo;        // ͨ���ı�ע��Ϣ
	PPLAYBAR_SELECT_SECTION   m_pSelectSections;      // ѡ�е�ʱ�����Ϣ     yxn20090917
	int  m_iElemSelectStart;
	int  m_iElemSelectEnd;
	time_t m_timeSelectStart;
	time_t m_timeSelectEnd;
	bool m_bSelect;
	int  m_iShowTop;       // ��ʾ����Ķ��� 
	int  m_iMoreCellNum;   // yxn20090915
	int  m_iMoreSpan;    
	int  m_iElemHeight;    // ÿһ�еĸ߶�
	int  m_iElemSpanHeight;// ÿһ��֮��ļ��
	int  m_iBackWidth;     // ��ͼ��� yxn20090915
	int  m_iBackHeight;    // ��ͼ�߶� yxn20090916
	bool m_bChgBack;       // yxn20090915
	bool m_bChgLeft;       // yxn20090915
	bool m_bChgTop;        // yxn20090915
	bool m_bDrawBigImage;  // yxn20091010
	int m_iRealHeight;     // ���ֲ�ͼʱ�ĸ߶�

	CScrollBar* m_pScrollBar;	// ������.
	int m_nScrollBarWidth;	// ���������.
	int m_nTopElem;    //�������¼�������
	int m_nCycleElem;  //ѭ��չʾ������
	int m_nCycleSpace;  //չʾ���


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
	//��Ϣ�ص�ָ��
	FCHK_CALLBACK_PLAYBAR	m_commanCallback;
protected:
	void _GetSelectSections();  // ��ȡѡ�е�ʱ�����Ϣ yxn20090916
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


