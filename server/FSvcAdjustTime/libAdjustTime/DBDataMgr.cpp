#include "StdAfx.h"
#include "../util/StringConverter.h"
#include "DBDataMgr.h"
#include "ClientSessionMgr.h"
#include "Config.h"
#include <SvcGeneralDef.h>
#include "Msger.h"

#include "Center/Alarmlog.pb.h"
#include "Center/Device.pb.h"
#include "Center/Server.pb.h"

#include "Server/ReqServerData.pb.h"
#include "verify/ReqServerLogin.pb.h"
#include "verify/MsgServerLogout.pb.h"


CDBDataMgr::CDBDataMgr(void)
:m_bInit(false)
,m_bStart(false)
,m_hLoginRetEvent(NULL)
{
}

CDBDataMgr::~CDBDataMgr(void)
{
    try
    {
        Fini();

        if (NULL != m_hLoginRetEvent)
        {
            CloseHandle(m_hLoginRetEvent);
            m_hLoginRetEvent = NULL;
        }
    }
    catch (...)
    {	
    }
}

/**	@fn	    Init
*	@brief	初始化函数
*	@param  [in] 
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::Init()
{
    TPLOG_INFO("CDBDataMgr::Init.........In");
    if (m_bInit)
    {
        //已经初始过了
        TPLOG_ERROR("CDBDataMgr::Init already");
        return ADJUST_ERROR_SUCC;
    }

    //等待登陆返回事件
	m_hLoginRetEvent = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	if (NULL == m_hLoginRetEvent)
	{
		return ADJUST_ERROR_FAIL;
	}
    m_bInit = true;
    TPLOG_INFO("CDBDataMgr::Init.........Out");
    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Fini
*	@brief	反初始化函数，负责资源释放
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::Fini()
{
    if (!m_bInit)
    {
        //已经释放
        TPLOG_ERROR("CDBDataMgr::Fini already");
        return ADJUST_ERROR_SUCC;
    }
    TPLOG_INFO("CDBDataMgr::Fini...in");

    Stop();

    if (NULL != m_hLoginRetEvent)
    {
        CloseHandle(m_hLoginRetEvent);
        m_hLoginRetEvent = NULL;
    }

    TPLOG_INFO("CDBDataMgr::Fini out................");
    m_bInit = false;
    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Start
*	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
*          此函数应该尽快返回，不应该占用太多时间，必须小于30s
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::Start()
{
    if (!m_bInit)
    {
        return ADJUST_ERROR_FAIL;
    }

    if (m_bStart)
    {
		TPLOG_INFO("CDBDataMgr Already Start");
        return ADJUST_ERROR_SUCC;
    }

    if (GetClientSessionMgrPtr()->Connect(GetConfigPtr()->m_strCenterIP,GetConfigPtr()->m_nCenterPort))
    {
	    TPLOG_ERROR("connect center fail, ip:%s,port:%d", GetConfigPtr()->m_strCenterIP.c_str(), GetConfigPtr()->m_nCenterPort);
	    return ADJUST_ERROR_FAIL;
    }

    //如果不启用本地配置，则执行登录操作
    if (!GetConfigPtr()->m_bLocalConfig)
    {
        if (LOGIN_RESULT_OK != Login())
        {
            TPLOG_ERROR("Login center fail");
			Msger_NotifyMsg(ALL_USER, _T("登录数据中心服务失败"));
            return ADJUST_ERROR_FAIL;
        }
        else
        {
            //更新服务器参数
             GetConfigPtr()->LoadWebConfig(m_ServerLoginRsp);
			 Msger_NotifyMsg(ALL_USER, _T("登录数据中心服务成功"));
        }
    }

	if (GetConfigPtr()->m_nATServerPort<1024 
		|| GetConfigPtr()->m_nATServerPort>65535)
	{
		TPLOG_ERROR("ATServer control_port is invalid!!!");
		Msger_NotifyMsg(ALL_USER,_T("巡检服务控制端口号不合法"));
		return ADJUST_ERROR_FAIL;
	}

	//启动系统巡检服务监听
	if(HPR_OK != GetClientSessionMgrPtr()->StartListen(GetConfigPtr()->m_nATServerPort))
	{
		TPLOG_ERROR("Start Server Listen failure");
		Msger_NotifyMsg(ALL_USER,_T("启动监听失败"));
		return ADJUST_ERROR_FAIL;
	}

    //连接系统巡检服务
    if (GetClientSessionMgrPtr()->Connect(GetConfigPtr()->m_strATServerIP, GetConfigPtr()->m_nATServerPort))
	{
		TPLOG_ERROR("connect ATServer fail, ip:%s,port:%d", GetConfigPtr()->m_strATServerIP.c_str(), GetConfigPtr()->m_nATServerPort);
		return ADJUST_ERROR_FAIL;
	}

    m_bStart = true;
	return ADJUST_ERROR_SUCC;

}

/**	@fn	    Stop
*	@brief	停止函数
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::Stop()
{
    TPLOG_INFO("CDBDataMgr::Stop in..................");
    if (!m_bInit || !m_bStart)
    {
        TPLOG_ERROR("CDBDataMgr has no Init or Start");
        return ADJUST_ERROR_FAIL;
    }
	//发送登出请求到数据中心
    Logout();

    //断开与数据中心连接
    GetClientSessionMgrPtr()->Disconnect(GetConfigPtr()->m_strCenterIP,
        GetConfigPtr()->m_nCenterPort);

	//断开与巡检服务连接
	GetClientSessionMgrPtr()->Disconnect(GetConfigPtr()->m_strATServerIP,
		GetConfigPtr()->m_nATServerPort);

    //停止巡检服务监听
	GetClientSessionMgrPtr()->StopListen();

    m_bStart = false;
    TPLOG_INFO("CDBDataMgr Stop out................");
    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Login
*	@brief	登录
*	@param  [in] 无
*	@return	ENUM_LOGIN_RESULT，LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
*/
ENUM_LOGIN_RESULT CDBDataMgr::Login()
{
	TPLOG_INFO("Login in");

	if (GetConfigPtr()->m_strCenterIP.empty() 
		|| GetConfigPtr()->m_nCenterPort<1024 
		|| GetConfigPtr()->m_nCenterPort>65535)
	{
		TPLOG_ERROR("Login fail.Param is invalid!!!");
		Msger_NotifyMsg(ALL_USER,_T("数据中心IP或端口号不合法"));
		return LOGIN_RESULT_FAIL;
	}

	//判断CMS是否连接上了
	bool bCmsIsConnect = false;
	for (int i=0;i<10;++i)
	{
		if (GetClientSessionMgrPtr()->IsConnect(GetConfigPtr()->m_strCenterIP, 
			GetConfigPtr()->m_nCenterPort))
		{
			bCmsIsConnect = true;
			break;
		}
		Sleep(1000);
	}

	if (!bCmsIsConnect)
	{
		TPLOG_ERROR("CMService %s:%d not connect!",GetConfigPtr()->m_strCenterIP.c_str(),
			GetConfigPtr()->m_nCenterPort);
		Msger_NotifyMsg(ALL_USER,_T("未连接数据中心"));
		return LOGIN_RESULT_FAIL;
	}

	//发送登陆请求
	if (HPR_OK != SendLoginReq())
	{
		TPLOG_ERROR("SendLoginReq Fail........!!!!!!");
		return LOGIN_RESULT_FAIL;
	}

	ENUM_LOGIN_RESULT loginResult = LOGIN_RESULT_FAIL;
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hLoginRetEvent, 1000*10))
	{
		if (LOGIN_RESULT_OK == m_ServerLoginRsp.result())
		{
			loginResult = LOGIN_RESULT_OK;
			TPLOG_INFO("Login Succ!!!");
		} 
		else
		{
			TPLOG_ERROR("Login Result %s", CSCT::Utf82Ansi(m_ServerLoginRsp.result_info()).c_str());
		}
	} 
	else
	{
		TPLOG_ERROR("Wait Login Time Out!!!Login Fail");
		Msger_NotifyMsg(ALL_USER,_T("登陆数据中心超时"));
	}

	return loginResult;
}

/**	@fn	    Logout
*	@brief	登出
*	@param  [in] 无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::Logout()
{
	TPLOG_INFO("Logout...in");

	//发送退出消息
	SendLogoutReq();

	TPLOG_INFO("Logout...out");

	return ADJUST_ERROR_SUCC;
}

/**	@fn	    SendLoginReq
*	@brief	发送登录消息
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::SendLoginReq()
{
	//构建登录消息
	cms_8120::ReqServerLogin msgLogin;
	msgLogin.set_cmd(cms_8120::CMD_REQ_SERVER_LOGIN);
	msgLogin.set_moudle_type(cms_8120::MT_SERVER_ADJUSTTIME);//服务器类型
	msgLogin.set_ip(CSCT::Ansi2Utf8(GetConfigPtr()->m_strLocalIP));
	std::string sMac;
	msgLogin.set_mac(CSCT::Ansi2Utf8(sMac));
	msgLogin.set_server_version(CSCT::Ansi2Utf8(GetConfigPtr()->m_sFileVersion));
	msgLogin.set_dc_ip(CSCT::Ansi2Utf8(GetConfigPtr()->m_strCenterIP));
	msgLogin.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(
		GetConfigPtr()->m_strCenterIP,
		GetConfigPtr()->m_nCenterPort,
		cms_8120::CMD_REQ_SERVER_LOGIN,&msgLogin);
}

/**	@fn	    SendLogoutReq
*	@brief	发送退出消息
*	@param  [in]无
*	@return	
*/
void CDBDataMgr::SendLogoutReq()
{
	//设置消息内容
	cms_8120::MsgServerLogout r_msg;
	r_msg.set_cmd(cms_8120::CMD_MSG_SERVER_LOGOUT);
	r_msg.set_moudle_type(cms_8120::MT_SERVER_ADJUSTTIME);
	r_msg.set_ip(CSCT::Ansi2Utf8(GetConfigPtr()->m_strLocalIP));
	r_msg.set_server_version(CSCT::Ansi2Utf8(GetConfigPtr()->m_sFileVersion));
	r_msg.set_dc_ip(CSCT::Ansi2Utf8(GetConfigPtr()->m_strCenterIP));
	r_msg.CheckInitialized();

	GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP,
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_MSG_SERVER_LOGOUT,&r_msg);
}

/**	@fn	    SendLoginRsp
*	@brief	获得登陆响应消息
*	@param  [in] serLoginRsp 登陆响应信息
*	@return	
*/
void CDBDataMgr::SendLoginRsp(const cms_8120::RspServerLogin & serLoginRsp)
{
	m_ServerLoginRsp = serLoginRsp;
	::SetEvent(m_hLoginRetEvent);
}

/**	@fn	    GetAllCollectDeviceReq
*	@brief	请求获取采集设备信息
*	@param  [in]  无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::GetAllCollectDeviceReq()
{
	cms_8120::ReqDevice r_msg;
	r_msg.set_cmd(cms_8120::CMD_DT_DVRDEVICE_LIST);
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_DT_DVRDEVICE_LIST, &r_msg);
}

/**	@fn	    GetAllStorageDeviceReq
*	@brief	请求获取存储设备信息
*	@param  [in]  无
*	@param  [out] 无
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::GetAllStorageDeviceReq()
{
    cms_8120::ReqDevice r_msg;
    r_msg.set_cmd(cms_8120::CMD_DT_STORAGEDEVICE_LIST);
    r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_DT_STORAGEDEVICE_LIST, &r_msg);
}

/**	@fn	    GetAlarmStatus
*	@brief	请求获得报警处理状态
*	@param  [in] workstate -- workstate信息
*	@return	int，0 表示没有错误，其他数值表示出现了错误
*/
int CDBDataMgr::GetAlarmLogReq(const work_state_t &workstate)
{
	cms_8120::AlarmlogInfo r_msg;
	r_msg.set_cmd(cms_8120::CMD_DT_ALARMLOG);
	cms_8120::Alarmlog* p_msgAlarmLogInfo = r_msg.mutable_alarmlog();
	p_msgAlarmLogInfo->set_alarmtype(workstate.nKey);
	p_msgAlarmLogInfo->set_checkchan(workstate.nChannel);
	p_msgAlarmLogInfo->set_strhost(CSCT::Tstring2Utf8(workstate.strHost));
	p_msgAlarmLogInfo->set_comments(CSCT::Tstring2Utf8(workstate.strInfo));
    r_msg.CheckInitialized();
	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_DT_ALARMLOG, &r_msg);

}

/** @fn     AddCenterAlarmReq
 *  @brief  中心报警信息表添加报警
 *  @param  [in]workstate, workstate信息
 *  @param  [in]ulCheckType, 100 dvr,102 采集器服务，200中心服务
 *  @return int,0表示无错误
 */
int CDBDataMgr::AddCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType)
{
	cms_8120::AlarmlogInfo r_msg;
	r_msg.set_cmd(cms_8120::CMD_WDT_ALARMLOG_INFO);
	cms_8120::Alarmlog* p_msgAlarmLogInfo = r_msg.mutable_alarmlog();
	p_msgAlarmLogInfo->set_checktype(ulCheckType);
	p_msgAlarmLogInfo->set_checkobjid(0);
	p_msgAlarmLogInfo->set_checkchan(workstate.nChannel);
	p_msgAlarmLogInfo->set_contactobjid(0);
	p_msgAlarmLogInfo->set_alarmtype(workstate.nKey);
	p_msgAlarmLogInfo->set_alarmtime(CSCT::Tstring2Utf8(workstate.strOccureTime));
	p_msgAlarmLogInfo->set_alarmbegintime(CSCT::Tstring2Utf8(workstate.strOccureTime));
	p_msgAlarmLogInfo->set_alarmregionid(workstate.nRegionID);
    p_msgAlarmLogInfo->set_comments(CSCT::Tstring2Utf8(workstate.strInfo));
	p_msgAlarmLogInfo->set_strhost(CSCT::Tstring2Utf8(workstate.strHost));
	p_msgAlarmLogInfo->set_status(0);
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_WDT_ALARMLOG_INFO, &r_msg);
}

/** @fn     UpdateCenterAlarmReq
 *  @brief  中心报警信息更新报警
 *  @param  [in]workstate, workstate信息
 *  @param  [in]nID 报警日志流水号
 *  @return int 0表示无错误
 */
int CDBDataMgr::UpdateCenterAlarmReq(const work_state_t &workstate, int nID)
{
	cms_8120::AlarmlogInfo r_msg;
	r_msg.set_cmd(cms_8120::CMD_MDT_ALARMTIME);
	cms_8120::Alarmlog* p_msgAlarmLogInfo = r_msg.mutable_alarmlog();
	p_msgAlarmLogInfo->set_id(nID);
	p_msgAlarmLogInfo->set_alarmtime(CSCT::Tstring2Utf8(workstate.strOccureTime));
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_MDT_ALARMTIME, &r_msg);
}

/** @fn    UpdateServerHeartBeartReq
*  @brief  更新巡检服务心跳
*  @return int 0表示无错误
*/
int CDBDataMgr::UpdateServerHeartBeartReq()
{
	cms_8120::ReqServer r_msg;
	r_msg.set_cmd(cms_8120::CMD_MDT_SERVER_STATUS);
	r_msg.set_ntype(cms_8120::MT_SERVER_ADJUSTTIME);
	r_msg.set_strip(CSCT::Ansi2Utf8(GetConfigPtr()->m_strLocalIP));
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_MDT_SERVER_STATUS, &r_msg);
}

/** @fn    UpdateDeviceOnlineStateReq
*  @brief  更新设备在线状态
*  @param  [in]Device_Type 设备类型
*  @param  [in]strIP       设备IP地址
*  @param  [in]nPort       设备端口号
*  @param  [in]iOnline,    0不在线 1在线
*  @return int 0表示无错误
*/
int CDBDataMgr::UpdateDeviceOnlineStateReq(int Device_Type, const std::string &strIP, int nPort, int iOnline)
{
	cms_8120::ReqDevice r_msg;
	r_msg.set_cmd(cms_8120::CMD_MDT_DEVIDE_STATUS);
	r_msg.set_ntype(Device_Type);
	r_msg.set_strip(CSCT::Ansi2Utf8(strIP));
	r_msg.set_nport(nPort);
	r_msg.set_online(iOnline);
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_MDT_DEVIDE_STATUS, &r_msg);
}

/** @fn     UpdateStorageCapacityReq
*  @brief  更新存储设备容量
*  @param  [in]Storage_Type 存储设备类型
*  @param  [in]strIP        存储设备IP地址
*  @param  [in]nPort        存储设备端口号
*  @param  [in]iTotalSpace, 总容量
*  @param  [in]iFreeSpace,  剩余容量
*  @return int 0表示无错误
*/
int CDBDataMgr::UpdateStorageCapacityReq(int Storage_Type, const std::string &strIP, int nPort, unsigned long long iTotalSpace, unsigned long long iFreeSpace)
{
	cms_8120::ReqDevice r_msg;
	r_msg.set_cmd(cms_8120::CMD_MDT_DEVIDE_SPACE);
	r_msg.set_ntype(Storage_Type);
	r_msg.set_strip(CSCT::Ansi2Utf8(strIP));
	r_msg.set_nport(nPort);
	r_msg.set_uitotalspace(iTotalSpace);
	r_msg.set_uifreespace(iFreeSpace);
	r_msg.CheckInitialized();

	return GetClientSessionMgrPtr()->SendMsg(GetConfigPtr()->m_strCenterIP, 
		GetConfigPtr()->m_nCenterPort,cms_8120::CMD_MDT_DEVIDE_SPACE, &r_msg);
}


