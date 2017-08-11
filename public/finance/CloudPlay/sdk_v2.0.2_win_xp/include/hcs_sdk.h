/** @file   hcs_sdk.h
 *  @note   Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief  
 *
 *  @author Jin Shengpeng
 *  @date   2015/01/28
 *
 *  @note   历史记录：
 *  @note   V1.0.0.0 创建
 */

#ifndef HCS_SDK_H_
#define HCS_SDK_H_

#include "hcs_sdk_define.h"

/******************************************************************************
 *                                                                            *
 *                               系统功能                                     *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_Init
 *  @ brief  初始化SDK
 *  @ return HCS_OK-初始化成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Init();



/** @ fn     HCS_SDK_Deinit
 *  @ brief  反初始化SDK[多次初始化，必须多次反初始化，最后一次反初始化返回成功，前面几次都会返回失败]
 *  @ return HCS_OK-反初始化成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Deinit();

/** @ fn     HCS_SDK_GetVersion
 *  @ brief  获取云存储版本号
 *  @ param  ip				- [in] 云存储IP地址
 *  @ param  port			- [in] 云存储服务端口
 *  @ param  version		- [out] 云存储版本信息
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetVersion(const char* ip, 
												  unsigned int port, 
												  HCS_Version* version);

/** @ fn     HCS_SDK_Login
 *  @ brief  登录云存储
 *  @ param  ip			- [in] 云存储IP地址
 *  @ param  port		- [in] 云存储服务端口
 *  @ param  username	- [in] 登录云存储用户名
 *  @ param  password	- [in] 登录云存储密码
 *  @ param  login_id	- [out]云存储登录ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Login(const char* ip, 
											 unsigned int port, 
											 const char* username, 
											 const char* password, 
											 unsigned long long* login_id);



/** @ fn     HCS_SDK_Logout
 *  @ brief  登出云存储
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Logout(unsigned long long login_id);



/** @ fn     HCS_SDK_GetPoolInfo
 *  @ brief  获取指定资源池信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  pool_id	- [in] 资源池ID[最大支持32个字符]
 *  @ param  pool_info	- [out]资源池信息
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetPoolInfo(unsigned long long login_id, 
												   const char* pool_id, 
												   HCS_PoolInfo* pool_info);



/** @ fn     HCS_SDK_GetAllPoolInfoStart
 *  @ brief  开始获取全部资源池信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  count		- [out]资源池数量
 *  @ param  handle		- [out]获取全部资源池信息操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoStart(unsigned long long login_id, 
														   int* count, 
														   int* handle);



/** @ fn     HCS_SDK_GetAllPoolInfoStop
 *  @ brief  获取下一条资源池信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取全部资源池信息操作句柄
 *  @ param  info		- [out] 获取的资源池信息
 *  @ param  status		- [out] 查询资源池结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoNext(unsigned long long login_id, 
														  int handle, 
														  HCS_PoolInfo* info, 
														  int* status);



/** @ fn     HCS_SDK_GetAllPoolInfoStop
 *  @ brief  停止获取全部资源池信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取全部资源池信息操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_GetAbility
 *  @ brief  获取云存储接入能力集
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  ability	- [out]能力集
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAbility(unsigned long long login_id, 
												  HCS_Ability* ability);

/******************************************************************************
 *                                                                            *
 *                               主动云										  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_SetRecPlan
 *  @ brief  设置录像计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  plan		- [in] 录像计划
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecPlan(unsigned long long login_id, 
												  const HCS_RecordPlan* plan);



/** @ fn     HCS_SDK_DelRecPlan
 *  @ brief  删除录像计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  camera_id	- [in] 编码器ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelRecPlan(unsigned long long login_id, 
												  const char* camera_id);



/** @ fn     HCS_SDK_GetRecPlan
 *  @ brief  获取编码器对应的录像计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  camera_id	- [in] 编码器ID
 *  @ param  plan		- [out]录像计划
 *  @ param  status		- [out]录像计划下发状态[0-未下发，1-已下发]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetRecPlan(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_RecordPlan* plan, 
												  int* status);



/** @ fn     HCS_SDK_GetAllRecPlanStart
 *  @ brief  开始获取全部录像计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  count		- [out]录像计划数目
 *  @ param  handle		- [out]获取录像计划操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanStart(unsigned long long login_id, 
														  int* count, 
														  int* handle);



/** @ fn     HCS_SDK_GetAllRecPlanNext
 *  @ brief  获取下一条录像计划信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取全部录像计划信息操作句柄
 *  @ param  info		- [out] 获取的录像计划信息
 *  @ param  alloc		- [out] 获取的录像计划下发状态[0-未下发，1-已下发]
 *  @ param  status		- [out] 获取录像计划结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanNext(unsigned long long login_id, 
	                                                     int handle, 
														 HCS_RecordPlan* info, 
														 int* alloc, 
														 int* status);



/** @ fn     HCS_SDK_GetAllRecPlanStop
 *  @ brief  停止获取全部录像计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取录像计划操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanStop(unsigned long long login_id, 
														 int handle);



/** @ fn     HCS_SDK_EventRecord
 *  @ brief  事件录像
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  camera_id	- [in] 编码器ID
 *  @ param  rec_type	- [in] 录像类型[范围：0-65535]
 *  @ param  time		- [in] 持续时间[建议大于5，避免过于频繁的调用该接口]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_EventRecord(unsigned long long login_id, 
												   const char* camera_id, 
												   unsigned int rec_type, 
												   unsigned int time);



/**  @ fn     HCS_SDK_GetAllVtduInfoStart
 *   @ brief  获取接入服务器列表
 *   @ param  login_id	- [in] 成功登录云存储返回的句柄
 *   @ param  count		- [out]接入服务器数目
 *   @ param  handle	- [out]获取句柄
 *   @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API  HCS_SDK_GetAllVtduInfoStart(unsigned long long login_id, 
													        int* count, 
													        int* handle);



/** @ fn     HCS_SDK_GetAllVtduInfoNext
 *  @ brief  获取下一条接入服务器信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取全部接入服务器信息操作句柄
 *  @ param  info		- [out] 获取的接入服务器信息
 *  @ param  status		- [out] 获取接入服务器信息结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllVtduInfoNext(unsigned long long login_id, 
														  int handle, 
														  HCS_VtduInfo* info, 
														  int* status);



/** @ fn     HCS_SDK_GetAllVtduInfoStop
 *  @ brief  停止获取全部接入服务器信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取接入服务器操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllVtduInfoStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_SetHisRecPlan
 *  @ brief  设置历史流存储计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  plan		- [in] 历史流存储计划
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetHisRecPlan(unsigned long long login_id, 
													 const HCS_HisRecPlan* plan);



/** @ fn     HCS_SDK_DelHisRecPlan
 *  @ brief  删除历史流存储计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  camera_id	- [in] 编码器ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelHisRecPlan(unsigned long long login_id, 
													 const char* camera_id);



/** @ fn     HCS_SDK_GetHisRecPlan
 *  @ brief  获取编码器对应的历史流存储计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  camera_id	- [in] 编码器ID
 *  @ param  plan		- [out]历史流存储计划
 *  @ param  status		- [out]录像计划下发状态[0-未下发，1-已下发]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetHisRecPlan(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_HisRecPlan* plan, 
													 int* status);



/** @ fn     HCS_SDK_GetAllHisRecPlanStart
 *  @ brief  开始获取全部历史补录计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  count		- [out]历史补录计划数目
 *  @ param  handle		- [out]获取历史流存储计划操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanStart(unsigned long long login_id, 
															 int* count, 
															 int* handle);



/** @ fn     HCS_SDK_GetAllHisRecPlanNext
 *  @ brief  获取下一条历史流计划信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取全部历史流计划信息操作句柄
 *  @ param  info		- [out] 获取的历史流计划信息
 *  @ param  alloc		- [out] 获取的历史流计划下发状态[0-未下发，1-已下发]
 *  @ param  status		- [out] 获取历史流计划结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanNext(unsigned long long login_id, 
															int handle, 
															HCS_HisRecPlan* info, 
															int* alloc, 
															int* status);



/** @ fn     HCS_SDK_GetAllHisRecPlanStop
 *  @ brief  停止获取全部历史流存储计划
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取历史流存储计划操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanStop(unsigned long long login_id, 
															int handle);



/** @ fn     HCS_SDK_HistoryRecord
 *  @ brief  手动补录
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  info		- [in] 手动补录参数
 *  @ param  url_len	- [in] url缓存区长度[建议1024字节]
 *  @ param  url		- [out] 返回的取流URL[可以通过URL取流也可以通过编码器，开始，结束时间取流]
 *  @ param  task_id	- [out] 手动补录任务ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecord(unsigned long long login_id, 
													 const HCS_HisRecInfo* info,
													 unsigned int url_len,
													 char* url,
													 unsigned long long* task_id);




/** @ fn     HCS_SDK_HistoryRecordStatus
 *  @ brief  获取手动补录状态
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  task_id	- [in] 手动补录任务ID
 *  @ param  task_info	- [out] 手动补录执行状态
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecordStatus(unsigned long long login_id, 
														   unsigned long long task_id,
														   HCS_TaskInfo* task_info);



/** @ fn     HCS_SDK_HistoryRecordStop
 *  @ brief  停止手动补录
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  task_id	- [in] 手动补录任务ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecordStop(unsigned long long login_id, 
														 unsigned long long task_id);


/******************************************************************************
 *                                                                            *
 *                               录像操作									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_RecStart
 *  @ brief  开启录像
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  rec_type		- [in] 录像类型[范围：0-65535]
 *  @ param  data_type		- [in] 数据类型
 *  @ param  iframe_type	- [in] I帧类型
 *  @ param  replication	- [in] 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
 *  @ param  pool_id		- [in] 资源池ID[最大支持32个字符]
 *  @ param  rep_pool_id	- [in] 冗余资源池ID[最大支持32个字符]，冗余份数不为0时有效
 *  @ param  rec_head		- [in] 录像头数据
 *  @ param  head_size		- [in] 录像头长度[最大为64字节]
 *  @ param  handle			- [out] 录像操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecStart(unsigned long long login_id, 
												const char* camera_id, 
												unsigned int rec_type, 
												HCS_RecDataType data_type, 
												HCS_IFrameType iframe_type, 
												unsigned int replication, 
												const char* pool_id, 
												const char* rep_pool_id, 
												const char* rec_head, 
												unsigned int head_size, 
												int* handle);



/** @ fn     HCS_SDK_RecData
 *  @ brief  存储录像数据
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像操作句柄
 *  @ param  frame_type		- [in] 帧类型
 *  @ param  data_type		- [in] 数据类型
 *  @ param  begin_time		- [in] 录像数据对应时间片的开始时间点
 *  @ param  end_time		- [in] 录像数据对应时间片的结束时间点
 *  @ param  rec_data		- [in] 录像数据
 *  @ param  data_size		- [in] 数据长度
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecData(unsigned long long login_id, 
											   int handle, 
											   HCS_FrameType frame_type, 
											   HCS_DataType data_type, 
											   HCS_Time begin_time, 
											   HCS_Time end_time, 
											   const char* rec_data, 
											   unsigned int data_size);



/** @ fn     HCS_SDK_SetRecType
 *  @ brief  修改录像类型
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像操作句柄
 *  @ param  rec_type		- [in] 录像类型[范围：0-65535]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecType(unsigned long long login_id, 
												  int handle, 
												  unsigned int rec_type);



/** @ fn     HCS_SDK_SetRecReplication
 *  @ brief  修改录像冗余份数
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像操作句柄
 *  @ param  replication	- [in] 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecReplication(unsigned long long login_id, 
														 int handle, 
														 unsigned int replication);



/** @ fn     HCS_SDK_RecStop
 *  @ brief  停止录像
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecStop(unsigned long long login_id, 
											   int handle);



/** @ fn     HCS_SDK_GetRecHead
 *  @ brief  获取录像头
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 录像段开始时间
 *  @ param  end_time		- [in] 录像段结束时间
 *  @ param  buff			- [in] 录像头数据缓冲区
 *  @ param  buff_size		- [in] 录像头数据缓冲区大小
 *  @ param  head_size		- [out]实际的录像头数据大小
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetRecHead(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_Time begin_time, 
												  HCS_Time end_time, 
												  char* buff, 
												  unsigned int buff_size, 
												  int* head_size);



/** @ fn     HCS_SDK_PlaybackStart
 *  @ brief  开启回放
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 回放录像段开始时间
 *  @ param  end_time		- [in] 回放录像段结束时间
 *  @ param  mode			- [in] 回放模式
 *  @ param  rec_type		- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  handle			- [out]录像回放操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackStart(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_Time begin_time, 
													 HCS_Time end_time, 
													 HCS_PlaybackMode mode, 
													 const char* rec_type, 
													 int* handle);



/** @ fn     HCS_SDK_PlaybackData
 *  @ brief  获取回放数据
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 回放操作句柄
 *  @ param  buff			- [in] 回放数据缓冲区
 *  @ param  buff_size		- [in] 回放数据缓冲区大小
 *  @ param  data_size		- [out]实际的数据大小
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackData(unsigned long long login_id, 
													int handle, 
													char* buff, 
													unsigned int buff_size, 
													int* data_size);



/** @ fn     HCS_SDK_PlaybackSeek
*  @ brief  回放定位
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 回放操作句柄
*  @ param  seek_time		- [in] 定位时间点
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackSeek(unsigned long long login_id, 
													int handle, 
													HCS_Time seek_time);



/** @ fn     HCS_SDK_PlaybackStop
 *  @ brief  停止回放
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 回放操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackStop(unsigned long long login_id, 
													int handle);



/** @ fn     HCS_SDK_RecDownloadStart
 *  @ brief  开启录像下载
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 下载录像段开始时间
 *  @ param  end_time		- [in] 下载录像段结束时间
 *  @ param  rec_type		- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  offset			- [in] 下载数据偏移(用于断点续传)
 *  @ param  cb				- [in] 录像下载数据回调函数
 *  @ param  user_data		- [in] 用户自定义数据
 *  @ param  handle			- [out]录像下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDownloadStart(unsigned long long login_id, 
														const char* camera_id, 
														HCS_Time begin_time, 
														HCS_Time end_time, 
														const char* rec_type, 
														unsigned long long offset, 
														RecDownloadCb cb, 
														void* user_data, 
														int* handle);



/** @ fn     HCS_SDK_RecDownloadStop
 *  @ brief  停止录像下载
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDownloadStop(unsigned long long login_id, 
													   int handle);



/** @ fn     HCS_SDK_RecHighSpeedDownloadStart
 *  @ brief  开启录像高速下载
 *  @ param  login_id			- [in] 云存储登录ID
 *  @ param  camera_id			- [in] 编码器ID
 *  @ param  begin_time			- [in] 下载录像段开始时间
 *  @ param  end_time			- [in] 下载录像段结束时间
 *  @ param  rec_type			- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  download_type		- [in] 下载类型[0-下载，1-续传]
 *  @ param  concurrency		- [in] 并发数[1-10]
 *  @ param  max_segment_count	- [in] 用户传入的段数组最大元素个数
 *  @ param  segment_infos		- [in/out] 下载时：获取的数据块信息（输出），续传时：平台给入的已下载的数据信息（输入）
 *  @ param  segment_count		- [in/out] 下载时：获取的数据块个数（输出），续传时：平台给入的已下载的数据个数（输入）
 *  @ param  cb					- [in] 录像高速下载数据回调函数
 *  @ param  user_data			- [in] 用户自定义数据
 *  @ param  handle				- [out]录像下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecHighSpeedDownloadStart(unsigned long long login_id, 
																 const char* camera_id, 
																 HCS_Time begin_time, 
																 HCS_Time end_time, 
																 const char* rec_type, 
																 unsigned int download_type, 
																 unsigned int concurrency, 
																 unsigned int max_segment_count, 
																 HCS_HighSpeedSegInfo* segment_infos, 
																 int* segment_count, 
																 RecHighSpeedDownloadCb cb, 
																 void* user_data, 
																 int* handle);




/** @ fn     HCS_SDK_RecHighSpeedDownloadStop
 *  @ brief  停止录像高速下载 
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecHighSpeedDownloadStop(unsigned long long login_id, 
																int handle);



/** @ fn     HCS_SDK_RecSearchStart
 *  @ brief  开启录像检索
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 检索时间段开始时间点
 *  @ param  end_time		- [in] 检索时间段结束时间点
 *  @ param  rec_type		- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  lock_type		- [in] 锁定类型
 *  @ param  count			- [out]录像片段数目
 *  @ param  handle			- [out]录像检索操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchStart(unsigned long long login_id, 
													  const char* camera_id, 
													  HCS_Time begin_time, 
													  HCS_Time end_time, 
													  const char* rec_type, 
													  HCS_SRLockType lock_type, 
													  int* count, 
													  int* handle);



/** @ fn     HCS_SDK_RecSearchNext
 *  @ brief  获取下一条录像段信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取录像段信息操作句柄
 *  @ param  info		- [out] 获取的录像段信息
 *  @ param  status		- [out] 获取录像段信息结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchNext(unsigned long long login_id, 
	                                                 int handle, 
												     HCS_RecordInfo* info, 
													 int* status);



/** @ fn     HCS_SDK_RecSearchStop
 *  @ brief  停止录像检索
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录像检索操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchStop(unsigned long long login_id, 
													 int handle);



/** @ fn     HCS_SDK_RecSearchByDay
 *  @ brief  录像日历查询
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  date			- [in] 日期
 *  @ param  info			- [in] 日历查询结果
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByDay(unsigned long long login_id, 
													  const char* camera_id,
													  HCS_Date date,
													  HCS_RecDayInfo *info);



/** @ fn     HCS_SDK_RecSearchByMonth
 *  @ brief  录像月历查询
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  year			- [in] 年
 *  @ param  month			- [in] 月
 *  @ param  info			- [in] 月历查询结果
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByMonth(unsigned long long login_id, 
														const char* camera_id,
														unsigned int year,
														unsigned int month,
														HCS_RecMonthInfo *info);



/** @ fn     HCS_SDK_RecDelete
 *  @ brief  删除录像
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  rec_type		- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  begin_time		- [in] 要删除的录像片段开始时间
 *  @ param  end_time		- [in] 要删除的录像片段结束时间
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDelete(unsigned long long login_id, 
												 const char* camera_id, 
												 const char* rec_type, 
												 HCS_Time begin_time, 
												 HCS_Time end_time);



/** @ fn     HCS_SDK_RecLock
 *  @ brief  锁定录像
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  rec_type		- [in] 录像类型[""表示全部,录像类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  begin_time		- [in] 要锁定的录像片段开始时间
 *  @ param  end_time		- [in] 要锁定的录像片段结束时间
 *  @ param  lock_time		- [in] 锁定录像片段的时长，0表示解锁,单位:秒
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecLock(unsigned long long login_id, 
											   const char* camera_id, 
											   const char* rec_type, 
											   HCS_Time begin_time, 
											   HCS_Time end_time, 
											   unsigned int lock_time);

/*****************************************************************************
 *                                                                           *
 *                               视频文件操作								 *
 *                                                                           *
 *****************************************************************************/

/**@ fn     HCS_SDK_StreamFileWriteStart
*  @ brief  开启视频文件存储
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  file_write_info	- [in] 视频文件开启写参数信息
*  @ param  handle			- [out] 录像操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteStart(unsigned long long login_id, 
															const HCS_StreamFileWriteInfo* file_write_info, 
															int* handle);



/**@ fn     HCS_SDK_StreamFileWriteData
*  @ brief  存储视频文件数据
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件录像操作句柄
*  @ param  frame_type		- [in] 文件帧类型
*  @ param  begin_time		- [in] 本文件块开始时间点
*  @ param  end_time		- [in] 本文件块结束时间点
*  @ param  rec_data		- [in] 视频文件数据
*  @ param  data_size		- [in] 数据长度
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteData(unsigned long long login_id, 
														   int handle, 
														   HCS_FrameType frame_type, 
														   HCS_Time begin_time, 
														   HCS_Time end_time, 
														   const char* rec_data, 
														   unsigned int data_size);



/**@ fn     HCS_SDK_StreamFileWriteStop
*  @ brief  停止视频文件存储
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteStop(unsigned long long login_id, 
														   int handle);



/**@ fn     HCS_SDK_StreamFileReadStart
*  @ brief  开启视频文件读取
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  file_key		- [in] 视频文件名
*  @ param  handle			- [out]视频文件读取操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadStart(unsigned long long login_id, 
														   const char* file_key, 
														   int* handle);



/**@ fn     HCS_SDK_StreamFileReadData
*  @ brief  读取视频文件数据
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件读取操作句柄
*  @ param  buff			- [in] 读取数据缓冲区
*  @ param  buff_size		- [in] 读取数据缓冲区大小
*  @ param  data_size		- [out]实际的数据大小
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadData(unsigned long long login_id, 
														  int handle, 
														  char* buff, 
														  unsigned int buff_size, 
														  int* data_size);



/**@ fn     HCS_SDK_StreamFileReadSeek
*  @ brief  视频文件读取定位
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件读取操作句柄
*  @ param  seek_time		- [in] 定位偏移时间
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadSeek(unsigned long long login_id, 
														  int handle, 
														  HCS_Time seek_time);



/**@ fn     HCS_SDK_StreamFileReadStop
*  @ brief  停止视频文件读取
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件读取操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadStop(unsigned long long login_id, 
														  int handle);



/**@ fn     HCS_SDK_StreamFileDownloadStart
*  @ brief  开启视频文件下载
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  file_key		- [in] 视频文件名
*  @ param  offset			- [in] 下载数据偏移(用于断点续传),单位:字节
*  @ param  cb				- [in] 视频文件下载数据回调函数
*  @ param  user_data		- [in] 用户自定义数据
*  @ param  handle			- [out]视频文件下载操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDownloadStart(unsigned long long login_id, 
															   const char* file_key, 
															   unsigned long long offset, 
															   RecDownloadCb cb, 
															   void* user_data, 
															   int* handle);



/**@ fn     HCS_SDK_StreamFileDownloadStop
*  @ brief  停止视频文件下载
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  handle			- [in] 视频文件下载操作句柄
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDownloadStop(unsigned long long login_id, 
															  int handle);



/* @ fn     HCS_SDK_GetStreamFileInfo
*  @ brief  获取视频文件信息
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  file_key		- [in] 视频文件名
*  @ param  file_info		- [out] 视频文件信息
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetStreamFileInfo(unsigned long long login_id, 
														 const char* file_key, 
														 HCS_StreamFileInfo* file_info);



/**@ fn     HCS_SDK_StreamFileDelete
*  @ brief  删除视频文件
*  @ param  login_id		- [in] 云存储登录ID
*  @ param  file_key		- [in] 视频文件名
*  @ return HCS_OK-成功 其他-错误码
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDelete(unsigned long long login_id, 
														const char* file_key);


/******************************************************************************
 *                                                                            *
 *                               图片操作									  *
 *                                                                            *
 ******************************************************************************/


/** @ fn     HCS_SDK_PicStorStart
 *  @ brief  开启存储图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  pool_id		- [in] 资源池ID[最大支持32个字符]
 *  @ param  replication	- [in] 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
 *  @ param  rep_pool_id	- [in] 冗余资源池ID[最大支持32个字符]，冗余份数不为0时有效
 *  @ param  handle			- [out]图片存储操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorStart(unsigned long long login_id, 
													const char* camera_id, 
													const char* pool_id, 
													unsigned int replication, 
													const char* rep_pool_id, 
													int* handle);



/** @ fn     HCS_SDK_PicStorData
 *  @ brief  存储图片写入数据
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 图片存储操作句柄
 *  @ param  time			- [in] 图片存储时间点
 *  @ param  pic_type		- [in] 图片类型
 *  @ param  pic_data		- [in] 图片数据
 *  @ param  data_size		- [in] 图片数据大小
 *  @ param  url_len		- [in] 图片对应URL字符串长度
 *  @ param  url			- [out]图片对应URL字符数组
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorData(unsigned long long login_id, 
												   int handle, 
												   HCS_Time time, 
												   HCS_PicType pic_type, 
												   const char* pic_data, 
												   unsigned int data_size, 
												   unsigned int url_len, 
												   char* url);



/** @ fn     HCS_SDK_PicStorStop
 *  @ brief  停止存储图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 图片存储操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorStop(unsigned long long login_id, 
												   int handle);



/** @ fn     HCS_SDK_PicDownloadByURL
 *  @ brief  根据URL下载图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  url			- [in] 图片对应的URL[最大支持256个字符]
 *  @ param  zoom_type		- [in] 图片缩放类型
 *  @ param  zoom_value		- [in] 图片缩放值
 *  @ param  buff			- [in] 图片数据存储缓存
 *  @ param  buff_size		- [in] 图片数据缓存大小
 *  @ param  data_size		- [out]实际的图片数据大小
 *  @ param  pic_type		- [out]图片类型
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByURL(unsigned long long login_id, 
														const char* url, 
														HCS_PicZoomType zoom_type, 
														const char* zoom_value, 
														char* buff, 
														unsigned int buff_size, 
														int* data_size, 
														int* pic_type);



/** @ fn     HCS_SDK_PicDownloadByTimeStart
 *  @ brief  开启根据时间下载图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  pic_type		- [in] 图片类型[""表示全部,类型组合使用“|”分割，比如0|2|3,最大支持32个字符]
 *  @ param  begin_time		- [in] 下载时间范围开始时间点
 *  @ param  end_time		- [in] 下载时间范围结束时间点
 *  @ param  zoom_type		- [in] 图片缩放类型
 *  @ param  zoom_value		- [in] 图片缩放值
 *  @ param  cb				- [in] 图片下载回调函数
 *  @ param  user_data		- [in] 用户自定义数据
 *  @ param  handle			- [out]图片下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByTimeStart(unsigned long long login_id, 
															  const char* camera_id, 
															  const char* pic_type, 
															  HCS_Time begin_time, 
															  HCS_Time end_time, 
															  HCS_PicZoomType zoom_type, 
															  const char* zoom_value, 
															  PicDownloadCb cb, 
															  void* user_data, 
															  int* handle);



/** @ fn     HCS_SDK_PicDownloadByTimeStop
 *  @ brief  停止根据时间下载图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 图片下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByTimeStop(unsigned long long login_id, 
															 int handler);



/** @ fn     HCS_SDK_PicLockByURL
 *  @ brief  根据URL锁定图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  url			- [in] 图片对应的URL[最大支持256个字符]
 *  @ param  lock_time		- [in] 锁定时长，0表示解锁
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicLockByURL(unsigned long long login_id, 
													const char* url, 
													unsigned int lock_time);



/** @ fn     HCS_SDK_PicLockByTime
 *  @ brief  根据时间段锁定图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 锁定时间范围开始时间点
 *  @ param  end_time		- [in] 锁定时间范围结束时间点
 *  @ param  lock_time		- [in] 锁定时长，0表示解锁,单位:秒
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicLockByTime(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_Time begin_time, 
													 HCS_Time end_time, 
													 unsigned int lock_time);



/** @ fn     HCS_SDK_PicDeleteByURL
 *  @ brief  根据URL删除图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  url			- [in] 图片对应的URL[最大支持256个字符]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDeleteByURL(unsigned long long login_id, 
													  const char* url);



/** @ fn     HCS_SDK_PicDeleteByTime
 *  @ brief  根据时间段删除图片
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 删除时间范围开始时间点
 *  @ param  end_time		- [in] 删除时间范围结束时间点
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDeleteByTime(unsigned long long login_id, 
													   const char* camera_id, 
													   HCS_Time begin_time, 
													   HCS_Time end_time);


/*****************************************************************************
 *                                                                           *
 *                               附属操作									 *
 *                                                                           *
 *****************************************************************************/

/** @ fn     HCS_SDK_AttachedStorStart
 *  @ brief  开启存储附属
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  pool_id		- [in] 资源池ID[最大支持32个字符]
 *  @ param  replication	- [in] 冗余数[0-不冗余 1-1份冗余 2-双份冗余]
 *  @ param  rep_pool_id	- [in] 冗余资源池ID[最大支持32个字符]，冗余份数不为0时有效
 *  @ param  handle			- [out]附属存储操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorStart(unsigned long long login_id, 
														 const char* camera_id, 
														 const char* pool_id, 
														 int replication, 
														 const char* rep_pool_id, 
														 int* handle);



/** @ fn     HCS_SDK_AttachedStorData
 *  @ brief  存储附属写入数据
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 附属存储操作句柄
 *  @ param  begin_time		- [in] 附属存储开始时间
 *  @ param  end_time		- [in] 附属存储结束时间
 *  @ param  data			- [in] 附属数据
 *  @ param  data_size		- [in] 附属数据大小
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorData(unsigned long long login_id, 
														int handle, 
														HCS_Time begin_time, 
														HCS_Time end_time, 
														const char* data, 
														unsigned int data_size);



/** @ fn     HCS_SDK_AttachedStorStop
 *  @ brief  停止存储附属
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 附属存储操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorStop(unsigned long long login_id, 
														int handle);



/** @ fn     HCS_SDK_AttachedSearchStart
 *  @ brief  开启附属检索
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 检索时间段开始时间点
 *  @ param  end_time		- [in] 检索时间段结束时间点
 *  @ param  count			- [out]附属片段数目
 *  @ param  handle			- [out]附属检索操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchStart(unsigned long long login_id, 
														   const char* camera_id, 
														   HCS_Time begin_time, 
														   HCS_Time end_time,  
														   int* count, 
														   int* handle);



/** @ fn     HCS_SDK_AttachedSearchNext
 *  @ brief  获取下一条附属段信息
 *  @ param  login_id	- [in] 云存储登录ID
 *  @ param  handle		- [in] 获取附属段信息操作句柄
 *  @ param  info		- [out] 获取的附属段信息
 *  @ param  status		- [out] 获取附属段信息结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchNext(unsigned long long login_id, 
	                                                      int handle, 
												          HCS_RecordInfo* info, 
													      int* status);



/** @ fn     HCS_SDK_AttachedSearchStop
 *  @ brief  停止附属检索
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 录附属检索操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_AttachedDownloadStart
 *  @ brief  开启附属下载
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 下载附属段开始时间
 *  @ param  end_time		- [in] 下载附属段结束时间
 *  @ param  offset			- [in] 下载数据偏移(用于断点续传),单位:字节
 *  @ param  cb				- [in] 下载数据回调函数
 *  @ param  user_data		- [in] 用户自定义数据
 *  @ param  handle			- [out]附属下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedDownloadStart(unsigned long long login_id, 
															 const char* camera_id, 
															 HCS_Time begin_time, 
															 HCS_Time end_time, 
															 unsigned long long offset, 
															 AttachedDownloadCb cb, 
															 void* user_data, 
															 int* handle);



/** @ fn     HCS_SDK_AttachedDownloadStop
 *  @ brief  停止附属下载
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  handle			- [in] 附属下载操作句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedDownloadStop(unsigned long long login_id, 
															int handle);


/******************************************************************************
 *                                                                            *
 *                               备份操作									  *
 *                                                                            *
 ******************************************************************************/
/** @ fn     HCS_SDK_DataBackup
 *  @ brief  数据备份[最少要有两台VS，不能备份到本机]
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  camera_id		- [in] 编码器ID
 *  @ param  begin_time		- [in] 备份开始时间点
 *  @ param  end_time		- [in] 备份结束时间点[最大支持3天的录像备份]
 *  @ param  data_type		- [in] 数据类型1：录像 2：视频文件 3：图片 10：智能流
 *  @ param  pool_id		- [in] 资源池ID[最大支持32个字符]
 *  @ param  pool_type		- [in] 目的资源池类型(不必须)；0：通用池 1：视频池 3：图片池 4：文件池
 *  @ param  cloud_ip_port	- [in] 云IP和端口，可以是本云或异云的IP端口;格式：IP:PORT1:PORT2 PORT1：信令端口 PORT2：数据端口
 *  @ param  replication	- [in] 备份数[ 1-1份 2-双份]
 *  @ param  do_begin_time	- [in] 每天执行备份任务的开始时间,单位:分钟
 *  @ param  do_end_time	- [in] 每天执行备份任务的结束时间,单位:分钟
 *  @ param  task_id		- [out] 备份任务ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DataBackup(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_Time begin_time, 
												  HCS_Time end_time, 
												  unsigned int data_type, 
												  const char* pool_id, 
												  unsigned int pool_type, 
												  const char* cloud_ip_port, 
												  unsigned int replication, 
												  unsigned long long do_begin_time, 
												  unsigned long long do_end_time, 
												  unsigned long long* task_id);



/** @ fn     HCS_SDK_GetTaskInfo
 *  @ brief  获取任务信息
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  task_id		- [in] 任务ID
 *  @ param  task_info		- [out] 任务信息
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetTaskInfo(unsigned long long login_id, 
												   unsigned long long task_id, 
												   HCS_TaskInfo* task_info);



/** @ fn     HCS_SDK_TaskDelete
 *  @ brief  任务删除
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  task_id		- [in] 任务ID
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TaskDelete(unsigned long long login_id, 
												  unsigned long long task_id);



/** @ fn     HCS_SDK_SetTaskNotifyCallBack
 *  @ brief  设置任务通知回调 
 *  @ param  login_id		- [in] 云存储登录ID
 *  @ param  TaskNotifyCb	- [in] 任务通知回调[回调设为NULL表示取消任务通知注册]
 *  @ param  user_data		- [in] 用户数据
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetTaskNotifyCallBack(unsigned long long login_id, 
															 TaskNotifyCb cb, 
															 void* user_data);


/******************************************************************************
 *                                                                            *
 *                               智能检索									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn	 HCS_SDK_SmartSearchStart
 *  @ brief	 开启智能检索
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 param					-[in] 智能检索参数
 *  @ param	 handle					-[out] 智能检索句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchStart(unsigned long long login_id, 
														const HCS_SmartSearchParam* param, 
														int* handle);



/** @ fn	 HCS_SDK_SmartSearchNextInfo
 *  @ brief	 智能检索，检索下一条信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ param	 result					-[out]保存搜索结果信息
 *  @ param	 status					-[out]检索结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchNextInfo(unsigned long long login_id, 
														   int handle, 
														   HCS_SmartSearchRet* result, 
														   int* status);



/** @ fn     HCS_SDK_SmartSearchStop
 *  @ brief	 停止智能检索
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchStop(unsigned long long login_id, 
													   int handle);



/** @ fn	 HCS_SDK_GetSmartIPCAbility
 *  @ brief	 获取智能IPC能力
 *  @ param	 login_id					-[in] 云存储登录ID
 *  @ param	 camera_id					-[in] 智能IPC的ID
 *  @ param	 buff						-[in] 存放能力集XML的Buf
 *  @ param	 buff_len					-[in] 存放能力集XML的Buf的长度
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetSmartIPCAbility(unsigned long long login_id, 
														  const char* camera_id, 
														  char* buff, 
														  unsigned int buff_len);



/******************************************************************************
 *                                                                            *
 *                              以图搜图									  *
 *                                                                            *
 ******************************************************************************/


/** @ fn	 HCS_SDK_SBPStart
 *  @ brief	 开启以图搜图
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 cameras				-[in] 编码器ID二维数据
 *  @ param  camera_num				-[in] 编码器数量
 *  @ param  begin_time				-[in] 查询图片的开始时间 
 *  @ param  end_time				-[in] 查询图片的结束时间 
 *  @ param  pic_mode				-[in] 查询图片的建模数据
 *  @ param	 handle					-[out] 以图搜图句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPStart(unsigned long long login_id, 
												const char* cameras[HCS_CAMERA_ID_LEN], 
												unsigned int camera_num, 
												HCS_Time begin_time, 
												HCS_Time end_time,
												const HCS_PicModel* pic_mode,
												int* handle);


/** @ fn	 HCS_SDK_SBPNextInfo
 *  @ brief	 以图搜图，检索下一条信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ param	 result					-[out]保存搜索结果信息
 *  @ param	 status					-[out]检索结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ param	 speed					-[out]以图搜图进度[0-100]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPNextInfo(unsigned long long login_id, 
												   int handle, 
												   HCS_SBPRet* result, 
												   int* status,
												   int* speed);


/** @ fn     HCS_SDK_SBPStop
 *  @ brief	 停止以图搜图
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPStop(unsigned long long login_id, 
											   int handle);




/******************************************************************************
 *                                                                            *
 *                              标注管理									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_SetTag
 *  @ brief	 设置标注
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 camera_id				-[in] 编码器ID
 *  @ param	 tag_time				-[in] 备注时间点
 *  @ param	 tag					-[in] 标注内容
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetTag(unsigned long long login_id, 
											  const char* camera_id,
											  HCS_Time tag_time,
											  const char* tag);




/** @ fn	 HCS_SDK_TagSearchStart
 *  @ brief	 开始标注查询
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 camera_id				-[in] 编码器ID
 *  @ param  begin_time				-[in] 查询的开始时间 
 *  @ param  end_time				-[in] 查询的结束时间 
 *  @ param	 tag_key				-[in] 标注内容关键字
 *  @ param  count					-[out]标注数目
 *  @ param	 handle					-[out] 标注查询句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchStart(unsigned long long login_id, 
													  const char* camera_id, 
													  HCS_Time begin_time, 
													  HCS_Time end_time,
													  const char* tag_key,
													  int* count,
													  int* handle);


/** @ fn	 HCS_SDK_TagSearchNext
 *  @ brief	 标注查询，检索下一条信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ param	 result					-[out]保存搜索结果信息
 *  @ param	 status					-[out]检索结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchNext(unsigned long long login_id, 
													 int handle, 
													 HCS_TagInfo* result, 
													 int* status);


/** @ fn     HCS_SDK_TagSearchStop
 *  @ brief	 停止标注查询
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchStop(unsigned long long login_id, 
													 int handle);



/** @ fn     HCS_SDK_DelTag
 *  @ brief	 删除标注
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 camera_id				-[in] 编码器ID
 *  @ param  begin_time				-[in] 标注开始时间 
 *  @ param  end_time				-[in] 标注结束时间 
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelTag(unsigned long long login_id, 
											  const char* camera_id,
											  HCS_Time begin_time, 
											  HCS_Time end_time);



/** @ fn	 HCS_SDK_RecSearchByTagStart
 *  @ brief	 开始根据标注查询录像段信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 camera_id				-[in] 编码器ID
 *  @ param	 tag_key				-[in] 标注内容关键字
 *  @ param  count					-[out]录像段数目
 *  @ param	 handle					-[out] 录像查询句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagStart(unsigned long long login_id, 
														   const char* camera_id, 
														   const char* tag_key,
														   int* count,
														   int* handle);


/** @ fn	 HCS_SDK_RecSearchByTagNext
 *  @ brief	 根据标注查询录像段信息，检索下一条信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ param	 result					-[out]保存搜索结果信息
 *  @ param	 status					-[out]检索结果[1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束]
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagNext(unsigned long long login_id, 
														  int handle, 
														  HCS_RecordInfo* result, 
														  int* status);


/** @ fn     HCS_SDK_TagSearchStop
 *  @ brief	 停止根据标注查询录像段信息
 *  @ param	 login_id				-[in] 云存储登录ID
 *  @ param	 handle					-[in] 搜索句柄
 *  @ return HCS_OK-成功 其他-错误码
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagStop(unsigned long long login_id, 
														  int handle);


#endif // HCS_SDK_H_
