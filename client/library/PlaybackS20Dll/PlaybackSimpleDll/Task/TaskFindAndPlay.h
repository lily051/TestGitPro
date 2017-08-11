/**   @file		TaskFindAndPlay.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	查找并回放录像文件.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/05/10
 *
 *    @note  
 *    @note 
 *    @note		历史记录：
 *    @note		V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskBase.h"

// 按查找并回放参数.
typedef struct _TaskDataFindAndPlay
{
	HANDLE hDlg;						// 对话框句柄.
	int nCamID;							// 监控点ID.
	PB_LOGININFO stLoginInfo;			// 登录信息.
	PLAYBACK_TIME_INFO stPlaybackParam; // 按时间回放参数.
	DWORD dwRev;						// 保留参数.
	BOOL bPlayTime;						// 是否是按时间回放.
	FINDFILE_PARAM stFindFileParam;		// 查找参数.
	LPB_FILEINFO pFileList;			// 文件列表指针.
	int nFileCount;					// 文件个数指针.
	BOOL bReversePlay;					// 是否是倒放模式.
}TaskDataFindAndPlay, *LPTaskDataFindAndPlay;

class CTaskFindAndPlay : public CTaskBase
{
public:
	CTaskFindAndPlay(const TaskDataFindAndPlay& stData);
	~CTaskFindAndPlay(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

protected:
	// 查找并回放参数.
	TaskDataFindAndPlay m_stData;

protected:
	// 查找文件.
	BOOL FindFile(int& nFileCount);
	BOOL FindFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, PB_FILEINFO& stFileInfo);
	// 按时间回放.
	void PlaybackByTime(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);
	// 按文件回放.
	void PlaybackbyFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, const PB_FILEINFO& stFileInfo);
};
