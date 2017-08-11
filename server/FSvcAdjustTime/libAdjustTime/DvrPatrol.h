/**	@file    DvrPatrol.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   dvr�豸Ѳ�촦�� ��
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
    *  @brief  ���dvr�Ƿ�����
    *  @param  [in]stDvrInfo, dvr��Ϣ
    *  @return bool
    */
    bool ChkDvrOnLine(const collect_info_t &lstDvrInfo);

	 /** @fn     ChkDvr
    *  @brief  ���dvrӲ�������Ƿ���
    *  @param  [in]lUserId��¼���
    *  @return bool
    */
    bool CheckDvrDisk(const collect_info_t &stDvrInfo, long lUserId);

    /** @fn     StartDvrAlarmListen
    *  @brief  ��ʼdvr��������
    *  @param  [in]stDvrInfo, dvr��Ϣ
    *  @return bool
    */
    bool StartDvrAlarmListen(const collect_info_t &stDvrInfo);

    /** @fn     DvrAlarmListen
    *  @brief  dvr��������
    *  @param  [in]stDvrInfo, dvr��Ϣ
    *  @return bool
    */
    bool StopDvrAlarmListen(const collect_info_t &stDvrInfo);
private:
    CDvrPatrol(void);
    virtual ~CDvrPatrol(void);
private:
    std::map<std::string, unsigned long> m_mapAlarmListen; //ά��dvr��Ӧ�Ĳ��������
    std::map<unsigned long, long>        m_mapAlarmLogin;  //ά��dv������Ӧ�ĵ�¼��������
};
inline CDvrPatrol* GetDvrPatrolPtr(void)
{
    return CDvrPatrol::instance();
}
#endif //GUARD_DVRPATROL_H
