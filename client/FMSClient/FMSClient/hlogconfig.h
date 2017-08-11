#ifndef GUARD_HLOGCONFIG_H
#define GUARD_HLOGCONFIG_H
#pragma once

/**	@file    hlogconfig.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   hlog 配置辅助文件
 *
 *	@author	 xuezhihu
 *	@date	 2012/3/22
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V2.0.0  添加了一个导出接口
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#include <hlog.h>

#define _USE_HLOG

#ifdef _USE_HLOG

// for CLIENTCOMM module
#if (defined _WIN32 || defined _WIN64)
#define CLIENT_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "CLIENTCOMM", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CLIENT_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "CLIENTCOMM", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLIENT_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "CLIENTCOMM", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLIENT_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "CLIENTCOMM", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLIENT_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "CLIENTCOMM", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define CLIENT_ERROR(fmt, args...)     hlog_format(HLOG_LEVEL_ERROR, "CLIENTCOMM", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CLIENT_WARN(fmt, args...)      hlog_format(HLOG_LEVEL_WARN, "CLIENTCOMM", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CLIENT_INFO(fmt, args...)      hlog_format(HLOG_LEVEL_INFO, "CLIENTCOMM", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CLIENT_DEBUG(fmt, args...)     hlog_format(HLOG_LEVEL_DEBUG, "CLIENTCOMM", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CLIENT_TRACE(fmt, args...)     hlog_format(HLOG_LEVEL_TRACE, "CLIENTCOMM", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for FMSCLIENT module
#if (defined _WIN32 || defined _WIN64)
#define FMSCLIENT_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "FMSCLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define FMSCLIENT_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "FMSCLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define FMSCLIENT_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "FMSCLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define FMSCLIENT_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "FMSCLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define FMSCLIENT_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "FMSCLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define FMSCLIENT_ERROR(fmt, args...)     hlog_format(HLOG_LEVEL_ERROR, "FMSCLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define FMSCLIENT_WARN(fmt, args...)      hlog_format(HLOG_LEVEL_WARN, "FMSCLIENT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CLIENT_INFO(fmt, args...)      hlog_format(HLOG_LEVEL_INFO, "FMSCLIENT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define FMSCLIENT_DEBUG(fmt, args...)     hlog_format(HLOG_LEVEL_DEBUG, "FMSCLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define FMSCLIENT_TRACE(fmt, args...)     hlog_format(HLOG_LEVEL_TRACE, "FMSCLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for HPP module
#if (defined _WIN32 || defined _WIN64)
#define COMM_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "COMMIDLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define COMM_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "COMMIDLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define COMM_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "COMMIDLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define COMM_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "COMMIDLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define COMM_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "COMMIDLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define COMM_ERROR(fmt, args...)     hlog_format(HLOG_LEVEL_ERROR, "COMMIDLAYER", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define COMM_WARN(fmt, args...)      hlog_format(HLOG_LEVEL_WARN, "COMMIDLAYER", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define COMM_INFO(fmt, args...)      hlog_format(HLOG_LEVEL_INFO, "COMMIDLAYER", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define COMM_DEBUG(fmt, args...)     hlog_format(HLOG_LEVEL_DEBUG, "COMMIDLAYER", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define COMM_TRACE(fmt, args...)     hlog_format(HLOG_LEVEL_TRACE, "COMMIDLAYER", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

#else   // _USE_HLOG

// for CLIENT module
#if (defined _WIN32 || defined _WIN64)
#define CLIENT_ERROR(fmt, ...)         
#define CLIENT_WARN(fmt, ...)          
#define CLIENT_INFO(fmt, ...)          
#define CLIENT_DEBUG(fmt, ...)         
#define CLIENT_TRACE(fmt, ...)         
#else
#define CLIENT_ERROR(fmt, args...)     
#define CLIENT_WARN(fmt, args...)      
#define CLIENT_INFO(fmt, args...)      
#define CLIENT_DEBUG(fmt, args...)     
#define CLIENT_TRACE(fmt, args...)     
#endif

// for COMMIDLAYER module
#if (defined _WIN32 || defined _WIN64)
#define COMM_ERROR(fmt, ...)         
#define COMM_WARN(fmt, ...)          
#define COMM_INFO(fmt, ...)          
#define COMM_DEBUG(fmt, ...)         
#define COMM_TRACE(fmt, ...)         
#else
#define COMM_ERROR(fmt, args...)     
#define COMM_WARN(fmt, args...)      
#define COMM_INFO(fmt, args...)      
#define COMM_DEBUG(fmt, args...)     
#define COMM_TRACE(fmt, args...)     
#endif

#endif // _USE_HLOG

#endif // GUARD_HLOGCONFIG_H