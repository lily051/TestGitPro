#include "StdAfx.h"
#include "WorkerThread.h"

const WCHAR* WorkerThread::szWorkerWndClsName = L"WORKER_CLASS";
BOOL WorkerThread::s_bWndClsRegistered = FALSE;

WorkerThread::WorkerThread(void)
: m_hMsgWnd(NULL)
, m_hThread(NULL)
, m_nWorkItems(0)
, m_dwThreadId(0)
, m_nAboutToPost(0)
, m_bAvailable(TRUE)
, m_hStartEvent(NULL)
, m_dwSleepTime(50)
, m_bRunning(TRUE)
{
    m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

WorkerThread::~WorkerThread(void)
{
    try
    {
        if (m_hStartEvent)
        {
            CloseHandle(m_hStartEvent);
            m_hStartEvent = NULL;
        }
    }
    catch (...)
    {
    }
}

BOOL WorkerThread::Stop(BOOL bPeaceful)
{
    if (m_hThread)
    {
        m_bAvailable = FALSE;
        m_bRunning = FALSE;
        while (m_nAboutToPost > 0)
        {
            Sleep(10);
        }
        // From now on, No more WM_WORKITEM msgs will be posted

        // 
        if (bPeaceful)
        {
            ::PostMessage(m_hMsgWnd, WM_FINISH, NULL, NULL);
        }else
        {
            ::SendMessage(m_hMsgWnd, WM_FINISH, NULL, NULL);
        }        
        CLIENT_DEBUG("Wait for WorkerThread to exit ...");
        //WaitForSingleObject(m_hThread, INFINITE);
        while (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 1000))
        {
            ::SendMessage(m_hMsgWnd, WM_FINISH, NULL, NULL);
        }
        CloseHandle(m_hThread);
        m_hThread = NULL;
    }
    return TRUE;    
}

BOOL WorkerThread::Start(void)
{
    if (m_hThread)
    {
        return TRUE;
    }

    uintptr_t ret = _beginthreadex(NULL, NULL, &WorkerThread::ThreadProc
        , this, NULL, NULL);
    if (0 == ret)
    {
        CLIENT_ERROR("Create ThreadProc thread failed");
        return FALSE;
    }else
    {
        m_hThread = (HANDLE)ret;
        CLIENT_DEBUG("ThreadProc thread launched and Wait ...");
        WaitForSingleObject(m_hStartEvent, INFINITE);
        ResetEvent(m_hStartEvent);
        m_bAvailable = TRUE;
        m_bRunning = TRUE;
        return TRUE;
    }
}

BOOL WorkerThread::PostWorkItem(WorkItem* pItem)
{
    BOOL bRet = FALSE;
    if (m_bAvailable)
    {
        InterlockedIncrement(&m_nAboutToPost);
        if (m_bAvailable)
        {
            bRet = ::PostMessage(m_hMsgWnd, WM_WORKITEM, (WPARAM) pItem, NULL);
        }        
        InterlockedDecrement(&m_nAboutToPost);
    }

    if (bRet)
    {
        InterlockedIncrement(&m_nWorkItems);
    }
    return bRet;
}

BOOL WorkerThread::SetTimer(UINT_PTR nIDEvent, UINT uElapse)
{
    if (NULL == m_hMsgWnd)
    {
        return FALSE;
    }

    if (0 == ::SetTimer(m_hMsgWnd, nIDEvent, uElapse * 1000, NULL))
    {
        CLIENT_ERROR("SetTimer failed:%u", GetLastError());
        return FALSE;
    }
    ::PostMessage(m_hMsgWnd, WM_TIMER, nIDEvent, NULL);
    return TRUE;
}

BOOL WorkerThread::NotifyTimer(UINT_PTR nIDEvent)
{
    if (NULL == m_hMsgWnd)
    {
        return FALSE;
    }    
    return ::PostMessage(m_hMsgWnd, WM_TIMER, nIDEvent, NULL);
}

BOOL WorkerThread::KillTimer(UINT_PTR nIDEvent)
{
    if (NULL == m_hMsgWnd)
    {
        return FALSE;
    }

    return ::KillTimer(m_hMsgWnd, nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// Following Methods are executed in WorkerThread itself

unsigned _stdcall WorkerThread::ThreadProc(void * pStartup)
{
    WorkerThread* pThread = (WorkerThread*)pStartup;
    if (pThread)
    {
        return pThread->RunMessageLoop();
    }
    return 0;    
}

BOOL WorkerThread::RegisterWndCls(void)
{
    if (!s_bWndClsRegistered)
    {
        // Register Message Only Window
        WNDCLASSEX wx = {};
        wx.cbSize = sizeof(WNDCLASSEX);
        wx.lpfnWndProc = WorkerThread::MsgWndProc;    // function which will handle messages
        wx.hInstance = GetModuleHandle(NULL);
        wx.lpszClassName = WorkerThread::szWorkerWndClsName;
        if (!RegisterClassEx(&wx))
        {
            CLIENT_ERROR("Register Message Window failed: %u", GetLastError());
        }else
        {
            s_bWndClsRegistered = TRUE;
        }
    }
    return s_bWndClsRegistered;
}

unsigned WorkerThread::RunMessageLoop(void)
{
    if(!RegisterWndCls())
    {
        SetEvent(m_hStartEvent);
        return 1;
    }

    // Create Message Only Window
    HWND hWnd = CreateWindowEx( 0 
        , WorkerThread::szWorkerWndClsName
        , L"MsgWindow"
        , 0, 0, 0, 0, 0
        , HWND_MESSAGE
        , NULL
        , GetModuleHandle(NULL)
        , NULL);
    if (NULL == hWnd)
    {
        CLIENT_ERROR("Create Message Window failed: %u", GetLastError());
        SetEvent(m_hStartEvent);
        return 1;
    }
    m_hMsgWnd = hWnd;
    m_dwThreadId = GetCurrentThreadId();
    CLIENT_DEBUG("WorkerThread[%u] is going!", m_dwThreadId);
    SetEvent(m_hStartEvent);

    // ³õÊ¼»¯COM

    // Message loop
    BOOL bRet;
    while(TRUE)
    {
        MSG msg;
        if (::PeekMessage(&msg, NULL/*m_hMsgWnd*/, NULL, NULL, PM_NOREMOVE))   // has msg
        {
            //CLIENT_TRACE("Peek a MSG in ThreadProc thread: %d\r\n", msg.message);
            bRet = ::GetMessage(&msg, m_hMsgWnd, 0, 0); // get the msg            
            //CLIENT_TRACE("Get a MSG in ThreadProc thread: %d\r\n", msg.message);
            if (bRet == -1)
            {
                // handle the error and possibly exit
                CLIENT_ERROR("GetMessage error in WorkerThread[%u]:%u",m_dwThreadId,GetLastError());
            }else if (bRet == 0)    // quit of WM_QUIT message
            {
                CLIENT_DEBUG("WorkerThread[%u] break 'cause of WM_QUIT", m_dwThreadId);
                break;
            }
            else
            {
                if (WM_FINISH == msg.message)
                {
                    CLIENT_DEBUG("WorkerThread[%u] break 'cause of WM_FINISH", m_dwThreadId);
                    break;
                }
                if (WM_WORKITEM == msg.message)
                {
                    InterlockedDecrement(&m_nWorkItems);
                }
                if (WM_TIMER == msg.message)
                {
                    OnTimer((UINT_PTR)msg.wParam);
                }
                ::DispatchMessage(&msg);    // Call MsgWndProc
            }
        }else   // no msg(OnIdle)
        {
#pragma message("======== TODO: more responsive way")
            Sleep(m_dwSleepTime);
        }
    }

    DestroyWindow(m_hMsgWnd);
    m_hMsgWnd = NULL;

    CLIENT_DEBUG("WorkerThread[%u] is exiting![%ld]", m_dwThreadId, m_nWorkItems);
    return 1;
}

LRESULT CALLBACK WorkerThread::MsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //CLIENT_TRACE("Dispatched MSG in MsgWndProc : %d\r\n", uMsg);
    switch(uMsg)
    {
    case WM_WORKITEM:
        {
            WorkItem* pItem = (WorkItem*)wParam;
            {
                if(!pItem->UnderTake())
                {                    
                    CLIENT_WARN("WorkItem UnderTake failed");
                }
            }
            delete pItem;
        }
        break;
    case WM_FINISH:
        {
            // Cleanup all messsages in the queue
            MSG msg;
            int nCount = 0;
            while (::PeekMessage(&msg, NULL/*hWnd*/, NULL, NULL, PM_REMOVE))
            {
                //CLIENT_TRACE("Peek a MSG in MsgWndProc WM_FINISH: %d\r\n", msg.message);
                if (WM_WORKITEM == msg.message)
                {
                    WorkItem* pItem = (WorkItem*)msg.wParam;
                    delete pItem;
                    nCount++;
                }
            }
            CLIENT_DEBUG("%d WorkItems abandoned", nCount);
            ::PostQuitMessage(0);
            //::PostMessage(hWnd, WM_FINISH, NULL, NULL);
            CLIENT_DEBUG("Quit Message Posted");
        }
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
