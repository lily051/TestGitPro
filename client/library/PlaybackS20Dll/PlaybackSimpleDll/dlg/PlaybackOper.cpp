//lint -library
#include "StdAfx.h"
#include "PlaybackOper.h"

CPlaybackOper::CPlaybackOper(void)
: m_hPlayHandle(NULL)
{
}

CPlaybackOper::~CPlaybackOper(void)
{
	m_hPlayHandle = NULL;
}

/**   @fn          PlaybackTime
 *    @brief   	   �����ļ����ط�.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   stLoginInfo:��¼��Ϣ.
 *    @param[in]   stPlaybackParam:�طŲ���.
 *    @param[in]   emPlayType:�ط�ģ��.
 *    @param[in]   bPlayTime:�Ƿ�ʱ��ط�.
 *    @param[in]   bReversePlay:�Ƿ��ǵ���.
 *    @return      
 */
void CPlaybackOper::FindAndPlayback(
								 HANDLE hDlg,
								 int nCamID, 
								 DWORD dwRev,
								 const PB_LOGININFO& stLoginInfo, 
								 const PLAYBACK_TIME_INFO& stPlaybackParam, 
								 const FINDFILE_PARAM& stFindFileParam, 
								 LPB_FILEINFO pFileList,
								 BOOL bReversePlay,
								 BOOL bPlayTime)
{
	TaskDataFindAndPlay stData;
	stData.hDlg = hDlg;
	stData.nCamID = nCamID;
	memcpy_s(&stData.stLoginInfo, sizeof(PB_LOGININFO), &stLoginInfo, sizeof(PB_LOGININFO));
	memcpy_s(&stData.stPlaybackParam, sizeof(PLAYBACK_TIME_INFO), &stPlaybackParam, sizeof(PLAYBACK_TIME_INFO));
	stData.dwRev = dwRev;
	stData.bPlayTime = bPlayTime;
	memcpy_s(&stData.stFindFileParam, sizeof(FINDFILE_PARAM), &stFindFileParam, sizeof(FINDFILE_PARAM));
	stData.pFileList = pFileList;
	stData.bReversePlay = bReversePlay;

	// ����ʱ��ط�����.
	CTaskFindAndPlay* pTask = new CTaskFindAndPlay(stData);
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          PlaybackTime
 *    @brief   	   ��ʱ��ط�.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   stLoginInfo:��¼��Ϣ.
 *    @param[in]   stPlaybackParam:�طŲ���.
 *    @param[in]   emPlayType:�ط�ģ��.
 *    @param[in]   bReversePlay:�Ƿ��ǵ���.
 *    @return      
 */
void CPlaybackOper::PlaybackTime(
								 HANDLE hDlg,
								 int nCamID, 
								 DWORD dwRev,
								 const PB_LOGININFO& stLoginInfo, 
								 const PLAYBACK_TIME_INFO& stPlaybackParam, 
								 BOOL bReversePlay)
{
	TaskDataByTime stData;
	stData.hDlg = hDlg;
	stData.nCamID = nCamID;
	memcpy_s(&stData.stLoginInfo, sizeof(PB_LOGININFO), &stLoginInfo, sizeof(PB_LOGININFO));
	memcpy_s(&stData.stPlaybackParam, sizeof(PLAYBACK_TIME_INFO), &stPlaybackParam, sizeof(PLAYBACK_TIME_INFO));
	stData.dwRev = dwRev;
	stData.bReversePlay = bReversePlay;

	// ����ʱ��ط�����.
	CTaskPlaybackByTime* pTask = new CTaskPlaybackByTime(stData);
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          PlaybackFile
 *    @brief   	   ���ļ��ط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::PlaybackFile(
								 HANDLE hDlg,
								 int nCamID, 
								 DWORD dwRev,
								 PB_LOGININFO& stLoginInfo, 
								 PLAYBACK_FILE_INFO& stPlaybackParam,
								 BOOL bReversePlay)
{
	TaskDataByFile stData;
	stData.hDlg = hDlg;
	stData.nCamID = nCamID;
	stData.dwRev = dwRev;
	stData.bReversePlay = bReversePlay;
	memcpy_s(&stData.stLoginInfo, sizeof(PB_LOGININFO), &stLoginInfo, sizeof(PB_LOGININFO));
	memcpy_s(&stData.stPlaybackParam, sizeof(PLAYBACK_FILE_INFO), &stPlaybackParam, sizeof(PLAYBACK_FILE_INFO));

	// �����ļ��ط�����.
	CTaskPlaybackByFile* pTask = new CTaskPlaybackByFile(stData);
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          StopPlayback
 *    @brief   	   ֹͣ�ط�.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @param[in]   emPlayType:�ط�����.
 *    @return      
 */
void CPlaybackOper::StopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl)
{
	TaskDataStop stData;
	stData.hDlg = hDlg;
	stData.nCamID = nCamID;
	stData.dwRev = dwRev;
	stData.bAutoStop = bAutoStop;
	stData.emBwControl = emBwControl;

	// ����ֹͣ�ط�����.
	CTaskStopPlayback* pTask = new CTaskStopPlayback(stData);
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          TaskStopPlayback
 *    @brief   	   ֹͣ�ط�.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @param[in]   emPlayType:�ط�����.
 *    @return      
 */
void CPlaybackOper::TaskStopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl)
{
	TaskDataStop stData;
	stData.hDlg = hDlg;
	stData.nCamID = nCamID;
	stData.dwRev = dwRev;
	stData.bAutoStop = bAutoStop;
	stData.emBwControl = emBwControl;

	// ����ֹͣ�ط�����.
	CTaskStopPlayback TaskStop(stData);
	TaskDynData stDynData;
	stDynData.pObject = (LPVOID)m_hPlayHandle;
	TaskStop.DoTask(m_pfnTaskMsgCB, m_pUserData, &stDynData);
}

/**   @fn          PlaybackControl
 *    @brief   	   �طſ���.
 *    @param[in]   emControlType:��������.
 *    @param[in]   lValueIn:�������.
 *    @param[out]  lValueOut:�������.
 *    @return      
 */
BOOL CPlaybackOper::PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	return PB_PlaybackControl(m_hPlayHandle, emControlType, lValueIn, lValueOut);
}	

/**   @fn          GetOsdTime
 *    @brief   	   ��ȡOSDʱ��.
 *    @param[out]  lOsdTime:osdʱ��. 
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackOper::GetOsdTime(LONG & lOsdTime)
{
	return PB_GetOsdTime(m_hPlayHandle, lOsdTime);
}

/**   @fn          GetPlayPos
 *    @brief   	   ��ȡ�طŽ���.
 *    @param[out]  lPos:�طŽ���.	   
 *    @return	   TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackOper::GetPlayPos(LONG & lPos)
{
	return PB_GetPlayPos(m_hPlayHandle, lPos);
}

/**   @fn          Capture
 *    @brief   	   �ط�ץͼ.
 *    @param[in]   szPicFile:�ļ���.
 *    @param[in]   emPicType:ͼƬ����.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @return      
 */
BOOL CPlaybackOper::Capture(HANDLE hDlg, char * szPicFile, ENUM_PIC_TYPE emPicType, int nCamID, DWORD dwRev)
{
	BOOL bRet =  PB_Capture(m_hPlayHandle, szPicFile, emPicType);
	return bRet;
}

/**   @fn          SaveData
 *    @brief   	   ¼�����.
 *    @param[in]   szSaveFile:�ļ���.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   dwRev:�����ֶ�.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackOper::SaveData(HANDLE hDlg, char * szSaveFile, int nCamID, DWORD dwRev)
{
	BOOL bRet = PB_SaveData(m_hPlayHandle, szSaveFile);
	return bRet;
}

/**   @fn          StopSave
 *    @brief   	   ֹͣ����.
 *    @param[in]   nCamID:��ص�ID.
 *    @return      
 */
void CPlaybackOper::StopSave(HANDLE hDlg, int nCamID, DWORD dwRev)
{
	BOOL bRet = PB_StopSave(m_hPlayHandle);
	if (FALSE == bRet)
	{
		PLAYBACKDLL_ERROR(_T("ֹͣ����ʧ��!"));
	}
}

/**   @fn          LocatePlayTime
 *    @brief   	   ����ʱ��ط�.
 *    @param[in]   stStartTime:�ط�ʱ��.
 *    @param[in]   stStopTime:�ط�ֹͣʱ��.
 *    @param[in]   nCamID:��ص�ID.
 *    @param[in]   bReversePlay:�Ƿ��ǵ���.
 *    @return      
 */
void CPlaybackOper::LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nCamID, BOOL bReversePlay)
{
	TaskDataLocate stDataLocate;
	memcpy_s(&stDataLocate.stStartTime, sizeof(PB_TIME), &stStartTime, sizeof(PB_TIME));
	memcpy_s(&stDataLocate.stStopTime, sizeof(PB_TIME), &stStopTime, sizeof(PB_TIME));
	stDataLocate.nCamID = nCamID;
	stDataLocate.bReversePlay = bReversePlay;

	// ��������ʱ��ط�����.
	CTaskLocatePlayTime* pTask = new CTaskLocatePlayTime(stDataLocate);
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          SetPlayHandle
 *    @brief   	   ���ûطž��.
 *    @param[in]   hPlayHandle:�طž��.
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::SetPlayHandle(HANDLE hPlayHandle)
{
	m_hPlayHandle = hPlayHandle;
}

/**   @fn          GetTaskDynData
 *    @brief   	   ��ȡ����̬����.
 *    @param[in]   stData:����̬����.
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::GetTaskDynData(TaskDynData& stData)
{
	stData.pObject = (LPVOID)m_hPlayHandle;
}

/**   @fn          GetSDKLastError
 *    @brief   	   ��ȡSDK������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */

DWORD CPlaybackOper::GetSDKLastError()
{
	return PB_GetLastError();
}

/**   @fn          InvalidateWindow
 *    @brief   	   ˢ�´���.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::InvalidateWindow()
{
	// ����ˢ������.
	CTaskInvalidate* pTask = new CTaskInvalidate;
	// �������.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          GetCompressCfgAbility
 *    @brief   	   �Ƿ����ڻط�.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-��,FALSE-��.
 */
BOOL CPlaybackOper::GetCompressCfgAbility( PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutBuf )
{
	return PB_GetCompressCfgAbility(stLoginInfo, lChannel, pOutBuf);
}