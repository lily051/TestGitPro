#pragma once

#include "FButton.h"
#include "FDateTimeCtrl.h"
#include "CalSelectWnd.h"

// CFCalendarDlg �Ի���

#define WM_CALDLG_CLOSE (WM_USER + 200)  // ���ڹر���Ϣ
#define WPARAM_UNSELECT    0             // ��ѡ������
#define WPARAM_SELECT      1             // ѡ������


#define DATE_ROW_MAX 6   // ���ڹ�6��
#define DATE_COL_MAX 7   // ���ڹ�7��

// ������ʽ
enum calendar_style
{
    CS_DATE = 0,     // ����
    CS_DATETIME = 1, // ����ʱ��
};


// ����״̬
#define DATE_STATE_NORMAL  0x00      // ����
#define DATE_STATE_UNUSE   0x01      // ����
#define DATE_STATE_HOVER   0x04      // ��ͣ
#define DATE_STATE_TODAY   0x08      // ����
#define DATE_STATE_SELECT  0x10      // ѡ��


// ������Ϣ
typedef struct date_info
{
    int nState;     // ״̬
    int nDate;              // ����
    CRect rect;             // ����
    date_info()
    {
        nState = DATE_STATE_NORMAL;
        nDate = 0;
        rect = CRect(0, 0, 0, 0);
    }
}STRU_DATE_INFO;


class AFX_EXT_CLASS CCalendarDlg : public CWnd
{
    DECLARE_DYNAMIC(CCalendarDlg)

public:
    CCalendarDlg();
    virtual ~CCalendarDlg();
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
        const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	void SetStyle(int nStyle);
	void SetSelectTime(const SYSTEMTIME& timeSelect);
    void GetSelectTime(SYSTEMTIME& timeSelect);

protected:

    bool m_isInit;
    bool m_bEnable;         // �Ƿ񲻿���
    bool m_bAllEnable;      // �Ƿ�ȫ��������
    int m_nStyle;
    CString m_strClassName;     // ���ڻ�����
    CWnd* m_pParent;      // ������
    CRect m_rcDate;       // ��ʾ���ڵ�����
    CPoint m_ptHover;     // �����ͣ����������
    CPoint m_ptSelect;    // ���ѡ������������
    Image* m_pImgBkg;
    Image* m_pImgTranBkg; // ͸������
    CFButton* m_pbtnPreMonth;
    CFButton* m_pbtnNextMonth;
    CFButton* m_pbtnYear;
    CFButton* m_pbtnMonth;
    CFButton* m_pbtnToday;
    CFButton* m_pbtnCurTime;
    CFButton* m_pbtnOk;
    CFDateTimeCtrl* m_pdtTime;
    CCalSelectWnd* m_pCalselWnd;   // ����ѡ�񴰿�

    void InitDlg();
    void InitBtn();
    void DrawWnd(CDC *pDC);
    void DrawDate(CDC *pDC);
    // ���ô��ڲ����õĳ̶�
    void SetEnable(BOOL bEnable, BOOL bAllEnable);
    // ����ʱ��ѡ�񴰿�
    void CreateSelWnd(int nStyle);

/* ���ڼ��� */
    int m_nCurYear;       // ��ǰ������ʾ�����
    int m_nCurMonth;      // ��ǰ������ʾ���·�
    SYSTEMTIME m_timeToday;       // �������������
    SYSTEMTIME m_timeSelect;      // ѡ�е���������

    // ����������Ϣ������
    STRU_DATE_INFO m_strArrDate[DATE_ROW_MAX + 1][DATE_COL_MAX + 1];
    // ��ȡһ���µ�������
    int GetDaysOfMonth(int nYear, int nMonth);
    // �Ƿ�Ϊ����
    bool IsLeapYear(int nYear);
    // ����������Ϣ
    void UpdataDate(int nYear, int nMonth, int nToday);
    // ����������ڵ�����
    void CalDateRect();

    LRESULT OnMsgSeltime(WPARAM wParam, LPARAM lParam);
    LRESULT OnMsgCalselWndClose(WPARAM wParam, LPARAM lParam);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedBtnPreMonth();
    afx_msg void OnBnClickedBtnNextMonth();
    afx_msg void OnBnClickedBtnYear();
    afx_msg void OnBnClickedBtnMonth();
    afx_msg void OnBnClickedBtnToday();
    afx_msg void OnBnClickedBtnCurtime();

    DECLARE_MESSAGE_MAP()
};
