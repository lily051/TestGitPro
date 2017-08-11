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
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
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
	// ֹͣ�ط�.
	BOOL bRet = PB_StopPlayback((HANDLE)pData->pObject);

	// ����ֹͣ�ط���Ϣ.
	pfnTaskMsgCB(MSG_PLAYBACK_STOPPLAY, (WPARAM)m_stDataStop.emBwControl, (LPARAM)m_stDataStop.bAutoStop, pUserData);
}
