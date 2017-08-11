/**   @file TaskList.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ���������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/18
 *
 *    @note  
 *    @note 
 *    @note ��ʷ��¼��
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
	// �����б�.
	CArray<CTaskBase*, CTaskBase*> m_ArrayTask;
	// ������.
	CRITICAL_SECTION m_csTaskLock;

public:
	// �������.
	void AddTask(CTaskBase* pTask);
	// ��ȡ����.
	BOOL GetTask(CTaskBase*& pTask);
	// �������.
	void Clear();
};
