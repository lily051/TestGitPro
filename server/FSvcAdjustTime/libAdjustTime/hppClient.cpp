/** @file   hppClient.cpp
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  hpp �������ͻ���
 *          
 *  @author chenxiaoyan3@hikvision.com
 *  @date   2014/12/12
 */
#include "StdAfx.h" 
#include <fclib/fcTchar.h>
#include "hlogConfig.h"
#include <SvcGeneralDef.h>
#include "MsgLayer.h"
#include "hppClient.h"

CHPPClient* CHPPClient::m_instance = NULL;

namespace {
    //��������
    static const HPR_UINT16 SERVICE_TYPE_CLIENT = 0x3738;
    static const HPR_UINT16 ALARM_HPP_SERVER_PORT = 6677;
} //~ end of anonymous namespace

/** @fn     CHPPClient::CHPPClient()
 *  @brief  Constructor
 *  @param  void
 *  @return void
 */
CHPPClient::CHPPClient()
: m_h_hpp(NULL)
, m_server_session(NULL)
, m_server_session_id(-1)
, m_strServerIp("127.0.0.1")
, m_iServerPort(ALARM_HPP_SERVER_PORT)
, m_bConn(false)
{
}

/** @fn     CHPPClient::~CHPPClient()
 *  @brief  Destructor
 *  @param  void
 *  @return void
 */
CHPPClient::~CHPPClient()
{
    if(NULL != m_h_hpp)
    {
        HPP_Close(m_h_hpp);
        m_h_hpp = NULL;
    }
}

/** @fn     CHPPDemoServer* CHPPDemoServer::Instance()
 *  @brief  Get HPP server instance
 *  @param  void
 *  @return HPP server instance
 */
CHPPClient* CHPPClient::Instance()
{
    if (NULL == m_instance)
    {
        m_instance = new (std::nothrow) CHPPClient();
    }
    return m_instance;
}

/** @fn     void CHPPDemoServer::Free()
 *  @brief  Destroy HPP server instance
 *  @param  void
 *  @return void
 */
void CHPPClient::Free()
{
    if (m_instance != NULL)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

/** @fn     CHPPClient::Init
 *  @brief  Initialize HPP server
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPClient::Init()
{
    m_h_hpp = HPP_Init(2, 30, HPR_FALSE);
    if (m_h_hpp == NULL)
    {
        TPLOG_ERROR("HPP init failed");
        return HPR_ERROR;
    }

    //HPP_SetPbMsgCreateFun(m_h_hpp, CreateMessageByCmdId);
    if(!GetLocalIp(m_strServerIp))
    {
        TPLOG_ERROR("Get Local Ip failed");
        return HPR_ERROR;
    }
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPClient::Finalize()
*  @brief  Finalize HPP  server
*  @param  void
*  @return HPR_OK
*/
HPR_INT32 CHPPClient::Finalize()
{
    if (CHPPClient::Instance()->m_server_session != NULL)
    {
        ///< Stop ConnSession
        CHPPClient::Instance()->m_server_session->StopService();
        CHPPClient::Instance()->m_server_session = NULL;
        CHPPClient::Instance()->m_server_session_id = -1;
    }

    if(NULL != m_h_hpp)
    {
        HPP_Close(m_h_hpp);
        m_h_hpp = NULL;
    }

    CHPPClient::Instance()->m_bConn = false;

    Free();
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPClient::StartUp()
*  @brief  StartUp HPP server
*  @param  void
*  @return success, HPR_OK; else, HPR_ERROR
*/
HPR_INT32 CHPPClient::StartUp()
{
    if (true == CHPPClient::Instance()->m_bConn)
    {
        return HPR_OK;
    }
    ///< Use asynchronous connect api to connect to server
    HPR_INT32 n_ret = HPP_ConnRemoteServiceNoBlock(
        m_h_hpp, m_strServerIp.c_str(), (HPR_INT16)m_iServerPort,
        OnConnectionComplete, NULL, 0, NULL, SERVICE_NORMAL_PRI, OnConnectionError);

    if (n_ret == HPR_ERROR)
    {
        TPLOG_ERROR("Connect to HPP server(%s:%d) failed",
            m_strServerIp.c_str(), m_iServerPort);
        return HPR_ERROR;
    }

    //�ȴ����ӳɹ��ŷ��أ� ����6���ж�Ϊ�쳣 
    if(!(CHPPClient::Instance()->m_bConn))
    {
		//ÿ��50���� 120��
        for(int i = 0; i < 120; i++)
        {
            Sleep(50);
            if(CHPPClient::Instance()->m_bConn)
            {
                return HPR_OK;
            }
        }
        return HPR_ERROR;
    }

    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPClient::StartUp()
*  @brief  StartUp HPP server
*  @param  [in]strServerIp, ip
*  @param  [in]iServerPort, �˿�
*  @return success, HPR_OK; else, HPR_ERROR
*/
HPR_INT32 CHPPClient::StartUp(const std::string &strServerIp, HPR_INT32 iServerPort)
{
    if (true == CHPPClient::Instance()->m_bConn)
    {
        return HPR_OK;
    }
    ///< Use asynchronous connect api to connect to server
    HPR_INT32 n_ret = HPP_ConnRemoteServiceNoBlock(
        m_h_hpp, strServerIp.c_str(), (HPR_INT16)iServerPort,
        OnConnectionComplete, NULL, 0, NULL, SERVICE_NORMAL_PRI, OnConnectionError);

    if (n_ret == HPR_ERROR)
    {
        TPLOG_ERROR("Connect to HPP server(%s:%d) failed",
            m_strServerIp.c_str(), m_iServerPort);
        return HPR_ERROR;
    }
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPClient::ShutDown()
 *  @brief  ShutDown HPP demo client
 *  @param  void
 *  @return HPR_OK
 */
HPR_INT32 CHPPClient::ShutDown()
{
    if (CHPPClient::Instance()->m_server_session != NULL)
    {
        ///< Stop ConnSession
        CHPPClient::Instance()->m_server_session->StopService();
        CHPPClient::Instance()->m_server_session = NULL;
        CHPPClient::Instance()->m_server_session_id = -1;
    }

    CHPPClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     void CHPPClient::SendMessage()
 *  @brief  ���� dMessage
*  @param  [in]msg, ��Ϣ
*  @param  [in]iMsgSize, ��Ϣ����
 *  @return void
 */
void CHPPClient::SendMessage(const ALARM_MSG &msg, int iMsgSize)
{
    WrapIf* p_wrap = FormAuthorizationWrap(msg, iMsgSize);
    if (NULL != p_wrap
        && NULL != CHPPClient::Instance()->m_server_session
        && -1 != CHPPClient::Instance()->m_server_session_id)
    {
        int n_ret = CHPPClient::Instance()->m_server_session->SendMessage(p_wrap);
        if (HPR_OK != n_ret)
        {
            PBWrapFactory<AlarmMsg::AuthorizeReq>::DestroyPBWrap(p_wrap);
            TPLOG_ERROR("HPP demo client send authorization wrap failed");
        }
    }
    return;
}

/** @fn     void CHPPClient::GetHppHandle()
*  @brief  ��ȡhpp handle
*  @return hpp handle
*/
HPP_HANDLE CHPPClient::GetHppHandle()
{
    return m_h_hpp;
}

/** @fn     void CHPPClient::OnConnectionComplete()
 *  @brief  hpp���ӳɹ� ������
 *  @param[in]  p_conn_session_ �Ự
 *  @return HPR_OK ���� ���� �쳣
 */
HPR_INT32 CHPPClient::OnConnectionComplete(ConnSession* p_conn_session_)
{
    if(NULL == p_conn_session_)
    {
        TPLOG_ERROR("session is NULL");
        return HPR_ERROR;
    }

    p_conn_session_->SetSocketClosedCallBack(OnConnectionClosed);
    p_conn_session_->SetServiceType(SERVICE_TYPE_CLIENT);    ///< client to server, 37 to 38

    ///< start ConnSession
    if (HPR_OK != p_conn_session_->StartService(SERVICE_NORMAL_PRI))
    {
        TPLOG_ERROR("Start session error");
        return HPR_ERROR;
    }

    CHPPClient::Instance()->ServerSessionId(p_conn_session_->GetSessionId());
    CHPPClient::Instance()->ServerSession(p_conn_session_);

    CHPPClient::Instance()->m_bConn = true;
    return HPR_OK;
}

/** @fn     void CHPPClient::OnConnectionClosed()
 *  @brief  hpp�Ͽ����Ӵ�����
 *  @param[in]  p_conn_session_ �Ự
 *  @return HPR_OK ���� ���� �쳣
 */
HPR_INT32 CHPPClient::OnConnectionClosed(ConnSession* p_conn_session_)
{
    if (p_conn_session_ != NULL)
    {
        p_conn_session_->StopService();
        CHPPClient::Instance()->ServerSessionId(-1);
        CHPPClient::Instance()->ServerSession(NULL);
        if (p_conn_session_->GetSessionId() ==
            CHPPClient::Instance()->ServerSessionId())
        {
            TPLOG_INFO("Session to HPP server has been closed");
        }
        else
        {
            TPLOG_ERROR("Unknow session closed");
        }
    }
    CHPPClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     void CHPPClient::OnConnectionError()
 *  @brief  hpp����ʧ�ܴ�����
 *  @param[in]  p_user_data_ �û���Ϣ
 *  @return HPR_OK ���� ���� �쳣
 */
HPR_INT32 CHPPClient::OnConnectionError(HPR_VOIDPTR p_user_data_)
{
    TPLOG_ERROR("HPP client connect to server failed");
    CHPPClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     WrapIf* CHPPClient::FormAuthorizationWrap()
 *  @brief  ���hpp��Ϣ
 *  @param  [in]msg, ��Ϣ
 *  @param  [in]iMsgSize, ��Ϣ����
 *  @return hpp��Ϣ ���� NULL �쳣
 */
WrapIf* CHPPClient::FormAuthorizationWrap(const ALARM_MSG &msg, int iMsgSize)
{
    PBWrap<AlarmMsg::AuthorizeReq>* p_wrap =
        PBWrapFactory<AlarmMsg::AuthorizeReq>::CreatePBWrap();

    if (NULL == p_wrap)
    {
        LOG_ERROR("Create DemoCheckPrimeRsp failed, no memory");
        return NULL;
    }

    AlarmMsg::AuthorizeReq& r_msg = p_wrap->GetMsg();
    AlarmMsg::Alarm* p_cmd = r_msg.mutable_alarminfo();

    p_wrap->SetCommandId(AlarmMsg::AUTHORIZE_REQ);
    p_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
    p_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);
    p_wrap->SetWrapDstId(CHPPClient::Instance()->m_server_session_id);

    char szBuff[10] = {0};

    //ģ��
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwSvcType);
    p_cmd->set_svctype(fcA2U(szBuff));

    //��Ϣ����
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwMsgType);
    p_cmd->set_dwmsgtype(fcA2U(szBuff));

    //��������
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwAlarmType);
    p_cmd->set_alarmtype(fcA2U(szBuff));

    //�û�
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwUserID);
    p_cmd->set_userid(fcA2U(szBuff));

    //ͨ��
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwChannel);
    p_cmd->set_channel(fcA2U(szBuff));

    //����״̬ 1���� 0 �Ǳ���
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwStatus);
    p_cmd->set_alarmstatus(fcA2U(szBuff));

    //���±�����������
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwRev);
    p_cmd->set_dwrev(fcA2U(szBuff));

    //������Ϣ
    p_cmd->set_alarminfo(fcA2U(msg.szAlarmInfo));

    //����ip��ַ
    p_cmd->set_alarmhost(fcA2U(msg.szHost));

    return p_wrap;
}

//��ȡ����ip
bool CHPPClient::GetLocalIp(std::string &strIp)
{
    char local[255] = {0};
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (NULL == ph)
    {
        return false;
    }
    // �������ȡ��һ��ip
    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
    std::string localIP;
    localIP.assign(inet_ntoa(addr));

    strIp = localIP.c_str();

    return true;
}
