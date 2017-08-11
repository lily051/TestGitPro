// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBPPVCLIENT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// _API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef LIBPPVCLIENT_H__
#define LIBPPVCLIENT_H__

#include <time.h>

#ifdef LIBPPVCLIENT_EXPORTS
#define LIBPPVCLIENT_API __declspec(dllexport)
#else
#define LIBPPVCLIENT_API __declspec(dllimport)
#endif


//错误码定义
#define E_SUCCEED								( 0 )
#define E_FAILED								(-1)

#define E_CONNDEVICE_BASE						( -10800 )
#define E_CONNDEVICE_CANNOTCONNETSERVER			( E_CONNDEVICE_BASE - 1 )
#define E_CONNDEVICE_NETWORKJITTER				( E_CONNDEVICE_BASE - 2 )
#define E_CONNDEVICE_DEVNOTONLINE				( E_CONNDEVICE_BASE - 3 )
#define E_CONNDEVICE_TRANSFERFAILED				( E_CONNDEVICE_BASE - 4 )


#define E_SENDUPDATEINFO_BASE					( -11100 )
#define E_SENDUPDATEINFO_DEVICENOTONLINE		( E_SENDUPDATEINFO_BASE - 1 )
#define E_SENDUPDATEINFO_TRANSFERFAILED			( E_SENDUPDATEINFO_BASE - 2 )
#define E_SENDUPDATEINFO_CANNOTCONNETDEVICE		( E_SENDUPDATEINFO_BASE - 3 )
#define E_SENDUPDATEINFO_CANNOTCONNETSERVER		( E_SENDUPDATEINFO_BASE - 4 )
#define E_SENDUPDATEINFO_INVALIDDEVICEHANDLE	( E_SENDUPDATEINFO_BASE - 5 )
#define E_SENDUPDATEINFO_CMDNOTEXECUTED			( E_SENDUPDATEINFO_BASE - 6 )

// 媒体头文件长度(40字节)
#define HEAD_FILE_LENGTH						40

/* 日志类型定义(新) */
/* alarm log */
#define MAJOR_ALARM 0x1        /*主类型用红色标出*/
#define MINOR_ALARM_IN 0x1 /* alarm in */
#define MINOR_ALARM_OUT 0x2 /* alarm out */
#define MINOR_MOTDET_START 0x3 /* motion detect start */
#define MINOR_MOTDET_STOP 0x4 /* motion detect stop */
#define MINOR_HIDE_ALARM_START 0x5 /* hide alarm start */
#define MINOR_HIDE_ALARM_STOP 0x6 /* hide alarm stop */
/* exception */
#define MAJOR_EXCEPTION 0x2
#define MINOR_VI_LOST 0x21 /* VI lost */
#define MINOR_ILLEGAL_ACCESS 0x22 /* illegal access */
#define MINOR_HD_FULL 0x23 /* hard disk full */
#define MINOR_HD_ERROR 0x24 /* hard disk error */
#define MINOR_DCD_LOST 0x25 /* modem DCD lost */
#define MINOR_IP_CONFLICT 0x26 /* ip address conflict */
#define MINOR_NET_BROKEN 0x27 /* net broken */
/* operation */
#define MAJOR_OPERATION 0x3
#define MINOR_START_DVR 0x41 /* start device */
#define MINOR_STOP_DVR 0x42 /* stop device */
#define MINOR_STOP_ABNORMAL 0x43 /* stop device abnormally */
#define MINOR_LOCAL_LOGIN 0x50 /* local login */
#define MINOR_LOCAL_LOGOUT 0x51 /* local logout */
#define MINOR_LOCAL_CFG_PARM 0x52 /* local config parameter */
#define MINOR_LOCAL_PLAYBYFILE 0x53 /* local playback by file */
#define MINOR_LOCAL_PLAYBYTIME 0x54 /* local playback by time */
#define MINOR_LOCAL_START_REC 0x55 /* local start record */
#define MINOR_LOCAL_STOP_REC 0x56 /* local stop record */
#define MINOR_LOCAL_PTZCTRL 0x57 /* local PTZ control */
#define MINOR_LOCAL_PREVIEW 0x58 /* local preview */
#define MINOR_LOCAL_MODIFY_TIME 0x59 /* local modify date/time */
#define MINOR_LOCAL_UPGRADE 0x5a /* local upgrade software */
#define MINOR_REMOTE_LOGIN 0x70 /* remote login */
#define MINOR_REMOTE_LOGOUT 0x71 /* remote logout */
#define MINOR_REMOTE_START_REC 0x72 /* remote start record */
#define MINOR_REMOTE_STOP_REC 0x73 /* remote stop record */
#define MINOR_START_TRANS_CHAN 0x74 /* start transparent channel */
#define MINOR_STOP_TRANS_CHAN 0x75 /* stop transparent channel */
#define MINOR_REMOTE_GET_PARM 0x76 /* remote get parameter */
#define MINOR_REMOTE_CFG_PARM 0x77 /* remote config parameter */
#define MINOR_REMOTE_GET_STATUS 0x78 /* remote get status */
#define MINOR_REMOTE_ARM 0x79 /* remote arm */
#define MINOR_REMOTE_DISARM 0x7a /* remote disarm */
#define MINOR_REMOTE_REBOOT 0x7b /* remote reboot device */
#define MINOR_START_VT 0x7c /* start voice talk */
#define MINOR_STOP_VT 0x7d /* stop voice talk */
#define MINOR_REMOTE_UPGRADE 0x7e /* remote upgrade software */
#define MINOR_REMOTE_PLAYBYFILE 0x7f /* remote playback by file */
#define MINOR_REMOTE_PLAYBYTIME 0x80 /* remote playback by time */
#define MINOR_REMOTE_PTZCTRL 0x81 /* remote PTZ control */



//客户端与设备连接类型
typedef enum
{
	CONN_MODE_P2P = 0,		//点对点
	CONN_MODE_TRANSFER,		//通过服务器中转
}EN_CONN_MODE;

//视频数据回调时数据类型标识
typedef enum
{
	STREAM_TYPE_HDR = 0,		//流头
	STREAM_TYPE_DATA,			//数据
}EN_STREAM_DATATYPE;

//云台控制命令
typedef enum
{
	PTZ_CMD_ZOOM_IN = 11, 		/* 焦距以变大(倍率变大) */
	PTZ_CMD_ZOOM_OUT, 			/* 焦距变小(倍率变小) */
	PTZ_CMD_FOCUS_IN, 			/* 焦点前调 */
	PTZ_CMD_FOCUS_OUT, 			/* 焦点后调 */
	PTZ_CMD_IRIS_ENLARGE,	 	/* 光圈扩大 */
	PTZ_CMD_IRIS_SHRINK, 		/* 光圈缩小 */
	PTZ_CMD_TILT_UP = 21, 		/* 云台向上 */
	PTZ_CMD_TILT_DOWN,			/* 云台向下 */
	PTZ_CMD_PAN_LEFT, 			/* 云台左转 */
	PTZ_CMD_PAN_RIGHT, 			/* 云台右转 */
	PTZ_CMD_UP_LEFT,			/* 云台左上 */
	PTZ_CMD_UP_RIGHT,			/* 云台右上 */
	PTZ_CMD_DOWN_LEFT,			/* 云台左下 */
	PTZ_CMD_DOWN_RIGHT,			/* 云台右下 */
	PTZ_CMD_SET_PRESET = 31,	/* 设置预置点 */
	PTZ_CMD_CLE_PRESET,			/* 清除预置点 */
	PTZ_CMD_GOTO_PRESET,		/* 转到预置点 */
	PTZ_CMD_LIGHT_PWRON = 41,	/* 灯光开关 */
	PTZ_CMD_WIPER_PWRON,		/* 雨刷开关 */
	PTZ_CMD_PAN_AUTO,           /* 自动巡航 */
}EN_PTZ_COMMAND;

//vod控制命令
typedef enum
{
	VOD_CONTROL_CMD_GETPOS = 3,		/*获取回放进度*/
	VOD_CONTROL_CMD_SETPOS,			/*设置回放进度*/
}EN_VOD_CONTROL_CMD;

// (文件)偏移类型
typedef enum
{
	SEEK_BY_SIZE	=	1,			/*偏移文件长度 BYSIZE*/
	SEEK_BY_TIME	=	2,			/*偏移时间坐标 BYTIME*/
}SEEK_TYPE;

//注册服务器登录信息
typedef struct  
{
	char				szAcessServerIP[32];
	unsigned short int	nAcessServerPort;
	unsigned short int	nReserved;
	char				szUserName[32];
	char				szUserPwd[32];
}ST_ACCESS_SERVER_INFO;

//流媒体服务器信息
typedef struct  
{
	char				szStreamServerIP[32];      /*流媒体服务器IP*/
	unsigned short int  nStreamServerPort;         /*流媒体服务器端口*/         
	unsigned short int  nReserved;                 /*保留*/
}ST_STREAM_SERVER_INFO;

//解码器取流信息
typedef struct
{
	char				    pszDeviceID[32];        /*取流设备的FrontId*/ 
	int					    iChan;					/*设备视频通道号*/
	int						iStreamType;			/*码流类型*/
	ST_STREAM_SERVER_INFO   stStreamServerInfo;     /*编码器流媒体服务器信息*/
	ST_ACCESS_SERVER_INFO   stAccessServerInfo;     /*编码器注册服务器信息*/
}ST_DECODE_STREAM_INFO;

//抓图信息
typedef struct
{
	int						iSaveTo;				/*图片保存位置,1:上传中心 2:保存本地 3:上传中心&本地存储*/
	int						iResolution;			/*图片分辨率*/
	int						iQuality;				/*图片质量,40:Comman;60:better;80:Best*/
	char					szTime[32];				/*图片时间*/
	int						iSequence;				/*图片序号*/
	int						iPreset;				/*图片抓拍的预置点*/
	unsigned short int      nReserved;              /*保留*/
}ST_CAPTUREPICTURE_INFO;

//------------省略,以消除和HCNetSDK.h中的重定义 刘大煜-----------------
//typedef enum
//{
//	VGA_NOT_AVALIABLE,
//	VGA_THS8200_MODE_SVGA_60HZ,         //（800*600）
//	VGA_THS8200_MODE_SVGA_75HZ,         //（800*600）
//	VGA_THS8200_MODE_XGA_60HZ,          //（1024*768）
//	VGA_THS8200_MODE_XGA_70HZ,          //（1024*768）
//	VGA_THS8200_MODE_SXGA_60HZ,         //（1280*1024）
//	VGA_THS8200_MODE_720P_60HZ,         //（1280*720 ）
//	VGA_THS8200_MODE_1080i_60HZ,        //（1920*1080）
//	VGA_THS8200_MODE_1080P_30HZ,        //（1920*1080）
//	VGA_THS8200_MODE_1080P_25HZ,        //（1920*1080）
//	VGA_THS8200_MODE_UXGA_30HZ          //（1600*1200）
//}VGA_MODE;                              //（所支持的分辨率从能力集中获取）
//
////解码器显示通道信息
//typedef struct  
//{                              
//	int			          iMode;					 /*画面分割模式1/4/9/16*/			 
//	char                  szStandard[32];            /*输出制式PAL/NTSC*/
//	VGA_MODE              iResolution;				 /*分辨率：0-VGA_NOT_AVALIABLE；0-10依次对应枚举定义*/
//	int					  iSubWndDecChan[32];		 /*子画面窗口与解码通道,0号单元代表第一个子画面对应的解码通道号*/
//}ST_DISPLAY_CHANNEL_INFO;

//轮询设置信息
typedef struct 
{
	int						iIndex;					 /*轮询点序号，取值范围 1～64*/
	int						iPollTime;				 /*停留时间*/
	int 				    iEnable;				 /*1-轮询点有效，0-无效*/
	ST_DECODE_STREAM_INFO   stDecodeStreamInfo;		 /*解码器取流信息*/
}ST_POLL_INFO;

//解码器能力集信息
typedef struct  
{
	int			  iDecChans;                 /*解码通道个数*/
	int			  iDSPs;				     /*DSP数量*/
	int			  iBNCNums;				     /*BNC输出个数*/
	int			  iVGANums;			         /*VGA输出个数*/
	char		  szBNCScreenSplits[32];     /*BNC输出支持的画面分割模式*/
	char		  bySupportResolution[32];   /*支持的VGA分辨率刷新频率，按照上面的枚举定义,一个字节代表一个分辨率是否支持，1：支持，0：不支持*/
	char          byScreenSurport[8][32];    /*VGA输出支持的画面分割模式*/  
}ST_DECODER_ABILITY;

//图像参数结构
typedef struct  
{
	unsigned char byBrightness;		/*亮度,0-255*/
	unsigned char byContrast;		/*对比度,0-255*/ 
	unsigned char bySaturation;		/*饱和度,0-255 */ 
	unsigned char byHue;			/*色调,0-255*/
}ST_IMAGE_PARAM;


//日期时间结构
typedef struct  
{
	int		iYear;
	int		iMonth;
	int		iDay;
	char	chHours;
	char	chMinutes;
	char	chSeconds;
	char	chReserved;
}ST_DATETIME;


//日志信息
typedef struct 
{
	ST_DATETIME stLogTime;
	int			iMajorType;		//主类型 1-报警; 2-异常; 3-操作; 0xffff-全部
	int			iMinorType;		//次类型 0xffff-全部;
	char		szPanelUser[64];//面板操作用户名
	char		szNetUser[64];	//网络用户名
	char		szRemoteHostAddr[64];//远程主机IP 
	int			iParaType;		//参数类型
	int			iChannel;		//通道号
	int			iDiskNumber;	//硬盘号
	int			iAlarmInPort;	//报警输入端口
	int			iAlarmOutPort;	//报警输出端口
	int			iInfoLen;
	char		szInfo[512];	//日志信息
}ST_DVR_LOG, *LPST_DVR_LOG;

//注册服务器上的设备信息
typedef struct  
{
	char				szDeviceIP[32];				//设备外网IP
	unsigned short int	nDevicePort;				//设备外网通信端口
	unsigned short int	nDeviceOnlie;				//设备在线状态: 0--不在线;1--在线
	char				szDeviceLocalIP[32];		//设备内网IP
	unsigned short int	nDeviceLocalPort;			//设备内网通信端口
	unsigned short int	nNetType;					//设备所处网络类型:0--内网;1--外网
	char				szFirmWareVersion[128];		//设备固件版本号
	char				szProtocalVersion[128];		//设备协议版本号
	time_t				tOnlineTime;				//已在线时间
}ST_DEVICEINFO_ONSERVER;

//注册服务器状态
typedef struct  
{
	char				szServerIP[32];				//服务器IP
	unsigned short int	nServerPort;				//服务器通信端口
	unsigned short int	nOnlineDeviceNum;			//在线设备数目
	char				szServerVersion[128];		//服务器版本号
	time_t				tRunningTime;				//服务器运行时间
	unsigned int		nCpuUsage;					//服务器CPU使用率
	unsigned int		nTotalPhys;					//服务器总的物理内存
	unsigned int		nAvailPhys;					//服务器可用物理内存
	unsigned short int	nLicence;					//服务器是否使用加密狗许可: 0--未使用;1--使用
	unsigned short int	nLicenceType;				//服务器使用加密狗许可方式: 0--设备限制;1--通道限制
	unsigned int		nLicenceLimit;				//服务器限制注册数目(设备或者通道)
}ST_SERVERSTATUS;

//历史文件查找信息
typedef struct 
{
	char		sFileName[100];	//文件名
	ST_DATETIME struStartTime;	//文件的开始时间
	ST_DATETIME struStopTime;	//文件的结束时间
	int			iFileSize;		//文件的大小
	int         iFileType;      //文件类型  
}ST_FINDFILE;

//图片文件查找信息
typedef struct 
{
    char		sFileName[100];	//文件名
    ST_DATETIME struStartTime;	//文件的开始时间
    int			iFileSize;		//文件的大小
    int         iFileType;      //文件类型  
}ST_FINDPICFILE;

//设备升级时的ftp信息
typedef struct 
{
	char				szFTPIP[32];				//ftp服务器ip
	unsigned short int	nFTPPort;					//ftp服务器端口
	unsigned short int	nReserved;					//保留
	char				szUserName[32];				//ftp登录用户名
	char				szPasswd[32];				//ftp登录密码
	char				szUpdateFile[128];			//升级文件名
}ST_UPDATEFTP_INFO;

//live stream信息
typedef struct  
{
	unsigned int iStreamHdrLen;			//流头长度
	unsigned char chStreamHdr[256];		//流头数据
}ST_LIVESTREAM_SDPINFO;


//会话信息
typedef struct  
{
	unsigned int nRecvedRTPCount;	//接收到的总rtp包数目
	int			 iLostRTPCount;		//丢失的rtp包数目
}ST_PPV_SESSION_INFO;

#ifdef __cplusplus
extern "C" {
#endif


	/*************************************************
	Function:		pNotifyCallBack
	Description:	通知回调函数类型声明
	Input:			iNotifySubsribeID:		订阅通知信息的返回句柄
					pNotifyBuffer:			通知信息数据
					dwNotifySize:			通知信息长度
					nUserDataForCallBack:	用户数据
					nReserved:				保留
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pNotifyCallBack)( int iNotifySubsribeID, const void* pNotifyBuffer, size_t dwNotifySize, unsigned int nUserDataForCallBack, unsigned int nReserved );

	/*************************************************
	Function:		pAVDataCallBack
	Description:	流数据回调函数类型声明
	Input:			
	iStreamID:	区分不同音视频流的ID
	iDataType:	流数据类型:流头数据还是普通数据
	pAVData:	指向流数据缓冲
	nAVDataLen:	流数据字节数
	nUsrData:	用户数据
	pNALU:		NALU数据
	nNALU:		NALU数据长度
	pRawRTP:	包括rtp头部的原始rtp数据,可以为NULL
	nRawRTP:	原始rtp数据的长度,可以为0
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pAVDataCallBack)( int iStreamID,  int iDataType, const void* pAVData, 
		size_t nAVDataLen, unsigned int nUsrData, const void* pNALU, size_t nNALU, const void* pRawRTP, size_t nRawRTP );

	/*************************************************
	Function:		pFileDownloadDataCallBack
	Description:	流数据回调函数类型声明
	Input:			
	iDownloadID:	区分不同音视频流的ID
	pFileData:		用户数据
	nFileDataLen:	流数据类型:流头数据还是普通数据
	nUsrData:		用户数据
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pFileDownloadDataCallBack)( int iDownloadID,  const void* pFileData, size_t nFileDataLen, unsigned int nUsrData );

	/*************************************************
	Function:		pProgressNotifyCallBack
	Description:	调用某些函数时的处理过程回调
	Input:			nUserData:	用户数据
					nReserved:	保留
					pDesc:		当前步骤描述
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pProgressNotifyCallBack)( unsigned int nUserData, unsigned int nReserved, const char* pDesc );

   	/*************************************************
	Function:		pVoiceDataCallBack
    Description:	语音数据回调函数类型声明
    Input:			
    iVoiceID:	    区分不同语音流的ID
    pVoiceData:		指向语音数据缓存
    nVoiceDataLen:	语音数据字节数
    nUsrData:		用户数据
    Output:
    return:
	*************************************************/
    typedef void (__stdcall *pVoiceDataCallBack)( int iVoiceID,  const void* pVoiceData, size_t nVoiceDataLen, int nFrameNumber, unsigned int nUsrData);


	/*************************************************
	Function:		PPVInitLib
	Description:	初始化库
	Input:			
	Output:
	return:			0: 成功;
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVInitLib( );

	/*************************************************
	Function:		PPVInitLibEx
	Description:	初始化库
	Input:			
	nStartPort1:  UDP模式下监听设备起始端口   
	nEndPort1:	  UDP模式下监听设备结束端口
	nStartPort2:  TCP模式下监听设备开始端口
	nEndPort2:	  TCP模式下监听设备结束端口	   
	Output:
	return:			0: 成功;
	-1:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVInitLibEx( unsigned short int nStartPort1, unsigned short int nEndPort1,unsigned short int nStartPort2, unsigned short int nEndPort2 );

	/*************************************************
	Function:		PPVFiniLib
	Description:	销毁库
	Input:			
	Output:
	return:			0: 成功;
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVFiniLib( void );

	
	/*************************************************
	Function:		pszLocalIP
	Description:	设置本地要绑定的IP
	Input:			pszIP:	要设置的IP
	Output:			NULL
	return:			>=0: 成功,代表设置的IP标识
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetBindedIP( const char* pszIP );
	

	/*************************************************
	Function:		PPVUnSetBindedIP
	Description:	取消本地要绑定的IP
	Input:			iBindID:PPVSetBindedIP返回值
	Output:			NULL
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVUnSetBindedIP( int iBindID );

	/*************************************************
	Function:		PPVConnectDevice
	Description:	连接设备
	Input:			pszDeviceID:	要连接的设备ID
					stPassbyServer:	设备注册的服务器
					nUserData:		用户数据,回调时传给回调函数
	Output:			pConnMode:		指向存放连接类型的地址,函数返回后该地址存放的是连接类型
	return:			>=0: 作为标识被连接设备的ID
					<0:  连接失败,不同的值代表不同失败原因
	*************************************************/
	LIBPPVCLIENT_API int	PPVConnectDevice( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nUserData, EN_CONN_MODE* pConnMode );

	/*************************************************
	Function:		PPVConnectDeviceByACS
	Description:	通过服务器中转方式连接设备,不是点对点
	Input:
	pszDeviceID:	要连接的设备ID
	stPassbyServer:	设备注册的服务器
	nUserData:		用户数据,回调时传给回调函数
	Output:
	pConnMode:		指向存放连接类型的地址,函数返回后该地址存放的是连接类型
	return:
	>=0: 作为标识被连接设备的ID
	<0:连接失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVConnectDeviceByACS( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nUserDataForCallBack, EN_CONN_MODE* pConnMode );


	/*************************************************
	Function:		PPVDisConnectDevice
	Description:	断开与设备的连接
	Input:			iDevice: PPVConnectDevice的返回值
	Output:			无
	return:			无
	*************************************************/
	LIBPPVCLIENT_API void	PPVDisConnectDevice( int iDevice );


	/*************************************************
	Function:		PPVGetDeviceInfoOnServer
	Description:	从服务器上获取设备状态
	Input:			pszDeviceID:	设备序列号
					stPassbyServer:	设备注册的服务器					
	Output:			pSTDeviceStatus:设备信息
	return:			0: 获取成功;
					<0:获取失败;
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceInfoOnServer( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, ST_DEVICEINFO_ONSERVER* pSTDeviceStatus );


	/*************************************************
	Function:		PPVRebootDevice
	Description:	发送重启设备命令
	Input:			iDevice: PPVConnectDevice的返回值
	Output:			无
	return:			0: 发送重启命令成功;
					<0:发送重启命令失败;
	*************************************************/
	LIBPPVCLIENT_API int	PPVRebootDevice( int iDevice );


	/*************************************************
	Function:		PPVGetDeviceWorkStatus
	Description:	获取设备状态
	Input:			iDevice:			设备标识					
	Output:			pszDeviceStatusXML:	设备状态的XML文本	
					nBufferSize:		指示获取前缓冲区的大小，以及获取后XML文本的长度		
	return:			0: 获取成功;
					<0:获取失败;
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceWorkStatus( int iDevice, char* pszDeviceStatusXML, size_t* nBufferSize );


	/*************************************************
	Function:		PPVGetServerStatus
	Description:	获取服务器状态
	Input:			stPassbyServer:	服务器连接信息 
	Output:			pSTServerStatus:服务器状态
	return:			0:获取成功;
					<0:获取失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetServerInfo( ST_ACCESS_SERVER_INFO stPassbyServer, ST_SERVERSTATUS* pSTServerStatus );


	/*************************************************
	Function:		PPVRealPlayStart
	Description:	开始视频浏览
	Input:			iDevice:	设备标识
					iChan:		通道号,从1开始
					iChanType:  通道类型:0-主通道; 1-子通道
					pfnCallBack: 视频数据回调
					nUsrData:	用户数据
	Output:
	return:			>=0: 流标识
					<0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int	PPVRealPlayStart(  int iDevice, int iChan, int iChanType, 
		pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData, 
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVRealPlayStop
	Description:	停止视频浏览
	Input:			iStreamID: PPVRealPlayStart的返回值
	Output:
	return:
	*************************************************/
	LIBPPVCLIENT_API void	PPVRealPlayStop( int iStreamID );


	/*************************************************
	Function:		PPVGetImageParam
	Description:	获取图像参数
	Input:			iDevice:		设备标识
					iChan:			通道号					
	Output:			pstImageParam:	图像参数结构体指针
	return:			0:	成功
					<0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetImageParam( int iDevice, int iChan, ST_IMAGE_PARAM* pstImageParam );


	/*************************************************
	Function:		PPVSetImageParam
	Description:	设置图像参数
	Input:			iDevice:		设备标识
					iChan:			通道号
					stImageParam:	图像参数结构体
	Output:
	return:			0:	成功
					<0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetImageParam( int iDevice, int iChan, ST_IMAGE_PARAM stImageParam );


	/*************************************************
	Function:		PPVPTZControl
	Description:	云台控制
	Input:			iDevice:		设备标识
	iChan:			通道号,			从1开始
	iPTZCommand:	云台控制命令,	参见 EN_PTZ_COMMAND
	iAction:		动作:			0-开始; 1-停止
	iSpeed:			云台控制的速度，用户按不同解码器的速度控制值设置.取值为1～7
	Output:
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVPTZControl( int iDevice, int iChan, int iPTZCommand, int iAction, int iSpeed );


	/*************************************************
	Function:		PPVPTZPresetControl
	Description:	设置、清除、转到预置点
	Input:			iDevice:			设备标识
					iChan:				通道号,从1开始
					iPTZPresetCommand:	参见 EN_PTZ_COMMAND
					iParam:				预置点号,最多255个,从1开始
	Output:			无
	return:			0:	成功
					<0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVPTZPresetControl( int iDevice, int iChan, int iPTZPresetCommand, int iParam );


	/*************************************************
	Function:		PPVVodStartByTime
	Description:	开始按时间
	Input:			iDevice:	设备标识
					iChan:		通道号,从1开始
					stStart:	开始日期时间
					stStop:     结束日期时间
					pfnAVDataCallBack:	视频数据回调
					nUsrData:		用户数据
	Output:
	return:			>=0: 流标识
					<0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int	PPVVodStartByTime(  int iDevice, int iChan, ST_DATETIME stStart, ST_DATETIME stStop, pAVDataCallBack pfnAVDataCallBack, unsigned int nUsrData );


	/*************************************************
	Function:		PPVFindFile
	Description:	查找服务器文件
	Input:			iDevice:		设备标识
	iChan:			通道号,从1开始
					dwFileType		文件类型 
									0xff-全部   0-定时录像 
									1-移动报警	2-报警触发 
									3-报警|动测 4-报警&动测
									5-命令触发 	6-手动录像
					stStart:		开始日期时间
					stStop:			结束日期时间
					pArray:			文件列表结构体指针 
					iFileNum:		最多查找文件个数 
	Output:			pArray:			文件结构体数组首地址
					iFileNum:		实际查找到文件个数
	return:			0:  成功
					<0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVFindFile( int iDevice, int iChan, int iFileType, ST_DATETIME stStart, ST_DATETIME stStop, ST_FINDFILE* pArray, int *iFileNum);


	/*************************************************
	Function:		PPVStartDownloadThroughACS
	Description:	开始通过服务器下载设备上的文件
	Input:			
	pszDeviceID:	设备标识
	iChan:			通道号,从1开始
	pszFileName:	设备上的文件名
	nFileLen:		文件长度
	stPassbyServer:	设备注册的服务器
	pDownCallBack:	下载时数据回调
	nDownCallBackUsrData:下载回调函数的用户数据
	return:			
		>=0:  成功,返回值为下载数据流标识
		<0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVStartDownloadThroughACS( const char* pszDeviceID, int iChan, const char* pszFileName, size_t nFileLen, SEEK_TYPE enumSeekType, unsigned int uSeek, ST_ACCESS_SERVER_INFO stPassbyServer, pFileDownloadDataCallBack pDownCallBack, unsigned int nDownCallBackUsrData );


	/*************************************************
	Function:		PPVStopDownloadThroughACS
	Description:	停止通过服务器下载设备上的文件
	Input:			
	iDownloadID:	下载数据流标识
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVStopDownloadThroughACS(  int iDownloadID );

	/*************************************************
	Function:		PPVGetDownloadPosThroughACS
	Description:	获取下载进度
	Input:		
		iDownloadID:下载数据流标识
	return:			
		返回指定下载标识的下载进度
	*************************************************/
	LIBPPVCLIENT_API long PPVGetDownloadPosThroughACS(  int iDownloadID );

	/*************************************************
	Function:		PPVPlayBackByFileNameStart
	Description:	开始历史视频浏览(TCP模式),按照文件名称
	Input:			
		iDevice:				设备标识
		iChannel:				频道编号
		pszFileName:			文件名称
		nFileLen:				文件长度
		stPassbyServer,			设备注册的服务器PPVSServer
		pfnAVDataCallBack:		视频数据回调
		nAVDataCBUsrData:		用户数据
		pfnProgressNotifyUser：	通知回调函数
		nProgressCBUsrData：	通知用户数据
	Output:
	return:			
					>=0:	流标识
					<0:		失败 
	*************************************************/
	LIBPPVCLIENT_API int	PPVPlayBackByFileNameStart(  int iDevice, int iChannel, const char *pszFileName, size_t nFileLen, ST_ACCESS_SERVER_INFO stPassbyServer, pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData,
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVPlayBackStop
	Description:	停止历史视频浏览(TCP模式)
	Input:			
		iVodStream: PPVPlayBackStartByFileName的返回值
	Output:
	return:
	*************************************************/
	LIBPPVCLIENT_API void	PPVPlayBackStop( int iVodStream );


	/*************************************************
	Function:		 PPVPlayBackControl
	Description:	 历史视频回放进度控制(TCP模式)
	Input:			
		iVodStream:  PPVVodStart的返回值
		iControlCmd: 回放控制命令, 参见EN_VOD_CONTROL_CMD
		iParamIn:	 控制命令相关的参数	
	Output:			
		iParamOut:	 控制返回结果
	return:			
					 0:	成功
					 <0: 失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVPlayBackControl( int iVodStream, int iControlCmd, int iParamIn, int* iParamOut );


	/*************************************************
	Function:		PPVFindDeviceLog
	Description:	查找服务器文件
	Input:			iDevice:		设备标识
					iSelectMode:	保留参数，可传入0
					iMajorType:		主类型,参见宏定义,0xffff代表所有类型
					iMinorType:		子类型,参见宏定义,0xffff代表所有类型
					stStart:		开始日期时间
					stStop:			结束日期时间
					pArray:			日志列表结构体指针 
					iFileNum:		最多查找日志个数 
	Output:			pArray:			日志结构体数组首地址
					iFileNum:		实际查找到日志个数
	return:			0:  成功
					<0: 失败
	*************************************************/
LIBPPVCLIENT_API int	PPVFindDeviceLog( int iDevice, int iSelectMode, int iMajorType, int iMinorType, ST_DATETIME stStart, ST_DATETIME stStop, ST_DVR_LOG* pArray, int *iItemNum );
	

	/*************************************************
	Function:		PPVSendUpdateInfo
	Description:	发送升级信息给设备
	Input:			iDevice:		设备标识
					stUpdateInfo:	升级信息
	Output:			
	return:			0: 设备收到了升级命令;
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVSendUpdateInfo( int iDevice, ST_UPDATEFTP_INFO stUpdateInfo );


	/*************************************************
	Function:		PPVGetDeviceConfig
	Description:	获取设备配置信息
	Input:			iDevice:		设备标识
					pszConfigCMD:	命令字符串
					iCmdParam1:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam2:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam3:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam4:	命令参数，可以是通道号，异常类型，报警输入通道号
	Output:			pszConfigXML:	包含配置信息的xml文本
					nBufferSize:	指示获取前缓冲区的大小，以及获取后xml文本的长度	
	return:			0: 成功
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceConfig( int iDevice, const char* pszConfigCmd, int iCmdParam1, 
		int iCmdParam2, int iCmdParam3,  int iCmdParam4,  char* pszConfigXML, size_t* nBufferSize );


	/*************************************************
	Function:		PPVSetDeviceConfig
	Description:	设置设备配置信息
	Input:			iDevice:		设备标识
					pszConfigCMD:	命令字符串
					iCmdParam1:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam2:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam3:	命令参数，可以是通道号，异常类型，报警输入通道号
					iCmdParam4:	命令参数，可以是通道号，异常类型，报警输入通道号
					pszConfigXML:	包含配置信息的xml文本
	Output:
	return:			0: 成功
					<0:失败
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetDeviceConfig( int iDevice, const char* pszConfigCmd, int iCmdParam1, 
		int iCmdParam2, int iCmdParam3,  int iCmdParam4, const char* pszConfigXML );


	/*************************************************
	Function:					PPVRealPlayByTcpStart
	Description:				通过TCP模式开始进行实时视频数据接收
	Input:			
		iDevice:				设备标识
		iChan:					通道号,从1开始
		iChanType:				通道类型:0-主通道; 1-子通道
		pfnCallBack:			视频数据回调
		nUsrData:				用户数据
		pfnProgressNotifyUser:	用户通知回调
		nProgressCBUsrData:		用户通知数据
	Output:
	return:			
		>=0:		流标识
		<0:			失败 
	*************************************************/
	LIBPPVCLIENT_API int	PPVRealPlayByTcpStart(  int iDevice, int iChan, int iChanType, 
		pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData, 
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVRealPlayByTcpStop
	Description:	通过TCP模式停止进行实时视频数据接收
	Input:			
		iStreamID:	流标识
	Output:
	return:			
		//>=0:		流标识
		//<0:			停止失败 
	*************************************************/
	LIBPPVCLIENT_API void PPVRealPlayByTcpStop( int iStreamID );

	/*************************************************
	Function:		PPVGetLiveStreamSDP
	Description:	获取流信息,主要是为了获取流头
	Input:			
					pszDeviceID:		设备序列号
					iChan:				设备通道号
					iStreamType:		0:主码流,1:子码流
					stPassbyServer:		设备注册的服务器	
	Output:			pSTStreamSDPInfo:	设备流头信息
	return:			
					0: 获取成功;
					<0:获取失败; 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetLiveStreamSDP(  int iDevice, int iChan, int iStreamType,  ST_LIVESTREAM_SDPINFO* pSTStreamSDPInfo );
	
	/*************************************************
	Function:			PPVSetAlarmGuardsStatus
	Description:		一键布/撤防
	Input:			
		iChangeCount:	改变的通道数量
		pChannel;		通道编号指针数组
	Output:
	return:			
		>0:				成功
		<=0:			失败
	*************************************************/
	LIBPPVCLIENT_API int PPVSetAlarmGuardsStatus(  int iDevice, const char* pszAlarmGuardsInfo );

	/*************************************************
	Function:		PPVGetSessionInfo
	Description:	获取会话信息
	Input:			
	iStream:		会话ID
	pSessionInfo:	会话信息结构体指针
	Output:
	return:		
	0:成功 
	-1:失败
	*************************************************/
	LIBPPVCLIENT_API int PPVGetSessionInfo( int iStream, ST_PPV_SESSION_INFO* pSessionInfo);

    /*************************************************
    Function:		PPVVoiceTalkStart
    Description:	开始语音对讲或广播
    Input:
                    pszDeviceID:	        设备ID
                    nVoiceChan:             语音通道
                    vtype:                  0-对讲; 1-广播
                    stPassbyServer:	        设备注册的服务器
                    pfnVoiceDataCallBack:   视频数据回调
                    nVoiceDataCBUsrData:	用户数据
    Output:
    return:			>=0: 语音标识
                    <0: 失败  -1--其他错误; -2--设备未准备好; -2--设备语音通道正在使用
    *************************************************/
    LIBPPVCLIENT_API int PPVVoiceTalkStart( const char* pszDeviceID, int nVoiceChan, int vtype, ST_ACCESS_SERVER_INFO stPassbyServer,
        pVoiceDataCallBack pfnVoiceDataCallBack, unsigned int nVoiceDataCBUsrData);

    /*************************************************
    Function:		PPVVoiceTalkStop
    Description:	停止语音对讲
    Input:			iVoiceID: PPVVoiceTalkStart的返回值
    Output:
    return:
    *************************************************/
    LIBPPVCLIENT_API void PPVVoiceTalkStop( int iVoiceID );

    /*************************************************
    Function:		PPVVoiceTalkInputData
    Description:	发送语音数据
    Input:			
                    iVoiceID: PPVVoiceTalkStart的返回值
    Output:
    return:
    *************************************************/
    LIBPPVCLIENT_API void PPVVoiceTalkInputData( int iVoiceID,  const void* pVoiceData, size_t nVoiceDataLen, int nFrameNumber);

    /*************************************************
    Function:		PPVTransparentChannel
    Description:	透明通道
    Input:
                    pszDeviceID:	        设备ID
                    nTransChan:             通道:1-232; 2-485
                    nRS485Chan:             485通道号，从1开始
                    stPassbyServer:	        设备注册的服务器
                    pData:                  指向数据缓冲
                    nDataLen:               数据长度
    Output:
    return:			>=0: 成功
                    <0: 失败
    *************************************************/
    LIBPPVCLIENT_API int PPVTransparentChannel( const char* pszDeviceID, int nTransChan, int nRS485Chan, ST_ACCESS_SERVER_INFO stPassbyServer,
        const void* pData, size_t nDataLen);

    /*************************************************
    Function:		PPVFindPictureFile
    Description:	查找服务器图片文件
    Input:			iDevice:		设备标识
                    iChan:			通道号,从1开始
                    dwFileType		文件类型,暂时不用
                                    0xff-全部   0-定时录像 
                                    1-移动报警	2-报警触发 
                                    3-报警|动测 4-报警&动测
                                    5-命令触发 	6-手动录像
                    stStart:		开始日期时间
                    stStop:			结束日期时间
                    pArray:			文件列表结构体指针 
                    iFileNum:		最多查找文件个数 
    Output:			pArray:			文件结构体数组首地址
                    iFileNum:		实际查找到文件个数
    return:			0:  成功
                    <0: 失败
    *************************************************/
    LIBPPVCLIENT_API int PPVFindPictureFile( int iDevice, int iChan, int iFileType, ST_DATETIME stStart, ST_DATETIME stStop, ST_FINDPICFILE* pArray, int *iFileNum);

    /*************************************************
    Function:		PPVNotifyDeviceOnOffLine
    Description:	通知设备上下线
    Input:
                    pszDeviceID:	        设备ID
                    action:                 1:上线, 0:下线
                    stPassbyServer:	        设备注册的服务器
    Output:
    return:			>=0: 成功
                    <0: 失败
    *************************************************/
    LIBPPVCLIENT_API int PPVNotifyDeviceOnOffLine( const char* pszDeviceID, int action, ST_ACCESS_SERVER_INFO stPassbyServer);
	
	/*************************************************
	Function:		PPVStartDecode
	Description:	开始动态解码
	Input:			iDecDevice:		   解码器设备标识
	                iDecChannel:       解码通道号
	Output:         
	return:			>=0: 成功
	                 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVStartDecode(  int iDecDevice, int iDecChannel, ST_DECODE_STREAM_INFO stDecodeStreamInfo);
	
	/*************************************************
	Function:		PPVStopDecode
	Description:	开始动态解码
	Input:			iDecDevice:		   解码器设备标识
					iDecChannel:       解码通道号
	Output:         
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVStopDecode( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVStartPoll
	Description:	开始轮询解码
	Input:			iDecDevice:		   解码器设备标识
	                iDecChannel:       解码通道号
	Output:         
	return:			>=0: 成功
	                 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVStartPoll( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVStopPoll
	Description:	停止轮询解码
	Input:			iDecDevice:		   解码器设备标识
					iDecChannel:       解码通道号
	Output:         
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVStopPoll( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVSetDecoderDisplayChannel
	Description:	设置解码器显示通道
	Input:			iDecDevice:		     解码器设备标识
					iDisplaycChannel:    显示通道号1-8VGA输出;9开始BNC输出
					stDisplayChannelInfo 解码器显示通道信息
	Output:         
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	//------------省略,以消除和HCNetSDK.h中的重定义 刘大煜-----------------
	//LIBPPVCLIENT_API int PPVSetDecoderDisplayChannel( int  iDecDevice, int iDisplaycChannel,  ST_DISPLAY_CHANNEL_INFO stDisplayChannelInfo);

	/*************************************************
	Function:		PPVGetDecoderDisplayChannel
	Description:	获取解码器显示通道
	Input:			iDecDevice:		       解码器设备标识
					iDisplaycChannel:      显示通道号
					stDisplayChannelInfo   解码器显示通道信息

	Output:			stDecoderChannelInfo   解码器显示通道信息
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	//------------省略,以消除和HCNetSDK.h中的重定义 刘大煜-----------------
	//LIBPPVCLIENT_API int PPVGetDecoderDisplayChannel(  int iDecDevice, int iDisplaycChannel, ST_DISPLAY_CHANNEL_INFO * stDisplayChannelInfo);

	/*************************************************
	Function:		PPVSetDecoderPoll
	Description:	设定轮询解码参数
	Input:			iDecDevice:		       解码器设备标识
					iDecChannel:           解码通道号
				    stPollInfo;            轮询点参数
					iNumberOfstPollInfo;   设定轮询点个数
	Output:
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVSetDecoderPoll(  int iDecDevice, int iDecChannel, const ST_POLL_INFO *stPollInfo, int iNumberOfstPollInfo);
	/*************************************************
	Function:		PPVGetDecoderPol
	Description:	获取轮询解码参数
	Input:			iDecDevice:		       解码器设备标识
					iDecChannel:           解码通道号
					iNumberOfstPollInfp    目前输入解码器最大信息个数64

	Output:		    stPollInfo;            轮询点参数
					iNumberOfstPollInfo;   实际获取轮询点信息个数
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecoderPoll(  int iDecDevice, int iDecChannel, ST_POLL_INFO *stPollInfo, int* iNumberOfstPollInfo );

	/*************************************************
	Function:		PPVGetDecChanStatus 
	Description:	获取解码通道状态
	Input:			iDecDevice:		       解码器设备标识
					iDecChannel:           解码通道号
  
	Output:		    iType;                 解码通道状态,iType="0/1/2",0:未解码；1:动态解码；2:轮询解码
					stDecodeStreamInfo     解码器取流信息
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecChanStatus(  int iDecDevice, int iDecChannel, int* iType, ST_DECODE_STREAM_INFO* stDecodeStreamInfo);

	/*************************************************
	Function:		PPVGetDecAbility
	Description:	获取解码器能力集
	Input:			iDecDevice:		   解码器设备标识

	Output:		    ST_DECODER_ABILITY;解码通道状态
	return:			>=0: 成功
					 <0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecAbility(  int iDecDevice, ST_DECODER_ABILITY* stDecoderAbility); 

	/*************************************************
	Function:		PPVCapturePicture
	Description:	开始抓拍
	Input:			iDecDevice:		      设备标识
					iChan:				  设备通道号			  
					stCapturePictureInfo  抓拍信息
	Output:         stCapturePictureInfo  抓拍信息
	return:			>=0: 成功
	<0: 失败 
	*************************************************/
	LIBPPVCLIENT_API int PPVCapturePicture(  int iDecDevice, int iChan, ST_CAPTUREPICTURE_INFO * stCapturePictureInfo);

	/*************************************************
	Function:		NotifySubscribe
	Description:	通知信息订阅
	Input:			stPassbyServer:					注册服务器的信息
					pfnNotifyCallBack:				接收Notify的回调函数
					nUserDataForCallBack;			用户参数
	Output:			
	return:			>=0 订阅成功返回的句柄
					-1  失败
	remark: 
	设备上下线消息:
	<Notify Type="DeviceInfo">
	<DeviceID>test0123</DeviceID>
	<!--设备上线:on;设备掉线:off-->
	<OnOffLine>On/Off</OnOffLine>
	</Notify>

	*************************************************/
	LIBPPVCLIENT_API int PPVNotifySubscribe( ST_ACCESS_SERVER_INFO stPassbyServer, pNotifyCallBack pfnNotifyCallBack, unsigned int nUserDataForCallBack );

	/*************************************************
	Function:		NotifyUnSubscribe
	Description:	取消通知订阅(包括全部订阅、部分订阅)
	Input:			
					iNotifySubsribeID:	订阅通知信息的返回句柄
	Output:
	return:			
	*************************************************/
	LIBPPVCLIENT_API void PPVNotifyUnSubscribe( int iNotifySubsribeID );


#ifdef __cplusplus
}
#endif

#endif