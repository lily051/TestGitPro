#pragma once

#include "Thread/WorkItem.h"
#include <hpp_hpp.h>

//////////////////////////////////////////////////////////////////////////
// FiPbWrapWorkItem
class FiPbWrapWorkItem : public WorkItem
{
public:
    FiPbWrapWorkItem(void);
    virtual ~FiPbWrapWorkItem(void);

    string m_sMsgSrc;
    HPR_UINT32 m_nSeq;
    HPR_INT32 m_nSrcID;
    HPR_UINT32 m_nCommand;
    HPR_UINT32 m_nMark;

    virtual BOOL UnderTake(void);
};
