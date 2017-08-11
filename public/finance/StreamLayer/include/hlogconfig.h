#include "hlog.h"
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


// log macro for the service
#if (defined _WIN32 || defined _WIN64)
#define STREAMLAYER_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "STREAMLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define STREAMLAYER_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "STREAMLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define STREAMLAYER_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "STREAMLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define STREAMLAYER_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "STREAMLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define STREAMLAYER_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "STREAMLAYER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    

#else
#error  Other OS Not Support 
#endif


#endif // end of GUARD_HLOGCONFIG_H
