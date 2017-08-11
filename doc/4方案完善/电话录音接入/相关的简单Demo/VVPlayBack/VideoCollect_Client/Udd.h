#ifndef GUARD_UDD_H
#define GUARD_UDD_H

/**	@file    Udd.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   user defined data
 *
 *	@author	 xuezhihu
 *	@date	 2013/1/25
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#include <cassert>
#include <ctime>
#include <fcTchar.h>
#include <HPR_Types.h>

static const HPR_INT32 CENTER_STORAGE_TYPE_CVR = 1101;//中心走CVR存储
static const HPR_INT32 CENTER_STORAGE_TYPE_CLOUD = 1100;//中心走CLOUD存储

static const DWORD TP_ALARM_INIT = 1;//初始化相关
static const DWORD TP_ALARM_DB = 2;//数据库相关
static const DWORD TP_ALARM_FILE = 3;//上传文件操作相关
static const DWORD TP_ALARM_STORAGE = 4;//中心存储相关
static const DWORD TP_ALARM_CENTER = 5;//中心web相关

typedef struct _fileToTrans {
    HPR_INT32     id;
    std::_tstring strLinkOrderID; // 中心数据库中的流水号
    std::_tstring channelID;      // cvr 上使用的标示符
    std::_tstring filePath;
    HPR_INT32     regionID;
    HPR_INT32     mediaType;      // video or audio
    HPR_INT32     mediaFormat;    // hikmp4, wav, mp3
    HPR_INT32     headSize;       // 媒体头大小
    HPR_INT32     callType;       // call in/out
    std::_tstring callNo;
	std::_tstring strUserNo;
    HPR_INT32     nUserID;
    HPR_INT32     linkType;       // 0-普通记录 1-交易记录 2-投诉记录
    HPR_INT32     fileBegintime;
    HPR_INT32     fileEndtime;
    std::_tstring dtBusinessTime;
    HPR_INT32     state;
    HPR_INT32     uploadPercent;
    HPR_INT32     iTaskType;     //任务类型 0一般录像 1补录
    std::_tstring strVrmIp;      //Vrm服务 IP
    HPR_INT32     iVrmPort;      //Vrm服务 端口号
    HPR_INT32     iVodPort;      //Vod服务 端口号
    std::_tstring strDvrIp;
    HPR_INT32     iDvrChannel;
	std::_tstring strRecordName;
    std::_tstring strDvrUserName; //dvr登录用户名
    std::_tstring strDvrPasswd;   //dvr登录密码
    HPR_INT32     iDvrPort;       //dvr登录通道号
    HPR_INT32     iManufacturerType; //DVR厂商0-海康1-大华
    std::_tstring strMac;       //文件所在机器网卡的MAC地址
}fileToTrans_t;

typedef struct _fileTransing {
    HPR_INT32     recordID; // records中的流水号
    HPR_INT32     regionID;
    HPR_INT32     mediaType;
    HPR_INT32     mediaFormat;
    HPR_INT32     callType;
    HPR_INT32     linkType;
    std::_tstring callNo;
    std::_tstring strLinkOrderID;
    std::_tstring strUserNo;
    std::_tstring strRecordName;

    std::_tstring sceneID; // not use
    std::_tstring channelID;
    std::_tstring filePath;
    HPR_INT32     headSize;
    HPR_INT64     startOffset;
    HPR_INT32     segBegintime;
    HPR_INT32     segEndtime;
    HPR_INT32     segNo;
    HPR_INT32     uploadPercent;
	HPR_INT32     tryTimes;
    HPR_INT32     iTaskType;     //任务类型 0一般录像 1补录
    std::_tstring strVrmIp;      //Vrm服务 IP
    HPR_INT32     iVrmPort;      //Vrm服务 端口号
    HPR_INT32     iVodPort;      //Vod服务 端口号
    std::_tstring strDvrIp;
    HPR_INT32     iDvrChannel;
    std::_tstring strDvrUserName; //dvr登录用户名
    std::_tstring strDvrPasswd;   //dvr登录密码
    HPR_INT32     iDvrPort;       //dvr登录通道号
    HPR_INT32     iManufacturerType; //DVR厂商0-海康1-大华
    _fileTransing()
        : recordID(0)
        , sceneID(_T(""))
        , channelID(_T(""))
        , filePath(_T(""))
        , headSize(0)
        , startOffset(0)
        , segBegintime(0)
        , segEndtime(0)
        , segNo(0)
        , uploadPercent(0)
        , tryTimes(1)
        , iTaskType(0)
        , strVrmIp(_T(""))
        , iVrmPort(0)
        , iVodPort(0)
        , strDvrIp(_T(""))
        , iDvrChannel(0)
        , strDvrUserName(_T(""))
        , strDvrPasswd(_T(""))
        , iDvrPort(0)
        , iManufacturerType(0)
    {}
    _fileTransing(const fileToTrans_t &fileInfo, HPR_INT64 offset,
        HPR_INT32 segNumber)
        : recordID(fileInfo.id)
        , regionID(fileInfo.regionID)
        , callType(fileInfo.callType)
        , linkType(fileInfo.linkType)
        , mediaType(fileInfo.mediaType)
        , mediaFormat(fileInfo.mediaFormat)
        , callNo(fileInfo.callNo)
        , strUserNo(fileInfo.strUserNo)
        , strRecordName(fileInfo.strRecordName)
        , strLinkOrderID(fileInfo.strLinkOrderID)

        , channelID(fileInfo.channelID)
        , filePath(fileInfo.filePath)
        , headSize(fileInfo.headSize)
        , startOffset(offset)
        , segBegintime(fileInfo.fileBegintime)
        , segEndtime(fileInfo.fileEndtime)
        , segNo(segNumber)
        , uploadPercent(fileInfo.uploadPercent)
        , tryTimes(1)
        , iTaskType(fileInfo.iTaskType)
        , strVrmIp(fileInfo.strVrmIp)
        , iVrmPort(fileInfo.iVrmPort)
        , iVodPort(fileInfo.iVodPort)
        , strDvrIp(fileInfo.strDvrIp)
        , iDvrChannel(fileInfo.iDvrChannel)
        , strDvrUserName(fileInfo.strDvrUserName)
        , strDvrPasswd(fileInfo.strDvrPasswd)
        , iDvrPort(fileInfo.iDvrPort)
        , iManufacturerType(fileInfo.iManufacturerType)
    {}
}fileTransing_t;

typedef struct _FileDel
{
    HPR_INT32     nID;     // 在表里的流水号
    std::_tstring strFilePath;
    _FileDel()
        :nID(-1)
        ,strFilePath(_T(""))
    {}
}FileDel_t, *pFileDel_t;

typedef struct _storage_info_t {
	HPR_INT32 type;
	HPR_INT32 partitionid;
	std::_tstring id;
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	std::_tstring devicecode;
}storage_info_t;

typedef struct _cvr_info_t {
    std::_tstring id;
    std::_tstring host;
    std::_tstring port;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
}cvr_info_t;

typedef struct _cloud_info_t {
	std::_tstring id;
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	std::_tstring devicecode;
	HPR_INT32 poolid;
}cloud_info_t;

typedef struct _collector_info_t {
    std::_tstring ip;
    HPR_INT32 port;
}collector_info_t;

typedef struct _center_info_t {
	std::_tstring ip;
	HPR_INT32 port;
}center_info_t;

// 工作参数
typedef struct _work_param_t {
    std::_tstring startTime; // 开始工作时间，形如 01:00:00
    std::_tstring stopTime; // 停止工作时间，形如 23:00:00
}work_param_t;

//KMS信息
typedef struct _Struct_KMSInfo
{
    std::_tstring strKMSIP;
    std::_tstring strKMSPort;
    std::_tstring strKMSUserId;
    std::_tstring strKMSPasswd;
}Struct_KMSInfo;
//NAS信息
typedef struct _Struct_NASInfo
{
    std::_tstring strNASIP;
    std::_tstring strNASPort;
    std::_tstring strNASUserId;
    std::_tstring strNASPasswd;
}Struct_NASInfo;
// 判断是否刚刚刷新过的工具
class CRefresher 
{
public:
    CRefresher()
        :m_updateTime(0)
    {}
    // 是否刚刚刷新过
    bool IsJustRefresh(int const nInterval)
    {
        assert(nInterval > 0);
        return (::time(NULL) - m_updateTime) < nInterval;
    }
    // 刷新一下时间
    void Refresh(void)
    {
        m_updateTime = ::time(NULL);
    }
private:
    time_t m_updateTime; 
};

#endif // end of GUARD_UDD_H

