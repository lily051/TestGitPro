#pragma once
#ifndef _HPP_WRAPPERIF_H_
#define _HPP_WRAPPERIF_H_

#include "hpp_common.h"
#include "hpp_head.h"
#include <iostream>

class HPP_DECLARE_CLASS Wrap
{
public:
    virtual ~Wrap()
    {
    }

    virtual HPR_INT32 DoTimeout() = 0;

    virtual HPR_INT32 GetWrapSrcId() = 0;                       /** 信令来源*/
    virtual HPR_INT32 GetWrapDstId() = 0;                       /** 信令目的*/
    virtual HPR_VOID SetWrapSrcId(HPR_INT32 srcId) = 0;         /** 信令来源*/
    virtual HPR_VOID SetWrapDstId(HPR_INT32 dstId) = 0;         /** 信令目的*/
    virtual HPR_UINT32 GetInnerSequence() = 0;                  /** 获取内部序号*/
    virtual HPR_VOID SetInnerSequence(HPR_UINT32 seq) = 0;      /** 设置内部序号*/

    virtual HPR_INT32 GetContentLen() = 0;                      /** 信令内容长度*/
    virtual HPR_VOID GetContent(HPR_INT8* buffer, HPR_INT32 bufferLen) = 0;
    virtual HPR_VOID PrintToString(std::string* string) = 0;

    virtual HPR_BOOL IsWrapFromNetwork() = 0;
    virtual HPR_VOID SetWrapFromNetwork(HPR_BOOL bFrom) = 0;

    virtual HPR_VOID SetSendBuffer(HPR_INT8* pBuffer) = 0;
    virtual HPR_INT8* GetSendBuffer() = 0;

    virtual HPR_VOID SetWrapMarkStr(std::string sCommandName) = 0;              //设置报文的特征串,该字符串记在Wrap内部,由用户设置并定义其内容,一般用于记件事件
    virtual std::string GetWrapMarkStr() = 0;

    virtual HPR_BOOL IsTimeoutRspWrap() = 0;
    virtual HPR_VOID SetBTimeoutRspWrap(HPR_BOOL bTimeoutRsp) = 0;
};

class HPP_DECLARE_CLASS WrapIf:public Wrap
{
public:
    virtual WrapIf* DoExecute() = 0;                                /*处理信令逻辑，由逻辑层实现*/
    virtual ~WrapIf()
    {
    }
    virtual HPP_PACK_HEAD GetPackHead() = 0;
    virtual HPR_VOID SetPackHead(HPP_PACK_HEAD head) = 0;
    virtual HPR_INT32 FillWithData(HPP_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen) = 0;
    virtual WrapIf* GetOrigRequest() = 0;                           /*如果是响应报文，该接口可以获得相关的请求报文，如果是请求报文，该接口返回NULL*/
    virtual HPR_VOID SetOrigRequest(WrapIf* pReq) = 0;                          /*如果是响应报文，该接口可以获得相关的请求报文，如果是请求报文，该接口返回NULL*/

/** @note 如果需要启用这部分接口，HPP以及使用HPP的模块都应定义PBWRAP_CUSTOMIZE_CAST
 *  @date 02/06/2015 by fangyu
 */
#ifdef PBWRAP_CUSTOMIZE_CAST
    virtual void* GetOrigRequestEx() = 0;
    virtual HPR_VOID SetOrigRequestEx(void* pReq) = 0;

#endif

    virtual HPR_VOID SetMqProducerId(int nProducerId) = 0;
    virtual int GetMqProducerId() = 0;
    virtual HPR_VOID SetMqGroupId(const char* sGroupId) = 0;
    virtual std::string& GetMqGroupId() = 0;
    virtual HPR_VOID SetMqSendType(HPR_UINT32 nSendType) = 0;
    virtual HPR_UINT32 GetMqSendType() = 0;

    virtual WrapIf* Clone() = 0;
};

class HPP_DECLARE_CLASS WrapEhIf:public Wrap
{
public:
    virtual WrapEhIf* DoExecute() = 0;                              /*处理信令逻辑，由逻辑层实现*/
    virtual ~WrapEhIf()
    {
    }
    virtual HPP_EHOME_PACK_HEAD GetPackHead() = 0;
    virtual HPR_VOID SetPackHead(HPP_EHOME_PACK_HEAD head) = 0;
    virtual HPR_INT32 FillWithData(HPP_EHOME_PACK_HEAD head, HPR_INT8* data, HPR_INT32 datalen) = 0;

    virtual WrapEhIf* GetOrigRequest() = 0;                         /*如果是响应报文，该接口可以获得相关的请求报文，如果是请求报文，该接口返回NULL*/
    virtual HPR_VOID SetOrigRequest(WrapEhIf* pReq) = 0;                            /*如果是响应报文，该接口可以获得相关的请求报文，如果是请求报文，该接口返回NULL*/

    virtual HPR_VOID SetDstAddr(HPR_ADDR_T iAddr)
    {
    }                               /*对于UDP，需要设置目的IP、端口,TCP而目不必如此，所以在此有个空实现*/
    virtual HPR_ADDR_T* GetDstAddrPtr()
    {
        return NULL;
    }

    virtual std::string GetLogName() = 0;
    virtual HPR_VOID SetLogName(const std::string& sLogName) = 0;
};

/*#define WrapIf Wrap<>*/


#endif /* WRAPPERIF_H_ */
