/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtprtcp.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTPRTCP_DEFINE_H__
#define __RTPRTCP_DEFINE_H__
#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

#define RTPRTCPSESSION_MAXNUM 2000
#define MAX_RTPRTCP_PKT_SIZE 10240			//最大RTPRTCP包长度
#define MAX_RTP_PAYLOAD 9216				//最大PAYLOAD长度
#define MAX_RTP_EXCEPT_PAYLOAD 1024			//最大除了PAYLOAD的长度

#define RTPRTSP_TRANSMODE 0x9000			//RTP over RTSP
#define RTPTCP_TRANSMODE 0x9001				//RTP over TCP
#define RTPUDP_TRANSMODE 0x9002				//RTP over UDP
#define RTPUDP_RELIABLE_TRANSMODE 0x9003	//Reliable RTP over UDP
#define RTPMCAST_TRANSMODE 0x9004			//RTP over multicast

#define NOTNEWBUFFER_PACKMODE 0x9100
#define NEWBUFFER_PACKMODE 0x9101
#define DONOTHING_PACKMODE 0x9102

#define RTPRTCP_MSGOPT_CONNECT 0x9300
#define RTPRTCP_MSGOPT_ACCEPT 0x9301

#define MAX_IP_LEN 23									//最大IP地址长度
#define DEFAULT_RTPTCP_PORT 6010						//默认的RTPTCP侦听端口

#define STREAM_VERSION_20 20							//流媒体系统版本2.0
#define STREAM_VERSION_30 30							//流媒体系统版本3.0

typedef struct rtpbasicparam
{
	HPR_UINT32 ssrc;
	HPR_UINT16 sequence;
	HPR_UINT16 csrccount;
	HPR_UINT32 timestamp;
	HPR_INT32 marker;
	HPR_INT32 payload;
	HPR_INT32 extension;
	HPR_INT32 cycle;
} RTPBASICPARAM, *PRTPBASICPARAM;

typedef struct rtprtcp_globalconfig
{
} RTPRTCP_GLOBALCONFIG, *PRTPRTCP_GLOBALCONFIG;

typedef struct rtprtcp_sessionconfig
{
} RTPRTCP_SESSIONCONFIG, *PRTPRTCP_SESSIONCONFIG;

typedef struct tcpparam
{
	bool bistoconnect;
	HPR_INT32 isessionid;
} TCPPARAM, *PTCPPARAM, RTPTCPPARAM, *PRTPTCPPARAM;

typedef struct mcastparam
{
	bool bistojoin;
} MCASTPARAM, *PMCASTPARAM, RTPMCASTPARAM, *PRTPMCASTPARAM;

#define HPR_INVALID_RTPRTCPSESSION -1
#define HPR_RTPRTCP_HANDLE HPR_INT32
typedef HPR_INT32(CALLBACK *pRTPRTCPMsgFunc)(HPR_INT32 sessionhandle, HPR_VOIDPTR userdata, HPR_INT32 opt,\
		HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);
typedef HPR_INT32(CALLBACK *pRTPRTCPDataFunc)(HPR_INT32 sessionhandle, HPR_VOIDPTR userdata, HPR_VOIDPTR payload,\
		HPR_INT32 payloadlen, HPR_VOIDPTR pdata, HPR_INT32 datalen);

#if defined(OS_WINDOWS)	
	#if defined(RTPRTCP_EXPORTS)
		#define HPR_RTPRTCP_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_RTPRTCP_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_RTPRTCP_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

#endif