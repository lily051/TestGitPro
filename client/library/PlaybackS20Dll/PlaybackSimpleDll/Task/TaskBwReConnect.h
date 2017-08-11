/**   @file TaskBwReConnect.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ����������ɺ�����.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/22
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

typedef struct _TaskDataBwReConnect
{
	int nCamID;			// ��ص�ID.
}TaskDataBwReconnect;

class CTaskBwReConnect : public CTaskBase
{
public:
	CTaskBwReConnect(const TaskDataBwReconnect& stDataBw);
	~CTaskBwReConnect(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// ��������������ṹ��.
	TaskDataBwReconnect m_stDataBw;
};
