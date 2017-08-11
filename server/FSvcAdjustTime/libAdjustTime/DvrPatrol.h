/**	@file    DvrPatrol.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   dvr设备巡检处理 类
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
#ifndef GUARD_DVRPATROL_H
#define GUARD_DVRPATROL_H

#include <HPR_Singleton.h>
#include "Common.h"
#include <map>

class CDvrPatrol : public singleton<CDvrPatrol>
{
    friend class singleton<CDvrPatrol>;
public:
    /** @fn     ChkDvr
    *  @brief  检查dvr是否在线
    *  @param  [in]stDvrInfo, dvr信息
    *  @return bool
    */
    bool ChkDvrOnLine(const collect_info_t &lstDvrInfo);

	 /** @fn     ChkDvr
    *  @brief  检查dvr硬盘容量是否满
    *  @param  [in]lUserId登录句柄
    *  @return bool
    */
    bool CheckDvrDisk(const collect_info_t &stDvrInfo, long lUserId);

    /** @fn     StartDvrAlarmListen
    *  @brief  开始dvr报警布防
    *  @param  [in]stDvrInfo, dvr信息
    *  @return bool
    */
    bool StartDvrAlarmListen(const collect_info_t &stDvrInfo);

    /** @fn     DvrAlarmListen
    *  @brief  dvr监听布防
    *  @param  [in]stDvrInfo, dvr信息
    *  @return bool
    */
    bool StopDvrAlarmListen(const collect_info_t &stDvrInfo);
private:
    CDvrPatrol(void);
    virtual ~CDvrPatrol(void);
private:
    std::map<std::string, unsigned long> m_mapAlarmListen; //维护dvr对应的布防句柄。
    std::map<unsigned long, long>        m_mapAlarmLogin;  //维护dv布防对应的登录句柄句柄。
};
inline CDvrPatrol* GetDvrPatrolPtr(void)
{
    return CDvrPatrol::instance();
}
#endif //GUARD_DVRPATROL_H
