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
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @param[in]   pData:��̬����.
 *    @return      
 */
void CTaskPlaybackByTime::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB)
	{
		return;
	}

	// �ط���.
	pfnTaskMsgCB(MSG_PLAYBACK_PLAYING, 0, 0, pUserData);

	// �ط�.
	HANDLE hPlayback = Playback();
	BOOL bRet = FALSE;
	if (NULL != hPlayback)
	{
		// �طųɹ�.
		pfnTaskMsgCB((UINT)MSG_PLAYBACK_PLAY_SUCCESS, (DWORD)hPlayback, m_stByTime.stPlaybackParam.bConvertPlay, pUserData);
		bRet = TRUE;
	}
}

/**   @fn          Playback
 *    @brief   	   �ط�.
 *    @return      �طž��.
 */
HANDLE CTaskPlaybackByTime::Playback()
{
	return PB_PlaybackTime(m_stByTime.stLoginInfo, m_stByTime.stPlaybackParam);
}
