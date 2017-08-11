/**	@file GlobalParam.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief ȫ�ֵĲ������á�
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once

#include <string>

#define HPP_WORKER_THREAD	 10	//HPP�����߳���
#define HPP_TIMEOUT			 10	//HPP��ʱʱ��.��λ��

#define TOP_DIRECTORY_CONFIG_FILE   "ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

#define NODE_PARAM_UPLOAD   "PARAM_UPLOAD"
#define NODE_PARAM_ATSERVER "PARAM_ATSERVER"
#define NODE_PARAM_COLLECT  "PARAM_COLLECT"

//��¼���
typedef enum _loginResult
{
    LOGIN_RESULT_FAIL = -1,//��¼ʧ��
    LOGIN_RESULT_OK = 0,//��¼�ɹ�
}ENUM_LOGIN_RESULT;

//¼������
enum ENUM_RECORD_TYPE
{
	RECORD_TYPE_NORMAL        = 1, //�ֶ�¼��
	RECORD_TYPE_REPAIR        = 2, //��¼¼��
	RECORD_TYPE_MAX           = 3, 
};

//¼��״̬
enum ENUM_RECORD_STATUS
{
    RECORDSTAT_READY        = 2, //���ϴ�
    RECORDSTAT_UPLOADING    = 3, //�ϴ���
    RECORDSTAT_OVER         = 4, //�ϴ����
    RECORDSTAT_FAIL         = 9, //�ϴ�ʧ��
    RECORDSTAT_DELETE       = 100,//ɾ��
};

//¼��洢����
enum ENUM_RECORD_STORAGE_TYPE
{
    RECORD_STORAGETYPE_NORMAL       = 1,//DVR¼��
    RECORD_STORAGETYPE_LOCAL_KMS    = 2,//����¼���ϴ�KMS
	RECORD_STORAGETYPE_OBJECT_CLOUD = 3,//����¼���ϴ��ƴ洢������洢ģʽ
};

//�ļ�����
enum ENUM_FILE_TYPE
{
	FILE_TYPE_VIDEO       = 1,//¼��
	FILE_TYPE_AUDIO       = 2,//¼��
	FILE_TYPE_PICTURE     = 3,//ͼƬ
	FILE_TYPE_OTHER       = 4,//����
};

//�洢������
enum ENUM_STORAGE_TYPE
{
    STORAGE_TYPE_CVR = 0,      //CVR
    STORAGE_TYPE_CLOUD,        //�ƴ洢
    STORAGE_TYPE_KMS,          //KMS
	STORAGE_TYPE_OBJECT_CLOUD, //�ƴ洢������洢
    STORAGE_TYPE_MAX
};

static const DWORD TP_ALARM_INIT = 1;//��ʼ�����
static const DWORD TP_ALARM_DB = 2;//���ݿ����
static const DWORD TP_ALARM_FILE = 3;//�ϴ��ļ��������
static const DWORD TP_ALARM_STORAGE = 4;//���Ĵ洢���
static const DWORD TP_ALARM_CENTER = 5;//����web���

//�洢�豸��Ϣ
typedef struct _Struct_StorageConfig
{
    ENUM_STORAGE_TYPE   storageType;    //�洢�豸����
//    LONG                lLoginID;       //��¼���
    std::string         strIp;          //�洢�豸ip
    int                 nPort;          //�洢�豸�˿�
    std::string         strUser;        //�洢�豸�û���
    std::string         strPwd;         //�洢�豸����
	std::string         strAccessKey;   //����洢Access Key(�����ƴ洢ר��)
	std::string         strSecretKey;   //����洢Secret Key(�����ƴ洢ר��)
//     std::string         strStartTime;   //�洢�豸��ʼ����ʱ��
//     std::string         strStopTime;    //�洢�豸ֹͣ����ʱ��
    int                 nPoolId;        //�洢�豸¼����
    _Struct_StorageConfig()
    {
        storageType = STORAGE_TYPE_MAX;
        nPort = 0;
        nPoolId = 0;
    }
}Struct_StorageConfig;

typedef struct _Struct_RecordTime
{
    DWORD	dwYear;					//��
    DWORD	dwMonth;				//��
    DWORD	dwDay;					//��
    DWORD	dwHour;					//ʱ
    DWORD	dwMinute;				//��
    DWORD	dwSecond;				//��
	_Struct_RecordTime()
	{
		dwYear   = 0;
		dwMonth  = 0;
		dwDay    = 0;
		dwHour   = 0;
		dwMinute = 0;
		dwSecond = 0;
	}
}Struct_RecordTime;

//¼����Ϣ
typedef struct _Struct_RecordInfo
{
    ENUM_RECORD_STATUS       recordStatus;       //¼��״̬
    ENUM_RECORD_STORAGE_TYPE recordStorageType;  //¼��洢����   
	ENUM_FILE_TYPE          fileType;           //�ļ�����(�����ƴ洢ר��)
    int                     nID;                //�����ݿ��е���ˮ��(�����ƴ洢ר��)
    int                     nUserID;            //�û�ID
    std::string             strDvrIp;           //¼�������DVRIP
    int                     nDvrPort;           //¼�������DVR�˿�
    std::string             strDvrUser;         //DVR�û���
    std::string             strDvrPwd;          //DVR����
    int                     nChannelID;         //DVRͨ����
    Struct_RecordTime       struRecStartTime;   //¼��ʼʱ��
    Struct_RecordTime       struRecStopTime;    //¼�����ʱ��
    std::string             strFilePath;        //�����ļ�·��
    std::string             strCameraID;        //(�ƴ洢:�����ı�����ID��CVR:��������ID�������ƴ洢���ļ�Key)
    int                     nTaskID;            //����ID
    int                     nUploadPercent;     //�ϴ�����
    int                     nFailTimes;         //�ϴ�ʧ�ܴ���
    std::string             strRtspPath;        //�ϴ��ļ���RTSP·����KMSר�ã�
    std::string             strStartTime;       //�ϴ��ļ��Ŀ�ʼʱ�䣨KMS�������ƴ洢ר�ã�
	std::string             strEndTime;         //�ϴ��ļ��Ľ���ʱ�䣨KMS�������ƴ洢ר�ã�
	std::string             strHost;            //�����ļ�����������ַ  (�����ƴ洢ר��)
	std::string             strBucket;          //����洢Bucket(�����ƴ洢ר��)
    std::string             strRecordName;      //��¼����
    int                     nFileSize;          //����¼���ļ���С
    Struct_StorageConfig    struStorage;        //�洢�豸��Ϣ
    _Struct_RecordInfo()
    {
        recordStatus = RECORDSTAT_READY;
        recordStorageType = RECORD_STORAGETYPE_NORMAL;
		fileType = FILE_TYPE_VIDEO;
		nID = -1;
        nUserID = 0;
        nDvrPort = 0;
        nChannelID = -1;
        nTaskID = -1;
        nUploadPercent = 0;
        nFailTimes = 0;
        nFileSize = 0;
    }
}Struct_RecordInfo;

//�洢�豸��ѯ���Ļش�¼����Ϣ
typedef struct _Struct_UploadRecordParam
{
	Struct_RecordTime       struRecStartTime;   //¼��ʼʱ��
	Struct_RecordTime       struRecStopTime;    //¼�����ʱ��
	DWORD                   dwRecordNum;        //¼�����
	BYTE                    byLockFlag;         //������־ 0��û���� 1������
	DWORD                   dwRecordLength;     //¼���ܴ�С
	_Struct_UploadRecordParam()
	{
		dwRecordNum    = 0;
		byLockFlag     = 0;
		dwRecordLength = 0;
	}
}Struct_UploadRecordParam;
