/**
*   @file MsgQueue.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 消息队列的定义
*   
*   @author mazhentong
*   @date   2012/08/16
*
*   @note   由NVDServer中实现的消息队列直接实现的
*	
*   @warning
*/

#pragma once

#include <afxtempl.h>

#include <list>

#include "Guard.h"
#include "GenDef.h"
using namespace std;

/** @class CMsgQueue
 *  @brief 接收到的theService发送的消息队列
 */
class CMsgQueue
{
public:
    CMsgQueue();
    ~CMsgQueue(void);

    BOOL Append(BASIC_MESSAGE_EX * pMsgElem);
    BOOL Remove(BASIC_MESSAGE_EX * pMsgElem);

    void RemoveAll(void);

protected:
    std::list <BASIC_MESSAGE_EX> m_listElem;
    int m_nElments;
    CRITICAL_SECTION m_csLock;
};

// CMsgQueue的构造函数
inline CMsgQueue::CMsgQueue()
:m_nElments(0)
{
    InitializeCriticalSectionAndSpinCount(&m_csLock, 4 * 1000);
}

// CMsgQueue的析构函数
inline CMsgQueue::~CMsgQueue(void)
{
    DeleteCriticalSection(&m_csLock);
}

/** @class CCtrlMsgQueue
 *  @brief 接收到的智能巡检发送的控制消息对列
 */
class CCtrlMsgQueue
{
public:
    CCtrlMsgQueue();
    ~CCtrlMsgQueue();

    BOOL Append(const RECV_CMD_MSG& recvCmdMsg);
    BOOL Remove(RECV_CMD_MSG& recvCmdMsg);

    void RemoveAll(void);
private:
    std::list <RECV_CMD_MSG> m_listElem;
    int m_nElments;
    CRITICAL_SECTION m_cCtrlLock;
};

// CCtrlMsgQueue类的构造函数
inline CCtrlMsgQueue::CCtrlMsgQueue()
:m_nElments(0)
{
    InitializeCriticalSectionAndSpinCount(&m_cCtrlLock, 4 * 1000);
}

// CCtrlMsgQueue类的析构函数
inline CCtrlMsgQueue::~CCtrlMsgQueue(void)
{
    DeleteCriticalSection(&m_cCtrlLock);
}
