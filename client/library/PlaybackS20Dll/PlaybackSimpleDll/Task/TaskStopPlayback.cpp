//lint -library
#include "StdAfx.h"
#include "TaskStopPlayback.h"
#include "../PlaybackSimpleDll.h"

CTaskStopPlayback::CTaskStopPlayback(const TaskDataStop& stDataStop)
{
    ZeroMemory(&m_stDataStop, sizeof(TaskDataStop));
	memcpy_s(&m_stDataStop, sizeof(TaskDataStop), &stDataStop, sizeof(TaskDataStop));
}

CTaskStopPlayback::~CTaskStopPlayback(void)
{
}

/**   @fn          DoTask
 *    @brief   	   执行任务.
 *    @param[in]   pfnTaskMsgCB:消息回调函数.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
void CTaskStopPlayback::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB  ||  NULL == pData)
	{
		return;
	}
	if (NULL == pData->pObject)
	{
		return;
	}
	// 停止回放.
	BOOL bRet = PB_StopPlayback((HANDLE)pData->pObject);

	// 发送停止回放消息.
	pfnTaskMsgCB(MSG_PLAYBACK_STOPPLAY, (WPARAM)m_stDataStop.emBwControl, (LPARAM)m_stDataStop.bAutoStop, pUserData);
}
