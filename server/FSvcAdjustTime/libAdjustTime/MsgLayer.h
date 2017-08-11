/**	@file    MsgLayer.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   HPP请求和响应处理文件
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#ifndef GUARD_MSG_LAYER_H
#define GUARD_MSG_LAYER_H

#include <hpp_hpp.h>
#include <AlarmMsg.pb.h>
#include <SvcGeneralDef.h>
#include "AlarmProcessor.h"
#include <fclib/fcTchar.h>
#include "DBDataMgr.h"
#include "AdjustTime.h"
#include "Center/Device.pb.h"
#include "Center/Alarmlog.pb.h"

//#include "Collect/Records.pb.h"
//#include "Collect/Users.pb.h"
//#include "Collect/Systeminfo.pb.h"
#include "Server/RspServerData.pb.h"
//#include "Center/StorageConfig.pb.h"
#include "verify/RspServerLogin.pb.h"

template<> inline WrapIf* PBWrap<AlarmMsg::AuthorizeRsp>::DoExecute()
{
    return NULL;
}

// 
template<> inline WrapIf* PBWrap<AlarmMsg::AuthorizeReq>::DoExecute()
{
    AlarmMsg::AuthorizeReq& r_req_msg = GetMsg();

    ALARM_MSG stAlarm;

    std::string strInfo = "";

    //模块
    strInfo = fcU2A((r_req_msg.alarminfo().svctype()).c_str());
    stAlarm.dwSvcType = atoi(strInfo.c_str());

    //消息类型
    strInfo = fcU2A((r_req_msg.alarminfo().dwmsgtype()).c_str());
    stAlarm.dwMsgType = atoi(strInfo.c_str());

    //服务类型
    strInfo = fcU2A((r_req_msg.alarminfo().alarmtype()).c_str());
    stAlarm.dwAlarmType = atoi(strInfo.c_str());

    //用户
    strInfo = fcU2A((r_req_msg.alarminfo().userid()).c_str());
    stAlarm.dwUserID = atoi(strInfo.c_str());

    //通道
    strInfo = fcU2A((strInfo = r_req_msg.alarminfo().channel()).c_str());
    stAlarm.dwChannel = atoi(strInfo.c_str());

    //报警状态 1报警 0 非报警
    strInfo = fcU2A((strInfo = r_req_msg.alarminfo().alarmstatus()).c_str());
    stAlarm.dwStatus = atoi(strInfo.c_str());

    //导致报警操作类型
    strInfo = fcU2A((r_req_msg.alarminfo().dwrev()).c_str());
    stAlarm.dwRev = atoi(strInfo.c_str());

    //报警信息
    strInfo = fcU2A((r_req_msg.alarminfo().alarminfo()).c_str());
    if(strInfo.size() > sizeof(stAlarm.szAlarmInfo))
    {
        TPLOG_ERROR("alarm info is too long(%s)",strInfo.c_str());
        return NULL;
    }
    memcpy(stAlarm.szAlarmInfo, strInfo.c_str(), strInfo.size());

    //报警ip地址
    strInfo = fcU2A((r_req_msg.alarminfo().alarmhost()).c_str());
    if(strInfo.size() > sizeof(stAlarm.szHost))
    {
        TPLOG_ERROR("alarm host is too long(%s)",strInfo.c_str());
        return NULL;
    }
    memcpy(stAlarm.szHost, strInfo.c_str(), strInfo.size());

    GetCAlarmProcPtr()->AddAlarmMsg(&stAlarm);
    return NULL;
}

template<> inline HPR_INT32 PBWrap<AlarmMsg::AuthorizeReq>::DoTimeout()
{
    TPLOG_ERROR("Found client authorization req timeout");
    return HPR_OK;
}

//登录返回服务器的信息
template<> inline WrapIf* PBWrap<cms_8120::RspServerLogin>::DoExecute()
{
	TPLOG_INFO("cms_8100::RspServerLogin::DoExecute...");

	cms_8120::RspServerLogin & r_msg = GetMsg();
	GetDBDataMgrPtr()->SendLoginRsp(r_msg);

	return NULL;
}

//报警状态返回
template<> inline WrapIf* PBWrap<cms_8120::AlarmlogInfo>::DoExecute()
{
	cms_8120::AlarmlogInfo & r_msg = GetMsg();

	if (cms_8120::CMD_DT_ALARMLOG == r_msg.cmd())
	{
		GetCAlarmProcPtr()->GetAlarmLogRsp(r_msg,TRUE);
	}

	return NULL;
}

//设备信息返回
template<> inline WrapIf* PBWrap<cms_8120::DeviceList>::DoExecute()
{
	cms_8120::DeviceList & r_msg = GetMsg();

	if (cms_8120::CMD_DT_DVRDEVICE_LIST == r_msg.cmd())
	{
		GetAdjustPtr()->GetAllCollectDeviceRsp(r_msg,TRUE);
	}
	else if (cms_8120::CMD_DT_STORAGEDEVICE_LIST == r_msg.cmd())
	{	
		GetAdjustPtr()->GetAllStorageDeviceRsp(r_msg,TRUE);
	}

	return NULL;
}


#endif //GUARD_MSG_LAYER_H
