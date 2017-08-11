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
 *    @brief   	   查找文件并回放.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   stLoginInfo:登录信息.
 *    @param[in]   stPlaybackParam:回放参数.
 *    @param[in]   emPlayType:回放模块.
 *    @param[in]   bPlayTime:是否按时间回放.
 *    @param[in]   bReversePlay:是否是倒放.
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

	// 创建时间回放任务.
	CTaskFindAndPlay* pTask = new CTaskFindAndPlay(stData);
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          PlaybackTime
 *    @brief   	   按时间回放.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   stLoginInfo:登录信息.
 *    @param[in]   stPlaybackParam:回放参数.
 *    @param[in]   emPlayType:回放模块.
 *    @param[in]   bReversePlay:是否是倒放.
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

	// 创建时间回放任务.
	CTaskPlaybackByTime* pTask = new CTaskPlaybackByTime(stData);
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          PlaybackFile
 *    @brief   	   按文件回放.
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

	// 创建文件回放任务.
	CTaskPlaybackByFile* pTask = new CTaskPlaybackByFile(stData);
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          StopPlayback
 *    @brief   	   停止回放.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @param[in]   emPlayType:回放类型.
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

	// 创建停止回放任务.
	CTaskStopPlayback* pTask = new CTaskStopPlayback(stData);
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          TaskStopPlayback
 *    @brief   	   停止回放.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @param[in]   emPlayType:回放类型.
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

	// 创建停止回放任务.
	CTaskStopPlayback TaskStop(stData);
	TaskDynData stDynData;
	stDynData.pObject = (LPVOID)m_hPlayHandle;
	TaskStop.DoTask(m_pfnTaskMsgCB, m_pUserData, &stDynData);
}

/**   @fn          PlaybackControl
 *    @brief   	   回放控制.
 *    @param[in]   emControlType:控制类型.
 *    @param[in]   lValueIn:输入参数.
 *    @param[out]  lValueOut:输出参数.
 *    @return      
 */
BOOL CPlaybackOper::PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut)
{
	return PB_PlaybackControl(m_hPlayHandle, emControlType, lValueIn, lValueOut);
}	

/**   @fn          GetOsdTime
 *    @brief   	   获取OSD时间.
 *    @param[out]  lOsdTime:osd时间. 
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlaybackOper::GetOsdTime(LONG & lOsdTime)
{
	return PB_GetOsdTime(m_hPlayHandle, lOsdTime);
}

/**   @fn          GetPlayPos
 *    @brief   	   获取回放进度.
 *    @param[out]  lPos:回放进度.	   
 *    @return	   TRUE:成功,FALSE:失败.
 */
BOOL CPlaybackOper::GetPlayPos(LONG & lPos)
{
	return PB_GetPlayPos(m_hPlayHandle, lPos);
}

/**   @fn          Capture
 *    @brief   	   回放抓图.
 *    @param[in]   szPicFile:文件名.
 *    @param[in]   emPicType:图片类型.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @return      
 */
BOOL CPlaybackOper::Capture(HANDLE hDlg, char * szPicFile, ENUM_PIC_TYPE emPicType, int nCamID, DWORD dwRev)
{
	BOOL bRet =  PB_Capture(m_hPlayHandle, szPicFile, emPicType);
	return bRet;
}

/**   @fn          SaveData
 *    @brief   	   录像剪辑.
 *    @param[in]   szSaveFile:文件名.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlaybackOper::SaveData(HANDLE hDlg, char * szSaveFile, int nCamID, DWORD dwRev)
{
	BOOL bRet = PB_SaveData(m_hPlayHandle, szSaveFile);
	return bRet;
}

/**   @fn          StopSave
 *    @brief   	   停止剪辑.
 *    @param[in]   nCamID:监控点ID.
 *    @return      
 */
void CPlaybackOper::StopSave(HANDLE hDlg, int nCamID, DWORD dwRev)
{
	BOOL bRet = PB_StopSave(m_hPlayHandle);
	if (FALSE == bRet)
	{
		PLAYBACKDLL_ERROR(_T("停止剪辑失败!"));
	}
}

/**   @fn          LocatePlayTime
 *    @brief   	   本地时间回放.
 *    @param[in]   stStartTime:回放时间.
 *    @param[in]   stStopTime:回放停止时间.
 *    @param[in]   nCamID:监控点ID.
 *    @param[in]   bReversePlay:是否是倒放.
 *    @return      
 */
void CPlaybackOper::LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nCamID, BOOL bReversePlay)
{
	TaskDataLocate stDataLocate;
	memcpy_s(&stDataLocate.stStartTime, sizeof(PB_TIME), &stStartTime, sizeof(PB_TIME));
	memcpy_s(&stDataLocate.stStopTime, sizeof(PB_TIME), &stStopTime, sizeof(PB_TIME));
	stDataLocate.nCamID = nCamID;
	stDataLocate.bReversePlay = bReversePlay;

	// 创建本地时间回放任务.
	CTaskLocatePlayTime* pTask = new CTaskLocatePlayTime(stDataLocate);
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          SetPlayHandle
 *    @brief   	   设置回放句柄.
 *    @param[in]   hPlayHandle:回放句柄.
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::SetPlayHandle(HANDLE hPlayHandle)
{
	m_hPlayHandle = hPlayHandle;
}

/**   @fn          GetTaskDynData
 *    @brief   	   获取任务动态参数.
 *    @param[in]   stData:任务动态参数.
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::GetTaskDynData(TaskDynData& stData)
{
	stData.pObject = (LPVOID)m_hPlayHandle;
}

/**   @fn          GetSDKLastError
 *    @brief   	   获取SDK错误码.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */

DWORD CPlaybackOper::GetSDKLastError()
{
	return PB_GetLastError();
}

/**   @fn          InvalidateWindow
 *    @brief   	   刷新窗口.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackOper::InvalidateWindow()
{
	// 创建刷新任务.
	CTaskInvalidate* pTask = new CTaskInvalidate;
	// 添加任务.
	CTaskCustom::AddTask(pTask);
}

/**   @fn          GetCompressCfgAbility
 *    @brief   	   是否正在回放.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-是,FALSE-否.
 */
BOOL CPlaybackOper::GetCompressCfgAbility( PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutBuf )
{
	return PB_GetCompressCfgAbility(stLoginInfo, lChannel, pOutBuf);
}