#include "StdAfx.h"
#include "TaskReversePlaySwitch.h"

CTaskReversePlaySwitch::CTaskReversePlaySwitch(const TaskDataRevSwitch& stData)
{
	memcpy_s(&m_stDataRevPlay, sizeof(TaskDataRevSwitch), &stData, sizeof(TaskDataRevSwitch));
}

CTaskReversePlaySwitch::~CTaskReversePlaySwitch(void)
{
}

/**   @fn          DoTask
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CTaskReversePlaySwitch::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB  ||  NULL == pData)
	{
		return;
	}

	BOOL bRet = FALSE;

	bRet = PB_ReversePlaySwitch((HANDLE)pData->pObject, m_stDataRevPlay.bReversePlay, m_stDataRevPlay.stStartTime, m_stDataRevPlay.stStopTime);

	if (bRet)
	{
		// ���ý��ȳɹ�.
		//Sleep(1000);
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_SUCCESS, 0, 0, pUserData);
	}
	else
	{
		//���ʹ��������Ϣ
		CTaskBase::RequestBwControl(m_stDataRevPlay.nCamID, FALSE);
		// ���ý���ʧ��.
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_FAIL, 0, 0, pUserData);
	}
}

