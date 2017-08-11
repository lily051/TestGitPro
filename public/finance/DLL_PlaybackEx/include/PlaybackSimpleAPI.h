/**   @file PlaybackSimpleAPI.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	简单回放组件.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/12/19
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
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


// 权限控制.
#define PBSD_RIGHT_ALL 0xFF			// 所有权限.
#define PBSD_RIGHT_DOWNLOAD 0x01	// 下载权限
#define PBSD_RIGHT_CAPTURE 0x02		// 抓图权限.
#define PBSD_RIGHT_SAVE 0x04		// 保存数据权限.

// 组件风格.
#define PBSD_STYLE_SIMPLE 0x00		// 简洁风格.
#define PBSD_STYLE_COMPLETE 0xFF	// 完整风格.
#define PBSD_STYLE_DOWNLOAD 0x01	// 下载模块.
#define PBSD_STYLE_LOCALPLAYBACK 0x02	// 本地回放.

// 皮肤类型.
typedef enum _PBSDSkinType
{
	PBSD_SKIN_RED = 0,		// 红色.
	PBSD_SKIN_BLUE = 1,		// 蓝色.
}PBSDSkinType;

// 画面分割模式.
typedef enum _PBSDLayout
{
	PBSD_LAYOUT_WND_1 = 1,                     //一画面分屏
	PBSD_LAYOUT_WND_4 = 4,                     //四画面分屏
	PBSD_LAYOUT_WND_9 = 9,                     //九画面分屏
	PBSD_LAYOUT_WND_16= 16,                    //十六画面分屏
}PBSDLayout;

typedef enum _PBSDPlayType
{
	PBSD_TYPE_DVR = 0,
	PBSD_TYPE_CVR,					
	PBSD_TYPE_CLOUD,
    PBSD_TYPE_KMS                       
}PBSDPlayType;

//录像类型
typedef enum _PBSDRecordType
{
	PBSD_RECORD_TYPE_ALL = 0xFF,            //全部
	PBSD_RECORD_TYPE_TIMING = 0,            //定时录像 
	PBSD_RECORD_TYPE_MOVE	= 1,           //移动侦测
	PBSD_RECORD_TYPE_ALARM_TRIGER = 2,      //报警触发
	PBSD_RECORD_TYPE_ALARM_OR_MOVE = 3,     //报警|移动侦测	
	PBSD_RECORD_TYPE_ALARM_AND_MOVE = 4,    //报警&移动侦测
	PBSD_RECORD_TYPE_CMD_TRIGER	= 5,       //命令触发
	PBSD_RECORD_TYPE_MANUAL_RECORD = 6,     //手动录像
}PBSDRecordType;

// 带宽控制客户端类型.
typedef enum _EnumBwClientType
{
	PBSD_BW_CLIENT_TYPE_CS = 0,				// CS客户端.
	PBSD_BW_CLIENT_TYPE_BS,					// BS客户端.
}EnumBwClientType;

//回放参数信息
typedef struct _stPlaybackParam
{
	char szDownloadPath[MAX_PATH];		// 下载路径.
	char szPicPath[MAX_PATH];		// 图片保存路径.
	int nPicType;					// 图片类型,0-BMP, 1-JPG.
	int nShowFileWnd;				// 保留.
	int nShowWndTitle;				// 是否显示标题栏.
	_stPlaybackParam()
	{
        ZeroMemory(szDownloadPath, sizeof(szDownloadPath));
        ZeroMemory(szPicPath, sizeof(szPicPath));
		nPicType = 0;
		nShowFileWnd = 0;
		nShowWndTitle = 1;
	}
}PLAYBACK_PARAM;

//回放配置信息
typedef struct _stPlaybackConfig
{
	PLAYBACK_PARAM stPBParam;
	// 是否初始化代理.
	BOOL bInitProx;
	// 是否初始化PCNVR.
	BOOL bInitNvr;
	// 是否初始化bdp.
	BOOL bInitBdp;
	// 是否初始化FinAudio.
	BOOL bInitFinAudio;

	_stPlaybackConfig()
	{
		bInitProx = TRUE;
		bInitNvr  = TRUE;
		bInitBdp  = TRUE;
		bInitFinAudio = TRUE;
	}
}PLAYBACK_CONFIG;

//dvr/cvr登录信息
typedef struct _PBSDDvrLoginInfo
{
	char szIP[16];                     //ip
	int nPort;                         //端口
	int nDataPort;                     //数据端口
	char szUserName[32];               //用户名
	char szPassword[32];               //密码
	char szIndex[32];                   //云存储唯一标示
	char szRev[160];                    //预留
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

//时间结构体
typedef struct _PBSDTime
{
	DWORD	dwYear;			           //年
	DWORD	dwMonth;		           //月
	DWORD	dwDay;			           //日
	DWORD	dwHour;			           //时
	DWORD	dwMinute;		           //分
	DWORD	dwSecond;		           //秒

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

// 登录类型信息.
typedef struct _PBSDLoginTypeInfo
{
	PBSDDvrLoginInfo stDvrLoginInfo;			// DVR、CVR登陆信息.
}PBSDLoginTypeInfo;

// 登录参数.
typedef struct _PBSDLoginParam
{
	PBSDPlayType emPlayType;         //回放类型.
	int nManType;					 // 厂商类型.
	int nDevType;					 //设备类型
	PBSDLoginTypeInfo unLoginType;
	_PBSDLoginParam()
	{
		emPlayType = PBSD_TYPE_DVR;
		nDevType = 0;
		nManType = 0;
	}
}PBSDLoginParam, *LPPBSDLoginParam;

// 柜员号参数.
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

// 时间回放参数.
typedef struct _PBSDTimeParam
{
	int nCamID;			         // 监控点ID.
	char szCamName[128];         // 监控点名称.
	int nChannel;		         // 通道号
	PBSDTime startTime;          // 开始时间
	PBSDTime stopTime;           // 结束时间
	PBSDRecordType emFileType;   // 查询文件类型.
	BOOL bByCardNum;	         // 是否按卡号查询.
	char szCardNum[32];          // 卡号.
	char szStreamID[32];         // 流ID，只对CVR有效.
	DWORD dwRev;                 // 预留

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

// 柜员回放时间参数.
typedef struct _PBSDTelTimeParam
{
	PBSDTime startTime;		// 开始时间.
	PBSDTime stopTime;		// 结束时间.
	PBSDRecordType emFileType; // 查询文件类型.
	DWORD dwRev;			// 预留字段.

    _PBSDTelTimeParam()
    {
        dwRev = 0;
    }
}PBSDTelTimeParam, *LPPBSDTelTimeParam;

// 文件参数.
typedef struct _PBSDFileData
{
	char szFileName[128];               //文件名
	DWORD dwFileSize;                   //文件大小
	int nFileType;                      //文件类型
	PBSDTime stStartTime;                //起始时间
	PBSDTime stEndTime;                  //结束时间
	PBSDPlayType emPlayType;            //文件存储源
	int nDevType;         //设备类型
    char szPlayUrl[255];                //回放url--过vrm回放需要
	char szRev[32];                     //预留

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

// 文件回放参数.
typedef struct _PBSDFileParam
{
	int nCamID;				// 监控点ID.
	DWORD dwRev;			// 预留字段.
	char szCamName[128];    // 监控点名称.
	int nChannel;			// 通道号.
	char szStreamID[32];    // 流ID.
	PBSDFileData stFileData;	// 文件参数.

    _PBSDFileParam()
    {
        nCamID = 0;
        dwRev = 0;
        ZeroMemory(szCamName, sizeof(szCamName));
		ZeroMemory(szStreamID, sizeof(szStreamID));
        nChannel = 0;
    }
}PBSDFileParam, *LPPBSDFileParam;

// 柜员回放文件参数.
typedef struct _PBSDTelFileParam
{
	PBSDFileData stFileData;	// 文件参数.
	DWORD dwRev;				// 预留字段.

    _PBSDTelFileParam()
    {
        dwRev = 0;
    }
}PBSDTelFileParam, *LPPBSDTelFileParam;

// 回放参数类型信息.
typedef struct _PlayTypeParam
{
	PBSDTimeParam stTimePram;			// 时间回放参数.
	PBSDFileParam stFileParam;			// 文件回放参数.
}PlayTypeParam, *LPPlayTypeParam;

// 柜员回放参数类型信息.
typedef struct _PlayTypeTelParam
{
	PBSDTelTimeParam stTimePram;		// 时间回放参数.
	PBSDTelFileParam stFileParam;			// 文件回放参数.
}PlayTypeTelParam, *LPPlayTypeTelParam;

// 回放参数.
typedef struct _PBSDPlayParam
{
	int nType;								// 回放方式,0-按时间,1-按文件.
	LONG lBwControl;                        // 是否支持带宽控制
	PlayTypeParam unPlayParam;

    _PBSDPlayParam()
    {
        nType = 0;
		lBwControl = 0;
    }
}PBSDPlayParam, *LPPBSDPlayParam;

// 柜员号回放参数.
typedef struct _PBSDTelPlayParam
{
	int nType;								// 回放方式，0-按时间，1-按文件.
	PlayTypeTelParam unPlayParam;	

    _PBSDTelPlayParam()
    {
        nType = 0;
    }
}PBSDTelPlayParam, *LPPBSDTelPlayParam;

// 查找文件参数.
typedef struct _PBSDFindParam
{
	PBSDRecordType emFileType;							// 文件参数.
	PBSDTimeParam stTimeParam;				// 时间参数.
}PBSDFindParam, *LPPBSDFindParam;

// 菜单回调数据.
typedef struct _MenuCBData
{
	PBSDLoginParam stLogin;					// 登录参数.
	LPVOID pInfo;							// 相关参数.

    _MenuCBData()
    {
        pInfo = NULL;
    }
}MenuCBData, *LPMenuCBData;

// 事件标签信息.
typedef struct _EventLabelData
{
	int nCamID;								// 监控点ID.
	char szFileName[128];					// 文件名.
	PBSDTime stStartTime;					// 开始时间.
	PBSDTime stStopTime;					// 结束时间.

    _EventLabelData()
    {
        nCamID = 0;
        ZeroMemory(szFileName, sizeof(szFileName));
    }
}EventLabelData, *LPEventLabelData;

// 回放状态信息.
typedef struct _PlaybackStatData
{
	BOOL bIsPlaying;						// 是否正在回放中.
	BOOL bIsDownloading;					// 是否正在下载中.
	BOOL bIsSaving;							// 是否正在剪辑中.
	int nCamID;								// 监控点ID.
	DWORD dwRev;							// 保留字段.

    _PlaybackStatData()
    {
        bIsPlaying = FALSE;
        bIsDownloading = FALSE;
        bIsSaving = FALSE;
        nCamID = 0;
        dwRev = 0;
    }
}PlaybackStatData, *LPPlaybackStatData;

// 日志信息结构体.
typedef struct _WorkLogData
{
	int nCamID;			// 监控点ID.
	DWORD dwRev;		// 保留字段，8120用此字段做唯一标识,对上层传入的检索ID不做处理原样返回.
	DWORD dwLogType;	// 日志类型.626012-回放开始, 626013-回放停止, 626002-回放抓图, 626016-剪辑开始, 626017-剪辑停止, 626014-下载开始, 626015-下载停止
	int nResult;		// 结果， 0-失败，1-成功.
	char szRemark[512];	// 备注.
	DWORD dwParamUp;	// dwLogType为626014或者626015时有效，此值为录像开始时间.
	DWORD dwParamLow;	// dwLogType为626014或者626015时有效，此值为录像结束时间.
	DWORD dwRet;		// dwLogType为626015时有效,1-下载成功,2-下载失败.

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

//传入组件的数据信息
typedef struct _PBSDUserInfo
{
	char szLoginUser[32];     //客户端登录用户
	int nUserID;               //用户ID
	int nUserLevel;            //用户级别
	int nRoleID;               //客户端登录用户角色ID
	_PBSDUserInfo()
	{
		ZeroMemory(szLoginUser, sizeof(szLoginUser));
		nUserID = -1;
		nUserLevel = 0;
		nRoleID = -1;
	}
}PBSDUserInfo, *LPPBSDUserInfo;

// CMS数据.
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

// 客户端信息.
typedef struct _PBSDClientData
{
	EnumBwClientType emClientType;			// 客户端类型.
	char szDes[128];						// 客户端描述， 如“接警中心”.

    _PBSDClientData()
    {
        ZeroMemory(szDes, sizeof(szDes));
        emClientType = PBSD_BW_CLIENT_TYPE_CS;
    }
}PBSDClientData, *LPPBSDClientData;

// 功能权限.
typedef struct _PBSDRights
{
	BOOL bCapture;
	BOOL bMontage;
	_PBSDRights()
	{
		bCapture = TRUE;  //抓图权限
		bMontage = TRUE;  //剪辑权限
	}
}PBSDRights, *LPPBSDRights;

/**   @fn          PBSD_PlaybackCB
 *    @brief   	   回放回调函数，返回文件报文.
 *    @param[in]   nWndID:窗口号. 
 *    @param[in]   strXML:文件报文.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
typedef void (CALLBACK * PBSD_PlaybackCB)(int nWndID, CString strXML, LPVOID pUserData);

/**   @fn          PBSD_MenuCB
 *    @brief   	   菜单回调函数.	
 *    @param[in]   WM_USER + 1802:写事件标签,stCBData.pInfo为EventLabelData
 *    @param[in]   
 *    @return      
 */
typedef void (CALLBACK * PBSD_MenuCB)(int nMenuID, MenuCBData stCBData, LPVOID pUserData);

/**   @fn          PBSD_WorkLogCB
 *    @brief   	   写操作日志回调.
 *    @param[in]   pLogData:日志信息结构体.
 *    @param[in]   pUserData:用户自定义数据.
 *    @return      
 */
typedef void (CALLBACK * PBSD_WorkLogCB)(LPWorkLogData pLogData, LPVOID pUserData);

/**   @fn          PBSD_Init
 *    @brief   	   初始化.
 *    @param[in]   emSkinType:皮肤类型.
 *    @param[in]   dwStyle:组件风格.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_Init(PBSDSkinType emSkinType = PBSD_SKIN_RED, DWORD dwStyle = PBSD_STYLE_SIMPLE, const CString csRights = _T("11111111"));

/**   @fn          PBSD_UnInit
 *    @brief   	   反初始化.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
PBSD_API BOOL PBSD_UnInit();

/**   @fn          PBSD_Create
 *    @brief   	   创建窗口.
 *    @param[in]   hWnd:父窗口.
 *    @param[in]   emLayout:画面分割模式.
 *    @return      HANDLE:回放窗口唯一标识,NULL表示失败.
 */
PBSD_API HANDLE PBSD_Create(HWND hWnd, PBSDLayout emLayout = PBSD_LAYOUT_WND_1);

/**   @fn          PBSD_Destroy
 *    @brief   	   销毁窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
PBSD_API BOOL PBSD_Destroy(HANDLE hDlg);

/**   @fn          PBSD_MoveWindow
 *    @brief   	   移动窗口.
 *    @param[in]   pRect:窗口大小.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @return      TRUE:成功，FALSE:失败.
 */
PBSD_API BOOL PBSD_MoveWindow(HANDLE hDlg, LPCRECT pRect);

/**   @fn          PBSD_ShowWindow
 *    @brief   	   显示隐藏窗口.
 *    @param[in]   bShow:TRUE-显示,FALSE-隐藏.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_ShowWindow(HANDLE hDlg, BOOL bShow);

/**   @fn          PBSD_PlaybackByWndNo
 *    @brief   	   在指定窗口进行回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pLoginParam:登录参数.
 *	  @param[in]   nWndNo:窗口号.
 *    @param[in]   pPlayParam:回放参数.
 *    @param[in]   dwRight:权限控制.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByWndNo(HANDLE hDlg, 
								   int nWndNo, 
								   LPPBSDLoginParam pLoginParam, 
								   LPPBSDPlayParam pPlayParam, 
								   DWORD dwRight = PBSD_RIGHT_ALL);

/**   @fn          PBSD_LocalPlayback
 *    @brief   	   本地回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   nVideoType:录像类型，0-海康
 *    @param[in]   csFilePath:本地文件路径.
 *    @return      
 */
PBSD_API BOOL PBSD_LocalPlayback(HANDLE hDlg, const int nVideoType, const CString csFilePath);

/**   @fn          PBSD_PlaybackOnSelWnd
 *    @brief   	   在光标选中的窗口进行回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pLoginParam:登录参数.
 *    @param[in]   pPlayParam:回放参数.
 *    @param[in]   dwRight:权限控制.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_PlaybackOnSelWnd(HANDLE hDlg, 
									LPPBSDLoginParam pLoginParam, 
									LPPBSDPlayParam pPlayParam, 
									DWORD dwRight = PBSD_RIGHT_ALL);

/**   @fn          PBSD_PlaybackByWndNo
 *    @brief   	   在指定窗口进行回放.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pLoginParam:登录参数.
 *	  @param[in]   nWndNo:窗口号,从0开始，-1表示光标所在窗口..
 *    @param[in]   pPlayParam:回放参数.
 *    @param[in]   dwRight:权限控制.
 *    @return      
 */
PBSD_API BOOL PBSD_PlaybackByTelNo(HANDLE hDlg,
								   int nWndNo,
								   LPTelNoInfo pLoginParam,
								   LPPBSDTelPlayParam pPlayParam, 
								   DWORD dwRight = PBSD_RIGHT_ALL);


/**   @fn          PBSD_StopPlayByWndNo
 *    @brief   	   停止指定的回放窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   nWndNo:窗口号.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_StopPlayByWndNo(HANDLE hDlg, int nWndNo);

/**   @fn          PBSD_GetPlaybackInfo
 *    @brief   	   获取回放状态信息.	
 *    @param[in]   hDlg:PBSD_Create返回值.
 *    @param[in]   nWndNo:窗口号.
 *    @param[out]  stData:状态信息.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_GetPlaybackInfo(HANDLE hDlg, int nWndNo, PlaybackStatData& stData);

/**   @fn          PBSD_StopAll
 *    @brief   	   停止全部窗口.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
PBSD_API BOOL PBSD_StopAll(HANDLE hDlg);

/**   @fn          PBSD_SetMenuCallBack
 *    @brief   	   设置菜单回调.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnMenuCB:回调函数.
 *    @param[in]   pUserData:用户数据.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetMenuCallBack(HANDLE hDlg, PBSD_MenuCB pfnMenuCB, LPVOID pUserData);

/**   @fn          PBSD_SetWorkLogCallBack
 *    @brief   	   设置写操作日志回调.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnWorkLogCB:回调函数.
 *    @param[in]   pUserData:用户数据.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetWorkLogCallBack(HANDLE hDlg, PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData);

/**   @fn          PBSD_SetPlaybackCallBack
 *    @brief   	   设置回放回调,返回文件报文.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   pfnPlaybackCB:回调函数.
 *    @param[in]   pUserData:用户数据.
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetPlaybackCallBack(HANDLE hDlg, PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData);

/**   @fn          PBSD_SetParameter
 *    @brief   	   设置参数.
 *    @param[in]   stParam:参数引用
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
PBSD_API BOOL PBSD_SetParameter(const PLAYBACK_PARAM& stParam);

/**   @fn          PBSD_SetIEProtected
 *    @brief   	   设置IE保护模式,用于网页OCX.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_SetIEProtected();

/**   @fn          PBSD_SetBwControl
 *    @brief   	   设置带宽控制模式.
 *    @param[in]   stUserData:用户信息.
 *    @param[in]   stCmsData:CMS数据.
 *    @return      
 */
PBSD_API void PBSD_SetBwControl(const PBSDUserInfo& stUserData, const PBSDCmsData& stCmsData, const PBSDClientData& stClientData);

/**   @fn          PBSD_Invalida
 *    @brief   	   刷新窗口信息.
 *    @param[in]   hDlg:PBSD_Create的返回值.
 *    @param[in]   
 *    @return      
 */
PBSD_API void PBSD_Invalidate(HANDLE hDlg);

/**   @fn          PDSD_SetPlayBackConfig
 *    @brief   	   设置回放配置信息.
 *    @param[in]   szCapPath:抓图路径， szDownPath下载路径， nPicType图片类型 0：bmp 1jpg
 *    @param[in]   
 *    @return      void
 */
PBSD_API void PDSD_SetPlayBackConfig(const PLAYBACK_CONFIG& stPBConfig);

#ifdef __cplusplus
};
#endif

#endif