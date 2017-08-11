#pragma once

#include "TaskBase.h"

typedef struct _TaskDataRevSwitch
{
	int nCamID;			// ��ص�ID.
	PB_TIME stStartTime;	// �ط�ʱ��.
	PB_TIME stStopTime;		// ֹͣʱ��.
	BOOL bReversePlay;		// �Ƿ��ǵ���.
}TaskDataRevSwitch, *LPTaskDataRevSwitch;

class CTaskReversePlaySwitch : public CTaskBase
{
public:
	CTaskReversePlaySwitch(const TaskDataRevSwitch& stData);
	~CTaskReversePlaySwitch(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// ���ûطŽ��Ƚṹ��.
	TaskDataRevSwitch m_stDataRevPlay;
};
