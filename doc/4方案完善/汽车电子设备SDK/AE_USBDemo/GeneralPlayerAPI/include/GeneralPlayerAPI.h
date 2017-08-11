/**************************************************************************
*   Copyright 2007-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayerHie.h
*   Description:    播放库API
*   Author:         created by 薛智虎 and  continued by 刘大煜  and continued by 刘兵
*   Date:           24/3/2010
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         24/3/2010       刘大煜         created
**************************************************************************/
#ifndef GUARD_GENERALPLAYERAPI_H
#define GUARD_GENERALPLAYERAPI_H

#ifdef GENERALPLAYERAPI_EXPORTS
#define GENERALPLAYER_API  __declspec(dllexport)
#else
#define GENERALPLAYER_API __declspec(dllimport)
#endif

//定义播放类型
typedef enum _PlayerType{
	PT_HIK = 0,          //海康设备
	PT_DAHUA,			//大华设备
	PT_BSR,				//蓝色星际设备
	PT_HIE,				//恒亿设备、城丰设备
	PT_TM,				//图敏设备
	PT_DALI,			//大立设备
	PT_QT,				//秋田设备
	PT_DL400,			//大立400设备
	PT_SS,				//三星设备
	PT_INF,				//英飞特设备
	PT_BSRLimit,		//蓝色星际limit
	PT_HENGTONG,		//恒通设备
	PT_SHENGXUN,		//声迅设备
	PT_ZHINUO,			//智诺设备
	PT_LULINWANG,		//录林王设备
	PT_TIANDIIPC,		//天地伟业设备
	PT_WAV,				//标准wav格式非扩展音频
	PT_TIANYUE,			//天跃设备
	PT_HUANGHE,			//黄河IPC
	PT_GB28181,			//国家标准GB28181码流
	PT_WSQ,				//伟视清设备
	PT_HUAANTAI,		//华安泰设备
	PT_VAG,				//VAG接入设备码流
	PT_HB,				//汉邦设备
}PLAYERTYPE;

//定义流播放类型
typedef enum _StreamMode {
	SM_REALTIME = 0,
	SM_FILE
}STREAMMODE;

//图像格式
typedef enum _resolution{
	R_CIF = 0,
	R_2CIF,
    R_4CIF,
	R_D1,
    R_HD1
}RESOLUTION;

//视频格式
typedef enum _videoFormat{
	VF_PAL = 0,
	VF_NTSC
}GP_VIDEOFORMAT;

//保存图片类型
typedef enum _PictureFormat{
	PF_BMP = 0,
	PF_JPEG,
}PICTUREFORMAT;

//播放速度
typedef enum _PlaySpeed{
	PS_NORMAL = 0, //正常速度
	PS_Fast2 = 1,	// 2倍速(快进)					
	PS_Fast4 = 2,	// 4倍速(快进)					
	PS_Fast8 = 3,	// 8倍速(快进)					
	PS_Fast16 = 4,	// 16倍速(快进)			
	PS_SLOW2 = -1,	// 1/2速(慢进)					
	PS_SLOW4 = -2,	// 1/4速(慢进)					
	PS_SLOW8 = -3,	// 1/8速(慢进)					
	PS_SLOW16 = -4,	// 1/16速(慢进)
}PLAYSPEED;

//海康解码回调图像与声音信息
typedef struct{
	long nWidth;
	long nHeight;
	long nStamp;
	long nType;
	long nFrameRate;
	DWORD dwFrameNum;
}HIK_FRAME_INFO;

//VAG解码回调图像帧信息
typedef struct tag_VAG_FrameInfo											// 图像帧信息
{
	int iWidth;															// 图像宽
	int iHeight;														// 图像高
	int iTimeStamp;														// 时间戳
	int iFrameType;														// 图像yuv类型

	tag_VAG_FrameInfo()
	{
		iWidth = 0;
		iHeight = 0;
		iTimeStamp = 0;
		iFrameType = 0;
	}
}VAG_VPLAY_FRAME_INFO, *LVAG_FRAME_INFO;

enum VAG_SNAP_MODE
{
	VAG_SNAP_ONCELICK = 0,											// 单张抓拍
	VAG_SNAP_CONTINUOUS_BYTIME =1,									// 按时间连续抓拍
	VAG_SNAP_CONTINUOUS_BYFRAME = 2,									// 按帧连续抓拍
	VAG_SNAP_ONE_BLOCK = 3											// 堵塞模式抓图,单张
};

// VAG抓图模式
typedef struct tag_VAG_SnapModeStruct										
{
	VAG_SNAP_MODE enumSnapMode;										// 抓图模式
	int iSnapTimes;														// 连续抓拍次数,当抓图模式为VAG_SNAP_CONTINUOUS_BYTIME
	// 或VAG_SNAP_CONTINUOUS_BYFRAME时,需要设置该值
	int iSpan;															// 抓拍间隔,单位:毫秒

	tag_VAG_SnapModeStruct()
	{
		enumSnapMode = VAG_SNAP_ONCELICK;
		iSnapTimes = 0;
		iSpan = 0;
	}
}VAG_SNAP_MODE_PARA, *LVAG_SNAP_MODE_PARA;

// VAG图片格式
enum VAG_SNAP_PICTYPE													
{
	VAG_PICTYPE_JPG = 0,
	VAG_PICTYPE_BMP = 1
};

//VAG图片信息
typedef struct tag_VAG_SnapPicStruct										
{
	VAG_SNAP_PICTYPE enumPicType;										// 图片格式
	int iPicQuality;													// 0-100,0最差,100 最好,默认80 

	tag_VAG_SnapPicStruct()
	{
		enumPicType = VAG_PICTYPE_JPG;
		iPicQuality = 80;
	}
}VAG_SNAP_PIC_PARA,* LVAG_SNAP_PIC_PARA;

//VAG抓图参数
typedef struct tag_VAG_SnapStruct
{
	unsigned int nFlag;													// 额外参数 //HIWORD(dwFlag) id LOWORD(dwFlag) type
	int	iSnapId;
	int iSnapType;
	VAG_SNAP_PIC_PARA stPicType;										// 图片格式
	VAG_SNAP_MODE_PARA stSnapMode;										// 抓图模式
	char szSavePath[260];												// 保存目录,默认存放系统临时目录::GetTempPath
	char szKeyName[260];												// 图片名称应当包含的关键字

	tag_VAG_SnapStruct()
	{
		nFlag = 0;
		szSavePath[0] = '\0';
		szKeyName[0] = '\0';
	}
}VAG_SNAP_PARA, *LVAG_SNAP_PARA;

typedef struct tag_VAG_MediaHead
{
	unsigned char nMediaMagic[4];										// HVAG
	unsigned short nMediaVersion;										// 版本号:指本信息结构版本号,目前为0x0101,即1.01版本,01:主版本号;01:子版本号.

	unsigned short nRev1;
	unsigned short nSystemFormat;										// 系统封装层
	unsigned short nVideoFormat;										// 视频编码类型
	unsigned char chRev2[12];
	unsigned char nDecodeId[10];										// 解码插件id
	unsigned char nPrivateSystemFormat;									// 是否有私有头以及私有头类型,该字段由转封装库开发人员定义
	unsigned char cnRev3[3];
	unsigned short nStreamHeadLen;										// 辅助码流头
}VAG_MEDIA_HEAD, *LPVAG_MEDIA_HEAD;

//媒体头分析结果
typedef struct tag_VAG_MediaHeadAnalInfo									// 媒体头分析结果
{
	VAG_MEDIA_HEAD stSysMediaHead;									// 媒体系统头信息(一般为40字节)
	int iSysMediaHeadLen;												// 媒体系统头信息长度
	char* pPriMediaHead;												// 媒体私有头信息(各厂家自定义的媒体头)
	int iPriMediaHeadLen;												// 媒体私有头信息长度
}VAG_MEDIA_HEAD_ANAL_INFO, *LPVAG_MEDIA_HEAD_ANAL_INFO;

//VAG视频数据类型
enum VAG_STREAM_DATA_TYPE
{
	GPAPI_VAG_SYSHEAD = 1,													// 系统头数据
	GPAPI_VAG_STREAMDATA,													// 视频流数据(包括复合流和音视频分开的视频流数据)
	GPAPI_VAG_AUDIOSTREAMDATA,												// 音频流数据
	GPAPI_VAG_STD_VIDEODATA,												// 标准视频流数据
	GPAPI_VAG_STD_AUDIODATA													// 标准音频流数据
};

//解码帧类型说明
#define DECODE_NORMAIL 0		//正常解码
#define DECODE_KEY_FRAME 1		//只解关键帧
#define DECODE_NONE 2			//不解视频帧

//默认缓冲区大小
const DWORD DEFAULT_PLAY_BUFFER = 1024 * 1024; // 默认播放缓冲大小为一兆

//默认存放帧数
const DWORD DEFAULT_DISPLAY_BUFFER = 15; // 默认存放15帧的画面，25帧/s 的情况下为0.6秒的数据

//大立设备初始化消息回调函数
typedef void (CALLBACK *GPAPI_NotifyCallBack)(UINT Msg, UINT wParam, UINT lPara, void* cbpara);

/**************************************************************************
*   Function:		VIDEODRAWFUNC
*   Description:    画图回调函数
*   Param:          long nPort 播放端口
*					HDC hDC 表面设备上下文
*					LONG nUser 用户数据
**************************************************************************/
typedef void (CALLBACK *VIDEODRAWFUNC)(LONG nPort, HDC hDC, LONG nUser);

/**************************************************************************
*  Function:  GPAPI_HIKSetDecCallBack
*  brief:     海康、GB28181设备解码回调函数
*  parameter: long nPort 播放端口
*  parameter: GPAPI_HIKSetDecCallBack 用户定义解码回调
*  parameter: char * pDest 目标数据
*  parameter: long nDestSize 目标数据大小
*  parameter: long nUser   用户自定义数据
**************************************************************************/
typedef void (CALLBACK *GPAPI_HIKSetDecCallBack)(long nPort, char* pBuf, long nSize, HIK_FRAME_INFO* pFrameInfo,
												 long nUser, long nReserved2);

/**************************************************************************
 *  Function:  VAG_fDecodeDataCallback
 *	@brief	<VAG设备解码数据回调函数>
 *	@param iPlayHandle 解码句柄
 *	@param pDataBuf 数据指针
 *	@param iDataSize 数据长度
 *	@param pstFrameInfo 图像帧信息
 *	@param pUserData 用户自定义数据
 */
typedef void (CALLBACK* VAG_fDecodeDataCallback)(int iPlayHandle, const char* pDataBuf, int iDataSize, LVAG_FRAME_INFO pstFrameInfo, void* pUserData);

/**	@fn	int VAG_fSnapCallback
 *	@brief	<VAG设备抓图回调函数>
 *	@param iPlayHandle 解码句柄
 *	@param pSavePath 图片路径
 *	@param iErrorcode 错误码
 *	@param stSnapPara 抓图参数
 *	@param pUserData 用户自定义数据
 */
typedef void (CALLBACK* VAG_fSnapCallback)(int iPlayHandle, const char* pSavePath, int iErrorcode, LVAG_SNAP_PARA pstSnapPara, void* pUserData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<VAG设备原始媒体数据回调>
 *	@param iPlayHandle 解码句柄
  *	@param iDataType 数据类型 1－头数据，2－码流数据
 *	@param iDataType 数据
 *	@param pData 数据长度
 *	@param pUser 用户自定义数据
 */
typedef void(CALLBACK* VAG_fGPAPIStreamDataCallBack)(int iPlayHandle, int iDataType, void* pData, int iDataSize, void* pUser);

/**************************************************************************
*   Function:		GPAPI_Startup
*   Description:    初始化设备类型的播放库，最开始调用
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   HWND owner, 大立、秋田、录林王设备可能需使用，
								大立设备：owner 窗口句柄，用来接收播放库在解码播放的过程中产生的一些消息，若不需要可置为NULL
								秋田设备：owner 保留，暂时无效，可置为NULL
							    录林王设备:播放库动态加载，owner为1表示不录林王内部不初始化其网络库
							    其它设备该值置为NULL
*                   GPAPI_NotifyCallBack Notify,大立设备初始化时需用到消息回调，其它设备设置为NULL
*                   void* cbpara,大立设备初始化时需用到自定义数据，其它设备设置为NULL
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Startup(PLAYERTYPE playerType, HWND owner, GPAPI_NotifyCallBack Notify, void* cbpara);

/**************************************************************************
*   Function:		GPAPI_Cleanup
*   Description:    反初始化设备类型播放库，结束时调用
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Cleanup(PLAYERTYPE playerType);

/**************************************************************************
*   Function:		GPAPI_GetPort
*   Description:    获取未使用的播放端口号
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*	Input:			HWND hwnd,显示窗体句柄
							  暂只支持设备：图敏、恒通、声讯
							  其它设备置为NULL
*   Output:         LONG& nPort,引用，返回未使用的端口号
*   Return:			BOOL, TRUE:获取成功,FALSE:获取失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_GetPort(PLAYERTYPE playerType, LONG& nPort,HWND hwnd = NULL);

/**************************************************************************
*   Function:		GPAPI_FreePort
*   Description:    释放已使用的播放端口号
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort,nPort 播放端口号 
*   Output:         (null)
*   Return:			BOOL, TRUE:获取成功,FALSE:获取失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FreePort(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StartPlay
*   Description:    开始播放，此接口整合了常见的SetStreamOpenMode
*                   OpenStream，和Play接口，需在GPAPI_GetPort函数后调用
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口
*                   HWND hWnd， 播放（显示图像的）窗口句柄
*                   PBYTE pFileHeadBuf， 文件头缓冲区指针
*                   DWORD nHeadSize， 文件头缓冲大小			
*					char *strOsdChannelName 视频窗体显示的内容中增加显示扩展信息，暂只支持设备：蓝色星际、大立设备。
					VIDEODRAWFUNC fnVideoDraw 画图回调函数，暂只支持设备：海康、大华、蓝色星际、恒亿、图敏、大立、大立400、秋田、VAG、汉邦
					LONG nUser 画图回调用户自定义数据
*                   DWORD nBufPoolSize， 播放缓冲大小，默认1兆（DEFAULT_PLAY_BUFFER）
*                   STREAMMODE nMode，播放方式，默认实时流模式（SM_REALTIME）
*                   int frameRate,播放缓冲区最大缓冲帧数，默认25帧,海康高清设备可设置为6帧，取值6－50
*                   VIDEOFORMAT videoFormat, 视频制式，大立专用，默认PAL（VF_PAL）
*                   RESOLUTION resolution， 码流分辨率，大立专用，默认CIF（R_CIF）
*                   
*   Output:         (null)
*   Return:			long，海康，大华返回是否播放成功，成功返回0，失败返回-1
*                   大立返回播放句柄
**************************************************************************/
GENERALPLAYER_API long GPAPI_StartPlay(PLAYERTYPE playerType, LONG& nPort, HWND hWnd, 
					  PBYTE pFileHeadBuf, DWORD nHeadSize, char * strOsdChannelName = "大立科技", VIDEODRAWFUNC fnVideoDraw = NULL, LONG nUser = 0, DWORD nBufPoolSize = DEFAULT_PLAY_BUFFER, 
					  STREAMMODE nMode = SM_REALTIME, int frameRate = 25, GP_VIDEOFORMAT videoFormat = VF_PAL,
					  RESOLUTION resolution = R_CIF);

/**************************************************************************
*   Function:		GPAPI_StopPlay
*   Description:    停止播放，此接口整合了常见的CloseStream和Stop接口
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放端口
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_StopPlay(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_InputData
*   Description:    向播放库输入数据，塞数据，需在GPAPI_StartPlay后调用
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort，播放用端口，大立是播放句柄
*                   PBYTE pBuf，流数据缓冲地址
*                   DWORD nSize， 流数据大小
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_InputData(PLAYERTYPE playerType, LONG nPort, PBYTE pBuf, DWORD nSize);

/**************************************************************************
*   Function:		GPAPI_ResetSourceBuffer
*   Description:    清除流播放模式下源缓冲区剩余数据，暂不支持三星、蓝色星际Limit,恒通、秋田、英飞特、天跃设备、华安泰
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放端口
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ResetSourceBuffer(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_Pause
*   Description:    暂停
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口
*                   BOOL bPause, TRUE则暂停，FALSE则继续
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Pause(PLAYERTYPE playerType, LONG nPort,BOOL bPause);

/**************************************************************************
*   Function:		GPAPI_SetPlaySpeed
*   Description:    设置播放速度
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*                   PLAYSPEED playSpeed，播放速度 
*                   HWND hWnd, 播放显示窗口
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlaySpeed(PLAYERTYPE playerType, LONG nPort,PLAYSPEED playSpeed, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_SetPlayReverseSpeed
*   Description:    设置倒放播放速度
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华、VAG等标识
*                   LONG nPort, 播放用端口或播放句柄
*                   PLAYSPEED playSpeed，播放速度 
*                   HWND hWnd, 播放显示窗口
*   Return:			BOOL, TRUE:成功,FALSE:失败 
*	Remark:			目前只支持海康设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlayReverseSpeed(PLAYERTYPE playerType, LONG nPort,PLAYSPEED playSpeed, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_Capture
*   Description:    自动抓图（BMP或JPEG）
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识，暂不支持恒通
*                   LONG nPort, 播放用端口或播放句柄
*                   LPCTSTR strFileFullPath，抓图保存的绝对路径
*                   PICTUREFORMAT picFormat，抓图格式，默认BMP
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Capture(PLAYERTYPE playerType, LONG nPort, const char* strFileFullPath, 
					PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_CaptureByUser
*   Description:    用户手动保存抓图 不能设置图像大小，最大支持抓图大小为25M
*   Input:          PLAYERTYPE playerType,播放器类型，暂只支持海康，大华设备、恒亿（只支持BMP格式）、图敏（只支持BMP格式）、
					大立400设备（只支持BMP格式)、黄河设备（只支持BMP格式)、华安泰设备（只支持BMP格式)
*                   LONG nPort, 播放用端口或播放句柄
*					PBYTE pBitmap, 存放数据图像地址，不得小于图像大小
*					          推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
*					          其中w和h分别为图像宽高.
*					DWORD bmpSize,获得了实际的图像大小
*                   PICTUREFORMAT picFormat，抓图格式，默认BMP
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_CaptureByUser(PLAYERTYPE playerType, LONG nPort, PBYTE pBitmap, DWORD &bmpSize, 
									 PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_CaptureByUserEx
*   Description:    手动抓图扩展 可设置图像大小
*   Input:          PLAYERTYPE playerType,播放器类型，暂只支持海康，大华、GB28181、VAG设备
*                   LONG nPort, 播放用端口或播放句柄
*					PBYTE pBitmap, 存放数据图像地址，不得小于图像大小
*					          推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
*					          其中w和h分别为图像宽高.
*					DWORD dBufSize,申请缓冲区大小
*                   PICTUREFORMAT picFormat，抓图格式，默认BMP
*   Output:         DWORD & bmpSize 获得的实际图像大小
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_CaptureByUserEx(PLAYERTYPE playerType, LONG nPort, 
											 PBYTE pBitmap,DWORD dBufSize,DWORD &bmpSize,PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_GetPictureSize
*   Description:    获取原始图像大小，暂只支持海康、大华、华安泰设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         pWidth  原始图像的宽度
*					pHeight 原始图像的高度
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_GetPictureSize(PLAYERTYPE playerType,LONG nPort, LONG * pWidth, LONG * pHeight);

// 画图回调函数(隐藏这个接口，需要注册直接在StartPlay中进行就行)
//GENERALPLAYER_API BOOL GPAPI_RigisterDrawFun(PLAYERTYPE playerType,LONG nPort,void (CALLBACK* DrawFun)(long nPort,HDC hDc,LONG nUser),LONG nUser);

/**************************************************************************
*   Function:		GPAPI_SetDisplayBuf
*   Description:    设置播放缓冲,暂只支持海康、大华、蓝星、恒亿、大立设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口
*                   DWORD nNum,缓冲区帧数，取值范围1-50
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayBuf(PLAYERTYPE playerType, LONG nPort,DWORD nNum);

/**************************************************************************
*   Function:		GPAPI_ThrowBFrameNum
*   Description:    设置不解码 B 帧帧数;不解码 B 帧,可以减小CPU利用率,
*                   如果码流中没有 B 帧，那么设置这个值将不会有作用。
*                   如在快速播放，和支持多路而 CPU利用率太高的情况下可以考虑使用。
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口
*                   DWORD nNum, 不解码 B 帧的帧数(0-2)
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ThrowBFrameNum(PLAYERTYPE playerType, LONG nPort, DWORD nNum);

/**************************************************************************
*   Function:		GPAPI_SetDisplayCallBack
*   Description:    设置抓图回调
*   Input:          PLAYERTYPE playerType 播放类型,如海康，大华等标识
*					LONG nPort   播放端口
*					DisplayCBFun 抓图回调函数，如不需要可置为NULL
*   Output:         (null)
*   Return:			BOOL TRUE表示成功，FALSE表示失败
*   Remark:         暂只支持海康、大华、恒亿设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayCallBack(PLAYERTYPE playerType, LONG nPort, void (CALLBACK *DisplayCBFun)(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long Receaved));

/**************************************************************************
*   Function:		GPAPI_GetLastError
*   Description:    获取解码库错误码，最好使用GPAPI_GetLastErrorString接口获取厂家标识
*   Input:          PLAYERTYPE playerType 播放类型,如海康，大华等标识
*					LONG nPort   播放端口
*   Output:         (null)
*   Return:			DWORD 错误码
*   Remark:         暂只支持海康、大华、蓝色星际、恒亿、大立、天地伟业、黄河、伟视清、华安泰、VAG码流设备
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetLastError(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_GetLastErrorString
*   Description:    获取带有厂家标识的错误码
*   Input:          PLAYERTYPE playerType 播放类型,如海康，大华等标识
*					LONG nPort   播放端口
*   Output:         (null)
*   Return:			const char* 带有厂家标识的错误信息
*   Remark:         暂只支持海康、大华、蓝色星际、恒亿、大立、天地伟业、黄河、伟视清、华安泰、VAG码流设备
**************************************************************************/
GENERALPLAYER_API const char* GPAPI_GetLastErrorString(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*  Function:  GPAPI_SetDecCallBack
*  brief:     设置播放库解码回调
*  parameter: long nPort 播放端口
*  parameter: SetDecCallBack 用户定义解码回调
*  parameter: char * pDest 目标数据
*  parameter: long nDestSize 目标数据大小
*  parameter: long nUser   用户自定义数据
*  returns:   BOOL 成功返回TRUE,失败返回FALSE
*  remark:    暂只支持海康设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDecCallBack(PLAYERTYPE playerType,long nPort,void* DecDataCallBack,char* pDest, long nDestSize, long nUser);

// 以下是声音函数
/**************************************************************************
*   Function:		GPAPI_PlaySound
*   Description:    打开声音；同一时刻只能有一路声音。
*                   如果现在已经有声音打开，则自动关闭原来已经打开的声音。
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_PlaySound(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StopSound
*   Description:    关闭声音
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_StopSound(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_PlaySoundShare
*   Description:    打开共享声音；同一时刻可以打开多路声音,暂只支持海康、大华设备
*                   以共享方式播放声音,播放本路声音不会关闭其他路的声音。
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识。
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_PlaySoundShare(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StopSoundShare
*   Description:    关闭共享声音,暂只支持海康、大华设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_StopSoundShare(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetVolume
*   Description:    设置音量,可以在播放之前设置，暂不支持三星、蓝色星际Limit、恒通、英飞特、华安泰设备
*                   但设置的值被保存，并作为启动声音时的初始音量；
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识。
*                   LONG nPort, 播放用端口或播放句柄
*                   WORD nVolume, 音量的值，范围 0-100; 
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_SetVolume(PLAYERTYPE playerType, LONG nPort, WORD nVolume);

/**************************************************************************
*   Function:		GPAPI_GetVolume
*   Description:    获得当前设置的音量。暂不支持三星、蓝色星际Limit,恒通、英飞特设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			WORD,音量值
**************************************************************************/
GENERALPLAYER_API WORD  GPAPI_GetVolume(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetDisplayRegion
*   Description:    设置或增加显示区域,可以做局部放大显示。暂只支持海康、大华、GB28181、智诺设备
*   Input:          PLAYERTYPE playerType,播放器类型
*                   LONG nPort, 播放用端口或播放句柄
*					DWORD nRegionNum，显示区域序号，0~3。
									如果nRegionNum 为0 ， 表示对主要显示窗口
									(PlayM4_Play 中设置的窗口) 进行设置， 将忽略
									hDestWnd 和bEnable 的设置。
*					RECT * pSrcRect，设置在要显示的原始图像上的区域，如：如果原始图
									像是352*288，那么pSrcRect 可设置的范围只能在（0，
									0，352，288）之中。如果pSrcRect=NULL,将显示整个
									图像。
*					HWND hDestWnd，设置显示窗口。如果该区域的窗口已经设置过（打开
									过），那么该参数被忽略。
*					BOOL bEnable, 打开（设置）或关闭显示区域
*   Return:			成功返回TURE；失败返回FALSE
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayRegion(PLAYERTYPE playerType,LONG nPort,DWORD nRegionNum, RECT *pSrcRect,
											  HWND hDestWnd, BOOL bEnable);

// 文件操作函数
/**************************************************************************
*   Function:		GPAPI_FileStart
*   Description:    打开播放文件,文件大小不能超过4G或小于4K
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*                   LPCTSTR strFileName,文件名
*                   HWND hWnd, 播放窗口
					VIDEODRAWFUNC fnVideoDraw,画图回调函数，暂不支持设备：恒通、英飞特设备
					LONG nUser，画图回调用户自定义数据
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FileStart(PLAYERTYPE playerType, LONG &nPort, LPSTR strFileName, HWND hWnd, VIDEODRAWFUNC fnVideoDraw = NULL, LONG nUser = 0);

/**************************************************************************
*   Function:		GPAPI_FileStop
*   Description:    关闭播放文件
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FileStop(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_SetPlayPos
*   Description:    设置文件播放指针的相对位置（百分比）,暂不支持三星、恒通、英飞特设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*                   float fRelativePos, 播放位置，范围 0-100%
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlayPos(PLAYERTYPE playerType, LONG nPort,float fRelativePos); 

/**************************************************************************
*   Function:		GPAPI_GetPlayPos
*   Description:    获得文件播放指针的相对位置，暂不支持三星、恒通、英飞特设备
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
*                   LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			float, 播放位置，范围 0-100%
**************************************************************************/
GENERALPLAYER_API float GPAPI_GetPlayPos(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetFileTime
*   Description:    获取文件总的时间长度，单位秒，暂不支持三星、恒通、英飞特设备
*   Input:			PLAYERTYPE playerType,播放器类型，如海康，大华等标识      
					LONG nPort, 播放用端口或播放句柄
*   Return:			DWORD 文件总的时间长度,单位秒
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetFileTime(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetPlayedTime
*   Description:    获取文件当前播放的时间，单位秒，暂不支持三星、恒通、英飞特设备
*   Input:			PLAYERTYPE playerType,播放器类型，如海康，大华等标识       
					LONG nPort, 播放用端口或播放句柄
*   Return:			DWORD 文件当前播放的时间，单位秒
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetPlayedTime(PLAYERTYPE playerType, LONG nPort); 


/**************************************************************************
*   Function:		GPAPI_GetCurrentFrameRate
*   Description:    获取当前帧率，暂只支持海康、大华、GB28181、VAG设备
*   Input:			PLAYERTYPE playerType,播放器类型，如海康，大华等标识       
					LONG nPort, 播放用端口或播放句柄
	Remark:			如果码流中的帧率小于1，则返回0
*   Return:			DWORD 当前帧率
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetCurrentFrameRate(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetPlayedFrames
*   Description:    获取当前已解码的视频帧数，暂只海康、大华、GB28181、VAG设备
*   Input:			PLAYERTYPE playerType,播放器类型，如海康，大华等标识       
					LONG nPort, 播放用端口或播放句柄
*   Return:			DWORD 已解码的视频帧数
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetPlayedFrames(PLAYERTYPE playerType, LONG nPort); 


/**************************************************************************
*   Function:		GPAPI_GetSpecialData
*   Description:    获取当前显示帧的全局时间,暂只支持海康、大华，有正常图像显示后才能调用此接口
*   Input:          PLAYERTYPE playerType,播放器类型，如海康，大华等标识
					LONG nPort, 播放用端口或播放句柄
*   Return:			DWORD 失败返回-1,成功返回一个全局时间的压缩值，精确到秒
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetSpecialData(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_SetFileEndCallback
*   Description:    设置文件播放结束回调函数。该接口在OpenStream或OpenFile 之前
*                   调用才有效。 
*   Input:          PLAYERTYPE playerType 播放类型
*					LONG nPort, 播放用端口或播放句柄
*					FileEndCallback，回调函数 
*                   void *pUser, 回调函数参数
*   Return:			BOOL,成功返回TRUE,失败返回FALSE
*	Remark:			暂只支持海康
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetFileEndCallback(PLAYERTYPE playerType, LONG nPort,void(CALLBACK*FileEndCallback)(long nPort, void *pUser),void *pUser);

//Display type
#define DISPLAY_NORMAL  1   
#define DISPLAY_QUARTER 2
/**************************************************************************
*   Function:		GPAPI_SetDisplayType
*   Description:    设置显示模式
*   Input:          LONG nPort, 播放用端口或播放句柄LONG nType
					LONG nType，两种模式：
					DISPLAY_NORMAL 正常分辨率数据送显卡显示。
					DISPLAY_QUARTER 1/4 分辨率数据送显卡显示。
					DISPLAY_YC_SCALE YC 伸张
					DISPLAY_NOTEARING 无显示撕裂效果
*   Return:			成功返回TURE；失败返回FALSE
*	Remark:			只支持海康设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayType(PLAYERTYPE playerType, LONG nPort, LONG nType);

/**************************************************************************
*   Function:		GPAPI_GetSourceBufferRemain
*   Description:    获取播放缓冲区剩余数据大小
*   Input:          LONG nPort, 播放用端口或播放句柄
*   Return:			DWORD:数据大小，单位[BYTE]
*	Remark:			暂时只支持海康、大华、GB28181、VAG、伟视清设备
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetSourceBufferRemain(PLAYERTYPE playerType, LONG lPort);

/**************************************************************************
*   Function:		GPAPI_GetBufferValue
*   Description:    获取指定缓冲区的大小
*   Input:          LONG nPort, 播放用端口或播放句柄,nBufType,缓冲区类型.
*   Return:			DWORD:数据大小
*	Remark:			暂时只支持海康、大华
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetBufferValue(PLAYERTYPE playerType, LONG nPort,DWORD nBufType);

/**************************************************************************
*   Function:		GPAPI_PlayOneByOne
*   Description:    单帧前进
*   Input:          LONG nPort, 播放用端口或播放句柄
*   Return:			BOOL,TRUE表示设置成功
*	Remark:			暂时只支持海康、大华、GB28181设备、VAG、华安泰、伟视清
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOne(PLAYERTYPE playerType, LONG nPort);

/*************************************************************************** 
*  Function:		GPAPI_PlayOneByOneRestore
*  Description:     单帧播放恢复
*  Input:           LONG nPort, 播放用端口或播放句柄,HWND hWnd,播放窗口.
*  Return:			BOOL,TRUE表示设置成功
*  Remark:			暂时只支持海康、大华、GB28181、VAG、伟视清设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOneRestore(PLAYERTYPE playerType, LONG nPort, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_PlayOneByOneBack
*   Description:    单帧回退。每调用一次倒退一帧。只支持文件播放，此函数必须在文件索引
*                   生成之后才能调用 
*   Input:          PLAYERTYPE playerType,播放器类型，LONG nPort, 播放用端口或播放句柄
*  Return:			BOOL,TRUE表示设置成功
*  Remark:			暂时只支持海康、大华、GB28181、VAG、伟视清设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOneBack(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_ReversePlay
*   Description:    回放倒放
*   Input:          PLAYERTYPE playerType,播放器类型，LONG nPort, 播放用端口或播放句柄.
*   Return:			BOOL,TRUE表示设置成功
*	Remark:			暂时只支持海康设、GB28181设备，海康播放库6.3版本开始支持该接口，
                    该接口必须在PLAYM4_Play之后调用
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ReversePlay(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetDecodeFrameType
*   Description:    设置视频帧解码类型
*   Input:          PLAYERTYPE playerType,播放器类型，LONG nPort, 播放用端口或播放句柄
					DWORD nFrameType 解码帧类型说明
					#define DECODE_NORMAIL 0		//正常解码
					#define DECODE_KEY_FRAME 1		//只解关键帧
					#define DECODE_NONE 2			//不解视频帧
*   Return:			BOOL,TRUE表示设置成功
*	Remark:			暂时只支持海康、VAG码流设备
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDecodeFrameType(PLAYERTYPE playerType, LONG nPort,DWORD nFrameType);

/**************************************************************************
*   Function:		GPAPI_RenderPrivateData
*   Description:    显示私有数据（智能信息显示开关）。
*   Input:          PLAYERTYPE playerType,播放器类型，LONG nPort, 播放用端口或播放句柄
					int nIntelType 内部类型
					enum _PLAYM4_PRIDATA_RENDER{
					PLAYM4_RENDER_ANA_INTEL_DATA    = 0x00000001, //智能分析
					PLAYM4_RENDER_MD                = 0x00000002, //移动侦测
					PLAYM4_RENDER_ADD_POS           = 0x00000004  //POS信息后叠加
					}PLAYM4_PRIDATA_RENDER
					BOOL bTrue	是否显示
*   Return:			成功返回TRUE;错误时返回FALSE
*	Remark:			暂时只支持海康码流。
					此接口适用于带有智能信息的码流，
					若无智能信息的码流则调用此接口没有效果。
					POS叠加信息是默认关闭的，其他两个是默认开启的。
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_RenderPrivateData(PLAYERTYPE playerType,LONG nPort,int nIntelType, BOOL bTrue);



// 通用类型接口，所有播放类型的码流数据保存时都需调用以下流程
/**************************************************************************/
/**	@fn	GPAPI_SetTransFormatCallback
*	@brief	<设置原始码流封装转换回>
*	@param   <playerType 播放类型>
*	@param	<nPort (IN) 播放用端口或播放句柄，该nPort值需单独创建不能和正在播放的nPort共用>
*	@param	<pStreamDataFun (IN)封装转换后数据回调函数>
*	@param	<pcomplexHead (char*)复合头数据>
*	@param	<complexHeadLen (IN)复合头长度>
*	@param	<pUserData (IN)用户数据>
*	@remark  调用步骤   GPAPI_SetTransFormatCallback
									GPAPI_AnalyzeMediaHead	  
									GPAPI_InputTransformData
*	@return	成功返回TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_SetTransFormatCallback(PLAYERTYPE playerType, LONG nPort, VAG_fGPAPIStreamDataCallBack pStreamDataFun, const char* pcomplexHead, int complexHeadLen, void* pUserData);

/**	@fn	    GPAPI_AnalyzeMediaHead
*	@brief	<分析pStreamDataFun中回调码流头>
*	@param   <playerType 播放类型>
*	@param	<pMediaHead (IN)媒体头数据指针>
*	@param	<iMediaHeadLen (IN)媒体头数据长度>
*	@param	<pstMediaAnalyzeInfo (OUT)返回的分析结果> 
					<pMediaHead 应传入GPAPI_SetTransFormatCallback回调出来的头数据，
					LPVAG_MEDIA_HEAD_ANAL_INFO 中的
					char* pPriMediaHead 设备原始头放在文件最开始，中间放回调出来的码流数据
					stSysMediaHead 放在文件最末尾>
*	@return	成功返回 TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_AnalyzeMediaHead(PLAYERTYPE playerType, const char* pMediaHead, int iMediaHeadLen, LPVAG_MEDIA_HEAD_ANAL_INFO pstMediaAnalyzeInfo);

/**	@fn	GPAPI_InputTransformData
*	@brief	<输入视频数据流>
*	@param   <playerType 播放类型>
*	@param	<nPort (IN) 播放用端口或播放句柄，应和GPAPI_SetTransFormatCallback传入的nPort值一致>
*	@param	<emDataType (IN)输入的视频流数据类型>
*	@param	<pDataBuf (IN)输入的视频流数据指针>
*	@param	<nBufSize (IN)输入的视频流数据长度>
*	@remark	<在GPAPI_SetTransFormatCallback后，调用该函数>
*	@return	成功返回TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_InputTransformData(PLAYERTYPE playerType, LONG nPort, VAG_STREAM_DATA_TYPE emDataType, char* pDataBuf, UINT64 nBufSize, void* pUserData);
//end of 通用类型接口，所有播放类型的码流数据保存时都需调用以上流程
/**************************************************************************/




//以下接口只针对VAG码流设备(后续将不再维护，请使用GPAPI通用接口)
/**************************************************************************
**************************************************************************/

/**************************************************************************
*   Function:		GPAPI_VAG_Capture
*   Description:    VAG抓图，单独针对VAG码流使用接口（GPAPI_CaptureByUserEx）
*   Input:          LONG nPort, 播放用端口或播放句柄
*                   LPVPLAY_SNAP_PARA pstSnapPara, 播放用端口或播放句柄
*                   pSnapShotFun，抓图回调函数
*                   pUserData，用户自定义数据
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_Capture(LONG nPort, LVAG_SNAP_PARA pstSnapPara, VAG_fSnapCallback pSnapShotFun, void* pUserData);

/**************************************************************************
*   Function:		GPAPI_VAG_SaveFile
*   Description:    VAG自动保存录像文件，该接口在通道开始播放后调用
*   Input:          LONG nPort, 播放用端口或播放句柄
*                   pFileName 保存录像文件路径
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_SaveFile( LONG nPort,const char* pFileName );

/**************************************************************************
*   Function:		GPAPI_VAG_CloseSaveFile
*   Description:    VAG停止自动保存录像文件
*   Input:          LONG nPort, 播放用端口或播放句柄
*   Output:         (null)
*   Return:			BOOL, TRUE:成功,FALSE:失败 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_CloseSaveFile( LONG nPort);

//以下接口只针对VAG码流格式保存设备原始码流的录象文件
/**************************************************************************
**************************************************************************/
/**	@fn	    GPAPI_VAG_AnalyzeMediaHead
*	@brief	<分析VAG码流头>
*	@param	<pMediaHead (IN)媒体头数据指针>
*	@param	<iMediaHeadLen (IN)媒体头数据长度>
*	@param	<pstMediaAnalyzeInfo (OUT)返回的分析结果,
*			char* pPriMediaHead 设备原始头放在文件最开始，stSysMediaHead 放在文件最末尾>
*	@return	成功返回 TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_AnalyzeMediaHead(const char* pMediaHead, int iMediaHeadLen, LPVAG_MEDIA_HEAD_ANAL_INFO pstMediaAnalyzeInfo);

/**	@fn	GPAPI_VAG_SetTransFormatCallback
*	@brief	<VAG注册码流封装转换回调函数>
*	@param	<nPort (IN) 播放用端口或播放句柄，该端口需另外创建>
*	@param	<pStreamDataFun (IN)封装转换后数据回调函数>
*	@param	<pUserData (IN)用户数据>
*	@return	成功返回TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_SetTransFormatCallback(LONG nPort, VAG_fGPAPIStreamDataCallBack pStreamDataFun, void* pUserData);

/**	@fn	GPAPI_VAG_InputTransformData
*	@brief	<输入VAG视频数据流>
*	@param	<nPort (IN) 播放用端口或播放句柄>
*	@param	<emDataType (IN)输入的视频流数据类型>
*	@param	<pDataBuf (IN)输入的视频流数据指针>
*	@param	<nBufSize (IN)输入的视频流数据长度>
*	@remark	<调用该函数塞入数据后，在GPAPI_VAG_SetTransFormatCallback设置的回调函数中回调出设备的原始码流>
*	@return	成功返回TRUE;错误时返回FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_InputTransformData(LONG nPort, VAG_STREAM_DATA_TYPE emDataType, char* pDataBuf, UINT64 nBufSize);

#endif // GUARD_GENERALPLAYERAPI_H