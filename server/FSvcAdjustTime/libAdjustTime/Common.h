/**	@file    Common.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   user defined data
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#ifndef GUARD_COMMON_H
#define GUARD_COMMON_H
#include <string>

//函数返回值
#define ADJUST_ERROR_SUCC 0 //成功
#define ADJUST_ERROR_FAIL 1 //通用失败
#define ADJUST_ERROR_DB   2 //数据库操作失败
#define ADJUST_ERROR_CFG  3 //配置相关错误
#define ADJUST_ERROR_DVR  4 //DVR操作相关错误
#define ADJUST_ERROR_HPP  5 //HPP操作相关错误

//更新策略
typedef enum _adjustmode {
    ADJUST_TIMING_MODE   = 0, //每天定时校时
    ADJUST_INTERVAL_MODE = 1, //固定时间间隔校时
    ADJUST_EXEC_ONCE = 2,     //只执行一次
}adj_mode_en;

static const DWORD TP_ALARM_INIT = 1;//初始化相关
static const DWORD TP_ALARM_DB = 2;//数据库相关
static const DWORD TP_ALARM_FILE = 3;//上传文件操作相关
static const DWORD TP_ALARM_STORAGE = 4;//中心存储相关
static const DWORD TP_ALARM_CENTER = 5;//中心web相关
static const DWORD TP_ALARM_DVR  = 6;//DVR操作相关
static const DWORD TP_ALARM_ADJUST = 7;//系统巡检服务相关
static const DWORD TP_ALARM_CVR    = 8;//CVR操作相关
static const DWORD TP_ALARM_CLOUD  = 9;//CLOUD操作相关
static const DWORD TP_ALARM_KMS    = 10;//KMS操作相关
static const DWORD TP_ALARM_OBJECTCLOUD = 11;//标准云对象存储操作相关


#define DEVICE_LOGIN_FAILED -1

//配置信息
typedef struct _configinfo {
    adj_mode_en  enAdjustMode;     //更新策略
    unsigned int uiTimingHour;     //校时时间 时  每日定时校时策略用
    unsigned int uiTimingMin;      //校时时间 分  每日定时校时策略用
    unsigned int uiTimingSec;      //校时时间 秒  每日定时校时策略用
    unsigned int uiInterval;       //校时时间间隔 小时 固定时间间隔校时策略用
    unsigned int uiMaxTimeLag;     //dvr和本地允许自动校时的最大时间差 单位分钟
}config_info_t;

//配置信息
typedef struct _dvrinfo {
    std::string   strDvrIp;    //ip
    unsigned short  usPort;      //端口
    std::string   strUserName; //用户名
    std::string   strPassWd;   //密码
}collect_info_t;

//存储器类型
enum ENUM_STORAGE_TYPE
{
	STORAGE_TYPE_CVR           = 50011,   //CVR
	STORAGE_TYPE_CLOUD         = 50015,   //云存储
	STORAGE_TYPE_KMS           = 50050,   //KMS
	STORAGE_TYPE_OBJECTCLOUD   = 50060,   //标准云-对象存储
	STORAGE_TYPE_MAX
};

//存储设备信息
typedef struct _Struct_StorageDevice
{
	int                 nType;          //存储设备类型
	std::string         strName;        //存储设备名字
	std::string         strIp;          //存储设备ip
	int                 nPort;          //存储设备端口
	std::string         strUser;        //存储设备用户名
	std::string         strPwd;         //存储设备密码
	std::string         strAccessKey;   //对象存储Access Key
	std::string         strSecretKey;   //对象存储Secret Key
	int                 nReginId;       //存储设备所属区域流水号
	_Struct_StorageDevice()
	{
		nType    = 0;
		nPort    = 0;
		nReginId = 0;
	}
}Struct_StorageDevice;


#define HPP_WORKER_THREAD	 10	//HPP工作线程数
#define HPP_TIMEOUT			 10	//HPP超时时间.单位秒

#define TOP_DIRECTORY_CONFIG_FILE   "./ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

#define NODE_PARAM_TIMING     "PARAM_TIMING"
#define NODE_PARAM_ALARM      "PARAM_ALARM"

//登录结果
typedef enum _loginResult
{
	LOGIN_RESULT_FAIL = -1,//登录失败
	LOGIN_RESULT_OK = 0,//登录成功
}ENUM_LOGIN_RESULT;


static const int CLOUD_GET_NEXT_POOL_SUCCESS   = 1; //获取信息成功
static const int CLOUD_GET_NEXT_POOL_NEED_WAIT = 2; //正在查询请等待
static const int CLOUD_GET_NEXT_POOL_FINISH    = 3; //查询结束

static const int ONLINE  = 1;
static const int OFFLINE = 0;


#endif //#define GUARD_COMMON_H
