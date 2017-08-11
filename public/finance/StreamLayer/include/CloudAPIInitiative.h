/** @file   CloudAPIInitiative.h
*  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief  API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date   2013-01-17
*
*  @change log
*
*/

#ifndef __CLOUD_API_INITIATIVE_H__
#define __CLOUD_API_INITIATIVE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CAMERAID_LEN		64
#define MAX_DEV_DATA_LEN		64
#define MAX_SDK_DAYS			7
#define MAX_SDK_TIMESEGMENT		8
#define MAX_IP_LEN				64

typedef unsigned char       BYTE;
typedef unsigned long       DWORD;

extern "C"
{

	// ¼����Ϣ���ݽṹ����
	typedef struct tagstrRecordInfo
	{
		char CamerID[MAX_CAMERAID_LEN] ;   // ������ID
		unsigned int StartTime;			   // ¼������ʱ��
		unsigned int CoverTime;			   // ���Ǻ��¼������ʱ��
		unsigned int LastTime;			   // ¼�����ʱ�� 
		long long ToatalSizeunsigned;	   // ¼���ļ����ܴ�С
	}strRecordInfo;


	// �豸״̬���ݽṹ����
	typedef struct tagDevStatus
	{
		char CamerID[MAX_CAMERAID_LEN];	   // ������ID
		unsigned int Online;	           // ����������״̬ [0:�����ߡ�1:����]
		unsigned int Status;	           // ¼��״̬ [0:�쳣��1:����]
		unsigned int Defence;	           // ����״̬ [0:û�в�����1:����]
		unsigned int Stream;	           // ȡ��״̬ [0:�쳣��1:����]
		unsigned int Analy;		           // ֡����״̬ [0:�쳣��1:����]
	}DevStatus;



	//ʱ���(�ӽṹ)
	typedef struct _sdk_schedtime
	{
		//��ʼʱ��
		unsigned char byStartHour;
		unsigned char byStartMin;

		//����ʱ��
		unsigned char byStopHour;
		unsigned char byStopMin;
	}SDK_SCHEDTIME, *LPSDK_SCHEDTIME;

	//¼��ƻ�
	typedef struct _sdk_recordday
	{
		unsigned short wAllDayRecord;                    // �Ƿ�ȫ��¼��[0:��; 1:��]
		unsigned char byRecordType;                      // [0:��ʱ¼��; 1:�ƶ����¼��]
		char    reservedData;                            // ����ֵ
	}SDK_RECORDDAY, *LPSDK_RECORDDAY;

	typedef struct _sdk_recordsched
	{
		SDK_SCHEDTIME struRecordTime;                    // ¼��ʱ��
		unsigned char byRecordType;                      // ¼������[0:��ʱ¼��; 1:�ƶ����¼��]
		char reservedData[3];//
	}SDK_RECORDSCHED, *LPSDK_RECORDSCHED;

	typedef struct _sdk_recordplan{
		unsigned int dwSize;                            // �ṹ���С
		unsigned int dwRecord;                          // �Ƿ�¼��[0:��; 1:��]
		unsigned int nStreamType;						//�������͡�0-��������1-��������
		SDK_RECORDDAY struRecAllDay[MAX_SDK_DAYS];
		SDK_RECORDSCHED struRecordSched[MAX_SDK_DAYS][MAX_SDK_TIMESEGMENT];
		unsigned int dwRecordTime;                      // (����)¼����ʱ����0-5�룬1-20�룬2-30�룬3-1���ӣ�4-2���ӣ�5-5���ӣ�6-10����
		unsigned int dwPreRecordTime;                   // Ԥ¼[0:��Ԥ¼; 1:��ʼԤ¼]
		unsigned int dwRecorderDuration;	            // (����)¼�񱣴���ʱ��
		unsigned char byRedundancyRec;	                // (����)�Ƿ�����¼��,��Ҫ����˫���ݣ�/1
		unsigned char byAudioRec;	                    // (����)¼��ʱ����������ʱ�Ƿ��¼��Ƶ���ݣ������д˷���
		unsigned char byReserve[10];                    // (����)
	}SDK_RECORD, *LPSDK_RECORD;


	// ¼����Ϣ���ݽṹ����
	typedef struct strRecordPlan
	{
		char szDevIP[MAX_IP_LEN];                  // ǰ���豸IP
		char szCameraID[MAX_CAMERAID_LEN];         // ������ID
		char szCameraName[MAX_CAMERAID_LEN];	   // ����������
		char szUserName[MAX_DEV_DATA_LEN];         // ��¼ǰ���豸���û���
		char szPassword[MAX_DEV_DATA_LEN];         // ��¼ǰ���豸������
		unsigned int nDevPort;                     // ǰ���豸�˿ں�
		unsigned int nDevType;                     // �豸����[0:������1:��]
		unsigned int nChanlNum;                    // �豸ͨ����
		unsigned short nTransType;                 // ��ý���ǰ���豸ȡ����Э������[0:RTSP��1:RTP/TCP��2:RTP/UDP��
		char szStreamIP[MAX_IP_LEN];               // ��ý�������IP
		unsigned int nStreamPort;                  // ��ý��˿ں�
		unsigned int nRecordPoolID;                // ¼���ID
		SDK_RECORD pRecordTask;                    // ¼��ƻ�
		unsigned int nAlloc;					   // ¼��ƻ��·�״̬[0:δ�·���1:���·�]
	}SDK_RECORDPLAN, *LPSDK_RECORDPLAN;




	// ��ȡ�豸״̬��Ϣ�ص�����
	// DevStatus *DevStatusInfo    �豸״̬��Ϣ�ṹ
	// unsigned int Count          ��ѯ�����豸״̬��Ϣ������
	// int Flat                    ʣ���豸״̬��Ϣ����[����0: ��ʾ��Ϣȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ�����Ϣ����]
	//void *pUserData              �û��Զ�����Ϣ
	
	//  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ��ȡ��ǰ�ƴ洢ϵͳ���豸״̬��Ϣ����
	//        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�����
	typedef void (*CDevStatusCallBack)(DevStatus *DevStatusInfo, unsigned int Count, int Flat, void *pUserData);


	// ��ȡ¼����Ϣ�ص�����
	// strRecordInfo *RecordInfo   ¼����Ϣ�ṹ
	// unsigned int Count          ��ѯ����¼����Ϣ������
	// int Flat                    ʣ��¼����Ϣ����[С�ڵ���0: ��ʾ��Ϣȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ�����Ϣ����]
	// void *pUserData             �û��Զ�����Ϣ    
	
	//  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ��ȡ��ǰ�ƴ洢ϵͳ��¼����Ϣ����
	//        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�����           
	typedef void (*CRecordInfoCB)(strRecordInfo *RecordInfo, unsigned int Count, int Flat, void *pUserData) ;


	// ��ȡԤ����Ƶ���ݻص�����
	// int PlayViewHandle          �ɹ�����Ԥ�����صľ��
	// int nDataType               ��ȡ����������[1:�ļ�ͷ��2:��Ƶ���ݡ�3:��Ƶ����]
	// void *pDataBuf              ��ȡ����Ƶ����(��pDataBufΪNULL��nBufLenΪ0ʱ��ʾ���ݻص�����)
	// unsigned int nBufLen        ��ȡ����Ƶ���ݳ���
	// void *pUserData             �û��Զ�����Ϣ
	typedef void (*CPlayVeiwDataCB)(int PlayViewHandle, int nDataType, void *pDataBuf, unsigned int nBufLen, void *pUserData);


	// ��ȡ¼��ƻ���Ϣ�ص�����
	// strRecordPlan *RecordInfo   ¼����Ϣ���ݽṹ
	// unsigned int Count          ��ѯ����¼��ƻ���Ϣ������
	// int Flat                    ʣ��¼��ƻ���Ϣ����[С�ڵ���0: ��ʾ��Ϣȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ�����Ϣ����]
	// void*pUserData              �û��Զ�����Ϣ
	
	//  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ��ȡ��ǰ�ƴ洢ϵͳ��¼��ƻ���Ϣ����
	//        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�����
	typedef void (*CRecordPlanCB)(strRecordPlan *RecordInfo, unsigned int Count, int Flat, void*pUserData);

}//extern "C"

#endif  //__CLOUD_API_INITIATIVE_H__
