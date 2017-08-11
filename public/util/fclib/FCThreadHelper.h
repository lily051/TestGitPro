#ifndef GUARD_FCTHREADHELPER_H
#define GUARD_FCTHREADHELPER_H

/**	@file    FCThreadHelper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   与线程相关的函数
 *
 *	@author	 xuezhihu
 *	@date	 2012/4/16
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V2.0.0  添加了一个导出接口
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#ifndef _INC_PROCESS
#include <process.h>
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

namespace fclib {
    namespace fcthread {

typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
    pvParam, fdwCreate, pdwThreadId)                 \
    ((HANDLE)_beginthreadex(                      \
    (void *)        (psa),                     \
    (unsigned)      (cbStack),                 \
    (PTHREAD_START) (pfnStartAddr),            \
    (void *)        (pvParam),                 \
    (unsigned)      (fdwCreate),               \
    (unsigned*)    (pdwThreadId)))

#define fcCreateThread chBEGINTHREADEX

HANDLE fcFancyCreateThread(LPTHREAD_START_ROUTINE lpStartAddr, 
                           LPVOID lpParameter, bool bSuspend = false)
{
    return (HANDLE)_beginthreadex(NULL, 0, (PTHREAD_START)lpStartAddr, lpParameter,
        bSuspend ? (CREATE_SUSPENDED):(0), NULL);
}
void fcWaitAndCloseThread(HANDLE& hThread)
{
    WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);
    hThread = NULL;
}
    
    
    
} // end for namespace fcthread
} // end for namespace fclib

#endif // end of GUARD_FCTHREADHELPER_H
