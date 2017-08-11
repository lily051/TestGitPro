/** @file   hcs_sdk_define.h
 *  @note   Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief  
 *
 *  @author Jin Shengpeng
 *  @date   2015/01/28
 *
 *  @note   历史记录：
 *  @note   V1.0.0.0 创建
 */

#ifndef HCS_SDK_DEFINE_H_
#define HCS_SDK_DEFINE_H_

#if (defined(_WIN32) || defined(_WIN64))
#  if defined(HCS_SDK_EXPORTS)
#    define HCS_SDK_EXTERN extern "C" __declspec(dllexport)
#  else
#    define HCS_SDK_EXTERN extern "C" __declspec(dllimport)
#  endif
#  define HCS_SDK_API
#elif defined(__linux__)
#  define HCS_SDK_EXTERN extern "C"
#  define HCS_SDK_API
#else
#  define HCS_SDK_EXTERN
#  define HCS_SDK_API
#endif


#define HCS_OK									0		// 成功
#define HCS_ERROR								1		// 失败
#define HCS_DEVICE_IP_LEN						256		// 定义前端设备的IP地址信息长度
#define HCS_IP_LEN								40		// 定义IP地址信息长度
#define HCS_FILE_KEY_LEN						260		// 视频文件key长度
#define HCS_RESERVED_DATA_LEN					64		// 保留数据最大长度
#define HCS_CAMERA_ID_LEN						64		// 编码器ID长度
#define HCS_RECORD_TYPE_LEN						32		// 最大的录像类型长度
#define HCS_CAMERA_NAME_LEN						64		// 最大的编码器名称长度
#define HCS_DEV_NAME_LEN						64		// 设备名称最大长度
#define HCS_USERNAME_LEN						32		// 用户名的最大长度
#define HCS_POOLUSER_COUNT						60		// 资源池绑定的最多用户个数
#define HCS_PASSWORD_LEN						32		// 密码的最大长度
#define HCS_DAYS_PER_WEEK						7		// 云存储录像计划支持的最大天数
#define HCS_PLAN_SEGMENT_COUNT					8		// 云存储录像计划支持的最大段数
#define HCS_STREAM_URL_LEN						512		// 最大取流URL的长度
#define HCS_POOL_ID_LEN							32		// 最大资源池ID长度
#define HCS_POOL_NAME_LEN						64		// 最大资源池名长度
#define HCS_STORDEV_ID_LEN						64		// 存储设备ID长度
#define HCS_MAX_ABILITY							100		// 能力集最大数目
#define HCS_MAX_REPLICATION_NUM					2		// 最大冗余数目
#define HCS_IP_PORT_LEN							64		// IP地址加端口号最大长度
#define HCS_DESC_LEN							16		// 定义云存储支持的设备类型描述信息最大长度
#define HCS_SUP_DEV_NUM							500		// 定义云存储支持设备厂商最大数量
#define HCS_HEAD_LEN							64		// 最大视频头长度
#define HCS_STREAM_INFO_SIZE					1472	// 取流信息UNION长度
#define HCS_PICTURE_URL_LEN						256		// 最大的图片URL长度
#define HCS_DATA_HOURS							24		// 一天24小时
#define HCS_MONTH_DAYS							31		// 一个月最多31天
#define HCS_TAG_LEN								128		// 标注最大长度
#define HCS_MAX_RECPLAN_ABILITY					8		// 录像计划能力集最大数目
#define HCS_MAX_FILE_URL_LEN					1024    // 最大视频文件URL长度

#define HCS_MAX_VIDEO_KEY_LEN					1024	// 数据最大读取key长度


/** @ enum  [HCS_AUX_SERVICE]
 *  @ brief 设备接入时使用的附加服务，如流媒体、VAG等
 */
typedef enum
{
	VTDU	= 0,										// 流媒体服务
	VAG		= 1											// 设备接入服务

}HCS_AUX_SERVICE;


/** @ enum  [HCS_RecDataType]
 *  @ brief 录像数据类型
 */
typedef enum
{
	HCS_RECDATA_REALTIME	= 1,						// 实时流
	HCS_RECDATA_HISTORICAL	= 2							// 历史流

}HCS_RecDataType;

/** @ enum  [HCS_DataType]
 *  @ brief 数据类型
 */
typedef enum
{
	HCS_DATATYPE_RECORD	= 1,							// 录像数据
	HCS_DATATYPE_STREAMFILE	= 2							// 视频文件数据

}HCS_DataType;


/** @ enum  [HCS_IFrameType]
 *  @ brief I帧类型
 */
typedef enum 
{
	HCS_IFRAME_REAL	= 0,								// 真实I帧
	HCS_IFRAME_FAKE	= 1,								// 自定义I帧
	HCS_IFRAME_SMART264	= 2								// smart 264流

}HCS_IFrameType;


/** @ enum  [HCS_FrameType]
 *  @ brief 帧类型
 */
typedef enum
{
	HCS_FRAME_TYPE_NORMAL = 1,							// 普通帧
	HCS_FRAME_TYPE_REFRESH = 2,							// 刷新帧
	HCS_FRAME_TYPE_IFRAME = 3							// 关键帧(背景帧)

}HCS_FrameType;


/** @ enum  [HCS_PlaybackMode]
 *  @ brief 回放模式
 */
typedef enum
{
	HCS_PBMODE_NORMAL	= 1,							// 顺序回放
	HCS_PBMODE_IFRAME	= 2,							// 正常I帧回放
	HCS_PBMODE_REVERSE	= 3								// 倒序回放

}HCS_PlaybackMode;


/** @ enum  [HCS_SRLockType]
 *  @ brief 锁定类型
 */
typedef enum
{
	HCS_SRLOCK_TYPE_All      = 0,						// 全部（包括锁定的录像和没有锁定的录像）
	HCS_SRLOCK_TYPE_UNLOCKED = 1,						// 没有锁定(可以对当前录像执行所有操作)
	HCS_SRLOCK_TYPE_LOCKED   = 2						// 锁定(当前录像不允许覆盖，但可以回放、下载和删除操作)

}HCS_SRLockType;


/** @ enum  [HCS_PicType]
 *  @ brief 图片类型
 */
typedef enum
{
	HCS_PICTYPE_JPG = 1,								// JPG图片
	HCS_PICTYPE_BMP = 2,								// BMP图片
	HCS_PICTYPE_PNG = 3									// PNG图片

}HCS_PicType;


/** @ enum  [HCS_PicZoomType]
 *  @ brief 图片缩放类型
 */
typedef enum
{
	HCS_PICZOOMTYPE_NONE	= 0,						// 不缩放
	HCS_PICZOOMTYPE_BYSCALE = 1,						// 按比例缩放
	HCS_PICZOOMTYPE_BYSIZE  = 2,						// 按大小缩放
	HCS_PICZOOMTYPE_BYLW	= 3							// 按长宽缩放

}HCS_PicZoomType;

/* @ struct [HCS_Time]
 * @ brief  时间结构参数
 */
typedef struct
{
	unsigned int year;									// 年
	unsigned int month;									// 月
	unsigned int day;									// 日
	unsigned int hour;									// 时
	unsigned int minute;								// 分
	unsigned int second;								// 秒
	unsigned int millisecond;							// 毫秒
	char		 reserved[8];							// 保留字节

}HCS_Time;

/** @ struct [HCS_Version]
 *  @ brief 云存储版本信息
 */
typedef struct 
{
	int		major_version;								// 主版本号
	int		sub_version;								// 子版本号
	int		minor_version;								// 次版本号

}HCS_Version;


/** @ struct [HCS_PoolInfo]
 *  @ brief  资源池信息
 */
typedef struct
{
	char				pool_id[HCS_POOL_ID_LEN];		// 资源池ID	
	char				pool_name[HCS_POOL_NAME_LEN];	// 资源池名称[不支持中文]
	unsigned char		pool_type;						// 资源池类型[0-通用池  1-视频池  3-图片池  4-文件池]
	int					pool_cycle;						// 资源池覆盖周期[单位：天]
	unsigned char		disp_policy;					// 分散策略[0-集中  1-分散]
	unsigned char		cov_policy;						// 覆盖策略[0-不覆盖  1-按容量覆盖  2-周期覆盖]
	unsigned char		lock_limit;						// 当前资源池支持的最大锁定容量百分比[0% - 30%]
	unsigned long long	pool_size;						// 资源池大小[单位: MB]
	unsigned char		trans_type;						// 转码类型0不转码，1：抽帧转存	
	unsigned int		trans_cycle;					// 原始数据周期；单位：天
	unsigned char		pool_status;					// 资源池状态1：正常2：废弃3：容量不足4：提前覆盖
	unsigned long long	free_size;						// 资源池剩余空间[单位: MB]
	HCS_Time			begin_time;						// 写入数据的开始时间[第一次写入数据的时间]
	HCS_Time			cov_time;						// 发生数据覆盖的时间点
	HCS_Time			trans_time;						// 转码时间点
	unsigned int		domain_id;						// 域ID;资源池所在域ID
	int					user_count;						// 绑定用户个数
	char				username[HCS_POOLUSER_COUNT][HCS_USERNAME_LEN];			// 绑定的用户名,未做绑定时保留

}HCS_PoolInfo;


/** @struct [HCS_SupDevInfo]
 *  @brief 设备类型数据结构定义
 */
typedef struct 
{
	int		dev_type;									// 设备类型
	char	dev_desc[HCS_DESC_LEN];						// 设备类型描述信息

}HCS_SupDevInfo;

/** @ struct [HCS_Ability]
 *  @ brief  云存储能力集
 */
typedef struct 
{
	int				dev_count;							// 云存储支持的设备个数
	HCS_SupDevInfo	dev_list[HCS_SUP_DEV_NUM];			// 云存储支持的设备列表
	char			res[4];								// 保留字段，置为0 

}HCS_Ability;

/** @ struct [HCS_RecordInfo]
 *  @ brief  录像信息
 */
typedef struct
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// 编码器ID
	HCS_Time			begin_time;						// 录像开始时间
	HCS_Time			end_time;						// 录像结束时间
	unsigned char		type;							// 数据类型1-录像  2-视频文件
	unsigned short      record_type;					// 录像类型
	unsigned char		lock;							// 录像锁定状态[0: 全部类型(包括锁定的录像和没有锁定的录像)、1: 没有锁定、2: 锁定]
	unsigned char		trans_type;						// 转码类型[0: 原始，1: I帧]
	unsigned int		size;							// 录像大小[单位: 字节]
	unsigned int		unit_count;						// 段内单元数（图片数、I帧数和附属数）
	char				pool_id[HCS_POOL_ID_LEN];		// 资源池ID 
	char				vs_ip[HCS_IP_PORT_LEN];			// 存储服务器IP端口 格式：CVSIP:PORT1:PORT2:PORT3:PORT4
	char				device_id[HCS_STORDEV_ID_LEN];	// 存储设备ID
	unsigned int		block_id;						// 存储块ID
	unsigned int		segment_id;						// 存储段ID
	unsigned char		status;							// 当前录像可用状态[0: 正常、1: VS不在线、2: 设备不可用、3: 设备不存在]
	char				reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_RecordInfo;

/** @ struct [HCS_Period]
 *  @ brief  时间段信息
 */
typedef struct 
{
	char ip[HCS_IP_LEN];								// IP地址
	int  online;										// 在线状态，0不在线，1-在线

}HCS_VtduInfo;

/** @ struct [HCS_Period]
 *  @ brief  时间段信息
 */
typedef struct 
{
	int start_hour;										// 开始时间小时数
	int start_min;										// 开始时间分钟数
	int stop_hour;										// 结束时间小时数
	int stop_min;										// 结束时间分钟数

}HCS_Period;

/** @ struct [HCS_Endpoint]
 *  @ brief  网络上可连接的端点
 */
typedef struct 
{
	char	ip[HCS_DEVICE_IP_LEN];						// IP地址
	int		port;										// 端口
	char	username[HCS_USERNAME_LEN];					// 用户名
	char	password[HCS_PASSWORD_LEN];					// 密码

}HCS_Endpoint;

/** @ struct [HCS_DevInfo]
 *  @ brief  录像计划执行时间信息
 */
typedef struct 
{
	char			name[HCS_DEV_NAME_LEN];				// 设备名称
	HCS_Endpoint	endpoint;							// 设备基本信息
	unsigned int	protocol;							// 接入协议
	char			reserved[8];

}HCS_DevInfo;

/** @ struct [HCS_AuxServiceInfo]
 *  @ brief  录像计划执行时附加服务信息
 */
typedef struct 
{
	HCS_AUX_SERVICE type;								// 服务类型
	HCS_Endpoint	endpoint;							// 服务基本信息
	char			reserved[8];

}HCS_AuxServiceInfo;

/** @ struct [HCS_ServiceVTDU]
 *  @ brief  VTDU服务信息结构体
 */
typedef struct 
{
	HCS_Endpoint	endpoint;							// 服务基本信息
	char			reserved[8];

}HCS_ServiceVTDU;

/** @ struct [HCS_ServiceVAG]
 *  @ brief  VAG服务信息结构体
 */
typedef struct 
{
	HCS_Endpoint	endpoint;							// 服务基本信息
	int				vag_trans_stream;					// 是否过VAG转流 0 - 强制不转流 1 - 过VAG转流
	char			reserved[8];

}HCS_ServiceVAG;

/** @ struct [HCS_RecPlanDetail]
 *  @ brief  录像计划执行时间细节
 */
typedef struct 
{
	int			record;									// 是否录像[0-不录像  1-录像]
	int			stream_type;							// 码流类型[0-主码流  1-子码流]
	int			record_allday[HCS_DAYS_PER_WEEK];		// 是否全天录像
	HCS_Period	record_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// 录像计划时间段
	int			pre_record;								// 预录时长[0-不预录  其他值为预录时长(单位秒)]
	char		replication;							// 冗余份数[0-不冗余 1-1份冗余 2-双份冗余]
	char		reserved[4];

}HCS_RecPlanDetail;


/** @ struct [HCS_StreamDev]
 *  @ brief  取流设备信息
 */
typedef struct 
{
	HCS_DevInfo			device;							// 前端设备信息
	int					channel;						// 通道号
	HCS_ServiceVTDU		svc_vtdu;						// VTDU服务
	HCS_ServiceVAG		svc_vag;						// VAG服务
	HCS_AuxServiceInfo	svc_reserved;					// 附加服务保留
	char				reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_StreamDev;

/** @ struct [HCS_StreamUrl]
 *  @ brief  取流URL信息,如果通过流媒体向前端设备取流，用户名，密码是登录前端设备的用户名，密码
 *           如果是过VAG取流，用户密码则是VAG的用户名，密码。
 */
typedef struct 
{
	char				username[HCS_USERNAME_LEN];		// 用户名
	char				password[HCS_PASSWORD_LEN];		// 密码  
	char				stream_url[HCS_STREAM_URL_LEN];	// 取流URL
	char				reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_StreamUrl;

/** @ union [HCS_StreamInfo]
 *  @ brief  录像计划取流信息
 */
typedef union 
{
	char				data[HCS_STREAM_INFO_SIZE];		// 联合的内容
	HCS_StreamDev       stream_dev;						// 取流设备信息
	HCS_StreamUrl       stream_url;						// 取流URL信息

}HCS_StreamInfo;

/** @ struct [HCS_RecordPlan]
 *  @ brief  录像计划结构
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];		// 编码器ID，最大支持64个字符
	char				camera_name[HCS_CAMERA_NAME_LEN];	// 编码器名称

	int					stream_mode;						// 取流类型[0-设备取流，1-URL取流]
	HCS_StreamInfo		stream_info;						// 取流信息

	char				pool_id[HCS_POOL_ID_LEN];			// 资源池ID
	char				rep_pool_id[HCS_POOL_ID_LEN];		// 冗余资源池ID,冗余份数大于0时有效
	HCS_RecPlanDetail	detail;								// 录像计划执行时间信息细节
	char				reserved[HCS_RESERVED_DATA_LEN];	// 保留字段

}HCS_RecordPlan;


/** @ struct [HCS_Date]
 *  @ brief  日期结构体
 */
typedef struct 
{
	int year;
	int month;
	int day;

}HCS_Date;


/** @ struct [HCS_HisRecPlanDetail]
 *  @ brief  历史补录计划时间详细信息
 */
typedef struct 
{	
	int				history_stream_allday[HCS_DAYS_PER_WEEK];						// 是否补录全天数据
	HCS_Period		history_stream_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// 补录数据时间段
	
	int				replenish_allday[HCS_DAYS_PER_WEEK];							// 是否全天执行补录
	HCS_Period		replenish_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];		// 补录执行时间段

	HCS_Date		date;															// 补录开始日期,全0表示从下发计划当天开始补录													// 是否录像

	char			reserved[HCS_RESERVED_DATA_LEN];								// 保留字段

}HCS_HisRecPlanDetail;


/** @ struct [HCS_HisRecPlan]
 *  @ brief  历史流录像计划结构
 */
typedef struct 
{	
	char					camera_id[HCS_CAMERA_ID_LEN];							// 编码器ID，最大支持64个字符
	char					camera_name[HCS_CAMERA_NAME_LEN];						// 编码器名称
	
	int						stream_mode;											// 取流类型[0-设备取流，1-URL取流]
	HCS_StreamInfo			stream_info;											// 取流信息

	char					pool_id[HCS_POOL_ID_LEN];								// 资源池ID
	int						replication;											// 冗余份数[0-不冗余 1-1份冗余 2-双份冗余]
	char					rep_pool_id[HCS_POOL_ID_LEN];							// 冗余资源池ID,冗余份数大于0时有效

	HCS_HisRecPlanDetail	detail;													// 历史流录像计划执行时间信息细节
	char					reserved[HCS_RESERVED_DATA_LEN];						// 保留字段

}HCS_HisRecPlan;


/** @ struct [HCS_TaskInfo]
 *  @ brief  任务信息结构体
 */
typedef struct 
{
	unsigned long long	task_id;						// 任务ID
	int					task_type;						// 任务类型[1-备份,2-手动录像，3-手动补录]
	int					task_status;					// 任务状态[1-正常执行 2-异常 3-正常完成]
	float				task_speed;						// 任务进度,百分比[0-100]
	char				res[HCS_RESERVED_DATA_LEN];		// 保留

}HCS_TaskInfo;

/** @ struct [HCS_StreamFileInfo]
 *  @ brief  视频文件信息结构体
 */
typedef struct 
{
	char				file_key[HCS_FILE_KEY_LEN];		// 文件名
	unsigned long long	file_size;						// 文件大小,单位:字节
	HCS_Time			begin_time;						// 文件开始时间
	HCS_Time			end_time;						// 文件结束时间

}HCS_StreamFileInfo;

/** @ struct [HCS_StreamFileWriteInfo]
 *  @ brief  视频文件开启写信息结构体
 */
typedef struct 
{
	char file_key[HCS_FILE_KEY_LEN];					// 文件名
	char pool_id[HCS_POOL_ID_LEN];						// 资源池ID[最大支持32个字符]
	unsigned int replication;							// 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
	char rep_pool_id[HCS_POOL_ID_LEN];					// 冗余资源池ID[最大支持32个字符，冗余份数不为0时有效]
	char rec_head[HCS_HEAD_LEN];						// 录像头
	unsigned int head_size;								// 录像头长度

}HCS_StreamFileWriteInfo;
/** @ struct [HCS_RecDetailInfo]
 *  @ brief  录像参数
 */
typedef struct 
{
	int						enable;													// 是否使用此结构信息[0-不使用，则使用录像计划模板中的信息,1-使用]
	char					user_name[HCS_USERNAME_LEN];							// 设备用户名
	char					password[HCS_PASSWORD_LEN];								// 设备用户密码
	char					stream_url[HCS_STREAM_URL_LEN];							// 取流URL
	char					pool_id[HCS_POOL_ID_LEN];								// 资源池ID
	int						replication;											// 冗余份数[0-不冗余 1-1份冗余]
	char					rep_pool_id[HCS_POOL_ID_LEN];							// 冗余资源池ID,冗余份数大于0时有效
	char					reserved[HCS_RESERVED_DATA_LEN];						// 保留字段

}HCS_RecDetailInfo;

/** @ struct [HCS_RealRecInfo]
 *  @ brief  手动录像参数
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// 编码器ID
	unsigned int		rec_type;						// 录像类型[0-65535]
	HCS_RecDetailInfo	detail;							// 取流录像详细参数
	char				res[HCS_RESERVED_DATA_LEN];		// 保留字段

}HCS_RealRecInfo;

/** @ struct [HCS_HisRecInfo]
 *  @ brief  手动补录参数
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// 编码器ID
	unsigned int		rec_type;						// 录像类型[0-65535]
	HCS_Time			begin_time;						// 录像开始时间
	HCS_Time			end_time;						// 录像结束时间
	HCS_RecDetailInfo	detail;							// 取流录像详细参数
	char				res[HCS_RESERVED_DATA_LEN];		// 保留字段

}HCS_HisRecInfo;

/* @ struct [HCS_HighSpeedSegInfo]
 * @ brief 高速下载数据段结构
 */
typedef struct
{
	int					index;							// 数据段编号
	int					segment_id;						// 存储数据的数据段ID,由云存储内部生成
	HCS_Time			begin_time;						// 当前存储块的录像开始时间
	HCS_Time			end_time;						// 当前存储块的录像结束时间
	int					size;							// 当前存储块数据的总长度,单位:字节
	int					complete_len;					// 已经下载数据的长度,单位:字节
	int					record_type;					// 录像数据类型
	char				vs_ip[HCS_IP_PORT_LEN];			// 数据段所属vs ip

}HCS_HighSpeedSegInfo;


/* @ struct [HCS_RecDayInfo]
 * @ brief 日历查询信息
 */
typedef struct
{
	int				rec_set[HCS_DATA_HOURS];			// 查询当日每个小时的录像信息 [数组下标0 - 23，表示1 - 24个小时，0: 表示当前小时内没有录像信息，1: 表示当前小时内有录像信息]
	char			res[HCS_RESERVED_DATA_LEN];			// 保留字段

}HCS_RecDayInfo;


/* @ struct [HCS_RecMonthInfo]
 * @ brief 月历查询信息
 */
typedef struct
{
	int				rec_set[HCS_MONTH_DAYS];			// 查询当前月份每日的录像信息 [数组下标0 - 30，表示1 - 31号，0: 表示当前日没有录像信息，1: 表示当前日有录像信息]
	char			res[HCS_RESERVED_DATA_LEN];			// 保留字段

}HCS_RecMonthInfo;


/* @ struct [HCS_TagInfo]
 * @ brief 标注信息
 */
typedef struct
{
	HCS_Time		time;								// 标注时间点
	int				type;								// 标注类型（保留）
	char			tag[HCS_TAG_LEN];					// 标注内容
	char			res[HCS_RESERVED_DATA_LEN];			// 保留字段

}HCS_TagInfo;


/******************************************************************************
*                                                                            *
*                               智能检索数据结构									  *
*                                                                            *
******************************************************************************/

#define HCS_MAX_POLYGON_VERTEX       10					// 多边形最大顶点数
#define HCS_MAX_RULE_NUM             8					// 最多支持八条规则
#define HCS_MAX_RULE_NUM_EX          16					// 扩展规则，最多支持十六条规则
#define HCS_MAX_TARGET_NUM           30					// 最多同时监控30个移动目标
#define HCS_MAX_SHIELD_NUM           4					// 最多四个屏蔽区域
#define HCS_MAX_PARAM_NUM            200				// 最多关键参数个数
#define HCS_MAX_AUX_AREA_NUM         16					// 辅助区域最大数目


/** @ struct [HCS_PointF]
 *  @ brief  点(浮点) 
 */
typedef struct
{
	float x;
	float y;

}HCS_PointF;

/** @ struct [HCS_VectorF]
 *  @ brief  矢量
 */
typedef struct
{
	HCS_PointF   start_point;							// 起点
	HCS_PointF   end_point;								// 终点

}HCS_VectorF;


/** @ struct [HCS_PolygonF]
 *  @ brief  多边形(浮点)
 */
typedef struct
{
	unsigned int  vertex_num;							// 顶点数
	HCS_PointF    point[HCS_MAX_POLYGON_VERTEX];		// 顶点

}HCS_PolygonF;

/** @ struct [HCS_Shield]
 *  @ brief  屏蔽区域
 */
typedef struct
{
	unsigned char     id;								// 规则id，从1开始
	unsigned char     enable;							// 是否激活
	unsigned char     reserved[2];						// 保留字节
	HCS_PolygonF      polygon;							// 屏蔽区对应的多边形区域

}HCS_Shield;

/** @ struct [HCS_ShieldList]
 *  @ brief  屏蔽区链表
 */
typedef struct
{
	unsigned int  shield_num;
	HCS_Shield    shield[HCS_MAX_SHIELD_NUM];

}HCS_ShieldList;


/** @ struct [HCS_RectF]
 *  @ brief  矩形(浮点)
 */
typedef struct
{
	float x;											// 矩形左上角X轴坐标
	float y;											// 矩形左上角Y轴坐标
	float width;										// 矩形宽度
	float height;										// 矩形高度

}HCS_RectF;

/** @ enum   [HCS_SizeMode]
 *  @ brief  尺寸类型
 */
typedef enum
{
	HCS_IMAGE_PIX_MODE,									// 根据像素大小设置，单位pixel
	HCS_REAL_WORLD_MODE,								// 根据实际大小设置，单位m
	HCS_DEFAULT_MODE,									// 缺省模式
	HCS_IMAGE_PIX_AREA_MODE,							// 像素面积模式
	HCS_REAL_WORLD_AREA_MODE							// 实际面积模式

}HCS_SizeMode;


/** @ struct [HCS_SizeFilter]
 *  @ brief  目标尺寸过滤器结构体
 */
typedef struct
{
	unsigned int   enable;								// 是否激活尺寸过滤器
	HCS_SizeMode   mode;								// 过滤器类型:       
	HCS_RectF      min_rect;							// 最小目标框        
	HCS_RectF      max_rect;							// 最大目标框

}HCS_SizeFilter;


/** @ enum  [HCS_CrossDirection]
 *  @ brief  跨线方向类型
 */
typedef enum
{
	HCS_BOTH_DIRECTION,									// 双向
	HCS_LEFT_TO_RIGHT,									// 由左至右
	HCS_RIGHT_TO_LEFT									// 由右至左

}HCS_CrossDirection;


/** @ struct [HCS_TraversePlane]
 *  @ brief  跨线参数
 */
typedef struct
{
	HCS_CrossDirection cross_direction;					// 跨越方向 

}HCS_TraversePlane;


/** @ struct [HCS_ReachHeightEx]
 *  @ brief  跨线参数
 */
typedef struct
{
	HCS_CrossDirection cross_direction;					// 跨越方向 

}HCS_ReachHeightEx;


/** @ struct [HCS_Intrusion]
 *  @ brief  区域入侵参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_Intrusion;


/** @ struct [HCS_Loiter]
 *  @ brief  徘徊参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_Loiter;


/** @ struct [HCS_LeftTake]
 *  @ brief  物品拿取放置参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_LeftTake;


/** @ struct [HCS_StickUp]
 *  @ brief  异物粘贴参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_StickUp;


/** @ struct [HCS_InstallScanner]
 *  @ brief  读卡器参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_InstallScanner;


/** @ struct [HCS_Parking]
 *  @ brief  停车参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_Parking;


/** @ struct [HCS_AbnormalFace]
 *  @ brief  异常人脸参数
 */
typedef struct
{
	unsigned int delay;
	unsigned int mode;

}HCS_AbnormalFace;


/** @ struct [HCS_ToiletTarry]
 *  @ brief  如厕超时参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_ToiletTarry;


/** @ struct [HCS_YaryTarry]
 *  @ brief  放风场滞留参数
 */
typedef struct
{
	unsigned int delay; 

}HCS_YaryTarry;


/** @ struct [HCS_OverTime]
 *  @ brief  操作超时参数
 */
typedef struct
{
	unsigned int delay;

}HCS_OverTime;


/** @ struct [HCS_Run]
 *  @ brief  快速移动参数
 */
typedef struct
{
	float         speed;								// 速度
	unsigned int  delay;								// 报警延迟
	HCS_SizeMode  mode;									// 参数模式,只支持像素模式和实际模式两种

}HCS_Run;


/** @ struct [HCS_HighDensity]
 *  @ brief  人员聚集参数
 */
typedef struct
{
	float        alert_density;							// 人员聚集报警密度, 范围: [0.1, 1.0]
	unsigned int delay;									// 时间，达到该时间后发出拥挤报警

}HCS_HighDensity;   


/** @ enum  [HCS_ViolentMode]
 *  @ brief 剧烈运动模式
 */
typedef enum
{
	HCS_VIOLENT_VIDEO_MODE,								// 纯视频模式
	HCS_VIOLENT_VIDEO_AUDIO_MODE,						// 音视频联合模式
	HCS_VIOLENT_AUDIO_MODE								// 纯音频模式

}HCS_ViolentMode;


/** @ struct [HCS_Violent]
 *  @ brief  剧烈运动参数
 */
typedef struct
{
	unsigned int        delay;
	HCS_ViolentMode     mode;

}HCS_Violent;

/** @ struct [HCS_FlowCounter]
 *  @ brief  客流量统计进入方向参数
 */
typedef struct
{
	HCS_VectorF direction;

}HCS_FlowCounter;


/** @ enum   [HCS_LeavePosMode]
 *  @ brief  值岗状态模式
 */
typedef enum
{
	HCS_LEAVE_POSITION_LEAVE_MODE        = 0x01,		// 离岗模式
	HCS_LEAVE_POSITION_SLEEP_MODE        = 0x02,		// 睡岗模式
	HCS_LEAVE_POSITION_LEAVE_SLEEP_MODE  = 0x04			// 离岗睡岗模式

}HCS_LeavePosMode;


/** @ enum   [HCS_LeavePosPersonMode]
 *  @ brief  值岗人数模式
 */
typedef enum
{
	HCS_LEAVE_POSITION_SINGLE_MODE,						// 单人值岗模式
	HCS_LEAVE_POSITION_DOUBLE_MODE						// 双人值岗模式

}HCS_LeavePosPersonMode;


/** @ enum   [HCS_LeavePosChannelType]
 *  @ brief  主从通道设置
 */
typedef enum
{
	HCS_LEAVE_POSITION_INDEPENDENT,						// 独立通道
	HCS_LEAVE_POSITION_MASTER,							// 主通道
	HCS_LEAVE_POSITION_SLAVE							// 辅通道

}HCS_LeavePosChannelType;

/** @ struct [HCS_LeavePost]
 *  @ brief  离岗检测报警参数
 */
typedef struct
{
	HCS_LeavePosMode           mode;					// 离岗状态模式
	unsigned int               leave_delay;				// 无人报警时间
	unsigned int               static_delay;			// 睡觉报警时间
	HCS_LeavePosPersonMode     peo_mode;				// 离岗人数模式
	HCS_LeavePosChannelType    chan_type;				// 通道属性，独立通道，主通道，辅通道

}HCS_LeavePost;


/** @ struct [HCS_FallDown]
 *  @ brief  倒地检测参数
 */
typedef struct
{
	unsigned int delay;

}HCS_FallDown;


/** @ struct [HCS_AudioAbnormal]
 *  @ brief  声强突变参数
 */
typedef struct
{
	unsigned int  decibel;							// 声音强度
	unsigned char audio_mode;						// 声音检测模式，0：启用灵敏度检测；1：启用分贝阈值检测

}HCS_AudioAbnormal;


/** @ enum   [HCS_GetUpMode]
 *  @ brief  起床规则模式
 */
typedef enum
{
	HCS_GET_UP_OVER_BED_MODE  = 0x1,				// 大床通铺模式
	HCS_GET_UP_AREA_MOVE_MODE = 0x2,				// 高低铺模式
	HCS_GET_UP_SITTING_MODE   = 0x3					// 坐立起身模式

}HCS_GetUpMode;

/** @ struct [HCS_GetUp]
 *  @ brief  起身检测
 */
typedef struct
{
	HCS_GetUpMode mode;								// 起身检测模式

}HCS_GetUp;


/** @ struct [HCS_Static]
 *  @ brief  静坐
 */
typedef struct
{
	int delay;

}HCS_Static;


/** @ struct [HCS_BvFallDown]
 *  @ brief  双目倒地规则
 */
typedef struct
{
	float        height;							// 倒地高度阈值
	unsigned int delay;								// 倒地时间

}HCS_BvFallDown; 


/** @ struct [HCS_BvStandUp]
 *  @ brief  双目站立检测规则
 */
typedef struct
{
	float        height;							// 站立高度阈值
	unsigned int delay;								// 站立时间

}HCS_BvStandUp; 


/** @ enum   [HCS_ComparkMode]
 *  @ brief  双目人数检测及间距大于、小于、等于、不等于判断模式
 */
typedef enum
{
	HCS_MORE_MODE,									// 大于模式
	HCS_LESS_MODE,									// 小于模式
	HCS_EQUAL_MODE,									// 等于模式
	HCS_UNEQUAL_MODE								// 不等于模式

}HCS_ComparkMode;


/** @ struct [HCS_BvPepleNum]
 *  @ brief  双目人数检测规则
 */
typedef struct
{
	unsigned int     people_num;					// 人数
	unsigned int     people_state;					// 有无人状态,1:有人 0:无人
	HCS_ComparkMode  mode;							// 大于小于模式
	unsigned int     delay;							// 延时

}HCS_BvPepleNum;


/** @ struct [HCS_BvPeopleDistance]
 *  @ brief  双目间距检测规则
 */
typedef struct
{
	float            distance;						// 两目标间距
	HCS_ComparkMode  mode;							// 大于小于模式
	unsigned int     delay;							// 延时

}HCS_BvPeopleDistance;


/** @ union [HCS_RuleParamSets]
 *  @ brief 规则参数联合体
 */
typedef union
{
	int                      param[6];				// 参数
	HCS_TraversePlane        traverse_plane;		// 跨线参数  
	HCS_ReachHeightEx        reach_height_ex;		// 攀高参数
	HCS_Intrusion            intrusion;				// 区域入侵参数
	HCS_Loiter               loiter;				// 徘徊参数
	HCS_LeftTake             left_take;				// 物品拿取放置参数
	HCS_Parking              parking;				// 停车参数
	HCS_Run                  run;					// 快速移动参数
	HCS_HighDensity          high_density;			// 人员聚集参数
	HCS_Violent              violent;				// 剧烈运动参数
	HCS_AbnormalFace         abnormal_face;			// 异常人脸参数
	HCS_OverTime             over_time;				// 操作超时
	HCS_StickUp              stick_up;				// 异物粘贴参数
	HCS_InstallScanner       insert_scanner;		// 安装读卡器
	HCS_FlowCounter          flow_counter;			// 客流量统计进入方向
	HCS_LeavePost            leave_post;			// 离岗检测报警时间
	HCS_FallDown             fall_down;				// 倒地检测参数
	HCS_AudioAbnormal        audio_abnormal;		// 声音异常
	HCS_GetUp                get_up;				// 起身检测参数
	HCS_YaryTarry            yard_tarry;			// 放风场滞留时间
	HCS_ToiletTarry          toilet_tarry;			// 如厕超时时间
	HCS_Static               stic;					// 静坐
	HCS_BvFallDown           bv_fall_down;			// 双目倒地
	HCS_BvStandUp            bv_stand_up;			// 双目站立
	HCS_BvPepleNum           bv_people_num;			// 双目人数 
	HCS_BvPeopleDistance     bv_people_distance;	// 双目间距

}HCS_RuleParamSets;

/** @ enum [HCS_RuleType]
 *  @ brief 报警规则类型
 */
typedef enum
{
	HCS_LINE_RULE,									// 线规则 
	HCS_REGION_RULE,								// 区域规则
	HCS_LINE_EX_RULE								// 折线规则

}HCS_RuleType;


/** @ enum  [HCS_EventType]
 *  @ brief 事件类型
 */
typedef enum
{ 
	HCS_TRAVERSE_PLANE  = 0x01,						// 穿越警戒面
	HCS_ENTER_AREA      = 0x02,						// 进入区域
	HCS_EXIT_AREA       = 0x04,						// 离开区域
	HCS_INTRUSION       = 0x08,						// 区域入侵
	HCS_LOITER          = 0x10,						// 徘徊
	HCS_LEFT_TAKE       = 0x20,						// 物品拿取放置
	HCS_PARKING         = 0x40,						// 停车
	HCS_RUN             = 0x80,						// 快速移动
	HCS_HIGH_DENSITY    = 0x100,					// 人员聚集
	HCS_FLOW_COUNTER    = 0x200,					// 客流量
	HCS_VIOLENT_MOTION  = 0x400,					// 剧烈运动
	HCS_TRAIL           = 0x800,					// 尾随
	HCS_LEFT            = 0x1000,					// 物品遗留
	HCS_TAKE            = 0x2000,					// 物品拿取
	HCS_ABNORMAL_FACE   = 0x4000,					// 异常人脸
	HCS_GET_UP          = 0x8000,					// 起身检测
	HCS_REACH_HEIGHT    = 0x10000,					// 攀高检测
	HCS_LEAVE_POSITION  = 0x20000,					// 离岗检测
	HCS_FACE_CAPTURE	= 0x40000,					// 正常人脸
	HCS_FALL_DOWN       = 0x80000,					// 倒地检测
	HCS_AUDIO_ABNORMAL  = 0x100000,					// 声音异常检测
	HCS_MULTI_FACES     = 0x200000,					// 多张人脸
	HCS_TOILET_TARRY    = 0x400000,					// 如厕超时
	HCS_YARD_TARRY      = 0x800000,					// 放风场滞留
	HCS_REACH_HEIGHT_EX = 0x1000000,				// 攀高检测(折线)
	HCS_HUMAN_ENTER     = 0x10000000,				// 人员进入
	HCS_OVER_TIME       = 0x20000000,				// 操作超时
	HCS_STICK_UP        = 0x40000000,				// 异物粘贴              
	HCS_INSTALL_SCANNER = 0x80000000				// 安装读卡器   

}HCS_EventType;


/** @ struct [HCS_RuleInfo]
 *  @ brief  简化的规则信息, 包含规则的基本信息
 */
typedef struct
{
	unsigned char         id;						// 警戒规则计数器 id
	unsigned char         rule_type;				// 规则类型，0: line; 1: region;
	unsigned char         reserved[6];				// 对齐字节
	unsigned int          event_type;				// 警戒事件类型
	HCS_RuleParamSets     param;					// 规则参数
	HCS_PolygonF          polygon;					// 规则对应的多边形区域

}HCS_RuleInfo;


/** @ struct [HCS_RuleParam]
 *  @ brief  规则参数
 */
typedef struct
{
	unsigned int          sensitivity;				// 规则灵敏度参数，范围[1,5]
	unsigned int          reserved[4];				// 预留字节
	HCS_RuleParamSets     param;					// 规则参数
	HCS_SizeFilter        size_filter;				// 尺寸过滤器	

}HCS_RuleParam;


/** @ struct [HCS_Rule]
 *  @ brief  警戒规则
 */
typedef struct
{
	unsigned char      id;							// 规则id
	unsigned char      enable;						// 是否激活;
	unsigned char      rule_type;					// 规则类型，0: line; 1: region;
	unsigned char      target_type;					// 警戒目标类型，2: Human; 1: Vehicle; 0: anything
	unsigned int       event_type;					// 警戒事件类型

	unsigned char      reset_counter;				// 流量统计重置标记
	unsigned char      update_rule;					// 规则更新标记
	unsigned char      enable_counter;				// 是否开启事件计数器
	unsigned char      reserved[5];					// 保留字节,第一个字节用来表示使用新的事件类型还是老的；0：老的；1：新的;

	HCS_RuleParam      rule_param;					// 规则参数
	HCS_PolygonF       polygon;						// 规则对应的多边形区域

}HCS_Rule;


/** @ struct [HCS_RuleList]
 *  @ brief  警戒规则链表
 */
typedef struct
{
	unsigned int  rule_num;							// 链表中规则数量
	HCS_Rule      rule[HCS_MAX_RULE_NUM];			// 规则数组

}HCS_RuleList;


/** @ struct [HCS_RuleListEx]
 *  @ brief  扩展警戒规则链表
 */
typedef struct
{
	unsigned int  rule_num;							// 链表中规则数量
	HCS_Rule      rule[HCS_MAX_RULE_NUM_EX];		// 规则数组

}HCS_RuleListEx;


/* @ struct [HCS_SmartSearchParam]
 * @ brief  智能检索条件
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];	//编码器id
	HCS_Time		beg_time;						//录像开始时间
	HCS_Time		end_time;						//录像结束时间
	HCS_RuleList	rule_list;						//检索规则列表
	char			res[HCS_RESERVED_DATA_LEN];		//保留

}HCS_SmartSearchParam;


/* @ struct [HCS_SmartSearchRet]
 * @ brief 智能搜索结果
 */
typedef struct
{
	HCS_Time	start_time;						// 移动侦测报警开始的时间
	HCS_Time	end_time;						// 事件停止的时间
	char		res[64];

}HCS_SmartSearchRet;


/* @ struct [HCS_SmartSearchRet]
 * @ brief 车牌信息
 */
typedef struct
{
	HCS_RectF	plate;                          // 车牌位置，注意这里的x,y存放的是车牌的中心坐标(用于建模)
	float		angle;                          // 车牌倾斜角度[用于建模]
	int			color;							// 车牌颜色[车牌颜色使用数字表示，1 - 蓝、2 - 黄、3 - 白、4 - 黑、5 - 绿]
	char		res[64];						// 保留参数

}HCS_PlateInfo;


/* @ struct [HCS_PicModel]
 * @ brief 检索图片信息数据结构定义
 */
typedef struct 
{
	bool			use_model;					// 是否启用车牌数据模型
	HCS_PlateInfo	plate_info;					// 车牌数据模型
	char*			pic_buf;					// 图片数据信息
	unsigned int	buf_len;					// 图片数据长度
	float			similarity;					// 查询图片的相似度[0 - 1]
	char			res[64];					// 保留参数

}HCS_PicModel;

/* @ struct [HCS_SBPRet]
 * @ brief 以图搜图结果
 */
typedef struct
{
	char		camera_id[HCS_CAMERA_ID_LEN];	//编码器id
	char		url[HCS_PICTURE_URL_LEN];		//相似图片URL
	float		similarity;						//相似度
	char		res[64];

}HCS_SBPRet;





extern "C"
{
	/** @ fn     RecDownloadCb
	 *  @ brief  录像下载回调函数
	 *  @ param  handle		- [in] 录像下载操作句柄
	 *  @ param  data		- [in] 录像数据
	 *  @ param  data_size	- [in] 数据大小
	 *  @ param  flag		- [in] 剩余数据[0-已无更多数据  1-还有未回调数据]
	 *  @ param  error_code	- [in] 操作错误码
	 *  @ param  user_data	- [in] 用户自定义数据
	 *  @ return void
	 */
	typedef void (*RecDownloadCb)(int handle, const char* data, int data_size, int flag, int error_code, void* user_data);

	/**@ fn     AttachedDownloadCb
	*  @ brief  附属下载回调函数
	*  @ param  handle		- [in] 附属下载操作句柄
	*  @ param  data		- [in] 附属数据
	*  @ param  data_size	- [in] 数据大小
	*  @ param  begin_time	- [in] 开始时间（毫秒）
	*  @ param  end_time	- [in] 结束时间（毫秒）
	*  @ param  type		- [in] 附属类型（10 智能流）
	*  @ param  flag		- [in] 剩余数据[0-已无更多数据  1-还有未回调数据]
	*  @ param  error_code	- [in] 操作错误码
	*  @ param  user_data	- [in] 用户自定义数据
	*  @ return void
	*/
	typedef void (*AttachedDownloadCb)(int handle, const char* data, int data_size, HCS_Time begin_time,HCS_Time end_time,int type,int flag, int error_code, void* user_data);

	/**@ fn     RecHighSpeedDownloadCb
	*  @ brief  录像高速下载回调函数
	*  @ param  handle		- [in] 录像高速下载操作句柄
	*  @ param  data		- [in] 录像数据
	*  @ param  data_size	- [in] 数据大小
	*  @ param  index		- [in] 录像数据段序号,[index为-1,flag为0表示所有数据已下完]
	*  @ param  flag		- [in] 当前段剩余数据[0-已无更多数据  1-还有未回调数据]
	*  @ param  error_code	- [in] 操作错误码
	*  @ param  user_data	- [in] 用户自定义数据
	*  @ return void
	*/
	typedef void (*RecHighSpeedDownloadCb)(int handle, const char* data, int data_size, int index,int flag, int error_code, void* user_data);



	/** @ fn     PicDownloadCb
	 *  @ brief  图片下载回调函数
	 *  @ param  handle		- [in] 图片下载操作句柄
	 *  @ param  data		- [in] 图片数据
	 *  @ param  data_size	- [in] 图片数据大小
	 *  @ param  pic_type	- [in] 图片类型
	 *  @ param  flag		- [in] 剩余数据[0-已无更多数据  大于0-剩余的待回调条目数]
	 *  @ param  error_code	- [in] 操作错误码
	 *  @ param  user_data	- [in] 用户自定义数据
	 *  @ return void
	 */
	typedef void (*PicDownloadCb)(int handle, const char* data, int data_size, int pic_type, int flag, int error_code, void* user_data);

	/**@ fn     TaskNotifyCb
	*  @ brief  任务通知回调函数
	*  @ param  info		- [in] 任务信息
	*  @ param  flag		- [in] 剩余数据[0-已无更多数据  大于0-剩余的待回调条目数]
	*  @ param  error_code	- [in] 操作错误码
	*  @ param  user_data	- [in] 用户自定义数据
	*  @ return void
	*/
	typedef void (*TaskNotifyCb)(HCS_TaskInfo *info, int flag, int error_code, void* user_data);

}

#endif // HCS_SDK_DEFINE_H_
