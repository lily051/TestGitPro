#include "StdAfx.h"
#include "TimerThread.h"
#include "ConnSessionMgr.h"
#include "NetComm.h"
#include <type/cmd.pb.h>
#include <msg/MsgAction.pb.h>
#include "PbMsgDispatcher.h"
#include "RspDispatcher.h"
#include "PbWrapWorkItem.h"

void TimerThread::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
        // 检查连接,重连
    case CONN_CHECK_TIMER:
        {
            ConnSessionMgr::SnglPtr()->MaintainConnSessions(&m_bRunning);
        }
        break;
    case CONN_PRINT_TIMER:
        {
            ConnSessionMgr::SnglPtr()->PrintHppConns();
        }
        break;
    default:
        break;
    }
}

void AsyncMsgCheckThread::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case CHECK_TIMER:
        {
            CNetComm::SnglPtr()->CheckTimeoutMsg();
        }
        break;
    default:
        break;
    }
}