#ifndef GUARD_HLOGCONFIG_H
#define GUARD_HLOGCONFIG_H

/**	@file    hlogConfig.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   hlog的配置头文件
 *
 *	@author	 xuezhihu
 *	@date	 2012/7/18
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
#include <hlogex.h>

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
        hlog_format(m_logLevel, m_logModule.c_str(), 
            "<%s [%d] - %s> <%s  %s>", m_fileName.c_str(), m_lineNumber, 
            m_blockName.c_str(), m_leaveMsg.c_str(), m_blockName.c_str());
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

// MYLOG 的日志例子
// for MYLOG module
#if (defined _WIN32 || defined _WIN64)
#define MYLOG_OBJECT_NAME             "MYLOG"
#define MYLOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, MYLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define MYLOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, MYLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYLOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, MYLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYLOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, MYLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define MYLOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, MYLOG_OBJECT_NAME, "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    

// MYLOG LOGGER
inline HlogWrapper& GetLogger_MYLOG()
{
    static HlogWrapper instance("MYLOG");
    return instance;
}

#define MYLOG_EX_LOCATION_LOGGER(level) LOCATION_LOGGER(GetLogger_MYLOG(), level)

#define MYLOG_EX_TRACE   MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_TRACE)
#define MYLOG_EX_DEBUG   MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_DEBUG)
#define MYLOG_EX_INFO    MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_INFO)
#define MYLOG_EX_WARN    MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_WARN)
#define MYLOG_EX_ERROR   MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_ERROR)
#define MYLOG_EX_FATAL   MYLOG_EX_LOCATION_LOGGER(HLOG_LEVEL_FATAL)

#else
#error  Other OS Not Support 
#endif


#endif // end of GUARD_HLOGCONFIG_H
