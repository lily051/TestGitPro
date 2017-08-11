/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamgeneral_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.02.19    
*****************************************************/
#pragma once
#ifndef __STREAMGENERAL_DEFINE_H__
#define __STREAMGENERAL_DEFINE_H__

#include "rtspserver_define.h"
#include "rtspclient_define.h"

#define MAX_SHAREDBUFF_BLOCK 101			//一个共享缓冲区中的最大共享缓冲块个数
#define MAX_SHAREDBUFF_LENGTH 4072			//一个共享缓冲块的最大长度

#define MAX_SDP_LEN 2048						//最大SDP长度
#define MAX_ENCRYPTDES_LEN 64					//最大加密方式描述长度

//下载结束标志
#define HEADER_FLAG    0x68696b24
#define HEADER_FLAG_EX 0x01020304
typedef struct _DOWNLOAD_BYE_
{
	int iByte;
	int iByteEx;
}DOWNLOAD_BYE;

#endif