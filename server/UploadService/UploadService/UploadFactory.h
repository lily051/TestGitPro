#pragma once
#include "stdafx.h"
#include <list>
#include <queue>
#include <map>
#include <time.h>
#include <ATLComTime.h>
#include "../util/Singleton.h"
#include "../util/Guard.h"
#include "Center/Records.pb.h"
#include "Center/StorageConfig.pb.h"
//#include "Collect/Users.pb.h"
#include "HCNetSDK/include/HCNetSDK.h"

class CUploadFactory
{
    SINGLETON_DECLARE(CUploadFactory)///<�����������
public:
    CUploadFactory(void);
    virtual ~CUploadFactory(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
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
    /**	@fn	    AddRecordsList
    *	@brief	���¼����Ϣ�б�
    *	@param  [in] recordsList -- ¼����Ϣ�б�
    *   @param  [in] bFinish -- �Ƿ����
    *	@return	
    */
    void AddRecordsList(const cms_8120::RecordsList &recordsList, BOOL bFinish = true);

	/**	@fn	    DeleteLocalRecordFiles
    *	@brief	ɾ������¼���ļ�
    *	@param  [in] recordsList -- ¼����Ϣ�б�
    *	@return	
    */
    void DeleteLocalRecordFiles(const cms_8120::RecordsList &recordsList);

    /**	@fn	    HandleStorage
    *	@brief	����洢�豸��Ϣ
    *	@param  [in] storage -- �洢�豸��Ϣ
    *   @param  [in] bFinish -- �Ƿ����
    *	@return	
    */
    void HandleStorage(const cms_8120::RspStorageConfig &storage, BOOL bFinish = true);

    /**	@fn	    RunCycleUpload[�̺߳���]
	*	@brief	�ϴ�¼��
	*	@param  [in] ��
	*	@return	
	*/
	void RunCycleUpload();

    /**	@fn	    RunCycleGetStatus[�̺߳���]
	*	@brief	��ȡ״̬
	*	@param  [in] ��
	*	@return	
	*/
	void RunCycleGetStatus();

    /**	@fn	    RunCycleGetRecords[�̺߳���]
    *	@brief	��ȡ¼��
    *	@param  [in] ��
    *	@return	
    */
    void RunCycleGetRecords();

    /**	@fn	    RunCycleHeartBeat[�̺߳���]
    *	@brief	����
    *	@param  [in] ��
    *	@return	
    */
    void RunCycleHeartBeat();

    /**	@fn	    SetUploadPercent
    *	@brief	�����ϴ�����(KMSר��)
    *	@param  [in] nPercent -- �ϴ�����
    *	@return	
    */
    void SetUploadPercent(int nPercent);

private:
    /**	@fn	    isWorkingTime
    *	@brief	�ж��Ƿ��ڹ���ʱ����
    *	@param  [in] strStartTime -- ��ʼʱ��
    *   @param  [in] strStopTime -- ����ʱ��
    *	@return	int, -1��ʾ����0��ʾ���ڹ���ʱ���ڣ�1��ʾ�ڹ���ʱ����
    */
    int isWorkingTime(const std::string &strStartTime, const std::string &strStopTime);

    /**	@fn	    convertToCTime
    *	@brief	ת��ʱ�����ͣ�string-COleDateTime��
    *	@param  [in] strStartTime -- ��ʼʱ��
    *   @param  [in] strStopTime -- ����ʱ��
    *	@param  [out] startTime -- ��ʼʱ��
    *   @param  [out] stopTime -- ����ʱ��
    *	@return	BOOL
    */
    BOOL convertToCTime(const std::string &strStartTime, const std::string &strStopTime,
        COleDateTime &startTime,
        COleDateTime &stopTime);

    /**	@fn	    convertRecordInfo
    *	@brief	ת��¼����Ϣ
    *	@param  [in] recordInfo -- ԭ¼����Ϣ
    *   @param  [out] struRecord -- ת�����¼����Ϣ
    *	@return	BOOL
    */
    BOOL convertRecordInfo(const cms_8120::Records &recordInfo, Struct_RecordInfo &struRecord);

    /**	@fn	    uploadFailManage
    *	@brief	�ϴ�ʧ�ܴ���
    *   @param  [in] recordInfo -- ¼����Ϣ
    *   @param  [in] strErrorMsg -- ������Ϣ
    *   @param  [out] nFailTimes -- �ϴ�ʧ�ܴ���
    *	@return	void
    */
    void uploadFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes);

	/**	@fn	    lockFailManage
    *	@brief	����ʧ�ܴ���
    *   @param  [in] recordInfo -- ¼����Ϣ
    *   @param  [in] strErrorMsg -- ������Ϣ
    *   @param  [out] nFailTimes -- ����ʧ�ܴ���
    *	@return	void
    */
    void lockFailManage(const cms_8120::Records &recordInfo, const std::string& strErrorMsg, int &nFailTimes);

    /**	@fn	    setRecordTime
    *	@brief	����ʱ����Ϣ
    *	@param  [in] nTime -- ʱ���
    *	@param  [out] struTime -- ʱ����Ϣ
    *	@return	BOOL
    */
    BOOL setRecordTime(const std::string &strTime, Struct_RecordTime &struTime);

    /**	@fn	    uploadOver
    *	@brief	�ϴ���ɴ���
    *	@param  [in] struRecordInfo -- ¼����Ϣ
    *	@param  [in] strTip-- ��ʾ��Ϣ
    *	@return	BOOL
    */
    BOOL uploadOver(const int &nRecordID, const Struct_RecordInfo &struRecordInfo, std::string strTip = "");

	/**	@fn	    loginDvr
	*	@brief	��¼Dvr
	*	@param  [in] struStorage -- �豸��Ϣ
	*	@return	LONG -- ��¼���
	*/
	LONG loginDvr(const Struct_RecordInfo &struRecordInfo);

	/**	@fn	    lockDvrRecord
    *	@brief	����DVR¼��
    *	@param  [in] struRecordInfo -- ¼����Ϣ
    *	@return	BOOL
    */
    BOOL lockDvrRecord(const Struct_RecordInfo &struRecordInfo);

	/**	@fn	    getDvrTimeStamp
	*	@brief	�õ�ʱ���
	*	@param  [in]  stDvrTimeSt -- ʱ��
	*	@param  [out] iTime -- ʱ���
	*	@return	void
	*/
	void getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime);

	/**	@fn	    getRecordTimeStamp
	*	@brief	�õ�¼��ʱ���
	*	@param  [in]  stRecordTime -- ¼��ʱ��
	*	@param  [out] iTime -- ʱ���
	*	@return	void
	*/
	void getRecordTimeStamp(const Struct_RecordTime &stRecordTime, time_t &iTime);

	/**	@fn	    setUserTips
    *	@brief	������ʾ
	*	@param  [in] struRecordInfo -- ¼����Ϣ
    *	@param  [in] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
    *	@param  [out] strTip-- ��ʾ��Ϣ
    *	@return	BOOL
    */
    BOOL setUserTips(const Struct_RecordInfo &struRecordInfo, const Struct_UploadRecordParam &struRecordParam, std::string &strTip);

private:
    bool m_bInit;                       //�Ƿ񱻳�ʼ����
    bool m_bStart;                      //�Ƿ�������
    volatile bool m_bStopWorkerThread;  //ֹͣ�����߳�

    HANDLE m_hUploadThread;     //�ϴ��߳̾��
    HANDLE m_hGetStatusThread;  //��ȡ�ϴ�״̬�߳̾��
    HANDLE m_hGetRecordsThread; //��ȡ¼����Ϣ�߳̾��
    HANDLE m_hHeartBeatThread;  //�����߳�
//    HANDLE m_hGetDataSucc;//��ȡ���ݳɹ��¼�

    CRITICAL_SECTION m_csListRecords;                     //¼����Ϣ�б���
    std::list<cms_8120::Records> m_listRecords;          //¼����Ϣ����

    CRITICAL_SECTION m_csMapRecordStatus;                  //¼��״̬�б���
    std::map<int, Struct_RecordInfo> m_mapRecordStatus;    //¼��״̬�б�

    HANDLE m_hStorageEvent;                                //�洢�豸�ź���
    Struct_StorageConfig m_struStorage;                    //�洢�豸��Ϣ  
    BOOL m_bStorageReturn;                                 //��ȡ�洢�豸�ӿڵķ���ֵ

};
