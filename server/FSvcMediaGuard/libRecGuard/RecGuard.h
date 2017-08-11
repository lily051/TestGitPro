#ifndef GUARD_RECGUARD_H
#define GUARD_RECGUARD_H
/**	@file    RecGuard.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   录像删除实现类
 *
 *	@author	 xuezhihu
 *	@date	 2013/3/20
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#pragma once
#include <ATLComTime.h>
#include <fclib/fcTchar.h>
#include <HPR_Singleton.h>
#include "CVR_Udd.h"
#include "Cloud_Udd.h"
#include "Udd.h"
#include "Center/OvertimeRecord.pb.h"
#include "Center/Sysconfig.pb.h"
#include "Center/Prosaverecord.pb.h"
#include "Center/Device.pb.h"
#include "HCNetSDK/include/HCNetSDK.h"
#include "hcs_sdk.h"
#include "hcs_object_api.h"

#define STORAGE_LOGIN_FAILED -1 //存储设备登录句柄

// work param
HPR_INT32 const WP_STORAGE_TYPE = 10009;
HPR_INT32 const WP_TIP_DAY = 6106;
HPR_INT32 const WP_STARTTIME = 6104;
HPR_INT32 const WP_STOPTIME = 6105;

class CRecGuard : public singleton<CRecGuard>
{
    friend class singleton<CRecGuard>;
private:
    CRecGuard(void);
    ~CRecGuard(void);
public:
    int Init(void);
    int Fini(void);
    int Start(void);
    int Stop(void);
    /**	@fn	    GetSysConfig
    *	@brief	获取中心系统信息
    *	@param  [in] stWorkParam -- 工作参数
    *	@param  [out] stWorkParam -- 工作参数
    *	@return	bool
    */
    bool GetSysConfig(work_param_t &stWorkParam);

    /**	@fn	    HandleStorage
    *	@brief	处理中心系统信息
    *	@param  [in] sysconfigList -- 存储设备信息
    *   @param  [in] bFinish -- 是否结束
    *	@return	
    */
    void HandleSysConfig(const cms_8120::SysconfigList &sysconfigList, BOOL bFinish = true);

    /**	@fn	    HandleOverRecord
    *	@brief	处理过期录像信息
    *	@param  [in] overtimeRecord -- 过期录像信息
    *   @param  [in] bFinish -- 是否结束
    *	@return	
    */
    void HandleOverRecord(const cms_8120::RspOvertimeRecord &overtimeRecord, BOOL bFinish = true);

    /**	@fn	    HandleRecord
    *	@brief	处理录像信息
    *	@param  [in] prosaverecordList -- 录像信息
    *   @param  [in] bFinish -- 是否结束
    *	@return	
    */
    void HandleRecord(const cms_8120::ProsaverecordList &prosaverecordList, BOOL bFinish = true);

    /**	@fn	    HandleStorageInfo
    *	@brief	处理存储器信息
    *	@param  [in] storage -- 存储器信息
    *	@return	bool
    */
    void HandleStorageInfo(const cms_8120::DeviceList &storage);

    /**	@fn	    GetStorageInfo
    *	@brief	获取存储信息
    *	@param  [in] storageID -- 存储器ID
    *	@param  [out] storage -- 存储器信息
    *	@return	bool
    */
    bool GetStorageInfo(const HPR_INT32 &storageID, storage_info_t &storage);
private:
    // 管理线程，主要是任务调度
    static DWORD WINAPI WT_Manage(LPVOID lpParam);
    DWORD ManageProc(void);

    // 心跳线程
    static DWORD WINAPI WT_HeartBeat(LPVOID lpParam);
    DWORD HeartBeatProc(void);

    // 工作线程，负责录像过期标记和删除逻辑
    static DWORD WINAPI WT_RecGuard(LPVOID lpParam);
    DWORD RecGuardProc(void);
    bool GetOldBzRecord(std::vector<bz_record_t> &bzRecords);
    bool DelOldBzRecordByIDs(const std::vector<HPR_INT32> &bzRecordIDs);
    bool GetRecRecord(const HPR_INT32 recordID, std::vector<rec_record_t> &recRecords);
    bool DelRecRecordByIDs(const int &nRecordID);
    bool ProcessOldBzRecord(const bz_record_t &bzRecord);
    bool ProcessRecRecord(const rec_record_t &recRecord);
    static bool GetTimeFromTimeString(LPCTSTR timeString, __time32_t &t);
    static bool GetHcsTimeFromTimeString(LPCTSTR timeString, HCS_Time &t);
    static bool GetNetTimeFromTimeString(LPCTSTR timeString, NET_DVR_TIME &t);

    bool MarkOldBzRecords(const int &nClientID);

    bool IsWorkingTime(void);

    bool ConvertToCTime(const work_param_t &workParam,
        COleDateTime &startTime,
        COleDateTime &stopTime);

    bool StartWorker(void);
    bool StopWorker(void);

    /**	@fn	    DelayBySleep
     *	@brief	通过循环sleep的方式等待一段时间
     *	@param  [in]dwMilliseconds, 需要等待的时间，单位毫秒
     *	@param  [in]stopFlag, 标志，如果为true则等待完dwMilliseconds，
     *                        否则会提前返回
     *	@param  [in]dwInterval, 等待间隔，单位毫秒
     *	@return	void
     */
    static void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);
   
    /**	@fn	    LoginCVR
     *	@brief	登录CVR
     *	@param  [in]struCVRInfo -- CVR参数
     *	@return	LONG -- 登录句柄
     */
    LONG LoginCVR(const storage_info_t& struCVRInfo);

    /**	@fn	    LockFileInCVR
     *	@brief	锁定CVR上的录像文件
     *	@param  [in]lLoginID -- 登录句柄
     *	@param  [in]strCameraID -- 流ID
     *	@param  [in]struStartTime -- 录像开始时间
     *	@param  [in]struStopTime -- 录像结束时间
     *	@return	bool
     */
    bool LockFileInCVR(const LONG& lLoginID, const std::string& strCameraID, 
        const NET_DVR_TIME& struStartTime, const NET_DVR_TIME& struStopTime);

    /** @fn GetHttpToken
    *  @brief  向KMS获取Token认证信息
    *  @param [in]struKMSInfo KMS的配置信息
    *  @param [out]strToken 获取到的token信息
    *  @return bool
    */
    bool GetHttpToken(const storage_info_t& struKMSInfo, std::string& strToken);

    /** @fn DeleteFileInKMS
    *  @brief  向KMS获取Token认证信息
    *  @param [in]struKMSInfo KMS的配置信息
    *  @param [in]strToken token信息
    *  @param [in]strFileId 文件ID
    *  @return bool
    */
    bool DeleteFileInKMS(const storage_info_t& struKMSInfo, const std::string& strToken, const std::_tstring& strFileId);

	 /** @fn UnLockDvrRecord
    *  @brief  解锁前端设备录像
    *  @param [in]struStorageInfo，设备登陆信息
	*  @param [in]recRecord，录像信息
    *  @return bool
    */
    bool UnLockDvrRecord(const storage_info_t& struStorageInfo, const rec_record_t &recRecord);

	/**	@fn	    getDvrTimeStamp
	*	@brief	得到时间戳
	*	@param  [in]  stDvrTimeSt -- 时间
	*	@param  [out] iTime -- 时间戳
	*	@return	BOOL
	*/
	void getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime);


public:
    work_param_t m_struWorkParam;   //中心系统参数
    std::vector<bz_record_t> m_vecBzRecord; //过期录像信息
    std::vector<rec_record_t> m_vecRecRecord;   //录像信息
    storage_info_t m_struStorage;               //存储设备
	std::map<std::string, HCSClient *> m_mapHCSClient;    //标准云-对象存储初始化句柄map，key：云存储IP

private:
    HANDLE m_hThreadManage;
    HANDLE m_hThreadHeartBeat;
    HANDLE m_hThreadCVRGuard;
    HANDLE m_hThreadRecGuard;
    volatile bool m_bStopWorker;
    volatile bool m_bStopServer;

    HANDLE m_hRecordEvent;      //录像信息
    HANDLE m_hWorkParamEvent;   //工作参数
    HANDLE m_hStorageDevEvent;  //存储设备
    HANDLE m_hOverRecordEvent;  //过期录像信息
};

inline CRecGuard* GetRecGuardPtr(void)
{
    return CRecGuard::instance();
}

#endif // end of GUARD_RECGUARD_H
