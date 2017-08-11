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
    //��¼���ݿ�
    BOOL LoginMySQL();
    MysqlPool *sqlp;//����2.0���ݿ���������ݿ�
    MysqlPool *pRecord;//����2.0��record��
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
