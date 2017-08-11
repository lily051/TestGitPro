#pragma once
#include "dbinterface.h"
class MysqlPool;
class operateDBV20 :
    public DBInterface
{
public:
    operateDBV20(void);
    ~operateDBV20(void);

    void setDBInfo(mysqlInfo& pInfo,std::vector<Iteminfo>& plist);
    void getRecordInfo(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime);
    void getDeviceInfo();

    volatile LONG m_beLoginMysql;
    HANDLE m_loginThreadHandle;
    //登录数据库
    BOOL LoginMySQL();
    MysqlPool *sqlp;//连接2.0数据库的中心数据库
    MysqlPool *pRecord;//连接2.0的record表
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
