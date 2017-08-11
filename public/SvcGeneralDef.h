#ifndef GUARD_SVCGENERALDEF_H
#define GUARD_SVCGENERALDEF_H
#pragma once

/**	@file    SvcGeneralDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   DLL即时消息定义共用头文件
 *
 *	@author	 yuchengzhi
 *	@date	 2012/8/22
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V1.0.0  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

//模块类型定义,大类:
static const DWORD SYS_INFO      = 6000; // 系统
static const DWORD SVC_RECGUARD  = 6001; // 录像守卫模块
static const DWORD SVC_ADJUST    = 6002; // 校时模块
static const DWORD SVC_TRANS     = 6003; // 上传模块
static const DWORD SVC_VRM       = 6004; // VRM模块

static const DWORD SERVER_TYPE_ADJUSTTIME    = 20001;// 系统巡检服务
static const DWORD SERVER_TYPE_MEDIAGUARD    = 20002;// 录像守卫服务
static const DWORD SERVER_TYPE_TRANSPORT     = 20004;// 上传服务

static const DWORD DEV_TYPE_DVR                = 50000 ; // DVR
static const DWORD DEV_TYPE_ALARM_HOST         = 50001 ; //报警主机
static const DWORD DEV_TYPE_GATE               = 50002 ; //门禁
static const DWORD DEV_TYPE_TALK               = 50003 ; //对讲
static const DWORD DEV_TYPE_DECODER            = 50004 ; //解码器
static const DWORD DEV_TYPE_VIP                = 50005 ; //视频综合平台
static const DWORD DEV_TYPE_SC                 = 50006 ; //防护舱(Safety Cabin)
static const DWORD DEV_TYPE_IPC                = 50007 ; // IPC
static const DWORD DEV_TYPE_DVS                = 50008 ; // DVS
static const DWORD DEV_TYPE_PCDVR              = 50009 ; // PCDVR
static const DWORD DEV_TYPE_AH_NETWORK_MODULE  = 50010 ; //报警主机网络模块
static const DWORD DEV_TYPE_CVR                = 50011 ; // CVR
static const DWORD DEV_TYPE_NVR                = 50012 ; // NVR
static const DWORD DEV_TYPE_ENVIRONMENT        = 50013 ; // 环境量
static const DWORD DEV_TYPE_FIRE               = 50014 ; // 消防
static const DWORD DEV_TYPE_CLOUD_STORAGE      = 50015 ; // 云存储
static const DWORD DEV_TYPE_GUN                = 50016 ; // 枪弹柜(昆明红河邮政)
static const DWORD DEV_TYPE_YS7		           = 50017 ; //萤石云设备
static const DWORD DEV_TYPE_KMS                = 50050 ; // KMS


//消息类型定义
static const DWORD MSG_TYPE_NOTIFY   = 1;   // 普通消息
static const DWORD MSG_TYPE_ALARM    = 2;   // 报警消息
static const DWORD MSG_TYPE_DOWNLOAD = 3;   // 下载进度
static const DWORD MSG_TYPE_UPLOAD   = 4;   // 上传进度

//报警类型
static const DWORD WORK_TYPE_CENTTER_OFFLINE           = 60001; // 中心不在线        (未用)
static const DWORD WORK_TYPE_TRANSPORT_SVC_OFFLINE     = 61001; // 上传服务不在线    (未用)
static const DWORD WORK_TYPE_TRANSPORT_SVC_FAIL        = 61002; // 上传服务失败
static const DWORD WORK_TYPE_GUARD_SVC_OFFLINE         = 62001; // 录像守卫服务不在线 (未用)
static const DWORD WORK_TYPE_GUARD_SVC_FAIL            = 62002; // 录像守卫服务失败
static const DWORD WORK_TYPE_ADJUST_SVC_OFFLINE        = 63001; // 巡检服务不在线     (未用)
static const DWORD WORK_TYPE_ADJUST_SVC_FAIL           = 63002; // 系统巡检服务失败   
static const DWORD WORK_TYPE_PATROL_SVC_FAIL           = 64001; // 采集设备巡检失败   (未用)
static const DWORD WORK_TYPE_DVR_OFFLINE               = 64002; // 采集设备不在线
static const DWORD WORK_TYPE_DVR_TIME_ERROR            = 64003; // 采集设备时间和本地时间差很大
static const DWORD WORK_TYPE_DVR_VI_LOST               = 64004; // 采集设备视频信号丢失
static const DWORD WORK_TYPE_DVR_HD_EXCEPTION          = 64005; // 采集设备硬盘异常
static const DWORD WORK_TYPE_DVR_OPERATION_FAIL        = 64006; // 采集设备某些操作失败  (未用)
static const DWORD WORK_TYPE_DVR_NEARLLY_FULL          = 64007; // 采集设备磁盘满
static const DWORD WORK_TYPE_CVR_OFFLINE               = 65001; // CVR不在线
static const DWORD WORK_TYPE_CVR_CANNOT_LOGIN          = 65002; // CVR登录失败           (未用)
static const DWORD WORK_TYPE_CVR_NEARLLY_FULL          = 65003; // CVR磁盘满
static const DWORD WORK_TYPE_CVR_OPERATION_FAIL        = 65004; // CVR某些操作失败       (未用)
static const DWORD WORK_TYPE_CLOUD_OFFLINE             = 66001; // Cloud不在线
static const DWORD WORK_TYPE_CLOUD_CANNOT_LOGIN        = 66002; // Cloud登录失败         (未用)
static const DWORD WORK_TYPE_CLOUD_GET_POOLLIST_FAIL   = 66003; // 获取录像池超时        (未用)
static const DWORD WORK_TYPE_CLOUD_NEARLLY_FULL        = 66004; // Cloud磁盘满
static const DWORD WORK_TYPE_CLOUD_OPERATION_FAIL      = 66005; // Cloud某些操作失败     (未用)
static const DWORD WORK_TYPE_KMS_OFFLINE               = 67001; // KMS不在线
static const DWORD WORK_TYPE_KMS_NEARLLY_FULL          = 67002; // KMS磁盘满
static const DWORD WORK_TYPE_KMS_OPERATION_FAIL        = 67003; // KMS某些操作失败       (未用)
static const DWORD WORK_TYPE_VRM_SVC_OFFLINE           = 68001; // VRM服务不在线         (未用)
static const DWORD WORK_TYPE_OBJECTCLOUD_OFFLINE       = 69001; // 标准云-对象存储不在线
static const DWORD WORK_TYPE_OBJECTCLOUD_NEARLLY_FULL  = 69002; // 标准云-对象存储磁盘满

static const DWORD ALL_USER = 0;
static const DWORD ALL_CHANNEL = 255;

static const DWORD ALARM_NORMAL = 0;//非报警
static const DWORD ALARM_OCCUR = 1;//报警

static const DWORD ALARM_REV_DEFAULT_VALUE = 0;

static const DWORD SVCMSG_CONTENT_LEN      = 1024; // 即时消息内容最大长度
static const DWORD SVCMSG_RESULT_LEN       = 16;   // IP地址最大长度

#pragma pack(8)

// 回调消息
typedef struct STRUC_BASIC_MESSAGE{        
    DWORD    dwSvcType;
    DWORD    dwMsgType;
    STRUC_BASIC_MESSAGE()
        : dwSvcType(0)
        , dwMsgType(0)
    {}
} BASIC_MESSAGE, *pBASIC_MESSAGE;

//回调消息
typedef struct STRUC_BASIC_MESSAGE_EX : public BASIC_MESSAGE{
    char   chContent[SVCMSG_CONTENT_LEN];
    char   chResult[SVCMSG_RESULT_LEN];
} BASIC_MESSAGE_EX, *pBASIC_MESSAGE_EX;

// 提醒消息
typedef struct NOTIFY_MSG : public BASIC_MESSAGE{
	DWORD  dwUserID;
	DWORD  dwChannel;
    char   szNotifyInfo[SVCMSG_CONTENT_LEN]; // utf8编码
} NOTIFY_MSG, *pNOTIFY_MSG;

// 报警消息
typedef struct ALARM_MSG_T : public BASIC_MESSAGE{
    DWORD  dwAlarmType;
    DWORD  dwStatus;
	DWORD  dwUserID;
	DWORD  dwChannel;
	DWORD  dwRev;
    char   szAlarmInfo[SVCMSG_CONTENT_LEN]; // utf8编码
    char   szHost[SVCMSG_RESULT_LEN];
    ALARM_MSG_T()
        : dwAlarmType(0)
        , dwStatus(0)
        , dwUserID(0)
        , dwChannel(0)
        , dwRev(0)
    {
        for (int i = 0; i < SVCMSG_CONTENT_LEN; i++)
        {
            szAlarmInfo[i] = 0;
        }
        for (int i = 0; i < SVCMSG_RESULT_LEN; i++)
        {
            szHost[i] = 0;
        }
    }
} ALARM_MSG, *pALARM_MSG;

// 上传录像进度消息
typedef struct UPLOAD_MSG : public BASIC_MESSAGE{
    DWORD dwRecordID;
	DWORD dwUserID;
    DWORD dwUploadPercent;
} UPLOAD_MSG, *pUPLOAD_MSG;

// 补录录像进度消息
typedef struct DOWNLOAD_MSG : public BASIC_MESSAGE{
    DWORD  dwRecordID;
	DWORD  dwUserID;
    DWORD  dwDownloadPercent;
} DOWNLOAD_MSG, *pDOWNLOAD_MSG;

// 服务心跳消息
typedef struct HAEARTBEAT_MSG : public BASIC_MESSAGE{
	DWORD  dwCurrentTime;
} HAEARTBEAT_MSG, *pHAEARTBEAT_MSG;

//消息回调函数原型
typedef DWORD (__stdcall *pTransMsgBack)(const BASIC_MESSAGE *struMessage);
typedef DWORD (__stdcall *pTransMsgBackEx)(const void *msg, const size_t cbMsg);
//typedef DWORD (__stdcall *pMsgBack)(const BASIC_MESSAGE *struMessage);
//typedef DWORD (__stdcall *pMsgBackEx)(const void *msg, const size_t cbMsg);

#endif    // end of GUARD_SVCGENERALDEF_H
