/**	@File   AUDIOINTERCOM.H
 *	@Note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@Brief  音频对讲库

 *	@Author zhangxinyf3 张欣
 *	@Date   2012/4/9
 *  @Ver    1.1.0
 *  @Remark 
 */

#ifndef _AUDIO_INTERCOM_H_
#define _AUDIO_INTERCOM_H_

#ifdef WIN32
    #if defined(_WINDLL)
        #define AUDIOCOM_API  __declspec(dllexport) 
    #else 
        #define AUDIOCOM_API  __declspec(dllimport) 
    #endif
#else
    #ifndef __stdcall
        #define __stdcall
    #endif

    #ifndef AUDIOCOM_API
        #define AUDIOCOM_API
    #endif
#endif

///<错误码
#define ERROR_NO            0      ///<无错误
#define ERROR_SUPPORT       1      ///<类型不支持
#define ERROR_ALLOC_MEMORY  2      ///<内存分配出错
#define ERROR_PARAMETER     3      ///<调用参数出错
#define ERROR_CALL_ORDER    4      ///<调用顺序出错
#define ERROR_FIND_DEVICE   5      ///<不能找到设备
#define ERROR_OPEN_DEVICE   6      ///<不能打开设备
#define ERROR_NO_CONTEXT    7      ///<设备上下文出错
#define ERROR_NO_WAVEFILE   8      ///<WAV文件出错
#define ERROR_INVALID_TYPE  9      ///<无效的WAV参数类型
#define ERROR_ENCODE_FAIL   10     ///<编码失败
#define ERROR_DECODE_FAIL   11     ///<解码失败
#define ERROR_NO_PLAYBACK   12     ///<播放失败
#define ERROR_DENOISE_FAIL  13     ///<降噪失败
#define ERROR_UNKNOWN       99     ///<未知错误

///<声卡设备信息结构体
typedef struct _SOUND_CARD_INFO
{
    char           byDeviceName[128];///<设备名称
    unsigned int   dwFrequency;      ///<采集频率
    unsigned int   dwRefresh;        ///<刷新频率
    unsigned int   dwSync;           ///<同步
    unsigned int   dwMonoSources;    ///<单声道源数量
    unsigned int   dwStereoSources;  ///<多声道源数量
    unsigned int   dwMajorVersion;   ///<主版本号
    unsigned int   dwMinorVersion;   ///<次版本号
    unsigned int   dwReserved[16];   ///<保留参数
}SoundCardInfo;

///<音频编解码格式
typedef enum _AUDIO_ENCODE_TYPE
{
    AUDIO_TYPE_PCM_S16K    = 0x00,
    AUDIO_TYPE_G711A_S8K   = 0x01,
    AUDIO_TYPE_G711U_S8K   = 0x02,
    AUDIO_TYPE_G722_S16K   = 0x03,
    AUDIO_TYPE_G726_S8K    = 0x04,
    AUDIO_TYPE_MPEG2_S16K  = 0x05,
    AUDIO_TYPE_AAC_S32K    = 0x06
}AudioEncodeType;

///<回调函数输出数据信息结构体
typedef struct _OUTPUT_DATA_INFO 
{
    unsigned char*    pData;
    unsigned int      dwDataLen;
    AudioEncodeType   enDataType;
} OutputDataInfo;

typedef void (__stdcall* OutputDataCallBack)(OutputDataInfo* pstDataInfo,void* pUser);

#ifdef __cplusplus
extern "C" {
#endif

/**********************************获取声卡信息*******************************************************************************/

///<获取声卡数量
AUDIOCOM_API int __stdcall AUDIOCOM_GetSoundCardNum(unsigned int* pdwDeviceNum);

///<获取指定声卡的设备信息
AUDIOCOM_API int __stdcall AUDIOCOM_GetOneSoundCardInfo(unsigned int dwDeviceIndex, SoundCardInfo* pstDeviceInfo);

/**********************************采集编码模块接口***************************************************************************/

///<根据指定设备名称，创建采集编码模块端口号
AUDIOCOM_API int __stdcall AUDIOCOM_CreateCaptureHandle(int* pnCapturePort, const char* pDeviceName);

///<注册回调函数，可以指定不同格式的编码数据，并通过回调函数输出
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterOutputDataCallBack(int nCapturePort, AudioEncodeType enDataType, OutputDataCallBack pfnOutputDataCallBack, void* pUser);

///<开始采集编码
AUDIOCOM_API int __stdcall AUDIOCOM_StartCapture(int nCapturePort);

///<停止采集编码
AUDIOCOM_API int __stdcall AUDIOCOM_StopCapture(int nCapturePort);

///<注册采集编码前回调函数
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterCaptureDataCallBack(int nCapturePort, OutputDataCallBack pfnCaptureDataCallBack, void* pUser);

///<释放采集编码模块端口号
AUDIOCOM_API int __stdcall AUDIOCOM_ReleaseCaptureHandle(int nCapturePort);

/**********************************播放解码模块接口**************************************************************************/

///<根据指定设备名称，创建播放解码模块端口号
AUDIOCOM_API int __stdcall AUDIOCOM_CreatePlayHandle(int* pnPlayPort, const char* pDeviceName);
/*
///<指定文件路径，打开WAV文件
AUDIOCOM_API int __stdcall AUDIOCOM_OpenWaveFile(int nPlayPort, const char* pFilePath);
*/
///<指定流类型，打开流
AUDIOCOM_API int __stdcall AUDIOCOM_OpenStream(int nPlayPort, AudioEncodeType enDataType);

///<输入流数据
AUDIOCOM_API int __stdcall AUDIOCOM_InputStreamData(int nPlayPort, unsigned char* pData, unsigned int dwDataLen);

///<开始解码播放
AUDIOCOM_API int __stdcall AUDIOCOM_StartPlay(int nPlayPort);

///<停止解码播放
AUDIOCOM_API int __stdcall AUDIOCOM_StopPlay(int nPlayPort);

///<解码后数据回调
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterDecodeDataCallBack(int nPlayPort, OutputDataCallBack pfnDecodeDataCallBack, void* pUser);

///<设置音量
AUDIOCOM_API int __stdcall AUDIOCOM_SetVolume(int nPlayPort, float fVolume);

///<获取音量
AUDIOCOM_API int __stdcall AUDIOCOM_GetVolume(int nPlayPort, float* fVolume);

///<释放播放解码模块端口号
AUDIOCOM_API int __stdcall AUDIOCOM_ReleasePlayHandle(int nPlayPort);

/******************************************************************************************************************************/

///<获取错误信息
AUDIOCOM_API int __stdcall AUDIOCOM_GetLastError(int nPort);

///<获取版本号
AUDIOCOM_API unsigned int __stdcall AUDIOCOM_GetVersion();

#ifdef __cplusplus
}
#endif


#endif//_AUDIO_INTERCOM_H_