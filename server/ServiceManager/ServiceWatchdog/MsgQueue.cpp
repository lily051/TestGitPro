/**
*   @file MsgQueue.cpp
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 消息队列的函数定义
*   
*   @author mazhentong
*   @date   2012/08/16
*
*   @note   由NVDServer中实现的消息队列直接来实现的
*	
*   @warning  m_csLock的加锁和解锁由CGuard来完成
*/

#include "StdAfx.h"
#include ".\msgqueue.h"
#include "MsgQueue.h"

using namespace std;

/////////////////////////////////////////////////////////////
// CMsgQueue队列

/** @fn       BOOL CMsgQueue::Append(BASIC_MESSAGE_EX * pMsgElem)
 *  @brief    CMsgQueue的元素入队操作
 *  @param    (BASIC_MESSAGE_EX*) pMsgElem [IN] : 
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CMsgQueue::Append(BASIC_MESSAGE_EX * pMsgElem)
{
    CGuard guard(&m_csLock);

    if (m_nElments >= MSG_QUEUE_SIZE)
    {
        return FALSE; // the queue is full
    }

    // push an element

    m_listElem.push_back(*pMsgElem);

    ++m_nElments;

    return TRUE;
}

/** @fn       BOOL CMsgQueue::Remove(BASIC_MESSAGE_EX * pMsgElem)
 *  @brief    CMsgQueue队列的出队操作
 *  @param    (BASIC_MESSAGE_EX*) pMsgElem [OUT] : 
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CMsgQueue::Remove(BASIC_MESSAGE_EX * pMsgElem)
{
    CGuard guard(&m_csLock);

    if (m_nElments <= 0)
    {
        return FALSE;	// the queue is empty
    }

    // pull an element from

    *pMsgElem = m_listElem.front();
    m_listElem.pop_front();

    -- m_nElments;

    return TRUE;
}

/** @fn       void CMsgQueue::RemoveAll(void)
 *  @brief    移除CMsgQueue队列的所有元素
 *  @param    void
 *  @return:  void
 */
void CMsgQueue::RemoveAll(void)
{
    CGuard guard(&m_csLock);
    if (m_nElments <= 0)
    {
        return;
    }

    m_listElem.clear();
    m_nElments = 0;
}

/////////////////////////////////////////////////////////////
// CCtrlMsgQueue队列

/** @fn       BOOL CCtrlMsgQueue::Append(const RECV_CMD_MSG& recvCmdMsg)
 *  @brief    CCtrlMsgQueue
 *  @param    (const RECV_CMD_MSG&) recvCmdMsg [IN] : 
 *  @return:  BOOL：TRUE: ; FALSE:
 */
BOOL CCtrlMsgQueue::Append(const RECV_CMD_MSG& recvCmdMsg)
{
    CGuard guard(&m_cCtrlLock);

    if (m_nElments >= MSG_QUEUE_SIZE)
    {
        return FALSE; // the queue is full
    }

    m_listElem.push_back(recvCmdMsg);

    ++m_nElments;

    return TRUE;
}

/** @fn       BOOL CCtrlMsgQueue::Remove(RECV_CMD_MSG& recvCmdMsg)
 *  @brief    CCtrlMsgQueue
 *  @param    (RECV_CMD_MSG&) recvMsgQueue [OUT] : 
 *  @return:  BOOL：TRUE: ; FALSE:
 */
BOOL CCtrlMsgQueue::Remove(RECV_CMD_MSG& recvCmdMsg)
{
    CGuard guard(&m_cCtrlLock);

    if (m_nElments <= 0)
    {
        return FALSE;	// the queue is empty
    }

    // pull an element from
    recvCmdMsg = m_listElem.front();
    m_listElem.pop_front();

    -- m_nElments;

    return TRUE;
}

/** @fn       void CCtrlMsgQueue::RemoveAll(void)
 *  @brief    移除CCtrlMsgQueue队列的所有元素
 *  @param    void
 *  @return:  void
 */
void CCtrlMsgQueue::RemoveAll(void)
{
    CGuard guard(&m_cCtrlLock);
    if (m_nElments <= 0)
    {
        return;
    }

    m_listElem.clear();
    m_nElments = 0;
}
