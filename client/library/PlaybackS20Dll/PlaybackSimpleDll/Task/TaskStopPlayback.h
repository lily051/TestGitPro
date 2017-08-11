/**   @file TaskStopPlayback.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 停止回放.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/18
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

typedef struct _TaskDataStop
{
	HANDLE hDlg;					// 对话框句柄.
	int nCamID;						// 监控点ID.
	DWORD dwRev;					// 保留字段.
	BOOL bAutoStop;					// 是否是自动停止.
	BWControlType emBwControl;		// 带宽控制类型.
}TaskDataStop, *LPTaskDataStop;

class CTaskStopPlayback : public CTaskBase
{
public:
	CTaskStopPlayback(const TaskDataStop& stDataStop);
	~CTaskStopPlayback(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:

private:
	// 停止回放任务参数.
    TaskDataStop m_stDataStop;
};
