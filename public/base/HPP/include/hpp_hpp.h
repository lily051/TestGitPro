/** @file hpp_hpp.h
 *  @brief HPPЭ��ջ����ӿ�
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
 *  @brief      HPP_Init()�������ص�����
 *  @see        HPP_Init   
 */
typedef void* HPP_HANDLE;

/** @typedef    HPP_SERVER_HANDLE 
 *  @brief      ����TCP���������HANDLE
 *  @see        HPP_StartLocalServer
 */
typedef void* HPP_SERVER_HANDLE;

/** @typedef    PBMSG_CREATE_BYCMD_FUN
 *  @brief      HPP(Protocol buffer)Э�����Э���Ŵ���Э�鱨�ĵĻص���������
 *  @see        HPP_SetPbMsgCreateFun
 */
typedef WrapIf* (*PBMSG_CREATE_BYCMD_FUN)(HPR_INT32);

/** @typedef    EHMSG_CREATE_BYCMD_FUN
 *  @brief      E��Э����ݱ��Ĺؼ�����(Command) (ConfigName)�������ĵĻص���������
 *  @see        HPP_SetEhMsgCreateFun
 */
typedef WrapEhIf* (*EHMSG_CREATE_BYCMD_FUN)(const std::string&, const std::string&, const std::string);


/** @class  ConnSession 
 *  @brief  ���ӻỰ�ӿ�.
 *
 *          ���ӻỰ�ӿ�,����������ϵ������շ�,������ά�֡�
 */
class HPP_DECLARE_CLASS ConnSession
{
public:

    virtual ~ConnSession(){}

    /** @fn     HPR_INT32   SendMessage(Wrap* pMessage)
     *  @brief  ��ǰ�Ự�����ӷ��ͱ���
     *  @param  pMessage ����
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SendMessage(Wrap* pMessage) = 0;

    /** @fn     PostInnerMessage(HPR_VOID* pMessage, HPR_INT32 nDstId)
     *  @brief  �ѱ������͵�Ŀ�����ȥ����
     *  @param  pMessage ����
     *  @param  nDstId Ŀ��ỰID
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   PostInnerMessage(HPR_VOID* pMessage, HPR_INT32 nDstId) = 0;

    /** @fn     StartService(HPR_INT32 nPri)
     *  @brief  ��ʼ���Ự�ϵķ���,ע��,ÿ���Ự���뿪������ſ�ʼ����ҵ���߼�
     *  @param  nPri ���ӻỰ�Ĵ������ȼ�,ȡֵΪ(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)��
     *  @param  nRecvBufferSize ��Ϣ���ջ�������С��ʹ��Ĭ��ֵ0�����Ĭ�ϵ�TCP��UDP��������С
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   StartService(HPR_INT32 nPri, HPR_INT32 nRecvBufferSize = 0) = 0;

    /** @fn     HPR_INT32   StopService(HPR_BOOL bInThisSessionProc = HPR_FALSE)
     *  @brief  ֹͣ�Ự�ϵķ���,��ֹͣҵ���߼��Ĵ���
     *  @param  bInThisSessionProc �Ƿ���ConnSession���ڵ��̵߳��ô˺���,�����߼���ʹ��Ĭ�ϲ���
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   StopService(HPR_BOOL bInThisSessionProc = HPR_FALSE) = 0;

    /** @fn     HPR_INT32   SetSocketClosedCallBack(HPR_INT32 (*_OnSocketClosed)(ConnSession*))
     *  @brief  �������ӶϿ�ʱ(Socket�Ͽ�)ʱ�Ļص����� 
     *  @param  _OnSocketClosed ���ӶϿ��ص�����
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetSocketClosedCallBack(HPR_INT32 (*_OnSocketClosed)(ConnSession*)) = 0;

    /** @fn     HPR_INT32   SetTimeOutCallBack(HPR_BOOL (*_OnCheckTimeout)(ConnSession*))
     *  @brief  �������ӳ�ʱ�жϺ���, ע���߼�������ʱ�ж�ʱ��ز�Ҫ����������ʱ�ж����߼��ж��Լ����
     *  @param  _OnCheckTimeout��ʱ�жϻص�����,���߼���ûص���������HPR_TRUEʱ,HPP��Ͽ���·����,���ص�_OnSocketClosed
     *  @see SetSocketClosedCallBack
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetTimeOutCallBack(HPR_BOOL (*_OnCheckTimeout)(ConnSession*)) = 0;

    /** @fn     SetUserDefineDataCallBack(HPR_INT32 (*_OnRecvUserDefineData)(HPPTransferable* pUserData))
     *  @brief  �����û��Զ�������(��Ϣ)����ص�����
     *  @param  _OnRecvUserDefineData �û��Զ�������(��Ϣ)����ص�����
     *  @return HPR_OK/HPR_ERROR
     */
    virtual HPR_INT32   SetUserDefineDataCallBack(HPR_INT32 (*_OnRecvUserDefineData)(HPPTransferable* pUserData)) = 0;

    /** @fn     HPR_INT32   GetSessionId();
     *  @brief  ��ȡ��ǰ�Ự��ID
     *  @param  void
     *  @return ��ǰ�Ự��ID
     */
    virtual HPR_INT32   GetSessionId() = 0;

    /** @fn     HPR_INT32   SendMessage_r(HPR_VOID* pMessage, HPR_INT32 nDstId);
     *  @brief  ���ͱ��ĵ�Ŀ�����ӻỰ,�ú������̰߳�ȫ��.�ú����Ǿ�̬����
     *  @param  iHandle  HPP��ʼ����õ��ľ�� 
     *  @param  pMessage ���� 
     *  @param  nDstId Ŀ��ỰID
     *  @return HPR_OK/HPR_ERROR
     */
    static HPR_INT32    SendMessage_r(HPR_VOID* pMessage, HPR_INT32 nDstId);

    /** @fn     HPR_INT32   SendUserData_r(HPPTransferable *pUserData, HPR_INT32 nDstId);
     *  @brief  �����û��Զ�������(��Ϣ)��Ŀ�����ӻỰ,�ú������̰߳�ȫ��.�ú����Ǿ�̬����
     *  @param  iHandle  HPP��ʼ����õ��ľ�� 
     *  @param  pMessage �û��Զ�������(��Ϣ),��ͬ��GetUserData/SetUserData����Զ�������,����ָ�Զ��屨�ĵĸ���
     *  @param  nDstId Ŀ��ỰID
     *  @return HPR_OK/HPR_ERROR
     */
    static  HPR_INT32   SendUserData_r(HPPTransferable *pUserData, HPR_INT32 nDstId);

    /** @fn     HPR_VOID    SetServiceType(HPR_UINT16 nType) ;  
     *  @brief  ���÷�������,��ӦHPP_PACK_HEAD���service�ֶΣ�cms__vtm()0x0102/0x0201
     *  @param  nType ��������
     *  @see    GetServiceType
     *  @return HPR_VOID
     */
    virtual HPR_VOID    SetServiceType(HPR_UINT16 nType) = 0;

    /** @fn     HPR_VOID    SetServiceType(HPR_UINT16 nClientType, HPR_UINT16 nServerType)
     *  @brief  ���÷������ͣ���ӦHPP_PACK_HEAD���service�ֶΣ����ø�ͨ�õĽӿڣ�������չ��������ʱ��Ҫ��HPP
     *          Э��ջ�������Ӧ�Ķ���
     *  @param  �Ự����Ϊ�ͻ��˵ķ��������ͣ���ӦHPPЭ���з��������Ͷ��壬ȡֵ��Χ0x01 - 0xFF
     *  @param  �Ự����Ϊ����˵ķ��������ͣ���ӦHPPЭ���з��������Ͷ��壬ȡֵ��Χ0x01 - 0xFF
     *  @return HPR_VOID
     *  
     *  @note   ��PAG����CMS�ĻỰ��PAG��Ϊ�ͻ��ˣ�PAG����Ϊ0x04��CMS��Ϊ����ˣ�CMS����Ϊ0x01
     */
    virtual HPR_VOID    SetServiceType(HPR_UINT16 nClientType, HPR_UINT16 nServerType) = 0;

    /** @fn     HPR_INT32   GetServiceType()
     *  @brief  ��ȡ�������� 
     *  @param  void
     *  @see    SetServiceType
     *  @return ��������
     */
    virtual HPR_INT32   GetServiceType() = 0;

    /** @fn     HPR_VOID    SetSessionLevel(HPR_INT32 nLevel)
     *  @brief  ����Session�����ȼ�,�Ѿ���ʹ��,��һ�汾ȥ��
     *  @param  nLevel ���ȼ�
     *  @return HPR_VOID
     */
    virtual HPR_VOID    SetSessionLevel(HPR_INT32 nLevel) = 0;

    /** @fn     HPR_INT32   AddTimer(TIMER_INFO iTimeinfo)
     *  @brief  ���һ����ʱ������ǰConnSession
     *  @param  iTimeinfo ��ʱ����Ϣ
     *  @see    TIMER_INFO
     *  @return 
     */
    virtual HPR_INT32   AddTimer(TIMER_INFO iTimeinfo) = 0;

    /** @fn     HPR_VOID    DelTimer(HPR_UINT32 nTimerIndex)
     *  @brief  ɾ����ʱ��
     *  @param  nTimerIndex ��ʱ�����,AddTimer����ֵ
     *  @see    AddTimer
     *  @return HPR_VOID
     */
    virtual HPR_VOID    DelTimer(HPR_UINT32 nTimerIndex) = 0;

    /** @fn     HPR_VOID            GetUserData()
     *  @brief  �����û��Զ�������
     *  @param  pUserData �û��Զ�������
     *  @return �û��Զ������ݣ�����û��Զ�������δ���룬����NULL
     */
    virtual HPR_VOID    SetUserData(HPR_VOIDPTR pUserData) = 0;

    /** @fn     HPR_VOIDPTR         GetUserData()
     *  @brief  ��ȡ�û��Զ�������
     *  @param  void
     *  @return �û��Զ������ݣ�����û��Զ�������δ���룬����NULL
     */
    virtual HPR_VOIDPTR GetUserData() = 0;

    /** @fn     const char* GetSockRemoteIp()
     *  @brief  ��ȡԶ��IP��ַ
     *  @param  void
     *  @return Զ��IP��ַ,ʧ��ʱ����NULL
     */
    virtual const char* GetSockRemoteIp() = 0;

    /** @fn     GetSockRemotePort
     *  @brief  ��ȡԶ�˶˿�
     *  @param  void
     *  @return Զ�˶˿�,ʧ��ʱ����0
     */
    virtual HPR_UINT16  GetSockRemotePort() = 0;

    /** @fn     GetSockFd
     *  @brief  ���ConnSession��Ӧ��Socket���
     *  @param  void
     *  @return Socket���
     */
    virtual HPR_SOCK_T GetSockFd() = 0;

    static const HPR_INT32  PROTOCOL_TYPE_HPP = 0;              /**< HPP (Protocol Buffer) Э�� */
    static const HPR_INT32  PROTOCOL_TYPE_EHOME = 1;            /**< �ҵ�E�� Э�� */
    static const HPR_INT32  PROTOCOL_TYPE_MQ = 2;               /**< MQ (Protocol Buffer) Э�� */
    static const HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;          /**< ����Ự */
};


/** @fn     HPR_INT8* HPP_GetVersion();
 *  @brief  ��ȡ��ǰЭ��ջ�İ汾�š�
 *  @param  void
 *  @return �汾���ַ�������\0����
 */
HPP_DECLARE HPR_INT8*   HPP_GetVersion();

/** @fn     HPP_HANDLE HPP_Init(HPR_INT32 nWorkNumber);
 *  @brief  ��ʼ��HPPЭ��ջ,δ������,���ɶ�ε���
 *  @param  nWorkNumber HPPЭ��ջ�����߳���
 *  @param  nContextTimeout �����ĳ�ʱʱ��,����Ϊ��λ
 *  @return NULL���ʧ�ܣ���NULLֵHPPHANDLE����ɹ�
 */
HPP_DECLARE HPP_HANDLE  HPP_Init(HPR_INT32 nWorkNumber, HPR_UINT32 nContextTimeout = 30, HPR_BOOL bInitMQ = HPR_TRUE);


/** @fn     HPR_VOID HPP_Close(HPP_HANDLE iHandle);
 *  @brief  �ر�HPPЭ��ջ,һ����ϵͳ�˳�ʱ����,�����ϲ��߼�ȫ��ͣ���ٵ��ô˺�����
 *  @param  iHandle ��HPP_Init�ķ���ֵ
 *  @see    HPP_Init
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID    HPP_Close(HPP_HANDLE iHandle);


/** @fn     HPP_SERVER_HANDLE HPP_StartLocalServer(HPP_HANDLE iHandle, const char* pLocalIp, HPR_UINT16 nLocalPort, 
 *              HPR_INT32 (*_OnAcceptNewClient)(ConnSession*), HPR_INT32 Protocol);
 *  @brief  ����TCP���ؼ�������
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  pLocalIp ���ؼ���ʹ�õ�IP
 *  @param  nLocalPort ���ؼ����Ķ˿�
 *  @param  _OnAcceptNewClient �ӵ��ͻ������ӵĻص�����
 *  @param  Protocol Э������:
 *              const static HPR_INT32  PROTOCOL_TYPE_HPP = 0;
 *              const static HPR_INT32  PROTOCOL_TYPE_EHOME = 1;
 *              const static HPR_INT32  PROTOCOL_TYPE_MQ = 2;
 *              const static HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;
 *  @param  pSelfData �����Ĺ���ָ��
 *  @see    ConnSession::PROTOCOL_TYPE_HPP
 *  @see    ConnSession::PROTOCOL_TYPE_EHOME
 *  @see    ConnSession::PROTOCOL_TYPE_MQ
 *  @see    ConnSession::PROTOCOL_TYPE_VIRTUAL
 *  @see    HPP_Init 
 *  @see    HPP_StopLocalServer
 *  @return HPP_SERVER_HANDLE����ɹ�,NULL���ʧ��
 */
HPP_DECLARE HPP_SERVER_HANDLE HPP_StartLocalServer(HPP_HANDLE iHandle,
                                                   const char* pLocalIp,
                                                   HPR_UINT16 nLocalPort,
                                                   HPR_INT32 (*_OnAcceptNewClient)(ConnSession*),
                                                   HPR_INT32 Protocol,
                                                   HPR_VOID* pSelfData = NULL
                                                   );

/** @fn     HPR_VOID HPP_StopLocalServer(HPP_HANDLE iHandle, HPP_SERVER_HANDLE iServerHandle);
 *  @brief  ֹͣ���ؼ�������
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  iServerHandle HPP_StartLocalServer�ķ���ֵ
 *  @see    HPP_StartLocalServer
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID    HPP_StopLocalServer(HPP_HANDLE iHandle, HPP_SERVER_HANDLE iServerHandle);




/** @fn     HPR_INT32 HPP_ConnRemoteService(HPP_HANDLE iHandle, const char* pRemoteIp, HPR_UINT16 nRemotePort, HPR_INT32 (*_OnCompleteConn)(ConnSession*),
 *                              const char* pLocalIp, HPR_UINT16 nLocalPort, HPR_UINT32 nProtocolType, HPR_VOID* pSelfData = NULL, HPR_INT32 nPri=SERVICE_NORMAL_PRI);
 *  @brief  ����Զ�̷���,����ģʽ
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  pRemoteIp Զ�̷����IP��ַ
 *  @param  nRemotePort Զ�̷���Ķ˿�
 *  @param  pLocalIp ����ʹ�õ�IP��ַ
 *  @param  nLocalPort ����ʹ�õĶ˿�
 *  @param  nProtocolType Э������,�����ô���ֵ(0~3):
 *                        const static HPR_INT32  PROTOCOL_TYPE_HPP = 0;
 *                        const static HPR_INT32  PROTOCOL_TYPE_EHOME = 1;
 *                        const static HPR_INT32  PROTOCOL_TYPE_MQ = 2;
 *                        const static HPR_INT32  PROTOCOL_TYPE_VIRTUAL = 3;
 *  @param  pSelfData �û��Զ�������,���ӳɹ�ʱ,��_OnCompleteConn�����ConnSession::GetUserData������ȡ
 *  @param  _OnCompleteConn ���ӳɹ��Ļص�����
 *  @param  nPri ���ӻỰ�Ĵ������ȼ�,ȡֵΪ(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)��
 *               Ĭ��ΪSERVICE_NORMAL_PRI�����Ҫ�޸����ȼ�,�ڴ˴�����,ConnSession::StartService��������ȼ��Ѳ���ʹ�á�
 *  @see    ConnSession::PROTOCOL_TYPE_HPP
 *  @see    ConnSession::PROTOCOL_TYPE_EHOME
 *  @see    ConnSession::PROTOCOL_TYPE_MQ
 *  @see    ConnSession::PROTOCOL_TYPE_VIRTUAL
 *  @see    ConnSession::GetUserData
 *  @see    ConnSession::StartService
 *  @see    ConnSession
 *  @return HPR_OK/HPR_ERROR(����ʧ��)
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
 *  @brief  ����Զ�̷���,�첽����ģʽ,ע��,�ú���ֻ����HPP_Init����֮��ʹ��.
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  pRemoteIp Զ�̷����IP��ַ
 *  @param  nRemotePort Զ�̷���Ķ˿�
 *  @param  _OnCompleteConn �������(�ɹ�)ʱ�Ļص�����
 *  @param  pLocalIp �������ӵ�IP
 *  @param  nLocalPort �������ӵĶ˿�
 *  @param  pSelfData �û��Զ�������,����ʧ�ܿ�����_OnConnError��ǿ��ת����һ��������ȡ��
 *                  ���ӳɹ���_OnCompleteConn�����ConnSession::GetUserData��ȡ��
 *  @param  nPri ���ӻỰ�Ĵ������ȼ�,ȡֵΪ(SERVICE_HIGH_PRI/SERVICE_NORMAL_PRI)��
 *              Ĭ��ΪSERVICE_NORMAL_PRI�����Ҫ�޸����ȼ�,�ڴ˴�����,ConnSession::StartService��������ȼ��Ѳ���ʹ�á�
 *  @param  _OnConnError ����ʧ�ܻص�����,��̨5s��������Ŀ�꽫��ͨ���ûص�֪ͨʹ����,ʹ�õ�ǰ�ӿڱ���ָ������ʧ�ܻص�����
 *  @see    ConnSession::GetUserData
 *  @see    ConnSession::StartService
 *  @see    ConnSession
 *  @return HPR_OK����ɹ�(����ζ���ӳɹ�),HPR_ERROR���ʧ��.�������ʧ��,_OnConnError/_OnCompleteConn�����ᱻ����.
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
 *  @brief  ������MQ�ĻỰ
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  nMQClientBroekrID MQ�������ͻ��˴���ID
 *  @param  sMQQcuName MQ������QCU����
 *  @param  sMQClusterName MQ��������Ⱥ������
 *  @return ������MQ���������ӵĻỰ,����������󷵻�NULL
 */
HPP_DECLARE ConnSession* HPP_CreateMqSession(HPP_HANDLE iHandle,
                                             const char* sMQClientBrokerIp,
                                             HPR_INT32 nMQClientBrokerPort,
                                             const char* sMQQcuName,
                                             const char* sMQClusterName = "",
                                             const char* sMQJndiUrl = ""
                                             );

/** @fn     HPR_INT32    HPP_AddMqConsumer(ConnSession* pMqSession, const HPR_INT8* sUri, HPR_UINT32 nSendType);
 *  @brief  ��MQ�����ӻỰ�������Ϣ������(����MQ��Ϣ),
 *  @param  pMqSession HPP_CreateMqSession�ķ���ֵ
 *  @param  sDstName ��ϢĿ�ĵ���: ���л�������
 *  @param  nSendType ���õ�MQ��������.
 *  @return HPR_OK, �����������߳ɹ�; HPR_ERROR, ������������ʧ��
 */
HPP_DECLARE HPR_INT32   HPP_AddMqConsumer(ConnSession* pMqSession,
                                          const char* sDstName,
                                          HPR_UINT32 nSendType
                                          );

/** @fn     HPR_INT32   HPP_AddMqProducer(ConnSession* pMqSession, const char* sUri, HPR_UINT32 nSendType);
 *  @brief  ��MQ�����ӻỰ�������Ϣ������(����MQ��Ϣ)
 *  @param  pMqSession HPP_CreateMqSession�ķ���ֵ
 *  @param  sDstName ��ϢĿ�ĵ���: ���л�������
 *  @param  nSendType ���õ�MQ��������.
 *  @return ������ID, >= 0, �����������߳ɹ�; HPR_ERROR, ������������ʧ��
 */
HPP_DECLARE HPR_INT32   HPP_AddMqProducer(ConnSession* pMqSession,
                                          const char* sDstName,
                                          HPR_UINT32 nSendType
                                          );

/** @fn     ConnSession* HPP_CreateUdpSession(const char* pIp, HPR_INT16 nPort);
 *  @brief  ��������UDP�����Ự
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  pIp ���ؼ�����IP
 *  @param  nPort ���ؼ����Ķ˿�
 *  @return ���ر��ؼ����ĻỰ,����NULL�������ʧ��
 */
HPP_DECLARE ConnSession* HPP_CreateUdpSession(HPP_HANDLE iHandle, const char* pIp, HPR_INT16 nPort);

/** @fn     HPP_CreateVitualSession
 *  @brief  ����һ������ĻỰ
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  void
 *  @return ����һ������ĻỰ,����NULL�������ʧ��
 */
HPP_DECLARE ConnSession* HPP_CreateVitualSession(HPP_HANDLE iHandle);

/** @fn     HPP_GetHppSessionById
 *  @brief  ���ݻỰ��ID��ȡ�Ự
 *  @param  iSessionId �ỰID
 *  @return ������iSessionId��Ӧ�ĻỰ,��������ڷ���NULL
 */
HPP_DECLARE ConnSession* HPP_GetHppSessionById(HPR_INT32 iSessionId);

/** @fn     HPR_VOID HPP_SetEhMsgCreateFun(EHMSG_CREATE_BYCMD_FUN fun);
 *  @brief  ����E��Э�鱨�����ɻص�
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  fun �������ɻص�����
 *  @see    EHMSG_CREATE_BYCMD_FUN
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID HPP_SetEhMsgCreateFun(HPP_HANDLE iHandle, EHMSG_CREATE_BYCMD_FUN fun);

/** @fn     HPR_VOID HPP_SetPbMsgCreateFun(PBMSG_CREATE_BYCMD_FUN fun);
 *  @brief  ����HPP(Protocol BufferЭ��)�������ɻص�����
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  fun �������ɻص�����
 *  @see    PBMSG_CREATE_BYCMD_FUN
 *  @return HPR_VOID
 */
HPP_DECLARE HPR_VOID HPP_SetPbMsgCreateFun(HPP_HANDLE iHandle, PBMSG_CREATE_BYCMD_FUN fun);
HPP_DECLARE HPR_VOID HPP_DelPbMsgCreateFun(HPP_HANDLE iHandle, PBMSG_CREATE_BYCMD_FUN fun);

/** @fn     HPP_SendWrapIfToHppStack
 *  @brief  ��Ŀ������(�Ự)�ӱ���,�ú����̰߳�ȫ
 *  @param  iHandle Hpp_Init()����ֵ
 *  @param  pMessage ����
 *  @param  nDstId Ŀ�����ӻỰ
 *  @return HPR_OK/HPR_ERROR
 */
HPP_DECLARE HPR_INT32 HPP_SendWrapIfToHppStack(HPP_HANDLE iHandle, WrapIf* pMessage, HPR_INT32 nDstId);

/** @fn     HPP_GetUnRepeatSeq
 *  @brief  ��ȡһ�����ظ������,�ú����ڲ�ʵ���Ǽ����Լ�
 *  @param  void
 *  @return ����һ��HPR_UINT32�����
 */
HPP_DECLARE HPR_UINT32 HPP_GetUnRepeatSeq();


/** @fn     HPR_SOCK_T HPP_TCPConnNoBlock(const char* pRemoteIp, HPR_UINT16 nRemotePort, HPR_INT32 (*_OnCompleteConn)(HPR_VOIDPTR),
                            const char* pLocalIp, HPR_UINT16 nLocalPort, HPR_VOID* pSelfData = NULL,
                            HPR_INT32 (*_OnConnError)(HPR_VOIDPTR) = NULL);
 *  @brief  TCP�첽����
 *  @param  iHandle HPP_Init�ķ���ֵ
 *  @param  pRemoteIp Զ��IP
 *  @param  nRemotePort Զ�̶˿�
 *  @param  _OnCompleteConn���ӳɹ��ص�
 *  @param  pLocalIp ����IP
 *  @param  nLocalPort ���ض˿�
 *  @param  pSelfData �û��Զ������� _OnConnError/_OnCompleteConn��ĵ�һ������.
 *  @param  _OnConnError ����ʧ�ܻص�
 *  @return HPR_SOCK_T ������socket
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

