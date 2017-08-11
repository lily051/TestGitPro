/**   @file TaskPlaybackByFile.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	���ļ����лط�������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/18
 *
 *    @note		
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskBase.h"

typedef struct _TaskDataByFile
{
	HANDLE hDlg;				// �Ի�����.
	int nCamID;					// ��ص�ID.
	DWORD dwRev;				// Ԥ���ֶ�.
	PB_LOGININFO stLoginInfo;	// ��¼��Ϣ.
	PLAYBACK_FILE_INFO stPlaybackParam;	// �ط���Ϣ.
	BOOL bReversePlay;					// �Ƿ��ǵ���.
}TaskDataByFile, *LPTaskDataByFile;


class CTaskPlaybackByFile : public CTaskBase
{
public:
	CTaskPlaybackByFile(const TaskDataByFile& stDataByFile);
	~CTaskPlaybackByFile(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// �ط�.
	HANDLE Playback();

private:
	// ���ļ��طŲ���.
	TaskDataByFile m_stByFile;
};
