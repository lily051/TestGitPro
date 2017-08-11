#ifndef _HEADER_H
#define _HEADER_H

#ifndef _WIN32
#define NVR_LINUX
#endif

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <signal.h>

#ifdef NVR_LINUX
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/un.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define DWORD unsigned long
#define INT int
#define UINT unsigned int
#define LONG long
#define BYTE unsigned char
#define INVALID_SOCKET ((unsigned int )-1)
#define SOCKET_ERROR -1
#define NULL 0
typedef int SOCKET;
#endif

#ifdef NVR_LINUX
#define NVR_DWORD void*  
#else
#define NVR_DWORD DWORD  
#endif

/*************************
回调函数说明：
lRealHandle:服务器返回的key
pBuffer:存放数据的缓冲区指针
dwBufSize:缓冲区的大小
dwUser:用户数据
************************/
#ifndef NVR_LINUX
typedef void(CALLBACK *GetData_CallBack)(UINT lRealHandle, BYTE *pBuffer, DWORD dwBufSize, NVR_DWORD dwUser);

typedef void(CALLBACK *MsgCallback)(UINT hPlayTask, NVR_DWORD dwUser, INT iErrorCode);

typedef void(CALLBACK *SearchResult_CallBack)(unsigned int loginID,const char *CamerID, unsigned int nBeginTime,unsigned int nEndTime,const char *AlarmType,const char *desc,int desclen,NVR_DWORD dwUser);

typedef void(CALLBACK *GetGroup_CallBack)(int m_rvID,const char *m_rvName,int m_activeDevices,int m_totalDevices,unsigned int  m_gSizeHigh,unsigned int m_gSizeLow,unsigned int  m_UsedHigh,unsigned int m_UsedLow,int m_wdays,int m_dPolicy,NVR_DWORD pUserData);
#else
typedef void(*GetData_CallBack)(UINT lRealHandle, BYTE *pBuffer, DWORD dwBufSize, NVR_DWORD dwUser);

typedef void(*MsgCallback)(UINT hPlayTask, NVR_DWORD dwUser, INT iErrorCode);

typedef void(*SearchResult_CallBack)(unsigned int loginID,const char *CamerID, unsigned int nBeginTime,unsigned int nEndTime,const char *AlarmType,const char *desc,int desclen,NVR_DWORD dwUser);

typedef void(*GetGroup_CallBack)(int m_rvID,const char *m_rvName,int m_activeDevices,int m_totalDevices,unsigned int  m_gSizeHigh,unsigned int m_gSizeLow,unsigned int  m_UsedHigh,unsigned int m_UsedLow,int m_wdays,int m_dPolicy,NVR_DWORD pUserData);
#endif


//回放回调错误码定义
#define STOP_BY_ERROR		10001	//由于错误而结束
#define STOP_BY_EXIT		10002   //由于主动停止而结束
#define STOP_BY_END			10003   //视频流结束
#define SEND_FAIL           10004   //录像时发送数据失败
#define NVR_SUCCESS                  0x11111111 /*成功*/
#define NVR_VOLUME_USED_FAIL         0x00100000  /*录像卷不可用*/
#define NVR_VOLUME_FULL              0x00200000 /*录像卷已满*/
#define NVR_ERROR5                   0x04000000 /*: 写磁盘错误*/
#define CONNECT_FAIL                 0x20002000 /*连接服务器失败*/
#endif
