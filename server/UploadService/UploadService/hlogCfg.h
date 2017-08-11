#pragma once

#include <hlog.h>

#if (defined _WIN32 || defined _WIN64)

#define ULSERVER_LOG_ERROR(fmt, ...)		 hlog_format(HLOG_LEVEL_ERROR, "ULSERVER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ULSERVER_LOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "ULSERVER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define ULSERVER_LOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "ULSERVER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define ULSERVER_LOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "ULSERVER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define ULSERVER_LOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "ULSERVER", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  

#define CVR_LOG_ERROR(fmt, ...)		         hlog_format(HLOG_LEVEL_ERROR, "CVR", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CVR_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "CVR", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CVR_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "CVR", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CVR_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "CVR", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CVR_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "CVR", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  

#define CLOUD_LOG_ERROR(fmt, ...)		     hlog_format(HLOG_LEVEL_ERROR, "CLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define CLOUD_LOG_WARN(fmt, ...)             hlog_format(HLOG_LEVEL_WARN, "CLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLOUD_LOG_INFO(fmt, ...)             hlog_format(HLOG_LEVEL_INFO, "CLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLOUD_LOG_DEBUG(fmt, ...)            hlog_format(HLOG_LEVEL_DEBUG, "CLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define CLOUD_LOG_TRACE(fmt, ...)            hlog_format(HLOG_LEVEL_TRACE, "CLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  

#define KMS_LOG_ERROR(fmt, ...)		         hlog_format(HLOG_LEVEL_ERROR, "KMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define KMS_LOG_WARN(fmt, ...)               hlog_format(HLOG_LEVEL_WARN, "KMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define KMS_LOG_INFO(fmt, ...)               hlog_format(HLOG_LEVEL_INFO, "KMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define KMS_LOG_DEBUG(fmt, ...)              hlog_format(HLOG_LEVEL_DEBUG, "KMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define KMS_LOG_TRACE(fmt, ...)              hlog_format(HLOG_LEVEL_TRACE, "KMS", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  

#define OBJECTCLOUD_LOG_ERROR(fmt, ...)		 hlog_format(HLOG_LEVEL_ERROR, "OBJECTCLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define OBJECTCLOUD_LOG_WARN(fmt, ...)       hlog_format(HLOG_LEVEL_WARN, "OBJECTCLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define OBJECTCLOUD_LOG_INFO(fmt, ...)       hlog_format(HLOG_LEVEL_INFO, "OBJECTCLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define OBJECTCLOUD_LOG_DEBUG(fmt, ...)      hlog_format(HLOG_LEVEL_DEBUG, "OBJECTCLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#define OBJECTCLOUD_LOG_TRACE(fmt, ...)      hlog_format(HLOG_LEVEL_TRACE, "OBJECTCLOUD", "<%s [%d] - %s> <"fmt">",  __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  


#else
#error  Other OS Not Support 
#endif
