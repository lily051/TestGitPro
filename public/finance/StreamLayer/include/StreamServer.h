/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: StreamServer.h
Description: API�������� 	 
Author: guanguochen       
Date: 2009.03.20
*****************************************************/
#ifndef __STREAMSERVER_H__
#define __STREAMSERVER_H__

#include "streamserver_define.h"
#include "ConfigFunc.h"

#ifdef SUPPORT_CASCADE
typedef HPR_INT32 (CALLBACK *CascadeCB)(char* pZone_id, char* pCamera_id, HPR_UINT32 iType, char* purl, HPR_VOIDPTR userdata);
#endif

typedef HPR_INT32(CALLBACK *CBF_NotifyANewConnection)(bool badd, const char *clientip, HPR_UINT16 port, const char *url);//bAdd false��ʾ�Ͽ�һClient���� ��ʱURLΪNULL��true��ʾ����һ��Client

HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_InitLib(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_FiniLib(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_Start(pStreamServerMsgFunc pMsgFunc, HPR_UINT16 port = 554);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_Stop(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetRTPTCPPort(HPR_UINT16 baseport);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetRTPUDPPortRange(HPR_UINT16 count, HPR_UINT16 baseport);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_GetSessionCount(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_GetChannelCount(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_CreateIndex(char *filename);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetRTPMcastAddr(char *mcastaddr);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_AddRelativePath(char *path);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_RemoveRelativePath(HPR_INT32 index);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_ClearRelativePath(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_GetFirstRelativePath(char *buf, HPR_INT32 len, HPR_INT32 *index);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_GetNextRelativePath(char *buf, HPR_INT32 len, HPR_INT32 *index);

HPR_STREAMSERVER_DECLARE HPR_UINT32 CALLBACK StreamServer_GetCurUpNetPerSec(HPR_VOID);

HPR_STREAMSERVER_DECLARE HPR_UINT32 CALLBACK StreamServer_GetCurDownNetPerSec(HPR_VOID);

//add by hdj
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_KickClientByIPAndPort(char* ip, HPR_UINT16 port);

//�����ϰ汾����ý��
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK InitStreamServerLib(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK FiniStreamServerLib(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StartServer(const char* path, HPR_UINT16 port = 554);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK RunServer(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK RunServerLoop(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StopServer(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK GetServerState(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK GetCurConnection(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK GetCurChannel(HPR_VOID);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK VodCreateIndex(char* filename, HPR_INT32 len);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK SetNewConnectionCallBack(CBF_NotifyANewConnection callback);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK GetTransCount(HPR_VOID);

//for test
//HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK GetMutexCounts();

#ifdef SUPPORT_VTDU
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetVTMServer( char* ip, HPR_UINT16 port);
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetIndexCode( char* vtdu_indexcode, char* vtm_indexcode);
#endif

#ifdef SUPPORT_CASCADE
HPR_STREAMSERVER_DECLARE HPR_INT32 CALLBACK StreamServer_SetCascadeCallBack(CascadeCB pCallBack, HPR_VOIDPTR pUserData);
#endif

#endif

/**************************/
/*��Ϣ�ص����������뼰��������*/
/************************************************************************************************
1	��	�ͻ�����Ϣ			(param1: �ͻ���IP��ַ, param2: �ͻ��˶˿�, param3: ˽����֤��Ϣ, param4: ��Ч)
�ɹ����� 0��ʧ�ܷ��� -1
********************************************************************************************************/





