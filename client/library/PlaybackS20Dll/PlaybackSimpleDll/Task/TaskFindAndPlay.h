/**   @file		TaskFindAndPlay.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	���Ҳ��ط�¼���ļ�.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/05/10
 *
 *    @note  
 *    @note 
 *    @note		��ʷ��¼��
 *    @note		V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskBase.h"

// �����Ҳ��طŲ���.
typedef struct _TaskDataFindAndPlay
{
	HANDLE hDlg;						// �Ի�����.
	int nCamID;							// ��ص�ID.
	PB_LOGININFO stLoginInfo;			// ��¼��Ϣ.
	PLAYBACK_TIME_INFO stPlaybackParam; // ��ʱ��طŲ���.
	DWORD dwRev;						// ��������.
	BOOL bPlayTime;						// �Ƿ��ǰ�ʱ��ط�.
	FINDFILE_PARAM stFindFileParam;		// ���Ҳ���.
	LPB_FILEINFO pFileList;			// �ļ��б�ָ��.
	int nFileCount;					// �ļ�����ָ��.
	BOOL bReversePlay;					// �Ƿ��ǵ���ģʽ.
}TaskDataFindAndPlay, *LPTaskDataFindAndPlay;

class CTaskFindAndPlay : public CTaskBase
{
public:
	CTaskFindAndPlay(const TaskDataFindAndPlay& stData);
	~CTaskFindAndPlay(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

protected:
	// ���Ҳ��طŲ���.
	TaskDataFindAndPlay m_stData;

protected:
	// �����ļ�.
	BOOL FindFile(int& nFileCount);
	BOOL FindFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, PB_FILEINFO& stFileInfo);
	// ��ʱ��ط�.
	void PlaybackByTime(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);
	// ���ļ��ط�.
	void PlaybackbyFile(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData, const PB_FILEINFO& stFileInfo);
};
