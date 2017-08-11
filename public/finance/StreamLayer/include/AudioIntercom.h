/**	@File   AUDIOINTERCOM.H
 *	@Note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@Brief  ��Ƶ�Խ���

 *	@Author zhangxinyf3 ����
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

///<������
#define ERROR_NO            0      ///<�޴���
#define ERROR_SUPPORT       1      ///<���Ͳ�֧��
#define ERROR_ALLOC_MEMORY  2      ///<�ڴ�������
#define ERROR_PARAMETER     3      ///<���ò�������
#define ERROR_CALL_ORDER    4      ///<����˳�����
#define ERROR_FIND_DEVICE   5      ///<�����ҵ��豸
#define ERROR_OPEN_DEVICE   6      ///<���ܴ��豸
#define ERROR_NO_CONTEXT    7      ///<�豸�����ĳ���
#define ERROR_NO_WAVEFILE   8      ///<WAV�ļ�����
#define ERROR_INVALID_TYPE  9      ///<��Ч��WAV��������
#define ERROR_ENCODE_FAIL   10     ///<����ʧ��
#define ERROR_DECODE_FAIL   11     ///<����ʧ��
#define ERROR_NO_PLAYBACK   12     ///<����ʧ��
#define ERROR_DENOISE_FAIL  13     ///<����ʧ��
#define ERROR_UNKNOWN       99     ///<δ֪����

///<�����豸��Ϣ�ṹ��
typedef struct _SOUND_CARD_INFO
{
    char           byDeviceName[128];///<�豸����
    unsigned int   dwFrequency;      ///<�ɼ�Ƶ��
    unsigned int   dwRefresh;        ///<ˢ��Ƶ��
    unsigned int   dwSync;           ///<ͬ��
    unsigned int   dwMonoSources;    ///<������Դ����
    unsigned int   dwStereoSources;  ///<������Դ����
    unsigned int   dwMajorVersion;   ///<���汾��
    unsigned int   dwMinorVersion;   ///<�ΰ汾��
    unsigned int   dwReserved[16];   ///<��������
}SoundCardInfo;

///<��Ƶ������ʽ
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

///<�ص��������������Ϣ�ṹ��
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

/**********************************��ȡ������Ϣ*******************************************************************************/

///<��ȡ��������
AUDIOCOM_API int __stdcall AUDIOCOM_GetSoundCardNum(unsigned int* pdwDeviceNum);

///<��ȡָ���������豸��Ϣ
AUDIOCOM_API int __stdcall AUDIOCOM_GetOneSoundCardInfo(unsigned int dwDeviceIndex, SoundCardInfo* pstDeviceInfo);

/**********************************�ɼ�����ģ��ӿ�***************************************************************************/

///<����ָ���豸���ƣ������ɼ�����ģ��˿ں�
AUDIOCOM_API int __stdcall AUDIOCOM_CreateCaptureHandle(int* pnCapturePort, const char* pDeviceName);

///<ע��ص�����������ָ����ͬ��ʽ�ı������ݣ���ͨ���ص��������
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterOutputDataCallBack(int nCapturePort, AudioEncodeType enDataType, OutputDataCallBack pfnOutputDataCallBack, void* pUser);

///<��ʼ�ɼ�����
AUDIOCOM_API int __stdcall AUDIOCOM_StartCapture(int nCapturePort);

///<ֹͣ�ɼ�����
AUDIOCOM_API int __stdcall AUDIOCOM_StopCapture(int nCapturePort);

///<ע��ɼ�����ǰ�ص�����
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterCaptureDataCallBack(int nCapturePort, OutputDataCallBack pfnCaptureDataCallBack, void* pUser);

///<�ͷŲɼ�����ģ��˿ں�
AUDIOCOM_API int __stdcall AUDIOCOM_ReleaseCaptureHandle(int nCapturePort);

/**********************************���Ž���ģ��ӿ�**************************************************************************/

///<����ָ���豸���ƣ��������Ž���ģ��˿ں�
AUDIOCOM_API int __stdcall AUDIOCOM_CreatePlayHandle(int* pnPlayPort, const char* pDeviceName);
/*
///<ָ���ļ�·������WAV�ļ�
AUDIOCOM_API int __stdcall AUDIOCOM_OpenWaveFile(int nPlayPort, const char* pFilePath);
*/
///<ָ�������ͣ�����
AUDIOCOM_API int __stdcall AUDIOCOM_OpenStream(int nPlayPort, AudioEncodeType enDataType);

///<����������
AUDIOCOM_API int __stdcall AUDIOCOM_InputStreamData(int nPlayPort, unsigned char* pData, unsigned int dwDataLen);

///<��ʼ���벥��
AUDIOCOM_API int __stdcall AUDIOCOM_StartPlay(int nPlayPort);

///<ֹͣ���벥��
AUDIOCOM_API int __stdcall AUDIOCOM_StopPlay(int nPlayPort);

///<��������ݻص�
AUDIOCOM_API int __stdcall AUDIOCOM_RegisterDecodeDataCallBack(int nPlayPort, OutputDataCallBack pfnDecodeDataCallBack, void* pUser);

///<��������
AUDIOCOM_API int __stdcall AUDIOCOM_SetVolume(int nPlayPort, float fVolume);

///<��ȡ����
AUDIOCOM_API int __stdcall AUDIOCOM_GetVolume(int nPlayPort, float* fVolume);

///<�ͷŲ��Ž���ģ��˿ں�
AUDIOCOM_API int __stdcall AUDIOCOM_ReleasePlayHandle(int nPlayPort);

/******************************************************************************************************************************/

///<��ȡ������Ϣ
AUDIOCOM_API int __stdcall AUDIOCOM_GetLastError(int nPort);

///<��ȡ�汾��
AUDIOCOM_API unsigned int __stdcall AUDIOCOM_GetVersion();

#ifdef __cplusplus
}
#endif


#endif//_AUDIO_INTERCOM_H_