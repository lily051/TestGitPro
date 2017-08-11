#pragma once

#include "../FUiControl.h"
#include "FScrollBar.h"
// CFScrollWnd

class CFScrollWnd : public CWnd
{
    DECLARE_DYNAMIC(CFScrollWnd)

public:
    CFScrollWnd();
    virtual ~CFScrollWnd();

    BOOL m_bOpering;
    int m_nScrollWidth;
    int m_nAngleType;     // 滚动条组合类型

    CFScrollBar m_sbVert;
    CFScrollBar m_sbHorz;
    WNDPROC m_funOldProc;

    void SkinScroll(CWnd *pWnd);

protected:
    CWnd m_wndLimit;   // 用于屏蔽系统的滚动条

    DECLARE_MESSAGE_MAP()

    LRESULT OnDestMove(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


FUICONTROL_API CFScrollWnd* SkinWndScroll(CWnd *pWnd);

FUICONTROL_API BOOL UnskinWndScroll(CWnd *pWnd);
