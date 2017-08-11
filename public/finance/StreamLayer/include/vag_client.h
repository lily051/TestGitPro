/**	@file vag_client.h
*   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*   @brief �豸�������ؿͻ���
*
*	@author liyingtao@hikvision.com
*	@date 2013��01��15��
*
*	@note ��ʷ��¼��V1.0.0
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
	/**********************  SDK��ʼ����ؽӿ�  begin**********************************/
	/**	@fn	int VAG_Init()
	 *	@brief	<��ʼ���ӿ�>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Init();

	/**	@fn	int VAG_Fini()
	 *	@brief	<��Դ�ͷŽӿ�,�ر����е�����>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Fini();

	/**	@fn	int VAG_Login(const VAG_CLIENT_LOGIN_PARA* pstLoginPara)
	 *	@brief	<��¼���豸��������>
	 *	@param <pstLoginPara (IN)��½����>
	 *	@param <pstVagMediaInfo (OUT)VAGý��������Ϣ>
	 *	@return	>0 �ɹ�,����һ���Ự���;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Login(const VAG_LOGIN_PARA* pstLoginPara, VAG_MEDIA_INFO* pstVagMediaInfo);

	/**	@fn	int VAG_Logout(int iUserHandle)
	 *	@brief	<�˳���¼>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_Logout(int iClientHandle);
	
	/**	@fn	int VAG_BindLocalIP(VAG_BIND_LOCALIP_INFO* pstLocalIpPara)
	 *	@brief	<���ÿͻ������������IP>
	 *	@param <pstLocalIpPara (IN)���������IP>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_BindLocalIP(VAG_BIND_LOCALIP_INFO* pstLocalIpPara);
	/**********************  SDK��ʼ����ؽӿ�  end***********************************/

	/**********************  ��Դ��Ϣ��ؽӿ�  begin**********************************/
	/**	@fn	int VAG_DownloadResource(int iClientHandle)
	 *	@brief	<���豸������������������Դ��Ϣ>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@return	�ɹ�����0;����ʱ����-1.������ɺ�ʹ��VAG_QueryResource()��ȡ��Դ��Ϣ.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_DownloadResource(int iClientHandle);

	/**	@fn	int VAG_QueryResource(int iClientHandle, int iResourceType)
	 *	@brief	<��ѯָ��������Դ>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <iResourceType (IN)��Դ����>
	 *	@return	�ɹ�����0;����ʱ����-1.��ѯ�ɹ���ʹ��VAG_NextResource()������ȡ��Դ��Ϣ.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResource(int iClientHandle, int iResourceType);

	/**	@fn	int VAG_NextResource(int iClientHandle, int iResourceType, void* pstResVal)
	 *	@brief	<����ƶ����ݷ����α�,��ȡ��һ��ָ��������Դ>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <iResourceType (IN)��Դ����>
	 *	@param <pstResVal (OUT)���ص���һ��ָ��������Դֵ>
	 *	@return	����δ��ȡ�귵��0;���ݶ�ȡ��ɷ���-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_NextResource(int iClientHandle, int iResourceType, void* pstResVal);

	/**	@fn	int VAG_QueryResVal(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, void* pstResVal)
	 *	@brief	<��ȡָ��������Դ>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstQureyPara (IN)��ѯ����>
	 *	@param <pstResVal (OUT)���ص�ָ��������Դֵ>
	 *	@return	�ɹ�����0;ʧ�ܷ���-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResVal(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, void* pstResVal);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<��ȡ��Դ״̬>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstQureyPara (IN)��ѯ����>
	 *	@param <piResStatus (OUT)��ǰ״̬, 1Ϊ���� 0Ϊ������>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus);

	/**	@fn	int VAG_QueryDeviceWorkStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, char* pOutBuffer, int iOutLen)
	 *	@brief	<��ȡ�豸����״̬>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstQureyPara (IN)��ѯ����>
	 *	@param <pOutBuffer (OUT)״̬��Ϣ���������ָ��>
	 *	@param <iOutLen (OUT)�������������>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_QueryDeviceWorkStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, char* pOutBuffer, int iOutLen);
	/**********************  ��Դ��Ϣ��ؽӿ�   end***************************************/

	/*************************  ȡ����ؽӿ�  begin***************************************/
	/**	@fn	int VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<ʵʱ��Ƶȡ��>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstOpenStreamPara (IN)ȡ������>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara);
	
	/**	@fn	int VAG_StartStream(int iClientHandle, VAG_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<ʵʱ��Ƶȡ��>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstOpenStreamPara (IN)ȡ������>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartStreamEx(int iClientHandle, VAG_OPEN_STREAM_PARAEX* pstOpenStreamPara);

	/**	@fn	int VAG_StopStream(int iClientHandle, int iStreamHandle)
	 *	@brief	<ֹͣʵʱȡ��>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <iStreanHandle (IN)VAG_StartStream���صľ��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopStream(int iClientHandle, int iStreamHandle);

	/**	@fn	int VAG_StartDeviceStream(int iClientHandle, VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<���豸ʵʱ��Ƶȡ��>
	 *	@param <pstOpenStreamPara (IN)ȡ������>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceStream(VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara);

	/**	@fn	int VAG_StartDeviceStream(int iClientHandle, VAG_DIRECT_OPEN_STREAM_PARA* pstOpenStreamPara, void* pUser)
	 *	@brief	<���豸ʵʱ��Ƶȡ��>
	 *	@param <pstOpenStreamPara (IN)ȡ������>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceStreamEx(VAG_DIRECT_OPEN_STREAM_PARAEX* pstOpenStreamPara);

	/**	@fn	int VAG_StopDeviceStream(int iClientHandle, int iStreamHandle)
	 *	@brief	<ֹͣ���豸ʵʱ��Ƶȡ��>
	 *	@param <iStreanHandle (IN)VAG_StartDeviceStream���صľ��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopDeviceStream(int iStreamHandle);

	/**	@fn	int VAG_GetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara)
	 *	@brief	<��ȡȡ����Ƶ����>
	 *	@param <iStreanHandle (IN)VAG_StartStream���صľ��>
	 *	@param <pstOpenStreamPara (OUT)ȡ����Ƶ����>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara);

	/**	@fn	int VAG_SetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara)
	 *	@brief	<����ȡ����Ƶ����>
	 *	@param <iStreanHandle (IN)VAG_StartStream���صľ��>
	 *	@param <pstOpenStreamPara (IN)ȡ����Ƶ����>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetVideoParam(int iStreamHandle, VAG_CFG_VIDEO_IMAGE_PARA* pstVideoImagePara);
	/*************************  ȡ����ؽӿ�   end****************************************/

	/*************************  �豸¼����ؽӿ�  begin***********************************/
	/**	@fn	int VAG_GetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara)
	 *	@brief	<��ѯ�豸¼��ƻ�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstConfigPara (IN)��ѯ����>
	 *	@param <pstRecordConfigPara (OUT)�豸¼��ƻ�����>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾȡ�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara);

	/**	@fn	int VAG_SetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara)
	 *	@brief	<�����豸¼��ƻ�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstConfigPara (IN)���ò���>
	 *	@param <pstRecordConfigPara (OUT)�豸¼��ƻ�����>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetRecordConfig(int iClientHandle, LPVAG_RECORD_CONFIG_PARA pstConfigPara, LPVAG_RECORD_CONFIG_INFO pstRecordConfigPara);

	/**	@fn	int VAG_FindRecordFiles(int iClientHandle, LPVAG_FIND_RECORD_PARA pstFindPara)
	 *	@brief	<��ʱ���ѯ¼���ļ�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstFindPara (IN)¼���ѯ����>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾ��ѯ���.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_FindRecordFiles(int iClientHandle, LPVAG_FIND_RECORD_PARA pstFindPara);

	/**	@fn	int VAG_FindNextRecordFile(int iFileHandle, LPVAG_RECORD_FILE_INFO pstRecordFileInfo)
	 *	@brief	<��ȡ��һ��¼���ļ���Ϣ,��ִ��VAG_FindRecordFiles֮�����>
	 *	@param <iFileHandle (IN)VAG_FindRecordFiles���صľ��>
	 *	@param <pstFindPara (IN)¼���ѯ����>
	 *	@return	¼���ļ�δ��ȡ�귵��0;¼���ļ���ȡ��ɷ���-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_FindNextRecordFile(int iFileHandle, LPVAG_RECORD_FILE_INFO pstRecordFileInfo);
	
    /** @fn int VAG_CloseFindRecord(int iFileHandle)
     *   @brief <�ر��ļ����Ҿ��>
     *   @param <iFileHandle (IN)VAG_FindRecordFiles���صľ��>
     *   @return �ɹ�����0;����ʱ����-1.
     *   @note 
     */
    VAG_CLIENT_DECLARE int CALLBACK VAG_CloseFindRecord(int iFileHandle);

	/** @fn int VAG_StartPlayBackEx(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara)
	 *  @brief <��ʼ�ط�ȡ��>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstPlaybackPara (IN)�ط�ȡ������>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartPlayBackEx(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara);
	
	/** @fn int VAG_StopPlayBack(int iPlayHandle)
	 *  @brief <ֹͣ�ط�ȡ��>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopPlayBack(int iPlayHandle);

	/** @fn int VAG_PausePlayBack(int iPlayHandle)
	 *  @brief <��ͣ�ط�ȡ��>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
 	VAG_CLIENT_DECLARE int CALLBACK VAG_PausePlayBack(int iPlayHandle);

	/** @fn int VAG_ResumePlayBack(int iPlayHandle)
	 *  @brief <�ָ��ط�ȡ��>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_ResumePlayBack(int iPlayHandle);

	/** @fn int VAG_SetPlayBackScale(int iPlayHandle, double fScale)
	 *  @brief <������Ƶ�طŽ���>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @param <fScale (IN)�طŽ���>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetPlayBackScale(int iPlayHandle, double fScale);

	/** @fn int VAG_SeekPlayBackByRelTime(int iPlayHandle, int iRelFrom, int iRelTo)
	 *  @brief <���ʱ�䶨λ�ط�>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @param <iRelFrom (IN)��Կ�ʼʱ��>
	 *  @param <iRelTo (IN)��Խ���ʱ��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SeekPlayBackByRelTime(int iPlayHandle, int iRelFrom, int iRelTo);

	/** @fn int VAG_SeekPlayBackByAbsTime(int iPlayHandle, LPVAG_TIME_INFO pstAbsFrom, LPVAG_TIME_INFO pstAbsTo)
	 *  @brief <����ʱ�䶨λ�ط�>
	 *  @param <iPlayHandle (IN)VAG_StartPlayBackEx���صĻطž��>
	 *  @param <pstAbsFrom (IN)���Կ�ʼʱ��>
	 *  @param <pstAbsTo (IN)���Կ�ʼʱ��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SeekPlayBackByAbsTime(int iPlayHandle, LPVAG_TIME_INFO pstAbsFrom, LPVAG_TIME_INFO pstAbsTo);
    /** @fn int VAG_StartPlayBackByReverse(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara)
    *  @brief <��ʼ����ȡ��>
    *	@param <iClientHandle (IN)VAG_Login���صľ��>
    *  @param <pstPlaybackPara (IN)����ȡ������>
    *  @return �ɹ�����0;����ʱ����-1.
    *  @note 
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_StartPlayBackByReverse(int iClientHandle, LPVAG_PLAY_BACK_PARA pstPlaybackPara);

    /** @fn int VAG_StopPlayBackByReverse(int iPlayHandle)
    *  @brief <ֹͣ����ȡ��>
    *  @param <iPlayHandle (IN)VAG_StartPlayBackByReverse���صĵ��ž��>
    *  @return �ɹ�����0;����ʱ����-1.
    *  @note 
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_StopPlayBackByReverse(int iPlayHandle);
	/*************************  �豸¼����ؽӿ�   end************************************/

	/*************************  ��̨������ؽӿ�  begin***********************************/
	/**	@fn	int VAG_ControlPtz(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara)
	 *	@brief	<��̨����>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstPtzControlPara (IN)��̨���Ʋ���>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_ControlPtz(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<��ȡ��̨���Ʋ���>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstPtzControlPara (IN)��̨���Ʋ���>
	 *	@param <pstPtzConfigPara (OUT)��ȡ��̨���Ʋ���>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetPtzConfig(int iClientHandle, VAG_PTZ_CONTROL_PARA* pstPtzControlPara, VAG_PTZ_CFG_INFO* pstPtzConfigPara);

	/**	@fn	int VAG_QueryResStatus(int iClientHandle, VAG_QUERY_RES_PARA* pstQureyPara, int* piResStatus)
	 *	@brief	<������̨���Ʋ���>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstPtzConfigPara (IN)������̨���Ʋ���>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetPtzConfig(int iClientHandle, VAG_PTZ_CFG_INFO* pstPtzConfigPara);
	/*************************  ��̨������ؽӿ�   end************************************/

	/*************************  �豸����������ؽӿ�  begin********************************/
	/** @fn int VAG_GetDevConfigParam(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pOutBuffer, int iOutBufferSize, int* pRetLen)
	 *  @brief <��ȡ�豸����>
	 *  @param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstConfigPara (IN)�ӿڵ��ò���>
	 *  @param <pOutBuffer (OUT)��ȡ�Ĳ�������ָ��>
	 *  @param <nOutBufferSize (IN)���ջ�ȡ�Ĳ������峤��,����Ϊ0>
	 *  @param <pRetLen (OUT)ʵ���յ������ݳ���ָ�룬����ΪNULL>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetDevConfigParam(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pOutBuffer, int iOutBufferSize, int* pRetLen);

	/** @fn int DeviceObject_SetConfigParam(HPR_HANDLE pHandle, HPR_ULONG nCommand, HPR_VOIDPTR pInBuffer, HPR_ULONG nInBufferSize, HPR_VOIDPTR pUserData)
	 *  @brief <�����豸����>
	 *  @param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstConfigPara (IN)�ӿڵ��ò���>
	 *  @param <pInBuffer (IN)���õĲ�������ָ��>
	 *  @param <nInBufferSize (IN)���õĲ������峤��,����Ϊ0>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetDevConfigParam(int iClientHandle, VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize);

	/** @fn int VAG_GetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize,
										void* pOutBuffer, int iOutBufferSize, int* pRetLen)
	 *  @brief <͸����ʽ��ȡ�豸����>
	 *  @param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstConfigPara (IN)�ӿڵ��ò���>
	 *  @param <pInBuffer (IN)�����������ָ��>
	 *  @param <nInBufferSize (IN)����������峤��,����Ϊ0>
	 *  @param <pOutBuffer (OUT)��ȡ�Ĳ�������ָ��>
	 *  @param <nOutBufferSize (IN)���ջ�ȡ�Ĳ������峤��,����Ϊ0>
	 *  @param <pRetLen (OUT)ʵ���յ������ݳ���ָ�룬����ΪNULL>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize, 
															void* pOutBuffer, int iOutBufferSize, int* pRetLen);

	/** @fn int VAG_SetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize)
	 *  @brief <͸����ʽ�����豸����>
	 *  @param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstConfigPara (IN)�ӿڵ��ò���>
	 *  @param <pInBuffer (IN)�����������ָ��>
	 *  @param <nInBufferSize (IN)����������峤��,����Ϊ0>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetConfigTransparent(int iClientHandle,  VAG_DEV_CONFIG_PARA* pstConfigPara, void* pInBuffer, int iInBufferSize);

	/** @fn int VAG_GetDeviceAbility(int iClientHandle, VAG_DEV_ABILITY_PARA* pstAbilityPara, void* pOutBuffer, int iOutBufferSize)
	 *  @brief <��ȡ�豸������>
	 *  @param <iClientHandle (IN)VAG_Login���صľ��>
	 *  @param <pstConfigPara (IN)�ӿڵ��ò���>
	 *  @param <pOutBuffer (OUT)�����������ָ��>
	 *  @param <iOutBufferSize (OUT)����������峤��>
	 *  @return �ɹ�����0;����ʱ����-1.
     *  @note 
     */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetDeviceAbility(int iClientHandle, VAG_DEV_ABILITY_PARA* pstAbilityPara, void* pOutBuffer, int iOutBufferSize);
	/*************************  �豸����������ؽӿ�  end*********************************/

	/*************************  �豸�����Խ���ؽӿ�  begin*******************************/
	/**	@fn	int VAG_StartDeviceTalk(int iClientHandle, VAG_DEVICE_TALK_INFO* pstDeviceTalkInfo)
	 *	@brief	<��ʼ�豸�����Խ�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstDeviceTalkInfo (IN)�豸�����Խ�����>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾ�Խ����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StartDeviceTalk(int iClientHandle, VAG_DEVICE_TALK_INFO* pstDeviceTalkInfo);

	/**	@fn	int VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle)
	 *	@brief	<ֹͣ�豸�����Խ�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <iTalkHandle (IN)VAG_StartDeviceTalk���صľ��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle);

	/**	@fn	int VAG_SendTalkData(int iClientHandle, VAG_DEVICE_TALK_DATA* pstDeviceTalkData)
	 *	@brief	<�����豸�����Խ�����>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstDeviceTalkData (IN)�豸�����Խ����ݽṹ��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SendTalkData(int iClientHandle, VAG_DEVICE_TALK_DATA* pstDeviceTalkData);
	/*************************  �豸�����Խ���ؽӿ�   end********************************/

	/*************************  �¼�������ؽӿ�  begin**********************************/
	/**	@fn	int VAG_SubscribeEvent(int iClientHandle, VAG_SUBSCRIBE_EVENT_PARA* pstSubscribeEventPara)
	 *	@brief	<�����¼�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstSubscribeEventPara (IN)�¼����Ĳ���>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾ�Խ����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SubscribeEvent(int iClientHandle, VAG_SUBSCRIBE_EVENT_PARA* pstSubscribeEventPara);

	/**	@fn	int VAG_StopDeviceTalk(int iClientHandle, int iTalkHandle)
	 *	@brief	<ȡ�������¼�>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstDeSubscribeEventPara (IN)ȡ���¼����Ĳ���>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_DeSubscribeEvent(int iClientHandle, VAG_DESUBSCRIBE_EVENT_PARA* pstDeSubscribeEventPara);
	/*************************  �¼�������ؽӿ�   end***********************************/
	
	/**	@fn	int VAG_CaptureImg(int iClientHandle, LPVAG_CAPTURE_PARA pstCapturePara, char* pImgBuffer, int iBufferSize, int* pSizeReturned)
	 *	@brief	<�豸ץͼ(����ͼƬ����)>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstCapturePara (IN)ץͼ����>
	 *	@param <pImgBuffer (IN)����ͼƬ���ݵĻ�����>
	 *	@param <iBufferSize (IN)���뻺������С>
	 *	@param <pSizeReturned (OUT)����ͼƬ���ݵĴ�С>
	 *	@return	����ʱ����-1;�ɹ���������,��ʾ�Խ����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_CaptureImg(int iClientHandle, LPVAG_CAPTURE_PARA pstCapturePara, char* pImgBuffer, int iBufferSize, int* pSizeReturned);

    /**	@fn	int VAG_CapturePicSave(int iClientHandle, LPVAG_PICTURE2SERVER_PARA pstCapturePara, char** pstrRetPicUr)
    *	@brief	<�豸ץͼ(����ͼƬ��ַ��Ϣ)>
    *	@param <iClientHandle (IN)VAG_Login���صľ��>
    *	@param <pstCapturePara (IN)ץͼ����>
    *	@param <pstrRetPicUrl (IN)����ͼƬURL����ʽftp://10.64.51.26:21/2013-11-18/Pic_20131118144906312500.jpg>
    *	@param <iRetPicUrlSize (IN)���뻺������С>
    *	@return	����ʱ����-1;�ɹ�0.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_CapturePicSave(int iClientHandle, LPVAG_PICTURE2SERVER_PARA pstCapturePara, char* pstrRetPicUrl, int iRetPicUrlSize);

	/**	@fn	int VAG_SetIOStatus(int iClientHandle, LPVAG_IO_SET_PARA pstIOSetPara)
	 *	@brief	<�����豸IO״̬>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstIOSetPara (IN)IO���Ʋ���>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_SetIOStatus(int iClientHandle, LPVAG_IO_SET_PARA pstIOSetPara);

	/**	@fn	int VAG_GetIOStatus(int iClientHandle, LPVAG_IO_GET_PARA pstIOGetPara, int* pIoStatus)
	 *	@brief	<��ȡ�豸IO״̬>
	 *	@param <iClientHandle (IN)VAG_Login���صľ��>
	 *	@param <pstIOSetPara (IN)��ȡIO״̬�������>
	 *	@param <pIoStatus (OUT)IO״̬:0-�ر�,1-��>
	 *	@return	�ɹ�����0;����ʱ����-1.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetIOStatus(int iClientHandle, LPVAG_IO_GET_PARA pstIOGetPara, int* pIoStatus);

	/**	@fn	VAG_ERROR_CODE VAG_GetLastError()
	 *	@brief	<��ȡ������Ϣ>
	 *	@return	�ɹ�����0;����ʱ���ش�����.
	 */
	VAG_CLIENT_DECLARE int CALLBACK VAG_GetLastError();

    /**	@fn	int VAG_SubscribeGps(int iClientHandle, VAG_SUBSCRIBE_GPS_PARA* pstSubscribeGpsPara)
    *	@brief	<����GPS��Ϣ>
    *	@param <iClientHandle (IN)VAG_Login���صľ��>
    *	@param <pstSubscribeEventPara (IN)GPS���Ĳ���>
    *	@return	����ʱ����-1;�ɹ���������,��ʾ�Խ����.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_SubscribeGps(int iClientHandle, VAG_SUBSCRIBE_GPS_PARA* pstSubscribeGpsPara);

    /**	@fn	int VAG_DeSubscribeGps(int iClientHandle, VAG_DESUBSCRIBE_GPS_PARA* pstDeSubscribeGPSPara)
    *	@brief	<ȡ������GPS>
    *	@param <iClientHandle (IN)VAG_Login���صľ��>
    *	@param <pstDeSubscribeEventPara (IN)ȡ��GPS���Ĳ���>
    *	@return	�ɹ�����0;����ʱ����-1.
    */
    VAG_CLIENT_DECLARE int CALLBACK VAG_DeSubscribeGps(int iClientHandle, VAG_DESUBSCRIBE_GPS_PARA* pstDeSubscribeGPSPara);

}
#endif
