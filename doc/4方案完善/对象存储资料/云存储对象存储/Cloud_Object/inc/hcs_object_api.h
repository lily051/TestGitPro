/** @file   hcs_object_api.h
*  @note	HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief	API Interface H File
*  @author  xiaweiqiang@hikvision.com.cn
*  @date	2015-01-04
*
*  @change log
*
*/

#ifndef _HCS_OBJECT_API_
#define _HCS_OBJECT_API_

#define HCS_OBJECT_API_EXPORTS

#if (defined _WIN32 || defined _WIN64)
#ifdef HCS_OBJECT_API_EXPORTS
#define HCS_OBJECT_API __declspec(dllexport)
#else
#define HCS_OBJECT_API __declspec(dllimport)
#endif
#else
#define HCS_OBJECT_API
#endif

/**
 * 表示hik对象存储服务的访问控制列表（Access Control List, ACL),
 */
enum HCSAccessACL
{
	PUBLIC_READ_WRITE = 0,      //能读能写
	PUBLIC_READ,				//只读
	PRIVATE,					//私有
};


/**
 * 创建bucket时,需指定bucket的存储策略
 默认不开启版本号管理,即bucket下的所有object只有一个版本
 当重新设置bucket的版本号后,同一个bucket下同一个key可以对应多个object，每个object的version不同
 且开启后只能对以后新写的数据起作用
 版本号管理不能关闭,只能挂起,当挂起之后，新写的数据只有一个版本,以前写的数据不变
 */
enum HCSBucketVersionManage
{
	UNVERSIONED = 0,			//不开启版本号管理
	VERSION_ENABLED,			//开启版本号管理服务
	VERSION_SUSPENDED,			//挂起版本号管理服务
};


/**
 * 获取object列表或者信息时,object的状态所表示的信息
 */
enum HCSObjectStatus
{
	STATUS_NORMAL = 0,			//正常
	STATUS_UPLOADING,			//正在上传
	STATUS_COPYING,				//正在拷贝
	STATUS_DELETED,				//已删除
	STATUS_COPY_FAILED,			//拷贝失败
	STATUS_UPLOAD_ERROR,		//上传失败
};


/**
 * 用户上传流式文件(主要是视频文件)时指定的动作
 */
enum HCSStreamFileAct
{
	ACT_NOACT = 0,              //无动作
	ACT_FRAME_ANALYZE,			//需要云存储做帧分析
	ACT_TRANSCODE_FILE,			//需要云存储转码
	ACT_ANALYZE_TRANSCODE,		//需要云存储帧分析且转码
};


/**
 * 输出封装格式
 */
enum HCSOutputPacketType
{
	TYPE_SOURCE = 0,			//保持原格式不变
	TYPE_HIK,					//海康封装
	TYPE_MPEG2_PS,				//MPEG2-PS
	TYPE_MPEG2_TS,				//MPEG2-TS
	TYPE_RTP,					//RTP
	TYPE_MP4_3GPP_MOV,			//MP4/3GPP/MOV
	TYPE_AVI,					//AVI
};


/**
 * 输出视频封装格式
 */
enum HCSVideoEncodeFormat
{
	VIDEO_SOURCE = 0,			//保持原有
	VIDEO_HIK264,				//海康264
	VIDEO_MPEG2,				//mpeg2
	VIDEO_MPEG4,				//mpeg4
	VIDEO_H264,					//H264
	VIDEO_MJPEG,				//MJPEG
	VIDEO_MPEG4V1_V2_V3,		//MS_MPEG4V1/2/3
	VIDEO_WMV2,					//WMV2
};


/**
 * 输出音频封装格式
 */
enum HCSAudioEncodeFormat
{
	AUDIO_SOURCE = 0,			//保持原有
	AUDIO_MPGA,					//mpga
	AUDIO_AAC,					//AAC
	AUDIO_G711,					//G.711
	AUDIO_G722,					//G.722
	AUDIO_G7231,				//G.7231
	AUDIO_G726,					//G.726
	AUDIO_G729,					//G.729
	AUDIO_AMR_NB,				//AMR_NB
	AUDIO_WNAV2,				//WNAV2
	AUDIO_PCM,					//PCM
};


/**
 * 下载图片目标格式
 */
enum HCSImageOutputFormat
{
	FOAMAT_SOURCE = 0,//保持原有格式
	FORMAT_JPG,       //转换为JPG
	FORMAT_PNG,       //转换为PNG
	FORMAT_BMP,       //转换为BMP
};


/**
 * 按时间段下载视频数据的输出视频封装格式
 */
enum HCSVideoOutPutFormat
{
	OUTPUT_VIDEO_SOURCE = 0,			//保持原有
	OUTPUT_VIDEO_HIK264,				//海康264
	OUTPUT_VIDEO_PS,				    //ps
	OUTPUT_VIDEO_TX,				    //tx
	OUTPUT_VIDEO_RTP,					//rtp
	OUTPUT_VIDEO_MPEG4,					//MPEG4
};

/**
 * 用户注册后，返回的用户标识指针
 */
typedef struct _HCSClient 
{
	char *endpoint;				//云存储服务器入口地址,若有域名则填写域名，若无则填写IP:PORT
	char *access_key;			//云存储返回的公钥信息
	char *secret_key;			//云存储返回的私钥信息
}HCSClient;


/**
 * 元数据键值对结构体
 */
typedef struct _KeyValuePair
{
	char *key;
	char *value;
}KeyValuePair;


/**
 * 在获取Bucket列表时返回的Bucket信息
 */
typedef struct _HCSBucketInfo
{
	char *create_date;							//Bucket的创建时间
	char *bucket_name;							//Bucket的名称
	char *owner;								//Bucket的所有者
	char *region;								//Bucket所在的区域信息
	HCSAccessACL acl;							//Bucket的ACL
	HCSBucketVersionManage version_policy;		//Bucket的多版本管理策略
	int life_cycle;								//Bucket的生命周期,单位为天(当未设置,值为-1,该值在bucket开启版本管理之后不能设置)
	int trash_del_cycle;						//Bucket的回收站周期,单位为天(当未设置,值为0,该值表示在object存储周期到期之后能在回收站中存储的时长)
	long long bucket_size;						//Bucket的容量,单位为GB
	double free_size;				            //Bucket的剩余容量,单位为GB
}HCSBucketInfo;


/**
 * Object的用户自定义元数据
 */
typedef struct _HCSObjectUserDefineMetadata
{
	int count;
	KeyValuePair *key_value_pair;
}HCSObjectUserDefineMetadata;


/**
 * Object的系统定义元数据,如object大小、存储类别等
 */
typedef struct _HCSObjectSystemDefineMetadata
{
	int count;
	KeyValuePair *key_value_pair;
}HCSObjectSystemDefineMetadata;


/**
 * Object的所有元数据信息
 */
typedef struct _HCSObjectMetadata
{
	HCSObjectUserDefineMetadata *user_def;		//用户自定义元数据集合
	HCSObjectSystemDefineMetadata *sys_def;		//系统定义元数据集合
}HCSObjectMetadata;


/**
 * Object的简要信息
 */
typedef struct _HCSObjectSummary
{
	char *key;					//Object的名称
	char *version_id;			//Object对应的版本ID
	int status;					//Object对应在hcs中的状态
	HCSAccessACL acl;			//Object对应的ACL值
	char *storage_class;		//Object的存储类型
	char *create_time;			//Object对应的创建时间
	char *user;					//创建该object的用户名
	long long size;				//Object的大小
}HCSObjectSummary;


/**
 * Object的列表,在获取Object列表时返回
 */
typedef struct _HCSObjectList
{
	char *bucket_name;			//Object所在的Bucket名称
	char *delimiter;			//指定的分组符
	char *prefix;				//指定的Object的前缀
	int   max_keys;				//指定一次返回的最大条数
	char *marker;				//指定Object的key的开始标志

	bool is_truncated;			//标识是否有未显示的Object
	char *next_marker;			//返回下一集合的marker,当用户设置了delimiter,且is_truncated为true时才有值
	
	long summaries_count;		 //所有满足prefix的Object条数
	HCSObjectSummary **summaries;//所有满足prefix的Object信息
	
	long common_prefix_count;	 //指定前缀且第一次出现delimiter之间的object个数
	char **common_prefixes;      //指定前缀且第一次出现delimiter之间的object key集合
}HCSObjectList;


/**
 * 下载Object请求参数结构体
 */
typedef struct _HCSGetObjectRequest
{
	char *bucket_name;			  //Object所在的Bucket名称
	char *key;					  //Object名称
	char *version_id;			  //object对应的版本信息,为NULL则获取最新Object数据
	long long start_pos;		  //需要获取的开始偏移,单位为字节
	long long end_pos;            //需要获取的结束偏移位置,单位为字节
}HCSGetObjectRequest;


/**
 * 获取Object列表请求参数结构体
 */
typedef struct _HCSListObjectRequest
{
	char *delimiter;			  //分隔符
	char *marker;                 //标识表示从哪里返回列表
	unsigned int max_keys;        //限制的最大返回记录数
	char *prefix;                 //过滤object名称的前缀(如 aaa.txt,若前缀设置成a或aa或aaa则aaa.txt会被返回)
}HCSListObjectRequest;


/**
 * 包含Multipart上传的Part的返回结果信息
 */
typedef struct _HCSMultipartUploadPart
{
	int part_number;        //Part标识号码
	int part_size;			//Part对应的大小,以字节为单位
}HCSMultipartUploadPart;


/**
 * 包含完成一个Multipart上传事件的请求参数
 */
typedef struct _HCSCompleteMultipartUploadRequest
{
	char *bucket_name;						//Bucket 名称
	char *key;								//Object 名称
	HCSMultipartUploadPart *part_info;      //标识上传Part的结果
	char *upload_id;						//标识Multipart上传事件的uploadId
	int part_info_num;						//part_info的个数
}HCSCompleteMultipartUploadRequest;


/**
 * Object对应的时间段的列表,仅流式数据类型的object使用该接口才有效
 */
typedef struct _HCSTimeSegmentInfo
{
	long long  size;				//满足查询条件的时间段的大小，如果标记为1，为总大小，如果为0，显示为每个段的大小
	long long start_time;		 //满足查询条件的时间段的开始时间
	long long end_time;		     //满足查询条件的时间段的结束时间
}HCSTimeSegmentInfo;


/**
 * Object对应的时间段的列表,仅流式数据类型的object使用该接口才有效
 */
typedef struct _HCSStreamTimeSegmentList
{
	long segment_count;		 //所有满足开始时间与结束时间的时间段数量
	HCSTimeSegmentInfo *segment_list;
}HCSStreamTimeSegmentList;


/*typedef struct _HCSTimeSegmentInfoEx
{
	long long  size;				//单个短的大小
	long long start_time;		 
	long long end_time;		     //满足查询条件的时间段的结束时间
}HCSTimeSegmentInfoEx;

*/
/**
 * Object对应的时间段的列表,仅流式数据类型的object使用该接口才有效
 */
typedef struct _HCSStreamTimeSegmentListEx
{
	long segment_count;		 //所有满足开始时间与结束时间的时间段数量 标记为1,显示只有一个段
	HCSTimeSegmentInfo *segment_list;//标记为1，里面只存储一个段，且段的大小为所有段的总和；
}HCSStreamTimeSegmentListEx;


/**
 * 下发转码的参数集合
 */
typedef struct _HCSTranscodeParam
{
	char* output_bucket;//转码之后的输出对象bucket
	char* output_key;	//转码之后的输出对象key
	int packet_type;	//参考HCSOutputPacketType类型值说明
	int video_format;	//参考HCSVideoEncodeFormat类型值说明
	int audio_format;	//参考HCSAudioEncodeFormat类型值说明
	int frame_width;	//输出帧宽
	int frame_height;	//输出帧高
}HCSTranscodeParam;


/**
 * 转码任务进度
 */
typedef struct _HCSTranscodeProgress
{
	char* output_bucket;//转码之后的输出对象bucket
	char* output_key;	//转码之后的输出对象key
	int status;	        //转码状态
	int progress;	    //转码进度
}HCSTranscodeProgress;



/**
 * 取流设备信息结构体
 */
typedef struct _HCSStreamDevInfo
{
	char *user_name;
	char *passwd;
	char *url;
	int reserved1;
	char *reserved2;
}HCSStreamDevInfo;



#ifdef __cplusplus
extern "C"
{
#endif

    /** 
	*  根据开始时间结束时间下载流式数据回调函数定义
	*  @param buff[out]         录像数据
	*  @param bufflen[out]      数据长度 
	*  @param offset[out]       当前数据在整个待下载数据中的偏移
	*  @param error[out]        等于0: 当前下载成功; 小于0: 表示下载出现错误,值为具体出错错误码
	*  @param pUser[out]        用户自定义数据 
	*  @@return 等于0: 数据已处理; 小于0: 数据未处理
	*/
	typedef int (*DownloadStreamDataCb)(const char* buff, int bufflen, long long offset, int error, void* p_user);
	
	  /** 
	*  普通文件下载
	*  @param buff[out]         录像数据
	*  @param bufflen[out]      数据长度 
	*  @param offset[out]       当前数据在整个待下载数据中的偏移
	*  @param error[out]        等于0: 当前下载成功; 小于0: 表示下载出现错误,值为具体出错错误码
	*  @param pUser[out]        用户自定义数据 
	*  @@return 等于0: 数据已处理; 小于0: 数据未处理
	*/
	typedef int (*DownloadNormalDataCb)(const char* buff, int bufflen,int error, void* p_user);
	/**
	 * HCSClient 的构造函数
	 * @param access_id [in]  用户注册的对象存储服务用户名
	 * @param access_key [in] 用户注册的对象存储服务密码
	 * @param endpoint [in]   云存储服务的域名或IP:PORT，如www.hcs.com或192.168.1.1:80
	 * @return 返回一个标识该用户身份的结构指针
	 * @retval 非空 表示成功
	 * @retval NULL 表示失败
	 */
	HCS_OBJECT_API HCSClient *ClientInitialize(const char *access_key, const char *secret_key, const char *endpoint);

	/**
	 * HCSClient 析构函数
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @retval void
	 * @pre client 必须使用ClientInitialize的返回值
	 */
	HCS_OBJECT_API void ClientFinalize(HCSClient *client);

	/**
	 * 创建 Bucket
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要创建的Bucket的名称
	 * @param region [in] Bucket里面的数据需要存储的区域,目前只支持"LOCAL-1"
	 * @param bucket_size [in] Bucket的容量,单位为GB
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int CreateBucket(HCSClient *client, const char *bucket_name, const char* region, long long bucket_size);
	
    /**
	 * 设置bucket的容量
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要设置的Bucket的名称
	 * @param bucket_size [in] 修改之后的容量值,单位为GB
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int SetBucketSize(HCSClient *client, const char *bucket_name, long long bucket_size);

	/**
	 * 设置bucket的版本号管理策略
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要设置的Bucket的名称
	 * @param policy [in] 修改之后的版本号管理策略
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int SetBucketVersionPolicy(HCSClient *client, const char *bucket_name, HCSBucketVersionManage policy);

	/**
	 * 设置bucket下object生命周期
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要设置的Bucket的名称
	 * @param expiration [in] bucket下的object保存周期,单位为天,默认不过期,设置为-1则表示永不过期,若对应的bucket开启了版本管理，则不能设置该规则，云存储会返回对应错误
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int SetBucketLifecycle(HCSClient *client, const char *bucket_name, int expiration);

	/**
	 * 设置bucket下回收站的存储周期,表示bucket下的object的存储周期到达之后能临时在回收站中存储的时长
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要设置的Bucket的名称
	 * @param expiration [in] bucket下的回收站的存储周期,单位为天,默认是0,表示不经过回收站动作(bucket存储周期到期后object立即被物理删除)
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int SetBucketTrashDelcycle(HCSClient *client, const char *bucket_name, int expiration);

	/**
	 * 删除 Bucket,当bucket下面没有任何object时才能删除
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要删除的Bucket的名称
	 * @retval 0    表示成功
	 * @retval 非0  表示失败
	 */
	HCS_OBJECT_API int DeleteBucket(HCSClient *client, const char *bucket_name);

	/**
	 * 返回请求者拥有的所有 Bucket 的列表
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param buckets_number [out] 所有Bucket的总数
	 * @buckets_list [out] 返回一个HCSBucketInfo的双重指针，可通过buckets_num来遍历,遍历完成之后用户需释放该参数内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int ListBuckets(HCSClient *client, int *buckets_num, HCSBucketInfo **buckets_list);

	/** FreeHCSBucketInfo
	 *  释放库中动态申请的资源
	 *  @param client[in] 客户连接句柄
	 *  @param buckets_num[in out] 资源个数
	 *  @param buckets_info[in out] 待释放的资源
	 *  @return 成功返回 HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeHCSBucketInfo(HCSClient *client, int *buckets_num, HCSBucketInfo** buckets_info);

	/**
	 * 设置指定 Bucket 的 Access Control List(ACL)
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name 要设置权限的Bucket的名称
	 * @param acl 要设置的权限
	 * @retval 0     成功
	 * @retval 其他  失败
	 */
	HCS_OBJECT_API int SetBucketAcl(HCSClient *client, const char *bucket_name, HCSAccessACL acl);

	/**
	 * 返回给定 Bucket 的 详细信息
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param bucket_info [out] 返回的Bucket的详细信息,获取完成用户需释放该参数内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int GetBucketInfo(HCSClient *client, const char *bucket_name, HCSBucketInfo **bucket_info);

	/**
	 * 上传指定的object到指定的 Bucket,注意该接口只能上传小于100M的文件,大于100M的文件建议采用分片上传接口
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param key [in] 要上传到服务器上显示的Object的名称
	 * @param file_handle [in] 要上传的文件句柄
	 * @param metadata [in] 标识用户自定义的object 的一些元数据信息
	 * @param storage_class [in] 标识object的存储类型,目前只支持"STANDARD"
	 * @param object_type [in] 标识object的类型,如"application/xml",赋值为空则默认为"application/octet-stream"
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int PutObjectFromFile(HCSClient *client, const char *bucket_name, const char *key, FILE *file_handle, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type);

	/**
	 * 上传指定的object到指定的 Bucket
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param key [in] 要上传到服务器上显示的Object的名称
	 * @param data [in] 要上传的数据
	 * @param data_len [in] 要上传的数据的长度
	 * @param metadata [in] 标识object 的一些元信息
	 * @param storage_class [in] 标识object的存储类型,目前只支持"STANDARD"
	 * @param object_type [in] 标识object的类型,如"application/xml",赋值为空则默认为"application/octet-stream"
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int PutObjectFromBuffer(HCSClient *client, const char *bucket_name, const char *key, void *data, long long data_len, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type);

	/**
	 * 设置指定 object 的 Access Control List(ACL)
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name 要设置权限的Bucket的名称
	 * @param key 要设置的object的key值
	 * @retval version_id     需要设置的object的对应版本号,若设置为NULL,则表示更改当前最新版本的ACL值
	 * @param acl 要设置的权限
	 * @retval 其他  失败
	 */
	HCS_OBJECT_API int SetObjectAcl(HCSClient *client, const char *bucket_name, const char *key, const char *version_id, HCSAccessACL acl);

	/**
	 * 返回给定 object 的所有简要信息,若bucket的版本管理策略已经打开,则也会返回对应历史版本信息
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param key 要获取的object的key值
	 * @param list_count 获取成功的object条数
	 * @retval summary     获取之后的摘要信息,获取完成之后用户需要释放该指针指向的所有内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int ListObjectSummary(HCSClient *client, const char *bucket_name, const char *key, int *list_count, HCSObjectSummary **summary);

	/**
	 * 显示调用释放库中动态申请的HCSObjectSummary资源
	 * @param list_count[in out] 对象的个数
	 * @param summary[in out] 对象的存放地址的指针
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeHCSObjectSummary(HCSClient *client, int *list_count, HCSObjectSummary **summary);

	/**
	 * 按参数设置返回bucket下面的所有object信息,该接口只返回最新版本object信息,如果想获取object的历史数据,请调用ListObjectSummary接口
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param list_params [in] 获取列表时设置的参数
	 * @retval summary     获取之后的摘要信息,获取完成之后用户需要释放该指针指向的所有内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int ListObjects(HCSClient *client, const char *bucket_name, HCSListObjectRequest *list_params, HCSObjectList **list);

	/**
	 * 显示调用释放库中动态申请的HCSObjectList资源
	 * @param client 用户连接句柄
	 * @param summary[in out] 对象的存放地址的指针
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeHCSObjectList(HCSClient *client, HCSObjectList **summary);

	/**
	 * 返回 Object 的元数据
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name 要查询的Object所在的Bucket名称
	 * @param key 要查询的Object名称
	 * @param version_id 要获取的object对应的版本号,若设置为NULL,则返回最新object的metadata
	 * @param metadata [out] 返回的object元数据信息,获取条数大于0则用户需释放该内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int GetObjectMetadata(HCSClient *client, const char *bucket_name, const char *key, const char *version_id, HCSObjectMetadata **metadata);

	/**
	 * 显示调用释放库中动态申请的HCSObjectMetadata资源
	 * @param client 用户连接句柄
	 * @param metadata[in out] 待释放的元数据资源
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeHCSObjectMetadata(HCSClient *client, HCSObjectMetadata **metadata);


	/**
	 * 从指定的 Bucket 中导出指定的Object到目标文件
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param request [in] 需要获取的object的一些条件参数
	 * @param file_handle [in] 要存放Object的文件指针
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int AsyncGetObject(HCSClient *client,HCSGetObjectRequest *request, DownloadNormalDataCb download_callbk,void *p_user);


	/**
	 * 从指定的 Bucket 中导出指定的Object到目标文件
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param request [in] 需要获取的object的一些条件参数
	 * @param file_handle [in] 要存放Object的文件指针
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int GetObjectToFile(HCSClient *client,HCSGetObjectRequest *request, void *file_handle);

	/**
	 * 从指定的Bucket中导出指定的Object 到内存
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param request [in] 需要获取的object的一些条件参数
	 * @param output [in] 要存放Object的内存地址,该内存由hcs申请,成功后由用户释放
	 * @param output_len [out] 输入值为缓冲区大小，输出值为实际返回的数据长度
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int GetObjectToBuffer(HCSClient *client, HCSGetObjectRequest *request, void **output, unsigned int *output_len);

	/**
	 * 显示调用释放库中动态申请的buffer资源
	 * @param client[in] 用户连接句柄
	 * @param output[in out] 待释放的buffer资源
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeBuffer(HCSClient *client, void **output);

	/**
	 * 拷贝一个在云存储上已经存在的 Object 成另外一个 Object
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param source_bucket_name 被拷贝的Object所在的Bucket的名称
	 * @param source_key 被拷贝的Object的名称
	 * @param destination_bucket_name 目标Object所在的Bucket的名称
	 * @param destination_key 目标Object的名称
	 * @param metadata 目标Object的元数据定义
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int CopyObject(HCSClient *client, const char *source_bucket_name, const char *source_key, const char *destination_bucket_name, const char *destination_key, HCSObjectUserDefineMetadata *metadata);

	/**
	 * 删除指定的 Object
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要删除Object所在的Bucket的名称
	 * @param key [in] 要删除Object的名称
	 * @param version_id [in] 要删除Object的版本ID，为NULL则表示删除最新版本(可以通过list_object获得，或者通过get_object中的metadata中获得)
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int DeleteObjectByName(HCSClient *client, const char *bucket_name, const char *key, const char *version_id);

	/**
	 * 初始化一个multipart上传任务
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] bucket的名字
	 * @param key [in]  object的key值
	 * @param metadata [in]  object的用户自定义元数据
	 * @param storage_class [in]  object的存储类型,目前只支持"STANDARD"
	 * @param object_type [in] 标识object的类型,如"application/xml",赋值为空则默认为"application/octet-stream"
	 * @param error_code [out]  接口返回错误码. 0:成功 其他值:失败
	 * @retval 非空 表示成功，且返回任务ID
	 * @retval NULL 表示失败
	 */
	HCS_OBJECT_API char *InitiateMultipartUpload(HCSClient *client, const char *bucket_name, const char *key, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type, int *error_code);

	/**
	 * 上传一个分块(part  针对大对象)
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] bucket的名字
	 * @param key [in]  object的key值
	 * @param task_id [in] initiate_multipart_upload返回
	 * @param part_index [in] 要上传的part索引
	 * @param part_data [in] 要上传的part的数据(推荐2M大小，若为最后一片，长度任意)
	 * @param part_data_len [in] part数据的长度
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int UploadPart(HCSClient *client, const char *bucket_name, const char *key, const char *task_id, int part_index, void *part_data, int part_data_len);

	/**
	 * 终止一个multipart上传任务
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] bucket的名字
	 * @param key [in]  object的key值
	 * @param task_id [in] initiate_multipart_upload返回
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int AbortMultipartUpload(HCSClient *client, const char *bucket_name, const char *key, const char *task_id);

	/**
	 * 释放multipart上传任务
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param task_id [in] initiate_multipart_upload返回
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int MultipartUploadFinalize(HCSClient *client, HCSCompleteMultipartUploadRequest *request);

	/**
	 * 获取已经上传成功的分片信息
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] bucket的名字
	 * @param key [in]  object的key值
	 * @param task_id [in] initiate_multipart_upload返回
	 * @param result [out] 所有分片信息的容器,由hcs赋值申请内存,由调用者释放内存(free)
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int ListPart(HCSClient *client, const char *bucket_name, const char *key, const char *task_id, HCSCompleteMultipartUploadRequest **result);

	/**
	 * 显示调用释放库中动态申请的HCSCompleteMultipartUploadRequest资源
	 * @param client[in] 用户连接句柄
	 * @param result[in] 待释放的HCSCompleteMultipartUploadRequest资源
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeHCSCompleteMultipartUploadRequest(HCSClient *client, HCSCompleteMultipartUploadRequest **result);

    /**
	 * object数据追加
	 * @param client[in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param object_key [in] 要上传到服务器上显示的Object的名称
	 * @param data [in] 要上传的数据
	 * @param data_len [in] 要上传的数据的长度，长度小于0 或者 大于16MB时候，函数会直接返回 INVALID_ARGUMENT
	 * @param position[in] 该数据在整个object中的开始位置,单位为字节
	 * @param next_position[out] 下次用户应该从何处开始执行append操作
	 * @return 成功返回HCS_OK， 失败返回错误码。成功 或者 错误码为 PositionNotEqualToLength 时候，next_position表示了下次append的正确position。
	 */
	HCS_OBJECT_API int AppendObject(HCSClient* client, const char* bucket_name, const char* object_key, const char* data, int data_len, long long position, long long* next_position);

	/**
	 * 打开一个流式句柄,该接口针对视频数据上传应用
	 * @param client[in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param object_key [in] 要上传到服务器上显示的Object的名称
	 * @param stream_handle [out] 云存储返回的流句柄,该内存由调用者申请,流句柄是一个不超过64个字符的字符串
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int OpenStream(HCSClient* client, const char* bucket_name, const char* object_key, char* stream_handle);

	/**
	 * 开始上传流数据
	 * @param client[in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param object_key [in] 要上传到服务器上显示的Object的名称
	 * @param stream_handle [in] 云存储返回的流句柄,该参数由调用OpenStream接口获得
	 * @param stream_data [in] 需要上传的数据
	 * @param data_len [in] 需要上传的数据的长度，长度小于0 或者 大于16MB时候，函数会直接返回 INVALID_ARGUMENT
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int UploadStreamData(HCSClient* client, const char* bucket_name, const char* object_key, const char* stream_handle, const char* stream_data, int data_len);

	/**
	 * 关闭一个流式句柄,若用户停止流式文件写入时调用
	 * @param client[in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要上传到的Bucket名称
	 * @param object_key [in] 要上传到服务器上显示的Object的名称
	 * @param stream_handle [in] 流句柄,该参数由调用OpenStream接口获得
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int CloseStream(HCSClient* client, const char* bucket_name, const char* object_key, const char* stream_handle);

	/**
	 * 通过时间段范围检索该object下所有的时间段信息
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称
	 * @param start_time [in] 要获取的object的开始时间
	 * @param end_time [in] 要获取的object的结束时间
	 * @param segment_list [out] 获取到的所有时间段信息，获取完成之后用户需要释放该指针指向的所有内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int QueryObjectTimesegment(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSStreamTimeSegmentList **segment_list);
	/**
	 * 通过时间段范围检索该object下所有的时间段信息
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称
	 * @param start_time [in] 要获取的object的开始时间
	 * @param end_time [in] 要获取的object的结束时间
	 * @param segment_list [out] 获取到的所有时间段信息，获取完成之后用户需要释放该指针指向的所有内存
	 * @param  flag [int] 传递类型，1返回一个，0返回多个。
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int QueryObjectTimesegmentEx(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSStreamTimeSegmentListEx **segment_list,int flag);
    /**
	 * 显示释放库中动态申请的HCSStreamTimeSegmentList资源
	 * @param client[in] 用户连接句柄
	 * @param segment_list[in] 待释放的HCSStreamTimeSegmentList资源
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeStreamTimeSegmentList(HCSClient *client, HCSStreamTimeSegmentList **segment_list);
	HCS_OBJECT_API int FreeStreamTimeSegmentListEx(HCSClient *client, HCSStreamTimeSegmentListEx **segment_list);

	/**
	 * 通过时间段范围下载Object数据
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称
	 * @param start_time [in] 要获取的object的开始时间
	 * @param end_time [in] 要获取的object的结束时间
	 * @param segment_list [out] 获取到的所有时间段信息，获取完成之后用户需要释放该指针指向的所有内存
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int DownloadStreamData(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSVideoOutPutFormat output_format, DownloadStreamDataCb download_callbk, void *p_user);

	/**
	 * 下发转码任务接口
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称
	 * @param param_list [in] 输出流参数列表
	 * @param param_count [in] 输出流参数列表的元素个数
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int SendTranscodeTask(HCSClient *client, const char *bucket_name, const char *object_key, HCSTranscodeParam *param_list, int param_count);

	/**
	* 下发转码任务接口
	* @param client [in] 标识一个HCSClient的结构指针
	* @param bucket_name [in] 要获取的Bucket名称
	* @param object_key [in] 要获取的object名称
	* @param param_list [in] 输出流参数列表
	* @param param_count [in] 输出流参数列表的元素个数
	* @retval 0	表示成功
	* @retval 其他 表示失败
	*/
	HCS_OBJECT_API int SendTranscodeTaskEx(HCSClient *client, const char *bucket_name, const char *object_key,const  char *callback_ip_port, HCSTranscodeParam *param_list, int param_count);
	/**
	 * 删除转码接口
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要删除转码的Bucket名称，该值是下发转码任务中的源bucket
	 * @param object_key [in] 要删除转码的object名称,该值是下发转码任务中的源key
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int DeleteTranscodeTask(HCSClient *client, const char *bucket_name, const char *object_key);

	/**
	 * 查询转码状态与进度接口
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称,该值填入下发转码任务参数中的output_key
	 * @param output_key [in] 转码之后的对象对应的key
	 * @param progress [out] 该key对应的转码进度列表结构体
	 * @param count [out] 进度列表结构体数量
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	
	HCS_OBJECT_API int QueryTranscodeTaskProgress(HCSClient *client, const char *bucket_name, const char *object_key, HCSTranscodeProgress** progress, int* count);

	/**
	 * 显示释放库中动态申请的HCSTranscodeProgress资源
	 * @param client[in] 用户连接句柄
	 * @param progress[in out] 待释放的HCSTranscodeProgress资源列表
	 * @param count[in out] 待释放HCSTranscodeProgress结构体的个数
	 * @return 成功返回HCS_OK， 失败返回错误码
	 */
	HCS_OBJECT_API int FreeTranscodeProgress(HCSClient *client, HCSTranscodeProgress** progress, int* count);

	/**
	 * 指定云存储按照宽高或者格式返回图片数据
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 要获取的Bucket名称
	 * @param object_key [in] 要获取的object名称
	 * @param dest_width [in] 要返回的图片数据的宽
	 * @param dest_high [in] 要返回的图片数据的高
	 * @param change_format [in] 要返回的图片数据的格式
	 * @param image_data [out] 云存储返回的具体的图片数据,内存由用户释放
	 * @param image_data_len [out] 云存储返回的图片数据的长度
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int GetImageDataWithWidthAndHigh(HCSClient *client, const char *bucket_name, const char *object_key, int dest_width, int dest_high, HCSImageOutputFormat change_format, char **image_data, unsigned int *image_data_len);

	/**
	 * 下发开启手动录像命令
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 手动录像存储的Bucket名称
	 * @param object_key [in] 手动录像存储的object名称
	 * @param param [in] 取流设备信息
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int StartManualRecord(HCSClient *client, const char *bucket_name, const char *object_key, HCSStreamDevInfo *param);
	/**
	 * 下发开启手动录像命令
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 手动录像存储的Bucket名称
	 * @param object_key [in] 手动录像存储的object名称
	 * @param callback_ip [in] 手动录像存储的下发的ip
	 * @param param [in] 取流设备信息
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int StartManualRecordEx(HCSClient *client, const char *bucket_name, const char *object_key,const  char *callback_ip_port, HCSStreamDevInfo *param);	

	/**
	 * 下发开启手动补录命令,主要针对历史数据
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 手动补录的数据所存储的Bucket名称
	 * @param object_key [in] 手动补录的数据所存储的object名称
	 * @param param [in] 取流设备信息
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int StartManualHistoryRecord(HCSClient *client, const char *bucket_name, const char *object_key, HCSStreamDevInfo *param);

	/**
	 * 下发开启手动补录命令,主要针对历史数据
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] 手动补录的数据所存储的Bucket名称
	 * @param object_key [in] 手动补录的数据所存储的object名称
	 * @param param [in] 取流设备信息
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int StartManualHistoryRecordEx(HCSClient *client, const char *bucket_name, const char *object_key, const  char *callback_ip_port,HCSStreamDevInfo *param);
	/**
	 * 下发停止录像命令,该接口可以停止手动录像或者手动补录
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] Bucket名称
	 * @param object_key [in] object名称
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int StopRecord(HCSClient *client, const char *bucket_name, const char *object_key);

	/**
	 * 查询录像状态,该接口可以查询录像或者手动补录任务状态
	 * @param client [in] 标识一个HCSClient的结构指针
	 * @param bucket_name [in] Bucket名称
	 * @param object_key [in] object名称
	 * @param status [out] 返回给用户的状态值
	 * @retval 0    表示成功
	 * @retval 其他 表示失败
	 */
	HCS_OBJECT_API int QueryRecordStatus(HCSClient *client, const char *bucket_name, const char *object_key, int* status);

    /**
	 * 获取hcs对外接口版本号信息
	 * @retval 非空    表示成功
	 * @retval 其他    表示失败
	 */
	HCS_OBJECT_API const char * const GetVersions();

#ifdef __cplusplus
}//end of extern "C"


#endif

#endif //_HCS_OBJECT_API_
