/**    @file    CameraDS.h 
 *    @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief   ��ȡusb����ͷ���������֡��
 *
 *    @author     chenxiaoyan3
 *    @date     2015/07/23
 *
 *    @note    �����note��warningΪ��ѡ��Ŀ
 *    @note    ������д���ļ�����ϸ����������ע��
 *    @note    ��ʷ��¼��
 *    @note    V0.0.1  ����
 *
 *    @warning ������д���ļ���صľ�����Ϣ
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
     *	@brief	��ʼ������
     *	@param  
     *	@return	
    */
    int Init();

    /**	@fn	    Fini
     *	@brief	����ʼ������
     *	@param  
     *	@return	
    */
    int Fini();

    /**	@fn	    GetAllCameraInfo
     *	@brief	��ȡ��������ͷ��Ϣ
     *	@param  [out] lstCameraInfo ����ͷ��Ϣ�б�
     *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    static int GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo);

    /**	@fn	    OpenCamera
     *	@brief	������ͷ
     *	@param  [in] nCamID ����ͷ���
     *	@param  [in] uiMicIndex ��˷�������
     *	@param  [in] strMicName ��˷�����
     *	@param  [in] stProperty �ֱ��ʵ�����
     *	@return	long�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long OpenCamera(unsigned int nCamID,
        unsigned int uiMicIndex,
        const std::string &strMicName,
        const PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    CloseCamera
     *	@brief	�ر�����ͷ
     *	@param  
     *	@return	
    */
    void CloseCamera();

    /**	@fn	    GetWidth
     *	@brief	��ȡ��ǰ����¼��Ŀ�����ֵ
     *	@param  [in] 
     *	@return	������ֵ
    */
    inline int GetWidth(){return m_nWidth;}

    /**	@fn	    GetHeight
     *	@brief	��ȡ��ǰ����¼��ĸ�����ֵ
     *	@param  [in] 
     *	@return	������ֵ
    */
    inline int GetHeight(){return m_nHeight;}

    /**	@fn	    GetState
     *	@brief	��ȡȡ��������ǰ״̬
     *	@param  [out] lState ��������ǰ״̬
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    int GetState(long &lState);

    /**	@fn	    GetPictureInfo
     *	@brief	��ȡ��ǰ��ȡ������ͼ��������Ϣ
     *	@param  [out] stProperty ͼ��������Ϣ
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    int GetPictureInfo(PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    Run
     *	@brief	��ʼȡ������
     *	@param
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long Run();

    /**	@fn	    Pause
     *	@brief	��ͣȡ������
     *	@param
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long Pause();

    /**	@fn	    Stop
     *	@brief	ֹͣȡ������
     *	@param
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long Stop();

    /**	@fn	    SetDataCallback
     *	@brief	�����������ص�����
     *	@param  [in] datafun �ص�����
     *	@param  [in] pUser �û���Ϣ
     *	@return	���ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long SetDataCallback(const pCameraDSDataBack datafun, void *pUser);

private:
    //CCameraDS();
    //virtual ~CCameraDS();

    /**	@fn	    BindVedioFilter
    *	@brief	�����Ƶ�豸�˾�
    *	@param  [in] nCamIDX ����ͷ���
    *	@param  [in] pFilter �˾�
    *	@return	
    */
    long BindVedioFilter(int nCamIDX, IBaseFilter **pFilter);

    /**	@fn	    BindAudioFilter
    *	@brief	��������豸�˾�
    *	@param  [in] nCamIDX ��˷���
    *	@param  [in] pFilter �˾�
    *	@return	
    */
    long BindAudioFilter(int nCamID, IBaseFilter **pFilter);

    /**	@fn	    InitUsbCamera
    *	@brief	������ͷ
    *	@param  [in] nCamID ����ͷ���
    *	@param  [in] stProperty �ֱ��ʵ�����
    *	@return	long�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long InitUsbCamera(unsigned int nCamID, const PICTURE_PROPERTY_ST &stProperty);

    /**	@fn	    InitMicrophone
    *	@brief	����˷�
    *	@param  [in] uiMicIndex ��˷�������
    *	@param  [in] strMicName ��˷�����
    *	@return	long�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long InitMicrophone(unsigned int uiMicIndex, const std::string &strMicName);

    /**	@fn	    ConnectPins
    *	@brief	����PIN
    *	@return	long�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
    */
    long ConnectPins(const PICTURE_PROPERTY_ST &stProperty);

private:
    //pin�����Ƿ�������
    bool m_bConnected;

    //�Ƿ��ʼ����
    bool m_bInit;

    //�Ƿ��ͷ�
    bool m_bFree;

    //��ǰͼ�����ؿ�
    int m_nWidth;

    //��ǰͼ�����ظ�
    int m_nHeight;

    //��ǰȡ��״̬
    long m_lState;

    //����
    static CCameraDS* m_instance;

    //GraphBuilder ָ��
    CComPtr<IGraphBuilder> m_pGraph;
    //GraphBuilder ָ��
    CComPtr<IGraphBuilder> m_pGraphAudio;

    //��Ƶ�豸�˾� ָ��
    CComPtr<IBaseFilter> m_pVedioDeviceFilter;

    //��Ƶ�豸�˾� ָ��
    CComPtr<IBaseFilter> m_pAudioDeviceFilter;

    //ý�������� ָ��
    CComPtr<IMediaControl> m_pMediaControl;

    //ý�������� ָ��
    CComPtr<IMediaControl> m_pAudioControl;

    //SampleGrabber�˾� ָ��
    CComPtr<IBaseFilter> m_pSampleGrabberFilterVedio;

    //SampleGrabber�˾� ָ��
    CComPtr<IBaseFilter> m_pSampleGrabberFilterAudio;

    //MJPEG�����˾� ָ��
    CComPtr<IBaseFilter> m_pMjpegDecompress;

    //264�����˾� ָ��
    CComPtr<IBaseFilter> m_pH264Encoder; 

    //SampleGrabber ָ��
    CComPtr<ISampleGrabber> m_pSampleGrabberVedio;

    //SampleGrabber ָ��
    CComPtr<ISampleGrabber> m_pSampleGrabberAudio;

    //��Ƶ�豸���pin ָ��
    CComPtr<IPin> m_pCaptureMJPEGOutputPin;

    //��Ƶ�豸���pin ָ��
    CComPtr<IPin> m_pCapturePmcOutputPin;

    //Grabber����Դpin ָ��
    CComPtr<IPin> m_pGrabberInputVedio;

    //Grabber���Ŀ��pin ָ��
    CComPtr<IPin> m_pGrabberOutputVedio;

    //Grabber����Դpin ָ��
    CComPtr<IPin> m_pGrabberInputAudio;

    //Grabber���Ŀ��pin ָ��
    CComPtr<IPin> m_pGrabberOutputAudio;

    //��������Ŀ��pin ָ��
    CComPtr<IPin> m_pCameraOutput;

    //Dshow�����˾� ָ��
    CComPtr<IBaseFilter> m_pNullFilterVedio;

    //Dshow�����˾� ָ��
    CComPtr<IBaseFilter> m_pNullFilterAudio;

    //Dshow����pin ָ��
    CComPtr<IPin> m_pNullInputPinVedio;

    //Dshow����pin ָ��
    CComPtr<IPin> m_pNullInputPinAudio;

    //CSampleGrabberCB�����
    CSampleGrabberCB m_GrabberCBVedio;

    //CSampleGrabberCB�����
    CSampleGrabberCBAudio m_GrabberCBAudio;

    CComPtr<IPin> m_pMjpegDecInput;
    CComPtr<IPin> m_pMjpegDecOutput;
    CComPtr<IPin> m_p264EncodeInput;
    CComPtr<IPin> m_p264EncodeOutput;

    void* m_phTrans; //ps����װ����

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
