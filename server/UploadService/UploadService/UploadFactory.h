#pragma once
#include "stdafx.h"
#include <list>
#include <queue>
#include <map>
#include <time.h>
#include <ATLComTime.h>
#include "../util/Singleton.h"
#include "../util/Guard.h"
#include "Center/Records.pb.h"
#include "Center/StorageConfig.pb.h"
//#include "Collect/Users.pb.h"
#include "HCNetSDK/include/HCNetSDK.h"

class CUploadFactory
{
    SINGLETON_DECLARE(CUploadFactory)///<单例相关声明
public:
    CUploadFactory(void);
    virtual ~CUploadFactory(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Init();

    /**	@fn	    Fini
	*	@brief	反初始化函数，负责资源释放
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Fini();

	/**	@fn	    Start
	*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
	*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Start();

	/**	@fn	    Stop
	*	@brief	停止函数
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Stop();

public:
    /**	@fn	    AddRecordsList
    *	@brief	添加录像信息列表
    *	@param  [in] recordsList -- 录像信息列表
    *   @param  [in] bFinish -- 是否结束
    *	@return	
    */
    void AddRecordsList(const cms_8120::RecordsList &recordsList, BOOL bFinish = true);

	/**	@fn	    DeleteLocalRecordFiles
    *	@brief	删除本地录像文件
    *	@param  [in] recordsList -- 录像信息列表
    *	@return	
    */
    void DeleteLocalRecordFiles(const cms_8120::RecordsList &recordsList);

    /**	@fn	    HandleStorage
    *	@brief	处理存储设备信息
    *	@param  [in] storage -- 存储设备信息
    *   @param  [in] bFinish -- 是否结束
    *	@return	
    */
    void HandleStorage(const cms_8120::RspStorageConfig &storage, BOOL bFinish = true);

    /**	@fn	    RunCycleUpload[线程函数]
	*	@brief	上传录像
	*	@param  [in] 无
	*	@return	
	*/
	void RunCycleUpload();

    /**	@fn	    RunCycleGetStatus[线程函数]
	*	@brief	获取状态
	*	@param  [in] 无
	*	@return	
	*/
	void RunCycleGetStatus();

    /**	@fn	    RunCycleGetRecords[线程函数]
    *	@brief	获取录像
    *	@param  [in] 无
    *	@return	
    */
    void RunCycleGetRecords();

    /**	@fn	    RunCycleHeartBeat[线程函数]
    *	@brief	心跳
    *	@param  [in] 无
    *	@return	
    */
    void RunCycleHeartBeat();

    /**	@fn	    SetUploadPercent
    *	@brief	设置上传进度(KMS专用)
    *	@param  [in] nPercent -- 上传进度
    *	@return	
    */
    void SetUploadPercent(int nPercent);

private:
    /**	@fn	    isWorkingTime
    *	@brief	判断是否在工作时间内
    *	@param  [in] strStartTime -- 开始时间
    *   @param  [in] strStopTime -- 结束时间
    *	@return	int, -1表示错误，0表示不在工作时间内，1表示在工作时间内
    */
    int isWorkingTime(const std::string &strStartTime, const std::string &strStopTime);

    /**	@fn	    convertToCTime
    *	@brief	转换时间类型（string-COleDateTime）
    *	@param  [in] strStartTime -- 开始时间
    *   @param  [in] strStopTime -- 结束时间
    *	@param  [out] startTime -- 开始时间
    *   @param  [out] stopTime -- 结束时间
    *	@return	BOOL
    */
    BOOL convertToCTime(const std::string &strStartTime, const std::string &strStopTime,
        COleDateTime &startTime,
        COleDateTime &stopTime);

    /**	@fn	    convertRecordInfo
    *	@brief	转换录像信息
    *	@param  [in] recordInfo -- 原录像信息
    *   @param  [out] struRecord -- 转换后的录像信息
    *	@return	BOOL
    */
    BOOL convertRecordInfo(const cms_8120::Records &recordInfo, Struct_RecordInfo &struRecord);

    /**	@fn	    uploadFailManage
    *	@brief	上传失败处理
    *   @param  [in] recordInfo -- 录像信息
    *   @param  [in] strErrorMsg -- 错误信息
    *   @param  [out] nFailTimes -- 上传失败次数
    *	@return	void
    */
    void uploadFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes);

	/**	@fn	    lockFailManage
    *	@brief	锁定失败处理
    *   @param  [in] recordInfo -- 录像信息
    *   @param  [in] strErrorMsg -- 错误信息
    *   @param  [out] nFailTimes -- 锁定失败次数
    *	@return	void
    */
    void lockFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes);

    /**	@fn	    setRecordTime
    *	@brief	设置时间信息
    *	@param  [in] nTime -- 时间戳
    *	@param  [out] struTime -- 时间信息
    *	@return	BOOL
    */
    BOOL setRecordTime(const std::string &strTime, Struct_RecordTime &struTime);

    /**	@fn	    uploadOver
    *	@brief	上传完成处理
    *	@param  [in] struRecordInfo -- 录像信息
    *	@param  [in] strTip-- 提示信息
    *	@return	BOOL
    */
    BOOL uploadOver(const int &nRecordID, const Struct_RecordInfo &struRecordInfo, std::string strTip = "");

	/**	@fn	    loginDvr
	*	@brief	登录Dvr
	*	@param  [in] struStorage -- 设备信息
	*	@return	LONG -- 登录句柄
	*/
	LONG loginDvr(const Struct_RecordInfo &struRecordInfo);

	/**	@fn	    lockDvrRecord
    *	@brief	锁定DVR录像
    *	@param  [in] struRecordInfo -- 录像信息
    *	@return	BOOL
    */
    BOOL lockDvrRecord(const Struct_RecordInfo &struRecordInfo);

	/**	@fn	    getDvrTimeStamp
	*	@brief	得到时间戳
	*	@param  [in]  stDvrTimeSt -- 时间
	*	@param  [out] iTime -- 时间戳
	*	@return	void
	*/
	void getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime);

	/**	@fn	    getRecordTimeStamp
	*	@brief	得到录像时间戳
	*	@param  [in]  stRecordTime -- 录像时间
	*	@param  [out] iTime -- 时间戳
	*	@return	void
	*/
	void getRecordTimeStamp(const Struct_RecordTime &stRecordTime, time_t &iTime);

	/**	@fn	    setUserTips
    *	@brief	设置提示
	*	@param  [in] struRecordInfo -- 录像信息
    *	@param  [in] struRecordParam -- 已上传到存储设备中的录像信息
    *	@param  [out] strTip-- 提示信息
    *	@return	BOOL
    */
    BOOL setUserTips(const Struct_RecordInfo &struRecordInfo, const Struct_UploadRecordParam &struRecordParam, std::string &strTip);

private:
    bool m_bInit;                       //是否被初始化过
    bool m_bStart;                      //是否启动过
    volatile bool m_bStopWorkerThread;  //停止工作线程

    HANDLE m_hUploadThread;     //上传线程句柄
    HANDLE m_hGetStatusThread;  //获取上传状态线程句柄
    HANDLE m_hGetRecordsThread; //获取录像信息线程句柄
    HANDLE m_hHeartBeatThread;  //心跳线程
//    HANDLE m_hGetDataSucc;//获取数据成功事件

    CRITICAL_SECTION m_csListRecords;                     //录像信息列表锁
    std::list<cms_8120::Records> m_listRecords;          //录像信息队列

    CRITICAL_SECTION m_csMapRecordStatus;                  //录像状态列表锁
    std::map<int, Struct_RecordInfo> m_mapRecordStatus;    //录像状态列表

    HANDLE m_hStorageEvent;                                //存储设备信号量
    Struct_StorageConfig m_struStorage;                    //存储设备信息  
    BOOL m_bStorageReturn;                                 //获取存储设备接口的返回值

};
