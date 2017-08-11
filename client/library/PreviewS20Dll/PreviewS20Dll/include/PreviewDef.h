/**	@file    PreviewDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   预览Dll全局数据定义
 *
 *	@author	 Liu Ronghua
 *	@date	 2013/01/04
 *	@note   
 *	@note    历史记录：
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
//带宽控制-关闭预览消息
#define WM_BW_ENFORCE_STOP_PREVIEW  (WM_USER + 1056)
//带宽控制-重新请求预览消息
#define WM_BW_REREQUEST_PREVIEW     (WM_USER + 1057)

#define WM_PV_REALWND_NET_EXCEPTION (WM_USER + 1058)
#define WM_PV_TOOLBAR_BTN_CLK_REF	(WM_USER + 1059)
#define WM_PV_REALPLAY_STOPPREVIEW  (WM_USER + 1060)
#define WM_PV_TOOLBAR_EXECUTE       (WM_USER + 1061)

//////////////////////////////////////////////////////////////////////////
// String

#define STR_PREVIEW_CONNECT_FAIL                _T("连接失败，正在重新连接...")
#define STR_PREVIEW_RECONNECT_FAIL				_T("重连失败!")
#define STR_PREVIEW_NETWORK_EXCEPTION			_T("网络异常,请稍后再试...")
#define STR_PREVIEW_RECONNECT_OK				_T("重连成功")
#define STR_PREVIEW_CONNECT_OK					_T("连接成功")
#define STR_PREVIEW_RECONNECT_OVER				_T("重连失败!")
#define STR_PREVIEW_STATUS_NULL					_T("")
#define STR_POPUPMENU_STOPPREVIEW				_T("停止预览")
#define STR_POPUPMENU_SNAPSHOT					_T("预览抓图")
#define STR_POPUPMENU_TALK_START				_T("语音对讲")
#define STR_POPUPMENU_TALK_STOP					_T("关闭对讲")
#define STR_POPUPMENU_OPENSOUND					_T("开启音频")
#define STR_POPUPMENU_CLOSESOUND				_T("关闭音频")
#define STR_POPUPMENU_STARTRECORD				_T("开始录像")
#define STR_POPUPMENU_STOPRECORD				_T("停止录像")
#define STR_POPUPMENU_EVENTRECORD				_T("事件标签")
#define STR_POPUPMENU_MAIN_STREAM			    _T("主码流")
#define STR_POPUPMENU_SUB_STREAM			    _T("子码流")
#define STR_DRAWTEXT_RECORDING                  _T("●录像中,录像打包长度%dM")
#define STR_DRAWTEXT_TALKING                    _T("●对讲中...")
#define STR_DRAWTEXT_OPENSOUDN                  _T("●音频开启中...")
#define STR_DRAWTEXT_FONT						_T("华文楷体")
#define STR_DRAWTEXT_DISK_INFFICIENT            _T("磁盘空间不足,请停止录像")
#define STR_PREVIEW_TOOLBAR_FULLSCREEN			_T("全屏")
#define STR_PREVIEW_TOOLBAR_ARRANGE1			_T("一画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE4			_T("四画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE6			_T("六画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE8			_T("八画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE9			_T("九画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE10			_T("十画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE13			_T("十三画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE14			_T("十四画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE16			_T("十六画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE17			_T("十七画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE22			_T("二十二画面")
#define STR_PREVIEW_TOOLBAR_ARRANGE25			_T("二十五画面")
#define STR_PREVIEW_TOOLBAR_ARRANGEMORE			_T("画面分割")
#define STR_PREVIEW_TOOLBAR_STOPALL				_T("停止所有预览")
#define STR_PREVIEW_TOOLBAR_SNAPSHOTALL			_T("全部抓图")
#define STR_PREVIEW_TOOLBAR_CONFIG              _T("本地配置")
#define STR_PREVIEW_TOOLBAR_PREGROUPCFG			_T("预览分组配置")
#define STR_PREVIEW_TOOLBAR_BROADCAST			_T("广播")
#define STR_PREVIEW_TOOLBAR_CLUSTERSHOCK	    _T("群震")
#define STR_PREVIEW_TOOLBAR_PREVIOUS			_T("上一页")
#define STR_PREVIEW_TOOLBAR_NEXT				_T("下一页")
#define STR_PREVIEW_TOOLBAR_STARTRECORD			_T("开始录像")
#define STR_PREVIEW_TOOLBAR_STOPRECORD			_T("停止录像")
#define STR_PREVIEW_TOOLBAR_STARTPREVIEW			_T("开始预览")
#define STR_PREVIEW_TOOLBAR_STOPPREVIEW			_T("停止预览")
#define STR_PREVIEW_TOOLBAR_STARTSOUND			_T("开启声音")
#define STR_PREVIEW_TOOLBAR_STOPSOUND			_T("关闭声音")
#define STR_POPUPMENU_OSDINFO				    _T("OSD信息")
#define STR_POPUPMENU_SHOWOSD			    	_T("显示水印")
#define STR_POPUPMENU_HIDEOSD				    _T("隐藏水印")
#define STR_POPUPMENU_SHOW4OSD			    	_T("4行显示")
#define STR_POPUPMENU_SHOW8OSD				    _T("8行显示")

//////////////////////////////////////////////////////////////////////////
// Color

// 红黑系
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

// 蓝白系
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
#define UI_FONT_BAR_NAME            _T("宋体")
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

// 预览界面窗口定义
enum PREVIEWWND_DEFINE
{
    ID_WND_PLAYVIEWWND			= 0x2000,   ///< 预览窗口
    ID_WND_PLAYVIEW_CHILD		= 0x2001,   ///< 播放子窗口ID
    ID_WND_PLAYVIEWTOOLBAR		= 0x2002,   ///< 工具条窗口
    ID_WND_PLAYVIEW				= 0x2003,   ///< 预览播放窗口
    ID_WND_GROUPPLAYVIEW		= 0x2004,   ///< 分组轮巡预览窗口
    ID_WND_GROUPPLAYTOOLBAR		= 0x2005,   ///< 分组轮巡工具条
    ID_WND_PLAYVIEW_LAYOUTMORE  = 0x2006,   ///< 扩展分割模式		
    ID_WND_PLAYVIEW_FLOWWND		= 0x2007,   ///< 监控视图按钮弹出的浮动窗口
    ID_WND_TVWALL_FLOWWND		= 0x2008,   ///< 监视屏组按钮弹出的浮动窗口
};

// 自定义控件大小
enum CTRL_CUSTOMSIZE
{
    CTRL_BIGBTN_HEIGHT			= 50,       //自定义按钮大小
    CTRL_EDIT_NORMAL_WIDTH		= 25,       //编辑框
    CTRL_TREE_NORMAL_WIDTH		= 245,      //树控件
    CTRL_TREE_NODE_HEIGHT		= 20,
    CTRL_TREE_SMALL_WIDTH		= 200,
    CTRL_SPACE_LITTLE			= 2,        //控件间距
    CTRL_SPACE_NORMAL			= 5,
    CTRL_BORD_LITTLE			= 3,        //细边框
    CTRL_PREVIEW_TOOLBAR_HEIGHT	= 45,        //预览工具条高度
};

// 工具条按钮ID
enum PREVIEWWND_PLAYVIEWTOOLBAR_BTNDEFINE
{
    IDC_BTN_PT_ARRANGE		= 0x2010,       //画面分割
    IDC_BTN_PT_ARRANGE_1	= 0x2011,       //画面分割1
    IDC_BTN_PT_ARRANGE_4	= 0x2012,       //画面分割2*2
    IDC_BTN_PT_ARRANGE_9	= 0x2013,       //画面分割3*3
    IDC_BTN_PT_ARRANGE_16	= 0x2014,       //画面分割4*4
    IDC_BTN_PT_FULLSCREEN	= 0x2015,       //多画面全屏
    IDC_BTN_PT_STOPALL		= 0x2016,       //停止全部预览
    IDC_BTN_PT_GROUPCFG		= 0x2017,       //预览分组配置
    IDC_BTN_PT_SNAPSHOT		= 0x2018,       //抓图
    IDC_BTN_PT_PRECONFIG	= 0x2019,       //预览配置
    IDC_BTN_PT_BROADCAST	= 0x2020,       //广播
    IDC_BTN_PT_CLUSTERSHOCK	= 0x2021,       //群震
    IDC_BTN_PT_PREVIOUSPAGE = 0x4000,       //上一页
    IDC_BTN_PT_NEXTPAGE		= 0x4001,       //下一页
	IDC_BTN_PT_STARTRECORD	= 0x5000,       //开始录像
	IDC_BTN_PT_STOPRECORD	= 0x5001,       //停止录像
	IDC_STC_PT_VIDEOWAV 	= 0x5002,       //录像波形
	IDC_STC_PT_AUDIOWAV 	= 0x5003,       //录音波形
	IDC_BTN_PT_VIDEOWAVE	= 0x5004,       //开始预览
	IDC_BTN_PT_PHONEWAVE	= 0x5005,       //录音按钮
	IDC_BTN_PT_VIDEOWAVE_STOP	= 0x5006,   //停止预览
	IDC_BTN_PT_SOUND_START	= 0x5007,       //录音按钮
	IDC_BTN_PT_SOUND_STOP	= 0x5008,       //录音按钮
	IDC_BTN_PT_COMMERROR	= 0x5010,       //通信失败
	IDC_BTN_PT_CAPTURE	= 0x5011,           //抓图
	IDC_BTN_PT_SOUND	= 0x5012,           //音频
	IDC_BTN_PT_STREAM	= 0x5013,           //清晰度
	IDC_BTN_PT_OSD	= 0x5014,               //OSD
};

enum  PLAYVIEWTOOLBAR_CMD
{
	PT_CMD_START_RECORD,		
	PT_CMD_STOP_RECORD,
	PT_CMD_DISABLE_RECORD,
	PT_CMD_ENABLE_RECORD
};

// 抓图模式
enum PREVIEW_SNAPMODE_DEFINE
{
    PREVIEW_SNAPMODE_BMP	= 0,
    PREVIEW_SNAPMODE_JPG	= 1
};

// 录像打包大小
enum PREVIEW_RECORDFILESIZE_DEFINE
{
    PREVIEW_RECORDFILESIZE_MAX      = 0,
    PREVIEW_RECORDFILESIZE_64M		= 1,
    PREVIEW_RECORDFILESIZE_128M		= 2,
    PREVIEW_RECORDFILESIZE_256M		= 4,
    PREVIEW_RECORDFILESIZE_512M		= 8
};

// 界面风格
enum _ENUM_UI_STYLE
{
    UI_STYLE_RED = 0,               ///< 红黑风格
    UI_STYLE_BLUE,                  ///< 蓝白风格
};

// 界面色值类型
enum _ENUM_UI_COLOR_TYPE
{
    UI_COLOR_TYPE_LIGHT = 0,        ///< 界面背景浅色
    UI_COLOR_TYPE_DEEP,             ///< 界面背景深色
    UI_COLOR_TYPE_PV,               ///< 预览管理界面背景色
    UI_COLOR_TYPE_RW,               ///< 预览窗口背景色
    UI_COLOR_TYPE_RW_BORDER,        ///< 预览窗口边框颜色
    UI_COLOR_TYPE_UIDIALOG,         ///< UIDialog背景色
    UI_COLOR_TYPE_UIDIALOG_TITLE,   ///< UIDialog标题文字颜色
    UI_COLOR_TYPE_STATISTIC,        ///< 统计窗体背景色
    UI_COLOR_TYPE_TREE_BKG,         ///< 树背景色
    UI_COLOR_TYPE_LIST_BKG,         ///< 列表背景色
    UI_COLOR_TYPE_LIST_LIGHT,       ///< 列表奇数行背景色
    UI_COLOR_TYPE_LIST_DEEP,        ///< 类别偶数行背景色
};

// 实时声音波形显示窗口定义
enum CTRL_REALWAVE_DLG_DEFINE
{
    CTRL_REALWAVE_DLG_MINI_HEIGHT = 24,       ///< 窗口最小化高度
    CTRL_REALWAVE_DLG_RESTOR_HEIGHT = 108     ///< 窗口还原高度
};

//菜单ID
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

//抓图配置信息
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

//录像配置信息
typedef struct tagRecordVideoParams
{
    TCHAR szRecordPath[MAX_PATH];
    int   nRecordFileSize;
    BOOL  bMaxRecordTime;		//是否限制文件最长时间
    int	  nMaxRecordTime;		//限制文件最长时间 以分钟单位,5-60分钟

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

//预览配置信息
typedef struct tagLocalCfg
{
    SNAP_CFGINFO stSnapInfo;
    RECORD_CFGINFO stRecordInfo;
    COM_INFO stCom;
}LOCAL_CFG, *PLOCAL_CFG;

//语音对讲信息结构体
typedef struct _tagTALKINFO
{
    int     nDevType;       //厂家类型	
    int     nChan;			//对讲通道	
    int     nPort;			//设备端口
    CString strIP;			//设备IP
    CString strUser;        //设备用户名
    CString strPassword;    //设备设备密码
    CString strName;        //对讲通道名称
    CString strServer;		//对讲服务器IP
    int     nServerPort;    //对讲服务器端口
    int     nTermNo;        //终端序号
    int     nPanel;         //终端面板号
    BOOL    bExpand;        //是否为外设
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

//服务器结构体
typedef struct _Struct_Server
{
    int     ID;
    int     nType;
    CString strIP;
    int     nCtrlPort;
    int     nDataPort;
    CString strUser;
    CString strPassword;	// 在   虚拟服务器下为 设备登陆密码
    CString strName;		// 用于 虚拟服务器-CVR 设备登陆名称
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

// 负载均衡请求及返回信息
typedef struct _Struct_VLB_Info
{
    std::string strVlbIP;       // 负载均衡服务器IP
    int         nVlbPort;       // 负载均衡服务器Port
    int         nSrvID;         // 源流媒体ID
    std::string strDevIP;       // 请求负载的设备IP
    int         nDevChan;       // 请求负载的设备通道号
    int         nSrvType;       // 返回的服务器类型
    std::string strSrvIP;       // 返回的服务器IP
    int         nSrvPort;       // 返回的服务器Port
    int         nSymbol;        // 取流ID
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

// 监控点信息
typedef struct _Struct_CameraInfo
{
    int         nID;            ///< 在数据库中的流水号
    std::string strName;        ///< 监控点名称
    int         nDevType;       ///< 设备类型
    int         nRegionID;      ///< 区域ID
    std::string strRegName;     ///< 区域名称
    std::string strDevIP;       ///< 设备IP
    int         nDevPort;       ///< 设备Port
    std::string strLoginName;   ///< 设备登陆名称
    std::string strLoginPwd;    ///< 设备登陆密码
    int         nChannel;       ///< 通道号
    int         nRealType;      ///< 播放类型
    int         nVideoFormat;   ///< 视频格式
    int         nCamType;       ///< 摄像头类型
    std::string strStreamIP;    ///< 流媒体服务器IP
    int         nStreamPort;    ///< 流媒体服务器Port
    int         nStreamSrvID;   ///< 流媒体服务器ID(当启用负载均衡时,ID要传给负载均衡服务器)
    std::string strProxyIP;     ///< 代理服务器IP
    int         nProxyPort;     ///< 代理服务器Port
    std::string strVlbSrvIP;    ///< 负载均衡服务器IP
    int         nVlbSrvPort;    ///< 负载均衡服务器Port
    std::string strEhStreamIP;  ///< E家流媒体服务器IP
    int         nEhStreamPort;  ///< E家流媒体服务器Port
    std::string strDeviceID;    ///< E家设备序列号
    int         nDevID;         ///< 设备ID
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