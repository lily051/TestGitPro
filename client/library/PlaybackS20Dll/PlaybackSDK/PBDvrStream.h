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

class CPBDvrStream : public CPlayStream
{
public:
	CPBDvrStream(void);
	~CPBDvrStream(void);

protected:
	// 登录句柄.
	LONG m_lLogin; 
	// 回放句柄.
	LONG m_lPlayHandle;  
	// 回放类型.
	ENUM_PLAY_TYPE m_emPlayType;

protected:
	// 开始取流.  MOD 20140402 扩展是否转码回放
	virtual BOOL StartData(LONG lLogin, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime, BYTE byResolution = 0, DWORD dwVideoBitrate = 0, DWORD dwVideoFrameRate = 0);
	// 停止取流.
	virtual BOOL StopData();
	// 文件类型转换.
	virtual int ChangeFileType(int nFileType);


public:
	// 初始化海康SDK.
	static BOOL InitDvrSdk();

	// 释放海康SDK.
	static void ReleaseHikSdk();

	// 登陆设备.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// 登出设备.
	void Logout();

	// 查找文件.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

    // 按事件查找文件.
    BOOL FindFileByEvent(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// 智能检索.
	BOOL SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// 获取视频制式.
	BOOL GetVideoFormat( int& nVideoFormat);

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

	// 是否支持按卡号查询.
	virtual BOOL IsCardNumAllowed();

    // 是否支持按事件查询.
    virtual BOOL IsEventAllowed();
   
	//获取压缩参数能力集.
	BOOL GetCompressCfgAbility( char* pOutbuf );
};

extern void CALLBACK pDvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser);