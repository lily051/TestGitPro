/** @file   hppClient.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  hpp 发报警客户端
 *          
 *  @author chenxiaoyan3@hikvision.com
 *  @date   2014/12/12
 */

#ifndef GUARD_HPP_CLIENT_H
#define GUARD_HPP_CLIENT_H

#include <hpp_hpp.h>
#include <string>

class CHPPClient
{
public:
    CHPPClient();
    virtual ~CHPPClient();

    /** @fn     CHPPClient* CHPPClient::Instance()
     *  @brief  Get HPP server instance
     *  @param  void
     *  @return HPP server instance
     */
    static CHPPClient* Instance();

    /** @fn     void CHPPClient::Free()
     *  @brief  Destroy HPP server instance
     *  @param  void
     *  @return void
     */
    static void Free();

    /** @fn     CHPPClient::Init
     *  @brief  Initialize HPP server
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 Init();

    /** @fn     HPR_INT32 CHPPClient::Finalize()
     *  @brief  Finalize HPP  server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 Finalize();

    /** @fn     HPR_INT32 CHPPClient::StartUp()
     *  @brief  StartUp HPP server
     *  @param  void
     *  @return success, HPR_OK; else, HPR_ERROR
     */
    HPR_INT32 StartUp();

    /** @fn     HPR_INT32 CHPPClient::StartUp()
    *  @brief  StartUp HPP server
    *  @param  [in]strServerIp, ip
    *  @param  [in]iServerPort, 端口
    *  @return success, HPR_OK; else, HPR_ERROR
    */
    HPR_INT32 StartUp(const std::string &strServerIp, HPR_INT32 iServerPort);

    /** @fn     HPR_INT32 CHPPClient::ShutDown()
     *  @brief  ShutDown HPP server
     *  @param  void
     *  @return HPR_OK
     */
    HPR_INT32 ShutDown();

    /** @fn     void CHPPClient::SendMessage()
     *  @brief  发送 dMessage
    *  @param  [in]msg, 消息
    *  @param  [in]iMsgSize, 消息长度
     *  @return void
     */
    void SendMessage(const ALARM_MSG &msg, int iMsgSize);

    /** @fn     void CHPPClient::GetHppHandle()
     *  @brief  获取hpp handle
     *  @return hpp handle
     */
    HPP_HANDLE GetHppHandle();

    HPR_INT32 ServerSessionId() const
	{
		return m_server_session_id;
	}
	
    void ServerSessionId(HPR_INT32 n_server_session_id_)
    {
		m_server_session_id = n_server_session_id_;
	}
	
    void ServerSession(ConnSession* p_server_session_)
	{
		m_server_session = p_server_session_;
    }

private:
    /** @fn     void CHPPClient::OnConnectionComplete()
     *  @brief  hpp连接成功 处理函数
     *  @param[in]  p_conn_session_ 会话
     *  @return HPR_OK 正常 其他 异常
     */
    static HPR_INT32 OnConnectionComplete(ConnSession* p_conn_session_);

    /** @fn     void CHPPClient::OnConnectionClosed()
     *  @brief  hpp断开连接处理函数
     *  @param[in]  p_conn_session_ 会话
     *  @return HPR_OK 正常 其他 异常
     */
    static HPR_INT32 OnConnectionClosed(ConnSession* p_conn_session_);

    /** @fn     void CHPPClient::OnConnectionError()
     *  @brief  hpp连接失败处理函数
     *  @param[in]  p_user_data_ 用户信息
     *  @return HPR_OK 正常 其他 异常
     */
    static HPR_INT32 OnConnectionError(HPR_VOIDPTR p_user_data_);

    /** @fn     WrapIf* CHPPClient::FormAuthorizationWrap()
     *  @brief  填充hpp信息
     *  @param  [in]msg, 消息
     *  @param  [in]iMsgSize, 消息长度
     *  @return hpp信息 正常 NULL 异常
     */
    WrapIf* FormAuthorizationWrap(const ALARM_MSG &msg, int iMsgSize);

    //获取本机ip
    bool GetLocalIp(std::string &strIp);

private:
    HPP_HANDLE         m_h_hpp;             // hpp handle
    static CHPPClient* m_instance;
    ConnSession*       m_server_session;    // 会话
    HPR_INT32          m_server_session_id; // 会话id
    std::string        m_strServerIp;       // hpp服务器ip
    HPR_INT32          m_iServerPort;       // hpp服务器端口
    volatile bool      m_bConn;             //是否已经连接上hpp服务器
};

#endif //GUARD_HPP_CLIENT_H
