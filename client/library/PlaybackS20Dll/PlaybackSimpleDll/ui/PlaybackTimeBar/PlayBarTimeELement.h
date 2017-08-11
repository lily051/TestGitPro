#pragma once
//#include "atltypes.h"

#define COLOR_RECORD_BLACK RGB(32,35,39)//背景色
#define COLOR_WNDNAMETEXT RGB(0xff,0xff,0xff)//窗口名称字体颜色
#define COLOR_WNDNAMEBK RGB(32,35,39)//窗口名称背景色
#define COLOR_BORDER RGB(195,221,246)//边框颜色
#define COLOR_MANUAL_RECORD  RGB(57,215,189)//手动录像颜色
#define COLOR_PLAN_RECORD  RGB(68,84,172)//计划录像颜色
#define COLOR_ALARMR_ECORD   RGB(255,0,0)//报警录像颜色
#define COLOR_NOR8   RGB(99,121,0XFF)
#define COLOR_NOR9  RGB(80,120,0xff)
#define COLOR_TEXT RGB(255,255,255)//文本颜色

typedef struct _tagSctionInfo{
	unsigned int	iCameraID;
	unsigned int	tTimeStart;
	unsigned int	tTimeEnd;
	unsigned int	iDataType; //0计划录像,1手动录像，2报警录像
	_tagSctionInfo* pNext;
	_tagSctionInfo()
	{
	    iCameraID = 0;
        tTimeStart = 0;
        tTimeEnd = 0;
        iDataType = 0;
		pNext = NULL;
	}
}SCTIOININFO,*PSECTIONINFO;
typedef struct CONTROLCENTER_RecordCell
{
	long m_lStartPoint;
	long m_lEndPoint;
	//	BYTE m_bRecType;
	unsigned int m_iDataType;
	CONTROLCENTER_RecordCell *m_pNext;
	CONTROLCENTER_RecordCell()
	{
		m_lStartPoint = -1;
		m_lEndPoint = -1;
		//		m_bRecType = 0;
		m_iDataType = 0;
		m_pNext = NULL;
	}
}RECORD_CELL,*PRECORD_CELL;

// CPlayBarTimeELement
#define EVENT_SELECT WM_USER + 816

class CPlayBarTimeELement : public CWnd
{
	DECLARE_DYNAMIC(CPlayBarTimeELement)

public:
	CPlayBarTimeELement();
	virtual ~CPlayBarTimeELement();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	BOOL Create(const RECT & rect, CWnd * pParentWnd, int nChannelID,int iLineStart);
private:
	CBitmap m_SelBmp[2];
	int m_nCameraID;
	CPen m_pen[9];
	CBrush m_Brush[8];
	void *m_pParent;
	int m_iLineStart;
	CFont m_Font;
	void DrawUI(CDC * pDC);
protected:
	virtual void PreSubclassWindow();
private:
	BOOL m_bHighLight;
public:
	BOOL SetbHighLight(bool bHighLight);
	BOOL SetRecordList(PSECTIONINFO pSegInfo);
private:
	PSECTIONINFO m_pSection;
public:
	void ClearSection(void);
	time_t GetStartTime();
	time_t GetEndTime();
private:
	PRECORD_CELL m_pRecordCell;
public:
	void ClearRecordCell(void);
	BOOL BuildRecordCell(void);
	long GetRealLen(void);
	long GetCurLen(void);
private:
	CRect m_SelectRect;
	BOOL m_bSelect;
public:
	int GetID();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetSelect(BOOL bSelect);
	BOOL IsSelected(void);
	int GetCellWid(void);
	long GetSelPos(void);
};


