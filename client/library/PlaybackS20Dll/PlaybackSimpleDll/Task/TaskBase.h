/**   @file TaskBase.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 任务基类,具体任务需要继承子类.
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


// 任务消息回调函数.
typedef void (CALLBACK *TaskMsgCB)(UINT dwMsgID, DWORD dwParam1, DWORD dwParam2, LPVOID dwUserData);

class CTaskBase
{
public:
	CTaskBase(void);
	virtual ~CTaskBase(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData) = 0;

protected:
};
