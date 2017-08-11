/**   @file TaskPlaybackByTime.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 按时间进行回放.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/17
 *
 *    @note  
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskBase.h"

// 按时间回放参数.
typedef struct _TaskDataByTime
{
	HANDLE hDlg;						// 对话框句柄.
	int nCamID;							// 监控点ID.
	DWORD dwRev;						// 保留字段.
	PB_LOGININFO stLoginInfo;			// 登录信息.
	PLAYBACK_TIME_INFO stPlaybackParam; // 按时间回放参数.
	BOOL bReversePlay;					// 是否是倒放模式.
}TaskDataByTime, *LPTaskDataByTime;


class CTaskPlaybackByTime : public CTaskBase
{
public:
	CTaskPlaybackByTime(const TaskDataByTime& rDataByTime);
	~CTaskPlaybackByTime(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// 回放.
	HANDLE Playback();

private:
	// 按时间回放参数.
	TaskDataByTime m_stByTime;
};
