#pragma once

#include "Thread/IocpThreadPool.h"

class RspDispatcher : public IocpThreadPool
{
    SINGLETON_DECLARE(RspDispatcher)///<�����������
private:
    RspDispatcher(void);
    virtual ~RspDispatcher(void);
};
