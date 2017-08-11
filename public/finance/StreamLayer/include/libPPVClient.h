// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBPPVCLIENT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// _API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef LIBPPVCLIENT_H__
#define LIBPPVCLIENT_H__

#include <time.h>

#ifdef LIBPPVCLIENT_EXPORTS
#define LIBPPVCLIENT_API __declspec(dllexport)
#else
#define LIBPPVCLIENT_API __declspec(dllimport)
#endif


//�����붨��
#define E_SUCCEED								( 0 )
#define E_FAILED								(-1)

#define E_CONNDEVICE_BASE						( -10800 )
#define E_CONNDEVICE_CANNOTCONNETSERVER			( E_CONNDEVICE_BASE - 1 )
#define E_CONNDEVICE_NETWORKJITTER				( E_CONNDEVICE_BASE - 2 )
#define E_CONNDEVICE_DEVNOTONLINE				( E_CONNDEVICE_BASE - 3 )
#define E_CONNDEVICE_TRANSFERFAILED				( E_CONNDEVICE_BASE - 4 )


#define E_SENDUPDATEINFO_BASE					( -11100 )
#define E_SENDUPDATEINFO_DEVICENOTONLINE		( E_SENDUPDATEINFO_BASE - 1 )
#define E_SENDUPDATEINFO_TRANSFERFAILED			( E_SENDUPDATEINFO_BASE - 2 )
#define E_SENDUPDATEINFO_CANNOTCONNETDEVICE		( E_SENDUPDATEINFO_BASE - 3 )
#define E_SENDUPDATEINFO_CANNOTCONNETSERVER		( E_SENDUPDATEINFO_BASE - 4 )
#define E_SENDUPDATEINFO_INVALIDDEVICEHANDLE	( E_SENDUPDATEINFO_BASE - 5 )
#define E_SENDUPDATEINFO_CMDNOTEXECUTED			( E_SENDUPDATEINFO_BASE - 6 )

// ý��ͷ�ļ�����(40�ֽ�)
#define HEAD_FILE_LENGTH						40

/* ��־���Ͷ���(��) */
/* alarm log */
#define MAJOR_ALARM 0x1        /*�������ú�ɫ���*/
#define MINOR_ALARM_IN 0x1 /* alarm in */
#define MINOR_ALARM_OUT 0x2 /* alarm out */
#define MINOR_MOTDET_START 0x3 /* motion detect start */
#define MINOR_MOTDET_STOP 0x4 /* motion detect stop */
#define MINOR_HIDE_ALARM_START 0x5 /* hide alarm start */
#define MINOR_HIDE_ALARM_STOP 0x6 /* hide alarm stop */
/* exception */
#define MAJOR_EXCEPTION 0x2
#define MINOR_VI_LOST 0x21 /* VI lost */
#define MINOR_ILLEGAL_ACCESS 0x22 /* illegal access */
#define MINOR_HD_FULL 0x23 /* hard disk full */
#define MINOR_HD_ERROR 0x24 /* hard disk error */
#define MINOR_DCD_LOST 0x25 /* modem DCD lost */
#define MINOR_IP_CONFLICT 0x26 /* ip address conflict */
#define MINOR_NET_BROKEN 0x27 /* net broken */
/* operation */
#define MAJOR_OPERATION 0x3
#define MINOR_START_DVR 0x41 /* start device */
#define MINOR_STOP_DVR 0x42 /* stop device */
#define MINOR_STOP_ABNORMAL 0x43 /* stop device abnormally */
#define MINOR_LOCAL_LOGIN 0x50 /* local login */
#define MINOR_LOCAL_LOGOUT 0x51 /* local logout */
#define MINOR_LOCAL_CFG_PARM 0x52 /* local config parameter */
#define MINOR_LOCAL_PLAYBYFILE 0x53 /* local playback by file */
#define MINOR_LOCAL_PLAYBYTIME 0x54 /* local playback by time */
#define MINOR_LOCAL_START_REC 0x55 /* local start record */
#define MINOR_LOCAL_STOP_REC 0x56 /* local stop record */
#define MINOR_LOCAL_PTZCTRL 0x57 /* local PTZ control */
#define MINOR_LOCAL_PREVIEW 0x58 /* local preview */
#define MINOR_LOCAL_MODIFY_TIME 0x59 /* local modify date/time */
#define MINOR_LOCAL_UPGRADE 0x5a /* local upgrade software */
#define MINOR_REMOTE_LOGIN 0x70 /* remote login */
#define MINOR_REMOTE_LOGOUT 0x71 /* remote logout */
#define MINOR_REMOTE_START_REC 0x72 /* remote start record */
#define MINOR_REMOTE_STOP_REC 0x73 /* remote stop record */
#define MINOR_START_TRANS_CHAN 0x74 /* start transparent channel */
#define MINOR_STOP_TRANS_CHAN 0x75 /* stop transparent channel */
#define MINOR_REMOTE_GET_PARM 0x76 /* remote get parameter */
#define MINOR_REMOTE_CFG_PARM 0x77 /* remote config parameter */
#define MINOR_REMOTE_GET_STATUS 0x78 /* remote get status */
#define MINOR_REMOTE_ARM 0x79 /* remote arm */
#define MINOR_REMOTE_DISARM 0x7a /* remote disarm */
#define MINOR_REMOTE_REBOOT 0x7b /* remote reboot device */
#define MINOR_START_VT 0x7c /* start voice talk */
#define MINOR_STOP_VT 0x7d /* stop voice talk */
#define MINOR_REMOTE_UPGRADE 0x7e /* remote upgrade software */
#define MINOR_REMOTE_PLAYBYFILE 0x7f /* remote playback by file */
#define MINOR_REMOTE_PLAYBYTIME 0x80 /* remote playback by time */
#define MINOR_REMOTE_PTZCTRL 0x81 /* remote PTZ control */



//�ͻ������豸��������
typedef enum
{
	CONN_MODE_P2P = 0,		//��Ե�
	CONN_MODE_TRANSFER,		//ͨ����������ת
}EN_CONN_MODE;

//��Ƶ���ݻص�ʱ�������ͱ�ʶ
typedef enum
{
	STREAM_TYPE_HDR = 0,		//��ͷ
	STREAM_TYPE_DATA,			//����
}EN_STREAM_DATATYPE;

//��̨��������
typedef enum
{
	PTZ_CMD_ZOOM_IN = 11, 		/* �����Ա��(���ʱ��) */
	PTZ_CMD_ZOOM_OUT, 			/* �����С(���ʱ�С) */
	PTZ_CMD_FOCUS_IN, 			/* ����ǰ�� */
	PTZ_CMD_FOCUS_OUT, 			/* ������ */
	PTZ_CMD_IRIS_ENLARGE,	 	/* ��Ȧ���� */
	PTZ_CMD_IRIS_SHRINK, 		/* ��Ȧ��С */
	PTZ_CMD_TILT_UP = 21, 		/* ��̨���� */
	PTZ_CMD_TILT_DOWN,			/* ��̨���� */
	PTZ_CMD_PAN_LEFT, 			/* ��̨��ת */
	PTZ_CMD_PAN_RIGHT, 			/* ��̨��ת */
	PTZ_CMD_UP_LEFT,			/* ��̨���� */
	PTZ_CMD_UP_RIGHT,			/* ��̨���� */
	PTZ_CMD_DOWN_LEFT,			/* ��̨���� */
	PTZ_CMD_DOWN_RIGHT,			/* ��̨���� */
	PTZ_CMD_SET_PRESET = 31,	/* ����Ԥ�õ� */
	PTZ_CMD_CLE_PRESET,			/* ���Ԥ�õ� */
	PTZ_CMD_GOTO_PRESET,		/* ת��Ԥ�õ� */
	PTZ_CMD_LIGHT_PWRON = 41,	/* �ƹ⿪�� */
	PTZ_CMD_WIPER_PWRON,		/* ��ˢ���� */
	PTZ_CMD_PAN_AUTO,           /* �Զ�Ѳ�� */
}EN_PTZ_COMMAND;

//vod��������
typedef enum
{
	VOD_CONTROL_CMD_GETPOS = 3,		/*��ȡ�طŽ���*/
	VOD_CONTROL_CMD_SETPOS,			/*���ûطŽ���*/
}EN_VOD_CONTROL_CMD;

// (�ļ�)ƫ������
typedef enum
{
	SEEK_BY_SIZE	=	1,			/*ƫ���ļ����� BYSIZE*/
	SEEK_BY_TIME	=	2,			/*ƫ��ʱ������ BYTIME*/
}SEEK_TYPE;

//ע���������¼��Ϣ
typedef struct  
{
	char				szAcessServerIP[32];
	unsigned short int	nAcessServerPort;
	unsigned short int	nReserved;
	char				szUserName[32];
	char				szUserPwd[32];
}ST_ACCESS_SERVER_INFO;

//��ý���������Ϣ
typedef struct  
{
	char				szStreamServerIP[32];      /*��ý�������IP*/
	unsigned short int  nStreamServerPort;         /*��ý��������˿�*/         
	unsigned short int  nReserved;                 /*����*/
}ST_STREAM_SERVER_INFO;

//������ȡ����Ϣ
typedef struct
{
	char				    pszDeviceID[32];        /*ȡ���豸��FrontId*/ 
	int					    iChan;					/*�豸��Ƶͨ����*/
	int						iStreamType;			/*��������*/
	ST_STREAM_SERVER_INFO   stStreamServerInfo;     /*��������ý���������Ϣ*/
	ST_ACCESS_SERVER_INFO   stAccessServerInfo;     /*������ע���������Ϣ*/
}ST_DECODE_STREAM_INFO;

//ץͼ��Ϣ
typedef struct
{
	int						iSaveTo;				/*ͼƬ����λ��,1:�ϴ����� 2:���汾�� 3:�ϴ�����&���ش洢*/
	int						iResolution;			/*ͼƬ�ֱ���*/
	int						iQuality;				/*ͼƬ����,40:Comman;60:better;80:Best*/
	char					szTime[32];				/*ͼƬʱ��*/
	int						iSequence;				/*ͼƬ���*/
	int						iPreset;				/*ͼƬץ�ĵ�Ԥ�õ�*/
	unsigned short int      nReserved;              /*����*/
}ST_CAPTUREPICTURE_INFO;

//------------ʡ��,��������HCNetSDK.h�е��ض��� ������-----------------
//typedef enum
//{
//	VGA_NOT_AVALIABLE,
//	VGA_THS8200_MODE_SVGA_60HZ,         //��800*600��
//	VGA_THS8200_MODE_SVGA_75HZ,         //��800*600��
//	VGA_THS8200_MODE_XGA_60HZ,          //��1024*768��
//	VGA_THS8200_MODE_XGA_70HZ,          //��1024*768��
//	VGA_THS8200_MODE_SXGA_60HZ,         //��1280*1024��
//	VGA_THS8200_MODE_720P_60HZ,         //��1280*720 ��
//	VGA_THS8200_MODE_1080i_60HZ,        //��1920*1080��
//	VGA_THS8200_MODE_1080P_30HZ,        //��1920*1080��
//	VGA_THS8200_MODE_1080P_25HZ,        //��1920*1080��
//	VGA_THS8200_MODE_UXGA_30HZ          //��1600*1200��
//}VGA_MODE;                              //����֧�ֵķֱ��ʴ��������л�ȡ��
//
////��������ʾͨ����Ϣ
//typedef struct  
//{                              
//	int			          iMode;					 /*����ָ�ģʽ1/4/9/16*/			 
//	char                  szStandard[32];            /*�����ʽPAL/NTSC*/
//	VGA_MODE              iResolution;				 /*�ֱ��ʣ�0-VGA_NOT_AVALIABLE��0-10���ζ�Ӧö�ٶ���*/
//	int					  iSubWndDecChan[32];		 /*�ӻ��洰�������ͨ��,0�ŵ�Ԫ�����һ���ӻ����Ӧ�Ľ���ͨ����*/
//}ST_DISPLAY_CHANNEL_INFO;

//��ѯ������Ϣ
typedef struct 
{
	int						iIndex;					 /*��ѯ����ţ�ȡֵ��Χ 1��64*/
	int						iPollTime;				 /*ͣ��ʱ��*/
	int 				    iEnable;				 /*1-��ѯ����Ч��0-��Ч*/
	ST_DECODE_STREAM_INFO   stDecodeStreamInfo;		 /*������ȡ����Ϣ*/
}ST_POLL_INFO;

//��������������Ϣ
typedef struct  
{
	int			  iDecChans;                 /*����ͨ������*/
	int			  iDSPs;				     /*DSP����*/
	int			  iBNCNums;				     /*BNC�������*/
	int			  iVGANums;			         /*VGA�������*/
	char		  szBNCScreenSplits[32];     /*BNC���֧�ֵĻ���ָ�ģʽ*/
	char		  bySupportResolution[32];   /*֧�ֵ�VGA�ֱ���ˢ��Ƶ�ʣ����������ö�ٶ���,һ���ֽڴ���һ���ֱ����Ƿ�֧�֣�1��֧�֣�0����֧��*/
	char          byScreenSurport[8][32];    /*VGA���֧�ֵĻ���ָ�ģʽ*/  
}ST_DECODER_ABILITY;

//ͼ������ṹ
typedef struct  
{
	unsigned char byBrightness;		/*����,0-255*/
	unsigned char byContrast;		/*�Աȶ�,0-255*/ 
	unsigned char bySaturation;		/*���Ͷ�,0-255 */ 
	unsigned char byHue;			/*ɫ��,0-255*/
}ST_IMAGE_PARAM;


//����ʱ��ṹ
typedef struct  
{
	int		iYear;
	int		iMonth;
	int		iDay;
	char	chHours;
	char	chMinutes;
	char	chSeconds;
	char	chReserved;
}ST_DATETIME;


//��־��Ϣ
typedef struct 
{
	ST_DATETIME stLogTime;
	int			iMajorType;		//������ 1-����; 2-�쳣; 3-����; 0xffff-ȫ��
	int			iMinorType;		//������ 0xffff-ȫ��;
	char		szPanelUser[64];//�������û���
	char		szNetUser[64];	//�����û���
	char		szRemoteHostAddr[64];//Զ������IP 
	int			iParaType;		//��������
	int			iChannel;		//ͨ����
	int			iDiskNumber;	//Ӳ�̺�
	int			iAlarmInPort;	//��������˿�
	int			iAlarmOutPort;	//��������˿�
	int			iInfoLen;
	char		szInfo[512];	//��־��Ϣ
}ST_DVR_LOG, *LPST_DVR_LOG;

//ע��������ϵ��豸��Ϣ
typedef struct  
{
	char				szDeviceIP[32];				//�豸����IP
	unsigned short int	nDevicePort;				//�豸����ͨ�Ŷ˿�
	unsigned short int	nDeviceOnlie;				//�豸����״̬: 0--������;1--����
	char				szDeviceLocalIP[32];		//�豸����IP
	unsigned short int	nDeviceLocalPort;			//�豸����ͨ�Ŷ˿�
	unsigned short int	nNetType;					//�豸������������:0--����;1--����
	char				szFirmWareVersion[128];		//�豸�̼��汾��
	char				szProtocalVersion[128];		//�豸Э��汾��
	time_t				tOnlineTime;				//������ʱ��
}ST_DEVICEINFO_ONSERVER;

//ע�������״̬
typedef struct  
{
	char				szServerIP[32];				//������IP
	unsigned short int	nServerPort;				//������ͨ�Ŷ˿�
	unsigned short int	nOnlineDeviceNum;			//�����豸��Ŀ
	char				szServerVersion[128];		//�������汾��
	time_t				tRunningTime;				//����������ʱ��
	unsigned int		nCpuUsage;					//������CPUʹ����
	unsigned int		nTotalPhys;					//�������ܵ������ڴ�
	unsigned int		nAvailPhys;					//���������������ڴ�
	unsigned short int	nLicence;					//�������Ƿ�ʹ�ü��ܹ����: 0--δʹ��;1--ʹ��
	unsigned short int	nLicenceType;				//������ʹ�ü��ܹ���ɷ�ʽ: 0--�豸����;1--ͨ������
	unsigned int		nLicenceLimit;				//����������ע����Ŀ(�豸����ͨ��)
}ST_SERVERSTATUS;

//��ʷ�ļ�������Ϣ
typedef struct 
{
	char		sFileName[100];	//�ļ���
	ST_DATETIME struStartTime;	//�ļ��Ŀ�ʼʱ��
	ST_DATETIME struStopTime;	//�ļ��Ľ���ʱ��
	int			iFileSize;		//�ļ��Ĵ�С
	int         iFileType;      //�ļ�����  
}ST_FINDFILE;

//ͼƬ�ļ�������Ϣ
typedef struct 
{
    char		sFileName[100];	//�ļ���
    ST_DATETIME struStartTime;	//�ļ��Ŀ�ʼʱ��
    int			iFileSize;		//�ļ��Ĵ�С
    int         iFileType;      //�ļ�����  
}ST_FINDPICFILE;

//�豸����ʱ��ftp��Ϣ
typedef struct 
{
	char				szFTPIP[32];				//ftp������ip
	unsigned short int	nFTPPort;					//ftp�������˿�
	unsigned short int	nReserved;					//����
	char				szUserName[32];				//ftp��¼�û���
	char				szPasswd[32];				//ftp��¼����
	char				szUpdateFile[128];			//�����ļ���
}ST_UPDATEFTP_INFO;

//live stream��Ϣ
typedef struct  
{
	unsigned int iStreamHdrLen;			//��ͷ����
	unsigned char chStreamHdr[256];		//��ͷ����
}ST_LIVESTREAM_SDPINFO;


//�Ự��Ϣ
typedef struct  
{
	unsigned int nRecvedRTPCount;	//���յ�����rtp����Ŀ
	int			 iLostRTPCount;		//��ʧ��rtp����Ŀ
}ST_PPV_SESSION_INFO;

#ifdef __cplusplus
extern "C" {
#endif


	/*************************************************
	Function:		pNotifyCallBack
	Description:	֪ͨ�ص�������������
	Input:			iNotifySubsribeID:		����֪ͨ��Ϣ�ķ��ؾ��
					pNotifyBuffer:			֪ͨ��Ϣ����
					dwNotifySize:			֪ͨ��Ϣ����
					nUserDataForCallBack:	�û�����
					nReserved:				����
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pNotifyCallBack)( int iNotifySubsribeID, const void* pNotifyBuffer, size_t dwNotifySize, unsigned int nUserDataForCallBack, unsigned int nReserved );

	/*************************************************
	Function:		pAVDataCallBack
	Description:	�����ݻص�������������
	Input:			
	iStreamID:	���ֲ�ͬ����Ƶ����ID
	iDataType:	����������:��ͷ���ݻ�����ͨ����
	pAVData:	ָ�������ݻ���
	nAVDataLen:	�������ֽ���
	nUsrData:	�û�����
	pNALU:		NALU����
	nNALU:		NALU���ݳ���
	pRawRTP:	����rtpͷ����ԭʼrtp����,����ΪNULL
	nRawRTP:	ԭʼrtp���ݵĳ���,����Ϊ0
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pAVDataCallBack)( int iStreamID,  int iDataType, const void* pAVData, 
		size_t nAVDataLen, unsigned int nUsrData, const void* pNALU, size_t nNALU, const void* pRawRTP, size_t nRawRTP );

	/*************************************************
	Function:		pFileDownloadDataCallBack
	Description:	�����ݻص�������������
	Input:			
	iDownloadID:	���ֲ�ͬ����Ƶ����ID
	pFileData:		�û�����
	nFileDataLen:	����������:��ͷ���ݻ�����ͨ����
	nUsrData:		�û�����
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pFileDownloadDataCallBack)( int iDownloadID,  const void* pFileData, size_t nFileDataLen, unsigned int nUsrData );

	/*************************************************
	Function:		pProgressNotifyCallBack
	Description:	����ĳЩ����ʱ�Ĵ�����̻ص�
	Input:			nUserData:	�û�����
					nReserved:	����
					pDesc:		��ǰ��������
	Output:
	return:
	*************************************************/
	typedef void (__stdcall *pProgressNotifyCallBack)( unsigned int nUserData, unsigned int nReserved, const char* pDesc );

   	/*************************************************
	Function:		pVoiceDataCallBack
    Description:	�������ݻص�������������
    Input:			
    iVoiceID:	    ���ֲ�ͬ��������ID
    pVoiceData:		ָ���������ݻ���
    nVoiceDataLen:	���������ֽ���
    nUsrData:		�û�����
    Output:
    return:
	*************************************************/
    typedef void (__stdcall *pVoiceDataCallBack)( int iVoiceID,  const void* pVoiceData, size_t nVoiceDataLen, int nFrameNumber, unsigned int nUsrData);


	/*************************************************
	Function:		PPVInitLib
	Description:	��ʼ����
	Input:			
	Output:
	return:			0: �ɹ�;
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVInitLib( );

	/*************************************************
	Function:		PPVInitLibEx
	Description:	��ʼ����
	Input:			
	nStartPort1:  UDPģʽ�¼����豸��ʼ�˿�   
	nEndPort1:	  UDPģʽ�¼����豸�����˿�
	nStartPort2:  TCPģʽ�¼����豸��ʼ�˿�
	nEndPort2:	  TCPģʽ�¼����豸�����˿�	   
	Output:
	return:			0: �ɹ�;
	-1:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVInitLibEx( unsigned short int nStartPort1, unsigned short int nEndPort1,unsigned short int nStartPort2, unsigned short int nEndPort2 );

	/*************************************************
	Function:		PPVFiniLib
	Description:	���ٿ�
	Input:			
	Output:
	return:			0: �ɹ�;
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVFiniLib( void );

	
	/*************************************************
	Function:		pszLocalIP
	Description:	���ñ���Ҫ�󶨵�IP
	Input:			pszIP:	Ҫ���õ�IP
	Output:			NULL
	return:			>=0: �ɹ�,�������õ�IP��ʶ
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetBindedIP( const char* pszIP );
	

	/*************************************************
	Function:		PPVUnSetBindedIP
	Description:	ȡ������Ҫ�󶨵�IP
	Input:			iBindID:PPVSetBindedIP����ֵ
	Output:			NULL
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVUnSetBindedIP( int iBindID );

	/*************************************************
	Function:		PPVConnectDevice
	Description:	�����豸
	Input:			pszDeviceID:	Ҫ���ӵ��豸ID
					stPassbyServer:	�豸ע��ķ�����
					nUserData:		�û�����,�ص�ʱ�����ص�����
	Output:			pConnMode:		ָ�����������͵ĵ�ַ,�������غ�õ�ַ��ŵ�����������
	return:			>=0: ��Ϊ��ʶ�������豸��ID
					<0:  ����ʧ��,��ͬ��ֵ����ͬʧ��ԭ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVConnectDevice( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nUserData, EN_CONN_MODE* pConnMode );

	/*************************************************
	Function:		PPVConnectDeviceByACS
	Description:	ͨ����������ת��ʽ�����豸,���ǵ�Ե�
	Input:
	pszDeviceID:	Ҫ���ӵ��豸ID
	stPassbyServer:	�豸ע��ķ�����
	nUserData:		�û�����,�ص�ʱ�����ص�����
	Output:
	pConnMode:		ָ�����������͵ĵ�ַ,�������غ�õ�ַ��ŵ�����������
	return:
	>=0: ��Ϊ��ʶ�������豸��ID
	<0:����ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVConnectDeviceByACS( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nUserDataForCallBack, EN_CONN_MODE* pConnMode );


	/*************************************************
	Function:		PPVDisConnectDevice
	Description:	�Ͽ����豸������
	Input:			iDevice: PPVConnectDevice�ķ���ֵ
	Output:			��
	return:			��
	*************************************************/
	LIBPPVCLIENT_API void	PPVDisConnectDevice( int iDevice );


	/*************************************************
	Function:		PPVGetDeviceInfoOnServer
	Description:	�ӷ������ϻ�ȡ�豸״̬
	Input:			pszDeviceID:	�豸���к�
					stPassbyServer:	�豸ע��ķ�����					
	Output:			pSTDeviceStatus:�豸��Ϣ
	return:			0: ��ȡ�ɹ�;
					<0:��ȡʧ��;
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceInfoOnServer( const char* pszDeviceID, ST_ACCESS_SERVER_INFO stPassbyServer, ST_DEVICEINFO_ONSERVER* pSTDeviceStatus );


	/*************************************************
	Function:		PPVRebootDevice
	Description:	���������豸����
	Input:			iDevice: PPVConnectDevice�ķ���ֵ
	Output:			��
	return:			0: ������������ɹ�;
					<0:������������ʧ��;
	*************************************************/
	LIBPPVCLIENT_API int	PPVRebootDevice( int iDevice );


	/*************************************************
	Function:		PPVGetDeviceWorkStatus
	Description:	��ȡ�豸״̬
	Input:			iDevice:			�豸��ʶ					
	Output:			pszDeviceStatusXML:	�豸״̬��XML�ı�	
					nBufferSize:		ָʾ��ȡǰ�������Ĵ�С���Լ���ȡ��XML�ı��ĳ���		
	return:			0: ��ȡ�ɹ�;
					<0:��ȡʧ��;
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceWorkStatus( int iDevice, char* pszDeviceStatusXML, size_t* nBufferSize );


	/*************************************************
	Function:		PPVGetServerStatus
	Description:	��ȡ������״̬
	Input:			stPassbyServer:	������������Ϣ 
	Output:			pSTServerStatus:������״̬
	return:			0:��ȡ�ɹ�;
					<0:��ȡʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetServerInfo( ST_ACCESS_SERVER_INFO stPassbyServer, ST_SERVERSTATUS* pSTServerStatus );


	/*************************************************
	Function:		PPVRealPlayStart
	Description:	��ʼ��Ƶ���
	Input:			iDevice:	�豸��ʶ
					iChan:		ͨ����,��1��ʼ
					iChanType:  ͨ������:0-��ͨ��; 1-��ͨ��
					pfnCallBack: ��Ƶ���ݻص�
					nUsrData:	�û�����
	Output:
	return:			>=0: ����ʶ
					<0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int	PPVRealPlayStart(  int iDevice, int iChan, int iChanType, 
		pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData, 
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVRealPlayStop
	Description:	ֹͣ��Ƶ���
	Input:			iStreamID: PPVRealPlayStart�ķ���ֵ
	Output:
	return:
	*************************************************/
	LIBPPVCLIENT_API void	PPVRealPlayStop( int iStreamID );


	/*************************************************
	Function:		PPVGetImageParam
	Description:	��ȡͼ�����
	Input:			iDevice:		�豸��ʶ
					iChan:			ͨ����					
	Output:			pstImageParam:	ͼ������ṹ��ָ��
	return:			0:	�ɹ�
					<0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetImageParam( int iDevice, int iChan, ST_IMAGE_PARAM* pstImageParam );


	/*************************************************
	Function:		PPVSetImageParam
	Description:	����ͼ�����
	Input:			iDevice:		�豸��ʶ
					iChan:			ͨ����
					stImageParam:	ͼ������ṹ��
	Output:
	return:			0:	�ɹ�
					<0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetImageParam( int iDevice, int iChan, ST_IMAGE_PARAM stImageParam );


	/*************************************************
	Function:		PPVPTZControl
	Description:	��̨����
	Input:			iDevice:		�豸��ʶ
	iChan:			ͨ����,			��1��ʼ
	iPTZCommand:	��̨��������,	�μ� EN_PTZ_COMMAND
	iAction:		����:			0-��ʼ; 1-ֹͣ
	iSpeed:			��̨���Ƶ��ٶȣ��û�����ͬ���������ٶȿ���ֵ����.ȡֵΪ1��7
	Output:
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVPTZControl( int iDevice, int iChan, int iPTZCommand, int iAction, int iSpeed );


	/*************************************************
	Function:		PPVPTZPresetControl
	Description:	���á������ת��Ԥ�õ�
	Input:			iDevice:			�豸��ʶ
					iChan:				ͨ����,��1��ʼ
					iPTZPresetCommand:	�μ� EN_PTZ_COMMAND
					iParam:				Ԥ�õ��,���255��,��1��ʼ
	Output:			��
	return:			0:	�ɹ�
					<0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVPTZPresetControl( int iDevice, int iChan, int iPTZPresetCommand, int iParam );


	/*************************************************
	Function:		PPVVodStartByTime
	Description:	��ʼ��ʱ��
	Input:			iDevice:	�豸��ʶ
					iChan:		ͨ����,��1��ʼ
					stStart:	��ʼ����ʱ��
					stStop:     ��������ʱ��
					pfnAVDataCallBack:	��Ƶ���ݻص�
					nUsrData:		�û�����
	Output:
	return:			>=0: ����ʶ
					<0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int	PPVVodStartByTime(  int iDevice, int iChan, ST_DATETIME stStart, ST_DATETIME stStop, pAVDataCallBack pfnAVDataCallBack, unsigned int nUsrData );


	/*************************************************
	Function:		PPVFindFile
	Description:	���ҷ������ļ�
	Input:			iDevice:		�豸��ʶ
	iChan:			ͨ����,��1��ʼ
					dwFileType		�ļ����� 
									0xff-ȫ��   0-��ʱ¼�� 
									1-�ƶ�����	2-�������� 
									3-����|���� 4-����&����
									5-����� 	6-�ֶ�¼��
					stStart:		��ʼ����ʱ��
					stStop:			��������ʱ��
					pArray:			�ļ��б�ṹ��ָ�� 
					iFileNum:		�������ļ����� 
	Output:			pArray:			�ļ��ṹ�������׵�ַ
					iFileNum:		ʵ�ʲ��ҵ��ļ�����
	return:			0:  �ɹ�
					<0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVFindFile( int iDevice, int iChan, int iFileType, ST_DATETIME stStart, ST_DATETIME stStop, ST_FINDFILE* pArray, int *iFileNum);


	/*************************************************
	Function:		PPVStartDownloadThroughACS
	Description:	��ʼͨ�������������豸�ϵ��ļ�
	Input:			
	pszDeviceID:	�豸��ʶ
	iChan:			ͨ����,��1��ʼ
	pszFileName:	�豸�ϵ��ļ���
	nFileLen:		�ļ�����
	stPassbyServer:	�豸ע��ķ�����
	pDownCallBack:	����ʱ���ݻص�
	nDownCallBackUsrData:���ػص��������û�����
	return:			
		>=0:  �ɹ�,����ֵΪ������������ʶ
		<0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVStartDownloadThroughACS( const char* pszDeviceID, int iChan, const char* pszFileName, size_t nFileLen, SEEK_TYPE enumSeekType, unsigned int uSeek, ST_ACCESS_SERVER_INFO stPassbyServer, pFileDownloadDataCallBack pDownCallBack, unsigned int nDownCallBackUsrData );


	/*************************************************
	Function:		PPVStopDownloadThroughACS
	Description:	ֹͣͨ�������������豸�ϵ��ļ�
	Input:			
	iDownloadID:	������������ʶ
	return:			
	*************************************************/
	LIBPPVCLIENT_API void	PPVStopDownloadThroughACS(  int iDownloadID );

	/*************************************************
	Function:		PPVGetDownloadPosThroughACS
	Description:	��ȡ���ؽ���
	Input:		
		iDownloadID:������������ʶ
	return:			
		����ָ�����ر�ʶ�����ؽ���
	*************************************************/
	LIBPPVCLIENT_API long PPVGetDownloadPosThroughACS(  int iDownloadID );

	/*************************************************
	Function:		PPVPlayBackByFileNameStart
	Description:	��ʼ��ʷ��Ƶ���(TCPģʽ),�����ļ�����
	Input:			
		iDevice:				�豸��ʶ
		iChannel:				Ƶ�����
		pszFileName:			�ļ�����
		nFileLen:				�ļ�����
		stPassbyServer,			�豸ע��ķ�����PPVSServer
		pfnAVDataCallBack:		��Ƶ���ݻص�
		nAVDataCBUsrData:		�û�����
		pfnProgressNotifyUser��	֪ͨ�ص�����
		nProgressCBUsrData��	֪ͨ�û�����
	Output:
	return:			
					>=0:	����ʶ
					<0:		ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int	PPVPlayBackByFileNameStart(  int iDevice, int iChannel, const char *pszFileName, size_t nFileLen, ST_ACCESS_SERVER_INFO stPassbyServer, pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData,
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVPlayBackStop
	Description:	ֹͣ��ʷ��Ƶ���(TCPģʽ)
	Input:			
		iVodStream: PPVPlayBackStartByFileName�ķ���ֵ
	Output:
	return:
	*************************************************/
	LIBPPVCLIENT_API void	PPVPlayBackStop( int iVodStream );


	/*************************************************
	Function:		 PPVPlayBackControl
	Description:	 ��ʷ��Ƶ�طŽ��ȿ���(TCPģʽ)
	Input:			
		iVodStream:  PPVVodStart�ķ���ֵ
		iControlCmd: �طſ�������, �μ�EN_VOD_CONTROL_CMD
		iParamIn:	 ����������صĲ���	
	Output:			
		iParamOut:	 ���Ʒ��ؽ��
	return:			
					 0:	�ɹ�
					 <0: ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVPlayBackControl( int iVodStream, int iControlCmd, int iParamIn, int* iParamOut );


	/*************************************************
	Function:		PPVFindDeviceLog
	Description:	���ҷ������ļ�
	Input:			iDevice:		�豸��ʶ
					iSelectMode:	�����������ɴ���0
					iMajorType:		������,�μ��궨��,0xffff������������
					iMinorType:		������,�μ��궨��,0xffff������������
					stStart:		��ʼ����ʱ��
					stStop:			��������ʱ��
					pArray:			��־�б�ṹ��ָ�� 
					iFileNum:		��������־���� 
	Output:			pArray:			��־�ṹ�������׵�ַ
					iFileNum:		ʵ�ʲ��ҵ���־����
	return:			0:  �ɹ�
					<0: ʧ��
	*************************************************/
LIBPPVCLIENT_API int	PPVFindDeviceLog( int iDevice, int iSelectMode, int iMajorType, int iMinorType, ST_DATETIME stStart, ST_DATETIME stStop, ST_DVR_LOG* pArray, int *iItemNum );
	

	/*************************************************
	Function:		PPVSendUpdateInfo
	Description:	����������Ϣ���豸
	Input:			iDevice:		�豸��ʶ
					stUpdateInfo:	������Ϣ
	Output:			
	return:			0: �豸�յ�����������;
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVSendUpdateInfo( int iDevice, ST_UPDATEFTP_INFO stUpdateInfo );


	/*************************************************
	Function:		PPVGetDeviceConfig
	Description:	��ȡ�豸������Ϣ
	Input:			iDevice:		�豸��ʶ
					pszConfigCMD:	�����ַ���
					iCmdParam1:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam2:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam3:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam4:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
	Output:			pszConfigXML:	����������Ϣ��xml�ı�
					nBufferSize:	ָʾ��ȡǰ�������Ĵ�С���Լ���ȡ��xml�ı��ĳ���	
	return:			0: �ɹ�
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVGetDeviceConfig( int iDevice, const char* pszConfigCmd, int iCmdParam1, 
		int iCmdParam2, int iCmdParam3,  int iCmdParam4,  char* pszConfigXML, size_t* nBufferSize );


	/*************************************************
	Function:		PPVSetDeviceConfig
	Description:	�����豸������Ϣ
	Input:			iDevice:		�豸��ʶ
					pszConfigCMD:	�����ַ���
					iCmdParam1:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam2:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam3:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					iCmdParam4:	���������������ͨ���ţ��쳣���ͣ���������ͨ����
					pszConfigXML:	����������Ϣ��xml�ı�
	Output:
	return:			0: �ɹ�
					<0:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int	PPVSetDeviceConfig( int iDevice, const char* pszConfigCmd, int iCmdParam1, 
		int iCmdParam2, int iCmdParam3,  int iCmdParam4, const char* pszConfigXML );


	/*************************************************
	Function:					PPVRealPlayByTcpStart
	Description:				ͨ��TCPģʽ��ʼ����ʵʱ��Ƶ���ݽ���
	Input:			
		iDevice:				�豸��ʶ
		iChan:					ͨ����,��1��ʼ
		iChanType:				ͨ������:0-��ͨ��; 1-��ͨ��
		pfnCallBack:			��Ƶ���ݻص�
		nUsrData:				�û�����
		pfnProgressNotifyUser:	�û�֪ͨ�ص�
		nProgressCBUsrData:		�û�֪ͨ����
	Output:
	return:			
		>=0:		����ʶ
		<0:			ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int	PPVRealPlayByTcpStart(  int iDevice, int iChan, int iChanType, 
		pAVDataCallBack pfnAVDataCallBack, unsigned int nAVDataCBUsrData, 
		pProgressNotifyCallBack pfnProgressNotifyUser, unsigned int nProgressCBUsrData );


	/*************************************************
	Function:		PPVRealPlayByTcpStop
	Description:	ͨ��TCPģʽֹͣ����ʵʱ��Ƶ���ݽ���
	Input:			
		iStreamID:	����ʶ
	Output:
	return:			
		//>=0:		����ʶ
		//<0:			ֹͣʧ�� 
	*************************************************/
	LIBPPVCLIENT_API void PPVRealPlayByTcpStop( int iStreamID );

	/*************************************************
	Function:		PPVGetLiveStreamSDP
	Description:	��ȡ����Ϣ,��Ҫ��Ϊ�˻�ȡ��ͷ
	Input:			
					pszDeviceID:		�豸���к�
					iChan:				�豸ͨ����
					iStreamType:		0:������,1:������
					stPassbyServer:		�豸ע��ķ�����	
	Output:			pSTStreamSDPInfo:	�豸��ͷ��Ϣ
	return:			
					0: ��ȡ�ɹ�;
					<0:��ȡʧ��; 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetLiveStreamSDP(  int iDevice, int iChan, int iStreamType,  ST_LIVESTREAM_SDPINFO* pSTStreamSDPInfo );
	
	/*************************************************
	Function:			PPVSetAlarmGuardsStatus
	Description:		һ����/����
	Input:			
		iChangeCount:	�ı��ͨ������
		pChannel;		ͨ�����ָ������
	Output:
	return:			
		>0:				�ɹ�
		<=0:			ʧ��
	*************************************************/
	LIBPPVCLIENT_API int PPVSetAlarmGuardsStatus(  int iDevice, const char* pszAlarmGuardsInfo );

	/*************************************************
	Function:		PPVGetSessionInfo
	Description:	��ȡ�Ự��Ϣ
	Input:			
	iStream:		�ỰID
	pSessionInfo:	�Ự��Ϣ�ṹ��ָ��
	Output:
	return:		
	0:�ɹ� 
	-1:ʧ��
	*************************************************/
	LIBPPVCLIENT_API int PPVGetSessionInfo( int iStream, ST_PPV_SESSION_INFO* pSessionInfo);

    /*************************************************
    Function:		PPVVoiceTalkStart
    Description:	��ʼ�����Խ���㲥
    Input:
                    pszDeviceID:	        �豸ID
                    nVoiceChan:             ����ͨ��
                    vtype:                  0-�Խ�; 1-�㲥
                    stPassbyServer:	        �豸ע��ķ�����
                    pfnVoiceDataCallBack:   ��Ƶ���ݻص�
                    nVoiceDataCBUsrData:	�û�����
    Output:
    return:			>=0: ������ʶ
                    <0: ʧ��  -1--��������; -2--�豸δ׼����; -2--�豸����ͨ������ʹ��
    *************************************************/
    LIBPPVCLIENT_API int PPVVoiceTalkStart( const char* pszDeviceID, int nVoiceChan, int vtype, ST_ACCESS_SERVER_INFO stPassbyServer,
        pVoiceDataCallBack pfnVoiceDataCallBack, unsigned int nVoiceDataCBUsrData);

    /*************************************************
    Function:		PPVVoiceTalkStop
    Description:	ֹͣ�����Խ�
    Input:			iVoiceID: PPVVoiceTalkStart�ķ���ֵ
    Output:
    return:
    *************************************************/
    LIBPPVCLIENT_API void PPVVoiceTalkStop( int iVoiceID );

    /*************************************************
    Function:		PPVVoiceTalkInputData
    Description:	������������
    Input:			
                    iVoiceID: PPVVoiceTalkStart�ķ���ֵ
    Output:
    return:
    *************************************************/
    LIBPPVCLIENT_API void PPVVoiceTalkInputData( int iVoiceID,  const void* pVoiceData, size_t nVoiceDataLen, int nFrameNumber);

    /*************************************************
    Function:		PPVTransparentChannel
    Description:	͸��ͨ��
    Input:
                    pszDeviceID:	        �豸ID
                    nTransChan:             ͨ��:1-232; 2-485
                    nRS485Chan:             485ͨ���ţ���1��ʼ
                    stPassbyServer:	        �豸ע��ķ�����
                    pData:                  ָ�����ݻ���
                    nDataLen:               ���ݳ���
    Output:
    return:			>=0: �ɹ�
                    <0: ʧ��
    *************************************************/
    LIBPPVCLIENT_API int PPVTransparentChannel( const char* pszDeviceID, int nTransChan, int nRS485Chan, ST_ACCESS_SERVER_INFO stPassbyServer,
        const void* pData, size_t nDataLen);

    /*************************************************
    Function:		PPVFindPictureFile
    Description:	���ҷ�����ͼƬ�ļ�
    Input:			iDevice:		�豸��ʶ
                    iChan:			ͨ����,��1��ʼ
                    dwFileType		�ļ�����,��ʱ����
                                    0xff-ȫ��   0-��ʱ¼�� 
                                    1-�ƶ�����	2-�������� 
                                    3-����|���� 4-����&����
                                    5-����� 	6-�ֶ�¼��
                    stStart:		��ʼ����ʱ��
                    stStop:			��������ʱ��
                    pArray:			�ļ��б�ṹ��ָ�� 
                    iFileNum:		�������ļ����� 
    Output:			pArray:			�ļ��ṹ�������׵�ַ
                    iFileNum:		ʵ�ʲ��ҵ��ļ�����
    return:			0:  �ɹ�
                    <0: ʧ��
    *************************************************/
    LIBPPVCLIENT_API int PPVFindPictureFile( int iDevice, int iChan, int iFileType, ST_DATETIME stStart, ST_DATETIME stStop, ST_FINDPICFILE* pArray, int *iFileNum);

    /*************************************************
    Function:		PPVNotifyDeviceOnOffLine
    Description:	֪ͨ�豸������
    Input:
                    pszDeviceID:	        �豸ID
                    action:                 1:����, 0:����
                    stPassbyServer:	        �豸ע��ķ�����
    Output:
    return:			>=0: �ɹ�
                    <0: ʧ��
    *************************************************/
    LIBPPVCLIENT_API int PPVNotifyDeviceOnOffLine( const char* pszDeviceID, int action, ST_ACCESS_SERVER_INFO stPassbyServer);
	
	/*************************************************
	Function:		PPVStartDecode
	Description:	��ʼ��̬����
	Input:			iDecDevice:		   �������豸��ʶ
	                iDecChannel:       ����ͨ����
	Output:         
	return:			>=0: �ɹ�
	                 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVStartDecode(  int iDecDevice, int iDecChannel, ST_DECODE_STREAM_INFO stDecodeStreamInfo);
	
	/*************************************************
	Function:		PPVStopDecode
	Description:	��ʼ��̬����
	Input:			iDecDevice:		   �������豸��ʶ
					iDecChannel:       ����ͨ����
	Output:         
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVStopDecode( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVStartPoll
	Description:	��ʼ��ѯ����
	Input:			iDecDevice:		   �������豸��ʶ
	                iDecChannel:       ����ͨ����
	Output:         
	return:			>=0: �ɹ�
	                 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVStartPoll( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVStopPoll
	Description:	ֹͣ��ѯ����
	Input:			iDecDevice:		   �������豸��ʶ
					iDecChannel:       ����ͨ����
	Output:         
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVStopPoll( int iDecDevice, int iDecChannel );

	/*************************************************
	Function:		PPVSetDecoderDisplayChannel
	Description:	���ý�������ʾͨ��
	Input:			iDecDevice:		     �������豸��ʶ
					iDisplaycChannel:    ��ʾͨ����1-8VGA���;9��ʼBNC���
					stDisplayChannelInfo ��������ʾͨ����Ϣ
	Output:         
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	//------------ʡ��,��������HCNetSDK.h�е��ض��� ������-----------------
	//LIBPPVCLIENT_API int PPVSetDecoderDisplayChannel( int  iDecDevice, int iDisplaycChannel,  ST_DISPLAY_CHANNEL_INFO stDisplayChannelInfo);

	/*************************************************
	Function:		PPVGetDecoderDisplayChannel
	Description:	��ȡ��������ʾͨ��
	Input:			iDecDevice:		       �������豸��ʶ
					iDisplaycChannel:      ��ʾͨ����
					stDisplayChannelInfo   ��������ʾͨ����Ϣ

	Output:			stDecoderChannelInfo   ��������ʾͨ����Ϣ
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	//------------ʡ��,��������HCNetSDK.h�е��ض��� ������-----------------
	//LIBPPVCLIENT_API int PPVGetDecoderDisplayChannel(  int iDecDevice, int iDisplaycChannel, ST_DISPLAY_CHANNEL_INFO * stDisplayChannelInfo);

	/*************************************************
	Function:		PPVSetDecoderPoll
	Description:	�趨��ѯ�������
	Input:			iDecDevice:		       �������豸��ʶ
					iDecChannel:           ����ͨ����
				    stPollInfo;            ��ѯ�����
					iNumberOfstPollInfo;   �趨��ѯ�����
	Output:
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVSetDecoderPoll(  int iDecDevice, int iDecChannel, const ST_POLL_INFO *stPollInfo, int iNumberOfstPollInfo);
	/*************************************************
	Function:		PPVGetDecoderPol
	Description:	��ȡ��ѯ�������
	Input:			iDecDevice:		       �������豸��ʶ
					iDecChannel:           ����ͨ����
					iNumberOfstPollInfp    Ŀǰ��������������Ϣ����64

	Output:		    stPollInfo;            ��ѯ�����
					iNumberOfstPollInfo;   ʵ�ʻ�ȡ��ѯ����Ϣ����
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecoderPoll(  int iDecDevice, int iDecChannel, ST_POLL_INFO *stPollInfo, int* iNumberOfstPollInfo );

	/*************************************************
	Function:		PPVGetDecChanStatus 
	Description:	��ȡ����ͨ��״̬
	Input:			iDecDevice:		       �������豸��ʶ
					iDecChannel:           ����ͨ����
  
	Output:		    iType;                 ����ͨ��״̬,iType="0/1/2",0:δ���룻1:��̬���룻2:��ѯ����
					stDecodeStreamInfo     ������ȡ����Ϣ
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecChanStatus(  int iDecDevice, int iDecChannel, int* iType, ST_DECODE_STREAM_INFO* stDecodeStreamInfo);

	/*************************************************
	Function:		PPVGetDecAbility
	Description:	��ȡ������������
	Input:			iDecDevice:		   �������豸��ʶ

	Output:		    ST_DECODER_ABILITY;����ͨ��״̬
	return:			>=0: �ɹ�
					 <0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVGetDecAbility(  int iDecDevice, ST_DECODER_ABILITY* stDecoderAbility); 

	/*************************************************
	Function:		PPVCapturePicture
	Description:	��ʼץ��
	Input:			iDecDevice:		      �豸��ʶ
					iChan:				  �豸ͨ����			  
					stCapturePictureInfo  ץ����Ϣ
	Output:         stCapturePictureInfo  ץ����Ϣ
	return:			>=0: �ɹ�
	<0: ʧ�� 
	*************************************************/
	LIBPPVCLIENT_API int PPVCapturePicture(  int iDecDevice, int iChan, ST_CAPTUREPICTURE_INFO * stCapturePictureInfo);

	/*************************************************
	Function:		NotifySubscribe
	Description:	֪ͨ��Ϣ����
	Input:			stPassbyServer:					ע�����������Ϣ
					pfnNotifyCallBack:				����Notify�Ļص�����
					nUserDataForCallBack;			�û�����
	Output:			
	return:			>=0 ���ĳɹ����صľ��
					-1  ʧ��
	remark: 
	�豸��������Ϣ:
	<Notify Type="DeviceInfo">
	<DeviceID>test0123</DeviceID>
	<!--�豸����:on;�豸����:off-->
	<OnOffLine>On/Off</OnOffLine>
	</Notify>

	*************************************************/
	LIBPPVCLIENT_API int PPVNotifySubscribe( ST_ACCESS_SERVER_INFO stPassbyServer, pNotifyCallBack pfnNotifyCallBack, unsigned int nUserDataForCallBack );

	/*************************************************
	Function:		NotifyUnSubscribe
	Description:	ȡ��֪ͨ����(����ȫ�����ġ����ֶ���)
	Input:			
					iNotifySubsribeID:	����֪ͨ��Ϣ�ķ��ؾ��
	Output:
	return:			
	*************************************************/
	LIBPPVCLIENT_API void PPVNotifyUnSubscribe( int iNotifySubsribeID );


#ifdef __cplusplus
}
#endif

#endif