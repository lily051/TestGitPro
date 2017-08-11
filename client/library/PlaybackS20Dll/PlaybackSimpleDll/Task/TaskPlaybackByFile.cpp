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
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CTaskPlaybackByFile::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
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
		pfnTaskMsgCB(MSG_PLAYBACK_PLAY_SUCCESS, (DWORD)hPlayback, m_stByFile.stPlaybackParam.bConvertPlay, pUserData);
		bRet = TRUE;
	}
}

/**   @fn          Playback
 *    @brief   	   �ط�.
 *    @return      �طž��.
 */
HANDLE CTaskPlaybackByFile::Playback()
{
	return PB_PlaybackFile(m_stByFile.stLoginInfo, m_stByFile.stPlaybackParam);
}