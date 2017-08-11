#ifndef GUARD_SVCGENERALDEF_H
#define GUARD_SVCGENERALDEF_H
#pragma once

/**	@file    SvcGeneralDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   DLL��ʱ��Ϣ���干��ͷ�ļ�
 *
 *	@author	 yuchengzhi
 *	@date	 2012/8/22
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V1.0.0  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

//ģ�����Ͷ���,����:
static const DWORD SYS_INFO      = 6000; // ϵͳ
static const DWORD SVC_RECGUARD  = 6001; // ¼������ģ��
static const DWORD SVC_ADJUST    = 6002; // Уʱģ��
static const DWORD SVC_TRANS     = 6003; // �ϴ�ģ��
static const DWORD SVC_VRM       = 6004; // VRMģ��

static const DWORD SERVER_TYPE_ADJUSTTIME    = 20001;// ϵͳѲ�����
static const DWORD SERVER_TYPE_MEDIAGUARD    = 20002;// ¼����������
static const DWORD SERVER_TYPE_TRANSPORT     = 20004;// �ϴ�����

static const DWORD DEV_TYPE_DVR                = 50000 ; // DVR
static const DWORD DEV_TYPE_ALARM_HOST         = 50001 ; //��������
static const DWORD DEV_TYPE_GATE               = 50002 ; //�Ž�
static const DWORD DEV_TYPE_TALK               = 50003 ; //�Խ�
static const DWORD DEV_TYPE_DECODER            = 50004 ; //������
static const DWORD DEV_TYPE_VIP                = 50005 ; //��Ƶ�ۺ�ƽ̨
static const DWORD DEV_TYPE_SC                 = 50006 ; //������(Safety Cabin)
static const DWORD DEV_TYPE_IPC                = 50007 ; // IPC
static const DWORD DEV_TYPE_DVS                = 50008 ; // DVS
static const DWORD DEV_TYPE_PCDVR              = 50009 ; // PCDVR
static const DWORD DEV_TYPE_AH_NETWORK_MODULE  = 50010 ; //������������ģ��
static const DWORD DEV_TYPE_CVR                = 50011 ; // CVR
static const DWORD DEV_TYPE_NVR                = 50012 ; // NVR
static const DWORD DEV_TYPE_ENVIRONMENT        = 50013 ; // ������
static const DWORD DEV_TYPE_FIRE               = 50014 ; // ����
static const DWORD DEV_TYPE_CLOUD_STORAGE      = 50015 ; // �ƴ洢
static const DWORD DEV_TYPE_GUN                = 50016 ; // ǹ����(�����������)
static const DWORD DEV_TYPE_YS7		           = 50017 ; //өʯ���豸
static const DWORD DEV_TYPE_KMS                = 50050 ; // KMS


//��Ϣ���Ͷ���
static const DWORD MSG_TYPE_NOTIFY   = 1;   // ��ͨ��Ϣ
static const DWORD MSG_TYPE_ALARM    = 2;   // ������Ϣ
static const DWORD MSG_TYPE_DOWNLOAD = 3;   // ���ؽ���
static const DWORD MSG_TYPE_UPLOAD   = 4;   // �ϴ�����

//��������
static const DWORD WORK_TYPE_CENTTER_OFFLINE           = 60001; // ���Ĳ�����        (δ��)
static const DWORD WORK_TYPE_TRANSPORT_SVC_OFFLINE     = 61001; // �ϴ���������    (δ��)
static const DWORD WORK_TYPE_TRANSPORT_SVC_FAIL        = 61002; // �ϴ�����ʧ��
static const DWORD WORK_TYPE_GUARD_SVC_OFFLINE         = 62001; // ¼�������������� (δ��)
static const DWORD WORK_TYPE_GUARD_SVC_FAIL            = 62002; // ¼����������ʧ��
static const DWORD WORK_TYPE_ADJUST_SVC_OFFLINE        = 63001; // Ѳ���������     (δ��)
static const DWORD WORK_TYPE_ADJUST_SVC_FAIL           = 63002; // ϵͳѲ�����ʧ��   
static const DWORD WORK_TYPE_PATROL_SVC_FAIL           = 64001; // �ɼ��豸Ѳ��ʧ��   (δ��)
static const DWORD WORK_TYPE_DVR_OFFLINE               = 64002; // �ɼ��豸������
static const DWORD WORK_TYPE_DVR_TIME_ERROR            = 64003; // �ɼ��豸ʱ��ͱ���ʱ���ܴ�
static const DWORD WORK_TYPE_DVR_VI_LOST               = 64004; // �ɼ��豸��Ƶ�źŶ�ʧ
static const DWORD WORK_TYPE_DVR_HD_EXCEPTION          = 64005; // �ɼ��豸Ӳ���쳣
static const DWORD WORK_TYPE_DVR_OPERATION_FAIL        = 64006; // �ɼ��豸ĳЩ����ʧ��  (δ��)
static const DWORD WORK_TYPE_DVR_NEARLLY_FULL          = 64007; // �ɼ��豸������
static const DWORD WORK_TYPE_CVR_OFFLINE               = 65001; // CVR������
static const DWORD WORK_TYPE_CVR_CANNOT_LOGIN          = 65002; // CVR��¼ʧ��           (δ��)
static const DWORD WORK_TYPE_CVR_NEARLLY_FULL          = 65003; // CVR������
static const DWORD WORK_TYPE_CVR_OPERATION_FAIL        = 65004; // CVRĳЩ����ʧ��       (δ��)
static const DWORD WORK_TYPE_CLOUD_OFFLINE             = 66001; // Cloud������
static const DWORD WORK_TYPE_CLOUD_CANNOT_LOGIN        = 66002; // Cloud��¼ʧ��         (δ��)
static const DWORD WORK_TYPE_CLOUD_GET_POOLLIST_FAIL   = 66003; // ��ȡ¼��س�ʱ        (δ��)
static const DWORD WORK_TYPE_CLOUD_NEARLLY_FULL        = 66004; // Cloud������
static const DWORD WORK_TYPE_CLOUD_OPERATION_FAIL      = 66005; // CloudĳЩ����ʧ��     (δ��)
static const DWORD WORK_TYPE_KMS_OFFLINE               = 67001; // KMS������
static const DWORD WORK_TYPE_KMS_NEARLLY_FULL          = 67002; // KMS������
static const DWORD WORK_TYPE_KMS_OPERATION_FAIL        = 67003; // KMSĳЩ����ʧ��       (δ��)
static const DWORD WORK_TYPE_VRM_SVC_OFFLINE           = 68001; // VRM��������         (δ��)
static const DWORD WORK_TYPE_OBJECTCLOUD_OFFLINE       = 69001; // ��׼��-����洢������
static const DWORD WORK_TYPE_OBJECTCLOUD_NEARLLY_FULL  = 69002; // ��׼��-����洢������

static const DWORD ALL_USER = 0;
static const DWORD ALL_CHANNEL = 255;

static const DWORD ALARM_NORMAL = 0;//�Ǳ���
static const DWORD ALARM_OCCUR = 1;//����

static const DWORD ALARM_REV_DEFAULT_VALUE = 0;

static const DWORD SVCMSG_CONTENT_LEN      = 1024; // ��ʱ��Ϣ������󳤶�
static const DWORD SVCMSG_RESULT_LEN       = 16;   // IP��ַ��󳤶�

#pragma pack(8)

// �ص���Ϣ
typedef struct STRUC_BASIC_MESSAGE{        
    DWORD    dwSvcType;
    DWORD    dwMsgType;
    STRUC_BASIC_MESSAGE()
        : dwSvcType(0)
        , dwMsgType(0)
    {}
} BASIC_MESSAGE, *pBASIC_MESSAGE;

//�ص���Ϣ
typedef struct STRUC_BASIC_MESSAGE_EX : public BASIC_MESSAGE{
    char   chContent[SVCMSG_CONTENT_LEN];
    char   chResult[SVCMSG_RESULT_LEN];
} BASIC_MESSAGE_EX, *pBASIC_MESSAGE_EX;

// ������Ϣ
typedef struct NOTIFY_MSG : public BASIC_MESSAGE{
	DWORD  dwUserID;
	DWORD  dwChannel;
    char   szNotifyInfo[SVCMSG_CONTENT_LEN]; // utf8����
} NOTIFY_MSG, *pNOTIFY_MSG;

// ������Ϣ
typedef struct ALARM_MSG_T : public BASIC_MESSAGE{
    DWORD  dwAlarmType;
    DWORD  dwStatus;
	DWORD  dwUserID;
	DWORD  dwChannel;
	DWORD  dwRev;
    char   szAlarmInfo[SVCMSG_CONTENT_LEN]; // utf8����
    char   szHost[SVCMSG_RESULT_LEN];
    ALARM_MSG_T()
        : dwAlarmType(0)
        , dwStatus(0)
        , dwUserID(0)
        , dwChannel(0)
        , dwRev(0)
    {
        for (int i = 0; i < SVCMSG_CONTENT_LEN; i++)
        {
            szAlarmInfo[i] = 0;
        }
        for (int i = 0; i < SVCMSG_RESULT_LEN; i++)
        {
            szHost[i] = 0;
        }
    }
} ALARM_MSG, *pALARM_MSG;

// �ϴ�¼�������Ϣ
typedef struct UPLOAD_MSG : public BASIC_MESSAGE{
    DWORD dwRecordID;
	DWORD dwUserID;
    DWORD dwUploadPercent;
} UPLOAD_MSG, *pUPLOAD_MSG;

// ��¼¼�������Ϣ
typedef struct DOWNLOAD_MSG : public BASIC_MESSAGE{
    DWORD  dwRecordID;
	DWORD  dwUserID;
    DWORD  dwDownloadPercent;
} DOWNLOAD_MSG, *pDOWNLOAD_MSG;

// ����������Ϣ
typedef struct HAEARTBEAT_MSG : public BASIC_MESSAGE{
	DWORD  dwCurrentTime;
} HAEARTBEAT_MSG, *pHAEARTBEAT_MSG;

//��Ϣ�ص�����ԭ��
typedef DWORD (__stdcall *pTransMsgBack)(const BASIC_MESSAGE *struMessage);
typedef DWORD (__stdcall *pTransMsgBackEx)(const void *msg, const size_t cbMsg);
//typedef DWORD (__stdcall *pMsgBack)(const BASIC_MESSAGE *struMessage);
//typedef DWORD (__stdcall *pMsgBackEx)(const void *msg, const size_t cbMsg);

#endif    // end of GUARD_SVCGENERALDEF_H
