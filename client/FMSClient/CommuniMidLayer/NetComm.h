#pragma once
/**	@file    CNetComm.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   通信中间层接口函数
*
*	@author	 yudan
*	@date	 2012/05/07
*	@note
*	@note    历史记录：
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
    SINGLETON_DECLARE(CNetComm)///<单例相关声明
private:
    CNetComm();
    ~CNetComm();

    std::map<int, int> m_mapCmdSessionId;

public:

    CEventPool m_poolEvent;

	std::vector<SYNCRSP_CONTEXT> m_vSyncRspContext;  //等待返回消息
	CRITICAL_SECTION m_lckSyncRspContext;  //等待返回消息信号量

	std::vector<ASYNRSP_CONTEXT> m_vAsynRspContext;    //异步消息保存列表
	CRITICAL_SECTION m_lckAsynRspContext;  //异步消息控制信号量

    AsyncMsgCheckThread m_CheckThread;  //检查超时消息线程

    int m_nMsgTimeout;  // 消息返回超时阈值

public:

	/** @fn GetComponentID
	*   @brief 获取目标组件ID
	*   @param[in] nCmdID: 消息命令码
	*   @param NULL
	*   @return 组件ID
	*/
	static int GetComponentID(int nCmdID);

	/** @fn CheckTimeoutMsg
	*   @brief 超时消息检查线程
	*   @param[in] lpvoid: 自定义参数
	*   @param NULL
	*   @return NULL
	*/
	void CheckTimeoutMsg(void);

public:
	/** @fn Init
	*   @brief 网络初始化
	*   @param NULL
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	BOOL Init();

	/** @fn Fini
	*   @brief 网络资源释放
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
    *   @brief 组件消息
    *   @param[in] msgReq: 请求消息
    *   @param[in] nCmdID: 请求消息命令码
    *   @param[in] reqMsgType: 请求消息类型
    *   @param[in] stServerInfo: 目标服务器信息
    *   @param[in] nComponentID: 目标组件标识
    *   @param[out] msgRsp: 返回消息引用
    *   @param[in] nCmdRsp: 返回消息命令码
    *   @param[in] nTimeout: 等待超时时间
    *   @return 见TRANSMIT枚举值
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
	*   @brief 连接中心服务器
	*   @param[in] stServerInfo
	*   @param NULL
	*   @return TRUE成功，FALSE失败
	*/
	BOOL ConnectCms(SERVER_INFO stServerInfo);

    void RecievingTransmissionEx(string& pbmsg, const HPR_UINT32 nCommand);

    void RecievingResponseEx(string& pbmsg
        , const HPR_INT32 nSrcID
        , const HPR_UINT32 nSeq
        , const HPR_UINT32 nCommand);

    /** @fn AddSessionId
        @brief 将SessionId保存到一个map中。为了将报警消息同步到其他客户端而作的特殊处理
        @param nCmdID 命令，如果nCmdID == cms_8100::CMD_MSG_ACTION，就将nSessionId保存起来
        @param nSessionId
        @return TRUE成功
    */
    BOOL AddSessionId(int nCmdID, int nSessionId);
};
