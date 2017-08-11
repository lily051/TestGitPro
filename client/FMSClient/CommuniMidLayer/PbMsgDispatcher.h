#pragma once

#include "Thread/IocpThreadPool.h"

class PbMsgDispatcher : public IocpThreadPool
{
    SINGLETON_DECLARE(PbMsgDispatcher)///<单例相关声明
private:
    PbMsgDispatcher(void);
    virtual ~PbMsgDispatcher(void);
};
