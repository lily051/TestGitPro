/**	@file ClientConnSessionMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief �ͻ������ӻỰ����
*	@author		liangjungao
*	@date		2012-4-26 16:01:53
*	@note       
*/
#pragma once
#include "../util/Singleton.h"
#include <string>
#include <map>
#include <hpp_hpp.h>//<HPPЭ��ջ��ͷ�ļ�
#include "ConnSessionInfo.h"
#include <Type/cmd.pb.h>
#include "AlarmMsg.pb.h"

#define HPP_WORKER_THREAD	 10	//HPP�����߳���
#define HPP_TIMEOUT			 10	//HPP��ʱʱ��.��λ��

#define RECONNECT_OVER_TIME     5 //����ʱ��

class CConnSessionMgr
{
    SINGLETON_DECLARE(CConnSessionMgr)///<�����������

public:
    /**	@fn	    Init
    *	@brief	��ʼ������
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Init();

    /**	@fn	    Fini
    *	@brief	����ʼ��������������Դ�ͷ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Fini();

    /**	@fn	    Start
    *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
    *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Start();

    /**	@fn	    Stop
    *	@brief	ֹͣ����
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Stop();

    /**	@fn	    Connect
    *	@brief	����
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Connect(const std::string & ip_, int port_);

    /**	@fn	    Disconnect
    *	@brief	�Ͽ�����
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Disconnect(const std::string & ip_, int port_);

    /**	@fn	    IsConnect
    *	@brief	�ж��Ƿ�������
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	true���ӣ�falseû������
    */
    bool IsConnect(const std::string & ip_, int port_);
    
    /**	@fn	    SendMsg
    *	@brief	������Ϣ
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SendMsg(const std::string & ip_, int port_, cms_8120::CMD cmd_, void * pMsg);

    /**	@fn	    SendAlarmMsg
    *	@brief	���ͱ�����Ϣ
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SendAlarmMsg(const std::string & ip_, int port_, AlarmMsg::AlarmId cmd_, void * pMsg);

    /**	@fn	    StartListen
    *	@brief	��������
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int StartListen(const std::string & ip_, int port_);

    /**	@fn	    StopListen
    *	@brief	ֹͣ����
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int StopListen();
public:
    /**	@fn	    AddConnSession
    *	@brief	��ӻỰSession
    *	@param  [in] pConnSession_ 
    *	@param  [out] ��
    *	@return	std::string ����
    */
    void AddConnSession(ConnSession * pConnSession_);

    /**	@fn	    CloseConnSession
    *	@brief	�رջỰ
    *	@param  [in] pConnSession_ 
    *	@param  [out] ��
    *	@return	std::string ����
    */
    void CloseConnSession(ConnSession * pConnSession_);

    /**	@fn	    RunCycleReconnect
    *	@brief	�����̺߳���
    *	@param  [in]  
    *	@param  [out] ��
    *	@return	
    */
    void RunCycleReconnect();
private:
    /**	@fn	    FormatKey
    *	@brief	�γ�����
    *	@param  [in] ip_ ip
    *	@param  [in] port_ �˿�
    *	@param  [out] ��
    *	@return	std::string ����
    */
    inline std::string FormatKey(const std::string & ip_, int port_);

    /**	@fn	    ClearConnect
    *	@brief	������е�������Ϣ
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	std::string ����
    */
    void ClearConnect();
public:
    CConnSessionMgr(void);
    virtual ~CConnSessionMgr(void);
private:
    std::map<std::string,CConnSessionInfo> m_mapConnSessionInfo;//���е����ӻỰ��Ϣ
    typedef std::map<std::string,CConnSessionInfo>::iterator ConnSessionInfoIterator;
    CRITICAL_SECTION m_csConnSesssionLock;
    bool m_bInit;//�Ƿ��ʼ��
    bool m_bStart;//�Ƿ�����
    HPP_HANDLE m_hHppHandle;//HPP���ʼ������ֵ
    HANDLE m_hReconnectThread;//�����߳�
    volatile bool m_bStopWorkerThread;//ֹͣ�����߳�
    HPP_SERVER_HANDLE m_hServerHandle;//���ط���
};

