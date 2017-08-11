/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: streamserver_define.h
Description: 结构和宏定义  	 
Author: guanguochen       
Date: 2009.03.06    
*****************************************************/
#pragma once
#ifndef __STREAMSERVER_DEFINE_H__
#define __STREAMSERVER_DEFINE_H__

#include "streamgeneral_define.h"

#define MAX_SHAREDBUFF_BLOCK 101				//一个共享缓冲区中的最大共享缓冲块个数
#define STREAMSERVER_MSG_CLIENTINFO 0x0101		//流媒体服务端客户端信息通知
#define MAX_RELATIVE_PATH_COUNT 10				//支持相对路径的最大数目
#define MAX_RELATIVEPATH_LEN 256				//最大相对路径长度

#define HPR_STREAMSERVER_HANDLE HPR_INT32		//流媒体服务器会话句柄类型定义
#define HPR_INVALID_STREAMSERVERSESSION -1		//无效的流媒体服务器会话句柄值定义

typedef HPR_INT32 (CALLBACK *pStreamServerMsgFunc)(HPR_STREAMSERVER_HANDLE sessionhandle, HPR_INT32 opt,\
						HPR_VOIDPTR param1, HPR_VOIDPTR param2, HPR_VOIDPTR param3, HPR_VOIDPTR param4);

#if defined(OS_WINDOWS)
	#if defined(STREAMSERVER_EXPORTS)
		#define HPR_STREAMSERVER_DECLARE extern "C" __declspec(dllexport)
	#else
		#define HPR_STREAMSERVER_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define HPR_STREAMSERVER_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif

#endif