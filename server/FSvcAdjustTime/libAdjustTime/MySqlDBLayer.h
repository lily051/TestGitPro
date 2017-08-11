/**	@file    MySqlDBLayer.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ���ݿ������
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
     *  @brief  ��ȡ����DVR��Ϣ
     *  @param  [out]lstDvrInfo, dvr��Ϣ����
     *  @return bool
     */
    bool getAllDvrInfo(std::list<collect_info_t> &lstDvrInfo);


    /** @fn     UpdateWorkState
     *  @brief  ����WorkState��
     *  @param  [in]workstate, workstate��Ϣ
     *  @return bool
     */
    bool UpdateWorkState(const work_state_t &workstate);

    /** @fn     UpdateHeartBeat
    *  @brief  ���ı�����Ϣ�����Уʱ��������
    *  @param  [in]workstate, workstate��Ϣ
    *  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
    *  @return bool
    */
    bool UpdateHeartBeat(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     AddDeleteLog
     *  @brief  ¼��ɾ����Ϣ�������־
     *  @param  [in]workstate, workstate��Ϣ
     *  @return bool
     */
    bool AddDeleteLog(const work_state_t &workstate);

    /** @fn     AddCenterAlarmReq
     *  @brief  ���ı�����Ϣ����ӱ���
     *  @param  [in]workstate, workstate��Ϣ
     *  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
     *  @return bool
     */
    bool AddCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     UpdateCenterAlarmReq
     *  @brief  ���ı�����Ϣ���±���
     *  @param  [in]workstate, workstate��Ϣ
     *  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
     *  @return bool
     */
    bool UpdateCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

    /** @fn     GetAlarmStat
    *  @brief  ��ȡ�����Ƿ��Ѿ�����
    *  @param  [in]workstate, workstate��Ϣ
    *  @param  [out]nStat, ��������״̬
    *  @return bool
    */
    bool GetAlarmStat(const work_state_t &workstate, int &nStat);

    /** @fn     AddCollectAlarm
     *  @brief  �ɼ���������Ϣ����ӱ���
     *  @param  [in]workstate, workstate��Ϣ
     *  @return bool
     */
    bool AddCollectAlarm(const work_state_t &workstate);

    /** @fn     UpdateCollectAlarm
     *  @brief  �ɼ���������Ϣ���±���
     *  @param  [in]workstate, workstate��Ϣ
     *  @return bool
     */
    bool UpdateCollectAlarm(const work_state_t &workstate);

private:
    /** @fn     ExtractDvrInfo
     *  @brief  �����������ȡDVR��Ϣ
     *  @param  [out]stDvrInfo, dvr��Ϣ
     *  @param  [in]recordSet, �������
     *  @return 
    */
    void ExtractDvrInfo(collect_info_t &stDvrInfo, CMySQLRecordSet &recordSet);

    /** @fn CMySqlDBLayer::GetGUID
     *  @brief  ����GUID
     *  @return std::_tstring��GUID�ַ���
     */
    std::_tstring GetGUID(const work_state_t &workstate);

private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};
#endif //GUARD_MYSQLDBLAYER_H
