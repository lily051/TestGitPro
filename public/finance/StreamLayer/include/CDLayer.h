/**   @file	    CDLayer.h
 *    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    提供设备操作统一接口(海康、大华)
 *
 *    @author   ZhangYunjun
 *    @date     2014/08/11
 *
 *    @note     Description
 *    @note     历史记录：
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

    // 设备通道参数
    typedef struct _DeviceChannelInfo 
    {
        char szIp[IPV4_LEN];
        int  nPort;
        char szUser[USER_LEN];
        char szPwd[PWD_LEN];
        int  nDevType;      // 0:hik, 1:dahua
    }CD_DEV_CHAN_INFO;

    // 云台控制参数
    typedef struct _PtzParam
    {
        int  nChannel;
        DWORD dwCmd;    // 参照 streamlayer传入的方式
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
    
    // 视频参数
    typedef struct _EffectParam 
    {
        int  nChannel;
        DWORD dwBright;
        DWORD dwContrast;
        DWORD dwSaturation;
        DWORD dwHue;
    }CD_EFFECT_PARAM;

    // 零通道翻页操作
    typedef struct _ZeroPageOper 
    {
        int nChannel;
        BOOL bNextPreview;  // TRUE-下一页；FALSE-上一页 
    }CD_ZERO_OPER;

    /** @fn        CDLayer_Startup
    *  @brief      获取当前预览通道的压缩参数信息
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_Startup(void);

    /** @fn        CDLayer_Cleanup
    *  @brief      获取当前预览通道的压缩参数信息
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_Cleanup(void);

    /** @fn        CDLayer_SetVideoParam
    *  @brief      设置视频参数
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_SetVideoParam(const CD_DEV_CHAN_INFO& dci, const CD_EFFECT_PARAM& ep);

    /** @fn        CDLayer_GetVideoParam
    *  @brief      获取视频参数
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_GetVideoParam(const CD_DEV_CHAN_INFO& dci, CD_EFFECT_PARAM& ep);

    /** @fn        CDLayer_PtzControl
    *  @brief      云台控制
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_PtzControl(const CD_DEV_CHAN_INFO& dci, const CD_PTZ_PARAM& pp);

    /** @fn        CDLayer_PTZSelZoomIn
    *  @brief      云台图象区域选择放大或缩小
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_PTZSelZoomIn(const CD_DEV_CHAN_INFO& dci, const CD_PTZ_ZOOM_PARAM& pzp);

    /** @fn        CDLayer_ZeroTurnOver
    *  @brief      零通道翻页
    *  @param[IN]  hStream: SLStartStream返回的句柄
    *  @param[OUT] stCompressionInfo: 压缩参数信息
    *  @return     TRUE: 成功, FALSE: 失败
    */
    CDLAYER_API BOOL CDLayer_ZeroTurnOver(const CD_DEV_CHAN_INFO& dci, const CD_ZERO_OPER& zo);



#ifdef __cplusplus
};
#endif


#endif