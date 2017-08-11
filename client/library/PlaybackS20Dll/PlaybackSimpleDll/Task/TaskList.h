/**   @file TaskList.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 任务管理类.
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

class CTaskList
{
public:
	CTaskList(void);
	~CTaskList(void);

private:
	// 任务列表.
	CArray<CTaskBase*, CTaskBase*> m_ArrayTask;
	// 任务锁.
	CRITICAL_SECTION m_csTaskLock;

public:
	// 添加任务.
	void AddTask(CTaskBase* pTask);
	// 获取任务.
	BOOL GetTask(CTaskBase*& pTask);
	// 清空任务.
	void Clear();
};
