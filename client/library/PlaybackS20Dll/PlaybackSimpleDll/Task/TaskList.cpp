//lint -library
#include "StdAfx.h"
#include "TaskList.h"

CTaskList::CTaskList(void)
{
	InitializeCriticalSection(&m_csTaskLock);
}

CTaskList::~CTaskList(void)
{
	DeleteCriticalSection(&m_csTaskLock);
}

/**   @fn          AddTask
 *    @brief   	   �������.
 *    @param[in]   pTask:����ָ��.
 *    @param[in]   
 *    @return      
 */
void CTaskList::AddTask(CTaskBase* pTask)
{
	CGuard guard(&m_csTaskLock);

	m_ArrayTask.Add(pTask);
}

/**   @fn          GetTask
 *    @brief   	   ��ȡ����.
 *    @param[in]   pTask:����ָ��.
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CTaskList::GetTask(CTaskBase*& pTask)
{
	CGuard guard(&m_csTaskLock);

	if (m_ArrayTask.IsEmpty())
	{
		return FALSE;
	}

	pTask = m_ArrayTask.GetAt(0);
	m_ArrayTask.RemoveAt(0);

	return TRUE;
}

/**   @fn          Clear
 *    @brief   	   �������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CTaskList::Clear()
{
	CGuard guard(&m_csTaskLock);

	int nTaskCount = m_ArrayTask.GetCount();
	while (nTaskCount > 0)
	{
		nTaskCount--;

		CTaskBase* pTask = m_ArrayTask.GetAt(nTaskCount);
		if (pTask != NULL)
		{
			delete pTask;
			pTask = NULL;
		}

		m_ArrayTask.RemoveAt(nTaskCount);
	}
}