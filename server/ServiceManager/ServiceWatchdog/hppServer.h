/** @file   hpp_Server.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  hpp �ձ���������
 *          
 *  @author chenxiaoyan3@hikvision.com
 *  @date   2014/12/12
 */

#ifndef GUARD_HPP_SERVER_H
#define GUARD_HPP_SERVER_H

#include "GenDef.h"
#include "MainDialog.h"
#include "ServiceWatchdog.h"
#include <hpp_hpp.h>
#include <cmd.pb.h>
#include <ServiceWatchDogControl.pb.h>
#include <SvcGeneralDef.h>
#include <fclib/fcTchar.h>
#include <HPR_Singleton.h>

//#include "HPR_Thread.h"
//#include "HPR_Guard.h"
//#include "HPR_Atomic.h"


#include <string>
#include <map>
#include <vector>

class CHPPServer : public singleton<CHPPServer>
{
    friend class singleton<CHPPServer>;
public:
    CHPPServer();
    ~CHPPServer();

    /** @fn     CHPPServer* CHPPServer::Instance()
     *  @brief  Get HPP server instance
     *  @param  void
     *  @return HPP server instance
     */
    static CHPPServer* Instance();

    /** @fn     void CHPPServer::Free()
     *  @brief  Destroy HPP server instance
     *  @param  void
     *  @return void
     */
    static void FreeInstance();

    /** @fn     CHPPServer::Init
     *  @brief  Initialize HPP server
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 InitHpp();

    /** @fn     HPR_INT32 CHPPServer::Finalize()
     *  @brief  Finalize HPP  server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 UninitHpp();

    /** @fn     HPR_INT32 CHPPServer::StartUp()
     *  @brief  StartUp HPP server
     *  @param  uiListenPort hpp server port
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 StartListen(HPR_UINT16 uiListenPort);

    /** @fn     HPR_INT32 CHPPServer::ShutDown()
     *  @brief  ShutDown HPP server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 StopListen();

    /** @fn       HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_)
     *  @brief    ���ӳ�ʱ�ص�����
     *  @param
     *  @return:  HPR_BOOL
     */
    static HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_);

    /** @fn     HPR_INT32 CHPPServer::OnAcceptClient(ConnSession* p_conn_session_)
     *  @brief  Callback function when listener accept a client
     *  @param  p_conn_session_ [in] client session
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    static HPR_INT32 OnAcceptClient(ConnSession* p_conn_session_);

    /** @fn     HPR_INT32 CHPPServer::OnSocketClosed(ConnSession* p_conn_session_)
     *  @brief  Callback function when listener found session closed
     *  @param  p_conn_session_ [in] client session has been closed
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    static HPR_INT32 OnSocketClosed(ConnSession* p_conn_session_);

private:
    /** @fn     HPR_INT32 CHPPServer::AddSessionId(HPR_INT32 n_session_id_, const std::string& s_name_ = "")
     *  @brief  Add session id to tmp list or client map
     *  @param  n_session_id_ [in] session id
     *  @param  s_name_ [in] client name, default to ""
     *  @return success, HPR_OK; else, HPR_ERROR;
     */
    HPR_INT32 AddSessionId(HPR_INT32 n_session_id_, const std::string& s_name_ = "");

    /** @fn     HPR_INT32 CHPPServer::RemoveSessionId(HPR_INT32 n_session_id_)
     *  @brief  Remove session id from tmp list or client map
     *  @param  n_session_id_ [in] session id
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 RemoveSessionId(HPR_INT32 n_session_id_);

    //��ȡ����ip
    bool GetLocalIp(std::string &strIp);

private:
    typedef std::vector<HPR_INT32 /*session id*/> tmp_session_list; //�Ự�б�
    typedef std::map<HPR_INT32/* session_id*/, std::string/*client name*/> clients;   //�Ựmap��

    std::string        m_strServerIp;      // hpp������ip
    HPP_HANDLE         m_h_hpp;            //hpp handle
    HPP_SERVER_HANDLE  m_h_server;         //hpp ����TCP���������HANDLE
    HPR_Mutex          m_client_map_mutex; //map��
    tmp_session_list   m_tmp_session_list; //�Ự�б�
    clients            m_clients;          //�Ựmap��
    static CHPPServer* m_instance;
};
WrapIf* CreateMessageByCmdId(HPR_INT32 n_cmd_id);

inline CHPPServer* GetHPPServer(void)
{
    return CHPPServer::instance();
}

template<> inline WrapIf* PBWrap<cms_8120::ServiceWatchDogControlReq>::DoExecute()
{
    cms_8120::ServiceWatchDogControlReq& req_msg = GetMsg();
    HPR_INT32 Datatype = req_msg.data_type();

    if (Datatype != cms_8120::CMD_SERVICE_WATCH_DOG_CONTROL_REQ)
    {
        SVCMANAGERLOG_DEBUG(_T("PBWrap<cms_8100::ServiceWatchDogControlReq>::DoExecute: Datatype != cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_REQ."));
        return NULL;
    }

    HPR_INT32 ControlType = req_msg.control_type();
    HPR_INT32 ModuleType = req_msg.server_type();
    std::string ServerName = req_msg.server_name();
    std::string ShellCmd = req_msg.shell_cmd();
    std::string ShellDir = req_msg.shell_dir();

    // ��־
    {
        SVCMANAGERLOG_DEBUG(_T("data_type[%d], ControlType[%d], ServerName[%s]."), Datatype, ControlType, ServerName.c_str());
    }

    HPP_PACK_HEAD hppPackHead = GetPackHead();
    if (HPP_PACK_HEAD::REQ_MSG_NEED_RSP == hppPackHead.m_nMark)
    {
        //ֻ����һ����Ӧ
        PBWrap<cms_8120::ServiceWatchDogControlRsp>* p_rsp_wrap = 
            PBWrapFactory<cms_8120::ServiceWatchDogControlRsp>::CreatePBWrap();

        if (p_rsp_wrap != NULL)
        {
            p_rsp_wrap->SetCommandId(cms_8120::CMD_SERVICE_WATCH_DOG_CONTROL_RSP);
            p_rsp_wrap->SetInnerSequence(GetInnerSequence());
            p_rsp_wrap->SetMessageType(HPP_PACK_HEAD::RSP_MSG_FINISH);
            p_rsp_wrap->SetWrapDstId(GetWrapSrcId());

            cms_8120::ServiceWatchDogControlRsp & p_rsp_msg = p_rsp_wrap->GetMsg();
            p_rsp_msg.set_data_type(cms_8120::CMD_SERVICE_WATCH_DOG_CONTROL_RSP);
            p_rsp_msg.set_result(1);
            p_rsp_msg.set_control_type(ControlType);

            //������Ӧ��
            if (HPR_OK != ConnSession::SendMessage_r(p_rsp_wrap, GetWrapSrcId()))
            {
                SVCMANAGERLOG_ERROR(_T("ServiceWatchDogControlRsp SendMessage failed."));

                PBWrapFactory<cms_8120::ServiceWatchDogControlRsp>::DestroyPBWrap(p_rsp_wrap);
            }
        }
    }

    // ��Ϣ����
    RECV_CMD_MSG struRecvCmdMsg;
    struRecvCmdMsg.nCtrlType = ControlType;
    struRecvCmdMsg.nModuleType = ModuleType;
    struRecvCmdMsg.svcName = ServerName;
    struRecvCmdMsg.shellCmd = fcU2A(ShellCmd.c_str());
    struRecvCmdMsg.shellDir = fcU2A(ShellDir.c_str());

    {// RemoteIPInfo
        ConnSession* pConnSession = HPP_GetHppSessionById(GetWrapSrcId());
        if (pConnSession != NULL)
        {
            CString csRemoteIPInfo;
            csRemoteIPInfo.Format(_T("%s[%d]"), pConnSession->GetSockRemoteIp(), pConnSession->GetSockRemotePort());
            struRecvCmdMsg.remoteIP = csRemoteIPInfo.GetBuffer(0);
            csRemoteIPInfo.ReleaseBuffer();
        }
    }

    // ��������Ϣ��ӵ�������Ϣ������
    BOOL bRet = theApp.m_ctrlMsgQueue.Append(struRecvCmdMsg);
    if (FALSE == bRet)
    {
        SVCMANAGERLOG_ERROR(_T("theApp.m_ctrlMsgQueue.Append(struRecvCmdMsg) return FALSE."));
    }

    return NULL;
}

#endif //GUARD_HPP_SERVER_H
