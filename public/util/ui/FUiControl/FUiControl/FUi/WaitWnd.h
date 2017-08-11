
#pragma once
#include "FuiBase.h"
#include <vector>


class CWaitWnd : public CWnd
{
	DECLARE_DYNAMIC(CWaitWnd)

public:
	CWaitWnd();
	virtual ~CWaitWnd();

protected:
	DECLARE_MESSAGE_MAP()


public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    // ���ڴ�С
    CRect m_rcWnd;  
    // ����ͼƬ������ͼƬ
    Image *m_pImgWait, *m_pImgBkg;
    // ����
    CString m_strText;
    // ������ʾ����
    CRect m_rcText;
    // ����ͼƬƫ��
    int m_nImageOffset;
    // ����ͼƬ��ʾ����
    CRect m_rcImage;

    // �Ź��񱳾�ͼƬ�ָ�
    std::vector<RectF> m_vecRcfBkg;
    // �Ź��񴰿ڷָ�
    std::vector<RectF> m_vecRcfWnd;
    
    // ����ˢ���߳�
    HANDLE m_hExit, m_hThread;

    // ˢ�½���
    void SplashStep(CDC* pDC);
    // �ػ汳��
    void DrawBkg(CDC* pDC);
public:
    // ��������
    BOOL CreateWnd(CWnd* pParent);
    // ��ʾ����
    BOOL ShowWaitWnd(LPCTSTR szText);
    // ˢ���̵߳��ú���
    unsigned UpdateImage();
};


