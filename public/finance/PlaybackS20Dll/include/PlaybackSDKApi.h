#pragma once
/**	@file    PlaybackSDKApi.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   dll接口函数定义
*
*	@author	 yudan
*	@date	 2012/06/04
*	@note   
*	@note    历史记录：
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


//#pragma region 错误码
#define PB_NOERROR								0				// 无错误.
#define PB_UNSUPPORT_REVPLAY_TYPE				1				// 该回放方式或设备不支持倒放.
#define PB_UNSUPPORT_OPERATION					2				// 该操作不支持.
#define PB_UNSUPPORT_CARDNUM					3				// 该回放方式或设备不支持按卡号查询.
#define PB_CREATEFILE_FILE                      4               // 创建本地文件失败
#define PB_LOGIN_FAILD                          5               // 登录设备失败
#define PB_SEND_TO_DEV_FAILD                    6               // 与设备通讯失败
#define PB_UNSUPPORT_SMARTSEARCH				7				// 该回放方式或设备不支持智能检索.

//#pragma endregion


/** @fn PB_InitSDK
*   @brief 初始化
*   @param[in] emPlayType: 回放方式
*   @param[in] emDeviceType: 设备类型
*   @param[in] pUserData: 用户数据
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_InitSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType = PB_TYPE_DVR, void * pUserData = NULL);

/** @fn PB_ReleaseSDK
*   @brief 释放sdk
*   @param NULL
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API void PB_ReleaseSDK(ENUM_DEVICE_TYPE emDeviceType, ENUM_PLAY_TYPE emPlayType = PB_TYPE_DVR);

/** @fn PB_PlaybackTime
*   @brief 开始时间回放
*   @param[in] stLoginInfo: 设备登录信息
*   @param[in] stPlaybackParam: 回放参数
*   @return 回放句柄
*/
PLAYBACKSDK_API HANDLE PB_PlaybackTime(PB_LOGININFO& stLoginInfo, PLAYBACK_TIME_INFO& stPlaybackParam);

/** @fn PB_PlaybackFile
*   @brief 开始文件回放
*   @param[in] stLoginInfo: 设备登录信息
			   stPlaybackParam: 文件回放参数
*   @param NULL
*   @return NULL
*/
PLAYBACKSDK_API HANDLE PB_PlaybackFile(PB_LOGININFO& stLoginInfo, PLAYBACK_FILE_INFO& stPlaybackParam);

/** @fn PB_LocatePlayTime
*   @brief 定位回放绝对时间
*   @param[in]  hPlayback:  回放句柄
*	@param[in]	stStartTime: 定位时间
*   @parma[in]  stStopTime:停止时间.
*   @param[out] NULL
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_LocatePlayTime(HANDLE hPlayback, const PB_TIME& stStartTime, const PB_TIME& stStopTime);

/** @fn PB_StopPlayback
*   @brief 停止回放
*   @param[in] hPlayback: 回放句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
*/
PLAYBACKSDK_API BOOL PB_StopPlayback(HANDLE& hPlayback);

/** @fn PB_FindFile
*   @brief 查找文件
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] stLoginInfo: 登录信息
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_FindFile(PB_LOGININFO& stLoginInfo, FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO & lpFileList, int & nFileCount);

/** @fn PB_SmartSearch
*   @brief 智能搜索
*   @param[in] stFindFileParam: 查找文件参数
*   @param[in] stLoginInfo: 登录信息
*   @param[in] lpFileList: 文件链表
*   @param[in] nFileCount: 文件数量
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_SmartSearch(PB_LOGININFO& stLoginInfo, SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO & lpFileList, int & nFileCount);

/** @fn PB_PlaybackControl
*   @brief 回放控制
*   @param[in] hPlayback: 回放句柄
*   @param[in] emControlType: 控制类型
*   @param[in] lValueIn: 输入参数
*   @param[out] lValueOut: 输出参数
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_PlaybackControl(HANDLE hPlayback, ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

/** @fn PB_GetOsdTime
*   @brief 获取OSD时间
*   @param[in] hPlayback: 回放句柄
*   @param[out] lOsdTime: OSD时间
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetOsdTime(HANDLE hPlayback, LONG & lOsdTime);

/** @fn PB_GetPlayPos
*   @brief 获取回放进度
*   @param[in] hPlayback: 回放句柄
*   @param[out] lPos: 进度
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetPlayPos(HANDLE hPlayback, LONG & lPos);

/** @fn PB_Capture
*   @brief 抓图
*   @param[in] hPlayback: 回放句柄
*   @param[in] szPicFile: 文件存储地址
*   @param[in] emPicType: 抓图类型
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_Capture(HANDLE hPlayback, char * szPicFile, ENUM_PIC_TYPE emPicType);

/** @fn PB_SaveData
*   @brief 录像剪辑
*   @param[in] hPlayback: 回放句柄
*   @param[in] szFile: 文件存储地址
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_SaveData(HANDLE hPlayback, char * szPicFile);

/** @fn PB_StopSave
*   @brief 停止剪辑
*   @param[in] hPlayback: 回放句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
*/
PLAYBACKSDK_API BOOL PB_StopSave(HANDLE hPlayback);

/** @fn PB_DownloadFile
*   @brief 文件下载
*   @param[in] stLoginInfo: 登录信息
*   @param[in] stDownloadParam: 下载参数
*   @return 下载句柄，NULL表示下载失败
*/
PLAYBACKSDK_API HANDLE PB_DownloadFile(PB_LOGININFO& stLoginInfo, DOWNLOAD_FILE_INFO& stDownloadParam);

/** @fn PB_DownloadTime
*   @brief 时间下载
*   @param[in] stLoginInfo: 登录信息
*   @param[in] stDownloadParam: 下载信息
*   @return 下载句柄，NULL表示下载失败
*/
PLAYBACKSDK_API HANDLE PB_DownloadTime(PB_LOGININFO& stLoginInfo, DOWNLOAD_TIME_INFO& stDownloadParam);

/** @fn PB_StopDownload
*   @brief 停止下载
*   @param[in] hDownload: 下载句柄
*   @param NULL
*   @return TRUE:成功,FALSE:失败.
*/
PLAYBACKSDK_API BOOL PB_StopDownload(HANDLE& hDownload);

/** @fn PB_GetDownloadPos
*   @brief 获取下载进度
*   @param[in] hDownload: 下载句柄
*   @param[out] nPos: 进度
*   @return TRUE成功 FALSE失败
*/
PLAYBACKSDK_API BOOL PB_GetDownloadPos(HANDLE hDownload, int & nPos);

/**   @fn          PB_GetLastError
 *    @brief   	   获取错误码.
 *    @param[in]   
 *    @param[in]   
 *    @return      错误码.
 */
PLAYBACKSDK_API DWORD PB_GetLastError();

/**   @fn          PB_StartGetStreamData
 *    @brief   	   获取回放流数据.
 *    @param[in]   hPlayback:回放句柄.
 *    @param[in]   pStreamData:取流回调函数.
 *    @param[in]   dwUser:用户自定义数据.
 *    @return      TRUE-成功,FALSE-失败.
 */
PLAYBACKSDK_API BOOL PB_StartGetStreamData(HANDLE hPlayback, PbStreamDataCB pStreamData, DWORD dwUser);

/**   @fn          PB_StopGetStreamData
 *    @brief   	   停止获取回放流数据.
 *    @param[in]   hPlayback:回放句柄.
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_StopGetStreamData(HANDLE hPlayback);

/**   @fn          PB_GetDeviceAbility
 *    @brief   	   获取dvr压缩参数能力集.
 *    @param[in]   stLoginInfo: 登录信息
 *    @param[in]   lChannel: 通道
 *    @param[out]  stCompresscfgAbility: 压缩参数能力集结构体
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutbuf);

/**   @fn          PB_GetDvrConfig
 *    @brief   	   获取dvr视频参数，此处主要为了获取视频制式.
 *    @param[in]   stLoginInfo: 登录信息
 *    @param[in]   lChannel: 通道
 *    @param[out]  nVideoFormat: 视频制式：1-NTSC，2-PAL 
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetDvrConfig(PB_LOGININFO& stLoginInfo, LONG lChannel, int & nVideoFormat);

/**   @fn          PB_GetPictureSize
 *    @brief   	   获取解码图像大小.
 *    @param[in]   hPlayback: 回放句柄
 *    @param[out]   lWidth: 图像宽度
 *    @param[out]   lHeight: 图像高度
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetPictureSize(HANDLE hPlayback, LONG* lWidth, LONG* lHeight);

/**   @fn          PB_GetKeyFrameData
 *    @brief   	   获取视频关键帧数据.
 *    @param[in]   hPlayback: 回放句柄
 *    @param[in]   stFrameTime: 关键帧时间
 *    @param[in]   pFunGetKeyFrame: 数据回调函数
 *    @param[in]   dwUser: 用户数据
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_GetKeyFrameData( HANDLE hPlayback,const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser );

/**   @fn          PB_StopGetKeyFrameData
 *    @brief   	   获取解码图像大小.
 *    @param[in]   hPlayback: 回放句柄
 *    @return     
 */
PLAYBACKSDK_API BOOL PB_StopGetKeyFrameData( HANDLE hPlayback );
};

#ifdef __cplusplus
#endif

#endif
