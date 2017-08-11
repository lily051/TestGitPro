#include "StdAfx.h"
#include <fclib/fcTchar.h>
#include "hlogConfig.h"
#include <SvcGeneralDef.h>
#include "MsgLayer.h"
#include "hppTTSClient.h"
#include <fclib/Markup.h>
#include <HPR_FileSystem.h>
#include <iostream>
#include <fstream>

CHPPTTSClient* CHPPTTSClient::m_instance = NULL;

namespace {
    //服务类型
    static const HPR_UINT16 SERVICE_TYPE_CLIENT    = 0x3738;
    static const HPR_UINT16 ALARM_HPP_SERVER_PORT = 6678;
    TCHAR  const SVC_CFG_FILE[]                    = _T("\\SvcCfg.dat");
} //~ end of anonymous namespace

/** @fn     CHPPTTSClient::CHPPTTSClient()
 *  @brief  Constructor
 *  @param  void
 *  @return void
 */
CHPPTTSClient::CHPPTTSClient()
: m_h_hpp(NULL)
, m_server_session(NULL)
, m_server_session_id(-1)
, m_strServerIp("127.0.0.1")
, m_iServerPort(ALARM_HPP_SERVER_PORT)
, m_bConn(false)
{
}

/** @fn     CHPPTTSClient::~CHPPTTSClient()
 *  @brief  Destructor
 *  @param  void
 *  @return void
 */
CHPPTTSClient::~CHPPTTSClient()
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
CHPPTTSClient* CHPPTTSClient::Instance()
{
    if (NULL == m_instance)
    {
        m_instance = new (std::nothrow) CHPPTTSClient();
    }
    return m_instance;
}

/** @fn     void CHPPDemoServer::Free()
 *  @brief  Destroy HPP server instance
 *  @param  void
 *  @return void
 */
void CHPPTTSClient::Free()
{
    if (m_instance != NULL)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

/** @fn     CHPPTTSClient::Init
 *  @brief  Initialize HPP server
 *  @return success, HPR_OK; else, HPR_ERROR
 */
HPR_INT32 CHPPTTSClient::Init()
{
    m_h_hpp = HPP_Init(2, 30, HPR_FALSE);
    if (m_h_hpp == NULL)
    {
        TPLOG_ERROR("HPP init failed");
        return HPR_ERROR;
    }

    //HPP_SetPbMsgCreateFun(m_h_hpp, CreateMessageByCmdId);
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPTTSClient::Finalize()
*  @brief  Finalize HPP  server
*  @param  void
*  @return HPR_OK
*/
HPR_INT32 CHPPTTSClient::Finalize()
{
    if (CHPPTTSClient::Instance()->m_server_session != NULL)
    {
        ///< Stop ConnSession
        CHPPTTSClient::Instance()->m_server_session->StopService();
        CHPPTTSClient::Instance()->m_server_session = NULL;
        CHPPTTSClient::Instance()->m_server_session_id = -1;
    }

    if(NULL != m_h_hpp)
    {
        HPP_Close(m_h_hpp);
        m_h_hpp = NULL;
    }

    CHPPTTSClient::Instance()->m_bConn = false;

    Free();
    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPTTSClient::StartUp()
*  @brief  StartUp HPP server
*  @param  void
*  @return success, HPR_OK; else, HPR_ERROR
*/
HPR_INT32 CHPPTTSClient::StartUp()
{
    std::string strAlarmCenterIp = m_strServerIp;
    if(!LoadConfig())
    {
        return HPR_ERROR;
    }
    if (0 != strcmp(m_strServerIp.c_str(), strAlarmCenterIp.c_str()))
    {
        ShutDown();
        strAlarmCenterIp = m_strServerIp;
    }

    if (true == CHPPTTSClient::Instance()->m_bConn)
    {
        return HPR_OK;
    }

    if (0 == strcmp(strAlarmCenterIp.c_str(), "127.0.0.1"))
    {
        if(!GetLocalIp(strAlarmCenterIp))
        {
            TPLOG_ERROR("Get Local Ip failed");
            return HPR_ERROR;
        }
    }
    ///< Use asynchronous connect api to connect to server
    HPR_INT32 n_ret = HPP_ConnRemoteServiceNoBlock(
        m_h_hpp, strAlarmCenterIp.c_str(), (HPR_UINT16)m_iServerPort,
        OnConnectionComplete, NULL, 0, NULL, SERVICE_NORMAL_PRI, OnConnectionError);

    if (n_ret == HPR_ERROR)
    {
        TPLOG_ERROR("Connect to HPP server(%s:%d) failed",
            strAlarmCenterIp.c_str(), m_iServerPort);
        return HPR_ERROR;
    }

    //等待连接成功才返回， 超过6秒判断为异常
    if(!(CHPPTTSClient::Instance()->m_bConn))
    {
        for(int i = 0; i < 120; i++)
        {
            Sleep(50);
            if(CHPPTTSClient::Instance()->m_bConn)
            {
                TPLOG_INFO("Connect to HPP server(%s:%d) success",
                    m_strServerIp.c_str(), m_iServerPort);
                return HPR_OK;
            }
        }
        TPLOG_ERROR("Connect to HPP server(%s:%d) timeout",
            m_strServerIp.c_str(), m_iServerPort);
        return HPR_ERROR;
    }

    return HPR_OK;
}

/** @fn     HPR_INT32 CHPPTTSClient::ShutDown()
 *  @brief  ShutDown HPP demo client
 *  @param  void
 *  @return HPR_OK
 */
HPR_INT32 CHPPTTSClient::ShutDown()
{
    if (CHPPTTSClient::Instance()->m_server_session != NULL)
    {
        ///< Stop ConnSession
        CHPPTTSClient::Instance()->m_server_session->StopService();
        CHPPTTSClient::Instance()->m_server_session = NULL;
        CHPPTTSClient::Instance()->m_server_session_id = -1;
    }

    CHPPTTSClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     void CHPPTTSClient::SendMessage()
 *  @brief  发送 dMessage
*  @param  [in]msg, 消息
*  @param  [in]iMsgSize, 消息长度
 *  @return void
 */
void CHPPTTSClient::SendMessage(const ALARM_MSG &msg, int iMsgSize)
{
    WrapIf* p_wrap = FormAuthorizationWrap(msg, iMsgSize);
    if (NULL != p_wrap
        && NULL != CHPPTTSClient::Instance()->m_server_session
        && -1 != CHPPTTSClient::Instance()->m_server_session_id)
    {
        int n_ret = CHPPTTSClient::Instance()->m_server_session->SendMessage(p_wrap);
        if (HPR_OK != n_ret)
        {
            PBWrapFactory<AlarmMsg::AuthorizeReq>::DestroyPBWrap(p_wrap);
            TPLOG_ERROR("HPP client send authorization wrap failed");
        }
    }
    return;
}

/** @fn     void CHPPTTSClient::GetHppHandle()
*  @brief  获取hpp handle
*  @return hpp handle
*/
HPP_HANDLE CHPPTTSClient::GetHppHandle()
{
    return m_h_hpp;
}

/** @fn     void CHPPTTSClient::GetConnState()
*  @brief  获取连接状态
*  @return m_bConn
*/
bool CHPPTTSClient::GetConnState()
{
    return CHPPTTSClient::Instance()->m_bConn;
}

/** @fn     void CHPPTTSClient::OnConnectionComplete()
 *  @brief  hpp连接成功 处理函数
 *  @param[in]  p_conn_session_ 会话
 *  @return HPR_OK 正常 其他 异常
 */
HPR_INT32 CHPPTTSClient::OnConnectionComplete(ConnSession* p_conn_session_)
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

    CHPPTTSClient::Instance()->ServerSessionId(p_conn_session_->GetSessionId());
    CHPPTTSClient::Instance()->ServerSession(p_conn_session_);

    CHPPTTSClient::Instance()->m_bConn = true;
    return HPR_OK;
}

/** @fn     void CHPPTTSClient::OnConnectionClosed()
 *  @brief  hpp断开连接处理函数
 *  @param[in]  p_conn_session_ 会话
 *  @return HPR_OK 正常 其他 异常
 */
HPR_INT32 CHPPTTSClient::OnConnectionClosed(ConnSession* p_conn_session_)
{
    TPLOG_INFO("HPP Connect to server is Closed");
    CHPPTTSClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     void CHPPTTSClient::OnConnectionError()
 *  @brief  hpp连接失败处理函数
 *  @param[in]  p_user_data_ 用户信息
 *  @return HPR_OK 正常 其他 异常
 */
HPR_INT32 CHPPTTSClient::OnConnectionError(HPR_VOIDPTR p_user_data_)
{
    TPLOG_ERROR("HPP client connect to server failed");
    CHPPTTSClient::Instance()->m_bConn = false;
    return HPR_OK;
}

/** @fn     WrapIf* CHPPTTSClient::FormAuthorizationWrap()
 *  @brief  填充hpp信息
 *  @param  [in]msg, 消息
 *  @param  [in]iMsgSize, 消息长度
 *  @return hpp信息 正常 NULL 异常
 */
WrapIf* CHPPTTSClient::FormAuthorizationWrap(const ALARM_MSG &msg, int iMsgSize)
{
    PBWrap<AlarmMsg::AuthorizeReq>* p_wrap =
        PBWrapFactory<AlarmMsg::AuthorizeReq>::CreatePBWrap();

    if (NULL == p_wrap)
    {
        TPLOG_ERROR("Create DemoCheckPrimeRsp failed, no memory");
        return NULL;
    }

    AlarmMsg::AuthorizeReq& r_msg = p_wrap->GetMsg();
    AlarmMsg::Alarm* p_cmd = r_msg.mutable_alarminfo();

    p_wrap->SetCommandId(AlarmMsg::AUTHORIZE_REQ);
    p_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
    p_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);
    p_wrap->SetWrapDstId(CHPPTTSClient::Instance()->m_server_session_id);

    char szBuff[10] = {0};

    //模块
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwSvcType);
    p_cmd->set_svctype(szBuff);

    //消息类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwMsgType);
    p_cmd->set_dwmsgtype(szBuff);

    //服务类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwAlarmType);
    p_cmd->set_alarmtype(szBuff);

    //用户
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwUserID);
    p_cmd->set_userid(szBuff);

    //通道
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwChannel);
    p_cmd->set_channel(szBuff);

    //报警状态 1报警 0 非报警
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwStatus);
    p_cmd->set_alarmstatus(szBuff);

    //导致报警操作类型
    memset(szBuff, 0, sizeof(szBuff));
    sprintf_s(szBuff, sizeof(szBuff), "%d", msg.dwRev);
    p_cmd->set_dwrev(szBuff);

    //报警信息
    p_cmd->set_alarminfo(msg.szAlarmInfo);

    //报警ip地址
    p_cmd->set_alarmhost(msg.szHost);

    return p_wrap;
}

/** @fn     LoadConfig
*  @brief  read hpp config info
*  @bool
*/
bool CHPPTTSClient::LoadConfig()
{
    char rootPath[MAX_PATH] = {0};
    DWORD nLength;
    nLength = GetModuleFileNameA(NULL, rootPath, MAX_PATH);
    if (0 == nLength)
    {
        TPLOG_ERROR("GetModuleFileNameA failed");
        return false;
    }


    for(int i = 0; i < 2; i++)
    {
        char *pEnd = NULL;
        if ((pEnd = ::strrchr(rootPath, '\\')) != NULL
            || (pEnd = ::strrchr(rootPath, '/')) != NULL)
        {
            if (pEnd - rootPath + 1 < (int)(_countof(rootPath)))
            {
                *(pEnd) = 0;
            }
            else
            {
                return false;
            }
        }
        else
        {
            TPLOG_ERROR("failed to get splash for: %s", rootPath);
            return false;
        }
    }

    std::wstring strFileParth = fcA2T(rootPath);
    strFileParth = strFileParth + SVC_CFG_FILE;

    if (HPR_OK != HPR_AccessFile(fcT2A(strFileParth.c_str()), HPR_F_OK))
    {
        TPLOG_ERROR("config file %s not exists", fcT2A(strFileParth.c_str()));
        return false;
    }

    std::ifstream in(strFileParth.c_str());

    in.seekg(0, std::ios::end);
    int len = in.tellg();
    in.seekg(std::ios::beg);

    char* str = new char[len];
    in.read(str, len);
    std::string strFileContent = str;
    delete[] str;

    CMarkup xmlInfo;
    xmlInfo.SetDoc(fcA2T(strFileContent.c_str()));

    if (xmlInfo.FindElem(_T("CentorProcessor")) && xmlInfo.IntoElem())
    {
        // ALARM IP
        if (xmlInfo.FindElem(_T("AlarmCenter")))
        {
            m_strServerIp = fcT2A(xmlInfo.GetData().c_str());
        }
        else
        {
            TPLOG_ERROR("ALARM IP is not exist in config file");
            return false;
        }

        return true;
    }

    return false;
}

//获取本机ip
bool CHPPTTSClient::GetLocalIp(std::string &strIp)
{
    char local[255] = {0};
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (ph == NULL)
    {
        return false;
    }
    // 这里仅获取第一个ip
    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
    std::string localIP;
    localIP.assign(inet_ntoa(addr));

    strIp = localIP.c_str();

    return true;
}
