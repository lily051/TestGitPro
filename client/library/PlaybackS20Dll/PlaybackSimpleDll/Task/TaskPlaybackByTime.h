/**   @file TaskPlaybackByTime.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ��ʱ����лط�.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/17
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

// ��ʱ��طŲ���.
typedef struct _TaskDataByTime
{
	HANDLE hDlg;						// �Ի�����.
	int nCamID;							// ��ص�ID.
	DWORD dwRev;						// �����ֶ�.
	PB_LOGININFO stLoginInfo;			// ��¼��Ϣ.
	PLAYBACK_TIME_INFO stPlaybackParam; // ��ʱ��طŲ���.
	BOOL bReversePlay;					// �Ƿ��ǵ���ģʽ.
}TaskDataByTime, *LPTaskDataByTime;


class CTaskPlaybackByTime : public CTaskBase
{
public:
	CTaskPlaybackByTime(const TaskDataByTime& rDataByTime);
	~CTaskPlaybackByTime(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// �ط�.
	HANDLE Playback();

private:
	// ��ʱ��طŲ���.
	TaskDataByTime m_stByTime;
};
