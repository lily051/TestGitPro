/**   @file HikStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 海康设备管理类
 *
 *    @author   zhoupeiqiang
 *    @date     2012/11/13
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "./PlayStream.h"
#include "HCNetSDK.h"
#include "StreamClient.h"

class CPBKmsStream : public CPlayStream
{
public:
	CPBKmsStream(void);
	~CPBKmsStream(void);

protected:
	// 登录句柄.
	LONG m_lLogin; 
	// 回放句柄.
	LONG m_lPlayHandle;  
	// 回放类型.
	ENUM_PLAY_TYPE m_emPlayType;

	BOOL m_bStreamPause;			 // 是否是流停止状态.

protected:
	// 停止取流.
	virtual BOOL StopData();
	// 文件类型转换.
	virtual int ChangeFileType(int nFileType);


public:
	// 初始化海康SDK.
	static BOOL InitKmsSdk();

	// 释放海康SDK.
	static void ReleaseHikSdk();

	// 登陆设备.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// 登出设备.
	void Logout();

	// 查找文件.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// 按时间回放.
	virtual BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo);

	// 按文件回放.
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo);

	// 停止回放.
	virtual void StopPlayback();

	// 下载文件.
	virtual BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam);

	// 按时间下载.
	virtual BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam);

	// 停止下载.
	void StopDownload();

	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

};

extern int CALLBACK pKmsStream(int sessionhandle, void* userdata, int datatype, void* pdata, int ilen);
extern int CALLBACK pKmsMsg( int sessionhandle, void* userdata, int opt, void* param1, void* param2, void* param3, void* param4 );