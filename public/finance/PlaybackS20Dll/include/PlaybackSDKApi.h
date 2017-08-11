#pragma once
/**	@file    PlaybackSDKApi.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   dll�ӿں�������
*
*	@author	 yudan
*	@date	 2012/06/04
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/06/04 by yudan
*/
#include "./PlaybackSdkDef.h"
using namespace PlaybackSdk;


#ifndef __PLAYBACKSDK_API__
#define __PLAYBACKSDK_API__

#define PLAYBACKSDK_API_EXPORT

#ifdef PLAYBACKSDK_API_EXPORT
#define PLAYBACKSDK_API __declspec(dllexport)
#else
#define PLAYBACKSDK_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif


//#pragma region ������
#define PB_NOERROR								0				// �޴���.
#define PB_UNSUPPORT_REVPLAY_TYPE				1				// �ûطŷ�ʽ���豸��֧�ֵ���.
#define PB_UNSUPPORT_OPERATION					2				// �ò�����֧��.
#define PB_UNSUPPORT_CARDNUM					3				// �ûطŷ�ʽ���豸��֧�ְ����Ų�ѯ.
#define PB_CREATEFILE_FILE                      4               // ���������ļ�ʧ��
#define PB_LOGIN_FAILD                          5               // ��¼�豸ʧ��
#define PB_SEND_TO_DEV_FAILD                    6               // ���豸ͨѶʧ��
#define PB_UNSUPPORT_SMARTSEARCH				7				// �ûطŷ�ʽ���豸��֧�����ܼ���.

//#pragma endregion


/** @fn PB_InitSDK
*   @brief ��ʼ��
*   @param[in] emPlayType: �طŷ�ʽ
*   @param[in] emDeviceType: �豸����
*   @param[in] pUserData: �û�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_InitSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType = PB_TYPE_DVR, void * pUserData = NULL);

/** @fn PB_ReleaseSDK
*   @brief �ͷ�sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API void PB_ReleaseSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType = PB_TYPE_DVR);

/** @fn PB_PlaybackTime
*   @brief ��ʼʱ��ط�
*   @param[in] stLoginInfo: �豸��¼��Ϣ
*   @param[in] stPlaybackParam: �طŲ���
*   @return �طž��
*/
PLAYBACKSDK_API HANDLE PB_PlaybackTime(PB_LOGININFO& stLoginInfo, PLAYBACK_TIME_INFO& stPlaybackParam);

/** @fn PB_PlaybackFile
*   @brief ��ʼ�ļ��ط�
*   @param[in] stLoginInfo: �豸��¼��Ϣ
			   stPlaybackParam: �ļ��طŲ���
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API HANDLE PB_PlaybackFile(PB_LOGININFO& stLoginInfo, PLAYBACK_FILE_INFO& stPlaybackParam);

/** @fn PB_LocatePlayTime
*   @brief ��λ�طž���ʱ��
*   @param[in]  hPlayback:  �طž��
*	@param[in]	stStartTime: ��λʱ��
*   @parma[in]  stStopTime:ֹͣʱ��.
*   @param[out] NULL
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_LocatePlayTime(HANDLE hPlayback, const PB_TIME& stStartTime, const PB_TIME& stStopTime);

/** @fn PB_StopPlayback
*   @brief ֹͣ�ط�
*   @param[in] hPlayback: �طž��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopPlayback(HANDLE& hPlayback);

/** @fn PB_FindFile
*   @brief �����ļ�
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_FindFile(PB_LOGININFO& stLoginInfo, FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO & lpFileList, int & nFileCount);

/** @fn PB_SmartSearch
*   @brief ��������
*   @param[in] stFindFileParam: �����ļ�����
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] lpFileList: �ļ�����
*   @param[in] nFileCount: �ļ�����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_SmartSearch(PB_LOGININFO& stLoginInfo, SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO & lpFileList, int & nFileCount);

/** @fn PB_PlaybackControl
*   @brief �طſ���
*   @param[in] hPlayback: �طž��
*   @param[in] emControlType: ��������
*   @param[in] lValueIn: �������
*   @param[out] lValueOut: �������
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_PlaybackControl(HANDLE hPlayback, ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

/** @fn PB_GetOsdTime
*   @brief ��ȡOSDʱ��
*   @param[in] hPlayback: �طž��
*   @param[out] lOsdTime: OSDʱ��
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetOsdTime(HANDLE hPlayback, LONG & lOsdTime);

/** @fn PB_GetPlayPos
*   @brief ��ȡ�طŽ���
*   @param[in] hPlayback: �طž��
*   @param[out] lPos: ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetPlayPos(HANDLE hPlayback, LONG & lPos);

/** @fn PB_Capture
*   @brief ץͼ
*   @param[in] hPlayback: �طž��
*   @param[in] szPicFile: �ļ��洢��ַ
*   @param[in] emPicType: ץͼ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_Capture(HANDLE hPlayback, char * szPicFile, ENUM_PIC_TYPE emPicType);

/** @fn PB_SaveData
*   @brief ¼�����
*   @param[in] hPlayback: �طž��
*   @param[in] szFile: �ļ��洢��ַ
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_SaveData(HANDLE hPlayback, char * szPicFile);

/** @fn PB_StopSave
*   @brief ֹͣ����
*   @param[in] hPlayback: �طž��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopSave(HANDLE hPlayback);

/** @fn PB_DownloadFile
*   @brief �ļ�����
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] stDownloadParam: ���ز���
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
PLAYBACKSDK_API HANDLE PB_DownloadFile(PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam);

/** @fn PB_DownloadTime
*   @brief ʱ������
*   @param[in] stLoginInfo: ��¼��Ϣ
*   @param[in] stDownloadParam: ������Ϣ
*   @return ���ؾ����NULL��ʾ����ʧ��
*/
PLAYBACKSDK_API HANDLE PB_DownloadTime(PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam);

/** @fn PB_StopDownload
*   @brief ֹͣ����
*   @param[in] hDownload: ���ؾ��
*   @param NULL
*   @return TRUE:�ɹ�,FALSE:ʧ��.
*/
PLAYBACKSDK_API BOOL PB_StopDownload(HANDLE& hDownload);

/** @fn PB_GetDownloadPos
*   @brief ��ȡ���ؽ���
*   @param[in] hDownload: ���ؾ��
*   @param[out] nPos: ����
*   @return TRUE�ɹ� FALSEʧ��
*/
PLAYBACKSDK_API BOOL PB_GetDownloadPos(HANDLE hDownload, int & nPos);

/**   @fn          PB_GetLastError
 *    @brief   	   ��ȡ������.
 *    @param[in]   
 *    @param[in]   
 *    @return      ������.
 */
PLAYBACKSDK_API DWORD PB_GetLastError();

/**   @fn          PB_StartGetStreamData
 *    @brief   	   ��ȡ�ط�������.
 *    @param[in]   hPlayback:�طž��.
 *    @param[in]   pStreamData:ȡ���ص�����.
 *    @param[in]   dwUser:�û��Զ�������.
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PLAYBACKSDK_API BOOL PB_StartGetStreamData(HANDLE hPlayback, PbStreamDataCB pStreamData, DWORD dwUser);

/**   @fn          PB_StopGetStreamData
 *    @brief   	   ֹͣ��ȡ�ط�������.
 *    @param[in]   hPlayback:�طž��.
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_StopGetStreamData(HANDLE hPlayback);

/**   @fn          PB_GetDeviceAbility
 *    @brief   	   ��ȡdvrѹ������������.
 *    @param[in]   stLoginInfo: ��¼��Ϣ
 *    @param[in]   lChannel: ͨ��
 *    @param[out]  stCompresscfgAbility: ѹ�������������ṹ��
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutbuf);

/**   @fn          PB_GetDvrConfig
 *    @brief   	   ��ȡdvr��Ƶ�������˴���ҪΪ�˻�ȡ��Ƶ��ʽ.
 *    @param[in]   stLoginInfo: ��¼��Ϣ
 *    @param[in]   lChannel: ͨ��
 *    @param[out]  nVideoFormat: ��Ƶ��ʽ��1-NTSC��2-PAL 
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetDvrConfig(PB_LOGININFO& stLoginInfo, LONG lChannel, int & nVideoFormat);

/**   @fn          PB_GetPictureSize
 *    @brief   	   ��ȡ����ͼ���С.
 *    @param[in]   hPlayback: �طž��
 *    @param[out]   lWidth: ͼ����
 *    @param[out]   lHeight: ͼ��߶�
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetPictureSize(HANDLE hPlayback, LONG* lWidth, LONG* lHeight);

/**   @fn          PB_GetKeyFrameData
 *    @brief   	   ��ȡ��Ƶ�ؼ�֡����.
 *    @param[in]   hPlayback: �طž��
 *    @param[in]   stFrameTime: �ؼ�֡ʱ��
 *    @param[in]   pFunGetKeyFrame: ���ݻص�����
 *    @param[in]   dwUser: �û�����
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetKeyFrameData( HANDLE hPlayback,const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser );

/**   @fn          PB_StopGetKeyFrameData
 *    @brief   	   ��ȡ����ͼ���С.
 *    @param[in]   hPlayback: �طž��
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_StopGetKeyFrameData( HANDLE hPlayback );
};

#ifdef __cplusplus
#endif

#endif
