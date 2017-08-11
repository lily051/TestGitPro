#if !defined(AFX_SCOPE1_H__20373609_0B81_493C_8F94_B4644298A84F__INCLUDED_)
#define AFX_SCOPE1_H__20373609_0B81_493C_8F94_B4644298A84F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif
#include <list>

#define     BACKCOLOR       RGB(0,0,0)      //������ɫ
#define     WAVECOLOR       RGB(255,0,0)    //������ɫ
#define     MAXFRAMENUM     10              //������Ƶ֡��
#define     MAXVAULENUM     1024            //���������
#define     MAXVAULE        100             //�������ֵ

typedef struct _tag_audio_frame 
{
    long    nFrameRate;     //����Ƶ��
    long    nStamp;         //��ʼʱ��
    int     nType;          //����λ��
    long    nSize;          //���ݳ���
    char    *pBuf;          //��Ƶ����
    _tag_audio_frame()
    {
        nFrameRate = 0;
        nStamp = 0;
        nType = 8;
        nSize = 0;
        pBuf = NULL;
    }
}AUDIOFRAME;

class CScope : public CStatic
{
public:
	CScope();
    virtual ~CScope();
    virtual void PreSubclassWindow();
    //��ʾ����
    BOOL OpenScope();
    //�رղ���
    BOOL CloseScope();
    //�Ƿ��ʾ����
    inline BOOL IsOpen()
    {
        return (NULL == m_hThread)?FALSE:TRUE;
    }
    //�����Ƶ֡����
    void AddAudioFrame(AUDIOFRAME audioFrame);
    //���÷Ŵ��� nZoomΪ(1-10)
    BOOL SetZoomNumber(int nZoom);
    //��Ƶ֡���ݴ�����(�ڲ�ʹ��)
    void DealAudioFrame();
protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
private:
    CRect   m_rectClient;                   //�ؼ�Rect
    HANDLE  m_hThread;                      //��Ƶ֡���ݴ����߳�
    BOOL    m_bShowWave;                    //��ʾ����
    std::list<AUDIOFRAME> m_listAudioFrame; //��Ƶ֡�����б�
    CRITICAL_SECTION m_csListAudio;         //��Ƶ֡�����б���
    int     m_Vaule[MAXVAULENUM];           //����ֵ100�ȷ�
    int     m_nZoom;                        //�Ŵ���
    void    DrawVaule(CDC *pDC);            //����Ƶ����
};

#endif 
