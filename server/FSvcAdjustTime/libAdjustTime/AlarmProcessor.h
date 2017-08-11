/**	@file    AlarmProcessor.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   �������� ��
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
#ifndef GUARD_ALARMPROCESSOR_H
#define GUARD_ALARMPROCESSOR_H

#include <fclib/fcTchar.h>
#include <HPR_Singleton.h>
#include "SynchroQueue.h"
#include <SvcGeneralDef.h>
#include "../util/Guard.h"
//#include <HPR_Guard.h>
//#include "hppTTSClient.h"
#include <map>
#include <time.h>
#include "Center/Alarmlog.pb.h"
#include "DBDataMgr.h"


class CAlarmProcessor : public singleton<CAlarmProcessor>
{
    friend class singleton<CAlarmProcessor>;
public:
    CAlarmProcessor(void);
    ~CAlarmProcessor(void);

    /** @fn Init
     *  @brief  ��ʼ������
     *  @return 0�ɹ� ����ʧ��
     */
    HPR_INT32 Init();

   /** @fn      Fini
    *   @brief  ����ʼ������
    *   @return ���� ADJUST_ERROR_SUCC
                    �쳣 �� ADJUST_ERROR_SUCC
    */
    int Fini(void);

    /** @fn AddAlarmMsg
     *  @brief  ��ӱ�����Ϣ
     *  @param [in] pAlarm, ������Ϣָ�� 
     *  @return void
     */
    void AddAlarmMsg(const pALARM_MSG pAlarm);

    /** @fn SaveAlarmThreadProc
	 *  @brief  ��ӱ���������
	 *  @return HPR_INT32�� 0 �˳�������������
	 */
    HPR_INT32 SaveAlarmThreadProc();

   /** @fn AlarmDisposeProc
    *  @brief  ������Ϣ������
    *  @return HPR_INT32�� 0 �˳�������������
    */
    HPR_INT32 AlarmDisposeProc();

private:
    /** @fn GetSvcKeyValue
     *  @brief  ��ȡ״̬��������
     *  @param [in] workstate, ����״̬�ṹ��
     *  @return std::_tstring��״̬��������
     */
    std::_tstring GetSvcKeyValue(const work_state_t &workstate);

    /** @fn FormatKey
     *  @brief  ���ɹ���״̬key
     *  @param [in] workstate, ����״̬�ṹ��
     *  @return std::_tstring��key�ַ���
     */
    std::_tstring FormatKey(const work_state_t &workstate);

    /**	@fn	    DelayBySleep
     *	@brief	ͨ��ѭ��sleep�ķ�ʽ�ȴ�һ��ʱ��
     *	@param  [in]dwMilliseconds, ��Ҫ�ȴ���ʱ�䣬��λ����
     *	@param  [in]stopFlag, ��־�����Ϊtrue��ȴ���dwMilliseconds��
     *                        �������ǰ����
     *	@param  [in]dwInterval, �ȴ��������λ����
     *	@return	void
     */
    void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);


    /** @fn     strTime
     *  @brief  ʱ��ת��Ϊ�ַ���
     *  @param  [in]t, ʱ����Ϣ
     *  @return ʱ���ַ���
     */
    std::_tstring strTime(__time32_t const t);


   /**	@fn	    GetAlarmLogReq
	*	@brief	��ñ�����־
	*   @param  [in]workstate, workstate��Ϣ
	*   @param  [out]stAlarmLog, ������־��Ϣ
	*   @return bool
	*/
	bool GetAlarmLogReq(const work_state_t &workstate, Struct_AlarmLog &stAlarmLog);

	/** @fn     UpdateCenterAlarmReq
	*  @brief  ���ı�����Ϣ���±���
	*  @param  [in]workstate, workstate��Ϣ
	*  @return bool
	*/
	bool UpdateCenterAlarmReq(const work_state_t &workstate);

public:

   /**	@fn	    GetAlarmLogRsp
	*	@brief	��ñ�����־
	*	@param  [in] alarmlogInfo -- ������־��Ϣ
	*   @param  [in] bFinish -- �Ƿ����
	*	@return	
	*/
	void GetAlarmLogRsp(const cms_8120::AlarmlogInfo& alarmlogInfo, BOOL bFinish /*= true*/);

	/**	@fn	    convertAlarmLog
	*	@brief	ת��������־
	*	@param  [in]  alarmlogInfo -- ԭ������־
	*   @param  [out] stAlarmLog -- ת����ı�����־
	*	@return	BOOL
	*/
	BOOL convertAlarmLog(const cms_8120::Alarmlog &alarmlogInfo, Struct_AlarmLog &stAlarmLog);

	/**	@fn	    AddAlarmRegionId
	*	@brief	���ӱ�������ID
	*	@param  [in]  work_state_t -- ������Ϣ
	*   @param  [out] work_state_t -- ��������ID
	*	@return	void
	*/
	void AddAlarmRegionId(work_state_t& workstate);

public:
    bool                       m_bExitFlag;  ///<�����˳���־

	CRITICAL_SECTION m_csRegion;     //������
	std::map<std::string, int> m_mapRegion; //�豸����

private:
    CSynchroQueue <work_state_t>          m_msgWorkState;  // ״̬��Ϣ����
    std::map<std::_tstring, work_state_t> m_mapWorkState;  // ����״̬map
    CRITICAL_SECTION                      m_csWorkState;  // ����״̬map��

	HANDLE           m_hAlarmLogEvent; //������־�ź���
    CRITICAL_SECTION m_csAlarmLog;     //������־��
	Struct_AlarmLog  m_stAlarmLog;     //������־��Ϣ
};
inline CAlarmProcessor* GetCAlarmProcPtr(void)
{
    return CAlarmProcessor::instance();
}
#endif //GUARD_ALARMPROCESSOR_H
