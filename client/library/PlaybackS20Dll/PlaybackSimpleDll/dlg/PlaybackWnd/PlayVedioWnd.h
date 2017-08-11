#pragma once

/*****************************************************************
*   Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayVedioWnd.h
*   Description:    �ط���Ƶ�ؼ��࣬ �����ڽ�����Ƶ
*   Author:         yudan
*   Date:			2011-1-30 08:54
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-27 16:33   yudan          create
*****************************************************************/

#define TIMER_SHOW_TEXT       WM_USER + 5001
#define TIMER_SHOW_PIC       WM_USER + 5002
#define TIME_SHOW_VALUE       60

typedef enum _EnumShowType
{
	PLAYVEDIO_SHOWTYPE_NULL = 0,
	PLAYVEDIO_SHOWTYPE_TEXT,
	PLAYVEDIO_SHOWTYPE_PIC,
}EnumShowType;

class CPlayVedioWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayVedioWnd)

public:
	CPlayVedioWnd();
	virtual ~CPlayVedioWnd();

private:
	COLORREF m_clrBk;  //����ɫ
	COLORREF m_clrText;  //������ɫ
	CFont m_fTextFont;  //����
	CString m_strText;    //��ʾ������
	CString m_strOnWnd;   //����Ƶ����ʱ������
	CRect m_rcClient;	// �ͻ�����С.
	BOOL m_bShowStatePic;	// ��ʾ״̬ͼ�궨ʱ���Ƿ�������.
	BOOL m_bShowText;		// ��ʾ���ֶ�ʱ���Ƿ�������.
	EnumShowType m_emShowType;	// ��ǰ��ʾ��ʽ.
	int m_nState;		// ��ǰͼ��״̬.
	int m_nTextCount;		// ��ʾ���ּ���.

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	//������ʾ����
	void SetTextShow(CString strText, BOOL bErase);
	//���õ���Ƶ����ʱ������
	void SetTextOnWnd(const CString &strText) {m_strOnWnd = strText;};
	// ������ʾ״̬ͼ��.
	void SetStatePicShow();
	// ����״̬ͼ������.
	void SetStatePicHide();
	// �ص������ϵ�������
	void DrawTextOnVedio(CDC *pDC);

private:
	// д״̬����.
	void DrawStateText(CDC* pDC);
	// ��״̬ͼ.
	void DrawStatePic(CDC* pDC);
	// �����ʾ����.
	void DrawCleanRect(CDC* pDC);


public:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
