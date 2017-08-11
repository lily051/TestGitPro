#pragma once

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//lint -library
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxctl.h>         // ActiveX �ؼ��� MFC ֧��
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

// �����ϣ��ʹ�� MFC ���ݿ��࣬
//  ��ɾ����������������ļ�
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#endif // _WIN64
#include <afxwin.h>

// hlog.
#include "hlog.h"
#include "hlogex.h"
#include "RPCLog.h"
#include "HCNetSDK.h"
//////////////////////////////////////////////////////////////////////////
//��ʼ�����������
typedef struct _tagInitLibParam 
{
    CString sLogPath;//��־·��
}INIT_LIB_PARAM;

//������
enum
{
    CODE_GET_DEV_INFO           = 8100,//��ȡ�豸��Ϣ
    CODE_GET_CHANNEL_NAME       = 8101,//��ȡͨ������
    CODE_SHOW_REMOTE_CONFIG     = 8102,//Զ������
    CODE_SET_NTPCONFIG
};

//�豸����
enum
{
    DEV_TYPE_HIK                = 0,//�����豸
	DEV_TYPE_CVR                = 1,//CVR
	DEV_TYPE_CLOUD              = 2,//�ƴ洢
};

// typedef struct _tagNTPConfigInfo
// {
//     CString sNTPServer; //NTP��������������IP��ַ 
//     int wInterval;       //Уʱ���ʱ�䣬�Է��ӻ�СʱΪ��λ 
//     int byEnableNTP;     //NTPУʱ�Ƿ����ã�0����1���� 
//     int NtpPort;         //NTP�������˿ڣ��豸Ĭ��Ϊ123 
//     _tagNTPConfigInfo()
//     {
//         sNTPServer.Empty();
//         wInterval = 0;
//         byEnableNTP = 1;
//         NtpPort = 123;
//     }
// }NTPCONFIGINFO;

//��������
typedef struct _tagFunParam 
{
    int nCode;//������
    int nDevType;//�豸����
    CString sDevIP;//�豸IP
    int nDevPort;//�豸�˿ں�
    CString sDevUser;//�û���
    CString sDevPWD;//����
    int nChannel;//ͨ����
    BOOL bShowIPMode;//�Ƿ���ʾIPͨ������
    NET_DVR_NTPPARA nNtpConfigInfo;  //����NTPУʱ
    _tagFunParam()
    {
        nCode = -1;
        nDevType = -1;
        nDevPort = 8000;
        nChannel = -1;
        bShowIPMode = FALSE;
        ZeroMemory(&nNtpConfigInfo, sizeof(NET_DVR_NTPPARA));
    }
}FUN_PARAM;

//Զ���豸��Ϣ
typedef struct _tagRPDeviceInfo
{
    CString strName;                           //�豸����
    int     nAlarmInNum;                   //DVR�����������
    int     nAlarmOutNum;                  //DVR�����������
    int     nDiskNum;                          //Ӳ�̸���
    int     nChanNum;                          //ͨ������
    int     nStartChanNum;                     //��ʼͨ����
    CString strSerialNumber;				   //���к�
    int     nAudioNum;                          //����ͨ����
    int     nIPChannel;                         // IPͨ������
    int     nIPChannelStart;                    // IPͨ����ʼ��
    _tagRPDeviceInfo()
    {
        nAlarmInNum = 0;
        nAlarmOutNum = 0;
        nDiskNum = 0;
        nChanNum = 0;
        nStartChanNum = 0;
        nAudioNum = 0;
        nIPChannel = 0;
        nIPChannelStart = 0;
    }
}RP_DEV_INFO;
//////////////////////////////////////////////////////////////////////////

