#ifndef _HPP_PB_MSG_WRAP_H_
#define _HPP_PB_MSG_WRAP_H_

#include <sstream>
#include <typeinfo>

#if (defined _WIN32 || defined _WIN64)
#pragma warning (push)  ///< disable warning 4244 4267 from google/protobuf/text_format.h
#pragma warning (disable:4244 4267)
#include <google/protobuf/text_format.h>
#pragma warning (pop)   ///< disable warning 4244 4267 from google/protobuf/text_format.h end
#else
#include <google/protobuf/text_format.h>
#endif

#include "hpp_common.h"
#include "hpp_wrap.h"


template <class PBMSG>
class PBWrapFactory;

template <class PBMSG>
class PBWrap :public WrapIf
{
public: 
    /************************** Implement Of Base Class *************************************/
    inline WrapIf* DoExecute();                     /*���������߼������߼���ʵ��*/
    inline HPR_INT32 DoTimeout();

    inline HPR_INT32 GetWrapSrcId();                        /*������Դ*/
    inline HPR_INT32 GetWrapDstId();                        /*����Ŀ��*/
    inline HPR_VOID SetWrapSrcId(HPR_INT32 srcId);          /*������Դ*/
    inline HPR_VOID SetWrapDstId(HPR_INT32 dstId);          /*����Ŀ��*/
    inline PBWrap<PBMSG>* Clone();

    inline HPR_UINT32 GetInnerSequence();               /*��ȡ�ڲ����*/
    inline HPR_VOID SetInnerSequence(HPR_UINT32 seq);       /*�����ڲ����*/
    inline WrapIf* GetOrigRequest();                        /*��ȡ��ص������ģ�ʧ�ܷ���NULL*/
    inline HPR_VOID SetOrigRequest(WrapIf* pReq);

#ifdef PBWRAP_CUSTOMIZE_CAST
    inline void* GetOrigRequestEx();
    inline HPR_VOID SetOrigRequestEx(void* pReq);

#endif

    inline HPR_VOID SetMessageType(HPR_UINT32 iMsgType);
    inline HPR_VOID SetCommandId(HPR_UINT32 nCommandId);
    inline HPR_INT32 FillWithData(HPP_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen);
    inline HPR_INT32 GetContentLen();                                       /*�������ݳ���*/
    inline HPR_VOID GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen);
    virtual HPR_VOID PrintToString(std::string* string);                    /*�������ݸ�ʽ�����ַ����Է������*/

    inline HPP_PACK_HEAD GetPackHead();                 /*��ȡЭ��ͷ*/
    inline HPR_VOID SetPackHead(HPP_PACK_HEAD head);    /*����Э��ͷ*/
    inline HPR_BOOL IsWrapFromNetwork();
    inline HPR_VOID SetWrapFromNetwork(HPR_BOOL bFrom);

    inline HPR_VOID SetSendBuffer(HPR_INT8* pBuffer);
    inline HPR_INT8* GetSendBuffer();

    inline HPR_BOOL IsTimeoutRspWrap();
    inline HPR_VOID SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp);

    inline HPR_VOID SetWrapMarkStr(std::string sCommandName);
    inline std::string GetWrapMarkStr();

    // mq interface
    virtual HPR_VOID SetMqProducerId(int nProducerId);
    virtual int GetMqProducerId();

    virtual HPR_VOID SetMqGroupId(const char* sGroupId);
    virtual std::string& GetMqGroupId();
    virtual HPR_VOID SetMqSendType(HPR_UINT32 nSendType);
    virtual HPR_UINT32 GetMqSendType();
    /**************************** Other Function/Method Of This Class ***************************/
    inline PBMSG& GetMsg();
    PBWrap(void);
    PBWrap(const PBWrap& c);
    ~PBWrap(void);

private:
    HPR_INT8* m_pSendBuffer;
    HPP_PACK_HEAD m_iHead;
    PBMSG m_iMsg;
    HPR_INT32 m_nSrcId;                                     /*������Դ*/
    HPR_INT32 m_nDstId;                                     /*����Ŀ��*/
    HPR_UINT32 m_nInnerSequence;
    HPR_BOOL m_bFromNetWork;
    WrapIf* m_OrigRequest;                                  /*��Ӧ���Ķ�Ӧ�������ģ�����������ģ��ýӿڷ���NULL*/

#ifdef PBWRAP_CUSTOMIZE_CAST
    void* m_pOrigRequestEx;

#endif

    HPR_BOOL m_bTimeoutRsp;
    std::string m_sCommadName;
    int m_nProducerId;
    std::string m_sMqGropuId;
    HPR_UINT32 m_nMqSendType;
};

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetWrapMarkStr(std::string sCommandName)
{
    m_sCommadName = sCommandName;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetMqProducerId(int nProducerId)
{
    m_nProducerId = nProducerId;
};

template <class PBMSG>
inline int PBWrap<PBMSG>::GetMqProducerId()
{
    return m_nProducerId;
};

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetMqGroupId(const char* sGroupId)
{
    m_sMqGropuId = sGroupId;
}

template <class PBMSG>
inline std::string& PBWrap<PBMSG>::GetMqGroupId()
{
    return m_sMqGropuId;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetMqSendType(HPR_UINT32 nSendType)
{
    m_nMqSendType = nSendType;
}

template <class PBMSG>
inline HPR_UINT32 PBWrap<PBMSG>::GetMqSendType()
{
    return m_nMqSendType;
}

template <class PBMSG>
inline std::string PBWrap<PBMSG>::GetWrapMarkStr()
{
    return m_sCommadName;
}

template <class PBMSG>
inline HPR_BOOL PBWrap<PBMSG>::IsTimeoutRspWrap()
{
    return m_bTimeoutRsp;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp)
{
    m_bTimeoutRsp = bTimeoutRsp;
}

template <class PBMSG>
inline HPR_BOOL PBWrap<PBMSG>::IsWrapFromNetwork()
{
    return m_bFromNetWork;
}

template <class PBMSG>
inline PBWrap<PBMSG>* PBWrap<PBMSG>::Clone()
{
    PBWrap<PBMSG>* pClone = PBWrapFactory<PBMSG>::ClonePbWrap(this);
    return pClone;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetSendBuffer(HPR_INT8* pBuffer)
{
    m_pSendBuffer = pBuffer;
}
template <class PBMSG>
inline HPR_INT8* PBWrap<PBMSG>::GetSendBuffer()
{
    return m_pSendBuffer;
}
template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetWrapFromNetwork(HPR_BOOL bFrom)
{
    m_bFromNetWork = bFrom;
}

template <class PBMSG>
inline WrapIf* PBWrap<PBMSG>::GetOrigRequest()
{
    return m_OrigRequest;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetOrigRequest(WrapIf* pReq)
{
    m_OrigRequest = pReq;
}

#ifdef PBWRAP_CUSTOMIZE_CAST
template <class PBMSG>
inline void* PBWrap<PBMSG>::GetOrigRequestEx()
{
    return m_pOrigRequestEx;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetOrigRequestEx(void* pReq)
{
    m_pOrigRequestEx = pReq;
}

#endif


template <class PBMSG>
inline PBWrap<PBMSG>::PBWrap(void)
{
    m_iHead.m_nMagic = HEADER_FLAG_;
    m_iHead.m_cVersion = 0x1;
    m_iHead.m_cCompress= 0;
    m_iHead.m_nReserve = 0;
    m_iHead.m_nService = 0;
    m_bFromNetWork = HPR_FALSE;
    m_pSendBuffer = NULL;
    m_nSrcId = 0;
    m_nDstId = 0;
    m_nInnerSequence = 0;
    m_OrigRequest = NULL;
    m_bTimeoutRsp = HPR_FALSE;
    m_nProducerId = -1;
}
template <class PBMSG>
inline PBWrap<PBMSG>::PBWrap(const PBWrap& c)
: m_pSendBuffer(NULL),
  m_iHead(c.m_iHead),
  m_iMsg(c.m_iMsg),
  m_nSrcId(c.m_nSrcId),
  m_nDstId(c.m_nDstId),
  m_nInnerSequence(c.m_nInnerSequence),
  m_bFromNetWork(c.m_bFromNetWork),
  m_OrigRequest(NULL),
  m_bTimeoutRsp(c.m_bTimeoutRsp),
  m_nProducerId(c.m_nProducerId)
{
    if (c.m_OrigRequest)
    {
        m_OrigRequest =c.m_OrigRequest->Clone();
    }
}
template <class PBMSG>
inline PBWrap<PBMSG>::~PBWrap(void)
{
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetCommandId(HPR_UINT32 nCommandId)
{
    m_iHead.m_nCommand = nCommandId;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetMessageType(HPR_UINT32 iMsgType)
{
    m_iHead.m_nMark = (HPR_UINT32)iMsgType;
}
/** @fn FillWithData
*   @brief ���ݶ�����������䱨��
*   @param data ����������
*   @param datalen ���������ݵĳ���
*   @return ���ɹ�����HPR_OK ʧ�ܷ���HPR_ERROR
*/
template <class PBMSG>
inline HPR_INT32 PBWrap<PBMSG>::FillWithData(HPP_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen)
{
    if (m_iMsg.ParseFromArray(data, datalen))
    {
        this->m_iHead = head;
        return HPR_OK;
    }
    return HPR_ERROR;
}

/** @fn GetContentLen
*   @brief ��ȡ���ݱ����ȣ�����HPPͷ��HPPβ����
*   @return ���ݱ�����
*/
template <class PBMSG>
inline HPR_INT32 PBWrap<PBMSG>::GetContentLen()
{
    return m_iMsg.ByteSize() /*+ HPP_PACK_HEAD::nHEAD_LEN + PACK_TAIL_LEN*/;
}

/** @fn GetContent
*   @brief ��ȡ���ݱ����ݣ�����HPPͷ��HPPβ����
*   @param [out]buffer �������ݱ�����
*   @param bufferLen �������buffer����
*/
template <class PBMSG>
HPR_VOID PBWrap<PBMSG>::GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen)
{
    HPR_INT32 msgLen;

    msgLen = m_iMsg.ByteSize();
    m_iHead.m_nLength = msgLen/* + PACK_TAIL_LEN + HPP_PACK_HEAD::nHEAD_LEN*/;
    m_iHead.SaveHeadToBuffer(buffer);
    m_iMsg.SerializeToArray(buffer + HPP_PACK_HEAD::nHEAD_LEN, bufferLen);
    *((HPR_UINT32*)(buffer+ HPP_PACK_HEAD::nHEAD_LEN+msgLen)) = htonl(ENDMARK);
}

/** @fn DoExecute
*   @brief Ĭ��ִ�к���
*   @return HPR_OK;
*/
template <class PBMSG>
inline WrapIf* PBWrap<PBMSG>::DoExecute()
{
    //  std::string className = std::string(typeid(m_iMsg).name()).substr(6);
    //  HPP_DEBUG("Please complete DoExecute() function for %s message", className.c_str());
    //  std::string str;
    //  PrintToString(&str);
    //  HPP_DEBUG("\nRecv message %s", className.c_str());

    return NULL;
}

/** @fn DoTimeout
*   @brief Ĭ�ϳ�ʱִ�к��� 
*   @return HPR_OK
*/
template <class PBMSG>
inline HPR_INT32 PBWrap<PBMSG>::DoTimeout()
{
    std::string className = std::string(typeid(m_iMsg).name()).substr(6);
    HPP_DEBUG("Please complete DoTimeout for %s message", className.c_str());
    return 0;
}

/** @fn PrintToString
*   @brief ��ʽ��������ַ���
*   @param [out]output ����ַ���
*/
template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::PrintToString(std::string* output)
{
    std::stringstream ss;
    google::protobuf::TextFormat::PrintToString(m_iMsg, output);
    ss.clear();
    ss << "\r\n----------------------------------------\n";
    ss << "Message Header: sequence = " << m_iHead.m_nSequence <<", ";
    ss << "command = " << (HPR_INT32)m_iHead.m_nCommand << ", ";
    ss << "length = " << (HPR_INT32)m_iHead.m_nLength << "\n";
    ss << "Message Content: \n";
    ss << (*output) << std::endl;  

    *output = ss.str();
}


/******************************* Getter and Setter*******************************/
template <class PBMSG>
inline HPR_INT32 PBWrap<PBMSG>::GetWrapSrcId()
{
    return m_nSrcId;
}


template <class PBMSG>
inline HPR_INT32 PBWrap<PBMSG>::GetWrapDstId()
{
    return m_nDstId;
}


template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetWrapSrcId(HPR_INT32 srcId)            /*������Դ*/
{
    this->m_nSrcId =srcId;
}


template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetWrapDstId(HPR_INT32 dstId)            /*����Ŀ��*/
{
    this->m_nDstId = dstId;
}

template <class PBMSG>
inline HPR_UINT32 PBWrap<PBMSG>::GetInnerSequence()
{
    return m_nInnerSequence;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetInnerSequence(HPR_UINT32 nInnerSequence)
{
    m_nInnerSequence = nInnerSequence;
}

template <class PBMSG>
inline HPP_PACK_HEAD PBWrap<PBMSG>::GetPackHead()
{
    return m_iHead;
}

template <class PBMSG>
inline HPR_VOID PBWrap<PBMSG>::SetPackHead(HPP_PACK_HEAD iHead)
{
    m_iHead = iHead;
}

template <class PBMSG>
inline PBMSG& PBWrap<PBMSG>::GetMsg()
{
    return m_iMsg;
}


#endif
