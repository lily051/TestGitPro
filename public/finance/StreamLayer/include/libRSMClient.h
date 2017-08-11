/**	@file libRSMClient.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 流媒体客户端SDK，从流媒体服务器获取视频数据
 *
 *	@author		kuangjg
 *	@date		 2008/4/14
 *
 *	@note V2.1.0.0  用doxygen格式进行注解
 *
 *	@warning 无
 */

#ifndef __LIBRSMCLIENT_H__
#define __LIBRSMCLIENT_H__

#include <time.h>

#ifdef LIBRSMCLIENT_EXPORTS
#define RSM_CLIENT_API __declspec(dllexport)
#else
#define RSM_CLIENT_API __declspec(dllimport)
#endif


typedef int HSESSION;	/**< 会话类型定义 */


/**	@struct ST_DEVICE_NETINFO
*  @brief 设备信息
*/
typedef struct  
{
	char				szAccessSrvIP[32];	/**< 接入服务器IP*/
	unsigned short int	nAccessSrvPort;		/**< 接入服务器端口*/
	//unsigned char		szReserved[2];		/**< 已删除字段*/
	char				nNetLine;			/**< 设备所属网络线路编号:0,1,2...*/
	char				nStreamEnCodeing;	/**< 保留:转码标识*/
	char				szDeviceID[64];		/**< 设备序列号*/
	int					nChan;				/**< 通道号,从1开始*/
	int					nStreamType;		/**< 流类型: 0-主码流; 1-子码流*/
	int					nTransPortType;		/**< 要求流媒体从前端取流的方式0:UDP;1:TCP*/
}ST_DEVICE_NETINFO;


/**	@struct ST_SERVER_STATUS
*  @brief 服务器信息
*/
typedef struct  
{
	char				szSrvIP[32];				/**< 服务器IP*/
	unsigned short int	nSrvPort;					/**< 服务器端口*/
	unsigned short int	nInputStreamNum;			/**< 输入流数*/
	unsigned short int	nOutputStreamNum;			/**< 输出流数*/
	char				szSrvVersion[32];			/**< 服务器版本*/
	unsigned short int  nReserved;					/**< 保留字节*/
	time_t 				tOnlineTime;		        /**< 服务器在线时间*/  
	unsigned int		nCpuUsage;					/**< 服务器CPU使用率*/
	unsigned int		nTotalPhys;					/**< 服务器总的物理内存*/
	unsigned int		nAvailPhys;					/**< 服务器可用物理内存*/
}ST_SERVER_STATUS;

/**	@struct ST_RSM_SESSION_INFO
*  @brief 会话信息
*/
typedef struct  
{
	unsigned int nRecvedRTPCount;	/**< 接收到的总rtp包数目*/
	int			 iLostRTPCount;		/**< 丢失的rtp包数目*/
}ST_RSM_SESSION_INFO;


/**	@enum EN_RSM_STREAM_DATATYPE
*  @brief 码流类型,注意与EN_STREAM_DATATYPE中的常量定义区分开
*/
typedef enum
{
	STREAM_DATATYPE_DATA = 0,			/**< 数据*/
	STREAM_DATATYPE_HDR,				/**< 流头*/
	
}EN_RSM_STREAM_DATATYPE;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	*@brief 流数据回调函数类型声明
	*@param hSessionID:	区分不同音视频流的ID
	*@param iUsrData:	用户数据
	*@param iDataType:	流数据类型:流头数据还是普通数据 STREAM_DATATYPE_DATA:普通数据; STREAM_DATATYPE_HDR:头数据
	*@param pAVData:	指向流数据缓冲
	*@param nAVDataLen:	流数据字节数
	*@return 无返回值
	**/
	typedef void (__stdcall *pRSMAVDataCallBack)( HSESSION hSessionID, unsigned int nUsrData,
		int iDataType, const void* pAVData, size_t nAVDataLen);


	/**
	*@brief 初始化流媒体客户端库
	*@return 0:成功; -1:失败
	**/
	RSM_CLIENT_API int __stdcall InitRSMClientLib( );


	/**
	*@brief 终止流媒体客户端库
	*@return 0:成功; -1:失败
	**/
	RSM_CLIENT_API int __stdcall FiniRSMClientLib(void);


	/**
	*@brief 开始从流媒体服务器取流
	*@param pszSrvIP:		服务端IP
	*@param nSrvPort:		服务端端口
	*@param pszPrivate:		私有串
	*@param pDeviceInfo:	指向要连接的设备信息
	*@param pRSMAVDataCallBack:获取到的视频数据回调函数指针
	*@param nUsrData:		与流相关的用户数据
	*@return >=0:取流成功, 该值作为流的ID; -1:失败
	**/
	RSM_CLIENT_API HSESSION __stdcall PlayRSMAVStream( const char* pszSrvIP, 
		unsigned short int nSrvPort, const char* pszPrivate,
		const ST_DEVICE_NETINFO* pDeviceInfo, pRSMAVDataCallBack pfn, unsigned int nUsrData );


	/**
	*@brief 停止从服务器取流
	*@param hSessionID:	PlayAVStream返回的流ID 
	*@return 无返回值
	**/
	RSM_CLIENT_API void __stdcall StopRSMAVStream( HSESSION hSessionID );


	/**
	*@brief 获取服务器状态
	*@param pszSrvIP:		服务器IP
	*@param nSrvPort:		服务器端口
	*@param pServerStatus:	指向服务器信息
	*@return 0:成功; -1:失败
	**/
	RSM_CLIENT_API int	__stdcall RSMGetStatus( const char* pszSrvIP, unsigned short int nSrvPort, ST_SERVER_STATUS* pServerStatus );

	/**
	*@brief 获取会话信息
	*@param hSessionID:		会话ID
	*@param pSessionInfo:	会话信息结构体指针
	*@return 0:成功; -1:失败
	**/
	RSM_CLIENT_API int __stdcall GetRSMSessionInfo( HSESSION hSessionID,  ST_RSM_SESSION_INFO* pSessionInfo );


	/**
	*@brief 开始从服务器取流,采用tcp方式
	*@param pszSrvIP:		服务端IP
	*@param nSrvPort:		服务端端口
	*@param pszPrivate:		私有串
	*@param pDeviceInfo:	指向要连接的设备信息
	*@param pRSMAVDataCallBack:获取到的视频数据回调函数指针
	*@param nUsrData:		与流相关的用户数据
	*@return 0:成功; -1:失败
	**/
	RSM_CLIENT_API HSESSION __stdcall PlayRSMAVStreamByTCP( const char* pszSrvIP, 
		unsigned short int nSrvPort, const char* pszPrivate,
		const ST_DEVICE_NETINFO* pDeviceInfo, pRSMAVDataCallBack pfn, unsigned int nUsrData );


	/**
	*@brief 停止TCP方式从服务器取流
	*@param hSessionID:	PlayRSMAVStreamByTCP返回的流ID 
	*@return 无
	**/
	RSM_CLIENT_API void __stdcall StopRSMAVStreamByTCP( HSESSION hSessionID );

	/**
	*@brief 取消正在发送的视频播放请求，调用该接口后，PlayRSMAVStreamByTCP和PlayRSMAVStream接口将快速返回
	*@return 无
	**/
    RSM_CLIENT_API void __stdcall CancellPlayRequest( );


#ifdef __cplusplus
}
#endif

#endif

