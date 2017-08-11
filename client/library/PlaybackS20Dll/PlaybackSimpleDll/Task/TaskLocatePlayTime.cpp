//lint -library
#include "StdAfx.h"
#include "TaskLocatePlayTime.h"

CTaskLocatePlayTime::CTaskLocatePlayTime(const TaskDataLocate& stDataLocate)
{
	memcpy_s(&m_stDataLocate, sizeof(TaskDataLocate), &stDataLocate, sizeof(TaskDataLocate));
}

CTaskLocatePlayTime::~CTaskLocatePlayTime(void)
{
}

/**   @fn          DoTask
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CTaskLocatePlayTime::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	if (NULL == pfnTaskMsgCB  ||  NULL == pData)
	{
		return;
	}

	BOOL bRet = PB_LocatePlayTime((HANDLE)pData->pObject, m_stDataLocate.stStartTime, m_stDataLocate.stStopTime);
	if (bRet)
	{
		// ���ý��ȳɹ�.
		//Sleep(1000);
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_SUCCESS, 0, 0, pUserData);
	}
	else
	{
		// ���ý���ʧ��.
		pfnTaskMsgCB(MSG_PLAYBACK_LOCATEPLAY_FAIL, 0, 0, pUserData);
	}
}

