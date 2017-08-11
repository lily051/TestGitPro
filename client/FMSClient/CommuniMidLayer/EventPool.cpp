#include "stdafx.h"
#include "EventPool.h"


/**	@fn	void OnEventTimeCheck
*	@brief 线程函数，检查使用中的事件时间是否超过允许的最大时间
*	@param[in] lpvoid：类指针
*	@param null
*	@return null
*/
void CEventPool::OnEventTimeCheck(LPVOID lpvoid)
{
	CEventPool * pInstance = (CEventPool *)lpvoid;
	if (pInstance == NULL)
	{
		return;
	}

	while (pInstance->m_bWorking)
	{
        {
            CGuard guard(&(pInstance->m_lckEvent));
            std::list<LPEVENT_INFO>::iterator itor = pInstance->m_listUsedEvent.begin();
            for (; itor != pInstance->m_listUsedEvent.end(); itor++)
            {
                if ((*itor)->lSetTime > pInstance->m_nEventTime)
                {
                    SetEvent((*itor)->hEvent);
                    (*itor)->bStatus = FALSE;
                    (*itor)->bStatus = 0;
                    pInstance->m_listFreeEvent.push_back(*itor);
                    pInstance->m_listUsedEvent.erase(itor);
                    break;
                }
                (*itor)->lSetTime++;                
            }
        }		
		Sleep(1000);
	}

}


CEventPool::CEventPool()
{
	m_nEventNum = 30;
	m_nEventTime = 60;
	m_bWorking = FALSE;
	m_hThread = NULL;
	InitializeCriticalSection(&m_lckEvent);
}

CEventPool::CEventPool(int nEventNum)
{
	m_nEventNum = nEventNum;
	m_nEventTime = 60;
	m_bWorking = FALSE;
	m_hThread = NULL;
	InitializeCriticalSection(&m_lckEvent);
}

CEventPool::~CEventPool()
{
	m_hThread = NULL;
	DeleteCriticalSection(&m_lckEvent);
}


/**	@fn	void SetEventNum
*	@brief 设置事件池中事件数量
*	@param[in] nEventNum：数量
*	@param null
*	@return null
*/
void CEventPool::SetEventNum(int nEventNum)
{
	m_nEventNum = nEventNum;
}

/**	@fn	void SetEventTime
*	@brief 设置每个事件允许最大激活时间
*	@param[in] nEventTime：设置的时间
*	@param null
*	@return null
*/
void CEventPool::SetEventTime(int nEventTime)
{
	m_nEventTime = nEventTime;
}

/**	@fn	void InitAllEvent
*	@brief 初始化所有事件对象
*	@param null
*	@param null
*	@return 成功:TRUE  失败:FALSE
*/
BOOL CEventPool::InitAllEvent()
{
	for (int i = 0; i < m_nEventNum; i++)
	{
		//创建Event
		HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hEvent == NULL)
		{
			continue;
		}

		//保存到free表中
		LPEVENT_INFO lpEventInfo = new EVENT_INFO;
		lpEventInfo->hEvent = hEvent;
		lpEventInfo->bStatus = FALSE;
		lpEventInfo->lSetTime = 0;

		m_listFreeEvent.push_back(lpEventInfo);
	}

	//创建线程
	m_bWorking = TRUE;
	DWORD dwRet;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnEventTimeCheck, this, 0, &dwRet);
	if (m_hThread == NULL)
	{
		m_bWorking = FALSE;
		CLIENT_ERROR("EventPool---事件池时间检测线程函数创建失败");
		return FALSE;
	}

	return TRUE;
}

/**	@fn	void ReleaseAllEvent
*	@brief 释放所有事件对象资源
*	@param null
*	@param null
*	@return null
*/
void CEventPool::ReleaseAllEvent()
{
	//thread
	m_bWorking = FALSE;
	WaitForSingleObject(m_hThread, INFINITE);
	if(m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	CGuard guard(&m_lckEvent);
	std::list<LPEVENT_INFO>::iterator itFreeList = m_listFreeEvent.begin();
	while (itFreeList != m_listFreeEvent.end())
	{
		if ((*itFreeList)->hEvent)
		{
			CloseHandle((*itFreeList)->hEvent);
		}
		//释放事件信息资源
		delete (*itFreeList);
		itFreeList++;
	}
    m_listFreeEvent.clear();

	std::list<LPEVENT_INFO>::iterator itUsedList = m_listUsedEvent.begin();
	while (itUsedList != m_listUsedEvent.end())
	{
		if ((*itUsedList)->hEvent)
		{
			CloseHandle((*itUsedList)->hEvent);
		}
		//释放事件信息资源
		delete (*itUsedList);
		itUsedList++;
	}
    m_listUsedEvent.clear();
}

/**	@fn	BOOL ResetSingleEvent
*	@brief 获取激活单个空闲事件对象
*	@param null
*	@param null
*	@return 获取的事件句柄
*/
HANDLE CEventPool::GetFreeEvent()
{
    {
        CGuard guard(&m_lckEvent);
        std::list<LPEVENT_INFO>::iterator itFreeList = m_listFreeEvent.begin();
        //存在空闲事件，直接获取
        if (itFreeList != m_listFreeEvent.end())
        {
            HANDLE hEvent = (*itFreeList)->hEvent;
            ResetEvent((*itFreeList)->hEvent);
            (*itFreeList)->bStatus = TRUE;
            (*itFreeList)->lSetTime = 0;
            m_listUsedEvent.push_back(*itFreeList);
            m_listFreeEvent.erase(itFreeList);
            return hEvent;
        }
    }	

	//无空闲事件，创建
	int nIndex = ++m_nEventNum;
	CString strEventName;
	strEventName.Format(_T("MsgWaitRsp%d"), nIndex);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, strEventName);
	if (hEvent == NULL)
	{
		return NULL;
	}
	LPEVENT_INFO lpEventInfo = new EVENT_INFO;
	lpEventInfo->hEvent = hEvent;
	lpEventInfo->lSetTime = 0;
	ResetEvent(hEvent);
	lpEventInfo->bStatus = TRUE;
	
    //加入使用中事件表中
    {
        CGuard guard(&m_lckEvent);
        m_listUsedEvent.push_back(lpEventInfo);
    }	

	return lpEventInfo->hEvent;
}

/**	@fn	BOOL SetSingleEvent
*	@brief set单个事件
*	@param[in] hEventHandle：事件句柄
*	@param null
*	@return 成功:TRUE  失败:FALSE
*/
BOOL CEventPool::SetSingleEvent(HANDLE hEvent)
{
    if (NULL == hEvent)
    {
        COMM_ERROR("SetSingleEvent for NULL Handle");
        return FALSE;
    }

	CGuard guard(&m_lckEvent);
	std::list<LPEVENT_INFO>::iterator itor = m_listUsedEvent.begin();
	while (itor != m_listUsedEvent.end())
	{
		if (hEvent == (*itor)->hEvent)
		{
			SetEvent((*itor)->hEvent);
			(*itor)->bStatus = FALSE;
			m_listFreeEvent.push_back(*itor);
			m_listUsedEvent.erase(itor);
			return TRUE;
		}
		itor++;
	}

	return FALSE;
}