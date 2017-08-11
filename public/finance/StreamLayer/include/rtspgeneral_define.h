/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: rtspgeneral_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __RTSPGENERAL_DEFINE_H__
#define __RTSPGENERAL_DEFINE_H__

#include "rtprtcp_define.h"

#define MAX_RTSPTIME_LEN 128				//最大RTSP时间长度
#define MAX_ACCEPTTYEP_LEN 512				//最大RTSP接受类型长度

#define MAX_URL_LEN 512						//最大URL长度
#define MAX_USERAGENT_LEN 256				//最大useragent长度
#define MAX_SERVERAGENT_LEN 256				//最大serveragent长度
#define MAX_SESSIONID_LEN 256				//最大sessionid长度
#define MAX_HEAD_LEN 256					//最大head长度
#define MAX_AUTH_LEN 512					//最大认证信息长度
#define MAX_USERNAME_LEN 32					//最大username长度
#define MAX_PASSWD_LEN 32					//最大passwd长度
#define MAX_IP_LEN 23						//最大IP地址长度
#define MAX_VERSIONINFO_LEN 128				//最大版本信息长度
#define MAX_SINGLERTSP_RTP_COUNT 10			//单个RTSP会话中最多包含的RTP会话数目

#define RTSP_NPT_TIMETYPE 0x8100			//NPT时间
#define RTSP_SMPTE_TIMETYPE 0x8101			//SMPTE时间
#define RTSP_CLOCK_TIMETYPE 0x8102			//壁钟时间

enum RTSPSESSION_STATE {RTSPSTATE_STOP, RTSPSTATE_INIT, RTSPSTATE_DESCRIBE, RTSPSTATE_SETUP, RTSPSTATE_PLAY, RTSPSTATE_PAUSE};

#endif