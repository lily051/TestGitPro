/**   @file PlaybackOper.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	�طŲ�����.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/10/30
 *
 *    @note		���ڽ��лطſ���.
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskCustom.h"
#include "TaskPlaybackByTime.h"
#include "TaskPlaybackByFile.h"
#include "TaskStopPlayback.h"
#include "TaskLocatePlayTime.h"
#include "TaskInvalidate.h"
#include "TaskFindAndPlay.h"

class CPlaybackOper : public CTaskCustom
{
public:
	CPlaybackOper(void);
	~CPlaybackOper(void);

private:
	// ���ž��.
	HANDLE m_hPlayHandle;

public:
	// ���ûطž��.
	void SetPlayHandle(HANDLE hPlayHandle);
	// ��ȡ����̬����.
	virtual void GetTaskDynData(TaskDynData& stData);

public:
	// ����¼���ļ����ط�.
	void FindAndPlayback(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindFileParam, 
		LPB_FILEINFO pFileList,
		BOOL bReversePlay,
		BOOL bPlayTime);
	// ��ʱ��ط�.
	void PlaybackTime(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		BOOL bReversePlay);
	// ���ļ��ط�.
	void PlaybackFile(
		HANDLE hDlg,
		int nCamID, 
		DWORD dwRev,
		PB_LOGININFO& stLoginInfo, 
		PLAYBACK_FILE_INFO& stPlaybackParam,
		BOOL bReversePlay);
	// ֹͣ�ط�.
	void StopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl);
	// ��������ʱֹͣ�ط�.
	void TaskStopPlayback(HANDLE hDlg, int nCamID, DWORD dwRev, BOOL bAutoStop, BWControlType emBwControl);
	// �طſ���.
	BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);
	// ��ȡOSDʱ��.
	BOOL GetOsdTime(LONG & lOsdTime);
	// ��ȡ�طŽ���.
	BOOL GetPlayPos(LONG & lPos);
	// �ط�ץͼ.
	BOOL Capture(HANDLE hDlg, char * szPicFile, ENUM_PIC_TYPE emPicType, int nCamID, DWORD dwRev);
	// ¼�����.
	BOOL SaveData(HANDLE hDlg,  char * szSaveFile, int nCamID, DWORD dwRev);
	// ֹͣ����.
	void StopSave(HANDLE hDlg,  int nCamID, DWORD dwRev);
	// ����ʱ��ط�.
	void LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nCamID, BOOL bReversePlay);
	// ��ȡ������.
	DWORD GetSDKLastError();
	// ˢ�´���.
	void InvalidateWindow();
    // ��ȡdvrת��ط�������
    BOOL GetCompressCfgAbility(PB_LOGININFO& stLoginInfo, LONG lChannel, char* pOutBuf );
};
