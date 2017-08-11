/**   @file PlaybackSimpleAPI.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	�򵥻ط����.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/12/19
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */
//lint -library
#ifndef __PBSD_API_H__
#define __PBSD_API_H__


#ifdef  PBSD_API_EXPORT
#define PBSD_API __declspec(dllexport)
#else
#define PBSD_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif


// Ȩ�޿���.
#define PBSD_RIGHT_ALL 0xFF			// ����Ȩ��.
#define PBSD_RIGHT_DOWNLOAD 0x01	// ����Ȩ��
#define PBSD_RIGHT_CAPTURE 0x02		// ץͼȨ��.
#define PBSD_RIGHT_SAVE 0x04		// ��������Ȩ��.

// ������.
#define PBSD_STYLE_SIMPLE 0x00		// �����.
#define PBSD_STYLE_COMPLETE 0xFF	// �������.
#define PBSD_STYLE_DOWNLOAD 0x01	// ����ģ��.
#define PBSD_STYLE_LOCALPLAYBACK 0x02	// ���ػط�.

// Ƥ������.
typedef enum _PBSDSkinType
{
	PBSD_SKIN_RED = 0,		// ��ɫ.
	PBSD_SKIN_BLUE = 1,		// ��ɫ.
}PBSDSkinType;

// ����ָ�ģʽ.
typedef enum _PBSDLayout
{
	PBSD_LAYOUT_WND_1 = 1,                     //һ�������
	PBSD_LAYOUT_WND_4 = 4,                     //�Ļ������
	PBSD_LAYOUT_WND_9 = 9,                     //�Ż������
	PBSD_LAYOUT_WND_16= 16,                    //ʮ���������
}PBSDLayout;

typedef enum _PBSDPlayType
{
	PBSD_TYPE_DVR = 0,
	PBSD_TYPE_CVR,					
	PBSD_TYPE_CLOUD,
    PBSD_TYPE_KMS                       
}PBSDPlayType;

//¼������
typedef enum _PBSDRecordType
{
	PBSD_RECORD_TYPE_ALL = 0xFF,            //ȫ��
	PBSD_RECORD_TYPE_TIMING = 0,            //��ʱ¼�� 
	PBSD_RECORD_TYPE_MOVE	= 1,           //�ƶ����
	PBSD_RECORD_TYPE_ALARM_TRIGER = 2,      //��������
	PBSD_RECORD_TYPE_ALARM_OR_MOVE = 3,     //����|�ƶ����	
	PBSD_RECORD_TYPE_ALARM_AND_MOVE = 4,    //����&�ƶ����
	PBSD_RECORD_TYPE_CMD_TRIGER	= 5,       //�����
	PBSD_RECORD_TYPE_MANUAL_RECORD = 6,     //�ֶ�¼��
}PBSDRecordType;

// ������ƿͻ�������.
typedef enum _EnumBwClientType
{
	PBSD_BW_CLIENT_TYPE_CS = 0,				// CS�ͻ���.
	PBSD_BW_CLIENT_TYPE_BS,					// BS�ͻ���.
}EnumBwClientType;

//�طŲ�����Ϣ
typedef struct _stPlaybackParam
{
	char szDownloadPath[MAX_PATH];		// ����·��.
	char szPicPath[MAX_PATH];		// ͼƬ����·��.
	int nPicType;					// ͼƬ����,0-BMP, 1-JPG.
	int nShowFileWnd;				// ����.
	int nShowWndTitle;				// �Ƿ���ʾ������.
	_stPlaybackParam()
	{
        ZeroMemory(szDownloadPath, sizeof(szDownloadPath));
        ZeroMemory(szPicPath, sizeof(szPicPath));
		nPicType = 0;
		nShowFileWnd = 0;
		nShowWndTitle = 1;
	}
}PLAYBACK_PARAM;

//�ط�������Ϣ
typedef struct _stPlaybackConfig
{
	PLAYBACK_PARAM stPBParam;
	// �Ƿ��ʼ������.
	BOOL bInitProx;
	// �Ƿ��ʼ��PCNVR.
	BOOL bInitNvr;
	// �Ƿ��ʼ��bdp.
	BOOL bInitBdp;
	// �Ƿ��ʼ��FinAudio.
	BOOL bInitFinAudio;

	_stPlaybackConfig()
	{
		bInitProx = TRUE;
		bInitNvr  = TRUE;
		bInitBdp  = TRUE;
		bInitFinAudio = TRUE;
	}
}PLAYBACK_CONFIG;

//dvr/cvr��¼��Ϣ
typedef struct _PBSDDvrLoginInfo
{
	char szIP[16];                     //ip
	int nPort;                         //�˿�
	int nDataPort;                     //���ݶ˿�
	char szUserName[32];               //�û���
	char szPassword[32];               //����
	char szIndex[32];                   //�ƴ洢Ψһ��ʾ
	char szRev[160];                    //Ԥ��
    _PBSDDvrLoginInfo()
    {
        ZeroMemory(szIP, sizeof(szIP));
        nPort = 0;
        nDataPort = 0;
        ZeroMemory(szUserName, sizeof(szUserName));
        ZeroMemory(szPassword, sizeof(szPassword));
		ZeroMemory(szIndex, sizeof(szIndex));
        ZeroMemory(szRev, sizeof(szRev));
    }
}PBSDDvrLoginInfo;

//ʱ��ṹ��
typedef struct _PBSDTime
{
	DWORD	dwYear;			           //��
	DWORD	dwMonth;		           //��
	DWORD	dwDay;			           //��
	DWORD	dwHour;			           //ʱ
	DWORD	dwMinute;		           //��
	DWORD	dwSecond;		           //��

    _PBSDTime()
    {
        dwYear = 1970;
        dwMonth = 1;
        dwDay = 1;
        dwHour = 0;
        dwMinute = 0;
        dwSecond = 0;
    }
}PBSDTime,*LPPBSDTime;

// ��¼������Ϣ.
typedef struct _PBSDLoginTypeInfo
{
	PBSDDvrLoginInfo stDvrLoginInfo;			// DVR��CVR��½��Ϣ.
}PBSDLoginTypeInfo;

// ��¼����.
typedef struct _PBSDLoginParam
{
	PBSDPlayType emPlayType;         //�ط�����.
	int nManType;					 // ��������.
	int nDevType;					 //�豸����
	PBSDLoginTypeInfo unLoginType;
	_PBSDLoginParam()
	{
		emPlayType = PBSD_TYPE_DVR;
		nDevType = 0;
		nManType = 0;
	}
}PBSDLoginParam, *LPPBSDLoginParam;

// ��Ա�Ų���.
typedef struct _TelNoInfo
{
	char szTelNo[32];
	int nRev;
	char szRev[32];

    _TelNoInfo()
    {
        ZeroMemory(szTelNo, sizeof(szTelNo));
        nRev = 0;
        ZeroMemory(szRev, sizeof(szRev));
    }
}TelNoInfo, *LPTelNoInfo;

// ʱ��طŲ���.
typedef struct _PBSDTimeParam
{
	int nCamID;			         // ��ص�ID.
	char szCamName[128];         // ��ص�����.
	int nChannel;		         // ͨ����
	PBSDTime startTime;          // ��ʼʱ��
	PBSDTime stopTime;           // ����ʱ��
	PBSDRecordType emFileType;   // ��ѯ�ļ�����.
	BOOL bByCardNum;	         // �Ƿ񰴿��Ų�ѯ.
	char szCardNum[32];          // ����.
	char szStreamID[32];         // ��ID��ֻ��CVR��Ч.
	DWORD dwRev;                 // Ԥ��

    _PBSDTimeParam()
    {
        nCamID = 0;
        ZeroMemory(szCamName, sizeof(szCamName));
        nChannel = 0;
        bByCardNum = FALSE;
        ZeroMemory(szCardNum, sizeof(szCardNum));
		ZeroMemory(szStreamID, sizeof(szStreamID));
		dwRev = 0;
    }
}PBSDTimeParam, *LPPBSDTimeParam;

// ��Ա�ط�ʱ�����.
typedef struct _PBSDTelTimeParam
{
	PBSDTime startTime;		// ��ʼʱ��.
	PBSDTime stopTime;		// ����ʱ��.
	PBSDRecordType emFileType; // ��ѯ�ļ�����.
	DWORD dwRev;			// Ԥ���ֶ�.

    _PBSDTelTimeParam()
    {
        dwRev = 0;
    }
}PBSDTelTimeParam, *LPPBSDTelTimeParam;

// �ļ�����.
typedef struct _PBSDFileData
{
	char szFileName[128];               //�ļ���
	DWORD dwFileSize;                   //�ļ���С
	int nFileType;                      //�ļ�����
	PBSDTime stStartTime;                //��ʼʱ��
	PBSDTime stEndTime;                  //����ʱ��
	PBSDPlayType emPlayType;            //�ļ��洢Դ
	int nDevType;         //�豸����
    char szPlayUrl[255];                //�ط�url--��vrm�ط���Ҫ
	char szRev[32];                     //Ԥ��

    _PBSDFileData()
    {
        ZeroMemory(szFileName, sizeof(szFileName));
        dwFileSize = 0;
        nFileType = 0;
        nDevType = 0;
        ZeroMemory(szPlayUrl, sizeof(szPlayUrl));
        ZeroMemory(szRev, sizeof(szRev));
    }
}PBSDFileData, *LPPBSDFileData;

// �ļ��طŲ���.
typedef struct _PBSDFileParam
{
	int nCamID;				// ��ص�ID.
	DWORD dwRev;			// Ԥ���ֶ�.
	char szCamName[128];    // ��ص�����.
	int nChannel;			// ͨ����.
	char szStreamID[32];    // ��ID.
	PBSDFileData stFileData;	// �ļ�����.

    _PBSDFileParam()
    {
        nCamID = 0;
        dwRev = 0;
        ZeroMemory(szCamName, sizeof(szCamName));
		ZeroMemory(szStreamID, sizeof(szStreamID));
        nChannel = 0;
    }
}PBSDFileParam, *LPPBSDFileParam;

// ��Ա�ط��ļ�����.
typedef struct _PBSDTelFileParam
{
	PBSDFileData stFileData;	// �ļ�����.
	DWORD dwRev;				// Ԥ���ֶ�.

    _PBSDTelFileParam()
    {
        dwRev = 0;
    }
}PBSDTelFileParam, *LPPBSDTelFileParam;

// �طŲ���������Ϣ.
typedef struct _PlayTypeParam
{
	PBSDTimeParam stTimePram;			// ʱ��طŲ���.
	PBSDFileParam stFileParam;			// �ļ��طŲ���.
}PlayTypeParam, *LPPlayTypeParam;

// ��Ա�طŲ���������Ϣ.
typedef struct _PlayTypeTelParam
{
	PBSDTelTimeParam stTimePram;		// ʱ��طŲ���.
	PBSDTelFileParam stFileParam;			// �ļ��طŲ���.
}PlayTypeTelParam, *LPPlayTypeTelParam;

// �طŲ���.
typedef struct _PBSDPlayParam
{
	int nType;								// �طŷ�ʽ,0-��ʱ��,1-���ļ�.
	LONG lBwControl;                        // �Ƿ�֧�ִ������
	PlayTypeParam unPlayParam;

    _PBSDPlayParam()
    {
        nType = 0;
		lBwControl = 0;
    }
}PBSDPlayParam, *LPPBSDPlayParam;

// ��Ա�ŻطŲ���.
typedef struct _PBSDTelPlayParam
{
	int nType;								// �طŷ�ʽ��0-��ʱ�䣬1-���ļ�.
	PlayTypeTelParam unPlayParam;	

    _PBSDTelPlayParam()
    {
        nType = 0;
    }
}PBSDTelPlayParam, *LPPBSDTelPlayParam;

// �����ļ�����.
typedef struct _PBSDFindParam
{
	PBSDRecordType emFileType;							// �ļ�����.
	PBSDTimeParam stTimeParam;				// ʱ�����.
}PBSDFindParam, *LPPBSDFindParam;

// �˵��ص�����.
typedef struct _MenuCBData
{
	PBSDLoginParam stLogin;					// ��¼����.
	LPVOID pInfo;							// ��ز���.

    _MenuCBData()
    {
        pInfo = NULL;
    }
}MenuCBData, *LPMenuCBData;

// �¼���ǩ��Ϣ.
typedef struct _EventLabelData
{
	int nCamID;								// ��ص�ID.
	char szFileName[128];					// �ļ���.
	PBSDTime stStartTime;					// ��ʼʱ��.
	PBSDTime stStopTime;					// ����ʱ��.

    _EventLabelData()
    {
        nCamID = 0;
        ZeroMemory(szFileName, sizeof(szFileName));
    }
}EventLabelData, *LPEventLabelData;

// �ط�״̬��Ϣ.
typedef struct _PlaybackStatData
{
	BOOL bIsPlaying;						// �Ƿ����ڻط���.
	BOOL bIsDownloading;					// �Ƿ�����������.
	BOOL bIsSaving;							// �Ƿ����ڼ�����.
	int nCamID;								// ��ص�ID.
	DWORD dwRev;							// �����ֶ�.

    _PlaybackStatData()
    {
        bIsPlaying = FALSE;
        bIsDownloading = FALSE;
        bIsSaving = FALSE;
        nCamID = 0;
        dwRev = 0;
    }
}PlaybackStatData, *LPPlaybackStatData;

// ��־��Ϣ�ṹ��.
typedef struct _WorkLogData
{
	int nCamID;			// ��ص�ID.
	DWORD dwRev;		// �����ֶΣ�8120�ô��ֶ���Ψһ��ʶ,���ϲ㴫��ļ���ID��������ԭ������.
	DWORD dwLogType;	// ��־����.626012-�طſ�ʼ, 626013-�ط�ֹͣ, 626002-�ط�ץͼ, 626016-������ʼ, 626017-����ֹͣ, 626014-���ؿ�ʼ, 626015-����ֹͣ
	int nResult;		// ����� 0-ʧ�ܣ�1-�ɹ�.
	char szRemark[512];	// ��ע.
	DWORD dwParamUp;	// dwLogTypeΪ626014����626015ʱ��Ч����ֵΪ¼��ʼʱ��.
	DWORD dwParamLow;	// dwLogTypeΪ626014����626015ʱ��Ч����ֵΪ¼�����ʱ��.
	DWORD dwRet;		// dwLogTypeΪ626015ʱ��Ч,1-���سɹ�,2-����ʧ��.

    _WorkLogData()
    {
        nCamID = 0;
        dwRev = 0;
        dwLogType = 0;
        nResult = 0;
        ZeroMemory(szRemark, sizeof(szRemark));
        dwParamUp = 0;
        dwParamLow = 0;
        dwRet = 0;
    }
}WorkLogData, *LPWorkLogData;

//���������������Ϣ
typedef struct _PBSDUserInfo
{
	char szLoginUser[32];     //�ͻ��˵�¼�û�
	int nUserID;               //�û�ID
	int nUserLevel;            //�û�����
	int nRoleID;               //�ͻ��˵�¼�û���ɫID
	_PBSDUserInfo()
	{
		ZeroMemory(szLoginUser, sizeof(szLoginUser));
		nUserID = -1;
		nUserLevel = 0;
		nRoleID = -1;
	}
}PBSDUserInfo, *LPPBSDUserInfo;

// CMS����.
typedef struct _PBSDCmsData
{
	char strCMSIP[16];
	int nPort;

    _PBSDCmsData()
    {
        ZeroMemory(strCMSIP, sizeof(strCMSIP));
        nPort = 0;
    }
}PBSDCmsData, *LPPBSDCmsData;

// �ͻ�����Ϣ.
typedef struct _PBSDClientData
{
	EnumBwClientType emClientType;			// �ͻ�������.
	char szDes[128];						// �ͻ��������� �硰�Ӿ����ġ�.

    _PBSDClientData()
    {
        ZeroMemory(szDes, sizeof(szDes));
        emClientType = PBSD_BW_CLIENT_TYPE_CS;
    }
}PBSDClientData, *LPPBSDClientData;

// ����Ȩ��.
typedef struct _PBSDRights
{
	BOOL bCapture;
	BOOL bMontage;
	_PBSDRights()
	{
		bCapture = TRUE;  //ץͼȨ��
		bMontage = TRUE;  //����Ȩ��
	}
}PBSDRights, *LPPBSDRights;

/**   @fn          PBSD_PlaybackCB
 *    @brief   	   �طŻص������������ļ�����.
 *    @param[in]   nWndID:���ں�. 
 *    @param[in]   strXML:�ļ�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
typedef void (CALLBACK * PBSD_PlaybackCB)(int nWndID, CString strXML, LPVOID pUserData);

/**   @fn          PBSD_MenuCB
 *    @brief   	   �˵��ص�����.	
 *    @param[in]   WM_USER + 1802:д�¼���ǩ,stCBData.pInfoΪEventLabelData
 *    @param[in]   
 *    @return      
 */
typedef void (CALLBACK * PBSD_MenuCB)(int nMenuID, MenuCBData stCBData, LPVOID pUserData);

/**   @fn          PBSD_WorkLogCB
 *    @brief   	   д������־�ص�.
 *    @param[in]   pLogData:��־��Ϣ�ṹ��.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
typedef void (CALLBACK * PBSD_WorkLogCB)(LPWorkLogData pLogData, LPVOID pUserData);

/**   @fn          PBSD_Init
 *    @brief   	   ��ʼ��.
 *    @param[in]   emSkinType:Ƥ������.
 *    @param[in]   dwStyle:������.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_Init(PBSDSkinType emSkinType = PBSD_SKIN_RED, DWORD dwStyle = PBSD_STYLE_SIMPLE, const CString csRights = _T("11111111"));

/**   @fn          PBSD_UnInit
 *    @brief   	   ����ʼ��.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PBSD_API BOOL PBSD_UnInit();

/**   @fn          PBSD_Create
 *    @brief   	   ��������.
 *    @param[in]   hWnd:������.
 *    @param[in]   emLayout:����ָ�ģʽ.
 *    @return      HANDLE:�طŴ���Ψһ��ʶ,NULL��ʾʧ��.
 */
PBSD_API HANDLE PBSD_Create(HWND hWnd, PBSDLayout emLayout = PBSD_LAYOUT_WND_1);

/**   @fn          PBSD_Destroy
 *    @brief   	   ���ٴ���.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PBSD_API BOOL PBSD_Destroy(HANDLE hDlg);

/**   @fn          PBSD_MoveWindow
 *    @brief   	   �ƶ�����.
 *    @param[in]   pRect:���ڴ�С.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @return      TRUE:�ɹ���FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_MoveWindow(HANDLE hDlg, LPCRECT pRect);

/**   @fn          PBSD_ShowWindow
 *    @brief   	   ��ʾ���ش���.
 *    @param[in]   bShow:TRUE-��ʾ,FALSE-����.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_ShowWindow(HANDLE hDlg, BOOL bShow);

/**   @fn          PBSD_PlaybackByWndNo
 *    @brief   	   ��ָ�����ڽ��лط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pLoginParam:��¼����.
 *	  @param[in]   nWndNo:���ں�.
 *    @param[in]   pPlayParam:�طŲ���.
 *    @param[in]   dwRight:Ȩ�޿���.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByWndNo(HANDLE hDlg, 
								   int nWndNo, 
								   LPPBSDLoginParam pLoginParam, 
								   LPPBSDPlayParam pPlayParam, 
								   DWORD dwRight = PBSD_RIGHT_ALL);

/**   @fn          PBSD_LocalPlayback
 *    @brief   	   ���ػط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   nVideoType:¼�����ͣ�0-����
 *    @param[in]   csFilePath:�����ļ�·��.
 *    @return      
 */
PBSD_API BOOL PBSD_LocalPlayback(HANDLE hDlg, const int nVideoType, const CString csFilePath);

/**   @fn          PBSD_PlaybackOnSelWnd
 *    @brief   	   �ڹ��ѡ�еĴ��ڽ��лط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pLoginParam:��¼����.
 *    @param[in]   pPlayParam:�طŲ���.
 *    @param[in]   dwRight:Ȩ�޿���.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_PlaybackOnSelWnd(HANDLE hDlg, 
									LPPBSDLoginParam pLoginParam, 
									LPPBSDPlayParam pPlayParam, 
									DWORD dwRight = PBSD_RIGHT_ALL);

/**   @fn          PBSD_PlaybackByWndNo
 *    @brief   	   ��ָ�����ڽ��лط�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pLoginParam:��¼����.
 *	  @param[in]   nWndNo:���ں�,��0��ʼ��-1��ʾ������ڴ���..
 *    @param[in]   pPlayParam:�طŲ���.
 *    @param[in]   dwRight:Ȩ�޿���.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByTelNo(HANDLE hDlg,
								   int nWndNo,
								   LPTelNoInfo pLoginParam,
								   LPPBSDTelPlayParam pPlayParam, 
								   DWORD dwRight = PBSD_RIGHT_ALL);


/**   @fn          PBSD_StopPlayByWndNo
 *    @brief   	   ָֹͣ���ĻطŴ���.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   nWndNo:���ں�.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_StopPlayByWndNo(HANDLE hDlg, int nWndNo);

/**   @fn          PBSD_GetPlaybackInfo
 *    @brief   	   ��ȡ�ط�״̬��Ϣ.	
 *    @param[in]   hDlg:PBSD_Create����ֵ.
 *    @param[in]   nWndNo:���ں�.
 *    @param[out]  stData:״̬��Ϣ.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_GetPlaybackInfo(HANDLE hDlg, int nWndNo, PlaybackStatData& stData);

/**   @fn          PBSD_StopAll
 *    @brief   	   ֹͣȫ������.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
PBSD_API BOOL PBSD_StopAll(HANDLE hDlg);

/**   @fn          PBSD_SetMenuCallBack
 *    @brief   	   ���ò˵��ص�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnMenuCB:�ص�����.
 *    @param[in]   pUserData:�û�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_SetMenuCallBack(HANDLE hDlg, PBSD_MenuCB pfnMenuCB, LPVOID pUserData);

/**   @fn          PBSD_SetWorkLogCallBack
 *    @brief   	   ����д������־�ص�.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnWorkLogCB:�ص�����.
 *    @param[in]   pUserData:�û�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_SetWorkLogCallBack(HANDLE hDlg, PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData);

/**   @fn          PBSD_SetPlaybackCallBack
 *    @brief   	   ���ûطŻص�,�����ļ�����.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   pfnPlaybackCB:�ص�����.
 *    @param[in]   pUserData:�û�����.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_SetPlaybackCallBack(HANDLE hDlg, PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData);

/**   @fn          PBSD_SetParameter
 *    @brief   	   ���ò���.
 *    @param[in]   stParam:��������
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
PBSD_API BOOL PBSD_SetParameter(const PLAYBACK_PARAM& stParam);

/**   @fn          PBSD_SetIEProtected
 *    @brief   	   ����IE����ģʽ,������ҳOCX.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_SetIEProtected();

/**   @fn          PBSD_SetBwControl
 *    @brief   	   ���ô������ģʽ.
 *    @param[in]   stUserData:�û���Ϣ.
 *    @param[in]   stCmsData:CMS����.
 *    @return      
 */
PBSD_API void PBSD_SetBwControl(const PBSDUserInfo& stUserData, const PBSDCmsData& stCmsData, const PBSDClientData& stClientData);

/**   @fn          PBSD_Invalida
 *    @brief   	   ˢ�´�����Ϣ.
 *    @param[in]   hDlg:PBSD_Create�ķ���ֵ.
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_Invalidate(HANDLE hDlg);

/**   @fn          PDSD_SetPlayBackConfig
 *    @brief   	   ���ûط�������Ϣ.
 *    @param[in]   szCapPath:ץͼ·���� szDownPath����·���� nPicTypeͼƬ���� 0��bmp 1jpg
 *    @param[in]   
 *    @return      void
 */
PBSD_API void PDSD_SetPlayBackConfig(const PLAYBACK_CONFIG& stPBConfig);

#ifdef __cplusplus
};
#endif

#endif