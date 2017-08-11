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
*   @brief ����PBWrap 
*   @param[in] nCmdID: ������Ϣ������
*   @param[in] nSeq: ��Ϣ���к�
*   @param[in] nServerID: �����ID
*   @param[in] msgType: ��Ϣ����
*   @param[in] msgReq: ������Ϣ����
*   @return NULL
*/
FiPBWrap* CreateWrapEx( HPR_INT32 nCmdID
                  , int nSeq
                  , int nServerID
                  , HPR_UINT32 msgType
                  , string& msgReq);

/** @fn DoExecute
*   @brief Ĭ��ִ�к���
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
