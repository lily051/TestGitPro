#pragma once

#define WM_SELTIME (WM_USER + 200)  // 改变选中的时间项消息

enum dtctrl_style
{
    FDTS_CAL = 0,  // 日历样式
    FDTS_TIME,     // 时间样式
};

enum select_time_rect
{
    ST_HOUR = 0,  // 时区域
    ST_MINI,      // 分区域
    ST_SECD,      // 秒区域
};
// CFDateTimeCtrl

class AFX_EXT_CLASS CFDateTimeCtrl : public CDateTimeCtrl
{
    DECLARE_DYNAMIC(CFDateTimeCtrl)

public: 
    CFDateTimeCtrl();
    virtual ~CFDateTimeCtrl();

    // 设置时间控件的样式
    void SetStyle(int nStyle);
    // 设置日历的样式
    void SetCalStyle(int nCalStyle);

protected:

    bool m_bEnable;     // 是否可用
    bool m_isSelTime;   // 是否选中时间
    int m_nStyle;       // 时间控件样式
    int m_nCalStyle;    // 日历控件样式
    int m_nSelTimeRect; // 选中的时间区域：时、分、秒

    CRect m_rcMask;     // 屏蔽系统日历的区域
    CRect m_rcCal;      // 日历按钮区域
    CRect m_rcTime;     // 时间按钮区域
    CRect m_rcHour;     // 时区域
    CRect m_rcMini;     // 分区域
    CRect m_rcSecd;     // 秒区域
    Image* m_pImgCal;   // 日历按钮图标
    Image* m_pImgTime;  // 时间按钮图标
    Image* m_pImgTranBkg;   // 透明背景

    virtual void PreSubclassWindow();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    LRESULT OnMsgCaldlgClose(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnable(BOOL bEnable);
};


