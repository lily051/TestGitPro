#pragma once
/**	@file    CNetComm.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ͨ���м��ӿں���
*
*	@author	 yudan
*	@date	 2012/05/07
*	@note
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/05/07 by yudan
*/

#include <hpp_hpp.h>
#include <vector>
#include <map>
#include "./NetInfoApp.h"
#include "./EventPool.h"
#include "./TimerThread.h"
//#include "../CentralClient/ComponentDef.h"
#include "AppDef.h"
#include "../FMSClient/utils/Singleton.h"

using ::google::protobuf::Message;

class CNetComm
{
    SINGLETON_DECLARE(CNetComm)///<�����������
private:
    CNetComm();
    ~CNetComm();

    std::map<int, int> m_mapCmdSessionId;

public:

    CEventPool m_poolEvent;

	std::vector<SYNCRSP_CONTEXT> m_vSyncRspContext;  //�ȴ�������Ϣ
	CRITICAL_SECTION m_lckSyncRspContext;  //�ȴ�������Ϣ�ź���

	std::vector<ASYNRSP_CONTEXT> m_vAsynRspContext;    //�첽��Ϣ�����б�
	CRITICAL_SECTION m_lckAsynRspContext;  //�첽��Ϣ�����ź���

    AsyncMsgCheckThread m_CheckThread;  //��鳬ʱ��Ϣ�߳�

    int m_nMsgTimeout;  // ��Ϣ���س�ʱ��ֵ

public:

	/** @fn GetComponentID
	*   @brief ��ȡĿ�����ID
	*   @param[in] nCmdID: ��Ϣ������
	*   @param NULL
	*   @return ���ID
	*/
	static int GetComponentID(int nCmdID);

	/** @fn CheckTimeoutMsg
	*   @brief ��ʱ��Ϣ����߳�
	*   @param[in] lpvoid: �Զ������
	*   @param NULL
	*   @return NULL
	*/
	void CheckTimeoutMsg(void);

public:
	/** @fn Init
	*   @brief �����ʼ��
	*   @param NULL
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	BOOL Init();

	/** @fn Fini
	*   @brief ������Դ�ͷ�
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	void Fini();

    enum {
        TRANSMIT_FAIL = -3,
        TRANSMIT_TIMEOUT = -2,
        TRANSMIT_NO_CONN = -1,
        TRANSMIT_ERROR = 0,
        TRANSMIT_SUCC = 1,
    };

	/** @fn TransmitCompntMsgEx
    *   @brief �����Ϣ
    *   @param[in] msgReq: ������Ϣ
    *   @param[in] nCmdID: ������Ϣ������
    *   @param[in] reqMsgType: ������Ϣ����
    *   @param[in] stServerInfo: Ŀ���������Ϣ
    *   @param[in] nComponentID: Ŀ�������ʶ
    *   @param[out] msgRsp: ������Ϣ����
    *   @param[in] nCmdRsp: ������Ϣ������
    *   @param[in] nTimeout: �ȴ���ʱʱ��
    *   @return ��TRANSMITö��ֵ
    */
    int TransmitCompntMsgEx(string& msgReq
        , HPR_UINT32 nCmdID
        , int reqMsgType
        , SERVER_INFO stServerInfo
        , int nComponentID
        , HPR_UINT32& nCmdRsp
        , string& msgRsp
        , int nTimeout = 0);

	/** @fn ConnectCms
	*   @brief �������ķ�����
	*   @param[in] stServerInfo
	*   @param NULL
	*   @return TRUE�ɹ���FALSEʧ��
	*/
	BOOL ConnectCms(SERVER_INFO stServerInfo);

    void RecievingTransmissionEx(string& pbmsg, const HPR_UINT32 nCommand);

    void RecievingResponseEx(string& pbmsg
        , const HPR_INT32 nSrcID
        , const HPR_UINT32 nSeq
        , const HPR_UINT32 nCommand);

    /** @fn AddSessionId
        @brief ��SessionId���浽һ��map�С�Ϊ�˽�������Ϣͬ���������ͻ��˶��������⴦��
        @param nCmdID ������nCmdID == cms_8100::CMD_MSG_ACTION���ͽ�nSessionId��������
        @param nSessionId
        @return TRUE�ɹ�
    */
    BOOL AddSessionId(int nCmdID, int nSessionId);
};
