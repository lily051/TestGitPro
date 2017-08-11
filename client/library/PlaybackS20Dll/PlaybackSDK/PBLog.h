#pragma once

#include <string>

#define PB_OBJECT_NAME             "PlaybackSDK"

// for MYLOG module
#if (defined _WIN32 || defined _WIN64)

void PB_PRINT(HLOG_LEVEL nLogType, const char* pFile, int nLine, const char* pFunction, const TCHAR *fmt, ...);

#define PLAYBACKSDK_ERROR(fmt, ...) PB_PRINT(HLOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define PLAYBACKSDK_INFO(fmt, ...) PB_PRINT(HLOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define PLAYBACKSDK_WARN(fmt, ...) PB_PRINT(HLOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define PLAYBACKSDK_DEBUG(fmt, ...) PB_PRINT(HLOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
// MYLOG LOGGER
inline HlogWrapper& GetLogger_MYLOG()
{
	static HlogWrapper instance(PB_OBJECT_NAME);
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