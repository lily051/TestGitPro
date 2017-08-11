#include "stdafx.h"
#include "MySqlDBLayer.h"
#include "hlogConfig.h"
#include <fcTchar.h>
CMySqlDBLayer::CMySqlDBLayer(void)
//:creatTableFirst(true)
{
}

CMySqlDBLayer::~CMySqlDBLayer(void)
{
}
//
bool CMySqlDBLayer::ChecKConnect()
{
    //与数据库交互一下，如果false
    CMySQLRecordSet recordSet;
    return ExecSQL(_T("SELECT 1;"), recordSet);
}
//封装Connect接口,如果失败，就返回false；成功，返回true
bool CMySqlDBLayer::GetConnect(db_conn_info_t& stCenterDBInfo)
{
    if (this->ChecKConnect())
    {
        return true;
    }
    this->CloseConnect();
    bool bRet = this->Connect(stCenterDBInfo, false);
    if (!bRet)
    {
        TPLOG_ERROR("connect Center db failed! host:%s, port:%s, user:%s,dbschema:%s",
            (LPCSTR)fcT2A(stCenterDBInfo.host.c_str()),
            (LPCSTR)fcT2A(stCenterDBInfo.port.c_str()),
            (LPCSTR)fcT2A(stCenterDBInfo.username.c_str()),
            (LPCSTR)fcT2A(stCenterDBInfo.dbschema.c_str()));
        return false;
    }
    return true;
}

bool CMySqlDBLayer::CloseConnect()
{
    //TODO:调用基类的关闭方法
    return this->Disconnect();
}
//
bool CMySqlDBLayer::StateRelateOfflineVideo(std::string& FileName)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM Records WHERE strRecordName = '%s';"),
           FileName.c_str());
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        return false;
    }
    if (!recordSet.IsEmpty())
    {
        if(1 != recordSet.GetRowCount())
        {
            TPLOG_ERROR("recordSet.GetRowCount() != 1(FileName:%s)",FileName.c_str());
                //return false;
        }
    }
    else
        return false;
    return true;
}
bool CMySqlDBLayer::StateRelateOfflineVideo(CString& DevIp,LONG lChannel,NET_DVR_FINDDATA_V30& FindData)
{
    CString StartTime,StopTime;
    StartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),FindData.struStartTime.dwYear,FindData.struStartTime.dwMonth,FindData.struStartTime.dwDay,\
        FindData.struStartTime.dwHour,FindData.struStartTime.dwMinute,FindData.struStartTime.dwSecond);
    StopTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),FindData.struStopTime.dwYear,FindData.struStopTime.dwMonth,FindData.struStopTime.dwDay,\
        FindData.struStopTime.dwHour,FindData.struStopTime.dwMinute,FindData.struStopTime.dwSecond);

    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM recordfileinfo WHERE strDevIp = '%s' and nChannelNo = %d and dtStartTime = '%s' and dtEndTime = '%s';"),
        DevIp,lChannel,StartTime,StopTime);
    CMySQLRecordSet recordSet;
    if(!ExecSQL(sqlStmt, recordSet))
    {
        return false;
    }
    if (!recordSet.IsEmpty())
    {
        if(1 != recordSet.GetRowCount())
        {
            //TPLOG_ERROR("recordSet.GetRowCount() != 1(FileName:%s)",FileName.c_str());
            //return false;
        }
    }
    else
        return false;
    return true;
}