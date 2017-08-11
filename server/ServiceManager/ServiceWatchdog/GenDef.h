/**
*   @file   GenDef.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  常量定义
*   
*   @author mazhentong
*   @date   2012/08/28
*
*   @note
*	
*   @warning
*/

#ifndef _WATCHDOG_GEN_DEF_H_
#define _WATCHDOG_GEN_DEF_H_

//#include "stdafx.h"

#include "SkinListCtrl.h"
#include "SkinListCtrl2.h"

#include <string>
#include <vector>
#include <map>

#include "SvcGeneralDef.h"

using namespace std;

// Get Host Name
static const USHORT GET_HOST_NAME_PORT = 6000;

//System TimerID定义

//CViewDlg Timer
static const UINT VIEWMESSAGE_TIMER = 0;            ///< 消息查看的定时器

// LocalServer 重连
static const UINT_PTR LOCAL_SERVER_RECONN = 1;
// LocalServer 重连
static const UINT_PTR LOCAL_SERVER_LISTEN = 1;

//ServiceController 异常定义
static const DWORD CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST = 0x00000000;  // 服务不存在
static const DWORD CONTROL_COMMAND_ERROR_UNKNOWN                = 0x00000001;  // 未知异常

static const DWORD CONTROL_COMMAND_ERROR_OPEN_SCMANAGER         = 0x00000011;  // OpenSCManager 出错
static const DWORD CONTROL_COMMAND_ERROR_OPEN_SERVICE           = 0x00000012;  // OpenService 出错

static const DWORD CONTROL_COMMAND_ERROR_START                  = 0x00000021;  // control_command_start_service 执行失败
static const DWORD CONTROL_COMMAND_ERROR_START_TIMEOUT          = 0x00000022;  // control_command_start_service 执行超时

static const DWORD CONTROL_COMMAND_ERROR_STOP                   = 0x00000031;  // control_command_stop_service 执行失败
static const DWORD CONTROL_COMMAND_ERROR_STOP_TIMEOUT           = 0x00000031;  // control_command_stop_service 执行超时

static const DWORD CONTROL_COMMAND_ERROR_PAUSE                  = 0x00000041;  // control_command_pause_service 执行失败
static const DWORD CONTROL_COMMAND_ERROR_PAUSE_TIMEOUT          = 0x00000042;  // control_command_pause_service 执行超时

static const DWORD CONTROL_COMMAND_ERROR_CONTINUE               = 0x00000051;  // control_command_continue_service 执行失败
static const DWORD CONTROL_COMMAND_ERROR_CONTINUE_TIMEOUT       = 0x00000052;  // control_command_continue_service 执行超时

static const DWORD CONTROL_COMMAND_ERROR_QUERY_STATUS           = 0x00000061;  // control_command_query_service_status 执行失败
static const DWORD CONTROL_COMMAND_ERROR_QUERY_STATUS_TIMEOUT   = 0x00000062;  // control_command_query_service_status 执行超时

//消息队列
static const int MSG_QUEUE_SIZE = 1024;             ///< 消息队列的长度

//模块类型定义,大类:4
//static const DWORD SYS_INFO                = 40000; ///< 系统
////小类:40
//static const DWORD MT_CLIENT               = 40100; ///< 客户端
//static const DWORD MT_CLIENT_PREVIEW       = 40101; ///< 预览组件
//static const DWORD MT_CLIENT_TVWALL        = 40102; ///< 电视墙组件
//static const DWORD MT_CLIENT_PLAYBACK      = 40103; ///< 回放组件
//static const DWORD MT_CLIENT_ALARMCENTER   = 40104; ///< 接警中心组件
//static const DWORD MT_CLIENT_EMAP          = 40105; ///< 静态地图组件
//static const DWORD MT_CLIENT_GISMAP        = 40106; ///< GIS地图组件
//static const DWORD MT_CLIENT_STATUSCONTROL = 40107; ///< 控制与状态组件
//static const DWORD MT_CLIENT_LOG           = 40108; ///< 日志组件

////小类:41
//static const DWORD MT_SERVICE_DVR          = 41200; ///< DVR服务
//static const DWORD MT_SERVICE_SC           = 41201; ///< 防护舱服务
//static const DWORD MT_SERVICE_AC           = 41202; ///< 门禁服务
//static const DWORD MT_SERVICE_TALK         = 41203; ///< 对讲服务
//static const DWORD MT_SERVICE_INTEL        = 41204; ///< 智能服务
//static const DWORD MT_SERVICE_AH           = 41205; ///< 报警主机服务
//static const DWORD MT_SERVICE_ENV          = 41206; ///< 环境量服务
//static const DWORD MT_SERVICE_PROCESS      = 41207; ///< 联动服务
//static const DWORD MT_SERVICE_NETWORK      = 41208; ///< 网管服务
//static const DWORD MT_SERVICE_MULTIMEDIA   = 41209; ///< 多媒体服务
//static const DWORD MT_SERVICE_FIRE         = 41210; ///< 消防服务
//static const DWORD MT_SERVICE_EVENT        = 41211; ///< 事件服务
//static const DWORD MT_SERVICE_CMD          = 41212; ///< 信令服务
//static const DWORD MT_SERVICE_STREAM       = 41213; ///< 流媒体服务
//static const DWORD MT_SERVICE_NVR          = 41214; ///< NVR存储服务
//static const DWORD MT_SERVICE_PROXY        = 41215; ///< 代理服务
//static const DWORD MT_SERVICE_TVWALL       = 41217; ///< 电视墙服务
//static const DWORD MT_SERVICE_INTER_AREA   = 41218; ///< 级联服务
//static const DWORD MT_SERVICE_FILE         = 41219; ///< 文件服务
//static const DWORD MT_SERVICE_BW           = 41220; ///< 带宽控制服务
//static const DWORD MT_SERVICE_DOWNLOAD     = 41221; ///< 下载服务
//static const DWORD MT_SERVICE_VLB          = 41222; ///< 负载均衡服务
//static const DWORD MT_SERVICE_VRM          = 41223; ///< VRM服务器
//static const DWORD MT_SERVICE_EH_PROXY     = 41224; ///< E家代理服务器
//static const DWORD MT_SERVICE_EH_ALARM     = 41225; ///< E家报警服务器
//static const DWORD MT_SERVICE_EH_STREAM    = 41226; ///< E家流媒体服务器
//static const DWORD MT_SERVICE_EH_PIC_TRANS = 41227; ///< E家图片中转服务器
static const DWORD MT_SERVICE_DC           = 41228; ///< 数据中心服务器

////小类42
//static const DWORD MT_WEB                  = 42000; ///< WEB客户端

////消息类型定义
//static const DWORD MESSAGE_TYPE_2PARAM     = 1;     ///< 两个填充参数
//static const DWORD MESSAGE_TYPE_SVC_CTRL   = 2;     ///< 服务控制信息

//static const DWORD SVCMSG_CONTENT_LEN      = 1024;  ///< 即时消息内容最大长度
//static const DWORD SVCMSG_RESULT_LEN       = 16;    ///< 即时消息结果最大长度

static const DWORD SVC_CTRL_START_SERVICE  = 30001;      ///< 开始服务
static const DWORD SVC_CTRL_STOP_SERVICE   = 30002;      ///< 停止服务

// Service Config File
static const CString SVC_CFG_FILE           = _T("SvcCfg.dat");
// SysInfo and ProcInfo XML CString
static const CString XML_HEAD               = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
static const CString XML_ROOT               = _T("ROOT");
static const CString XML_SERVER             = _T("SERVER");
static const CString XML_NET_WORK_INFO      = _T("NETWORK_INFO");
static const CString XML_NET_WORK           = _T("NETWORK");
static const CString XML_IP_ADD             = _T("IpAddress");
static const CString XML_GATE_WAY           = _T("Gataway");
static const CString XML_OS_INFO            = _T("OS_INFO");
static const CString XML_START_TIME         = _T("START_TIME");
static const CString XML_MEMORY             = _T("MEMORY");
static const CString XML_CPU                = _T("CPU");
static const CString XML_HARD_DISK          = _T("HARDDISK");
static const CString XML_ADAPTER            = _T("ADAPTER");

static const CString XML_PROCESS_INFO       = _T("PROCESS_INFO");
static const CString XML_PRCOESS            = _T("PROCESS");

// instant msg list
static const WORD MAX_COL_NUM               = 3;      ///< list MAX column count
static const CString COL_WIDTH_INFO_FILE    = _T("ColWidthInfo.dll");
static const CString XML_LIST_COL_INFO      = _T("LIST_COL_INFO");
static const CString XML_COL_INFO           = _T("COL_INFO");
static const CString XML_MODULE_TYPE        = _T("ModuleType");
static const CString XML_COL_ONE            = _T("ColOne");
static const CString XML_COL_TWO            = _T("ColTwo");
static const CString XML_COL_THREE          = _T("ColThree");

// CollectorProcessor para
static const CString XML_DB_ROOT               = _T("DBConfig");
static const CString XML_DB_SERVER             = _T("DBHost");
static const CString XML_DB_SERVER_PORT        = _T("DBPort");
static const CString XML_DB_SERVER_USER        = _T("DBUser");
static const CString XML_DB_SERVER_PASSWORD    = _T("DBPassword");
static const CString XML_DB_SERVER_SCHEMA      = _T("DBSchema");
// CentorProcessor para
static const CString XML_CENTER_PROCESSOR_ROOT = _T("CentorProcessor");
static const CString XML_ALARM_SERVER          = _T("AlarmCenter");
static const CString XML_ALARM_PORT            = _T("AlarmPort");
static const CString XML_LOCAL_IP              = _T("LocalIP");
static const CString XML_LOCAL_PORT            = _T("LocalPort");
static const CString XML_DEFAULT_ALARM_SERVER  = _T("127.0.0.1");
static const CString XML_DEFAULT_ALARM_PORT    = _T("9567");
static const CString XML_DEFAULT_LOCAL_IP      = _T("127.0.0.1");
static const CString XML_DEFAULT_LOCAL_PORT    = _T("80");

static const DWORD ALARM_UPLOAD_CENTER_FALSE  = 0; ///< 报警没有上传中心
static const DWORD ALARM_UPLOAD_CENTER_TRUE  = 1;  ///< 报警已上传中心
static const DWORD GET_ALARMS_NUM  = 50;           ///< 获取报警条数

typedef enum _DLG_FLAGS
{
    SVC_LIST_DLG = 0,   ///< 服务列表
    VIEW_MSG_DLG = 1,   ///< 即时消息
    SVC_CFG_DLG  = 2,   ///< 服务配置
}DLG_FLAGS;

#pragma pack(8)
////回调消息
//typedef struct STRUC_BASIC_MESSAGE{        
//    DWORD    dwSvcType;
//    DWORD    dwMsgType;
//    STRUC_BASIC_MESSAGE()
//    {
//        dwSvcType = 0;
//        dwMsgType = 0;
//    }
//} BASIC_MESSAGE, *pBASIC_MESSAGE;

//回调消息
typedef struct STRUC_BASIC_MESSAGE_EX : public BASIC_MESSAGE{
    char   chContent[SVCMSG_CONTENT_LEN];
    char   chResult[SVCMSG_RESULT_LEN];
    STRUC_BASIC_MESSAGE_EX()
    {
        chContent[0] = '\0';
        chResult[0] = '\0';
    }
} BASIC_MESSAGE_EX, *pBASIC_MESSAGE_EX;

/* @struct tagServiceMsgInfo [GenDef.h]
 * @brief  服务的即时消息列表信息
 */
typedef struct tagServiceMsgInfo 
{
    int nTabIdx;            ///< Tab序列号
//    int nListID;            ///< list的ID

    DWORD nModuleType;      ///< 服务器类型号
    std::string strSrvName;      ///< 服务器显示名

    CSkinListCtrl2 *pListCtrl;   ///< 消息显示列表

    tagServiceMsgInfo()
    {
        nTabIdx = 0;
        //nListID = -1;
        nModuleType = 0;
        pListCtrl = NULL;
    }
}SERVICE_INFO, *PLSERVICE_INFO;

/* @struct STURCT_COL_WIDTH_INFO [GenDef.h]
 * @brief  即时消息列表的列宽信息
 */
typedef struct STURCT_COL_WIDTH_INFO
{  
    DWORD nModuleType;      ///< 服务器类型号
    //std::string strSrvName;      ///< 服务器显示名
    int szColWidth[MAX_COL_NUM];

    STURCT_COL_WIDTH_INFO()
    {
        nModuleType = 0;
        memset(szColWidth, 0, MAX_COL_NUM);
    }
}COL_WIDTH_INFO, *LP_COL_WIDTH_INFO;

/** struct STRU_DISK_INFO [GenDef.h]
 *  brief 磁盘信息
 */
typedef struct STRU_DISK_INFO
{
    int totalNum;           // 磁盘容量(GB)
    int UsingNum;           // 已用容量(GB)
    char disk;              // 磁盘名
    char res[3];
    STRU_DISK_INFO()
    {
        totalNum = 0;
        UsingNum = 0;
        disk = 0;
        memset(res, 0, sizeof(res));
    }
}DISK_INFO, *LPDISK_INFO;

/** @struct STRU_IP_ADDRESS [GenDef.h]
 *  @brief IP信息
 */
typedef struct STRU_IP_ADD_INFO
{
    CString IpAdd;  ///< ip地址
    CString csMask; ///< 子网掩码
}IP_ADD_INFO, *LPIP_ADD_INFO;

/* @struct STRU_NETWORK_INFO [GenDef.h]
 * @brief 网络信息
 */
typedef struct STRU_NETWORK_INFO
{
    char IpType[8];                                 ///< 网络类型
    std::vector<IP_ADD_INFO> vecIPAddAndSubNet;      ///< ip地址和子网掩码
    std::vector<CString> vecGateway;                ///< 网关

    STRU_NETWORK_INFO()
    {
        IpType[0] = 0;
    }
}NET_WORK_INFO, *PNET_WORK_INFO;

/* @struct STRU_ADAPTER_INFO [GenDef.h]
 * @brief 适配器信息
 */
typedef struct STRU_ADAPTER_INFO
{
    int WorkUsing;
    CString LinkSpeed;
    CString AdapterName;
    CString AdapterStatus;
    STRU_ADAPTER_INFO()
    {
        WorkUsing = 0;
    }
}ADAPTER_INFO, *PADAPTER_INFO;

/* @struct STRU_SERVER_SYSINFO [GenDef.h]
 * @brief 服务器信息
 */
typedef struct STRU_SERVER_SYSINFO
{
    int OsType;         ///< 操作系统类型 32/64
    int TotalMem;       ///< 总内存大小(MB)
    int UsingMem;       ///< 已使用内存大小(MB)
    int CpuSysWorking;  ///< CPU(%)

    CString SN;         ///<
    CString HostName;   ///< 主机名
    CString OsInfo;     ///< 操作系统信息
    CString CpuInfo;    ///< CPU信息
    CString startTime;  ///< 开机时间
    CString csServiceManagerStartTime;  ///< Service Manager 的启动时间
    
    std::vector<NET_WORK_INFO> NetWorkInfo;
    std::vector<DISK_INFO> DiskInfo;
    std::vector<ADAPTER_INFO> AdapterInfo;

    STRU_SERVER_SYSINFO()
    {
        OsType = 0;
        TotalMem = 0;
        UsingMem = 0;
        CpuSysWorking = 0;
    }

}SERVER_SYS_INFO, *PSERVER_SYS_INFO;

/* @struct STRU_PROCESS_INFO [GenDef.h]
 * @brief 服务进程信息
 */
typedef struct STRU_SERVER_PROCESS_INFO
{
    int ModuleType;             ///< 服务类型
    int ThreadCount;            ///< 线程数
    int HandleCount;            ///< 句柄数
    int MemWorkingSize;         ///< 内存使用大小(KB)
    int CpuWorking;             ///< CPU占用率(%)
    int ListenPort;             ///< 监听端口
    int ServerStatus;           ///< 服务状态
    CString ServerName;         ///< 服务名
    CString ProcName;           ///< 进程名
    CString startTime;          ///< 启动时间
    STRU_SERVER_PROCESS_INFO()
    {
        ModuleType = -1;
        ThreadCount = 0;
        HandleCount = 0;
        MemWorkingSize = 0;
        CpuWorking = 0;
        ListenPort = 0;
        ServerStatus = 0;
    }
}SERVER_PROCESS_INFO, *PSERVER_PROCESS_INFO;

/*  @struct STRU_RECV_CMD_MSG [GenDef.h]
 *  @brief  接收到的控制命令信息
 */
typedef struct STRU_RECV_CMD_MSG
{
    int nCtrlType;          ///< 控制命令
    int nModuleType;        ///< 服务模块类型
    std::string svcName;    ///< 服务名
    std::string shellCmd;   ///< cmd命令
    std::string shellDir;   ///< cmd命令的默认执行路径
    std::string remoteIP;   ///< 发送命令的服务的IP地址

    STRU_RECV_CMD_MSG()
    {
        nCtrlType = -1;
        nModuleType = 0;
    }
}RECV_CMD_MSG, *LPRECV_CMD_MSG;

struct dummy_msg_t {
	size_t cbMsgSize;
	void *lpMsg;
};

typedef struct _collector_info_t {
	std::_tstring ip;
	HPR_INT32 port;
}collector_info_t;

typedef struct _center_info_t {
	std::_tstring ip;
	HPR_INT32 port;
}center_info_t;

typedef struct _alarmer_info_t {
	std::_tstring ip;
	HPR_INT32 port;
	_alarmer_info_t()
	{
		ip = _T("127.0.0.1");
		port = 9567;
	}
}alarmer_info_t;

typedef struct _work_state_t {
	HPR_INT32 nKey;
	HPR_INT32 nParentKey;
	HPR_INT32 nState;
	HPR_INT32 nUserID;
	HPR_INT32 nChannel;
	HPR_INT32 nRev;
	HPR_INT32 nLastOccureTime;
	HPR_INT32 nOccureTime;
	HPR_INT64 nLastAlarmID;
	//HPR_INT64 nCurrAlarmID;
	std::_tstring strKeyValue;
	std::_tstring strOccureTime;
	std::_tstring strInfo;
}work_state_t;

typedef struct _alarm_msg_t {
	HPR_UINT32 nID;
	HPR_INT32 nKey;
	HPR_INT32 nParentKey;
	HPR_INT32 nState;
	HPR_INT32 nUserID;
	HPR_INT32 nChannel;
	HPR_INT32 nRev;
	HPR_INT32 nOccureTime;
	HPR_INT32 nport;
	HPR_INT32 nCenterFlag;
	HPR_INT64 nLastAlarmID;
	HPR_INT64 nCurrAlarmID;
	std::_tstring strOccureTime;
	std::_tstring strInfo;
	std::_tstring strGUID;
	std::_tstring strIP;
}alarm_msg_t;

typedef struct _tag_MsgTTSAlarm
{
	HPR_INT32 nAlarmType;
	HPR_INT32 nPort;
	char szIP[16];
	char szTime[64];
	char szGUID[128];
	char szAlarmInfo[512];
	_tag_MsgTTSAlarm()
	{
		nAlarmType = -1;
		nPort = 80;
		memset(szIP, 0, 16);
		memset(szTime, 0, 64);
		memset(szGUID, 0, 128);
		memset(szAlarmInfo, 0, 512);
	}
}MsgTTSAlarm, *pMsgTTSAlarm;

#endif
