/**   @file	    CDLayer.h
 *    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �ṩ�豸����ͳһ�ӿ�(��������)
 *
 *    @author   ZhangYunjun
 *    @date     2014/08/11
 *
 *    @note     Description
 *    @note     ��ʷ��¼��
 *    @note     V1.0.0:details
 *
 *    @warning 
 */

#pragma once
#ifndef __CDLAYER_H_
#define __CDLAYER_h_

#ifdef CDLAYER_EXPORTS
#define CDLAYER_API __declspec(dllexport)
#else
#define CDLAYER_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    enum _data_len
    {
        IPV4_LEN = 16,
        USER_LEN = 32,
        PWD_LEN  = 32,
    };

    // �豸ͨ������
    typedef struct _DeviceChannelInfo 
    {
        char szIp[IPV4_LEN];
        int  nPort;
        char szUser[USER_LEN];
        char szPwd[PWD_LEN];
        int  nDevType;      // 0:hik, 1:dahua
    }CD_DEV_CHAN_INFO;

    // ��̨���Ʋ���
    typedef struct _PtzParam
    {
        int  nChannel;
        DWORD dwCmd;    // ���� streamlayer����ķ�ʽ
        DWORD dwStop;
        DWORD dwSpeed;
    }CD_PTZ_PARAM;

    typedef struct _PtzZoomParam
    {
        int nChannel;
        int nXTop;
        int nYTop;
        int nXBottom;
        int nYBottom;
        int nBCounter;
    }CD_PTZ_ZOOM_PARAM;
    
    // ��Ƶ����
    typedef struct _EffectParam 
    {
        int  nChannel;
        DWORD dwBright;
        DWORD dwContrast;
        DWORD dwSaturation;
        DWORD dwHue;
    }CD_EFFECT_PARAM;

    // ��ͨ����ҳ����
    typedef struct _ZeroPageOper 
    {
        int nChannel;
        BOOL bNextPreview;  // TRUE-��һҳ��FALSE-��һҳ 
    }CD_ZERO_OPER;

    /** @fn        CDLayer_Startup
    *  @brief      ��ȡ��ǰԤ��ͨ����ѹ��������Ϣ
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_Startup(void);

    /** @fn        CDLayer_Cleanup
    *  @brief      ��ȡ��ǰԤ��ͨ����ѹ��������Ϣ
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_Cleanup(void);

    /** @fn        CDLayer_SetVideoParam
    *  @brief      ������Ƶ����
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_SetVideoParam(const CD_DEV_CHAN_INFO& dci, const CD_EFFECT_PARAM& ep);

    /** @fn        CDLayer_GetVideoParam
    *  @brief      ��ȡ��Ƶ����
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_GetVideoParam(const CD_DEV_CHAN_INFO& dci, CD_EFFECT_PARAM& ep);

    /** @fn        CDLayer_PtzControl
    *  @brief      ��̨����
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_PtzControl(const CD_DEV_CHAN_INFO& dci, const CD_PTZ_PARAM& pp);

    /** @fn        CDLayer_PTZSelZoomIn
    *  @brief      ��̨ͼ������ѡ��Ŵ����С
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_PTZSelZoomIn(const CD_DEV_CHAN_INFO& dci, const CD_PTZ_ZOOM_PARAM& pzp);

    /** @fn        CDLayer_ZeroTurnOver
    *  @brief      ��ͨ����ҳ
    *  @param[IN]  hStream: SLStartStream���صľ��
    *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
    *  @return     TRUE: �ɹ�, FALSE: ʧ��
    */
    CDLAYER_API BOOL CDLayer_ZeroTurnOver(const CD_DEV_CHAN_INFO& dci, const CD_ZERO_OPER& zo);



#ifdef __cplusplus
};
#endif


#endif