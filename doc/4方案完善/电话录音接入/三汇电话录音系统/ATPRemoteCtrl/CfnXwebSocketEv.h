// SYATP.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifndef __CFNXWEBSOCKETEV_H__
#define __CFNXWEBSOCKETEV_H__


#ifdef __cplusplus
extern "C"
{
#endif

enum SyAtpEventCode
{
	E_START	= 0xff,
	E_GetHardwareInfo,	//获取硬件信息
	E_GetChannelStatus,	//获取通道状态
	E_GetDiskFreeSpace,	//获取磁盘剩余空间
	E_StartMonitor,		//开始监听
	E_StopMonitor,		//停止监听
	E_RebootDevice,		//重启设备
	E_SetDateTime,		//设置时间
	E_GetGlobalCfg,		//读取全局配置
	E_SetGlobalCfg,		//设置全局配置
	E_GetChannelCfg,	//获取通道配置
	E_SetChannelCfg,	//设置通道配置
	E_SetNetworkCfg,	//设置网络信息
	E_GetLineVol,		//获取线路电压
	E_DelFile,			//删除文件
	E_DelFiles,			//批量删除文件
	E_StopDelFile,		//停止批量删除文件
	E_EnableDebugMsg,	//开启debug模式
	E_StartUploadFile,	//开始上传文件
	E_SetAdminPassword,	//设置用户名密码
	E_SetNetTimeout,	//设置超时时间
	E_Connect,			//连接
	E_Disconnect,		//断开连接
	E_CaculateData,
	E_WriteDVD,
	E_CheckMediaAvailable,
	E_CheckMediaConnected,
	E_CALL_STANDBY,
	E_CALL_PICKUPED,
	E_CALL_RINGING,
	E_CALL_TALKING,
	E_CALL_PENDING,
	E_CALL_OFFLINE,
	E_CALL_UNAVAILABLE,
	E_LOW_DISK_SPACE,
	E_GETCHANNELRUNNINGSTATUS,
	E_GETDEVICETIME,
	E_GETDEVICEID,
	E_LONGCALLALERT,
	E_OUTCALL_DTMF,
	E_INCALL_CALLERID,
	E_MISSEDCALL,
	E_ONRECORDING,
	E_SETWEBPORT,	
	E_GETWEBPORT,
	E_Unknown,			//未知命令
	E_MAX,
};


typedef enum DVDMediaType{
	E_DVD_R=10,
	E_DVD_RW
}E_DVDMediaType;

/* 所有通道状态*/
#pragma pack(1)
/* 发送通道状态变化的事件*/
typedef struct _tagRecEventInfo{
	int nCh;
	char szIPAddr[16];
	char szEventTime[20];
}RecEventInfo,*PRecEventInfo;

typedef struct _tagCallerIDEvent{
	int nCh;
	char szIPAddr[16];
	char szCallerID[128];
}CallerIDEvent,*PCallerIDEvent;

typedef struct _tagDTMFEvent{
	int nCh;
	char szIPAddr[16];
	char cDtmf;
}DTMFEvent,*PDTMFEvent;

typedef struct _tagMissedCallEvent
{
	int nCh;
	char szIPAddr[16];
	char szCallerID[128];
}MissedCallEvent,*PMissedCallEvent;

typedef struct _tagOnRecordingEvent{
	int nCh;
	char szIPAddr[16];	
	char szEventTime[20];
	char szBindPhoneNum[128];
}OnRecordingEvent,*POnRecordingEvent;

/* 硬件信息*/
typedef struct _tagHARDWARE_INFO{
	int  iTotalChannels;//总通道数
	char szBoardType[20];//板卡类型
	char szIpAddress[20];//IP地址
	char szGateway[20];//网关
	char szMacaddress[20];//MAC地址
	char szMask[20];//子网 掩码
	char szDns[20];//DNS
	char szDevId[50];//设备号
	char szWebVersion[30];
	char szRecSvrVersion[30];
	char szKernelVersion[100];
	char szCpldVersion[10];
	char szSqlVersion[10];
}HARDWARE_INFO,*PHARDWARE_INFO;

typedef struct _tagTotalChStatus{
	char cTotalCh;//通道总数
	int iStartChannel;//起始通道
	char szChStatus[25];//通道状态
}TotalChStatus,*PTotalChStatus;

typedef struct _tagChRunStatus{
	char cChID ;//通道号
	char cChStatus;//通道状态
	signed char cRecDir;//录音方向 call direction
	char szDtmf[200];//DTMF
	DWORD dwRecLen;//录音时间 hook-on time
	char szRingTime[50];//振铃时间
	char cRingCnt;//振铃计数
	char szCallerID[128];//主叫号码
	char szCalleeID[128];//被叫号码
}ChRunStatus,*PChRunStatus;

typedef struct _tagGlobalCfg
{
	int iCompress;       //压缩格式
	int iFileFormat;   //文件保存格式	
	int iStartChannel;//开始通道
	int iTotalChannels;//通道总数
	char szRecPath[260];//录音路径硬盘和sd卡切换
	char cSetReboot;//设置自动重启
	int iRebootHour;//重启小时
	int iRebootMin;//重启分
	char szDevRebootTime[50];//重启时间
}GlobalCfg,*PGlobalCfg;

typedef struct _tagChannelCfg{
	int iChID;//通道号
	int iRecType;//录音类型0压控1声控 2码控3一直录音4压控(只录通话内容)
	int iRecDir;//录音方向0 只录来话1 只录去话2 全部都录3 全部都不录
	int iLongRecInterval;//长录音的间隔必须小于等于最长录音时间 单位分
	int iMaxReclen;//最大录音时间单位分
	int iRecvDtmfTime;//接收DTMF时间(当为码控时必须设置为0)
	char szBindPhoneNumber[128];//绑定号码
	char szStartDtmf[32];//码控开始录音的DTMF 只有码控时才有效
	char szStopDtmf[32];//码控停止录音的DTMF
}ChannelCfg,*PChannelCfg;
typedef struct _tagIPConfig{
	char szIPAddr[16] ;//IP地址
	char szMask[16] ;//子网掩码
	char szGateWay[16] ;//默认网关
	char szDNS1[16] ;//DNS
}IPConfig,*PIPConfig;
//所有通道电压
typedef struct _tagTotalChVol{
	int iTotalCh;//通道总数
	int iStartCh;//开始通道
	char szChVol[100];//通道电压
}TotalChVol,*PTotalChVol;
/* 监听信息*/
typedef struct _tagMonitorInfo{
	int iChannel;
	char szRemoteIp[20];
}MonitorInfo,*PMonitorInfo;
/* 设备时间*/
typedef struct _tagDevTime{
	char szDevTime[64];
}DevTime,*PDevTime;
//删除范围
typedef struct _tagDelRange{
	char szStartTime[20];
	char szStartDate[20];
	char szEndDate[20];
	char szEndTime[20];
	int iStartCh;
	int iEndCh;
}DelRange,*PDelRange;

typedef struct _tagPasswordInfo{
	char szOldPass[21];
	char szNewPass[21];
}PasswordInfo,*PPasswordInfo;

typedef struct _tagDiskSpace
{
	char szDiskType[20];	//磁盘类型名称sd/hd
	char szDiskName[128];	//磁盘名称
	int  iDiskTotalSpace;	//磁盘总空间，单位是MB
	int iDiskFreeSpace;	//磁盘空闲的空间，单位是MB
	char szDiskType1[20];	//磁盘类型名称sd/hd
	char szDiskName1[128];	//磁盘名称
	int  iDiskTotalSpace1;	//磁盘总空间，单位是MB
	int iDiskFreeSpace1;	//磁盘空闲的空间，单位是MB
	char szDiskType2[20];	//磁盘类型名称sd/hd
	char szDiskName2[128];	//磁盘名称
	int  iDiskTotalSpace2;	//磁盘总空间，单位是MB
	int iDiskFreeSpace2;	//磁盘空闲的空间，单位是MB
}DiskSpace,*PDiskSpace;

typedef struct _tagDateInfo{
	int iYear;
	unsigned char ucMonth;
	unsigned char ucDay;
}DateInfo;
typedef struct _tagCalculateInput{
	char cSelectType;//0 date range  1 monthwise
	DateInfo stStartDate;
	DateInfo stEndDate;
	DateInfo stMonthwise;
	int mediaType;
}CalculateInput,*PCalculateInput;
typedef struct _tagCalculateResult{
	unsigned int iTotalCalls;
	unsigned int iTotalSizeM;
	unsigned int iTotalSIzeKB;
	char cSizeAvailable;
}CalculateResult,*PCalculateResult;

typedef struct _tagDiskFullEvent{
	char	szDiskIndex[20];
	DWORD dwDiskFreeSpace;
	DWORD	dwDiskSpaceMin;	
	char szEventTime[20];
}DiskFullEvent,*PDiskFullEvent;
#pragma pack()

typedef struct _SYATP_EVENT
{
	WORD		wEventCode;		// event code
	int			nReference;		// reference
	PCHAR		pcRemoteAddr;	// pointer to remote addr
	PVOID		pvBuffer;		// pointer to buffer
	DWORD		dwBufferLength;	// length of buffer pointed by pvBuffer

}SYATP_EVENT, *PSYATP_EVENT;

enum eRebootCmd
{
	REBOOT_DEVICE,
	RESTART_DEVICE
};


int GetEvent(PSYATP_EVENT pSyAtp_Event);
int Connect(const char* pcSvrAddr,const char *pcUsername,const char *pcPassword,const char* pcLocalAddr,int nLocalPort);
int Disconnect(const char* pcRemoteAddr);
int SetNetTimeout(const char* pcRemoteAddr,int nSecond);
int GetHardwareInfo(const char *pcRemoteAddr);
int GetChannelStatus(const char *pcRemoteAddr);
int GetDiskFreeSpaceB(const char *pcRemoteAddr);
int StartMonitor(const char *pcRemoteAddr,int nCh,const char *pcMonitorAddr);
int StopMonitor(const char *pcRemoteAddr,const char *pcMonitorAddr);
int RebootDevice(const char *pcRemoteAddr,enum eRebootCmd eCmd);
int SetDateTime(const char *pcRemoteAddr,const char *pcTime);
int GetGlobalCfg(const char *pcRemoteAddr);
int SetGlobalCfg(const char *pcRemoteAddr,int nCompress,int nFileFormat,int nStartCh,char cSetReboot,int rebootHour,int rebootMin);
int GetChannelCfg(const char *pcRemoteAddr,int nCh);
int SetChannelCfg(const char *pcRemoteAddr,int nCh,int nLongRecInterval,int nMaxReclen,int nRecDir,int nRecType,int iRecvDtmfTime,const char * pcBindPhoneNum,const char *pcStartDtmf,const char*pcStopDtmf);
int SetNetworkCfg(const char *pcRemoteAddr,const char *pcIPaddr,const char *pcMask,const char *pcGateWay,const char *pcDNS);
int GetLineVol(const char *pcRemoteAddr);
int DelFile(const char *pcRemoteAddr,const char *pcFileName);
int DelFiles(const char *pcRemoteAddr,int nStartCh,int nEndCh,const char *pcStartDate,const char *pcEndDate,const char *pcStartTime,const char *pcEndTime);
int StopDelFile(const char *pcRemoteAddr);
int EnableDebugMsg(const char *pcRemoteAddr,unsigned short usLogLevel);
int StartUploadFile(const char *pcRemoteAddr);
int SetAdminPassword(const char *pcRemoteAddr,const char *pcOldPassword,const char *pcNewPassword);
int UploadFile(const char *pcIPAddr,const char *pcLocalFilePath);
int DownloadFile(const char *pcIPAddr,const char *pcFileName,const char *pcSaveFileName);
int WriteDVD(const char* pcRemoteAddr);
int CheckMediaAvailable(const char* pcRemoteAddr);
int CheckMediaConnected(const char* pcRemoteAddr);
int CalculateData(const char *pcRemoteIp,PCalculateInput pstInput);
int GetChannelRunningStatus(const char *pcRemoteAddr,int nCh);
int GetDeviceID(const char *pcRemoteAddr);
int GetDeviceTime(const char * pcRemoteAddr);
int SetWebPort(const char *pcRemoteAddr,int iWebPort);
int GetWebPort(const char *pcRemoteAddr);

#ifdef __cplusplus
}
#endif

#endif
