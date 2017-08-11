#pragma once

#include "PlayBarTimeELement.h"
// CPlayBarTimeList

class CPlayBarTimeList : public CWnd
{
	DECLARE_DYNAMIC(CPlayBarTimeList)

public:
	CPlayBarTimeList();
	virtual ~CPlayBarTimeList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(const RECT & rect, CWnd * pParentWnd,int iListStart,int iLineHeit);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	CMapWordToPtr m_ElementMap;
	int m_nVScrollPos;
	int m_iLineHeit;
	int m_iListStart;
public:
	void RemoveALlElement(void);
	BOOL AddElement(int iCameraID);
private:
	long m_lRealHeit;
public:
	void RebuildList();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	BOOL RemoveElement(int iCameraID);
	void SetScroll(void);
	BOOL SetRecordSlice(PSECTIONINFO pSegInfo,int iCameraID);
	BOOL SetSelect(BOOL bSelect, int nCameraID);
	BOOL IsSelect(int nCameraID);
	int GetParentWid(void);
	void InvalidateList(void);
};


