#ifndef GUARD_RECGUARD_H
#define GUARD_RECGUARD_H
/**	@file    RecGuard.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ¼��ɾ��ʵ����
 *
 *	@author	 xuezhihu
 *	@date	 2013/3/20
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#pragma once
#include <ATLComTime.h>
#include <fclib/fcTchar.h>
#include <HPR_Singleton.h>
#include "CVR_Udd.h"
#include "Cloud_Udd.h"
#include "Udd.h"
#include "Center/OvertimeRecord.pb.h"
#include "Center/Sysconfig.pb.h"
#include "Center/Prosaverecord.pb.h"
#include "Center/Device.pb.h"
#include "HCNetSDK/include/HCNetSDK.h"
#include "hcs_sdk.h"
#include "hcs_object_api.h"

#define STORAGE_LOGIN_FAILED -1 //�洢�豸��¼���

// work param
HPR_INT32 const WP_STORAGE_TYPE = 10009;
HPR_INT32 const WP_TIP_DAY = 6106;
HPR_INT32 const WP_STARTTIME = 6104;
HPR_INT32 const WP_STOPTIME = 6105;

class CRecGuard : public singleton<CRecGuard>
{
    friend class singleton<CRecGuard>;
private:
    CRecGuard(void);
    ~CRecGuard(void);
public:
    int Init(void);
    int Fini(void);
    int Start(void);
    int Stop(void);
    /**	@fn	    GetSysConfig
    *	@brief	��ȡ����ϵͳ��Ϣ
    *	@param  [in] stWorkParam -- ��������
    *	@param  [out] stWorkParam -- ��������
    *	@return	bool
    */
    bool GetSysConfig(work_param_t &stWorkParam);

    /**	@fn	    HandleStorage
    *	@brief	��������ϵͳ��Ϣ
    *	@param  [in] sysconfigList -- �洢�豸��Ϣ
    *   @param  [in] bFinish -- �Ƿ����
    *	@return	
    */
    void HandleSysConfig(const cms_8120::SysconfigList &sysconfigList, BOOL bFinish = true);

    /**	@fn	    HandleOverRecord
    *	@brief	�������¼����Ϣ
    *	@param  [in] overtimeRecord -- ����¼����Ϣ
    *   @param  [in] bFinish -- �Ƿ����
    *	@return	
    */
    void HandleOverRecord(const cms_8120::RspOvertimeRecord &overtimeRecord, BOOL bFinish = true);

    /**	@fn	    HandleRecord
    *	@brief	����¼����Ϣ
    *	@param  [in] prosaverecordList -- ¼����Ϣ
    *   @param  [in] bFinish -- �Ƿ����
    *	@return	
    */
    void HandleRecord(const cms_8120::ProsaverecordList &prosaverecordList, BOOL bFinish = true);

    /**	@fn	    HandleStorageInfo
    *	@brief	����洢����Ϣ
    *	@param  [in] storage -- �洢����Ϣ
    *	@return	bool
    */
    void HandleStorageInfo(const cms_8120::DeviceList &storage);

    /**	@fn	    GetStorageInfo
    *	@brief	��ȡ�洢��Ϣ
    *	@param  [in] storageID -- �洢��ID
    *	@param  [out] storage -- �洢����Ϣ
    *	@return	bool
    */
    bool GetStorageInfo(const HPR_INT32 &storageID, storage_info_t &storage);
private:
    // �����̣߳���Ҫ���������
    static DWORD WINAPI WT_Manage(LPVOID lpParam);
    DWORD ManageProc(void);

    // �����߳�
    static DWORD WINAPI WT_HeartBeat(LPVOID lpParam);
    DWORD HeartBeatProc(void);

    // �����̣߳�����¼����ڱ�Ǻ�ɾ���߼�
    static DWORD WINAPI WT_RecGuard(LPVOID lpParam);
    DWORD RecGuardProc(void);
    bool GetOldBzRecord(std::vector<bz_record_t> &bzRecords);
    bool DelOldBzRecordByIDs(const std::vector<HPR_INT32> &bzRecordIDs);
    bool GetRecRecord(const HPR_INT32 recordID, std::vector<rec_record_t> &recRecords);
    bool DelRecRecordByIDs(const int &nRecordID);
    bool ProcessOldBzRecord(const bz_record_t &bzRecord);
    bool ProcessRecRecord(const rec_record_t &recRecord);
    static bool GetTimeFromTimeString(LPCTSTR timeString, __time32_t &t);
    static bool GetHcsTimeFromTimeString(LPCTSTR timeString, HCS_Time &t);
    static bool GetNetTimeFromTimeString(LPCTSTR timeString, NET_DVR_TIME &t);

    bool MarkOldBzRecords(const int &nClientID);

    bool IsWorkingTime(void);

    bool ConvertToCTime(const work_param_t &workParam,
        COleDateTime &startTime,
        COleDateTime &stopTime);

    bool StartWorker(void);
    bool StopWorker(void);

    /**	@fn	    DelayBySleep
     *	@brief	ͨ��ѭ��sleep�ķ�ʽ�ȴ�һ��ʱ��
     *	@param  [in]dwMilliseconds, ��Ҫ�ȴ���ʱ�䣬��λ����
     *	@param  [in]stopFlag, ��־�����Ϊtrue��ȴ���dwMilliseconds��
     *                        �������ǰ����
     *	@param  [in]dwInterval, �ȴ��������λ����
     *	@return	void
     */
    static void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);
   
    /**	@fn	    LoginCVR
     *	@brief	��¼CVR
     *	@param  [in]struCVRInfo -- CVR����
     *	@return	LONG -- ��¼���
     */
    LONG LoginCVR(const storage_info_t& struCVRInfo);

    /**	@fn	    LockFileInCVR
     *	@brief	����CVR�ϵ�¼���ļ�
     *	@param  [in]lLoginID -- ��¼���
     *	@param  [in]strCameraID -- ��ID
     *	@param  [in]struStartTime -- ¼��ʼʱ��
     *	@param  [in]struStopTime -- ¼�����ʱ��
     *	@return	bool
     */
    bool LockFileInCVR(const LONG& lLoginID, const std::string& strCameraID, 
        const NET_DVR_TIME& struStartTime, const NET_DVR_TIME& struStopTime);

    /** @fn GetHttpToken
    *  @brief  ��KMS��ȡToken��֤��Ϣ
    *  @param [in]struKMSInfo KMS��������Ϣ
    *  @param [out]strToken ��ȡ����token��Ϣ
    *  @return bool
    */
    bool GetHttpToken(const storage_info_t& struKMSInfo, std::string& strToken);

    /** @fn DeleteFileInKMS
    *  @brief  ��KMS��ȡToken��֤��Ϣ
    *  @param [in]struKMSInfo KMS��������Ϣ
    *  @param [in]strToken token��Ϣ
    *  @param [in]strFileId �ļ�ID
    *  @return bool
    */
    bool DeleteFileInKMS(const storage_info_t& struKMSInfo, const std::string& strToken, const std::_tstring& strFileId);

	 /** @fn UnLockDvrRecord
    *  @brief  ����ǰ���豸¼��
    *  @param [in]struStorageInfo���豸��½��Ϣ
	*  @param [in]recRecord��¼����Ϣ
    *  @return bool
    */
    bool UnLockDvrRecord(const storage_info_t& struStorageInfo, const rec_record_t &recRecord);

	/**	@fn	    getDvrTimeStamp
	*	@brief	�õ�ʱ���
	*	@param  [in]  stDvrTimeSt -- ʱ��
	*	@param  [out] iTime -- ʱ���
	*	@return	BOOL
	*/
	void getDvrTimeStamp(NET_DVR_TIME &stDvrTimeSt, time_t &iTime);


public:
    work_param_t m_struWorkParam;   //����ϵͳ����
    std::vector<bz_record_t> m_vecBzRecord; //����¼����Ϣ
    std::vector<rec_record_t> m_vecRecRecord;   //¼����Ϣ
    storage_info_t m_struStorage;               //�洢�豸
	std::map<std::string, HCSClient *> m_mapHCSClient;    //��׼��-����洢��ʼ�����map��key���ƴ洢IP

private:
    HANDLE m_hThreadManage;
    HANDLE m_hThreadHeartBeat;
    HANDLE m_hThreadCVRGuard;
    HANDLE m_hThreadRecGuard;
    volatile bool m_bStopWorker;
    volatile bool m_bStopServer;

    HANDLE m_hRecordEvent;      //¼����Ϣ
    HANDLE m_hWorkParamEvent;   //��������
    HANDLE m_hStorageDevEvent;  //�洢�豸
    HANDLE m_hOverRecordEvent;  //����¼����Ϣ
};

inline CRecGuard* GetRecGuardPtr(void)
{
    return CRecGuard::instance();
}

#endif // end of GUARD_RECGUARD_H
