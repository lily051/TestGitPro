/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       StreamLayerAPI.h
*   Description:    a audio/video stream layer from RSMS/Device
*   Author:         JiangTianjing(jiangtj@hikvision.com)
*   Date:           2008-10-06 Monday
*   Modification History:      
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-06      JiangTianjing   created
**************************************************************************/


#ifndef __STREAM_LAYER_API__
#define __STREAM_LAYER_API__

#ifdef STREAM_LAYER_API_EXPORTS
#define STREAM_LAYER_API __declspec(dllexport)
#else
#define STREAM_LAYER_API __declspec(dllimport)
#endif

#define MAX_USERNAME_LEN_8100    64
#define MAX_PASSWORD_LEN    64
#define MAX_DEVICE_LEN      128
#define DESCRIBE_LEN_32     32

typedef enum _Enum_DataLen
{
    SL_LEN_IPv4         = 16,   ///< IP��ַ����
    SL_LEN_VAG_USER     = 32,   ///< VAG��½������
    SL_LEN_VAG_PWD      = 32    ///< VAG��½���볤��
};

typedef enum _enumConnectType
{
    CONNECT_DEVICE = 0,         // ֱ��
    CONNECT_RSM = 1,            // ����ý��
    CONNECT_CLOUD = 2,          // ���ƴ洢
    CONNECT_NCG28181_GET = 3,   // ����������NCG28181 ȡ��ģʽ
    CONNECT_NCG28181_PUSH = 4,  // ����������NCG28181 ����ģʽ
    CONNECT_VAG = 5             // ��VAG
};

typedef enum _enumDevType
{
	SL_DEV_HIK = 0,			    //��������
	SL_DEV_DAHUA,			    //��
	SL_DEV_HIK_3G,			    //����3G
    SL_DEV_BLUESKY_LIMIT = 441, //����limit
    SL_DEV_ONVIF = 584,         //Onvif��׼Э��
    SL_DEV_PAG = 1005           //vag
}DEVTYPE;

typedef struct tagStreamDataInfo
{
    int nStreamType;			//0��ʾ����Ϊ���ʽ��1��ʾ����Ϊ֡���ݸ�ʽ,Ŀǰ����0
    char * pBuff;				//���ݻ����ַ
    int nDataLen;				//���ݳ���
	int	nPacketType;			//���������� 0��ͷ����
    
	// global time
    int	nYear;					// year	
    int	nMonth;					// month
    int	nDay;					// day
    int	nHour;					// hour
    int	nMinute;				// minute
    int	nSecond;				// second
    
	// relatively timestamp
    DWORD dwTimeStamp;					// relatively timestamp
	int   nFrame_DALI;					//֡�� ʹ�ó��ң�����
	int   nResolution_DALI;				//�ֱ��� ʹ�ó��ң�����
}STREAM_DATA_INFO, *PSTREAM_DATA_INFO;

// ʵʱ�����ݻص�����,
typedef void (CALLBACK * SLStreamCallback)(HANDLE hStream, HANDLE hUserData, STREAM_DATA_INFO * pStreamDataInfo);


//ȡ��������Ϣ
#define E_CONNECT_FAIL			0	//����ʧ��ʱ���ش���Ϣ
#define E_NETWORK_EXCEPTION		1	//���ӳɹ���û�������ش���Ϣ
#define E_RECONNECT_FAIL		2	//����ʧ���Ƿ��ش���Ϣ
#define E_RECONNECT_OK			3	//�����ɹ�
#define E_RECONNECT_OVER		4	//����������ʼ���������᷵�ش���Ϣ��
#define E_CONNECT_OK            5	//���ӳɹ�

// int nMsgID: 0��ʾhStream��Ӧ��ͨ����Ϣֵ�������nMsgID�궨��
typedef void (CALLBACK * SLMsgNotify)(HANDLE hStream, HANDLE hUserData, int nMsgID);

// �豸������Ϣ
typedef struct tagDeviceParam
{
    char szName[MAX_DEVICE_LEN];                // �豸ip��ַ�ַ����������ַ���
    char szUsername[MAX_USERNAME_LEN_8100];	    // �豸�û�����
                                                // ����ǡ���������NCG28181������ʾ�����ServerIndexCode
    char szPassword[MAX_DEVICE_LEN];	        // �豸�û�����
    int nPort;                                  // �豸�˿�
    int nType;                                  // 0-��ʾszName��Ӧ����IP��ʽ�ַ���, 1-��ʾszName��Ӧ����������DNS������
	int nDeviceType;                            // �豸����
	char szDeviceId[MAX_DEVICE_LEN];            // ����ǡ�3G�豸������ʾ����3G�豸���������IP���豸Ψһ��ʶID
	                                            // ����ǡ���������NCG28181������ʾ��ص�Code����
                                                // ����ǡ�VAG������ʾ��ص�strNo����
	char szAccessSrvIP[32];	                    // ���������IP
	unsigned short int	nAccessSrvPort;         // ����������˿�,Ĭ��7660
    int nDataPort;                              // ���ݶ˿�

    tagDeviceParam()
    {
        memset(szUsername, 0, sizeof(szUsername));
        memset(szPassword, 0, sizeof(szPassword));
        memset(szAccessSrvIP, 0, sizeof(szAccessSrvIP));
        memset(szName, 0, sizeof(szName));
        memset(szDeviceId, 0, sizeof(szDeviceId));
        nPort = -1;
        nType = 0;
        nDataPort = 0;
        nDeviceType = 0;
        nAccessSrvPort = 0;
    }

}DEVICE_PARAM,*PDEVICE_PARAM;

// ʵʱ��ת��·����Ϣ
#define MAX_TRANSMIT_COUNT	16	//���ת������
typedef struct tagTransmitPath
{
    int nTransmit;      // ��ǰת���Ĵ���
    char szSrvIp[MAX_TRANSMIT_COUNT][16];   // ÿ�ξ���ת������ý���������ip��ַ�ַ���
    int	nPort[MAX_TRANSMIT_COUNT];         // 0-��ʾĬ�϶˿ں�
    tagTransmitPath()
    {
        nTransmit = 0;
        for (int i = 0; i < MAX_TRANSMIT_COUNT; i++)
        {
            memset(szSrvIp[i], 0, 16*sizeof(char));
            nPort[i] = 27070;
        }
    }
}TRANSMIT_PATH,*PTRANSMIT_PATH;

// ʵʱ�������ṹ
typedef struct tagStreamParams
{
    DEVICE_PARAM device;        // �豸ע����Ϣ
    int nType;                  // ��CONNECT_DEVICE��-0��ʾ��ʾֱ���� 
                                // ��CONNECT_RSM��-1��ʾ����ý��ת����·����
                                // ��CONNECT_CLOUD��-2��ʾ���ƶ�ȡ��
                                // ��CONNECT_NCG28181_GET��-3��ʾ����������NCG28181ȡ����ʽ
                                // ��CONNECT_NCG28181_PUSH��-4��ʾ����������NCG28181������ʽ
                                // ��CONNECT_VAG��-5��ʾ��VAGȡ��
    TRANSMIT_PATH path;         // �� nTypeΪ0ʱ������ý���������ȡ����
    int nChannel;               // �豸ͨ��
    int	nSubChannel;            // ��nSubChannelΪ0ʱ��ʾ��������1��ʾ������
    int nMode;		            // 0-��TCP��, 1-��UDP��, ��nTypeΪ0��ʾ��ʾֱ��ʱ2-��MULTICAST��.
	char szMultiCastIP[16];	    // �ಥ���ַ����nModeΪ2ʱ��Ч
    HANDLE hUserData;           // �û���������ݣ��ڻص������������ܹ����	
	char szProxyServerIP[16];   // ����ǡ����������������ʾ�����������IP
                                // ����ǡ�����3G�豸��,��ʾ3G��ý���������IP�Ͷ˿�
                                // ����ǡ���������NCG28181��,��ʾ��������IP
                                // ����ǡ�VAG������ʾVAG��IP
	int  nProxyServerPort;      // ����ǡ��������������Ĭ��Ϊ7554
                                // ����ǡ���������NCG28181������ʾ�������ط����Port
                                // ����ǡ�VAG������ʾVAG��PORT
    char szVAGUser[SL_LEN_VAG_USER];    // VAG��½�û���
    char szVAGPwd[SL_LEN_VAG_PWD];      // VAG��½����
    tagStreamParams()
    {
        nType = 0;
        nChannel = 0;
        nSubChannel = 0;
        nMode = 0;
        nProxyServerPort = 7554;
        hUserData = NULL;
        memset(szProxyServerIP, 0, 16*sizeof(char));
        memset(szMultiCastIP, 0, 16*sizeof(char));
        memset(szVAGUser, 0, sizeof(char)*SL_LEN_VAG_USER);
        memset(szVAGPwd, 0, sizeof(char)*SL_LEN_VAG_PWD);
    }
}STREAMPARAMS,*PSTREAMPARAMS;

typedef struct tagStartupInfo
{
	int nTryTimes;			//�����Ͽ�ʱ��>0��ʾ���������Ĵ����� 0��ʾһֱ����
	int nTimeout;			//ִ������ʱ�䣨��λs������Ч��Χ(1~60), 
							//ֻ�е�û���������ۻ�ʱ��ﵽ������ʱ��ʱ����ִ����������.
	BOOL bInitHcSdk;		//�Ƿ��ʼ��hcnetsdk
    
	tagStartupInfo()
    {
        nTryTimes = 1;
        nTimeout = 10;
        bInitHcSdk = TRUE;
    }
}STARTUP_INFO, *PSTARTUP_INFO;

// ��̨��������ṹ
typedef struct tagPTZCommand
{
	int nCmdID;
	int nPara0;
	int nPara1;
	int nPara2;
	int nPara3;
}SL_PTZ_COMMAND, *PSL_PTZ_COMMAND;

// ��Ƶѹ�������ṹ
typedef struct tagCompressionInfo
{
    int nCompressType;          // ѹ������ 0������ 1������
    int nStreamType;            // ��������
    int nResolution;            // �ֱ���
    int nBitrateType;           // ��������
    int nPicQuality;            // ͼ������
    int nVideoBitrate;          // ��Ƶ����
    int nVideoFrameRate;        // ��Ƶ֡��
    int nVideoEncType;          // ��Ƶ��������
    char szResolution[DESCRIBE_LEN_32];         // �ֱ�������
    char szVideoBitrate[DESCRIBE_LEN_32];       // ��Ƶ��������
    char szVideoFrameRate[DESCRIBE_LEN_32];     // ��Ƶ֡������
    tagCompressionInfo()
    {
        nCompressType = 0;
        nStreamType = -1;
        nResolution = -1;
        nBitrateType = -1;
        nPicQuality = -1;
        nVideoBitrate = -1;
        nVideoFrameRate = -1;
        nVideoEncType = -1;
        szResolution[0] = '\0';
        szVideoBitrate[0] = '\0';
        szVideoFrameRate[0] = '\0';
    }
}SL_Compression_Info;

// ��̨ͼ������λ����Ϣ
typedef struct tagPTZPointFrame
{
    int nXtop;          // ������ʼ���x����
    int nYtop;          // ������ʼ���y����
    int nXbottom;       // ����������x����
    int nYbottom;       // ����������y����
    int nCounter;       // ������1- ���ϣ�2- ���ϣ�3- ���£�4- ����
    tagPTZPointFrame()
    {
        nXtop = 0;
        nYtop = 0;
        nXbottom = 0;
        nYbottom = 0;
        nCounter = 0;
    }
}SL_PTZ_POINT_FRAME;

#ifdef __cplusplus
extern "C" {
#endif

    //
    /**************************************************************************
    *   Function:           SLStartup
    *   Description:        initialize the API library
    *   Input:              STARTUP_INFO * pStartupInfo: the param needed in initialization                  
    *   Output:             (null)
    *   Return:             return TRUE if successful; otherwise FALSE.
    **************************************************************************/
    STREAM_LAYER_API BOOL SLStartup(STARTUP_INFO * pStartupInfo);
    
    
    /**************************************************************************
    *   Function:           SLCleanup
    *   Description:        �����������������Դ�����ͷ�
    *   Input:              (null)
    *   Output:             (null)
    *   Return:             void
    **************************************************************************/
    STREAM_LAYER_API void SLCleanup();
    
    /**************************************************************************
    *   Function:           SLStartStream
    *   Description:        �����豸����ý���������ȡ��������
    *   Input:              STREAMPARAMS * pStreamParam: ȡ��������Ϣ��������ṹ����
    *                       SLStreamCallback pfnStream: �����ݻص�����ָ��
    *   Output:             (null)
    *   Return:             �������ɹ����򷵻ط���ľ����ʧ�ܷ���NULL
	*   Modify:				qmx 2009-12-2 ��Ӳ������ڴ��������
    **************************************************************************/
    STREAM_LAYER_API HANDLE SLStartStream(STREAMPARAMS * pStreamParam, SLStreamCallback pfnStream, SLMsgNotify msgNotify = NULL);

    /**************************************************************************
    *   Function:           SLStopStream
    *   Description:        ֹͣ�������ݻ�ȡ
    *   Input:              HANDLE hStream: SLStartStream�򿪳ɹ����صľ��
    *   Output:             (null)
    *   Return:             ʧ�ܷ���FALSE���ɹ�����TRUE.
    **************************************************************************/
    STREAM_LAYER_API BOOL SLStopStream(HANDLE hStream);

	/*************************************************************************
	*   Function��			slSetVideoPara 
	*   Description��		�����豸ͨ������Ƶ����
	*   Input��                HANDLE hStream: SLStartStream���صľ��
	*                       nBValue: brightness ����(ȡֵΪ 1-10) 
	*                       nCValue: contrast �Աȶ�(ȡֵΪ 1-10) 
	*                       nSValue: Saturation ���Ͷ�(ȡֵΪ 1-10) 
	*                       nHValue: Hue ɫ��(ȡֵΪ 1-10) 
	*   Output:             (null)
	*   Return:				TRUE ��ʾ�ɹ���FALSE ��ʾʧ�ܡ� 
	*************************************************************************/
	STREAM_LAYER_API BOOL slSetVideoPara(HANDLE hStream, int nBValue, int nCValue, int nSValue, int nHValue);

	/*************************************************************************
	*   Function��			slGetVideoPara 
	*   Description��		�����豸ͨ������Ƶ����
	*   Input��                HANDLE hStream: SLStartStream���صľ��                
	*   Output:				pBValue: �������ֵ��ָ��(ȡֵΪ 1-10) 
	*                       pCValue: ��ŶԱȶ�ֵ��ָ��(ȡֵΪ 1-10) 
	*                       pSValue: ��ű��Ͷ�ֵ��ָ��(ȡֵΪ 1-10) 
	*                       pHValue: ���ɫ��ֵ��ָ��(ȡֵΪ 1-10) 
	*   Return:				TRUE ��ʾ�ɹ���FALSE ��ʾʧ�ܡ� 
	*************************************************************************/
	STREAM_LAYER_API BOOL slGetVideoPara(HANDLE hStream, int * pBValue, int * pCValue, int * pSValue, int * pHValue);


	/*************************************************************************
	*   Function��			slForceKeyFrame 
	*   Description��		��ȡ��������ǿ������I֡
	*   Input��                HANDLE hStream: SLStartStream���صľ��                
	*   Output:				(null)
	*   Return:				TRUE ��ʾ�ɹ���FALSE ��ʾʧ�ܡ� 
	*************************************************************************/
	STREAM_LAYER_API BOOL slForceKeyFrame(HANDLE hStream);

	/*************************************************************************
	*   Function��			slPTZControl 
	*   Description��		���Ѿ�ʵʱԤ����ͼ�������̨����
	*   Input��                HANDLE hStream: SLStartStream���صľ��                
	*   Output:				(null)
	*   Return:				TRUE ��ʾ�ɹ���FALSE ��ʾʧ�ܡ� 
	*************************************************************************/
	STREAM_LAYER_API BOOL slPTZControl(HANDLE hStream, SL_PTZ_COMMAND * pPTZ);

    /** @fn         SLGetCompresssionInfo
     *  @brief      ��ȡ��ǰԤ��ͨ����ѹ��������Ϣ
     *  @param[IN]  hStream: SLStartStream���صľ��
     *  @param[OUT] stCompressionInfo: ѹ��������Ϣ
     *  @return     TRUE: �ɹ�, FALSE: ʧ��
     */
    STREAM_LAYER_API BOOL SLGetCompresssionInfo(HANDLE hStream, SL_Compression_Info &stCompressionInfo);

    /** @fn         SLPTZSelZoomInEx
     *  @brief      ��̨ͼ������ѡ��Ŵ����С(��3D��λ����)
     *  @param[IN]  hStream: SLStartStream���صľ��
     *  @param[IN]  stPTZPointFrame: ��̨ͼ������λ����Ϣ
     *  @return     TRUE: �ɹ�, FALSE: ʧ��
     */
    STREAM_LAYER_API BOOL SLPTZSelZoomInEx(HANDLE hStream, const SL_PTZ_POINT_FRAME &stPTZPointFrame);

    /** @fn         SLStartSteam_V30
    *  @brief       ��������iVMS-8100 v3.0��ȡ���ӿ�
    *  @param[IN]   const char* pMsg: ��ص���Ϣ
    *  @param[IN]   int nMsgSize: ��ص���Ϣ����
    *  @param[IN]   pfnStream: �����ݻص�����
    *  @param[IN]   msgNotify: ��Ϣ�ص�
    *  @param[IN]   hUserData: �û��Զ�������
    *  @param[OUT]  int& nPlayType: ��������
    *  @return      ����ȡ����������ʧ�ܷ���NULL
    */
    STREAM_LAYER_API HANDLE SLStartStream_V30(const char* pMsg, int nMsgSize, int& nPlayType, SLStreamCallback pfnStream, SLMsgNotify msgNotify, void *hUserData);

    /** @fn         SLStartSteam_OCX
    *  @brief       ��������iVMS-8100 v3.0 OCX�ؼ�ʹ�õ�ȡ���ӿ�
    *  @param[IN]   LPCTSTR szMsg: ��ص���Ϣ,xml��ʽ
    *  @param[IN]   pfnStream: �����ݻص�����
    *  @param[IN]   msgNotify: ��Ϣ�ص�
    *  @param[IN]   hUserData: �û��Զ�������
    *  @param[OUT]  int& nPlayType: ��������
    *  @return      ����ȡ����������ʧ�ܷ���NULL
    */
    STREAM_LAYER_API HANDLE SLStartStream_OCX(const char* pMsg, int& nPlayType, SLStreamCallback pfnStream, SLMsgNotify msgNotify, void *hUserData);

    /** @fn         SLZeroTurnOver
     *  @brief      ��ͨ��Ԥ�����淭ҳ
     *  @param[IN]  hStream: SLStartStream���صľ��
     *  @param[IN]  bNextPreview: ���»����Ϸ�ҳ��TRUE-��һҳ��FALSE-��һҳ
     *  @return     TRUE: �ɹ�, FALSE: ʧ��
     */
    STREAM_LAYER_API BOOL SLZeroTurnOver(HANDLE hStream, BOOL bNextPreview);

    STREAM_LAYER_API void SLStreamInfo(const char* pMsg, int nMsgSize, char* szServerInfo, int nServerInfoSize, char* szStreamDesc, int nDescSize);
#ifdef __cplusplus
}
#endif

#endif // __STREAM_LAYER_API__
