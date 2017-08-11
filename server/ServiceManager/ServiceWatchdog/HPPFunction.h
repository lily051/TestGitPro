/**
*   @file   HPPFunction.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  HPP����
*  
*   @author mazhentong
*   @date   2012/10/29
*
*   @note   ��ʷ��¼
*           2012/10/29  ����
*
*   @warning ��Ҫ����HPP��Ŀ
*/
#pragma once

#ifndef _HPP_FUNCTION_H_
#define _HPP_FUNCTION_H_

#include "MainDialog.h"
#include "ServiceWatchdog.h"

#include <tables/ServerMonitorInfo.pb.h>
#include <MSG/ServiceWatchDogControl.pb.h>
#include <type/cmd.pb.h>
#include <tables/DBInfo.pb.h>
#include <Server/ReqServerData.pb.h>
#include <tables/WorkState.pb.h>

#include <time.h>
#include <hpp_hpp.h>
#include "./fclib/fcTchar.h"

#include <string>
using namespace std;
////////////////////////////////////////////////////////////////////////////////////////
//�������ĵĿͻ���HPP���ܺ���������

// ��ʼ���������ĵķ�����Ϣ��ClientHandle
BOOL InitCMSClientHppHandle(HPP_HANDLE &hHandle, const CString& strServerIP, int nServerPort);

// ���������������ķ���
BOOL ReConnDCServer(HPP_HANDLE& hHandle, const CString& strServerIP, int nServerPort);

// �������ĵĿͻ��˵ı������ɻص�����
WrapIf* DCClientCreateMessageByCmdId(HPR_INT32 cmdId);

// �������ĵĿͻ��˵����ӹرջص�����
HPR_INT32 OnClientConnectionClosed(ConnSession* p_conn_session_);

// �������ĵĿͻ��˵�ʱ�䳬ʱ�ص�����
HPR_BOOL OnClientCheckTimeout(ConnSession* p_conn_session_);

// �������ĵĿͻ��˵����ӻص�
HPR_INT32 OnClientConnectionComplete(ConnSession* p_conn_session_);

// // Ĭ��ִ�к���
// template<> inline WrapIf* PBWrap<cms_8100::ServerMonitorInfo>::DoExecute()
// {
//     // do something
// 
//     return NULL;
// }

// DBInfoĬ��ִ�к���
template<> inline WrapIf* PBWrap<cms_8100::DBInfo>::DoExecute()
{
    cms_8100::DBInfo & r_msg = GetMsg();

    SVCMANAGERLOG_DEBUG("DBInfo:%s", r_msg.Utf8DebugString().c_str());

    std::string sTime = /*CSC::Utf82Ansi(*/r_msg.db_cur_time()/*)*/;
    CString csTime = sTime.c_str();
    COleDateTime oleDT;
    if (oleDT.ParseDateTime(csTime))
    {
        SYSTEMTIME systemTime;
        if (oleDT.GetAsSystemTime(systemTime))
        {
            if (!SetLocalTime(&systemTime))
            {
                SVCMANAGERLOG_ERROR("SetLocalTime fail!");
            }
        }
        else
        {
            SVCMANAGERLOG_ERROR("GetAsSystemTime fail!");
        }
    }
    else
    {
        SVCMANAGERLOG_ERROR("ParseDateTime fail!");
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////
// ServerWatchDog��������HPP���ܺ���������

// ��ʼ�����Ƽ�������
BOOL InitServerHppHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port);

// ���¿�ʼ���ط���
BOOL ReStartLocalServer(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle, int Port);

// ���Ʒ��������ɻص�����
WrapIf* ServerCreateMessageByCmdId(HPR_INT32 cmdId);

// CMS�Ŀͻ��˵����ӹرջص�����
HPR_INT32 OnServerConnectionClosed(ConnSession* p_conn_session_);

// CMS�Ŀͻ��˵�ʱ�䳬ʱ�ص�����
HPR_BOOL OnServerCheckTimeout(ConnSession* p_conn_session_);

// CMS�Ŀͻ��˵����ӻص�
HPR_INT32 OnAcceptClient(ConnSession* p_conn_session_);

// ServiceWatchDogControlReqĬ��ִ�к���
template<> inline WrapIf* PBWrap<cms_8100::ServiceWatchDogControlReq>::DoExecute()
{
    cms_8100::ServiceWatchDogControlReq& req_msg = GetMsg();
    HPR_INT32 Datatype = req_msg.data_type();

    if (Datatype != cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_REQ)
    {
        SVCMANAGERLOG_DEBUG(_T("PBWrap<cms_8100::ServiceWatchDogControlReq>::DoExecute: Datatype != cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_REQ."));
        return NULL;
    }

    HPR_INT32 ControlType = req_msg.control_type();
    HPR_INT32 ModuleType = req_msg.module_type();
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
        PBWrap<cms_8100::ServiceWatchDogControlRsp>* p_rsp_wrap = 
            PBWrapFactory<cms_8100::ServiceWatchDogControlRsp>::CreatePBWrap();

        if (p_rsp_wrap != NULL)
        {
            p_rsp_wrap->SetCommandId(cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_RSP);
            p_rsp_wrap->SetInnerSequence(GetInnerSequence());
            p_rsp_wrap->SetMessageType(HPP_PACK_HEAD::RSP_MSG_FINISH);
            p_rsp_wrap->SetWrapDstId(GetWrapSrcId());

            cms_8100::ServiceWatchDogControlRsp & p_rsp_msg = p_rsp_wrap->GetMsg();
            p_rsp_msg.set_data_type(cms_8100::CMD_SERVICE_WATCH_DOG_CONTROL_RSP);
            p_rsp_msg.set_result(1);
            p_rsp_msg.set_control_type(ControlType);

            //������Ӧ��
            if (HPR_OK != ConnSession::SendMessage_r(p_rsp_wrap, GetWrapSrcId()))
            {
                SVCMANAGERLOG_ERROR(_T("ServiceWatchDogControlRsp SendMessage failed."));

                PBWrapFactory<cms_8100::ServiceWatchDogControlRsp>::DestroyPBWrap(p_rsp_wrap);
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

//////////////////////////////////////////////////////////////////////////////////////////
//

// ��ȡ���ص�IP��ַ
//BOOL AppFun_GetLocalIPAddr(CString & csIPAddr);

// �ر�HPP�ľ��
BOOL CloseHPPHandle(HPP_HANDLE &hHandle);

// �ر�HPP�ľ����������
BOOL CloseHPPHandle(HPP_HANDLE &hHandle, HPP_SERVER_HANDLE &hServerHandle);

#endif
