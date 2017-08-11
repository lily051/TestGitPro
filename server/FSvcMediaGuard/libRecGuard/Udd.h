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
#include <ctime>
#include <fclib/fcTchar.h>
//#include <hpr/include/HPR_Types.h>


// static const HPR_INT32 CENTER_STORAGE_TYPE_CVR = 0;//中心走CVR存储
// static const HPR_INT32 CENTER_STORAGE_TYPE_CLOUD = 1;//中心走CLOUD存储
// static const HPR_INT32 CENTER_STORAGE_TYPE_KMS = 2;//中心走KMS存储

typedef struct _cvr_info_t {
    HPR_INT32 nID; // 流水号
    std::_tstring nameID; // 名称标识
    std::_tstring host;
    std::_tstring port;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
    HPR_INT32 regionID; // 从属的区域ID
}cvr_info_t;

typedef struct _cloud_info_t {
	HPR_INT32 nID;// 流水号
	std::_tstring nameID; // 名称标识
	std::_tstring host;
	std::_tstring port;
	std::_tstring username;
	std::_tstring password;
	HPR_INT32 poolid;
	HPR_INT32 regionID; // 从属的区域IDs
}cloud_info_t;

typedef struct _storage_info_t {
    HPR_INT32     nID; // 流水号
    HPR_INT32     nType;  //存储器类型
    std::_tstring nameID; // 名称标识
    std::_tstring host;
    HPR_INT32     nPort;
    std::_tstring username;
    std::_tstring password;
    std::_tstring groupname;
	std::_tstring strAccessKey;
	std::_tstring strSecretKey;
    HPR_INT32 regionID; // 从属的区域ID
}storage_info_t;

// 存在中心的业务记录,clieninput
typedef struct _bz_record_t {
    HPR_INT32 recordID; // 业务数字ID
    HPR_INT32 tipFlag; // 提醒了多少天了
    std::_tstring bzID; // 业务系统ID
    std::_tstring bzTime; // 业务发生时间
    std::_tstring productName; // 产品名称
}bz_record_t;

// 存在中心的录像记录,prosaverecord
typedef struct _rec_record_t {
    HPR_INT32 ID; // 流水号
    HPR_INT32 recordID; // 录像关联的业务数字ID
    std::_tstring cameraID; // 录像在CVR上的ID
    HPR_INT32     cvrID; // cvr的标识，这里是名字
	std::_tstring strBucket; // 对象存储资源池
    std::_tstring startTime; // 录像开始时间
    std::_tstring stopTime; // 录像段结束时间
    bool bUpload;           // 是否上传成功
	HPR_INT32 devId;        //采集设备标识
	HPR_INT32 devChannel;   //采集设备通道
}rec_record_t;

// 工作参数
typedef struct _work_param_t {
	HPR_INT32 storageType; // 存储方式
	HPR_INT32 tipDays; // 几天的提示期
    std::_tstring startTime; // 开始工作时间
    std::_tstring stopTime; // 停止工作时间
    _work_param_t(void)
        : storageType(-1)
		, tipDays(-1)
    {
    }
}work_param_t;

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

