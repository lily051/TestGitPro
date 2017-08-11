#ifndef __HPP_TRANSFERABLE_H__
#define __HPP_TRANSFERABLE_H__

class HPP_DECLARE_CLASS HPPTransferable
{
public:
    virtual ~HPPTransferable()
    {
    }
    HPR_INT32 m_nSrcId;
    HPR_INT32 m_nDstId;
    HPR_UINT32 m_nSequence;
};

#endif

