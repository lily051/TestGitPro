#pragma once
#include "ui/CustomDrawBlackWnd.h"
#include "Layoutctrl.h"
#include "ui/ButtonST_Skin.h"
#include "ui/DrawFillRectFun.h"

#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_6  0x2300 //6�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_8  0x2400 //8�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_10 0x2500 //10�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_13 0x2600 //13�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_14 0x2700 //14�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_17 0x2800 //17�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_22 0x2900 //22�ָ��
#define ID_WND_PLAYVIEWTOOLBAR_ARRANGE_25 0x2A00 //25�ָ��

#define TIMER_EVENT_CLOSEARRWND  1
#define TIMER_EVENT_ELAPSE       1000
#define TIMER_EVENT_CLOSEELAPSE    4 * 1000

#define WM_LAYOUTCLOSE  WM_USER + 8765
// CLayoutMoreWnd
enum ScreenDisplayMode
{
	MODE_NORMALSCREEN  = 0,//Display on 
	MODE_WIDDENSCREEN  = 1 //Display On 
};

class CLayoutMoreWnd : public CCustomDrawBlackWnd
{
	DECLARE_DYNAMIC(CLayoutMoreWnd)

public:
	CLayoutMoreWnd();
	virtual ~CLayoutMoreWnd();

	ScreenDisplayMode m_screenMode;

	BOOL LayoutMoreSetWnd_Create(const RECT &rect, CWnd *pParentWnd, UINT nID,ScreenDisplayMode nMode = MODE_NORMALSCREEN);
	BOOL LayoutMoreSetWnd_CreateEx(const RECT &rect, CWnd *pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
private:
	CButtonST_Skin m_ButtonArrange6;
	CButtonST_Skin m_ButtonArrange8;
	CButtonST_Skin m_ButtonArrange10;
	CButtonST_Skin m_ButtonArrange13;
	CButtonST_Skin m_ButtonArrange14;
	CButtonST_Skin m_ButtonArrange17;
	CButtonST_Skin m_ButtonArrange22;
	CButtonST_Skin m_ButtonArrange25;

	CWnd *m_pParentWnd;//������ָ��

	UINT m_uiCloseTimer;//ʱ�Ӿ��
	UINT m_uiIdleTime;

    std::vector<HWND> m_vecTipWnd;
	CDrawFillRectFun m_FillRectDraw;
private:
	void CreateChildWnd(void);

	void InitCtrl(void);
	void InitPose(void);

	/*************************************************
	Function:    	UnInitCtrl
	Description:	���ٿؼ�
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void UnInitCtrl(void);

	/*************************************************
	Function:    	SetCloseTimer
	Description:	����ʱ��
	Input:		
	Output:      	
	Return:			
	*************************************************/
	bool SetCloseTimer(void);

	/*************************************************
	Function:    	KillColseTimer
	Description:	ֹͣʱ��
	Input:		
	Output:      	
	Return:			
	*************************************************/
	void KillColseTimer(void);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void Wnd_DrawBK(CDC * pDC);

	void SetFocusBtn(int nArrangMode);
	LRESULT OnBtnClicked(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnBtnClickedArr6();
	afx_msg void OnBtnClickedArr8();
	afx_msg void OnBtnClickedArr10();
	afx_msg void OnBtnClickedArr13();
	afx_msg void OnBtnClickedArr14();
	afx_msg void OnBtnClickedArr17();
	afx_msg void OnBtnClickedArr22();
	afx_msg void OnBtnClickedArr25();
public:
	void SetButtonToolTip();
protected:
	virtual void PostNcDestroy();
};


