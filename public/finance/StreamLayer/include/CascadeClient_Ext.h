#ifndef __CASCADECLIENT_EXT_H__
#define __CASCADECLIENT_EXT_H__

#include <stdlib.h>
#if defined (_WIN32)
#if (!defined(CALLBACK))
#define CALLBACK __stdcall
#endif
#else
#define CALLBACK 
#endif

//CASC_fClientCallBack，回调数据类型 iDataType
#define DATA_TYPE_CATALOG                    0              /**< 目录                     */
#define DATA_TYPE_REALMEDIA                  1              /**< 实时URL                  */
#define DATA_TYPE_FILE                       2              /**< 录像文件列表             */
#define DATA_TYPE_FILEURL                    3              /**< 录像点播URL              */
#define DATA_TYPE_REGISTER                   4              /**< 注册                     */
#define DATA_TYPE_HEARTBEAT                  5              /**< 心跳                     */
#define DATA_TYPE_PTZ                        6              /**< 云台控制                 */
#define DATA_TYPE_STARTSTREAM                7              /**< 请求实时流、回放流       */
#define DATA_TYPE_STOPSTREAM                 8              /**< 停止实时流、回放流       */
#define DATA_TYPE_SERVERSTOP                 10             /**< 与级联服务器连接状态     */
#define DATA_TYPE_PAGVOICEURL                11             /**< 语音对讲URL              */
#define DATA_TYPE_CATALOGQUERY               12             /**< 目录查询                 */
#define DATA_TYPE_FILEEND                    13             /**< 回放或者下载结束         */

//add by yangcheng begin 
//解码插件相关回调消息类型
#define DATA_TYPE_DECODER_RECV_DATA_ERROR    21              /**< 媒体处理异常，接收数据错误   */
#define DATA_TYPE_DECODER_RECV_PACKET_NUM    22              /**< 周期内媒体包数信息           */
#define DATA_TYPE_DECODER_PLAYBACK_TIME      27              /**< 录像回放进度信息（单位为秒） */
#define DATA_TYPE_DECODER_FILE_EDN           28              /**< 录像回放结束标识             */

//码流类型
#define STREAM_TYPE_HEAD		0
#define STREAM_TYPE_VIDEO		1
#define STREAM_TYPE_AUDIO		2
#define STREAM_TYPE_END		    100
//add by yangcheng end

//返回值
#define   CASC_RET_OK		                 0              /**< 执行成功                 */
#define   CASC_RET_ERROR                     -1             /**< 执行失败                 */
#define   CASC_RET_FILEOVER                  1              /**< 录像文件全部返回         */


//级联服务器错误码定义 
#define ERR_CASCADE_400_FAIL                 400          //失败响应，请求失败
#define ERR_CASCADE_401_FAIL                 401          //失败响应，请求失败

#define ERR_HPR_INIT_FAIL                    1001         //hpr库初始化失败
#define ERR_TIMER_CREATE_FAIL                1002         //创建定时器失败
#define ERR_LETTERLIB_INIT_FAIL              1003         //letterlib库初始化失败
#define ERR_THREAD_CREATE_FAIL               1004         //创建线程失败
#define ERR_EXOSIP_INIT_FAIL                 1005         //SIP库初始化失败
#define ERR_EXOSIP_LISTEN_FAIL               1006         //SIP库启动监听失败
#define ERR_EXOSIP_SEND_FAIL                 1007         //SIP库发送消息失败
#define ERR_SOCKET_CREATE_FAIL               1008         //创建socket失败或启动socket监听失败
#define ERR_IOCP_CREATE_FAIL                 1009         //iocp创建失败
#define ERR_CONFIG_GETPARAM_FAIL             1010         //从配置文件读取配置参数错误
#define ERR_DB_CREATE_FAIL                   1011         //创建或打开sqlite数据库文件失败
#define ERR_MEMORY_APPLY_FAIL                1012         //内存申请失败
#define ERR_SOCKET_SET_REUSE_FAIL            1013         //设置SOCKET重用失败
#define ERR_SOCKET_SET_BLOCK_FAIL            1014         //设置SOCKET是否阻塞失败
#define ERR_APPWEB_INIT_FAIL                 1015         //appWeb服务器初始化失败
#define ERR_WEBLAYER_INIT_FAIL               1016         //WebLayer，处理web消息层初始化失败


#define ERR_HIK_REQHEAD_PARESE_FAIL          1201         //解析海康协议请求xml头错误
#define ERR_HIK_RSPHEAD_PARESE_FAIL          1202         //解析海康协议响应xml头错误
#define ERR_HIK_RSP_PARESE_FAIL              1203         //解析海康协议响应xml体错误
#define ERR_HIK_REQ_PARESE_FAIL              1204         //解析海康协议请求xml体错误
#define ERR_HIK_UP_NOTREGISTER               1205         //海康协议上级未注册
#define ERR_HIK_CATALOG_FAIL                 1206         //海康协议推送目录上级回应失败
#define ERR_HIK_CREATE_LISTENSOCKET_FAIL     1207         //海康协议，创建监听socket失败（7100）
#define ERR_HIK_DOWN_CASCADE_NOT_FIND        1208         //海康协议，未找到对应的下级级联服务器

#define ERR_CMSWS_INIT_FAIL                  1401         //初始化CMS WebService 服务失败
#define ERR_CMSWS_START_FAIL                 1402         //启动 CMS WebService 服务失败
#define ERR_CMSWS_GETCONTROLUNIT_FAIL        1403         //从cms获取所有的控制中心组织失败
#define ERR_CMSWS_GETREGIONUNIT_FAIL         1404         //从cms获取所有的区域失败
#define ERR_CMSWS_GETVRMSERVER_FAIL          1405         //从cms获取所有的VRM信息失败
#define ERR_CMSWS_GETVTDUSERVER_FAIL         1406         //从cms获取所有的vtdu信息失败
#define ERR_CMSWS_GETPTZSERVER_FAIL          1407         //从cms获取所有的ptz信息失败
#define ERR_CMSWS_GETALARMSERVER_FAIL        1408         //从cms获取所有的alarm信息失败
#define ERR_CMSWS_GETPAGSERVER_FAIL          1409         //从cms获取所有的pag信息失败
#define ERR_CMSWS_GETCASCADESERVER_FAIL      1410         //从cms获取所有的cascade信息失败
#define ERR_CMSWS_GETALLDEVICE_FAIL          1411         //从cms获取所有的设备信息失败
#define ERR_CMSWS_GETALLCAMERA_FAIL          1412         //从cms获取所有的监控点信息失败
#define ERR_CMSWS_SAVECATALOG_FAIL           1413         //向cms发送下级目录信息失败

#define ERR_CAMEAR_NOEXIST                   1601         //该监控点不存在
#define ERR_CASCADE_NOEXIST                  1602         //对应的级联服务器不存在
#define ERR_INSERT_PTZQUEUE_FAIL             1603         //插入任务到ptz队列失败
#define ERR_INSERT_MEDIAQUEUE_FAIL           1604         //插入任务到media队列失败
#define ERR_INSERT_DB33QUEUE_FAIL            1605         //插入任务到db33队列失败
#define ERR_INSERT_DHKPQUEUE_FAIL            1606         //插入任务到dhkp队列失败
#define ERR_DOWN_CASCADE_NOT_FIND            1607         //找不到对应的下级级联服务器
#define ERR_UP_CASCADE_NOT_FIND              1608         //找不到对应的上级级联服务器

#define ERR_PTZ_SERVER_NOEXIST               1701         //云台控制时，获取ptz服务器信息失败
#define ERR_PTZ_COMM_PTZSERVER_FAIL          1702         //与云台服务器通信失败
#define ERR_PTZ_CONTROL_FAIL                 1703         //云台服务器返回云台控制失败
#define ERR_PTZ_HIKTODB33_FAIL               1704         //云台控制命令由海康格式转换为db33格式失败
#define ERR_PTZ_DB33TOHIK_FAIL               1705         //云台控制命令由db33格式转换为海康格式失败

#define ERR_FILE_VRM_NOEXIST                 1801         //获取vrm信息失败
#define ERR_FILE_QUERYINTERFACE_NEW_FAIL     1802         //调用vrm录像查询库，创建失败
#define ERR_FILE_QUERYINTERFACE_OPEN_FAIL    1803         //调用vrm录像查询库，打开失败
#define ERR_FILE_STATS_OVER_FAIL             1804         //录像查询返回状态失败
#define ERR_FILE_FILES_NUM_ZERO              1805         //查询到的录像文件数为0
#define ERR_FILE_FROMINDEX_OVER_TOTAL        1806         //录像分页查询开始序号大于文件总数


#define ERR_FILEURL_FILES_NOEXIST            1901         //查询点播url，对应文件列表不存在
#define ERR_FILEURL_FILE_SEARCH_FAIL         1902         //查询点播url，对应文件列表查询失败
#define ERR_FILEURL_FILE_NOEXIST             1903         //查询点播url，该文件在对应文件列表中不存在

#define ERR_MEDIAURL_DEVICE_NOEXIST          2001         //未找到该监控点对应的设备信息
#define ERR_MEDIAURL_REGION_NOEXIST          2002         //未找到该监控点对应的区域信息
#define ERR_MEDIAURL_VTDU_NOEXIST            2003         //未找到该监控点对应的VTDU服务器信息
#define ERR_MEDIAURL_PAG_NOEXIST             2004         //未找到该监控点对应的pag服务器信息
#define ERR_MEDIAURL_CAMEAR_NOEXIST          2005         //该监控点在本级不存在


#define ERR_MEDIA_CONNECT_SERVER_FAIL        2101         //连接MEDIA服务器失败
#define ERR_MEDIA_SEND_HEAD_TO_SERVER_FAIL   2102         //向MEDIA服务器发送消息头失败
#define ERR_MEDIA_SEND_BODY_TO_SERVER_FAIL   2103         //向MEDIA服务器发送消息体失败
#define ERR_MEDIA_RECV_HEAD_FROM_SERVER_FAIL 2104         //从MEDIA服务器接收消息头失败
#define ERR_MEDIA_RECV_BODY_FROM_SERVER_FAIL 2105         //从MEDIA服务器接收消息体失败
#define ERR_COMM_MEDIA_RSP_FAIL              2106         //MEDIA响应回复失败
#define ERR_COMM_MEDIA_FAIL                  2107         //与MEDIA通信失败
#define ERR_COMM_MEDIA_RSP_PARSE_FAIL        2108         //解析MEDIA返回报文错误
#define ERR_COMM_MEDIA_OUTOF_LIMIT           2109         //超过处理上限

#define ERR_CAMERA_INDEXCODE_NULL            2201         //监控点编码为空
#define ERR_DB33_REQ_PARSE_FAIL              2202         //DB33协议，解析请求xml报文错误
#define ERR_DB33_DOWN_NO_REGISTER            2203         //DB33协议，下级未注册
#define ERR_DB33_DOWN_CASCADE_NOT_FIND       2204         //DB33协议，未找到对应的下级级联服务器
#define ERR_CASCADE_USERCODE_NULL            2205         //级联服务器用户编码为空
#define ERR_CASCADE_INDEXCODE_NULL           2206         //级联服务器域编码为空
#define ERR_CAMERA_SHARENODE_NOEXIST         2207         //监控点共享给上级节点信息未找到
#define ERR_CAMERA_WITHOUT_PRIVILEGE         2208         //上级没有该项功能权限
#define ERR_DB33_RSP_PARSE_FAIL              2209         //DB33协议，解析响应xml报文错误
#define ERR_DB33_CATALOG_PARSE_FAIL          2210         //解析目录推送报文xml体错误
#define ERR_DB33_CATALOG_DECODE_FAIL         2211         //解析编码错误

#define ERR_MOUDLE_MQ_INIT_FAIL              2401         //mq模块初始化失败

#define ERR_LOAD_MEDIA_NOEXIST               2501         //找不到对应的media
#define ERR_LOAD_MEDIA_EMPTY                 2502         //无media可用
#define ERR_LOAD_MEDIA_NO_REGISTER           2503         //向media注册未成功
#define ERR_LOAD_MEDIA_SESSION_NOEXIST       2504         //SessionId不存在

#define ERR_RES_SHARETOUP_NOTFIND            2601         //共享给该上级的资源未找到

#define ERR_RSP_TIMEOUT                      2701         //命令响应超时

#define Playback					"Playback"
#define Download					"Download"
//级联服务器错误码定义 end



/**	@fn	int CASC_fClientCallBack
 *	@brief	<消息回调>
 *	@param sessionHandle 标识句柄，与请求返回值相同.
 *	@param iDataType 数据类型，见上 CASC_fClientCallBack回调数据类型 iDataType.
 *	@param iErrorCode 0为请求成功，1为查询的录像文件全部返回，查询结束。返回其他值为失败.具体错误码，见级联服务器错误码宏定义。
 *	@param pResult 返回结果内容.
 *	@param pUsrData 用户数据.
 */
typedef int (CALLBACK *CASC_fClientCallBack)(int iSessionHandle, int iDataType, int iErrorCode, char *pResult, void* pUsrData);


/**	@fn	struct CASC_REQ_INIT
 *	@brief	<CascadeClient库初始化结构体，包含级联服务器相关信息>
            < 回调函数，所有结果可从该回调返回, 也可选择从各自回调返回>
 */
struct CASC_REQ_INIT
{
	char szServerIP[32];                     /**< 级联服务器IP地址                     */
	int iServerPort;                         /**< 级联服务器监听端口                   */
	char szSvrIndexCode[32];                 /**< 级联服务器编号                       */
	char szVersion[32];                      /**< 级联服务器版本编号                   */
	char szServerCode[32];                   /**< 级联服务器代码                       */
	CASC_fClientCallBack fClientCallBack;    /**< 回调函数，所有结果从该回调返回。     */
	void* pUserData;                         /**< 用户数据                             */

	char szUserAgent[128];                   /**< 用户代理标识，当需要使用CASC_StartRealStreamEx等新接口时，该字段请赋值为“2.3.6”*/
	char szNetZone[128];                     /**< 网域标识                             */

	CASC_REQ_INIT()
	{
		strcpy_s(szServerIP, "127.0.0.1");
		iServerPort = 7100;
		strcpy_s(szSvrIndexCode, "11111111100000001");
		strcpy_s(szVersion, "V1.0.0");
		strcpy_s(szServerCode, "2012");
		fClientCallBack = NULL;
		pUserData = NULL;

		strcpy_s(szUserAgent, "Client");
		strcpy_s(szNetZone, "");
	}
};


/**	@fn	struct CASC_REQ_REALURL
 *	@brief	<实时URL结构体，包含请求通道相关信息>
            < 回调fRealUrlCallBack，返回实时Url,
			  回调若为NULL,实时Url从初始化结构体CASC_REQ_INIT中的fClientCallBack返回>
 */
struct CASC_REQ_REALURL
{
	char szCamaraIndexCode[32];             /**< 摄像机编码                     */
	CASC_fClientCallBack fRealUrlCallBack;  /**< 回调，返回实时Url，可为NULL    */
    void* pUserData;                        /**< 用户数据                       */   
	int iUseCastMap;                        /**< 启用映射地址 0-不启用，1-启用  */
	CASC_REQ_REALURL() 
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
        fRealUrlCallBack = NULL;
		pUserData = NULL;
		iUseCastMap = 0;
	}
};

/**	@fn	struct CASC_REQ_PAGVOICEURL
 *	@brief	<语音对讲URL结构体，包含请求通道相关信息>
            < 回调fPagVoiceUrlCallBack，返回语音对讲Url,
			  回调若为NULL, Url从初始化结构体CASC_REQ_INIT中的fClientCallBack返回>
 */
struct CASC_REQ_PAGVOICEURL
{
	char szCamaraIndexCode[32];                 /**< 摄像机编码                     */
	CASC_fClientCallBack fPagVoiceUrlCallBack;  /**< 回调，返回语音对接Url，可为NULL    */
    void* pUserData;                            /**< 用户数据                       */   
	CASC_REQ_PAGVOICEURL() 
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
        fPagVoiceUrlCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_CATALOGQUERY
 *	@brief	<目录查询结构体，包含目录相关信息>
            < 回调fCatalogQueryCallBack，返回查询结果，成功或失败
			  回调若为NULL, 查询结果从初始化结构体CASC_REQ_INIT中的fClientCallBack返回>
 */
struct CASC_REQ_CATALOGQUERY
{
	char szCatalogIndexCode[32];                 /**< 目录编码                     */
	CASC_fClientCallBack fCatalogQueryCallBack;  /**< 回调，返回查询结果，可为NULL */
    void* pUserData;                             /**< 用户数据                     */   
	CASC_REQ_CATALOGQUERY() 
	{
		strcpy_s(szCatalogIndexCode, "11111111100000001");
        fCatalogQueryCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_FILELIST
 *	@brief	<查询录像文件结构体，包含相关查询条件>
            < 回调fFilelistCallBack，返回文件列表,
			  回调若为NULL,文件列表从初始化结构体CASC_REQ_INIT中的fClientCallBack返回>
 */
struct CASC_REQ_FILELIST
{
	char szCamaraIndexCode[32];            /**< 摄像机编码                     */
	char szBeginTime[32];                  /**< 开始查询时间                   */
	char szEndTime[32];                    /**< 结束查询时间                   */
	int  iRecordPos;                       /**< 存储方式 DVR/PCNVR/NVT         */
	int  iRecordType;                      /**< 录像类型                       */
	int  iFromIndex;                       /**< 开始查询文件序号               */
	int  iToIndex;                         /**< 结束查询文件序号               */
	CASC_fClientCallBack fFilelistCallBack;/**< 回调，返回文件列表，可为NULL   */
    void* pUserData;                       /**< 用户数据                       */   
	CASC_REQ_FILELIST()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szBeginTime, "20111118T000000Z");
		strcpy_s(szEndTime, "20111118T235959Z");
		iRecordPos = 0;
		iRecordType = 0;
		iFromIndex = 1;
		iToIndex = 10;
		fFilelistCallBack = NULL;
		pUserData = NULL;
	}
};


/**	@fn	struct CASC_REQ_FILEURL
 *	@brief	<点播URL结构体，包含相关查询条件>
            <回调fFileUrlCallBack，返回录像点播Url,
			 回调若为NULL,点播Url从初始化结构体CASC_REQ_INIT中的fClientCallBack返回>
 */
struct CASC_REQ_FILEURL
{
	char szCamaraIndexCode[32];               /**< 摄像机编码                     */
	char szFileName[128];                     /**< 文件名称                       */
	char szBeginTime[32];                     /**< 文件开始回放时间               */
	char szEndTime[32];                       /**< 文件结束回放时间               */
	int  iRecordPos;                          /**< 存储方式 DVR/PCNVR/NVT         */
	int  iRecordType;                         /**< 录像类型                       */
	CASC_fClientCallBack fFileUrlCallBack;    /**< 回调，返回录像点播Url，可为NULL*/
    void* pUserData;                          /**< 用户数据                       */
	int iUseCastMap;						  /**< 启用映射地址 0-不启用，1-启用  */
	CASC_REQ_FILEURL()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szFileName, "ch11111111100000001");
		strcpy_s(szBeginTime, "20111118T000000Z");
		strcpy_s(szEndTime, "20111118T235959Z");
		iRecordPos = 0;
		iRecordType = 0;
        fFileUrlCallBack = NULL;
		pUserData = NULL;
		iUseCastMap = 0;
	}
};


/**	@fn	struct CASC_REQ_PTZ
 *	@brief	<云台控制命令结构体，包含云台控制命令相关信息>
 */
struct CASC_REQ_PTZ
{
	char szCamaraIndexCode[32];           /**< 摄像机编码                      */
	int iCommand;                         /**< 云台控制命令编号                */
	int iAction;                          /**< 云台动作类别                    */
	int iSpeed;                           /**< 云台速度                        */
	int iPriority;
	int iMatrixCameraId;
	int iMonitorId;
	int iStartX;
	int iStartY;
	int iEndX;
	int iEndY;
	int iPresetIndex;
	CASC_fClientCallBack fPTZCallBack;    /**< 回调，返回云台控制结果，可为NULL*/
	void* pUserData;                      /**< 用户数据                        */
	CASC_REQ_PTZ()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		iCommand = 21;
		iAction = 0;
		iSpeed = 4;
		iStartX = 0;
		iStartY = 0;
		iEndX = 0;
		iEndY = 0;
		iPresetIndex = 0;
		iPriority = 3;
		iMatrixCameraId = 1;
		iMonitorId = 1;
		fPTZCallBack = NULL;
		pUserData = NULL;
	}
};


/**	@fn	struct CASC_REQ_REALSTREAM
 *	@brief	<实时视频流结构体，包含请求通道相关信息>
 */
struct CASC_REQ_REALSTREAM
{
	char szCamaraIndexCode[32];               /**< 摄像机编码                     */
	char szLocalIP[32];                       /**< 收流IP地址                     */
	int iRecvPort;                            /**< 收流端口                       */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< 回调，返回请求成功与否         */
	void* pUserData;                          /**< 用户数据                       */
    CASC_REQ_REALSTREAM()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szLocalIP, "127.0.0.1");
		iRecvPort = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_PLAYBACK
*	@brief	<回放结构体，包含请求通道相关信息>
*/
struct CASC_REQ_PLAYBACK
{
	char szCamaraIndexCode[32];               /**< 摄像机编码                     */
	char szSSRC[32];							//直接填199999999
	char szPlayType[32];						//填Playback或者Download
	char szStartTime[32];						/*vrmtime*/
	char szEndTime[32];							/*vrmtime*/
	char szLocalIP[32];                       /**< 收流IP地址                     */
	int iRecvPort;                            /**< 收流端口                       */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< 回调，返回请求成功与否         */
	void* pUserData;                          /**< 用户数据                       */
	CASC_REQ_PLAYBACK()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szLocalIP, "127.0.0.1");
		strcpy_s(szSSRC, "1999999999");
		strcpy_s(szStartTime, "13311111111");
		strcpy_s(szEndTime, "13311112111");
		strcpy_s(szPlayType, Playback);
		iRecvPort = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_PLAYBACK
*	@brief	<回放结构体，包含请求通道相关信息>
*/
struct CASC_REQ_PLAYBACK_CONTROL
{
	char szCamaraIndexCode[32];               /**< 摄像机编码                                          */
	char szControlType[32];					  /**<恢复播放和拖动播放时-PLAY；暂停-PAUSE;               */
	char szScale[32];						  /**<0.125;0.25;0.5;1;2;4;8 分别是慢8、4、2和快2、4、8速度*/
	char szNpt[32];							  /**<播放拖动使用，非拖动操作可不填
											      当调用CASC_StartPlayBackControl时，该字段表示 相对播放开始时间的秒数；
											      当调用CASC_StartPlayBackControlEx时，该字段表示 绝对开始时间，形如yyyyMMddTHHmmssZ*/
	int m_nSessionId;                         /**< playback 返回的ID                                   */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< 回调，返回请求成功与否                              */
	void* pUserData;                          /**< 用户数据                                            */
	CASC_REQ_PLAYBACK_CONTROL()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szControlType, "PLAY");
		strcpy_s(szScale, "1");
		strcpy_s(szNpt, "0");
		m_nSessionId = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

enum CASC_TRANS_TYPE
{
	CASC_TRANS_UDP = 0,												//UDP传输方式
	CASC_TRANS_TCP													//TCP传输方式
};

enum CASC_STREAM_TYPE
{
	CASC_STREAM_MAIN = 0,											//主码流
	CASC_STREAM_SUB													//子码流
};

typedef int (CALLBACK * CASCMsgCB)(int iSessionHandle, int iMsgType, int iMsgCode, char *pResult, void* pUsrData);

typedef int (CALLBACK * CASCStreamCB)(int iSessionHandle, int iDataType, char* pData, int iDataLen, void* pUserData);

struct CASC_REALSTREAM_PARA
{
	char szCamaraIndexCode[32];                                //摄像机编码    
	char szRecvAddr[32];              	                       //接收地址 UDP时使用
	char szDecoderTag[32];                                     //解码插件类型（海康：hikvision；大华：dahua）

	int iStreamType;						                   //主子码流，CASC_STREAM_TYPE 
	int iTransMethod;						                   //传输协议，CASC_TRANS_TYPE 
	int iPort;							                       //接口端口 UDP时使用  
	void* hwnd;                                                //窗口句柄  
	CASCMsgCB pMsgCB;							               //消息回调       
	CASCStreamCB pStreamCB;						               //码流回调      
	void* pUserData;                                           //回调用户参数
	CASC_REALSTREAM_PARA()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szRecvAddr, "127.0.0.1");
		strcpy_s(szDecoderTag, "hikvision");
		iStreamType = CASC_STREAM_MAIN;
		iTransMethod = CASC_TRANS_UDP;
		iPort = 0;
		hwnd = NULL;
		pMsgCB = NULL;
		pStreamCB = NULL;
		pUserData = NULL;
	}
};

struct CASC_PLAYBACKSTREAM_PARA
{
	char szCamaraIndexCode[32];            /**< 摄像机编码                */
	char szDecoderTag[32];                 /**< 解码插件类型（海康：hikvision；大华：dahua）*/
	char szBeginTime[32];                  /**< 开始查询时间              */
	char szEndTime[32];                    /**< 结束查询时间              */
	CASCMsgCB pMsgCB;					   /**< 消息回调                  */
	CASCStreamCB pStreamCB;				   /**< 码流回调                  */
	void* pUserData;				       /**< 用户参数                  */
	int iTransMethod;				       /**< 传输协议                  */
	char szRecvAddr[16];                   /**< 接收地址 UDP时使用        */
	int iPort;						       /**< 接口端口 UDP时使用        */
	int iDownload;                         /**< 0为Playback，1为Download  */
	char szFileName[128];                  /**< 回放文件名                */
	void* hwnd;                            /**< 窗口句柄                  */
	int  iRecordPos;                       /**< 存储方式 DVR/PCNVR/NVT    */
	int  iRecordType;                      /**< 录像类型                  */
	CASC_PLAYBACKSTREAM_PARA()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szDecoderTag, "hikvision");
		//时间格式暂时定为yyyyMMddTHHmmssZ
		strcpy_s(szBeginTime, "20130907T101112Z");
		strcpy_s(szEndTime, "20130907T101112Z");
		pMsgCB = NULL;
		pStreamCB = NULL;
		pUserData = NULL;
		iTransMethod = CASC_TRANS_UDP;
		strcpy_s(szRecvAddr, "127.0.0.1");
		iPort = 0;
		iDownload = 0;
		strcpy_s(szFileName, "");
		hwnd = NULL;
		iRecordPos = 0;
		iRecordType = 0;
	}
};

#endif