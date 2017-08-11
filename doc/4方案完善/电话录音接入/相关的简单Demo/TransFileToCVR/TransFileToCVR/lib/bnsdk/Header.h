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
�ص�����˵����
lRealHandle:���������ص�key
pBuffer:������ݵĻ�����ָ��
dwBufSize:�������Ĵ�С
dwUser:�û�����
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


//�طŻص������붨��
#define STOP_BY_ERROR		10001	//���ڴ��������
#define STOP_BY_EXIT		10002   //��������ֹͣ������
#define STOP_BY_END			10003   //��Ƶ������
#define SEND_FAIL           10004   //¼��ʱ��������ʧ��
#define NVR_SUCCESS                  0x11111111 /*�ɹ�*/
#define NVR_VOLUME_USED_FAIL         0x00100000  /*¼�������*/
#define NVR_VOLUME_FULL              0x00200000 /*¼�������*/
#define NVR_ERROR5                   0x04000000 /*: д���̴���*/
#define CONNECT_FAIL                 0x20002000 /*���ӷ�����ʧ��*/
#endif
