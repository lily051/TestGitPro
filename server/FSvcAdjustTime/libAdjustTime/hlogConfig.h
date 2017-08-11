#ifndef GUARD_HLOGCONFIG_H
#define GUARD_HLOGCONFIG_H
#pragma once
/**	@file    hlogConfig.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   hlog 的配置
 *
 *	@author	 xuezhihu
 *	@date	 2012/4/11
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#include <string>
#include <hlog.h>

// block log object
class CBlockLogObject
{
public:
    CBlockLogObject(const char *logModule,
        const char *pFileName/* = __FILE__*/,
        const char *pBlockName/* = __FUNCTION__*/, 
        int lineNumber/* = __LINE__*/,
        HLOG_LEVEL logLevel = HLOG_LEVEL_INFO,
        const char *pEnterMsg = "Enter",
        const char *pLeaveMsg = "Leave")
        : m_blockName(pBlockName)
        , m_logModule(logModule)
        , m_logLevel(logLevel)
        , m_enterMsg(pEnterMsg)
        , m_leaveMsg(pLeaveMsg)
        , m_lineNumber(lineNumber)
        , m_fileName(pFileName)
    {
        hlog_format(m_logLevel, m_logModule.c_str(), 
            "<%s [%d] - %s> <%s  %s>", m_fileName.c_str(), m_lineNumber, 
            m_blockName.c_str(), m_enterMsg.c_str(), m_blockName.c_str());
    }
    ~CBlockLogObject(void)
    {
        try
        {
	        hlog_format(m_logLevel, m_logModule.c_str(), 
	            "<%s [%d] - %s> <%s  %s>", m_fileName.c_str(), m_lineNumber, 
	            m_blockName.c_str(), m_leaveMsg.c_str(), m_blockName.c_str());
        }
        catch (...)
        {
        }
    }

private:
    const std::string m_blockName;
    const HLOG_LEVEL m_logLevel;
    const std::string m_enterMsg;
    const std::string m_leaveMsg;
    const std::string m_logModule;
    const std::string m_fileName;
    const int m_lineNumber;
private:
    CBlockLogObject(const CBlockLogObject &);
    CBlockLogObject &operator=(const CBlockLogObject &);
};
#define BLOCK_LOG_OBJECT(logModule) CBlockLogObject logObj((logModule), __FILE__, __FUNCTION__, __LINE__)

// for backup service module
#if (defined _WIN32 || defined _WIN64)
#define TPLOG_OBJECT_NAME             "LIBADJUST"
#define TPLOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, TPLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define TPLOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, TPLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define TPLOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, TPLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define TPLOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, TPLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define TPLOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, TPLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
#error  Other OS Not Support 
#endif

#endif // end of GUARD_HLOGCONFIG_H
