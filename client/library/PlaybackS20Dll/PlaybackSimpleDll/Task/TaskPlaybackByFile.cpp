//lint -library
#include "StdAfx.h"
#include "TaskPlaybackByFile.h"
#include "../PlaybackSimpleDll.h"

CTaskPlaybackByFile::CTaskPlaybackByFile(const TaskDataByFile& stDataByFile)
{
	memcpy_s(&m_stByFile, sizeof(TaskDataByFile), &stDataByFile, sizeof(TaskDataByFile));
}

CTaskPlaybackByFile::~CTaskPlaybackByFile(void)
{
}

/**   @fn          DoTask
 *    @brief   	   执行任务.
 *    @param[in]   pfnTaskMsgCB:消息回调函数.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
void CTaskPlaybackByFile::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
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
		pfnTaskMsgCB(MSG_PLAYBACK_PLAY_SUCCESS, (DWORD)hPlayback, m_stByFile.stPlaybackParam.bConvertPlay, pUserData);
		bRet = TRUE;
	}
}

/**   @fn          Playback
 *    @brief   	   回放.
 *    @return      回放句柄.
 */
HANDLE CTaskPlaybackByFile::Playback()
{
	return PB_PlaybackFile(m_stByFile.stLoginInfo, m_stByFile.stPlaybackParam);
}