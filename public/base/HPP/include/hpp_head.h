#ifndef _HPP_HEAD_H_
#define _HPP_HEAD_H_
#include <string>
#include "hpp_common.h"

class HPP_DECLARE_CLASS HPP_PACK_HEAD
{
public:
    HPR_BOOL InitFromBuffer(void* pBuffer);
    HPR_VOID SaveHeadToBuffer(void* pBuffer);

    HPR_UINT32 m_nMagic;
    HPR_UINT32 m_nSequence;
    HPR_INT32 m_nLength;
    HPR_UINT32 m_nMark;                 /*有请求或响应参数标记  */
    HPR_UINT8 m_cVersion;
    HPR_UINT8 m_cCompress;
    HPR_UINT16 m_nService;
    HPR_UINT32 m_nCommand;
    HPR_UINT32 m_nReserve;

    static const HPR_UINT32 REQ_MSG_NEED_RSP = 0x00008000;
    static const HPR_UINT32 REQ_MSG_NOT_NEED_RSP = 0x00008001;
    static const HPR_UINT32 RSP_MSG_FINISH = 0x00008080;
    static const HPR_UINT32 RSP_MSG_NOT_FINISH = 0x00008081;        /* 表示应答未结束，仍有后续包 */

    static const HPR_INT32 nHEAD_LEN = 28;
    HPP_PACK_HEAD(void);
    ~HPP_PACK_HEAD(void);
};


class HPP_DECLARE_CLASS HPP_EHOME_PACK_HEAD
{
public:
    HPP_EHOME_PACK_HEAD();
    HPR_BOOL InitFromBuffer(void* pBuffer);
    HPR_VOID SaveHeadToBuffer(void* pBuffer);

    HPR_UINT32 m_nMagic;
    HPR_UINT32 m_nSequence;
    HPR_INT32 m_nLength;
    HPR_UINT32 m_nMark;                 /*有请求或响应参数标记  */
    HPR_UINT8 m_cVersion;
    HPR_UINT8 m_cCompress;
    HPR_UINT16 m_nService;

    std::string sCommandName;
    static const HPR_UINT32 REQ_MSG_NEED_RSP = 0x00008000;
    static const HPR_UINT32 REQ_MSG_NOT_NEED_RSP = 0x00008001;
    static const HPR_UINT32 RSP_MSG_FINISH = 0x00008080;
    static const HPR_UINT32 RSP_MSG_NOT_FINISH = 0x00008081;        /* 表示应答未结束，仍有后续包 */

    static const HPR_INT32 nHEAD_LEN = 20;
};
#endif


