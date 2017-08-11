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
#include "Center/Collectconfig.pb.h"

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

    /**	@fn	    GetRecordByStatus
    *	@brief	����¼��״̬��ȡ¼����Ϣ
    *	@param  [in] nStatus -- ¼��״̬
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetRecordByStatus(/*ENUM_RECORD_STATUS nStatus*/);

	/**	@fn	    GetUploadedRecords
    *	@brief	��ȡĳһʱ��Σ�web���ã������ϴ�¼����Ϣ
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetUploadedRecords();

    /**	@fn	    SetRecordStatus
    *	@brief	����¼��״̬
    *	@param  [in] nID -- ¼��ID
    *	@param  [in] nStatus -- ¼��״̬
    *	@param  [in] nPercent -- �ϴ�����
	*	@param  [in] strTip -- ��ʾ��Ϣ
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SetRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const int &nTaskId = -1, const int &nPercent = 0, std::string strTip = "");

    /**	@fn	    SetRecordFail
    *	@brief	����¼��ʧ��״̬
    *	@param  [in] nID -- ¼��ID
    *	@param  [in] strErrorMsg -- ������Ϣ
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SetRecordFail(const int &nID, const std::string& strErrorMsg);

    /**	@fn	    SetKmsRecordStatus(KMSר��)
    *	@brief	����¼��״̬(KMSר��)
    *	@param  [in] nID -- ¼��ID
    *	@param  [in] nStatus -- ¼��״̬
    *	@param  [in] strFiledID -- �ļ�ID
    *	@param  [in] strRtspPath -- �ļ���KMS��RTSP·��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SetKmsRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const std::string &strFiledID, 
		const std::string &strRtspPath, const int &nFileSize, const std::string &strStartTime, const std::string &strEndTime);

	 /**	@fn	    SetObjectCloudRecordStatus(�����ƴ洢ר��)
    *	@brief	����¼��״̬(�����ƴ洢ר��)
    *	@param  [in] nID -- ¼��ID
    *	@param  [in] nStatus -- ¼��״̬
    *	@param  [in] strFiledID -- �ļ�ID
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SetObjectCloudRecordStatus(const int &nID, const ENUM_RECORD_STATUS nStatus, const std::string &strFiledID, const std::string &strStartTime, const std::string &strEndTime);

    /**	@fn	    GetStorageInfo
    *	@brief	��ȡ�洢�豸����
    *	@param  [in] strIP -- �ɼ���IP
    *                nPort -- �ɼ����˿�
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetStorageInfo(const std::string &strIP, const int &nPort);

private:
    bool m_bInit;                                       //�Ƿ񱻳�ʼ����
    bool m_bStart;                                      //�Ƿ�������

    HANDLE m_hLoginRetEvent;                            //��¼�����ź�
    volatile bool m_bStopWorkerThread;                  //ֹͣ�����߳�
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //��������½��Ӧ��Ϣ
};
