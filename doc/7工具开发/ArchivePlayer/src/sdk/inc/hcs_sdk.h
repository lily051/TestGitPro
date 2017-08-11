/** @file   hcs_sdk.h
 *  @note   Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief  
 *
 *  @author Jin Shengpeng
 *  @date   2015/01/28
 *
 *  @note   ��ʷ��¼��
 *  @note   V1.0.0.0 ����
 */

#ifndef HCS_SDK_H_
#define HCS_SDK_H_

#include "hcs_sdk_define.h"

/******************************************************************************
 *                                                                            *
 *                               ϵͳ����                                     *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_Init
 *  @ brief  ��ʼ��SDK
 *  @ return HCS_OK-��ʼ���ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Init();



/** @ fn     HCS_SDK_Deinit
 *  @ brief  ����ʼ��SDK[��γ�ʼ���������η���ʼ�������һ�η���ʼ�����سɹ���ǰ�漸�ζ��᷵��ʧ��]
 *  @ return HCS_OK-����ʼ���ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Deinit();

/** @ fn     HCS_SDK_GetVersion
 *  @ brief  ��ȡ�ƴ洢�汾��
 *  @ param  ip				- [in] �ƴ洢IP��ַ
 *  @ param  port			- [in] �ƴ洢����˿�
 *  @ param  version		- [out] �ƴ洢�汾��Ϣ
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetVersion(const char* ip, 
												  unsigned int port, 
												  HCS_Version* version);

/** @ fn     HCS_SDK_Login
 *  @ brief  ��¼�ƴ洢
 *  @ param  ip			- [in] �ƴ洢IP��ַ
 *  @ param  port		- [in] �ƴ洢����˿�
 *  @ param  username	- [in] ��¼�ƴ洢�û���
 *  @ param  password	- [in] ��¼�ƴ洢����
 *  @ param  login_id	- [out]�ƴ洢��¼ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Login(const char* ip, 
											 unsigned int port, 
											 const char* username, 
											 const char* password, 
											 unsigned long long* login_id);



/** @ fn     HCS_SDK_Logout
 *  @ brief  �ǳ��ƴ洢
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_Logout(unsigned long long login_id);



/** @ fn     HCS_SDK_GetPoolInfo
 *  @ brief  ��ȡָ����Դ����Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  pool_id	- [in] ��Դ��ID[���֧��32���ַ�]
 *  @ param  pool_info	- [out]��Դ����Ϣ
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetPoolInfo(unsigned long long login_id, 
												   const char* pool_id, 
												   HCS_PoolInfo* pool_info);



/** @ fn     HCS_SDK_GetAllPoolInfoStart
 *  @ brief  ��ʼ��ȡȫ����Դ����Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  count		- [out]��Դ������
 *  @ param  handle		- [out]��ȡȫ����Դ����Ϣ�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoStart(unsigned long long login_id, 
														   int* count, 
														   int* handle);



/** @ fn     HCS_SDK_GetAllPoolInfoStop
 *  @ brief  ��ȡ��һ����Դ����Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡȫ����Դ����Ϣ�������
 *  @ param  info		- [out] ��ȡ����Դ����Ϣ
 *  @ param  status		- [out] ��ѯ��Դ�ؽ��[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoNext(unsigned long long login_id, 
														  int handle, 
														  HCS_PoolInfo* info, 
														  int* status);



/** @ fn     HCS_SDK_GetAllPoolInfoStop
 *  @ brief  ֹͣ��ȡȫ����Դ����Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡȫ����Դ����Ϣ�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllPoolInfoStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_GetAbility
 *  @ brief  ��ȡ�ƴ洢����������
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  ability	- [out]������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAbility(unsigned long long login_id, 
												  HCS_Ability* ability);

/******************************************************************************
 *                                                                            *
 *                               ������										  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_SetRecPlan
 *  @ brief  ����¼��ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  plan		- [in] ¼��ƻ�
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecPlan(unsigned long long login_id, 
												  const HCS_RecordPlan* plan);



/** @ fn     HCS_SDK_DelRecPlan
 *  @ brief  ɾ��¼��ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  camera_id	- [in] ������ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelRecPlan(unsigned long long login_id, 
												  const char* camera_id);



/** @ fn     HCS_SDK_GetRecPlan
 *  @ brief  ��ȡ��������Ӧ��¼��ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  camera_id	- [in] ������ID
 *  @ param  plan		- [out]¼��ƻ�
 *  @ param  status		- [out]¼��ƻ��·�״̬[0-δ�·���1-���·�]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetRecPlan(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_RecordPlan* plan, 
												  int* status);



/** @ fn     HCS_SDK_GetAllRecPlanStart
 *  @ brief  ��ʼ��ȡȫ��¼��ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  count		- [out]¼��ƻ���Ŀ
 *  @ param  handle		- [out]��ȡ¼��ƻ��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanStart(unsigned long long login_id, 
														  int* count, 
														  int* handle);



/** @ fn     HCS_SDK_GetAllRecPlanNext
 *  @ brief  ��ȡ��һ��¼��ƻ���Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡȫ��¼��ƻ���Ϣ�������
 *  @ param  info		- [out] ��ȡ��¼��ƻ���Ϣ
 *  @ param  alloc		- [out] ��ȡ��¼��ƻ��·�״̬[0-δ�·���1-���·�]
 *  @ param  status		- [out] ��ȡ¼��ƻ����[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanNext(unsigned long long login_id, 
	                                                     int handle, 
														 HCS_RecordPlan* info, 
														 int* alloc, 
														 int* status);



/** @ fn     HCS_SDK_GetAllRecPlanStop
 *  @ brief  ֹͣ��ȡȫ��¼��ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡ¼��ƻ��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllRecPlanStop(unsigned long long login_id, 
														 int handle);



/** @ fn     HCS_SDK_EventRecord
 *  @ brief  �¼�¼��
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  camera_id	- [in] ������ID
 *  @ param  rec_type	- [in] ¼������[��Χ��0-65535]
 *  @ param  time		- [in] ����ʱ��[�������5���������Ƶ���ĵ��øýӿ�]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_EventRecord(unsigned long long login_id, 
												   const char* camera_id, 
												   unsigned int rec_type, 
												   unsigned int time);



/**  @ fn     HCS_SDK_GetAllVtduInfoStart
 *   @ brief  ��ȡ����������б�
 *   @ param  login_id	- [in] �ɹ���¼�ƴ洢���صľ��
 *   @ param  count		- [out]�����������Ŀ
 *   @ param  handle	- [out]��ȡ���
 *   @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API  HCS_SDK_GetAllVtduInfoStart(unsigned long long login_id, 
													        int* count, 
													        int* handle);



/** @ fn     HCS_SDK_GetAllVtduInfoNext
 *  @ brief  ��ȡ��һ�������������Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡȫ�������������Ϣ�������
 *  @ param  info		- [out] ��ȡ�Ľ����������Ϣ
 *  @ param  status		- [out] ��ȡ�����������Ϣ���[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllVtduInfoNext(unsigned long long login_id, 
														  int handle, 
														  HCS_VtduInfo* info, 
														  int* status);



/** @ fn     HCS_SDK_GetAllVtduInfoStop
 *  @ brief  ֹͣ��ȡȫ�������������Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡ����������������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllVtduInfoStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_SetHisRecPlan
 *  @ brief  ������ʷ���洢�ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  plan		- [in] ��ʷ���洢�ƻ�
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetHisRecPlan(unsigned long long login_id, 
													 const HCS_HisRecPlan* plan);



/** @ fn     HCS_SDK_DelHisRecPlan
 *  @ brief  ɾ����ʷ���洢�ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  camera_id	- [in] ������ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelHisRecPlan(unsigned long long login_id, 
													 const char* camera_id);



/** @ fn     HCS_SDK_GetHisRecPlan
 *  @ brief  ��ȡ��������Ӧ����ʷ���洢�ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  camera_id	- [in] ������ID
 *  @ param  plan		- [out]��ʷ���洢�ƻ�
 *  @ param  status		- [out]¼��ƻ��·�״̬[0-δ�·���1-���·�]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetHisRecPlan(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_HisRecPlan* plan, 
													 int* status);



/** @ fn     HCS_SDK_GetAllHisRecPlanStart
 *  @ brief  ��ʼ��ȡȫ����ʷ��¼�ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  count		- [out]��ʷ��¼�ƻ���Ŀ
 *  @ param  handle		- [out]��ȡ��ʷ���洢�ƻ��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanStart(unsigned long long login_id, 
															 int* count, 
															 int* handle);



/** @ fn     HCS_SDK_GetAllHisRecPlanNext
 *  @ brief  ��ȡ��һ����ʷ���ƻ���Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡȫ����ʷ���ƻ���Ϣ�������
 *  @ param  info		- [out] ��ȡ����ʷ���ƻ���Ϣ
 *  @ param  alloc		- [out] ��ȡ����ʷ���ƻ��·�״̬[0-δ�·���1-���·�]
 *  @ param  status		- [out] ��ȡ��ʷ���ƻ����[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanNext(unsigned long long login_id, 
															int handle, 
															HCS_HisRecPlan* info, 
															int* alloc, 
															int* status);



/** @ fn     HCS_SDK_GetAllHisRecPlanStop
 *  @ brief  ֹͣ��ȡȫ����ʷ���洢�ƻ�
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡ��ʷ���洢�ƻ��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetAllHisRecPlanStop(unsigned long long login_id, 
															int handle);



/** @ fn     HCS_SDK_HistoryRecord
 *  @ brief  �ֶ���¼
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  info		- [in] �ֶ���¼����
 *  @ param  url_len	- [in] url����������[����1024�ֽ�]
 *  @ param  url		- [out] ���ص�ȡ��URL[����ͨ��URLȡ��Ҳ����ͨ������������ʼ������ʱ��ȡ��]
 *  @ param  task_id	- [out] �ֶ���¼����ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecord(unsigned long long login_id, 
													 const HCS_HisRecInfo* info,
													 unsigned int url_len,
													 char* url,
													 unsigned long long* task_id);




/** @ fn     HCS_SDK_HistoryRecordStatus
 *  @ brief  ��ȡ�ֶ���¼״̬
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  task_id	- [in] �ֶ���¼����ID
 *  @ param  task_info	- [out] �ֶ���¼ִ��״̬
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecordStatus(unsigned long long login_id, 
														   unsigned long long task_id,
														   HCS_TaskInfo* task_info);



/** @ fn     HCS_SDK_HistoryRecordStop
 *  @ brief  ֹͣ�ֶ���¼
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  task_id	- [in] �ֶ���¼����ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_HistoryRecordStop(unsigned long long login_id, 
														 unsigned long long task_id);


/******************************************************************************
 *                                                                            *
 *                               ¼�����									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_RecStart
 *  @ brief  ����¼��
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  rec_type		- [in] ¼������[��Χ��0-65535]
 *  @ param  data_type		- [in] ��������
 *  @ param  iframe_type	- [in] I֡����
 *  @ param  replication	- [in] ������[0-������ 1-1������ 2-˫������]
 *  @ param  pool_id		- [in] ��Դ��ID[���֧��32���ַ�]
 *  @ param  rep_pool_id	- [in] ������Դ��ID[���֧��32���ַ�]�����������Ϊ0ʱ��Ч
 *  @ param  rec_head		- [in] ¼��ͷ����
 *  @ param  head_size		- [in] ¼��ͷ����[���Ϊ64�ֽ�]
 *  @ param  handle			- [out] ¼��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecStart(unsigned long long login_id, 
												const char* camera_id, 
												unsigned int rec_type, 
												HCS_RecDataType data_type, 
												HCS_IFrameType iframe_type, 
												unsigned int replication, 
												const char* pool_id, 
												const char* rep_pool_id, 
												const char* rec_head, 
												unsigned int head_size, 
												int* handle);



/** @ fn     HCS_SDK_RecData
 *  @ brief  �洢¼������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼��������
 *  @ param  frame_type		- [in] ֡����
 *  @ param  data_type		- [in] ��������
 *  @ param  begin_time		- [in] ¼�����ݶ�Ӧʱ��Ƭ�Ŀ�ʼʱ���
 *  @ param  end_time		- [in] ¼�����ݶ�Ӧʱ��Ƭ�Ľ���ʱ���
 *  @ param  rec_data		- [in] ¼������
 *  @ param  data_size		- [in] ���ݳ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecData(unsigned long long login_id, 
											   int handle, 
											   HCS_FrameType frame_type, 
											   HCS_DataType data_type, 
											   HCS_Time begin_time, 
											   HCS_Time end_time, 
											   const char* rec_data, 
											   unsigned int data_size);



/** @ fn     HCS_SDK_SetRecType
 *  @ brief  �޸�¼������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼��������
 *  @ param  rec_type		- [in] ¼������[��Χ��0-65535]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecType(unsigned long long login_id, 
												  int handle, 
												  unsigned int rec_type);



/** @ fn     HCS_SDK_SetRecReplication
 *  @ brief  �޸�¼���������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼��������
 *  @ param  replication	- [in] ������[0-������ 1-1������ 2-˫������]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetRecReplication(unsigned long long login_id, 
														 int handle, 
														 unsigned int replication);



/** @ fn     HCS_SDK_RecStop
 *  @ brief  ֹͣ¼��
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼��������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecStop(unsigned long long login_id, 
											   int handle);



/** @ fn     HCS_SDK_GetRecHead
 *  @ brief  ��ȡ¼��ͷ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ¼��ο�ʼʱ��
 *  @ param  end_time		- [in] ¼��ν���ʱ��
 *  @ param  buff			- [in] ¼��ͷ���ݻ�����
 *  @ param  buff_size		- [in] ¼��ͷ���ݻ�������С
 *  @ param  head_size		- [out]ʵ�ʵ�¼��ͷ���ݴ�С
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetRecHead(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_Time begin_time, 
												  HCS_Time end_time, 
												  char* buff, 
												  unsigned int buff_size, 
												  int* head_size);



/** @ fn     HCS_SDK_PlaybackStart
 *  @ brief  �����ط�
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] �ط�¼��ο�ʼʱ��
 *  @ param  end_time		- [in] �ط�¼��ν���ʱ��
 *  @ param  mode			- [in] �ط�ģʽ
 *  @ param  rec_type		- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  handle			- [out]¼��طŲ������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackStart(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_Time begin_time, 
													 HCS_Time end_time, 
													 HCS_PlaybackMode mode, 
													 const char* rec_type, 
													 int* handle);



/** @ fn     HCS_SDK_PlaybackData
 *  @ brief  ��ȡ�ط�����
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] �طŲ������
 *  @ param  buff			- [in] �ط����ݻ�����
 *  @ param  buff_size		- [in] �ط����ݻ�������С
 *  @ param  data_size		- [out]ʵ�ʵ����ݴ�С
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackData(unsigned long long login_id, 
													int handle, 
													char* buff, 
													unsigned int buff_size, 
													int* data_size);



/** @ fn     HCS_SDK_PlaybackSeek
*  @ brief  �طŶ�λ
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] �طŲ������
*  @ param  seek_time		- [in] ��λʱ���
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackSeek(unsigned long long login_id, 
													int handle, 
													HCS_Time seek_time);



/** @ fn     HCS_SDK_PlaybackStop
 *  @ brief  ֹͣ�ط�
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] �طŲ������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PlaybackStop(unsigned long long login_id, 
													int handle);



/** @ fn     HCS_SDK_RecDownloadStart
 *  @ brief  ����¼������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ����¼��ο�ʼʱ��
 *  @ param  end_time		- [in] ����¼��ν���ʱ��
 *  @ param  rec_type		- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  offset			- [in] ��������ƫ��(���ڶϵ�����)
 *  @ param  cb				- [in] ¼���������ݻص�����
 *  @ param  user_data		- [in] �û��Զ�������
 *  @ param  handle			- [out]¼�����ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDownloadStart(unsigned long long login_id, 
														const char* camera_id, 
														HCS_Time begin_time, 
														HCS_Time end_time, 
														const char* rec_type, 
														unsigned long long offset, 
														RecDownloadCb cb, 
														void* user_data, 
														int* handle);



/** @ fn     HCS_SDK_RecDownloadStop
 *  @ brief  ֹͣ¼������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼�����ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDownloadStop(unsigned long long login_id, 
													   int handle);



/** @ fn     HCS_SDK_RecHighSpeedDownloadStart
 *  @ brief  ����¼���������
 *  @ param  login_id			- [in] �ƴ洢��¼ID
 *  @ param  camera_id			- [in] ������ID
 *  @ param  begin_time			- [in] ����¼��ο�ʼʱ��
 *  @ param  end_time			- [in] ����¼��ν���ʱ��
 *  @ param  rec_type			- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  download_type		- [in] ��������[0-���أ�1-����]
 *  @ param  concurrency		- [in] ������[1-10]
 *  @ param  max_segment_count	- [in] �û�����Ķ��������Ԫ�ظ���
 *  @ param  segment_infos		- [in/out] ����ʱ����ȡ�����ݿ���Ϣ�������������ʱ��ƽ̨����������ص�������Ϣ�����룩
 *  @ param  segment_count		- [in/out] ����ʱ����ȡ�����ݿ�����������������ʱ��ƽ̨����������ص����ݸ��������룩
 *  @ param  cb					- [in] ¼������������ݻص�����
 *  @ param  user_data			- [in] �û��Զ�������
 *  @ param  handle				- [out]¼�����ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecHighSpeedDownloadStart(unsigned long long login_id, 
																 const char* camera_id, 
																 HCS_Time begin_time, 
																 HCS_Time end_time, 
																 const char* rec_type, 
																 unsigned int download_type, 
																 unsigned int concurrency, 
																 unsigned int max_segment_count, 
																 HCS_HighSpeedSegInfo* segment_infos, 
																 int* segment_count, 
																 RecHighSpeedDownloadCb cb, 
																 void* user_data, 
																 int* handle);




/** @ fn     HCS_SDK_RecHighSpeedDownloadStop
 *  @ brief  ֹͣ¼��������� 
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼�����ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecHighSpeedDownloadStop(unsigned long long login_id, 
																int handle);



/** @ fn     HCS_SDK_RecSearchStart
 *  @ brief  ����¼�����
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ����ʱ��ο�ʼʱ���
 *  @ param  end_time		- [in] ����ʱ��ν���ʱ���
 *  @ param  rec_type		- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  lock_type		- [in] ��������
 *  @ param  count			- [out]¼��Ƭ����Ŀ
 *  @ param  handle			- [out]¼������������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchStart(unsigned long long login_id, 
													  const char* camera_id, 
													  HCS_Time begin_time, 
													  HCS_Time end_time, 
													  const char* rec_type, 
													  HCS_SRLockType lock_type, 
													  int* count, 
													  int* handle);



/** @ fn     HCS_SDK_RecSearchNext
 *  @ brief  ��ȡ��һ��¼�����Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡ¼�����Ϣ�������
 *  @ param  info		- [out] ��ȡ��¼�����Ϣ
 *  @ param  status		- [out] ��ȡ¼�����Ϣ���[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchNext(unsigned long long login_id, 
	                                                 int handle, 
												     HCS_RecordInfo* info, 
													 int* status);



/** @ fn     HCS_SDK_RecSearchStop
 *  @ brief  ֹͣ¼�����
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼������������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchStop(unsigned long long login_id, 
													 int handle);



/** @ fn     HCS_SDK_RecSearchByDay
 *  @ brief  ¼��������ѯ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  date			- [in] ����
 *  @ param  info			- [in] ������ѯ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByDay(unsigned long long login_id, 
													  const char* camera_id,
													  HCS_Date date,
													  HCS_RecDayInfo *info);



/** @ fn     HCS_SDK_RecSearchByMonth
 *  @ brief  ¼��������ѯ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  year			- [in] ��
 *  @ param  month			- [in] ��
 *  @ param  info			- [in] ������ѯ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByMonth(unsigned long long login_id, 
														const char* camera_id,
														unsigned int year,
														unsigned int month,
														HCS_RecMonthInfo *info);



/** @ fn     HCS_SDK_RecDelete
 *  @ brief  ɾ��¼��
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  rec_type		- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  begin_time		- [in] Ҫɾ����¼��Ƭ�ο�ʼʱ��
 *  @ param  end_time		- [in] Ҫɾ����¼��Ƭ�ν���ʱ��
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecDelete(unsigned long long login_id, 
												 const char* camera_id, 
												 const char* rec_type, 
												 HCS_Time begin_time, 
												 HCS_Time end_time);



/** @ fn     HCS_SDK_RecLock
 *  @ brief  ����¼��
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  rec_type		- [in] ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  begin_time		- [in] Ҫ������¼��Ƭ�ο�ʼʱ��
 *  @ param  end_time		- [in] Ҫ������¼��Ƭ�ν���ʱ��
 *  @ param  lock_time		- [in] ����¼��Ƭ�ε�ʱ����0��ʾ����,��λ:��
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecLock(unsigned long long login_id, 
											   const char* camera_id, 
											   const char* rec_type, 
											   HCS_Time begin_time, 
											   HCS_Time end_time, 
											   unsigned int lock_time);

/*****************************************************************************
 *                                                                           *
 *                               ��Ƶ�ļ�����								 *
 *                                                                           *
 *****************************************************************************/

/**@ fn     HCS_SDK_StreamFileWriteStart
*  @ brief  ������Ƶ�ļ��洢
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  file_write_info	- [in] ��Ƶ�ļ�����д������Ϣ
*  @ param  handle			- [out] ¼��������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteStart(unsigned long long login_id, 
															const HCS_StreamFileWriteInfo* file_write_info, 
															int* handle);



/**@ fn     HCS_SDK_StreamFileWriteData
*  @ brief  �洢��Ƶ�ļ�����
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ�¼��������
*  @ param  frame_type		- [in] �ļ�֡����
*  @ param  begin_time		- [in] ���ļ��鿪ʼʱ���
*  @ param  end_time		- [in] ���ļ������ʱ���
*  @ param  rec_data		- [in] ��Ƶ�ļ�����
*  @ param  data_size		- [in] ���ݳ���
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteData(unsigned long long login_id, 
														   int handle, 
														   HCS_FrameType frame_type, 
														   HCS_Time begin_time, 
														   HCS_Time end_time, 
														   const char* rec_data, 
														   unsigned int data_size);



/**@ fn     HCS_SDK_StreamFileWriteStop
*  @ brief  ֹͣ��Ƶ�ļ��洢
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ��������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileWriteStop(unsigned long long login_id, 
														   int handle);



/**@ fn     HCS_SDK_StreamFileReadStart
*  @ brief  ������Ƶ�ļ���ȡ
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  file_key		- [in] ��Ƶ�ļ���
*  @ param  handle			- [out]��Ƶ�ļ���ȡ�������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadStart(unsigned long long login_id, 
														   const char* file_key, 
														   int* handle);



/**@ fn     HCS_SDK_StreamFileReadData
*  @ brief  ��ȡ��Ƶ�ļ�����
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ���ȡ�������
*  @ param  buff			- [in] ��ȡ���ݻ�����
*  @ param  buff_size		- [in] ��ȡ���ݻ�������С
*  @ param  data_size		- [out]ʵ�ʵ����ݴ�С
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadData(unsigned long long login_id, 
														  int handle, 
														  char* buff, 
														  unsigned int buff_size, 
														  int* data_size);



/**@ fn     HCS_SDK_StreamFileReadSeek
*  @ brief  ��Ƶ�ļ���ȡ��λ
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ���ȡ�������
*  @ param  seek_time		- [in] ��λƫ��ʱ��
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadSeek(unsigned long long login_id, 
														  int handle, 
														  HCS_Time seek_time);



/**@ fn     HCS_SDK_StreamFileReadStop
*  @ brief  ֹͣ��Ƶ�ļ���ȡ
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ���ȡ�������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileReadStop(unsigned long long login_id, 
														  int handle);



/**@ fn     HCS_SDK_StreamFileDownloadStart
*  @ brief  ������Ƶ�ļ�����
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  file_key		- [in] ��Ƶ�ļ���
*  @ param  offset			- [in] ��������ƫ��(���ڶϵ�����),��λ:�ֽ�
*  @ param  cb				- [in] ��Ƶ�ļ��������ݻص�����
*  @ param  user_data		- [in] �û��Զ�������
*  @ param  handle			- [out]��Ƶ�ļ����ز������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDownloadStart(unsigned long long login_id, 
															   const char* file_key, 
															   unsigned long long offset, 
															   RecDownloadCb cb, 
															   void* user_data, 
															   int* handle);



/**@ fn     HCS_SDK_StreamFileDownloadStop
*  @ brief  ֹͣ��Ƶ�ļ�����
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  handle			- [in] ��Ƶ�ļ����ز������
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDownloadStop(unsigned long long login_id, 
															  int handle);



/* @ fn     HCS_SDK_GetStreamFileInfo
*  @ brief  ��ȡ��Ƶ�ļ���Ϣ
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  file_key		- [in] ��Ƶ�ļ���
*  @ param  file_info		- [out] ��Ƶ�ļ���Ϣ
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetStreamFileInfo(unsigned long long login_id, 
														 const char* file_key, 
														 HCS_StreamFileInfo* file_info);



/**@ fn     HCS_SDK_StreamFileDelete
*  @ brief  ɾ����Ƶ�ļ�
*  @ param  login_id		- [in] �ƴ洢��¼ID
*  @ param  file_key		- [in] ��Ƶ�ļ���
*  @ return HCS_OK-�ɹ� ����-������
*/
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_StreamFileDelete(unsigned long long login_id, 
														const char* file_key);


/******************************************************************************
 *                                                                            *
 *                               ͼƬ����									  *
 *                                                                            *
 ******************************************************************************/


/** @ fn     HCS_SDK_PicStorStart
 *  @ brief  �����洢ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  pool_id		- [in] ��Դ��ID[���֧��32���ַ�]
 *  @ param  replication	- [in] ������[0-������ 1-1������ 2-˫������]
 *  @ param  rep_pool_id	- [in] ������Դ��ID[���֧��32���ַ�]�����������Ϊ0ʱ��Ч
 *  @ param  handle			- [out]ͼƬ�洢�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorStart(unsigned long long login_id, 
													const char* camera_id, 
													const char* pool_id, 
													unsigned int replication, 
													const char* rep_pool_id, 
													int* handle);



/** @ fn     HCS_SDK_PicStorData
 *  @ brief  �洢ͼƬд������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ͼƬ�洢�������
 *  @ param  time			- [in] ͼƬ�洢ʱ���
 *  @ param  pic_type		- [in] ͼƬ����
 *  @ param  pic_data		- [in] ͼƬ����
 *  @ param  data_size		- [in] ͼƬ���ݴ�С
 *  @ param  url_len		- [in] ͼƬ��ӦURL�ַ�������
 *  @ param  url			- [out]ͼƬ��ӦURL�ַ�����
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorData(unsigned long long login_id, 
												   int handle, 
												   HCS_Time time, 
												   HCS_PicType pic_type, 
												   const char* pic_data, 
												   unsigned int data_size, 
												   unsigned int url_len, 
												   char* url);



/** @ fn     HCS_SDK_PicStorStop
 *  @ brief  ֹͣ�洢ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ͼƬ�洢�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicStorStop(unsigned long long login_id, 
												   int handle);



/** @ fn     HCS_SDK_PicDownloadByURL
 *  @ brief  ����URL����ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  url			- [in] ͼƬ��Ӧ��URL[���֧��256���ַ�]
 *  @ param  zoom_type		- [in] ͼƬ��������
 *  @ param  zoom_value		- [in] ͼƬ����ֵ
 *  @ param  buff			- [in] ͼƬ���ݴ洢����
 *  @ param  buff_size		- [in] ͼƬ���ݻ����С
 *  @ param  data_size		- [out]ʵ�ʵ�ͼƬ���ݴ�С
 *  @ param  pic_type		- [out]ͼƬ����
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByURL(unsigned long long login_id, 
														const char* url, 
														HCS_PicZoomType zoom_type, 
														const char* zoom_value, 
														char* buff, 
														unsigned int buff_size, 
														int* data_size, 
														int* pic_type);



/** @ fn     HCS_SDK_PicDownloadByTimeStart
 *  @ brief  ��������ʱ������ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  pic_type		- [in] ͼƬ����[""��ʾȫ��,�������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
 *  @ param  begin_time		- [in] ����ʱ�䷶Χ��ʼʱ���
 *  @ param  end_time		- [in] ����ʱ�䷶Χ����ʱ���
 *  @ param  zoom_type		- [in] ͼƬ��������
 *  @ param  zoom_value		- [in] ͼƬ����ֵ
 *  @ param  cb				- [in] ͼƬ���ػص�����
 *  @ param  user_data		- [in] �û��Զ�������
 *  @ param  handle			- [out]ͼƬ���ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByTimeStart(unsigned long long login_id, 
															  const char* camera_id, 
															  const char* pic_type, 
															  HCS_Time begin_time, 
															  HCS_Time end_time, 
															  HCS_PicZoomType zoom_type, 
															  const char* zoom_value, 
															  PicDownloadCb cb, 
															  void* user_data, 
															  int* handle);



/** @ fn     HCS_SDK_PicDownloadByTimeStop
 *  @ brief  ֹͣ����ʱ������ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ͼƬ���ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDownloadByTimeStop(unsigned long long login_id, 
															 int handler);



/** @ fn     HCS_SDK_PicLockByURL
 *  @ brief  ����URL����ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  url			- [in] ͼƬ��Ӧ��URL[���֧��256���ַ�]
 *  @ param  lock_time		- [in] ����ʱ����0��ʾ����
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicLockByURL(unsigned long long login_id, 
													const char* url, 
													unsigned int lock_time);



/** @ fn     HCS_SDK_PicLockByTime
 *  @ brief  ����ʱ�������ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ����ʱ�䷶Χ��ʼʱ���
 *  @ param  end_time		- [in] ����ʱ�䷶Χ����ʱ���
 *  @ param  lock_time		- [in] ����ʱ����0��ʾ����,��λ:��
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicLockByTime(unsigned long long login_id, 
													 const char* camera_id, 
													 HCS_Time begin_time, 
													 HCS_Time end_time, 
													 unsigned int lock_time);



/** @ fn     HCS_SDK_PicDeleteByURL
 *  @ brief  ����URLɾ��ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  url			- [in] ͼƬ��Ӧ��URL[���֧��256���ַ�]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDeleteByURL(unsigned long long login_id, 
													  const char* url);



/** @ fn     HCS_SDK_PicDeleteByTime
 *  @ brief  ����ʱ���ɾ��ͼƬ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ɾ��ʱ�䷶Χ��ʼʱ���
 *  @ param  end_time		- [in] ɾ��ʱ�䷶Χ����ʱ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_PicDeleteByTime(unsigned long long login_id, 
													   const char* camera_id, 
													   HCS_Time begin_time, 
													   HCS_Time end_time);


/*****************************************************************************
 *                                                                           *
 *                               ��������									 *
 *                                                                           *
 *****************************************************************************/

/** @ fn     HCS_SDK_AttachedStorStart
 *  @ brief  �����洢����
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  pool_id		- [in] ��Դ��ID[���֧��32���ַ�]
 *  @ param  replication	- [in] ������[0-������ 1-1������ 2-˫������]
 *  @ param  rep_pool_id	- [in] ������Դ��ID[���֧��32���ַ�]�����������Ϊ0ʱ��Ч
 *  @ param  handle			- [out]�����洢�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorStart(unsigned long long login_id, 
														 const char* camera_id, 
														 const char* pool_id, 
														 int replication, 
														 const char* rep_pool_id, 
														 int* handle);



/** @ fn     HCS_SDK_AttachedStorData
 *  @ brief  �洢����д������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] �����洢�������
 *  @ param  begin_time		- [in] �����洢��ʼʱ��
 *  @ param  end_time		- [in] �����洢����ʱ��
 *  @ param  data			- [in] ��������
 *  @ param  data_size		- [in] �������ݴ�С
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorData(unsigned long long login_id, 
														int handle, 
														HCS_Time begin_time, 
														HCS_Time end_time, 
														const char* data, 
														unsigned int data_size);



/** @ fn     HCS_SDK_AttachedStorStop
 *  @ brief  ֹͣ�洢����
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] �����洢�������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedStorStop(unsigned long long login_id, 
														int handle);



/** @ fn     HCS_SDK_AttachedSearchStart
 *  @ brief  ������������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ����ʱ��ο�ʼʱ���
 *  @ param  end_time		- [in] ����ʱ��ν���ʱ���
 *  @ param  count			- [out]����Ƭ����Ŀ
 *  @ param  handle			- [out]���������������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchStart(unsigned long long login_id, 
														   const char* camera_id, 
														   HCS_Time begin_time, 
														   HCS_Time end_time,  
														   int* count, 
														   int* handle);



/** @ fn     HCS_SDK_AttachedSearchNext
 *  @ brief  ��ȡ��һ����������Ϣ
 *  @ param  login_id	- [in] �ƴ洢��¼ID
 *  @ param  handle		- [in] ��ȡ��������Ϣ�������
 *  @ param  info		- [out] ��ȡ�ĸ�������Ϣ
 *  @ param  status		- [out] ��ȡ��������Ϣ���[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchNext(unsigned long long login_id, 
	                                                      int handle, 
												          HCS_RecordInfo* info, 
													      int* status);



/** @ fn     HCS_SDK_AttachedSearchStop
 *  @ brief  ֹͣ��������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] ¼���������������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedSearchStop(unsigned long long login_id, 
														  int handle);



/** @ fn     HCS_SDK_AttachedDownloadStart
 *  @ brief  ������������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ���ظ����ο�ʼʱ��
 *  @ param  end_time		- [in] ���ظ����ν���ʱ��
 *  @ param  offset			- [in] ��������ƫ��(���ڶϵ�����),��λ:�ֽ�
 *  @ param  cb				- [in] �������ݻص�����
 *  @ param  user_data		- [in] �û��Զ�������
 *  @ param  handle			- [out]�������ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedDownloadStart(unsigned long long login_id, 
															 const char* camera_id, 
															 HCS_Time begin_time, 
															 HCS_Time end_time, 
															 unsigned long long offset, 
															 AttachedDownloadCb cb, 
															 void* user_data, 
															 int* handle);



/** @ fn     HCS_SDK_AttachedDownloadStop
 *  @ brief  ֹͣ��������
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  handle			- [in] �������ز������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_AttachedDownloadStop(unsigned long long login_id, 
															int handle);


/******************************************************************************
 *                                                                            *
 *                               ���ݲ���									  *
 *                                                                            *
 ******************************************************************************/
/** @ fn     HCS_SDK_DataBackup
 *  @ brief  ���ݱ���[����Ҫ����̨VS�����ܱ��ݵ�����]
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  camera_id		- [in] ������ID
 *  @ param  begin_time		- [in] ���ݿ�ʼʱ���
 *  @ param  end_time		- [in] ���ݽ���ʱ���[���֧��3���¼�񱸷�]
 *  @ param  data_type		- [in] ��������1��¼�� 2����Ƶ�ļ� 3��ͼƬ 10��������
 *  @ param  pool_id		- [in] ��Դ��ID[���֧��32���ַ�]
 *  @ param  pool_type		- [in] Ŀ����Դ������(������)��0��ͨ�ó� 1����Ƶ�� 3��ͼƬ�� 4���ļ���
 *  @ param  cloud_ip_port	- [in] ��IP�Ͷ˿ڣ������Ǳ��ƻ����Ƶ�IP�˿�;��ʽ��IP:PORT1:PORT2 PORT1������˿� PORT2�����ݶ˿�
 *  @ param  replication	- [in] ������[ 1-1�� 2-˫��]
 *  @ param  do_begin_time	- [in] ÿ��ִ�б�������Ŀ�ʼʱ��,��λ:����
 *  @ param  do_end_time	- [in] ÿ��ִ�б�������Ľ���ʱ��,��λ:����
 *  @ param  task_id		- [out] ��������ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DataBackup(unsigned long long login_id, 
												  const char* camera_id, 
												  HCS_Time begin_time, 
												  HCS_Time end_time, 
												  unsigned int data_type, 
												  const char* pool_id, 
												  unsigned int pool_type, 
												  const char* cloud_ip_port, 
												  unsigned int replication, 
												  unsigned long long do_begin_time, 
												  unsigned long long do_end_time, 
												  unsigned long long* task_id);



/** @ fn     HCS_SDK_GetTaskInfo
 *  @ brief  ��ȡ������Ϣ
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  task_id		- [in] ����ID
 *  @ param  task_info		- [out] ������Ϣ
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetTaskInfo(unsigned long long login_id, 
												   unsigned long long task_id, 
												   HCS_TaskInfo* task_info);



/** @ fn     HCS_SDK_TaskDelete
 *  @ brief  ����ɾ��
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  task_id		- [in] ����ID
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TaskDelete(unsigned long long login_id, 
												  unsigned long long task_id);



/** @ fn     HCS_SDK_SetTaskNotifyCallBack
 *  @ brief  ��������֪ͨ�ص� 
 *  @ param  login_id		- [in] �ƴ洢��¼ID
 *  @ param  TaskNotifyCb	- [in] ����֪ͨ�ص�[�ص���ΪNULL��ʾȡ������֪ͨע��]
 *  @ param  user_data		- [in] �û�����
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetTaskNotifyCallBack(unsigned long long login_id, 
															 TaskNotifyCb cb, 
															 void* user_data);


/******************************************************************************
 *                                                                            *
 *                               ���ܼ���									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn	 HCS_SDK_SmartSearchStart
 *  @ brief	 �������ܼ���
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 param					-[in] ���ܼ�������
 *  @ param	 handle					-[out] ���ܼ������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchStart(unsigned long long login_id, 
														const HCS_SmartSearchParam* param, 
														int* handle);



/** @ fn	 HCS_SDK_SmartSearchNextInfo
 *  @ brief	 ���ܼ�����������һ����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ param	 result					-[out]�������������Ϣ
 *  @ param	 status					-[out]�������[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchNextInfo(unsigned long long login_id, 
														   int handle, 
														   HCS_SmartSearchRet* result, 
														   int* status);



/** @ fn     HCS_SDK_SmartSearchStop
 *  @ brief	 ֹͣ���ܼ���
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SmartSearchStop(unsigned long long login_id, 
													   int handle);



/** @ fn	 HCS_SDK_GetSmartIPCAbility
 *  @ brief	 ��ȡ����IPC����
 *  @ param	 login_id					-[in] �ƴ洢��¼ID
 *  @ param	 camera_id					-[in] ����IPC��ID
 *  @ param	 buff						-[in] ���������XML��Buf
 *  @ param	 buff_len					-[in] ���������XML��Buf�ĳ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_GetSmartIPCAbility(unsigned long long login_id, 
														  const char* camera_id, 
														  char* buff, 
														  unsigned int buff_len);



/******************************************************************************
 *                                                                            *
 *                              ��ͼ��ͼ									  *
 *                                                                            *
 ******************************************************************************/


/** @ fn	 HCS_SDK_SBPStart
 *  @ brief	 ������ͼ��ͼ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 cameras				-[in] ������ID��ά����
 *  @ param  camera_num				-[in] ����������
 *  @ param  begin_time				-[in] ��ѯͼƬ�Ŀ�ʼʱ�� 
 *  @ param  end_time				-[in] ��ѯͼƬ�Ľ���ʱ�� 
 *  @ param  pic_mode				-[in] ��ѯͼƬ�Ľ�ģ����
 *  @ param	 handle					-[out] ��ͼ��ͼ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPStart(unsigned long long login_id, 
												const char* cameras[HCS_CAMERA_ID_LEN], 
												unsigned int camera_num, 
												HCS_Time begin_time, 
												HCS_Time end_time,
												const HCS_PicModel* pic_mode,
												int* handle);


/** @ fn	 HCS_SDK_SBPNextInfo
 *  @ brief	 ��ͼ��ͼ��������һ����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ param	 result					-[out]�������������Ϣ
 *  @ param	 status					-[out]�������[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ param	 speed					-[out]��ͼ��ͼ����[0-100]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPNextInfo(unsigned long long login_id, 
												   int handle, 
												   HCS_SBPRet* result, 
												   int* status,
												   int* speed);


/** @ fn     HCS_SDK_SBPStop
 *  @ brief	 ֹͣ��ͼ��ͼ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SBPStop(unsigned long long login_id, 
											   int handle);




/******************************************************************************
 *                                                                            *
 *                              ��ע����									  *
 *                                                                            *
 ******************************************************************************/

/** @ fn     HCS_SDK_SetTag
 *  @ brief	 ���ñ�ע
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 camera_id				-[in] ������ID
 *  @ param	 tag_time				-[in] ��עʱ���
 *  @ param	 tag					-[in] ��ע����
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_SetTag(unsigned long long login_id, 
											  const char* camera_id,
											  HCS_Time tag_time,
											  const char* tag);




/** @ fn	 HCS_SDK_TagSearchStart
 *  @ brief	 ��ʼ��ע��ѯ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 camera_id				-[in] ������ID
 *  @ param  begin_time				-[in] ��ѯ�Ŀ�ʼʱ�� 
 *  @ param  end_time				-[in] ��ѯ�Ľ���ʱ�� 
 *  @ param	 tag_key				-[in] ��ע���ݹؼ���
 *  @ param  count					-[out]��ע��Ŀ
 *  @ param	 handle					-[out] ��ע��ѯ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchStart(unsigned long long login_id, 
													  const char* camera_id, 
													  HCS_Time begin_time, 
													  HCS_Time end_time,
													  const char* tag_key,
													  int* count,
													  int* handle);


/** @ fn	 HCS_SDK_TagSearchNext
 *  @ brief	 ��ע��ѯ��������һ����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ param	 result					-[out]�������������Ϣ
 *  @ param	 status					-[out]�������[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchNext(unsigned long long login_id, 
													 int handle, 
													 HCS_TagInfo* result, 
													 int* status);


/** @ fn     HCS_SDK_TagSearchStop
 *  @ brief	 ֹͣ��ע��ѯ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_TagSearchStop(unsigned long long login_id, 
													 int handle);



/** @ fn     HCS_SDK_DelTag
 *  @ brief	 ɾ����ע
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 camera_id				-[in] ������ID
 *  @ param  begin_time				-[in] ��ע��ʼʱ�� 
 *  @ param  end_time				-[in] ��ע����ʱ�� 
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_DelTag(unsigned long long login_id, 
											  const char* camera_id,
											  HCS_Time begin_time, 
											  HCS_Time end_time);



/** @ fn	 HCS_SDK_RecSearchByTagStart
 *  @ brief	 ��ʼ���ݱ�ע��ѯ¼�����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 camera_id				-[in] ������ID
 *  @ param	 tag_key				-[in] ��ע���ݹؼ���
 *  @ param  count					-[out]¼�����Ŀ
 *  @ param	 handle					-[out] ¼���ѯ���
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagStart(unsigned long long login_id, 
														   const char* camera_id, 
														   const char* tag_key,
														   int* count,
														   int* handle);


/** @ fn	 HCS_SDK_RecSearchByTagNext
 *  @ brief	 ���ݱ�ע��ѯ¼�����Ϣ��������һ����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ param	 result					-[out]�������������Ϣ
 *  @ param	 status					-[out]�������[1-��ȡ��Ϣ�ɹ� 2-���ڲ�ѯ��ȴ� 3-û�и������Ϣ����ѯ����]
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagNext(unsigned long long login_id, 
														  int handle, 
														  HCS_RecordInfo* result, 
														  int* status);


/** @ fn     HCS_SDK_TagSearchStop
 *  @ brief	 ֹͣ���ݱ�ע��ѯ¼�����Ϣ
 *  @ param	 login_id				-[in] �ƴ洢��¼ID
 *  @ param	 handle					-[in] �������
 *  @ return HCS_OK-�ɹ� ����-������
 */
HCS_SDK_EXTERN int HCS_SDK_API HCS_SDK_RecSearchByTagStop(unsigned long long login_id, 
														  int handle);


#endif // HCS_SDK_H_
