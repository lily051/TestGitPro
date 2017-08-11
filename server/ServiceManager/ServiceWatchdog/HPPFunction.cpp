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
// 连接CMS服务器

/** @fn       BOOL InitCMSClientHppHandle(HPP_HANDLE &hHandle, const CString& strServerIP, int nServerPort)
 *  @brief    初始化与CMS之间的HPP协议栈及连接
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init的返回值
 *            (const CString&) strServerIP [IN] : CMS服务器IP地址
 *            (int) nServerPort [IN] : CMS服务器的端口
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
        // 初始化HPP库,使用两个工作线程,设置报文上下文超时时间为10秒
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("InitCMSClientHppHandle failed. HPP_Init return NULL."));
            return FALSE;
        }

        // 设置HPP(Protocol Buffer协议)报文生成回调函数
        HPP_SetPbMsgCreateFun(hHandle, DCClientCreateMessageByCmdId);
    }

    // 日志
    SVCMANAGERLOG_INFO(_T("InitCMSClientHppHandle:ServerIP[%s], ServerPort[%d]."), (LPCTSTR)strServerIP, nServerPort); 

    // 与CMS直接的会话
    if (g_pCMSConnSession != NULL)
    {
        g_pCMSConnSession->StopService();
        g_pCMSConnSession = NULL;
    }

    // 阻塞模式连接
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
 *  @brief    从新建立与DC之间的连接
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init的返回值
 *            (const CString&) strServerIP [IN] : CMS服务器IP地址
 *            (int) nServerPort [IN] : CMS服务器的端口
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
        // 初始化HPP库,使用两个工作线程,设置报文上下文超时时间为10秒
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("ReConnDCServer failed. HPP_Init return NULL."));
            return FALSE;
        }

        // 设置HPP(Protocol Buffer协议)报文生成回调函数
        HPP_SetPbMsgCreateFun(hHandle, DCClientCreateMessageByCmdId);
    }

    // 日志
    SVCMANAGERLOG_INFO(_T("ReConnDCServer:ServerIP[%s], ServerPort[%d]."), (LPCTSTR)strServerIP, nServerPort); 

    // 与DC直接的会话
    if (g_pCMSConnSession != NULL)
    {
        g_pCMSConnSession->StopService();
        g_pCMSConnSession = NULL;
    }

    // 阻塞模式连接
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
 *  @brief    报文生成回调函数
 *  @param    (HPR_INT32) cmdId [IN] : 命令ID
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
 *  @brief    连接断开时(Socket断开)时的回调函数
 *  @param
 *  @return:  HPR_INT32：HPR_OK:断开成功 ; HPR_ERROR:断开失败
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
 *  @brief    连接超时回调函数
 *  @param
 *  @return:  HPR_BOOL：HPR_OK:处理成功 ; HPR_ERROR:处理失败
 */
HPR_BOOL OnClientCheckTimeout(ConnSession* p_conn_session_)
{
    // do something
    SVCMANAGERLOG_WARN(_T("OnClientCheckTimeout..."));

    return HPR_FALSE;
}

/** @fn       HPR_INT32 OnClientConnectionComplete(ConnSession* p_conn_session_)
 *  @brief    连接回调函数
 *  @param    
 *  @return:  HPR_INT32：HPR_OK:连接成功 ; HPR_ERROR:连接失败
 */
HPR_INT32 OnClientConnectionComplete(ConnSession* p_conn_session_)
{
    if (NULL == p_conn_session_ )
    {
        return HPR_ERROR;
    }

    //设置回调，启动服务
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
// 本地监听服务

/** @fn       BOOL InitServerHppHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
 *  @brief    初始化本地监听服务及其协议栈
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init的返回值
 *            (HPP_SERVER_HANDLE &) hServerHandle [OUT] : HPP_StartLocalServer的返回值
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
        // 初始化HPP库,使用两个工作线程,设置报文上下文超时时间为10秒
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("InitServerHppHandle failed. HPP_Init return NULL."));
            return FALSE;
        }

        // 设置HPP(Protocol Buffer协议)报文生成回调函数,
        HPP_SetPbMsgCreateFun(hHandle, ServerCreateMessageByCmdId);
    }

    SVCMANAGERLOG_INFO(_T("InitServerHppHandle: LocalIP[%s], ListenPort[%d]"), (LPCTSTR)theApp.m_hostIP, Port);

    //初始化一个本地TCP监听服务
    hServerHandle = HPP_StartLocalServer
        (
        hHandle,						    //HPP_Init 返回值
        _T("0.0.0.0"),				        //本地绑定的IP地址
        (HPR_UINT16)Port,	                //本地监听的端口
        OnAcceptClient,					    //接收到连接的回调
        ConnSession::PROTOCOL_TYPE_HPP	    //使用HPP协议
        );	

    if(NULL == hServerHandle)   //本地服务创建失败
    {
        SVCMANAGERLOG_INFO(_T("InitServerHpp failed. HPP_StartLocalServer failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("InitServerHpp end."));
    return TRUE;
}

/** @fn       BOOL ReStartLocalServer(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port)
 *  @brief    重新启动本地监听服务
 *  @param    (HPP_HANDLE &) hHandle [OUT] : HPP_Init的返回值
 *            (HPP_SERVER_HANDLE &) hServerHandle [OUT] : HPP_StartLocalServer的返回值
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
        // 初始化HPP库,使用两个工作线程,设置报文上下文超时时间为10秒
        hHandle = HPP_Init(2, 10);
        if (NULL == hHandle)
        {
            SVCMANAGERLOG_ERROR(_T("ReStartLocalServer failed. HPP_Init return NULL."));
            return FALSE;
        }

        // 设置HPP(Protocol Buffer协议)报文生成回调函数,
        HPP_SetPbMsgCreateFun(hHandle, ServerCreateMessageByCmdId);
    }

    SVCMANAGERLOG_INFO(_T("ReStartLocalServer: LocalIP[%s], ListenPort[%d]"), (LPCTSTR)theApp.m_hostIP, Port);

    if (hServerHandle != NULL)
    {
        HPP_StopLocalServer(hHandle, hServerHandle);
        hServerHandle = NULL;
    }

    //初始化一个本地TCP监听服务
    hServerHandle = HPP_StartLocalServer
        (
        hHandle,						    //HPP_Init 返回值
        _T("0.0.0.0"),				        //本地绑定的IP地址
        (HPR_UINT16)Port,	                //本地监听的端口
        OnAcceptClient,					    //接收到连接的回调
        ConnSession::PROTOCOL_TYPE_HPP	    //使用HPP协议
        );	

    if(NULL == hServerHandle)   //本地服务创建失败
    {
        SVCMANAGERLOG_INFO(_T("ReStartLocalServer failed. HPP_StartLocalServer failed."));
        return FALSE;
    }

    SVCMANAGERLOG_INFO(_T("ReStartLocalServer end."));
    return TRUE;
}

/** @fn       WrapIf* ServerCreateMessageByCmdId(HPR_INT32 cmdId)
 *  @brief    本地监听服务的报文生成回调函数
 *  @param    (HPR_INT32) cmdId [IN] : 命令ID
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
 *  @brief    连接断开(Socket断开)时的回调函数 
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
 *  @brief    连接超时回调函数
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
 *  @brief    接收客户端连接的回调函数
 *  @param
 *  @return:  HPR_INT32
 */
HPR_INT32 OnAcceptClient(ConnSession* p_conn_session_)
{
    if (NULL == p_conn_session_ )
    {
        return HPR_ERROR;
    }

    //设置回调，启动服务
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
// 其他的功能函数

/** @fn       BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
 *  @brief    获取本机的IP地址
 *  @param    (CString &) csIPAddr [OUT] : 本地的IP地址
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
 *  @brief    关闭HPP协议栈
 *  @param    (HPP_HANDLE &) hHandle [IN] : HPP_Init的返回值
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
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
 *  @brief    停止本地监听服务及关闭HPP协议栈
 *  @param    (HPP_HANDLE &) hHandle : HPP_Init的返回值
 *            (HPP_SERVER_HANDLE &) hServerHandle : HPP_StartLocalServer的返回值
 *  @return:  BOOL：TRUE: ; FALSE:
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

