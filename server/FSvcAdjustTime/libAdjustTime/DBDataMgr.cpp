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
*	@brief	��ʼ������
*	@param  [in] 
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::Init()
{
    TPLOG_INFO("CDBDataMgr::Init.........In");
    if (m_bInit)
    {
        //�Ѿ���ʼ����
        TPLOG_ERROR("CDBDataMgr::Init already");
        return ADJUST_ERROR_SUCC;
    }

    //�ȴ���½�����¼�
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
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::Fini()
{
    if (!m_bInit)
    {
        //�Ѿ��ͷ�
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
*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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

    //��������ñ������ã���ִ�е�¼����
    if (!GetConfigPtr()->m_bLocalConfig)
    {
        if (LOGIN_RESULT_OK != Login())
        {
            TPLOG_ERROR("Login center fail");
			Msger_NotifyMsg(ALL_USER, _T("��¼�������ķ���ʧ��"));
            return ADJUST_ERROR_FAIL;
        }
        else
        {
            //���·���������
             GetConfigPtr()->LoadWebConfig(m_ServerLoginRsp);
			 Msger_NotifyMsg(ALL_USER, _T("��¼�������ķ���ɹ�"));
        }
    }

	if (GetConfigPtr()->m_nATServerPort<1024 
		|| GetConfigPtr()->m_nATServerPort>65535)
	{
		TPLOG_ERROR("ATServer control_port is invalid!!!");
		Msger_NotifyMsg(ALL_USER,_T("Ѳ�������ƶ˿ںŲ��Ϸ�"));
		return ADJUST_ERROR_FAIL;
	}

	//����ϵͳѲ��������
	if(HPR_OK != GetClientSessionMgrPtr()->StartListen(GetConfigPtr()->m_nATServerPort))
	{
		TPLOG_ERROR("Start Server Listen failure");
		Msger_NotifyMsg(ALL_USER,_T("��������ʧ��"));
		return ADJUST_ERROR_FAIL;
	}

    //����ϵͳѲ�����
    if (GetClientSessionMgrPtr()->Connect(GetConfigPtr()->m_strATServerIP, GetConfigPtr()->m_nATServerPort))
	{
		TPLOG_ERROR("connect ATServer fail, ip:%s,port:%d", GetConfigPtr()->m_strATServerIP.c_str(), GetConfigPtr()->m_nATServerPort);
		return ADJUST_ERROR_FAIL;
	}

    m_bStart = true;
	return ADJUST_ERROR_SUCC;

}

/**	@fn	    Stop
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::Stop()
{
    TPLOG_INFO("CDBDataMgr::Stop in..................");
    if (!m_bInit || !m_bStart)
    {
        TPLOG_ERROR("CDBDataMgr has no Init or Start");
        return ADJUST_ERROR_FAIL;
    }
	//���͵ǳ�������������
    Logout();

    //�Ͽ���������������
    GetClientSessionMgrPtr()->Disconnect(GetConfigPtr()->m_strCenterIP,
        GetConfigPtr()->m_nCenterPort);

	//�Ͽ���Ѳ���������
	GetClientSessionMgrPtr()->Disconnect(GetConfigPtr()->m_strATServerIP,
		GetConfigPtr()->m_nATServerPort);

    //ֹͣѲ��������
	GetClientSessionMgrPtr()->StopListen();

    m_bStart = false;
    TPLOG_INFO("CDBDataMgr Stop out................");
    return ADJUST_ERROR_SUCC;
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] ��
*	@return	ENUM_LOGIN_RESULT��LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
*/
ENUM_LOGIN_RESULT CDBDataMgr::Login()
{
	TPLOG_INFO("Login in");

	if (GetConfigPtr()->m_strCenterIP.empty() 
		|| GetConfigPtr()->m_nCenterPort<1024 
		|| GetConfigPtr()->m_nCenterPort>65535)
	{
		TPLOG_ERROR("Login fail.Param is invalid!!!");
		Msger_NotifyMsg(ALL_USER,_T("��������IP��˿ںŲ��Ϸ�"));
		return LOGIN_RESULT_FAIL;
	}

	//�ж�CMS�Ƿ���������
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
		Msger_NotifyMsg(ALL_USER,_T("δ������������"));
		return LOGIN_RESULT_FAIL;
	}

	//���͵�½����
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
		Msger_NotifyMsg(ALL_USER,_T("��½�������ĳ�ʱ"));
	}

	return loginResult;
}

/**	@fn	    Logout
*	@brief	�ǳ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::Logout()
{
	TPLOG_INFO("Logout...in");

	//�����˳���Ϣ
	SendLogoutReq();

	TPLOG_INFO("Logout...out");

	return ADJUST_ERROR_SUCC;
}

/**	@fn	    SendLoginReq
*	@brief	���͵�¼��Ϣ
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::SendLoginReq()
{
	//������¼��Ϣ
	cms_8120::ReqServerLogin msgLogin;
	msgLogin.set_cmd(cms_8120::CMD_REQ_SERVER_LOGIN);
	msgLogin.set_moudle_type(cms_8120::MT_SERVER_ADJUSTTIME);//����������
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
*	@brief	�����˳���Ϣ
*	@param  [in]��
*	@return	
*/
void CDBDataMgr::SendLogoutReq()
{
	//������Ϣ����
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
*	@brief	��õ�½��Ӧ��Ϣ
*	@param  [in] serLoginRsp ��½��Ӧ��Ϣ
*	@return	
*/
void CDBDataMgr::SendLoginRsp(const cms_8120::RspServerLogin & serLoginRsp)
{
	m_ServerLoginRsp = serLoginRsp;
	::SetEvent(m_hLoginRetEvent);
}

/**	@fn	    GetAllCollectDeviceReq
*	@brief	�����ȡ�ɼ��豸��Ϣ
*	@param  [in]  ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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
*	@brief	�����ȡ�洢�豸��Ϣ
*	@param  [in]  ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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
*	@brief	�����ñ�������״̬
*	@param  [in] workstate -- workstate��Ϣ
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
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
 *  @brief  ���ı�����Ϣ����ӱ���
 *  @param  [in]workstate, workstate��Ϣ
 *  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
 *  @return int,0��ʾ�޴���
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
 *  @brief  ���ı�����Ϣ���±���
 *  @param  [in]workstate, workstate��Ϣ
 *  @param  [in]nID ������־��ˮ��
 *  @return int 0��ʾ�޴���
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
*  @brief  ����Ѳ���������
*  @return int 0��ʾ�޴���
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
*  @brief  �����豸����״̬
*  @param  [in]Device_Type �豸����
*  @param  [in]strIP       �豸IP��ַ
*  @param  [in]nPort       �豸�˿ں�
*  @param  [in]iOnline,    0������ 1����
*  @return int 0��ʾ�޴���
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
*  @brief  ���´洢�豸����
*  @param  [in]Storage_Type �洢�豸����
*  @param  [in]strIP        �洢�豸IP��ַ
*  @param  [in]nPort        �洢�豸�˿ں�
*  @param  [in]iTotalSpace, ������
*  @param  [in]iFreeSpace,  ʣ������
*  @return int 0��ʾ�޴���
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


