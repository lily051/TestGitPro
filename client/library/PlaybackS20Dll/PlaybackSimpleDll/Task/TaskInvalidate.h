#pragma once

#include "TaskBase.h"

class CTaskInvalidate : public CTaskBase
{
public:
	CTaskInvalidate(void);
	~CTaskInvalidate(void);

public:
	// Ö´ÐÐÈÎÎñ.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);
};
