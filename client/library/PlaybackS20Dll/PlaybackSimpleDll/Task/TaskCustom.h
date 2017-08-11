/**   @file TaskCustom.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 任务线程管理.
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

#include "TaskList.h"

class CTaskCustom
{
public:
	CTaskCustom(void);
	~CTaskCustom(void);

protected:
	// 任务列表.
	CTaskList m_listTask;
	// 任务结束事件.
	HANDLE m_hTaskEvent;
	// 线程句柄.
	HANDLE m_hTaskThread;
	// 任务消息回调函数.
	TaskMsgCB m_pfnTaskMsgCB;
	// 用户自定义消息.
	LPVOID m_pUserData;
	// 是否初始化成功.
	BOOL m_bIsInit;

private:
	// 任务线程.
	static DWORD WINAPI ThreadFuncTask(LPVOID pParam);

public:
	// 初始化.
	BOOL InitTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData);
	// 释放资源.
	void UnInitTask();
	// 添加任务.
	void AddTask(CTaskBase* pTask);
	// 清空任务.
	void ClearTask();

public:
	// 获取任务动态参数.
	virtual void GetTaskDynData(TaskDynData& stData);
};
