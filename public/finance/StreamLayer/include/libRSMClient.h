/**	@file libRSMClient.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ��ý��ͻ���SDK������ý���������ȡ��Ƶ����
 *
 *	@author		kuangjg
 *	@date		 2008/4/14
 *
 *	@note V2.1.0.0  ��doxygen��ʽ����ע��
 *
 *	@warning ��
 */

#ifndef __LIBRSMCLIENT_H__
#define __LIBRSMCLIENT_H__

#include <time.h>

#ifdef LIBRSMCLIENT_EXPORTS
#define RSM_CLIENT_API __declspec(dllexport)
#else
#define RSM_CLIENT_API __declspec(dllimport)
#endif


typedef int HSESSION;	/**< �Ự���Ͷ��� */


/**	@struct ST_DEVICE_NETINFO
*  @brief �豸��Ϣ
*/
typedef struct  
{
	char				szAccessSrvIP[32];	/**< ���������IP*/
	unsigned short int	nAccessSrvPort;		/**< ����������˿�*/
	//unsigned char		szReserved[2];		/**< ��ɾ���ֶ�*/
	char				nNetLine;			/**< �豸����������·���:0,1,2...*/
	char				nStreamEnCodeing;	/**< ����:ת���ʶ*/
	char				szDeviceID[64];		/**< �豸���к�*/
	int					nChan;				/**< ͨ����,��1��ʼ*/
	int					nStreamType;		/**< ������: 0-������; 1-������*/
	int					nTransPortType;		/**< Ҫ����ý���ǰ��ȡ���ķ�ʽ0:UDP;1:TCP*/
}ST_DEVICE_NETINFO;


/**	@struct ST_SERVER_STATUS
*  @brief ��������Ϣ
*/
typedef struct  
{
	char				szSrvIP[32];				/**< ������IP*/
	unsigned short int	nSrvPort;					/**< �������˿�*/
	unsigned short int	nInputStreamNum;			/**< ��������*/
	unsigned short int	nOutputStreamNum;			/**< �������*/
	char				szSrvVersion[32];			/**< �������汾*/
	unsigned short int  nReserved;					/**< �����ֽ�*/
	time_t 				tOnlineTime;		        /**< ����������ʱ��*/  
	unsigned int		nCpuUsage;					/**< ������CPUʹ����*/
	unsigned int		nTotalPhys;					/**< �������ܵ������ڴ�*/
	unsigned int		nAvailPhys;					/**< ���������������ڴ�*/
}ST_SERVER_STATUS;

/**	@struct ST_RSM_SESSION_INFO
*  @brief �Ự��Ϣ
*/
typedef struct  
{
	unsigned int nRecvedRTPCount;	/**< ���յ�����rtp����Ŀ*/
	int			 iLostRTPCount;		/**< ��ʧ��rtp����Ŀ*/
}ST_RSM_SESSION_INFO;


/**	@enum EN_RSM_STREAM_DATATYPE
*  @brief ��������,ע����EN_STREAM_DATATYPE�еĳ����������ֿ�
*/
typedef enum
{
	STREAM_DATATYPE_DATA = 0,			/**< ����*/
	STREAM_DATATYPE_HDR,				/**< ��ͷ*/
	
}EN_RSM_STREAM_DATATYPE;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	*@brief �����ݻص�������������
	*@param hSessionID:	���ֲ�ͬ����Ƶ����ID
	*@param iUsrData:	�û�����
	*@param iDataType:	����������:��ͷ���ݻ�����ͨ���� STREAM_DATATYPE_DATA:��ͨ����; STREAM_DATATYPE_HDR:ͷ����
	*@param pAVData:	ָ�������ݻ���
	*@param nAVDataLen:	�������ֽ���
	*@return �޷���ֵ
	**/
	typedef void (__stdcall *pRSMAVDataCallBack)( HSESSION hSessionID, unsigned int nUsrData,
		int iDataType, const void* pAVData, size_t nAVDataLen);


	/**
	*@brief ��ʼ����ý��ͻ��˿�
	*@return 0:�ɹ�; -1:ʧ��
	**/
	RSM_CLIENT_API int __stdcall InitRSMClientLib( );


	/**
	*@brief ��ֹ��ý��ͻ��˿�
	*@return 0:�ɹ�; -1:ʧ��
	**/
	RSM_CLIENT_API int __stdcall FiniRSMClientLib(void);


	/**
	*@brief ��ʼ����ý�������ȡ��
	*@param pszSrvIP:		�����IP
	*@param nSrvPort:		����˶˿�
	*@param pszPrivate:		˽�д�
	*@param pDeviceInfo:	ָ��Ҫ���ӵ��豸��Ϣ
	*@param pRSMAVDataCallBack:��ȡ������Ƶ���ݻص�����ָ��
	*@param nUsrData:		������ص��û�����
	*@return >=0:ȡ���ɹ�, ��ֵ��Ϊ����ID; -1:ʧ��
	**/
	RSM_CLIENT_API HSESSION __stdcall PlayRSMAVStream( const char* pszSrvIP, 
		unsigned short int nSrvPort, const char* pszPrivate,
		const ST_DEVICE_NETINFO* pDeviceInfo, pRSMAVDataCallBack pfn, unsigned int nUsrData );


	/**
	*@brief ֹͣ�ӷ�����ȡ��
	*@param hSessionID:	PlayAVStream���ص���ID 
	*@return �޷���ֵ
	**/
	RSM_CLIENT_API void __stdcall StopRSMAVStream( HSESSION hSessionID );


	/**
	*@brief ��ȡ������״̬
	*@param pszSrvIP:		������IP
	*@param nSrvPort:		�������˿�
	*@param pServerStatus:	ָ���������Ϣ
	*@return 0:�ɹ�; -1:ʧ��
	**/
	RSM_CLIENT_API int	__stdcall RSMGetStatus( const char* pszSrvIP, unsigned short int nSrvPort, ST_SERVER_STATUS* pServerStatus );

	/**
	*@brief ��ȡ�Ự��Ϣ
	*@param hSessionID:		�ỰID
	*@param pSessionInfo:	�Ự��Ϣ�ṹ��ָ��
	*@return 0:�ɹ�; -1:ʧ��
	**/
	RSM_CLIENT_API int __stdcall GetRSMSessionInfo( HSESSION hSessionID,  ST_RSM_SESSION_INFO* pSessionInfo );


	/**
	*@brief ��ʼ�ӷ�����ȡ��,����tcp��ʽ
	*@param pszSrvIP:		�����IP
	*@param nSrvPort:		����˶˿�
	*@param pszPrivate:		˽�д�
	*@param pDeviceInfo:	ָ��Ҫ���ӵ��豸��Ϣ
	*@param pRSMAVDataCallBack:��ȡ������Ƶ���ݻص�����ָ��
	*@param nUsrData:		������ص��û�����
	*@return 0:�ɹ�; -1:ʧ��
	**/
	RSM_CLIENT_API HSESSION __stdcall PlayRSMAVStreamByTCP( const char* pszSrvIP, 
		unsigned short int nSrvPort, const char* pszPrivate,
		const ST_DEVICE_NETINFO* pDeviceInfo, pRSMAVDataCallBack pfn, unsigned int nUsrData );


	/**
	*@brief ֹͣTCP��ʽ�ӷ�����ȡ��
	*@param hSessionID:	PlayRSMAVStreamByTCP���ص���ID 
	*@return ��
	**/
	RSM_CLIENT_API void __stdcall StopRSMAVStreamByTCP( HSESSION hSessionID );

	/**
	*@brief ȡ�����ڷ��͵���Ƶ�������󣬵��øýӿں�PlayRSMAVStreamByTCP��PlayRSMAVStream�ӿڽ����ٷ���
	*@return ��
	**/
    RSM_CLIENT_API void __stdcall CancellPlayRequest( );


#ifdef __cplusplus
}
#endif

#endif

