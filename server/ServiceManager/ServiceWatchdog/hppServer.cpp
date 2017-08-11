#include "StdAfx.h"
#include "fclib/fcTchar.h"
#include "hlogConfig.h"
#include <SvcGeneralDef.h>
#include "hppServer.h"

namespace {
    //服务类型
    static const HPR_UINT16 SERVICE_TYPE_SERVER = 0x1020;
} //~ end of anonymous namespace

CHPPServer* CHPPServer::m_instance = NULL;

WrapIf* CreateMessageByCmdId(HPR_INT32 n_cmd_id)
{
    WrapIf* p_wrap = NULL;

    switch (n_cmd_id)
    {
    case cms_8120::CMD_SERVICE_WATCH_DOG_CONTROL_REQ:
        p_wrap = PBWrapFactory<cms_8120::ServiceWatchDogControlReq>::CreatePBWrap();
        break;
    default:
        p_wrap = NULL;
    }

    return p_wrap;
}

/** @fn     CHPPServer::CHPPServer()
 *  @brief  Constructor
 *  @param  void
 *  @return void
 */
CHPPServer::CHPPServer()
: m_strServerIp("0.0.0.0")
, m_h_hpp(NULL)
, m_h_server(NULL)
{

}

/** @fn     CHPPServer::~CHPPServer()
 *  @brief  Destructor
 *  @param  void
 *  @return void
 */
CHPPServer::~CHPPServer()
{
    if (m_h_server != NULL)
    {
        HPP_StopLocalServer(m_h_hpp, m_h_server);
        m_h_server = NULL;
    }

    if(NULL != m_h_hpp)
    {
        HPP_Close(m_h_hpp);
        m_h_hpp = NULL;
    }
}


/** @fn     CHPPServer* CHPPServer::Instance()
 *  @brief  Get HPP server instance
 *  @param  void
 *  @return HPP server instance
 */
CHPPServer* CHPPServer::Instance()
{
    if (m_instance == NULL)
    {
        m_instance = new (std::nothrow) CHPPServer();
    }
    return m_instance;
}

/** @fn     void CHPPServer::FreeInstance()
 *  @brief  Destroy HPP server instance
 *  @param  void
 *  @return void
 */
void CHPPServer::FreeInstance()
{
    if (m_instance != NULL)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

/** @fn     CHPPServer::Init
 *  @brief  Initialize HPP server
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPServer::InitHpp()
{
    m_h_hpp = HPP_Init(5, 10, HPR_FALSE);
    if (m_h_hpp == NULL)
    {
        SVCMANAGERLOG_ERROR("HPP init failed");
        return HPR_ERROR;
    }

    HPP_SetPbMsgCreateFun(m_h_hpp, CreateMessageByCmdId);

    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPServer::Finalize()
 *  @brief  Finalize HPP  server
 *  @param  void
 *  @return HPR_OK
 */
HPR_INT32 CHPPServer::UninitHpp()
{
    ///< stop all ConnSession
    for (clients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->first != 0)
        {
            HPP_GetHppSessionById(it->first)->StopService();
        }
    }
    m_clients.clear();

    ///< stop all temp ConnSession
    for (size_t i = 0; i < m_tmp_session_list.size(); i++)
    {
        HPP_GetHppSessionById(m_tmp_session_list[i])->StopService();
    }
    m_tmp_session_list.clear();

    ///< stop local server
    if (m_h_server != NULL)
    {
        HPP_StopLocalServer(m_h_hpp, m_h_server);
        m_h_server = NULL;
    }

    if(NULL != m_h_hpp)
    {
        HPP_DelPbMsgCreateFun(m_h_hpp, CreateMessageByCmdId);
        HPP_Close(m_h_hpp);
        m_h_hpp = NULL;
    }
    FreeInstance();
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPServer::StartUp()
 *  @brief  StartUp HPP server
 *  @param  hpp server port
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPServer::StartListen(HPR_UINT16 uiListenPort)
{
    if (NULL != m_h_server)
    {
        return HPR_OK;
    }
    m_h_server = HPP_StartLocalServer(m_h_hpp, m_strServerIp.c_str(),
        (HPR_UINT16)uiListenPort, OnAcceptClient, ConnSession::PROTOCOL_TYPE_HPP);
    if (NULL == m_h_server)
    {
        SVCMANAGERLOG_ERROR("start local server failed");
        return HPR_ERROR;
    }

    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPServer::ShutDown()
 *  @brief  ShutDown HPP server
 *  @param  void
 *  @return HPR_OK
 */
HPR_INT32 CHPPServer::StopListen()
{
    ///< stop local server
    if (m_h_server != NULL)
    {
        HPP_StopLocalServer(m_h_hpp, m_h_server);
        m_h_server = NULL;
    }

    return HPR_OK;
}

/** @fn       HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_)
 *  @brief    连接超时回调函数
 *  @param
 *  @return:  HPR_BOOL
 */
HPR_BOOL CHPPServer::OnServerCheckTimeout(ConnSession* p_conn_session_)
{
    SVCMANAGERLOG_WARN(_T("OnServerCheckTimeout..."));

    // do something

    return HPR_TRUE;
}

/** @fn     HPR_INT32 CHPPServer::OnAcceptClient(ConnSession* p_conn_session_)
 *  @brief  Callback function when listener accept a client
 *  @param  p_conn_session_ [in] client session
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPServer::OnAcceptClient(ConnSession* p_conn_session_)
{
    SVCMANAGERLOG_INFO("HPP server accept connection:%s, session id:%d.",
        p_conn_session_->GetSockRemoteIp(), p_conn_session_->GetSessionId());

    p_conn_session_->SetServiceType(SERVICE_TYPE_SERVER);
    p_conn_session_->SetSocketClosedCallBack(OnSocketClosed);
    p_conn_session_->SetTimeOutCallBack(OnServerCheckTimeout);

    ///< start ConnSession
    if (HPR_OK != p_conn_session_->StartService(SERVICE_NORMAL_PRI))
    {
        return HPR_ERROR;
    }

    return Instance()->AddSessionId(p_conn_session_->GetSessionId());
}


/** @fn     HPR_INT32 CHPPServer::OnSocketClosed(ConnSession* p_conn_session_)
 *  @brief  Callback function when listener found session closed
 *  @param  p_conn_session_ [in] client session has been closed
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPServer::OnSocketClosed(ConnSession* p_conn_session_)
{
    SVCMANAGERLOG_INFO("HPP server found connection closed, session id:%d", p_conn_session_->GetSessionId());
    if (p_conn_session_ != NULL)
    {
        p_conn_session_->StopService();
    }
    return Instance()->RemoveSessionId(p_conn_session_->GetSessionId());
}

/** @fn     HPR_INT32 CHPPServer::AddSessionId(HPR_INT32 n_session_id_, const std::string& s_name_ = "")
 *  @brief  Add session id to tmp list or client map
 *  @param  n_session_id_ [in] session id
 *  @param  s_name_ [in] client name, default to ""
 *  @return success, HPR_OK; else, HPR_ERROR;
 */
HPR_INT32 CHPPServer::AddSessionId(HPR_INT32 n_session_id_, const std::string& s_name_ /* = "" */)
{
    HPR_Guard lock(&m_client_map_mutex);
    if (0 == s_name_.size())
    {
        for (tmp_session_list::iterator it = m_tmp_session_list.begin(); it != m_tmp_session_list.end(); ++it)
        {
            if (*it == n_session_id_)
            {
                SVCMANAGERLOG_ERROR("Session already exist");
                return HPR_ERROR;
            }
        }
        m_tmp_session_list.push_back(n_session_id_);
        return HPR_OK;
    }
    else
    {
        bool b_tmp_session_found = false;
        for (tmp_session_list::iterator it = m_tmp_session_list.begin(); it != m_tmp_session_list.end(); ++it)
        {
            if (*it == n_session_id_)
            {
                m_tmp_session_list.erase(it);
                b_tmp_session_found = true;
                break;
            }
        }

        if (!b_tmp_session_found)
        {
            SVCMANAGERLOG_ERROR("Unknow session found");
            return HPR_ERROR;
        }

        clients::iterator it = m_clients.find(n_session_id_);
        if (it != m_clients.end())
        {
            SVCMANAGERLOG_ERROR("Session %d to client %s already exit", it->first, it->second.c_str());
            return HPR_ERROR;
        }
        else
        {
            SVCMANAGERLOG_INFO("Session %d to client %s add to client map", n_session_id_, s_name_.c_str());
            m_clients.insert(clients::value_type(n_session_id_, s_name_));
            return HPR_OK;
        }
    }
}

/** @fn     HPR_INT32 CHPPServer::RemoveSessionId(HPR_INT32 n_session_id_)
 *  @brief  Remove session id from tmp list or client map
 *  @param  n_session_id_ [in] session id
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPServer::RemoveSessionId(HPR_INT32 n_session_id_)
{
    HPR_Guard lock(&m_client_map_mutex);

    bool b_tmp_session_found = false;
    for (tmp_session_list::iterator it = m_tmp_session_list.begin(); it != m_tmp_session_list.end(); it++)
    {
        if (*it == n_session_id_)
        {
            m_tmp_session_list.erase(it);
            b_tmp_session_found = true;
            break;
        }
    }
    if (b_tmp_session_found)
    {
        SVCMANAGERLOG_INFO("Remove tmp session %d", n_session_id_);
        return HPR_OK;
    }

    clients::iterator it = m_clients.find(n_session_id_);
    if (it != m_clients.end())
    {
        SVCMANAGERLOG_ERROR("Remove session %d to client %s", it->first, it->second.c_str());
        m_clients.erase(it);
        return HPR_OK;
    }

    SVCMANAGERLOG_ERROR("Found session %d not exist", n_session_id_);
    return HPR_ERROR;
}
