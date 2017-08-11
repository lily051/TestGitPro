/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief DC���ݽ�����
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once
#include "../util/Singleton.h"
#include "verify/RspServerLogin.pb.h"

//��¼���
typedef enum _loginResult
{
    LOGIN_RESULT_FAIL = -1,//��¼ʧ��
    LOGIN_RESULT_OK = 0,//��¼�ɹ�
}ENUM_LOGIN_RESULT;

class CDBDataMgr
{
    SINGLETON_DECLARE(CDBDataMgr)///<�����������

public:
    CDBDataMgr(void);
    virtual ~CDBDataMgr(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ������
	*	@param  [in] 
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
public:
    /**	@fn	    Login
    *	@brief	��¼
    *	@param  [in] ��
    *	@return	ENUM_LOGIN_RESULT��LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
    */
    ENUM_LOGIN_RESULT Login();

    /**	@fn	    Logout
    *	@brief	�ǳ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Logout();

    /**	@fn	    AddLoginRsp
    *	@brief	��ӵ�½��Ӧ��Ϣ
    *	@param  [in] serLoginRsp ��½��Ӧ��Ϣ
    *	@return	
    */
    void AddLoginRsp(const cms_8120::RspServerLogin & serLoginRsp);

    /**	@fn	    SendLoginReq
    *	@brief	���͵�¼��Ϣ
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SendLoginReq();

    /**	@fn	    SendLogoutMsg
    *	@brief	�����˳���Ϣ
    *	@param  [in]��
    *	@return	
    */
    void SendLogoutMsg();

    /**	@fn	    SendHeartBeat
    *	@brief	������������
    *	@param  [in]��
    *	@return	
    */
    void SendHeartBeat();

    /**	@fn	    GetSysConfig
    *	@brief	��ȡ����ϵͳ��Ϣ
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetSysConfig();

    /**	@fn	    GetStorageInfo
    *	@brief	��ȡ�洢�豸����
    *	@param  [in] strStorageID -- �洢�豸ID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetStorageInfo(const HPR_INT32 &storageID);

    /**	@fn	    GetOldBzRecord
    *	@brief	��ȡ����¼����Ϣ
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetOldBzRecord();

    /**	@fn	    GetRecRecord
    *	@brief	��ȡ¼����Ϣ
    *	@param  [in] nClientId -- ClientID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetRecRecord(const int &nClientId);

    /**	@fn	    MarkOldBzRecords
    *	@brief	���clientinput���еĹ��ڼ�¼
    *	@param  [in] nClientID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int MarkOldBzRecords(const int &nClientID);

    /**	@fn	    DelClientRecords
    *	@brief	ɾ��¼���¼
    *	@param  [in] nClientId -- ClientID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int DelClientRecords(const std::string &strClientIds);

    /**	@fn	    DelCenterRecords
    *	@brief	ɾ��prosaverecord���е�¼���¼
    *	@param  [in] nRecordID -- ����¼��ID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int CDBDataMgr::DelCenterRecords(const int &nRecordID);
private:
    bool m_bInit;                                       //�Ƿ񱻳�ʼ����
    bool m_bStart;                                      //�Ƿ�������

    HANDLE m_hLoginRetEvent;                            //��¼�����ź�
    volatile bool m_bStopWorkerThread;                  //ֹͣ�����߳�
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //��������½��Ӧ��Ϣ
};
