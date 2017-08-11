#ifndef _HPP_WRAP_EH_H_
#define _HPP_WRAP_EH_H_

#include <iostream>
#include <google/protobuf/text_format.h>
#include "hpp_common.h"
#include "hpp_wrap.h"

template <class PBMSG>
class EhWrap:public WrapEhIf
{
public: 
    /************************** Implement Of Base Class *************************************/
    inline WrapEhIf* DoExecute();                       /*处理信令逻辑，由逻辑层实现*/
    inline HPR_INT32 DoTimeout();

    inline HPR_INT32 GetWrapSrcId();                        /*信令来源*/
    inline HPR_INT32 GetWrapDstId();                        /*信令目的*/
    inline HPR_VOID SetWrapSrcId(HPR_INT32 srcId);          /*信令来源*/
    inline HPR_VOID SetWrapDstId(HPR_INT32 dstId);          /*信令目的*/

    inline HPR_UINT32 GetInnerSequence();                   /*获取内部序号*/
    inline HPR_VOID SetInnerSequence(HPR_UINT32 seq);       /*设置内部序号*/
    inline WrapEhIf* GetOrigRequest();                      /*获取相关的请求报文，失败返回NULL*/
    inline HPR_VOID SetOrigRequest(WrapEhIf* pReq);

    inline HPR_VOID SetMessageType(HPR_INT32 iMsgType);
    inline HPR_INT32 FillWithData(HPP_EHOME_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen);
    inline HPR_INT32 GetContentLen();                                       /*信令内容长度*/
    inline HPR_VOID GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen);
    virtual HPR_VOID PrintToString(std::string* string);                    /*信令内容格式化到字符串以方便调试*/

    inline HPP_EHOME_PACK_HEAD GetPackHead();                               /*获取协议头*/
    inline HPR_VOID SetPackHead(HPP_EHOME_PACK_HEAD head);                  /*设置协议头*/
    inline HPR_BOOL IsWrapFromNetwork();
    inline HPR_VOID SetWrapFromNetwork(HPR_BOOL bFrom);

    inline HPR_VOID SetSendBuffer(HPR_INT8* pBuffer);
    inline HPR_INT8* GetSendBuffer();

    inline HPR_VOID SetDstAddr(HPR_ADDR_T iAddr);
    inline HPR_ADDR_T* GetDstAddrPtr();
    inline std::string GetLogName();
    inline HPR_VOID SetLogName(const std::string& sLogName);

    inline HPR_VOID SetWrapMarkStr(std::string sCommandName);
    inline std::string GetWrapMarkStr();

    inline HPR_BOOL IsTimeoutRspWrap();
    inline HPR_VOID SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp);

    /**************************** Other Function/Method Of This Class ***************************/
    inline PBMSG& GetMsg();
    EhWrap(void);
    ~EhWrap(void);

private:
    HPR_INT8* m_pSendBuffer;
    HPP_EHOME_PACK_HEAD m_iHead;
    PBMSG m_iMsg;
    HPR_INT32 m_nSrcId;                                     /*信令来源*/
    HPR_INT32 m_nDstId;                                     /*信令目的*/
    HPR_UINT32 m_nInnerSequence;
    HPR_BOOL m_bFromNetWork;
    WrapEhIf* m_OrigRequest;                                    /*响应报文对应的请求报文，如果是请求报文，该接口返回NULL*/
    HPR_INT8 m_sContent[4096];
    HPR_INT32 m_nContentLen;
    HPR_ADDR_T m_iAddr;
    std::string m_sPuid;

    HPR_BOOL m_bTimeoutRsp;
};

template <class PBMSG>
inline HPR_BOOL EhWrap<PBMSG>::IsTimeoutRspWrap()
{
    return m_bTimeoutRsp;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp)
{
    m_bTimeoutRsp = bTimeoutRsp;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetWrapMarkStr(std::string sCommandName)
{
    m_iHead.sCommandName = sCommandName;
}

template <class PBMSG>
inline std::string EhWrap<PBMSG>::GetWrapMarkStr()
{
    return m_iHead.sCommandName;
}
template <class PBMSG>
inline std::string EhWrap<PBMSG>::GetLogName()
{
    return m_sPuid;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetLogName(const std::string& sPuid)
{
    m_sPuid = sPuid;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetDstAddr(HPR_ADDR_T iAddr)
{
    m_iAddr = iAddr;
}

template <class PBMSG>
inline HPR_ADDR_T* EhWrap<PBMSG>::GetDstAddrPtr()
{
    return &m_iAddr;
}

template <class PBMSG>
inline HPR_BOOL EhWrap<PBMSG>::IsWrapFromNetwork()
{
    return m_bFromNetWork;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetSendBuffer(HPR_INT8* pBuffer)
{
    m_pSendBuffer = pBuffer;
}
template <class PBMSG>
inline HPR_INT8* EhWrap<PBMSG>::GetSendBuffer()
{
    return m_pSendBuffer;
}
template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetWrapFromNetwork(HPR_BOOL bFrom)
{
    m_bFromNetWork = bFrom;
}

template <class PBMSG>
inline WrapEhIf* EhWrap<PBMSG>::GetOrigRequest()
{
    return m_OrigRequest;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetOrigRequest(WrapEhIf* pReq)
{
    m_OrigRequest = pReq;
}


template <class PBMSG>
inline EhWrap<PBMSG>::EhWrap(void)
{
    m_iHead.m_nMagic = HEADER_FLAG_;
    m_iHead.m_cVersion = 0x1;
    m_iHead.m_cCompress = 0;
    m_iHead.m_nService = 0;

    m_bFromNetWork = HPR_FALSE;
    m_pSendBuffer = NULL;
    m_nSrcId = -1;
    m_nDstId = -1;
    m_nInnerSequence = 0;
    m_OrigRequest = NULL;
    m_nContentLen = -1;
    m_bTimeoutRsp = HPR_FALSE;
    m_iHead.sCommandName = "";
}

template <class PBMSG>
inline EhWrap<PBMSG>::~EhWrap(void)
{
}



template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetMessageType(HPR_INT32 iMsgType)
{
    m_iHead.m_nMark = iMsgType;
}
/** @fn FillWithData
\brief 根据二进制数据填充报文
\param data 二进制数据
\param datalen 二进制数据的长度
\return 填充成功返回HPR_OK 失败返回HPR_ERROR
*/
template <class PBMSG>
inline HPR_INT32 EhWrap<PBMSG>::FillWithData(HPP_EHOME_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen)
{
    m_nContentLen = min(datalen, 4095);
    strncpy((char*)m_sContent, (char*)data, m_nContentLen);
    m_sContent[min(datalen, 4095)] = 0;
    if (m_iMsg.ParseCharArray((char*)data))
    {
        m_iHead = head;
        m_iHead.m_nSequence = m_iMsg.GetHead().get_sequence();
        m_iHead.m_nMark = m_iMsg.GetHead().get_commandtype() == "REQUEST" ? HPP_EHOME_PACK_HEAD::REQ_MSG_NEED_RSP : HPP_EHOME_PACK_HEAD::RSP_MSG_FINISH;
        return HPR_OK;
    }

    return HPR_ERROR;
}

/** @fn GetContentLen
*   @brief 获取数据报长度，包括HPP头及HPP尾长度
*   @return 数据报长度
*/
template <class PBMSG>
inline HPR_INT32 EhWrap<PBMSG>::GetContentLen()
{
    m_nContentLen = m_iMsg.GetCharArray((char*)m_sContent, sizeof(m_sContent));
    return m_nContentLen + HPP_EHOME_PACK_HEAD::nHEAD_LEN;
}

/** @fn GetContent
*   @brief 获取数据报内容，包括HPP头及HPP尾长度
*   @param [out]buffer 传出数据报内容
*   @param bufferLen 传入可用buffer长度
*/
template <class PBMSG>
HPR_VOID EhWrap<PBMSG>::GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen)
{
    if (m_nContentLen <= 0)
    {
        m_nContentLen = m_iMsg.GetCharArray((char*)m_sContent, sizeof(m_sContent));
    }

    if (m_iMsg.GetHead().get_commandtype() == "REQUEST")
    {
        m_iHead.m_nMark = HPP_EHOME_PACK_HEAD::REQ_MSG_NEED_RSP;
    }
    else
    {
        m_iHead.m_nMark = HPP_EHOME_PACK_HEAD::RSP_MSG_FINISH;
    }

    m_iHead.m_nSequence = m_iMsg.GetHead().get_sequence();
    m_iHead.m_nLength = m_nContentLen + HPP_EHOME_PACK_HEAD::nHEAD_LEN;
    m_iHead.SaveHeadToBuffer(buffer);

    memcpy(buffer + HPP_EHOME_PACK_HEAD::nHEAD_LEN, m_sContent, m_nContentLen);
}

/** @fn DoExecute
\brief 默认执行函数
\return HPR_OK;
*/
template <class PBMSG>
inline WrapEhIf* EhWrap<PBMSG>::DoExecute()
{
    //  std::string className = std::string(typeid(m_iMsg).name()).substr(6);
    //  HPP_DEBUG("Please complete DoExecute() function for %s message", className.c_str());
    // 
    //  std::string str;
    //  PrintToString(&str);
    //  HPP_DEBUG("\nRecv message %s", className.c_str());
    return NULL;
}

/** @fn DoTimeout
*   @brief 默认超时执行函数 
*   @return HPR_OK
*/
template <class PBMSG>
inline HPR_INT32 EhWrap<PBMSG>::DoTimeout()
{
    //  std::string className = std::string(typeid(m_iMsg).name()).substr(6);
    //  HPP_DEBUG("Please complete DoTimeout for %s message", className.c_str());
    return 0;
}

/** @fn PrintToString
*   @brief 格式化输出到字符串
*   @param [out]output 输出字符串
*/
template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::PrintToString(std::string* output)
{
    m_iHead.m_nMagic = HEADER_FLAG_;
    m_iHead.m_cVersion = 0x1;
    m_iHead.m_cCompress = 0;
    m_iHead.m_nService = 0;

    std::stringstream ss;
    ss << "Ehome message: \n";
    ss << "Head Sequence: " << m_iHead.m_nSequence << "\n";
    ss << "Message Len: " << m_iHead.m_nLength << "\n";
    ss << "Message Content: " << m_sContent << "\n";
    ss << "Message From/Dst" << HPR_GetAddrString(&m_iAddr) << ":" << HPR_GetAddrPort(&m_iAddr) << std::endl;

    *output = ss.str();
}





/******************************* Getter and Setter*******************************/
template <class PBMSG>
inline HPR_INT32 EhWrap<PBMSG>::GetWrapSrcId()
{
    return m_nSrcId;
}


template <class PBMSG>
inline HPR_INT32 EhWrap<PBMSG>::GetWrapDstId()
{
    return m_nDstId;
}


template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetWrapSrcId(HPR_INT32 srcId)            /*信令来源*/
{
    this->m_nSrcId = srcId;
}


template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetWrapDstId(HPR_INT32 dstId)            /*信令目的*/
{
    this->m_nDstId = dstId;
}


template <class PBMSG>
inline HPR_UINT32 EhWrap<PBMSG>::GetInnerSequence()
{
    return m_nInnerSequence;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetInnerSequence(HPR_UINT32 nInnerSequence)
{
    m_nInnerSequence = nInnerSequence;
}

template <class PBMSG>
inline HPP_EHOME_PACK_HEAD EhWrap<PBMSG>::GetPackHead()
{
    return m_iHead;
}

template <class PBMSG>
inline HPR_VOID EhWrap<PBMSG>::SetPackHead(HPP_EHOME_PACK_HEAD iHead)
{
    m_iHead = iHead;
}

template <class PBMSG>
inline PBMSG& EhWrap<PBMSG>::GetMsg()
{
    return m_iMsg;
}


#endif
