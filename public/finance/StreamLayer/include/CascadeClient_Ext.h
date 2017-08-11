#ifndef __CASCADECLIENT_EXT_H__
#define __CASCADECLIENT_EXT_H__

#include <stdlib.h>
#if defined (_WIN32)
#if (!defined(CALLBACK))
#define CALLBACK __stdcall
#endif
#else
#define CALLBACK 
#endif

//CASC_fClientCallBack���ص��������� iDataType
#define DATA_TYPE_CATALOG                    0              /**< Ŀ¼                     */
#define DATA_TYPE_REALMEDIA                  1              /**< ʵʱURL                  */
#define DATA_TYPE_FILE                       2              /**< ¼���ļ��б�             */
#define DATA_TYPE_FILEURL                    3              /**< ¼��㲥URL              */
#define DATA_TYPE_REGISTER                   4              /**< ע��                     */
#define DATA_TYPE_HEARTBEAT                  5              /**< ����                     */
#define DATA_TYPE_PTZ                        6              /**< ��̨����                 */
#define DATA_TYPE_STARTSTREAM                7              /**< ����ʵʱ�����ط���       */
#define DATA_TYPE_STOPSTREAM                 8              /**< ֹͣʵʱ�����ط���       */
#define DATA_TYPE_SERVERSTOP                 10             /**< �뼶������������״̬     */
#define DATA_TYPE_PAGVOICEURL                11             /**< �����Խ�URL              */
#define DATA_TYPE_CATALOGQUERY               12             /**< Ŀ¼��ѯ                 */
#define DATA_TYPE_FILEEND                    13             /**< �طŻ������ؽ���         */

//add by yangcheng begin 
//��������ػص���Ϣ����
#define DATA_TYPE_DECODER_RECV_DATA_ERROR    21              /**< ý�崦���쳣���������ݴ���   */
#define DATA_TYPE_DECODER_RECV_PACKET_NUM    22              /**< ������ý�������Ϣ           */
#define DATA_TYPE_DECODER_PLAYBACK_TIME      27              /**< ¼��طŽ�����Ϣ����λΪ�룩 */
#define DATA_TYPE_DECODER_FILE_EDN           28              /**< ¼��طŽ�����ʶ             */

//��������
#define STREAM_TYPE_HEAD		0
#define STREAM_TYPE_VIDEO		1
#define STREAM_TYPE_AUDIO		2
#define STREAM_TYPE_END		    100
//add by yangcheng end

//����ֵ
#define   CASC_RET_OK		                 0              /**< ִ�гɹ�                 */
#define   CASC_RET_ERROR                     -1             /**< ִ��ʧ��                 */
#define   CASC_RET_FILEOVER                  1              /**< ¼���ļ�ȫ������         */


//���������������붨�� 
#define ERR_CASCADE_400_FAIL                 400          //ʧ����Ӧ������ʧ��
#define ERR_CASCADE_401_FAIL                 401          //ʧ����Ӧ������ʧ��

#define ERR_HPR_INIT_FAIL                    1001         //hpr���ʼ��ʧ��
#define ERR_TIMER_CREATE_FAIL                1002         //������ʱ��ʧ��
#define ERR_LETTERLIB_INIT_FAIL              1003         //letterlib���ʼ��ʧ��
#define ERR_THREAD_CREATE_FAIL               1004         //�����߳�ʧ��
#define ERR_EXOSIP_INIT_FAIL                 1005         //SIP���ʼ��ʧ��
#define ERR_EXOSIP_LISTEN_FAIL               1006         //SIP����������ʧ��
#define ERR_EXOSIP_SEND_FAIL                 1007         //SIP�ⷢ����Ϣʧ��
#define ERR_SOCKET_CREATE_FAIL               1008         //����socketʧ�ܻ�����socket����ʧ��
#define ERR_IOCP_CREATE_FAIL                 1009         //iocp����ʧ��
#define ERR_CONFIG_GETPARAM_FAIL             1010         //�������ļ���ȡ���ò�������
#define ERR_DB_CREATE_FAIL                   1011         //�������sqlite���ݿ��ļ�ʧ��
#define ERR_MEMORY_APPLY_FAIL                1012         //�ڴ�����ʧ��
#define ERR_SOCKET_SET_REUSE_FAIL            1013         //����SOCKET����ʧ��
#define ERR_SOCKET_SET_BLOCK_FAIL            1014         //����SOCKET�Ƿ�����ʧ��
#define ERR_APPWEB_INIT_FAIL                 1015         //appWeb��������ʼ��ʧ��
#define ERR_WEBLAYER_INIT_FAIL               1016         //WebLayer������web��Ϣ���ʼ��ʧ��


#define ERR_HIK_REQHEAD_PARESE_FAIL          1201         //��������Э������xmlͷ����
#define ERR_HIK_RSPHEAD_PARESE_FAIL          1202         //��������Э����Ӧxmlͷ����
#define ERR_HIK_RSP_PARESE_FAIL              1203         //��������Э����Ӧxml�����
#define ERR_HIK_REQ_PARESE_FAIL              1204         //��������Э������xml�����
#define ERR_HIK_UP_NOTREGISTER               1205         //����Э���ϼ�δע��
#define ERR_HIK_CATALOG_FAIL                 1206         //����Э������Ŀ¼�ϼ���Ӧʧ��
#define ERR_HIK_CREATE_LISTENSOCKET_FAIL     1207         //����Э�飬��������socketʧ�ܣ�7100��
#define ERR_HIK_DOWN_CASCADE_NOT_FIND        1208         //����Э�飬δ�ҵ���Ӧ���¼�����������

#define ERR_CMSWS_INIT_FAIL                  1401         //��ʼ��CMS WebService ����ʧ��
#define ERR_CMSWS_START_FAIL                 1402         //���� CMS WebService ����ʧ��
#define ERR_CMSWS_GETCONTROLUNIT_FAIL        1403         //��cms��ȡ���еĿ���������֯ʧ��
#define ERR_CMSWS_GETREGIONUNIT_FAIL         1404         //��cms��ȡ���е�����ʧ��
#define ERR_CMSWS_GETVRMSERVER_FAIL          1405         //��cms��ȡ���е�VRM��Ϣʧ��
#define ERR_CMSWS_GETVTDUSERVER_FAIL         1406         //��cms��ȡ���е�vtdu��Ϣʧ��
#define ERR_CMSWS_GETPTZSERVER_FAIL          1407         //��cms��ȡ���е�ptz��Ϣʧ��
#define ERR_CMSWS_GETALARMSERVER_FAIL        1408         //��cms��ȡ���е�alarm��Ϣʧ��
#define ERR_CMSWS_GETPAGSERVER_FAIL          1409         //��cms��ȡ���е�pag��Ϣʧ��
#define ERR_CMSWS_GETCASCADESERVER_FAIL      1410         //��cms��ȡ���е�cascade��Ϣʧ��
#define ERR_CMSWS_GETALLDEVICE_FAIL          1411         //��cms��ȡ���е��豸��Ϣʧ��
#define ERR_CMSWS_GETALLCAMERA_FAIL          1412         //��cms��ȡ���еļ�ص���Ϣʧ��
#define ERR_CMSWS_SAVECATALOG_FAIL           1413         //��cms�����¼�Ŀ¼��Ϣʧ��

#define ERR_CAMEAR_NOEXIST                   1601         //�ü�ص㲻����
#define ERR_CASCADE_NOEXIST                  1602         //��Ӧ�ļ���������������
#define ERR_INSERT_PTZQUEUE_FAIL             1603         //��������ptz����ʧ��
#define ERR_INSERT_MEDIAQUEUE_FAIL           1604         //��������media����ʧ��
#define ERR_INSERT_DB33QUEUE_FAIL            1605         //��������db33����ʧ��
#define ERR_INSERT_DHKPQUEUE_FAIL            1606         //��������dhkp����ʧ��
#define ERR_DOWN_CASCADE_NOT_FIND            1607         //�Ҳ�����Ӧ���¼�����������
#define ERR_UP_CASCADE_NOT_FIND              1608         //�Ҳ�����Ӧ���ϼ�����������

#define ERR_PTZ_SERVER_NOEXIST               1701         //��̨����ʱ����ȡptz��������Ϣʧ��
#define ERR_PTZ_COMM_PTZSERVER_FAIL          1702         //����̨������ͨ��ʧ��
#define ERR_PTZ_CONTROL_FAIL                 1703         //��̨������������̨����ʧ��
#define ERR_PTZ_HIKTODB33_FAIL               1704         //��̨���������ɺ�����ʽת��Ϊdb33��ʽʧ��
#define ERR_PTZ_DB33TOHIK_FAIL               1705         //��̨����������db33��ʽת��Ϊ������ʽʧ��

#define ERR_FILE_VRM_NOEXIST                 1801         //��ȡvrm��Ϣʧ��
#define ERR_FILE_QUERYINTERFACE_NEW_FAIL     1802         //����vrm¼���ѯ�⣬����ʧ��
#define ERR_FILE_QUERYINTERFACE_OPEN_FAIL    1803         //����vrm¼���ѯ�⣬��ʧ��
#define ERR_FILE_STATS_OVER_FAIL             1804         //¼���ѯ����״̬ʧ��
#define ERR_FILE_FILES_NUM_ZERO              1805         //��ѯ����¼���ļ���Ϊ0
#define ERR_FILE_FROMINDEX_OVER_TOTAL        1806         //¼���ҳ��ѯ��ʼ��Ŵ����ļ�����


#define ERR_FILEURL_FILES_NOEXIST            1901         //��ѯ�㲥url����Ӧ�ļ��б�����
#define ERR_FILEURL_FILE_SEARCH_FAIL         1902         //��ѯ�㲥url����Ӧ�ļ��б��ѯʧ��
#define ERR_FILEURL_FILE_NOEXIST             1903         //��ѯ�㲥url�����ļ��ڶ�Ӧ�ļ��б��в�����

#define ERR_MEDIAURL_DEVICE_NOEXIST          2001         //δ�ҵ��ü�ص��Ӧ���豸��Ϣ
#define ERR_MEDIAURL_REGION_NOEXIST          2002         //δ�ҵ��ü�ص��Ӧ��������Ϣ
#define ERR_MEDIAURL_VTDU_NOEXIST            2003         //δ�ҵ��ü�ص��Ӧ��VTDU��������Ϣ
#define ERR_MEDIAURL_PAG_NOEXIST             2004         //δ�ҵ��ü�ص��Ӧ��pag��������Ϣ
#define ERR_MEDIAURL_CAMEAR_NOEXIST          2005         //�ü�ص��ڱ���������


#define ERR_MEDIA_CONNECT_SERVER_FAIL        2101         //����MEDIA������ʧ��
#define ERR_MEDIA_SEND_HEAD_TO_SERVER_FAIL   2102         //��MEDIA������������Ϣͷʧ��
#define ERR_MEDIA_SEND_BODY_TO_SERVER_FAIL   2103         //��MEDIA������������Ϣ��ʧ��
#define ERR_MEDIA_RECV_HEAD_FROM_SERVER_FAIL 2104         //��MEDIA������������Ϣͷʧ��
#define ERR_MEDIA_RECV_BODY_FROM_SERVER_FAIL 2105         //��MEDIA������������Ϣ��ʧ��
#define ERR_COMM_MEDIA_RSP_FAIL              2106         //MEDIA��Ӧ�ظ�ʧ��
#define ERR_COMM_MEDIA_FAIL                  2107         //��MEDIAͨ��ʧ��
#define ERR_COMM_MEDIA_RSP_PARSE_FAIL        2108         //����MEDIA���ر��Ĵ���
#define ERR_COMM_MEDIA_OUTOF_LIMIT           2109         //������������

#define ERR_CAMERA_INDEXCODE_NULL            2201         //��ص����Ϊ��
#define ERR_DB33_REQ_PARSE_FAIL              2202         //DB33Э�飬��������xml���Ĵ���
#define ERR_DB33_DOWN_NO_REGISTER            2203         //DB33Э�飬�¼�δע��
#define ERR_DB33_DOWN_CASCADE_NOT_FIND       2204         //DB33Э�飬δ�ҵ���Ӧ���¼�����������
#define ERR_CASCADE_USERCODE_NULL            2205         //�����������û�����Ϊ��
#define ERR_CASCADE_INDEXCODE_NULL           2206         //���������������Ϊ��
#define ERR_CAMERA_SHARENODE_NOEXIST         2207         //��ص㹲����ϼ��ڵ���Ϣδ�ҵ�
#define ERR_CAMERA_WITHOUT_PRIVILEGE         2208         //�ϼ�û�и����Ȩ��
#define ERR_DB33_RSP_PARSE_FAIL              2209         //DB33Э�飬������Ӧxml���Ĵ���
#define ERR_DB33_CATALOG_PARSE_FAIL          2210         //����Ŀ¼���ͱ���xml�����
#define ERR_DB33_CATALOG_DECODE_FAIL         2211         //�����������

#define ERR_MOUDLE_MQ_INIT_FAIL              2401         //mqģ���ʼ��ʧ��

#define ERR_LOAD_MEDIA_NOEXIST               2501         //�Ҳ�����Ӧ��media
#define ERR_LOAD_MEDIA_EMPTY                 2502         //��media����
#define ERR_LOAD_MEDIA_NO_REGISTER           2503         //��mediaע��δ�ɹ�
#define ERR_LOAD_MEDIA_SESSION_NOEXIST       2504         //SessionId������

#define ERR_RES_SHARETOUP_NOTFIND            2601         //��������ϼ�����Դδ�ҵ�

#define ERR_RSP_TIMEOUT                      2701         //������Ӧ��ʱ

#define Playback					"Playback"
#define Download					"Download"
//���������������붨�� end



/**	@fn	int CASC_fClientCallBack
 *	@brief	<��Ϣ�ص�>
 *	@param sessionHandle ��ʶ����������󷵻�ֵ��ͬ.
 *	@param iDataType �������ͣ����� CASC_fClientCallBack�ص��������� iDataType.
 *	@param iErrorCode 0Ϊ����ɹ���1Ϊ��ѯ��¼���ļ�ȫ�����أ���ѯ��������������ֵΪʧ��.��������룬������������������궨�塣
 *	@param pResult ���ؽ������.
 *	@param pUsrData �û�����.
 */
typedef int (CALLBACK *CASC_fClientCallBack)(int iSessionHandle, int iDataType, int iErrorCode, char *pResult, void* pUsrData);


/**	@fn	struct CASC_REQ_INIT
 *	@brief	<CascadeClient���ʼ���ṹ�壬�������������������Ϣ>
            < �ص����������н���ɴӸûص�����, Ҳ��ѡ��Ӹ��Իص�����>
 */
struct CASC_REQ_INIT
{
	char szServerIP[32];                     /**< ����������IP��ַ                     */
	int iServerPort;                         /**< ���������������˿�                   */
	char szSvrIndexCode[32];                 /**< �������������                       */
	char szVersion[32];                      /**< �����������汾���                   */
	char szServerCode[32];                   /**< ��������������                       */
	CASC_fClientCallBack fClientCallBack;    /**< �ص����������н���Ӹûص����ء�     */
	void* pUserData;                         /**< �û�����                             */

	char szUserAgent[128];                   /**< �û������ʶ������Ҫʹ��CASC_StartRealStreamEx���½ӿ�ʱ�����ֶ��븳ֵΪ��2.3.6��*/
	char szNetZone[128];                     /**< �����ʶ                             */

	CASC_REQ_INIT()
	{
		strcpy_s(szServerIP, "127.0.0.1");
		iServerPort = 7100;
		strcpy_s(szSvrIndexCode, "11111111100000001");
		strcpy_s(szVersion, "V1.0.0");
		strcpy_s(szServerCode, "2012");
		fClientCallBack = NULL;
		pUserData = NULL;

		strcpy_s(szUserAgent, "Client");
		strcpy_s(szNetZone, "");
	}
};


/**	@fn	struct CASC_REQ_REALURL
 *	@brief	<ʵʱURL�ṹ�壬��������ͨ�������Ϣ>
            < �ص�fRealUrlCallBack������ʵʱUrl,
			  �ص���ΪNULL,ʵʱUrl�ӳ�ʼ���ṹ��CASC_REQ_INIT�е�fClientCallBack����>
 */
struct CASC_REQ_REALURL
{
	char szCamaraIndexCode[32];             /**< ���������                     */
	CASC_fClientCallBack fRealUrlCallBack;  /**< �ص�������ʵʱUrl����ΪNULL    */
    void* pUserData;                        /**< �û�����                       */   
	int iUseCastMap;                        /**< ����ӳ���ַ 0-�����ã�1-����  */
	CASC_REQ_REALURL() 
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
        fRealUrlCallBack = NULL;
		pUserData = NULL;
		iUseCastMap = 0;
	}
};

/**	@fn	struct CASC_REQ_PAGVOICEURL
 *	@brief	<�����Խ�URL�ṹ�壬��������ͨ�������Ϣ>
            < �ص�fPagVoiceUrlCallBack�����������Խ�Url,
			  �ص���ΪNULL, Url�ӳ�ʼ���ṹ��CASC_REQ_INIT�е�fClientCallBack����>
 */
struct CASC_REQ_PAGVOICEURL
{
	char szCamaraIndexCode[32];                 /**< ���������                     */
	CASC_fClientCallBack fPagVoiceUrlCallBack;  /**< �ص������������Խ�Url����ΪNULL    */
    void* pUserData;                            /**< �û�����                       */   
	CASC_REQ_PAGVOICEURL() 
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
        fPagVoiceUrlCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_CATALOGQUERY
 *	@brief	<Ŀ¼��ѯ�ṹ�壬����Ŀ¼�����Ϣ>
            < �ص�fCatalogQueryCallBack�����ز�ѯ������ɹ���ʧ��
			  �ص���ΪNULL, ��ѯ����ӳ�ʼ���ṹ��CASC_REQ_INIT�е�fClientCallBack����>
 */
struct CASC_REQ_CATALOGQUERY
{
	char szCatalogIndexCode[32];                 /**< Ŀ¼����                     */
	CASC_fClientCallBack fCatalogQueryCallBack;  /**< �ص������ز�ѯ�������ΪNULL */
    void* pUserData;                             /**< �û�����                     */   
	CASC_REQ_CATALOGQUERY() 
	{
		strcpy_s(szCatalogIndexCode, "11111111100000001");
        fCatalogQueryCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_FILELIST
 *	@brief	<��ѯ¼���ļ��ṹ�壬������ز�ѯ����>
            < �ص�fFilelistCallBack�������ļ��б�,
			  �ص���ΪNULL,�ļ��б�ӳ�ʼ���ṹ��CASC_REQ_INIT�е�fClientCallBack����>
 */
struct CASC_REQ_FILELIST
{
	char szCamaraIndexCode[32];            /**< ���������                     */
	char szBeginTime[32];                  /**< ��ʼ��ѯʱ��                   */
	char szEndTime[32];                    /**< ������ѯʱ��                   */
	int  iRecordPos;                       /**< �洢��ʽ DVR/PCNVR/NVT         */
	int  iRecordType;                      /**< ¼������                       */
	int  iFromIndex;                       /**< ��ʼ��ѯ�ļ����               */
	int  iToIndex;                         /**< ������ѯ�ļ����               */
	CASC_fClientCallBack fFilelistCallBack;/**< �ص��������ļ��б���ΪNULL   */
    void* pUserData;                       /**< �û�����                       */   
	CASC_REQ_FILELIST()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szBeginTime, "20111118T000000Z");
		strcpy_s(szEndTime, "20111118T235959Z");
		iRecordPos = 0;
		iRecordType = 0;
		iFromIndex = 1;
		iToIndex = 10;
		fFilelistCallBack = NULL;
		pUserData = NULL;
	}
};


/**	@fn	struct CASC_REQ_FILEURL
 *	@brief	<�㲥URL�ṹ�壬������ز�ѯ����>
            <�ص�fFileUrlCallBack������¼��㲥Url,
			 �ص���ΪNULL,�㲥Url�ӳ�ʼ���ṹ��CASC_REQ_INIT�е�fClientCallBack����>
 */
struct CASC_REQ_FILEURL
{
	char szCamaraIndexCode[32];               /**< ���������                     */
	char szFileName[128];                     /**< �ļ�����                       */
	char szBeginTime[32];                     /**< �ļ���ʼ�ط�ʱ��               */
	char szEndTime[32];                       /**< �ļ������ط�ʱ��               */
	int  iRecordPos;                          /**< �洢��ʽ DVR/PCNVR/NVT         */
	int  iRecordType;                         /**< ¼������                       */
	CASC_fClientCallBack fFileUrlCallBack;    /**< �ص�������¼��㲥Url����ΪNULL*/
    void* pUserData;                          /**< �û�����                       */
	int iUseCastMap;						  /**< ����ӳ���ַ 0-�����ã�1-����  */
	CASC_REQ_FILEURL()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szFileName, "ch11111111100000001");
		strcpy_s(szBeginTime, "20111118T000000Z");
		strcpy_s(szEndTime, "20111118T235959Z");
		iRecordPos = 0;
		iRecordType = 0;
        fFileUrlCallBack = NULL;
		pUserData = NULL;
		iUseCastMap = 0;
	}
};


/**	@fn	struct CASC_REQ_PTZ
 *	@brief	<��̨��������ṹ�壬������̨�������������Ϣ>
 */
struct CASC_REQ_PTZ
{
	char szCamaraIndexCode[32];           /**< ���������                      */
	int iCommand;                         /**< ��̨����������                */
	int iAction;                          /**< ��̨�������                    */
	int iSpeed;                           /**< ��̨�ٶ�                        */
	int iPriority;
	int iMatrixCameraId;
	int iMonitorId;
	int iStartX;
	int iStartY;
	int iEndX;
	int iEndY;
	int iPresetIndex;
	CASC_fClientCallBack fPTZCallBack;    /**< �ص���������̨���ƽ������ΪNULL*/
	void* pUserData;                      /**< �û�����                        */
	CASC_REQ_PTZ()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		iCommand = 21;
		iAction = 0;
		iSpeed = 4;
		iStartX = 0;
		iStartY = 0;
		iEndX = 0;
		iEndY = 0;
		iPresetIndex = 0;
		iPriority = 3;
		iMatrixCameraId = 1;
		iMonitorId = 1;
		fPTZCallBack = NULL;
		pUserData = NULL;
	}
};


/**	@fn	struct CASC_REQ_REALSTREAM
 *	@brief	<ʵʱ��Ƶ���ṹ�壬��������ͨ�������Ϣ>
 */
struct CASC_REQ_REALSTREAM
{
	char szCamaraIndexCode[32];               /**< ���������                     */
	char szLocalIP[32];                       /**< ����IP��ַ                     */
	int iRecvPort;                            /**< �����˿�                       */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< �ص�����������ɹ����         */
	void* pUserData;                          /**< �û�����                       */
    CASC_REQ_REALSTREAM()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szLocalIP, "127.0.0.1");
		iRecvPort = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_PLAYBACK
*	@brief	<�طŽṹ�壬��������ͨ�������Ϣ>
*/
struct CASC_REQ_PLAYBACK
{
	char szCamaraIndexCode[32];               /**< ���������                     */
	char szSSRC[32];							//ֱ����199999999
	char szPlayType[32];						//��Playback����Download
	char szStartTime[32];						/*vrmtime*/
	char szEndTime[32];							/*vrmtime*/
	char szLocalIP[32];                       /**< ����IP��ַ                     */
	int iRecvPort;                            /**< �����˿�                       */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< �ص�����������ɹ����         */
	void* pUserData;                          /**< �û�����                       */
	CASC_REQ_PLAYBACK()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szLocalIP, "127.0.0.1");
		strcpy_s(szSSRC, "1999999999");
		strcpy_s(szStartTime, "13311111111");
		strcpy_s(szEndTime, "13311112111");
		strcpy_s(szPlayType, Playback);
		iRecvPort = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

/**	@fn	struct CASC_REQ_PLAYBACK
*	@brief	<�طŽṹ�壬��������ͨ�������Ϣ>
*/
struct CASC_REQ_PLAYBACK_CONTROL
{
	char szCamaraIndexCode[32];               /**< ���������                                          */
	char szControlType[32];					  /**<�ָ����ź��϶�����ʱ-PLAY����ͣ-PAUSE;               */
	char szScale[32];						  /**<0.125;0.25;0.5;1;2;4;8 �ֱ�����8��4��2�Ϳ�2��4��8�ٶ�*/
	char szNpt[32];							  /**<�����϶�ʹ�ã����϶������ɲ���
											      ������CASC_StartPlayBackControlʱ�����ֶα�ʾ ��Բ��ſ�ʼʱ���������
											      ������CASC_StartPlayBackControlExʱ�����ֶα�ʾ ���Կ�ʼʱ�䣬����yyyyMMddTHHmmssZ*/
	int m_nSessionId;                         /**< playback ���ص�ID                                   */
	CASC_fClientCallBack fStreamMsgCallBack;  /**< �ص�����������ɹ����                              */
	void* pUserData;                          /**< �û�����                                            */
	CASC_REQ_PLAYBACK_CONTROL()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szControlType, "PLAY");
		strcpy_s(szScale, "1");
		strcpy_s(szNpt, "0");
		m_nSessionId = 0;
		fStreamMsgCallBack = NULL;
		pUserData = NULL;
	}
};

enum CASC_TRANS_TYPE
{
	CASC_TRANS_UDP = 0,												//UDP���䷽ʽ
	CASC_TRANS_TCP													//TCP���䷽ʽ
};

enum CASC_STREAM_TYPE
{
	CASC_STREAM_MAIN = 0,											//������
	CASC_STREAM_SUB													//������
};

typedef int (CALLBACK * CASCMsgCB)(int iSessionHandle, int iMsgType, int iMsgCode, char *pResult, void* pUsrData);

typedef int (CALLBACK * CASCStreamCB)(int iSessionHandle, int iDataType, char* pData, int iDataLen, void* pUserData);

struct CASC_REALSTREAM_PARA
{
	char szCamaraIndexCode[32];                                //���������    
	char szRecvAddr[32];              	                       //���յ�ַ UDPʱʹ��
	char szDecoderTag[32];                                     //���������ͣ�������hikvision���󻪣�dahua��

	int iStreamType;						                   //����������CASC_STREAM_TYPE 
	int iTransMethod;						                   //����Э�飬CASC_TRANS_TYPE 
	int iPort;							                       //�ӿڶ˿� UDPʱʹ��  
	void* hwnd;                                                //���ھ��  
	CASCMsgCB pMsgCB;							               //��Ϣ�ص�       
	CASCStreamCB pStreamCB;						               //�����ص�      
	void* pUserData;                                           //�ص��û�����
	CASC_REALSTREAM_PARA()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szRecvAddr, "127.0.0.1");
		strcpy_s(szDecoderTag, "hikvision");
		iStreamType = CASC_STREAM_MAIN;
		iTransMethod = CASC_TRANS_UDP;
		iPort = 0;
		hwnd = NULL;
		pMsgCB = NULL;
		pStreamCB = NULL;
		pUserData = NULL;
	}
};

struct CASC_PLAYBACKSTREAM_PARA
{
	char szCamaraIndexCode[32];            /**< ���������                */
	char szDecoderTag[32];                 /**< ���������ͣ�������hikvision���󻪣�dahua��*/
	char szBeginTime[32];                  /**< ��ʼ��ѯʱ��              */
	char szEndTime[32];                    /**< ������ѯʱ��              */
	CASCMsgCB pMsgCB;					   /**< ��Ϣ�ص�                  */
	CASCStreamCB pStreamCB;				   /**< �����ص�                  */
	void* pUserData;				       /**< �û�����                  */
	int iTransMethod;				       /**< ����Э��                  */
	char szRecvAddr[16];                   /**< ���յ�ַ UDPʱʹ��        */
	int iPort;						       /**< �ӿڶ˿� UDPʱʹ��        */
	int iDownload;                         /**< 0ΪPlayback��1ΪDownload  */
	char szFileName[128];                  /**< �ط��ļ���                */
	void* hwnd;                            /**< ���ھ��                  */
	int  iRecordPos;                       /**< �洢��ʽ DVR/PCNVR/NVT    */
	int  iRecordType;                      /**< ¼������                  */
	CASC_PLAYBACKSTREAM_PARA()
	{
		strcpy_s(szCamaraIndexCode, "11111111100000001");
		strcpy_s(szDecoderTag, "hikvision");
		//ʱ���ʽ��ʱ��ΪyyyyMMddTHHmmssZ
		strcpy_s(szBeginTime, "20130907T101112Z");
		strcpy_s(szEndTime, "20130907T101112Z");
		pMsgCB = NULL;
		pStreamCB = NULL;
		pUserData = NULL;
		iTransMethod = CASC_TRANS_UDP;
		strcpy_s(szRecvAddr, "127.0.0.1");
		iPort = 0;
		iDownload = 0;
		strcpy_s(szFileName, "");
		hwnd = NULL;
		iRecordPos = 0;
		iRecordType = 0;
	}
};

#endif