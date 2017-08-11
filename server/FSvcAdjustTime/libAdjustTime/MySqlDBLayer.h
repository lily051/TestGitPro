/**	@file    MySqlDBLayer.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   数据库操作类
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
#ifndef GUARD_MYSQLDBLAYER_H
#define GUARD_MYSQLDBLAYER_H

#include <fclib/fcDBCommon.h>
#include <fclib/fcMySQL.h>
#include <HPR_Singleton.h>
#include "common.h"
#include <list>

typedef struct _tag_MsgTTSAlarm
{
    int  nAlarmType;
    int  nPort;
    char szIP[16];
    char szTime[64];
    char szGUID[128];
    char szAlarmInfo[512];
    _tag_MsgTTSAlarm()
    {
        nAlarmType = -1;
        nPort = 80;
        memset(szIP, 0, 16);
        memset(szTime, 0, 64);
        memset(szGUID, 0, 128);
        memset(szAlarmInfo, 0, 512);
    }
}MsgTTSAlarm, *pMsgTTSAlarm;

class CMySqlDBLayer : public CMySQLConnection
{
public:
    CMySqlDBLayer(void);
    ~CMySqlDBLayer(void);

    /** @fn     getAllDvrInfo
     *  @brief  获取所有DVR信息
     *  @param  [out]lstDvrInfo, dvr信息队列
     *  @return bool
     */
    bool getAllDvrInfo(std::list<collect_info_t> &lstDvrInfo);


    /** @fn     UpdateWorkState
     *  @brief  更新WorkState表
     *  @param  [in]workstate, workstate信息
     *  @return bool
     */
    bool UpdateWorkState(const work_state_t &workstate);

    /** @fn     UpdateHeartBeat
    *  @brief  中心报警信息表更新校时服务心跳
    *  @param  [in]workstate, workstate信息
    *  @param  [in]ulCheckType, 100 dvr,102 采集器服务，200中心服务
    *  @return bool
    */
    bool UpdateHeartBeat(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     AddDeleteLog
     *  @brief  录像删除信息表添加日志
     *  @param  [in]workstate, workstate信息
     *  @return bool
     */
    bool AddDeleteLog(const work_state_t &workstate);

    /** @fn     AddCenterAlarmReq
     *  @brief  中心报警信息表添加报警
     *  @param  [in]workstate, workstate信息
     *  @param  [in]ulCheckType, 100 dvr,102 采集器服务，200中心服务
     *  @return bool
     */
    bool AddCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     UpdateCenterAlarmReq
     *  @brief  中心报警信息更新报警
     *  @param  [in]workstate, workstate信息
     *  @param  [in]ulCheckType, 100 dvr,102 采集器服务，200中心服务
     *  @return bool
     */
    bool UpdateCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     GetAlarmStat
    *  @brief  获取报警是否已经处理
    *  @param  [in]workstate, workstate信息
    *  @param  [out]nStat, 报警处理状态
    *  @return bool
    */
    bool GetAlarmStat(const work_state_t &workstate, int &nStat);

    /** @fn     AddCollectAlarm
     *  @brief  采集器报警信息表添加报警
     *  @param  [in]workstate, workstate信息
     *  @return bool
     */
    bool AddCollectAlarm(const work_state_t &workstate);

    /** @fn     UpdateCollectAlarm
     *  @brief  采集器报警信息更新报警
     *  @param  [in]workstate, workstate信息
     *  @return bool
     */
    bool UpdateCollectAlarm(const work_state_t &workstate);

private:
    /** @fn     ExtractDvrInfo
     *  @brief  从搜索结果提取DVR信息
     *  @param  [out]stDvrInfo, dvr信息
     *  @param  [in]recordSet, 搜索结果
     *  @return 
    */
    void ExtractDvrInfo(collect_info_t &stDvrInfo, CMySQLRecordSet &recordSet);

    /** @fn CMySqlDBLayer::GetGUID
     *  @brief  生成GUID
     *  @return std::_tstring，GUID字符串
     */
    std::_tstring GetGUID(const work_state_t &workstate);

private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};
#endif //GUARD_MYSQLDBLAYER_H
