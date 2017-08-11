/**	@file GlobalParam.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 全局的参数配置。
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once

#include <string>

#define HPP_WORKER_THREAD	 10	//HPP工作线程数
#define HPP_TIMEOUT			 10	//HPP超时时间.单位秒

#define TOP_DIRECTORY_CONFIG_FILE   "ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

#define NODE_PARAM_UPLOAD   "PARAM_UPLOAD"
#define NODE_PARAM_ATSERVER "PARAM_ATSERVER"
#define NODE_PARAM_COLLECT  "PARAM_COLLECT"

//登录结果
typedef enum _loginResult
{
    LOGIN_RESULT_FAIL = -1,//登录失败
    LOGIN_RESULT_OK = 0,//登录成功
}ENUM_LOGIN_RESULT;

//录像类型
enum ENUM_RECORD_TYPE
{
	RECORD_TYPE_NORMAL        = 1, //手动录像
	RECORD_TYPE_REPAIR        = 2, //补录录像
	RECORD_TYPE_MAX           = 3, 
};

//录像状态
enum ENUM_RECORD_STATUS
{
    RECORDSTAT_READY        = 2, //待上传
    RECORDSTAT_UPLOADING    = 3, //上传中
    RECORDSTAT_OVER         = 4, //上传完成
    RECORDSTAT_FAIL         = 9, //上传失败
    RECORDSTAT_DELETE       = 100,//删除
};

//录像存储类型
enum ENUM_RECORD_STORAGE_TYPE
{
    RECORD_STORAGETYPE_NORMAL       = 1,//DVR录像
    RECORD_STORAGETYPE_LOCAL_KMS    = 2,//本地录像上传KMS
	RECORD_STORAGETYPE_OBJECT_CLOUD = 3,//本地录像上传云存储：对象存储模式
};

//文件类型
enum ENUM_FILE_TYPE
{
	FILE_TYPE_VIDEO       = 1,//录像
	FILE_TYPE_AUDIO       = 2,//录音
	FILE_TYPE_PICTURE     = 3,//图片
	FILE_TYPE_OTHER       = 4,//其它
};

//存储器类型
enum ENUM_STORAGE_TYPE
{
    STORAGE_TYPE_CVR = 0,      //CVR
    STORAGE_TYPE_CLOUD,        //云存储
    STORAGE_TYPE_KMS,          //KMS
	STORAGE_TYPE_OBJECT_CLOUD, //云存储：对象存储
    STORAGE_TYPE_MAX
};

static const DWORD TP_ALARM_INIT = 1;//初始化相关
static const DWORD TP_ALARM_DB = 2;//数据库相关
static const DWORD TP_ALARM_FILE = 3;//上传文件操作相关
static const DWORD TP_ALARM_STORAGE = 4;//中心存储相关
static const DWORD TP_ALARM_CENTER = 5;//中心web相关

//存储设备信息
typedef struct _Struct_StorageConfig
{
    ENUM_STORAGE_TYPE   storageType;    //存储设备类型
//    LONG                lLoginID;       //登录句柄
    std::string         strIp;          //存储设备ip
    int                 nPort;          //存储设备端口
    std::string         strUser;        //存储设备用户名
    std::string         strPwd;         //存储设备密码
	std::string         strAccessKey;   //对象存储Access Key(对象云存储专用)
	std::string         strSecretKey;   //对象存储Secret Key(对象云存储专用)
//     std::string         strStartTime;   //存储设备开始工作时间
//     std::string         strStopTime;    //存储设备停止工作时间
    int                 nPoolId;        //存储设备录像卷号
    _Struct_StorageConfig()
    {
        storageType = STORAGE_TYPE_MAX;
        nPort = 0;
        nPoolId = 0;
    }
}Struct_StorageConfig;

typedef struct _Struct_RecordTime
{
    DWORD	dwYear;					//年
    DWORD	dwMonth;				//月
    DWORD	dwDay;					//日
    DWORD	dwHour;					//时
    DWORD	dwMinute;				//分
    DWORD	dwSecond;				//秒
	_Struct_RecordTime()
	{
		dwYear   = 0;
		dwMonth  = 0;
		dwDay    = 0;
		dwHour   = 0;
		dwMinute = 0;
		dwSecond = 0;
	}
}Struct_RecordTime;

//录像信息
typedef struct _Struct_RecordInfo
{
    ENUM_RECORD_STATUS       recordStatus;       //录像状态
    ENUM_RECORD_STORAGE_TYPE recordStorageType;  //录像存储类型   
	ENUM_FILE_TYPE          fileType;           //文件类型(对象云存储专用)
    int                     nID;                //在数据库中的流水号(对象云存储专用)
    int                     nUserID;            //用户ID
    std::string             strDvrIp;           //录像关联的DVRIP
    int                     nDvrPort;           //录像关联的DVR端口
    std::string             strDvrUser;         //DVR用户名
    std::string             strDvrPwd;          //DVR密码
    int                     nChannelID;         //DVR通道号
    Struct_RecordTime       struRecStartTime;   //录像开始时间
    Struct_RecordTime       struRecStopTime;    //录像结束时间
    std::string             strFilePath;        //本地文件路径
    std::string             strCameraID;        //(云存储:关联的编码器ID，CVR:关联的流ID，对象云存储：文件Key)
    int                     nTaskID;            //任务ID
    int                     nUploadPercent;     //上传进度
    int                     nFailTimes;         //上传失败次数
    std::string             strRtspPath;        //上传文件的RTSP路径（KMS专用）
    std::string             strStartTime;       //上传文件的开始时间（KMS、对象云存储专用）
	std::string             strEndTime;         //上传文件的结束时间（KMS、对象云存储专用）
	std::string             strHost;            //本地文件所在主机地址  (对象云存储专用)
	std::string             strBucket;          //对象存储Bucket(对象云存储专用)
    std::string             strRecordName;      //记录名称
    int                     nFileSize;          //本地录像文件大小
    Struct_StorageConfig    struStorage;        //存储设备信息
    _Struct_RecordInfo()
    {
        recordStatus = RECORDSTAT_READY;
        recordStorageType = RECORD_STORAGETYPE_NORMAL;
		fileType = FILE_TYPE_VIDEO;
		nID = -1;
        nUserID = 0;
        nDvrPort = 0;
        nChannelID = -1;
        nTaskID = -1;
        nUploadPercent = 0;
        nFailTimes = 0;
        nFileSize = 0;
    }
}Struct_RecordInfo;

//存储设备查询到的回传录像信息
typedef struct _Struct_UploadRecordParam
{
	Struct_RecordTime       struRecStartTime;   //录像开始时间
	Struct_RecordTime       struRecStopTime;    //录像结束时间
	DWORD                   dwRecordNum;        //录像个数
	BYTE                    byLockFlag;         //锁定标志 0：没锁定 1：锁定
	DWORD                   dwRecordLength;     //录像总大小
	_Struct_UploadRecordParam()
	{
		dwRecordNum    = 0;
		byLockFlag     = 0;
		dwRecordLength = 0;
	}
}Struct_UploadRecordParam;
