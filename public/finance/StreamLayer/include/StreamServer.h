/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: StreamServer.h
Description: API函数定义 	 
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

typedef HPR_INT32(CALLBACK *CBF_NotifyANewConnection)(bool badd, const char *clientip, HPR_UINT16 port, const char *url);//bAdd false表示断开一Client连接 此时URL为NULL，true表示增加一个Client

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

//兼容老版本的流媒体
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
/*消息回调函数命令码及参数定义*/
/************************************************************************************************
1	－	客户端信息			(param1: 客户端IP地址, param2: 客户端端口, param3: 私有认证信息, param4: 无效)
成功返回 0，失败返回 -1
********************************************************************************************************/





