/**   @file TaskLocatePlayTime.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 设置回放进度任务.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/22
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

typedef struct _TaskDataLocate
{
	int nCamID;			// 监控点ID.
	PB_TIME stStartTime;	// 回放时间.
	PB_TIME stStopTime;		// 停止时间.
	BOOL bReversePlay;		// 是否是倒放.
}TaskDataLocate, *LPTaskDataLocate;

class CTaskLocatePlayTime : public CTaskBase
{
public:
	CTaskLocatePlayTime(const TaskDataLocate& stDataLocate);
	~CTaskLocatePlayTime(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);


private:
	// 设置回放进度结构体.
	TaskDataLocate m_stDataLocate;
};
