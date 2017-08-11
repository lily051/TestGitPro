#ifndef __PREVIEWS20_DLL_API_
#define __PREVIEWS20_DLL_API_

#ifdef PREVIEWS20_DLL_EXPORTS
#define PVS20_API __declspec(dllexport)
#else
#define PVS20_API __declspec(dllimport)
#endif

// 设备类型，可扩展
typedef enum _tag_PREVIEW_DEVTYPE
{
    PV_DEVTPYE_HIKVISON          = 0,   //海康威视

}PREVIEW_DEVTYPE;

// 设备类型
typedef enum _tag_PREVIEW_CAMTYPE
{
	PV_CAMTPYE_DVR          = 0,        //海康DVR
	PV_CAMTPYE_NVR          = 1,        //海康NVR
	PV_CAMTPYE_USB          = 2,        //海康USB

}PREVIEW_CAMTYPE;

// 皮肤风格
typedef enum _tag_SKIN_STYLE
{
    PV_SKIN_STYLE_RED           = 0,    //红黑皮肤
    PV_SKIN_STYLE_BLUE          = 1,    //蓝白皮肤
}SKIN_STYLE;

// 预览默认窗体分割模式，可扩展
typedef enum _tag_PREVIEW_DEFAULT_LAYOUT
{
    PV_DEFAULT_LAYOUT_1            = 0,        //一画面

}PREVIEW_DEFAULT_LAYOUT;

// 权限类型
typedef enum _tag_PREVIEW_RIGHTTYPE
{
    PV_RIGHTTYPE_CAPTURE        = 0x0001,   //抓图权限
    PV_RIGHTTYPE_RECORD         = 0x0002,   //录像权限
    PV_RIGHTTYPE_ALL            = 0x0003,   //所有

}PREVIEW_RIGHTTYPE;

// 预览悬浮ToolBar权限类型
typedef enum _tag_PREVIEW_HOVER_TOOLBAR_RIGHTTYPE
{
    PV_HOVER_TOOLBAR_STOP       = 0x00000001,   //停止预览
    PV_HOVER_TOOLBAR_AUDIO      = 0x00000002,   //音频
    PV_HOVER_TOOLBAR_RECORD     = 0x00000004,   //录像
    PV_HOVER_TOOLBAR_CAPTURE    = 0x00000006,   //抓图
	PV_HOVER_TOOLBAR_STREAM_TYPE= 0x00000008,   //主子码流切换
	PV_HOVER_TOOLBAR_OSDINFO    = 0x00000010,   //OSD信息
    PV_HOVER_TOOLBAR_ALL        = 0x0000001f,   //所有
}PREVIEW_HOVER_TOOLBAR_RIGHTTYPE;

// 预览ToolBar权限类型
typedef enum _tag_PREVIEW_TOOLBAR_RIGHTTYPE
{
    PV_TOOLBAR_FULLSCREEN       = 0x00000001,   //全屏
    PV_TOOLBAR_ALL              = 0x0000000f,   //所有

}PREVIEW_TOOLBAR_RIGHTTYPE;

typedef enum _tag_PREVIEW_MODEL
{
	PV__MODEL_COLLECT           = 0,    //采集
	PV__MODEL_CENTER            = 1,    //中心

}PREVIEW_MODEL;

// 预览初始化参数
typedef struct _tag_PREVIEW_PARAM
{
    int nDefaultLayout;         //默认窗口分割模式
    int nOperRight;             //预览操作权限，参见PREVIEW_RIGHTTYPE
    int nHoverToolBarRight;     //悬停ToolBar权限，参见PREVIEW_HOVER_TOOLBAR_RIGHTTYPE
    int nToolBarRight;          //ToolBar权限，参见PREVIEW_TOOLBAR_RIGHTTYPE
    int nUserLevel;             //用户等级
	int nPreviewModel;          //预览模式
	LONG lBtnRights;            //按钮权限
    _tag_PREVIEW_PARAM()
    {
        nDefaultLayout = PV_DEFAULT_LAYOUT_1;         //默认：一画面
        nOperRight = PV_RIGHTTYPE_ALL;                //默认：抓图、录像权限
        nHoverToolBarRight = PV_HOVER_TOOLBAR_ALL;    //默认：停止预览、语音
        nToolBarRight = PV_TOOLBAR_ALL;               //默认：停止所有、全屏
        nUserLevel = 0;
		nPreviewModel = PV__MODEL_COLLECT;                            //
		lBtnRights = 0;
    }
}PREVIEW_PARAM;

// 预览信息
typedef struct _tag_PREVIEW_INFO
{
    int nCamID;                     //用户业务通道ID
    CString strCamName;             //用户业务通道名称
    int nDevType;                   //设备类型
    int nRegionID;                  //摄像头所在区域ID
    CString strDevIP;               //设备IP
    int nDevPort;                   //设备Port
    CString strUser;                //设备登陆名
    CString strPwd;                 //设备登陆密码
    int nChannel;                   //通道号
    int nRealPlayType;              //播放类型,0-主码流，1-子码流
    int nVideoFormat;               //视频格式，大华专用，0：H.264，1：MPEG-4
    int nCamType;                   //摄像头类型：0-DVR，1-NVR，2-USB
    int nCamRight;                  //摄像头权限，参见PREVIEW_CAMERA_RIGHTTYPE
    int nProtocol;                  //协议类型 0-TCP, 1-UDP			
	int nUserID;			        //业务通道用户ID
	CString strCollectorIP;         //登录录像服务器ip
	int nPhonePort;                 //录音服务通信Port
	int nPChannel;                  //录音通道号
	int nTimeout;                   //超时
	int nDelayTime;                 //延时
	long lCamIndex;				    //USB摄像头Index
	long lMicIndex;			        //麦克风Index
	CString strMicName;			    //麦克风Name
	unsigned int  uiWidth;          //分辨率宽度，支持USB预览，当前支持分辨率只有1920*1080 1280*720 640*480 320*240
	unsigned int  uiHeight;         //分辨率高度，支持USB预览，当前支持分辨率只有1920*1080 1280*720 640*480 320*240
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

// 录像返回信息
typedef struct _tag_RECOED_FILE_INFO
{
	CString strDevIP;       //设备ip
	int nDevPort;           //设备port
	CString strUser;        //设备user
	CString strPwd;         //设备psd
	int nChannel;           //通道编号 
	CString strFileName;    //文件名称
	long lFileSize;         //文件大小
	long lFileType;         //文件类型
	long lStartTime;        //文件开始时间
	long lEndTime;          //文件结束时间
	long nRecordStatus;		//录像状态：正在采集-0，正常结束-1 ，异常结束-8
	CString csPicPath;      //抓图路径
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
	PV_RECORD_START = 0,	// lpData 为 RECOED_FILE_INFO
	PV_RECORD_STOP  = 1		// lpData 为 RECOED_FILE_INFO

} PV_EVENT_TYPE;

typedef void (*PV_EVENT_FUNC)(PV_EVENT_TYPE pvEvent, void* lpData);

#ifdef __cplusplus
extern "C"
{
#endif

    /*
	*  @fn         PV_Init
    *  @brief      初始化dll
    *  @param[IN]  emSkinStyle: 皮肤风格
    *  @return     TRUE: 成功 FALSE: 失败
    */
    PVS20_API BOOL PV_Init(const CString& strSavePath, SKIN_STYLE emSkinStyle = PV_SKIN_STYLE_RED);

    /*
	*  @fn         PV_UnInit
    *  @brief      反初始化dll
    *  @return     void
    */
    PVS20_API void PV_UnInit(void);

    /*
	*  @fn         PV_Create
    *  @brief      创建预览窗口
    *  @param[IN]  hParentWnd: 指定预览窗口的父窗口
    *  @param[IN]  stPreviewParam: 预览参数
    *  @return     成功：返回预览窗口句柄，失败：返回NULL
    */
    PVS20_API HANDLE PV_Create(HWND hParentWnd, const PREVIEW_PARAM &stPreviewParam);

    /*
	*  @fn         PV_Destroy
    *  @brief      销毁预览窗口
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
    PVS20_API BOOL PV_Destroy(HANDLE hPrevew);

    /*
	*  @fn         PV_MoveWindow
    *  @brief      设置预览窗口位置
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @param[IN]  rcWnd: 窗口位置
    *  @return     TRUE: 成功 FALSE: 失败
    */
    PVS20_API BOOL PV_MoveWindow(HANDLE hPrevew, CRect rcWnd);

    /*
	*  @fn         PV_StartPreview
    *  @brief      开始预览
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @param[IN]  struPreviewInfo: 预览参数信息
    *  @return     TRUE: 成功 FALSE: 失败
    */
    PVS20_API BOOL PV_StartPreview(HANDLE hPrevew, const PREVIEW_CAMINFO &struPreviewInfo);

	/*
	*  @fn         PV_StopPreview
    *  @brief      停止预览
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
    PVS20_API BOOL PV_StopPreview(HANDLE hPrevew);

	/*
	*  @fn         PV_StartRecord
    *  @brief      开始录像
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_StartRecord(HANDLE hPrevew);

	/*
	*  @fn         PV_StopRecord
    *  @brief      停止录像
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_StopRecord(HANDLE hPrevew);

	/*
	*  @fn         PV_SetPvEventFunc
    *  @brief      设置事件回调
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_SetPvEventFunc(HANDLE hPrevew, PV_EVENT_FUNC Func);

   /** @fn         PV_GetRecordFile
    *  @brief      获取最近一次录像文件信息 
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_GetRecordFile(HANDLE hPrevew, RECOED_FILE_INFO& recordFile);

	/*
	*  @fn         PV_SetCfgData
	*  @brief      设置抓图和录像配置信息
	*  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
	*  @param[IN]  strPath: 抓图和录像文件保存路径
	*  @param[IN]  nMaxRecSize: 录像文件大小 1：64M，2：128M，4：256M，8：512M
	*  @param[IN]  nCapType: 抓图图片格式 0：bmp，1：jpeg
	*  @return     TRUE: 成功 FALSE: 失败
	*/
	PVS20_API BOOL PV_SetCfgData(HANDLE hPrevew, const CString &strCfgInfo);

	/*
	*  @fn         PV_SetShowString
    *  @brief      设置OSD信息 
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
	*  @param[IN]  strBussInfo: 显示信息，xml格式
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_SetShowString( HANDLE hPrevew, const CString &strBussInfo );

	/*
	*  @fn         PV_GetAllUSBCameraInfo
    *  @brief      获取所有USB监控点信息
    *  @return     strCameraInfo: USB监控点信息
    */
	PVS20_API BOOL PV_GetAllUSBCameraInfo(CString& strCameraInfo);

	/*
	*  @fn         PV_OpenSound
    *  @brief      打开预览声音 
    *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
    *  @return     TRUE: 成功 FALSE: 失败
    */
	PVS20_API BOOL PV_OpenSound( HANDLE hPrevew);

#ifdef __cplusplus
}
#endif

#endif //end of __PREVIEWS20_DLL_API_