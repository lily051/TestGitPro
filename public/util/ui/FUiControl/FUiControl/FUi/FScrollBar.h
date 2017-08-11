#pragma once
#include "FButton.h"

#define DEFAULT_SCROLL_WIDTH 17

// CFScrollBar

class AFX_EXT_CLASS CFScrollBar : public CScrollBar
{
    DECLARE_DYNAMIC(CFScrollBar)

public:
    CFScrollBar();
    virtual ~CFScrollBar();

    void SetScrollWidth(const int nWidth);

    int GetScrollWidth();

    void UpdateSlider();

protected:
    BOOL m_bTrace;       // 是否拖动滚动条
    BOOL m_bLeaveTrace;  // 是否在窗口外拖动
    BOOL m_bHoverBtn;    // 是否长按按钮
    int m_nTraceBegin;   // 拖动开始鼠标位置
    int m_nTraceMove;    // 拖动的位移
    int m_nTracePos;     // 滚动条位置的改变

    int m_nHover;       // 悬停位置
    int m_nClick;       // 点击位置
    int m_nUseless;     // 不可用按钮
    int m_nSliderBegin; // 滚动条开始位置
    int m_nSliderEnd;   // 滚动条结束位置
    int m_nSliderPos;   // 滚动条实际位置（中间线）
    int m_nSliderHeight;// 滚动条高度

    int m_nScrollStyle;       // 垂直/水平
    Image* m_pImgUp;          // 向上按钮
    Image* m_pImgDown;        // 向下按钮

    int m_nScrollWidth;         // 滚动条宽度
    int m_nBtnImgWidth;         // 按钮图片宽度

    SCROLLINFO m_siCur;         // 滚动条信息

    void SetCurScrollInfo(const SCROLLINFO& si);
    void GetCurScrollInfo(SCROLLINFO& si);

    // 获取鼠标当前的位置
    int HitTest(const CPoint& point);

    // 画按钮
    void DrawBg(CDC* pDC);

    // 画按钮
    void DrawBtn(CDC* pDC);

    // 画滚动条
    void DrawSlider(CDC* pDC);

    // 刷新部分区域
    void InvalidateArea(int nPos);

    // 计算滚动条位置
    void CalSliderPos();

    // 移动滚动条
    void MoveSlider(const CPoint& point);

    // 响应滚动条
    void OnSlider();

    DECLARE_MESSAGE_MAP()

    virtual void PreSubclassWindow();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};


