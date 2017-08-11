/**	@file    IocpThreadPool.cpp
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   这里填写本文件的摘要。
 *
 *	@author	 liying
 *	@date	 2012/11/21
 *
 *	@note    下面的note和warning为可选项目
 *	@note    基于Windows IOCP的线程池
 *	@note    历史记录：
 *	@note    
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#include "StdAfx.h"
#include "IocpThreadPool.h"

IocpThread::IocpThread(IocpThreadPool* pPool)
: m_pPool(pPool)
, m_hThread(NULL)
, m_dwThreadId(0)
, m_bStopSignaled(FALSE)
{

}

IocpThread::~IocpThread(void)
{
    try
    {
        if (m_hThread)
        {
            if(!CloseHandle(m_hThread))
            {
                CLIENT_ERROR("CloseHandle in IocpThread[%u] failed:[%u]"
                            , m_dwThreadId, GetLastError());
            }
            m_hThread = NULL;
        }

        m_pPool = NULL;
    }
    catch (...)
    {
    }
}

BOOL IocpThread::Start(void)
{
    if (m_hThread)
    {
        return TRUE;
    }

    m_bStopSignaled = FALSE;
    uintptr_t ret = _beginthreadex(NULL, NULL, &IocpThread::ThreadProc
        , this, NULL, NULL);
    if (0 == ret)
    {
        CLIENT_ERROR("Launch IocpThread failed");
        return FALSE;
    }else
    {
        m_hThread = (HANDLE)ret;
        CLIENT_ERROR("IocpThread launched");
        return TRUE;
    }
}


void IocpThread::SignalStop(void)
{
    m_bStopSignaled = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Following Methods are executed in IocpThread itself

unsigned _stdcall IocpThread::ThreadProc(void * pStartup)
{
    IocpThread* pThread = (IocpThread*)pStartup;
    if (pThread)
    {
        return pThread->RunLoop();
    }
    return 0;
}

unsigned IocpThread::RunLoop(void)
{
    m_dwThreadId = GetCurrentThreadId();
    CLIENT_ERROR("IocpThread[%u] is going!", m_dwThreadId);

    BOOL bEverWorked = FALSE;

    // 初始化COM
    //CCOMInitObject oComInit;

    // Loop    
    while (TRUE)
    {
        WorkItem* pItem = NULL;
        BOOL bRet = m_pPool->InternalGetWorkItem(&pItem);
        if (bRet)
        {
            // dequeues a completion packet for WorkItem
            if (!m_bStopSignaled)
            {
                bEverWorked = TRUE;
                // Handle it
                if(!pItem->UnderTake())
                {                    
                    CLIENT_WARN("WorkItem UnderTake failed");
                }
                delete pItem;
            }else
            {
                CLIENT_ERROR("A WorkItem Abandoned");
                delete pItem;
                break;
            }            
        }
        // Check to exit after every WorkItem
        if (m_bStopSignaled)
        {
            break;
        }
    }

    CLIENT_ERROR("IocpThread[%u] is exiting!(%s)"
                , m_dwThreadId
                , (bEverWorked ? "EverWorked":"NeverWorked"));
    return 1;
}

//////////////////////////////////////////////////////////////////////////
// IocpThreadPool

IocpThreadPool::IocpThreadPool(void)
: m_hIOCP(NULL)
, m_nAboutToPost(0)
, m_bAvailable(FALSE)
, m_nWorkItems(0)
{
}

IocpThreadPool::~IocpThreadPool(void)
{
    try
    {
        if (m_hIOCP)
        {
            CloseHandle(m_hIOCP);
            m_hIOCP = NULL;
        }
    }    
    catch (...)
    {
    }
}

BOOL IocpThreadPool::Init(int nNumOfThreads)
{
    if (NULL != m_hIOCP)
    {
        return TRUE;
    }

    if (nNumOfThreads < MIN_THREADS_NUM)
    {
        nNumOfThreads = MIN_THREADS_NUM;
    }
    if (nNumOfThreads > MAX_THREADS_NUM)
    {
        nNumOfThreads = MAX_THREADS_NUM;
    }

    //
    m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE
                          , NULL
                          , NULL
                          , 0); // Concurrency value = number of processors, 0:let it decides
    if (NULL == m_hIOCP)
    {
        CLIENT_ERROR("CreateIoCompletionPort failed");
        return FALSE;
    }    

    //lint -e593
    int nCount = 0;
    for (int i = 0; i < nNumOfThreads; i++)
    {        
        IocpThread* pThread = new(std::nothrow) IocpThread(this);
        if (NULL == pThread)
        {
            CLIENT_ERROR("Out of Memory!");
            continue;
        }
        if (pThread->Start())
        {
            m_vThreadPool.push_back(pThread);
            nCount++;
        }else
        {
            delete pThread;
        }        
    }
    //lint +e593
    CLIENT_ERROR("Poputlate %d IocpThread in Pool", nCount);
    if (0 == nCount)
    {
        CloseHandle(m_hIOCP);
        m_bAvailable = FALSE;
        m_hIOCP = NULL;
        return FALSE;
    }
    m_bAvailable = TRUE;
    return TRUE;
}

void IocpThreadPool::Fini(void)
{
    // Shutdown iocp queue
    m_bAvailable = FALSE;
    while (m_nAboutToPost > 0)
    {
        CLIENT_WARN("Wait for pending requests");
        Sleep(10);
    }
    // From now on, No new WorkItem will be posted

    // Kill threads
    HANDLE hThreads[MAX_THREADS_NUM] = {0};
    int nCnt = 0;
    for (size_t i = 0; i < m_vThreadPool.size(); i++)
    {
        IocpThread* pThread = m_vThreadPool[i];
        if (pThread)
        {
            pThread->SignalStop();
            hThreads[nCnt] = pThread->GetThreadHandle();
            nCnt++;
        }
    }
    CLIENT_ERROR("%d IocpThread Signaled, wait for them to exit ...", nCnt);
    do 
    {
        for (int i = 0; i < nCnt; i++)
        {
            TestWorkItem* pItem = new TestWorkItem();
            InternalPostWorkItem(pItem);
        }
        CLIENT_ERROR("%d TeskWorkItem Posted", nCnt);
    } while (WAIT_TIMEOUT == WaitForMultipleObjects(nCnt, hThreads, TRUE, 1000));

    // Delete Thread Objects
    while (m_vThreadPool.size() > 0)
    {
        IocpThread* pThread = m_vThreadPool.back();
        if (pThread)
        {
            delete pThread;
        }
        m_vThreadPool.pop_back();
    }

    // Flush WorkItems
    int nCntItems = 0;
    DWORD dwNumOfBytes = 0;
    ULONG ulKey = 0;
    OVERLAPPED *pOL = NULL;
    while (GetQueuedCompletionStatus(m_hIOCP,&dwNumOfBytes,&ulKey,&pOL, 0))
    {
        WorkItem* pItem = (WorkItem*)pOL;
        delete pItem;
        nCntItems++;
    }
    CLIENT_INFO("%d WorkItems Abandoned", nCntItems);

    // Free IOCP
    if (m_hIOCP)
    {
        CloseHandle(m_hIOCP);
        m_hIOCP = NULL;
    }
}

#pragma message("======== TODO: IOCP Queue max work load")
BOOL IocpThreadPool::PostWorkItem(WorkItem* pItem)
{
    BOOL bRet = FALSE;
    if (m_bAvailable)
    {
        InterlockedIncrement(&m_nAboutToPost);
        if (m_bAvailable)
        {
            bRet = InternalPostWorkItem(pItem);
        }        
        InterlockedDecrement(&m_nAboutToPost);
    }
    
    return bRet;
}

BOOL IocpThreadPool::InternalPostWorkItem(WorkItem* pItem)
{
    BOOL bRet = FALSE;
    bRet = PostQueuedCompletionStatus( m_hIOCP, NULL, NULL
        , (LPOVERLAPPED)pItem);
    if (!bRet)
    {
        CLIENT_ERROR("PostQueuedCompletionStatus failed[%u]"
            , GetLastError());
    }else
    {
        InterlockedIncrement(&m_nWorkItems);
    }
    return bRet;
}

BOOL IocpThreadPool::InternalGetWorkItem(WorkItem** ppItem)
{
    DWORD dwNumOfBytes = 0;
    ULONG ulKey = 0;
    OVERLAPPED *pOL = NULL;
    BOOL bRet = GetQueuedCompletionStatus( m_hIOCP
        , &dwNumOfBytes
        , &ulKey
        , &pOL
        , INFINITE);
    if (bRet)
    {
        // dequeues a completion packet for WorkItem
         *ppItem = (WorkItem*)pOL;
         InterlockedDecrement(&m_nWorkItems);
    }else
    {
        if (NULL == pOL && WAIT_TIMEOUT == GetLastError())
        {
            CLIENT_ERROR("Unexpected WAIT_TIMEOUT");                
        }else
        {
            // dequeues a completion packet for a failed I/O operation
            CLIENT_ERROR("Unexpected I/O operation packet");
        }
    }
    return bRet;
}

LONG IocpThreadPool::PrintWorkLoad(void)
{
    //CMS_STAT_INFO("IocpThreadPool WorkItems:%ld", m_nWorkItems);
    CLIENT_INFO("IocpThreadPool WorkItems:%ld", m_nWorkItems);
    return m_nWorkItems;
}
