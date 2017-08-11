/**	@file    CSampleGrabberCBAudio.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ��ȡusb����ͷ�ص�����֡��װ��ps��
*
*	@author	 chenxiaoyan3
*	@date	 2015/07/23
*
*	@note    �����note��warningΪ��ѡ��Ŀ
*	@note    ������д���ļ�����ϸ����������ע��
*	@note    ��ʷ��¼��
*	@note    V0.0.1  ����
*
*	@warning ������д���ļ���صľ�����Ϣ
*/

#include <atlbase.h>
#include "qedit.h"
#include "dshow.h"
#include <windows.h>
#include "SystemTransform.h"
#include "GeneralDef.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
//#include "libavutil/channel_layout.h"
#include <libavutil/opt.h>
//#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
};

class CSampleGrabberCBAudio : public ISampleGrabberCB 
{
public:
    CSampleGrabberCBAudio();
    virtual ~CSampleGrabberCBAudio();

    long Init();
    long Fini();
    long SetDataCB(pMediaDataBack phTrans, CRITICAL_SECTION* pcs, void* pUser);
    //long Run();
    //long Stop();

    STDMETHODIMP_(ULONG) AddRef() { return 1; }
    STDMETHODIMP_(ULONG) Release() { return 2; }

    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject)
    {
        if (NULL == ppvObject) return E_POINTER;
        if (riid == __uuidof(IUnknown))
        {
            *ppvObject = static_cast<IUnknown*>(this);
            return S_OK;
        }
        if (riid == __uuidof(ISampleGrabberCB))
        {
            *ppvObject = static_cast<ISampleGrabberCB*>(this);
            return S_OK;
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP SampleCB(double Time, IMediaSample *pSample)
    {
        return E_NOTIMPL;
    }

    virtual STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);

private:
        pMediaDataBack   m_phTrans; //ps����װ����
        CRITICAL_SECTION *m_pcs;
        AVCodec          *m_codec;  
        AVCodecContext   *m_EncodeC;  
        AVFrame          *m_frame;  
        uint8_t          *m_samples;
        int              m_iBuffer_size;
        void             *m_pUser;
public:
        unsigned long    m_Timestamp_video;
        unsigned long    m_Timestamp_audio;
};
