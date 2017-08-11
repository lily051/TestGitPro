/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspclient_define.h
Description: �ṹ�ͺ궨��  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTSPCLIENT_DEFINE_H__
#define __RTSPCLIENT_DEFINE_H__
#include "rtspgeneral_define.h"

#include "ConfigFunc.h"

#define RTSPSESSION_CLIENT_MAXNUM 1000					//���ͻ��˸���
#define RTSPCLIENT_MSG_DESCRIBE 0x0001					//RTSP�ͻ���DESCRIBE��Ϣ֪ͨ
#define RTSPCLIENT_MSG_SETUP 0x0011						//RTSP�ͻ���SETUP��Ϣ֪ͨ
#define RTSPCLIENT_MSG_PLAY 0x0021						//RTSP�ͻ���PLAY��Ϣ֪ͨ
#define RTSPCLIENT_MSG_PAUSE 0x0031						//RTSP�ͻ���PAUSE��Ϣ֪ͨ
#define RTSPCLIENT_MSG_TEARDOWN 0x0041					//RTSP�ͻ���TEARDOWN��Ϣ֪ͨ
#define RTSPCLIENT_MSG_OPTIONS 0x0051					//RTSP�ͻ���OPTIONS��Ϣ֪ͨ
#define RTSPCLIENT_MSG_HEARTBEAT 0x0061					//RTSP�ͻ���HEARTBEAT��Ϣ֪ͨ
#define RTSPCLIENT_MSG_HEARTBEAT_TIMEOUT 0x0062			//RTSP�ͻ���HEARTBEAT TIMEOUT��Ϣ֪ͨ
#define RTSPCLIENT_MSG_EXIT 0x0091						//RTSP�ͻ���EXIT��Ϣ֪ͨ

typedef struct rtspclient_globalconfig
{
	int heartbeattype;						//������ʽ 0-��RTSP��չ����ʵ�� 1-��//SET_PARAMTER������ʵ��
} RTSPCLIENT_GLOBALCONFIG, *PRTSPCLIENT_GLOBALCONFIG;

typedef struct rtspclient_sessionconfig
{
	int TransMethod;						//���䷽ʽ 0-RTP/RTSP 1-RTP/RTCP 2-RTP/UDP 3-RTP/multicast
	char *useragent;						//�ͻ�������
	char *url;								//����URL
	char *auth;								//��֤��Ϣ
} RTSPCLIENT_SESSIONCONFIG, *PRTSPCLIENT_SESSIONCONFIG;

typedef struct rtspclient_describeparam
{
	char **acceptpresentation;				//�ɽ��ܵ�presentation���͵��б�
	int presentationcount;					//presentation���͵ĸ���
	bool bstandardstream;					//��ʾ�Ƿ��Ǳ�׼����
	bool bdevicepushdata;					//��ʾ�Ƿ����豸������������
	bool bdownload;                         //��ʾ�Ƿ��������
} RTSPCLIENT_DESCRIBEPARAM, *PRTSPCLIENT_DESCRIBEPARAM;

typedef struct rtspclient_setupparam
{
	char *url;								//�����URL
#ifdef SUPPORT_DB33
	bool bDB33;
	HPR_UINT16 port;
#endif
} RTSPCLIENT_SETUPPARAM, *PRTSPCLIENT_SETUPPARAM;

typedef struct rtspclient_playparam
{
	bool bhasplayrange;						//�Ƿ����Rangeͷ��
	int timetype;							//ʱ������
	char *starttime;						//��ʼʱ��
	char *endtime;							//����ʱ��
	char *playtime;							//��ʼ����ʱ��
	bool bhasplayscale;						//�Ƿ����Scaleͷ��
	int scale;								//���ű���
	bool bBackward;                         //��ʾ����
} RTSPCLIENT_PLAYPARAM, *PRTSPCLIENT_PLAYPARAM;

//��̨����
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
	HPR_INT32 ibri;   //����
	HPR_INT32 icon;   //�Աȶ�
	HPR_INT32 isat;
	HPR_INT32 ihue;
	HPR_INT32 imethods;
	stptzparam ptz_param;
}RTSPCLIENT_PTZPARAM, *PRTSPCLIENT_PTZPARAM;



typedef struct rtspclient_pauseparam
{
	bool bhaspausepoint;					//�Ƿ������ͣ��
	float pausepoint;						//��ͣ��
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
	char *presentation;						//presention �Ự����
	char *presentationtype;					//presentation����
	char *requesturl;						//����URL
	char *baseurl;							//����URL
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