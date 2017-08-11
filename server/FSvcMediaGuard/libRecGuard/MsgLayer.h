#pragma once
#include "Center/Records.pb.h"
#include "Server/RspServerData.pb.h"
#include "Center/StorageConfig.pb.h"
#include "verify/RspServerLogin.pb.h"

#include "DBDataMgr.h"
#include "RecGuard.h"
//登录返回服务器的信息
template<> inline WrapIf* PBWrap<cms_8120::RspServerLogin>::DoExecute()
{
    TPLOG_INFO("cms_8120::RspServerLogin::DoExecute...");

    cms_8120::RspServerLogin & r_msg = GetMsg();
    CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);

    return NULL;
}

//返回录像守卫服务工作时间信息
template<> inline WrapIf* PBWrap<cms_8120::SysconfigList>::DoExecute()
{
    TPLOG_INFO("cms_8120::SysconfigList::DoExecute...");

    cms_8120::SysconfigList & r_msg = GetMsg();
    //CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);
    GetRecGuardPtr()->HandleSysConfig(r_msg);

    return NULL;
}

//返回过期录像信息
template<> inline WrapIf* PBWrap<cms_8120::RspOvertimeRecord>::DoExecute()
{
    TPLOG_INFO("cms_8120::OvertimeRecord::DoExecute...");

    cms_8120::RspOvertimeRecord & r_msg = GetMsg();
    GetRecGuardPtr()->HandleOverRecord(r_msg);
    //CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);

    return NULL;
}

//返回录像信息
template<> inline WrapIf* PBWrap<cms_8120::ProsaverecordList>::DoExecute()
{
    TPLOG_INFO("cms_8120::ProsaverecordList::DoExecute...");

    cms_8120::ProsaverecordList & r_msg = GetMsg();
    //CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);
    GetRecGuardPtr()->HandleRecord(r_msg);
    return NULL;
}

//返回存储设备信息
template<> inline WrapIf* PBWrap<cms_8120::DeviceList>::DoExecute()
{
    TPLOG_INFO("cms_8120::DeviceList::DoExecute...");

    cms_8120::DeviceList & r_msg = GetMsg();
    GetRecGuardPtr()->HandleStorageInfo(r_msg);
    //CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);

    return NULL;
}