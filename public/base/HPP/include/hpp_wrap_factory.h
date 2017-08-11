#ifndef _HPP_WRAP_FACTORY_H_
#define _HPP_WRAP_FACTORY_H_

#include "hpp_common.h"
#include "hpp_wrap_pb.h"
//#include "hpp_wrap_eh.h"

template <class PBMSG>
class EhWrap;

template <class PBMSG>
class PBWrapFactory
{
public:
    static PBWrap<PBMSG>* CreatePBWrap();
    static PBWrap<PBMSG>* ClonePbWrap(const PBWrap<PBMSG>* pFrom);
    static inline HPR_VOID DestroyPBWrap(Wrap* pbWrapInstance);
    static EhWrap<PBMSG>* CreateEhWrap();
};

#define EhWrapFactory PBWrapFactory

template <class PBMSG>
inline PBWrap<PBMSG>* PBWrapFactory<PBMSG>::ClonePbWrap(const PBWrap<PBMSG>* pFrom)
{
    PBWrap<PBMSG>* pWrap = new (std::nothrow) PBWrap<PBMSG>(*pFrom);
    if (pWrap == NULL)
    {
        LOG_HPP_NO_MEM_ERROR;
        return NULL;
    }
    return pWrap;
}

template <class PBMSG>
PBWrap<PBMSG>* PBWrapFactory<PBMSG>::CreatePBWrap()
{
    PBWrap<PBMSG>* pWrap = new (std::nothrow) PBWrap<PBMSG>;
    if (pWrap == NULL)
    {
        LOG_HPP_NO_MEM_ERROR;
        return NULL;
    }

    return pWrap;
}

template <class PBMSG>
inline EhWrap<PBMSG>* PBWrapFactory<PBMSG>::CreateEhWrap()
{
    EhWrap<PBMSG>* pWrap = new (std::nothrow) EhWrap<PBMSG>;
    if (pWrap == NULL)
    {
        LOG_HPP_NO_MEM_ERROR;
        return NULL;
    }
    return pWrap;
}

template <class PBMSG>
inline HPR_VOID PBWrapFactory<PBMSG>::DestroyPBWrap(Wrap* pbWrapInstance)
{
    if (pbWrapInstance)
    {
        delete pbWrapInstance;
    }
}
#if 0
template<class PBMSG>
inline HPR_VOID PBWrapFactory<PBMSG>::DestroyPBWrap(WrapEhIf* pbWrapInstance)
{
    if(pbWrapInstance)
    {
        delete pbWrapInstance;
    }
}
#endif



#endif
