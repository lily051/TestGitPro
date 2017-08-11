
#pragma once

/*自定义消息*/
//
#define WM_UPDATE_CONFIG		(WM_USER + 1998)
#define WM_LAYOUT_BTN           (WM_USER + 1999)
#define WM_ADD_MODULE           (WM_USER + 2000)
#define WM_DEL_MODULE           (WM_USER + 2001)
#define WM_HEAD_MODULE          (WM_USER + 2002)

#define MSG_COMPONENT_UP        (WM_USER + 2003)     // 组件传上来的消息
#define MSG_TOOLBTN_CLOSE       (WM_USER + 2004)     // 按钮关闭，通知父窗口关闭组件
#define MSG_TOOLBTN_SELECT      (WM_USER + 2005)     // 按钮点击，通知父窗口显示窗口
#define MSG_WND_SHOW            (WM_USER + 2006)     // 窗口自动显示时的通知消息
#define WM_SYS_UNLOCK           (WM_USER + 2007)     // 解锁消息
#define MSG_CFGCHANGE_NOTIFY    (WM_USER + 2008)     // 组件配置更改通知消息
#define MSG_TITLE_NOTIFYICON    (WM_USER + 2009)     // 组件在第二窗口打开,通知在标题栏显示小图标
#define MSG_NETCONNECT_NOTIFY   (WM_USER + 2010)     // 通信连接通知网络连接标识wnd
#define MSG_TOOLBTN_BLINK       (WM_USER + 2011)     // 通知主窗口组件按钮闪烁
#define MSG_FLASH_WINDOW        (WM_USER + 2012)     // 通知主窗口任务栏闪烁
#define MSG_FLASH_VIEWTALK_WINDOW (WM_USER + 2013)   // 通知可视对讲组件窗口刷新（可视对讲组件窗口的父窗口为框架窗口，因此框架窗口最小化时，可视对讲组件窗口会隐藏，因此需要重新刷新可是对讲组件窗口位置，注意是组件窗口哦）
#define MSG_SELECT_COMPONENT    (WM_USER + 2014)     // 通知窗口选择组件
#define MSG_CLOSE_COMPONENT     (WM_USER + 2015)     // 通知窗口关闭组件
#define MSG_ADD_EXE_COMPONENT   (WM_USER + 2016)     // 添加EXE组件
#define MSG_ADD_EXE_COM_FAILED  (WM_USER + 2017)     // 添加EXE组件失败
#define MSG_COM_HEART_REQ       (WM_USER + 2018)     // 心跳请求消息
#define MSG_COM_HEART_RSP       (WM_USER + 2019)     // 心跳回应消息
#define MSG_SECOND_WND          (WM_USER + 2020)     // 第二窗口打开组件
#define MSG_CLOSE_SECOND_WND    (WM_USER + 2021)     // 关闭第二窗口
#define MSG_FORCE_LOGOFF        (WM_USER + 2022)     // 强制下线消息
#define MSG_REBOOT_APP          (WM_USER + 2023)     // 重启消息
#define MSG_EXEMODULE_ALARM     (WM_USER + 2024)     // 模块进程报警消息
#define MSG_USER_CLOSE          (WM_USER + 2025)     // 接受其他程序发来的退出消息, [ID不要修改！]
#define WM_ON_ALARM_NET_DISCONNT	(WM_USER + 2026) //网络连接状态消息
#define MSG_SHOW_VIEWTALK       (WM_USER + 2030)     // 显示可视对讲窗口消息
#define MSG_START_LOCK          (WM_USER + 2031)     // EXE模块开启锁定
#define MSG_UNLOCK_SUCCESS      (WM_USER + 2032)     // EXE模块解锁成功

#define WM_PV_STARTRECORD	(WM_USER + 2500)		//控件录像开始消息
#define WM_PV_STOPRECORD		(WM_USER + 2501)		//控件录像停止消息

#define WM_SELECT_REGION				(WM_USER + 2510)		//选择区域消息		
#define WM_SELECT_PRODUCTTYPE		(WM_USER + 2511)		//选择产品类型消息
#define WM_SELECT_PRODUCT				(WM_USER + 2512)		//选择产品消息


// 主窗口的默认大小
#define  DEFAULT_WND_SIZE   CRect(0, 0, 900, 600)

//业务记录
#define BUSINESS_WND_SIZE CRect(0, 0, 898, 536)
//历史业务单窗口大小
#define HISTORYLIST_WND_SIZE CRect(0, 0, 758, 460)

//话术模板窗口大小
#define WORD_TEMPLATE_WND_SIZE CRect(0, 0, 530, 336)

//预览窗口大小
#define PREVIEW_WND_SIZE	CRect(0, 0, 400, 260)

//回放窗口大小
#define PLAYBACK_WND_SIZE CRect(0, 0, 800, 500)

//离线客户端窗口大小
#define WORD_TEMPLATE_WND_SIZE CRect(0, 0, 530, 336)

#define CHANGE_PWD_WND_SIZE CRect(0, 0, 350, 260)

#define EDIT_FILENAME_WND_SIZE CRect(0, 0, 350, 150)

#define PLAYBACK_WND_TOOLBAR_HEIGHT 35

// 主窗口四周边框大小
#define MARGIN_TOP      40
#define MARGIN_LEFT     0
#define MARGIN_RIGHT    0
#define MARGIN_BOTTOM   0

#define MARGIN_TOP_DLG      35
#define MARGIN_LEFT_DLG     1
#define MARGIN_RIGHT_DLG    1
#define MARGIN_BOTTOM_DLG   1

#define BOTTOM_HEIGHT   28
// 主窗口导航栏的高度
#define NAVIBAR_HEIGHT  36

// 
#define BASE_LAYER_ID   102

// 首页右侧信息栏的宽度（固定）
#define EXTINFO_WIDTH   320

// 公告窗口高度
#define BBS_HEIGHT      345

// 程序字体
#define YAHEI_18_N      _T("Yahei18N")     // 雅黑, 18, NORMAL
#define YAHEI_20_N      _T("Yahei20N")     // 雅黑, 20, 
#define YAHEI_42_N      _T("Yahei42N")     // 雅黑, 30, NORMAL

// 颜色
const DWORD COLOR_BLUE = RGB(63, 133, 228);

// 获取授权中子系统类型
#define SYS_TYPE_CONFIG     1
#define SYS_TYPE_8100       1
#define SYS_TYPE_ACSUBSYS   2

// 登录历史保存文件
#define LOGIN_FILE  _T("\\Config\\Login.xml")

#define CENTRALCLIENT_FILEMAP   _T("CentralClientMapFile")  // 共享内存名
#define MUTEX_INSTANCE_NAME     _T("FMSClient")         // 进程唯一标识互斥对象名

// 字符长度定义
#define LEN_IP        16    // IP地址长度
#define LEN_USER_NAME 64    // 用户名长度

// 系统菜单
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

// 组件ID
enum COMPONENT_ID
{
    MENUID_INVALID = -1,    // 无效的组件ID，在检查时用到
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

// 客户端配置信息
typedef struct __sys_config 
{
	typedef struct __base_config 
	{
		BOOL bRemPwd;		// 保存密码
		BOOL bAutoLogin;	// 自动登录
		int  nNetTimeout;	// 网络超时时间 0-15s, 2-20s, 3-30s, 4-45s
		BOOL bShowLogo;		// 显示Logo
		BOOL bShowCpRight;	// 显示版权
		BOOL bShowNetInfo;	// 显示网络信息
		BOOL bShowDate;		// 显示日期
		BOOL bShowTime;		// 显示时间
		BOOL bShowWeek;		// 显示星期

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
		int nPicFormat;			// 抓图格式 0-JPG, 1-BMP
		CString strPicPath;		// 抓图路径
		int nRecSize;			// 录像大小 0-64M, 1-128M, 2-256M, 3-512M
		CString strRecPath;		// 录像路径
		BOOL bAutoStreamType;	// 自动码流类型
		BOOL bMainStreamOne;	// 单画面主码流
		BOOL bDirectDevice;		// 直连设备
		int nDirectStreamType;	// 直连设备时码流类型 0-平台， 1-主， 2-子
		int nDecodeBuffer;		// 解码缓存 0-512K, 1-1M, 2-2M, 3-3M
		int nDecodeFrame;		// 解码帧数 ?-?帧
		BOOL bShowIntel;		// 显示智能规则
		BOOL bShowInfo;			// 显示监控点信息
		BOOL bShowCardNo;		// 显示卡号

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
		int nPicFormat;			// 抓图格式 0-JPG, 1-BMP
		CString strPicPath;		// 抓图路径
		CString strRecPath;		// 录像路径
		BOOL bAutoNextWnd;		// 光标自动下移
		BOOL bShowFileWnd;		// 显示文件列表窗口
		BOOL bShowVideoTitle;	// 显示录像标题

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


// 组件加载方式
enum _e_component_load_type
{
    TYPE_DLL = 0,   // DLL方式
    TYPE_EXE = 1,   // EXE方式
    TYPE_HOME = 2,  // HOME页
};

// 组件配置属性
typedef struct _s_component_config
{
    int nLoadType;              // 加载方式
    int nHeartBeat;             // 心跳时间
    int nListenPort;            // 监听端口
    CString strKey;             // 关键词
    CString strVersion;         // 最低版本
    CString strFileName;        // 组件文件名
    CString strPreLoad;         // 预加载配置文件

    _s_component_config()
    {
        nLoadType = TYPE_DLL;
        nHeartBeat = 10;
        nListenPort = 0;
    }
}STU_COMPONENT_CONFIG;

// 命令行参数
typedef struct _s_comman_param
{
    int nDcPort;         // 数据库端口
    int nComponentId;    // 模块ID
    CString strKey;      // 模块关键字
    CString strDcIp;     // 数据库IP地址
    CString strUser;     // 用户名
    CString strPassword; // 密码
    CString strLocalIp;  // 本地IP
    CString strFileMapName;  // 共享内存名(用于获取主窗口句柄)

    _s_comman_param()
    {
        nDcPort = 0;
		nComponentId = 0;
    }
}STU_COMMAND_PARAM;

// 预览日志信息
typedef struct _s_oper_Log
{
    int nCamID;                 // 监控点ID
    int nLogType;               // 日志类型
    int nResult;                // 结果
    std::string strRemark;      // 备注
    _s_oper_Log()
    {
        nCamID = 0;
        nLogType = 0;
        nResult = 0;
    }
}STU_OPER_LOG;

#define PV_STREAM_TIMEOUT_MAX 60		//单位为秒
#define PV_STREAM_TIMEOUT_MIN 5			//单位为秒
#define PV_STREAM_TIMEOUT_DEFAULT 10	//单位为秒

#define PB_STREAM_TIMEOUT_MAX 60		//单位为秒
#define PB_STREAM_TIMEOUT_MIN 10		//单位为秒
#define PB_STREAM_TIMEOUT_DEFAULT 60	//单位为秒

#define PV_CYCLE_CAMERA_LIMIT	1000


typedef struct _tagServerInfo    //服务器信息
{
	TCHAR szServerIp[16];                  // 服务器ip
	int nServerPort;                       // 服务器port
	int nComponmentID;                     // 组件ID
	int nServerID;                         // 服务器ID
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
	CMS_SERVER = 1,            //数据中心
	CLIENT_SELF,               //客户端自身
};

enum REQ_MSG_TYPE
{
	REQ_MSG_WAITING_RSP = 0,       //等待返回消息
	REQ_MSG_ASYN_RSP,              //异步返回
	REQ_MSG_NOT_NEED_RSP,          //无需返回消息
};

// 资源树节点类别
enum 
{
	RRTREE_CLASSIFY_REGION,	// 组织
	RRTREE_CLASSIFY_BRANCH,		// 分行
	RRTREE_CLASSIFY_BANK            // 银行
};

#define TREE_DLG_TYPE_REGION				1
#define TREE_DLG_TYPE_PRODUCTTYPE	2

#define RECORD_TYPE_NVR		1  //NVR录像
#define RECORD_TYPE_LOCAL	2 //本地录像

#define SYS_BTN_MENU		601
#define SYS_BTN_USER			602
#define SYS_BTN_CONFIG		700
#define SYS_BTN_LIST			701
#define SYS_BTN_BUSINESS	702

//打印业务单html模板文件关键字段位置
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


