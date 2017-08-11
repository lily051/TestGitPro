/** @file hpp_hpp.h
 *  @brief HPP协议栈对外接口
 *  @version 1.0
 *  @date   2010/03/31
 */

#ifndef _HPP_HPP_H_
#define _HPP_HPP_H_

#include "hpp_head.h"
#include "hpp_wrap_pb.h"
#include "hpp_wrap_factory.h"
#include "hpp_transferable.h"
#include "hpp_common.h"

/** @typedef    HPP_HANDLE
 *  @brief      HPP_Init()函数返回的类型
 *  @see        HPP_Init   
 */
typedef void* HPP_HANDLE;

/** @typedef    HPP_SERVER_HANDLE 
 *  @brief      本地TCP监听服务的HANDLE
 *  @see        HPP_StartLocalServer
 */
typedef void* HPP_SERVER_HANDLE;

/** @typedef    PBMSG_CREATE_BYCMD_FUN
 *  @brief      HPP(Protocol buffer)协议根据协议编号创建协议报文的回调函数类型
 *  @see        HPP_SetPbMsgCreateFun
 */
typedef WrapIf* (*PBMSG_CREATE_BYCMD_FUN)(HPR_INT32);

/** @typedef    EHMSG_CREATE_BYCMD_FUN
 *  @brief      E家协议根据报文关键内容(Command) (ConfigName)创建报文的回调函数类型
 *  @see        HPP_SetEhMsgCreateFun
 */
typedef WrapEhIf* (*EHMSG_CREATE_BYCMD_FUN)(const std::string&, const std::string&, const std::string);


/** @class  ConnSession 
 *  @brief  连接会话接口.
 *
 *          连接会话接口,处理该连接上的数据收发,上下文维持。
 */
class HPP_DECLARE_CLASS ConnSession
{
public:

    virtual ~ConnSession(){}

    /** @fn     HPR_INT32   SendMessage(Wrap* pMessage)
     *  @brief  向当前会话的连接发送报文
     *  @param  pMessage 报文
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SendMessage(Wrap* pMessage) = 0;

    /** @fn     PostInnerMessage(HPR_VOID* pMessage, HPR_INT32 nDstId)
     *  @brief  把报文抛送到目标队列去处理
     *  @param  pMessage 报文
     *  @param  nDstId 目标会话ID
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   PostInnerMessage(HPR_VOID* pMessage, HPR_INT32 nDstId) = 0;

    /** @fn     StartService(HPR_INT32 nPri)
     *  @brief  开始本会话上的服务,注意,每个会话都须开启服务才开始处理业务逻辑
     *  @param  nPri 连接会话的处理优先级,取值为(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)。
     *  @param  nRecvBufferSize 消息接收缓冲区大小，使用默认值0则采用默认的TCP或UDP缓冲区大小
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   StartService(HPR_INT32 nPri, HPR_INT32 nRecvBufferSize = 0) = 0;

    /** @fn     HPR_INT32   StopService(HPR_BOOL bInThisSessionProc = HPR_FALSE)
     *  @brief  停止会话上的服务,即停止业务逻辑的处理
     *  @param  bInThisSessionProc 是否在ConnSession所在的线程调用此函数,建议逻辑层使用默认参数
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   StopService(HPR_BOOL bInThisSessionProc = HPR_FALSE) = 0;

    /** @fn     HPR_INT32   SetSocketClosedCallBack(HPR_INT32 (*_OnSocketClosed)(ConnSession*))
     *  @brief  设置连接断开时(Socket断开)时的回调函数 
     *  @param  _OnSocketClosed 连接断开回调函数
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetSocketClosedCallBack(HPR_INT32 (*_OnSocketClosed)(ConnSession*)) = 0;

    /** @fn     HPR_INT32   SetTimeOutCallBack(HPR_BOOL (*_OnCheckTimeout)(ConnSession*))
     *  @brief  设置连接超时判断函数, 注，逻辑层做超时判断时务必不要加锁，否则超时判断由逻辑判断自己完成
     *  @param  _OnCheckTimeout超时判断回调函数,当逻辑层该回调函数返回HPR_TRUE时,HPP会断开该路连接,并回调_OnSocketClosed
     *  @see SetSocketClosedCallBack
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetTimeOutCallBack(HPR_BOOL (*_OnCheckTimeout)(ConnSession*)) = 0;

    /** @fn     SetUserDefineDataCallBack(HPR_INT32 (*_OnRecvUserDefineData)(HPPTransferable* pUserData))
     *  @brief  设置用户自定义数据(消息)处理回调函数
     *  @param  _OnRecvUserDefineData 用户自定义数据(消息)处理回调函数
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetUserDefineDataCallBack(HPR_INT32 (*_OnRecvUserDefineData)(HPPTransferable* pUserData)) = 0;

    /** @fn     HPR_INT32   GetSessionId();
     *  @brief  获取当前会话的ID
     *  @param  void
     *  @return 当前会话的ID
     */
    virtual HPR_INT32   GetSessionId() = 0;

    /** @fn     HPR_INT32   SendMessage_r(HPR_VOID* pMessage, HPR_INT32 nDstId);
     *  @brief  发送报文到目标连接会话,该函数是线程安全的.该函数是静态方法
     *  @param  iHandle  HPP初始化后得到的句柄 
     *  @param  pMessage 报文 
     *  @param  nDstId 目标会话ID
     *  @return HPR_OK/HPR_ERROR
     */
    static HPR_INT32    SendMessage_r(HPR_VOID* pMessage, HPR_INT32 nDstId);

    /** @fn     HPR_INT32   SendUserData_r(HPPTransferable *pUserData, HPR_INT32 nDstId);
     *  @brief  发送用户自定义数据(消息)到目标连接会话,该函数是线程安全的.该函数是静态方法
     *  @param  iHandle  HPP初始化后得到的句柄 
     *  @param  pMessage 用户自定义数据(消息),不同于GetUserData/SetUserData里的自定义数据,这里指自定义报文的概念
     *  @param  nDstId 目标会话ID
     *  @return HPR_OK/HPR_ERROR
     */
    static  HPR_INT32   SendUserData_r(HPPTransferable *pUserData, HPR_INT32 nDstId);

    /** @fn     HPR_VOID    SetServiceType(HPR_UINT16 nType) ;  
     *  @brief  设置服务类型,对应HPP_PACK_HEAD里的service字段，cms__vtm()0x0102/0x0201
     *  @param  nType 服务类型
     *  @see    GetServiceType
     *  @return HPR_VOID
     */
    virtual HPR_VOID    SetServiceType(HPR_UINT16 nType) = 0;

    /** @fn     HPR_VOID    SetServiceType(HPR_UINT16 nClientType, HPR_UINT16 nServerType)
     *  @brief  设置服务类型，对应HPP_PACK_HEAD里的service字段，采用更通用的接口，避免扩展服务类型时需要在HPP
     *          协议栈中添加相应的定义
     *  @param  会话中作为客户端的服务器类型，对应HPP协议中服务器类型定义，取值范围0x01 - 0xFF
     *  @param  会话中作为服务端的服务器类型，对应HPP协议中服务器类型定义，取值范围0x01 - 0xFF
     *  @return HPR_VOID
     *  
     *  @note   如PAG连接CMS的会话，PAG作为客户端，PAG类型为0x04，CMS作为服务端，CMS类型为0x01
     */
    virtual HPR_VOID    SetServiceType(HPR_UINT16 nClientType, HPR_UINT16 nServerType) = 0;

    /** @fn     HPR_INT32   GetServiceType()
     *  @brief  获取服务类型 
     *  @param  void
     *  @see    SetServiceType
     *  @return 服务类型
     */
    virtual HPR_INT32   GetServiceType() = 0;

    /** @fn     HPR_VOID    SetSessionLevel(HPR_INT32 nLevel)
     *  @brief  设置Session的优先级,已经不使用,下一版本去掉
     *  @param  nLevel 优先级
     *  @return HPR_VOID
     */
    virtual HPR_VOID    SetSessionLevel(HPR_INT32 nLevel) = 0;

    /** @fn     HPR_INT32   AddTimer(TIMER_INFO iTimeinfo)
     *  @brief  添加一个定时器到当前ConnSession
     *  @param  iTimeinfo 定时器信息
     *  @see    TIMER_INFO
     *  @return 
     */
    virtual HPR_INT32   AddTimer(TIMER_INFO iTimeinfo) = 0;

    /** @fn     HPR_VOID    DelTimer(HPR_UINT32 nTimerIndex)
     *  @brief  删除定时器
     *  @param  nTimerIndex 定时器编号,AddTimer返回值
     *  @see    AddTimer
     *  @return HPR_VOID
     */
    virtual HPR_VOID    DelTimer(HPR_UINT32 nTimerIndex) = 0;

    /** @fn     HPR_VOID            GetUserData()
     *  @brief  设置用户自定义数据
     *  @param  pUserData 用户自定义数据
     *  @return 用户自定义数据，如果用户自定义数据未传入，返回NULL
     */
    virtual HPR_VOID    SetUserData(HPR_VOIDPTR pUserData) = 0;

    /** @fn     HPR_VOIDPTR         GetUserData()
     *  @brief  获取用户自定义数据
     *  @param  void
     *  @return 用户自定义数据，如果用户自定义数据未传入，返回NULL
     */
    virtual HPR_VOIDPTR GetUserData() = 0;

    /** @fn     const char* GetSockRemoteIp()
     *  @brief  获取远端IP地址
     *  @param  void
     *  @return 远端IP地址,失败时返回NULL
     */
    virtual const char* GetSockRemoteIp() = 0;

    /** @fn     GetSockRemotePort
     *  @brief  获取远端端口
     *  @param  void
     *  @return 远端端口,失败时返回0
     */
    virtual HPR_UINT16  GetSockRemotePort() = 0;

    /** @fn     GetSockFd
     *  @brief  获得ConnSession对应的Socket句柄
     *  @param  void
     *  @return Socket句柄
     */
    virtual HPR_SOCK_T GetSockFd() = 0;

    static const HPR_INT32  PROTOCOL_TYPE_HPP = 0;              /**< HPP (Protocol Buffer) 协议 */
    static const HPR_INT32  PROTOCOL_TYPE_EHOME = 1;            /**< 我的E家 协议 */
    static const HPR_INT32  PROTOCOL_TYPE_MQ = 2;               /**< MQ (Protocol Buffer) 协议 */
    static const HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;          /**< 虚拟会话 */
};


/** @fn     HPR_INT8* HPP_GetVersion();
 *  @brief  获取当前协议栈的版本号。
 *  @param  void
 *  @return 版本号字符串，以\0结束
 */
HPP_DECLARE HPR_INT8*   HPP_GetVersion();

/** @fn     HPP_HANDLE HPP_Init(HPR_INT32 nWorkNumber);
 *  @brief  初始化HPP协议栈,未做计数,不可多次调用
 *  @param  nWorkNumber HPP协议栈工作线程数
 *  @param  nContextTimeout 上下文超时时间,以秒为单位
 *  @return NULL如果失败，非NULL值HPPHANDLE如果成功
 */
HPP_DECLARE HPP_HANDLE  HPP_Init(HPR_INT32 nWorkNumber, HPR_UINT32 nContextTimeout = 30, HPR_BOOL bInitMQ = HPR_TRUE);


/** @fn     HPR_VOID HPP_Close(HPP_HANDLE iHandle);
 *  @brief  关闭HPP协议栈,一般在系统退出时调用,建议上层逻辑全部停掉再调用此函数。
 *  @param  iHandle 由HPP_Init的返回值
 *  @see    HPP_Init
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID    HPP_Close(HPP_HANDLE iHandle);


/** @fn     HPP_SERVER_HANDLE HPP_StartLocalServer(HPP_HANDLE iHandle, const char* pLocalIp, HPR_UINT16 nLocalPort, 
 *              HPR_INT32 (*_OnAcceptNewClient)(ConnSession*), HPR_INT32 Protocol);
 *  @brief  创建TCP本地监听服务
 *  @param  iHandle HPP_Init的返回值
 *  @param  pLocalIp 本地监听使用的IP
 *  @param  nLocalPort 本地监听的端口
 *  @param  _OnAcceptNewClient 接到客户端连接的回调函数
 *  @param  Protocol 协议类型:
 *              const static HPR_INT32  PROTOCOL_TYPE_HPP = 0;
 *              const static HPR_INT32  PROTOCOL_TYPE_EHOME = 1;
 *              const static HPR_INT32  PROTOCOL_TYPE_MQ = 2;
 *              const static HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;
 *  @param  pSelfData 上下文关联指针
 *  @see    ConnSession::PROTOCOL_TYPE_HPP
 *  @see    ConnSession::PROTOCOL_TYPE_EHOME
 *  @see    ConnSession::PROTOCOL_TYPE_MQ
 *  @see    ConnSession::PROTOCOL_TYPE_VIRTUAL
 *  @see    HPP_Init 
 *  @see    HPP_StopLocalServer
 *  @return HPP_SERVER_HANDLE如果成功,NULL如果失败
 */
HPP_DECLARE HPP_SERVER_HANDLE HPP_StartLocalServer(HPP_HANDLE iHandle,
                                                   const char* pLocalIp,
                                                   HPR_UINT16 nLocalPort,
                                                   HPR_INT32 (*_OnAcceptNewClient)(ConnSession*),
                                                   HPR_INT32 Protocol,
                                                   HPR_VOID* pSelfData = NULL
                                                   );

/** @fn     HPR_VOID HPP_StopLocalServer(HPP_HANDLE iHandle, HPP_SERVER_HANDLE iServerHandle);
 *  @brief  停止本地监听服务
 *  @param  iHandle HPP_Init的返回值
 *  @param  iServerHandle HPP_StartLocalServer的返回值
 *  @see    HPP_StartLocalServer
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID    HPP_StopLocalServer(HPP_HANDLE iHandle, HPP_SERVER_HANDLE iServerHandle);




/** @fn     HPR_INT32 HPP_ConnRemoteService(HPP_HANDLE iHandle, const char* pRemoteIp, HPR_UINT16 nRemotePort, HPR_INT32 (*_OnCompleteConn)(ConnSession*),
 *                              const char* pLocalIp, HPR_UINT16 nLocalPort, HPR_UINT32 nProtocolType, HPR_VOID* pSelfData = NULL, HPR_INT32 nPri=SERVICE_NORMAL_PRI);
 *  @brief  连接远程服务,阻塞模式
 *  @param  iHandle HPP_Init的返回值
 *  @param  pRemoteIp 远程服务的IP地址
 *  @param  nRemotePort 远程服务的端口
 *  @param  pLocalIp 本地使用的IP地址
 *  @param  nLocalPort 本地使用的端口
 *  @param  nProtocolType 协议类型,外层调用传数值(0~3):
 *                        const static HPR_INT32  PROTOCOL_TYPE_HPP = 0;
 *                        const static HPR_INT32  PROTOCOL_TYPE_EHOME = 1;
 *                        const static HPR_INT32  PROTOCOL_TYPE_MQ = 2;
 *                        const static HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;
 *  @param  pSelfData 用户自定义数据,连接成功时,在_OnCompleteConn里可用ConnSession::GetUserData方法获取
 *  @param  _OnCompleteConn 连接成功的回调函数
 *  @param  nPri 连接会话的处理优先级,取值为(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)。
 *               默认为SERVICE_NORMAL_PRI。如果要修改优先级,在此处设置,ConnSession::StartService填入的优先级已不再使用。
 *  @see    ConnSession::PROTOCOL_TYPE_HPP
 *  @see    ConnSession::PROTOCOL_TYPE_EHOME
 *  @see    ConnSession::PROTOCOL_TYPE_MQ
 *  @see    ConnSession::PROTOCOL_TYPE_VIRTUAL
 *  @see    ConnSession::GetUserData
 *  @see    ConnSession::StartService
 *  @see    ConnSession
 *  @return HPR_OK/HPR_ERROR(连接失败)
 */
HPP_DECLARE HPR_INT32 HPP_ConnRemoteService(HPP_HANDLE iHandle,
                                            const char* pRemoteIp,
                                            HPR_UINT16 nRemotePort, 
                                            HPR_INT32 (*_OnCompleteConn)(ConnSession*),
                                            const char* pLocalIp,
                                            HPR_UINT16 nLocalPort,
                                            HPR_UINT32 nProtocolType, 
                                            HPR_VOID* pSelfData = NULL,
                                            HPR_INT32 nPri=SERVICE_NORMAL_PRI
                                            );

/** @fn     HPP_ConnRemoteServiceNoBlock(const char* pRemoteIp, HPR_UINT16 nRemotePort, HPR_INT32 (*_OnCompleteConn)(ConnSession*),
                                        const char* pLocalIp, HPR_UINT16 nLocalPort, HPR_VOID* pSelfData = NULL,
                                        HPR_INT32 nPri=SERVICE_NORMAL_PRI, HPR_INT32 (*_OnConnError)(HPR_VOIDPTR) = NULL);
 *  @brief  连接远程服务,异步连接模式,注意,该函数只能在HPP_Init函数之后使用.
 *  @param  iHandle HPP_Init的返回值
 *  @param  pRemoteIp 远程服务的IP地址
 *  @param  nRemotePort 远程服务的端口
 *  @param  _OnCompleteConn 连接完成(成功)时的回调函数
 *  @param  pLocalIp 本地连接的IP
 *  @param  nLocalPort 本地连接的端口
 *  @param  pSelfData 用户自定义数据,连接失败可以在_OnConnError里强制转换第一个参数获取。
 *                  连接成功，_OnCompleteConn里可用ConnSession::GetUserData获取。
 *  @param  nPri 连接会话的处理优先级,取值为(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)。
 *              默认为SERVICE_NORMAL_PRI。如果要修改优先级,在此处设置,ConnSession::StartService填入的优先级已不再使用。
 *  @param  _OnConnError 连接失败回调函数,后台5s钟连不上目标将会通过该回调通知使用着,使用当前接口必须指定连接失败回调函数
 *  @see    ConnSession::GetUserData
 *  @see    ConnSession::StartService
 *  @see    ConnSession
 *  @return HPR_OK如果成功(不意味连接成功),HPR_ERROR如果失败.如果返回失败,_OnConnError/_OnCompleteConn均不会被调用.
 */
HPP_DECLARE HPR_INT32 HPP_ConnRemoteServiceNoBlock(HPP_HANDLE iHandle,
                                                   const char* pRemoteIp, 
                                                   HPR_UINT16 nRemotePort, 
                                                   HPR_INT32 (*_OnCompleteConn)(ConnSession*),
                                                   const char* pLocalIp,
                                                   HPR_UINT16 nLocalPort, 
                                                   HPR_VOID* pSelfData = NULL,
                                                   HPR_INT32 nPri=SERVICE_NORMAL_PRI, 
                                                   HPR_INT32 (*_OnConnError)(HPR_VOIDPTR) = NULL
                                                   );

/** @fn     HPP_CreateMqSession(const HPR_INT8* sBrokerUri);
 *  @brief  创建与MQ的会话
 *  @param  iHandle HPP_Init的返回值
 *  @param  nMQClientBroekrID MQ服务器客户端代理ID
 *  @param  sMQQcuName MQ服务器QCU名字
 *  @param  sMQClusterName MQ服务器集群队列名
 *  @return 返回与MQ服务器连接的会话,如果发生错误返回NULL
 */
HPP_DECLARE ConnSession* HPP_CreateMqSession(HPP_HANDLE iHandle,
                                             const char* sMQClientBrokerIp,
                                             HPR_INT32 nMQClientBrokerPort,
                                             const char* sMQQcuName,
                                             const char* sMQClusterName = "",
                                             const char* sMQJndiUrl = ""
                                             );

/** @fn     HPR_INT32    HPP_AddMqConsumer(ConnSession* pMqSession, const HPR_INT8* sUri, HPR_UINT32 nSendType);
 *  @brief  在MQ的连接会话上添加消息消费者(接收MQ信息),
 *  @param  pMqSession HPP_CreateMqSession的返回值
 *  @param  sDstName 消息目的地名: 队列或主题名
 *  @param  nSendType 启用的MQ队列类型.
 *  @return HPR_OK, 如果添加消费者成功; HPR_ERROR, 如果添加消费者失败
 */
HPP_DECLARE HPR_INT32   HPP_AddMqConsumer(ConnSession* pMqSession,
                                          const char* sDstName,
                                          HPR_UINT32 nSendType
                                          );

/** @fn     HPR_INT32   HPP_AddMqProducer(ConnSession* pMqSession, const char* sUri, HPR_UINT32 nSendType);
 *  @brief  在MQ的连接会话上添加消息生产者(发送MQ消息)
 *  @param  pMqSession HPP_CreateMqSession的返回值
 *  @param  sDstName 消息目的地名: 队列或主题名
 *  @param  nSendType 启用的MQ队列类型.
 *  @return 生产者ID, >= 0, 如果添加生产者成功; HPR_ERROR, 如果添加生产者失败
 */
HPP_DECLARE HPR_INT32   HPP_AddMqProducer(ConnSession* pMqSession,
                                          const char* sDstName,
                                          HPR_UINT32 nSendType
                                          );

/** @fn     ConnSession* HPP_CreateUdpSession(const char* pIp, HPR_INT16 nPort);
 *  @brief  创建本地UDP监听会话
 *  @param  iHandle HPP_Init的返回值
 *  @param  pIp 本地监听的IP
 *  @param  nPort 本地监听的端口
 *  @return 返回本地监听的会话,返回NULL如果创建失败
 */
HPP_DECLARE ConnSession* HPP_CreateUdpSession(HPP_HANDLE iHandle, const char* pIp, HPR_INT16 nPort);

/** @fn     HPP_CreateVitualSession
 *  @brief  创建一个虚拟的会话
 *  @param  iHandle HPP_Init的返回值
 *  @param  void
 *  @return 返回一个虚拟的会话,返回NULL如果创建失败
 */
HPP_DECLARE ConnSession* HPP_CreateVitualSession(HPP_HANDLE iHandle);

/** @fn     HPP_GetHppSessionById
 *  @brief  根据会话的ID获取会话
 *  @param  iSessionId 会话ID
 *  @return 返回与iSessionId对应的会话,如果不存在返回NULL
 */
HPP_DECLARE ConnSession* HPP_GetHppSessionById(HPR_INT32 iSessionId);

/** @fn     HPR_VOID HPP_SetEhMsgCreateFun(EHMSG_CREATE_BYCMD_FUN fun);
 *  @brief  设置E家协议报文生成回调
 *  @param  iHandle HPP_Init的返回值
 *  @param  fun 报文生成回调函数
 *  @see    EHMSG_CREATE_BYCMD_FUN
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID HPP_SetEhMsgCreateFun(HPP_HANDLE iHandle, EHMSG_CREATE_BYCMD_FUN fun);

/** @fn     HPR_VOID HPP_SetPbMsgCreateFun(PBMSG_CREATE_BYCMD_FUN fun);
 *  @brief  设置HPP(Protocol Buffer协议)报文生成回调函数
 *  @param  iHandle HPP_Init的返回值
 *  @param  fun 报文生成回调函数
 *  @see    PBMSG_CREATE_BYCMD_FUN
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID HPP_SetPbMsgCreateFun(HPP_HANDLE iHandle, PBMSG_CREATE_BYCMD_FUN fun);
HPP_DECLARE HPR_VOID HPP_DelPbMsgCreateFun(HPP_HANDLE iHandle, PBMSG_CREATE_BYCMD_FUN fun);

/** @fn     HPP_SendWrapIfToHppStack
 *  @brief  向目标连接(会话)扔报文,该函数线程安全
 *  @param  iHandle Hpp_Init()返回值
 *  @param  pMessage 报文
 *  @param  nDstId 目标连接会话
 *  @return HPR_OK/HPR_ERROR
 */
HPP_DECLARE HPR_INT32 HPP_SendWrapIfToHppStack(HPP_HANDLE iHandle, WrapIf* pMessage, HPR_INT32 nDstId);

/** @fn     HPP_GetUnRepeatSeq
 *  @brief  获取一个不重复的序号,该函数内部实现是加锁自加
 *  @param  void
 *  @return 返回一个HPR_UINT32型序号
 */
HPP_DECLARE HPR_UINT32 HPP_GetUnRepeatSeq();


/** @fn     HPR_SOCK_T HPP_TCPConnNoBlock(const char* pRemoteIp, HPR_UINT16 nRemotePort, HPR_INT32 (*_OnCompleteConn)(HPR_VOIDPTR),
                            const char* pLocalIp, HPR_UINT16 nLocalPort, HPR_VOID* pSelfData = NULL,
                            HPR_INT32 (*_OnConnError)(HPR_VOIDPTR) = NULL);
 *  @brief  TCP异步连接
 *  @param  iHandle HPP_Init的返回值
 *  @param  pRemoteIp 远程IP
 *  @param  nRemotePort 远程端口
 *  @param  _OnCompleteConn连接成功回调
 *  @param  pLocalIp 本地IP
 *  @param  nLocalPort 本地端口
 *  @param  pSelfData 用户自定义数据 _OnConnError/_OnCompleteConn里的第一个参数.
 *  @param  _OnConnError 连接失败回调
 *  @return HPR_SOCK_T 创建的socket
 */
HPP_DECLARE HPR_SOCK_T HPP_TCPConnNoBlock(HPP_HANDLE iHandle,
                                          const char* pRemoteIp,
                                          HPR_UINT16 nRemotePort,
                                          HPR_INT32 (*_OnCompleteConn)(HPR_VOIDPTR),
                                          const char* pLocalIp,
                                          HPR_UINT16 nLocalPort,
                                          HPR_VOID* pSelfData = NULL,
                                          HPR_INT32 (*_OnConnError)(HPR_VOIDPTR) = NULL
                                          );

#endif

