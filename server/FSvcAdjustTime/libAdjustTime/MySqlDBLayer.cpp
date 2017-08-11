/**	@file    MySqlDBLayer.cpp
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
#include "StdAfx.h"
#include <fclib/fcTchar.h>
#include "MySqlDBLayer.h"
#include "HPR_Time.h"
#include <SvcGeneralDef.h>
#include "hlogConfig.h"
#include <hak.h>

CMySqlDBLayer::CMySqlDBLayer(void)
{
}

CMySqlDBLayer::~CMySqlDBLayer(void)
{
}

/** @fn     getAllDvrInfo
 *  @brief  ��ȡ����DVR��Ϣ
 *  @param  [out]lstDvrInfo, dvr��Ϣ����
 *  @return bool
*/
bool CMySqlDBLayer::getAllDvrInfo(std::list<dvr_info_t> &lstDvrInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};

    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM Device WHERE nDevType = 100 or nDevType = 101 or nDevType = 10210"));

    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        return false;
    }

    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            dvr_info_t stDvrInfo;
            ExtractDvrInfo(stDvrInfo, recordSet);
            lstDvrInfo.push_back(stDvrInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}


/** @fn     UpdateWorkState
*  @brief  ����WorkState��
*  @param  [in]workstate, workstate��Ϣ
*  @return bool
*/
bool CMySqlDBLayer::UpdateWorkState(const work_state_t &workstate)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    if (WORK_TYPE_DVR_VI_LOST == workstate.nKey
        || WORK_TYPE_DVR_HD_EXCEPTION == workstate.nKey)
    {
        _stprintf_s(sqlStmt, 
            _countof(sqlStmt),
            _T("select nKey, nChannel from workstate where nKey = %d and nChannel = %d"),
            workstate.nKey, workstate.nChannel);
    }
    else
    {
        _stprintf_s(sqlStmt, 
            _countof(sqlStmt),
            _T("select nKey, nUserID from workstate where nKey = %d and nUserID = %d"),
            workstate.nKey, workstate.nUserID);
    }

    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        return false;
    }

    memset(sqlStmt, 0, MAX_SQL_LEN);
    if (!recordSet.IsEOF())
    {
        if (WORK_TYPE_DVR_VI_LOST == workstate.nKey
            || WORK_TYPE_DVR_HD_EXCEPTION == workstate.nKey)
        {
            _stprintf_s(sqlStmt, _countof(sqlStmt),
                _T("UPDATE WorkState SET nState=%d, dtOccurTime ='%s', strInfo='%s' \
                   WHERE nKey = %d and nChannel = %d"),
                   workstate.nState, 
                   workstate.strOccureTime.c_str(), 
                   workstate.strInfo.c_str(), 
                   workstate.nKey, workstate.nChannel);
        }
        else
        {
            _stprintf_s(sqlStmt, _countof(sqlStmt),
                _T("UPDATE WorkState SET nState=%d, dtOccurTime ='%s', strInfo='%s' \
                   WHERE nKey = %d and nUserID = %d"),
                   workstate.nState, 
                   workstate.strOccureTime.c_str(), 
                   workstate.strInfo.c_str(), 
                   workstate.nKey, workstate.nUserID);
        }
    }
    else
    {
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("insert into workstate(nKey, nParentkey, strKeyValue, nUserID,\
               nState, dtOccurTime, strInfo, nChannel) \
               values(%d, %d, '%s', %d, %d, '%s', '%s', %d)"),
               workstate.nKey, workstate.nParentKey,
               workstate.strKeyValue.c_str(),
               workstate.nUserID,workstate.nState, 
               workstate.strOccureTime.c_str(), 
               workstate.strInfo.c_str(), 
               workstate.nChannel);
    }

    if (!ExecSQL(sqlStmt))
    {
        return false;
    }

    return true;
}

/** @fn     UpdateHeartBeat
*  @brief  ���ı�����Ϣ�����Уʱ��������
*  @param  [in]workstate, workstate��Ϣ
*  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
*  @return bool
*/
bool CMySqlDBLayer::UpdateHeartBeat(const work_state_t &workstate, DWORD ulCheckType)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select ID from alarmlog \
           where alarmType = %d ORDER BY ID DESC LIMIT 1"),
           workstate.nKey);
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        TPLOG_ERROR("UpdateHeartBeat: %s", (const char*)fcT2A(sqlStmt));
        return false;
    }

    int nID = -1;
    if (!recordSet.IsEOF())
    {
        nID = recordSet.GetInt32Item("ID");
        memset(sqlStmt, 0, MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("UPDATE alarmlog SET alarmTime='%s' where ID = %d;"),
            workstate.strOccureTime.c_str(), nID);
        if (!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("UpdateCenterAlarm: %s", (const char*)fcT2A(sqlStmt));
            return false;
        }
    }
    else
    {
        memset(sqlStmt, 0, MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("insert into alarmlog(checkType, checkObjID, checkChan, contactObjID, \
               alarmType, alarmTime, alarmbeginTime,\
               alarmRegionID, comments, strHost, status) \
               VALUES (%d, 0, %d, 0, %d, '%s', '%s', 0, '%s', '%s', 0);"),
               ulCheckType,
               workstate.nChannel,
               workstate.nKey,
               workstate.strOccureTime.c_str(),
               workstate.strOccureTime.c_str(),
               workstate.strInfo.c_str(),
               workstate.strHost.c_str());

        if (!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("ModLocalAlarmLog: %s", (const char*)fcT2A(sqlStmt));
            return false;
        }
    }

    return true;
}

/** @fn     AddDeleteLog
*  @brief  ¼��ɾ����Ϣ�������־
*  @param  [in]workstate, workstate��Ϣ
*  @return bool
*/
bool CMySqlDBLayer::AddDeleteLog(const work_state_t &workstate)
{
    //TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    //_stprintf_s(sqlStmt, _countof(sqlStmt),
    //    _T("insert into clientdeletelog(checkType, checkObjID, checkChan, contactObjID, \
    //       alarmType, alarmTime, alarmbeginTime,\
    //       alarmRegionID, comments, strHost, status) \
    //       VALUES (%d, 0, %d, 0, %d, '%s', '%s', 0, '%s', '%s', 0);"),
    //       ulCheckType,
    //       workstate.nChannel,
    //       workstate.nKey,
    //       workstate.strOccureTime.c_str(),
    //       workstate.strOccureTime.c_str(),
    //       workstate.strInfo.c_str(),
    //       workstate.strHost.c_str());

    //if (!ExecSQL(sqlStmt))
    //{
    //    TPLOG_ERROR("ModLocalAlarmLog: %s", (const char*)fcT2A(sqlStmt));
    //    return false;
    //}
    return true;
}


/** @fn     UpdateCenterAlarm
*  @brief  ���ı�����Ϣ���±���
*  @param  [in]workstate, workstate��Ϣ
*  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
*  @return bool
*/
bool CMySqlDBLayer::UpdateCenterAlarm(const work_state_t &workstate, DWORD ulCheckType)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select ID from alarmlog \
           where alarmType = %d and checkChan = %d and strHost = '%s'ORDER BY ID DESC LIMIT 1"),
           workstate.nKey, workstate.nChannel, workstate.strHost.c_str());
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        TPLOG_ERROR("UpdateCenterAlarm: %s", (const char*)fcT2A(sqlStmt));
        return false;
    }

    int nID = -1;
    if (!recordSet.IsEOF())
    {
        nID = recordSet.GetInt32Item("ID");
        memset(sqlStmt, 0, MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("UPDATE alarmlog SET alarmTime='%s' where ID = %d;"),
            workstate.strOccureTime.c_str(), nID);
        if (!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("UpdateCenterAlarm: %s", (const char*)fcT2A(sqlStmt));
            return false;
        }
    }
    else
    {
        TPLOG_ERROR("UpdateCenterAlarm: %d, %d have no recode",
            workstate.nKey,
            workstate.nChannel);
        return false;
    }

    return true;
}

/** @fn     GetAlarmStat
*  @brief  ��ȡ�����Ƿ��Ѿ�����
*  @param  [in]workstate, workstate��Ϣ
*  @param  [out]nStat, ��������״̬
*  @return bool
*/
bool CMySqlDBLayer::GetAlarmStat(const work_state_t &workstate, int &nStat)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select status from alarmlog \
           where alarmType = %d and checkChan = %d and strHost = '%s'ORDER BY ID DESC LIMIT 1"),
           workstate.nKey, workstate.nChannel, workstate.strHost.c_str());
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        TPLOG_ERROR("UpdateCenterAlarm: %s", (const char*)fcT2A(sqlStmt));
        return false;
    }

    if (!recordSet.IsEOF())
    {
        nStat = recordSet.GetInt32Item("status");
    }
    else
    {
        TPLOG_ERROR("%d, %d, %s have no recode",
            workstate.nKey,
            workstate.nChannel,
            fcT2A(workstate.strHost.c_str()));
        return false;
    }

    return true;
}

/** @fn     AddCollectAlarm
*  @brief  �ɼ���������Ϣ����ӱ���
*  @param  [in]workstate, workstate��Ϣ
*  @return bool
*/
bool CMySqlDBLayer::AddCollectAlarm(const work_state_t &workstate)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    std::_tstring strGUID = GetGUID(workstate);
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("insert into alarmlogs(nAlarmID, strGUID, nType, nSubType, \
           nStatus, dtOccurtime, dtBegintime, nCChannel,\
           nDevID, strAppendInfo, strHost, nCenterFlag, nUserID, nProcStatus) \
           VALUES (%I64d, '%s', %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s', 1, %d, 0)"),
           workstate.nAlarmID,
           strGUID.c_str(),
           workstate.nParentKey,
           workstate.nKey,
           workstate.nState,
           workstate.strOccureTime.c_str(),
           workstate.strOccureTime.c_str(),
           workstate.nChannel,
           workstate.nRev,
           workstate.strInfo.c_str(),
           workstate.strHost.c_str(),
           workstate.nUserID);

    if (!ExecSQL(sqlStmt))
    {
        TPLOG_ERROR("ModLocalAlarmLog: %s", (const char*)fcT2A(sqlStmt));
        return false;
    }

    return true;
}

/** @fn     UpdateCollectAlarm
*  @brief  �ɼ���������Ϣ���±���
*  @param  [in]workstate, workstate��Ϣ
*  @return bool
*/
bool CMySqlDBLayer::UpdateCollectAlarm(const work_state_t &workstate)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select ID from alarmlogs \
           where nSubType = %d and nCChannel = %d  and strHost = '%s' ORDER BY ID DESC LIMIT 1"),
           workstate.nKey, workstate.nChannel, workstate.strHost.c_str());
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        return false;
    }

    int nID = -1;
    if (!recordSet.IsEOF())
    {
        nID = recordSet.GetInt32Item("ID");
        memset(sqlStmt, 0, MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("UPDATE alarmlogs SET dtOccurtime='%s' where ID = %d;"),
            workstate.strOccureTime.c_str(), nID);
        if (!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("ModLocalAlarmLog: %s", (const char*)fcT2A(sqlStmt));
            return false;
        }
    }
    else
    {
        TPLOG_ERROR("UpdateCollectAlarm: %d, %d, %d have no recode",
            workstate.nKey,
            workstate.nChannel,
            workstate.nUserID);
        return false;
    }

    return true;
}

/** @fn     ExtractFileToTrans
 *  @brief  �����������ȡDVR��Ϣ
 *  @param  [out]stDvrInfo, dvr��Ϣ
 *  @param  [in]recordSet, �������
 *  @return 
*/
void CMySqlDBLayer::ExtractDvrInfo(dvr_info_t &stDvrInfo, CMySQLRecordSet &recordSet)
{
    stDvrInfo.strDvrIp = fcU2A(recordSet.GetStringItem("strIP"));
    stDvrInfo.usPort = (unsigned short)(recordSet.GetInt32Item("nPort"));
    stDvrInfo.strUserName = fcU2A(recordSet.GetStringItem("strUser"));
    stDvrInfo.strPassWd = fcU2A(recordSet.GetStringItem("strPassword"));
    //��������� �����
    if (Hak9((char *)(stDvrInfo.strPassWd.c_str()), NULL, NULL))
    {
        char aes[1024] = { '\0' };
        int aes_len = 1024;
        char* out = aes;
        char* outl = (char*)&aes_len;
        Hak7(out, (char *)(stDvrInfo.strPassWd.c_str()), outl);
        stDvrInfo.strPassWd = out;
    }
}


/** @fn CMySqlDBLayer::GetGUID
 *  @brief  ����GUID
 *  @return std::_tstring��GUID�ַ���
 */
std::_tstring CMySqlDBLayer::GetGUID(const work_state_t &workstate)
{
    //���к�-ʱ��-���̺�-����IP��ַ
    static LONG seqNO=0;
    TCHAR buff[256] = {0};
    _stprintf_s(buff, sizeof(buff), _T("%d-%I64d-%d-%s"),
        InterlockedIncrement(&seqNO), 
        _time64(NULL),
        GetCurrentProcessId(),
        workstate.strHost.c_str());
    return (std::_tstring)buff;
}
