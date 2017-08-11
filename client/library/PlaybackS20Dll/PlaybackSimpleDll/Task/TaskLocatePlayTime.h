/**   @file TaskLocatePlayTime.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ���ûطŽ�������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/22
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

typedef struct _TaskDataLocate
{
	int nCamID;			// ��ص�ID.
	PB_TIME stStartTime;	// �ط�ʱ��.
	PB_TIME stStopTime;		// ֹͣʱ��.
	BOOL bReversePlay;		// �Ƿ��ǵ���.
}TaskDataLocate, *LPTaskDataLocate;

class CTaskLocatePlayTime : public CTaskBase
{
public:
	CTaskLocatePlayTime(const TaskDataLocate& stDataLocate);
	~CTaskLocatePlayTime(void);

public:
	// ִ������.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);


private:
	// ���ûطŽ��Ƚṹ��.
	TaskDataLocate m_stDataLocate;
};
