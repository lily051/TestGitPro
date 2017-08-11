//lint -library
#pragma once
#include <sstream>

#include <hpp_common.h>
#include <hpp_wrap.h>

class FiPBWrap :public WrapIf
{
public:
    /************************** Implement Of Base Class *************************************/
    inline WrapIf* DoExecute();                     /*���������߼������߼���ʵ��*/
    inline HPR_INT32 DoTimeout();

    inline HPR_INT32 GetWrapSrcId();                        /*������Դ*/
    inline HPR_INT32 GetWrapDstId();                        /*����Ŀ��*/
    inline HPR_VOID SetWrapSrcId(HPR_INT32 srcId);          /*������Դ*/
    inline HPR_VOID SetWrapDstId(HPR_INT32 dstId);          /*����Ŀ��*/
    inline FiPBWrap* Clone();

    inline HPR_UINT32 GetInnerSequence();               /*��ȡ�ڲ����*/
    inline HPR_VOID SetInnerSequence(HPR_UINT32 seq);       /*�����ڲ����*/
    inline WrapIf* GetOrigRequest();                        /*��ȡ��ص������ģ�ʧ�ܷ���NULL*/
    inline HPR_VOID SetOrigRequest(WrapIf* pReq);

    inline void* GetOrigRequestEx();
    inline HPR_VOID SetOrigRequestEx(void* pReq);

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
    virtual HPR_VOID SetMqDstUri(const char* sUri);
    virtual std::string& GetMqDstUri();

    virtual HPR_VOID SetMqProducerId(int nProducerId);
    virtual int GetMqProducerId();

    virtual HPR_VOID SetMqGroupId(const char* sGroupId);
    virtual std::string& GetMqGroupId();
    virtual HPR_VOID SetMqSendType(HPR_UINT32 nSendType);
    virtual HPR_UINT32 GetMqSendType();
    /**************************** Other Function/Method Of This Class ***************************/
    inline string& GetMsg();
    FiPBWrap(void);
    FiPBWrap(const FiPBWrap& c);
    ~FiPBWrap(void);

private:
    HPR_INT8* m_pSendBuffer;
    HPP_PACK_HEAD m_iHead;
    string m_sData;
    HPR_INT32 m_nSrcId;                                     /*������Դ*/
    HPR_INT32 m_nDstId;                                     /*����Ŀ��*/
    HPR_UINT32 m_nInnerSequence;
    HPR_BOOL m_bFromNetWork;
    WrapIf* m_OrigRequest;                                  /*��Ӧ���Ķ�Ӧ�������ģ�����������ģ��ýӿڷ���NULL*/
    void* m_pOrigRequestEx;

    HPR_BOOL m_bTimeoutRsp;
    std::string m_sCommadName;
    int m_nProducerId;
    std::string m_sMqDstUri;
    std::string m_sMqGropuId;
    HPR_UINT32 m_nMqSendType;
};

inline HPR_VOID FiPBWrap::SetWrapMarkStr(std::string sCommandName)
{
    m_sCommadName = sCommandName;
}

inline HPR_VOID FiPBWrap::SetMqProducerId(int nProducerId)
{
    m_nProducerId = nProducerId;
};

inline int FiPBWrap::GetMqProducerId()
{
    return m_nProducerId;
};

inline HPR_VOID FiPBWrap::SetMqGroupId(const char* sGroupId)
{
    m_sMqGropuId = sGroupId;
}

inline std::string& FiPBWrap::GetMqGroupId()
{
    return m_sMqGropuId;
}

inline HPR_VOID FiPBWrap::SetMqSendType(HPR_UINT32 nSendType)
{
    m_nMqSendType = nSendType;
}

inline HPR_UINT32 FiPBWrap::GetMqSendType()
{
    return m_nMqSendType;
}

inline std::string FiPBWrap::GetWrapMarkStr()
{
    return m_sCommadName;
}

inline HPR_BOOL FiPBWrap::IsTimeoutRspWrap()
{
    return m_bTimeoutRsp;
}

inline HPR_VOID FiPBWrap::SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp)
{
    m_bTimeoutRsp = bTimeoutRsp;
}

inline HPR_BOOL FiPBWrap::IsWrapFromNetwork()
{
    return m_bFromNetWork;
}

inline FiPBWrap* FiPBWrap::Clone()
{
    FiPBWrap* pWrap = new (std::nothrow) FiPBWrap(*this);
    if (pWrap == NULL)
    {
        LOG_HPP_NO_MEM_ERROR;
        return NULL;
    }
    return pWrap;
}

inline HPR_VOID FiPBWrap::SetSendBuffer(HPR_INT8* pBuffer)
{
    m_pSendBuffer = pBuffer;
}

inline HPR_INT8* FiPBWrap::GetSendBuffer()
{
    return m_pSendBuffer;
}

inline HPR_VOID FiPBWrap::SetWrapFromNetwork(HPR_BOOL bFrom)
{
    m_bFromNetWork = bFrom;
}

inline WrapIf* FiPBWrap::GetOrigRequest()
{
    return m_OrigRequest;
}

inline HPR_VOID FiPBWrap::SetOrigRequest(WrapIf* pReq)
{
    m_OrigRequest = pReq;
}

inline void* FiPBWrap::GetOrigRequestEx()
{
    return m_pOrigRequestEx;
}

inline HPR_VOID FiPBWrap::SetOrigRequestEx(void* pReq)
{
    m_pOrigRequestEx = pReq;
}



inline FiPBWrap::FiPBWrap(void)
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
	m_pOrigRequestEx = NULL;
	m_nMqSendType = 0;
    m_bTimeoutRsp = HPR_FALSE;
    m_nProducerId = -1;
}

inline FiPBWrap::FiPBWrap(const FiPBWrap& c)
: m_pSendBuffer(NULL),
  m_iHead(c.m_iHead),
  m_sData(c.m_sData),
  m_nSrcId(c.m_nSrcId),
  m_nDstId(c.m_nDstId),
  m_nInnerSequence(c.m_nInnerSequence),
  m_bFromNetWork(c.m_bFromNetWork),
  m_OrigRequest(NULL),
  m_pOrigRequestEx(NULL),
  m_nMqSendType(0),
  m_bTimeoutRsp(c.m_bTimeoutRsp),
  m_nProducerId(c.m_nProducerId)
{
    if (c.m_OrigRequest)
    {
        m_OrigRequest = c.m_OrigRequest->Clone();
    }
}

inline FiPBWrap::~FiPBWrap(void)
{
}

inline HPR_VOID FiPBWrap::SetCommandId(HPR_UINT32 nCommandId)
{
    m_iHead.m_nCommand = nCommandId;
}

inline HPR_VOID FiPBWrap::SetMessageType(HPR_UINT32 iMsgType)
{
    m_iHead.m_nMark = (HPR_UINT32)iMsgType;
}

/** @fn FillWithData
*   @brief ���ݶ�����������䱨��
*   @param data ����������
*   @param datalen ���������ݵĳ���
*   @return ���ɹ�����HPR_OK ʧ�ܷ���HPR_ERROR
*/

inline HPR_INT32 FiPBWrap::FillWithData(HPP_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen)
{
    if (NULL != data)
    {
        m_sData.assign((char*)data, datalen);
        this->m_iHead = head;
        return HPR_OK;
    }
    return HPR_ERROR;
}

/** @fn GetContentLen
*   @brief ��ȡ���ݱ����ȣ�����HPPͷ��HPPβ����
*          liying: ʵ�ʲ�������HPPͷ��HPPβ���ȣ��μ�hpp_wrap_pb.h��ʵ��
*   @return ���ݱ�����
*/
inline HPR_INT32 FiPBWrap::GetContentLen()
{
    HPR_INT32 nLen = m_sData.size();
    return  nLen/*+ HPP_PACK_HEAD::nHEAD_LEN + PACK_TAIL_LEN*/;
}

/** @fn GetContent
*   @brief ��ȡ���ݱ����ݣ�����HPPͷ��HPPβ����
*   @param [out]buffer �������ݱ�����
*   @param bufferLen �������buffer����
*          liying: bufferLen������HPPͷ��HPPβ���μ�hpp_wrap_pb.h��ʵ��
*/
HPR_VOID FiPBWrap::GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen)
{
    HPR_INT32 msgLen;

    msgLen = m_sData.size();
    m_iHead.m_nLength = msgLen/* + PACK_TAIL_LEN + HPP_PACK_HEAD::nHEAD_LEN*/;
    m_iHead.SaveHeadToBuffer(buffer);
    HPR_INT32 nCopied = m_sData._Copy_s((char*)(buffer + HPP_PACK_HEAD::nHEAD_LEN), bufferLen, msgLen);
    if (nCopied != msgLen)
    {
        DebugBreak();
    }
    *((HPR_UINT32*)(buffer+ HPP_PACK_HEAD::nHEAD_LEN+msgLen)) = htonl(ENDMARK);
}

/** @fn DoExecute
*   @brief Ĭ��ִ�к���
*   @return HPR_OK;
*/
//inline WrapIf* FiPBWrap::DoExecute()
//{
//    return NULL;
//}

/** @fn DoTimeout
*   @brief Ĭ�ϳ�ʱִ�к���
*   @return HPR_OK
*/
inline HPR_INT32 FiPBWrap::DoTimeout()
{
    HPP_DEBUG("Please complete DoTimeout");
    return 0;
}

/** @fn PrintToString
*   @brief ��ʽ��������ַ���
*   @param [out]output ����ַ���
*/
inline HPR_VOID FiPBWrap::PrintToString(std::string* output)
{
    std::stringstream ss;
    ss.clear();
    ss << "\r\n----------------------------------------\n";
    ss << "Message Header: sequence = " << m_iHead.m_nSequence <<", ";
    ss << "command = " << (HPR_INT32)m_iHead.m_nCommand << ", ";
    ss << "length = " << (HPR_INT32)m_iHead.m_nLength << "\n";
    ss << "Message Content: \n";
    ss << m_sData.size() << "bytes" << std::endl;

    *output = ss.str();
}

/******************************* Getter and Setter*******************************/
inline HPR_INT32 FiPBWrap::GetWrapSrcId()
{
    return m_nSrcId;
}

inline HPR_INT32 FiPBWrap::GetWrapDstId()
{
    return m_nDstId;
}

inline HPR_VOID FiPBWrap::SetWrapSrcId(HPR_INT32 srcId)            /*������Դ*/
{
    this->m_nSrcId =srcId;
}

inline HPR_VOID FiPBWrap::SetWrapDstId(HPR_INT32 dstId)            /*����Ŀ��*/
{
    this->m_nDstId = dstId;
}

inline HPR_UINT32 FiPBWrap::GetInnerSequence()
{
    return m_nInnerSequence;
}

inline HPR_VOID FiPBWrap::SetInnerSequence(HPR_UINT32 nInnerSequence)
{
    m_nInnerSequence = nInnerSequence;
}

inline HPP_PACK_HEAD FiPBWrap::GetPackHead()
{
    return m_iHead;
}

inline HPR_VOID FiPBWrap::SetPackHead(HPP_PACK_HEAD iHead)
{
    m_iHead = iHead;
}

inline string& FiPBWrap::GetMsg()
{
    return m_sData;
}

inline HPR_VOID FiPBWrap::SetMqDstUri(const char* sUri)
{
    m_sMqDstUri = sUri;
};

inline std::string& FiPBWrap::GetMqDstUri()
{
    return m_sMqDstUri;
};