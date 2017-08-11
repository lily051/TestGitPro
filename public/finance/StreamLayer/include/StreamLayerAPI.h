/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       StreamLayerAPI.h
*   Description:    a audio/video stream layer from RSMS/Device
*   Author:         JiangTianjing(jiangtj@hikvision.com)
*   Date:           2008-10-06 Monday
*   Modification History:      
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-06      JiangTianjing   created
**************************************************************************/


#ifndef __STREAM_LAYER_API__
#define __STREAM_LAYER_API__

#ifdef STREAM_LAYER_API_EXPORTS
#define STREAM_LAYER_API __declspec(dllexport)
#else
#define STREAM_LAYER_API __declspec(dllimport)
#endif

#define MAX_USERNAME_LEN_8100    64
#define MAX_PASSWORD_LEN    64
#define MAX_DEVICE_LEN      128
#define DESCRIBE_LEN_32     32

typedef enum _Enum_DataLen
{
    SL_LEN_IPv4         = 16,   ///< IP地址长度
    SL_LEN_VAG_USER     = 32,   ///< VAG登陆名长度
    SL_LEN_VAG_PWD      = 32    ///< VAG登陆密码长度
};

typedef enum _enumConnectType
{
    CONNECT_DEVICE = 0,         // 直连
    CONNECT_RSM = 1,            // 走流媒体
    CONNECT_CLOUD = 2,          // 走云存储
    CONNECT_NCG28181_GET = 3,   // 走联网网关NCG28181 取流模式
    CONNECT_NCG28181_PUSH = 4,  // 走联网网关NCG28181 推流模式
    CONNECT_VAG = 5             // 走VAG
};

typedef enum _enumDevType
{
	SL_DEV_HIK = 0,			    //海康威视
	SL_DEV_DAHUA,			    //大华
	SL_DEV_HIK_3G,			    //海康3G
    SL_DEV_BLUESKY_LIMIT = 441, //蓝星limit
    SL_DEV_ONVIF = 584,         //Onvif标准协议
    SL_DEV_PAG = 1005           //vag
}DEVTYPE;

typedef struct tagStreamDataInfo
{
    int nStreamType;			//0表示数据为块格式；1表示数据为帧数据格式,目前总是0
    char * pBuff;				//数据缓冲地址
    int nDataLen;				//数据长度
	int	nPacketType;			//流数据类型 0：头数据
    
	// global time
    int	nYear;					// year	
    int	nMonth;					// month
    int	nDay;					// day
    int	nHour;					// hour
    int	nMinute;				// minute
    int	nSecond;				// second
    
	// relatively timestamp
    DWORD dwTimeStamp;					// relatively timestamp
	int   nFrame_DALI;					//帧率 使用厂家：大立
	int   nResolution_DALI;				//分辨率 使用厂家：大立
}STREAM_DATA_INFO, *PSTREAM_DATA_INFO;

// 实时流数据回调函数,
typedef void (CALLBACK * SLStreamCallback)(HANDLE hStream, HANDLE hUserData, STREAM_DATA_INFO * pStreamDataInfo);


//取流返回消息
#define E_CONNECT_FAIL			0	//连接失败时返回此消息
#define E_NETWORK_EXCEPTION		1	//连接成功，没有流返回此消息
#define E_RECONNECT_FAIL		2	//重连失败是返回此消息
#define E_RECONNECT_OK			3	//重连成功
#define E_RECONNECT_OVER		4	//重连结束（始终重连不会返回此消息）
#define E_CONNECT_OK            5	//连接成功

// int nMsgID: 0表示hStream对应的通道消息值，具体见nMsgID宏定义
typedef void (CALLBACK * SLMsgNotify)(HANDLE hStream, HANDLE hUserData, int nMsgID);

// 设备连接信息
typedef struct tagDeviceParam
{
    char szName[MAX_DEVICE_LEN];                // 设备ip地址字符串或域名字符串
    char szUsername[MAX_USERNAME_LEN_8100];	    // 设备用户名称
                                                // 如果是【联网网关NCG28181】，表示域编码ServerIndexCode
    char szPassword[MAX_DEVICE_LEN];	        // 设备用户口令
    int nPort;                                  // 设备端口
    int nType;                                  // 0-表示szName对应的是IP格式字符串, 1-表示szName对应的是域名（DNS）名称
	int nDeviceType;                            // 设备类型
	char szDeviceId[MAX_DEVICE_LEN];            // 如果是【3G设备】，表示海康3G设备接入服务器IP，设备唯一标识ID
	                                            // 如果是【联网网关NCG28181】，表示监控点Code编码
                                                // 如果是【VAG】，表示监控点strNo编码
	char szAccessSrvIP[32];	                    // 接入服务器IP
	unsigned short int	nAccessSrvPort;         // 接入服务器端口,默认7660
    int nDataPort;                              // 数据端口

    tagDeviceParam()
    {
        memset(szUsername, 0, sizeof(szUsername));
        memset(szPassword, 0, sizeof(szPassword));
        memset(szAccessSrvIP, 0, sizeof(szAccessSrvIP));
        memset(szName, 0, sizeof(szName));
        memset(szDeviceId, 0, sizeof(szDeviceId));
        nPort = -1;
        nType = 0;
        nDataPort = 0;
        nDeviceType = 0;
        nAccessSrvPort = 0;
    }

}DEVICE_PARAM,*PDEVICE_PARAM;

// 实时流转发路径信息
#define MAX_TRANSMIT_COUNT	16	//最大转发次数
typedef struct tagTransmitPath
{
    int nTransmit;      // 当前转发的次数
    char szSrvIp[MAX_TRANSMIT_COUNT][16];   // 每次经过转发的流媒体服务器的ip地址字符串
    int	nPort[MAX_TRANSMIT_COUNT];         // 0-表示默认端口号
    tagTransmitPath()
    {
        nTransmit = 0;
        for (int i = 0; i < MAX_TRANSMIT_COUNT; i++)
        {
            memset(szSrvIp[i], 0, 16*sizeof(char));
            nPort[i] = 27070;
        }
    }
}TRANSMIT_PATH,*PTRANSMIT_PATH;

// 实时流启动结构
typedef struct tagStreamParams
{
    DEVICE_PARAM device;        // 设备注册信息
    int nType;                  // 【CONNECT_DEVICE】-0表示表示直连， 
                                // 【CONNECT_RSM】-1表示从流媒体转发的路径，
                                // 【CONNECT_CLOUD】-2表示从云端取流
                                // 【CONNECT_NCG28181_GET】-3表示走联网网关NCG28181取流方式
                                // 【CONNECT_NCG28181_PUSH】-4表示走联网网关NCG28181推流方式
                                // 【CONNECT_VAG】-5表示走VAG取流
    TRANSMIT_PATH path;         // 当 nType为0时，从流媒体服务器获取码流
    int nChannel;               // 设备通道
    int	nSubChannel;            // 当nSubChannel为0时表示主码流，1表示子码流
    int nMode;		            // 0-【TCP】, 1-【UDP】, 当nType为0表示表示直连时2-【MULTICAST】.
	char szMultiCastIP[16];	    // 多播组地址，当nMode为2时有效
    HANDLE hUserData;           // 用户保存的数据，在回调函数参数中能够输出	
	char szProxyServerIP[16];   // 如果是【代理服务器】，表示代理服务器的IP
                                // 如果是【海康3G设备】,表示3G流媒体服务器的IP和端口
                                // 如果是【联网网关NCG28181】,表示联网网关IP
                                // 如果是【VAG】，表示VAG的IP
	int  nProxyServerPort;      // 如果是【代理服务器】，默认为7554
                                // 如果是【联网网关NCG28181】，表示联网网关服务的Port
                                // 如果是【VAG】，表示VAG的PORT
    char szVAGUser[SL_LEN_VAG_USER];    // VAG登陆用户名
    char szVAGPwd[SL_LEN_VAG_PWD];      // VAG登陆密码
    tagStreamParams()
    {
        nType = 0;
        nChannel = 0;
        nSubChannel = 0;
        nMode = 0;
        nProxyServerPort = 7554;
        hUserData = NULL;
        memset(szProxyServerIP, 0, 16*sizeof(char));
        memset(szMultiCastIP, 0, 16*sizeof(char));
        memset(szVAGUser, 0, sizeof(char)*SL_LEN_VAG_USER);
        memset(szVAGPwd, 0, sizeof(char)*SL_LEN_VAG_PWD);
    }
}STREAMPARAMS,*PSTREAMPARAMS;

typedef struct tagStartupInfo
{
	int nTryTimes;			//当流断开时，>0表示进行重连的次数， 0表示一直重连
	int nTimeout;			//执行重连时间（单位s），有效范围(1~60), 
							//只有当没有码流的累积时间达到此重连时间时，才执行重连操作.
	BOOL bInitHcSdk;		//是否初始化hcnetsdk
    
	tagStartupInfo()
    {
        nTryTimes = 1;
        nTimeout = 10;
        bInitHcSdk = TRUE;
    }
}STARTUP_INFO, *PSTARTUP_INFO;

// 云台控制命令结构
typedef struct tagPTZCommand
{
	int nCmdID;
	int nPara0;
	int nPara1;
	int nPara2;
	int nPara3;
}SL_PTZ_COMMAND, *PSL_PTZ_COMMAND;

// 视频压缩参数结构
typedef struct tagCompressionInfo
{
    int nCompressType;          // 压缩类型 0主码流 1子码流
    int nStreamType;            // 码流类型
    int nResolution;            // 分辨率
    int nBitrateType;           // 码率类型
    int nPicQuality;            // 图象质量
    int nVideoBitrate;          // 视频码率
    int nVideoFrameRate;        // 视频帧率
    int nVideoEncType;          // 视频编码类型
    char szResolution[DESCRIBE_LEN_32];         // 分辨率描述
    char szVideoBitrate[DESCRIBE_LEN_32];       // 视频码率描述
    char szVideoFrameRate[DESCRIBE_LEN_32];     // 视频帧率描述
    tagCompressionInfo()
    {
        nCompressType = 0;
        nStreamType = -1;
        nResolution = -1;
        nBitrateType = -1;
        nPicQuality = -1;
        nVideoBitrate = -1;
        nVideoFrameRate = -1;
        nVideoEncType = -1;
        szResolution[0] = '\0';
        szVideoBitrate[0] = '\0';
        szVideoFrameRate[0] = '\0';
    }
}SL_Compression_Info;

// 云台图像区域位置信息
typedef struct tagPTZPointFrame
{
    int nXtop;          // 方框起始点的x坐标
    int nYtop;          // 方框起始点的y坐标
    int nXbottom;       // 方框结束点的x坐标
    int nYbottom;       // 方框结束点的y坐标
    int nCounter;       // 画框方向：1- 左上，2- 右上，3- 左下，4- 右下
    tagPTZPointFrame()
    {
        nXtop = 0;
        nYtop = 0;
        nXbottom = 0;
        nYbottom = 0;
        nCounter = 0;
    }
}SL_PTZ_POINT_FRAME;

#ifdef __cplusplus
extern "C" {
#endif

    //
    /**************************************************************************
    *   Function:           SLStartup
    *   Description:        initialize the API library
    *   Input:              STARTUP_INFO * pStartupInfo: the param needed in initialization                  
    *   Output:             (null)
    *   Return:             return TRUE if successful; otherwise FALSE.
    **************************************************************************/
    STREAM_LAYER_API BOOL SLStartup(STARTUP_INFO * pStartupInfo);
    
    
    /**************************************************************************
    *   Function:           SLCleanup
    *   Description:        对重连库所申请的资源进行释放
    *   Input:              (null)
    *   Output:             (null)
    *   Return:             void
    **************************************************************************/
    STREAM_LAYER_API void SLCleanup();
    
    /**************************************************************************
    *   Function:           SLStartStream
    *   Description:        连接设备或流媒体服务器获取码流数据
    *   Input:              STREAMPARAMS * pStreamParam: 取流参数信息，具体见结构定义
    *                       SLStreamCallback pfnStream: 流数据回调函数指针
    *   Output:             (null)
    *   Return:             打开码流成功，则返回非零的句柄；失败返回NULL
	*   Modify:				qmx 2009-12-2 添加参数用于代理服务器
    **************************************************************************/
    STREAM_LAYER_API HANDLE SLStartStream(STREAMPARAMS * pStreamParam, SLStreamCallback pfnStream, SLMsgNotify msgNotify = NULL);

    /**************************************************************************
    *   Function:           SLStopStream
    *   Description:        停止码流数据获取
    *   Input:              HANDLE hStream: SLStartStream打开成功返回的句柄
    *   Output:             (null)
    *   Return:             失败返回FALSE，成功返回TRUE.
    **************************************************************************/
    STREAM_LAYER_API BOOL SLStopStream(HANDLE hStream);

	/*************************************************************************
	*   Function：			slSetVideoPara 
	*   Description：		设置设备通道的视频参数
	*   Input：                HANDLE hStream: SLStartStream返回的句柄
	*                       nBValue: brightness 亮度(取值为 1-10) 
	*                       nCValue: contrast 对比度(取值为 1-10) 
	*                       nSValue: Saturation 饱和度(取值为 1-10) 
	*                       nHValue: Hue 色度(取值为 1-10) 
	*   Output:             (null)
	*   Return:				TRUE 表示成功，FALSE 表示失败。 
	*************************************************************************/
	STREAM_LAYER_API BOOL slSetVideoPara(HANDLE hStream, int nBValue, int nCValue, int nSValue, int nHValue);

	/*************************************************************************
	*   Function：			slGetVideoPara 
	*   Description：		设置设备通道的视频参数
	*   Input：                HANDLE hStream: SLStartStream返回的句柄                
	*   Output:				pBValue: 存放亮度值的指针(取值为 1-10) 
	*                       pCValue: 存放对比度值的指针(取值为 1-10) 
	*                       pSValue: 存放饱和度值的指针(取值为 1-10) 
	*                       pHValue: 存放色度值的指针(取值为 1-10) 
	*   Return:				TRUE 表示成功，FALSE 表示失败。 
	*************************************************************************/
	STREAM_LAYER_API BOOL slGetVideoPara(HANDLE hStream, int * pBValue, int * pCValue, int * pSValue, int * pHValue);


	/*************************************************************************
	*   Function：			slForceKeyFrame 
	*   Description：		在取流过程中强制生成I帧
	*   Input：                HANDLE hStream: SLStartStream返回的句柄                
	*   Output:				(null)
	*   Return:				TRUE 表示成功，FALSE 表示失败。 
	*************************************************************************/
	STREAM_LAYER_API BOOL slForceKeyFrame(HANDLE hStream);

	/*************************************************************************
	*   Function：			slPTZControl 
	*   Description：		对已经实时预览的图像进行云台控制
	*   Input：                HANDLE hStream: SLStartStream返回的句柄                
	*   Output:				(null)
	*   Return:				TRUE 表示成功，FALSE 表示失败。 
	*************************************************************************/
	STREAM_LAYER_API BOOL slPTZControl(HANDLE hStream, SL_PTZ_COMMAND * pPTZ);

    /** @fn         SLGetCompresssionInfo
     *  @brief      获取当前预览通道的压缩参数信息
     *  @param[IN]  hStream: SLStartStream返回的句柄
     *  @param[OUT] stCompressionInfo: 压缩参数信息
     *  @return     TRUE: 成功, FALSE: 失败
     */
    STREAM_LAYER_API BOOL SLGetCompresssionInfo(HANDLE hStream, SL_Compression_Info &stCompressionInfo);

    /** @fn         SLPTZSelZoomInEx
     *  @brief      云台图象区域选择放大或缩小(即3D定位功能)
     *  @param[IN]  hStream: SLStartStream返回的句柄
     *  @param[IN]  stPTZPointFrame: 云台图像区域位置信息
     *  @return     TRUE: 成功, FALSE: 失败
     */
    STREAM_LAYER_API BOOL SLPTZSelZoomInEx(HANDLE hStream, const SL_PTZ_POINT_FRAME &stPTZPointFrame);

    /** @fn         SLStartSteam_V30
    *  @brief       仅适用于iVMS-8100 v3.0的取流接口
    *  @param[IN]   const char* pMsg: 监控点信息
    *  @param[IN]   int nMsgSize: 监控点信息长度
    *  @param[IN]   pfnStream: 流数据回调函数
    *  @param[IN]   msgNotify: 消息回调
    *  @param[IN]   hUserData: 用户自定义数据
    *  @param[OUT]  int& nPlayType: 播放类型
    *  @return      返回取流对象句柄，失败返回NULL
    */
    STREAM_LAYER_API HANDLE SLStartStream_V30(const char* pMsg, int nMsgSize, int& nPlayType, SLStreamCallback pfnStream, SLMsgNotify msgNotify, void *hUserData);

    /** @fn         SLStartSteam_OCX
    *  @brief       仅适用于iVMS-8100 v3.0 OCX控件使用的取流接口
    *  @param[IN]   LPCTSTR szMsg: 监控点信息,xml形式
    *  @param[IN]   pfnStream: 流数据回调函数
    *  @param[IN]   msgNotify: 消息回调
    *  @param[IN]   hUserData: 用户自定义数据
    *  @param[OUT]  int& nPlayType: 播放类型
    *  @return      返回取流对象句柄，失败返回NULL
    */
    STREAM_LAYER_API HANDLE SLStartStream_OCX(const char* pMsg, int& nPlayType, SLStreamCallback pfnStream, SLMsgNotify msgNotify, void *hUserData);

    /** @fn         SLZeroTurnOver
     *  @brief      零通道预览画面翻页
     *  @param[IN]  hStream: SLStartStream返回的句柄
     *  @param[IN]  bNextPreview: 向下或向上翻页：TRUE-下一页；FALSE-上一页
     *  @return     TRUE: 成功, FALSE: 失败
     */
    STREAM_LAYER_API BOOL SLZeroTurnOver(HANDLE hStream, BOOL bNextPreview);

    STREAM_LAYER_API void SLStreamInfo(const char* pMsg, int nMsgSize, char* szServerInfo, int nServerInfoSize, char* szStreamDesc, int nDescSize);
#ifdef __cplusplus
}
#endif

#endif // __STREAM_LAYER_API__
