#pragma once

// #include <verify/ReqClientLogin.pb.h>
// #include <verify/RspClientLogin.pb.h>
#include <verify/ClientVerify.pb.h>
#include <type/cmd.pb.h>
#include <type/ModuleType.pb.h>
#include <Server/ReqServerData.pb.h>
#include <Server/RspServerData.pb.h>
//#include <msg/MsgAction.pb.h>
#include <verify/MsgClientLogout.pb.h>
#include "PbWrapWorkItem.h"
#include "PbMsgDispatcher.h"
#include "RspDispatcher.h"
#include "hpp_wrap_pb_fi.h"

WrapIf * CreateMsgByCmdIdEx(HPR_INT32 cmdID);

/** @fn CreateWrapEx
*   @brief 创建PBWrap 
*   @param[in] nCmdID: 发送消息命令码
*   @param[in] nSeq: 消息序列号
*   @param[in] nServerID: 服务端ID
*   @param[in] msgType: 消息类型
*   @param[in] msgReq: 请求消息正文
*   @return NULL
*/
FiPBWrap* CreateWrapEx( HPR_INT32 nCmdID
                  , int nSeq
                  , int nServerID
                  , HPR_UINT32 msgType
                  , string& msgReq);

/** @fn DoExecute
*   @brief 默认执行函数
*   @return HPR_OK;
*/
//lint -e438
inline WrapIf* FiPBWrap::DoExecute()
{
    //string& msgSrc = GetMsg();
    const HPP_PACK_HEAD stHead = GetPackHead();
    const HPR_UINT32 nSeq = stHead.m_nSequence;
    const HPR_INT32 nSrcID = GetWrapSrcId();
    const HPR_UINT32 nCommand = stHead.m_nCommand;
    const HPR_UINT32 nMark = stHead.m_nMark;

    // Schedule WorkItem
    FiPbWrapWorkItem* pWorkItem = new FiPbWrapWorkItem;
    pWorkItem->m_sMsgSrc = this->GetMsg();
    pWorkItem->m_nCommand = nCommand;
    pWorkItem->m_nMark = nMark;
    pWorkItem->m_nSeq = nSeq;
    pWorkItem->m_nSrcID = nSrcID;
    BOOL bRet = FALSE;
    switch (nMark)
    {
    case HPP_PACK_HEAD::RSP_MSG_FINISH:
    case HPP_PACK_HEAD::REQ_MSG_NEED_RSP:
        bRet = RspDispatcher::SnglPtr()->PostWorkItem(pWorkItem);
        break;
    default:
        bRet = PbMsgDispatcher::SnglPtr()->PostWorkItem(pWorkItem);
        break;
    }

    if (!bRet)
    {
        delete pWorkItem;
        pWorkItem = NULL;
    }

    return NULL;
}
//lint +e438
