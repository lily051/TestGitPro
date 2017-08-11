
#pragma once
#include "./Log/hlog.h"
#include "./Log/hlogex.h"
#pragma comment(lib, "./Log/hlog.lib")


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

#if (defined _WIN32 || defined _WIN64)
#define SVCMANAGERLOG_OBJECT_NAME             "SVCMANAGER"
#define SVCMANAGERLOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, SVCMANAGERLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define SVCMANAGERLOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, SVCMANAGERLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCMANAGERLOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, SVCMANAGERLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCMANAGERLOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, SVCMANAGERLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCMANAGERLOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, SVCMANAGERLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    

#define SVCCONTROLLOG_OBJECT_NAME             "SVCCONTROL"
#define SVCCONTROLLOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, SVCCONTROLLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define SVCCONTROLLOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, SVCCONTROLLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCCONTROLLOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, SVCCONTROLLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCCONTROLLOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, SVCCONTROLLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define SVCCONTROLLOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, SVCCONTROLLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    

#define VIEWMSGLOG_OBJECT_NAME             "VIEWMSGLOG"
#define VIEWMSGLOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, VIEWMSGLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define VIEWMSGLOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, VIEWMSGLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define VIEWMSGLOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, VIEWMSGLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define VIEWMSGLOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, VIEWMSGLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define VIEWMSGLOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, VIEWMSGLOG_OBJECT_NAME, "<[%d] - %s> <"fmt">", __LINE__, __FUNCTION__, ##__VA_ARGS__)    

#else
#error  Other OS Not Support 
#endif
