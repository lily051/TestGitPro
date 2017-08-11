#pragma once

#include "Thread/IocpThreadPool.h"

class RspDispatcher : public IocpThreadPool
{
    SINGLETON_DECLARE(RspDispatcher)///<单例相关声明
private:
    RspDispatcher(void);
    virtual ~RspDispatcher(void);
};
