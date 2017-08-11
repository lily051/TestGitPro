/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspclient_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTSPCLIENT_DEFINE_H__
#define __RTSPCLIENT_DEFINE_H__
#include "rtspgeneral_define.h"

#include "ConfigFunc.h"

#define RTSPSESSION_CLIENT_MAXNUM 1000					//最多客户端个数
#define RTSPCLIENT_MSG_DESCRIBE 0x0001					//RTSP客户端DESCRIBE消息通知
#define RTSPCLIENT_MSG_SETUP 0x0011						//RTSP客户端SETUP消息通知
#define RTSPCLIENT_MSG_PLAY 0x0021						//RTSP客户端PLAY消息通知
#define RTSPCLIENT_MSG_PAUSE 0x0031						//RTSP客户端PAUSE消息通知
#define RTSPCLIENT_MSG_TEARDOWN 0x0041					//RTSP客户端TEARDOWN消息通知
#define RTSPCLIENT_MSG_OPTIONS 0x0051					//RTSP客户端OPTIONS消息通知
#define RTSPCLIENT_MSG_HEARTBEAT 0x0061					//RTSP客户端HEARTBEAT消息通知
#define RTSPCLIENT_MSG_HEARTBEAT_TIMEOUT 0x0062			//RTSP客户端HEARTBEAT TIMEOUT消息通知
#define RTSPCLIENT_MSG_EXIT 0x0091						//RTSP客户端EXIT消息通知

typedef struct rtspclient_globalconfig
{
	int heartbeattype;						//心跳方式 0-用RTSP扩展命令实现 1-用//SET_PARAMTER命令来实现
} RTSPCLIENT_GLOBALCONFIG, *PRTSPCLIENT_GLOBALCONFIG;

typedef struct rtspclient_sessionconfig
{
	int TransMethod;						//传输方式 0-RTP/RTSP 1-RTP/RTCP 2-RTP/UDP 3-RTP/multicast
	char *useragent;						//客户端名称
	char *url;								//请求URL
	char *auth;								//认证信息
} RTSPCLIENT_SESSIONCONFIG, *PRTSPCLIENT_SESSIONCONFIG;

typedef struct rtspclient_describeparam
{
	char **acceptpresentation;				//可接受的presentation类型的列表
	int presentationcount;					//presentation类型的个数
	bool bstandardstream;					//表示是否是标准码流
	bool bdevicepushdata;					//表示是否由设备主动推送码流
	bool bdownload;                         //表示是否进行下载
} RTSPCLIENT_DESCRIBEPARAM, *PRTSPCLIENT_DESCRIBEPARAM;

typedef struct rtspclient_setupparam
{
	char *url;								//请求的URL
#ifdef SUPPORT_DB33
	bool bDB33;
	HPR_UINT16 port;
#endif
} RTSPCLIENT_SETUPPARAM, *PRTSPCLIENT_SETUPPARAM;

typedef struct rtspclient_playparam
{
	bool bhasplayrange;						//是否带有Range头部
	int timetype;							//时间类型
	char *starttime;						//开始时间
	char *endtime;							//结束时间
	char *playtime;							//开始播放时间
	bool bhasplayscale;						//是否带有Scale头部
	int scale;								//播放倍率
	bool bBackward;                         //表示倒放
} RTSPCLIENT_PLAYPARAM, *PRTSPCLIENT_PLAYPARAM;

//云台控制
typedef struct ptzparam
{
	HPR_UINT32 command;
	HPR_INT32  param1;
	HPR_INT32  param2;
	HPR_INT32  param3;
	HPR_INT32  param4;
}stptzparam, *pstptzparam;

typedef struct rtspclient_ptzparam
{
	HPR_INT32 ibri;   //亮度
	HPR_INT32 icon;   //对比度
	HPR_INT32 isat;
	HPR_INT32 ihue;
	HPR_INT32 imethods;
	stptzparam ptz_param;
}RTSPCLIENT_PTZPARAM, *PRTSPCLIENT_PTZPARAM;



typedef struct rtspclient_pauseparam
{
	bool bhaspausepoint;					//是否带有暂停点
	float pausepoint;						//暂停点
} RTSPCLIENT_PAUSEPARAM, *PRTSPCLIENT_PAUSEPARAM;

typedef struct rtspclient_teardownparam
{
} RTSPCLIENT_TEARDOWNPARAM, *PRTSPCLIENT_TEARDOWNPARAM;

struct ClientPTZControl
{
	HPR_INT32 ibri;
	HPR_INT32 icon;
	HPR_INT32 isat;
	HPR_INT32 ihue;
	HPR_BOOL  bSet;
};

struct ClientDescribeInfo
{
	char *presentation;						//presention 会话描述
	char *presentationtype;					//presentation类型
	char *requesturl;						//请求URL
	char *baseurl;							//基础URL
};

struct ClientSetupInfo
{
};

struct ClientPlayInfo
{
	int scale;
};

struct ClientPauseInfo
{
};

struct ClientTeardownInfo
{
};

typedef struct rtspclient_mediainfo
{
	int rtpsessionhandle;
	char mediaurl[MAX_URL_LEN + 1];
	unsigned short clientport;
} RTSPCLIENT_MEDIAINFO, *PRTSPCLIENT_MEDIAINFO;

#define HPR_RTSPCLIENT_HANDLE HPR_INT32
#define HPR_INVALID_RTSPCLIENTSESSION -1
typedef HPR_INT32 (CALLBACK *pRTSPClientMsgFunc)(HPR_RTSPCLIENT_HANDLE sessionhandle, HPR_VOIDPTR userdata, HPR_INT32 opt, HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);
typedef HPR_INT32 (CALLBACK *pRTSPClientDataFunc)(HPR_RTSPCLIENT_HANDLE sessionhandle, HPR_VOIDPTR userdata, HPR_INT32 datatype, HPR_VOIDPTR pdata, HPR_INT32 datalen);

#if defined(OS_WINDOWS)
	#if defined(RTSPCLIENT_EXPORTS)
		#define HPR_RTSPCLIENT_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_RTSPCLIENT_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_RTSPCLIENT_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

#endif