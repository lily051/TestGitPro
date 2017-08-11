#include "CSampleGrabberCBAudio.h"
#include "hlogConfig.h"

CSampleGrabberCBAudio::CSampleGrabberCBAudio():
m_codec(NULL)
,m_EncodeC(NULL)
,m_frame(NULL)
,m_samples(NULL)
,m_iBuffer_size(0)
,m_pUser(NULL)
,m_Timestamp_video(0)
,m_Timestamp_audio(0)
{

}

CSampleGrabberCBAudio::~CSampleGrabberCBAudio()
{
    if (m_samples)
    {
        av_freep(&m_samples);
        m_samples = NULL;
    }

    if (m_frame)
    {
        avcodec_free_frame(&m_frame);
        m_frame = NULL;
    }

    if (m_EncodeC)
    {
        avcodec_close(m_EncodeC);
        av_free(m_EncodeC);
        m_EncodeC = NULL;
    }
}

long CSampleGrabberCBAudio::Init()
{
    int ret = 0;
    av_register_all();
    /* find the AAC encoder */ 
    m_codec = avcodec_find_encoder(CODEC_ID_AAC);  
    if (!m_codec)
    {
        TPLOG_ERROR("avcodec_find_encoder CODEC_ID_AAC failed");
        Fini();
        return -1;
    }

    m_EncodeC = avcodec_alloc_context3(m_codec);
    if (!m_EncodeC)
    {
        TPLOG_ERROR("avcodec_alloc_context3 failed");
        Fini();
        return -1;
    }

    /* put sample parameters */  
    m_EncodeC->bit_rate = 64000;
    m_EncodeC->sample_rate = 44100;
    m_EncodeC->channels = 2;
    m_EncodeC->sample_fmt = AV_SAMPLE_FMT_S16;
    m_EncodeC->codec_type = AVMEDIA_TYPE_AUDIO;
    m_EncodeC->channel_layout = AV_CH_LAYOUT_STEREO;

    ret = avcodec_open2(m_EncodeC, m_codec, NULL );
    if (ret < 0)
    {
        TPLOG_ERROR("avcodec_open2 failed:%d.",ret);
        Fini();
        return -1;
    }

    /* frame containing input raw audio */  
    m_frame = avcodec_alloc_frame();  
    if (!m_frame)
    {  
        TPLOG_ERROR("avcodec_alloc_frame failed");
        Fini();
        return -1;
    }

    m_frame->nb_samples = m_EncodeC->frame_size;  
    m_frame->format = m_EncodeC->sample_fmt;  
    m_frame->channel_layout = m_EncodeC->channel_layout;  
  
    /* the codec gives us the frame size, in samples, 
     * we calculate the size of the samples buffer in bytes */
    m_iBuffer_size = av_samples_get_buffer_size(NULL, m_EncodeC->channels, m_EncodeC->frame_size,  
            m_EncodeC->sample_fmt, 0);  
    m_samples = (uint8_t *)av_malloc(m_iBuffer_size);
    if (!m_samples)
    {  
        TPLOG_ERROR("av_malloc failed");
        Fini();
        return -1;
    }  
    /* setup the data pointers in the AVFrame */  
    ret = avcodec_fill_audio_frame(m_frame, m_EncodeC->channels, m_EncodeC->sample_fmt,  
            (const uint8_t*) m_samples, m_iBuffer_size, 0);  
    if (ret < 0)
    {  
        TPLOG_ERROR("avcodec_fill_audio_frame failed:%d",ret);
        Fini();
        return -1;
    }

    return 0;
}

long CSampleGrabberCBAudio::Fini()
{
    m_iBuffer_size = 0;
    if (m_samples)
    {
        av_freep(&m_samples);
        m_samples = NULL;
    }

    if (m_frame)
    {
        avcodec_free_frame(&m_frame);
        m_frame = NULL;
    }

    if (m_EncodeC)
    {
        avcodec_close(m_EncodeC);
        av_free(m_EncodeC);
        m_EncodeC = NULL;
    }

    return 0;
}

long CSampleGrabberCBAudio::SetDataCB(pMediaDataBack phTrans, CRITICAL_SECTION* pcs, void* pUser)
{
    m_phTrans = phTrans;
    m_pcs = pcs;
    m_pUser = pUser;
    return 0;
}

//long CSampleGrabberCBAudio::Run()
//{
//    return SYSTRANS_Start(m_phTrans, NULL, NULL);
//}

//long CSampleGrabberCBAudio::Stop()
//{
//    return SYSTRANS_Stop(m_phTrans);
//}
int itime = 0;
STDMETHODIMP CSampleGrabberCBAudio::BufferCB(double Time, BYTE *pBuffer, long BufferLen)
{
    long ret = 0;
    int got_output = 0;
    int iDatalen = 0;
    int iNum = 0;
    int iBuffsize = 0;
    AVPacket pkt;
    while (iDatalen < BufferLen)
    {
        if(iDatalen + m_iBuffer_size < BufferLen)
        {
            iBuffsize = m_iBuffer_size;
        }
        else
        {
            iBuffsize = BufferLen - iDatalen;
        }
        memcpy(m_samples, pBuffer + iNum * m_iBuffer_size, iBuffsize);
        av_init_packet(&pkt);
        pkt.data = m_samples;
        pkt.size = iBuffsize;
        iDatalen += m_iBuffer_size;
        iNum++;
        if (pkt.size == 0)
        {  
            TPLOG_INFO("pkt.size == 0");
            return 0;
        }  
        //m_frame->pts = itime;
        //itime += 10;
        ret = avcodec_encode_audio2(m_EncodeC, &pkt, m_frame, &got_output);
        if (ret < 0)
        {
            TPLOG_ERROR("Error encoding audio frame:%d",ret);
        }  
        if (got_output) {
            //static FILE *jpgFile = fopen("d:\\audio.aac", "wb+");
            //fwrite(pkt.data, pkt.size, 1, jpgFile);
            //EnterCriticalSection(m_pcs);
            //ret = SYSTRANS_InputData(m_phTrans, AUDIO_DATA, pkt.data, pkt.size);
            //LeaveCriticalSection(m_pcs); 
            //if (SYSTRANS_OK != ret)
            //{
            //    TPLOG_ERROR("SYSTRANS_InputData failed:%d",ret);
            //}
            m_phTrans(pkt.data, pkt.size, m_pUser);
            av_free_packet(&pkt);
        }
    }
    av_free_packet(&pkt);
    return E_NOTIMPL;
}