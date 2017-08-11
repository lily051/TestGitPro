
#pragma once

/*�Զ�����Ϣ*/
//
#define WM_UPDATE_CONFIG		(WM_USER + 1998)
#define WM_LAYOUT_BTN           (WM_USER + 1999)
#define WM_ADD_MODULE           (WM_USER + 2000)
#define WM_DEL_MODULE           (WM_USER + 2001)
#define WM_HEAD_MODULE          (WM_USER + 2002)

#define MSG_COMPONENT_UP        (WM_USER + 2003)     // �������������Ϣ
#define MSG_TOOLBTN_CLOSE       (WM_USER + 2004)     // ��ť�رգ�֪ͨ�����ڹر����
#define MSG_TOOLBTN_SELECT      (WM_USER + 2005)     // ��ť�����֪ͨ��������ʾ����
#define MSG_WND_SHOW            (WM_USER + 2006)     // �����Զ���ʾʱ��֪ͨ��Ϣ
#define WM_SYS_UNLOCK           (WM_USER + 2007)     // ������Ϣ
#define MSG_CFGCHANGE_NOTIFY    (WM_USER + 2008)     // ������ø���֪ͨ��Ϣ
#define MSG_TITLE_NOTIFYICON    (WM_USER + 2009)     // ����ڵڶ����ڴ�,֪ͨ�ڱ�������ʾСͼ��
#define MSG_NETCONNECT_NOTIFY   (WM_USER + 2010)     // ͨ������֪ͨ�������ӱ�ʶwnd
#define MSG_TOOLBTN_BLINK       (WM_USER + 2011)     // ֪ͨ�����������ť��˸
#define MSG_FLASH_WINDOW        (WM_USER + 2012)     // ֪ͨ��������������˸
#define MSG_FLASH_VIEWTALK_WINDOW (WM_USER + 2013)   // ֪ͨ���ӶԽ��������ˢ�£����ӶԽ�������ڵĸ�����Ϊ��ܴ��ڣ���˿�ܴ�����С��ʱ�����ӶԽ�������ڻ����أ������Ҫ����ˢ�¿��ǶԽ��������λ�ã�ע�����������Ŷ��
#define MSG_SELECT_COMPONENT    (WM_USER + 2014)     // ֪ͨ����ѡ�����
#define MSG_CLOSE_COMPONENT     (WM_USER + 2015)     // ֪ͨ���ڹر����
#define MSG_ADD_EXE_COMPONENT   (WM_USER + 2016)     // ���EXE���
#define MSG_ADD_EXE_COM_FAILED  (WM_USER + 2017)     // ���EXE���ʧ��
#define MSG_COM_HEART_REQ       (WM_USER + 2018)     // ����������Ϣ
#define MSG_COM_HEART_RSP       (WM_USER + 2019)     // ������Ӧ��Ϣ
#define MSG_SECOND_WND          (WM_USER + 2020)     // �ڶ����ڴ����
#define MSG_CLOSE_SECOND_WND    (WM_USER + 2021)     // �رյڶ�����
#define MSG_FORCE_LOGOFF        (WM_USER + 2022)     // ǿ��������Ϣ
#define MSG_REBOOT_APP          (WM_USER + 2023)     // ������Ϣ
#define MSG_EXEMODULE_ALARM     (WM_USER + 2024)     // ģ����̱�����Ϣ
#define MSG_USER_CLOSE          (WM_USER + 2025)     // �����������������˳���Ϣ, [ID��Ҫ�޸ģ�]
#define WM_ON_ALARM_NET_DISCONNT	(WM_USER + 2026) //��������״̬��Ϣ
#define MSG_SHOW_VIEWTALK       (WM_USER + 2030)     // ��ʾ���ӶԽ�������Ϣ
#define MSG_START_LOCK          (WM_USER + 2031)     // EXEģ�鿪������
#define MSG_UNLOCK_SUCCESS      (WM_USER + 2032)     // EXEģ������ɹ�

#define WM_PV_STARTRECORD	(WM_USER + 2500)		//�ؼ�¼��ʼ��Ϣ
#define WM_PV_STOPRECORD		(WM_USER + 2501)		//�ؼ�¼��ֹͣ��Ϣ

#define WM_SELECT_REGION				(WM_USER + 2510)		//ѡ��������Ϣ		
#define WM_SELECT_PRODUCTTYPE		(WM_USER + 2511)		//ѡ���Ʒ������Ϣ
#define WM_SELECT_PRODUCT				(WM_USER + 2512)		//ѡ���Ʒ��Ϣ


// �����ڵ�Ĭ�ϴ�С
#define  DEFAULT_WND_SIZE   CRect(0, 0, 900, 600)

//ҵ���¼
#define BUSINESS_WND_SIZE CRect(0, 0, 898, 536)
//��ʷҵ�񵥴��ڴ�С
#define HISTORYLIST_WND_SIZE CRect(0, 0, 758, 460)

//����ģ�崰�ڴ�С
#define WORD_TEMPLATE_WND_SIZE CRect(0, 0, 530, 336)

//Ԥ�����ڴ�С
#define PREVIEW_WND_SIZE	CRect(0, 0, 400, 260)

//�طŴ��ڴ�С
#define PLAYBACK_WND_SIZE CRect(0, 0, 800, 500)

//���߿ͻ��˴��ڴ�С
#define WORD_TEMPLATE_WND_SIZE CRect(0, 0, 530, 336)

#define CHANGE_PWD_WND_SIZE CRect(0, 0, 350, 260)

#define EDIT_FILENAME_WND_SIZE CRect(0, 0, 350, 150)

#define PLAYBACK_WND_TOOLBAR_HEIGHT 35

// ���������ܱ߿��С
#define MARGIN_TOP      40
#define MARGIN_LEFT     0
#define MARGIN_RIGHT    0
#define MARGIN_BOTTOM   0

#define MARGIN_TOP_DLG      35
#define MARGIN_LEFT_DLG     1
#define MARGIN_RIGHT_DLG    1
#define MARGIN_BOTTOM_DLG   1

#define BOTTOM_HEIGHT   28
// �����ڵ������ĸ߶�
#define NAVIBAR_HEIGHT  36

// 
#define BASE_LAYER_ID   102

// ��ҳ�Ҳ���Ϣ���Ŀ�ȣ��̶���
#define EXTINFO_WIDTH   320

// ���洰�ڸ߶�
#define BBS_HEIGHT      345

// ��������
#define YAHEI_18_N      _T("Yahei18N")     // �ź�, 18, NORMAL
#define YAHEI_20_N      _T("Yahei20N")     // �ź�, 20, 
#define YAHEI_42_N      _T("Yahei42N")     // �ź�, 30, NORMAL

// ��ɫ
const DWORD COLOR_BLUE = RGB(63, 133, 228);

// ��ȡ��Ȩ����ϵͳ����
#define SYS_TYPE_CONFIG     1
#define SYS_TYPE_8100       1
#define SYS_TYPE_ACSUBSYS   2

// ��¼��ʷ�����ļ�
#define LOGIN_FILE  _T("\\Config\\Login.xml")

#define CENTRALCLIENT_FILEMAP   _T("CentralClientMapFile")  // �����ڴ���
#define MUTEX_INSTANCE_NAME     _T("FMSClient")         // ����Ψһ��ʶ���������

// �ַ����ȶ���
#define LEN_IP        16    // IP��ַ����
#define LEN_USER_NAME 64    // �û�������

// ϵͳ�˵�
enum
{
    SYS_MENU_CONFIG = 100,
    SYS_MENU_MODPWD,
    SYS_MENU_LOGOUT,
    SYS_MENU_ABOUT,
    SYS_MENU_EXIT,

    SYS_MENU_MAX_ID,
};

enum
{
	ALARM_CONFIG_MENU_ADD = 200,
	ALARM_CONFIG_MENU_DELTE,
	ALARM_CONFIG_MENU_UPDATE,
};

// ���ID
enum COMPONENT_ID
{
    MENUID_INVALID = -1,    // ��Ч�����ID���ڼ��ʱ�õ�
    MENUID_VIEWBASE = 6020,
    MENUID_HOME,
    MENUID_PREVIEW,
    MENUID_TVWALL,
    MENUID_TVMS,
    MENUID_PLAYBACK,
    MENUID_ALARMCENTER,
    MENUID_EMAP,
    MENUID_GISMAP,
    MENUID_STATUSCONTROL,
    MENUID_INM,
    MENUID_LOG,
    MENUID_COFFER,
    MENUID_AC,
    MENUID_VIEWWALL,
    MENUID_VIEWBASEEND,
    MENUID_VIEWTALK,
    MENUID_FBROWSER_MIN = 6070,
    MENUID_FBROWSER_MAX = 6090,
    MENUID_MORE,

    MENUID_VIEWBASE_MAX = 6100,
    MENUID_CONFIG,
    MENUID_TOOL,

    MENUID_AVIODDOZING,
};

// �ͻ���������Ϣ
typedef struct __sys_config 
{
	typedef struct __base_config 
	{
		BOOL bRemPwd;		// ��������
		BOOL bAutoLogin;	// �Զ���¼
		int  nNetTimeout;	// ���糬ʱʱ�� 0-15s, 2-20s, 3-30s, 4-45s
		BOOL bShowLogo;		// ��ʾLogo
		BOOL bShowCpRight;	// ��ʾ��Ȩ
		BOOL bShowNetInfo;	// ��ʾ������Ϣ
		BOOL bShowDate;		// ��ʾ����
		BOOL bShowTime;		// ��ʾʱ��
		BOOL bShowWeek;		// ��ʾ����

		__base_config()
		{
			bRemPwd = FALSE;
            bAutoLogin = FALSE;
            nNetTimeout = 0;
            bShowLogo = FALSE;
            bShowCpRight = FALSE;
            bShowNetInfo = FALSE;
            bShowDate = FALSE;
            bShowTime = FALSE;
            bShowWeek = FALSE;
		}
	}BASE_CONFIG;

	typedef struct __preview_config 
	{
		int nPicFormat;			// ץͼ��ʽ 0-JPG, 1-BMP
		CString strPicPath;		// ץͼ·��
		int nRecSize;			// ¼���С 0-64M, 1-128M, 2-256M, 3-512M
		CString strRecPath;		// ¼��·��
		BOOL bAutoStreamType;	// �Զ���������
		BOOL bMainStreamOne;	// ������������
		BOOL bDirectDevice;		// ֱ���豸
		int nDirectStreamType;	// ֱ���豸ʱ�������� 0-ƽ̨�� 1-���� 2-��
		int nDecodeBuffer;		// ���뻺�� 0-512K, 1-1M, 2-2M, 3-3M
		int nDecodeFrame;		// ����֡�� ?-?֡
		BOOL bShowIntel;		// ��ʾ���ܹ���
		BOOL bShowInfo;			// ��ʾ��ص���Ϣ
		BOOL bShowCardNo;		// ��ʾ����

		__preview_config()
		{
			nPicFormat = 0;
			nRecSize = 0;		
			bAutoStreamType = 0;
			bMainStreamOne = 0;
			bDirectDevice = 0;
			nDirectStreamType = 0;
			nDecodeBuffer = 2;	
			nDecodeFrame = 6;
			bShowIntel = 0;
			bShowInfo = 0;
			bShowCardNo = 0;
		}
	}PREVIEW_CONFIG;

	typedef struct __playback_config 
	{
		int nPicFormat;			// ץͼ��ʽ 0-JPG, 1-BMP
		CString strPicPath;		// ץͼ·��
		CString strRecPath;		// ¼��·��
		BOOL bAutoNextWnd;		// ����Զ�����
		BOOL bShowFileWnd;		// ��ʾ�ļ��б���
		BOOL bShowVideoTitle;	// ��ʾ¼�����

		__playback_config()
		{
			nPicFormat = 0;
			bAutoNextWnd = 0;
			bShowFileWnd = 0;
			bShowVideoTitle = 0;
		}
	}PLAYBACK_CONFIG;

	typedef struct __alarm_config 
	{
		int nWndType;
		int nShowPos;
		int nCoverType;
		int nHotKey;	
		std::vector<CString> strHandleTemplate;
		BOOL bTransmit;
		int nTransmitTime;
		CString strTransmitTo;

		__alarm_config()
		{
			nWndType = 0;
			nShowPos = 0;
			nCoverType = 0;
			nHotKey = 0;
			bTransmit = 0;
			nTransmitTime = 0;
		}
	}ALARM_CONFIG;

	BASE_CONFIG		baseConfig;
	PREVIEW_CONFIG	previewConfig;
	PLAYBACK_CONFIG playbackConfig;
	ALARM_CONFIG	alarmConfig;
}SYS_CONFIG;


// ������ط�ʽ
enum _e_component_load_type
{
    TYPE_DLL = 0,   // DLL��ʽ
    TYPE_EXE = 1,   // EXE��ʽ
    TYPE_HOME = 2,  // HOMEҳ
};

// �����������
typedef struct _s_component_config
{
    int nLoadType;              // ���ط�ʽ
    int nHeartBeat;             // ����ʱ��
    int nListenPort;            // �����˿�
    CString strKey;             // �ؼ���
    CString strVersion;         // ��Ͱ汾
    CString strFileName;        // ����ļ���
    CString strPreLoad;         // Ԥ���������ļ�

    _s_component_config()
    {
        nLoadType = TYPE_DLL;
        nHeartBeat = 10;
        nListenPort = 0;
    }
}STU_COMPONENT_CONFIG;

// �����в���
typedef struct _s_comman_param
{
    int nDcPort;         // ���ݿ�˿�
    int nComponentId;    // ģ��ID
    CString strKey;      // ģ��ؼ���
    CString strDcIp;     // ���ݿ�IP��ַ
    CString strUser;     // �û���
    CString strPassword; // ����
    CString strLocalIp;  // ����IP
    CString strFileMapName;  // �����ڴ���(���ڻ�ȡ�����ھ��)

    _s_comman_param()
    {
        nDcPort = 0;
		nComponentId = 0;
    }
}STU_COMMAND_PARAM;

// Ԥ����־��Ϣ
typedef struct _s_oper_Log
{
    int nCamID;                 // ��ص�ID
    int nLogType;               // ��־����
    int nResult;                // ���
    std::string strRemark;      // ��ע
    _s_oper_Log()
    {
        nCamID = 0;
        nLogType = 0;
        nResult = 0;
    }
}STU_OPER_LOG;

#define PV_STREAM_TIMEOUT_MAX 60		//��λΪ��
#define PV_STREAM_TIMEOUT_MIN 5			//��λΪ��
#define PV_STREAM_TIMEOUT_DEFAULT 10	//��λΪ��

#define PB_STREAM_TIMEOUT_MAX 60		//��λΪ��
#define PB_STREAM_TIMEOUT_MIN 10		//��λΪ��
#define PB_STREAM_TIMEOUT_DEFAULT 60	//��λΪ��

#define PV_CYCLE_CAMERA_LIMIT	1000


typedef struct _tagServerInfo    //��������Ϣ
{
	TCHAR szServerIp[16];                  // ������ip
	int nServerPort;                       // ������port
	int nComponmentID;                     // ���ID
	int nServerID;                         // ������ID
	_tagServerInfo()
	{
		_tcscpy_s(szServerIp, 16, _T("0.0.0.0"));
		nServerPort = 0;
		nComponmentID = -1;
		nServerID = -1;
	}
} SERVER_INFO;

enum COMMON_SERVER_ID
{
	CMS_SERVER = 1,            //��������
	CLIENT_SELF,               //�ͻ�������
};

enum REQ_MSG_TYPE
{
	REQ_MSG_WAITING_RSP = 0,       //�ȴ�������Ϣ
	REQ_MSG_ASYN_RSP,              //�첽����
	REQ_MSG_NOT_NEED_RSP,          //���践����Ϣ
};

// ��Դ���ڵ����
enum 
{
	RRTREE_CLASSIFY_REGION,	// ��֯
	RRTREE_CLASSIFY_BRANCH,		// ����
	RRTREE_CLASSIFY_BANK            // ����
};

#define TREE_DLG_TYPE_REGION				1
#define TREE_DLG_TYPE_PRODUCTTYPE	2

#define RECORD_TYPE_NVR		1  //NVR¼��
#define RECORD_TYPE_LOCAL	2 //����¼��

#define SYS_BTN_MENU		601
#define SYS_BTN_USER			602
#define SYS_BTN_CONFIG		700
#define SYS_BTN_LIST			701
#define SYS_BTN_BUSINESS	702

//��ӡҵ��htmlģ���ļ��ؼ��ֶ�λ��
#define INDEX_CONSUMER_NAME	44
#define INDEX_ID_TYPE						49
#define INDEX_ID_NUMBER				55
#define INDEX_BUSINESS_ID				71
#define INDEX_BUSINESS_TIME		86
#define INDEX_ISSUER						101
#define INDEX_PRODUCT_TYPE		116
#define INDEX_PRODUCT_NAME		127
#define INDEX_SALESMAN_NAME	142
#define INDEX_SALESMAN_ID			153
#define INDEX_PIC1							165
#define INDEX_PIC2							169


