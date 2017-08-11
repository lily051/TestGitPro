#include "stdafx.h"
#include "MySqlDBLayer.h"
#include <fcTchar.h>
CMySqlDBLayer::CMySqlDBLayer(void)
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
int CMySqlDBLayer::GetRegionNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Region;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Region;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetProductNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Product;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
       bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Product;");
          return -1;
    }

    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetUsersNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Users;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Users;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetDeviceChannelNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from DeviceChannel;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from DeviceChannel;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetDeviceNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Device;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Device;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetProsaveRecordNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from ProsaveRecord;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from ProsaveRecord;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetClientinputNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Clientinput;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Clientinput;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
int CMySqlDBLayer::GetAlarmlogNumber()
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("select count(*) from Alarmlog;"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return -1;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Alarmlog;");
        return -1;
    }
    return recordSet.GetInt32Item("count(*)");
}
//////////////////////////////////////////////////////////////////////////
//区域表操作
bool CMySqlDBLayer::GetRegionInfo(std::vector<region_info_t> &vecRegionInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM region LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Region;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            region_info_t stRegionInfo;
            try
            {
                ExtractRegionInfo(stRegionInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractRegionInfo 异常!");
                return false;
            }
            vecRegionInfo.push_back(stRegionInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractRegionInfo(region_info_t &stRegionInfo, CMySQLRecordSet &recordSet)
{
    stRegionInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    stRegionInfo.nParentID = (unsigned short)(recordSet.GetInt32Item("nParentID"));
    stRegionInfo.nSortID = (unsigned int)(recordSet.GetInt32Item("nSortID"));
    stRegionInfo.strInterAreaCode = fcU2A(recordSet.GetStringItem("strInterAreaCode"));
    stRegionInfo.strPath = fcU2A(recordSet.GetStringItem("strPath"));
    stRegionInfo.strName = fcU2A(recordSet.GetStringItem("strName"));
    stRegionInfo.strCode = fcU2A(recordSet.GetStringItem("strCode"));
    stRegionInfo.strTxt = fcU2A(recordSet.GetStringItem("strTxt"));
}
bool CMySqlDBLayer::InsertRegionInfo(std::vector<region_info_t> &vecRegionInfo,int nIndexVecID)
{
    for (std::vector<region_info_t>::iterator veciter=vecRegionInfo.begin();veciter != vecRegionInfo.end();++veciter)
    {
        region_info_t stRegionInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO region(ID,nParentId,nSortId,strInterAreaCode,strPath,strName,strCode,strTxt)\
               VALUES (NULL,%d,%d,'%s','%s','%s','%s','%s')")
               ,/*stRegionInfo.ID + nIndexVecID + 1,*/stRegionInfo.nParentID,stRegionInfo.nSortID,fcA2T((stRegionInfo.strInterAreaCode).c_str())
               ,fcA2T((stRegionInfo.strPath).c_str()),fcA2T((stRegionInfo.strName).c_str()),fcA2T((stRegionInfo.strCode).c_str())
               ,fcA2T((stRegionInfo.strTxt).c_str()));
        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//2-设备表信息
bool CMySqlDBLayer::GetDeviceInfo(std::vector<device_info_t> &vecDeviceInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM device LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Device;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            device_info_t stDeviceInfo;
            try
            {
                ExtractDeviceInfo(stDeviceInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractDeviceInfo is failed...");
                return false;
            }
            vecDeviceInfo.push_back(stDeviceInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
//只取一条
bool CMySqlDBLayer::GetDeviceInfoOneDvr(std::vector<device_info_t> &vecDeviceInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM device where nType = 50000 LIMIT 1"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Device;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            device_info_t stDeviceInfo;
            try
            {
                ExtractDeviceInfo(stDeviceInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractDeviceInfo is failed...");
                return false;
            }
            vecDeviceInfo.push_back(stDeviceInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
//
void CMySqlDBLayer::ExtractDeviceInfo(device_info_t &stDeviceInfo, CMySQLRecordSet &recordSet)
{
    stDeviceInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    stDeviceInfo.nType = (unsigned int)(recordSet.GetInt32Item("nType"));
    stDeviceInfo.strName = fcU2A(recordSet.GetStringItem("strName"));
    stDeviceInfo.strIP = fcU2A(recordSet.GetStringItem("strIP"));
    stDeviceInfo.nPort = (unsigned int)(recordSet.GetInt32Item("nPort"));
    stDeviceInfo.strUser = fcU2A(recordSet.GetStringItem("strUser"));
    stDeviceInfo.strPassword = fcU2A(recordSet.GetStringItem("strPassword"));
    stDeviceInfo.strPasswordLevel = fcU2A(recordSet.GetStringItem("strPasswordLevel"));
    stDeviceInfo.nDevType = (unsigned int)(recordSet.GetInt32Item("nDevType"));
    stDeviceInfo.nProductType = (unsigned int)(recordSet.GetInt32Item("nProductType"));
    stDeviceInfo.nChanNum = (unsigned int)(recordSet.GetInt32Item("nChanNum"));
    stDeviceInfo.strDeviceID = fcU2A(recordSet.GetStringItem("strDeviceID"));
    stDeviceInfo.telChanNum = (unsigned int)(recordSet.GetInt32Item("telChanNum"));
    stDeviceInfo.nRegionID = (unsigned int)(recordSet.GetInt32Item("nRegionID"));
    stDeviceInfo.nStreamType = (unsigned int)(recordSet.GetInt32Item("nStreamType"));

    stDeviceInfo.dvrType = (unsigned int)(recordSet.GetInt32Item("dvrType"));
    stDeviceInfo.nConfigPort = (unsigned int)(recordSet.GetInt32Item("nConfigPort"));
    stDeviceInfo.strConfigUser = fcU2A(recordSet.GetStringItem("strConfigUser"));
    stDeviceInfo.strConfigPassword = fcU2A(recordSet.GetStringItem("strConfigPassword"));
    stDeviceInfo.online = (unsigned int)(recordSet.GetInt32Item("online"));
    stDeviceInfo.uiTotalSpace = (unsigned int)(recordSet.GetInt32Item("uiTotalSpace"));
    stDeviceInfo.uiFreeSpace = (unsigned int)(recordSet.GetInt32Item("uiFreeSpace"));
    stDeviceInfo.strIndexCode = fcU2A(recordSet.GetStringItem("strIndexCode"));
    stDeviceInfo.strManufacture = fcU2A(recordSet.GetStringItem("strManufacture"));

    ////如果加密了 则解密
    //if (Hak9((char *)(stDeviceInfo.strPassword.c_str()), NULL, NULL))
    //{
    //    char aes[1024] = { '\0' };
    //    int aes_len = 1024;
    //    char* out = aes;
    //    char* outl = (char*)&aes_len;
    //    Hak7(out, (char *)(stDeviceInfo.strPassword.c_str()), outl);
    //    stDeviceInfo.strPassword = out;
    //}
}
bool CMySqlDBLayer::InsertDeviceInfo(std::vector<device_info_t> &vecDeviceInfo,int nIndexVecID)
{
    for (std::vector<device_info_t>::iterator veciter=vecDeviceInfo.begin();veciter != vecDeviceInfo.end();++veciter)
    {
        device_info_t stDeviceInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO device(ID,nType,strName,strIP,\
               nPort,strUser,strPassword,strPasswordLevel,nDevType,nProductType,nChanNum,\
               strDeviceID,telChanNum,nRegionID,nStreamType,dvrType,nConfigPort,strConfigUser,strConfigPassword,\
               online,uiTotalSpace,uiFreeSpace,strIndexCode,strManufacture) \
               VALUES (NULL,%d,'%s','%s',%d,'%s','%s','%s',%d,%d,%d,'%s',%d,%d,%d,%d,%d,'%s','%s',%d,%d,%d,'%s','%s')"),
               /*stDeviceInfo.ID + nIndexVecID + 1,*/stDeviceInfo.nType,fcA2T((stDeviceInfo.strName).c_str()),fcA2T((stDeviceInfo.strIP).c_str()),
               stDeviceInfo.nPort,fcA2T((stDeviceInfo.strUser).c_str()),fcA2T((stDeviceInfo.strPassword).c_str()),
               fcA2T((stDeviceInfo.strPasswordLevel).c_str()),stDeviceInfo.nDevType,
               stDeviceInfo.nProductType,stDeviceInfo.nChanNum,fcA2T((stDeviceInfo.strDeviceID).c_str()),stDeviceInfo.telChanNum,
               stDeviceInfo.nRegionID,stDeviceInfo.nStreamType,stDeviceInfo.dvrType,stDeviceInfo.nConfigPort,
               fcA2T((stDeviceInfo.strConfigUser).c_str()),fcA2T((stDeviceInfo.strConfigPassword).c_str()),stDeviceInfo.online,
               stDeviceInfo.uiTotalSpace,stDeviceInfo.uiFreeSpace,fcA2T((stDeviceInfo.strIndexCode).c_str()),fcA2T((stDeviceInfo.strManufacture).c_str()));

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//监控点
bool CMySqlDBLayer::GetDevicechannelInfo(std::vector<devicechannel_info_t> &vecDevicechannelInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM devicechannel LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Devicechannel;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            devicechannel_info_t stDevicechannelInfo;
            try
            {
                ExtractDevicechannelInfo(stDevicechannelInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractDevicechannelInfo 异常!");
                return false;
            }
            vecDevicechannelInfo.push_back(stDevicechannelInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractDevicechannelInfo(devicechannel_info_t &stDevicechannelInfo, CMySQLRecordSet &recordSet)
{
    stDevicechannelInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    stDevicechannelInfo.nDeviceId = (unsigned int)(recordSet.GetInt32Item("nDeviceId"));
    stDevicechannelInfo.nChannelNo = (unsigned int)(recordSet.GetInt32Item("nChannelNo"));
    stDevicechannelInfo.nChannelName = fcU2A(recordSet.GetStringItem("nChannelName"));
    stDevicechannelInfo.nflag = (unsigned int)(recordSet.GetInt32Item("nflag"));
    stDevicechannelInfo.nRegionId = (unsigned int)(recordSet.GetInt32Item("nRegionId"));
    stDevicechannelInfo.recordPlane = (unsigned int)(recordSet.GetInt32Item("recordPlane"));
}
bool CMySqlDBLayer::InsertDevicechannelInfo(std::vector<devicechannel_info_t> &vecDevicechannelInfo,int nIndexVecID)
{
    for (std::vector<devicechannel_info_t>::iterator veciter=vecDevicechannelInfo.begin();veciter != vecDevicechannelInfo.end();++veciter)
    {
        devicechannel_info_t stDevicechannelInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO devicechannel(ID,nDeviceId,nChannelNo,nChannelName,nflag,nRegionId,recordPlane)\
               VALUES (NULL,%d,%d,'%s',%d,%d,%d)"),
               /*stDevicechannelInfo.ID + nIndexVecID + 1,*/stDevicechannelInfo.nDeviceId,
               stDevicechannelInfo.nChannelNo,fcA2T((stDevicechannelInfo.nChannelName).c_str()),
               stDevicechannelInfo.nflag,stDevicechannelInfo.nRegionId,stDevicechannelInfo.recordPlane);

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//产品
bool CMySqlDBLayer::GetProductInfo(std::vector<product_info_t> &vecProductInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM product LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Product;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            product_info_t stProductInfo;
            try
            {
                ExtractProductInfo(stProductInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractProductInfo 异常!");
                return false;
            }
            vecProductInfo.push_back(stProductInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractProductInfo(product_info_t &lstProductInfo, CMySQLRecordSet &recordSet)
{
    lstProductInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    lstProductInfo.strCode = fcU2A(recordSet.GetStringItem("strCode"));
    lstProductInfo.strName = fcU2A(recordSet.GetStringItem("strName"));
    lstProductInfo.pId = (unsigned int)(recordSet.GetInt32Item("pId"));
    lstProductInfo.saveYears = (unsigned int)(recordSet.GetInt32Item("saveYears"));
    lstProductInfo.guaranteeYears = (unsigned int)(recordSet.GetInt32Item("guaranteeYears"));
    lstProductInfo.nEnabled = (unsigned int)(recordSet.GetInt32Item("nEnabled"));
    lstProductInfo.strEndDate = fcU2A(recordSet.GetStringItem("strEndDate"));
    lstProductInfo.strApplyDate = fcU2A(recordSet.GetStringItem("strApplyDate"));
    lstProductInfo.strVideoEndDate = fcU2A(recordSet.GetStringItem("strVideoEndDate"));
    lstProductInfo.importUser = (unsigned int)(recordSet.GetInt32Item("importUser"));
    lstProductInfo.checkUser = (unsigned int)(recordSet.GetInt32Item("checkUser"));
    lstProductInfo.importTime = fcU2A(recordSet.GetStringItem("importTime"));
    lstProductInfo.checkTime = fcU2A(recordSet.GetStringItem("checkTime"));
    lstProductInfo.strGrade = fcU2A(recordSet.GetStringItem("strGrade"));
    lstProductInfo.strRisklevel = fcU2A(recordSet.GetStringItem("strRisklevel"));
    lstProductInfo.remark = fcU2A(recordSet.GetStringItem("remark"));
    lstProductInfo.wordTemplate = fcU2A(recordSet.GetStringItem("wordTemplate"));
    if ("" == lstProductInfo.strEndDate)
    {
        lstProductInfo.strEndDate = "2015-05-01";
    }
    if ("" == lstProductInfo.strApplyDate)
    {
        lstProductInfo.strApplyDate = "2015-05-01";
    }
    if ("" == lstProductInfo.strVideoEndDate)
    {
        lstProductInfo.strVideoEndDate = "2015-05-01";
    }
    if ("" == lstProductInfo.importTime)
    {
        lstProductInfo.importTime = "2015-05-01 00:00:00";
    }
    if ("" == lstProductInfo.checkTime)
    {
        lstProductInfo.checkTime = "2015-05-01 00:00:00";
    }

}
bool CMySqlDBLayer::InsertProductInfo(std::vector<product_info_t> &vecProductInfo,int nIndexVecID)
{
    for (std::vector<product_info_t>::iterator veciter=vecProductInfo.begin();veciter != vecProductInfo.end();++veciter)
    {
        product_info_t lstProductInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO product(ID,strCode,strName,pId,saveYears,guaranteeYears,nEnabled,\
               strEndDate,strApplyDate,strVideoEndDate,importUser,checkUser,importTime,checkTime,\
               strGrade,strRisklevel,remark,wordTemplate)\
               VALUES (NULL,'%s','%s',%d,%d,%d,%d,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s')"),
               /*lstProductInfo.ID + nIndexVecID + 1,*/fcA2T((lstProductInfo.strCode).c_str()),fcA2T((lstProductInfo.strName).c_str())
               ,lstProductInfo.pId,lstProductInfo.saveYears,lstProductInfo.guaranteeYears,lstProductInfo.nEnabled
               ,fcA2T((lstProductInfo.strEndDate).c_str()),fcA2T((lstProductInfo.strApplyDate).c_str()),fcA2T((lstProductInfo.strVideoEndDate).c_str())
               ,lstProductInfo.importUser,lstProductInfo.checkUser,fcA2T((lstProductInfo.importTime).c_str()),fcA2T((lstProductInfo.checkTime).c_str())
               ,fcA2T((lstProductInfo.strGrade).c_str()),fcA2T((lstProductInfo.strRisklevel).c_str()),fcA2T((lstProductInfo.remark).c_str()),fcA2T((lstProductInfo.wordTemplate).c_str()));

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}

//用户信息
bool CMySqlDBLayer::GetUsersInfo(std::vector<users_info_t> &vecUsersInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM users where sysRole = 1 LIMIT 0,1000"));//选择理财经理用户

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from Users;");
        return false;
    }

    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            users_info_t stUsersInfo;
            try
            {
                ExtractUsersInfo(stUsersInfo, recordSet);
            }
            catch(...)
            {
                return false;
            }
            vecUsersInfo.push_back(stUsersInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractUsersInfo(users_info_t &lstUsersInfo, CMySQLRecordSet &recordSet)
{
    lstUsersInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    lstUsersInfo.strName = fcU2A(recordSet.GetStringItem("strName"));
    lstUsersInfo.strCode = fcU2A(recordSet.GetStringItem("strCode"));
    lstUsersInfo.sysRole = (unsigned int)(recordSet.GetInt32Item("sysRole"));
    lstUsersInfo.strPassword = fcU2A(recordSet.GetStringItem("strPassword"));
    lstUsersInfo.nPasswordLevel = (unsigned int)(recordSet.GetInt32Item("nPasswordLevel"));
    lstUsersInfo.strTel = fcU2A(recordSet.GetStringItem("strTel"));
    lstUsersInfo.strEmail = fcU2A(recordSet.GetStringItem("strEmail"));
    lstUsersInfo.strComment = fcU2A(recordSet.GetStringItem("strComment"));
    lstUsersInfo.nState = (unsigned int)(recordSet.GetInt32Item("nState"));
    lstUsersInfo.strDate = fcU2A(recordSet.GetStringItem("strDate"));
    lstUsersInfo.nSex = (unsigned int)(recordSet.GetInt32Item("nSex"));
    lstUsersInfo.branch = fcU2A(recordSet.GetStringItem("branch"));
    lstUsersInfo.netId = (unsigned int)(recordSet.GetInt32Item("netId"));
    lstUsersInfo.net = fcU2A(recordSet.GetStringItem("net"));
    lstUsersInfo.strDuty = fcU2A(recordSet.GetStringItem("strDuty"));
    lstUsersInfo.userNo = fcU2A(recordSet.GetStringItem("userNo"));
    lstUsersInfo.strUpdatePwdTime = fcU2A(recordSet.GetStringItem("strUpdatePwdTime"));
    lstUsersInfo.nOwner = (unsigned int)(recordSet.GetInt32Item("nOwner"));
    lstUsersInfo.nupdated = (unsigned int)(recordSet.GetInt32Item("nupdated"));
    lstUsersInfo.nonline = (unsigned int)(recordSet.GetInt32Item("nonline"));
    lstUsersInfo.strCredentialNo = fcU2A(recordSet.GetStringItem("strCredentialNo"));
    lstUsersInfo.strDepartment = fcU2A(recordSet.GetStringItem("strDepartment"));
    lstUsersInfo.lastTimeOnline = fcU2A(recordSet.GetStringItem("lastTimeOnline"));
    lstUsersInfo.nCrtificateId = (unsigned int)(recordSet.GetInt32Item("nCrtificateId"));
    if ("" == lstUsersInfo.strDate)
    {
        lstUsersInfo.strDate = "2015-05-01";
    }
    if ("" == lstUsersInfo.lastTimeOnline)
    {
        lstUsersInfo.lastTimeOnline = "null";
    }
}
bool CMySqlDBLayer::InsertUsersInfo(std::vector<users_info_t> &vecUsersInfo,int nIndexVecID)
{
    for (std::vector<users_info_t>::iterator veciter=vecUsersInfo.begin();veciter != vecUsersInfo.end();++veciter)
    {
        users_info_t lstUsersInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO users(ID,strName,strCode,sysRole,strPassword,nPasswordLevel,strTel,strEmail,\
               strComment,nState,strDate,nSex,branch,netId,net,strDuty,userNo,strUpdatePwdTime,nOwner,nupdated,\
               nonline,strCredentialNo,strDepartment,lastTimeOnline,nCrtificateId)\
               VALUES (NULL,'%s','%s',%d,'%s',%d,'%s','%s','%s',%d,'%s',%d,'%s',%d,'%s','%s','%s','%s',%d\
               ,%d,%d,'%s','%s',%s,%d)"),
               /*lstUsersInfo.ID + nIndexVecID + 1,*/fcA2T(lstUsersInfo.strName.c_str()),fcA2T(lstUsersInfo.strCode.c_str())
               ,lstUsersInfo.sysRole,fcA2T(lstUsersInfo.strPassword.c_str()),lstUsersInfo.nPasswordLevel
               ,fcA2T(lstUsersInfo.strTel.c_str()),fcA2T(lstUsersInfo.strEmail.c_str()),fcA2T(lstUsersInfo.strComment.c_str())
               ,lstUsersInfo.nState,fcA2T(lstUsersInfo.strDate.c_str()),lstUsersInfo.nSex,fcA2T(lstUsersInfo.branch.c_str())
               ,lstUsersInfo.netId,fcA2T(lstUsersInfo.net.c_str()),fcA2T(lstUsersInfo.strDuty.c_str()),fcA2T(lstUsersInfo.userNo.c_str())
               ,fcA2T(lstUsersInfo.strUpdatePwdTime.c_str()),lstUsersInfo.nOwner,lstUsersInfo.nupdated,lstUsersInfo.nonline
               ,fcA2T(lstUsersInfo.strCredentialNo.c_str()),fcA2T(lstUsersInfo.strDepartment.c_str()),fcA2T(lstUsersInfo.lastTimeOnline.c_str()),lstUsersInfo.nCrtificateId);

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//业务单
bool CMySqlDBLayer::GetClientinputInfo(std::vector<clientinput_info_t> &vecClientinputInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM clientinput LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from clientinput;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            clientinput_info_t stClientinputInfo;
            try
            {
                ExtractClientinputInfo(stClientinputInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractClientinputInfo 异常!");
                return false;
            }
            vecClientinputInfo.push_back(stClientinputInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractClientinputInfo(clientinput_info_t &stClientinputInfo, CMySQLRecordSet &recordSet)
{
    stClientinputInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    stClientinputInfo.nProId = fcU2A(recordSet.GetStringItem("nProId"));
    stClientinputInfo.inputTime = fcU2A(recordSet.GetStringItem("inputTime"));
    stClientinputInfo.businessTime = fcU2A(recordSet.GetStringItem("businessTime"));
    stClientinputInfo.inputRegionId = (unsigned int)(recordSet.GetInt32Item("inputRegionId"));
    stClientinputInfo.inputRegionCode = fcU2A(recordSet.GetStringItem("inputRegionCode"));
    stClientinputInfo.inputRegionName = fcU2A(recordSet.GetStringItem("inputRegionName"));
    stClientinputInfo.status = (unsigned int)(recordSet.GetInt32Item("status"));
    stClientinputInfo.nisRead = (unsigned int)(recordSet.GetInt32Item("nisRead"));
    stClientinputInfo.nisPay = (unsigned int)(recordSet.GetInt32Item("nisPay"));
    stClientinputInfo.delaydays = (unsigned int)(recordSet.GetInt32Item("delaydays"));
    stClientinputInfo.strEndDate = fcU2A(recordSet.GetStringItem("strEndDate"));
    stClientinputInfo.name = fcU2A(recordSet.GetStringItem("name"));
    stClientinputInfo.creditCode = fcU2A(recordSet.GetStringItem("creditCode"));
    stClientinputInfo.creditId = fcU2A(recordSet.GetStringItem("creditId"));
    stClientinputInfo.inputUser = fcU2A(recordSet.GetStringItem("inputUser"));
    stClientinputInfo.proName = fcU2A(recordSet.GetStringItem("proName"));
    stClientinputInfo.proUserNo = fcU2A(recordSet.GetStringItem("proUserNo"));
    stClientinputInfo.productTypeId = (unsigned int)(recordSet.GetInt32Item("productTypeId"));
    stClientinputInfo.productTypeName = fcU2A(recordSet.GetStringItem("productTypeName"));
    stClientinputInfo.productId = (unsigned int)(recordSet.GetInt32Item("productId"));
    stClientinputInfo.productName = fcU2A(recordSet.GetStringItem("productName"));
    stClientinputInfo.productCode = fcU2A(recordSet.GetStringItem("productCode"));
    stClientinputInfo.strVideoEndDate = fcU2A(recordSet.GetStringItem("strVideoEndDate"));
    stClientinputInfo.guaranteeYears = (unsigned int)(recordSet.GetInt32Item("guaranteeYears"));
    stClientinputInfo.productSaveMonth = (unsigned int)(recordSet.GetInt32Item("productSaveMonth"));
    stClientinputInfo.strGrade = fcU2A(recordSet.GetStringItem("strGrade"));
    stClientinputInfo.strRisklevel = fcU2A(recordSet.GetStringItem("strRisklevel"));
    stClientinputInfo.strBankNo = fcU2A(recordSet.GetStringItem("strBankNo"));
    stClientinputInfo.strBuyMoney = fcU2A(recordSet.GetStringItem("strBuyMoney"));
    stClientinputInfo.strCreditPhone = fcU2A(recordSet.GetStringItem("strCreditPhone"));
    stClientinputInfo.strRemarks = fcU2A(recordSet.GetStringItem("strRemarks"));
    stClientinputInfo.tipflag = (unsigned int)(recordSet.GetInt32Item("tipflag"));
    stClientinputInfo.markDate = fcU2A(recordSet.GetStringItem("markDate"));
    stClientinputInfo.nisUpload = (unsigned int)(recordSet.GetInt32Item("nisUpload"));
    stClientinputInfo.saveflag = (unsigned int)(recordSet.GetInt32Item("saveflag"));
    
    stClientinputInfo.productType = (unsigned int)(recordSet.GetInt32Item("productType"));

    if ("" == stClientinputInfo.inputTime)
    {
        stClientinputInfo.inputTime="2015-05-01 00:00:00";
    }
    //
    if ("" == stClientinputInfo.businessTime)
    {
        stClientinputInfo.businessTime="2015-05-01 00:00:00";
    }
    //
    if ("" == stClientinputInfo.strEndDate)
    {
        stClientinputInfo.strEndDate="2015-05-01";
    }
    //
    if ("" == stClientinputInfo.strVideoEndDate)
    {
        stClientinputInfo.strVideoEndDate="2015-05-01";
    }
    //
    if ("" == stClientinputInfo.markDate)
    {
        stClientinputInfo.markDate="2015-05-01";
    }
}
bool CMySqlDBLayer::InsertClientinputInfo(std::vector<clientinput_info_t> &vecClientinputInfo,int nIndexVecID)
{
    for (std::vector<clientinput_info_t>::iterator veciter=vecClientinputInfo.begin();veciter != vecClientinputInfo.end();++veciter)
    {
        clientinput_info_t stClientinputInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO clientinput(ID, nProId,inputTime,businessTime,inputRegionId,inputRegionCode,\
            inputRegionName,status,nisRead,nisPay,delaydays,strEndDate,name,creditCode,creditId,inputUser,\
            proName,proUserNo,productTypeId,productTypeName,productId,productName,productCode,strVideoEndDate,\
            guaranteeYears,productSaveMonth,strGrade,strRisklevel,strBankNo,strBuyMoney,strCreditPhone,strRemarks,\
            tipflag,markDate,nisUpload,saveflag,productType)\
               VALUES (NULL,'%s','%s','%s',%d,'%s','%s',%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d\
               ,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s',%d,'%s',%d,%d,%d)"),
               /*stClientinputInfo.ID + nIndexVecID + 1,*/fcA2T(stClientinputInfo.nProId.c_str()),fcA2T(stClientinputInfo.inputTime.c_str())
               ,fcA2T(stClientinputInfo.businessTime.c_str()),stClientinputInfo.inputRegionId,fcA2T(stClientinputInfo.inputRegionCode.c_str())
               ,fcA2T(stClientinputInfo.inputRegionName.c_str()),stClientinputInfo.status,stClientinputInfo.nisRead
               ,stClientinputInfo.nisPay,stClientinputInfo.delaydays,fcA2T(stClientinputInfo.strEndDate.c_str())
               ,fcA2T(stClientinputInfo.name.c_str()),fcA2T(stClientinputInfo.creditCode.c_str()),fcA2T(stClientinputInfo.creditId.c_str())
               ,fcA2T(stClientinputInfo.inputUser.c_str()),fcA2T(stClientinputInfo.proName.c_str()),fcA2T(stClientinputInfo.proUserNo.c_str())
               ,stClientinputInfo.productTypeId,fcA2T(stClientinputInfo.productTypeName.c_str()),stClientinputInfo.productId
               ,fcA2T(stClientinputInfo.productName.c_str()),fcA2T(stClientinputInfo.productCode.c_str()),fcA2T(stClientinputInfo.strVideoEndDate.c_str())
               ,stClientinputInfo.guaranteeYears,stClientinputInfo.productSaveMonth,fcA2T(stClientinputInfo.strGrade.c_str())
               ,fcA2T(stClientinputInfo.strRisklevel.c_str()),fcA2T(stClientinputInfo.strBankNo.c_str()),fcA2T(stClientinputInfo.strBuyMoney.c_str())
               ,fcA2T(stClientinputInfo.strCreditPhone.c_str()),fcA2T(stClientinputInfo.strRemarks.c_str()),stClientinputInfo.tipflag
               ,fcA2T(stClientinputInfo.markDate.c_str()),stClientinputInfo.nisUpload,stClientinputInfo.saveflag,stClientinputInfo.productType);

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//
//录像记录
bool CMySqlDBLayer::GetProsaverecordInfo(std::vector<prosaverecord_info_t> &vecProsaverecordInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM prosaverecord LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from prosaverecord;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            prosaverecord_info_t stProsaverecordInfo;
            try
            {
                ExtractProsaverecordInfo(stProsaverecordInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractProsaverecordInfo 异常!");
                return false;
            }
            vecProsaverecordInfo.push_back(stProsaverecordInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractProsaverecordInfo(prosaverecord_info_t &stProsaverecordInfo, CMySQLRecordSet &recordSet)
{
    {
        stProsaverecordInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
        stProsaverecordInfo.recordId = (unsigned int)(recordSet.GetInt32Item("recordId"));
        stProsaverecordInfo.clientId = (unsigned int)(recordSet.GetInt32Item("clientId"));
        stProsaverecordInfo.regionId = (unsigned int)(recordSet.GetInt32Item("regionId"));
        stProsaverecordInfo.userId = (unsigned int)(recordSet.GetInt32Item("userId"));
        stProsaverecordInfo.cvr = fcU2A(recordSet.GetStringItem("cvr"));
        stProsaverecordInfo.streamNo = fcU2A(recordSet.GetStringItem("streamNo"));
        stProsaverecordInfo.stateType = (unsigned int)(recordSet.GetInt32Item("stateType"));
        stProsaverecordInfo.fileName = fcU2A(recordSet.GetStringItem("fileName"));
        stProsaverecordInfo.startTime = fcU2A(recordSet.GetStringItem("startTime"));
        stProsaverecordInfo.endTime = fcU2A(recordSet.GetStringItem("endTime"));
        stProsaverecordInfo.upload = (unsigned int)(recordSet.GetInt32Item("upload"));
        stProsaverecordInfo.recordCheck = (unsigned int)(recordSet.GetInt32Item("recordCheck"));
        stProsaverecordInfo.checkRemark = fcU2A(recordSet.GetStringItem("checkRemark"));
        stProsaverecordInfo.strUrl = fcU2A(recordSet.GetStringItem("strUrl"));

        stProsaverecordInfo.devId = (unsigned int)(recordSet.GetInt32Item("devId"));
        stProsaverecordInfo.devChannel = (unsigned int)(recordSet.GetInt32Item("devChannel"));
        stProsaverecordInfo.screenshotSrc = fcU2A(recordSet.GetStringItem("screenshotSrc"));
        stProsaverecordInfo.rtspPath = fcU2A(recordSet.GetStringItem("rtspPath"));
        stProsaverecordInfo.nType = (unsigned int)(recordSet.GetInt32Item("nType"));
        stProsaverecordInfo.mediaType = (unsigned int)(recordSet.GetInt32Item("mediaType"));
        stProsaverecordInfo.calltype = (unsigned int)(recordSet.GetInt32Item("calltype"));
        stProsaverecordInfo.callNo = fcU2A(recordSet.GetStringItem("callNo"));
        stProsaverecordInfo.recordUserNo = fcU2A(recordSet.GetStringItem("recordUserNo"));
        stProsaverecordInfo.locked = (unsigned int)(recordSet.GetInt32Item("locked"));
        stProsaverecordInfo.devIp = fcU2A(recordSet.GetStringItem("devIp"));

        //
        if ("" == stProsaverecordInfo.startTime)
        {
            stProsaverecordInfo.startTime="2015-05-01 00:00:00";
        }
        //
        if ("" == stProsaverecordInfo.endTime)
        {
            stProsaverecordInfo.endTime="2015-05-01 00:00:00";
        }
    }
}
bool CMySqlDBLayer::InsertProsaverecordInfo(std::vector<prosaverecord_info_t> &vecProsaverecordInfo,int nIndexVecID)
{
    for (std::vector<prosaverecord_info_t>::iterator veciter=vecProsaverecordInfo.begin();veciter != vecProsaverecordInfo.end();++veciter)
    {
        prosaverecord_info_t stProsaverecordInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO prosaverecord(ID,recordId,clientId,regionId,userId,cvr,streamNo,stateType,fileName,\
               startTime, endTime,upload,recordCheck,checkRemark,strUrl,devId,devChannel,screenshotSrc,rtspPath,\
               nType,mediaType,calltype,callNo,recordUserNo,locked,devIp)\
               VALUES(NULL,%d,%d,%d,%d,'%s','%s',%d,'%s','%s','%s',%d,%d,'%s','%s',%d,%d,'%s','%s',%d,%d,%d,'%s','%s',%d,'%s')"),
               /*stProsaverecordInfo.ID + nIndexVecID + 1,*/stProsaverecordInfo.recordId,stProsaverecordInfo.clientId,stProsaverecordInfo.regionId
               ,stProsaverecordInfo.userId,fcA2T(stProsaverecordInfo.cvr.c_str()),fcA2T(stProsaverecordInfo.streamNo.c_str())
               ,stProsaverecordInfo.stateType,fcA2T(stProsaverecordInfo.fileName.c_str()),fcA2T(stProsaverecordInfo.startTime.c_str())
               ,fcA2T(stProsaverecordInfo.endTime.c_str()),stProsaverecordInfo.upload,stProsaverecordInfo.recordCheck
               ,fcA2T(stProsaverecordInfo.checkRemark.c_str()),fcA2T(stProsaverecordInfo.strUrl.c_str()),stProsaverecordInfo.devId
               ,stProsaverecordInfo.devChannel,fcA2T(stProsaverecordInfo.screenshotSrc.c_str()),fcA2T(stProsaverecordInfo.rtspPath.c_str())
               ,stProsaverecordInfo.nType,stProsaverecordInfo.mediaType,stProsaverecordInfo.calltype,fcA2T(stProsaverecordInfo.callNo.c_str())
               ,fcA2T(stProsaverecordInfo.recordUserNo.c_str()),stProsaverecordInfo.locked,fcA2T(stProsaverecordInfo.devIp.c_str()));

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//报警日志
bool CMySqlDBLayer::GetAlarmlogInfo(std::vector<alarmlog_info_t> &vecAlarmlogInfo)
{
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM alarmlog LIMIT 0,1000"));

    CMySQLRecordSet recordSet;
    bool bRet = false;
    try
    {
        bRet = ExecSQL(sqlStmt, recordSet);
    }
    catch(...)
    {
        TPLOG_ERROR("使用数据库异常!");
        return false;
    }
    if(!bRet)
    {
        TPLOG_ERROR("查询数据库语句:select count(*) from alarmlog;");
        return false;
    }
    //
    if (!recordSet.IsEmpty())
    {
        while (!recordSet.IsEOF())
        {
            alarmlog_info_t stAlarmlogInfo;
            try
            {
                ExtractAlarmlogInfo(stAlarmlogInfo, recordSet);
            }
            catch(...)
            {
                TPLOG_ERROR("ExtractAlarmlogInfo 异常!");
                return false;
            }
            vecAlarmlogInfo.push_back(stAlarmlogInfo);
            recordSet.MoveNext();
        }
    }
    return true;
}
void CMySqlDBLayer::ExtractAlarmlogInfo(alarmlog_info_t &stAlarmlogInfo, CMySQLRecordSet &recordSet)
{
    stAlarmlogInfo.ID = (unsigned int)(recordSet.GetInt32Item("ID"));
    stAlarmlogInfo.checkType = (unsigned int)(recordSet.GetInt32Item("checkType"));
    stAlarmlogInfo.checkObjId = (unsigned int)(recordSet.GetInt32Item("checkObjId"));
    stAlarmlogInfo.checkChan = (unsigned int)(recordSet.GetInt32Item("checkChan"));
    stAlarmlogInfo.contactObjId = (unsigned int)(recordSet.GetInt32Item("contactObjId"));
    stAlarmlogInfo.alarmType = (unsigned int)(recordSet.GetInt32Item("alarmType"));
    stAlarmlogInfo.alarmTime = fcU2A(recordSet.GetStringItem("alarmTime"));
    stAlarmlogInfo.alarmbeginTime = fcU2A(recordSet.GetStringItem("alarmbeginTime"));
    stAlarmlogInfo.alarmRegionId = (unsigned int)(recordSet.GetInt32Item("alarmRegionId"));
    stAlarmlogInfo.comments = fcU2A(recordSet.GetStringItem("comments"));
    stAlarmlogInfo.strHost = fcU2A(recordSet.GetStringItem("strHost"));
    stAlarmlogInfo.status = (unsigned int)(recordSet.GetInt32Item("status"));
    stAlarmlogInfo.checker = fcU2A(recordSet.GetStringItem("checker"));
    stAlarmlogInfo.userNo = fcU2A(recordSet.GetStringItem("userNo"));
    stAlarmlogInfo.dealTime = fcU2A(recordSet.GetStringItem("dealTime"));
    stAlarmlogInfo.dealcomment = fcU2A(recordSet.GetStringItem("dealcomment"));
    stAlarmlogInfo.alarmlevel = (unsigned int)(recordSet.GetInt32Item("alarmlevel"));
    stAlarmlogInfo.strGuid = fcU2A(recordSet.GetStringItem("strGuid"));
    if ("" == stAlarmlogInfo.alarmTime)
    {
        stAlarmlogInfo.alarmTime = "2015-05-01 00:00:00";
    }
    if ("" == stAlarmlogInfo.alarmbeginTime)
    {
        stAlarmlogInfo.alarmbeginTime = "2015-05-01 00:00:00";
    }
    if ("" == stAlarmlogInfo.dealTime)
    {
        stAlarmlogInfo.dealTime = "2015-05-01 00:00:00";
    }
}
bool CMySqlDBLayer::InsertAlarmlogInfo(std::vector<alarmlog_info_t> &vecAlarmlogInfo,int nIndexVecID)
{
    for (std::vector<alarmlog_info_t>::iterator veciter=vecAlarmlogInfo.begin();veciter != vecAlarmlogInfo.end();++veciter)
    {
        alarmlog_info_t stAlarmlogInfo=(*veciter);
        TCHAR sqlStmt[MAX_SQL_LEN] = {0};
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO alarmlog(ID,checkType,checkObjId,checkChan,contactObjId,alarmType,alarmTime,\
               alarmbeginTime,alarmRegionId,comments,strHost,status,checker,userNo,dealTime,dealcomment,alarmlevel,strGuid) \
               VALUES(NULL,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','%s',%d,'%s','%s','%s','%s',%d,'%s')"),
               /*stAlarmlogInfo.ID + nIndexVecID + 1,*/stAlarmlogInfo.checkType,stAlarmlogInfo.contactObjId,stAlarmlogInfo.checkChan
               ,stAlarmlogInfo.contactObjId,stAlarmlogInfo.alarmType
               ,fcA2T(stAlarmlogInfo.alarmTime.c_str()),fcA2T(stAlarmlogInfo.alarmbeginTime.c_str())
               ,stAlarmlogInfo.alarmRegionId,fcA2T(stAlarmlogInfo.comments.c_str()),fcA2T(stAlarmlogInfo.strHost.c_str())
               ,stAlarmlogInfo.status,fcA2T(stAlarmlogInfo.checker.c_str()),fcA2T(stAlarmlogInfo.userNo.c_str())
               ,fcA2T(stAlarmlogInfo.dealTime.c_str()),fcA2T(stAlarmlogInfo.dealcomment.c_str()),stAlarmlogInfo.alarmlevel
               ,fcA2T(stAlarmlogInfo.strGuid.c_str()));

        if(!ExecSQL(sqlStmt))
        {
            return false;
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////
bool CMySqlDBLayer::StartSimulateRegion(const std::vector<region_info_t> &vecRegionInfo,int &nRegionNumber)
{
    int nRegionSize = vecRegionInfo.size();
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    for (int i = 0;i < nRegionNumber;i++)
    {
        int nIndex = (int)(rand() * (nRegionSize - 1))/(RAND_MAX + 1.0);
        int nIndexName = (int)(rand() * (nRegionNumber - 1))/(RAND_MAX + 1.0);
        std::string strName = "";
        char buf[100];
        sprintf(buf, "%d", nIndexName);
        strName = buf;
        memset(sqlStmt,0,MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO region(ID,nParentId,nSortId,strInterAreaCode,strPath,strName,strCode,strTxt)\
               VALUES (NULL,%d,%d,'%s','%s','%s','%s','%s')")
               ,/*vecRegionInfo[nRegionSize - 1].ID + i + 2,*/vecRegionInfo[nIndex].nParentID,vecRegionInfo[nRegionSize - 1].ID + i + 2/*vecRegionInfo[nIndex ].nSortID*/,fcA2T((vecRegionInfo[nIndex].strInterAreaCode + strName).c_str())
               ,fcA2T((vecRegionInfo[nIndex].strPath + strName).c_str()),fcA2T((vecRegionInfo[nIndex].strName + strName).c_str()),fcA2T((vecRegionInfo[nIndex].strCode).c_str())
               ,fcA2T((vecRegionInfo[nIndex].strTxt).c_str()));
        if(!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("StartSimulateRegion is failed..sqlStmt:%s",sqlStmt);
            return false;
        }
    }
    return true;
}
bool CMySqlDBLayer::StartSimulateDevice(const std::vector<device_info_t> &vecDeviceInfo,int &nDeviceNumber)
{
    int nDeviceSize = vecDeviceInfo.size();
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    for (int i = 0;i < nDeviceNumber;i++)
    {
        int nIndex = (int)(rand() * (nDeviceSize - 1))/(RAND_MAX + 1.0);
        int nIndexName = (int)(rand() * (nDeviceNumber - 1))/(RAND_MAX + 1.0);
        std::string strName = "";
        char buf[100];
        sprintf(buf, "%d", nIndexName);
        strName = buf;
        memset(sqlStmt,0,MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO device(ID,nType,strName,strIP,\
               nPort,strUser,strPassword,strPasswordLevel,nDevType,nProductType,nChanNum,\
               strDeviceID,telChanNum,nRegionID,nStreamType,dvrType,nConfigPort,strConfigUser,strConfigPassword,\
               online,uiTotalSpace,uiFreeSpace,strIndexCode,strManufacture) \
               VALUES (NULL,%d,'%s','%s',%d,'%s','%s','%s',%d,%d,%d,'%s',%d,%d,%d,%d,%d,'%s','%s',%d,%d,%d,'%s','%s')"),
               /*vecDeviceInfo[nDeviceSize - 1].ID + i + 2,*/vecDeviceInfo[nIndex].nType,fcA2T((vecDeviceInfo[nIndex].strName + strName).c_str()),fcA2T((vecDeviceInfo[nIndex].strIP).c_str()),
               vecDeviceInfo[nIndex].nPort,fcA2T((vecDeviceInfo[nIndex].strUser).c_str()),fcA2T((vecDeviceInfo[nIndex].strPassword).c_str()),
               fcA2T((vecDeviceInfo[nIndex].strPasswordLevel).c_str()),vecDeviceInfo[nIndex].nDevType,
               vecDeviceInfo[nIndex].nProductType,vecDeviceInfo[nIndex].nChanNum,fcA2T((vecDeviceInfo[nIndex].strDeviceID).c_str()),vecDeviceInfo[nIndex].telChanNum,
               vecDeviceInfo[nIndex].nRegionID,vecDeviceInfo[nIndex].nStreamType,vecDeviceInfo[nIndex].dvrType,vecDeviceInfo[nIndex].nConfigPort,
               fcA2T((vecDeviceInfo[nIndex].strConfigUser).c_str()),fcA2T((vecDeviceInfo[nIndex].strConfigPassword).c_str()),vecDeviceInfo[nIndex].online,
               vecDeviceInfo[nIndex].uiTotalSpace,vecDeviceInfo[nIndex].uiFreeSpace,fcA2T((vecDeviceInfo[nIndex].strIndexCode + strName).c_str()),fcA2T((vecDeviceInfo[nIndex].strManufacture).c_str()));
        if(!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("StartSimulateDevice is failed..sqlStmt:%s",sqlStmt);
            return false;
        }
    }
    return true;
}
bool CMySqlDBLayer::StartSimulateDevicechannel(const std::vector<devicechannel_info_t> &vecDevicechannelInfo,int &nDevicechannelNumber)
{
    int nDevicechannelSize = vecDevicechannelInfo.size();
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    for (int i = 0;i < nDevicechannelNumber;i++)
    {
        //随机从device表取一条DVR设备信息
        //std::vector<device_info_t> DvrDeviceInfo = {0};
        //bool bret = GetDeviceInfoOneDvr(DvrDeviceInfo);
        //if (!bret)
        //{
        //    TPLOG_ERROR("GetDeviceInfoOneDvr is failed..DvrDeviceInfo size:%d",DvrDeviceInfo.size());
        //}
        //
        int nIndex = (int)(rand() * (nDevicechannelSize - 1))/(RAND_MAX + 1.0);
        int nIndexName = (int)(rand() * (nDevicechannelNumber - 1))/(RAND_MAX + 1.0);
        std::string strName = "";
        char buf[100];
        sprintf(buf, "%d", nIndexName);
        strName = buf;
        memset(sqlStmt,0,MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO devicechannel(ID,nDeviceId,nChannelNo,nChannelName,nflag,nRegionId,recordPlane)\
               VALUES (NULL,%d,%d,'%s',%d,%d,%d)"),
               /*vecDevicechannelInfo[nDevicechannelSize - 1].ID + i + 2,*/vecDevicechannelInfo[nIndex].nDeviceId,
               vecDevicechannelInfo[nIndex].nChannelNo,fcA2T((vecDevicechannelInfo[nIndex].nChannelName + strName).c_str()),
               vecDevicechannelInfo[nIndex].nflag,vecDevicechannelInfo[nIndex].nRegionId,vecDevicechannelInfo[nIndex].recordPlane);
        if(!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("StartSimulateDevicechannel is failed..sqlStmt:%s",sqlStmt);
            return false;
        }
    }
    return true;
}
bool CMySqlDBLayer::StartSimulateProduct(const std::vector<product_info_t> &vecProductInfo,int &nProductNumber)
{
    int nProductSize = vecProductInfo.size();
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    for (int i = 0;i < nProductNumber;i++)
    {
        int nIndex = (int)(rand() * (nProductSize - 1))/(RAND_MAX + 1.0);
        int nIndexName = (int)(rand() * (nProductNumber - 1))/(RAND_MAX + 1.0);
        std::string strName = "";
        char buf[100];
        sprintf(buf, "%d", nIndexName);
        strName = buf;
        memset(sqlStmt,0,MAX_SQL_LEN);
        _stprintf_s(sqlStmt, _countof(sqlStmt),
            _T("INSERT INTO product(ID,strCode,strName,pId,saveYears,guaranteeYears,nEnabled,\
               strEndDate,strApplyDate,strVideoEndDate,importUser,checkUser,importTime,checkTime,\
               strGrade,strRisklevel,remark,wordTemplate)\
               VALUES (NULL,'%s','%s',%d,%d,%d,%d,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s')"),
               /*vecProductInfo[nProductSize - 1].ID + i + 2,*/fcA2T((vecProductInfo[nIndex].strCode + strName).c_str()),fcA2T((vecProductInfo[nIndex].strName + strName).c_str())
               ,vecProductInfo[nIndex].pId,vecProductInfo[nIndex].saveYears,vecProductInfo[nIndex].guaranteeYears,vecProductInfo[nIndex].nEnabled
               ,fcA2T((vecProductInfo[nIndex].strEndDate).c_str()),fcA2T((vecProductInfo[nIndex].strApplyDate).c_str()),fcA2T((vecProductInfo[nIndex].strVideoEndDate).c_str())
               ,vecProductInfo[nIndex].importUser,vecProductInfo[nIndex].checkUser,fcA2T((vecProductInfo[nIndex].importTime).c_str()),fcA2T((vecProductInfo[nIndex].checkTime).c_str())
               ,fcA2T((vecProductInfo[nIndex].strGrade).c_str()),fcA2T((vecProductInfo[nIndex].strRisklevel).c_str()),fcA2T((vecProductInfo[nIndex].remark).c_str()),fcA2T((vecProductInfo[nIndex].wordTemplate).c_str()));
        if(!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("StartSimulateProduct is failed..sqlStmt:%s",sqlStmt);
            return false;
        }
    }
    return true;
}
bool CMySqlDBLayer::StartSimulateUsers(const std::vector<users_info_t> &UsersInfo,int &nUsersNumber)
{
    std::vector<users_info_t> vecUsersInfo;
    for (int n = 0;n < UsersInfo.size();n++)
    {
        if (1 == UsersInfo[n].sysRole)
        {
            vecUsersInfo.push_back(UsersInfo[n]);//只模拟理财经理用户数量
        }
    }
    int nUsersSize = vecUsersInfo.size();
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    for (int i = 0;i < nUsersNumber;i++)
    {
        int nIndex = (int)(rand() * (nUsersSize - 1))/(RAND_MAX + 1.0);
        int nIndexName = (int)(rand() * (nUsersNumber - 1))/(RAND_MAX + 1.0);
        std::string strName = "";
        char buf[100];
        sprintf(buf, "%d", nIndexName);
        strName = buf;
        std::stringstream ss;
        std::string strCode = "";
        SYSTEMTIME sys;
        GetLocalTime( &sys );
        ss << sys.wMilliseconds;
        ss >> strCode;
        memset(sqlStmt,0,MAX_SQL_LEN);
        if ("null" == vecUsersInfo[nIndex].lastTimeOnline)
        {
            _stprintf_s(sqlStmt, _countof(sqlStmt),
                _T("INSERT INTO users(ID,strName,strCode,sysRole,strPassword,nPasswordLevel,strTel,strEmail,\
                   strComment,nState,strDate,nSex,branch,netId,net,strDuty,userNo,strUpdatePwdTime,nOwner,nupdated,\
                   nonline,strCredentialNo,strDepartment,lastTimeOnline,nCrtificateId)\
                   VALUES (NULL,'%s','%s',%d,'%s',%d,'%s','%s','%s',%d,'%s',%d,'%s',%d,'%s','%s','%s','%s',%d\
                   ,%d,%d,'%s','%s',%s,%d)"),
                   /*vecUsersInfo[nUsersSize - 1].ID + i + 2,*/fcA2T((vecUsersInfo[nIndex].strName + strName).c_str()),fcA2T((vecUsersInfo[nIndex].strCode + strName).c_str())
                   ,vecUsersInfo[nIndex].sysRole,fcA2T(vecUsersInfo[nIndex].strPassword.c_str()),vecUsersInfo[nIndex].nPasswordLevel
                   ,fcA2T(vecUsersInfo[nIndex].strTel.c_str()),fcA2T(vecUsersInfo[nIndex].strEmail.c_str()),fcA2T(vecUsersInfo[nIndex].strComment.c_str())
                   ,vecUsersInfo[nIndex].nState,fcA2T(vecUsersInfo[nIndex].strDate.c_str()),vecUsersInfo[nIndex].nSex,fcA2T(vecUsersInfo[nIndex].branch.c_str())
                   ,vecUsersInfo[nIndex].netId,fcA2T(vecUsersInfo[nIndex].net.c_str()),fcA2T(vecUsersInfo[nIndex].strDuty.c_str()),fcA2T((vecUsersInfo[nIndex].userNo + strName + strCode).c_str())
                   ,fcA2T(vecUsersInfo[nIndex].strUpdatePwdTime.c_str()),vecUsersInfo[nIndex].nOwner,vecUsersInfo[nIndex].nupdated,vecUsersInfo[nIndex].nonline
                   ,fcA2T(vecUsersInfo[nIndex].strCredentialNo.c_str()),fcA2T(vecUsersInfo[nIndex].strDepartment.c_str()),fcA2T(vecUsersInfo[nIndex].lastTimeOnline.c_str()),vecUsersInfo[nIndex].nCrtificateId);
        }
        else
        {
            _stprintf_s(sqlStmt, _countof(sqlStmt),
                _T("INSERT INTO users(ID,strName,strCode,sysRole,strPassword,nPasswordLevel,strTel,strEmail,\
                   strComment,nState,strDate,nSex,branch,netId,net,strDuty,userNo,strUpdatePwdTime,nOwner,nupdated,\
                   nonline,strCredentialNo,strDepartment,lastTimeOnline,nCrtificateId)\
                   VALUES (NULL,'%s','%s',%d,'%s',%d,'%s','%s','%s',%d,'%s',%d,'%s',%d,'%s','%s','%s','%s',%d\
                   ,%d,%d,'%s','%s','%s',%d)"),
                   /*vecUsersInfo[nUsersSize - 1].ID + i + 2,*/fcA2T((vecUsersInfo[nIndex].strName + strName).c_str()),fcA2T((vecUsersInfo[nIndex].strCode + strName).c_str())
                   ,vecUsersInfo[nIndex].sysRole,fcA2T(vecUsersInfo[nIndex].strPassword.c_str()),vecUsersInfo[nIndex].nPasswordLevel
                   ,fcA2T(vecUsersInfo[nIndex].strTel.c_str()),fcA2T(vecUsersInfo[nIndex].strEmail.c_str()),fcA2T(vecUsersInfo[nIndex].strComment.c_str())
                   ,vecUsersInfo[nIndex].nState,fcA2T(vecUsersInfo[nIndex].strDate.c_str()),vecUsersInfo[nIndex].nSex,fcA2T(vecUsersInfo[nIndex].branch.c_str())
                   ,vecUsersInfo[nIndex].netId,fcA2T(vecUsersInfo[nIndex].net.c_str()),fcA2T(vecUsersInfo[nIndex].strDuty.c_str()),fcA2T((vecUsersInfo[nIndex].userNo + strName + strCode).c_str())
                   ,fcA2T(vecUsersInfo[nIndex].strUpdatePwdTime.c_str()),vecUsersInfo[nIndex].nOwner,vecUsersInfo[nIndex].nupdated,vecUsersInfo[nIndex].nonline
                   ,fcA2T(vecUsersInfo[nIndex].strCredentialNo.c_str()),fcA2T(vecUsersInfo[nIndex].strDepartment.c_str()),fcA2T(vecUsersInfo[nIndex].lastTimeOnline.c_str()),vecUsersInfo[nIndex].nCrtificateId);
        }

        if(!ExecSQL(sqlStmt))
        {
            TPLOG_ERROR("StartSimulateUsers is failed..sqlStmt:%s",sqlStmt);
            return false;
        }
    }
    return true;
}
bool CMySqlDBLayer::StartSimulateClientinput(const std::vector<clientinput_info_t> &vecClientinputInfo,int &nClientinputNumber)
{
    const int nClientinputSize = vecClientinputInfo.size();
    TCHAR sqlTitle[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlTitle,_countof(sqlTitle),
        _T("INSERT INTO clientinput(ID, nProId,inputTime,businessTime,inputRegionId,inputRegionCode,\
           inputRegionName,status,nisRead,nisPay,delaydays,strEndDate,name,creditCode,creditId,inputUser,\
           proName,proUserNo,productTypeId,productTypeName,productId,productName,productCode,strVideoEndDate,\
           guaranteeYears,productSaveMonth,strGrade,strRisklevel,strBankNo,strBuyMoney,strCreditPhone,strRemarks,\
           tipflag,markDate,nisUpload,saveflag,productType) VALUES"));
    std::string strTitle = fcT2A(sqlTitle);
    time_t tT1 = time(NULL);
    for (int i = 0;i < nClientinputNumber/100;i++)
    {
        TCHAR sqlContent[MAX_SQL_LEN] = {0};
        std::string strContent = "";
        std::string sqlStmt = "";
        int j = 0;
        for (;j < 100;j++)
        {
            std::string str = "";
            memset(sqlContent,0,MAX_SQL_LEN);
            int nIndex = (int)(rand() * (nClientinputSize - 1))/(RAND_MAX + 1.0);
            int nIndexName = (int)(rand() * (nClientinputNumber - 1))/(RAND_MAX + 1.0);
            std::string strName = "";
            char buf[100];
            sprintf(buf, "%d", nIndexName);
            strName = buf;
            _stprintf_s(sqlContent,_countof(sqlContent),
                _T("(NULL,'%s','%s','%s',%d,'%s','%s',%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d\
                ,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s',%d,'%s',%d,%d,%d)"),
                /*vecClientinputInfo[nClientinputSize - 1].ID + i*100 + j + 2,*/fcA2T((vecClientinputInfo[nIndex].nProId + strName).c_str()),fcA2T(vecClientinputInfo[nIndex].inputTime.c_str())
                ,fcA2T(vecClientinputInfo[nIndex].businessTime.c_str()),vecClientinputInfo[nIndex].inputRegionId,fcA2T(vecClientinputInfo[nIndex].inputRegionCode.c_str())
                ,fcA2T(vecClientinputInfo[nIndex].inputRegionName.c_str()),vecClientinputInfo[nIndex].status,vecClientinputInfo[nIndex].nisRead
                ,vecClientinputInfo[nIndex].nisPay,vecClientinputInfo[nIndex].delaydays,fcA2T(vecClientinputInfo[nIndex].strEndDate.c_str())
                ,fcA2T((vecClientinputInfo[nIndex].name + strName).c_str()),fcA2T(vecClientinputInfo[nIndex].creditCode.c_str()),fcA2T(vecClientinputInfo[nIndex].creditId.c_str())
                ,fcA2T(vecClientinputInfo[nIndex].inputUser.c_str()),fcA2T(vecClientinputInfo[nIndex].proName.c_str()),fcA2T(vecClientinputInfo[nIndex].proUserNo.c_str())
                ,vecClientinputInfo[nIndex].productTypeId,fcA2T(vecClientinputInfo[nIndex].productTypeName.c_str()),vecClientinputInfo[nIndex].productId
                ,fcA2T((vecClientinputInfo[nIndex].productName).c_str()),fcA2T(vecClientinputInfo[nIndex].productCode.c_str()),fcA2T(vecClientinputInfo[nIndex].strVideoEndDate.c_str())
                ,vecClientinputInfo[nIndex].guaranteeYears,vecClientinputInfo[nIndex].productSaveMonth,fcA2T(vecClientinputInfo[nIndex].strGrade.c_str())
                ,fcA2T(vecClientinputInfo[nIndex].strRisklevel.c_str()),fcA2T(vecClientinputInfo[nIndex].strBankNo.c_str()),fcA2T(vecClientinputInfo[nIndex].strBuyMoney.c_str())
                ,fcA2T(vecClientinputInfo[nIndex].strCreditPhone.c_str()),fcA2T(vecClientinputInfo[nIndex].strRemarks.c_str()),vecClientinputInfo[nIndex].tipflag
                ,fcA2T(vecClientinputInfo[nIndex].markDate.c_str()),vecClientinputInfo[nIndex].nisUpload,vecClientinputInfo[nIndex].saveflag,vecClientinputInfo[nIndex].productType);
            if (99 != j)
            {
                str = ",";
            }
            else
            {
                str = ";";
            }
            strContent = strContent + fcT2A(sqlContent) + str;
        }
        sqlStmt = strTitle + strContent;
        if(!ExecSQL(fcA2T(sqlStmt.c_str())))
        {
            TPLOG_ERROR("vecClientinputInfo is failed..sqlStmt:%s",sqlStmt.c_str());
            return false;
        }
    }
    time_t tT2 = time(NULL);
    TPLOG_INFO("StartSimulateClientinput插完%d条数据耗时%d 秒",nClientinputNumber,tT2 - tT1);
    return true;
}
bool CMySqlDBLayer::StartSimulateAlarmlog(const std::vector<alarmlog_info_t> &vecAlarmlogInfo,int &nAlarmlogNumber)
{
    const int nAlarmlogSize = vecAlarmlogInfo.size();
    TCHAR sqlTitle[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlTitle,_countof(sqlTitle),
        _T("INSERT INTO alarmlog(ID,checkType,checkObjId,checkChan,contactObjId,alarmType,alarmTime,\
           alarmbeginTime,alarmRegionId,comments,strHost,status,checker,userNo,dealTime,dealcomment,alarmlevel,strGuid) VALUES"));
    std::string strTitle = fcT2A(sqlTitle);
    time_t tT1 = time(NULL);
    for (int i = 0;i < nAlarmlogNumber/100;i++)
    {
        TCHAR sqlContent[MAX_SQL_LEN] = {0};
        std::string strContent = "";
        std::string sqlStmt = "";
        int j = 0;
        for (;j < 100;j++)
        {
            std::string str = "";
            memset(sqlContent,0,MAX_SQL_LEN);
            int nIndex = (int)(rand() * (nAlarmlogSize - 1))/(RAND_MAX + 1.0);
            _stprintf_s(sqlContent,_countof(sqlContent),
                _T("(NULL,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','%s',%d,'%s','%s','%s','%s',%d,'%s')"),
                /*vecAlarmlogInfo[nAlarmlogSize - 1].ID + i*100 + j + 2,*/vecAlarmlogInfo[nIndex].checkType,vecAlarmlogInfo[nIndex].contactObjId,vecAlarmlogInfo[nIndex].checkChan
                ,vecAlarmlogInfo[nIndex].contactObjId,vecAlarmlogInfo[nIndex].alarmType
                ,fcA2T(vecAlarmlogInfo[nIndex].alarmTime.c_str()),fcA2T(vecAlarmlogInfo[nIndex].alarmbeginTime.c_str())
                ,vecAlarmlogInfo[nIndex].alarmRegionId,fcA2T(vecAlarmlogInfo[nIndex].comments.c_str()),fcA2T(vecAlarmlogInfo[nIndex].strHost.c_str())
                ,vecAlarmlogInfo[nIndex].status,fcA2T(vecAlarmlogInfo[nIndex].checker.c_str()),fcA2T(vecAlarmlogInfo[nIndex].userNo.c_str())
                ,fcA2T(vecAlarmlogInfo[nIndex].dealTime.c_str()),fcA2T(vecAlarmlogInfo[nIndex].dealcomment.c_str()),vecAlarmlogInfo[nIndex].alarmlevel
                ,fcA2T(vecAlarmlogInfo[nIndex].strGuid.c_str()));
            if (99 != j)
            {
                str = ",";
            }
            else
            {
                str = ";";
            }
            strContent = strContent + fcT2A(sqlContent) + str;
        }
        sqlStmt = strTitle + strContent;
        if(!ExecSQL(fcA2T(sqlStmt.c_str())))
        {
            TPLOG_ERROR("StartSimulateAlarmlog is failed..sqlStmt:%s",sqlStmt.c_str());
            return false;
        }
    }
    time_t tT2 = time(NULL);
    TPLOG_INFO("StartSimulateAlarmlog插完%d条数据耗时%d 秒",nAlarmlogNumber,tT2 - tT1);
    return true;
}
bool CMySqlDBLayer::StartSimulateProsaveRecord(const std::vector<prosaverecord_info_t> &vecProsaverecordInfo,int &nProsaverecordNumber)
{
    int nProsaverecordSize = vecProsaverecordInfo.size();
    TCHAR sqlTitle[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlTitle,_countof(sqlTitle),
        _T("INSERT INTO prosaverecord(ID,recordId,clientId,regionId,userId,cvr,streamNo,stateType,fileName,\
           startTime, endTime,upload,recordCheck,checkRemark,strUrl,devId,devChannel,screenshotSrc,rtspPath,\
           nType,mediaType,calltype,callNo,recordUserNo,locked,devIp) VALUES"));
    std::string strTitle = fcT2A(sqlTitle);
    time_t tT1 = time(NULL);
    //BeginTransaction();
    for (int i = 0;i < nProsaverecordNumber/100;i++)
    {
        //BeginTransaction();
        TCHAR sqlContent[MAX_SQL_LEN] = {0};
        std::string strContent = "";
        std::string sqlStmt = "";
        int j = 0;
        for (;j < 100;j++)
        {
            std::string str = "";
            memset(sqlContent,0,MAX_SQL_LEN);
            int nIndex = (int)(rand() * (nProsaverecordSize - 1))/(RAND_MAX + 1.0);
            int nIndexName = (int)(rand() * (nProsaverecordNumber - 1))/(RAND_MAX + 1.0);
            std::string strName = "";
            char buf[100];
            sprintf(buf, "%d", nIndexName);
            strName = buf;
            _stprintf_s(sqlContent,_countof(sqlContent),
                _T("(NULL,%d,%d,%d,%d,'%s','%s',%d,'%s','%s','%s',%d,%d,'%s','%s',%d,%d,'%s','%s',%d,%d,%d,'%s','%s',%d,'%s')"),
                   /*vecProsaverecordInfo[nProsaverecordSize - 1].ID + i*100 + j + 2,*/vecProsaverecordInfo[nProsaverecordSize - 1].ID + i*100 + j + 2/*vecProsaverecordInfo[nIndex].recordId*/,vecProsaverecordInfo[nIndex].clientId,vecProsaverecordInfo[nIndex].regionId
                   ,vecProsaverecordInfo[nIndex].userId,fcA2T(vecProsaverecordInfo[nIndex].cvr.c_str()),fcA2T(vecProsaverecordInfo[nIndex].streamNo.c_str())
                   ,vecProsaverecordInfo[nIndex].stateType,fcA2T((vecProsaverecordInfo[nIndex].fileName + strName).c_str()),fcA2T(vecProsaverecordInfo[nIndex].startTime.c_str())
                   ,fcA2T(vecProsaverecordInfo[nIndex].endTime.c_str()),vecProsaverecordInfo[nIndex].upload,vecProsaverecordInfo[nIndex].recordCheck
                   ,fcA2T(vecProsaverecordInfo[nIndex].checkRemark.c_str()),fcA2T(vecProsaverecordInfo[nIndex].strUrl.c_str()),vecProsaverecordInfo[nIndex].devId
                   ,vecProsaverecordInfo[nIndex].devChannel,fcA2T(vecProsaverecordInfo[nIndex].screenshotSrc.c_str()),fcA2T(vecProsaverecordInfo[nIndex].rtspPath.c_str())
                   ,vecProsaverecordInfo[nIndex].nType,vecProsaverecordInfo[nIndex].mediaType,vecProsaverecordInfo[nIndex].calltype,fcA2T(vecProsaverecordInfo[nIndex].callNo.c_str())
                   ,fcA2T(vecProsaverecordInfo[nIndex].recordUserNo.c_str()),vecProsaverecordInfo[nIndex].locked,fcA2T(vecProsaverecordInfo[nIndex].devIp.c_str()));
            if (99 != j)
            {
                str = ",";
            }
            else
            {
                str = ";";
            }
            strContent = strContent + fcT2A(sqlContent) + str;
        }
        sqlStmt = strTitle + strContent;
        if(!ExecSQL(fcA2T(sqlStmt.c_str())))
        {
            //RollBackTransaction();
            TPLOG_ERROR("StartSimulateProsaveRecord is failed..sqlStmt:%s",sqlStmt.c_str());
            return false;
        }
        //CommitTransaction();
    }
    //CommitTransaction();
    time_t tT2 = time(NULL);
    TPLOG_INFO("StartSimulateProsaveRecord插完%d条数据耗时%d 秒",nProsaverecordNumber,tT2 - tT1);
    return true;
}
//
bool CMySqlDBLayer::BeginTransaction()
{
    return ExecSQL(_T("begin transaction"));
}
bool CMySqlDBLayer::CommitTransaction()
{
    return ExecSQL(_T("commit transaction"));
}

bool CMySqlDBLayer::RollBackTransaction()
{
    return ExecSQL(_T("rollback transaction"));
}