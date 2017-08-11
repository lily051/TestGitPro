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

/** @ struct [HCS_AccessInfo]
 *  @ brief  录像计划接入信息
 */
typedef struct 
{
	int type;											// 录像计划类型，1-实时流录像计划,2-历史流录像计划
	int max_access_count;								// 最大接入数，云存储限制的最大录像计划接入数
	int current_access_count;							// 当前接入数，云存储当前实际接入的录像计划数
	int current_enable_access_count;					// 当前可接入数，云存储当前实际可接入的录像计划数
	int res[4];											// 保留

}HCS_AccessInfo;

/** @ struct [HCS_RecPlanAbility]
 *  @ brief  录像计划接入能力集
 */
typedef struct 
{
	int				count;								// 录像计划类型数量，现在默认支持2个，实时流录像计划和历史流录像计划
	HCS_AccessInfo  accss_info[HCS_MAX_RECPLAN_ABILITY];// 录像计划接入信息

}HCS_RecPlanAbility;

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


/** @ struct [HCS_RecSched]
 *  @ brief  时间段录像信息
 */
typedef struct 
{
	HCS_Period  rec_time;								// 录像时间
	int			rec_type;								// 录像类型[0:定时录像，1:移动侦测]
	char		reserved[8];

}HCS_RecSched;

/** @ struct [HCS_RecDay]
 *  @ brief  全天录像信息
 */
typedef struct 
{
	int			all_day_record;							// 是否全天录像[0-不全天录像  1-全天录像]
	int			rec_type;								// 录像类型[0:定时录像，1:移动侦测]
	char		reserved[8];

}HCS_RecDay;

/** @ struct [HCS_RecPlanDetail_V20]
 *  @ brief  录像计划执行时间细节,支持录像类型
 */
typedef struct 
{
	int				record;									// 是否录像[0-不录像  1-录像]
	int				defence;								// 是否布防[0-不布防  1-布防]
	int				stream_type;							// 码流类型[0-主码流  1-子码流]
	HCS_RecDay		record_allday[HCS_DAYS_PER_WEEK];		// 是否全天录像
	HCS_RecSched	record_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// 录像计划时间段
	int				pre_record;								// 预录时长[0-不预录  其他值为预录时长(单位秒)]
	char			replication;							// 冗余份数[0-不冗余 1-1份冗余 2-双份冗余]
	char			reserved[4];

}HCS_RecPlanDetail_V20;


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


/** @ struct [HCS_RecordPlan_V20]
 *  @ brief  录像计划结构,支持录像类型配置
 */
typedef struct 
{
	char					camera_id[HCS_CAMERA_ID_LEN];		// 编码器ID，最大支持64个字符
	char					camera_name[HCS_CAMERA_NAME_LEN];	// 编码器名称

	int						stream_mode;						// 取流类型[0-设备取流，1-URL取流]
	HCS_StreamInfo			stream_info;						// 取流信息

	char					pool_id[HCS_POOL_ID_LEN];			// 资源池ID
	char					rep_pool_id[HCS_POOL_ID_LEN];		// 冗余资源池ID,冗余份数大于0时有效
	HCS_RecPlanDetail_V20	detail;								// 录像计划执行时间信息细节
	char					reserved[HCS_RESERVED_DATA_LEN];	// 保留字段

}HCS_RecordPlan_V20;



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

/** @ struct [HCS_FileIndexData]
 *  @ brief  索引文件每一块的格式
 */
typedef struct
{
	unsigned int		frame_type;				// 帧主类型[1-I帧]
	unsigned int		frame_sub_type;         // 帧子类型[0-普通I帧，1-刷新帧（深P帧）]
	unsigned int		frame_rate;				// 帧率
	unsigned int		frame_num;				// 帧号[从1开始]
	HCS_Time			frame_abs_time;         // 帧绝对时间戳
	unsigned int		frame_rel_time;         // 帧相对时间戳[单位：毫秒]
	unsigned long long	frame_offset;			// 帧文件偏移，字节
	unsigned int		frame_size;				// 帧长度,字节
	unsigned int		res;					// 预留字段

}HCS_FileIndexData;


/** @ struct [HCS_VideoFileInfo]
 *  @ brief  视频文件信息结构体
 */
typedef struct 
{
	char				file_key[HCS_FILE_KEY_LEN];		// 文件名
	unsigned long long	file_size;						// 文件大小,单位:字节
	HCS_Time			begin_time;						// 文件开始时间
	HCS_Time			end_time;						// 文件结束时间
	unsigned int		index_count;					// 索引个数
	unsigned int		frame_rate;						// 帧率
	unsigned int		res[8];							// 预留

}HCS_VideoFileInfo;

/** @ struct [HCS_ThirdVideoFileInfo]
 *  @ brief  第三方视频文件信息结构体，当数据类型为非海康或非标准H.264格式文件时启用
 */
typedef struct  
{
	unsigned int		head_len;						// 文件头长度
	HCS_Time			begin_time;						// 文件的开始时间,单位是秒
	HCS_Time			end_time;						// 文件的结束时间,单位是秒
	unsigned long long	file_len;						// 文件的长度
	char				res[HCS_RESERVED_DATA_LEN];		// 保留字段

}HCS_ThirdVideoFileInfo;

/** @ struct [HCS_VideoFileWriteInfo]
 *  @ brief  视频文件信息结构体
 */
typedef struct  
{
	char					file_path[HCS_FILE_KEY_LEN];		// 文件路径（长度小于260）
	char					camera_id[HCS_CAMERA_ID_LEN];		// 视频文件对应编码器ID[未知可不填]
	unsigned int			replication;						// 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
	char					pool_id[HCS_POOL_ID_LEN];			// 资源池ID[最大支持32个字符]
	char					rep_pool_id[HCS_POOL_ID_LEN];		// 冗余资源池ID[最大支持32个字符]，冗余份数不为0时有效
	unsigned int			file_type;							// 文件类型（4--海康文件）
	HCS_ThirdVideoFileInfo	third_file_info;					// 第三方视频文件信息结构体信息
	char					res[HCS_RESERVED_DATA_LEN];			// 保留字段

}HCS_VideoFileWriteInfo;


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

/** @ struct [HCS_GetHeadInfo]
 *  @ brief  获取录像头结构体
 */
typedef struct 
{
	char					key[HCS_MAX_VIDEO_KEY_LEN];		// 数据读取key,如果是录像则填充编码器ID，如果是视频文件则填充文件key
	HCS_Time				begin_time;						// 开始时间
	HCS_Time				end_time;						// 结束时间
	unsigned int			data_type;						// 数据类型[0-录像,1-视频文件]
	char					reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_GetHeadInfo;


/** @ struct [HCS_DataReadInfo]
 *  @ brief  数据读取参数结构体
 */
typedef struct 
{
	char					key[HCS_MAX_VIDEO_KEY_LEN];		// 数据读取key,如果是录像则填充编码器ID，如果是视频文件则填充文件key
	HCS_Time				begin_time;						// 开始时间
	HCS_Time				end_time;						// 结束时间
	unsigned int			data_type;						// 数据类型[0-录像,1-视频文件]
	char					rec_type[HCS_RECORD_TYPE_LEN];	// 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符，视频文件时此字段无效]
	unsigned int			read_type;						// 读取类型[1-正常回放，2-正常I帧回放,3-倒序回放，4-下载]
	char					reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_DataReadInfo;


/** @ struct [HCS_VideoBaseInfo]
 *  @ brief  获取视频文件信息结构体
 */
typedef struct 
{
	char					file_key[HCS_FILE_KEY_LEN];		// 视频文件key
	char					reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_VideoBaseInfo;


/** @ struct [HCS_VideoReadInfo]
 *  @ brief  数据读取参数结构体
 */
typedef struct 
{
	char					file_key[HCS_MAX_VIDEO_KEY_LEN];// 视频文件key
	HCS_Time				begin_time;						// 开始时间
	HCS_Time				end_time;						// 结束时间
	unsigned int			read_type;						// 读取类型[1-正常回放，2-正常I帧回放,3-倒序回放，4-下载]
	char					reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_VideoReadInfo;


/** @ struct [HCS_HighSpeedDownloadInfo]
 *  @ brief  高速下载参数结构体
 */
typedef struct 
{
	HCS_DataReadInfo		data_info;						// 下载数据信息
	unsigned int			type;							// 下载类型[0-下载，1-续传]
	unsigned int			concurrency;					// 并发数[1-10]
	unsigned int			max_segment_count;				// 用户传入的段数组最大元素个数
	HCS_HighSpeedSegInfo*	segment_infos;					// 下载时：获取的数据块信息（输出），续传时：平台给入的已下载的数据信息（输入）
	int*					segment_count;					// 下载时：获取的数据块个数（输出），续传时：平台给入的已下载的数据个数（输入）
	char					reserved[HCS_RESERVED_DATA_LEN];// 保留字段

}HCS_HighSpeedDownloadInfo;


/* @ struct [HCS_RecFrameExtractInfo]
 * @ brief 抽帧显示信息
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];		// 编码器ID，最大支持64个字符
	char			record_type[HCS_RECORD_TYPE_LEN];	// 录像类型 [""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
	HCS_Time		begin_time;							// 录像开始时间
	HCS_Time		end_time;							// 录像结束时间
	int				time_sikp;							// 获取图片的时间间隔 [单位为秒]
	HCS_PicType		pic_type;							// 图片类型
	int				pic_width;							// 图片高度
	int				pic_hight;							// 图片宽度
	char			res[HCS_RESERVED_DATA_LEN];			// 保留字段

}HCS_RecFrameExtractInfo;

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

#define HCS_MAX_POLYGON_VERTEX			10					// 多边形最大顶点数
#define HCS_MAX_RULE_NUM				8					// 最多支持八条规则
#define HCS_MAX_RULE_NUM_EX				16					// 扩展规则，最多支持十六条规则
#define HCS_MAX_TARGET_NUM				30					// 最多同时监控30个移动目标
#define HCS_MAX_SHIELD_NUM				4					// 最多四个屏蔽区域
#define HCS_MAX_PARAM_NUM				200					// 最多关键参数个数
#define HCS_MAX_AUX_AREA_NUM			16					// 辅助区域最大数目


#define HCS_SEARCH_EVENT_INFO_LEN		800					// 智能事件信息长度
#define HCS_MAX_ANALOG_CHANNUM			32
#define HCS_MAX_IP_CHANNEL				32
#define HCS_MAX_CHANNUM							    ( HCS_MAX_ANALOG_CHANNUM + HCS_MAX_IP_CHANNEL )

#define HCS_STREAM_ID_LEN				32
#define HCS_MAX_POS_KEYWORDS_NUM		3					// 支持关键字查找条数
#define HCS_MAX_POS_KEYWORD_LEN			128					// 每条关键字长度
#define HCS_SEARCH_CASE_NO_LEN			56
#define HCS_SEARCH_CASE_NAME_LEN		100
#define HCS_SEARCH_LITIGANT_LEN			32
#define HCS_SEARCH_CHIEF_JUDGE_LEN		32

#define HCS_MAX_CHANNUM_V40				512
#define HCS_NAME_LEN					32      //用户名长度
#define HCS_CASE_NO_RET_LEN				52
#define HCS_CASE_NAME_RET_LEN			64
#define HCS_LITIGANT_RET_LEN			24
#define HCS_CHIEF_JUDGE_RET_LEN			24


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
	unsigned int	rule_num;						// 链表中规则数量
	HCS_Rule		rule[HCS_MAX_RULE_NUM];			// 规则数组

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


/* @ struct [HCS_SmartSearchParam_V20]
 * @ brief  智能检索条件(支持提前延后时间设置)
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];	//编码器id
	HCS_Time		beg_time;						//录像开始时间
	HCS_Time		end_time;						//录像结束时间
	unsigned int	pre_time;						//智能报警提前时间 单位:秒
	unsigned int	delay_time;						//智能报警延迟时间 单位:秒		
	HCS_RuleList	rule_list;						//检索规则列表
	char			res[HCS_RESERVED_DATA_LEN];		//保留

}HCS_SmartSearchParam_V20;

/* @ struct [HCS_SmartSearchRet]
 * @ brief 智能搜索结果
 */
typedef struct
{
	HCS_Time	start_time;						// 移动侦测报警开始的时间
	HCS_Time	end_time;						// 事件停止的时间
	char		res[64];

}HCS_SmartSearchRet;



// 流信息 - 72字节长
typedef struct 
{
	int		size;
	char	id[HCS_STREAM_ID_LEN];  //ID数据
	int		channel;                //关联设备通道，等于0xffffffff时，表示不关联
	char	res[32];                //保留

}StreamInfo;

/* @ struct [HCS_SmartEventSearchParam]
 * @ brief  智能事件检索条件
 */
typedef struct
{
	int			maior_type;						//0-移动侦测，1-报警输入, 2-智能事件 5-pos录像
	int			minor_type;						//搜索次类型- 根据主类型变化，0xffff表示全部
	char		camera[HCS_CAMERA_ID_LEN];		//检索的编码器
	HCS_Time	start_time;						//搜索的开始时间，停止时间: 同时为(0, 0) 表示从最早的时间开始，到最后，最前面的4000个事件
	HCS_Time	end_time;						//搜索的结束时间
	int			lock_type;						// 0xff-全部，0-未锁，1-锁定
	union
	{
		char				len[HCS_SEARCH_EVENT_INFO_LEN];

		struct//报警输入
		{
			short			alarm_info_num[128];			//报警输入号，按值表示,采用紧凑型排列，0xffff表示后续无效
			char			res[544];						//保留

		}AlarmParam;

		struct//移动侦测
		{
			short			motion_channel[HCS_MAX_CHANNUM];//移动侦测通道，按值表示 ,采用紧凑型排列，0xffff表示后续无效
			char			res[672];						//保留

		}MotionParam;

		struct //行为分析 
		{
			short			channel[HCS_MAX_CHANNUM];		//行为分析对应的通道，按值表示,采用紧凑型排列，0xffff表示后续无效
			char			rule_id;						//行为分析类型，规则0xff表示全部，从0开始
			char			res[671];    

		}VcaParam;

		struct//审讯事件搜索条件
		{
			char			room_index;						//审讯室编号,从1开始
			char			res[799];						//保留

		}InquestParam;

		struct//智能侦测查找条件 ，通道号按值表示
		{
			char			all;							//查找全部通道，0-否，此时dwChanNo参数有效，//1-查找全部通道，此时dwChanNo参数无效。
			char			res1[3];
			short			channel[HCS_MAX_CHANNUM];		// 触发通道号,按值表示，0xffff无效，且后续数据也表示无效值
			char			res[668];

		}VCADetect;

		struct
		{
			StreamInfo		id_info;						// 流id信息，72字节长
			int				cmd_type;						// 外部触发类型，NVR接入云存储使用
			char			backup_volume_num;				//存档卷号，CVR使用
			char			res[723];

		}StreamIDParam;

		struct //pos录像
		{
			short			channel[HCS_MAX_CHANNUM];		//通道，按值表示,紧凑型排列，遇到0xffff时表示数组后续值无效
			char			all_channel;					//是否查找全部通道，0-否，此时wChannel有效，1-全部通道，此时wChannel无效
			char			case_sensitive;					//0-不区分大小写， 1-区分大小写
			char			combinate_mode;					//关键字组合方式，0-或，1-与
			char			res1;							//保留
			char			key_word[HCS_MAX_POS_KEYWORDS_NUM][HCS_MAX_POS_KEYWORD_LEN]; //关键字查找时的条件
			char			res2[284];						//保留

		}PosAlarm;

		struct
		{
			char			case_index[HCS_SEARCH_CASE_NO_LEN];			//案件编号
			char			case_name[HCS_SEARCH_CASE_NAME_LEN];		//案件名称
			char			litigant1[HCS_SEARCH_LITIGANT_LEN];			//当事人1
			char			lLitigant2[HCS_SEARCH_LITIGANT_LEN];		//当事人2
			char			chief_judge[HCS_SEARCH_CHIEF_JUDGE_LEN];	//审判长
			char			case_type;
			char			res[47];

		}TrialParam;

	}SeniorParam;

	char			res[HCS_RESERVED_DATA_LEN];				//保留

}HCS_SmartEventSearchParam;


//警戒面参数
typedef struct
{	
	HCS_VectorF				plane_bottom;					//警戒面底边
	int						cross_direction;				//穿越方向: 0-双向，1-从左到右，2-从右到左
	char					sensitivity;                    //灵敏度参数，范围[1,5]
	char					plane_height;                   //警戒面高度
	char					dtection_target;				//0-所有目标，1-人，2-车
	char					res[37];						//保留

}HCS_Event_TraversePlan;

//进入/离开区域参数
typedef struct
{
	HCS_PolygonF			region;//区域范围
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					detection_target;				//0-所有目标，1-人，2-车
	char					res[6];

}HCS_Event_Area;

//根据报警延迟时间来标识报警中带图片，报警间隔和IO报警一致，1秒发送一个。
//入侵参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//行为事件触发时间阈值: 1-120秒，建议5秒，判断是有效报警的时间  在ATM系统中触发文件阈值为 1-1000秒
	char					sensitivity;					//灵敏度参数，范围[1-100]
	char					rate;							//占比：区域内所有未报警目标尺寸目标占区域面积的比重，归一化为－；
	char					detection_target;				//0-所有目标，1-人，2-车
	char					res[3];							//保留

}HCS_Event_Intrusion;

//徘徊参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发时间阈值：1-120秒，建议10秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res[5];

}HCS_Event_Loiter;

//物品遗留/物品拿取参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发时间阈值：1-120秒，建议10秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res[5];

}HCS_Event_take_left;

//停车参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发时间阈值：1-100秒，建议10秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res[5];

}HCS_Event_Parking;

//快速移动参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	float					run_distance;					//人快速移动最大距离, 范围: [0.1, 1.00] 像素模式 实际模式(1,20)m/s
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					mode;							// 0 像素模式  1 实际模式
	char					detection_target;				//0-所有目标，1-人，2-车
	char					res;

}HCS_Event_Run;

//人员聚集参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	float					density;						//聚集比率, 范围: [0.1, 1.0]
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res;							// 保留字节
	short					duration;						// 触发人员聚集参数报警阈值 20-360s

}HCS_Event_High_Density; 

//剧烈运动参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发剧烈运动报警阈值：1-50秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					mode;							//0-纯视频模式，1-音视频联合模式，2-纯音频模式
	char					res[4];							//保留

}HCS_Event_Violent_motion; 

// 攀高参数
typedef struct
{
	HCS_VectorF				vca_line;						//攀高警戒面
	short					duration;						//触发攀高报警阈值：1-120秒
	char					res[6];							// 保留字节

}HCS_Event_Reach_Hight;

// 起床参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发起床报警阈值1-100 秒
	char					mode;							//起身检测模式,0-大床通铺模式,1-高低铺模式,2-大床通铺坐立起身模式
	char					sensitivity;					//灵敏度参数，范围[1,10]
	char					res[4];							//保留字节

}HCS_Event_Get_Up;

// 物品遗留
typedef struct
{
	HCS_PolygonF			region;							// 区域范围
	short					duration;						// 触发物品遗留报警阈值 10-100秒
	char					sensitivity;					// 灵敏度参数，范围[1,5] 
	char					res[5];							// 保留字节

}HCS_Event_Left;

// 物品拿取
typedef struct
{
	HCS_PolygonF			region;							// 区域范围
	short					duration;						// 触发物品拿取报警阈值10-100秒
	char					sensitivity;					// 灵敏度参数，范围[1,5] 
	char					res[5];							// 保留字节

}HCS_Event_Take;

typedef struct
{
	HCS_PolygonF			region;							// 区域范围
	short					duration;						// 操作报警时间阈值 4s-60000s
	char					res[6];							// 保留字节

}HCS_Event_OverTime;

typedef struct
{
	int						res[23];						//保留字节

}HCS_Event_Enter;

//贴纸条参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发时间阈值：4-60秒，建议10秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res[5];

}HCS_Event_Stick_Up; 

//读卡器参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//读卡持续时间：4-60秒
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res[5];

}HCS_Event_Scanner; 

//离岗事件
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					leave_delay;					//无人报警时间，单位：s，取值1-1800
	short					static_delay;					//睡觉报警时间，单位：s，取值1-1800
	char					mode;							//模式，0-离岗事件，1-睡岗事件，2-离岗睡岗事件
	char					person_type;					//值岗人数类型，0-单人值岗，1-双人值岗
	char					res[2];							//保留

}HCS_Event_LEAVE_POSITION;

//尾随参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					res;							//保留
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					res2[5];

}HCS_Event_Trail;

//倒地参数
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//触发事件阈值 1-60
	char					sensitivity;					//灵敏度参数，范围[1,5]
	char					height_threshold;				//高度阈值，范围[0,250]，默认90，单位：厘米
	char					res[4];

}HCS_Event_Fall_Down;

//起立
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	char					sensitivity;					//灵敏度参数，范围[1,100]
	char					height_threshold;				//高度阈值，范围[0,250]，默认130，单位：厘米
	short					duration;						//触发事件阈值[1,3600]，默认2，单位：秒
	char					res[4];							//保留

}HCS_Event_Standup;


//人数变化
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	char					sensitivity;					//灵敏度参数，范围[1,100]
	char					people_num_threshold;			//人数阈值，范围[0,5]，默认1 
	char					detect_mode;					//检测方式，与人数阈值相比较。1-大于，2-小于，3-等于，4-不等于
	char					none_state_effective;			//无人状态是否有效，0-无效，1-有效
	short					duration;						//触发时间阈值[1,3600]，默认2，单位：秒
	char					res[2];							//保留

}HCS_Event_PeopleNum_Change;

//间距变化
typedef struct
{
	HCS_PolygonF			region;							//区域范围
	float					spacing_threshold;				//间距阈值，范围[0,10.0]，默认1.0，单位：米
	char					sensitivity;					//灵敏度参数，范围[1,100]
	char					detect_mode;					//检测方式，与间距阈值相比较。1-大于，2-小于
	short					duration;						//触发时间阈值[1,3600]，默认2，单位：秒

}HCS_Event_Spacing_Change;


//声强突变参数
typedef struct
{
	short					decibel;						//声音强度
	char					sensitivity;					//灵敏度参数，范围[1,100] 
	char					audio_mode;						//声音检测模式，0-灵敏度检测，1-分贝阈值检测，2-灵敏度与分贝阈值检测 
	char					enable;							//使能，是否开启(声强突变，陡升)
	char					threshold;						//声音阈值[1,100]
	char					res[54];						//保留   

}HCS_Event_Audio_Abnormal;

//声强陡降
typedef struct
{
	char					sensitivity;					//灵敏度参数，范围[1,100] 
	char					enable;							//使能，是否开启(声强突变，陡降)
	char					res[6];    

}HCS_Event_Steep_Drop;


typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					delay;							//如厕超时时间[1,3600]，单位：秒
	char					res[6];    

}HCS_Event_Toiler_Tarry;

typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					delay;							//放风场滞留时间[1,120]，单位：秒
	char					res[6];   

}HCS_Event_Yard_Tarry;

typedef struct
{
	HCS_PolygonF			region;							//攀高折线
	int						cross_direction;				//跨越方向: 0-双向，1-从左到右2-从右到左
	char					res[4];							//保留字节

}HCS_Event_AdvReach_Height;

typedef struct
{
	HCS_PolygonF			region;							//警戒面折线
	int						cross_direction;				//跨越方向: 0-双向，1-从左到右2-从右到左
	char					sensitivity;					//灵敏度参数，范围[1,5] 
	char					res[3];							//保留字节

} HCS_Event_AdvTraverse_Plane;

typedef struct
{
	HCS_PolygonF			region;							//区域范围
	short					duration;						//行为事件触发时间阈值: 1-10秒，建议1秒，判断是有效报警的时间
	char					sensitivity;					//灵敏度参数，范围[1-100]
	char					alarm_state;					//只读字段界面不显示；0-保留,1-报警开始,2-报警结束 
	char					tracking_mode ;					//跟踪模式，0-自动(默认) , 1-水平, 2-垂直
	char					zoom_mode;						//变倍模式, 0-固定(默认), 1-自动
	char					zoom_over;						//0-保留,1-变倍到位(报警上传录播主机，作为切换画面判断依据)
	char					res;

}HCS_Event_Lecture;

typedef struct
{
	HCS_PolygonF			region;							//区域范围
	char					sensitivity;					//灵敏度参数，范围[1-100]
	char					alarm_state;					//只读字段界面不显示；0-保留,1-报警开始,2-报警结束
	char					zoom_over;						//0-保留,1-变倍到位(报警上传录播主机，作为切换画面判断依据)
	char					res[5];							//保留

}HCS_Event_Answer;

//关联规则结构体
typedef struct
{   
	char					rule_id;						//规则序号 0-表示无
	char					res[3];							//保留

}HCS_Event_Relate_Rule_Param;

typedef struct
{
	char							rule_sequence;			//规则触发顺序，0-顺序或逆序触发，1-顺序触发
	char							res1[7];
	int								min_interval;			//最小时间间隔，单位：秒
	int								max_interval;			//最大时间间隔，单位：秒
	HCS_Event_Relate_Rule_Param		struRule1Raram;			//规则1
	HCS_Event_Relate_Rule_Param		struRule2Raram;			//规则2
	char							res2[36];

} HCS_Event_Combined_Rule;

//警戒事件参数
typedef union 
{
	int								len[23];				//参数
	HCS_Event_TraversePlan			traverse_plane;			//穿越警戒面参数 
	HCS_Event_Area    				area;					//进入/离开区域参数
	HCS_Event_Intrusion				intrusion;				//入侵参数
	HCS_Event_Loiter				loiter;					//徘徊参数
	HCS_Event_take_left				take_left;				//物品遗留/物品拿取参数
    HCS_Event_Parking				parking;				//停车参数
	HCS_Event_Run					run;					//快速移动参数  *
	HCS_Event_High_Density			high_density;			//人员聚集参数  *
	HCS_Event_Violent_motion		violent_motion;			//剧烈运动
	HCS_Event_Reach_Hight			reach_hight;			//攀高
	HCS_Event_Get_Up				get_up;					//起床
	HCS_Event_Left					left;					//物品遗留
	HCS_Event_Take					take;					// 物品拿取
	HCS_Event_Enter					human_enter;			//人员进入
	HCS_Event_OverTime				over_time;				//操作超时
	HCS_Event_Stick_Up 				stick_up;				//贴纸条
	HCS_Event_Scanner 				scanner;				//读卡器参数 	
	HCS_Event_LEAVE_POSITION		leave_pos;				//离岗参数
	HCS_Event_Trail					trail;					//尾随参数
	HCS_Event_Fall_Down				fall_down;				//倒地参数
	HCS_Event_Audio_Abnormal		audio_abnormal;			//声强突变
	HCS_Event_AdvReach_Height		adv_reach_height;		//折线攀高参数
	HCS_Event_Toiler_Tarry			toilet_tarry;			//如厕超时参数
	HCS_Event_Yard_Tarry			yard_tarry;				//放风场滞留参数
	HCS_Event_AdvTraverse_Plane		adv_traverse_plane;		//折线警戒面参数
	HCS_Event_Lecture				lecture;				//授课事件
	HCS_Event_Answer				answer;					//回答问题事件
	HCS_Event_Standup				stand_up;				//起立参数
	HCS_Event_PeopleNum_Change		people_num_change;		//人数变化参数
	HCS_Event_Spacing_Change		spacing_change;			//间距变化参数
	HCS_Event_Combined_Rule			combined_rule;			//组合规则参数

}HCS_SmartEventUnion;


/* @ struct [HCS_SmartEventSearchResult]
 * @ brief 智能事件检索结果
 */
typedef struct
{
	int			maior_type;									//0-移动侦测，1-报警输入, 2-智能事件 5-pos录像
	int			minor_type;									//搜索次类型- 根据主类型变化，0xffff表示全部
	HCS_Time	start_time;									//事件开始的时间
	HCS_Time	end_time;									//事件开始的时间
	short		chanel[HCS_MAX_CHANNUM_V40/*512*/];			//触发的通道号，0xffff表示后续无效
	union		
	{
		char				len[800];						//联合体长度

		struct//报警输入结果
		{
			int				alarm_info_num;					//报警输入号
			char			res[796];

		}AlarmRet;

		struct//移动侦测结果
		{
			int				motion_channel;					//移动侦测通道
			char			res[796];

		}MotionRet;

		struct//行为分析结果  
		{
			int				channel;						//触发事件的通道号
			char			rule_id;						//规则ID
			char			res1[3];						//保留
			char			rule_name[HCS_NAME_LEN];		//规则名称
			HCS_SmartEventUnion smart_event;				//行为事件参数
			char			res2[668];						//保留

		}VcaRet;

		struct//审讯事件
		{
			char			room_index;						//审讯室编号,从1开始
			char			drive_index;					//刻录机编号,从1开始
			char			res1[6];						//保留
			int				segment_number;					//本片断在本次审讯中的序号,从1开始 
			short			segment_size;					//本片断的大小, 单位M 
			short			segment_status;					//本片断状态 0 刻录正常，1 刻录异常，2 不刻录审讯
			char			res2[784];						//保留

		}InquestRet;

		struct //流id录像查询结果
		{
			int				record_type;					//录像类型 0-定时录像 1-移动侦测 2-报警录像 3-报警|移动侦测 4-报警&移动侦测 5-命令触发 6-手动录像 7-震动报警 8-环境触发 9-智能报警 10-回传录像
			int				record_len;						//录像大小
			char			lock;							//锁定标志 0：没锁定 1：锁定
			char			frame_type;						//0：非抽帧录像 1：抽帧录像
			char			res1[2];
			char			file_name[HCS_NAME_LEN];		//文件名
			int				file_index;						// 存档卷上的文件索引
			char			res2[752];

		}StreamIDRet;

		struct//POS录像查询结果
		{
			int				channel;						//触发产生pos事件的通道
			char			res[796];

		}PosRet;

		struct
		{
			char			room_index;						//审讯室编号,从1开始
			char			drive_index;					//刻录机编号,从1开始
			short			segment_size;					//本片断的大小, 单位M
			int				segment_index;					//本片断在本次审讯中的序号,从1开始
			char			segment_status;					//本片断状态，0-刻录正常，1-刻录异常，2-不刻录审讯
			char			case_type;						//案件类型；0-全部、1-刑事案件、2-民事案件
			char			res1[2];
			char			case_number[HCS_CASE_NO_RET_LEN];	//案件编号
			char			case_name[HCS_CASE_NAME_RET_LEN];	//案件名称；
			char			litigant1[HCS_LITIGANT_RET_LEN];	//当事人1；
			char			litigant2[HCS_LITIGANT_RET_LEN];	//当事人2；
			char			chief_judge[HCS_CHIEF_JUDGE_RET_LEN];//审判长
			char			res2[600];

		}TrialRet;

	}SeniorRet;

	char					res[HCS_RESERVED_DATA_LEN];		//保留

}HCS_SmartEventSearchResult;


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

   /** @ fn    RecFrameExtractCb
	*  @ brief 抽帧显示回调函数
	*  @ param handle       - [in] 获取抽帧显示的句柄
	*  @ param data			- [in] 抽帧显示数据 
	*  @ param data_size	- [in] 抽帧显示数据大小
	*  @ param pic_type		- [in] 图片类型
	*  @ param hcs_time  	- [in] 获取图片的时间
	*  @ param frame_width  - [in] 抽帧显示宽度[图片宽度]
	*  @ param frame_height - [in] 抽帧显示高度[图片高度]
	*  @ param flag         - [in] 当前段剩余数据[0-已无更多数据  1-还有未回调数据]
	*  @ param error_code	- [in] 操作错误码
	*  @ param user_data    - [in] 用户自定义数据 
	*  @ return void
	*/
	typedef void (*RecFrameExtractCb)(int handle, const char* data, long data_size, HCS_PicType pic_type, HCS_Time hcs_time,int frame_width, int frame_height, int flag,  int error_code, void* user_data);


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

	/**@ fn     UploadFileSpeedCb
	*  @ brief  异步上传文件进度回调函数
	*  @ param  handle		- [in] 开启文件上传句柄
	*  @ param  file_speed	- [in] 文件上传进度[0-100]
	*  @ param  error_code	- [in] 操作错误码
	*  @ param  user_data	- [in] 用户自定义数据
	*  @ return void
	*/
	typedef void (*UploadFileSpeedCb)(int handle, int file_speed, int error_code, void* user_data);

}

#endif // HCS_SDK_DEFINE_H_
