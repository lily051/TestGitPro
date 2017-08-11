#pragma once

#include "WorkItem.h"

#define WM_FINISH (WM_USER + 1024)
#define WM_WORKITEM (WM_USER + 1025)

class WorkerThread
{

private:
    HWND m_hMsgWnd;     // 工作线程通信窗口
    HANDLE m_hThread;   // 工作线程
    DWORD m_dwThreadId;

    HANDLE m_hStartEvent;

    volatile LONG m_nWorkItems;
    volatile LONG m_nAboutToPost;
    BOOL m_bAvailable;
    

    static BOOL s_bWndClsRegistered;
    static const WCHAR* szWorkerWndClsName;

    static unsigned _stdcall ThreadProc(void * pStartup);
    static LRESULT CALLBACK MsgWndProc( HWND hWnd
        , UINT uMsg
        , WPARAM wParam
        , LPARAM lParam);

    BOOL RegisterWndCls(void);
    unsigned RunMessageLoop(void);

protected:
    DWORD m_dwSleepTime;


public:
    WorkerThread(void);
    virtual ~WorkerThread(void);

    HWND GetMsgWnd(void) const {return m_hMsgWnd;};
    BOOL IsAvailable(void) const {return m_bAvailable;};
    DWORD GetThreadId(void) const {return m_dwThreadId;};
    LONG GetWorkLoad(void) const {return m_nWorkItems;};

    
    BOOL Stop(BOOL bPeaceful = FALSE);
    BOOL Start(void);
    BOOL PostWorkItem(WorkItem* pItem);

    BOOL SetTimer(UINT_PTR nIDEvent, UINT uElapse);
    BOOL KillTimer(UINT_PTR nIDEvent);
    BOOL NotifyTimer(UINT_PTR nIDEvent);
    virtual void OnTimer(UINT_PTR nIDEvent) {;};
    BOOL m_bRunning;
};