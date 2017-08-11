#pragma once

#include "TaskBase.h"

typedef struct _TaskDataRevSwitch
{
	int nCamID;			// 监控点ID.
	PB_TIME stStartTime;	// 回放时间.
	PB_TIME stStopTime;		// 停止时间.
	BOOL bReversePlay;		// 是否是倒放.
}TaskDataRevSwitch, *LPTaskDataRevSwitch;

class CTaskReversePlaySwitch : public CTaskBase
{
public:
	CTaskReversePlaySwitch(const TaskDataRevSwitch& stData);
	~CTaskReversePlaySwitch(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// 设置回放进度结构体.
	TaskDataRevSwitch m_stDataRevPlay;
};
