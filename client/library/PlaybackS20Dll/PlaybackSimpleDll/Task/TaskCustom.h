/**   @file TaskCustom.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief �����̹߳���.
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

#include "TaskList.h"

class CTaskCustom
{
public:
	CTaskCustom(void);
	~CTaskCustom(void);

protected:
	// �����б�.
	CTaskList m_listTask;
	// ��������¼�.
	HANDLE m_hTaskEvent;
	// �߳̾��.
	HANDLE m_hTaskThread;
	// ������Ϣ�ص�����.
	TaskMsgCB m_pfnTaskMsgCB;
	// �û��Զ�����Ϣ.
	LPVOID m_pUserData;
	// �Ƿ��ʼ���ɹ�.
	BOOL m_bIsInit;

private:
	// �����߳�.
	static DWORD WINAPI ThreadFuncTask(LPVOID pParam);

public:
	// ��ʼ��.
	BOOL InitTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData);
	// �ͷ���Դ.
	void UnInitTask();
	// �������.
	void AddTask(CTaskBase* pTask);
	// �������.
	void ClearTask();

public:
	// ��ȡ����̬����.
	virtual void GetTaskDynData(TaskDynData& stData);
};
