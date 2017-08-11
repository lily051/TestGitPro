#if !defined(AFX_SCOPE1_H__20373609_0B81_493C_8F94_B4644298A84F__INCLUDED_)
#define AFX_SCOPE1_H__20373609_0B81_493C_8F94_B4644298A84F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif
#include <list>

#define     BACKCOLOR       RGB(0,0,0)      //背景颜色
#define     WAVECOLOR       RGB(255,0,0)    //波形颜色
#define     MAXFRAMENUM     10              //缓存音频帧数
#define     MAXVAULENUM     1024            //保存采样数
#define     MAXVAULE        100             //采样最大值

typedef struct _tag_audio_frame 
{
    long    nFrameRate;     //采样频率
    long    nStamp;         //开始时间
    int     nType;          //数据位数
    long    nSize;          //数据长度
    char    *pBuf;          //音频数据
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
    //显示波形
    BOOL OpenScope();
    //关闭波形
    BOOL CloseScope();
    //是否打开示波器
    inline BOOL IsOpen()
    {
        return (NULL == m_hThread)?FALSE:TRUE;
    }
    //添加音频帧数据
    void AddAudioFrame(AUDIOFRAME audioFrame);
    //设置放大倍数 nZoom为(1-10)
    BOOL SetZoomNumber(int nZoom);
    //音频帧数据处理函数(内部使用)
    void DealAudioFrame();
protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
private:
    CRect   m_rectClient;                   //控件Rect
    HANDLE  m_hThread;                      //音频帧数据处理线程
    BOOL    m_bShowWave;                    //显示波形
    std::list<AUDIOFRAME> m_listAudioFrame; //音频帧数据列表
    CRITICAL_SECTION m_csListAudio;         //音频帧数据列表锁
    int     m_Vaule[MAXVAULENUM];           //采样值100等分
    int     m_nZoom;                        //放大倍数
    void    DrawVaule(CDC *pDC);            //画音频波形
};

#endif 
