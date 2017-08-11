#pragma once
#include "Thread/WorkerThread.h"

class TimerThread : public WorkerThread
{
public:
    TimerThread(void) {m_dwSleepTime = 500;};
    ~TimerThread(void) {};

    static const int CONN_CHECK_INTERVAL = 5;
    static const int CONN_PRINT_INTERVAL = 5;

    static const int CONN_CHECK_TIMER = 1;
    static const int CONN_PRINT_TIMER = 2;

    virtual void OnTimer(UINT_PTR nIDEvent);
};

class AsyncMsgCheckThread : public WorkerThread
{
public:
    AsyncMsgCheckThread(void) {m_dwSleepTime = 500;};
    ~AsyncMsgCheckThread(void) {};

    static const int CHECK_INTERVAL = 1;

    static const int CHECK_TIMER = 1;

    virtual void OnTimer(UINT_PTR nIDEvent);
};
