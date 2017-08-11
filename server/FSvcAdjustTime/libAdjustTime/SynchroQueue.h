#ifndef GUARD_CSYNCHROQUEUE_H
#define GUARD_CSYNCHROQUEUE_H
#pragma once

/**	@file    SynchroQueue.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ��Ϣ����ͬ����
 *
 *	@author	 yuchengzhi
 *	@date	 2012/8/22
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V1.0.0  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

#include <list>
#include <queue>
#include "../util/Guard.h"

template<typename T>
class CSynchroQueue
{
public:
    
    /**	@fn	    CSynchroQueue<T>::CSynchroQueue
     *	@brief	Consturctor
     *	@param  void
     *	@return	
     */
    CSynchroQueue(void) 
    {
        //InitializeCriticalSection(&m_criticalsec);
		InitializeCriticalSectionAndSpinCount(&m_criticalsec, 4 * 1000);
    }
    
    /**	@fn	    CSynchroQueue<T>::~CSynchroQueue
     *	@brief	Desturctor
     *	@param  void
     *	@return	
     */
    ~CSynchroQueue(void) 
    {
        DeleteCriticalSection(&m_criticalsec);
    }
private:
    std::list<T> m_csQueue;          ///< ��������
    CRITICAL_SECTION m_criticalsec;  ///< ���б�����
public:
    typedef typename std::queue<T>::size_type size_type;  ///< size_type is a datatype like int

    /**	@fn	    CSynchroQueue<T>::Push
     *	@brief	add a T object to the tail
     *	@param  const T & value
     *	@return	BOOL
     */
    BOOL Push(const T& value)
    {
        CGuard guard(&m_criticalsec);
        m_csQueue.push_back(value);
        return TRUE;
    }

    /**	@fn	    CSynchroQueue<T>::UniquePush
     *	@brief	add a T object to the tail without duplication
     *	@param  const T & value
     *	@return	BOOL
     */
    BOOL UniquePush(const T& value)
    {
        CGuard guard(&m_criticalsec);
        typedef std::list<T>::iterator Iter;
        Iter result = std::find(m_csQueue.begin(), m_csQueue.end(), value);
        if (result == m_csQueue.end())    //�ж��Ƿ��ظ�
        {
            m_csQueue.push_back(value);
        }
        return TRUE;
    }
    
    /**	@fn	    CSynchroQueue<T>::Pop
     *	@brief	Remove a T object from the head,and return it
     *	@param  T & value
     *	@return	FALSE--the queue is null, TRUE--succeed
     */
    BOOL Pop(T& value)
    {
        CGuard guard(&m_criticalsec);
        if(!m_csQueue.empty())
        {
            value = m_csQueue.front();
            m_csQueue.pop_front();
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

	///** @fn       void CMsgQueue::RemoveAll(void)
	//*  @brief    �Ƴ�CMsgQueue���е�����Ԫ��
	//*  @param    void
	//*  @return:  void
	//*/
	//void RemoveAll(void)
	//{
	//	CGuard guard(&m_criticalsec);
	//	if (m_csQueue.size() <= 0)
	//	{
	//		return;
	//	}

	//	m_csQueue.clear();
	//}

    /**	@fn	    CSynchroQueue<T>::size
     *	@brief	reture the size of the queue
     *	@return	size_type the size
     */
    size_type size()
    {
        CGuard guard(&m_criticalsec);
        return m_csQueue.size();
    }
};

#endif    // end GUARD_CSYNCHROQUEUE_H
