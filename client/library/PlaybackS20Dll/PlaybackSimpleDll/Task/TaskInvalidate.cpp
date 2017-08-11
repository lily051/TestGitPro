//lint -library
#include "StdAfx.h"
#include "TaskInvalidate.h"

CTaskInvalidate::CTaskInvalidate(void)
{
}

CTaskInvalidate::~CTaskInvalidate(void)
{
}

void CTaskInvalidate::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB)
	{
		return;
	}

	pfnTaskMsgCB(MSG_INVALIDATE_WINDOW, 0, 0, pUserData);
}
