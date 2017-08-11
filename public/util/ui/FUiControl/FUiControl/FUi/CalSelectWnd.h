#pragma once

#include "FButton.h"
// CCalSelectWnd

#define WM_CALSELWND_CLOSE (WM_USER + 300)  // ���ڹر���Ϣ

// ʱ��״̬
#define TIME_STATE_NORMAL  0x00      // ����
#define TIME_STATE_UNUSE   0x01      // ����
#define TIME_STATE_HOVER   0x04      // ��ͣ
#define TIME_STATE_CUR     0x08      // ��ǰ
#define TIME_STATE_SELECT  0x10      // ѡ��

// ʱ��ѡ�񴰿ڵ���ʽ
enum select_wnd_style
{
    SWS_YEAR = 0,   // ��
    SWS_MONTH,      // ��
    SWS_HOUR,      // ʱ
    SWS_MINI,      // ��
    SWS_SECD,      // ��
};

// ʱ����Ϣ
typedef struct time_info
{
    int nState;       // ״̬
    int nTime;        // ����
    CRect rect;       // ����
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

    // ������ʽ
    void SetStyle(int nStyle);
    // ���õ�ǰ���
    void SetCurYear(int nYear);

protected:
    int m_nPreStyle;  // ԭ��ʽ�����ڸı�ʱ����Ϣ
    int m_nStyle;     // ��ʽ
    int m_nCurYear;   // ��ǰ���
    int m_nTimeNum;   // ʱ����Ϣ�ĸ���
    int m_nHover;     // �����ͣ��ʱ��
    int m_nSelect;    // ���ѡ����ʱ��
    int m_nSelTime;   // ѡ���ʱ����Ϣ
    CRect m_rcDraw;  // ��Ҫ��ʱ����Ϣ������
    CFButton* m_pbtnPre;
    CFButton* m_pbtnNext;

    // �洢ʱ����Ϣ������
    STRU_TIME_INFO m_strArrayTime[60 + 1];
    // ����ʱ����Ϣ
    void UpdataTimeInfo(int nStyle);
    // ����ͬ��ʽ
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


