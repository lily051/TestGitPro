/**	@file    AlarmProcessor.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   报警处理 类
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
#ifndef GUARD_ALARMPROCESSOR_H
#define GUARD_ALARMPROCESSOR_H

#include <fclib/fcTchar.h>
#include <HPR_Singleton.h>
#include "SynchroQueue.h"
#include <SvcGeneralDef.h>
#include "../util/Guard.h"
//#include <HPR_Guard.h>
//#include "hppTTSClient.h"
#include <map>
#include <time.h>
#include "Center/Alarmlog.pb.h"
#include "DBDataMgr.h"


class CAlarmProcessor : public singleton<CAlarmProcessor>
{
    friend class singleton<CAlarmProcessor>;
public:
    CAlarmProcessor(void);
    ~CAlarmProcessor(void);

    /** @fn Init
     *  @brief  初始化函数
     *  @return 0成功 其他失败
     */
    HPR_INT32 Init();

   /** @fn      Fini
    *   @brief  反初始化函数
    *   @return 正常 ADJUST_ERROR_SUCC
                    异常 非 ADJUST_ERROR_SUCC
    */
    int Fini(void);

    /** @fn AddAlarmMsg
     *  @brief  添加报警消息
     *  @param [in] pAlarm, 报警消息指针 
     *  @return void
     */
    void AddAlarmMsg(const pALARM_MSG pAlarm);

    /** @fn SaveAlarmThreadProc
	 *  @brief  添加报警处理函数
	 *  @return HPR_INT32， 0 退出，其它无意义
	 */
    HPR_INT32 SaveAlarmThreadProc();

   /** @fn AlarmDisposeProc
    *  @brief  报警消息处理函数
    *  @return HPR_INT32， 0 退出，其它无意义
    */
    HPR_INT32 AlarmDisposeProc();

private:
    /** @fn GetSvcKeyValue
     *  @brief  获取状态类型名称
     *  @param [in] workstate, 工作状态结构体
     *  @return std::_tstring，状态类型名称
     */
    std::_tstring GetSvcKeyValue(const work_state_t &workstate);

    /** @fn FormatKey
     *  @brief  生成工作状态key
     *  @param [in] workstate, 工作状态结构体
     *  @return std::_tstring，key字符串
     */
    std::_tstring FormatKey(const work_state_t &workstate);

    /**	@fn	    DelayBySleep
     *	@brief	通过循环sleep的方式等待一段时间
     *	@param  [in]dwMilliseconds, 需要等待的时间，单位毫秒
     *	@param  [in]stopFlag, 标志，如果为true则等待完dwMilliseconds，
     *                        否则会提前返回
     *	@param  [in]dwInterval, 等待间隔，单位毫秒
     *	@return	void
     */
    void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);


    /** @fn     strTime
     *  @brief  时间转换为字符串
     *  @param  [in]t, 时间信息
     *  @return 时间字符串
     */
    std::_tstring strTime(__time32_t const t);


   /**	@fn	    GetAlarmLogReq
	*	@brief	获得报警日志
	*   @param  [in]workstate, workstate信息
	*   @param  [out]stAlarmLog, 报警日志信息
	*   @return bool
	*/
	bool GetAlarmLogReq(const work_state_t &workstate, Struct_AlarmLog &stAlarmLog);

	/** @fn     UpdateCenterAlarmReq
	*  @brief  中心报警信息更新报警
	*  @param  [in]workstate, workstate信息
	*  @return bool
	*/
	bool UpdateCenterAlarmReq(const work_state_t &workstate);

public:

   /**	@fn	    GetAlarmLogRsp
	*	@brief	获得报警日志
	*	@param  [in] alarmlogInfo -- 报警日志信息
	*   @param  [in] bFinish -- 是否结束
	*	@return	
	*/
	void GetAlarmLogRsp(const cms_8120::AlarmlogInfo& alarmlogInfo, BOOL bFinish /*= true*/);

	/**	@fn	    convertAlarmLog
	*	@brief	转换报警日志
	*	@param  [in]  alarmlogInfo -- 原报警日志
	*   @param  [out] stAlarmLog -- 转换后的报警日志
	*	@return	BOOL
	*/
	BOOL convertAlarmLog(const cms_8120::Alarmlog &alarmlogInfo, Struct_AlarmLog &stAlarmLog);

	/**	@fn	    AddAlarmRegionId
	*	@brief	增加报警区域ID
	*	@param  [in]  work_state_t -- 报警信息
	*   @param  [out] work_state_t -- 报警区域ID
	*	@return	void
	*/
	void AddAlarmRegionId(work_state_t& workstate);

public:
    bool                       m_bExitFlag;  ///<任务退出标志

	CRITICAL_SECTION m_csRegion;     //区域锁
	std::map<std::string, int> m_mapRegion; //设备区域

private:
    CSynchroQueue <work_state_t>          m_msgWorkState;  // 状态消息队列
    std::map<std::_tstring, work_state_t> m_mapWorkState;  // 工作状态map
    CRITICAL_SECTION                      m_csWorkState;  // 工作状态map锁

	HANDLE           m_hAlarmLogEvent; //报警日志信号量
    CRITICAL_SECTION m_csAlarmLog;     //报警日志锁
	Struct_AlarmLog  m_stAlarmLog;     //报警日志信息
};
inline CAlarmProcessor* GetCAlarmProcPtr(void)
{
    return CAlarmProcessor::instance();
}
#endif //GUARD_ALARMPROCESSOR_H
