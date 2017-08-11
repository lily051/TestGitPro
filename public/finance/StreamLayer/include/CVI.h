#ifndef CLOUD_CVI
#define CLOUD_CVI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CloudAPIInitiative.h"
#include "CloudAPIPublic.h"

#ifdef _WIN32
#ifdef CLOUD_API_EXPORTS
#define CLOUD_API __declspec(dllexport)
#else
#define CLOUD_API __declspec(dllimport)
#endif
#else
#define CLOUD_API
#endif




extern "C"
{

	/** @fn CLOUD_API int API_GetAllDevStatus(unsigned int loginID, CDevStatusCallBack pDevStatusFunc,  void *pUserData)
	*  @brief ��ȡ���е��豸״̬
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pDevStatusFunc[in]       �豸״̬��Ϣ�ص�����
	*  @param Count[out]               ��ѯ�����豸״̬������
	*  @param pUserData[in]            �û��Զ�������
	*  @@return �ɹ�=0 
	            ʧ��<0:������
	*/
	CLOUD_API int API_GetAllDevStatus(unsigned int loginID, CDevStatusCallBack pDevStatusFunc, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_DevStatus(unsigned int loginID,const char *CameraID, char *DevIP, int &Status,int& Defence,int& ReStatus,int& StreamStatus,int& FrameAnalyze)
	*  @brief ��ȡ�����豸״̬
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID
	*  @param Online[out]              �豸����״̬[0:�����ߡ�1:����]
	*  @param Status[out]              ��ǰ¼��״̬[0:�쳣��1:����]
	*  @param Defence[out]             ����״̬[0:δ������1:�Ѿ�����]
	*  @param Stream[out]              ��ȡ��Ƶ��״̬[0:�쳣��1:����]
	*  @param Analy[out]               ֡����״̬[0:�쳣��1:����]
	*  @@return �ɹ�=0 
	            ʧ��<0: ������
	*/
	CLOUD_API int API_DevStatus(unsigned int loginID, const char *CameraID, unsigned int &Online, unsigned int& Status, unsigned int& Defence, unsigned int& Stream, unsigned int& Analy);


	/** @fn CLOUD_API int API_GetCloudStatus(unsigned int loginID, int *pStatus)
	*  @brief ��ȡ�ƴ洢ϵͳ״̬
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pStatus[out]             �ƴ洢״̬[1:���ߡ�0:������]
	*  @@return �ɹ�=0 
	            ʧ��<0: ������
	*/
	CLOUD_API int API_GetCloudStatus(unsigned int loginID, int &pStatus);


	/** @fn int API_AddRecordPlan(unsigned int loginID, strRecordPlan *pRecordPlan)
	*  @brief ���¼��ƻ�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pRecordPlan[in]          ¼��ƻ���Ϣ 
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AddRecordPlan(unsigned int loginID, const strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_DelRecordPlan(unsigned int loginID, char *CameraID)
	*  @brief ɾ��¼��ƻ�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pRecordPlan[in]          ������ID
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_DelRecordPlan(unsigned int loginID, const char *CameraID);


	/** @fn CLOUD_API int API_ModifyRecordPlan(unsigned int loginID, strRecordPlan *pRecordPlan)
	*  @brief ɾ��¼��ƻ�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pRecordPlan[in]          ¼��ƻ���Ϣ
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_ModifyRecordPlan(unsigned int loginID, const strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_GetRecordPlan(unsigned int loginID, char *CameraID, strRecordPlan *pRecordPlan)
	*  @brief ��ȡָ����������¼��ƻ�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID
	*  @param pRecordPlan[out]         ¼��ƻ�����Ϣ
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_GetRecordPlan(unsigned int loginID, const char *CameraID, strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_GetAllRecordPlan(unsigned int loginID, unsigned int *pCount, CRecordInfoCB pRecordInfoCB, void *pUserData)
	*  @brief ��ѯ����¼��ƻ��б�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pRecordPlanCB[in]        ¼��ƻ���Ϣ�ص�����
	*  @param Count[in]                ��ѯ����¼��ƻ�������
	*  @param pUserData[in]            �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_GetAllRecordPlan(unsigned int loginID, CRecordPlanCB pRecordPlanCB, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_GetRecordInfoCB(unsigned int loginID, unsigned int *pCount, CRecordInfoCB pRecordInfoCB, void *pUserData)
	*  @brief ��ѯ����¼����Ϣ�б�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param pRecordInfoCB[in]        ¼����Ϣ�ص�����
	*  @param Count[in]                ��ѯ����¼����Ϣ������
	*  @param pUserData[in]            �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_GetAllRecordInfo(unsigned int loginID, CRecordInfoCB pRecordInfoCB, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_GetRecordInfo(unsigned int loginID, char *CameraID, strRecordInfo *pRecordInfo)
	*  @brief ��ѯָ����������¼����Ϣ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID
	*  @param pRecordInfo[out]         ¼����Ϣ�ṹ
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_GetRecordInfo(unsigned int loginID, const char *CameraID, strRecordInfo *pRecordInfo);


	/** @fn CLOUD_API int API_AddEventRecord(unsigned int loginID, char *CameraID, unsigned int nRecordType, unsigned int nTime)
	*  @brief ����¼�¼��(ʹ������ķ�ʽ�·�¼������)
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID 
	*  @param nRecordType[in]          ¼������[3~254֮��]
	*  @param nFlag[in]                ¼����������[0 �C ��ʼ/����¼��1 �C ֹͣ¼��]
	*  @param nTime[in]                ¼����ʱʱ��[�ڹ涨ʱ����û���յ���һ��¼���������ֹͣ�¼�¼��]
	*  @@return �ɹ�=0
	            ʧ��<0: ������
	*/
	CLOUD_API int API_AddEventRecord(unsigned int loginID, const char *CameraID, unsigned int nRecordType, unsigned int nFlag, unsigned int nTime);



	/** @fn CLOUD_API int API_PlayViewStart(unsigned int loginID, char *CameraID, unsigned short StreamType, CPlayVeiwDataCB PlayViewDataCB, void *pUserData)
	*  @brief ����Ԥ��
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID 
	*  @param StreamType[in]           ��������[0 �C ��������1 �C ������]
	*  @param PlayViewDataCB[in]       Ԥ�����ݻص�����
	*  @param pUserData[in]            �û��Զ�������
	*  @@return �ɹ�=>0: ����Ԥ�����صľ��
	            ʧ��<0: ������
	*/
	CLOUD_API int API_PlayViewStart(unsigned int loginID, const char *CameraID, unsigned short StreamType, CPlayVeiwDataCB PlayViewDataCB, void *pUserData);


	/** @fn CLOUD_API int API_PlayViewStop(unsigned int loginID , int PlayViewHandle)
	*  @brief ֹͣԤ��
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PlayViewHandle[in]       �ɹ�����Ԥ�����صľ��  
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PlayViewStop(unsigned int loginID , int PlayViewHandle);
}

#endif //CLOUD_CVI