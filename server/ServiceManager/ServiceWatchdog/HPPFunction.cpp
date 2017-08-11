/**
*   @file   HPPFunction.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*  
*   @author mazhentong
*   @date   2012/10/29
*
*   @note
*
*   @warning
*/
#include "stdafx.h"

#include "HPPFunction.h"

using namespace std;

///////////////////////////////////////////////////////////////
// ����CMS������

/** @fn       BOOL InitCMSClientHppHandle(HPP_HANDLE &hHandle, const CString& strServerIP, int nServerPort)
 *  @brief    ��ʼ����CMS֮���HPPЭ��ջ������
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init�ķ���ֵ
 *            (const CString&) strServerIP [IN] : CMS������IP��ַ
 *            (int) nServerPort [IN] : CMS�������Ķ˿�
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL InitCMSClientHppHandle(HPP_HANDLE &hHandle, const CString& strServerIP, int nServerPort)
{
    if (strcmp(strServerIP, _T("")) == 0 ||
        (nServerPort <=0 || nServerPort > 65535))
    {
        SVCMANAGERLOG_ERROR(_T("strServerIP[%s] or nServerPort[%d] is invalid."), (LPCTSTR)strServerIP, nServerPort);
        return FALSE;
    }

    if (NULL == hHandle)
    {
        // ��ʼ��HPP��,ʹ�����������߳�,���ñ��������ĳ�ʱʱ��Ϊ10��
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("InitCMSClientHppHandle failed. HPP_Init return NULL."));
            return FALSE;
        }

        // ����HPP(Protocol BufferЭ��)�������ɻص�����
        HPP_SetPbMsgCreateFun(hHandle, DCClientCreateMessageByCmdId);
    }

    // ��־
    SVCMANAGERLOG_INFO(_T("InitCMSClientHppHandle:ServerIP[%s], ServerPort[%d]."), (LPCTSTR)strServerIP, nServerPort); 

    // ��CMSֱ�ӵĻỰ
    if (g_pCMSConnSession != NULL)
    {
        g_pCMSConnSession->StopService();
        g_pCMSConnSession = NULL;
    }

    // ����ģʽ����
    HPR_INT32 hResult = HPP_ConnRemoteService(hHandle, strServerIP,
        (HPR_UINT16)nServerPort, OnClientConnectionComplete,
        NULL, 0, ConnSession::PROTOCOL_TYPE_HPP, NULL, SERVICE_NORMAL_PRI);

    if (hResult != HPR_OK)
    {
        SVCMANAGERLOG_ERROR(_T("InitCMSClientHppClient failed. HPP_ConnRemoteService failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("InitCMSClientHppClient success."));
    return TRUE;
}

/** @fn       BOOL ReConnDCServer(HPP_HANDLE& hHandle, const CString& strServerIP, int nServerPort)
 *  @brief    ���½�����DC֮�������
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init�ķ���ֵ
 *            (const CString&) strServerIP [IN] : CMS������IP��ַ
 *            (int) nServerPort [IN] : CMS�������Ķ˿�
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL ReConnDCServer(HPP_HANDLE& hHandle, const CString& strServerIP, int nServerPort)
{
    if (strcmp(strServerIP, _T("")) == 0 ||
        (nServerPort <=0 || nServerPort > 65535))
    {
        SVCMANAGERLOG_ERROR(_T("strServerIP[%s] or nServerPort[%d] is invalid."), (LPCTSTR)strServerIP, nServerPort);
        return FALSE;
    }

    if (NULL == hHandle)
    {
        // ��ʼ��HPP��,ʹ�����������߳�,���ñ��������ĳ�ʱʱ��Ϊ10��
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("ReConnDCServer failed. HPP_Init return NULL."));
            return FALSE;
        }

        // ����HPP(Protocol BufferЭ��)�������ɻص�����
        HPP_SetPbMsgCreateFun(hHandle, DCClientCreateMessageByCmdId);
    }

    // ��־
    SVCMANAGERLOG_INFO(_T("ReConnDCServer:ServerIP[%s], ServerPort[%d]."), (LPCTSTR)strServerIP, nServerPort); 

    // ��DCֱ�ӵĻỰ
    if (g_pCMSConnSession != NULL)
    {
        g_pCMSConnSession->StopService();
        g_pCMSConnSession = NULL;
    }

    // ����ģʽ����
    HPR_INT32 hResult = HPP_ConnRemoteService(hHandle, strServerIP,
        (HPR_UINT16)nServerPort, OnClientConnectionComplete,
        NULL, 0, ConnSession::PROTOCOL_TYPE_HPP, NULL, SERVICE_NORMAL_PRI);

    if (hResult != HPR_OK)
    {
        SVCMANAGERLOG_ERROR(_T("ReConnDCServer failed. HPP_ConnRemoteService failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("ReConnDCServer success."));
    return TRUE;
}

/** @fn       WrapIf* CMSClientCreateMessageByCmdId(HPR_INT32 cmdId)
 *  @brief    �������ɻص�����
 *  @param    (HPR_INT32) cmdId [IN] : ����ID
 *  @return:  WrapIf*
 */
WrapIf* DCClientCreateMessageByCmdId(HPR_INT32 cmdId)
{
    WrapIf * pWrapIf = NULL;
    switch (cmdId)
    {
    case cms_8100::CMD_DT_DB_INFO:
        {
            pWrapIf = PBWrapFactory<cms_8100::DBInfo>::CreatePBWrap();
        }
        break;
    default:
        pWrapIf = NULL;
        break;
    }

    return pWrapIf;
}

/** @fn       HPR_INT32 OnClientConnectionClosed(ConnSession* p_conn_session_)
 *  @brief    ���ӶϿ�ʱ(Socket�Ͽ�)ʱ�Ļص�����
 *  @param
 *  @return:  HPR_INT32��HPR_OK:�Ͽ��ɹ� ; HPR_ERROR:�Ͽ�ʧ��
 */
HPR_INT32 OnClientConnectionClosed(ConnSession* p_conn_session_)
{
    if ( (g_pCMSConnSession != NULL)
        && (g_pCMSConnSession == p_conn_session_))
    {
        SVCMANAGERLOG_WARN(_T("OnClientConnectionClosed. g_pCMSConnSession stopservice."));

        g_pCMSConnSession->StopService();

        g_pCMSConnSession = NULL;
    }
    else if (p_conn_session_ != NULL)
    {
        p_conn_session_->StopService();
    }

    return HPR_OK;
}

/** @fn       HPR_BOOL OnClientCheckTimeout(ConnSession* p_conn_session_)
 *  @brief    ���ӳ�ʱ�ص�����
 *  @param
 *  @return:  HPR_BOOL��HPR_OK:����ɹ� ; HPR_ERROR:����ʧ��
 */
HPR_BOOL OnClientCheckTimeout(ConnSession* p_conn_session_)
{
    // do something
    SVCMANAGERLOG_WARN(_T("OnClientCheckTimeout..."));

    return HPR_FALSE;
}

/** @fn       HPR_INT32 OnClientConnectionComplete(ConnSession* p_conn_session_)
 *  @brief    ���ӻص�����
 *  @param    
 *  @return:  HPR_INT32��HPR_OK:���ӳɹ� ; HPR_ERROR:����ʧ��
 */
HPR_INT32 OnClientConnectionComplete(ConnSession* p_conn_session_)
{
    if (NULL == p_conn_session_ )
    {
        return HPR_ERROR;
    }

    //���ûص�����������
    p_conn_session_->SetSocketClosedCallBack(OnClientConnectionClosed);
    p_conn_session_->SetTimeOutCallBack(OnClientCheckTimeout);

    if (HPR_OK != p_conn_session_->StartService(SERVICE_NORMAL_PRI))
    {
        SVCMANAGERLOG_ERROR(_T("OnClientConnectionComplete failed. StartService failed."));
        return HPR_ERROR;
    }

    g_pCMSConnSession = p_conn_session_;

    return HPR_OK;
}

///////////////////////////////////////////////////////////////
// ���ؼ�������

/** @fn       BOOL InitServerHppHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
 *  @brief    ��ʼ�����ؼ���������Э��ջ
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init�ķ���ֵ
 *            (HPP_SERVER_HANDLE &) hServerHandle [OUT] : HPP_StartLocalServer�ķ���ֵ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL InitServerHppHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
{
    if (Port<= 0 || Port > 65535)
    {
        SVCMANAGERLOG_ERROR(_T("InitServerHppHandle port[%d] is invalid."), Port);
        return FALSE;
    }

    if (hServerHandle != NULL)
    {
        SVCMANAGERLOG_ERROR(_T("InitServerHppHandle failed. hServerHandle != NULL."));
        return FALSE;
    }

    if (NULL == hHandle)
    {
        // ��ʼ��HPP��,ʹ�����������߳�,���ñ��������ĳ�ʱʱ��Ϊ10��
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("InitServerHppHandle failed. HPP_Init return NULL."));
            return FALSE;
        }

        // ����HPP(Protocol BufferЭ��)�������ɻص�����,
        HPP_SetPbMsgCreateFun(hHandle, ServerCreateMessageByCmdId);
    }

    SVCMANAGERLOG_INFO(_T("InitServerHppHandle: LocalIP[%s], ListenPort[%d]"), (LPCTSTR)theApp.m_hostIP, Port);

    //��ʼ��һ������TCP��������
    hServerHandle = HPP_StartLocalServer
        (
        hHandle,						    //HPP_Init ����ֵ
        _T("0.0.0.0"),				        //���ذ󶨵�IP��ַ
        (HPR_UINT16)Port,	                //���ؼ����Ķ˿�
        OnAcceptClient,					    //���յ����ӵĻص�
        ConnSession::PROTOCOL_TYPE_HPP	    //ʹ��HPPЭ��
        );	

    if(NULL == hServerHandle)   //���ط��񴴽�ʧ��
    {
        SVCMANAGERLOG_INFO(_T("InitServerHpp failed. HPP_StartLocalServer failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("InitServerHpp end."));
    return TRUE;
}

/** @fn       BOOL ReStartLocalServer(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
 *  @brief    �����������ؼ�������
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init�ķ���ֵ
 *            (HPP_SERVER_HANDLE &) hServerHandle [OUT] : HPP_StartLocalServer�ķ���ֵ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL ReStartLocalServer(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
{
    if (Port<= 0 || Port > 65535)
    {
        SVCMANAGERLOG_ERROR(_T("ReStartLocalServer port[%d] is invalid."), Port);
        return FALSE;
    }

    if (NULL == hHandle)
    {
        // ��ʼ��HPP��,ʹ�����������߳�,���ñ��������ĳ�ʱʱ��Ϊ10��
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("ReStartLocalServer failed. HPP_Init return NULL."));
            return FALSE;
        }

        // ����HPP(Protocol BufferЭ��)�������ɻص�����,
        HPP_SetPbMsgCreateFun(hHandle, ServerCreateMessageByCmdId);
    }

    SVCMANAGERLOG_INFO(_T("ReStartLocalServer: LocalIP[%s], ListenPort[%d]"), (LPCTSTR)theApp.m_hostIP, Port);

    if (hServerHandle != NULL)
    {
        HPP_StopLocalServer(hHandle, hServerHandle);
        hServerHandle = NULL;
    }

    //��ʼ��һ������TCP��������
    hServerHandle = HPP_StartLocalServer
        (
        hHandle,						    //HPP_Init ����ֵ
        _T("0.0.0.0"),				        //���ذ󶨵�IP��ַ
        (HPR_UINT16)Port,	                //���ؼ����Ķ˿�
        OnAcceptClient,					    //���յ����ӵĻص�
        ConnSession::PROTOCOL_TYPE_HPP	    //ʹ��HPPЭ��
        );	

    if(NULL == hServerHandle)   //���ط��񴴽�ʧ��
    {
        SVCMANAGERLOG_INFO(_T("ReStartLocalServer failed. HPP_StartLocalServer failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("ReStartLocalServer end."));
    return TRUE;
}

/** @fn       WrapIf* ServerCreateMessageByCmdId(HPR_INT32 cmdId)
 *  @brief    ���ؼ�������ı������ɻص�����
 *  @param    (HPR_INT32) cmdId [IN] : ����ID
 *  @return:  WrapIf*
 */
WrapIf* ServerCreateMessageByCmdId(HPR_INT32 cmdId)
{
    WrapIf* pWrapIf = NULL;

    switch (cmdId)
    {
    case cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_REQ:
        {
            pWrapIf = PBWrapFactory<cms_8100::ServiceWatchDogControlReq>::CreatePBWrap();
        }
        break;
    default:
        pWrapIf = NULL;
    }

    return pWrapIf;
}

/** @fn       HPR_INT32 OnServerConnectionClosed(ConnSession* p_conn_session_)
 *  @brief    ���ӶϿ�(Socket�Ͽ�)ʱ�Ļص����� 
 *  @param
 *  @return:  HPR_INT32
 */
HPR_INT32 OnServerConnectionClosed(ConnSession* p_conn_session_)
{
    if (p_conn_session_ != NULL)
    {
        p_conn_session_->StopService();
    }

    return HPR_OK;
}

/** @fn       HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_)
 *  @brief    ���ӳ�ʱ�ص�����
 *  @param
 *  @return:  HPR_BOOL
 */
HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_)
{
    SVCMANAGERLOG_WARN(_T("OnServerCheckTimeout..."));

    // do something

    return HPR_TRUE;
}

/** @fn       HPR_INT32 OnAcceptClient(ConnSession* p_conn_session_)
 *  @brief    ���տͻ������ӵĻص�����
 *  @param
 *  @return:  HPR_INT32
 */
HPR_INT32 OnAcceptClient(ConnSession* p_conn_session_)
{
    if (NULL == p_conn_session_ )
    {
        return HPR_ERROR;
    }

    //���ûص�����������
    p_conn_session_->SetServiceType(0x1020);
    p_conn_session_->SetSocketClosedCallBack(OnServerConnectionClosed);
    p_conn_session_->SetTimeOutCallBack(OnServerCheckTimeout);
    if(p_conn_session_->StartService(SERVICE_NORMAL_PRI) != HPR_OK)
    {
        SVCMANAGERLOG_ERROR(_T("OnAcceptClient. StartService failed."));
        return HPR_ERROR;
    }

    return HPR_OK;
}


///////////////////////////////////////////////////////////////
// �����Ĺ��ܺ���

/** @fn       BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
 *  @brief    ��ȡ������IP��ַ
 *  @param    (CString &) csIPAddr [OUT] : ���ص�IP��ַ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
//BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
//{
//     WORD      wVersionRequested;
//     WSADATA   wsaData;
//     char      name[255];
//     PHOSTENT  hostinfo;
// 
//     wVersionRequested = MAKEWORD(2,0);
//     if(WSAStartup(wVersionRequested,&wsaData) == 0)
//     {
//         if(gethostname(name,sizeof(name)) == 0)
//         {   
//             if((hostinfo = gethostbyname(name)) != NULL)
//             {
//                 csIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
//             }
//         }
//         WSACleanup();
//     }
// 
//     if (!csIPAddr.IsEmpty())
//     {
//         return TRUE;
//     }
//
//    return FALSE;
//}

/** @fn       BOOL CloseHPPHandle(HPP_HANDLE &hHandle)
 *  @brief    �ر�HPPЭ��ջ
 *  @param    (HPP_HANDLE &) hHandle [IN] : HPP_Init�ķ���ֵ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CloseHPPHandle(HPP_HANDLE &hHandle)
{
    if (hHandle != NULL)
    {
        HPP_DelPbMsgCreateFun(hHandle, DCClientCreateMessageByCmdId);
        HPP_Close(hHandle);
        hHandle = NULL;
    }

    return TRUE;
}

/** @fn       BOOL CloseHPPHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle)
 *  @brief    ֹͣ���ؼ������񼰹ر�HPPЭ��ջ
 *  @param    (HPP_HANDLE &) hHandle : HPP_Init�ķ���ֵ
 *            (HPP_SERVER_HANDLE &) hServerHandle : HPP_StartLocalServer�ķ���ֵ
 *  @return:  BOOL��TRUE: ; FALSE:
 */
BOOL CloseHPPHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle)
{
    if (hHandle != NULL)
    {
        if (hServerHandle != NULL)
        {
            HPP_StopLocalServer(hHandle, hServerHandle);
            hServerHandle = NULL;
        }

        HPP_DelPbMsgCreateFun(hHandle, ServerCreateMessageByCmdId);
        HPP_Close(hHandle);

        hHandle = NULL;
    }

    return TRUE;
}

