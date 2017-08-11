#pragma once

#include "FButton.h"
// CCalSelectWnd

#define WM_CALSELWND_CLOSE (WM_USER + 300)  // 窗口关闭消息

// 时间状态
#define TIME_STATE_NORMAL  0x00      // 正常
#define TIME_STATE_UNUSE   0x01      // 禁用
#define TIME_STATE_HOVER   0x04      // 悬停
#define TIME_STATE_CUR     0x08      // 当前
#define TIME_STATE_SELECT  0x10      // 选中

// 时间选择窗口的样式
enum select_wnd_style
{
    SWS_YEAR = 0,   // 年
    SWS_MONTH,      // 月
    SWS_HOUR,      // 时
    SWS_MINI,      // 分
    SWS_SECD,      // 秒
};

// 时间信息
typedef struct time_info
{
    int nState;       // 状态
    int nTime;        // 日期
    CRect rect;       // 区域
    time_info()
    {
        nState = TIME_STATE_NORMAL;
        nTime = 0;
        rect = CRect(0, 0, 0, 0);
    }
}STRU_TIME_INFO;

class CCalSelectWnd : public CWnd
{
    DECLARE_DYNAMIC(CCalSelectWnd)

public:
    CCalSelectWnd();
    virtual ~CCalSelectWnd();

    // 设置样式
    void SetStyle(int nStyle);
    // 设置当前年份
    void SetCurYear(int nYear);

protected:
    int m_nPreStyle;  // 原样式，用于改变时间信息
    int m_nStyle;     // 样式
    int m_nCurYear;   // 当前年份
    int m_nTimeNum;   // 时间信息的个数
    int m_nHover;     // 鼠标悬停的时间
    int m_nSelect;    // 鼠标选定的时间
    int m_nSelTime;   // 选择的时间信息
    CRect m_rcDraw;  // 需要画时间信息的区域
    CFButton* m_pbtnPre;
    CFButton* m_pbtnNext;

    // 存储时间信息的数组
    STRU_TIME_INFO m_strArrayTime[60 + 1];
    // 更新时间信息
    void UpdataTimeInfo(int nStyle);
    // 画不同样式
    void DrawForStyle(CDC *pDC, int nStyle);

    afx_msg void OnPaint();
    afx_msg void OnBnClickedBtnPre();
    afx_msg void OnBnClickedBtnNext();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};


