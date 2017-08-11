/**	@file    Common.h 
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   user defined data
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
#ifndef GUARD_COMMON_H
#define GUARD_COMMON_H
#include <string>

//��������ֵ
#define ADJUST_ERROR_SUCC 0 //�ɹ�
#define ADJUST_ERROR_FAIL 1 //ͨ��ʧ��
#define ADJUST_ERROR_DB   2 //���ݿ����ʧ��
#define ADJUST_ERROR_CFG  3 //������ش���
#define ADJUST_ERROR_DVR  4 //DVR������ش���
#define ADJUST_ERROR_HPP  5 //HPP������ش���

//���²���
typedef enum _adjustmode {
    ADJUST_TIMING_MODE   = 0, //ÿ�춨ʱУʱ
    ADJUST_INTERVAL_MODE = 1, //�̶�ʱ����Уʱ
    ADJUST_EXEC_ONCE = 2,     //ִֻ��һ��
}adj_mode_en;

static const DWORD TP_ALARM_INIT = 1;//��ʼ�����
static const DWORD TP_ALARM_DB = 2;//���ݿ����
static const DWORD TP_ALARM_FILE = 3;//�ϴ��ļ��������
static const DWORD TP_ALARM_STORAGE = 4;//���Ĵ洢���
static const DWORD TP_ALARM_CENTER = 5;//����web���
static const DWORD TP_ALARM_DVR  = 6;//DVR�������
static const DWORD TP_ALARM_ADJUST = 7;//ϵͳѲ��������
static const DWORD TP_ALARM_CVR    = 8;//CVR�������
static const DWORD TP_ALARM_CLOUD  = 9;//CLOUD�������
static const DWORD TP_ALARM_KMS    = 10;//KMS�������
static const DWORD TP_ALARM_OBJECTCLOUD = 11;//��׼�ƶ���洢�������


#define DEVICE_LOGIN_FAILED -1

//������Ϣ
typedef struct _configinfo {
    adj_mode_en  enAdjustMode;     //���²���
    unsigned int uiTimingHour;     //Уʱʱ�� ʱ  ÿ�ն�ʱУʱ������
    unsigned int uiTimingMin;      //Уʱʱ�� ��  ÿ�ն�ʱУʱ������
    unsigned int uiTimingSec;      //Уʱʱ�� ��  ÿ�ն�ʱУʱ������
    unsigned int uiInterval;       //Уʱʱ���� Сʱ �̶�ʱ����Уʱ������
    unsigned int uiMaxTimeLag;     //dvr�ͱ��������Զ�Уʱ�����ʱ��� ��λ����
}config_info_t;

//������Ϣ
typedef struct _dvrinfo {
    std::string   strDvrIp;    //ip
    unsigned short  usPort;      //�˿�
    std::string   strUserName; //�û���
    std::string   strPassWd;   //����
}collect_info_t;

//�洢������
enum ENUM_STORAGE_TYPE
{
	STORAGE_TYPE_CVR           = 50011,   //CVR
	STORAGE_TYPE_CLOUD         = 50015,   //�ƴ洢
	STORAGE_TYPE_KMS           = 50050,   //KMS
	STORAGE_TYPE_OBJECTCLOUD   = 50060,   //��׼��-����洢
	STORAGE_TYPE_MAX
};

//�洢�豸��Ϣ
typedef struct _Struct_StorageDevice
{
	int                 nType;          //�洢�豸����
	std::string         strName;        //�洢�豸����
	std::string         strIp;          //�洢�豸ip
	int                 nPort;          //�洢�豸�˿�
	std::string         strUser;        //�洢�豸�û���
	std::string         strPwd;         //�洢�豸����
	std::string         strAccessKey;   //����洢Access Key
	std::string         strSecretKey;   //����洢Secret Key
	int                 nReginId;       //�洢�豸����������ˮ��
	_Struct_StorageDevice()
	{
		nType    = 0;
		nPort    = 0;
		nReginId = 0;
	}
}Struct_StorageDevice;


#define HPP_WORKER_THREAD	 10	//HPP�����߳���
#define HPP_TIMEOUT			 10	//HPP��ʱʱ��.��λ��

#define TOP_DIRECTORY_CONFIG_FILE   "./ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

#define NODE_PARAM_TIMING     "PARAM_TIMING"
#define NODE_PARAM_ALARM      "PARAM_ALARM"

//��¼���
typedef enum _loginResult
{
	LOGIN_RESULT_FAIL = -1,//��¼ʧ��
	LOGIN_RESULT_OK = 0,//��¼�ɹ�
}ENUM_LOGIN_RESULT;


static const int CLOUD_GET_NEXT_POOL_SUCCESS   = 1; //��ȡ��Ϣ�ɹ�
static const int CLOUD_GET_NEXT_POOL_NEED_WAIT = 2; //���ڲ�ѯ��ȴ�
static const int CLOUD_GET_NEXT_POOL_FINISH    = 3; //��ѯ����

static const int ONLINE  = 1;
static const int OFFLINE = 0;


#endif //#define GUARD_COMMON_H
