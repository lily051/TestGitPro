/** @file   hpp_Server.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  hpp 收报警服务器
 *          
 *  @author chenxiaoyan3@hikvision.com
 *  @date   2014/12/12
 */

#ifndef GUARD_HPP_SERVER_H
#define GUARD_HPP_SERVER_H

#include "MsgLayer.h"

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
    HPR_INT32 Init();

    /** @fn     HPR_INT32 CHPPServer::Finalize()
     *  @brief  Finalize HPP  server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 Finalize();

    /** @fn     HPR_INT32 CHPPServer::StartUp()
     *  @brief  StartUp HPP server
     *  @param  void
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 StartUp();

    /** @fn     HPR_INT32 CHPPServer::ShutDown()
     *  @brief  ShutDown HPP server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 ShutDown();

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

private:
    typedef std::vector<HPR_INT32 /*session id*/> tmp_session_list; //会话列表
    typedef std::map<HPR_INT32/* session_id*/, std::string/*client name*/> clients;   //会话map表

    HPP_HANDLE         m_h_hpp;            //hpp handle
    HPP_SERVER_HANDLE  m_h_server;         //hpp 本地TCP监听服务的HANDLE
    HPR_Mutex          m_client_map_mutex; //map锁
    tmp_session_list   m_tmp_session_list; //会话列表
    clients            m_clients;          //会话map表
    static CHPPServer* m_instance;
};
WrapIf* CreateMessageByCmdId(HPR_INT32 n_cmd_id);

inline CHPPServer* GetHPPServer(void)
{
    return CHPPServer::instance();
}

#endif //GUARD_HPP_SERVER_H
