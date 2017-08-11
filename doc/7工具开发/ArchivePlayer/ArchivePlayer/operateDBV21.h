#pragma once
#include "dbinterface.h"
class MysqlPool;
class operateDBV21 :
    public DBInterface
{
public:
    operateDBV21(void);
    ~operateDBV21(void);

    void setDBInfo(mysqlInfo& pInfo,std::vector<Iteminfo>& plist);
    void getRecordInfo(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime);
    void getDeviceInfo();

    volatile LONG m_beLoginMysql;
    HANDLE m_loginThreadHandle;
    BOOL LoginMySQL();
    MysqlPool *sqlp;
private:
    mysqlInfo m_Info;
    int getStoreInfo(RecordInfo &record);
    int getCamerainfo(RecordInfo &record);
    BOOL searchFormRecords(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime);
    BOOL searchFormProRecords(RecordInfo& pRecordinfo);
    void searchBusinessForRecord(RecordInfo& pRecordinfo);
    BOOL searchBusiness();

    void getDeviceInfoFromDB();
};
