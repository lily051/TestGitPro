#include "StdAfx.h"
#include "DBDataMgr.h"
#include "ConnSessionMgr.h"
#include "../util/StringConverter.h"
#include "../util/fclib/fcTchar.h"
#include "Config.h"
#include "Msger.h"

#include "center/OvertimeRecord.pb.h"
#include "center/Sysconfig.pb.h"
#include "center/Prosaverecord.pb.h"
#include "center/Device.pb.h"
#include "center/Server.pb.h"
#include "server/ReqServerData.pb.h"
#include "verify/ReqServerLogin.pb.h"
#include "verify/MsgServerLogout.pb.h"

SINGLETON_IMPLEMENT(CDBDataMgr)//<�������ʵ��

CDBDataMgr::CDBDataMgr(void)
{
    m_bInit = false;
    m_bStart = false;
    m_bStopWorkerThread = false;
    m_hLoginRetEvent = NULL;
}

CDBDataMgr::~CDBDataMgr(void)
{
    try
    {
        //Fini();

//         if (NULL != m_hLoginRetEvent)
//         {
//             CloseHandle(m_hLoginRetEvent);
//             m_hLoginRetEvent = NULL;
//         }
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
        return HPR_OK;
    }

    //�ȴ���½�����¼�
    m_hLoginRetEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
    m_bInit = true;
    TPLOG_INFO("CDBDataMgr::Init.........Out");
    return HPR_OK;
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
        return HPR_OK;
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
    return HPR_OK;
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
        return HPR_ERROR;
    }

    if (m_bStart)
    {
        return HPR_OK;
    }

    CConnSessionMgr::GetSingletonPtr()->Connect(CConfig::GetSingletonPtr()->m_strCenterIP,CConfig::GetSingletonPtr()->m_nCenterPort);

    if (LOGIN_RESULT_OK != Login())
    {
        TPLOG_ERROR("CCmsClient Login fail");
        Msger_NotifyMsg(ALL_USER, _T("��¼�������ķ���ʧ��"));
        return HPR_ERROR;
    }
    else
    {
        //���·���������
        CConfig::GetSingletonPtr()->LoadWebConfigXML(m_ServerLoginRsp.config_info());
        Msger_NotifyMsg(ALL_USER, _T("��¼�������ķ���ɹ�"));
    }

    //���Ӳɼ�DC
    //CConnSessionMgr::GetSingletonPtr()->Connect(CConfig::GetSingletonPtr()->m_strCollectIP,CConfig::GetSingletonPtr()->m_nCollectPort);
    //����ϵͳѲ�����
    CConnSessionMgr::GetSingletonPtr()->Connect(CConfig::GetSingletonPtr()->m_strATServerIP, CConfig::GetSingletonPtr()->m_nATServerPort);
    m_bStart = true;
    return HPR_OK;
}

/**	@fn	    Stop
*	@brief	ֹͣ����
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::Stop()
{
    TPLOG_INFO("CCmsClient::Stop in..................");
    if (!m_bInit || !m_bStart)
    {
        TPLOG_ERROR("CCmsClient has no Init");
        return HPR_ERROR;
    }

    m_bStopWorkerThread = true;

    if ("0.0.0.0" != CConfig::GetSingletonPtr()->m_strATServerIP)
    {
        CConnSessionMgr::GetSingletonPtr()->Disconnect(CConfig::GetSingletonPtr()->m_strATServerIP,
            CConfig::GetSingletonPtr()->m_nATServerPort);
        CConfig::GetSingletonPtr()->m_strATServerIP = "0.0.0.0";
    }

    Logout();

    //�Ͽ�����
    CConnSessionMgr::GetSingletonPtr()->Disconnect(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort);
    m_bStart = false;
    TPLOG_INFO("CCmsClient Stop out................");
    return HPR_OK;
}

/**	@fn	    GetSysConfig
*	@brief	��ȡ����ϵͳ��Ϣ
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::GetSysConfig()
{
    cms_8120::ReqServerData r_msg;
    r_msg.set_cmd(cms_8120::CMD_DT_SYSCONFIG_LIST);
    r_msg.CheckInitialized();

    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP, 
        CConfig::GetSingletonPtr()->m_nCenterPort, cms_8120::CMD_DT_SYSCONFIG_LIST, &r_msg);
}

/**	@fn	    GetStorageInfo
*	@brief	��ȡ�洢�豸����
*	@param  [in] strStorageID -- �洢�豸ID
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::GetStorageInfo(const HPR_INT32 &storageID)
{
    cms_8120::ReqDevice r_msg;
    r_msg.set_cmd(cms_8120::CMD_DT_STORAGEDEVICE_INFO);
    //r_msg.set_ntype(50001); //50001��ʾ�洢�豸����
    r_msg.set_id(storageID);
    r_msg.CheckInitialized();

    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP, 
        CConfig::GetSingletonPtr()->m_nCenterPort, cms_8120::CMD_DT_STORAGEDEVICE_INFO, &r_msg);
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] ��
*	@return	ENUM_LOGIN_RESULT��LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
*/
ENUM_LOGIN_RESULT CDBDataMgr::Login()
{
    TPLOG_INFO("CCmsClient::Login into");

    if (CConfig::GetSingletonPtr()->m_strCenterIP.empty() 
        || CConfig::GetSingletonPtr()->m_nCenterPort<1024 
        || CConfig::GetSingletonPtr()->m_nCenterPort>65535)
    {
        TPLOG_ERROR("Login fail.Param is invalid!!!");
        return LOGIN_RESULT_FAIL;
    }

    //�ж�CMS�Ƿ���������
    bool bCmsIsConnect = false;
    for (int i=0;i<10;++i)
    {
        if (CConnSessionMgr::GetSingletonPtr()->IsConnect(CConfig::GetSingletonPtr()->m_strCenterIP, 
            CConfig::GetSingletonPtr()->m_nCenterPort))
        {
            bCmsIsConnect = true;
            break;
        }
        Sleep(1000);
    }

    if (!bCmsIsConnect)
    {
        TPLOG_ERROR("CMService %s:%d not connect!",CConfig::GetSingletonPtr()->m_strCenterIP.c_str(),
            CConfig::GetSingletonPtr()->m_nCenterPort);
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
    }

    return loginResult;
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
    msgLogin.set_moudle_type(cms_8120::MT_SERVER_MEDIAGUARD);//����������
//     msgLogin.set_login_name(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_sLoginName));
//     msgLogin.set_login_pwd(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_sLoginPwd));
    msgLogin.set_ip(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_strLocalIP));
    std::string sMac;
    msgLogin.set_mac(CSCT::Ansi2Utf8(sMac));
    msgLogin.set_server_version(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_sFileVersion));
    msgLogin.set_dc_ip(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_strCenterIP));
    msgLogin.CheckInitialized();

    return CConnSessionMgr::GetSingletonPtr()->SendMsg(
        CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,
        cms_8120::CMD_REQ_SERVER_LOGIN,&msgLogin);
}

/**	@fn	    SendHeartBeat
*	@brief	������������
*	@param  [in]��
*	@return	
*/
void CDBDataMgr::SendHeartBeat()
{
    //������������
    cms_8120::ReqServer msg;
    msg.set_cmd(cms_8120::CMD_MDT_SERVER_STATUS);
    msg.set_strip(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_strLocalIP));
    msg.set_ntype(cms_8120::MT_SERVER_MEDIAGUARD);//����������
    msg.CheckInitialized();

    CConnSessionMgr::GetSingletonPtr()->SendMsg(
        CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,
        cms_8120::CMD_MDT_SERVER_STATUS,&msg);
}

/**	@fn	    AddLoginRsp
*	@brief	��ӵ�½��Ӧ��Ϣ
*	@param  [in] serLoginRsp ��½��Ӧ��Ϣ
*	@return	
*/
void CDBDataMgr::AddLoginRsp(const cms_8120::RspServerLogin & serLoginRsp)
{
    m_ServerLoginRsp = serLoginRsp;
    ::SetEvent(m_hLoginRetEvent);
}

/**	@fn	    SendLogoutMsg
*	@brief	�����˳���Ϣ
*	@param  [in]��
*	@return	
*/
void CDBDataMgr::SendLogoutMsg()
{
    //������Ϣ����
    cms_8120::MsgServerLogout r_msg;
    r_msg.set_cmd(cms_8120::CMD_MSG_SERVER_LOGOUT);
    r_msg.set_moudle_type(cms_8120::MT_SERVER_MEDIAGUARD);
    //r_msg.set_login_name(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_sLoginName));
    r_msg.set_ip(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_strLocalIP));
    r_msg.set_server_version(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_sFileVersion));
    r_msg.set_dc_ip(CSCT::Ansi2Utf8(CConfig::GetSingletonPtr()->m_strCenterIP));
    r_msg.CheckInitialized();

    CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_MSG_SERVER_LOGOUT,&r_msg);
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
    SendLogoutMsg();

    TPLOG_INFO("Logout...out");

    return HPR_OK;
}

/**	@fn	    GetOldBzRecord
*	@brief	��ȡ����¼����Ϣ
*	@param  [in] ��
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::GetOldBzRecord()
{
    cms_8120::ReqOvertimeRecord r_msg;
    r_msg.set_cmd(cms_8120::CMD_DT_OVERTIMERECORD_LIST);
    r_msg.CheckInitialized();
    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_DT_OVERTIMERECORD_LIST,&r_msg);
}

/**	@fn	    GetRecRecord
*	@brief	��ȡ¼����Ϣ
*	@param  [in] nClientId -- ClientID
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::GetRecRecord(const int &nClientId)
{
    cms_8120::ReqProsaverecord r_msg;
    r_msg.set_cmd(cms_8120::CMD_DT_RECORD_LIST);
    r_msg.set_clientid(nClientId);
    r_msg.CheckInitialized();
    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_DT_RECORD_LIST,&r_msg);
}

/**	@fn	    MarkOldBzRecords
*	@brief	���clientinput���еĹ��ڼ�¼
*	@param  [in] nClientID
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::MarkOldBzRecords(const int &nClientID)
{
    cms_8120::ReqProsaverecord r_msg;
    r_msg.set_cmd(cms_8120::CMD_MDT_OVERTIMERECORD);
    r_msg.set_clientid(nClientID);
    r_msg.CheckInitialized();
    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_MDT_OVERTIMERECORD,&r_msg);
}

/**	@fn	    DelClientRecords
*	@brief	ɾ��¼���¼
*	@param  [in] nClientId -- ClientID
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::DelClientRecords(const std::string &strClientIds)
{
    cms_8120::ReqServerData r_msg;
    r_msg.set_cmd(cms_8120::CMD_DDT_OVERTIMELOG);
    r_msg.set_id(CSCT::Ansi2Utf8(strClientIds));
    r_msg.CheckInitialized();
    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_DDT_OVERTIMELOG,&r_msg);
}

/**	@fn	    DelCenterRecords
*	@brief	ɾ��prosaverecord���е�¼���¼
*	@param  [in] nRecordID -- ����¼��ID
*	@param  [out] ��
*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
*/
int CDBDataMgr::DelCenterRecords(const int &nRecordID)
{
    cms_8120::ReqProsaverecord r_msg;
    r_msg.set_cmd(cms_8120::CMD_DDT_RECORD_INFO);
    r_msg.set_id(nRecordID);
    r_msg.CheckInitialized();
    return CConnSessionMgr::GetSingletonPtr()->SendMsg(CConfig::GetSingletonPtr()->m_strCenterIP,
        CConfig::GetSingletonPtr()->m_nCenterPort,cms_8120::CMD_DDT_RECORD_INFO,&r_msg);
}


