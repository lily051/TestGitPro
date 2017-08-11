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
#include "hcs_object_api.h"

class CPBObjectCloudStream : public CPlayStream
{
public:
	CPBObjectCloudStream(void);
	~CPBObjectCloudStream(void);

private:
	 //云存储初始化句柄 
	HCSClient* m_hHCSClient; 
	// Bucket
	CString m_strBucket;
	// ObjectKey
	CString m_strObjectKey;
	//开始时间
	time_t m_iStartTime;
	//结束时间
	time_t m_iStopTime;
public:
	volatile BOOL m_bStopDataCb;   //停流
	//取流线程句柄
	HANDLE m_hGetObjectCloudStreamThread;

private:
	// 开始取流.
	BOOL StartData(HCSClient* pClient, const char* pBucket, const char* pObjectKey, const time_t iStartTime, const time_t iStopTime);
	// 停止取流.
	BOOL StopData();
	//取流回调函数
	static int ObjectCloudStreamDataCb(const char* buff, int bufflen, long long offset, int error, void* p_user);

	static DWORD WINAPI ThreadFuncGetObjectCloudStream(LPVOID pParam);
public:
	// 初始化对象云SDK.
	static BOOL InitObjectCloudSdk();

	// 释放对象云SDK.
	static void ReleaseObjectCloudSdk();

	// 登陆设备.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// 登出设备.
	void Logout();

	// 查找文件.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// 获取视频制式.
	//BOOL GetVideoFormat( int& nVideoFormat);

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

	//取流
	void GetObjectCloudStreamData();

};