#pragma once

#include "Thread/IocpThreadPool.h"

class PbMsgDispatcher : public IocpThreadPool
{
    SINGLETON_DECLARE(PbMsgDispatcher)///<�����������
private:
    PbMsgDispatcher(void);
    virtual ~PbMsgDispatcher(void);
};
