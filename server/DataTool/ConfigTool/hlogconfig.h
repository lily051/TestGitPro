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

#include "../../../public/base/HLOG/v2.0/include/hlog.h"

#define _USE_HLOG

#ifdef _USE_HLOG

// log macro for the myutils
#if (defined _WIN32 || defined _WIN64)
#define MYUTILS_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "MYUTILS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define MYUTILS_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "MYUTILS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYUTILS_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "MYUTILS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYUTILS_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "MYUTILS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYUTILS_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "MYUTILS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#error  Other OS Not Support 
#endif


// for CMS module
#if (defined _WIN32 || defined _WIN64)
#define CMS_ERROR(fmt, ...)          hlog_format(HLOG_LEVEL_ERROR, "CMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CMS_WARN(fmt, ...)           hlog_format(HLOG_LEVEL_WARN, "CMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_INFO(fmt, ...)           hlog_format(HLOG_LEVEL_INFO, "CMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_DEBUG(fmt, ...)          hlog_format(HLOG_LEVEL_DEBUG, "CMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_TRACE(fmt, ...)          hlog_format(HLOG_LEVEL_TRACE, "CMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define CMS_ERROR(fmt, args...)      hlog_format(HLOG_LEVEL_ERROR, "CMS", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_WARN(fmt, args...)       hlog_format(HLOG_LEVEL_WARN, "CMS", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_INFO(fmt, args...)       hlog_format(HLOG_LEVEL_INFO, "CMS", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_DEBUG(fmt, args...)      hlog_format(HLOG_LEVEL_DEBUG, "CMS", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_TRACE(fmt, args...)      hlog_format(HLOG_LEVEL_TRACE, "CMS", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif


// for CMS_DATA module
#if (defined _WIN32 || defined _WIN64)
#define CMS_DATA_ERROR(fmt, ...)          hlog_format(HLOG_LEVEL_ERROR, "CMS_DATA", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CMS_DATA_WARN(fmt, ...)           hlog_format(HLOG_LEVEL_WARN, "CMS_DATA", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_DATA_INFO(fmt, ...)           hlog_format(HLOG_LEVEL_INFO, "CMS_DATA", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_DATA_DEBUG(fmt, ...)          hlog_format(HLOG_LEVEL_DEBUG, "CMS_DATA", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_DATA_TRACE(fmt, ...)          hlog_format(HLOG_LEVEL_TRACE, "CMS_DATA", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define CMS_DATA_ERROR(fmt, args...)      hlog_format(HLOG_LEVEL_ERROR, "CMS_DATA", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_DATA_WARN(fmt, args...)       hlog_format(HLOG_LEVEL_WARN, "CMS_DATA", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_DATA_INFO(fmt, args...)       hlog_format(HLOG_LEVEL_INFO, "CMS_DATA", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_DATA_DEBUG(fmt, args...)      hlog_format(HLOG_LEVEL_DEBUG, "CMS_DATA", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_DATA_TRACE(fmt, args...)      hlog_format(HLOG_LEVEL_TRACE, "CMS_DATA", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for CMS_STAT module
#if (defined _WIN32 || defined _WIN64)
#define CMS_STAT_ERROR(fmt, ...)          hlog_format(HLOG_LEVEL_ERROR, "CMS_STAT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CMS_STAT_WARN(fmt, ...)           hlog_format(HLOG_LEVEL_WARN, "CMS_STAT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_STAT_INFO(fmt, ...)           hlog_format(HLOG_LEVEL_INFO, "CMS_STAT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_STAT_DEBUG(fmt, ...)          hlog_format(HLOG_LEVEL_DEBUG, "CMS_STAT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_STAT_TRACE(fmt, ...)          hlog_format(HLOG_LEVEL_TRACE, "CMS_STAT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define CMS_STAT_ERROR(fmt, args...)      hlog_format(HLOG_LEVEL_ERROR, "CMS_STAT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_STAT_WARN(fmt, args...)       hlog_format(HLOG_LEVEL_WARN, "CMS_STAT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_STAT_INFO(fmt, args...)       hlog_format(HLOG_LEVEL_INFO, "CMS_STAT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_STAT_DEBUG(fmt, args...)      hlog_format(HLOG_LEVEL_DEBUG, "CMS_STAT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_STAT_TRACE(fmt, args...)      hlog_format(HLOG_LEVEL_TRACE, "CMS_STAT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for CMS_CLIENT module
#if (defined _WIN32 || defined _WIN64)
#define CMS_CLIENT_ERROR(fmt, ...)          hlog_format(HLOG_LEVEL_ERROR, "CMS_CLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CMS_CLIENT_WARN(fmt, ...)           hlog_format(HLOG_LEVEL_WARN, "CMS_CLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_CLIENT_INFO(fmt, ...)           hlog_format(HLOG_LEVEL_INFO, "CMS_CLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_CLIENT_DEBUG(fmt, ...)          hlog_format(HLOG_LEVEL_DEBUG, "CMS_CLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CMS_CLIENT_TRACE(fmt, ...)          hlog_format(HLOG_LEVEL_TRACE, "CMS_CLIENT", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#define CMS_CLIENT_ERROR(fmt, args...)      hlog_format(HLOG_LEVEL_ERROR, "CMS_CLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_CLIENT_WARN(fmt, args...)       hlog_format(HLOG_LEVEL_WARN, "CMS_CLIENT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_CLIENT_INFO(fmt, args...)       hlog_format(HLOG_LEVEL_INFO, "CMS_CLIENT", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_CLIENT_DEBUG(fmt, args...)      hlog_format(HLOG_LEVEL_DEBUG, "CMS_CLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define CMS_CLIENT_TRACE(fmt, args...)      hlog_format(HLOG_LEVEL_TRACE, "CMS_CLIENT", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

#else   // _USE_HLOG

// log macro for the myutils
#if (defined _WIN32 || defined _WIN64)
#define MYUTILS_ERROR(fmt, ...)         
#define MYUTILS_WARN(fmt, ...)          
#define MYUTILS_INFO(fmt, ...)          
#define MYUTILS_DEBUG(fmt, ...)         
#define MYUTILS_TRACE(fmt, ...)         
#else
#error  Other OS Not Support 
#endif

// for CMS module
#if (defined _WIN32 || defined _WIN64)
#define CMS_ERROR(fmt, ...)          
#define CMS_WARN(fmt, ...)           
#define CMS_INFO(fmt, ...)           
#define CMS_DEBUG(fmt, ...)          
#define CMS_TRACE(fmt, ...)          
#else
#error  Other OS Not Support 
#endif


// for CMS_DATA module
#if (defined _WIN32 || defined _WIN64)
#define CMS_DATA_ERROR(fmt, ...)          
#define CMS_DATA_WARN(fmt, ...)           
#define CMS_DATA_INFO(fmt, ...)           
#define CMS_DATA_DEBUG(fmt, ...)          
#define CMS_DATA_TRACE(fmt, ...)          
#else
#error  Other OS Not Support 
#endif

// for CMS_STAT module
#if (defined _WIN32 || defined _WIN64)
#define CMS_STAT_ERROR(fmt, ...)          
#define CMS_STAT_WARN(fmt, ...)           
#define CMS_STAT_INFO(fmt, ...)           
#define CMS_STAT_DEBUG(fmt, ...)          
#define CMS_STAT_TRACE(fmt, ...)          
#else
#error  Other OS Not Support 
#endif

// for CMS_CLIENT module
#if (defined _WIN32 || defined _WIN64)
#define CMS_CLIENT_ERROR(fmt, ...)          
#define CMS_CLIENT_WARN(fmt, ...)           
#define CMS_CLIENT_INFO(fmt, ...)           
#define CMS_CLIENT_DEBUG(fmt, ...)          
#define CMS_CLIENT_TRACE(fmt, ...)          
#else
#error  Other OS Not Support 
#endif

#endif // _USE_HLOG

#define LogPbWrap(pWrap)\
    CMS_DEBUG("src[%d]-(%u)->dst[%d]", \
    pWrap->GetWrapSrcId(),\
    pWrap->GetInnerSequence(),\
    pWrap->GetWrapDstId())

#endif // end of GUARD_HLOGCONFIG_H
