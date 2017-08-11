#include "hlog.h"
#ifndef GUARD_HLOGCONFIG_H
#define GUARD_HLOGCONFIG_H
#pragma once

/**	@file    hlogconfig.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   hlog ���ø����ļ�
 *
 *	@author	 xuezhihu
 *	@date	 2012/3/22
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V2.0.0  �����һ�������ӿ�
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
