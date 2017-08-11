#include "stdafx.h"
#include "PbWrapRelative.h"
#include "hpp_wrap_pb_fi.h"


WrapIf * CreateMsgByCmdIdEx(HPR_INT32 cmdID)
{
    FiPBWrap* pWrap = new (std::nothrow) FiPBWrap;
    if (pWrap == NULL)
    {
        LOG_HPP_NO_MEM_ERROR;
        return NULL;
    }
    return pWrap;
}

FiPBWrap* CreateWrapEx(HPR_INT32 nCmdID
                  , int nSeq
                  , int nServerID
                  , HPR_UINT32 msgType
                  , string& msgReq)
{
    //创建报文对象
    FiPBWrap* pWrap = new (std::nothrow) FiPBWrap;
    if (pWrap)
    {
        pWrap->SetCommandId(nCmdID);
        pWrap->SetInnerSequence(nSeq);
        pWrap->SetMessageType(msgType);
        pWrap->SetWrapDstId(nServerID);
        string& msg = pWrap->GetMsg();
        msg.assign(msgReq);
    }    
    return pWrap;
}


