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
    BOOL m_bTrace;       // �Ƿ��϶�������
    BOOL m_bLeaveTrace;  // �Ƿ��ڴ������϶�
    BOOL m_bHoverBtn;    // �Ƿ񳤰���ť
    int m_nTraceBegin;   // �϶���ʼ���λ��
    int m_nTraceMove;    // �϶���λ��
    int m_nTracePos;     // ������λ�õĸı�

    int m_nHover;       // ��ͣλ��
    int m_nClick;       // ���λ��
    int m_nUseless;     // �����ð�ť
    int m_nSliderBegin; // ��������ʼλ��
    int m_nSliderEnd;   // ����������λ��
    int m_nSliderPos;   // ������ʵ��λ�ã��м��ߣ�
    int m_nSliderHeight;// �������߶�

    int m_nScrollStyle;       // ��ֱ/ˮƽ
    Image* m_pImgUp;          // ���ϰ�ť
    Image* m_pImgDown;        // ���°�ť

    int m_nScrollWidth;         // ���������
    int m_nBtnImgWidth;         // ��ťͼƬ���

    SCROLLINFO m_siCur;         // ��������Ϣ

    void SetCurScrollInfo(const SCROLLINFO& si);
    void GetCurScrollInfo(SCROLLINFO& si);

    // ��ȡ��굱ǰ��λ��
    int HitTest(const CPoint& point);

    // ����ť
    void DrawBg(CDC* pDC);

    // ����ť
    void DrawBtn(CDC* pDC);

    // ��������
    void DrawSlider(CDC* pDC);

    // ˢ�²�������
    void InvalidateArea(int nPos);

    // ���������λ��
    void CalSliderPos();

    // �ƶ�������
    void MoveSlider(const CPoint& point);

    // ��Ӧ������
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


