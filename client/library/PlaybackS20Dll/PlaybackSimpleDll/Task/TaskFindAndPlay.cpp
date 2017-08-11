//lint -library
#include "StdAfx.h"
#include "TaskFindAndPlay.h"
#include "TaskPlaybackByFile.h"
#include "TaskPlaybackByTime.h"

CTaskFindAndPlay::CTaskFindAndPlay(const TaskDataFindAndPlay& stData)
{
	memcpy_s(&m_stData, sizeof(TaskDataFindAndPlay), &stData, sizeof(TaskDataFindAndPlay));
}

CTaskFindAndPlay::~CTaskFindAndPlay(void)
{
}

/**   @fn          DoTask
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @param[in]   pData:��̬����.
 *    @return      
 */
void CTaskFindAndPlay::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB)
	{
		return;
	}

	PB_FILEINFO stFileInfo;
	if (!FindFile(pfnTaskMsgCB, pUserData, pData, stFileInfo))
	{
		return;
	}
    //�����ط�url
    strcpy_s(m_stData.stPlaybackParam.szPlayUrl, sizeof(m_stData.stPlaybackParam.szPlayUrl), stFileInfo.szPlayUrl);
	// ��ʱ��ط�.
	if (m_stData.bPlayTime)
	{
		PlaybackByTime(pfnTaskMsgCB, pUserData, pData);
	}
	else
	{
		PlaybackbyFile(pfnTaskMsgCB, pUserData, pData, stFileInfo);
	}
}

/**   @fn          FindFile
 *    @brief   	   �����ļ�.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @param[in]   pData:��̬����.
 *    @return      
 */
BOOL CTaskFindAndPlay::FindFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, PB_FILEINFO& stFileInfo)
{
	if (NULL == pfnTaskMsgCB)
	{
		return FALSE;
	}

	// �ط���.
	pfnTaskMsgCB(MSG_PLAYBACK_PLAYING, 0, 0, pUserData);

	// �����ļ�.
	int nFileCount = 0;
	if (!FindFile(nFileCount))
	{
		// ����ʧ��.
		pfnTaskMsgCB(MSG_PLAYBACK_FIND_FAIL, (DWORD)(m_stData.pFileList), 0, pUserData);
		return FALSE;
	}

	if (PBSD_TYPE_KMS == m_stData.stLoginInfo.emPlayType)
	{
		nFileCount = 1;
	}
	m_stData.nFileCount = nFileCount;
	if (nFileCount <= 0)
	{
		// û���ҵ������������ļ�.
		pfnTaskMsgCB(MSG_PLAYBACK_FIND_NO_FILE, (DWORD)(m_stData.pFileList), 0, pUserData);
		return FALSE;
	}

	// �����ļ�.
	int nIndex = m_stData.bReversePlay ? (m_stData.nFileCount > 0 ? m_stData.nFileCount - 1 : 0) : 0;
	memcpy_s(&stFileInfo, sizeof(PB_FILEINFO), &(m_stData.pFileList[nIndex]), sizeof(PB_FILEINFO));
	stFileInfo.emPlayType = m_stData.stLoginInfo.emPlayType;
	stFileInfo.emDevType = m_stData.stLoginInfo.emDevType;
	stFileInfo.stStartTime = m_stData.stFindFileParam.stStartTime;
	stFileInfo.stEndTime = m_stData.stFindFileParam.stStopTime;

	// ���ҳɹ�.
	pfnTaskMsgCB(MSG_PLAYBACK_FIND_SUCCESS, (DWORD)(m_stData.pFileList), (DWORD)nFileCount, pUserData);

	return TRUE;
}

/**   @fn          FindFile
 *    @brief   	   �����ļ�.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CTaskFindAndPlay::FindFile(int& nFileCount)
{
	return PB_FindFile(m_stData.stLoginInfo, m_stData.stFindFileParam, m_stData.pFileList, nFileCount);
}

/**   @fn          PlaybackByTime
 *    @brief   	   ��ʱ��ط�.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @param[in]   pData:��̬����.
 *    @return      
 */
void CTaskFindAndPlay::PlaybackByTime(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	PLAYBACKDLL_INFO(_T("CTaskFindAndPlay PlaybackByTime"));

	TaskDataByTime stByTime;
	stByTime.hDlg = m_stData.hDlg;
	stByTime.nCamID = m_stData.nCamID;
	stByTime.dwRev = m_stData.dwRev;
	stByTime.bReversePlay = m_stData.bReversePlay;
	memcpy_s(&stByTime.stLoginInfo, sizeof(PB_LOGININFO), &m_stData.stLoginInfo, sizeof(PB_LOGININFO));
	memcpy_s(&stByTime.stPlaybackParam, sizeof(PLAYBACK_TIME_INFO), &m_stData.stPlaybackParam, sizeof(PLAYBACK_TIME_INFO));
	strcpy_s(stByTime.stPlaybackParam.szPlayUrl, sizeof(stByTime.stPlaybackParam.szPlayUrl), m_stData.stLoginInfo.unLoginType.stDvrLoginInfo.szRev);

	CTaskPlaybackByTime PlayTime(stByTime);
	PlayTime.DoTask(pfnTaskMsgCB, pUserData, pData);
}

/**   @fn          PlaybackbyFile
 *    @brief   	   ���ļ��ط�.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @param[in]   pData:��̬����.
 *    @return      
 */
void CTaskFindAndPlay::PlaybackbyFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, const PB_FILEINFO& stFileInfo)
{
	PLAYBACKDLL_INFO(_T("CTaskFindAndPlay PlaybackbyFile"));

	TaskDataByFile stByFile;
	stByFile.hDlg = m_stData.hDlg;
	stByFile.nCamID = m_stData.nCamID;
	stByFile.dwRev = m_stData.dwRev;
	stByFile.bReversePlay = m_stData.bReversePlay;
	memcpy_s(&stByFile.stLoginInfo, sizeof(PB_LOGININFO), &m_stData.stLoginInfo, sizeof(PB_LOGININFO));
	stByFile.stPlaybackParam.lChannel = m_stData.stPlaybackParam.lChannel;
	stByFile.stPlaybackParam.hWnd = m_stData.stPlaybackParam.hWnd;
	memcpy_s(&stByFile.stPlaybackParam.stFileInfo, sizeof(PB_FILEINFO), &stFileInfo, sizeof(PB_FILEINFO));
	strcpy_s(stByFile.stPlaybackParam.stFileInfo.szPlayUrl, sizeof(stByFile.stPlaybackParam.stFileInfo.szPlayUrl), m_stData.stLoginInfo.unLoginType.stDvrLoginInfo.szRev);
	
	CTaskPlaybackByFile PlayFile(stByFile);
	PlayFile.DoTask(pfnTaskMsgCB, pUserData, pData);
}