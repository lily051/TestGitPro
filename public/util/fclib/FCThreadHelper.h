#ifndef GUARD_FCTHREADHELPER_H
#define GUARD_FCTHREADHELPER_H

/**	@file    FCThreadHelper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ���߳���صĺ���
 *
 *	@author	 xuezhihu
 *	@date	 2012/4/16
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V2.0.0  �����һ�������ӿ�
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
