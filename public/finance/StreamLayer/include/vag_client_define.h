/**	@file vag_client_define.h
*   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*   @brief 设备接入网关客户端结构和宏定义 
*
*	@author liyingtao@hikvision.com
*	@date 2013年01月15日
*
*	@note 历史记录：V1.0.0
*	@note 
*
*	@warning
*/

#ifndef _VAG_CLIENT_DEFINE_H_
#define _VAG_CLIENT_DEFINE_H_

/********************接口调用返回值*********************/
#define VAG_SUCCESS									0		// 调用接口正确
#define VAG_FAIL									-1		// 调用接口失败

/********************资源类型*********************/
#define VAG_GROUP									0x01	// 分组
#define VAG_DEVICE									0x02	// 编码器
#define VAG_CAMERA									0x03	// 摄像头
#define VAG_CAMERA_DEVICE							0x04	// 摄像头与设备信息

#define VAG_IPV4_LEN								16		// IPV4地址长度
#define VAG_USER_NAME_LEN							64		// 用户名长度
#define VAG_PASSWORD_LEN							64		// 密码长度
#define VAG_INDEXCODE_LEN							128		// 索引号长度
#define VAG_USERAGENT_LEN							128		// 用户代理长度
#define VAG_NAME_LEN								128		// 名称长度
#define VAG_PTZ_POIT_NUM							256		// 云台最大巡航预置点数

#define VAG_GPS_PUB_URL_LEN						    256		// 事件发布url长度
#define VAG_EVENT_PUB_URL_LEN						256		// 事件发布url长度
#define VAG_STREAM_URL_LEN							256		// 取流url长度
#define VAG_MAX_DAYS								7		// 一周最大天数
#define VAG_PICSERVER_URL_LEN						256		// 图片服务器url长度
#define VAG_URL_EXT_LEN				    	    	128		// 扩展URL长度

#define VAG_CHAN_OSD_MAX_LEN						128		// OSD字符长度
#define VAG_CHAN_OSD_MAX_NUM						16		// OSD字符最大个数

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif

#ifdef __linux__
typedef void* HWND;
#endif

/********************VAG_CLIENT错误码定义 begin*********************/
#define VAG_ERR_NOERROR								0		// 无错误

//VAG_CLIENT相关错误码
#define VAG_Client_EBase							1000						//< 错误码基数。  
#define VAG_ERR_VAG_CLIENT_NOT_INIT					(VAG_Client_EBase + 0)		// VAG_CLIENT未初始化
#define VAG_ERR_HPR_INIT_FAIL						(VAG_Client_EBase + 1)		// Hpr初始化失败
#define VAG_ERR_VAG_MALLOC_FAIL						(VAG_Client_EBase + 2)		// 申请内存失败
#define VAG_ERR_PARA_ERROR							(VAG_Client_EBase + 3)		// 参数错误
#define VAG_ERR_PTR_PARA_NULL						(VAG_Client_EBase + 4)		// 传入指针参数为空
#define VAG_ERR_COMMUNICATE_FAIL					(VAG_Client_EBase + 5)		// 与设备接入网关交互失败
#define VAG_ERR_RSP_PARSE_FAIL						(VAG_Client_EBase + 6)		// 设备接入网关返回的响应报文错误
#define VAG_ERR_RETURN_RSP_FAIL						(VAG_Client_EBase + 7)		// 设备接入网关返回响应失败
#define VAG_ERR_CLENT_HANDLE_NOEXIST				(VAG_Client_EBase + 8)		// 网关客户端句柄不存在
#define VAG_ERR_ADD_HANDLE_FAIL						(VAG_Client_EBase + 9)		// 生成客户端句柄失败
#define VAG_ERR_NOT_SUPPORT							(VAG_Client_EBase + 10)		// 不支持的功能
#define VAG_ERR_RECORD_TYPE_SUPPORT					(VAG_Client_EBase + 11)		// 查询的录像类型不支持
#define VAG_ERR_DEVICE_NOT_SUPPORT					(VAG_Client_EBase + 12)		// 所请求的设备不支持该功能
#define VAG_ERR_RECORD_FILE_QUERYING				(VAG_Client_EBase + 13)		// 正在查询录像文件
#define VAG_ERR_BUFFER_TOO_SMALL					(VAG_Client_EBase + 14)		// 缓冲区太小
#define VAG_ERR_DEV_PLUGUN_NULL						(VAG_Client_EBase + 15)		// 设备插件实例为空

#define VAG_ERR_RESOURCE_SIZE_ZERO					(VAG_Client_EBase + 101)	// 资源长度为0
#define VAG_ERR_RESOURCE_END_POS					(VAG_Client_EBase + 102)	// 到达资源列表尾
#define VAG_ERR_RESOURCE_NOEXIST					(VAG_Client_EBase + 103)	// 查询的资源不存在
#define VAG_ERR_RESOURCE_NOT_DOWNLOAD				(VAG_Client_EBase + 104)	// 未下载请求的资源列表

#define VAG_ERR_VAG_PTZ_CMD_UNKNOW					(VAG_Client_EBase + 201)	// 无法解析的云台控制命令号
#define VAG_ERR_VAG_PTZ_LOWER_PRIORITY				(VAG_Client_EBase + 202)	// 云台控制权限太低
#define VAG_ERR_VAG_PTZ_CAMERA_LOCKED				(VAG_Client_EBase + 203)	// 云台通道被锁定
#define VAG_ERR_VAG_PTZ_CAMERA_NOTEXIST				(VAG_Client_EBase + 204)	// 传入的通道编号不存在
#define VAG_ERR_VAG_PTZ_DEV_LOGIN_FAIL				(VAG_Client_EBase + 205)	// 登陆设备失败
#define VAG_ERR_VAG_PTZ_DEV_PLUGIN_FAIL				(VAG_Client_EBase + 206)	// 调用设备插件返回失败

#define VAG_ERR_VAG_VTDU_CLIENT_LOAD_FAIL			(VAG_Client_EBase + 301)	// 取流SDK加载失败
#define VAG_ERR_VAG_VTDU_START_FAIL					(VAG_Client_EBase + 302)	// 取流SDK取流失败
#define VAG_ERR_VAG_STREAM_CHAN_NOEXIST				(VAG_Client_EBase + 303)	// 取流通道不存在
#define VAG_ERR_DEV_START_STREAM_FAILD				(VAG_Client_EBase + 304)	// 直接从设备取流失败
#define VAG_ERR_VAG_VTDU_CREATE_SESSION_FAIL		(VAG_Client_EBase + 305)	// 取流SDK创建session失败
#define VAG_ERR_VAG_INITIATIVEDEV_STREAM_FAIL		(VAG_Client_EBase + 306)	// 主动设备取流交互失败
#define VAG_ERR_VAG_VTDU_GETVIDEOPARA_FAIL			(VAG_Client_EBase + 307)	// 获取预览视频参数失败
#define VAG_ERR_VAG_VTDU_SETVIDEOPARA_FAIL			(VAG_Client_EBase + 308)	// 设置预览视频参数失败
#define VAG_ERR_VAG_VTDU_STREAM_HANDLE_NOEXIST		(VAG_Client_EBase + 309)	// 预览句柄不存在
#define VAG_ERR_VAG_DEV_GETVIDEOPARA_FAIL			(VAG_Client_EBase + 310)	// 从设备获取预览视频参数失败
#define VAG_ERR_VAG_DEV_SETVIDEOPARA_FAIL			(VAG_Client_EBase + 311)	// 向设备设置预览视频参数失败
#define VAG_ERR_VAG_PLAY_SDK_LOAD_FAIL				(VAG_Client_EBase + 312)	// 解码插件库加载失败
#define VAG_ERR_VAG_CAPTURE_BUFFER_SMALL			(VAG_Client_EBase + 313)	// 抓图传入的缓冲区太小
#define VAG_ERR_VAG_PLAYBACK_HANDLE_NOEXIST			(VAG_Client_EBase + 314)	// 回放取流句柄不存在
#define VAG_ERR_ADD_STREAM_HANDLE_FAIL				(VAG_Client_EBase + 315)	// 生成取流句柄失败

#define VAG_ERR_VAG_NOT_ONLINE						(VAG_Client_EBase + 401)	// VAG不在线

#define VAG_ERR_VAG_TALK_MAX_NUM					(VAG_Client_EBase + 501)	// 达到最大设备对讲数
#define VAG_ERR_VAG_TALK_HANDLE_NOEXIST				(VAG_Client_EBase + 502)	// 设备对讲句柄不存在
#define VAG_ERR_ADD_TALK_HANDLE_FAIL				(VAG_Client_EBase + 503)	// 生成对讲句柄失败

#define VAG_ERR_CFG_CMD_NOT_SUPPORT					(VAG_Client_EBase + 601)	// 参数配置命令号不支持

//PAG服务相关错误码
#define PAG_Srv_EBase								4000						//< 错误码基数。
#define PAG_NEW_FAILED						        (PAG_Srv_EBase + 1)			// 内存申请失败
#define PAG_INDEXCODE_NOTEXIT						(PAG_Srv_EBase + 2)			// 设备索引不存在
#define PAG_UNKOWN_MSGTYPE							(PAG_Srv_EBase + 3)			// 未知的协议类型
#define PAG_PARSEXML_FAIL							(PAG_Srv_EBase + 4)			// 协议解析失败
#define PAG_PLUGIN_LOGINFAIL            	        (PAG_Srv_EBase + 5)			// 登录设备失败
#define PAG_DEVCONT_CONNC							(PAG_Srv_EBase + 6)			// 连接设备失败
#define PAG_DEVPLUGIN_NOTEXIT						(PAG_Srv_EBase + 7)			// 设备插件不存在
#define PAG_UNSUPPORT_DEVTYPE						(PAG_Srv_EBase + 8)			// 设备类型不支持
#define PAG_EVENT_RESUB								(PAG_Srv_EBase + 9)			// 重复订阅事件
#define PAG_EVENT_NOTSUB							(PAG_Srv_EBase + 10)		// 事件未订阅
#define PAG_NOT_ONLINE								(PAG_Srv_EBase + 11)		// 主动设备不在线
#define PAG_PARAIS_NULL								(PAG_Srv_EBase + 12)		// 指针参数为空
#define PAG_GETCHANNUM_FAIL							(PAG_Srv_EBase + 13)		// 从本地数据库获取通道号失败
#define PAG_GETTALKHANDL_FAIL						(PAG_Srv_EBase + 14)		// 获取对讲实例句柄失败
#define PAG_PICSIZE_TOO_LONG            	        (PAG_Srv_EBase + 15)		// 设备返回的图片数据过大
#define PAG_PICDATA_POST_FAIL						(PAG_Srv_EBase + 16)		// 图片数据投递平台SDK失败
#define PAG_BUFFER_TOO_SMALL						(PAG_Srv_EBase + 17)		// 申请的缓存空间太小
#define PAG_CHANNUM_ERR								(PAG_Srv_EBase + 18)		// 通道号错误
#define PAG_ALARMOUT_CONTROLVALUE_ERR				(PAG_Srv_EBase + 19)		// 报警输出控制类型值错误
#define PAG_CONFIG_CMD_UNSUPPORT					(PAG_Srv_EBase + 20)		// 不支持的远程配置类型
#define PAG_DEVICE_UNLOGIN							(PAG_Srv_EBase + 21)		// 设备未登录，或者登录句柄为空
#define PAG_DEVINFO_FROMDB_FAIL						(PAG_Srv_EBase + 22)		// 从数据库获取设备信息失败，设备编号不存在
#define PAG_CAMINFO_FROMDB_FAIL						(PAG_Srv_EBase + 23)		// 从数据库获取监控点信息失败，监控点编号不存在
#define PAG_UNKOWN_REGTYPE							(PAG_Srv_EBase + 24)		// 未知的注册类型

#define PAG_PLAN_SEG_TOO_MORE						(PAG_Srv_EBase + 30)		// 录像计划配置时的片段数太多
#define PAG_QUERY_RECORD_TIMEOUT					(PAG_Srv_EBase + 31)		// 查询录像文件超时
#define PAG_QUERY_RECORD_UNSUPORT					(PAG_Srv_EBase + 32)		// 查询录像文件类型不支持
#define PAG_QUERY_RECORD_HANDLE_ERR     	        (PAG_Srv_EBase + 33)		// 录像文件句柄为空

#define PAG_PTZ_CMD_UNSUPPORT						(PAG_Srv_EBase + 50)		// 云台控制命令不支持

#define PAG_SIP_MESSAGE_BUILD_FAIL					(PAG_Srv_EBase + 60)		// sip协议消息请求失败
#define PAG_SIP_MESSAGE_SEND_FAIL					(PAG_Srv_EBase + 61)		// sip协议消息构建失败
#define PAG_SIP_INVITE_BUILD_FAIL					(PAG_Srv_EBase + 62)		// sip协议INVITE请求失败
#define PAG_SIP_INVITE_SEND_FAIL					(PAG_Srv_EBase + 63)		// sip协议INVITE构建失败

#define PAG_BI_LIB_FAIL								(PAG_Srv_EBase + 70)		// 调用B接口库失败

#define PAG_SERVER_INNER_ERROR						(PAG_Srv_EBase + 401)		// vag服务内部错误
#define PAG_ERR_DEVICE_NOT_SUPPORT			        (PAG_Srv_EBase + 402)		// 所请求的设备不支持该功能
#define PAG_SRVCONECT_FAIL      			        (PAG_Srv_EBase + 403)		// 连接服务端失败

//PAG和设备相关的错误码
#define PAG_Plug_EBase								5000						///< 错误码基数

#define PAG_NOERROR									(PAG_Plug_EBase + 0)		// 没有错误
#define PAG_PASSWORD_ERROR 							(PAG_Plug_EBase + 1)		// 用户名密码错误
#define PAG_NOENOUGHPRI 							(PAG_Plug_EBase + 2)		// 权限不足
#define PAG_NOINIT 									(PAG_Plug_EBase + 3)		// 没有初始化
#define PAG_CHANNEL_ERROR 							(PAG_Plug_EBase + 4)		// 通道号错误
#define PAG_OVER_MAXLINK 							(PAG_Plug_EBase + 5)		// 连接到DVR的客户端个数超过最大
#define PAG_VERSIONNOMATCH							(PAG_Plug_EBase + 6)		// 版本不匹配
#define PAG_NETWORK_FAIL_CONNECT					(PAG_Plug_EBase + 7)		// 连接服务器失败
#define PAG_NETWORK_SEND_ERROR						(PAG_Plug_EBase + 8)		// 向服务器发送失败
#define PAG_NETWORK_RECV_ERROR						(PAG_Plug_EBase + 9)		// 从服务器接收数据失败
#define PAG_NETWORK_RECV_TIMEOUT					(PAG_Plug_EBase + 10)		// 从服务器接收数据超时
#define PAG_NETWORK_ERRORDATA						(PAG_Plug_EBase + 11)		// 传送的数据有误
#define PAG_ORDER_ERROR								(PAG_Plug_EBase + 12)		// 调用次序错误
#define PAG_OPERNOPERMIT							(PAG_Plug_EBase + 13)		// 无此权限
#define PAG_COMMANDTIMEOUT							(PAG_Plug_EBase + 14)		// DVR命令执行超时
#define PAG_ERRORSERIALPORT							(PAG_Plug_EBase + 15)		// 串口号错误
#define PAG_ERRORALARMPORT							(PAG_Plug_EBase + 16)		// 报警端口错误
#define PAG_PARAMETER_ERROR 						(PAG_Plug_EBase + 17)		// 参数错误
#define PAG_CHAN_EXCEPTION							(PAG_Plug_EBase + 18)		// 服务器通道处于错误状态
#define PAG_NODISK									(PAG_Plug_EBase + 19)		// 没有硬盘
#define PAG_ERRORDISKNUM							(PAG_Plug_EBase + 20)		// 硬盘号错误
#define PAG_DISK_FULL								(PAG_Plug_EBase + 21)		// 服务器硬盘满
#define PAG_DISK_ERROR								(PAG_Plug_EBase + 22)		// 服务器硬盘出错
#define PAG_NOSUPPORT								(PAG_Plug_EBase + 23)		// 服务器不支持
#define PAG_BUSY									(PAG_Plug_EBase + 24)		// 服务器忙
#define PAG_MODIFY_FAIL								(PAG_Plug_EBase + 25)		// 服务器修改不成功
#define PAG_PASSWORD_FORMAT_ERROR					(PAG_Plug_EBase + 26)		// 密码输入格式不正确
#define PAG_DISK_FORMATING							(PAG_Plug_EBase + 27)		// 硬盘正在格式化,不能启动操作
#define PAG_DVRNORESOURCE							(PAG_Plug_EBase + 28)		// DVR资源不足
#define	PAG_DVROPRATEFAILED							(PAG_Plug_EBase + 29)		// DVR操作失败
#define PAG_OPENHOSTSOUND_FAIL 						(PAG_Plug_EBase + 30)		// 打开PC声音失败
#define PAG_DVRVOICEOPENED 							(PAG_Plug_EBase + 31)		// 服务器语音对讲被占用
#define	PAG_TIMEINPUTERROR							(PAG_Plug_EBase + 32)		// 时间输入不正确
#define	PAG_NOSPECFILE								(PAG_Plug_EBase + 33)		// 回放时服务器没有指定的文件
#define PAG_CREATEFILE_ERROR						(PAG_Plug_EBase + 34)		// 创建文件出错
#define	PAG_FILEOPENFAIL							(PAG_Plug_EBase + 35)		// 打开文件出错
#define	PAG_OPERNOTFINISH							(PAG_Plug_EBase + 36)		// 上次的操作还没有完成
#define	PAG_GETPLAYTIMEFAIL							(PAG_Plug_EBase + 37)		// 获取当前播放的时间出错
#define	PAG_PLAYFAIL								(PAG_Plug_EBase + 38)		// 播放出错
#define PAG_FILEFORMAT_ERROR						(PAG_Plug_EBase + 39)		// 文件格式不正确
#define PAG_DIR_ERROR								(PAG_Plug_EBase + 40)		// 路径错误
#define PAG_ALLOC_RESOURCE_ERROR					(PAG_Plug_EBase + 41)		// 资源分配错误
#define PAG_AUDIO_MODE_ERROR						(PAG_Plug_EBase + 42)		// 声卡模式错误
#define PAG_NOENOUGH_BUF							(PAG_Plug_EBase + 43)		// 缓冲区太小
#define PAG_CREATESOCKET_ERROR		 				(PAG_Plug_EBase + 44)		// 创建SOCKET出错
#define PAG_SETSOCKET_ERROR							(PAG_Plug_EBase + 45)		// 设置SOCKET出错
#define PAG_MAX_NUM									(PAG_Plug_EBase + 46)		// 个数达到最大
#define PAG_USERNOTEXIST							(PAG_Plug_EBase + 47)		// 用户不存在
#define PAG_WRITEFLASHERROR							(PAG_Plug_EBase + 48)		// 写FLASH出错
#define PAG_UPGRADEFAIL								(PAG_Plug_EBase + 49)		// DVR升级失败
#define PAG_CARDHAVEINIT							(PAG_Plug_EBase + 50)		// 解码卡已经初始化过
#define PAG_PLAYERFAILED							(PAG_Plug_EBase + 51)		// 调用播放库中某个函数失败
#define PAG_MAX_USERNUM								(PAG_Plug_EBase + 52)		// 设备端用户数达到最大
#define PAG_GETLOCALIPANDMACFAIL					(PAG_Plug_EBase + 53)		// 获得客户端的IP地址或物理地址失败
#define PAG_NOENCODEING								(PAG_Plug_EBase + 54)		// 该通道没有编码
#define PAG_IPMISMATCH								(PAG_Plug_EBase + 55)		// IP地址不匹配
#define PAG_MACMISMATCH								(PAG_Plug_EBase + 56)		// MAC地址不匹配
#define PAG_UPGRADELANGMISMATCH						(PAG_Plug_EBase + 57)		// 升级文件语言不匹配
#define PAG_MAX_PLAYERPORT							(PAG_Plug_EBase + 58)		// 播放器路数达到最大
#define PAG_NOSPACEBACKUP							(PAG_Plug_EBase + 59)		// 备份设备中没有足够空间进行备份
#define PAG_NODEVICEBACKUP							(PAG_Plug_EBase + 60)		// 没有找到指定的备份设备
#define PAG_PICTURE_BITS_ERROR						(PAG_Plug_EBase + 61)		// 图像素位数不符,限24色
#define PAG_PICTURE_DIMENSION_ERROR					(PAG_Plug_EBase + 62)		// 图片高*宽超限,限128*256
#define PAG_PICTURE_SIZ_ERROR						(PAG_Plug_EBase + 63)		// 图片大小超限,限100K
#define PAG_LOADPLAYERSDKFAILED						(PAG_Plug_EBase + 64)		// 载入当前目录下Player Sdk出错
#define PAG_LOADPLAYERSDKPROC_ERROR					(PAG_Plug_EBase + 65)		// 找不到Player Sdk中某个函数入口
#define PAG_LOADDSSDKFAILED							(PAG_Plug_EBase + 66)		// 载入当前目录下DSsdk出错
#define PAG_LOADDSSDKPROC_ERROR						(PAG_Plug_EBase + 67)		// 找不到DsSdk中某个函数入口
#define PAG_DSSDK_ERROR								(PAG_Plug_EBase + 68)		// 调用硬解码库DsSdk中某个函数失败
#define PAG_VOICEMONOPOLIZE							(PAG_Plug_EBase + 69)		// 声卡被独占
#define PAG_JOINMULTICASTFAILED						(PAG_Plug_EBase + 70)		// 加入多播组失败
#define PAG_CREATEDIR_ERROR							(PAG_Plug_EBase + 71)		// 建立日志文件目录失败
#define PAG_BINDSOCKET_ERROR						(PAG_Plug_EBase + 72)		// 绑定套接字失败
#define PAG_SOCKETCLOSE_ERROR						(PAG_Plug_EBase + 73)		// socket连接中断,此错误通常是由于连接中断或目的地不可达
#define PAG_USERID_ISUSING							(PAG_Plug_EBase + 74)		// 注销时用户ID正在进行某操作
#define PAG_SOCKETLISTEN_ERROR						(PAG_Plug_EBase + 75)		// 监听失败
#define PAG_PROGRAM_EXCEPTION						(PAG_Plug_EBase + 76)		// 程序异常
#define PAG_WRITEFILE_FAILED						(PAG_Plug_EBase + 77)		// 写文件失败
#define PAG_FORMAT_READONLY							(PAG_Plug_EBase + 78)		// 禁止格式化只读硬盘
#define PAG_WITHSAMEUSERNAME						(PAG_Plug_EBase + 79)		// 用户配置结构中存在相同的用户名
#define PAG_DEVICETYPE_ERROR						(PAG_Plug_EBase + 80)		// 导入参数时设备型号不匹配
#define PAG_LANGUAGE_ERROR							(PAG_Plug_EBase + 81)		// 导入参数时语言不匹配
#define PAG_PARAVERSION_ERROR						(PAG_Plug_EBase + 82)		// 导入参数时软件版本不匹配
#define PAG_IPCHAN_NOTALIVE							(PAG_Plug_EBase + 83)		// 预览时外接IP通道不在线
#define PAG_RTSP_SDK_ERROR							(PAG_Plug_EBase + 84)		// 加载高清IPC通讯库StreamTransClient.dll失败
#define PAG_CONVERT_SDK_ERROR						(PAG_Plug_EBase + 85)		// 加载转码库失败
#define PAG_IPC_COUNT_OVERFLOW						(PAG_Plug_EBase + 86)		// 超出最大的ip接入通道数
#define PAG_MAX_ADD_NUM								(PAG_Plug_EBase + 87)		// 添加标签(一个文件片段64)等个数达到最大
#define PAG_PARAMMODE_ERROR							(PAG_Plug_EBase + 88)		// 图像增强仪,参数模式错误(用于硬件设置时,客户端进行软件设置时错误值)
#define PAG_CODESPITTER_OFFLINE						(PAG_Plug_EBase + 89)		// 视频综合平台,码分器不在线
#define PAG_BACKUP_COPYING							(PAG_Plug_EBase + 90)		// 设备正在备份
#define PAG_CHAN_NOTSUPPORT							(PAG_Plug_EBase + 91)		// 通道不支持该操作
#define PAG_CALLINEINVALID							(PAG_Plug_EBase + 92)		// 高度线位置太集中或长度线不够倾斜 
#define PAG_CALCANCELCONFLICT						(PAG_Plug_EBase + 93)		// 取消标定冲突,如果设置了规则及全局的实际大小尺寸过滤
#define PAG_CALPOINTOUTRANGE						(PAG_Plug_EBase + 94)		// 标定点超出范围 
#define PAG_FILTERRECTINVALID						(PAG_Plug_EBase + 95)		// 尺寸过滤器不符合要求
#define PAG_DDNS_DEVOFFLINE							(PAG_Plug_EBase + 96)		// 设备没有注册到ddns上
#define PAG_DDNS_INTER_ERROR						(PAG_Plug_EBase + 97)		// DDNS 服务器内部错误
#define PAG_FUNCTION_NOT_SUPPORT_OS					(PAG_Plug_EBase + 98)		// 此功能不支持该操作系统
#define PAG_DEC_CHAN_REBIND							(PAG_Plug_EBase + 99)		// 解码通道绑定显示输出次数受限
#define PAG_ALIAS_DUPLICATE							(PAG_Plug_EBase + 150)		// 别名重复  //2011-08-31 通过别名或者序列号来访问设备的新版本ddns的配置

//STREAM服务相关错误码
#define STREAM_CLIENT_EBase							6000						//< 错误码基数。
#define STREAM_NEW_FAILED							(STREAM_CLIENT_EBase + 1)	// 内存申请失败
#define STREAM_PARSEURL_FAIL						(STREAM_CLIENT_EBase + 2)	// URL解析失败
#define STREAM_CONNECT_VAG_FAIL            			(STREAM_CLIENT_EBase + 3)	// 连接信令网关失败
#define STREAM_RTSPOPEN_FAIL               			(STREAM_CLIENT_EBase + 4)	// RTSP会话建立失败
#define STREAM_RTSPPOST_RECVCB_FAIL        			(STREAM_CLIENT_EBase + 5)	// 投递数据回调失败
#define STREAM_RTSPSEND_OPTION_FAIL        			(STREAM_CLIENT_EBase + 6)	// 发送Option失败
#define STREAM_RTSPSEND_DESCRIBE_FAIL      			(STREAM_CLIENT_EBase + 7)	// 发送DESCRIBE失败
#define STREAM_RTSPSEND_SETUP_FAIL         			(STREAM_CLIENT_EBase + 8)	// 发送setup失败
#define STREAM_RTSPSEND_PLAY_FAIL          			(STREAM_CLIENT_EBase + 9)	// 发送Play失败
#define STREAM_RTSPSEND_PAUSE_FAIL         			(STREAM_CLIENT_EBase + 10)	// 发送PAUSE失败
#define STREAM_RTSPSEND_TEARDOWN_FAIL      			(STREAM_CLIENT_EBase + 11)	// 发送TEARDOWN失败
#define STREAM_PARAIS_NULL                 			(STREAM_CLIENT_EBase + 12)	// 指针参数为空
#define STREAM_RECV_RTSPRSP_TIMEOUT        			(STREAM_CLIENT_EBase + 13)	// 接收RTSP响应失败

/********************VAG_CLIENT错误码定义 end*********************/

/********************设备参数配置命令号 begin*********************/

#define VAG_DEV_CFG_GET_VIDEO_IMG_PARA				1201	// 获取通道视频输入图像参数
#define VAG_DEV_CFG_SET_VIDEO_IMG_PARA				1202	// 设置通道视频输入图像参数

#define VAG_DEV_CFG_GET_MOTION_DETECT_PARA			1301	// 获取通道移动侦测参数
#define VAG_DEV_CFG_SET_MOTION_DETECT_PARA			1302	// 设置通道移动侦测参数

#define VAG_DEV_CFG_GET_HIDE_ALARM_PARA				1305	// 获取通道视频遮挡报警参数
#define VAG_DEV_CFG_SET_HIDE_ALARM_PARA				1306	// 设置通道视频遮挡报警参数

#define VAG_DEV_CFG_GET_VIDEO_LOST_PARA				1309	// 获取通道视频丢失报警参数
#define VAG_DEV_CFG_SET_VIDEO_LOST_PARA				1310	// 设置通道视频丢失报警参数

#define VAG_DEV_CFG_GET_ALARM_IN_PARA				1311	// 获取报警输入通道参数
#define VAG_DEV_CFG_SET_ALARM_IN_PARA				1312	// 设置报警输入通道参数

#define VAG_DEV_CFG_GET_CHAN_OSD_PARA				1313	// 获取视频通道OSD字符参数
#define VAG_DEV_CFG_SET_CHAN_OSD_PARA				1314	// 设置视频通道OSD字符参数

/********************设备参数配置命令号 end***********************/

enum AUDIO_ENCODE_TYPE										// 音频编码类型
{
	AUDIO_ENCODE_G722 = 0,
	AUDIO_ENCODE_G711_U,
	AUDIO_ENCODE_G711_A,
	AUDIO_ENCODE_G726 = 6
};

enum TRANS_TYPE
{
	TRANS_UDP = 0,											// UDP传输方式
	TRANS_TCP												// TCP传输方式
};

enum STREAM_TYPE
{
	STREAM_MAIN = 0,										// 主码流
	STREAM_SUB												// 子码流
};

enum STREAM_OBTAIN_TYPE
{
	STREAM_OBTAIN_INITIATIVE = 0,							// 主动取流方式
	STREAM_OBTAIN_PASSIVE									// 被动获取的推流方式
};

enum DEV_REG_TYPE
{
	DEV_REG_PASSIVE = 0,									// 被动注册设备
	DEV_REG_INITIATIVE = 4									// 主动注册设备
};

enum CAP_IMG_TYPE
{
	CAP_IMG_JPEG = 0										// Jpeg抓图格式
};

/********************视频数据类型 begin*********************/
#define VAG_SYSHEAD									1		// 系统头数据
#define VAG_STREAMDATA								2		// 视频流数据（包括复合流和音视频分开的视频流数据）
#define VAG_AUDIOSTREAMDATA							3		// 音频流数据
#define VAG_STD_VIDEODATA							4		// 标准视频流数据
#define VAG_STD_AUDIODATA							5		// 标准音频流数据

/********************视频数据类型 end*********************/

/*****************云台控制命令 begin******************/  
#define PTZ_CMD_LIGHT_PWRON							2		/* 接通灯光电源 */
#define PTZ_CMD_WIPER_PWRON							3		/* 接通雨刷开关 */
#define PTZ_CMD_FAN_PWRON							4		/* 接通风扇开关 */
#define PTZ_CMD_HEATER_PWRON						5		/* 接通加热器开关 */
#define PTZ_CMD_AUX_PWRON1							6		/* 接通辅助设备开关 */
#define PTZ_CMD_AUX_PWRON2							7		/* 接通辅助设备开关 */

#define PTZ_CMD_ZOOM_IN								11		/* 焦距以速度SS变大(倍率变大) */
#define PTZ_CMD_ZOOM_OUT							12		/* 焦距以速度SS变小(倍率变小) */
#define PTZ_CMD_FOCUS_NEAR							13		/* 焦点以速度SS前调 */
#define PTZ_CMD_FOCUS_FAR							14		/* 焦点以速度SS后调 */
#define PTZ_CMD_IRIS_OPEN							15		/* 光圈以速度SS扩大 */
#define PTZ_CMD_IRIS_CLOSE							16		/* 光圈以速度SS缩小 */

#define PTZ_CMD_TILT_UP								21		/* 云台以SS的速度上仰 */
#define PTZ_CMD_TILT_DOWN							22		/* 云台以SS的速度下俯 */
#define PTZ_CMD_PAN_LEFT							23		/* 云台以SS的速度左转 */
#define PTZ_CMD_PAN_RIGHT							24		/* 云台以SS的速度右转 */
#define PTZ_CMD_UP_LEFT								25		/* 云台以SS的速度上仰和左转 */
#define PTZ_CMD_UP_RIGHT							26		/* 云台以SS的速度上仰和右转 */
#define PTZ_CMD_DOWN_LEFT							27		/* 云台以SS的速度下俯和左转 */
#define PTZ_CMD_DOWN_RIGHT							28		/* 云台以SS的速度下俯和右转 */
#define PTZ_CMD_PAN_AUTO							29		/* 云台以SS的速度左右自动扫描 */

#define PRESET_CMD_SET_PRESET						8		/* 设置预置点 */
#define PRESET_CMD_CLE_PRESET						9		/* 清除预置点 */
#define PRESET_CMD_GOTO_PRESET						39		/* 快球转到预置点 */

#define CRUISE_CMD_FILL_PRE_SEQ						30		/* 将预置点加入巡航序列 */
#define CRUISE_CMD_SET_SEQ_DWELL					31		/* 设置巡航点停顿时间 */
#define CRUISE_CMD_SET_SEQ_SPEED					32		/* 设置巡航速度 */
#define CRUISE_CMD_CLE_PRE_SEQ						33		/* 将预置点从巡航序列中删除 */
#define CRUISE_CMD_RUN_SEQ							37		/* 开始巡航 */
#define CRUISE_CMD_STOP_SEQ							38		/* 停止巡航 */
#define CRUISE_CMD_GET_ROUTE						104		/* 获取巡航路径 */

#define TRACK_CMD_STA_MEM_CRUISE					34		/* 开始记录轨迹 */
#define TRACK_CMD_STO_MEM_CRUISE					35		/* 停止记录轨迹 */
#define TRACK_CMD_RUN_CRUISE						36		/* 开始轨迹 */

#define PTZ_CMD_ZERO_NEXT							40		/* 零通道下一页 */
#define PTZ_CMD_ZERO_PRE							41		/* 零通道上一页 */

#define PTZ_CMD_SEL_ZOOM							99		/* 电子放大 */
#define PTZ_CMD_CAMERA_LOCK							200		/* 云台锁定 */
#define PTZ_CMD_CAMERA_UNLOCK						201		/* 解除云台锁定 */
#define PTZ_CAMERA_UNLOCK_ALL						202		/* 解除所有该优先级能够解除的锁定 */
#define PTZ_CAMERA_LOCK_CHECK						205		/* 锁定检查 */
#define PTZ_CMD_FOCUS_ONEPUSH						206		/* 辅助聚焦 */

/*****************云台控制命令 end******************/  
#define PTZ_ACTION_START							0		// 云台控制开始
#define PTZ_ACTION_STOP								1		// 云台控制停止

typedef struct
{
	unsigned int nYear;
	unsigned int nMonth;
	unsigned int nDay;
	unsigned int nHour;
	unsigned int nMinute;
	unsigned int nSecond;
	unsigned int nMsecond;
}VAG_TIME_INFO, *LPVAG_TIME_INFO;

typedef struct
{
	char* pszDataBuf;
	int iDataLen;
	int iPayload;
	bool bMark;
	unsigned int nTimestamp;
}VAG_MEDIA_PACKET, *LPVAG_MEDIA_PACKET;

/**	@fn	int VAG_fMsgCallBack
 *	@brief	<消息回调>
 *	@param iClientHandle 标识句柄,VAG_Login返回的句柄.
 *	@param iMsgType 消息类型.
 *	@param pMsgPtr 消息数据指针.
 *	@param iMsgSize 消息数据长度.
 *	@param pUsrData 用户数据.
 */
typedef int (CALLBACK* VAG_fMsgCallBack)(int iClientHandle, int iMsgType, char* pMsgPtr, int iMsgSize, void* pUsrData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<媒体数据回调>
 *	@param iDataType 媒体数据类型
 *	@param pData 媒体数据指针
 *	@param iDataSize 媒体数据长度
 *	@param pUser 用户自定义数据
 */
typedef void(CALLBACK* VAG_fStreamDataCallBack)(int iDataType, void* pData, int iDataSize, void* pUser);

/**	@fn	int VAG_fStreamDataCallBack
*	@brief	<媒体数据回调>
*	@param iStreamChannel 媒体通道
 *	@param iDataType 媒体数据类型
 *	@param pstData 媒体数据结构体
 *	@param pUser 用户自定义数据
 */
typedef void (CALLBACK *VAG_fStreamDataCallBackEx)(int iStreamChannel, int iDataType, LPVAG_MEDIA_PACKET pstData, void* pUserData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<媒体消息回调>
 *	@param iMsgType 消息类型
 *	@param pMsgPtr 消息结构
 *	@param pUser 用户自定义数据
 */
typedef int (CALLBACK *VAG_fStreamMsgCallBack)(int iMsgType, void* pMsgPtr, void* pUserData);

typedef struct
{
	char szPagURL[VAG_IPV4_LEN];							// 登陆VAG的Ip
	char szUserName[VAG_USER_NAME_LEN];						// 登陆用户名
	char szPassWord[VAG_PASSWORD_LEN];						// 登陆密码
	int iPort;												// 登陆端口
	int	iLoginType;											// 登陆类型(暂不使用)
	VAG_fMsgCallBack fMsgCallBack;							// 消息回调函数,所有结果从该回调返回
	void* pMsgUserData;										// 消息回调用户参数
	int iRes[36];
}VAG_LOGIN_PARA, *LPVAG_LOGIN_PARA;

typedef struct
{
	char szIp[VAG_IPV4_LEN];								// VAG媒体网关Ip
	int iPort;												// VAG媒体网关端口
	int iRes[36];
}VAG_MEDIA_INFO, *LPVAG_MEDIA_INFO;

typedef struct
{
	char szIp[VAG_IPV4_LEN];								// 传输Ip地址
	int iPort;												// 传输端口
	TRANS_TYPE eTransType;									// 传输方式
	int iRes[36];
}VAG_TRANS_PARA, *LPVAG_TRANS_PARA;

typedef struct
{
	char szResourceKey[VAG_INDEXCODE_LEN];					// 查询资源的字符串键值
	int iResourceKey;										// 查询资源的整型键值
	int iResourceType;										// 查询资源类型
	int iRes[36];
}VAG_QUERY_RES_PARA, *LPVAG_QUERY_RES_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 预览的通道索引号
	char szUserAgent[VAG_USERAGENT_LEN];					// 预览取流客户端类型
	STREAM_OBTAIN_TYPE eStreamObtainType;					// 取流模式,(被动收流模式需要传入参数stTransPara)
															// 主动取流模式下通过回调传回视频数据,或通过窗口句柄解码播放
	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBack cbStreamData;					// 取流数据回调函数,为NULL表示不获取视频数据
	VAG_fStreamMsgCallBack cbMsgData;						// 取流消息回调函数
	void* pUser;											// 视频流数据回调用户参数
	TRANS_TYPE eTransType;									// SDK到VAG取流传输方式(被动取流方式下无效)
	STREAM_TYPE eStreamType;								// 取流码流类型
	VAG_TRANS_PARA stTransPara;								// 推流模式下,接收媒体流的地址信息
    char szUrlExt[VAG_URL_EXT_LEN];				           	// 扩展URL字符串,如果用来进行token验证时，传"ext:token=..."或者""（白名单验证）;"ext:"字符串头。
                                                            // 不使用时传""。
    int iRes[4];											// iRes[0]的最高位(31)为0表示由VAG配置是否转流,为1表示自定义是否转流
															// 第30位表示自定义是否转流方式:0--不过VAG转流;1--过VAG转流 
}VAG_OPEN_STREAM_PARA, *LPVAG_OPEN_STREAM_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 预览的通道索引号
	char szUserAgent[VAG_USERAGENT_LEN];					// 预览取流客户端类型
	STREAM_OBTAIN_TYPE eStreamObtainType;					// 取流模式,(被动收流模式需要传入参数stTransPara)
															// 主动取流模式下通过回调传回视频数据,或通过窗口句柄解码播放
	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBackEx cbStreamData;					// 取流数据回调函数,为NULL表示不获取视频数据
	VAG_fStreamMsgCallBack cbMsgData;						// 取流消息回调函数
	void* pUser;											// 视频流数据回调用户参数
	TRANS_TYPE eTransType;									// SDK到VAG取流传输方式(被动取流方式下无效)
	STREAM_TYPE eStreamType;								// 取流码流类型
	VAG_TRANS_PARA stTransPara;								// 推流模式下,接收媒体流的地址信息
    char szUrlExt[VAG_URL_EXT_LEN];				           	// 扩展URL字符串,如果用来进行token验证时，传"ext:token=..."或者""（白名单验证）;"ext:"字符串头。
                                                            // 不使用时传""。
	int iRes[4];											// iRes[0]的最高位(31)为0表示由VAG配置是否转流,为1表示自定义是否转流
															// 第30位表示自定义是否转流方式:0--不过VAG转流;1--过VAG转流 
}VAG_OPEN_STREAM_PARAEX, *LPVAG_OPEN_STREAM_PARAEX;

typedef struct
{
	char szDevIp[VAG_IPV4_LEN];								// 取流的设备IP
	int iPort;												// 取流的设备端口
	int iChannel;											// 取流的通道号
	char szUserName[VAG_USER_NAME_LEN];						// 取流设备的用户名
	char szPassword[VAG_PASSWORD_LEN];						// 取流设备的密码
	char szIndexCode[VAG_INDEXCODE_LEN];					// 取流设备的索引号
	int iDeviceType;										// 取流设备的类型号

	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBack cbStreamData;					// 取流数据回调函数,为NULL表示不获取视频数据
	void* pUser;											// 视频流数据回调用户参数
	TRANS_TYPE eTransType;									// 取流传输方式
	STREAM_TYPE eStreamType;								// 取流码流类型
    char szUrlExt[VAG_URL_EXT_LEN];				           	// 扩展URL字符串,如果用来进行token验证时，传"ext:token=..."或者""（白名单验证）;"ext:"字符串头。
                                                            // 不使用时传""。
    int iRes[4];
}VAG_DIRECT_OPEN_STREAM_PARA, *LPVAG_DIRECT_OPEN_STREAM_PARA;

typedef struct
{
	char szDevIp[VAG_IPV4_LEN];								// 取流的设备IP
	int iPort;												// 取流的设备端口
	int iChannel;											// 取流的通道号
	char szUserName[VAG_USER_NAME_LEN];						// 取流设备的用户名
	char szPassword[VAG_PASSWORD_LEN];						// 取流设备的密码
	char szIndexCode[VAG_INDEXCODE_LEN];					// 取流设备的索引号
	int iDeviceType;										// 取流设备的类型号

	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBackEx cbStreamData;					// 取流数据回调函数,为NULL表示不获取视频数据
	void* pUser;											// 视频流数据回调用户参数
	TRANS_TYPE eTransType;									// 取流传输方式
	STREAM_TYPE eStreamType;								// 取流码流类型
    char szUrlExt[VAG_URL_EXT_LEN];				           	// 扩展URL字符串,如果用来进行token验证时，传"ext:token=..."或者""（白名单验证）;"ext:"字符串头。
                                                            // 不使用时传""。
    int iRes[4];
}VAG_DIRECT_OPEN_STREAM_PARAEX, *LPVAG_DIRECT_OPEN_STREAM_PARAEX;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 云台控制的通道索引号
	int iPtzCommand;										// 云台控制的命令号
	int iAction;											// 云台控制的开始或停止
	int iIndex;												// 当iPtzCommand为8,9,39时,为预置位序号;iPtzCommand为37,38时,为巡航轨迹编号
	int iSpeed;												// 云台控制的速度
	int iPriority;											// 云台控制的权限值
	int iUserId;											// 云台控制的用户Id(可选)
	int iMatrixCameraId;									// 矩阵编号
	int iMonitorId;											// 监视屏编号
	int iLockTime;											// 云台锁定时间
	int iPtzCruisePoint;									// 巡航点编号
	int iPtzCruiseInput;									// 不同巡航命令时的值不同，预置点(最大255)、时间(最大255)、速度(最大40)
	int iRes[36];
}VAG_PTZ_CONTROL_PARA, *LPVAG_PTZ_CONTROL_PARA;

typedef struct
{
	int nPtzPresetIndex;									// 云台预置点编号
	int nDwellTime;											// 云台预置点停留时间
	int nPtzSpeed;											// 云台预置点运行速度
	int iRes[36];
}VAG_PTZ_CFG_POINT_INFO, *LPVAG_PTZ_CFG_POINT_INFO;

typedef struct
{
	int iPtzCfgPointCount;									// 云台配置预置点数
	VAG_PTZ_CONTROL_PARA stPtzControlPara;					// 云台配置基本参数
	VAG_PTZ_CFG_POINT_INFO pstPtzCfgPoint[VAG_PTZ_POIT_NUM];// 云台配置预置点参数
}VAG_PTZ_CFG_INFO, *LPVAG_PTZ_CFG_INFO;

typedef struct
{
	char szLocalIp[VAG_IPV4_LEN];							// Ip
	int iRes[36];
}VAG_BIND_LOCALIP_INFO, *LPVAG_BIND_LOCALIP_INFO;

typedef struct
{
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// 设备分组编号
	char szGroupName[VAG_NAME_LEN];							// 设备名称
	char szParentGroup[VAG_INDEXCODE_LEN];					// 上级分组编号
	int iRes[36];
}VAG_GROUP_INFO, *LPVAG_GROUP_INFO;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	char szDeviceName[VAG_NAME_LEN];						// 设备名称
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// 设备所属分组编号
	char szDeviceAddr[VAG_IPV4_LEN];						// 设备Ip
	char szUserName[VAG_USER_NAME_LEN];						// 设备用户名
	char szPassWord[VAG_PASSWORD_LEN];						// 设备密码
	char szDeviceMulticast[VAG_IPV4_LEN];					// 设备多播Ip
	int iPort;												// 设备端口
	int	iDevType;											// 设备类型
	int	iDevRegType;										// 设备注册类型
	int	iVoiceType;											// 设备语音类型
	int iRes[36];
}VAG_DEVICE_INFO, *LPVAG_DEVICE_INFO;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	char szCamName[VAG_NAME_LEN];							// 视频通道名称
	int	iStreamTransform;									// 是否配置了转码
	int	iVagTransmit;										// 是否通过Vag转流
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 视频通道所属设备编号
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// 设备所属分组编号
	int iChanNum;											// 通道编号
	int iRes[36];
}VAG_CAMERA_INFO, *LPVAG_CAMERA_INFO;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	char szCamName[VAG_NAME_LEN];							// 视频通道名称
	int	iStreamTransform;									// 是否配置了转码
	int	iVagTransmit;										// 是否通过Vag转流
	int iChanNum;											// 通道编号
	int iStreamTransType;									// 通道支持的取流方式:0-UDP;1-TCP;2-UDP/TCP
	VAG_DEVICE_INFO stDeviceInfo;							// 通道所在的设备信息
	int iRes[36];
}VAG_CAMERA_DEVICE_INFO, *LPVAG_CAMERA_DEVICE_INFO;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	int iChanNum;											// 对讲通道编号
	VAG_fStreamDataCallBack cbStreamData;					// 语音数据回调函数
	void* pUser;											// 语音数据回调用户参数
	int iRes[36];
}VAG_DEVICE_TALK_INFO, *LPVAG_DEVICE_TALK_INFO;

typedef struct
{
	int iTalkHandle;										// VAG_StartDeviceTalk返回的设备对讲句柄
	int iDataSize;											// 设备对讲数据长度
	void* pData;											// 设备对讲数据
	int iRes[36];
}VAG_DEVICE_TALK_DATA, *LPVAG_DEVICE_TALK_DATA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	char szEventPubUrl[VAG_EVENT_PUB_URL_LEN];				// 事件发布url
	int iRes[36];
}VAG_SUBSCRIBE_EVENT_PARA, *LPVAG_SUBSCRIBE_EVENT_PARA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	char szGpsPubUrl[VAG_GPS_PUB_URL_LEN];		    		// GPS发布url
	int iRes[36];
}VAG_SUBSCRIBE_GPS_PARA, *LPVAG_SUBSCRIBE_GPS_PARA;

typedef struct
{
    char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
    int iRes[36];
}VAG_DESUBSCRIBE_GPS_PARA, *LPVAG_DESUBSCRIBE_GPS_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	CAP_IMG_TYPE eCaptureImgType;							// 抓图格式
	int iImgSize;											// 图片尺寸:0-CIF(352*288/352*240),1-QCIF(176*144/176*120),2-4CIF(704*576/704*480)或D1(720*576/720*486),3-UXGA(1600*1200)
															// ,4-SVGA(800*600),5-HD720P(1280*720),6-VGA(640*480),7-XVGA(1280*960),8-HD900P(1600*900),9-HD1080P(1920*1080),10-2560*1920
															// ,11-1600*304,12-2048*1536,13-2448*2048,14-2448*1200,15-2448*800,16-XGA(1024*768),17-SXGA(1280*1024),18-WD1(960*576/960*480)
															// ,19-1080I(1920*1080),20-576*576,21-1536*1536,22-1920*1920,0xff-Auto(使用当前码流分辨率)
	int iImgQuality;										// 图片质量系数:0-最好,1-较好,2-一般
	int iRes[36];
}VAG_CAPTURE_PARA, *LPVAG_CAPTURE_PARA;

typedef struct
{
	char szIndexCode[VAG_INDEXCODE_LEN];					// IO所在设备编号
	int iIoNum;												// IO通道号
	int iStatus;											// IO状态:0-关闭,1-打开
	int iDelayTime;											// IO状态延时(单位:秒)
	int iRes[36];
}VAG_IO_SET_PARA, *LPVAG_IO_SET_PARA;

typedef struct
{
	char szIndexCode[VAG_INDEXCODE_LEN];					// IO所在设备编号
	int iIoNum;												// IO通道号
	int iRes[36];
}VAG_IO_GET_PARA, *LPVAG_IO_GET_PARA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	int iRes[36];
}VAG_DESUBSCRIBE_EVENT_PARA, *LPVAG_DESUBSCRIBE_EVENT_PARA;

typedef struct												// 获取,设置录像参数
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	int iRes[36];
}VAG_RECORD_CONFIG_PARA, *LPVAG_RECORD_CONFIG_PARA;

typedef struct
{
	int iAllDayRecord;										// 是否全天录像:0-否;1-是
	char chRecordType;										// 录象类型
	int iRes[36];
}VAG_RECORD_DAY_INFO, *LPVAG_RECORD_DAY_INFO;

typedef struct
{
	int iRecord;											// 是否录像: 0-否;1-是
	int iRecordTime;										// 结束时延长录象时间:0-5秒;1-10秒;2-30秒;3-1分;4-2分;5-5分;6-10分
	int iPreRecordTime;										// 预录时间:0-不预录;1-5秒;2-10秒;3-15秒;4-20秒;5-25秒;6-30秒;7-尽可能预录
	int iRecorderDuration;									// 录像保存最长时间:等于0代表录像不要删除;大于0代表保存的天数
	int iRedundancyRec;										// 是否冗余录像,数据双备份:0-否;1-是
	int iAudioRec;											// 录像时复合流编码时是否记录音频数据:0-不记录;1-记录
	VAG_RECORD_DAY_INFO stRecAllDay[VAG_MAX_DAYS];			// 全天录像参数
	char chRecPlan[7][48];									// 0 表示不录象,其他值表示录象类型
															// 0-不录像;1-定时录像;2-移动侦测录像;3-动测|报警;4-报警触发录像;5-动测&报警;6-命令触发;
															// 7-智能录像;8-手动录像;9-震动报警;A-环境报警;B-PIR报警;C-无线报警;D-呼救报警;E-所有报警;
															// F-智能交通事件;G-区域入侵侦测;H-音频异常侦测
	char szRes[256];
	int iRes[36];
}VAG_RECORD_CONFIG_INFO, *LPVAG_RECORD_CONFIG_INFO;

typedef struct												// 查询录像文件参数
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	int iRecFileType;										// 查询录像文件类型
															// 0xff-所有录像;1-定时录像;2-移动侦测录像;3-动测|报警;4-报警触发录像;5-动测&报警;6-命令触发;
															// 7-智能录像;8-手动录像;9-震动报警;10-环境报警;11-PIR报警;12-无线报警;13-呼救报警;14-所有报警;
															// 15-智能交通事件;16-区域入侵侦测;17-音频异常侦测
	VAG_TIME_INFO stStartTime;								// 查询开始时间
	VAG_TIME_INFO stEndTime;								// 查询结束时间
	char szRes[256];
	int iRes[36];
}VAG_FIND_RECORD_PARA, *LPVAG_FIND_RECORD_PARA;

typedef struct												// 录像文件信息
{
	char szFileName[VAG_NAME_LEN];							// 录像文件名
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// 录像回放URL
	unsigned int iFileSize;									// 录像文件大小
	int iRecType;											// 录像类型
															// 1-定时录像;2-移动侦测录像;3-动测|报警;4-报警触发录像;5-动测&报警;6-命令触发;
															// 7-智能录像;8-手动录像;9-震动报警;10-环境报警;11-PIR报警;12-无线报警;13-呼救报警;14-所有报警;
															// 15-智能交通事件;16-区域入侵侦测;17-音频异常侦测
	VAG_TIME_INFO stStartTime;								// 录像文件开始时间
	VAG_TIME_INFO stStopTime;								// 录像文件结束时间
	char szRes[256];
	int iRes[36];
}VAG_RECORD_FILE_INFO, *LPVAG_RECORD_FILE_INFO;

typedef struct
{
	bool bDownload;
	bool bRelSeek;
	int iRelFrom;
	int iRelTo;
	bool bAbsSeek;
	VAG_TIME_INFO stAbsFrom;
	VAG_TIME_INFO stAbsTo;
	bool bScale;
	double fScale;
	int iRes[32];
}VAG_PLAY_SET_PARA, *LPVAG_PLAY_SET_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 预览的通道索引号
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// 录像回放URL
	char szUserAgent[VAG_USERAGENT_LEN];					// 预览取流客户端类型
	STREAM_OBTAIN_TYPE eStreamObtainType;					// 取流模式,(被动收流模式需要传入参数stTransPara)
															// 主动取流模式下通过回调传回视频数据
	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBackEx cbStreamData;					// 取流数据回调函数,为NULL表示不获取视频数据
	VAG_fStreamMsgCallBack cbMsgData;						// 取流消息回调函数
	void* pUserData;										// 视频流数据回调用户参数
	VAG_PLAY_SET_PARA stPlaybackPara;						// 回放参数
	TRANS_TYPE eTransType;									// SDK到VAG取流传输方式(被动取流方式下无效)
	STREAM_TYPE eStreamType;								// 取流码流类型
	VAG_TRANS_PARA stTransPara;								// 推流模式下,接收媒体流的地址信息
    char szUrlExt[VAG_URL_EXT_LEN];				           	// 扩展URL字符串,如果用来进行token验证时，传"ext:token=..."或者""（白名单验证）;"ext:"字符串头。
                                                            // 不使用时传""。
	int iRes[4];											// iRes[0]的最高位(31)为0表示由VAG配置是否转流,为1表示自定义是否转流
															// 第30位表示自定义是否转流方式:0--不过VAG转流;1--过VAG转流 
}VAG_PLAY_BACK_PARA, *LPVAG_PLAY_BACK_PARA;

typedef struct
{
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// 录像回放URL
	char szDevIp[VAG_IPV4_LEN];								// 取流的设备IP
	int iPort;												// 取流的设备端口
	int iChannel;											// 取流的通道号
	char szUserName[VAG_USER_NAME_LEN];						// 取流设备的用户名
	char szPassword[VAG_PASSWORD_LEN];						// 取流设备的密码
	char szIndexCode[VAG_INDEXCODE_LEN];					// 取流设备的索引号
	int iDeviceType;										// 取流设备的类型号

	HWND hPlayWnd;											// 解码预览的窗口句柄
	VAG_fStreamDataCallBackEx cbStreamDataEx;				// 取流数据回调函数,为NULL表示不获取视频数据
	void* pUser;											// 视频流数据回调用户参数
	TRANS_TYPE eTransType;									// 取流传输方式
	STREAM_TYPE eStreamType;								// 取流码流类型
	int iDownload;											// 0:不下载;1:下载
	VAG_TIME_INFO stAbsFrom;
	VAG_TIME_INFO stAbsTo;
	void* pReservePtr;
	int iReserveLen;
}VAG_DIRECT_PLAY_BACK_PARA, *LPVAG_DIRECT_PLAY_BACK_PARA;

typedef struct
{
	int iCommand;											// 参数配置命令号
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// 设备编号
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// 视频通道编号
	int iRes[36];
}VAG_DEV_CONFIG_PARA, *LPVAG_DEV_CONFIG_PARA;

typedef struct
{
	int iResourceType;										// 资源类型
	char szIndexCode[VAG_INDEXCODE_LEN];					// 资源编号
	char szAbilityCommand[VAG_USER_NAME_LEN];				// 获取能力集命令号
	int iAbilityVersion;									// 能力集版本
	char* pExtParams;										// 扩展输入参数
	int	iExtParamsLen;										// 扩展输入参数长度
	int iRes[36];
}VAG_DEV_ABILITY_PARA, *LPVAG_DEV_ABILITY_PARA;

typedef struct
{
	int iHue;												// 色调[1,10]
	int iContrast;											// 对比度[1,10]
	int iBright;											// 亮度[1,10]
	int iSaturation;										// 饱和度[1,10]
	int iRes[36];
}VAG_CFG_VIDEO_IMAGE_PARA, *LPVAG_CFG_VIDEO_IMAGE_PARA;

typedef struct
{
	int iHue;												// 色调[1,10]
	int iContrast;											// 对比度[1,10]
	int iBright;											// 亮度[1,10]
	int iSaturation;										// 饱和度[1,10]
	int iRes[36];
}VAG_CFG_DEV_IMAGE_PARA, *LPVAG_CFG_DEV_IMAGE_PARA;

typedef struct
{
	int iIsUse;												// 是否启用移动侦测报警:0-不启用;1-启用
	int iSensitive;											// 移动侦测灵敏度:[0,5],越高越灵敏
	int iRes[36];
}VAG_CFG_DEV_MOTION_DETECT_PARA, *LPVAG_CFG_DEV_MOTION_DETECT_PARA;

typedef struct
{
	int iIsUse;												// 是否启用视频遮挡报警:0-不启用;1-启用
	int iSensitive;											// 视频遮挡灵敏度:[1,3],越高越灵敏
	int iRes[36];
}VAG_CFG_DEV_HIDE_ALARM_PARA, *LPVAG_CFG_DEV_HIDE_ALARM_PARA;

typedef struct
{
	int iIsUse;												// 是否启用视频丢失报警:0-不启用;1-启用
	int iRes[36];
}VAG_CFG_DEV_VIDEO_LOST_PARA, *LPVAG_CFG_DEV_VIDEO_LOST_PARA;

typedef struct
{
	int iIsUse;												// 是否启用视频丢失报警:0-不启用;1-启用
	int iAlarminType;										// 报警输入通道类型:0-常开;1-常闭
	char chAlarminName[VAG_NAME_LEN];
	int iRes[36];
}VAG_CFG_DEV_ALARMIN_PARA, *LPVAG_CFG_DEV_ALARMIN_PARA;

typedef struct
{
	char szOsdString[VAG_CHAN_OSD_MAX_LEN];					// 视频通道OSD字符
	int iIsShowString;										// 是否叠加视频通道OSD字符
	int iXPos;												// 视频通道OSD字符X位置
	int iYPos;												// 视频通道OSD字符Y位置
	int iRes[36];
}VAG_CFG_DEV_OSD_STRING, *LPVAG_CFG_DEV_OSD_STRING;

typedef struct
{
	int iStringNum;											// 视频通道OSD字符个数
	VAG_CFG_DEV_OSD_STRING pstShowStringPara[VAG_CHAN_OSD_MAX_NUM];
	int iRes[36];
}VAG_CFG_DEV_OSD_STRING_PARA, *LPVAG_CFG_DEV_OSD_STRING_PARA;

typedef struct
{
    VAG_CAPTURE_PARA sCapturePara;                          ///< 抓图参数
    char chPicServerUrl[VAG_PICSERVER_URL_LEN];             ///< 图片服务器RUL,如：ftp://10.64.51.26:21/admin/12345/
}VAG_PICTURE2SERVER_PARA,*LPVAG_PICTURE2SERVER_PARA;
#endif
