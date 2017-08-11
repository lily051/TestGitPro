#ifndef  CLOUD_API_PUBLIC_H_H
#define  CLOUD_API_PUBLIC_H_H
//lint -library
#include <string.h>
#define CAMERAID_LEN         72    // 定义编码器字符长度
#define IPLength             20    // 定义IP地址信息长度
#define DeviceIDLength       48    // 定义设备ID长度
#define MAX_IP_LEN				64

/**  @enum PlayType  [CloudApiDefine.h]
*  @brief 录像回放的类型枚举量
*/
enum PlayType
{
	PlayNormal = 1,                          // 顺序回放
	PlayNormalIFrame = 3,                    // 正常I帧回放
	PlayReindex = 5,                         // 倒序回放
};



/**  @enum RecordLockType  [CloudApiDefine.h]
*  @brief 录像锁定类型
*
*/
enum RecordLockType
{  
	TypeAll = 0,	                          // 全部（包括锁定的录像和没有锁定的录像）
	TypeUnlock = 1,                         // 没有锁定(可以对当前录像执行所有操作)
	TypeLock = 2,	                          // 锁定(当前录像不允许覆盖，但可以回放、下载和删除操作)

};

/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief 录像池信息数据结构定义
*/
typedef struct tagCloudPoolInfo
{
	tagCloudPoolInfo()
	{
		memset(PoolName,0,sizeof(PoolName));
		PoolType =0;
		PoolTimeCycle=0;
		PoolSize=0;
		Disperse_Type=0;
		CoverType=0;
		LockLimit=0;
		PoolRate=0;
	}

	char  PoolName[100] ;	                // 录像池名称 [备注: 不支持中文]
	int   PoolType ;			            // 录像池类型 [0: 通用存储、1: 视频、2: 附加段信息、3: 图片信息]
	unsigned int PoolTimeCycle;             // 录像池覆盖周期 [单位: 天、范围: 大于0]
	unsigned int PoolSize ;                 // 录像池大小 [单位: MB]
	int   Disperse_Type;                    // 分散策略 [0: 集中、1: 完全分散]
	int   CoverType;                        // 覆盖策略类型 [0: 不覆盖、1:按容量覆盖、2:周期覆盖]
	int   LockLimit;                        // 锁定时长,从当前时间起到录像解锁时的时间间隔[单位: 秒]
	int   PoolRate;			                // 码率[单位: mb/s]
}CloudPoolInfo;

/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief 录像池信息数据结构定义
*/
typedef struct tagCloudPoolInfoEx
{
	tagCloudPoolInfoEx()
	{
		PoolID = 0 ;
		FreeSize=0;
		memset(&PoolInfo,0,sizeof(PoolInfo));
	}

	unsigned int PoolID;	                  // 录像池ID
	unsigned int FreeSize ;                   // 录像池剩余空间大小[单位: MB]
	unsigned int BeginTime;	                  // 最早时间点
	unsigned int CoverTime;                   // 覆盖时间点
	CloudPoolInfo PoolInfo ;                  // 录像池基本信息	
}CloudPoolInfoEx;


/**  @struct tagLableInfo  [CloudApiDefine.h]
*  @brief 标签信息
*
*   包括标签内、摄像头、开始时间、结束时间和标签内容
*/
typedef struct tagLabelInfo
{
	tagLabelInfo()
	{
		BeginTime = 0 ;
		EndTime = 0 ;
		memset(Camera,0,sizeof(Camera));
		memset(LableContext,0,sizeof(LableContext));
	}

	tagLabelInfo(tagLabelInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime = tag.BeginTime;
		EndTime = tag.EndTime ;
		memcpy(LableContext,tag.LableContext,sizeof(tag.LableContext));
	}

	tagLabelInfo& operator=(tagLabelInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime = tag.BeginTime;
		EndTime = tag.EndTime ;
		memcpy(LableContext,tag.LableContext,sizeof(tag.LableContext));
		return *this;
	}

	char Camera[CAMERAID_LEN];		            // 编码器名称
	unsigned int BeginTime;                     // 开始时间
	unsigned int EndTime ;                      // 结束时间
	char LableContext[1024];                    // 标签内容
}LabelInfo;


/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief 录像信息数据结构定义
*/
typedef struct  tagCloudRecordInfo
{
	tagCloudRecordInfo()
	{
		memset(Camera,0,sizeof(Camera) );
		BeginTime=0;
		EndTime=0;
		Type=0;
		Lock=0;
		Size=0;
		status = 0;
        Backup = 0;
	}

	tagCloudRecordInfo(tagCloudRecordInfo&tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime=tag.BeginTime;
		EndTime=tag.EndTime;
		Type=tag.Type;
		Lock=tag.Lock;
		Size=tag.Size;
		memcpy(VSIP,tag.VSIP,sizeof(tag.VSIP));
		memcpy(DeviceID,tag.DeviceID,sizeof(tag.DeviceID));
		BlockID=tag.BlockID;
		SegmentID=tag.SegmentID;
		status = tag.status;
        Backup = tag.Backup;
	}

	tagCloudRecordInfo& operator=(tagCloudRecordInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime=tag.BeginTime;
		EndTime=tag.EndTime;
		Type=tag.Type;
		Lock=tag.Lock;
		Size=tag.Size;
		memcpy(VSIP,tag.VSIP,sizeof(tag.VSIP));
		memcpy(DeviceID,tag.DeviceID,sizeof(tag.DeviceID));
		BlockID=tag.BlockID;
		SegmentID=tag.SegmentID;
		status = tag.status;
        Backup = tag.Backup;
		return *this;
	}

	char  Camera[CAMERAID_LEN];	          // 编码器ID
	unsigned int  BeginTime ;	          // 录像开始时间
	unsigned int  EndTime;	              // 录像结束时间
	int   Type;	                          // 录像类型
	int	  Lock;	                          // 录像锁定状态 [0: 全部类型(包括锁定的录像和没有锁定的录像)、1: 没有锁定、2: 锁定]
	int	  Size;                           // 录像大小[单位: 字节]
	char  VSIP[IPLength];		          // 录像服务器IP (VSIP)
	char  DeviceID[DeviceIDLength];       // 存储设备ID
	unsigned int BlockID;		          // 块ID
	unsigned int SegmentID;		          // 录像序列号
	int status;                           // 当前录像可用状态[0: 正常、1: VS不在线、2: 设备不可用、3: 设备不存在]
    unsigned int Backup;                 // 备份标志[0 – 原始数据，1 – 备份数据]
}CloudRecordInfo;

//时间段(子结构)
typedef struct _sdk_schedtime
{
    //开始时间
    unsigned char byStartHour;
    unsigned char byStartMin;

    //结束时间
    unsigned char byStopHour;
    unsigned char byStopMin;
}SDK_SCHEDTIME, *LPSDK_SCHEDTIME;

/*下发备份任务*/
typedef struct strBackupPlan
{
    unsigned int BeginTime; 	// 备份的开始时间
    unsigned int EndTime; 		// 备份的结束时间
    int BlockType;				// 数据类型 1：录像，2：附属，3：图片
    char LocalHost[MAX_IP_LEN];	// 本地IP
    int LocalPort1; 				// 本地端口
    int LocalPort2; 				// 本地端口
    char RemoteHost[MAX_IP_LEN]; 	// 远端IP
    int RemotePort1; 				// 远端端口
    int RemotePort2; 				// 远端端口
    int PoolID;					// 需要备份到指定录像池
    int PoolType;					// 需要备份到指定录像池的类型
    _sdk_schedtime SchedTime;	// 可以执行此任务的开始时间，0-23
}SDK_BACKUPPLAN, *LPSDK_BACKUPPLAN;


#ifdef _WIN32
#ifdef CLOUD_API_EXPORTS
#define CLOUD_API __declspec(dllexport)
#else
#define CLOUD_API __declspec(dllimport)
#endif
#else
#define CLOUD_API
#endif
extern "C"
{

	/** 
	*  @brief API事件回调函数定义.包含网络错误,程序异常,消息上报
	*  @param LogicType[in] API的模块序号 0:验证,1:录像(包含标注),2:图片,3:附属信息,4:录像池管理
	*  @param ErrorCode[in] 错误码
	*  @param pUser[in] 用户自定义数据 
	*  @@return =0:数据已处理;<0:数据未处理
	*/
	typedef int (*CUserMessage)(int LogicType,int ErrorCode,void* pUser);


	/** 
	*  @brief 录像池列表查询结果回调函数定义
	*  @param info[out] 录像池信息 
	*  @param Count[out] 录像池查询结果总数量
	*  @param Flat[out]  剩余录像池总数 [等于0: 表示录像池全部返回、 小于0: 为错误码、大于0: 表示剩余的录像池总数]
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	
	
	*  备注：当Flat标示符为0，Count标示符不为0时，表示获取当前云存储系统的录像池信息结束
	*        当Flat标示符为0，Count标示符也为0时，表示回调停止
	*/
	typedef int (*CPoolListDataCb)(CloudPoolInfoEx info, int Count, int Flat, void* pUser);


	/** 
	*  @brief 录像查询结果回调函数定义
	*  @param info[out] 录像信息 
	*  @param Count[out] 查询录像结果总数量
	*  @param Flat[out]  剩余录像信息数量 [等于0: 表示录像信息全部返回、 小于0: 为错误码、大于0: 表示剩余的录像总数]
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理; 小于0:数据未处理
	
	*  备注：当Flat标示符为0，Count标示符不为0时，表示获取当前编码器对应的录像信息结束
	*        当Flat标示符为0，Count标示符也为0时，表示回调停止
	*/
	typedef int (*CSearchDataCb)(CloudRecordInfo info, int Count, int Flat, void* pUser);


	/** 
	*  @brief 录像下载数据回调函数定义
	*  @param buff[in] 录像数据
	*  @param bufflen[in] 数据长度 
	*  @param Flat[in]  =0:下载结束 <0为错误码
	*  @param pUser[in] 用户自定义数据 
	*  @@return =0:数据已处理;<0:数据未处理
	
	*  备注：当Flat标示符为0，Count标示符不为0时，表示下载当前编码器对应的录像结束
	*        当Flat标示符为0，Count标示符也为0时，表示回调停止
	*/
	typedef int (*CDownloadDataCb)(const char* buff ,int bufflen,int Flat,void* pUser);



	/** 
	*  @brief 标签信息查询回调函数定义
	*  @param Info[in]  标签信息 
	*  @param Count[in] 查询到的标签总数量
	*  @param Flat[in]  剩余标签数量[小于等于0:表示结束、 小于0:为错误码]
	*  @param pUser[in] 用户自定义数据 
	*  @@return =0:数据已处理; <0:数据未处理
	*/
	typedef int (*CLabelCb)(LabelInfo Info,int Count,int Status,void* pUser);

    /** 
    *  @brief 备份任务的回调函数定义
    *  @param nLoginID[out] 登陆ID
    *  @param CameraID[out] 编码器名称
    *  @param nBeginTime[out] 成功备份的开始时间 
    *  @param nEndTime[out] 成功备份的结束时间
    *  @param nFlag[out]  判定任务返回是否完成 [等于0: 表示本次回调结束、 小于0: 为错误码、大于0: 表示待处理的总数]
    *  @param pUser[out] 用户自定义数据 
    *  @@
    */
    typedef void(*BackupDataCallBack) (unsigned int nLoginID, const char *CameraID, unsigned int nBeginTime, unsigned int nEndTime, unsigned int nFlag, void*pUserData);

	/** @fn CLOUD_API int API_Login(const char* CloudIP, unsigned int CloudPort, const char* Username,
	*	const char* Password, CUserMessage UserMsg, void* pUser, unsigned int& logiID);
	*  @brief 登录云存储
	*  @param CloudIP[in]   VMIP地址
	*  @param CloudPort[in] VM登录端口
	*  @param Username[in]  云存储用户名 最大32字符
	*  @param Password[in]  云存储密码 最大32字符
	*  @param UserMsg[in]   用户消息回调函数
	*  @param pUser[in]     用户自定义数据
	*  @param logiID[out]   返回的登录句柄
	*  @@return 成功=0 :登录成功 
	失败<0 :错误码
	*/
	CLOUD_API int API_Login(const char* CloudIP, unsigned int CloudPort, const char* Username,
		const char* Password, CUserMessage UserMsg, void* pUser, unsigned int& logiID);



	/** @fn CLOUD_API unsigned int API_LogOut(unsigned int logiID)
	*  @brief 退出云存储
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @@return 成功=0 
	失败<0 :错误码
	*/
	CLOUD_API int API_LogOut(unsigned int logiID);



	/******************************************************************************
	*                                                                             *
	*                                         录像池部分                          *
	*                                                                             *
	*******************************************************************************/


	/** @fn CLOUD_API int API_PoolCreate(unsigned int loginID, CloudPoolInfo info)
	*  @brief 录像池创建
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param info[in] 录像池信息
	*  @param PoolID[out] 创建录像池成功后返回的ID
	*  @@return  成功=0 :创建录像池成功 
	             失败<0 :错误码

	* 备注：目前创建录像池的名称不支持中文
	*/
	CLOUD_API int API_PoolCreate(unsigned int loginID, CloudPoolInfo info, unsigned int &PoolID);


	/** @fn CLOUD_API int API_PoolDestroy(unsigned int loginID,unsigned int PoolID )
	*  @brief 录像池删除
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param PoolID[in] 创建录像池成功后返回的ID
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PoolDestroy(unsigned int loginID, unsigned int PoolID );


	/** @fn CLOUD_API int API_PoolEdit(unsigned int loginID,unsigned int PoolID,CloudPoolInfo info)
	*  @brief 录像池修改
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param PoolID[in] 创建录像池成功后返回的ID
	*  @param info[in] 录像池信息
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PoolEdit(unsigned int loginID, unsigned int PoolID, CloudPoolInfo info);


	/** @fn CLOUD_API int API_PoolSize(unsigned int loginID,unsigned int PoolID,int PoolSize)
	*  @brief 修改录像池容量
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param PoolID[in] 创建录像池成功后返回的ID
	*  @param PoolSize[in] 录像池容量
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PoolSize(unsigned int loginID, unsigned int PoolID, unsigned int PoolSize);


	/** @fn CLOUD_API int API_PoolInfo(unsigned int loginID, unsigned int PoolID , CloudPoolInfoEx& loudPoolInfoEx)
	*  @brief 录像池查询.查询单个录像池信息
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param PoolID[in] 创建录像池成功后返回的ID
	*  @param loudPoolInfoEx[out] 录像池信息
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PoolInfo(unsigned int loginID, unsigned int PoolID , CloudPoolInfoEx& loudPoolInfoEx);


	/** @fn CLOUD_API int API_PoolList(unsigned int loginID,unsigned int& Count,CPoolListDataCb PoolListDataCb,void* pUser)
	*  @brief 查询所有录像池信息
	*  @param loginID[in] 成功登陆云存储返回的句柄
	*  @param Count[out]  查询到的录像池总数量
	*  @param PoolListDataCb[in] 录像池信息回调函数
	*  @param pUser[in]   用户自定义数据
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PoolList(unsigned int loginID, unsigned int& Count, CPoolListDataCb PoolListDataCb, void* pUser);






	/***********************************************************************************
	*                                                                                  *
	*                                     标签信息部分                                 *
	*                                                                                  *
	************************************************************************************/


	/** @fn CLOUD_API int API_LabelAdd(unsigned int loginID,const char* CameraID
	*                             ,unsigned int BeginTime,unsigned int EndTime ,const char* content)
	*  @brief 标签添加
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param BeginTime[in]            添加标签的开始时间
	*  @param EndTime[in]              添加标签的结束时间
	*  @param content[in]              标签内容，最大支持1024字符
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_LabelAdd(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, const char* content);


	/** @fn CLOUD_API int API_LabelSearch(unsigned int loginID,const char* CameraID,
	*                                unsigned int BeginTime,unsigned int EndTime ,unsigned int&Count,CLabelCb LabelCb,void* pUser)
	*  @brief 标签搜索
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符 
	*  @param BeginTime[in]            搜索标签的开始时间
	*  @param EndTime[in]              搜索标签的结束时间
	*  @param LabelKeyword[in]         搜索标签的关键字
	*  @param Count[out]               查询到标签的总数量
	*  @param LabelCb[in]              数据回调函数
	*  @param pUser[in]                用户自定义数据
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_LabelSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, const char* LabelKeyword, unsigned int& Count, CLabelCb LabelCb, void* pUser);


	/** @fn CLOUD_API int API_LabelDel(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime )
	*  @brief 标签删除
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param BeginTime[in]            删除标签的开始时间
	*  @param EndTime[in]              删除标签的结束时间
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_LabelDel(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime );


	/** @fn CLOUD_API int API_LabelEdit(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime ,const char* newcontent)
	*  @brief 标签修改
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param BeginTime[in]            修改标签的开始时间
	*  @param EndTime[in]              修改标签的结束时间
	*  @param newcontent[in]           新标签内容 最大支持1024字符
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_LabelEdit(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime, const char* newcontent);




	/**************************************************************************
	*                                                                         *
	*                             录像回放部分                                *
	*                                                                         *
	***************************************************************************/


	/** @fn CLOUD_API int API_GetRecordHead(unsigned int loginID,const char* CameraID
	*                        ,unsigned int BeginTime,unsigned int EndTime,char*& Headbuff,int& HeadSize)
	*  @brief 获取视频头文件
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param BeginTime[in]            录像开始时间
	*  @param EndTime[in]              录像结束时间
	*  @param Headbuff[out]            获取的录像数据
	*  @param headSize[out]            获取的录像数据大小
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_GetRecordHead(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, char* Headbuff, int& HeadSize);

	/** @fn CLOUD_API int API_PlayStart(unsigned int loginID,const char* CameraID
	*                                ,unsigned int BeginTime,unsigned int EndTime,PlayType type,const char* RecordType)
	*  @brief 开始回放
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param BeginTime[in]            回放录像的开始时间
	*  @param EndTime[in]              回放录像的结束时间
	*  @param type[in]                 回放类型[参见PlayType定义]
	*  @param RecordType[in]           录像类型(可以使用组合类型)
	1.RecordType支持报警的组合查询
	与操作表达式：1,2,3 
	2.若要不限制报警类型查询，请使用空串 ""
	3.报警类型字串中，允许0和-1的出现，0表示策略（定时）录像，-1表示手动开启的录像，并且0和-1同样可以和1，2，3等报警类型组合成各种复杂的录像查询条件
	*  @@return 成功>=0 :回放句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_PlayStart(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, PlayType type, const char* RecordType);


	/** @fn CLOUD_API int API_PlayData(unsigned int loginID,int PlayHandle,char* PlayBuff,int Bufflen)
	*  @brief 录像回放数据:此接口为不间断取数据接口，当取数据过程中出现任何异常，将跳过一段视频数据进行下一段视频数据的取流
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PlayHandle[in]           成功开启回放返回的句柄
	*  @param PlayBuff[in]             回放数据
	*  @param Bufflen[in]              数据长度 
	*  @@return 成功>=0: 读取数据大小[单位：字节]
	            失败<0 : 错误码
	            错误码为-50014时候，回放完成，可以进行停止回放
				      错误码为-50015时候，已经停止回放
	*/
	CLOUD_API int API_PlayData(unsigned int loginID, int PlayHandle, char* PlayBuff, int Bufflen);

    /** @fn CLOUD_API int API_PlayData_resuming(unsigned int loginID,int PlayHandle,char* PlayBuff,int Bufflen)
    *  @brief 录像回放数据：此接口当出现任何异常，返回给调用者；如若再次调用此接口，数据将从出现异常的位置继续取流
    *  @param loginID[in]              成功登陆云存储返回的句柄
    *  @param PlayHandle[in]           成功开启回放返回的句柄
    *  @param PlayBuff[in]             回放数据
    *  @param Bufflen[in]              数据长度 
    *  @@return 成功>=0: 读取数据大小[单位：字节]
    失败<0 : 错误码
    错误码为-50014时候，回放完成，可以进行停止回放
    错误码为-50015时候，已经停止回放
    */
    CLOUD_API int API_PlayData_resuming(unsigned int loginID, int PlayHandle, char* PlayBuff, int Bufflen);


	/** @fn CLOUD_API int API_PlayStop(unsigned int loginID,int PlayID)
	*  @brief 停止回放
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PlayHandle[in]           成功开启回放返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PlayStop(unsigned int loginID, int PlayHandle);


	/** @fn CLOUD_API int API_PlaySeek(unsigned int loginID,int PlayHandle,unsigned int PosTime)
	*  @brief 回放定位
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PlayHandle[in]           成功开启回放返回的句柄
	*  @param PosTime[in]              定位时间[时间点]
	*  @@return 成功>=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PlaySeek(unsigned int loginID, int PlayHandle, unsigned int PosTime);


	/** @fn CLOUD_API int API_RecordSearch(unsigned int loginID,const char* CameraID,
	*                                    unsigned int BeginTime,unsigned int EndTime,const char* SearchRecordType,
	*                                    RecordLockType bLock,CSearchDataCb SearchDataCb,void* pUser)
	*  @brief 录像搜索
	*  @param loginID[in]       成功登陆云存储返回的句柄
	*  @param CameraID[in]      编码器ID 最大64字符
	*  @param Type[in]          帧类型
	*  @param BeginTime[in]     录像开始时间
	*  @param EndTime[in]       录像结束时间
	*  @param SearchRecordType[in] 查询的类型组合
	1.SearchRecordType支持报警的组合查询
	与操作表达式：1,2,3 
	2.若要不限制报警类型查询，请使用空串 ""
	3.报警类型字串中，允许0和-1的出现，0表示策略（定时）录像，-1表示手动开启的录像，并且0和-1同样可以和1，2，3等报警类型组合成各种复杂的录像查询条件
	*  @param bLock[in]          录像锁定类型 [查询录像锁定类型：0: 全部类型(包括锁定的录像和没有锁定的录像)、1: 没有锁定、2: 锁定]
	*  @param Count[out]         查询到的录像段总数量
	*  @param SearchDataCb[in]   录像回调函数
	*  @param pUser[in]          用户自定义数据
	*  @@return 成功=0 
	            失败<0  : 错误码
	*/
	CLOUD_API int API_RecordSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                           unsigned int EndTime, const char* SearchRecordType, RecordLockType bLock,
								   unsigned int &Count, CSearchDataCb SearchDataCb,void* pUser);


    /** @fn CLOUD_API int API_RecordSearch(unsigned int loginID,const char* CameraID,
    *                                    unsigned int BeginTime,unsigned int EndTime,const char* SearchRecordType,
    *                                    RecordLockType bLock,CSearchDataCb SearchDataCb,void* pUser)
    *  @brief 录像搜索的扩展接口，可查询备份录像
    *  @param loginID[in]       成功登陆云存储返回的句柄
    *  @param CameraID[in]      编码器ID 最大64字符
    *  @param Type[in]          帧类型
    *  @param BeginTime[in]     录像开始时间
    *  @param EndTime[in]       录像结束时间
    *  @param SearchRecordType[in] 查询的类型组合
    1.SearchRecordType支持报警的组合查询
    与操作表达式：1,2,3 
    2.若要不限制报警类型查询，请使用空串 ""
    3.报警类型字串中，允许0和-1的出现，0表示策略（定时）录像，-1表示手动开启的录像，并且0和-1同样可以和1，2，3等报警类型组合成各种复杂的录像查询条件
    *  @param bLock[in]          录像锁定类型 [0: 全部类型(包括锁定的录像和没有锁定的录像)、1: 没有锁定、2: 已锁定]
    *  @param nBackup[in]        查询备份信息标志[0 – 查询所有录像信息，1 – 查询备份录像信息]
    *  @param Count[out]         查询到的录像段总数量
    *  @param SearchDataCb[in]   录像回调函数
    *  @param pUser[in]          用户自定义数据
    *  @@return 成功=0 
    失败<0  : 错误码
    */
    CLOUD_API int API_RecordSearchEx(unsigned int loginID, const char* CameraID, unsigned int BeginTime, 
                        unsigned int EndTime, const char* SearchRecordType, RecordLockType bLock,
                        unsigned int nBackup, unsigned int &Count, CSearchDataCb SearchDataCb, void* pUser);

	/** @fn CLOUD_API int API_RecordSearchByTag(unsigned int loginID,
	*                         const char* CameraID,const char* tagInfo,CSearchDataCb SearchDataCb,void* pUser)
	*  @brief 按标签搜索录像
	*  @param loginID[in]         成功登陆云存储返回的句柄
	*  @param CameraID[in]        编码器ID 最大64字符
	*  @param tagInfo[in]         标签信息  	如果为空字符("") 表示查询所有有标注的录像
	*  @param Count[out]          查询到的录像段总数量
	*  @param SearchDataCb[in]    标签回调函数
	*  @param pUser[in]           用户自定义数据
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordSearchByTag(unsigned int loginID, const char* CameraID, const char* tagInfo,
		                                unsigned int &Count, CSearchDataCb SearchDataCb, void* pUser);


	/** @fn CLOUD_API int API_RecordLock(unsigned int loginID,const char* CameraID
	*                                       ,unsigned int BeginTime,unsigned int EndTime ,unsigned int LockTime)
	*  @brief 录像锁定
	*  @param loginID[in]         成功登陆云存储返回的句柄
	*  @param CameraID[in]        编码器ID 最大64字符
	*  @param BeginTime[in]       录像开始时间
	*  @param EndTime[in]         录像结束时间
	*  @param LockTime[in]        锁定时长[0: 表示不锁定、大于0: 表示锁定的时间(从当前时间到需要解锁时间点的时长) 单位:秒]
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordLock(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                         unsigned int EndTime, unsigned int LockTime, const char* RecordType);


	/** @fn CLOUD_API int API_RecordDelete(unsigned int loginID,const char* CameraID,
	*                                          const char* RecordType,unsigned int BeginTime,unsigned int EndTime)
	*  @brief 录像删除
	*  @param loginID[in]         成功登陆云存储返回的句柄
	*  @param CameraID[in]        编码器ID 最大64字符
	*  @param RecordType[in]      删除录像的类型(可以使用组合类型),包含删除对应的附属信息的组合
	*  @param BeginTime[in]       需要删除录像的开始时间
	*  @param EndTime[in]         需要删除录像的结束时间
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordDelete(unsigned int loginID, const char* CameraID, const char* RecordType,
		                           unsigned int BeginTime, unsigned int EndTime);


	/** @fn CLOUD_API int API_RecordDownloadStart(unsigned int loginID,
	*                                    const char* CameraID,unsigned int BeginTime,unsigned int EndTime,
	*                                    const char* RecordType,int DownloadOffset,CDownloadDataCb DownloadDataCb,void* pUser)
	*  @brief 录像开始下载
	*  @param loginID[in]         成功登陆云存储返回的句柄
	*  @param CameraID[in]        编码器ID 最大64字符
	*  @param BeginTime[in]       下载录像的开始时间
	*  @param EndTime[in]         下载录像的结束时间
	*  @param RecordType[in]       需要下载的录像类型(可以使用组合类型)
	1.RecordType支持报警的组合查询
	与操作表达式：1,2,3 
	2.若要不限制报警类型查询，请使用空串 ""
	3.报警类型字串中，允许0和-1的出现，0表示策略（定时）录像，-1表示手动开启的录像，并且0和-1同样可以和1，2，3等报警类型组合成各种复杂的录像查询条件
	*  @param DownloadOffset[in]   已下载字节数 支持断点续传
	*  @param DownloadDataCb[in]   下载数据回调
	*  @param pUser[in]            用户自定义数据
	*  @@return 成功=>0:下载句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordDownloadStart(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime,
		                                  const char* RecordType, int DownloadOffset, CDownloadDataCb DownloadDataCb, void* pUser);


	/** @fn CLOUD_API int API_RecordDownloadStop(unsigned int loginID,int RecordDownloadHandle)
	*  @brief 录像停止下载
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param RecordDownloadHandle[in] 开启下载成功返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordDownloadStop(unsigned int loginID, int RecordDownloadHandle);


	/** @fn CLOUD_API int API_RecordMove(unsigned int loginID,const char* CameraID,
	*                           const char* RecordType,BOOL iFrameOnly,unsigned int BeginTime,unsigned int EndTime)
	*  @brief 录像转存或迁移
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param RecordType[in]           录像类型(可以使用组合类型)
	1.RecordType支持报警的组合查询
	与操作表达式：1,2,3 
	2.若要不限制报警类型查询，请使用空串 ""
	3.报警类型字串中，允许0和-1的出现，0表示策略（定时）录像，-1表示手动开启的录像，并且0和-1同样可以和1，2，3等报警类型组合成各种复杂的录像查询条件
	*  @param MoveType[in]             迁移类型 [0: 默认方式迁移所有帧类型数据、1:只迁移I帧数据]
	*  @param BeginTime[in]            录像开始时间
	*  @param EndTime[in]              录像结束时间
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordMove(unsigned int loginID, const char* CameraID, const char* RecordType,
		                         int MoveType, unsigned int BeginTime, unsigned int EndTime);


    /** @fn CLOUD_API int API_backup ( unsigned int loginID, const char *CameraID,const strBackupPlan *pBackupPlan, unsigned int BackupType, unsigned int Type)
    *  @brief 向云存储系统下发当前编码器的备份和冗余计划信息
    *  @param loginID[in]       成功登陆云存储返回的句柄
    *  @param CameraID[in]  成功开启录像返回的句柄
    *  @param strBackupPlan
    *  @param BackupType     备份类型，1:本云备份，2:异云备份（IP通），3:异云备份（IP不通）
    *  @param Type            1:设定任务，2:取消任务
    *  @@return 成功=0
    失败<0 :错误码
    */
    CLOUD_API int API_backup ( unsigned int loginID, const char *CameraID, 
        const strBackupPlan *pBackupPlan, unsigned int BackupType, unsigned int Type);

    /** @fn CLOUD_API int SetBackupCallBack(BackupDataCallBack  pBackupDataCallBak, void* pUserData)
    *  @brief 注册备份任务的回调函数
    *  @param BackupDataCallBack[in]       用户注册的回调函数
    *  @param handle[in]                   用户参数
    *  @@return 成功=0
    失败<0 :错误码
    */
    CLOUD_API int SetBackupCallBack(BackupDataCallBack  pBackupDataCallBak, void* pUserData);

};//extern "C"

#endif