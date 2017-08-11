//lint -library
#include "StdAfx.h"
#include "TaskLocatePlayTime.h"

CTaskLocatePlayTime::CTaskLocatePlayTime(const TaskDataLocate& stDataLocate)
{
	memcpy_s(&m_stDataLocate, sizeof(TaskDataLocate), &stDataLocate, sizeof(TaskDataLocate));
}

CTaskLocatePlayTime::~CTaskLocatePlayTime(void)
{
}

/**   @fn          DoTask
 *    @brief   	   执行任务.
 *    @param[in]   pfnTaskMsgCB:消息回调函数.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
void CTaskLocatePlayTime::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB  ||  NULL == pData)
	{
		return;
	}

	BOOL bRet = PB_LocatePlayTime((HANDLE)pData->pObject, m_stDataLocate.stStartTime, m_stDataLocate.stStopTime);
	if (bRet)
	{
		// 设置进度成功.
		//Sleep(1000);
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_SUCCESS, 0, 0, pUserData);
	}
	else
	{
		// 设置进度失败.
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_FAIL, 0, 0, pUserData);
	}
}

