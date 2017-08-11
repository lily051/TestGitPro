/** @file   hppClient.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  hpp �������ͻ���
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
    *  @param  [in]iServerPort, �˿�
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
     *  @brief  ���� dMessage
    *  @param  [in]msg, ��Ϣ
    *  @param  [in]iMsgSize, ��Ϣ����
     *  @return void
     */
    void SendMessage(const ALARM_MSG &msg, int iMsgSize);

    /** @fn     void CHPPClient::GetHppHandle()
     *  @brief  ��ȡhpp handle
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
     *  @brief  hpp���ӳɹ� ������
     *  @param[in]  p_conn_session_ �Ự
     *  @return HPR_OK ���� ���� �쳣
     */
    static HPR_INT32 OnConnectionComplete(ConnSession* p_conn_session_);

    /** @fn     void CHPPClient::OnConnectionClosed()
     *  @brief  hpp�Ͽ����Ӵ�����
     *  @param[in]  p_conn_session_ �Ự
     *  @return HPR_OK ���� ���� �쳣
     */
    static HPR_INT32 OnConnectionClosed(ConnSession* p_conn_session_);

    /** @fn     void CHPPClient::OnConnectionError()
     *  @brief  hpp����ʧ�ܴ�����
     *  @param[in]  p_user_data_ �û���Ϣ
     *  @return HPR_OK ���� ���� �쳣
     */
    static HPR_INT32 OnConnectionError(HPR_VOIDPTR p_user_data_);

    /** @fn     WrapIf* CHPPClient::FormAuthorizationWrap()
     *  @brief  ���hpp��Ϣ
     *  @param  [in]msg, ��Ϣ
     *  @param  [in]iMsgSize, ��Ϣ����
     *  @return hpp��Ϣ ���� NULL �쳣
     */
    WrapIf* FormAuthorizationWrap(const ALARM_MSG &msg, int iMsgSize);

    //��ȡ����ip
    bool GetLocalIp(std::string &strIp);

private:
    HPP_HANDLE         m_h_hpp;             // hpp handle
    static CHPPClient* m_instance;
    ConnSession*       m_server_session;    // �Ự
    HPR_INT32          m_server_session_id; // �Ựid
    std::string        m_strServerIp;       // hpp������ip
    HPR_INT32          m_iServerPort;       // hpp�������˿�
    volatile bool      m_bConn;             //�Ƿ��Ѿ�������hpp������
};

#endif //GUARD_HPP_CLIENT_H
