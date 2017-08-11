#pragma once
#include "GeneralDef.h"
typedef void (*mySqlErrMsg)(const int code ,const char* msg,void* userdata);

struct mysqlInfo
{
    std::string Ip;
    std::string Port;
    std::string Version;
    std::string Username;
    std::string Password;
    mySqlErrMsg pErrCallback;
    void *pData;
    mysqlInfo()
    {
        Ip       = "";
        Port     = "";
        Version  = "";
        Username = "";
        Password = "";
        pErrCallback  = NULL;
        pData  = NULL;
    }
};



class DBInterface
{
public:
    virtual ~DBInterface(void)=0 {};

    virtual void setDBInfo(mysqlInfo& pInfo,std::vector<Iteminfo>& plist) = 0;
    virtual void getRecordInfo(const NET_DVR_TIME &StartDayTime, const NET_DVR_TIME &StopDayTime)= 0;
    virtual void getDeviceInfo()= 0;
};
