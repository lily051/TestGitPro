/**
*   @file MsgQueue.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief ��Ϣ���еĶ���
*   
*   @author mazhentong
*   @date   2012/08/16
*
*   @note   ��NVDServer��ʵ�ֵ���Ϣ����ֱ��ʵ�ֵ�
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
 *  @brief ���յ���theService���͵���Ϣ����
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

// CMsgQueue�Ĺ��캯��
inline CMsgQueue::CMsgQueue()
:m_nElments(0)
{
    InitializeCriticalSectionAndSpinCount(&m_csLock, 4 * 1000);
}

// CMsgQueue����������
inline CMsgQueue::~CMsgQueue(void)
{
    DeleteCriticalSection(&m_csLock);
}

/** @class CCtrlMsgQueue
 *  @brief ���յ�������Ѳ�췢�͵Ŀ�����Ϣ����
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

// CCtrlMsgQueue��Ĺ��캯��
inline CCtrlMsgQueue::CCtrlMsgQueue()
:m_nElments(0)
{
    InitializeCriticalSectionAndSpinCount(&m_cCtrlLock, 4 * 1000);
}

// CCtrlMsgQueue�����������
inline CCtrlMsgQueue::~CCtrlMsgQueue(void)
{
    DeleteCriticalSection(&m_cCtrlLock);
}
