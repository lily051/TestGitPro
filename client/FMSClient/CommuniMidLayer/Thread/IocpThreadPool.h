/**	@file    IocpThreadPool.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   这里填写本文件的摘要。
 *
 *	@author	 liying
 *	@date	 2012/11/21
 *
 *	@note    下面的note和warning为可选项目
 *	@note    基于Windows IOCP的线程池
 *	@note    历史记录：
 *	@note    
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#pragma once

#include <vector>
#include "WorkItem.h"

using std::vector;

class IocpThreadPool;

class IocpThread
{
private:
    HANDLE m_hThread;   // 工作线程句柄
    DWORD m_dwThreadId; // 工作线程ID
    IocpThreadPool* m_pPool;
    BOOL m_bStopSignaled;   // 线程停止信号

    /**	@fn	    ThreadProc
     *	@brief	静态方法,用于启动线程
     *	@param  [in]pStartup 该工作线程对象指针
     *	@return	线程结束返回值
     */
    static unsigned _stdcall ThreadProc(void * pStartup);

    /**	@fn	    RunLoop
     *	@brief	工作线程主循环,不断获取任务并执行
     *	@return	线程结束返回值
     */
    unsigned RunLoop(void);

public:
    /**	@fn	    IocpThread
     *	@brief	构造函数
     *	@param  [in]pPool
     */
    IocpThread(IocpThreadPool* pPool);

    ~IocpThread(void);
    
    /**	@fn	    GetThreadId
     *	@brief	获取线程ID
     *	@return	返回线程ID
     */
    DWORD GetThreadId(void) const {return m_dwThreadId;};

    /**	@fn	    GetThreadHandle
     *	@brief	获取线程句柄
     *	@return	返回线程句柄
     */
    HANDLE GetThreadHandle(void) const {return m_hThread;};

    /**	@fn	    Start
     *	@brief	开始线程
     *	@return	成功与否
     */
    BOOL Start(void);

    /**	@fn	    SignalStop
     *	@brief	向线程发送停止信号
     *	@return	无
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
    vector<IocpThread*> m_vThreadPool;  // 工作线程
    HANDLE m_hIOCP;                     // IOCP句柄(工作队列),用于投递工作任务
    volatile LONG m_nAboutToPost;       // 当前可能会投递任务的线程数(关闭线程池时用)
    BOOL m_bAvailable;          // 线程池是否可用
    volatile LONG m_nWorkItems;

    static const int MAX_THREADS_NUM = 50;  // 最大线程数
    static const int MIN_THREADS_NUM = 4;   // 最小线程数

    /**	@fn	    InternalPostWorkItem
     *	@brief	投递工作任务,线程池自己使用
     *	@param  [in]pItem 工作任务对象指针
     *	@return	成功与否
     */
    BOOL InternalPostWorkItem(WorkItem* pItem);

    BOOL InternalGetWorkItem(WorkItem** ppItem);

public:

    /**	@fn	    Init
     *	@brief	初始化线程池
     *	@param  [in]nNumOfThreads 工作线程数,默认5,最大50,最小4
     *	@return	成功与否
     */
    BOOL Init(int nNumOfThreads = 5);
    
    /**	@fn	    Fini
     *	@brief	结束线程池
     */
    void Fini(void);

    /**	@fn	    PostWorkItem
    *	@brief	投递工作任务
    *	@param  [in]pItem 工作任务对象指针
    *	@return	成功与否
    */
    BOOL PostWorkItem(WorkItem* pItem);

    LONG PrintWorkLoad(void);
};
