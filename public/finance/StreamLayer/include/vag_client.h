/**	@file vag_client.h
*   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*   @brief 设备接入网关客户端
*
*	@author liyingtao@hikvision.com
*	@date 2013年01月15日
*
*	@note 历史记录：V1.0.0
*	@note 
*
*	@warning
*/

#ifndef _VAG_CLIENT_H_
#define _VAG_CLIENT_H_

#include "vag_client_define.h"

#if defined(_WIN32) || defined(_WIN64)
#if defined(VAG_CLIENT_EXPORTS)
#define VAG_CLIENT_DECLARE extern "C" __declspec(dllexport)
#else
#define VAG_CLIENT_DECLARE extern "C" __declspec(dllimport)
#endif
#elif defined(OS_POSIX)
#define VAG_CLIENT_DECLARE extern "C"
#else
#error OS Not Implement Yet.
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

namespace VagClient
{
	/**********************  SDK初始化相关接口  begin**********************************/
	/**	@fn	int VAG_Init()
	 *	@brief	<初始化接口>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Init();

	/**	@fn	int VAG_Fini()
	 *	@brief	<资源释放接口,关闭所有的连接>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Fini();

	/**	@fn	int VAG_Login(const VAG_CLIENT_LOGIN_PARA* pstLoginPara)
	 *	@brief	<登录到设备接入网关>
	 *	@param <pstLoginPara (IN)登陆参数>
	 *	@param <pstVagMediaInfo (OUT)VAG媒体网关信息>
	 *	@return	>0 成功,返回一个会话句柄;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Login(const VAG_LOGIN_PARA* pstLoginPara, VAG_MEDIA_INFO* pstVagMediaInfo);

	/**	@fn	int VAG_Logout(int iUserHandle)
	 *	@brief	<退出登录>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Logout(int iClientHandle);
	
	/**	@fn	int VAG_BindLocalIP(VAG_BIND_LOCALIP_INFO* pstLocalIpPara)
	 *	@brief	<设置客户端网络操作绑定IP>
	 *	@param <pstLocalIpPara (IN)网络操作绑定IP>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_BindLocalIP(VAG_BIND_LOCALIP_INFO* pstLocalIpPara);
	/**********************  SDK初始化相关接口  end***********************************/

	/**********************  资源信息相关接口  begin**********************************/
	/**	@fn	int VAG_DownloadResource(int iClientHandle)
	 *	@brief	<从设备接入网关下载所有资源信息>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@return	成功返回0;错误时返回-1.下载完成后使用VAG_QueryResource()获取资源信息.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_DownloadResource(int iClientHandle);

	/**	@fn	int VAG_QueryResource(int iClientHandle, int iResourceType)
	 *	@brief	<查询指定类型资源>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <iResourceType (IN)资源类型>
	 *	@return	成功返回0;错误时返回-1.查询成功后使用VAG_NextResource()逐条获取资源信息.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResource(int iClientHandle, int iResourceType);

	/**	@fn	int VAG_NextResource(int iClientHandle, int iResourceType, void* pstResVal)
	 *	@brief	<向后移动数据访问游标,获取下一个指定类型资源>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <iResourceType (IN)资源类型>
	 *	@param <pstResVal (OUT)返回的下一个指定类型资源值>
	 *	@return	数据未读取完返回0;数据读取完成返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_NextResource(int iClientHandle, int iResourceType, void* pstResVal);

	/**	@fn	int VAG_QueryResVal(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, void* pstResVal)
	 *	@brief	<获取指定类型资源>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstQureyPara (IN)查询参数>
	 *	@param <pstResVal (OUT)返回的指定类型资源值>
	 *	@return	成功返回0;失败返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResVal(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, void* pstResVal);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<获取资源状态>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstQureyPara (IN)查询参数>
	 *	@param <piResStatus (OUT)当前状态, 1为在线 0为不在线>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus);

	/**	@fn	int VAG_QueryDeviceWorkStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, char* pOutBuffer, int iOutLen)
	 *	@brief	<获取设备工作状态>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstQureyPara (IN)查询参数>
	 *	@param <pOutBuffer (OUT)状态信息输出缓冲区指针>
	 *	@param <iOutLen (OUT)输出缓冲区长度>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryDeviceWorkStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, char* pOutBuffer, int iOutLen);
	/**********************  资源信息相关接口   end***************************************/

	/*************************  取流相关接口  begin***************************************/
	/**	@fn	int VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<实时视频取流>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstOpenStreamPara (IN)取流参数>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara);
	
	/**	@fn	int VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<实时视频取流>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstOpenStreamPara (IN)取流参数>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartStreamEx(int iClientHandle, VAG_OPEN_STREAM_PARAEX* pstOpenStreamPara);

	/**	@fn	int VAG_StopStream(int iClientHandle, int iStreamHandle)
	 *	@brief	<停止实时取流>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <iStreanHandle (IN)VAG_StartStream返回的句柄>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopStream(int iClientHandle, int iStreamHandle);

	/**	@fn	int VAG_StartDeviceStream(int iClientHandle, VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<从设备实时视频取流>
	 *	@param <pstOpenStreamPara (IN)取流参数>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceStream(VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara);

	/**	@fn	int VAG_StartDeviceStream(int iClientHandle, VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<从设备实时视频取流>
	 *	@param <pstOpenStreamPara (IN)取流参数>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceStreamEx(VAG_DIRECT_OPEN_STREAM_PARAEX* pstOpenStreamPara);

	/**	@fn	int VAG_StopDeviceStream(int iClientHandle, int iStreamHandle)
	 *	@brief	<停止从设备实时视频取流>
	 *	@param <iStreanHandle (IN)VAG_StartDeviceStream返回的句柄>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopDeviceStream(int iStreamHandle);

	/**	@fn	int VAG_GetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara)
	 *	@brief	<获取取流视频参数>
	 *	@param <iStreanHandle (IN)VAG_StartStream返回的句柄>
	 *	@param <pstOpenStreamPara (OUT)取流视频参数>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara);

	/**	@fn	int VAG_SetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara)
	 *	@brief	<设置取流视频参数>
	 *	@param <iStreanHandle (IN)VAG_StartStream返回的句柄>
	 *	@param <pstOpenStreamPara (IN)取流视频参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara);
	/*************************  取流相关接口   end****************************************/

	/*************************  设备录像相关接口  begin***********************************/
	/**	@fn	int VAG_GetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara)
	 *	@brief	<查询设备录像计划>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstConfigPara (IN)查询参数>
	 *	@param <pstRecordConfigPara (OUT)设备录像计划配置>
	 *	@return	错误时返回-1;成功返回其他,表示取流句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara);

	/**	@fn	int VAG_SetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara)
	 *	@brief	<设置设备录像计划>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstConfigPara (IN)设置参数>
	 *	@param <pstRecordConfigPara (OUT)设备录像计划配置>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara);

	/**	@fn	int VAG_FindRecordFiles(int iClientHandle, LPVAG_FIND_RECORD_PARA pstFindPara)
	 *	@brief	<按时间查询录像文件>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstFindPara (IN)录像查询参数>
	 *	@return	错误时返回-1;成功返回其他,表示查询句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_FindRecordFiles(int iClientHandle, LPVAG_FIND_RECORD_PARA pstFindPara);

	/**	@fn	int VAG_FindNextRecordFile(int iFileHandle, LPVAG_RECORD_FILE_INFO pstRecordFileInfo)
	 *	@brief	<获取下一条录像文件信息,在执行VAG_FindRecordFiles之后调用>
	 *	@param <iFileHandle (IN)VAG_FindRecordFiles返回的句柄>
	 *	@param <pstFindPara (IN)录像查询参数>
	 *	@return	录像文件未获取完返回0;录像文件获取完成返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_FindNextRecordFile(int iFileHandle, LPVAG_RECORD_FILE_INFO pstRecordFileInfo);
	
    /** @fn int VAG_CloseFindRecord(int iFileHandle)
     *   @brief <关闭文件查找句柄>
     *   @param <iFileHandle (IN)VAG_FindRecordFiles返回的句柄>
     *   @return 成功返回0;错误时返回-1.
     *   @note 
     */
    VAG_CLIENT_DECLARE int CALLBACK VAG_CloseFindRecord(int iFileHandle);

	/** @fn int VAG_StartPlayBackEx(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara)
	 *  @brief <开始回放取流>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstPlaybackPara (IN)回放取流参数>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartPlayBackEx(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara);
	
	/** @fn int VAG_StopPlayBack(int iPlayHandle)
	 *  @brief <停止回放取流>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopPlayBack(int iPlayHandle);

	/** @fn int VAG_PausePlayBack(int iPlayHandle)
	 *  @brief <暂停回放取流>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
 	VAG_CLIENT_DECLARE int CALLBACK VAG_PausePlayBack(int iPlayHandle);

	/** @fn int VAG_ResumePlayBack(int iPlayHandle)
	 *  @brief <恢复回放取流>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_ResumePlayBack(int iPlayHandle);

	/** @fn int VAG_SetPlayBackScale(int iPlayHandle, double fScale)
	 *  @brief <设置视频回放进度>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @param <fScale (IN)回放进度>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetPlayBackScale(int iPlayHandle, double fScale);

	/** @fn int VAG_SeekPlayBackByRelTime(int iPlayHandle, int iRelFrom, int iRelTo)
	 *  @brief <相对时间定位回放>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @param <iRelFrom (IN)相对开始时间>
	 *  @param <iRelTo (IN)相对结束时间>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SeekPlayBackByRelTime(int iPlayHandle, int iRelFrom, int iRelTo);

	/** @fn int VAG_SeekPlayBackByAbsTime(int iPlayHandle, LPVAG_TIME_INFO pstAbsFrom, LPVAG_TIME_INFO pstAbsTo)
	 *  @brief <绝对时间定位回放>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx返回的回放句柄>
	 *  @param <pstAbsFrom (IN)绝对开始时间>
	 *  @param <pstAbsTo (IN)绝对开始时间>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SeekPlayBackByAbsTime(int iPlayHandle, LPVAG_TIME_INFO pstAbsFrom, LPVAG_TIME_INFO pstAbsTo);
    /** @fn int VAG_StartPlayBackByReverse(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara)
    *  @brief <开始倒放取流>
    *	@param <iClientHandle (IN)VAG_Login返回的句柄>
    *  @param <pstPlaybackPara (IN)倒放取流参数>
    *  @return 成功返回0;错误时返回-1.
    *  @note 
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_StartPlayBackByReverse(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara);

    /** @fn int VAG_StopPlayBackByReverse(int iPlayHandle)
    *  @brief <停止倒放取流>
    *  @param <iPlayHandle (IN)VAG_StartPlayBackByReverse返回的倒放句柄>
    *  @return 成功返回0;错误时返回-1.
    *  @note 
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_StopPlayBackByReverse(int iPlayHandle);
	/*************************  设备录像相关接口   end************************************/

	/*************************  云台控制相关接口  begin***********************************/
	/**	@fn	int VAG_ControlPtz(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara)
	 *	@brief	<云台控制>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstPtzControlPara (IN)云台控制参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_ControlPtz(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<获取云台控制参数>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstPtzControlPara (IN)云台控制参数>
	 *	@param <pstPtzConfigPara (OUT)获取云台控制参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetPtzConfig(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara, VAG_PTZ_CFG_INFO* pstPtzConfigPara);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<设置云台控制参数>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstPtzConfigPara (IN)设置云台控制参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetPtzConfig(int iClientHandle, VAG_PTZ_CFG_INFO* pstPtzConfigPara);
	/*************************  云台控制相关接口   end************************************/

	/*************************  设备参数配置相关接口  begin********************************/
	/** @fn int VAG_GetDevConfigParam(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pOutBuffer, int iOutBufferSize, int* pRetLen)
	 *  @brief <获取设备参数>
	 *  @param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstConfigPara (IN)接口调用参数>
	 *  @param <pOutBuffer (OUT)获取的参数缓冲指针>
	 *  @param <nOutBufferSize (IN)接收获取的参数缓冲长度,不能为0>
	 *  @param <pRetLen (OUT)实际收到的数据长度指针，不能为NULL>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetDevConfigParam(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pOutBuffer, int iOutBufferSize, int* pRetLen);

	/** @fn int DeviceObject_SetConfigParam(HPR_HANDLE pHandle, HPR_ULONG nCommand, HPR_VOIDPTR pInBuffer, HPR_ULONG nInBufferSize, HPR_VOIDPTR pUserData)
	 *  @brief <设置设备参数>
	 *  @param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstConfigPara (IN)接口调用参数>
	 *  @param <pInBuffer (IN)设置的参数缓冲指针>
	 *  @param <nInBufferSize (IN)设置的参数缓冲长度,不能为0>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetDevConfigParam(int iClientHandle, VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize);

	/** @fn int VAG_GetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize,
										void* pOutBuffer, int iOutBufferSize, int* pRetLen)
	 *  @brief <透传方式获取设备参数>
	 *  @param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstConfigPara (IN)接口调用参数>
	 *  @param <pInBuffer (IN)输入参数缓冲指针>
	 *  @param <nInBufferSize (IN)输入参数缓冲长度,不能为0>
	 *  @param <pOutBuffer (OUT)获取的参数缓冲指针>
	 *  @param <nOutBufferSize (IN)接收获取的参数缓冲长度,不能为0>
	 *  @param <pRetLen (OUT)实际收到的数据长度指针，不能为NULL>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize, 
															void* pOutBuffer, int iOutBufferSize, int* pRetLen);

	/** @fn int VAG_SetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize)
	 *  @brief <透传方式设置设备参数>
	 *  @param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstConfigPara (IN)接口调用参数>
	 *  @param <pInBuffer (IN)输入参数缓冲指针>
	 *  @param <nInBufferSize (IN)输入参数缓冲长度,不能为0>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize);

	/** @fn int VAG_GetDeviceAbility(int iClientHandle, VAG_DEV_ABILITY_PARA* pstAbilityPara, void* pOutBuffer, int iOutBufferSize)
	 *  @brief <获取设备能力集>
	 *  @param <iClientHandle (IN)VAG_Login返回的句柄>
	 *  @param <pstConfigPara (IN)接口调用参数>
	 *  @param <pOutBuffer (OUT)输出参数缓冲指针>
	 *  @param <iOutBufferSize (OUT)输出参数缓冲长度>
	 *  @return 成功返回0;错误时返回-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetDeviceAbility(int iClientHandle, VAG_DEV_ABILITY_PARA* pstAbilityPara, void* pOutBuffer, int iOutBufferSize);
	/*************************  设备参数配置相关接口  end*********************************/

	/*************************  设备语音对讲相关接口  begin*******************************/
	/**	@fn	int VAG_StartDeviceTalk(int iClientHandle, VAG_DEVICE_TALK_INFO* pstDeviceTalkInfo)
	 *	@brief	<开始设备语音对讲>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstDeviceTalkInfo (IN)设备语音对讲参数>
	 *	@return	错误时返回-1;成功返回其他,表示对讲句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceTalk(int iClientHandle, VAG_DEVICE_TALK_INFO* pstDeviceTalkInfo);

	/**	@fn	int VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle)
	 *	@brief	<停止设备语音对讲>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <iTalkHandle (IN)VAG_StartDeviceTalk返回的句柄>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle);

	/**	@fn	int VAG_SendTalkData(int iClientHandle, VAG_DEVICE_TALK_DATA* pstDeviceTalkData)
	 *	@brief	<发送设备语音对讲数据>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstDeviceTalkData (IN)设备语音对讲数据结构体>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SendTalkData(int iClientHandle, VAG_DEVICE_TALK_DATA* pstDeviceTalkData);
	/*************************  设备语音对讲相关接口   end********************************/

	/*************************  事件订阅相关接口  begin**********************************/
	/**	@fn	int VAG_SubscribeEvent(int iClientHandle, VAG_SUBSCRIBE_EVENT_PARA* pstSubscribeEventPara)
	 *	@brief	<订阅事件>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstSubscribeEventPara (IN)事件订阅参数>
	 *	@return	错误时返回-1;成功返回其他,表示对讲句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SubscribeEvent(int iClientHandle, VAG_SUBSCRIBE_EVENT_PARA* pstSubscribeEventPara);

	/**	@fn	int VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle)
	 *	@brief	<取消订阅事件>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstDeSubscribeEventPara (IN)取消事件订阅参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_DeSubscribeEvent(int iClientHandle, VAG_DESUBSCRIBE_EVENT_PARA* pstDeSubscribeEventPara);
	/*************************  事件订阅相关接口   end***********************************/
	
	/**	@fn	int VAG_CaptureImg(int iClientHandle, LPVAG_CAPTURE_PARA pstCapturePara, char* pImgBuffer, int iBufferSize, int* pSizeReturned)
	 *	@brief	<设备抓图(返回图片数据)>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstCapturePara (IN)抓图参数>
	 *	@param <pImgBuffer (IN)保存图片数据的缓冲区>
	 *	@param <iBufferSize (IN)输入缓冲区大小>
	 *	@param <pSizeReturned (OUT)返回图片数据的大小>
	 *	@return	错误时返回-1;成功返回其他,表示对讲句柄.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_CaptureImg(int iClientHandle, LPVAG_CAPTURE_PARA pstCapturePara, char* pImgBuffer, int iBufferSize, int* pSizeReturned);

    /**	@fn	int VAG_CapturePicSave(int iClientHandle, LPVAG_PICTURE2SERVER_PARA pstCapturePara, char** pstrRetPicUr)
    *	@brief	<设备抓图(返回图片地址信息)>
    *	@param <iClientHandle (IN)VAG_Login返回的句柄>
    *	@param <pstCapturePara (IN)抓图参数>
    *	@param <pstrRetPicUrl (IN)返回图片URL，格式ftp://10.64.51.26:21/2013-11-18/Pic_20131118144906312500.jpg>
    *	@param <iRetPicUrlSize (IN)输入缓冲区大小>
    *	@return	错误时返回-1;成功0.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_CapturePicSave(int iClientHandle, LPVAG_PICTURE2SERVER_PARA pstCapturePara, char* pstrRetPicUrl, int iRetPicUrlSize);

	/**	@fn	int VAG_SetIOStatus(int iClientHandle, LPVAG_IO_SET_PARA pstIOSetPara)
	 *	@brief	<设置设备IO状态>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstIOSetPara (IN)IO控制参数>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetIOStatus(int iClientHandle, LPVAG_IO_SET_PARA pstIOSetPara);

	/**	@fn	int VAG_GetIOStatus(int iClientHandle, LPVAG_IO_GET_PARA pstIOGetPara, int* pIoStatus)
	 *	@brief	<获取设备IO状态>
	 *	@param <iClientHandle (IN)VAG_Login返回的句柄>
	 *	@param <pstIOSetPara (IN)获取IO状态输入参数>
	 *	@param <pIoStatus (OUT)IO状态:0-关闭,1-打开>
	 *	@return	成功返回0;错误时返回-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetIOStatus(int iClientHandle, LPVAG_IO_GET_PARA pstIOGetPara, int* pIoStatus);

	/**	@fn	VAG_ERROR_CODE VAG_GetLastError()
	 *	@brief	<获取错误信息>
	 *	@return	成功返回0;错误时返回错误码.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetLastError();

    /**	@fn	int VAG_SubscribeGps(int iClientHandle, VAG_SUBSCRIBE_GPS_PARA* pstSubscribeGpsPara)
    *	@brief	<订阅GPS信息>
    *	@param <iClientHandle (IN)VAG_Login返回的句柄>
    *	@param <pstSubscribeEventPara (IN)GPS订阅参数>
    *	@return	错误时返回-1;成功返回其他,表示对讲句柄.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_SubscribeGps(int iClientHandle, VAG_SUBSCRIBE_GPS_PARA* pstSubscribeGpsPara);

    /**	@fn	int VAG_DeSubscribeGps(int iClientHandle, VAG_DESUBSCRIBE_GPS_PARA* pstDeSubscribeGPSPara)
    *	@brief	<取消订阅GPS>
    *	@param <iClientHandle (IN)VAG_Login返回的句柄>
    *	@param <pstDeSubscribeEventPara (IN)取消GPS订阅参数>
    *	@return	成功返回0;错误时返回-1.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_DeSubscribeGps(int iClientHandle, VAG_DESUBSCRIBE_GPS_PARA* pstDeSubscribeGPSPara);

}
#endif
