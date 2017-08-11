/**   @file TaskBwReConnect.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 带宽重连完成后任务.
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

typedef struct _TaskDataBwReConnect
{
	int nCamID;			// 监控点ID.
}TaskDataBwReconnect;

class CTaskBwReConnect : public CTaskBase
{
public:
	CTaskBwReConnect(const TaskDataBwReconnect& stDataBw);
	~CTaskBwReConnect(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// 带宽重连后任务结构体.
	TaskDataBwReconnect m_stDataBw;
};
