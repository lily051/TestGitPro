/**   @file PlaybackOper.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	回放操作类.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/10/30
 *
 *    @note		用于进行回放控制.
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskCustom.h"
#include "TaskPlaybackByTime.h"
#include "TaskPlaybackByFile.h"
#include "TaskStopPlayback.h"
#include "TaskLocatePlayTime.h"
#include "TaskInvalidate.h"
#include "TaskFindAndPlay.h"

class CPlaybackOper : public CTaskCustom
{
public:
	CPlaybackOper(void);
	~CPlaybackOper(void);

private:
	// 播放句柄.
	HANDLE m_hPlayHandle;

public:
	// 设置回放句柄.
	void SetPlayHandle(HANDLE hPlayHandle);
	// 获取任务动态参数.
	virtual void GetTaskDynData(TaskDynData& stData);

public:
	// 查找录像文件并回放.
	void FindAndPlayback(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindFileParam, 
		LPB_FILEINFO pFileList,
		BOOL bReversePlay,
		BOOL bPlayTime);
	// 按时间回放.
	void PlaybackTime(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		BOOL bReversePlay);
	// 按文件回放.
	void PlaybackFile(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		PB_LOGININFO& stLoginInfo, 
		PLAYBACK_FILE_INFO& stPlaybackParam,
		BOOL bReversePlay);
	// 停止回放.
	void StopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl);
	// 程序销毁时停止回放.
	void TaskStopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl);
	// 回放控制.
	BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);
	// 获取OSD时间.
	BOOL GetOsdTime(LONG & lOsdTime);
	// 获取回放进度.
	BOOL GetPlayPos(LONG & lPos);
	// 回放抓图.
	BOOL Capture(HANDLE hDlg, char * szPicFile, ENUM_PIC_TYPE emPicType, int nCamID, DWORD dwRev);
	// 录像剪辑.
	BOOL SaveData(HANDLE hDlg,  char * szSaveFile, int nCamID, DWORD dwRev);
	// 停止剪辑.
	void StopSave(HANDLE hDlg,  int nCamID, DWORD dwRev);
	// 本地时间回放.
	void LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nCamID, BOOL bReversePlay);
	// 获取错误码.
	DWORD GetSDKLastError();
	// 刷新窗口.
	void InvalidateWindow();
    // 获取dvr转码回放能力集
    BOOL GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutBuf );
};
