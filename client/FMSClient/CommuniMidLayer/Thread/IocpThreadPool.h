/**	@file    IocpThreadPool.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ������д���ļ���ժҪ��
 *
 *	@author	 liying
 *	@date	 2012/11/21
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ����Windows IOCP���̳߳�
 *	@note    ��ʷ��¼��
 *	@note    
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

#pragma once

#include <vector>
#include "WorkItem.h"

using std::vector;

class IocpThreadPool;

class IocpThread
{
private:
    HANDLE m_hThread;   // �����߳̾��
    DWORD m_dwThreadId; // �����߳�ID
    IocpThreadPool* m_pPool;
    BOOL m_bStopSignaled;   // �߳�ֹͣ�ź�

    /**	@fn	    ThreadProc
     *	@brief	��̬����,���������߳�
     *	@param  [in]pStartup �ù����̶߳���ָ��
     *	@return	�߳̽�������ֵ
     */
    static unsigned _stdcall ThreadProc(void * pStartup);

    /**	@fn	    RunLoop
     *	@brief	�����߳���ѭ��,���ϻ�ȡ����ִ��
     *	@return	�߳̽�������ֵ
     */
    unsigned RunLoop(void);

public:
    /**	@fn	    IocpThread
     *	@brief	���캯��
     *	@param  [in]pPool
     */
    IocpThread(IocpThreadPool* pPool);

    ~IocpThread(void);
    
    /**	@fn	    GetThreadId
     *	@brief	��ȡ�߳�ID
     *	@return	�����߳�ID
     */
    DWORD GetThreadId(void) const {return m_dwThreadId;};

    /**	@fn	    GetThreadHandle
     *	@brief	��ȡ�߳̾��
     *	@return	�����߳̾��
     */
    HANDLE GetThreadHandle(void) const {return m_hThread;};

    /**	@fn	    Start
     *	@brief	��ʼ�߳�
     *	@return	�ɹ����
     */
    BOOL Start(void);

    /**	@fn	    SignalStop
     *	@brief	���̷߳���ֹͣ�ź�
     *	@return	��
     */
    void SignalStop(void);
};

class IocpThreadPool
{
    friend class IocpThread;
public:
    IocpThreadPool(void);
    virtual ~IocpThreadPool(void);

private:    
    vector<IocpThread*> m_vThreadPool;  // �����߳�
    HANDLE m_hIOCP;                     // IOCP���(��������),����Ͷ�ݹ�������
    volatile LONG m_nAboutToPost;       // ��ǰ���ܻ�Ͷ��������߳���(�ر��̳߳�ʱ��)
    BOOL m_bAvailable;          // �̳߳��Ƿ����
    volatile LONG m_nWorkItems;

    static const int MAX_THREADS_NUM = 50;  // ����߳���
    static const int MIN_THREADS_NUM = 4;   // ��С�߳���

    /**	@fn	    InternalPostWorkItem
     *	@brief	Ͷ�ݹ�������,�̳߳��Լ�ʹ��
     *	@param  [in]pItem �����������ָ��
     *	@return	�ɹ����
     */
    BOOL InternalPostWorkItem(WorkItem* pItem);

    BOOL InternalGetWorkItem(WorkItem** ppItem);

public:

    /**	@fn	    Init
     *	@brief	��ʼ���̳߳�
     *	@param  [in]nNumOfThreads �����߳���,Ĭ��5,���50,��С4
     *	@return	�ɹ����
     */
    BOOL Init(int nNumOfThreads = 5);
    
    /**	@fn	    Fini
     *	@brief	�����̳߳�
     */
    void Fini(void);

    /**	@fn	    PostWorkItem
    *	@brief	Ͷ�ݹ�������
    *	@param  [in]pItem �����������ָ��
    *	@return	�ɹ����
    */
    BOOL PostWorkItem(WorkItem* pItem);

    LONG PrintWorkLoad(void);
};
