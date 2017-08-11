//lint -library
#include "StdAfx.h"
#include "TaskCustom.h"

CTaskCustom::CTaskCustom(void)
: m_hTaskThread(NULL)
, m_pfnTaskMsgCB(NULL)
, m_pUserData(NULL)
, m_bIsInit(FALSE)
{
	m_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == m_hTaskEvent)
    {
        DWORD err = GetLastError();
        CString strErr;
        strErr.Format(_T("�����¼�����ʧ��,������:%d"), err);
        OutputDebugString(strErr);
    }
}

CTaskCustom::~CTaskCustom(void)
{
	m_hTaskThread = NULL;
	m_hTaskEvent = NULL;
	if (m_hTaskEvent != NULL)
	{
		CloseHandle(m_hTaskEvent);
		m_hTaskEvent = NULL;
	}
}

/**   @fn          InitTask
 *    @brief   	   ��ʼ������.
 *    @param[in]   pfnTaskMsgCB:��Ϣ�ص�.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CTaskCustom::InitTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData)
{
	m_pfnTaskMsgCB = pfnTaskMsgCB;
	m_pUserData = pUserData;

	ResetEvent(m_hTaskEvent);
	m_hTaskThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFuncTask), this, 0, NULL);
	if (NULL == m_hTaskThread)
	{
		return FALSE;
	}

	m_bIsInit = TRUE;
	return TRUE;
}

/**   @fn          UnInitTask
 *    @brief   	   �ͷ���Դ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::UnInitTask()
{
	if (m_hTaskThread != NULL)
	{
		SetEvent(m_hTaskEvent);
		WaitForSingleObject(m_hTaskThread, INFINITE);
		CloseHandle(m_hTaskThread);
		m_hTaskThread = NULL;
	}
	m_bIsInit = FALSE;
	m_listTask.Clear();
}

/**   @fn          ThreadFuncTask
 *    @brief   	   �����߳�.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
DWORD WINAPI CTaskCustom::ThreadFuncTask(LPVOID pParam)
{
	CTaskCustom* pTaskCustom = (CTaskCustom*)pParam;
	if (NULL == pTaskCustom)
	{
		return 0;
	}

	while (WAIT_OBJECT_0 != WaitForSingleObject(pTaskCustom->m_hTaskEvent, 10))
	{
		CTaskBase* pTask = NULL;
		if (pTaskCustom->m_listTask.GetTask(pTask))
		{
			if (pTask != NULL)
			{
				// ��ȡ����̬����.
				TaskDynData stData;
				pTaskCustom->GetTaskDynData(stData);

				// ִ������.
				pTask->DoTask(pTaskCustom->m_pfnTaskMsgCB, pTaskCustom->m_pUserData, &stData);
			}
		}

		// ��������.
		SAFEDELETE(pTask);
	}

	ResetEvent(pTaskCustom->m_hTaskEvent);
	return 0;
}

/**   @fn			AddTask  
 *    @brief   	   	�������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::AddTask(CTaskBase* pTask)
{
	if (!m_bIsInit)
	{
		SAFEDELETE(pTask);
		return;
	}

	if (NULL == m_hTaskThread)
	{
		SAFEDELETE(pTask);
		return;
	}

	m_listTask.AddTask(pTask);
}

/**   @fn          ClearTask
 *    @brief   	   �������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::ClearTask()
{
	m_listTask.Clear();
}

/**   @fn          GetTaskDynData
 *    @brief   	   ��ȡ����̬����.
 *    @param[in]   stData:��̬�����ṹ��.
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::GetTaskDynData(TaskDynData& stData)
{

}