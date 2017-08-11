/*
*    @file        USBCamera.h
*    @note        HANGZHOU Hikvision Software Co.,Ltd.All Right Reserved.
*    @brief        USBCamera header file
*
*    @author        Hikvision
*
*    @date        
*                create:        2015/08/01
*                update:        2016/03/29    添加音视频设备过滤
*                update：    2016/04/08    添加显示回调函数
*                update:        2016/05/07
*                update：    2016/05/09    图像参数控制
*                update:      2016/07/28   修改SDK接口
*    
*    @note
*
*    @warning    Windows 32bit version
*/

#pragma once
#include <mmsystem.h>
#ifdef USBCAMERADLL_EXPORTS
#define USBCAMERADLL extern "C" __declspec(dllexport)
#else 
#define USBCAMERADLL extern "C" __declspec(dllimport)
#endif

//视频帧类型
#define USBCAMERA_UNKNOWN        -1    // 未知类型，异常
#define USBCAMERA_YUY2            1
//#define USBCAMERA_YUV420        2        // Y0 Y1 Y2 Y3......U0 U1 U2 U3......V0 V1 V2 V3......
#define USBCAMERA_YV12            3        // Y0 Y1 Y2 Y3......V0 V1 V2 V3......U0 U1 U2 U3.......
#define USBCAMERA_NV12            4
#define USBCAMERA_RGB24           6
#define USBCAMERA_RGB32           7
#define USBCAMERA_RGB565          8
#define USBCAMERA_RGB555          9
#define USBCAMERA_MJPEG          10
#define USBCAMERA_H264           11
#define USBCAMERA_PS_H264        12  //PS码流只封装H264视频
#define USBCAMERA_PS_H264_PCM    13  //PS码流封装H264视频和PCM音频

//音频类型
#define USBCAMERA_PCM            1
#define USBCAMERA_AUDIO16        31        // 解码后音频（采样率16khz、单通道、采样位数16，具体以解码后的音频信息为准）
#define USBCAMERA_AUDIO8         32        // 解码后音频


#define USBCAMERA_VIDEO            100        // video
#define USBCAMERA_AUDIO            101        // audio

// usbcamera device status
#define USBCAMERA_COMPLETE            0x01        // Complete
#define USBCAMERA_USERABORT           0x02        // user abort
#define USBCAMERA_ERRORABORT          0x03        // error abort
#define USBCAMERA_DEVICE_LOST         0x1f        // 0--remove  1--- add

//设备型号
#define USBCAMERA_ALL                300        // 注册所有设备
#define USBCAMERA_DS_2CS5432B        301        // DS-2CS5432B-S  第一款USB Camera
#define USBCAMERA_DS_2CS54D2B        302        // DS-2CS54D2B、DS-2CS54D2B-S
#define USBCAMERA_DS_2CS54C2B        303        // DS-2CS54C2B、DS-2CS54C2B-S
#define USBCAMERA_DS_2CS54D0B        304        // DS-2CS54D0B-S

//LED与蜂鸣器控制
#define USB_CAMERA_LED_BEEP_CLOSE     0
#define USB_CAMERA_LED_BLUE           1
#define USB_CAMERA_LED_RED            2
#define USB_CAMERA_LED_BLUE_BEEP_12   3
#define USB_CAMERA_LED_RED_BEEP_12    4
#define USB_CAMERA_LED_BLUE_BEEP_L    5
#define USB_CAMERA_LED_RED_BEEP_L     6

// 图像参数设置
#define USBCAMERA_Brightness                0        // brightness
#define USBCAMERA_Contrast                  USBCAMERA_Brightness + 1        // contrast
#define USBCAMERA_Hue                       USBCAMERA_Contrast    + 1            // Hue
#define USBCAMERA_Saturation                USBCAMERA_Hue + 1
#define USBCAMERA_Sharpness                 USBCAMERA_Saturation + 1
#define USBCAMERA_Gama                      USBCAMERA_Sharpness + 1
#define USBCAMERA_ColorEnable               USBCAMERA_Gama + 1
#define USBCAMERA_WhiteBalance              USBCAMERA_ColorEnable + 1        // 白平衡
#define USBCAMERA_BacklightCompensation     USBCAMERA_WhiteBalance + 1    // 背光补偿
#define USBCAMERA_Gain                      USBCAMERA_BacklightCompensation + 1

// 图像参数控制方式
#define USBCAMERA_Flag_Auto            1        // 自动
#define USBCAMERA_Flag_Manual          2        // 手动

// SDK错误码
#define USBCAMERA_FILE_NOT_FOUND        1            // 文件不存在
// 图像参数（设备可能不支持某些功能）
#define USBCAMERA_PROP_ID_UNSOPPORTED    2            // the devie does not support this property
#define USBCAMERA_INVALID_PORT           -1            // 无效端口号


typedef struct _CameraCapacity
{
    int          nIndex;
    int          nType;             // 码流类型（USBCamera_H264、USBCamera_MJPEG、USBCamera_RGB24等）
    DWORD        dwWidth;           // 图像宽度
    DWORD        dwHeight;          // 图像高度
    LONG         lListSize;         // 支持的帧率数量
    LONGLONG     *pFrameRates;      // 支持的帧率
} CAMERA_CAPACITY;

typedef struct  _CameraProperty 
{
    int          nType;        // 码流类型
    DWORD        dwWidth;        // 图像宽度
    DWORD        dwHeight;        // 图像高度
    LONG         lFrameRate;    // 帧率
} CAMERA_PROPERTY;

typedef struct _DISPLAY_INFO
{
    LONG            lPort;            // 播放通道号
    int             nType;            // 数据类型（USBCamera_YUY2、USBCamera_YV12）
    unsigned char*  pBuf;            // 图像数据指针
    DWORD           dwBufLen;        // 图像数据大小
    DWORD           dwWidth;            // 画面宽
    DWORD           dwHeight;        // 画面高
    LONG            lStamp;            // 时标信息，单位毫秒（暂时未用到）
    void*           pUser;            // 用户数据
} DISPLAY_INFO;

typedef struct _FRAME_INFO
{
    int         nType;        // 数据类型
    DWORD       dwWidth;        // 画面宽，如果是音频数据则为音频声道数
    DWORD       dwHeight;    // 画面高，如果是音频数据则为采样位数
    LONG        lStamp;        // 时标信息，单位毫秒
    LONG        lFrameRate;    // 编码时产生的图像帧率，如果是音频数据则为采样率
    LONG        lFrameNum;    // 帧号
} FRAME_INFO;

typedef struct _RECORD_DATA_INFO
{
    int                 nType;           // 数据类型（如文件头，视频，音频，私有数据）
    LONG                lStamp;          // 时间戳
    LONG                lFrameNum;       // 帧号
    LONG                nBufLen;         // 数据长度
    unsigned char*      pBuf;            // 帧数据，以帧为单位回调        
} RECORD_DATA_INFO;

typedef void (__stdcall *DecCallBack)(LONG lPort, unsigned char *pBuf, LONG nSize, FRAME_INFO *pFrameInfo, void* pUser);

typedef struct tagDEC_CALLBACK_INFO
{
    LONG lPort;
    DWORD dwVideoFormat; //支持MJPEG/H264/PS
    DWORD dwAudioFormat; //0-不回调音频，1-回调PCM
    DecCallBack fnDevCb;
    void* pUser;
} DEC_CALLBACK_INFO, *LPDEC_CALLBACK_INFO;

#define MAX_LEN_OSD_CONTENT 40
#define MAX_NUM_OSD         16

typedef struct tagUSB_CAM_OSD_INFO
{
    WORD wCoordinateX;
    WORD wCoordinateY;
    char szContent[MAX_LEN_OSD_CONTENT];
}USB_CAM_OSD_INFO, *LPUSB_CAM_OSD_INFO;

typedef struct tagUSB_CAM_OSD_LIST
{
    DWORD dwSize;
    BYTE  byEnabled;
    BYTE  byFontSize;
    DWORD dwOSDNums;
    USB_CAM_OSD_INFO struOSDInfo[MAX_NUM_OSD];
}USB_CAM_OSD_LIST, *LPUSB_CAM_OSD_LIST;

//------------------------------------------------------------------------------------------------
USBCAMERADLL BOOL __stdcall USBCamera_InitSDK();

USBCAMERADLL BOOL __stdcall USBCamera_UninitSDK();

USBCAMERADLL BOOL  __stdcall USBCamera_SetLogToFile(DWORD dwLogLevel, char const *pLogDir, BOOL bAutoDel);

USBCAMERADLL DWORD __stdcall USBCamera_GetSdkVersion();

USBCAMERADLL DWORD __stdcall USBCamera_GetLastError();

USBCAMERADLL BOOL __stdcall USBCamera_SetFilter(DWORD dwFilter);

USBCAMERADLL BOOL __stdcall USBCamera_EnumVideoDevice(int *pVideoDevNum);

USBCAMERADLL BOOL __stdcall USBCamera_EnumAudioDevice(int *pAudioDevNum);

USBCAMERADLL BOOL __stdcall USBCamera_GetVideoDevice(int nIndex, HANDLE *hDevice);

USBCAMERADLL BOOL __stdcall USBCamera_GetAudioDevice(int nIndex, HANDLE *hDevice);

USBCAMERADLL BOOL __stdcall USBCamera_GetDeviceFriendlyName(HANDLE device, LPWSTR pFriendlyName, DWORD bufSize, DWORD *pRetBufSize);

USBCAMERADLL BOOL __stdcall USBCamera_GetDevicePath(HANDLE device, LPWSTR* pDevicePath, DWORD bufSize, DWORD *pRetBufSize);

USBCAMERADLL BOOL __stdcall USBCamera_ReleaseDevice(HANDLE device);

USBCAMERADLL BOOL __stdcall USBCamera_SetDecCallBack(LONG lPort, DEC_CALLBACK_INFO struDecCallbackInfo);

/*USBCAMERADLL BOOL __stdcall USBCamera_SetEventSignalingCallBack(LONG lPort, void (__stdcall *EventSignalingCallBack)(LONG lPort, LONG lEvent, LONG nParam1, LONG nParam2));*/

USBCAMERADLL BOOL __stdcall USBCamera_GetPort(LONG* pPort);

USBCAMERADLL BOOL __stdcall USBCamera_FreePort(LONG lPort);

USBCAMERADLL BOOL __stdcall USBCamera_ChooseDevice(LONG lPort, HANDLE vDevice, HANDLE aDevice);

USBCAMERADLL BOOL __stdcall USBCamera_FreeDevice(LONG lPort);

USBCAMERADLL BOOL __stdcall USBCamera_EnumVideoProperty(LONG lPort, CAMERA_CAPACITY** ppCameraCapacity, int *pCount);

USBCAMERADLL BOOL __stdcall USBCamera_SetResolution(LONG lPort, DWORD dwWidth, DWORD dwHeight);

USBCAMERADLL BOOL __stdcall USBCamera_GetResolution(LONG lPort, DWORD *pWidth, DWORD *pHeight);

USBCAMERADLL BOOL __stdcall USBCamera_GetFrameRate(LONG lPort, DWORD *pFrameRate);

USBCAMERADLL BOOL __stdcall USBCamera_SetFrameRate(LONG lPort, DWORD dwFrameRate);

USBCAMERADLL BOOL __stdcall USBCamera_EnumAudioFormat(LONG lPort, WAVEFORMATEX **ppWF, int *pCount);

USBCAMERADLL BOOL __stdcall USBCamera_GetAudioFormat(LONG lPort, WAVEFORMATEX *wf);

USBCAMERADLL BOOL __stdcall USBCamera_SetAudioFormat(LONG lPort, WAVEFORMATEX wf);

USBCAMERADLL BOOL __stdcall USBCamera_GetBitRate(LONG lPort, DWORD *pBitRate);

USBCAMERADLL BOOL __stdcall USBCamera_SetBitRate(LONG lPort, DWORD dwBitRate);

// USBCAMERADLL BOOL __stdcall USBCamera_GetAudioVolume(LONG lPort, double *pLevel);
// 
// USBCAMERADLL BOOL __stdcall USBCamera_SetAudioVolume(LONG lPort, double dLevel);

USBCAMERADLL BOOL __stdcall USBCamera_SetPreview(LONG lPort, DWORD dwVideoFormat, BOOL bAudio, HWND hWnd);

USBCAMERADLL BOOL __stdcall USBCamera_ResizeWindow(LONG lPort, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom);

USBCAMERADLL BOOL __stdcall USBCamera_SetCapture(LONG lPort, DWORD dwVideoFormat, BOOL bAudio, LPCWSTR pCaptureFileName);

USBCAMERADLL BOOL __stdcall USBCamera_Snapshot(LONG lPort, LPCWSTR pJpegFile);

USBCAMERADLL BOOL __stdcall USBCamera_Start(LONG lPort);

USBCAMERADLL BOOL __stdcall USBCamera_Stop(LONG lPort);

USBCAMERADLL BOOL __stdcall USBCamera_DeleteMemory(void* pUser);

USBCAMERADLL BOOL __stdcall USBCamera_Control(LONG lPort, DWORD dwCommand, void* pBuffer, DWORD dwBufLen);

//USBCAMERADLL BOOL __stdcall USBCamera_GetConfig(LONG lPort, DWORD dwCommand, void* pBuffer, DWORD *pBufLen);

//USBCAMERADLL BOOL __stdcall USBCamera_SetConfig(LONG lPort, DWORD dwCommand, void* pBuffer, DWORD dwBufLen);

//USBCAMERADLL BOOL __stdcall USBCamera_SetOSD(LONG lPort, USB_CAM_OSD_LIST struOSDList);


