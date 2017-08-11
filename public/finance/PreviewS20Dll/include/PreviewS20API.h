#ifndef __PREVIEWS20_DLL_API_
#define __PREVIEWS20_DLL_API_

#ifdef PREVIEWS20_DLL_EXPORTS
#define PVS20_API __declspec(dllexport)
#else
#define PVS20_API __declspec(dllimport)
#endif

// �豸���ͣ�����չ
typedef enum _tag_PREVIEW_DEVTYPE
{
    PV_DEVTPYE_HIKVISON          = 0,   //��������

}PREVIEW_DEVTYPE;

// �豸����
typedef enum _tag_PREVIEW_CAMTYPE
{
	PV_CAMTPYE_DVR          = 0,        //����DVR
	PV_CAMTPYE_NVR          = 1,        //����NVR
	PV_CAMTPYE_USB          = 2,        //����USB

}PREVIEW_CAMTYPE;

// Ƥ�����
typedef enum _tag_SKIN_STYLE
{
    PV_SKIN_STYLE_RED           = 0,    //���Ƥ��
    PV_SKIN_STYLE_BLUE          = 1,    //����Ƥ��
}SKIN_STYLE;

// Ԥ��Ĭ�ϴ���ָ�ģʽ������չ
typedef enum _tag_PREVIEW_DEFAULT_LAYOUT
{
    PV_DEFAULT_LAYOUT_1            = 0,        //һ����

}PREVIEW_DEFAULT_LAYOUT;

// Ȩ������
typedef enum _tag_PREVIEW_RIGHTTYPE
{
    PV_RIGHTTYPE_CAPTURE        = 0x0001,   //ץͼȨ��
    PV_RIGHTTYPE_RECORD         = 0x0002,   //¼��Ȩ��
    PV_RIGHTTYPE_ALL            = 0x0003,   //����

}PREVIEW_RIGHTTYPE;

// Ԥ������ToolBarȨ������
typedef enum _tag_PREVIEW_HOVER_TOOLBAR_RIGHTTYPE
{
    PV_HOVER_TOOLBAR_STOP       = 0x00000001,   //ֹͣԤ��
    PV_HOVER_TOOLBAR_AUDIO      = 0x00000002,   //��Ƶ
    PV_HOVER_TOOLBAR_RECORD     = 0x00000004,   //¼��
    PV_HOVER_TOOLBAR_CAPTURE    = 0x00000006,   //ץͼ
	PV_HOVER_TOOLBAR_STREAM_TYPE= 0x00000008,   //���������л�
	PV_HOVER_TOOLBAR_OSDINFO    = 0x00000010,   //OSD��Ϣ
    PV_HOVER_TOOLBAR_ALL        = 0x0000001f,   //����
}PREVIEW_HOVER_TOOLBAR_RIGHTTYPE;

// Ԥ��ToolBarȨ������
typedef enum _tag_PREVIEW_TOOLBAR_RIGHTTYPE
{
    PV_TOOLBAR_FULLSCREEN       = 0x00000001,   //ȫ��
    PV_TOOLBAR_ALL              = 0x0000000f,   //����

}PREVIEW_TOOLBAR_RIGHTTYPE;

typedef enum _tag_PREVIEW_MODEL
{
	PV__MODEL_COLLECT           = 0,    //�ɼ�
	PV__MODEL_CENTER            = 1,    //����

}PREVIEW_MODEL;

// Ԥ����ʼ������
typedef struct _tag_PREVIEW_PARAM
{
    int nDefaultLayout;         //Ĭ�ϴ��ڷָ�ģʽ
    int nOperRight;             //Ԥ������Ȩ�ޣ��μ�PREVIEW_RIGHTTYPE
    int nHoverToolBarRight;     //��ͣToolBarȨ�ޣ��μ�PREVIEW_HOVER_TOOLBAR_RIGHTTYPE
    int nToolBarRight;          //ToolBarȨ�ޣ��μ�PREVIEW_TOOLBAR_RIGHTTYPE
    int nUserLevel;             //�û��ȼ�
	int nPreviewModel;          //Ԥ��ģʽ
	LONG lBtnRights;            //��ťȨ��
    _tag_PREVIEW_PARAM()
    {
        nDefaultLayout = PV_DEFAULT_LAYOUT_1;         //Ĭ�ϣ�һ����
        nOperRight = PV_RIGHTTYPE_ALL;                //Ĭ�ϣ�ץͼ��¼��Ȩ��
        nHoverToolBarRight = PV_HOVER_TOOLBAR_ALL;    //Ĭ�ϣ�ֹͣԤ��������
        nToolBarRight = PV_TOOLBAR_ALL;               //Ĭ�ϣ�ֹͣ���С�ȫ��
        nUserLevel = 0;
		nPreviewModel = PV__MODEL_COLLECT;                            //
		lBtnRights = 0;
    }
}PREVIEW_PARAM;

// Ԥ����Ϣ
typedef struct _tag_PREVIEW_INFO
{
    int nCamID;                     //�û�ҵ��ͨ��ID
    CString strCamName;             //�û�ҵ��ͨ������
    int nDevType;                   //�豸����
    int nRegionID;                  //����ͷ��������ID
    CString strDevIP;               //�豸IP
    int nDevPort;                   //�豸Port
    CString strUser;                //�豸��½��
    CString strPwd;                 //�豸��½����
    int nChannel;                   //ͨ����
    int nRealPlayType;              //��������,0-��������1-������
    int nVideoFormat;               //��Ƶ��ʽ����ר�ã�0��H.264��1��MPEG-4
    int nCamType;                   //����ͷ���ͣ�0-DVR��1-NVR��2-USB
    int nCamRight;                  //����ͷȨ�ޣ��μ�PREVIEW_CAMERA_RIGHTTYPE
    int nProtocol;                  //Э������ 0-TCP, 1-UDP			
	int nUserID;			        //ҵ��ͨ���û�ID
	CString strCollectorIP;         //��¼¼�������ip
	int nPhonePort;                 //¼������ͨ��Port
	int nPChannel;                  //¼��ͨ����
	int nTimeout;                   //��ʱ
	int nDelayTime;                 //��ʱ
	long lCamIndex;				    //USB����ͷIndex
	long lMicIndex;			        //��˷�Index
	CString strMicName;			    //��˷�Name
	unsigned int  uiWidth;          //�ֱ��ʿ�ȣ�֧��USBԤ������ǰ֧�ֱַ���ֻ��1920*1080 1280*720 640*480 320*240
	unsigned int  uiHeight;         //�ֱ��ʸ߶ȣ�֧��USBԤ������ǰ֧�ֱַ���ֻ��1920*1080 1280*720 640*480 320*240
    _tag_PREVIEW_INFO()
    {
        nCamID = 0;
        nDevType = 0;
        nRegionID = 0;
        nDevPort = 0;
        nChannel = 0;
        nRealPlayType = 0;
        nVideoFormat = 0;
        nCamType = PV_CAMTPYE_DVR;
        nCamRight = 0;
        nProtocol = 0;
		nUserID = 0;
		nPhonePort = 9527;
		nPChannel = 0;
		nTimeout = 1;
		nDelayTime = 1;
		lCamIndex = 0;
		lMicIndex = 0;
		uiWidth = 1280;
		uiHeight = 720;
    }
    void Clear()
    {
        nCamID = 0;
        strCamName.Empty();
        nDevType = 0;
        nRegionID = 0;
        strDevIP.Empty();
        nDevPort = 0;
        strUser.Empty();
        strPwd.Empty();
        nChannel = 0;
        nRealPlayType = 0;
        nVideoFormat = 0;
        nCamType = PV_CAMTPYE_DVR;
        nCamRight = 0;
        nProtocol = 0;
		nUserID = 0;
		strCollectorIP.Empty();
		nPhonePort = 9527;
		nPChannel = 0;
		nTimeout = 1;
		nDelayTime = 1;
		lCamIndex = 0;
		lMicIndex = 0;
		strCamName.Empty();
		strMicName.Empty();
		uiWidth = 1280;
		uiHeight = 720;
    }
}PREVIEW_CAMINFO;

// ¼�񷵻���Ϣ
typedef struct _tag_RECOED_FILE_INFO
{
	CString strDevIP;       //�豸ip
	int nDevPort;           //�豸port
	CString strUser;        //�豸user
	CString strPwd;         //�豸psd
	int nChannel;           //ͨ����� 
	CString strFileName;    //�ļ�����
	long lFileSize;         //�ļ���С
	long lFileType;         //�ļ�����
	long lStartTime;        //�ļ���ʼʱ��
	long lEndTime;          //�ļ�����ʱ��
	long nRecordStatus;		//¼��״̬�����ڲɼ�-0����������-1 ���쳣����-8
	CString csPicPath;      //ץͼ·��
	_tag_RECOED_FILE_INFO()
	{
		strDevIP = _T("");
		nDevPort = 0;
		strUser = _T("");
		strPwd = _T("");
		nChannel = 0;
		strFileName = _T("");
		lFileSize = 0;
		lFileType = 0;
		lStartTime = 0;
		lEndTime = 0;
		nRecordStatus = 0;
		csPicPath = _T("");
	}
} RECOED_FILE_INFO;

typedef enum _tag_PV_EVENT
{
	PV_RECORD_START = 0,	// lpData Ϊ RECOED_FILE_INFO
	PV_RECORD_STOP  = 1		// lpData Ϊ RECOED_FILE_INFO

} PV_EVENT_TYPE;

typedef void (*PV_EVENT_FUNC)(PV_EVENT_TYPE pvEvent, void* lpData);

#ifdef __cplusplus
extern "C"
{
#endif

    /*
	*  @fn         PV_Init
    *  @brief      ��ʼ��dll
    *  @param[IN]  emSkinStyle: Ƥ�����
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    PVS20_API BOOL PV_Init(const CString& strSavePath, SKIN_STYLE emSkinStyle = PV_SKIN_STYLE_RED);

    /*
	*  @fn         PV_UnInit
    *  @brief      ����ʼ��dll
    *  @return     void
    */
    PVS20_API void PV_UnInit(void);

    /*
	*  @fn         PV_Create
    *  @brief      ����Ԥ������
    *  @param[IN]  hParentWnd: ָ��Ԥ�����ڵĸ�����
    *  @param[IN]  stPreviewParam: Ԥ������
    *  @return     �ɹ�������Ԥ�����ھ����ʧ�ܣ�����NULL
    */
    PVS20_API HANDLE PV_Create(HWND hParentWnd, const PREVIEW_PARAM &stPreviewParam);

    /*
	*  @fn         PV_Destroy
    *  @brief      ����Ԥ������
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    PVS20_API BOOL PV_Destroy(HANDLE hPrevew);

    /*
	*  @fn         PV_MoveWindow
    *  @brief      ����Ԥ������λ��
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @param[IN]  rcWnd: ����λ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    PVS20_API BOOL PV_MoveWindow(HANDLE hPrevew, CRect rcWnd);

    /*
	*  @fn         PV_StartPreview
    *  @brief      ��ʼԤ��
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @param[IN]  struPreviewInfo: Ԥ��������Ϣ
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    PVS20_API BOOL PV_StartPreview(HANDLE hPrevew, const PREVIEW_CAMINFO &struPreviewInfo);

	/*
	*  @fn         PV_StopPreview
    *  @brief      ֹͣԤ��
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    PVS20_API BOOL PV_StopPreview(HANDLE hPrevew);

	/*
	*  @fn         PV_StartRecord
    *  @brief      ��ʼ¼��
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_StartRecord(HANDLE hPrevew);

	/*
	*  @fn         PV_StopRecord
    *  @brief      ֹͣ¼��
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_StopRecord(HANDLE hPrevew);

	/*
	*  @fn         PV_SetPvEventFunc
    *  @brief      �����¼��ص�
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_SetPvEventFunc(HANDLE hPrevew, PV_EVENT_FUNC Func);

   /** @fn         PV_GetRecordFile
    *  @brief      ��ȡ���һ��¼���ļ���Ϣ 
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_GetRecordFile(HANDLE hPrevew, RECOED_FILE_INFO& recordFile);

	/*
	*  @fn         PV_SetCfgData
	*  @brief      ����ץͼ��¼��������Ϣ
	*  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
	*  @param[IN]  strPath: ץͼ��¼���ļ�����·��
	*  @param[IN]  nMaxRecSize: ¼���ļ���С 1��64M��2��128M��4��256M��8��512M
	*  @param[IN]  nCapType: ץͼͼƬ��ʽ 0��bmp��1��jpeg
	*  @return     TRUE: �ɹ� FALSE: ʧ��
	*/
	PVS20_API BOOL PV_SetCfgData(HANDLE hPrevew, const CString &strCfgInfo);

	/*
	*  @fn         PV_SetShowString
    *  @brief      ����OSD��Ϣ 
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
	*  @param[IN]  strBussInfo: ��ʾ��Ϣ��xml��ʽ
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_SetShowString( HANDLE hPrevew, const CString &strBussInfo );

	/*
	*  @fn         PV_GetAllUSBCameraInfo
    *  @brief      ��ȡ����USB��ص���Ϣ
    *  @return     strCameraInfo: USB��ص���Ϣ
    */
	PVS20_API BOOL PV_GetAllUSBCameraInfo(CString& strCameraInfo);

	/*
	*  @fn         PV_OpenSound
    *  @brief      ��Ԥ������ 
    *  @param[IN]  hPrevew: PV_Create�ɹ����ص�Ԥ�����ھ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
	PVS20_API BOOL PV_OpenSound( HANDLE hPrevew);

#ifdef __cplusplus
}
#endif

#endif //end of __PREVIEWS20_DLL_API_