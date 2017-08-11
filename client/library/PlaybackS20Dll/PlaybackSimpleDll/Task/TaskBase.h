/**   @file TaskBase.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief �������,����������Ҫ�̳�����.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/17
 *
 *    @note  
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once


// ������Ϣ�ص�����.
typedef void (CALLBACK *TaskMsgCB)(UINT dwMsgID, DWORD dwParam1, DWORD dwParam2, LPVOID dwUserData);

class CTaskBase
{
public:
	CTaskBase(void);
	virtual ~CTaskBase(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData) = 0;

protected:
};
