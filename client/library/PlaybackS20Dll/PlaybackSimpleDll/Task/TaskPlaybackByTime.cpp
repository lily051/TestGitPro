//lint -library
#include "StdAfx.h"
#include "TaskPlaybackByTime.h"
#include "../PlaybackSimpleDll.h"

CTaskPlaybackByTime::CTaskPlaybackByTime(const TaskDataByTime& rDataByTime)
{
	memcpy_s(&m_stByTime, sizeof(TaskDataByTime), &rDataByTime, sizeof(TaskDataByTime));
}

CTaskPlaybackByTime::~CTaskPlaybackByTime(void)
{
}

/**   @fn          DoTask
 *    @brief   	   执行任务.
 *    @param[in]   pfnTaskMsgCB:消息回调函数.
 *    @param[in]   pUserData:用户自定义数据.
 *    @param[in]   pData:动态参数.
 *    @return      
 */
void CTaskPlaybackByTime::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB)
	{
		return;
	}

	// 回放中.
	pfnTaskMsgCB(MSG_PLAYBACK_PLAYING, 0, 0, pUserData);

	// 回放.
	HANDLE hPlayback = Playback();
	BOOL bRet = FALSE;
	if (NULL != hPlayback)
	{
		// 回放成功.
		pfnTaskMsgCB((UINT)MSG_PLAYBACK_PLAY_SUCCESS, (DWORD)hPlayback, m_stByTime.stPlaybackParam.bConvertPlay, pUserData);
		bRet = TRUE;
	}
}

/**   @fn          Playback
 *    @brief   	   回放.
 *    @return      回放句柄.
 */
HANDLE CTaskPlaybackByTime::Playback()
{
	return PB_PlaybackTime(m_stByTime.stLoginInfo, m_stByTime.stPlaybackParam);
}
