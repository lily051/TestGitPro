/**	@file    PreviewDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   Ԥ��Dllȫ�����ݶ���
 *
 *	@author	 Liu Ronghua
 *	@date	 2013/01/04
 *	@note   
 *	@note    ��ʷ��¼��
 *	@note    V1.0  create at 2013/01/04
 */

#ifndef _PREVIEW_DEF_
#define _PREVIEW_DEF_

#include <string>
using namespace std;

const int MAX_DLG_ARRAY = 128;

//////////////////////////////////////////////////////////////////////////
// Msg

#define WM_RBTNDOWNPRESETBTN        (WM_USER + 1049)        // CButtonST_Skin RButtonDown msg
#define WM_PV_FOCUSWND_CHANGED      (WM_USER + 1050)
#define WM_PV_TOOLBAR_BTN_CLK       (WM_USER + 1051)
#define WM_PV_REALWND_TOOLBAR_CLK   (WM_USER + 1052)
#define WM_PV_TOOLBAR_CONFIG        (WM_USER + 1053)
#define WM_PV_REALWND_MENU_CB       (WM_USER + 1054)
#define WM_PV_WRITE_OPERLOG_CB      (WM_USER + 1055)
//�������-�ر�Ԥ����Ϣ
#define WM_BW_ENFORCE_STOP_PREVIEW  (WM_USER + 1056)
//�������-��������Ԥ����Ϣ
#define WM_BW_REREQUEST_PREVIEW     (WM_USER + 1057)

#define WM_PV_REALWND_NET_EXCEPTION (WM_USER + 1058)
#define WM_PV_TOOLBAR_BTN_CLK_REF	(WM_USER + 1059)
#define WM_PV_REALPLAY_STOPPREVIEW  (WM_USER + 1060)
#define WM_PV_TOOLBAR_EXECUTE       (WM_USER + 1061)

//////////////////////////////////////////////////////////////////////////
// String

#define STR_PREVIEW_CONNECT_FAIL                _T("����ʧ�ܣ�������������...")
#define STR_PREVIEW_RECONNECT_FAIL				_T("����ʧ��!")
#define STR_PREVIEW_NETWORK_EXCEPTION			_T("�����쳣,���Ժ�����...")
#define STR_PREVIEW_RECONNECT_OK				_T("�����ɹ�")
#define STR_PREVIEW_CONNECT_OK					_T("���ӳɹ�")
#define STR_PREVIEW_RECONNECT_OVER				_T("����ʧ��!")
#define STR_PREVIEW_STATUS_NULL					_T("")
#define STR_POPUPMENU_STOPPREVIEW				_T("ֹͣԤ��")
#define STR_POPUPMENU_SNAPSHOT					_T("Ԥ��ץͼ")
#define STR_POPUPMENU_TALK_START				_T("�����Խ�")
#define STR_POPUPMENU_TALK_STOP					_T("�رնԽ�")
#define STR_POPUPMENU_OPENSOUND					_T("������Ƶ")
#define STR_POPUPMENU_CLOSESOUND				_T("�ر���Ƶ")
#define STR_POPUPMENU_STARTRECORD				_T("��ʼ¼��")
#define STR_POPUPMENU_STOPRECORD				_T("ֹͣ¼��")
#define STR_POPUPMENU_EVENTRECORD				_T("�¼���ǩ")
#define STR_POPUPMENU_MAIN_STREAM			    _T("������")
#define STR_POPUPMENU_SUB_STREAM			    _T("������")
#define STR_DRAWTEXT_RECORDING                  _T("��¼����,¼��������%dM")
#define STR_DRAWTEXT_TALKING                    _T("��Խ���...")
#define STR_DRAWTEXT_OPENSOUDN                  _T("����Ƶ������...")
#define STR_DRAWTEXT_FONT						_T("���Ŀ���")
#define STR_DRAWTEXT_DISK_INFFICIENT            _T("���̿ռ䲻��,��ֹͣ¼��")
#define STR_PREVIEW_TOOLBAR_FULLSCREEN			_T("ȫ��")
#define STR_PREVIEW_TOOLBAR_ARRANGE1			_T("һ����")
#define STR_PREVIEW_TOOLBAR_ARRANGE4			_T("�Ļ���")
#define STR_PREVIEW_TOOLBAR_ARRANGE6			_T("������")
#define STR_PREVIEW_TOOLBAR_ARRANGE8			_T("�˻���")
#define STR_PREVIEW_TOOLBAR_ARRANGE9			_T("�Ż���")
#define STR_PREVIEW_TOOLBAR_ARRANGE10			_T("ʮ����")
#define STR_PREVIEW_TOOLBAR_ARRANGE13			_T("ʮ������")
#define STR_PREVIEW_TOOLBAR_ARRANGE14			_T("ʮ�Ļ���")
#define STR_PREVIEW_TOOLBAR_ARRANGE16			_T("ʮ������")
#define STR_PREVIEW_TOOLBAR_ARRANGE17			_T("ʮ�߻���")
#define STR_PREVIEW_TOOLBAR_ARRANGE22			_T("��ʮ������")
#define STR_PREVIEW_TOOLBAR_ARRANGE25			_T("��ʮ�廭��")
#define STR_PREVIEW_TOOLBAR_ARRANGEMORE			_T("����ָ�")
#define STR_PREVIEW_TOOLBAR_STOPALL				_T("ֹͣ����Ԥ��")
#define STR_PREVIEW_TOOLBAR_SNAPSHOTALL			_T("ȫ��ץͼ")
#define STR_PREVIEW_TOOLBAR_CONFIG              _T("��������")
#define STR_PREVIEW_TOOLBAR_PREGROUPCFG			_T("Ԥ����������")
#define STR_PREVIEW_TOOLBAR_BROADCAST			_T("�㲥")
#define STR_PREVIEW_TOOLBAR_CLUSTERSHOCK	    _T("Ⱥ��")
#define STR_PREVIEW_TOOLBAR_PREVIOUS			_T("��һҳ")
#define STR_PREVIEW_TOOLBAR_NEXT				_T("��һҳ")
#define STR_PREVIEW_TOOLBAR_STARTRECORD			_T("��ʼ¼��")
#define STR_PREVIEW_TOOLBAR_STOPRECORD			_T("ֹͣ¼��")
#define STR_PREVIEW_TOOLBAR_STARTPREVIEW			_T("��ʼԤ��")
#define STR_PREVIEW_TOOLBAR_STOPPREVIEW			_T("ֹͣԤ��")
#define STR_PREVIEW_TOOLBAR_STARTSOUND			_T("��������")
#define STR_PREVIEW_TOOLBAR_STOPSOUND			_T("�ر�����")
#define STR_POPUPMENU_OSDINFO				    _T("OSD��Ϣ")
#define STR_POPUPMENU_SHOWOSD			    	_T("��ʾˮӡ")
#define STR_POPUPMENU_HIDEOSD				    _T("����ˮӡ")
#define STR_POPUPMENU_SHOW4OSD			    	_T("4����ʾ")
#define STR_POPUPMENU_SHOW8OSD				    _T("8����ʾ")

//////////////////////////////////////////////////////////////////////////
// Color

// ���ϵ
#define UI_RED_BKGND_LIGHT          RGB(234,233,231)
#define UI_RED_BKGND_DEEP 		    RGB(124,124,124)
#define UI_RED_BKGND_PV             RGB(39,39,39)
#define UI_RED_BKGND_RW             RGB(52,52,52)
#define UI_RED_RW_BORDER            RGB(182,0,0)
#define UI_RED_UIDIALOG_BKGND       RGB(243,243,243)
#define UI_RED_UIDIALOG_TITLE       RGB(255,255,255)
#define UI_RED_STATISTIC_BKGND      RGB(184,184,184)
#define UI_RED_TREE_BKGND           RGB(255,255,255)
#define UI_RED_LIST_BKGND           RGB(255,255,255)
#define UI_RED_LIST_ITEM_LIGHT      RGB(243,250,255)
#define UI_RED_LIST_ITEM_DEEP       RGB(247,247,247)

// ����ϵ
#define UI_BLUE_BKGND_LIGHT		    RGB(236,241,245)
#define UI_BLUE_BKGND_DEEP 		    RGB(167,195,217)
#define UI_BLUE_BKGND_PV            RGB(21,32,41)
#define UI_BLUE_BKGND_RW            RGB(37,48,57)
#define UI_BLUE_RW_BORDER           RGB(3,21,255)
#define UI_BLUE_UIDIALOG_BKGND      RGB(236,241,245)
#define UI_BLUE_UIDIALOG_TITLE      RGB(0,0,0)
#define UI_BLUE_STATISTIC_BKGND     RGB(186,209,220)//RGB(96,125,147)
#define UI_BLUE_TREE_BKGND          RGB(241,241,241)
#define UI_BLUE_LIST_BKGND          RGB(241,241,241)
#define UI_BLUE_LIST_ITEM_LIGHT     RGB(249,253,255)
#define UI_BLUE_LIST_ITEM_DEEP      RGB(236,241,245)

#define UI_COLOR_BKGND_175          RGB(175,175,175)

#define UI_FONT_BAR_HEIGHT          100
#define UI_FONT_BAR_WEIGHT          FW_MEDIUM
#define UI_FONT_BAR_NAME            _T("����")
#define UI_COLOR_TRANSPARENT        RGB(255,255,254)
//////////////////////////////////////////////////////////////////////////
// Enum

enum LOG_TYPE {
    LOG_TYPE_START_PREVIEW = 626000,
    LOG_TYPE_STOP_PREVIEW = 626001,
    LOG_TYPE_CAPTURE_PIC = 626002,
    LOG_TYPE_START_REC = 626003,
    LOG_TYPE_STOP_REC = 626004,
    LOG_TYPE_PTZ = 626005,
    LOG_TYPE_START_TALK = 626006,
    LOG_TYPE_STOP_TALK = 626007,
    LOG_TYPE_START_ALARM_OUT = 626008,
    LOG_TYPE_STOP_ALARM_OUT = 626009,
    LOG_TYPE_START_PREVIEW_ON_TVWALL = 626010,
    LOG_TYPE_STOP_PREVIEW_ON_TVWALL = 626011,
    LOG_TYPE_START_PLAYBACK = 626012,
    LOG_TYPE_STOP_PLAYBACK = 626013,
    LOG_TYPE_START_DOWNLOAD = 626014,
    LOG_TYPE_STOP_DOWNLOAD = 626015,
    LOG_TYPE_START_PB_REC = 626016,
    LOG_TYPE_STOP_PB_REC = 626017
};

// Ԥ�����洰�ڶ���
enum PREVIEWWND_DEFINE
{
    ID_WND_PLAYVIEWWND			= 0x2000,   ///< Ԥ������
    ID_WND_PLAYVIEW_CHILD		= 0x2001,   ///< �����Ӵ���ID
    ID_WND_PLAYVIEWTOOLBAR		= 0x2002,   ///< ����������
    ID_WND_PLAYVIEW				= 0x2003,   ///< Ԥ�����Ŵ���
    ID_WND_GROUPPLAYVIEW		= 0x2004,   ///< ������ѲԤ������
    ID_WND_GROUPPLAYTOOLBAR		= 0x2005,   ///< ������Ѳ������
    ID_WND_PLAYVIEW_LAYOUTMORE  = 0x2006,   ///< ��չ�ָ�ģʽ		
    ID_WND_PLAYVIEW_FLOWWND		= 0x2007,   ///< �����ͼ��ť�����ĸ�������
    ID_WND_TVWALL_FLOWWND		= 0x2008,   ///< �������鰴ť�����ĸ�������
};

// �Զ���ؼ���С
enum CTRL_CUSTOMSIZE
{
    CTRL_BIGBTN_HEIGHT			= 50,       //�Զ��尴ť��С
    CTRL_EDIT_NORMAL_WIDTH		= 25,       //�༭��
    CTRL_TREE_NORMAL_WIDTH		= 245,      //���ؼ�
    CTRL_TREE_NODE_HEIGHT		= 20,
    CTRL_TREE_SMALL_WIDTH		= 200,
    CTRL_SPACE_LITTLE			= 2,        //�ؼ����
    CTRL_SPACE_NORMAL			= 5,
    CTRL_BORD_LITTLE			= 3,        //ϸ�߿�
    CTRL_PREVIEW_TOOLBAR_HEIGHT	= 45,        //Ԥ���������߶�
};

// ��������ťID
enum PREVIEWWND_PLAYVIEWTOOLBAR_BTNDEFINE
{
    IDC_BTN_PT_ARRANGE		= 0x2010,       //����ָ�
    IDC_BTN_PT_ARRANGE_1	= 0x2011,       //����ָ�1
    IDC_BTN_PT_ARRANGE_4	= 0x2012,       //����ָ�2*2
    IDC_BTN_PT_ARRANGE_9	= 0x2013,       //����ָ�3*3
    IDC_BTN_PT_ARRANGE_16	= 0x2014,       //����ָ�4*4
    IDC_BTN_PT_FULLSCREEN	= 0x2015,       //�໭��ȫ��
    IDC_BTN_PT_STOPALL		= 0x2016,       //ֹͣȫ��Ԥ��
    IDC_BTN_PT_GROUPCFG		= 0x2017,       //Ԥ����������
    IDC_BTN_PT_SNAPSHOT		= 0x2018,       //ץͼ
    IDC_BTN_PT_PRECONFIG	= 0x2019,       //Ԥ������
    IDC_BTN_PT_BROADCAST	= 0x2020,       //�㲥
    IDC_BTN_PT_CLUSTERSHOCK	= 0x2021,       //Ⱥ��
    IDC_BTN_PT_PREVIOUSPAGE = 0x4000,       //��һҳ
    IDC_BTN_PT_NEXTPAGE		= 0x4001,       //��һҳ
	IDC_BTN_PT_STARTRECORD	= 0x5000,       //��ʼ¼��
	IDC_BTN_PT_STOPRECORD	= 0x5001,       //ֹͣ¼��
	IDC_STC_PT_VIDEOWAV 	= 0x5002,       //¼����
	IDC_STC_PT_AUDIOWAV 	= 0x5003,       //¼������
	IDC_BTN_PT_VIDEOWAVE	= 0x5004,       //��ʼԤ��
	IDC_BTN_PT_PHONEWAVE	= 0x5005,       //¼����ť
	IDC_BTN_PT_VIDEOWAVE_STOP	= 0x5006,   //ֹͣԤ��
	IDC_BTN_PT_SOUND_START	= 0x5007,       //¼����ť
	IDC_BTN_PT_SOUND_STOP	= 0x5008,       //¼����ť
	IDC_BTN_PT_COMMERROR	= 0x5010,       //ͨ��ʧ��
	IDC_BTN_PT_CAPTURE	= 0x5011,           //ץͼ
	IDC_BTN_PT_SOUND	= 0x5012,           //��Ƶ
	IDC_BTN_PT_STREAM	= 0x5013,           //������
	IDC_BTN_PT_OSD	= 0x5014,               //OSD
};

enum  PLAYVIEWTOOLBAR_CMD
{
	PT_CMD_START_RECORD,		
	PT_CMD_STOP_RECORD,
	PT_CMD_DISABLE_RECORD,
	PT_CMD_ENABLE_RECORD
};

// ץͼģʽ
enum PREVIEW_SNAPMODE_DEFINE
{
    PREVIEW_SNAPMODE_BMP	= 0,
    PREVIEW_SNAPMODE_JPG	= 1
};

// ¼������С
enum PREVIEW_RECORDFILESIZE_DEFINE
{
    PREVIEW_RECORDFILESIZE_MAX      = 0,
    PREVIEW_RECORDFILESIZE_64M		= 1,
    PREVIEW_RECORDFILESIZE_128M		= 2,
    PREVIEW_RECORDFILESIZE_256M		= 4,
    PREVIEW_RECORDFILESIZE_512M		= 8
};

// ������
enum _ENUM_UI_STYLE
{
    UI_STYLE_RED = 0,               ///< ��ڷ��
    UI_STYLE_BLUE,                  ///< ���׷��
};

// ����ɫֵ����
enum _ENUM_UI_COLOR_TYPE
{
    UI_COLOR_TYPE_LIGHT = 0,        ///< ���汳��ǳɫ
    UI_COLOR_TYPE_DEEP,             ///< ���汳����ɫ
    UI_COLOR_TYPE_PV,               ///< Ԥ��������汳��ɫ
    UI_COLOR_TYPE_RW,               ///< Ԥ�����ڱ���ɫ
    UI_COLOR_TYPE_RW_BORDER,        ///< Ԥ�����ڱ߿���ɫ
    UI_COLOR_TYPE_UIDIALOG,         ///< UIDialog����ɫ
    UI_COLOR_TYPE_UIDIALOG_TITLE,   ///< UIDialog����������ɫ
    UI_COLOR_TYPE_STATISTIC,        ///< ͳ�ƴ��屳��ɫ
    UI_COLOR_TYPE_TREE_BKG,         ///< ������ɫ
    UI_COLOR_TYPE_LIST_BKG,         ///< �б���ɫ
    UI_COLOR_TYPE_LIST_LIGHT,       ///< �б������б���ɫ
    UI_COLOR_TYPE_LIST_DEEP,        ///< ���ż���б���ɫ
};

// ʵʱ����������ʾ���ڶ���
enum CTRL_REALWAVE_DLG_DEFINE
{
    CTRL_REALWAVE_DLG_MINI_HEIGHT = 24,       ///< ������С���߶�
    CTRL_REALWAVE_DLG_RESTOR_HEIGHT = 108     ///< ���ڻ�ԭ�߶�
};

//�˵�ID
enum
{
	MENU_ID_SOUND			= 1,
	MENU_ID_STOP_PREVIEW ,
	MENU_ID_CAPTURE,
	MENU_ID_RECORD,
	MENU_ID_STREAMTYPE,
	MENU_ID_OSD
};


//////////////////////////////////////////////////////////////////////////
// Struct

//ץͼ������Ϣ
typedef struct tagCapturePicParams
{
    TCHAR szCappicPath[MAX_PATH];
    int  nCappicMode;
    BOOL bManualSave;
    tagCapturePicParams()
    {
        ZeroMemory(szCappicPath, sizeof(TCHAR)*MAX_PATH);
        nCappicMode = PREVIEW_SNAPMODE_BMP;
        bManualSave = FALSE;
    }
}SNAP_CFGINFO, *LPSNAP_CFGINFO;

//¼��������Ϣ
typedef struct tagRecordVideoParams
{
    TCHAR szRecordPath[MAX_PATH];
    int   nRecordFileSize;
    BOOL  bMaxRecordTime;		//�Ƿ������ļ��ʱ��
    int	  nMaxRecordTime;		//�����ļ��ʱ�� �Է��ӵ�λ,5-60����

    tagRecordVideoParams()
    {
        ZeroMemory(szRecordPath, sizeof(TCHAR)*MAX_PATH);
        nRecordFileSize = PREVIEW_RECORDFILESIZE_64M;
        bMaxRecordTime = FALSE;
        nMaxRecordTime = 5;
    }
}RECORD_CFGINFO, *LPRECORD_CFGINFO;

typedef struct tagComInfo
{
    int nComPort;
    tagComInfo()
    {
        nComPort = 0;
    }
}COM_INFO,*PCOMINFO;

//Ԥ��������Ϣ
typedef struct tagLocalCfg
{
    SNAP_CFGINFO stSnapInfo;
    RECORD_CFGINFO stRecordInfo;
    COM_INFO stCom;
}LOCAL_CFG, *PLOCAL_CFG;

//�����Խ���Ϣ�ṹ��
typedef struct _tagTALKINFO
{
    int     nDevType;       //��������	
    int     nChan;			//�Խ�ͨ��	
    int     nPort;			//�豸�˿�
    CString strIP;			//�豸IP
    CString strUser;        //�豸�û���
    CString strPassword;    //�豸�豸����
    CString strName;        //�Խ�ͨ������
    CString strServer;		//�Խ�������IP
    int     nServerPort;    //�Խ��������˿�
    int     nTermNo;        //�ն����
    int     nPanel;         //�ն�����
    BOOL    bExpand;        //�Ƿ�Ϊ����
    _tagTALKINFO()
    {
        nDevType = 0;
        nChan = 0;
        nPort = 0;
        nServerPort = 0;
        nTermNo = 0;
        nPanel = 0;
        bExpand = FALSE;
    }
}TALKINFO;

//�������ṹ��
typedef struct _Struct_Server
{
    int     ID;
    int     nType;
    CString strIP;
    int     nCtrlPort;
    int     nDataPort;
    CString strUser;
    CString strPassword;	// ��   �����������Ϊ �豸��½����
    CString strName;		// ���� ���������-CVR �豸��½����
    int     nSpeRes;
    int     nDogFlag;
    CString strComment;
    int     nMultID;
    int     nMultVer;
    int     nMultSysID;
    int		nAssoServer;
    int     nRev;
    CString strRev;
    _Struct_Server()
    {
        nMultID = 10;
        nMultVer = 0;
        nMultSysID = 0;
    }
}Struct_Server;

// ���ؾ������󼰷�����Ϣ
typedef struct _Struct_VLB_Info
{
    std::string strVlbIP;       // ���ؾ��������IP
    int         nVlbPort;       // ���ؾ��������Port
    int         nSrvID;         // Դ��ý��ID
    std::string strDevIP;       // �����ص��豸IP
    int         nDevChan;       // �����ص��豸ͨ����
    int         nSrvType;       // ���صķ���������
    std::string strSrvIP;       // ���صķ�����IP
    int         nSrvPort;       // ���صķ�����Port
    int         nSymbol;        // ȡ��ID
    _Struct_VLB_Info()
    {
        nVlbPort = 0;
        nSrvID = 0;
        nDevChan = 0;
        nSrvType = 0;
        nSrvPort = 0;
        nSymbol = 0;
    }
    void Clear()
    {
        strVlbIP.clear();
        nVlbPort = 0;
        nSrvID = 0;
        strDevIP.clear();
        nDevChan = 0;
        nSrvType = 0;
        strSrvIP.clear();
        nSrvPort = 0;
        nSymbol = 0;
    }
}Struct_VLB_Info;

// ��ص���Ϣ
typedef struct _Struct_CameraInfo
{
    int         nID;            ///< �����ݿ��е���ˮ��
    std::string strName;        ///< ��ص�����
    int         nDevType;       ///< �豸����
    int         nRegionID;      ///< ����ID
    std::string strRegName;     ///< ��������
    std::string strDevIP;       ///< �豸IP
    int         nDevPort;       ///< �豸Port
    std::string strLoginName;   ///< �豸��½����
    std::string strLoginPwd;    ///< �豸��½����
    int         nChannel;       ///< ͨ����
    int         nRealType;      ///< ��������
    int         nVideoFormat;   ///< ��Ƶ��ʽ
    int         nCamType;       ///< ����ͷ����
    std::string strStreamIP;    ///< ��ý�������IP
    int         nStreamPort;    ///< ��ý�������Port
    int         nStreamSrvID;   ///< ��ý�������ID(�����ø��ؾ���ʱ,IDҪ�������ؾ��������)
    std::string strProxyIP;     ///< ���������IP
    int         nProxyPort;     ///< ���������Port
    std::string strVlbSrvIP;    ///< ���ؾ��������IP
    int         nVlbSrvPort;    ///< ���ؾ��������Port
    std::string strEhStreamIP;  ///< E����ý�������IP
    int         nEhStreamPort;  ///< E����ý�������Port
    std::string strDeviceID;    ///< E���豸���к�
    int         nDevID;         ///< �豸ID
    _Struct_CameraInfo()
    {
        nID = 0;
        nDevType = 0;
        nRegionID = 0;
        nDevPort = 0;
        nChannel = 0;
        nRealType = 0;
        nVideoFormat = 0;
        nCamType = 0;
        nStreamPort = 0;
        nStreamSrvID = 0;
        nProxyPort = 0;
        nVlbSrvPort = 0;
        nEhStreamPort = 0;
        nDevID = 0;
    }
    void Clear()
    {
        nID = 0;
        strName.clear();
        nDevType = 0;
        nRegionID = 0;
        strRegName.clear();
        strDevIP.clear();
        nDevPort = 0;
        strLoginName.clear();
        strLoginPwd.clear();
        nChannel = 0;
        nRealType = 0;
        nVideoFormat = 0;
        nCamType = 0;
        strStreamIP.clear();
        nStreamPort = 0;
        nStreamSrvID = 0;
        strProxyIP.clear();
        nProxyPort = 0;
        strVlbSrvIP.clear();
        nVlbSrvPort = 0;
        strEhStreamIP.clear();
        nEhStreamPort = 0;
        strDeviceID.clear();
        nDevID = 0;
    }
}Struct_CameraInfo, *PStruct_CameraInfo;

#endif