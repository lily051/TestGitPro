#ifndef _HPP_COMMON_H_
#define _HPP_COMMON_H_

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Time.h"
#include "HPR_Mutex.h"
#include "HPR_Socket.h"
#include "HPR_AsyncIOEX.h"
#include "HPR_Thread.h"
#include "HPR_Utils.h"
#include "HPR_Select.h"
#include "HPR_Guard.h"
#include "HPR_Hpr.h"
#include <hlog.h>

#if (defined _WIN32 || defined _WIN64)
    #if (defined USE_HPP_LIB || defined BUILD_HPP_LIB)
        #define HPP_DECLARE extern "C"
        #define HPP_DECLARE_CLASS
    #else
        #if defined(HPP_EXPORTS)
            #define HPP_DECLARE extern "C" __declspec(dllexport)
            #define HPP_DECLARE_CLASS __declspec(dllexport)
        #else
            #define HPP_DECLARE extern "C" __declspec(dllimport)
            #define HPP_DECLARE_CLASS __declspec(dllimport)
        #endif
    #endif
#else
    #define HPP_DECLARE 
    #define HPP_DECLARE_CLASS
#endif

#define PRINT_PER_COMMAND   300
#define FUNC_DO_FOREVER     0xFFFF


struct HPP_MSG
{
    HPR_INT32 nMsgId;
    HPR_INT32 nSessionId;
    HPR_BOOL bWrapper;
    HPR_INT32 nIOType;
    HPR_VOIDPTR pData;
}; 

enum EVENT_TYPE
{
RECV_TYPE = 0,
SEND_TYPE = 1,
RECV_FROM_TYPE = 2,
SEND_TO_TYPE = 3
};

struct BUFFER_INFO
{
    HPR_INT8* buffer;
    HPR_INT8* bigBuffer;
    HPR_INT32 buffLen;
    HPR_INT32 bigBuffLen;
    HPR_INT32 writeIdx;
    HPR_BOOL bBigBuffer;            /* Flag of Using Big Size Buffer */
};

typedef HPR_VOID (*TimerCallBack)(HPR_VOIDPTR pSession);
struct TIMER_INFO
{
    HPR_INT32 nInterval;            /* 执行间隔 */
    HPR_INT32 nTimes;               /* 执行次数 */
    HPR_VOIDPTR pData;              
    TimerCallBack timeFunc;         /* 执行函数 */
};
struct SESSION_TIMER_INFO
{
    TIMER_INFO nTimerInfo;
    HPR_UINT32 nLastDoTime;
};


/*******************************服务器性能常量********************************/
#define HPP_MAX_MSGQ_SIZE               20000
#define HPP_MAX_CONN_NUM                5000
#define EHOME_MAX_CONN_NUM              5000
#define MQ_MAX_CONN_NUM                 100
#define VC_MAX_CONN_NUM                 100
#define HPP_TCP_RECV_BUF_LEN            (200 * 1024)
#define HPP_UDP_RECV_BUF_LEN            1500


#define HPP_MAX_DELIVER_LETTER          50000

#define PRINT_PER_COMMAND               300
#define FUNC_DO_FOREVER                 0xFFFF
#define HEADER_FLAG_                    0x68696b24          //头标志
#define ENDMARK                         0x0000879b          //结束标志

#define PACK_HEAD_LEN                   28                  //头部长度（16+12）
#define PACK_TAIL_LEN                   4                   //尾部长度(4)

// CAG
#define VTM_TO_CAG                      0x0205
#define CAG_TO_VTM                      0x0502
#define PAG_TO_CAG                      0x0405
#define CAG_TO_PAG                      0x0504
#define VRM_TO_CAG                      0x0A05
#define CAG_TO_VRM                      0x050A
#define VMS_TO_CAG                      0x0B05
#define CAG_TO_VMS                      0x050B
#define DES_TO_CAG                      0x0C05
#define CAG_TO_DES                      0x050C
#define CU_TO_CAG                       0x0D05  
#define CAG_TO_CU                       0x050D
#define CAG_TO_CAG                      0x050E

// VTM
#define VTDU_TO_VTM                     0x0302
#define VTM_TO_VTDU                     0x0203
#define PAG_TO_VTM                      0x0402
#define VTM_TO_PAG                      0x0204

//vtm与pcnvr连接
#define PCNVR_TO_VTM                    0x0B02
#define VTM_TO_PCNVR                    0x020B

// VRM
#define VTM_TO_VRM                      0x020A
#define VRM_TO_VTM                      0x0A02
#define VTDU_TO_VRM                     0x030A
#define VRM_TO_VTDU                     0x0A03
#define PAG_TO_VRM                      0x040A
#define VRM_TO_PAG                      0x0A04
#define PCNVR_TO_VRM                    0x0B0A
#define VRM_TO_PCNVR                    0x0A0B
#define VOD_TO_VRM                      0x0E0A
#define VRM_TO_VOD                      0x0A0E

// CMS
#define VTM_TO_CMS                      0x0201
#define CMS_TO_VTM                      0x0102
#define PAG_TO_CMS                      0x0401
#define CMS_TO_PAG                      0x0104
#define CAG_TO_CMS                      0x0501
#define CMS_TO_CAG                      0x0105
#define MQ_TO_CMS                       0x0701
#define CMS_TO_MQ                       0x0107
#define VRM_TO_CMS                      0x0A01
#define CMS_TO_VRM                      0x010A
#define NMS_TO_CMS                      0x0D01
#define CMS_TO_NMS                      0x010D

//cms与pcnvr连接
#define PCNVR_TO_CMS                    0x0B01
#define CMS_TO_PCNVR                    0x010B

// ALARM
#define CMS_TO_ALARM                    0x0106
#define ALARM_TO_CMS                    0x0601
#define VTM_TO_ALARM                    0x0206
#define ALARM_TO_VTM                    0x0602
#define PAG_TO_ALARM                    0x0406
#define ALARM_TO_PAG                    0x0604
#define CAG_TO_ALARM                    0x0506
#define ALARM_TO_CAG                    0x0605
#define VRM_TO_ALARM                    0x0A06
#define ALARM_TO_VRM                    0x060A
#define NMS_TO_ALARM                    0x0D06
#define ALARM_TO_NMS                    0x060D

// SHARE
#define SHARE_TO_CMS                    0xC801
#define SHARE_TO_CAG                    0xC805
#define CAG_TO_SHARE                    0x05C8
#define SHARE_TO_MQ                     0xC807
#define MQ_TO_SHARE                     0x07C8
#define SHARE_TO_NMS                    0xC80D
#define NMS_TO_SHARE                    0x0DC8
#define SHARE_TO_SHARE                  0xC8C8
#define SHARE_TO_ALARM                  0xC806
#define SHARE_TO_VTM                    0xC802
#define VTM_TO_SHARE                    0x02C8

// VMS
#define VMS_TO_CMS                      0x1201
#define CMS_TO_VMS                      0x0112
#define AMS_TO_VMS                      0x0612
#define VMS_TO_AMS                      0x1206
#define VMS_TO_VTM                      0x1202
#define VTM_TO_VMS                      0x0212

// IAS
#define IAS_TO_CMS                      0x1501
#define CMS_TO_IAS                      0x0115

// CCS
#define CCS_TO_CMS                      0x1901
#define CMS_TO_CCS                      0x0119
#define CAG_TO_CCS                      0x0519
#define CCS_TO_CAG                      0x1905
#define CCS_TO_ALARM                    0x1906
#define ALARM_TO_CCS                    0x0619



/********************************************* MQ SendType ****************************************/
#define USE_QUEUE                       1
#define USE_TEMP_QUEUE                  2
#define USE_TOPIC                       3
#define USE_TEMP_TOPIC                  4

/******************************************** HPP_MSG nMsgId **************************************/
#define HPP_MSG_EXCEPTION_SESSION       1                   /* 异常 */
#define HPP_MSG_NEW_SESSION             2                   /* 新的session */
#define HPP_MSG_NORMAL_SESSION          3                   /* 正常的Wrap及网络相关数据*/
#define HPP_MSG_STOP_SESSION            4                   /* STOPSESSION紧急处理*/
#define HPP_MSG_PROCINNERMSG            5
#define HPP_MSG_CONN_COMPLETE           6
#define HPP_MSG_USER_DEFINE_DATA        7                   /*应用层定义的数据*/

/********************************************* end HPP_MSG nMsgId**********************************/

#define SESSION_LEVEL_URGENT            1
#define SESSION_LEVEL_NORMAL            2
#define SESSION_LEVEL_IDLE              3

#define SERVICE_HIGH_PRI                1
#define SERVICE_NORMAL_PRI              2
#define SERVICE_WOULD_BLOCK             3

/****************************************** HPP消息队列优先级 **********************************/
#define HPP_MSGQ_PRI_STOP_SESSION       1000                    /* 停止会话最高优先级 */
#define HPP_MSGQ_PRI_IOCOMPLETE_SEND    100                     /* 发送成功*/
#define HPP_MSGQ_PRI_IOCOMPLETE_RECV    100                     /* 接到数据*/
#define HPP_MSGQ_PRI_POST_MESSAGE       20                      /* 内部队列间传递数据*/
#define HPP_MSGQ_PRI_NORMAL             100
/****************************************** HPP消息队列优先级结束 *******************************/

#define HPP_MSG_FROM_INNER              0x0FFFFFFF              /*消息的srcId为HPP_MSG_FROM_INNER，表示消息报文来自内部(PostInnerMessage传递)*/


/******************************************* LOG  ******************************************/
#define LOG_HPP_LOGIC_ERROR     HPP_ERROR("HPP logic error")
#define LOG_HPP_NO_LETTER_ERROR HPP_ERROR("HPP no letter error")
#define LOG_HPP_MSGQ_SEND_ERROR HPP_ERROR("HPP message queue send Error")
#define LOG_HPP_NO_MEM_ERROR    HPP_ERROR("No memory")

#endif
