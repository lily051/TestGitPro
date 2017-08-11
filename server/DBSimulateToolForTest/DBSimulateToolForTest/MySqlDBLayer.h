#ifndef GUARD_MYSQLDBLAYER_H
#define GUARD_MYSQLDBLAYER_H

#include <fcMySQL.h>
#include <list>
#include <iostream>
#include <string>
#include <sstream>
//1-区域表信息
typedef struct _regioninfo {
    unsigned int ID;
    unsigned int nParentID;
    unsigned int nSortID;
    std::string strInterAreaCode;
    std::string strPath;
    std::string strName;
    std::string strCode;
    std::string strTxt;
}region_info_t;
//2-设备表信息
typedef struct _deviceinfo {
    unsigned int  ID;
    unsigned int  nType;
    std::string   strName;
    std::string   strIP;
    unsigned int  nPort;
    std::string   strUser;
    std::string   strPassword;
    std::string   strPasswordLevel;
    unsigned int  nDevType;
    unsigned int  nProductType;
    unsigned int  nChanNum;
    std::string strDeviceID;
    unsigned int  telChanNum;
    unsigned int  nRegionID;
    unsigned int  nStreamType;
    unsigned int  dvrType;
    unsigned int  nConfigPort;
    std::string strConfigUser;
    std::string strConfigPassword;
    unsigned int online;
    unsigned int uiTotalSpace;
    unsigned int uiFreeSpace;
    std::string strIndexCode;
    std::string strManufacture;
}device_info_t;
//3-监控点信息表
typedef struct _devicechannelinfo {
    unsigned int  ID;
    unsigned int  nDeviceId;
    unsigned int  nChannelNo;
    std::string   nChannelName;
    unsigned int  nflag;
    unsigned int  nRegionId;
    unsigned int  recordPlane;
}devicechannel_info_t;
//4-产品信息表
typedef struct _productinfo {
    unsigned int  ID;
    std::string   strCode;
    std::string   strName;
    unsigned int  pId;
    unsigned int  saveYears;
    unsigned int  guaranteeYears;
    unsigned int  nEnabled;
    std::string   strEndDate;
    std::string   strApplyDate;
    std::string   strVideoEndDate;
    unsigned int  importUser;
    unsigned int  checkUser;
    std::string   importTime;
    std::string   checkTime;
    std::string   strGrade;
    std::string   strRisklevel;
    std::string   remark;
    std::string   wordTemplate;
}product_info_t;
//5-用户信息表
typedef struct _usersinfo {
    unsigned int  ID;
    std::string   strName;
    std::string   strCode;
    unsigned int  sysRole;
    std::string   strPassword;
    unsigned int  nPasswordLevel;
    std::string   strTel;
    std::string   strEmail;
    std::string   strComment;
    unsigned int  nState;
    std::string   strDate;
    unsigned int  nSex;
    std::string   branch;
    unsigned int  netId;
    std::string   net;
    std::string   strDuty;
    std::string   userNo;
    std::string   strUpdatePwdTime;
    unsigned int  nOwner;
    unsigned int  nupdated;
    unsigned int  nonline;
    std::string   strCredentialNo;
    std::string   strDepartment;
    std::string   lastTimeOnline;
    unsigned int  nCrtificateId;
}users_info_t;
//6-业务单表
typedef struct _clientinputinfo {
    unsigned int  ID;
    std::string  nProId;
    std::string inputTime;
    std::string businessTime;
    unsigned int inputRegionId;
    std::string inputRegionCode;
    std::string inputRegionName;
    unsigned int  status;
    unsigned int  nisRead;
    unsigned int  nisPay;
    unsigned int  delaydays;
    std::string   strEndDate;
    std::string   name;
    std::string creditCode;
    std::string creditId;
    std::string inputUser;
    std::string proName;
    std::string proUserNo;
    unsigned int  productTypeId;
    std::string productTypeName;
    unsigned int productId;
    std::string productName;
    std::string productCode;
    std::string strVideoEndDate;
    unsigned int guaranteeYears;
    unsigned int productSaveMonth;
    std::string strGrade;
    std::string strRisklevel;
    std::string strBankNo;
    std::string strBuyMoney;
    std::string strCreditPhone;
    std::string strRemarks;
    unsigned int tipflag;
    std::string markDate;
    unsigned int nisUpload;
    unsigned int saveflag;
    unsigned int productType;
}clientinput_info_t;
//7-报警日志表
typedef struct _alarmloginfo {
    unsigned int  ID;
    unsigned int  checkType;
    unsigned int  checkObjId;
    unsigned int  checkChan;
    unsigned int  contactObjId;
    unsigned int  alarmType;
    std::string   alarmTime;
    std::string   alarmbeginTime;
    unsigned int  alarmRegionId;
    std::string   comments;
    std::string   strHost;
    unsigned int  status;
    std::string   checker;
    std::string   userNo;
    std::string   dealTime;
    std::string   dealcomment;
    unsigned int  alarmlevel;
    std::string   strGuid;
}alarmlog_info_t;
//8-录像信息表
typedef struct _prosaverecordinfo {
    unsigned int  ID;
    unsigned int  recordId;
    unsigned int  clientId;
    unsigned int  regionId;
    unsigned int  userId;
    std::string cvr;
    std::string streamNo;
    unsigned int stateType;
    std::string fileName;
    std::string startTime;
    std::string endTime;
    unsigned int upload;
    unsigned int recordCheck;
    std::string checkRemark;
    std::string strUrl;
    unsigned int devId;
    unsigned int devChannel;
    std::string screenshotSrc;
    std::string rtspPath;
    unsigned int nType;
    unsigned int mediaType;
    unsigned int calltype;
    std::string callNo;
    std::string recordUserNo;
    unsigned int locked;
    std::string devIp;
}prosaverecord_info_t;
//录像文件
class CMySqlDBLayer : public CMySQLConnection
{
public:
    CMySqlDBLayer(void);
    ~CMySqlDBLayer(void);

    bool ChecKConnect();

    bool GetConnect(db_conn_info_t& stCenterDBInfo);

    bool CloseConnect();
    //
    int GetRegionNumber();
    int GetProductNumber();
    int GetUsersNumber();
    int GetDeviceChannelNumber();
    int GetDeviceNumber();
    int GetProsaveRecordNumber();
    int GetClientinputNumber();
    int GetAlarmlogNumber();
    //参考数据库区域表查询，立即访问模拟数据查询插入响应表中
    bool GetRegionInfo(std::vector<region_info_t> &vecRegionInfo);
    //从搜索结果提取区域信息
    void ExtractRegionInfo(region_info_t &stRegionInfo, CMySQLRecordSet &recordSet);
    //插入到模拟数据库
    bool InsertRegionInfo(std::vector<region_info_t> &vecRegionInfo,int nIndexVecID = 0);

    //设备
    bool GetDeviceInfo(std::vector<device_info_t> &vecDeviceInfo);
    bool GetDeviceInfoOneDvr(std::vector<device_info_t> &vecDeviceInfo);
    void ExtractDeviceInfo(device_info_t &stDeviceInfo, CMySQLRecordSet &recordSet);
    bool InsertDeviceInfo(std::vector<device_info_t> &vecDeviceInfo,int nIndexVecID = 0);
    
    //监控点
    bool GetDevicechannelInfo(std::vector<devicechannel_info_t> &vecDevicechannelInfo);
    void ExtractDevicechannelInfo(devicechannel_info_t &stDevicechannelInfo, CMySQLRecordSet &recordSet);
    bool InsertDevicechannelInfo(std::vector<devicechannel_info_t> &vecDevicechannelInfo,int nIndexVecID = 0);
    
    //产品
    bool GetProductInfo(std::vector<product_info_t> &vecProductInfo);
    void ExtractProductInfo(product_info_t &stProductInfo, CMySQLRecordSet &recordSet);
    bool InsertProductInfo(std::vector<product_info_t> &vecProductInfo,int nIndexVecID = 0);

    //用户信息
    bool GetUsersInfo(std::vector<users_info_t> &vecUsersInfo);
    void ExtractUsersInfo(users_info_t &stUsersInfo, CMySQLRecordSet &recordSet);
    bool InsertUsersInfo(std::vector<users_info_t> &vecUsersInfo,int nIndexVecID = 0);

    //业务单表
    bool GetClientinputInfo(std::vector<clientinput_info_t> &vecClientinputInfo);
    void ExtractClientinputInfo(clientinput_info_t &stClientinputInfo, CMySQLRecordSet &recordSet);
    bool InsertClientinputInfo(std::vector<clientinput_info_t> &lstClientinputInfo,int nIndexVecID = 0);

    //录像单
    bool GetProsaverecordInfo(std::vector<prosaverecord_info_t> &vecProsaverecordInfo);
    bool InsertProsaverecordInfo(std::vector<prosaverecord_info_t> &vecProsaverecordInfo,int nIndexVecID = 0);
    void ExtractProsaverecordInfo(prosaverecord_info_t &stProsaverecordInfo, CMySQLRecordSet &recordSet);

    //报警日志
    bool GetAlarmlogInfo(std::vector<alarmlog_info_t> &vecAlarmlogInfo);
    bool InsertAlarmlogInfo(std::vector<alarmlog_info_t> &vecAlarmlogInfo,int nIndexVecID = 0);
    void ExtractAlarmlogInfo(alarmlog_info_t &stAlarmlogInfo, CMySQLRecordSet &recordSet);

    //区域表模拟数据
    bool StartSimulateRegion(const std::vector<region_info_t> &vecRegionInfo,int &nRegionNumber);
    //
    bool StartSimulateDevice(const std::vector<device_info_t> &vecDeviceInfo,int &nDeviceNumber);
    //
    bool StartSimulateDevicechannel(const std::vector<devicechannel_info_t> &vecDevicechannelInfo,int &nDevicechannelNumber);
    //
    bool StartSimulateProduct(const std::vector<product_info_t> &vecProductInfo,int &nProductNumber);
    //
    bool StartSimulateUsers(const std::vector<users_info_t> &vecUsersInfo,int &nUsersNumber);
    //
    bool StartSimulateClientinput(const std::vector<clientinput_info_t> &vecClientinputInfo,int &nClientinputNumber);
    //
    bool StartSimulateAlarmlog(const std::vector<alarmlog_info_t> &vecAlarmlogInfo,int &nAlarmlogNumber);
    //
    bool StartSimulateProsaveRecord(const std::vector<prosaverecord_info_t> &vecProsaverecordInfo,int &nProsaverecordNumber);


    //
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();
private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};

#endif