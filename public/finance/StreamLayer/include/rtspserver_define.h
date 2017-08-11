/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspclient_define.h
Description: �ṹ�ͺ궨��  	 
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
#define RTSPSERVER_MSG_DESCRIBE 0x0101					//RTSP�����DESCRIBE��Ϣ֪ͨ
#define RTSPSERVER_MSG_SETUP 0x0111						//RTSP�����SETUP��Ϣ֪ͨ
#define RTSPSERVER_MSG_PLAY 0x0121						//RTSP�����PLAY��Ϣ֪ͨ
#define RTSPSERVER_MSG_PAUSE 0x0131						//RTSP�����PAUSE��Ϣ֪ͨ
#define RTSPSERVER_MSG_TEARDOWN 0x0141					//RTSP�����TEARDOWN��Ϣ֪ͨ
#define RTSPSERVER_MSG_OPTIONS 0x0151					//RTSP�����OPTIONS��Ϣ֪ͨ
#define RTSPSERVER_MSG_HEARTBEAT 0x0161					//RTSP�����HEARTBEAT��Ϣ֪ͨ
#define RTSPSERVER_MSG_HEARTBEAT_TIMEOUT 0x0162			//RTSP�����HEARTBEAT TIMEOUT��Ϣ֪ͨ
#define RTSPSERVER_MSG_EXIT 0x0191						//RTSP�����EXIT��Ϣ֪ͨ
#define RTSPSERVER_MSG_PTZCONTROL 0x0181                //RTSP������PTZCONTROL��Ϣ֪ͨ
#define RTSPSERVER_MSG_FORCEIFRAME 0x0182               //RTSP������FORCEIFRAME��Ϣ֪ͨ 
#define RTSPSERVER_MSG_FORCEQUIT 0x300

//for tetst audio
#define RTSPSERVER_MSG_VOICE 0x0183                     //audio data msg
 
#ifdef SUPPORT_VTDU
#define RTSPSERVER_MSG_VTMCONNECTRSP 0x0201             //RTSP������CONNECT��Ϣ֪ͨ
#define RTSPSERVER_MSG_CUTIMEOUT 0x0211                 //CU TIMEOUT
#endif

typedef struct rtspserver_globalconfig
{
} RTSPSERVER_GLOBALCONFIG, *PRTSPSERVER_GLOBALCONFIG;

typedef struct rtspserver_sessionconfig
{
	HPR_INT32 TransMethod;				//���䷽ʽ 0-RTP/RTSP 1-RTP/RTCP 2-RTP/UDP 3-RTP/multicast
	HPR_INT32 userdata;					//�û��Զ�������
	char *useragent;					//�ͻ�������
	char *url;							//����URL
	char *auth;							//��֤��Ϣ
} RTSPSERVER_SESSIONCONFIG, *PRTSPSERVER_SESSIONCONFIG;

typedef struct RTSPServer_DescribeInfo
{
	//����
	HPR_ADDR_T clientaddr;
	char *url;								//�ͻ��˵�����URL
	char *auth;								//��֤��Ϣ
	char **acceptpresentation;				//�ɽ��ܵ�presentation���͵��б�
	char *serveragent;						//server������
	char *sessionid;						//�ỰID
	HPR_INT32 presentationcount;			//presentation���͵ĸ���
	bool bstandardstream;					//��ʾ�Ƿ��Ǳ�׼����
	bool bdevicepushdata;					//��ʾ�Ƿ����豸������������
	//���
	char *baseurl;							//����URL
	HPR_BOOL bdownload;                     //��ʾ�Ƿ�����
#ifdef SOURCE_ASYNC_PROCESS
	int seq;
	void *callclass;
#endif
} RTSPSERVER_DESCRIBEINFO, *PRTSPSERVER_DESCRIBEINFO;

typedef struct RTSPServer_SetupInfo
{
	HPR_ADDR_T clientaddr;
	char *url;								//�ͻ��˵�����URL
	char *auth;								//��֤��Ϣ
	char *useragent;						//�ͻ�������
	HPR_INT32 transmethod;					//���䷽ʽ
	HPR_UINT16 devicestartport;				//�豸�������͵���ʼĿ�Ķ˿�
	HPR_UINT16 deviceendport;				//�豸�������͵Ľ���Ŀ�Ķ˿�
	//�����ϰ汾����ý��
	HPR_INT64 filesize;					//�ļ���С
	HPR_INT32 totaltime;				//��󲥷�ʱ��
	//////////////////////////////////////////////////
	HPR_INT32 sessionhandle;
	HPR_BOOL  bStream20;                  //�Ƿ���2.0�汾
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
	HPR_INT64 starttime;  //���ʱ��
	HPR_INT64 endtime;
	HPR_VOID  *abs_starttime;  //����ʱ��
	HPR_VOID  *abs_endtime;
	bool      babsplay;       //�Ƿ��ǰ�����ʱ�䲥��
	bool bvalidscale;
	HPR_INT32 iscale;
	HPR_BOOL bBackward;      //�Ƿ��ǵ���
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
	char clientip[MAX_IP_LEN];			//�ͻ��˵�IP��ַ
	HPR_UINT16 clientport;				//�ͻ��˵Ķ˿�
	char url[MAX_URL_LEN];				//�ͻ��������URL
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
	virtual HPR_INT32 GetPacket(char* buf, HPR_INT32 buflen) = 0;					//��ȡ������
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