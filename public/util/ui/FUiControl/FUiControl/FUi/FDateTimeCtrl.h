#pragma once

#define WM_SELTIME (WM_USER + 200)  // �ı�ѡ�е�ʱ������Ϣ

enum dtctrl_style
{
    FDTS_CAL = 0,  // ������ʽ
    FDTS_TIME,     // ʱ����ʽ
};

enum select_time_rect
{
    ST_HOUR = 0,  // ʱ����
    ST_MINI,      // ������
    ST_SECD,      // ������
};
// CFDateTimeCtrl

class AFX_EXT_CLASS CFDateTimeCtrl : public CDateTimeCtrl
{
    DECLARE_DYNAMIC(CFDateTimeCtrl)

public: 
    CFDateTimeCtrl();
    virtual ~CFDateTimeCtrl();

    // ����ʱ��ؼ�����ʽ
    void SetStyle(int nStyle);
    // ������������ʽ
    void SetCalStyle(int nCalStyle);

protected:

    bool m_bEnable;     // �Ƿ����
    bool m_isSelTime;   // �Ƿ�ѡ��ʱ��
    int m_nStyle;       // ʱ��ؼ���ʽ
    int m_nCalStyle;    // �����ؼ���ʽ
    int m_nSelTimeRect; // ѡ�е�ʱ������ʱ���֡���

    CRect m_rcMask;     // ����ϵͳ����������
    CRect m_rcCal;      // ������ť����
    CRect m_rcTime;     // ʱ�䰴ť����
    CRect m_rcHour;     // ʱ����
    CRect m_rcMini;     // ������
    CRect m_rcSecd;     // ������
    Image* m_pImgCal;   // ������ťͼ��
    Image* m_pImgTime;  // ʱ�䰴ťͼ��
    Image* m_pImgTranBkg;   // ͸������

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


