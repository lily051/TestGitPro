/**   @file TaskStopPlayback.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ֹͣ�ط�.
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

typedef struct _TaskDataStop
{
	HANDLE hDlg;					// �Ի�����.
	int nCamID;						// ��ص�ID.
	DWORD dwRev;					// �����ֶ�.
	BOOL bAutoStop;					// �Ƿ����Զ�ֹͣ.
	BWControlType emBwControl;		// �����������.
}TaskDataStop, *LPTaskDataStop;

class CTaskStopPlayback : public CTaskBase
{
public:
	CTaskStopPlayback(const TaskDataStop& stDataStop);
	~CTaskStopPlayback(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:

private:
	// ֹͣ�ط��������.
    TaskDataStop m_stDataStop;
};
