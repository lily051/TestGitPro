/**   @file APICloudStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief APICloudStream管理类
 *
 *    @author   wangzhiyong
 *    @date     2013/12/05
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "PlayStream.h"
#include "hcs_sdk.h"


class CPBCloudStream : public CPlayStream
{
public:
	CPBCloudStream();
	~CPBCloudStream(void);

private:
	// 登录句柄.
	unsigned long long m_nLogin; 
	// 播放句柄.
	int m_nPlayHandle;
	// 是否已经登录成功.
	BOOL m_bIsLogin;
	// 录像文件列表.
	PB_FILEINFO m_stFileInfo[MAX_FILE_NUM];
	// 录像文件个数.
	int m_nFileNum;
	// 编码器IP.
	CString m_strDecoderIP;
	// 是否是头数据.
	BOOL m_bIsHeadData;
	//取流线程
	HANDLE m_hCloudPlayThread;
    HANDLE m_hCloudDownloadThread;
	//头数据
	char m_szHeadBuf[4*1024];
	//查找文件结束
	BOOL m_bFinishFindFile;

	HANDLE m_hStopPlayEvent;

	HANDLE m_hStopCheckSearch;

	int m_nDownloadHandle;

public:
    HANDLE m_hStopDownloadEvent;
private:
	// 错误信息回调函数.
	static int UserMessage(int LogicType,int ErrorCode,void* pUser);
	// 查询录像结果.
	//static int GetResult(CloudRecordInfo info, int Count, int Flat, void* pUser);

	static void FunDownRecordDataCb(int handle, const char* buff ,int bufflen, int Flat, int error_code, void* pUser);

	// 检测搜索录像线程.
	static DWORD WINAPI ThreadFuncCheckSearch(LPVOID pParam);

	// 开始取流.
	BOOL StartData(unsigned long long uLogin, const char* pCamID, const PB_TIME &stStartTime, const PB_TIME &stStopTime);
	// 停止取流.
	BOOL StopData();

public:
	// 初始化SDK.
	static BOOL InitCloudSdk();

	// 释放SDK.
	static void ReleaseSdk();

	// 登陆设备.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// 登出设备.
	void Logout();

	// 查找文件.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// 按时间回放.
	BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo);

	// 按文件回放.
	BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo);

	// 停止回放.
	void StopPlayback();

	// 下载文件.
	BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam);

	// 按时间下载.
	BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam);

	// 停止下载.
	void StopDownload();

	//播放
	void CloudPlay();

    void ThreadStopDownLoad();

	//检查搜索是否结束
	void StartCheckSearchFinish();

	UINT GetPlayHandle() const { return m_nPlayHandle; }

	BOOL GetFinishFindFile() const { return m_bFinishFindFile; }

	void SetFinishFindFile(BOOL val) { m_bFinishFindFile = val; }

	void StopCheckSearchFinish();

	void SetDownloadPos(int nPos) {m_nDownloadPos = nPos;}
};
