#pragma once
#include "center/Records.pb.h"
#include "server/RspServerData.pb.h"
#include "center/StorageConfig.pb.h"
#include "verify/RspServerLogin.pb.h"

#include "UploadFactory.h"
#include "DBDataMgr.h"

//��¼���ط���������Ϣ
template<> inline WrapIf* PBWrap<cms_8120::RspServerLogin>::DoExecute()
{
    ULSERVER_LOG_INFO("cms_8100::RspServerLogin::DoExecute...");

    cms_8120::RspServerLogin & r_msg = GetMsg();
    CDBDataMgr::GetSingletonPtr()->AddLoginRsp(r_msg);

    return NULL;
}

//�ɼ�ƽ̨��Ϣ����
template<> inline WrapIf* PBWrap<cms_8120::RspCollectconfig>::DoExecute()
{
    ULSERVER_LOG_INFO("cms_8120::RspCollectconfig::DoExecute...");

    cms_8120::RspCollectconfig & r_msg = GetMsg();
    //CDBDataMgr::GetSingletonPtr()->UpdateCollectInfo(r_msg);

    return NULL;
}

//¼����Ϣ����
template<> inline WrapIf* PBWrap<cms_8120::RecordsList>::DoExecute()
{
    ULSERVER_LOG_INFO("cms_8120::RecordsList::DoExecute...");

    cms_8120::RecordsList & r_msg = GetMsg();

    if (cms_8120::CMD_DT_RECORDS_LIST == r_msg.cmd())
    {
        CUploadFactory::GetSingletonPtr()->AddRecordsList(r_msg,TRUE);
    }
	else if (cms_8120::CMD_DT_RECORDS_LIST_EX == r_msg.cmd())
	{
		CUploadFactory::GetSingletonPtr()->DeleteLocalRecordFiles(r_msg);
	}
    
    ULSERVER_LOG_INFO("%s...out", __FUNCTION__);

    return NULL;
}

//�洢�豸��Ϣ����
template<> inline WrapIf* PBWrap<cms_8120::RspStorageConfig>::DoExecute()
{
    ULSERVER_LOG_INFO("cms_8120::RecordsList::DoExecute...");

    cms_8120::RspStorageConfig & r_msg = GetMsg();

    if (cms_8120::CMD_DT_STORAGE_INFO == r_msg.cmd())
    {
        CUploadFactory::GetSingletonPtr()->HandleStorage(r_msg,TRUE);
    }

    ULSERVER_LOG_INFO("%s...out", __FUNCTION__);

    return NULL;
}

//�û���Ϣ����
// template<> inline WrapIf* PBWrap<cms_8120::UsersList>::DoExecute()
// {
//     ULSERVER_LOG_INFO("cms_8120::RspCollectconfig::DoExecute...");
// 
//     cms_8120::UsersList & r_msg = GetMsg();
//     CUploadFactory::GetSingletonPtr()->HandleUserNo(r_msg);
// 
//     return NULL;
// }