#include "StdAfx.h"
#include "TaskBwReConnect.h"

CTaskBwReConnect::CTaskBwReConnect(const TaskDataBwReconnect& stDataBw)
{
	m_stDataBw.nCamID = stDataBw.nCamID;
}

CTaskBwReConnect::~CTaskBwReConnect(void)
{
}

/**   @fn          DoTask
 *    @brief   	   ִ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CTaskBwReConnect::DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData)
{
	PLAYBACKDLL_INFO(_T("CTaskBwReConnect"));
	if (!CTaskBase::RequestBwControl(m_stDataBw.nCamID, TRUE))
	{
		PLAYBACKDLL_INFO(_T("CTaskBwReConnect FALSE"));
		pfnTaskMsgCB(MSG_PLAYBACK_BWRECONNECT_STOPPLAY, 0, 0, pUserData);
	}
}