#pragma once

#include "FButton.h"
#include "FDateTimeCtrl.h"
#include "CalSelectWnd.h"

// CFCalendarDlg 对话框

#define WM_CALDLG_CLOSE (WM_USER + 200)  // 窗口关闭消息
#define WPARAM_UNSELECT    0             // 不选择日期
#define WPARAM_SELECT      1             // 选择日期


#define DATE_ROW_MAX 6   // 日期共6行
#define DATE_COL_MAX 7   // 日期共7列

// 日历样式
enum calendar_style
{
    CS_DATE = 0,     // 日期
    CS_DATETIME = 1, // 日期时间
};


// 日期状态
#define DATE_STATE_NORMAL  0x00      // 正常
#define DATE_STATE_UNUSE   0x01      // 禁用
#define DATE_STATE_HOVER   0x04      // 悬停
#define DATE_STATE_TODAY   0x08      // 今天
#define DATE_STATE_SELECT  0x10      // 选中


// 日期信息
typedef struct date_info
{
    int nState;     // 状态
    int nDate;              // 日期
    CRect rect;             // 区域
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
    bool m_bEnable;         // 是否不可用
    bool m_bAllEnable;      // 是否全部不可用
    int m_nStyle;
    CString m_strClassName;     // 窗口化类名
    CWnd* m_pParent;      // 父窗口
    CRect m_rcDate;       // 显示日期的区域
    CPoint m_ptHover;     // 鼠标悬停的日期坐标
    CPoint m_ptSelect;    // 鼠标选定的日期坐标
    Image* m_pImgBkg;
    Image* m_pImgTranBkg; // 透明背景
    CFButton* m_pbtnPreMonth;
    CFButton* m_pbtnNextMonth;
    CFButton* m_pbtnYear;
    CFButton* m_pbtnMonth;
    CFButton* m_pbtnToday;
    CFButton* m_pbtnCurTime;
    CFButton* m_pbtnOk;
    CFDateTimeCtrl* m_pdtTime;
    CCalSelectWnd* m_pCalselWnd;   // 日历选择窗口

    void InitDlg();
    void InitBtn();
    void DrawWnd(CDC *pDC);
    void DrawDate(CDC *pDC);
    // 设置窗口不可用的程度
    void SetEnable(BOOL bEnable, BOOL bAllEnable);
    // 创建时间选择窗口
    void CreateSelWnd(int nStyle);

/* 日期计算 */
    int m_nCurYear;       // 当前日期显示的年份
    int m_nCurMonth;      // 当前日期显示的月份
    SYSTEMTIME m_timeToday;       // 今天的完整日期
    SYSTEMTIME m_timeSelect;      // 选中的完整日期

    // 储存日期信息的数组
    STRU_DATE_INFO m_strArrDate[DATE_ROW_MAX + 1][DATE_COL_MAX + 1];
    // 获取一个月的总天数
    int GetDaysOfMonth(int nYear, int nMonth);
    // 是否为闰年
    bool IsLeapYear(int nYear);
    // 更新日期信息
    void UpdataDate(int nYear, int nMonth, int nToday);
    // 计算各个日期的区域
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
