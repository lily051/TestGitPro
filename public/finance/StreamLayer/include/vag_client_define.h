/**	@file vag_client_define.h
*   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*   @brief �豸�������ؿͻ��˽ṹ�ͺ궨�� 
*
*	@author liyingtao@hikvision.com
*	@date 2013��01��15��
*
*	@note ��ʷ��¼��V1.0.0
*	@note 
*
*	@warning
*/

#ifndef _VAG_CLIENT_DEFINE_H_
#define _VAG_CLIENT_DEFINE_H_

/********************�ӿڵ��÷���ֵ*********************/
#define VAG_SUCCESS									0		// ���ýӿ���ȷ
#define VAG_FAIL									-1		// ���ýӿ�ʧ��

/********************��Դ����*********************/
#define VAG_GROUP									0x01	// ����
#define VAG_DEVICE									0x02	// ������
#define VAG_CAMERA									0x03	// ����ͷ
#define VAG_CAMERA_DEVICE							0x04	// ����ͷ���豸��Ϣ

#define VAG_IPV4_LEN								16		// IPV4��ַ����
#define VAG_USER_NAME_LEN							64		// �û�������
#define VAG_PASSWORD_LEN							64		// ���볤��
#define VAG_INDEXCODE_LEN							128		// �����ų���
#define VAG_USERAGENT_LEN							128		// �û�������
#define VAG_NAME_LEN								128		// ���Ƴ���
#define VAG_PTZ_POIT_NUM							256		// ��̨���Ѳ��Ԥ�õ���

#define VAG_GPS_PUB_URL_LEN						    256		// �¼�����url����
#define VAG_EVENT_PUB_URL_LEN						256		// �¼�����url����
#define VAG_STREAM_URL_LEN							256		// ȡ��url����
#define VAG_MAX_DAYS								7		// һ���������
#define VAG_PICSERVER_URL_LEN						256		// ͼƬ������url����
#define VAG_URL_EXT_LEN				    	    	128		// ��չURL����

#define VAG_CHAN_OSD_MAX_LEN						128		// OSD�ַ�����
#define VAG_CHAN_OSD_MAX_NUM						16		// OSD�ַ�������

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif

#ifdef __linux__
typedef void* HWND;
#endif

/********************VAG_CLIENT�����붨�� begin*********************/
#define VAG_ERR_NOERROR								0		// �޴���

//VAG_CLIENT��ش�����
#define VAG_Client_EBase							1000						//< �����������  
#define VAG_ERR_VAG_CLIENT_NOT_INIT					(VAG_Client_EBase + 0)		// VAG_CLIENTδ��ʼ��
#define VAG_ERR_HPR_INIT_FAIL						(VAG_Client_EBase + 1)		// Hpr��ʼ��ʧ��
#define VAG_ERR_VAG_MALLOC_FAIL						(VAG_Client_EBase + 2)		// �����ڴ�ʧ��
#define VAG_ERR_PARA_ERROR							(VAG_Client_EBase + 3)		// ��������
#define VAG_ERR_PTR_PARA_NULL						(VAG_Client_EBase + 4)		// ����ָ�����Ϊ��
#define VAG_ERR_COMMUNICATE_FAIL					(VAG_Client_EBase + 5)		// ���豸�������ؽ���ʧ��
#define VAG_ERR_RSP_PARSE_FAIL						(VAG_Client_EBase + 6)		// �豸�������ط��ص���Ӧ���Ĵ���
#define VAG_ERR_RETURN_RSP_FAIL						(VAG_Client_EBase + 7)		// �豸�������ط�����Ӧʧ��
#define VAG_ERR_CLENT_HANDLE_NOEXIST				(VAG_Client_EBase + 8)		// ���ؿͻ��˾��������
#define VAG_ERR_ADD_HANDLE_FAIL						(VAG_Client_EBase + 9)		// ���ɿͻ��˾��ʧ��
#define VAG_ERR_NOT_SUPPORT							(VAG_Client_EBase + 10)		// ��֧�ֵĹ���
#define VAG_ERR_RECORD_TYPE_SUPPORT					(VAG_Client_EBase + 11)		// ��ѯ��¼�����Ͳ�֧��
#define VAG_ERR_DEVICE_NOT_SUPPORT					(VAG_Client_EBase + 12)		// ��������豸��֧�ָù���
#define VAG_ERR_RECORD_FILE_QUERYING				(VAG_Client_EBase + 13)		// ���ڲ�ѯ¼���ļ�
#define VAG_ERR_BUFFER_TOO_SMALL					(VAG_Client_EBase + 14)		// ������̫С
#define VAG_ERR_DEV_PLUGUN_NULL						(VAG_Client_EBase + 15)		// �豸���ʵ��Ϊ��

#define VAG_ERR_RESOURCE_SIZE_ZERO					(VAG_Client_EBase + 101)	// ��Դ����Ϊ0
#define VAG_ERR_RESOURCE_END_POS					(VAG_Client_EBase + 102)	// ������Դ�б�β
#define VAG_ERR_RESOURCE_NOEXIST					(VAG_Client_EBase + 103)	// ��ѯ����Դ������
#define VAG_ERR_RESOURCE_NOT_DOWNLOAD				(VAG_Client_EBase + 104)	// δ�����������Դ�б�

#define VAG_ERR_VAG_PTZ_CMD_UNKNOW					(VAG_Client_EBase + 201)	// �޷���������̨���������
#define VAG_ERR_VAG_PTZ_LOWER_PRIORITY				(VAG_Client_EBase + 202)	// ��̨����Ȩ��̫��
#define VAG_ERR_VAG_PTZ_CAMERA_LOCKED				(VAG_Client_EBase + 203)	// ��̨ͨ��������
#define VAG_ERR_VAG_PTZ_CAMERA_NOTEXIST				(VAG_Client_EBase + 204)	// �����ͨ����Ų�����
#define VAG_ERR_VAG_PTZ_DEV_LOGIN_FAIL				(VAG_Client_EBase + 205)	// ��½�豸ʧ��
#define VAG_ERR_VAG_PTZ_DEV_PLUGIN_FAIL				(VAG_Client_EBase + 206)	// �����豸�������ʧ��

#define VAG_ERR_VAG_VTDU_CLIENT_LOAD_FAIL			(VAG_Client_EBase + 301)	// ȡ��SDK����ʧ��
#define VAG_ERR_VAG_VTDU_START_FAIL					(VAG_Client_EBase + 302)	// ȡ��SDKȡ��ʧ��
#define VAG_ERR_VAG_STREAM_CHAN_NOEXIST				(VAG_Client_EBase + 303)	// ȡ��ͨ��������
#define VAG_ERR_DEV_START_STREAM_FAILD				(VAG_Client_EBase + 304)	// ֱ�Ӵ��豸ȡ��ʧ��
#define VAG_ERR_VAG_VTDU_CREATE_SESSION_FAIL		(VAG_Client_EBase + 305)	// ȡ��SDK����sessionʧ��
#define VAG_ERR_VAG_INITIATIVEDEV_STREAM_FAIL		(VAG_Client_EBase + 306)	// �����豸ȡ������ʧ��
#define VAG_ERR_VAG_VTDU_GETVIDEOPARA_FAIL			(VAG_Client_EBase + 307)	// ��ȡԤ����Ƶ����ʧ��
#define VAG_ERR_VAG_VTDU_SETVIDEOPARA_FAIL			(VAG_Client_EBase + 308)	// ����Ԥ����Ƶ����ʧ��
#define VAG_ERR_VAG_VTDU_STREAM_HANDLE_NOEXIST		(VAG_Client_EBase + 309)	// Ԥ�����������
#define VAG_ERR_VAG_DEV_GETVIDEOPARA_FAIL			(VAG_Client_EBase + 310)	// ���豸��ȡԤ����Ƶ����ʧ��
#define VAG_ERR_VAG_DEV_SETVIDEOPARA_FAIL			(VAG_Client_EBase + 311)	// ���豸����Ԥ����Ƶ����ʧ��
#define VAG_ERR_VAG_PLAY_SDK_LOAD_FAIL				(VAG_Client_EBase + 312)	// �����������ʧ��
#define VAG_ERR_VAG_CAPTURE_BUFFER_SMALL			(VAG_Client_EBase + 313)	// ץͼ����Ļ�����̫С
#define VAG_ERR_VAG_PLAYBACK_HANDLE_NOEXIST			(VAG_Client_EBase + 314)	// �ط�ȡ�����������
#define VAG_ERR_ADD_STREAM_HANDLE_FAIL				(VAG_Client_EBase + 315)	// ����ȡ�����ʧ��

#define VAG_ERR_VAG_NOT_ONLINE						(VAG_Client_EBase + 401)	// VAG������

#define VAG_ERR_VAG_TALK_MAX_NUM					(VAG_Client_EBase + 501)	// �ﵽ����豸�Խ���
#define VAG_ERR_VAG_TALK_HANDLE_NOEXIST				(VAG_Client_EBase + 502)	// �豸�Խ����������
#define VAG_ERR_ADD_TALK_HANDLE_FAIL				(VAG_Client_EBase + 503)	// ���ɶԽ����ʧ��

#define VAG_ERR_CFG_CMD_NOT_SUPPORT					(VAG_Client_EBase + 601)	// ������������Ų�֧��

//PAG������ش�����
#define PAG_Srv_EBase								4000						//< �����������
#define PAG_NEW_FAILED						        (PAG_Srv_EBase + 1)			// �ڴ�����ʧ��
#define PAG_INDEXCODE_NOTEXIT						(PAG_Srv_EBase + 2)			// �豸����������
#define PAG_UNKOWN_MSGTYPE							(PAG_Srv_EBase + 3)			// δ֪��Э������
#define PAG_PARSEXML_FAIL							(PAG_Srv_EBase + 4)			// Э�����ʧ��
#define PAG_PLUGIN_LOGINFAIL            	        (PAG_Srv_EBase + 5)			// ��¼�豸ʧ��
#define PAG_DEVCONT_CONNC							(PAG_Srv_EBase + 6)			// �����豸ʧ��
#define PAG_DEVPLUGIN_NOTEXIT						(PAG_Srv_EBase + 7)			// �豸���������
#define PAG_UNSUPPORT_DEVTYPE						(PAG_Srv_EBase + 8)			// �豸���Ͳ�֧��
#define PAG_EVENT_RESUB								(PAG_Srv_EBase + 9)			// �ظ������¼�
#define PAG_EVENT_NOTSUB							(PAG_Srv_EBase + 10)		// �¼�δ����
#define PAG_NOT_ONLINE								(PAG_Srv_EBase + 11)		// �����豸������
#define PAG_PARAIS_NULL								(PAG_Srv_EBase + 12)		// ָ�����Ϊ��
#define PAG_GETCHANNUM_FAIL							(PAG_Srv_EBase + 13)		// �ӱ������ݿ��ȡͨ����ʧ��
#define PAG_GETTALKHANDL_FAIL						(PAG_Srv_EBase + 14)		// ��ȡ�Խ�ʵ�����ʧ��
#define PAG_PICSIZE_TOO_LONG            	        (PAG_Srv_EBase + 15)		// �豸���ص�ͼƬ���ݹ���
#define PAG_PICDATA_POST_FAIL						(PAG_Srv_EBase + 16)		// ͼƬ����Ͷ��ƽ̨SDKʧ��
#define PAG_BUFFER_TOO_SMALL						(PAG_Srv_EBase + 17)		// ����Ļ���ռ�̫С
#define PAG_CHANNUM_ERR								(PAG_Srv_EBase + 18)		// ͨ���Ŵ���
#define PAG_ALARMOUT_CONTROLVALUE_ERR				(PAG_Srv_EBase + 19)		// ���������������ֵ����
#define PAG_CONFIG_CMD_UNSUPPORT					(PAG_Srv_EBase + 20)		// ��֧�ֵ�Զ����������
#define PAG_DEVICE_UNLOGIN							(PAG_Srv_EBase + 21)		// �豸δ��¼�����ߵ�¼���Ϊ��
#define PAG_DEVINFO_FROMDB_FAIL						(PAG_Srv_EBase + 22)		// �����ݿ��ȡ�豸��Ϣʧ�ܣ��豸��Ų�����
#define PAG_CAMINFO_FROMDB_FAIL						(PAG_Srv_EBase + 23)		// �����ݿ��ȡ��ص���Ϣʧ�ܣ���ص��Ų�����
#define PAG_UNKOWN_REGTYPE							(PAG_Srv_EBase + 24)		// δ֪��ע������

#define PAG_PLAN_SEG_TOO_MORE						(PAG_Srv_EBase + 30)		// ¼��ƻ�����ʱ��Ƭ����̫��
#define PAG_QUERY_RECORD_TIMEOUT					(PAG_Srv_EBase + 31)		// ��ѯ¼���ļ���ʱ
#define PAG_QUERY_RECORD_UNSUPORT					(PAG_Srv_EBase + 32)		// ��ѯ¼���ļ����Ͳ�֧��
#define PAG_QUERY_RECORD_HANDLE_ERR     	        (PAG_Srv_EBase + 33)		// ¼���ļ����Ϊ��

#define PAG_PTZ_CMD_UNSUPPORT						(PAG_Srv_EBase + 50)		// ��̨�������֧��

#define PAG_SIP_MESSAGE_BUILD_FAIL					(PAG_Srv_EBase + 60)		// sipЭ����Ϣ����ʧ��
#define PAG_SIP_MESSAGE_SEND_FAIL					(PAG_Srv_EBase + 61)		// sipЭ����Ϣ����ʧ��
#define PAG_SIP_INVITE_BUILD_FAIL					(PAG_Srv_EBase + 62)		// sipЭ��INVITE����ʧ��
#define PAG_SIP_INVITE_SEND_FAIL					(PAG_Srv_EBase + 63)		// sipЭ��INVITE����ʧ��

#define PAG_BI_LIB_FAIL								(PAG_Srv_EBase + 70)		// ����B�ӿڿ�ʧ��

#define PAG_SERVER_INNER_ERROR						(PAG_Srv_EBase + 401)		// vag�����ڲ�����
#define PAG_ERR_DEVICE_NOT_SUPPORT			        (PAG_Srv_EBase + 402)		// ��������豸��֧�ָù���
#define PAG_SRVCONECT_FAIL      			        (PAG_Srv_EBase + 403)		// ���ӷ����ʧ��

//PAG���豸��صĴ�����
#define PAG_Plug_EBase								5000						///< ���������

#define PAG_NOERROR									(PAG_Plug_EBase + 0)		// û�д���
#define PAG_PASSWORD_ERROR 							(PAG_Plug_EBase + 1)		// �û����������
#define PAG_NOENOUGHPRI 							(PAG_Plug_EBase + 2)		// Ȩ�޲���
#define PAG_NOINIT 									(PAG_Plug_EBase + 3)		// û�г�ʼ��
#define PAG_CHANNEL_ERROR 							(PAG_Plug_EBase + 4)		// ͨ���Ŵ���
#define PAG_OVER_MAXLINK 							(PAG_Plug_EBase + 5)		// ���ӵ�DVR�Ŀͻ��˸����������
#define PAG_VERSIONNOMATCH							(PAG_Plug_EBase + 6)		// �汾��ƥ��
#define PAG_NETWORK_FAIL_CONNECT					(PAG_Plug_EBase + 7)		// ���ӷ�����ʧ��
#define PAG_NETWORK_SEND_ERROR						(PAG_Plug_EBase + 8)		// �����������ʧ��
#define PAG_NETWORK_RECV_ERROR						(PAG_Plug_EBase + 9)		// �ӷ�������������ʧ��
#define PAG_NETWORK_RECV_TIMEOUT					(PAG_Plug_EBase + 10)		// �ӷ������������ݳ�ʱ
#define PAG_NETWORK_ERRORDATA						(PAG_Plug_EBase + 11)		// ���͵���������
#define PAG_ORDER_ERROR								(PAG_Plug_EBase + 12)		// ���ô������
#define PAG_OPERNOPERMIT							(PAG_Plug_EBase + 13)		// �޴�Ȩ��
#define PAG_COMMANDTIMEOUT							(PAG_Plug_EBase + 14)		// DVR����ִ�г�ʱ
#define PAG_ERRORSERIALPORT							(PAG_Plug_EBase + 15)		// ���ںŴ���
#define PAG_ERRORALARMPORT							(PAG_Plug_EBase + 16)		// �����˿ڴ���
#define PAG_PARAMETER_ERROR 						(PAG_Plug_EBase + 17)		// ��������
#define PAG_CHAN_EXCEPTION							(PAG_Plug_EBase + 18)		// ������ͨ�����ڴ���״̬
#define PAG_NODISK									(PAG_Plug_EBase + 19)		// û��Ӳ��
#define PAG_ERRORDISKNUM							(PAG_Plug_EBase + 20)		// Ӳ�̺Ŵ���
#define PAG_DISK_FULL								(PAG_Plug_EBase + 21)		// ������Ӳ����
#define PAG_DISK_ERROR								(PAG_Plug_EBase + 22)		// ������Ӳ�̳���
#define PAG_NOSUPPORT								(PAG_Plug_EBase + 23)		// ��������֧��
#define PAG_BUSY									(PAG_Plug_EBase + 24)		// ������æ
#define PAG_MODIFY_FAIL								(PAG_Plug_EBase + 25)		// �������޸Ĳ��ɹ�
#define PAG_PASSWORD_FORMAT_ERROR					(PAG_Plug_EBase + 26)		// ���������ʽ����ȷ
#define PAG_DISK_FORMATING							(PAG_Plug_EBase + 27)		// Ӳ�����ڸ�ʽ��,������������
#define PAG_DVRNORESOURCE							(PAG_Plug_EBase + 28)		// DVR��Դ����
#define	PAG_DVROPRATEFAILED							(PAG_Plug_EBase + 29)		// DVR����ʧ��
#define PAG_OPENHOSTSOUND_FAIL 						(PAG_Plug_EBase + 30)		// ��PC����ʧ��
#define PAG_DVRVOICEOPENED 							(PAG_Plug_EBase + 31)		// �����������Խ���ռ��
#define	PAG_TIMEINPUTERROR							(PAG_Plug_EBase + 32)		// ʱ�����벻��ȷ
#define	PAG_NOSPECFILE								(PAG_Plug_EBase + 33)		// �ط�ʱ������û��ָ�����ļ�
#define PAG_CREATEFILE_ERROR						(PAG_Plug_EBase + 34)		// �����ļ�����
#define	PAG_FILEOPENFAIL							(PAG_Plug_EBase + 35)		// ���ļ�����
#define	PAG_OPERNOTFINISH							(PAG_Plug_EBase + 36)		// �ϴεĲ�����û�����
#define	PAG_GETPLAYTIMEFAIL							(PAG_Plug_EBase + 37)		// ��ȡ��ǰ���ŵ�ʱ�����
#define	PAG_PLAYFAIL								(PAG_Plug_EBase + 38)		// ���ų���
#define PAG_FILEFORMAT_ERROR						(PAG_Plug_EBase + 39)		// �ļ���ʽ����ȷ
#define PAG_DIR_ERROR								(PAG_Plug_EBase + 40)		// ·������
#define PAG_ALLOC_RESOURCE_ERROR					(PAG_Plug_EBase + 41)		// ��Դ�������
#define PAG_AUDIO_MODE_ERROR						(PAG_Plug_EBase + 42)		// ����ģʽ����
#define PAG_NOENOUGH_BUF							(PAG_Plug_EBase + 43)		// ������̫С
#define PAG_CREATESOCKET_ERROR		 				(PAG_Plug_EBase + 44)		// ����SOCKET����
#define PAG_SETSOCKET_ERROR							(PAG_Plug_EBase + 45)		// ����SOCKET����
#define PAG_MAX_NUM									(PAG_Plug_EBase + 46)		// �����ﵽ���
#define PAG_USERNOTEXIST							(PAG_Plug_EBase + 47)		// �û�������
#define PAG_WRITEFLASHERROR							(PAG_Plug_EBase + 48)		// дFLASH����
#define PAG_UPGRADEFAIL								(PAG_Plug_EBase + 49)		// DVR����ʧ��
#define PAG_CARDHAVEINIT							(PAG_Plug_EBase + 50)		// ���뿨�Ѿ���ʼ����
#define PAG_PLAYERFAILED							(PAG_Plug_EBase + 51)		// ���ò��ſ���ĳ������ʧ��
#define PAG_MAX_USERNUM								(PAG_Plug_EBase + 52)		// �豸���û����ﵽ���
#define PAG_GETLOCALIPANDMACFAIL					(PAG_Plug_EBase + 53)		// ��ÿͻ��˵�IP��ַ�������ַʧ��
#define PAG_NOENCODEING								(PAG_Plug_EBase + 54)		// ��ͨ��û�б���
#define PAG_IPMISMATCH								(PAG_Plug_EBase + 55)		// IP��ַ��ƥ��
#define PAG_MACMISMATCH								(PAG_Plug_EBase + 56)		// MAC��ַ��ƥ��
#define PAG_UPGRADELANGMISMATCH						(PAG_Plug_EBase + 57)		// �����ļ����Բ�ƥ��
#define PAG_MAX_PLAYERPORT							(PAG_Plug_EBase + 58)		// ������·���ﵽ���
#define PAG_NOSPACEBACKUP							(PAG_Plug_EBase + 59)		// �����豸��û���㹻�ռ���б���
#define PAG_NODEVICEBACKUP							(PAG_Plug_EBase + 60)		// û���ҵ�ָ���ı����豸
#define PAG_PICTURE_BITS_ERROR						(PAG_Plug_EBase + 61)		// ͼ����λ������,��24ɫ
#define PAG_PICTURE_DIMENSION_ERROR					(PAG_Plug_EBase + 62)		// ͼƬ��*����,��128*256
#define PAG_PICTURE_SIZ_ERROR						(PAG_Plug_EBase + 63)		// ͼƬ��С����,��100K
#define PAG_LOADPLAYERSDKFAILED						(PAG_Plug_EBase + 64)		// ���뵱ǰĿ¼��Player Sdk����
#define PAG_LOADPLAYERSDKPROC_ERROR					(PAG_Plug_EBase + 65)		// �Ҳ���Player Sdk��ĳ���������
#define PAG_LOADDSSDKFAILED							(PAG_Plug_EBase + 66)		// ���뵱ǰĿ¼��DSsdk����
#define PAG_LOADDSSDKPROC_ERROR						(PAG_Plug_EBase + 67)		// �Ҳ���DsSdk��ĳ���������
#define PAG_DSSDK_ERROR								(PAG_Plug_EBase + 68)		// ����Ӳ�����DsSdk��ĳ������ʧ��
#define PAG_VOICEMONOPOLIZE							(PAG_Plug_EBase + 69)		// ��������ռ
#define PAG_JOINMULTICASTFAILED						(PAG_Plug_EBase + 70)		// ����ಥ��ʧ��
#define PAG_CREATEDIR_ERROR							(PAG_Plug_EBase + 71)		// ������־�ļ�Ŀ¼ʧ��
#define PAG_BINDSOCKET_ERROR						(PAG_Plug_EBase + 72)		// ���׽���ʧ��
#define PAG_SOCKETCLOSE_ERROR						(PAG_Plug_EBase + 73)		// socket�����ж�,�˴���ͨ�������������жϻ�Ŀ�ĵز��ɴ�
#define PAG_USERID_ISUSING							(PAG_Plug_EBase + 74)		// ע��ʱ�û�ID���ڽ���ĳ����
#define PAG_SOCKETLISTEN_ERROR						(PAG_Plug_EBase + 75)		// ����ʧ��
#define PAG_PROGRAM_EXCEPTION						(PAG_Plug_EBase + 76)		// �����쳣
#define PAG_WRITEFILE_FAILED						(PAG_Plug_EBase + 77)		// д�ļ�ʧ��
#define PAG_FORMAT_READONLY							(PAG_Plug_EBase + 78)		// ��ֹ��ʽ��ֻ��Ӳ��
#define PAG_WITHSAMEUSERNAME						(PAG_Plug_EBase + 79)		// �û����ýṹ�д�����ͬ���û���
#define PAG_DEVICETYPE_ERROR						(PAG_Plug_EBase + 80)		// �������ʱ�豸�ͺŲ�ƥ��
#define PAG_LANGUAGE_ERROR							(PAG_Plug_EBase + 81)		// �������ʱ���Բ�ƥ��
#define PAG_PARAVERSION_ERROR						(PAG_Plug_EBase + 82)		// �������ʱ����汾��ƥ��
#define PAG_IPCHAN_NOTALIVE							(PAG_Plug_EBase + 83)		// Ԥ��ʱ���IPͨ��������
#define PAG_RTSP_SDK_ERROR							(PAG_Plug_EBase + 84)		// ���ظ���IPCͨѶ��StreamTransClient.dllʧ��
#define PAG_CONVERT_SDK_ERROR						(PAG_Plug_EBase + 85)		// ����ת���ʧ��
#define PAG_IPC_COUNT_OVERFLOW						(PAG_Plug_EBase + 86)		// ��������ip����ͨ����
#define PAG_MAX_ADD_NUM								(PAG_Plug_EBase + 87)		// ��ӱ�ǩ(һ���ļ�Ƭ��64)�ȸ����ﵽ���
#define PAG_PARAMMODE_ERROR							(PAG_Plug_EBase + 88)		// ͼ����ǿ��,����ģʽ����(����Ӳ������ʱ,�ͻ��˽����������ʱ����ֵ)
#define PAG_CODESPITTER_OFFLINE						(PAG_Plug_EBase + 89)		// ��Ƶ�ۺ�ƽ̨,�����������
#define PAG_BACKUP_COPYING							(PAG_Plug_EBase + 90)		// �豸���ڱ���
#define PAG_CHAN_NOTSUPPORT							(PAG_Plug_EBase + 91)		// ͨ����֧�ָò���
#define PAG_CALLINEINVALID							(PAG_Plug_EBase + 92)		// �߶���λ��̫���л򳤶��߲�����б 
#define PAG_CALCANCELCONFLICT						(PAG_Plug_EBase + 93)		// ȡ���궨��ͻ,��������˹���ȫ�ֵ�ʵ�ʴ�С�ߴ����
#define PAG_CALPOINTOUTRANGE						(PAG_Plug_EBase + 94)		// �궨�㳬����Χ 
#define PAG_FILTERRECTINVALID						(PAG_Plug_EBase + 95)		// �ߴ������������Ҫ��
#define PAG_DDNS_DEVOFFLINE							(PAG_Plug_EBase + 96)		// �豸û��ע�ᵽddns��
#define PAG_DDNS_INTER_ERROR						(PAG_Plug_EBase + 97)		// DDNS �������ڲ�����
#define PAG_FUNCTION_NOT_SUPPORT_OS					(PAG_Plug_EBase + 98)		// �˹��ܲ�֧�ָò���ϵͳ
#define PAG_DEC_CHAN_REBIND							(PAG_Plug_EBase + 99)		// ����ͨ������ʾ�����������
#define PAG_ALIAS_DUPLICATE							(PAG_Plug_EBase + 150)		// �����ظ�  //2011-08-31 ͨ�������������к��������豸���°汾ddns������

//STREAM������ش�����
#define STREAM_CLIENT_EBase							6000						//< �����������
#define STREAM_NEW_FAILED							(STREAM_CLIENT_EBase + 1)	// �ڴ�����ʧ��
#define STREAM_PARSEURL_FAIL						(STREAM_CLIENT_EBase + 2)	// URL����ʧ��
#define STREAM_CONNECT_VAG_FAIL            			(STREAM_CLIENT_EBase + 3)	// ������������ʧ��
#define STREAM_RTSPOPEN_FAIL               			(STREAM_CLIENT_EBase + 4)	// RTSP�Ự����ʧ��
#define STREAM_RTSPPOST_RECVCB_FAIL        			(STREAM_CLIENT_EBase + 5)	// Ͷ�����ݻص�ʧ��
#define STREAM_RTSPSEND_OPTION_FAIL        			(STREAM_CLIENT_EBase + 6)	// ����Optionʧ��
#define STREAM_RTSPSEND_DESCRIBE_FAIL      			(STREAM_CLIENT_EBase + 7)	// ����DESCRIBEʧ��
#define STREAM_RTSPSEND_SETUP_FAIL         			(STREAM_CLIENT_EBase + 8)	// ����setupʧ��
#define STREAM_RTSPSEND_PLAY_FAIL          			(STREAM_CLIENT_EBase + 9)	// ����Playʧ��
#define STREAM_RTSPSEND_PAUSE_FAIL         			(STREAM_CLIENT_EBase + 10)	// ����PAUSEʧ��
#define STREAM_RTSPSEND_TEARDOWN_FAIL      			(STREAM_CLIENT_EBase + 11)	// ����TEARDOWNʧ��
#define STREAM_PARAIS_NULL                 			(STREAM_CLIENT_EBase + 12)	// ָ�����Ϊ��
#define STREAM_RECV_RTSPRSP_TIMEOUT        			(STREAM_CLIENT_EBase + 13)	// ����RTSP��Ӧʧ��

/********************VAG_CLIENT�����붨�� end*********************/

/********************�豸������������� begin*********************/

#define VAG_DEV_CFG_GET_VIDEO_IMG_PARA				1201	// ��ȡͨ����Ƶ����ͼ�����
#define VAG_DEV_CFG_SET_VIDEO_IMG_PARA				1202	// ����ͨ����Ƶ����ͼ�����

#define VAG_DEV_CFG_GET_MOTION_DETECT_PARA			1301	// ��ȡͨ���ƶ�������
#define VAG_DEV_CFG_SET_MOTION_DETECT_PARA			1302	// ����ͨ���ƶ�������

#define VAG_DEV_CFG_GET_HIDE_ALARM_PARA				1305	// ��ȡͨ����Ƶ�ڵ���������
#define VAG_DEV_CFG_SET_HIDE_ALARM_PARA				1306	// ����ͨ����Ƶ�ڵ���������

#define VAG_DEV_CFG_GET_VIDEO_LOST_PARA				1309	// ��ȡͨ����Ƶ��ʧ��������
#define VAG_DEV_CFG_SET_VIDEO_LOST_PARA				1310	// ����ͨ����Ƶ��ʧ��������

#define VAG_DEV_CFG_GET_ALARM_IN_PARA				1311	// ��ȡ��������ͨ������
#define VAG_DEV_CFG_SET_ALARM_IN_PARA				1312	// ���ñ�������ͨ������

#define VAG_DEV_CFG_GET_CHAN_OSD_PARA				1313	// ��ȡ��Ƶͨ��OSD�ַ�����
#define VAG_DEV_CFG_SET_CHAN_OSD_PARA				1314	// ������Ƶͨ��OSD�ַ�����

/********************�豸������������� end***********************/

enum AUDIO_ENCODE_TYPE										// ��Ƶ��������
{
	AUDIO_ENCODE_G722 = 0,
	AUDIO_ENCODE_G711_U,
	AUDIO_ENCODE_G711_A,
	AUDIO_ENCODE_G726 = 6
};

enum TRANS_TYPE
{
	TRANS_UDP = 0,											// UDP���䷽ʽ
	TRANS_TCP												// TCP���䷽ʽ
};

enum STREAM_TYPE
{
	STREAM_MAIN = 0,										// ������
	STREAM_SUB												// ������
};

enum STREAM_OBTAIN_TYPE
{
	STREAM_OBTAIN_INITIATIVE = 0,							// ����ȡ����ʽ
	STREAM_OBTAIN_PASSIVE									// ������ȡ��������ʽ
};

enum DEV_REG_TYPE
{
	DEV_REG_PASSIVE = 0,									// ����ע���豸
	DEV_REG_INITIATIVE = 4									// ����ע���豸
};

enum CAP_IMG_TYPE
{
	CAP_IMG_JPEG = 0										// Jpegץͼ��ʽ
};

/********************��Ƶ�������� begin*********************/
#define VAG_SYSHEAD									1		// ϵͳͷ����
#define VAG_STREAMDATA								2		// ��Ƶ�����ݣ�����������������Ƶ�ֿ�����Ƶ�����ݣ�
#define VAG_AUDIOSTREAMDATA							3		// ��Ƶ������
#define VAG_STD_VIDEODATA							4		// ��׼��Ƶ������
#define VAG_STD_AUDIODATA							5		// ��׼��Ƶ������

/********************��Ƶ�������� end*********************/

/*****************��̨�������� begin******************/  
#define PTZ_CMD_LIGHT_PWRON							2		/* ��ͨ�ƹ��Դ */
#define PTZ_CMD_WIPER_PWRON							3		/* ��ͨ��ˢ���� */
#define PTZ_CMD_FAN_PWRON							4		/* ��ͨ���ȿ��� */
#define PTZ_CMD_HEATER_PWRON						5		/* ��ͨ���������� */
#define PTZ_CMD_AUX_PWRON1							6		/* ��ͨ�����豸���� */
#define PTZ_CMD_AUX_PWRON2							7		/* ��ͨ�����豸���� */

#define PTZ_CMD_ZOOM_IN								11		/* �������ٶ�SS���(���ʱ��) */
#define PTZ_CMD_ZOOM_OUT							12		/* �������ٶ�SS��С(���ʱ�С) */
#define PTZ_CMD_FOCUS_NEAR							13		/* �������ٶ�SSǰ�� */
#define PTZ_CMD_FOCUS_FAR							14		/* �������ٶ�SS��� */
#define PTZ_CMD_IRIS_OPEN							15		/* ��Ȧ���ٶ�SS���� */
#define PTZ_CMD_IRIS_CLOSE							16		/* ��Ȧ���ٶ�SS��С */

#define PTZ_CMD_TILT_UP								21		/* ��̨��SS���ٶ����� */
#define PTZ_CMD_TILT_DOWN							22		/* ��̨��SS���ٶ��¸� */
#define PTZ_CMD_PAN_LEFT							23		/* ��̨��SS���ٶ���ת */
#define PTZ_CMD_PAN_RIGHT							24		/* ��̨��SS���ٶ���ת */
#define PTZ_CMD_UP_LEFT								25		/* ��̨��SS���ٶ���������ת */
#define PTZ_CMD_UP_RIGHT							26		/* ��̨��SS���ٶ���������ת */
#define PTZ_CMD_DOWN_LEFT							27		/* ��̨��SS���ٶ��¸�����ת */
#define PTZ_CMD_DOWN_RIGHT							28		/* ��̨��SS���ٶ��¸�����ת */
#define PTZ_CMD_PAN_AUTO							29		/* ��̨��SS���ٶ������Զ�ɨ�� */

#define PRESET_CMD_SET_PRESET						8		/* ����Ԥ�õ� */
#define PRESET_CMD_CLE_PRESET						9		/* ���Ԥ�õ� */
#define PRESET_CMD_GOTO_PRESET						39		/* ����ת��Ԥ�õ� */

#define CRUISE_CMD_FILL_PRE_SEQ						30		/* ��Ԥ�õ����Ѳ������ */
#define CRUISE_CMD_SET_SEQ_DWELL					31		/* ����Ѳ����ͣ��ʱ�� */
#define CRUISE_CMD_SET_SEQ_SPEED					32		/* ����Ѳ���ٶ� */
#define CRUISE_CMD_CLE_PRE_SEQ						33		/* ��Ԥ�õ��Ѳ��������ɾ�� */
#define CRUISE_CMD_RUN_SEQ							37		/* ��ʼѲ�� */
#define CRUISE_CMD_STOP_SEQ							38		/* ֹͣѲ�� */
#define CRUISE_CMD_GET_ROUTE						104		/* ��ȡѲ��·�� */

#define TRACK_CMD_STA_MEM_CRUISE					34		/* ��ʼ��¼�켣 */
#define TRACK_CMD_STO_MEM_CRUISE					35		/* ֹͣ��¼�켣 */
#define TRACK_CMD_RUN_CRUISE						36		/* ��ʼ�켣 */

#define PTZ_CMD_ZERO_NEXT							40		/* ��ͨ����һҳ */
#define PTZ_CMD_ZERO_PRE							41		/* ��ͨ����һҳ */

#define PTZ_CMD_SEL_ZOOM							99		/* ���ӷŴ� */
#define PTZ_CMD_CAMERA_LOCK							200		/* ��̨���� */
#define PTZ_CMD_CAMERA_UNLOCK						201		/* �����̨���� */
#define PTZ_CAMERA_UNLOCK_ALL						202		/* ������и����ȼ��ܹ���������� */
#define PTZ_CAMERA_LOCK_CHECK						205		/* ������� */
#define PTZ_CMD_FOCUS_ONEPUSH						206		/* �����۽� */

/*****************��̨�������� end******************/  
#define PTZ_ACTION_START							0		// ��̨���ƿ�ʼ
#define PTZ_ACTION_STOP								1		// ��̨����ֹͣ

typedef struct
{
	unsigned int nYear;
	unsigned int nMonth;
	unsigned int nDay;
	unsigned int nHour;
	unsigned int nMinute;
	unsigned int nSecond;
	unsigned int nMsecond;
}VAG_TIME_INFO, *LPVAG_TIME_INFO;

typedef struct
{
	char* pszDataBuf;
	int iDataLen;
	int iPayload;
	bool bMark;
	unsigned int nTimestamp;
}VAG_MEDIA_PACKET, *LPVAG_MEDIA_PACKET;

/**	@fn	int VAG_fMsgCallBack
 *	@brief	<��Ϣ�ص�>
 *	@param iClientHandle ��ʶ���,VAG_Login���صľ��.
 *	@param iMsgType ��Ϣ����.
 *	@param pMsgPtr ��Ϣ����ָ��.
 *	@param iMsgSize ��Ϣ���ݳ���.
 *	@param pUsrData �û�����.
 */
typedef int (CALLBACK* VAG_fMsgCallBack)(int iClientHandle, int iMsgType, char* pMsgPtr, int iMsgSize, void* pUsrData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<ý�����ݻص�>
 *	@param iDataType ý����������
 *	@param pData ý������ָ��
 *	@param iDataSize ý�����ݳ���
 *	@param pUser �û��Զ�������
 */
typedef void(CALLBACK* VAG_fStreamDataCallBack)(int iDataType, void* pData, int iDataSize, void* pUser);

/**	@fn	int VAG_fStreamDataCallBack
*	@brief	<ý�����ݻص�>
*	@param iStreamChannel ý��ͨ��
 *	@param iDataType ý����������
 *	@param pstData ý�����ݽṹ��
 *	@param pUser �û��Զ�������
 */
typedef void (CALLBACK *VAG_fStreamDataCallBackEx)(int iStreamChannel, int iDataType, LPVAG_MEDIA_PACKET pstData, void* pUserData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<ý����Ϣ�ص�>
 *	@param iMsgType ��Ϣ����
 *	@param pMsgPtr ��Ϣ�ṹ
 *	@param pUser �û��Զ�������
 */
typedef int (CALLBACK *VAG_fStreamMsgCallBack)(int iMsgType, void* pMsgPtr, void* pUserData);

typedef struct
{
	char szPagURL[VAG_IPV4_LEN];							// ��½VAG��Ip
	char szUserName[VAG_USER_NAME_LEN];						// ��½�û���
	char szPassWord[VAG_PASSWORD_LEN];						// ��½����
	int iPort;												// ��½�˿�
	int	iLoginType;											// ��½����(�ݲ�ʹ��)
	VAG_fMsgCallBack fMsgCallBack;							// ��Ϣ�ص�����,���н���Ӹûص�����
	void* pMsgUserData;										// ��Ϣ�ص��û�����
	int iRes[36];
}VAG_LOGIN_PARA, *LPVAG_LOGIN_PARA;

typedef struct
{
	char szIp[VAG_IPV4_LEN];								// VAGý������Ip
	int iPort;												// VAGý�����ض˿�
	int iRes[36];
}VAG_MEDIA_INFO, *LPVAG_MEDIA_INFO;

typedef struct
{
	char szIp[VAG_IPV4_LEN];								// ����Ip��ַ
	int iPort;												// ����˿�
	TRANS_TYPE eTransType;									// ���䷽ʽ
	int iRes[36];
}VAG_TRANS_PARA, *LPVAG_TRANS_PARA;

typedef struct
{
	char szResourceKey[VAG_INDEXCODE_LEN];					// ��ѯ��Դ���ַ�����ֵ
	int iResourceKey;										// ��ѯ��Դ�����ͼ�ֵ
	int iResourceType;										// ��ѯ��Դ����
	int iRes[36];
}VAG_QUERY_RES_PARA, *LPVAG_QUERY_RES_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// Ԥ����ͨ��������
	char szUserAgent[VAG_USERAGENT_LEN];					// Ԥ��ȡ���ͻ�������
	STREAM_OBTAIN_TYPE eStreamObtainType;					// ȡ��ģʽ,(��������ģʽ��Ҫ�������stTransPara)
															// ����ȡ��ģʽ��ͨ���ص�������Ƶ����,��ͨ�����ھ�����벥��
	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBack cbStreamData;					// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	VAG_fStreamMsgCallBack cbMsgData;						// ȡ����Ϣ�ص�����
	void* pUser;											// ��Ƶ�����ݻص��û�����
	TRANS_TYPE eTransType;									// SDK��VAGȡ�����䷽ʽ(����ȡ����ʽ����Ч)
	STREAM_TYPE eStreamType;								// ȡ����������
	VAG_TRANS_PARA stTransPara;								// ����ģʽ��,����ý�����ĵ�ַ��Ϣ
    char szUrlExt[VAG_URL_EXT_LEN];				           	// ��չURL�ַ���,�����������token��֤ʱ����"ext:token=..."����""����������֤��;"ext:"�ַ���ͷ��
                                                            // ��ʹ��ʱ��""��
    int iRes[4];											// iRes[0]�����λ(31)Ϊ0��ʾ��VAG�����Ƿ�ת��,Ϊ1��ʾ�Զ����Ƿ�ת��
															// ��30λ��ʾ�Զ����Ƿ�ת����ʽ:0--����VAGת��;1--��VAGת�� 
}VAG_OPEN_STREAM_PARA, *LPVAG_OPEN_STREAM_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// Ԥ����ͨ��������
	char szUserAgent[VAG_USERAGENT_LEN];					// Ԥ��ȡ���ͻ�������
	STREAM_OBTAIN_TYPE eStreamObtainType;					// ȡ��ģʽ,(��������ģʽ��Ҫ�������stTransPara)
															// ����ȡ��ģʽ��ͨ���ص�������Ƶ����,��ͨ�����ھ�����벥��
	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBackEx cbStreamData;					// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	VAG_fStreamMsgCallBack cbMsgData;						// ȡ����Ϣ�ص�����
	void* pUser;											// ��Ƶ�����ݻص��û�����
	TRANS_TYPE eTransType;									// SDK��VAGȡ�����䷽ʽ(����ȡ����ʽ����Ч)
	STREAM_TYPE eStreamType;								// ȡ����������
	VAG_TRANS_PARA stTransPara;								// ����ģʽ��,����ý�����ĵ�ַ��Ϣ
    char szUrlExt[VAG_URL_EXT_LEN];				           	// ��չURL�ַ���,�����������token��֤ʱ����"ext:token=..."����""����������֤��;"ext:"�ַ���ͷ��
                                                            // ��ʹ��ʱ��""��
	int iRes[4];											// iRes[0]�����λ(31)Ϊ0��ʾ��VAG�����Ƿ�ת��,Ϊ1��ʾ�Զ����Ƿ�ת��
															// ��30λ��ʾ�Զ����Ƿ�ת����ʽ:0--����VAGת��;1--��VAGת�� 
}VAG_OPEN_STREAM_PARAEX, *LPVAG_OPEN_STREAM_PARAEX;

typedef struct
{
	char szDevIp[VAG_IPV4_LEN];								// ȡ�����豸IP
	int iPort;												// ȡ�����豸�˿�
	int iChannel;											// ȡ����ͨ����
	char szUserName[VAG_USER_NAME_LEN];						// ȡ���豸���û���
	char szPassword[VAG_PASSWORD_LEN];						// ȡ���豸������
	char szIndexCode[VAG_INDEXCODE_LEN];					// ȡ���豸��������
	int iDeviceType;										// ȡ���豸�����ͺ�

	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBack cbStreamData;					// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	void* pUser;											// ��Ƶ�����ݻص��û�����
	TRANS_TYPE eTransType;									// ȡ�����䷽ʽ
	STREAM_TYPE eStreamType;								// ȡ����������
    char szUrlExt[VAG_URL_EXT_LEN];				           	// ��չURL�ַ���,�����������token��֤ʱ����"ext:token=..."����""����������֤��;"ext:"�ַ���ͷ��
                                                            // ��ʹ��ʱ��""��
    int iRes[4];
}VAG_DIRECT_OPEN_STREAM_PARA, *LPVAG_DIRECT_OPEN_STREAM_PARA;

typedef struct
{
	char szDevIp[VAG_IPV4_LEN];								// ȡ�����豸IP
	int iPort;												// ȡ�����豸�˿�
	int iChannel;											// ȡ����ͨ����
	char szUserName[VAG_USER_NAME_LEN];						// ȡ���豸���û���
	char szPassword[VAG_PASSWORD_LEN];						// ȡ���豸������
	char szIndexCode[VAG_INDEXCODE_LEN];					// ȡ���豸��������
	int iDeviceType;										// ȡ���豸�����ͺ�

	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBackEx cbStreamData;					// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	void* pUser;											// ��Ƶ�����ݻص��û�����
	TRANS_TYPE eTransType;									// ȡ�����䷽ʽ
	STREAM_TYPE eStreamType;								// ȡ����������
    char szUrlExt[VAG_URL_EXT_LEN];				           	// ��չURL�ַ���,�����������token��֤ʱ����"ext:token=..."����""����������֤��;"ext:"�ַ���ͷ��
                                                            // ��ʹ��ʱ��""��
    int iRes[4];
}VAG_DIRECT_OPEN_STREAM_PARAEX, *LPVAG_DIRECT_OPEN_STREAM_PARAEX;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��̨���Ƶ�ͨ��������
	int iPtzCommand;										// ��̨���Ƶ������
	int iAction;											// ��̨���ƵĿ�ʼ��ֹͣ
	int iIndex;												// ��iPtzCommandΪ8,9,39ʱ,ΪԤ��λ���;iPtzCommandΪ37,38ʱ,ΪѲ���켣���
	int iSpeed;												// ��̨���Ƶ��ٶ�
	int iPriority;											// ��̨���Ƶ�Ȩ��ֵ
	int iUserId;											// ��̨���Ƶ��û�Id(��ѡ)
	int iMatrixCameraId;									// ������
	int iMonitorId;											// ���������
	int iLockTime;											// ��̨����ʱ��
	int iPtzCruisePoint;									// Ѳ������
	int iPtzCruiseInput;									// ��ͬѲ������ʱ��ֵ��ͬ��Ԥ�õ�(���255)��ʱ��(���255)���ٶ�(���40)
	int iRes[36];
}VAG_PTZ_CONTROL_PARA, *LPVAG_PTZ_CONTROL_PARA;

typedef struct
{
	int nPtzPresetIndex;									// ��̨Ԥ�õ���
	int nDwellTime;											// ��̨Ԥ�õ�ͣ��ʱ��
	int nPtzSpeed;											// ��̨Ԥ�õ������ٶ�
	int iRes[36];
}VAG_PTZ_CFG_POINT_INFO, *LPVAG_PTZ_CFG_POINT_INFO;

typedef struct
{
	int iPtzCfgPointCount;									// ��̨����Ԥ�õ���
	VAG_PTZ_CONTROL_PARA stPtzControlPara;					// ��̨���û�������
	VAG_PTZ_CFG_POINT_INFO pstPtzCfgPoint[VAG_PTZ_POIT_NUM];// ��̨����Ԥ�õ����
}VAG_PTZ_CFG_INFO, *LPVAG_PTZ_CFG_INFO;

typedef struct
{
	char szLocalIp[VAG_IPV4_LEN];							// Ip
	int iRes[36];
}VAG_BIND_LOCALIP_INFO, *LPVAG_BIND_LOCALIP_INFO;

typedef struct
{
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// �豸������
	char szGroupName[VAG_NAME_LEN];							// �豸����
	char szParentGroup[VAG_INDEXCODE_LEN];					// �ϼ�������
	int iRes[36];
}VAG_GROUP_INFO, *LPVAG_GROUP_INFO;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	char szDeviceName[VAG_NAME_LEN];						// �豸����
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// �豸����������
	char szDeviceAddr[VAG_IPV4_LEN];						// �豸Ip
	char szUserName[VAG_USER_NAME_LEN];						// �豸�û���
	char szPassWord[VAG_PASSWORD_LEN];						// �豸����
	char szDeviceMulticast[VAG_IPV4_LEN];					// �豸�ಥIp
	int iPort;												// �豸�˿�
	int	iDevType;											// �豸����
	int	iDevRegType;										// �豸ע������
	int	iVoiceType;											// �豸��������
	int iRes[36];
}VAG_DEVICE_INFO, *LPVAG_DEVICE_INFO;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	char szCamName[VAG_NAME_LEN];							// ��Ƶͨ������
	int	iStreamTransform;									// �Ƿ�������ת��
	int	iVagTransmit;										// �Ƿ�ͨ��Vagת��
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// ��Ƶͨ�������豸���
	char szGroupIndexCode[VAG_INDEXCODE_LEN];				// �豸����������
	int iChanNum;											// ͨ�����
	int iRes[36];
}VAG_CAMERA_INFO, *LPVAG_CAMERA_INFO;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	char szCamName[VAG_NAME_LEN];							// ��Ƶͨ������
	int	iStreamTransform;									// �Ƿ�������ת��
	int	iVagTransmit;										// �Ƿ�ͨ��Vagת��
	int iChanNum;											// ͨ�����
	int iStreamTransType;									// ͨ��֧�ֵ�ȡ����ʽ:0-UDP;1-TCP;2-UDP/TCP
	VAG_DEVICE_INFO stDeviceInfo;							// ͨ�����ڵ��豸��Ϣ
	int iRes[36];
}VAG_CAMERA_DEVICE_INFO, *LPVAG_CAMERA_DEVICE_INFO;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	int iChanNum;											// �Խ�ͨ�����
	VAG_fStreamDataCallBack cbStreamData;					// �������ݻص�����
	void* pUser;											// �������ݻص��û�����
	int iRes[36];
}VAG_DEVICE_TALK_INFO, *LPVAG_DEVICE_TALK_INFO;

typedef struct
{
	int iTalkHandle;										// VAG_StartDeviceTalk���ص��豸�Խ����
	int iDataSize;											// �豸�Խ����ݳ���
	void* pData;											// �豸�Խ�����
	int iRes[36];
}VAG_DEVICE_TALK_DATA, *LPVAG_DEVICE_TALK_DATA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	char szEventPubUrl[VAG_EVENT_PUB_URL_LEN];				// �¼�����url
	int iRes[36];
}VAG_SUBSCRIBE_EVENT_PARA, *LPVAG_SUBSCRIBE_EVENT_PARA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	char szGpsPubUrl[VAG_GPS_PUB_URL_LEN];		    		// GPS����url
	int iRes[36];
}VAG_SUBSCRIBE_GPS_PARA, *LPVAG_SUBSCRIBE_GPS_PARA;

typedef struct
{
    char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
    int iRes[36];
}VAG_DESUBSCRIBE_GPS_PARA, *LPVAG_DESUBSCRIBE_GPS_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	CAP_IMG_TYPE eCaptureImgType;							// ץͼ��ʽ
	int iImgSize;											// ͼƬ�ߴ�:0-CIF(352*288/352*240),1-QCIF(176*144/176*120),2-4CIF(704*576/704*480)��D1(720*576/720*486),3-UXGA(1600*1200)
															// ,4-SVGA(800*600),5-HD720P(1280*720),6-VGA(640*480),7-XVGA(1280*960),8-HD900P(1600*900),9-HD1080P(1920*1080),10-2560*1920
															// ,11-1600*304,12-2048*1536,13-2448*2048,14-2448*1200,15-2448*800,16-XGA(1024*768),17-SXGA(1280*1024),18-WD1(960*576/960*480)
															// ,19-1080I(1920*1080),20-576*576,21-1536*1536,22-1920*1920,0xff-Auto(ʹ�õ�ǰ�����ֱ���)
	int iImgQuality;										// ͼƬ����ϵ��:0-���,1-�Ϻ�,2-һ��
	int iRes[36];
}VAG_CAPTURE_PARA, *LPVAG_CAPTURE_PARA;

typedef struct
{
	char szIndexCode[VAG_INDEXCODE_LEN];					// IO�����豸���
	int iIoNum;												// IOͨ����
	int iStatus;											// IO״̬:0-�ر�,1-��
	int iDelayTime;											// IO״̬��ʱ(��λ:��)
	int iRes[36];
}VAG_IO_SET_PARA, *LPVAG_IO_SET_PARA;

typedef struct
{
	char szIndexCode[VAG_INDEXCODE_LEN];					// IO�����豸���
	int iIoNum;												// IOͨ����
	int iRes[36];
}VAG_IO_GET_PARA, *LPVAG_IO_GET_PARA;

typedef struct
{
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	int iRes[36];
}VAG_DESUBSCRIBE_EVENT_PARA, *LPVAG_DESUBSCRIBE_EVENT_PARA;

typedef struct												// ��ȡ,����¼�����
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	int iRes[36];
}VAG_RECORD_CONFIG_PARA, *LPVAG_RECORD_CONFIG_PARA;

typedef struct
{
	int iAllDayRecord;										// �Ƿ�ȫ��¼��:0-��;1-��
	char chRecordType;										// ¼������
	int iRes[36];
}VAG_RECORD_DAY_INFO, *LPVAG_RECORD_DAY_INFO;

typedef struct
{
	int iRecord;											// �Ƿ�¼��: 0-��;1-��
	int iRecordTime;										// ����ʱ�ӳ�¼��ʱ��:0-5��;1-10��;2-30��;3-1��;4-2��;5-5��;6-10��
	int iPreRecordTime;										// Ԥ¼ʱ��:0-��Ԥ¼;1-5��;2-10��;3-15��;4-20��;5-25��;6-30��;7-������Ԥ¼
	int iRecorderDuration;									// ¼�񱣴��ʱ��:����0����¼��Ҫɾ��;����0�����������
	int iRedundancyRec;										// �Ƿ�����¼��,����˫����:0-��;1-��
	int iAudioRec;											// ¼��ʱ����������ʱ�Ƿ��¼��Ƶ����:0-����¼;1-��¼
	VAG_RECORD_DAY_INFO stRecAllDay[VAG_MAX_DAYS];			// ȫ��¼�����
	char chRecPlan[7][48];									// 0 ��ʾ��¼��,����ֵ��ʾ¼������
															// 0-��¼��;1-��ʱ¼��;2-�ƶ����¼��;3-����|����;4-��������¼��;5-����&����;6-�����;
															// 7-����¼��;8-�ֶ�¼��;9-�𶯱���;A-��������;B-PIR����;C-���߱���;D-���ȱ���;E-���б���;
															// F-���ܽ�ͨ�¼�;G-�����������;H-��Ƶ�쳣���
	char szRes[256];
	int iRes[36];
}VAG_RECORD_CONFIG_INFO, *LPVAG_RECORD_CONFIG_INFO;

typedef struct												// ��ѯ¼���ļ�����
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	int iRecFileType;										// ��ѯ¼���ļ�����
															// 0xff-����¼��;1-��ʱ¼��;2-�ƶ����¼��;3-����|����;4-��������¼��;5-����&����;6-�����;
															// 7-����¼��;8-�ֶ�¼��;9-�𶯱���;10-��������;11-PIR����;12-���߱���;13-���ȱ���;14-���б���;
															// 15-���ܽ�ͨ�¼�;16-�����������;17-��Ƶ�쳣���
	VAG_TIME_INFO stStartTime;								// ��ѯ��ʼʱ��
	VAG_TIME_INFO stEndTime;								// ��ѯ����ʱ��
	char szRes[256];
	int iRes[36];
}VAG_FIND_RECORD_PARA, *LPVAG_FIND_RECORD_PARA;

typedef struct												// ¼���ļ���Ϣ
{
	char szFileName[VAG_NAME_LEN];							// ¼���ļ���
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// ¼��ط�URL
	unsigned int iFileSize;									// ¼���ļ���С
	int iRecType;											// ¼������
															// 1-��ʱ¼��;2-�ƶ����¼��;3-����|����;4-��������¼��;5-����&����;6-�����;
															// 7-����¼��;8-�ֶ�¼��;9-�𶯱���;10-��������;11-PIR����;12-���߱���;13-���ȱ���;14-���б���;
															// 15-���ܽ�ͨ�¼�;16-�����������;17-��Ƶ�쳣���
	VAG_TIME_INFO stStartTime;								// ¼���ļ���ʼʱ��
	VAG_TIME_INFO stStopTime;								// ¼���ļ�����ʱ��
	char szRes[256];
	int iRes[36];
}VAG_RECORD_FILE_INFO, *LPVAG_RECORD_FILE_INFO;

typedef struct
{
	bool bDownload;
	bool bRelSeek;
	int iRelFrom;
	int iRelTo;
	bool bAbsSeek;
	VAG_TIME_INFO stAbsFrom;
	VAG_TIME_INFO stAbsTo;
	bool bScale;
	double fScale;
	int iRes[32];
}VAG_PLAY_SET_PARA, *LPVAG_PLAY_SET_PARA;

typedef struct
{
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// Ԥ����ͨ��������
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// ¼��ط�URL
	char szUserAgent[VAG_USERAGENT_LEN];					// Ԥ��ȡ���ͻ�������
	STREAM_OBTAIN_TYPE eStreamObtainType;					// ȡ��ģʽ,(��������ģʽ��Ҫ�������stTransPara)
															// ����ȡ��ģʽ��ͨ���ص�������Ƶ����
	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBackEx cbStreamData;					// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	VAG_fStreamMsgCallBack cbMsgData;						// ȡ����Ϣ�ص�����
	void* pUserData;										// ��Ƶ�����ݻص��û�����
	VAG_PLAY_SET_PARA stPlaybackPara;						// �طŲ���
	TRANS_TYPE eTransType;									// SDK��VAGȡ�����䷽ʽ(����ȡ����ʽ����Ч)
	STREAM_TYPE eStreamType;								// ȡ����������
	VAG_TRANS_PARA stTransPara;								// ����ģʽ��,����ý�����ĵ�ַ��Ϣ
    char szUrlExt[VAG_URL_EXT_LEN];				           	// ��չURL�ַ���,�����������token��֤ʱ����"ext:token=..."����""����������֤��;"ext:"�ַ���ͷ��
                                                            // ��ʹ��ʱ��""��
	int iRes[4];											// iRes[0]�����λ(31)Ϊ0��ʾ��VAG�����Ƿ�ת��,Ϊ1��ʾ�Զ����Ƿ�ת��
															// ��30λ��ʾ�Զ����Ƿ�ת����ʽ:0--����VAGת��;1--��VAGת�� 
}VAG_PLAY_BACK_PARA, *LPVAG_PLAY_BACK_PARA;

typedef struct
{
	char szPlaybackUrl[VAG_STREAM_URL_LEN];					// ¼��ط�URL
	char szDevIp[VAG_IPV4_LEN];								// ȡ�����豸IP
	int iPort;												// ȡ�����豸�˿�
	int iChannel;											// ȡ����ͨ����
	char szUserName[VAG_USER_NAME_LEN];						// ȡ���豸���û���
	char szPassword[VAG_PASSWORD_LEN];						// ȡ���豸������
	char szIndexCode[VAG_INDEXCODE_LEN];					// ȡ���豸��������
	int iDeviceType;										// ȡ���豸�����ͺ�

	HWND hPlayWnd;											// ����Ԥ���Ĵ��ھ��
	VAG_fStreamDataCallBackEx cbStreamDataEx;				// ȡ�����ݻص�����,ΪNULL��ʾ����ȡ��Ƶ����
	void* pUser;											// ��Ƶ�����ݻص��û�����
	TRANS_TYPE eTransType;									// ȡ�����䷽ʽ
	STREAM_TYPE eStreamType;								// ȡ����������
	int iDownload;											// 0:������;1:����
	VAG_TIME_INFO stAbsFrom;
	VAG_TIME_INFO stAbsTo;
	void* pReservePtr;
	int iReserveLen;
}VAG_DIRECT_PLAY_BACK_PARA, *LPVAG_DIRECT_PLAY_BACK_PARA;

typedef struct
{
	int iCommand;											// �������������
	char szDeviceIndexCode[VAG_INDEXCODE_LEN];				// �豸���
	char szCamIndexCode[VAG_INDEXCODE_LEN];					// ��Ƶͨ�����
	int iRes[36];
}VAG_DEV_CONFIG_PARA, *LPVAG_DEV_CONFIG_PARA;

typedef struct
{
	int iResourceType;										// ��Դ����
	char szIndexCode[VAG_INDEXCODE_LEN];					// ��Դ���
	char szAbilityCommand[VAG_USER_NAME_LEN];				// ��ȡ�����������
	int iAbilityVersion;									// �������汾
	char* pExtParams;										// ��չ�������
	int	iExtParamsLen;										// ��չ�����������
	int iRes[36];
}VAG_DEV_ABILITY_PARA, *LPVAG_DEV_ABILITY_PARA;

typedef struct
{
	int iHue;												// ɫ��[1,10]
	int iContrast;											// �Աȶ�[1,10]
	int iBright;											// ����[1,10]
	int iSaturation;										// ���Ͷ�[1,10]
	int iRes[36];
}VAG_CFG_VIDEO_IMAGE_PARA, *LPVAG_CFG_VIDEO_IMAGE_PARA;

typedef struct
{
	int iHue;												// ɫ��[1,10]
	int iContrast;											// �Աȶ�[1,10]
	int iBright;											// ����[1,10]
	int iSaturation;										// ���Ͷ�[1,10]
	int iRes[36];
}VAG_CFG_DEV_IMAGE_PARA, *LPVAG_CFG_DEV_IMAGE_PARA;

typedef struct
{
	int iIsUse;												// �Ƿ������ƶ���ⱨ��:0-������;1-����
	int iSensitive;											// �ƶ����������:[0,5],Խ��Խ����
	int iRes[36];
}VAG_CFG_DEV_MOTION_DETECT_PARA, *LPVAG_CFG_DEV_MOTION_DETECT_PARA;

typedef struct
{
	int iIsUse;												// �Ƿ�������Ƶ�ڵ�����:0-������;1-����
	int iSensitive;											// ��Ƶ�ڵ�������:[1,3],Խ��Խ����
	int iRes[36];
}VAG_CFG_DEV_HIDE_ALARM_PARA, *LPVAG_CFG_DEV_HIDE_ALARM_PARA;

typedef struct
{
	int iIsUse;												// �Ƿ�������Ƶ��ʧ����:0-������;1-����
	int iRes[36];
}VAG_CFG_DEV_VIDEO_LOST_PARA, *LPVAG_CFG_DEV_VIDEO_LOST_PARA;

typedef struct
{
	int iIsUse;												// �Ƿ�������Ƶ��ʧ����:0-������;1-����
	int iAlarminType;										// ��������ͨ������:0-����;1-����
	char chAlarminName[VAG_NAME_LEN];
	int iRes[36];
}VAG_CFG_DEV_ALARMIN_PARA, *LPVAG_CFG_DEV_ALARMIN_PARA;

typedef struct
{
	char szOsdString[VAG_CHAN_OSD_MAX_LEN];					// ��Ƶͨ��OSD�ַ�
	int iIsShowString;										// �Ƿ������Ƶͨ��OSD�ַ�
	int iXPos;												// ��Ƶͨ��OSD�ַ�Xλ��
	int iYPos;												// ��Ƶͨ��OSD�ַ�Yλ��
	int iRes[36];
}VAG_CFG_DEV_OSD_STRING, *LPVAG_CFG_DEV_OSD_STRING;

typedef struct
{
	int iStringNum;											// ��Ƶͨ��OSD�ַ�����
	VAG_CFG_DEV_OSD_STRING pstShowStringPara[VAG_CHAN_OSD_MAX_NUM];
	int iRes[36];
}VAG_CFG_DEV_OSD_STRING_PARA, *LPVAG_CFG_DEV_OSD_STRING_PARA;

typedef struct
{
    VAG_CAPTURE_PARA sCapturePara;                          ///< ץͼ����
    char chPicServerUrl[VAG_PICSERVER_URL_LEN];             ///< ͼƬ������RUL,�磺ftp://10.64.51.26:21/admin/12345/
}VAG_PICTURE2SERVER_PARA,*LPVAG_PICTURE2SERVER_PARA;
#endif
