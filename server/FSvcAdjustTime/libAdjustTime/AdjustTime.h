/**	@file    AdjustTime.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   校时处理类
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
#ifndef GUARD_ADJUSTTIME_H
#define GUARD_ADJUSTTIME_H

#include <list>
#include <queue>
#include <map>
#include <sstream>
#include <time.h>
#include <ATLComTime.h>
#include <fclib/fcTchar.h>
#include <HCNetSDK.h>
#include "hcs_sdk.h"
#include "hcs_object_api.h"
#include <HPR_Singleton.h>
#include "Center/Device.pb.h"
#include "../lib/curl/curl.h"
#include "../lib/jsoncpp/include/json.h"

class CAdjustTime : public singleton<CAdjustTime>
{
    friend class singleton<CAdjustTime>;
private:
    CAdjustTime(void);
    ~CAdjustTime(void);

   /**	@fn	    CheckCVR
	*	@brief	检查CVR是否在线以及CVR容量是否足够
	*	@param  [in] struStorage -- CVR登陆信息
	*	@return	bool
	*/
	bool CheckCVR(const Struct_StorageDevice &struStorage);

   /**	@fn	    CheckCloud
	*	@brief	检查云存储是否在线以及云存储容量是否足够
	*	@param  [in] struStorage -- 云存储登陆信息
	*	@return	bool
	*/
	bool CheckCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    CheckKMS
	*	@brief	检查KMS是否在线以及KMS容量是否足够
	*	@param  [in] struStorage -- KMS登陆信息
	*	@return	bool
	*/
	bool CheckKMS(const Struct_StorageDevice &struStorage);

	 /**	@fn	    CheckObjectCloud
	*	@brief	检查对象云存储是否在线以及对象云存储容量是否足够
	*	@param  [in] struStorage -- 对象云存储登陆信息
	*	@return	bool
	*/
	bool CheckObjectCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    LoginCloud
	*	@brief	登录云存储
	*	@param  [in] struStorage -- 云存储登陆信息
	*	@return	LONG 云存储登录句柄
	*/
	LONG LoginCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    GetCloudPoolInfo
    *	@brief	获取Cloud数据池信息
    *	@param  [in] loginID,     云存储登陆句柄
    *	@param  [out]vecPoolInfo, 数据池信息容器
    *	@return	bool
    */
    bool GetCloudPoolInfo(LONG loginID, std::vector<HCS_PoolInfo>& vecPoolInfo);

	/**	@fn	    GetKmsSpaceInfo
	 *	@brief	获取KMS剩余容量及总容量大小
	 *	@param  [in]struStorage, KMS信息
	 *	@param  [in]strToken, token认证信息
	 *	@param  [out]dFreeSpace,  KMS剩余容量
	 *	@param  [out]dTotalSpace, KMS总容量
	 *	@return	bool
	 */
    bool GetKmsSpaceInfo(const Struct_StorageDevice &struStorage, const std::string& strToken, double& dFreeSpace, double& dTotalSpace);

	/**	@fn	    CheckCVRCapacity
	 *	@brief	检查CVR硬盘容量是否足够，并输出报警信息
	 *	@param  [in]struCvrHDInfo, CVR硬盘信息
	 *	@param  [out]alarmInfo, 报警信息
	 *	@return	void
	 */
	void CheckCVRCapacity(const NET_DVR_SINGLE_HD &struCvrHDInfo, std::_tstring &alarmInfo);

	/**	@fn	    CheckCloudCapacity
	 *	@brief	检查云存储容量是否足够，并输出报警信息
	 *	@param  [in]struPoolInfo, 云存储资源池信息
	 *	@param  [out]alarmInfo, 报警信息
	 *	@return	void
	 */
	void CheckCloudCapacity(const HCS_PoolInfo &struPoolInfo, std::_tstring &alarmInfo);

	/**	@fn	    CheckKmsCapacity
	 *	@brief	检查KMS容量是否足够，并输出报警信息
	 *	@param  [in]dFreeSpace,  KMS剩余容量
	 *	@param  [in]dTotalSpace, KMS总容量
	 *	@param  [out]alarmInfo, 报警信息
	 *	@return	void
	 */
    void CheckKmsCapacity(const double& dFreeSpace, const double& dTotalSpace, std::_tstring &alarmInfo);

	/**	@fn	    CheckObjectCloudCapacity
	 *	@brief	检查对象云存储容量是否足够，并输出报警信息
	 *	@param  [in]bucket_info, 对象云存储资源池信息
	 *	@param  [out]alarmInfo, 报警信息
	 *	@return	void
	 */
	void CheckObjectCloudCapacity(HCSBucketInfo* bucket_info, std::_tstring &alarmInfo);

   /** @fn GetHttpToken
	*  @brief  向KMS获取Token认证信息
	*  @param [in]struStorage KMS的配置信息
	*  @param [out]strToken 获取到的token信息
	*  @return bool
	*/
	bool GetHttpToken(const Struct_StorageDevice &struStorage, std::string& strToken);

	/**	@fn		TestRemoteHost
	*   @brief    测试远程主机是否在线
	*   @param  [in]  szIP, 远程主机IP地址
	*   @param  [in]  nPort,远程主机端口
	*   @return	BOOL, TRUE 在线 FALSE 不在线
	*/
	BOOL TestRemoteHost(const char *szIP, USHORT nPort);

   /**	@fn	    ConvertCollectInfo
	*	@brief	转换采集设备信息
	*	@param  [in] collectInfo -- 原采集设备信息
	*   @param  [out]stCollectCfg -- 转换后的采集设备信息
	*	@return	BOOL
	*/
	BOOL ConvertCollectInfo(const cms_8120::Device &collectInfo, collect_info_t &stCollectCfg);

   /**	@fn	    convertStorageInfo
	*	@brief	转换存储设备信息
	*	@param  [in] storageInfo -- 原存储设备信息
	*   @param  [out] struStorage -- 转换后的存储设备信息
	*	@return	BOOL
	*/
	BOOL ConvertStorageInfo(const cms_8120::Device &storageInfo, Struct_StorageDevice &struStorage);

   /** @fn     GetCollectTimeStamp
	*  @brief  获取采集设备当前时间的时间戳
	*  @param  [in]lUserId, 采集设备登陆句柄
	*  @param  [out]iCollectTimeStamp, 采集设备当前时间的时间戳
	*  @return 正常 ADJUST_ERROR_SUCC
			   异常 非 ADJUST_ERROR_SUCC
	*/
    int GetCollectTimeStamp(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError);

   /** @fn    SetCollectTime
	*  @brief  设置采集设备时间
	*  @param  [in]lUserId, 采集设备登陆句柄
	*  @param  [in]iCollectTimeStamp, 时间戳
	*  @param  [out]dwError, 异常码
	*  @return 正常 ADJUST_ERROR_SUCC
			   异常 非 ADJUST_ERROR_SUCC
	*/
    int SetCollectTime(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError);

   /** @fn     strTime
	*  @brief  时间戳转换为时间字符串
	*  @param  [in]t, 时间戳
	*  @return 时间字符串
	*/
	std::_tstring strTime(__time32_t const t);

   /**	@fn	    Deciphering
    *	@brief	解密操作
    *	@param  [in] strPwd -- 密文密码
    *	@param  [out] strPwd -- 明文密码
    *	@return	void
    */
    void Deciphering(std::string& strPwd);

    /**	@fn	    DelayBySleep
     *	@brief	通过循环sleep的方式等待一段时间
     *	@param  [in]dwMilliseconds, 需要等待的时间，单位毫秒
     *	@param  [in]stopFlag, 标志，如果为true则等待完dwMilliseconds，
     *                        否则会提前返回
     *	@param  [in]dwInterval, 等待间隔，单位毫秒
     *	@return	void
     */
    void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);

    //开始工作
    bool StartWorker(void);

public:

   /**	@fn	    GetAllCollectDeviceReq
	*	@brief	请求获取采集设备信息列表
	*	@param  [out] listCollectDevice -- 采集设备信息列表
	*   @param  [in] bFinish -- 是否结束
	*	@return	bool
	*/
	bool GetAllCollectDeviceReq(std::list<collect_info_t>& listCollectDevice);

   /**	@fn	    GetAllCollectDeviceRsp
	*	@brief	添加DVR设备信息列表
	*	@param  [in] deviceList -- DVR设备信息列表
	*   @param  [in] bFinish -- 是否结束
	*	@return	void
	*/
    void GetAllCollectDeviceRsp(const cms_8120::DeviceList &deviceList, BOOL bFinish = true);

   /**	@fn	    GetAllStorageDeviceReq
	*	@brief	请求获取存储设备信息列表
	*	@param  [out] listStorageDevice -- 存储设备信息列表
	*   @param  [in] bFinish -- 是否结束
	*	@return	bool
	*/
	bool GetAllStorageDeviceReq(std::list<Struct_StorageDevice>& listStorageDevice);

   /**	@fn	    GetAllStorageDeviceRsp
	*	@brief	添加存储设备信息列表
	*	@param  [in] deviceList -- 存储设备信息列表
	*   @param  [in] bFinish -- 是否结束
	*	@return	void
	*/
    void GetAllStorageDeviceRsp(const cms_8120::DeviceList &deviceList, BOOL bFinish = true);

public:
    /** @fn     Init
     *  @brief  初始化函数
     *  @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
     */
    int Init(void);

    /** @fn     Fini
     *  @brief  反初始化函数
     *  @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
     */
    int Fini(void);

    /** @fn     Start
     *  @brief  开始巡检
     *  @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
     */
    int Start(void);

	/** @fn     Stop
     *  @brief  停止巡检
     *  @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
     */
    int Stop(void);

   /** @fn	    ImmAdjustOnce
    *   @brief  立即执行一次校时操作
    *   @return 正常 ADJUST_ERROR_SUCC
                异常 非 ADJUST_ERROR_SUCC
    */
    int ImmAdjustOnce(void);

   /** @fn     AdjustTime
	*  @brief  从数据中心获取所有采集设备信息 对每个采集设备进行校时
	*  @return 正常 ADJUST_ERROR_SUCC
			   异常 非 ADJUST_ERROR_SUCC
	*/
    int AdjustTime(void);

   /** @fn     ManageProc
    *  @brief  任务管理线程
    *  @return 正常 ADJUST_ERROR_SUCC
               异常 非 ADJUST_ERROR_SUCC
    */
    DWORD ManageProc(void);

   /** @fn    AdjustTimeProc
	*  @brief  采集设备校时线程
	*  @return 正常 ADJUST_ERROR_SUCC
			   异常 非 ADJUST_ERROR_SUCC
	*/
    DWORD AdjustTimeProc(void);

   /** @fn     AddAlarmProc
	*  @brief  报警添加线程
	*  @return 正常 ADJUST_ERROR_SUCC
			   异常 非 ADJUST_ERROR_SUCC
	*/
    DWORD AddAlarmProc(void);

   /** @fn     DisAlarmProc
	*  @brief  报警处理线程
	*  @return 正常 ADJUST_ERROR_SUCC
		异常 非 ADJUST_ERROR_SUCC
	*/
    DWORD DisAlarmProc(void);

   /** @fn    CollectPatrolProc
	*  @brief  采集设备巡检线程
	*  @return 正常 ADJUST_ERROR_SUCC
		异常 非 ADJUST_ERROR_SUCC
	*/
    DWORD CollectPatrolProc(void);

   /** @fn     StoragePatrolProc
	*  @brief  存储设备巡检线程，检测设备在线状态，设备容量大小
	*  @return 正常 ADJUST_ERROR_SUCC
		异常 非 ADJUST_ERROR_SUCC
	*/
	DWORD StoragePatrolProc(void);

   /** @fn     ImmAjustProc
    *  @brief  立即校时一次处理线程
    *  @return 正常 ADJUST_ERROR_SUCC
               异常 非 ADJUST_ERROR_SUCC
    */
    DWORD ImmAjustProc(void);

   /** @fn     HeartBeatProc
    *  @brief  心跳线程
    *  @return 正常 ADJUST_ERROR_SUCC
               异常 非 ADJUST_ERROR_SUCC
    */
    DWORD HeartBeatProc(void);

private:
    HANDLE        m_hThreadManager;        //管理线程句柄
	HANDLE        m_hThreadHeartBeat;      //心跳线程句柄
    HANDLE        m_hThreadAdjustTime;     //校时工作线程句柄
    HANDLE        m_hThreadAddAlarm;       //添加报警线程句柄
    HANDLE        m_hThreadDisAlarm;       //报警处理线程句柄
    HANDLE        m_hThreadCollectPatrol;  //DVR巡检线程句柄
	HANDLE        m_hThreadStoragepatrol;  //存储设备容量检测句柄
    volatile bool m_bStopWorker;           //校时工作是否停止
    volatile bool m_bStopServer;           //校时工作是否停止

public:
	config_info_t m_stAdjustConfig;      //配置信息

	HANDLE                          m_hGetCollectDeviceListEvent;    //采集设备信号量
	CRITICAL_SECTION                m_csListCollectDevice;           //采集设备信息列表锁
	std::list<collect_info_t>       m_listCollectDevice;             //采集设备信息队列

	HANDLE                          m_hGetStorageDeviceListEvent;    //存储设备信号量
	CRITICAL_SECTION                m_cslistStorageDevice;           //存储设备信息列表锁
	std::list<Struct_StorageDevice> m_listStorageDevice;             //存储设备信息队列

};

inline CAdjustTime* GetAdjustPtr(void)
{
    return CAdjustTime::instance();
}

#endif //GUARD_ADJUSTTIME_H
