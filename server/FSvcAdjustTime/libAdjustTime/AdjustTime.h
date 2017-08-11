/**	@file    AdjustTime.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   Уʱ������
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#ifndef GUARD_ADJUSTTIME_H
#define GUARD_ADJUSTTIME_H

#include <list>
#include <queue>
#include <map>
#include <sstream>
#include <time.h>
#include <ATLComTime.h>
#include <fclib/fcTchar.h>
#include <HCNetSDK.h>
#include "hcs_sdk.h"
#include "hcs_object_api.h"
#include <HPR_Singleton.h>
#include "Center/Device.pb.h"
#include "../lib/curl/curl.h"
#include "../lib/jsoncpp/include/json.h"

class CAdjustTime : public singleton<CAdjustTime>
{
    friend class singleton<CAdjustTime>;
private:
    CAdjustTime(void);
    ~CAdjustTime(void);

   /**	@fn	    CheckCVR
	*	@brief	���CVR�Ƿ������Լ�CVR�����Ƿ��㹻
	*	@param  [in] struStorage -- CVR��½��Ϣ
	*	@return	bool
	*/
	bool CheckCVR(const Struct_StorageDevice &struStorage);

   /**	@fn	    CheckCloud
	*	@brief	����ƴ洢�Ƿ������Լ��ƴ洢�����Ƿ��㹻
	*	@param  [in] struStorage -- �ƴ洢��½��Ϣ
	*	@return	bool
	*/
	bool CheckCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    CheckKMS
	*	@brief	���KMS�Ƿ������Լ�KMS�����Ƿ��㹻
	*	@param  [in] struStorage -- KMS��½��Ϣ
	*	@return	bool
	*/
	bool CheckKMS(const Struct_StorageDevice &struStorage);

	 /**	@fn	    CheckObjectCloud
	*	@brief	�������ƴ洢�Ƿ������Լ������ƴ洢�����Ƿ��㹻
	*	@param  [in] struStorage -- �����ƴ洢��½��Ϣ
	*	@return	bool
	*/
	bool CheckObjectCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    LoginCloud
	*	@brief	��¼�ƴ洢
	*	@param  [in] struStorage -- �ƴ洢��½��Ϣ
	*	@return	LONG �ƴ洢��¼���
	*/
	LONG LoginCloud(const Struct_StorageDevice &struStorage);

   /**	@fn	    GetCloudPoolInfo
    *	@brief	��ȡCloud���ݳ���Ϣ
    *	@param  [in] loginID,     �ƴ洢��½���
    *	@param  [out]vecPoolInfo, ���ݳ���Ϣ����
    *	@return	bool
    */
    bool GetCloudPoolInfo(LONG loginID, std::vector<HCS_PoolInfo>& vecPoolInfo);

	/**	@fn	    GetKmsSpaceInfo
	 *	@brief	��ȡKMSʣ����������������С
	 *	@param  [in]struStorage, KMS��Ϣ
	 *	@param  [in]strToken, token��֤��Ϣ
	 *	@param  [out]dFreeSpace,  KMSʣ������
	 *	@param  [out]dTotalSpace, KMS������
	 *	@return	bool
	 */
    bool GetKmsSpaceInfo(const Struct_StorageDevice &struStorage, const std::string& strToken, double& dFreeSpace, double& dTotalSpace);

	/**	@fn	    CheckCVRCapacity
	 *	@brief	���CVRӲ�������Ƿ��㹻�������������Ϣ
	 *	@param  [in]struCvrHDInfo, CVRӲ����Ϣ
	 *	@param  [out]alarmInfo, ������Ϣ
	 *	@return	void
	 */
	void CheckCVRCapacity(const NET_DVR_SINGLE_HD &struCvrHDInfo, std::_tstring &alarmInfo);

	/**	@fn	    CheckCloudCapacity
	 *	@brief	����ƴ洢�����Ƿ��㹻�������������Ϣ
	 *	@param  [in]struPoolInfo, �ƴ洢��Դ����Ϣ
	 *	@param  [out]alarmInfo, ������Ϣ
	 *	@return	void
	 */
	void CheckCloudCapacity(const HCS_PoolInfo &struPoolInfo, std::_tstring &alarmInfo);

	/**	@fn	    CheckKmsCapacity
	 *	@brief	���KMS�����Ƿ��㹻�������������Ϣ
	 *	@param  [in]dFreeSpace,  KMSʣ������
	 *	@param  [in]dTotalSpace, KMS������
	 *	@param  [out]alarmInfo, ������Ϣ
	 *	@return	void
	 */
    void CheckKmsCapacity(const double& dFreeSpace, const double& dTotalSpace, std::_tstring &alarmInfo);

	/**	@fn	    CheckObjectCloudCapacity
	 *	@brief	�������ƴ洢�����Ƿ��㹻�������������Ϣ
	 *	@param  [in]bucket_info, �����ƴ洢��Դ����Ϣ
	 *	@param  [out]alarmInfo, ������Ϣ
	 *	@return	void
	 */
	void CheckObjectCloudCapacity(HCSBucketInfo* bucket_info, std::_tstring &alarmInfo);

   /** @fn GetHttpToken
	*  @brief  ��KMS��ȡToken��֤��Ϣ
	*  @param [in]struStorage KMS��������Ϣ
	*  @param [out]strToken ��ȡ����token��Ϣ
	*  @return bool
	*/
	bool GetHttpToken(const Struct_StorageDevice &struStorage, std::string& strToken);

	/**	@fn		TestRemoteHost
	*   @brief    ����Զ�������Ƿ�����
	*   @param  [in]  szIP, Զ������IP��ַ
	*   @param  [in]  nPort,Զ�������˿�
	*   @return	BOOL, TRUE ���� FALSE ������
	*/
	BOOL TestRemoteHost(const char *szIP, USHORT nPort);

   /**	@fn	    ConvertCollectInfo
	*	@brief	ת���ɼ��豸��Ϣ
	*	@param  [in] collectInfo -- ԭ�ɼ��豸��Ϣ
	*   @param  [out]stCollectCfg -- ת����Ĳɼ��豸��Ϣ
	*	@return	BOOL
	*/
	BOOL ConvertCollectInfo(const cms_8120::Device &collectInfo, collect_info_t &stCollectCfg);

   /**	@fn	    convertStorageInfo
	*	@brief	ת���洢�豸��Ϣ
	*	@param  [in] storageInfo -- ԭ�洢�豸��Ϣ
	*   @param  [out] struStorage -- ת����Ĵ洢�豸��Ϣ
	*	@return	BOOL
	*/
	BOOL ConvertStorageInfo(const cms_8120::Device &storageInfo, Struct_StorageDevice &struStorage);

   /** @fn     GetCollectTimeStamp
	*  @brief  ��ȡ�ɼ��豸��ǰʱ���ʱ���
	*  @param  [in]lUserId, �ɼ��豸��½���
	*  @param  [out]iCollectTimeStamp, �ɼ��豸��ǰʱ���ʱ���
	*  @return ���� ADJUST_ERROR_SUCC
			   �쳣 �� ADJUST_ERROR_SUCC
	*/
    int GetCollectTimeStamp(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError);

   /** @fn    SetCollectTime
	*  @brief  ���òɼ��豸ʱ��
	*  @param  [in]lUserId, �ɼ��豸��½���
	*  @param  [in]iCollectTimeStamp, ʱ���
	*  @param  [out]dwError, �쳣��
	*  @return ���� ADJUST_ERROR_SUCC
			   �쳣 �� ADJUST_ERROR_SUCC
	*/
    int SetCollectTime(long lUserId, time_t &iCollectTimeStamp, DWORD& dwError);

   /** @fn     strTime
	*  @brief  ʱ���ת��Ϊʱ���ַ���
	*  @param  [in]t, ʱ���
	*  @return ʱ���ַ���
	*/
	std::_tstring strTime(__time32_t const t);

   /**	@fn	    Deciphering
    *	@brief	���ܲ���
    *	@param  [in] strPwd -- ��������
    *	@param  [out] strPwd -- ��������
    *	@return	void
    */
    void Deciphering(std::string& strPwd);

    /**	@fn	    DelayBySleep
     *	@brief	ͨ��ѭ��sleep�ķ�ʽ�ȴ�һ��ʱ��
     *	@param  [in]dwMilliseconds, ��Ҫ�ȴ���ʱ�䣬��λ����
     *	@param  [in]stopFlag, ��־�����Ϊtrue��ȴ���dwMilliseconds��
     *                        �������ǰ����
     *	@param  [in]dwInterval, �ȴ��������λ����
     *	@return	void
     */
    void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);

    //��ʼ����
    bool StartWorker(void);

public:

   /**	@fn	    GetAllCollectDeviceReq
	*	@brief	�����ȡ�ɼ��豸��Ϣ�б�
	*	@param  [out] listCollectDevice -- �ɼ��豸��Ϣ�б�
	*   @param  [in] bFinish -- �Ƿ����
	*	@return	bool
	*/
	bool GetAllCollectDeviceReq(std::list<collect_info_t>& listCollectDevice);

   /**	@fn	    GetAllCollectDeviceRsp
	*	@brief	���DVR�豸��Ϣ�б�
	*	@param  [in] deviceList -- DVR�豸��Ϣ�б�
	*   @param  [in] bFinish -- �Ƿ����
	*	@return	void
	*/
    void GetAllCollectDeviceRsp(const cms_8120::DeviceList &deviceList, BOOL bFinish = true);

   /**	@fn	    GetAllStorageDeviceReq
	*	@brief	�����ȡ�洢�豸��Ϣ�б�
	*	@param  [out] listStorageDevice -- �洢�豸��Ϣ�б�
	*   @param  [in] bFinish -- �Ƿ����
	*	@return	bool
	*/
	bool GetAllStorageDeviceReq(std::list<Struct_StorageDevice>& listStorageDevice);

   /**	@fn	    GetAllStorageDeviceRsp
	*	@brief	��Ӵ洢�豸��Ϣ�б�
	*	@param  [in] deviceList -- �洢�豸��Ϣ�б�
	*   @param  [in] bFinish -- �Ƿ����
	*	@return	void
	*/
    void GetAllStorageDeviceRsp(const cms_8120::DeviceList &deviceList, BOOL bFinish = true);

public:
    /** @fn     Init
     *  @brief  ��ʼ������
     *  @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
     */
    int Init(void);

    /** @fn     Fini
     *  @brief  ����ʼ������
     *  @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
     */
    int Fini(void);

    /** @fn     Start
     *  @brief  ��ʼѲ��
     *  @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
     */
    int Start(void);

	/** @fn     Stop
     *  @brief  ֹͣѲ��
     *  @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
     */
    int Stop(void);

   /** @fn	    ImmAdjustOnce
    *   @brief  ����ִ��һ��Уʱ����
    *   @return ���� ADJUST_ERROR_SUCC
                �쳣 �� ADJUST_ERROR_SUCC
    */
    int ImmAdjustOnce(void);

   /** @fn     AdjustTime
	*  @brief  ���������Ļ�ȡ���вɼ��豸��Ϣ ��ÿ���ɼ��豸����Уʱ
	*  @return ���� ADJUST_ERROR_SUCC
			   �쳣 �� ADJUST_ERROR_SUCC
	*/
    int AdjustTime(void);

   /** @fn     ManageProc
    *  @brief  ��������߳�
    *  @return ���� ADJUST_ERROR_SUCC
               �쳣 �� ADJUST_ERROR_SUCC
    */
    DWORD ManageProc(void);

   /** @fn    AdjustTimeProc
	*  @brief  �ɼ��豸Уʱ�߳�
	*  @return ���� ADJUST_ERROR_SUCC
			   �쳣 �� ADJUST_ERROR_SUCC
	*/
    DWORD AdjustTimeProc(void);

   /** @fn     AddAlarmProc
	*  @brief  ��������߳�
	*  @return ���� ADJUST_ERROR_SUCC
			   �쳣 �� ADJUST_ERROR_SUCC
	*/
    DWORD AddAlarmProc(void);

   /** @fn     DisAlarmProc
	*  @brief  ���������߳�
	*  @return ���� ADJUST_ERROR_SUCC
		�쳣 �� ADJUST_ERROR_SUCC
	*/
    DWORD DisAlarmProc(void);

   /** @fn    CollectPatrolProc
	*  @brief  �ɼ��豸Ѳ���߳�
	*  @return ���� ADJUST_ERROR_SUCC
		�쳣 �� ADJUST_ERROR_SUCC
	*/
    DWORD CollectPatrolProc(void);

   /** @fn     StoragePatrolProc
	*  @brief  �洢�豸Ѳ���̣߳�����豸����״̬���豸������С
	*  @return ���� ADJUST_ERROR_SUCC
		�쳣 �� ADJUST_ERROR_SUCC
	*/
	DWORD StoragePatrolProc(void);

   /** @fn     ImmAjustProc
    *  @brief  ����Уʱһ�δ����߳�
    *  @return ���� ADJUST_ERROR_SUCC
               �쳣 �� ADJUST_ERROR_SUCC
    */
    DWORD ImmAjustProc(void);

   /** @fn     HeartBeatProc
    *  @brief  �����߳�
    *  @return ���� ADJUST_ERROR_SUCC
               �쳣 �� ADJUST_ERROR_SUCC
    */
    DWORD HeartBeatProc(void);

private:
    HANDLE        m_hThreadManager;        //�����߳̾��
	HANDLE        m_hThreadHeartBeat;      //�����߳̾��
    HANDLE        m_hThreadAdjustTime;     //Уʱ�����߳̾��
    HANDLE        m_hThreadAddAlarm;       //��ӱ����߳̾��
    HANDLE        m_hThreadDisAlarm;       //���������߳̾��
    HANDLE        m_hThreadCollectPatrol;  //DVRѲ���߳̾��
	HANDLE        m_hThreadStoragepatrol;  //�洢�豸���������
    volatile bool m_bStopWorker;           //Уʱ�����Ƿ�ֹͣ
    volatile bool m_bStopServer;           //Уʱ�����Ƿ�ֹͣ

public:
	config_info_t m_stAdjustConfig;      //������Ϣ

	HANDLE                          m_hGetCollectDeviceListEvent;    //�ɼ��豸�ź���
	CRITICAL_SECTION                m_csListCollectDevice;           //�ɼ��豸��Ϣ�б���
	std::list<collect_info_t>       m_listCollectDevice;             //�ɼ��豸��Ϣ����

	HANDLE                          m_hGetStorageDeviceListEvent;    //�洢�豸�ź���
	CRITICAL_SECTION                m_cslistStorageDevice;           //�洢�豸��Ϣ�б���
	std::list<Struct_StorageDevice> m_listStorageDevice;             //�洢�豸��Ϣ����

};

inline CAdjustTime* GetAdjustPtr(void)
{
    return CAdjustTime::instance();
}

#endif //GUARD_ADJUSTTIME_H
