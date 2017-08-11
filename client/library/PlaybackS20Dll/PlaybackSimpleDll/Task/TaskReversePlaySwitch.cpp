#include "StdAfx.h"
#include "TaskReversePlaySwitch.h"

CTaskReversePlaySwitch::CTaskReversePlaySwitch(const TaskDataRevSwitch& stData)
{
	memcpy_s(&m_stDataRevPlay, sizeof(TaskDataRevSwitch), &stData, sizeof(TaskDataRevSwitch));
}

CTaskReversePlaySwitch::~CTaskReversePlaySwitch(void)
{
}

/**   @fn          DoTask
 *    @brief   	   执行任务.
 *    @param[in]   pfnTaskMsgCB:消息回调函数.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
void CTaskReversePlaySwitch::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB  ||  NULL == pData)
	{
		return;
	}

	BOOL bRet = FALSE;

	bRet = PB_ReversePlaySwitch((HANDLE)pData->pObject, m_stDataRevPlay.bReversePlay, m_stDataRevPlay.stStartTime, m_stDataRevPlay.stStopTime);

	if (bRet)
	{
		// 设置进度成功.
		//Sleep(1000);
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_SUCCESS, 0, 0, pUserData);
	}
	else
	{
		//发送带宽控制信息
		CTaskBase::RequestBwControl(m_stDataRevPlay.nCamID, FALSE);
		// 设置进度失败.
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_FAIL, 0, 0, pUserData);
	}
}

