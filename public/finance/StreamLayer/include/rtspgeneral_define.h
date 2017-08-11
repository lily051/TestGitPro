/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspgeneral_define.h
Description: �ṹ�ͺ궨��  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTSPGENERAL_DEFINE_H__
#define __RTSPGENERAL_DEFINE_H__

#include "rtprtcp_define.h"

#define MAX_RTSPTIME_LEN 128				//���RTSPʱ�䳤��
#define MAX_ACCEPTTYEP_LEN 512				//���RTSP�������ͳ���

#define MAX_URL_LEN 512						//���URL����
#define MAX_USERAGENT_LEN 256				//���useragent����
#define MAX_SERVERAGENT_LEN 256				//���serveragent����
#define MAX_SESSIONID_LEN 256				//���sessionid����
#define MAX_HEAD_LEN 256					//���head����
#define MAX_AUTH_LEN 512					//�����֤��Ϣ����
#define MAX_USERNAME_LEN 32					//���username����
#define MAX_PASSWD_LEN 32					//���passwd����
#define MAX_IP_LEN 23						//���IP��ַ����
#define MAX_VERSIONINFO_LEN 128				//���汾��Ϣ����
#define MAX_SINGLERTSP_RTP_COUNT 10			//����RTSP�Ự����������RTP�Ự��Ŀ

#define RTSP_NPT_TIMETYPE 0x8100			//NPTʱ��
#define RTSP_SMPTE_TIMETYPE 0x8101			//SMPTEʱ��
#define RTSP_CLOCK_TIMETYPE 0x8102			//����ʱ��

enum RTSPSESSION_STATE {RTSPSTATE_STOP, RTSPSTATE_INIT, RTSPSTATE_DESCRIBE, RTSPSTATE_SETUP, RTSPSTATE_PLAY, RTSPSTATE_PAUSE};

#endif