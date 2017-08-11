#pragma once
#include "PlayBarTimeList.h"

// CPlayBarTimeCtrl

#define EVENT_SETTIME  WM_USER + 444

class CPlayBarTimeCtrl : public CWnd
{
	DECLARE_DYNAMIC(CPlayBarTimeCtrl)

public:
	CPlayBarTimeCtrl();
	virtual ~CPlayBarTimeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	HANDLE m_hEvent;
	BOOL m_BtnbClk[6];
	afx_msg LRESULT OnEventSelect(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void PreSubclassWindow();
private:
	CWinThread * m_hThread;
	int GetCellLen(void);
	long GetRealLen(void);
	void DrawUI(CDC* pDC);
	void InitDC(void);
	void ClearDC(void);
	void ChageSize(int cx,int cy);
	long GetCellNum(void);
	void GetFontSize(void);
private:
	BOOL m_bBackup;//wgf
	CPen m_Pen[4];
	CBrush m_Brush[7];
	CRect m_BtnRect[6];
	BOOL m_BtnbHover[6];
	long m_lCurLen;
	long m_lSelectPos;
	long m_lRealLen;
	int m_iCellWid;
	long m_lCellNum;
	time_t m_Start;
	time_t m_End;
	CRect m_ListRect;
	int m_iLineHeit;
	int m_iCellNormalWid;
	int m_iDaySpan;
	int m_iSizeStyle;
	int m_iTimeType;
	int m_iCellLenType;
	CFont m_Font[3];
	CSize TextSize[3];
	CPlayBarTimeList *m_pList;
	CString m_strShowInfo;//显示的信息 ljg add 2010-8-11 9:47:18
public:
	//nSizeStyle表示字体的大小
	BOOL PlayBar_Create(const RECT & rect, CWnd * pParentWnd, UINT nID,int nSizeStyle);
	BOOL PlayBar_InitTimeSpan(WORD wStartYear, BYTE bStartMonth, BYTE bStartDay, WORD wEndYear, BYTE bEndMonth, BYTE bEndDay);
private:
	BOOL GetText(long lCellNum,  time_t & ctTime);
	int m_iListStart;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	int m_iElementNum;
public:
	BOOL PlayBar_AddElement(int iCameraID);
	BOOL PlayBar_RemoveAllElement(void);
	BOOL PlayBar_RemoveElement(int iCameraID);
	BOOL IsSelect(int nCameraID);
	BOOL PlayBar_SetRecordSlice(PSECTIONINFO pSegInfo,int ICameraID = -1);
	BOOL PlayBar_SetBackupFlag(BOOL bFlag);
public:
	void GetCurLen(int iLen);
	time_t GetStartTime();
	time_t GetEndTime();
	long GetRealLenForElement(void);
	long GetCurLenForElement(void);
	BOOL SetSelect(BOOL bSelect, int nCameraID);
	int GetCellWidForElement(void);
	int GetWid(void);
	long GetSelPosForElement(void);
	BOOL SetSelPos(long lSelPos);
private:
	BOOL m_bTrack;
public:
	void SetTimeLenType(int iType);
	void SetCellLenType(int iType);
	afx_msg void OnDestroy();
	time_t PlayBar_GetSelTime(void);
	BOOL PlayBar_SetTime(time_t ct);
	void SetShowMsg(CString strMsg);
};


