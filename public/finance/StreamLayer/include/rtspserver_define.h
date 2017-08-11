/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspclient_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTSPSERVER_DEFINE_H__
#define __RTSPSERVER_DEFINE_H__
#include "HPR_Addr.h"
#include "rtspgeneral_define.h"
#include "FileLayer_define.h"

#define SOURCE_ASYNC_PROCESS
//#define SUPPORT_VTDU

#define RTSPSESSION_SERVER_MAXNUM 2000
#define RTSPSERVER_MSG_DESCRIBE 0x0101					//RTSP服务端DESCRIBE消息通知
#define RTSPSERVER_MSG_SETUP 0x0111						//RTSP服务端SETUP消息通知
#define RTSPSERVER_MSG_PLAY 0x0121						//RTSP服务端PLAY消息通知
#define RTSPSERVER_MSG_PAUSE 0x0131						//RTSP服务端PAUSE消息通知
#define RTSPSERVER_MSG_TEARDOWN 0x0141					//RTSP服务端TEARDOWN消息通知
#define RTSPSERVER_MSG_OPTIONS 0x0151					//RTSP服务端OPTIONS消息通知
#define RTSPSERVER_MSG_HEARTBEAT 0x0161					//RTSP服务端HEARTBEAT消息通知
#define RTSPSERVER_MSG_HEARTBEAT_TIMEOUT 0x0162			//RTSP服务端HEARTBEAT TIMEOUT消息通知
#define RTSPSERVER_MSG_EXIT 0x0191						//RTSP服务端EXIT消息通知
#define RTSPSERVER_MSG_PTZCONTROL 0x0181                //RTSP服务器PTZCONTROL消息通知
#define RTSPSERVER_MSG_FORCEIFRAME 0x0182               //RTSP服务器FORCEIFRAME消息通知 
#define RTSPSERVER_MSG_FORCEQUIT 0x300

//for tetst audio
#define RTSPSERVER_MSG_VOICE 0x0183                     //audio data msg
 
#ifdef SUPPORT_VTDU
#define RTSPSERVER_MSG_VTMCONNECTRSP 0x0201             //RTSP服务器CONNECT消息通知
#define RTSPSERVER_MSG_CUTIMEOUT 0x0211                 //CU TIMEOUT
#endif

typedef struct rtspserver_globalconfig
{
} RTSPSERVER_GLOBALCONFIG, *PRTSPSERVER_GLOBALCONFIG;

typedef struct rtspserver_sessionconfig
{
	HPR_INT32 TransMethod;				//传输方式 0-RTP/RTSP 1-RTP/RTCP 2-RTP/UDP 3-RTP/multicast
	HPR_INT32 userdata;					//用户自定义数据
	char *useragent;					//客户端名称
	char *url;							//请求URL
	char *auth;							//认证信息
} RTSPSERVER_SESSIONCONFIG, *PRTSPSERVER_SESSIONCONFIG;

typedef struct RTSPServer_DescribeInfo
{
	//输入
	HPR_ADDR_T clientaddr;
	char *url;								//客户端的请求URL
	char *auth;								//认证信息
	char **acceptpresentation;				//可接受的presentation类型的列表
	char *serveragent;						//server的名称
	char *sessionid;						//会话ID
	HPR_INT32 presentationcount;			//presentation类型的个数
	bool bstandardstream;					//表示是否是标准码流
	bool bdevicepushdata;					//表示是否由设备主动推送码流
	//输出
	char *baseurl;							//基础URL
	HPR_BOOL bdownload;                     //表示是否下载
#ifdef SOURCE_ASYNC_PROCESS
	int seq;
	void *callclass;
#endif
} RTSPSERVER_DESCRIBEINFO, *PRTSPSERVER_DESCRIBEINFO;

typedef struct RTSPServer_SetupInfo
{
	HPR_ADDR_T clientaddr;
	char *url;								//客户端的请求URL
	char *auth;								//认证信息
	char *useragent;						//客户端名称
	HPR_INT32 transmethod;					//传输方式
	HPR_UINT16 devicestartport;				//设备主动推送的起始目的端口
	HPR_UINT16 deviceendport;				//设备主动推送的结束目的端口
	//兼容老版本的流媒体
	HPR_INT64 filesize;					//文件大小
	HPR_INT32 totaltime;				//最大播放时间
	//////////////////////////////////////////////////
	HPR_INT32 sessionhandle;
	HPR_BOOL  bStream20;                  //是否是2.0版本
#ifdef SOURCE_ASYNC_PROCESS
	int seq;
	void *callclass;
#endif
} RTSPSERVER_SETUPINFO, *PRTSPSERVER_SETUPINFO;

typedef struct RTSPServer_HeadInfo
{
	HPR_UINT8 headbuf[MAX_HEAD_LEN];
	HPR_UINT32 headbuflen;
	HPR_UINT32 headlen;
} RTSPSERVER_HEADINFO, *PRTSPSERVER_HEADINFO;

typedef struct RTSPServer_PlayInfo
{
	HPR_INT64 starttime;  //相对时间
	HPR_INT64 endtime;
	HPR_VOID  *abs_starttime;  //绝对时间
	HPR_VOID  *abs_endtime;
	bool      babsplay;       //是否是按绝对时间播放
	bool bvalidscale;
	HPR_INT32 iscale;
	HPR_BOOL bBackward;      //是否是倒放
#ifdef SOURCE_ASYNC_PROCESS
	int seq;
	void *callclass;
#endif
} RTSPSERVER_PLAYINFO, *PRTSPSERVER_PLAYINFO;

struct ServerPauseInfo
{
};

struct ServerTeardownInfo
{
};

typedef struct RTSPServer_ExitInfo
{
#ifdef SOURCE_ASYNC_PROCESS
	void *callclass;
#endif
} RTSPSERVER_EXITINFO, *PRTSPSERVER_EXITINFO;

typedef struct serverclientinfo
{
	char clientip[MAX_IP_LEN];			//客户端的IP地址
	HPR_UINT16 clientport;				//客户端的端口
	char url[MAX_URL_LEN];				//客户端请求的URL
}RTSPSERVER_CLIENTINFO, *PRTSPSERVER_CLIENTINFO;

typedef struct _PTZ_CONTROL_
{
	HPR_UINT32 ucommand;
	HPR_INT32  iparam1;
	HPR_INT32  iparam2;
	HPR_INT32  iparam3;
	HPR_INT32  iparam4;
}stptz_control, *pstptz_control;

typedef struct RTSPServer_PTZControlInfo
{
	HPR_INT32 ibri;
	HPR_INT32 icon;
	HPR_INT32 isat;
	HPR_INT32 ihue;
	HPR_INT32 imethods;
	stptz_control stptz;
#ifdef SOURCE_ASYNC_PROCESS
	int seq;
	void *callclass;
#endif
}RTSPSERVER_PTZCONTROLINFO, *PRTSPSERVER_PTZCONTROLINFO;

class IMediaSource
{
public:
	virtual HPR_INT32 GetPacket(char* buf, HPR_INT32 buflen) = 0;					//获取流数据
	virtual HPR_INT32 SeekByAbsTime(ABSTIME *time) = 0;
	virtual HPR_INT32 SeekByRelTime(HPR_UINT64 time) = 0;
	virtual HPR_INT32 SeekByPos(HPR_UINT64 pos) = 0;
};

#define HPR_RTSPSERVER_HANDLE HPR_INT32
#define HPR_INVALID_RTSPSERVERSESSION -1
typedef HPR_INT32(CALLBACK *pRTSPServerMsgFunc)(HPR_INT32 sessionhandle, HPR_INT32 opt, HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);
typedef HPR_INT32(CALLBACK *pRTSPServer_Reverse_Msg)(HPR_INT32 sessionhandle, HPR_INT32 opt, HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);

#if defined(OS_WINDOWS)
	#if defined(RTSPSERVER_EXPORTS)
		#define HPR_RTSPSERVER_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_RTSPSERVER_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_RTSPSERVER_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

#define RTSPSERVER_FORTEST

#endif