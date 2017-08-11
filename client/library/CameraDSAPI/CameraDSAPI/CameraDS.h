/**    @file    CameraDS.h 
 *    @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief   获取usb摄像头编码后数据帧类
 *
 *    @author     chenxiaoyan3
 *    @date     2015/07/23
 *
 *    @note    下面的note和warning为可选项目
 *    @note    这里填写本文件的详细功能描述和注解
 *    @note    历史记录：
 *    @note    V0.0.1  创建
 *
 *    @warning 这里填写本文件相关的警告信息
 */

#ifndef CCAMERA_H
#define CCAMERA_H

#include "qedit.h"
#include "dshow.h"
#include "CSampleGrabberCB.h"
#include "CSampleGrabberCBAudio.h"
#include "GeneralDef.h"
#include "SystemTransform.h"
#include <atlbase.h>
#include <list>
#define MYFREEMEDIATYPE(mt)    {if ((mt).cbFormat != 0)        \
                    {CoTaskMemFree((PVOID)(mt).pbFormat);    \
                    (mt).cbFormat = 0;                        \
                    (mt).pbFormat = NULL;                    \
                }                                            \
                if ((mt).pUnk != NULL)                        \
                {                                            \
                    (mt).pUnk->Release();                    \
                    (mt).pUnk = NULL;                        \
                }}                                    


class CCameraDS  
{
public:
    CCameraDS();
    virtual ~CCameraDS();

    ///** @fn     CCameraDS* CCameraDS::Instance()
    // *  @brief  Get CameraDS instance
    // *  @param  void
    // *  @return CCameraDS instance
    // */
    //static CCameraDS* Instance();

    ///** @fn     void CCameraDS::Free()
    // *  @brief  Destroy CameraDS instance
    // *  @param  void
    // *  @return void
    // */
    //static void Free();

    /**	@fn	    Init
     *	@brief	初始化函数
     *	@param  
     *	@return	
    */
    int Init();

    /**	@fn	    Fini
     *	@brief	反初始化函数
     *	@param  
     *	@return	
    */
    int Fini();

    /**	@fn	    GetAllCameraInfo
     *	@brief	获取所有摄像头信息
     *	@param  [out] lstCameraInfo 摄像头信息列表
     *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
    */
    static int GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo);

    /**	@fn	    OpenCamera
     *	@brief	打开摄像头
     *	@param  [in] nCamID 摄像头编号
     *	@param  [in] uiMicIndex 麦克风索引号
     *	@param  [in] strMicName 麦克风名字
     *	@param  [in] stProperty 分辨率等属性
     *	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
    */
    long OpenCamera(unsigned int nCamID,
        unsigned int uiMicIndex,
        const std::string &strMicName,
        const PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    CloseCamera
     *	@brief	关闭摄像头
     *	@param  
     *	@return	
    */
    void CloseCamera();

    /**	@fn	    GetWidth
     *	@brief	获取当前播放录像的宽像素值
     *	@param  [in] 
     *	@return	宽像素值
    */
    inline int GetWidth(){return m_nWidth;}

    /**	@fn	    GetHeight
     *	@brief	获取当前播放录像的高像素值
     *	@param  [in] 
     *	@return	高像素值
    */
    inline int GetHeight(){return m_nHeight;}

    /**	@fn	    GetState
     *	@brief	获取取数据流当前状态
     *	@param  [out] lState 数据流当前状态
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    int GetState(long &lState);

    /**	@fn	    GetPictureInfo
     *	@brief	获取当前获取的数据图像属性信息
     *	@param  [out] stProperty 图像属性信息
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    int GetPictureInfo(PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    Run
     *	@brief	开始取数据流
     *	@param
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    long Run();

    /**	@fn	    Pause
     *	@brief	暂停取数据流
     *	@param
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    long Pause();

    /**	@fn	    Stop
     *	@brief	停止取数据流
     *	@param
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    long Stop();

    /**	@fn	    SetDataCallback
     *	@brief	设置数据流回调函数
     *	@param  [in] datafun 回调函数
     *	@param  [in] pUser 用户信息
     *	@return	大于等于零 表示没有错误，小于零表示出现了错误
    */
    long SetDataCallback(const pCameraDSDataBack datafun, void *pUser);

private:
    //CCameraDS();
    //virtual ~CCameraDS();

    /**	@fn	    BindVedioFilter
    *	@brief	添加视频设备滤镜
    *	@param  [in] nCamIDX 摄像头编号
    *	@param  [in] pFilter 滤镜
    *	@return	
    */
    long BindVedioFilter(int nCamIDX, IBaseFilter **pFilter);

    /**	@fn	    BindAudioFilter
    *	@brief	添加声音设备滤镜
    *	@param  [in] nCamIDX 麦克风编号
    *	@param  [in] pFilter 滤镜
    *	@return	
    */
    long BindAudioFilter(int nCamID, IBaseFilter **pFilter);

    /**	@fn	    InitUsbCamera
    *	@brief	打开摄像头
    *	@param  [in] nCamID 摄像头编号
    *	@param  [in] stProperty 分辨率等属性
    *	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
    */
    long InitUsbCamera(unsigned int nCamID, const PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    InitMicrophone
    *	@brief	打开麦克风
    *	@param  [in] uiMicIndex 麦克风索引号
    *	@param  [in] strMicName 麦克风名字
    *	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
    */
    long InitMicrophone(unsigned int uiMicIndex, const std::string &strMicName);

    /**	@fn	    ConnectPins
    *	@brief	链接PIN
    *	@return	long，大于等于零 表示没有错误，小于零表示出现了错误
    */
    long ConnectPins(const PICTURE_PROPERTY_ST &stProperty);

private:
    //pin链接是否连接上
    bool m_bConnected;

    //是否初始化过
    bool m_bInit;

    //是否释放
    bool m_bFree;

    //当前图像像素宽
    int m_nWidth;

    //当前图像像素高
    int m_nHeight;

    //当前取流状态
    long m_lState;

    //单例
    static CCameraDS* m_instance;

    //GraphBuilder 指针
    CComPtr<IGraphBuilder> m_pGraph;
    //GraphBuilder 指针
    CComPtr<IGraphBuilder> m_pGraphAudio;

    //视频设备滤镜 指针
    CComPtr<IBaseFilter> m_pVedioDeviceFilter;

    //视频设备滤镜 指针
    CComPtr<IBaseFilter> m_pAudioDeviceFilter;

    //媒体流控制 指针
    CComPtr<IMediaControl> m_pMediaControl;

    //媒体流控制 指针
    CComPtr<IMediaControl> m_pAudioControl;

    //SampleGrabber滤镜 指针
    CComPtr<IBaseFilter> m_pSampleGrabberFilterVedio;

    //SampleGrabber滤镜 指针
    CComPtr<IBaseFilter> m_pSampleGrabberFilterAudio;

    //MJPEG解码滤镜 指针
    CComPtr<IBaseFilter> m_pMjpegDecompress;

    //264编码滤镜 指针
    CComPtr<IBaseFilter> m_pH264Encoder; 

    //SampleGrabber 指针
    CComPtr<ISampleGrabber> m_pSampleGrabberVedio;

    //SampleGrabber 指针
    CComPtr<ISampleGrabber> m_pSampleGrabberAudio;

    //视频设备输出pin 指针
    CComPtr<IPin> m_pCaptureMJPEGOutputPin;

    //音频设备输出pin 指针
    CComPtr<IPin> m_pCapturePmcOutputPin;

    //Grabber输入源pin 指针
    CComPtr<IPin> m_pGrabberInputVedio;

    //Grabber输出目标pin 指针
    CComPtr<IPin> m_pGrabberOutputVedio;

    //Grabber输入源pin 指针
    CComPtr<IPin> m_pGrabberInputAudio;

    //Grabber输出目标pin 指针
    CComPtr<IPin> m_pGrabberOutputAudio;

    //摄像机输出目标pin 指针
    CComPtr<IPin> m_pCameraOutput;

    //Dshow基础滤镜 指针
    CComPtr<IBaseFilter> m_pNullFilterVedio;

    //Dshow基础滤镜 指针
    CComPtr<IBaseFilter> m_pNullFilterAudio;

    //Dshow基础pin 指针
    CComPtr<IPin> m_pNullInputPinVedio;

    //Dshow基础pin 指针
    CComPtr<IPin> m_pNullInputPinAudio;

    //CSampleGrabberCB类对象
    CSampleGrabberCB m_GrabberCBVedio;

    //CSampleGrabberCB类对象
    CSampleGrabberCBAudio m_GrabberCBAudio;

    CComPtr<IPin> m_pMjpegDecInput;
    CComPtr<IPin> m_pMjpegDecOutput;
    CComPtr<IPin> m_p264EncodeInput;
    CComPtr<IPin> m_p264EncodeOutput;

    void* m_phTrans; //ps流封装库句柄

    CComPtr<IBaseFilter> m_pMuxFilter;
    CComPtr<IBaseFilter> m_pWriteFilter;
    CComPtr<IPin> m_pMuxin1;
    CComPtr<IPin> m_pMuxin2;
    CComPtr<IPin> m_pMuxout;
    CComPtr<IPin> m_pWritein;
public:
    CRITICAL_SECTION m_cs;
};

#endif 
