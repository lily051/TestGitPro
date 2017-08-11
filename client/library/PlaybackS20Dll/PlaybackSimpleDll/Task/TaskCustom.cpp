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
        strErr.Format(_T("创建事件对象失败,错误码:%d"), err);
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
 *    @brief   	   初始化任务.
 *    @param[in]   pfnTaskMsgCB:消息回调.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      TRUE:成功,FALSE:失败.
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
 *    @brief   	   释放资源.
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
 *    @brief   	   任务线程.
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
				// 获取任务动态参数.
				TaskDynData stData;
				pTaskCustom->GetTaskDynData(stData);

				// 执行任务.
				pTask->DoTask(pTaskCustom->m_pfnTaskMsgCB, pTaskCustom->m_pUserData, &stData);
			}
		}

		// 销毁任务.
		SAFEDELETE(pTask);
	}

	ResetEvent(pTaskCustom->m_hTaskEvent);
	return 0;
}

/**   @fn			AddTask  
 *    @brief   	   	添加任务.
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
 *    @brief   	   清空任务.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::ClearTask()
{
	m_listTask.Clear();
}

/**   @fn          GetTaskDynData
 *    @brief   	   获取任务动态参数.
 *    @param[in]   stData:动态参数结构体.
 *    @param[in]   
 *    @return      
 */
void CTaskCustom::GetTaskDynData(TaskDynData& stData)
{

}