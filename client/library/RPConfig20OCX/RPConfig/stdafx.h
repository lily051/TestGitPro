#pragma once

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//lint -library
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxctl.h>         // ActiveX 控件的 MFC 支持
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

// 如果不希望使用 MFC 数据库类，
//  请删除下面的两个包含文件
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#endif // _WIN64
#include <afxwin.h>

// hlog.
#include "hlog.h"
#include "hlogex.h"
#include "RPCLog.h"
#include "HCNetSDK.h"
//////////////////////////////////////////////////////////////////////////
//初始化库参数定义
typedef struct _tagInitLibParam 
{
    CString sLogPath;//日志路径
}INIT_LIB_PARAM;

//操作码
enum
{
    CODE_GET_DEV_INFO           = 8100,//获取设备信息
    CODE_GET_CHANNEL_NAME       = 8101,//获取通道名称
    CODE_SHOW_REMOTE_CONFIG     = 8102,//远程配置
    CODE_SET_NTPCONFIG
};

//设备类型
enum
{
    DEV_TYPE_HIK                = 0,//海康设备
	DEV_TYPE_CVR                = 1,//CVR
	DEV_TYPE_CLOUD              = 2,//云存储
};

// typedef struct _tagNTPConfigInfo
// {
//     CString sNTPServer; //NTP服务器域名或者IP地址 
//     int wInterval;       //校时间隔时间，以分钟或小时为单位 
//     int byEnableNTP;     //NTP校时是否启用：0－否，1－是 
//     int NtpPort;         //NTP服务器端口，设备默认为123 
//     _tagNTPConfigInfo()
//     {
//         sNTPServer.Empty();
//         wInterval = 0;
//         byEnableNTP = 1;
//         NtpPort = 123;
//     }
// }NTPCONFIGINFO;

//函数参数
typedef struct _tagFunParam 
{
    int nCode;//操作码
    int nDevType;//设备类型
    CString sDevIP;//设备IP
    int nDevPort;//设备端口号
    CString sDevUser;//用户名
    CString sDevPWD;//密码
    int nChannel;//通道号
    BOOL bShowIPMode;//是否显示IP通道配置
    NET_DVR_NTPPARA nNtpConfigInfo;  //配置NTP校时
    _tagFunParam()
    {
        nCode = -1;
        nDevType = -1;
        nDevPort = 8000;
        nChannel = -1;
        bShowIPMode = FALSE;
        ZeroMemory(&nNtpConfigInfo, sizeof(NET_DVR_NTPPARA));
    }
}FUN_PARAM;

//远程设备信息
typedef struct _tagRPDeviceInfo
{
    CString strName;                           //设备名称
    int     nAlarmInNum;                   //DVR报警输入个数
    int     nAlarmOutNum;                  //DVR报警输出个数
    int     nDiskNum;                          //硬盘个数
    int     nChanNum;                          //通道个数
    int     nStartChanNum;                     //启始通道号
    CString strSerialNumber;				   //序列号
    int     nAudioNum;                          //语音通道数
    int     nIPChannel;                         // IP通道个数
    int     nIPChannelStart;                    // IP通道起始号
    _tagRPDeviceInfo()
    {
        nAlarmInNum = 0;
        nAlarmOutNum = 0;
        nDiskNum = 0;
        nChanNum = 0;
        nStartChanNum = 0;
        nAudioNum = 0;
        nIPChannel = 0;
        nIPChannelStart = 0;
    }
}RP_DEV_INFO;
//////////////////////////////////////////////////////////////////////////

