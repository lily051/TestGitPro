#pragma once
/**	@file    PlaybackDef.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   Playback数据定义类
*
*	@author	 yudan
*	@date	 2012/06/20
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/06/20 by yudan
*/
//lint -library
/***************DEFINE*************************************************/
//color
#define UI_COLOR_BKGND           RESOURCELOAD_COLOR(IDS_UI_COLOR_BKGND)   //回放整体背景色
#define UI_WND_BKGND             RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //Tab页Wnd背景色
#define UI_WNDSPACE_BKGND        RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //窗口控件space背景色
#define UI_LISTHEADER_COLOR      RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //list Ctrl头背景色
#define UI_LISTITEMF_COLOR       RESOURCELOAD_COLOR(IDS_UI_LISTITEMF_COLOR)   //list单号行色
#define UI_LISTITEMS_COLOR       RESOURCELOAD_COLOR(IDS_UI_LISTITEMS_COLOR)   //list双号行色
#define UI_LISTSELECT_COLOR      RESOURCELOAD_COLOR(IDS_UI_LISTSELECT_COLOR)   //list选中行色
#define UI_TEXT_COLOR            RESOURCELOAD_COLOR(IDS_UI_TEXT_COLOR)         //文本色
#define UI_TREE_BKCOLOR          RESOURCELOAD_COLOR(IDS_UI_TREE_BKCOLOR)   //树的背景色,list通用  
#define UI_TREE_TEXT_COLOR_153	 RGB(153,153,153)     //树的文本颜色
#define UI_PLAYWND_COLOR         RESOURCELOAD_COLOR(IDS_UI_PLAYWND_COLOR)      //视频窗口背景色
#define UI_PLAYWND_SPACE         RESOURCELOAD_COLOR(IDS_UI_PLAYWND_SPACE)      //视频窗口分割色
#define UI_PLAYWNDSEL_FRAME      RESOURCELOAD_COLOR(IDS_UI_PLAYWNDSEL_FRAME)       //视频窗口选中边框
#define UI_PLAYWND_TITLE_COLOR   RESOURCELOAD_COLOR(IDS_UI_PLAYWND_TITLE_COLOR)	   // 视频窗口标题颜色.

#define UI_PLAYBAR_TITLEBK       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_RECORDBK      RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDBK)
#define UI_PLAYBAR_TITLESPACE    RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLESPACE)
#define UI_PLAYBAR_CURTIME       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_CURTIME)
#define UI_PLAYBAR_SELECT        RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_SELECT)
#define UI_PLAYBAR_BK            RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_MINPEN        RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_MINPEN)
#define UI_PLAYBAR_LEFT          RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_ELEMENT       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_ELEMENT)
#define UI_PLAYBAR_TOP           RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_LIGHT         RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_LIGHT)   //滑动条颜色
#define UI_PLAYBAR_RECORDTIMEING RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDTIMEING)
#define UI_PLAYBAR_RECORDMANUAL  RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDMANUAL)
#define UI_PLAYBAR_RECORDALARM   RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDALARM)

//size
#define UI_TREE_ITEM_HEIGHT	18	                 //树的每项的高度

// max file count.
#define MAX_FILE_COUNT 4000

//define
#define MAX_PLAYWND_NUM 16                       //最大回放窗口数量
#define MAX_SUBSERACHWND_NUM 16                  //最大分段搜索窗口数量
#define GROUPNAME_MAXLEN 32                      //分组名最大长度
#define LOCAL_MAX_FILENUM 1000                   //本地回放list最大视频数量
#define FLOATWND_CLOSE_TIME 4                    //浮动窗口显示时长
#define TREEDLG_WIDTH 259                        //树窗口宽度
#define DOWNLOADTITLE_WIDTH 200                  //下载窗口宽度

#define MSG_PLAYBACK_CONTROL WM_USER + 1001      //回放控制按钮响应消息
#define MSG_PLAYOSD_GET WM_USER + 1003           //获取回放OSD时间
#define MSG_DB_VEDIO (WM_USER + 1004)              //双击视频放大消息
#define MSG_WND_FULLSCREEN (WM_USER + 1005)        //窗口全屏消息
#define MSG_DATA_RESET WM_USER + 1006            //数据改变消息
#define MSG_SELECT_VEDIO (WM_USER + 1007)          //窗口选中消息
#define MSG_PLAYSTATE_UPDATE (WM_USER + 1008)      //回放状态改变消息
#define MSG_MENU_CALLBACK (WM_USER + 1009)		 //菜单事件

#define PB_SPEED_ONESIXTEEN  0                   // 1/16
#define PB_SPEED_ONEEIGHTH   1                   // 1/8
#define PB_SPEED_QUARTER   2                     // 1/4
#define PB_SPEED_HALF   3                        // 1/2d
#define PB_SPEED_NORMAL   4                      // 1
#define PB_SPEED_TWO   5                         // 2
#define PB_SPEED_FOUR   6                        // 4
#define PB_SPEED_EIGHT   7                       // 8
#define PB_SPEED_SIXTEEN   8                     // 16
#define PB_PAGE_SIZE   30                        //页大小
#define PB_MAX_SPEED_RANGE PB_PAGE_SIZE * PB_SPEED_SIXTEEN    //最大播放速度
#define PB_MIN_SPEED_RANGE PB_SPEED_ONESIXTEEN                //最小播放速度

//msg
#define MSG_TOOLBAR_BTN_CLICK  (WM_USER + 2001)    //工具栏按钮点击响应消息
#define MSG_TREENODE_DBCLICK   (WM_USER + 2002)    //树节点双击消息
#define MSG_TREENODE_DRAG      (WM_USER + 2003)    //树节点拖拽消息
#define MSG_RECORDFILE_PLAY    WM_USER + 2004    //文件回放消息
#define MSG_RECORDFILE_DRAG    WM_USER + 2005    //文件拖拽消息
#define MSG_RECORDFILE_DOWNLOAD (WM_USER+ 2006)    //文件下载信息
#define MSG_RECORD_DOWNLOAD_NOTIFY (WM_USER + 2007)//文件下载通知
#define MSG_TOOLBAR_UPDATE     (WM_USER + 2017)    //工具栏刷新消息
#define MSG_FULLSCREEN_CLICK   WM_USER + 2018    //全屏按钮点击消息
#define MSG_LOCALVEDIO_PLAY    (WM_USER + 2029)    //本地文件播放通知消息
#define MSG_LOCALVEDIO_OTHER   WM_USER + 2030    //本地播放文件切换消息
#define MSG_FLOATWND_CLOST     (WM_USER + 2041)    //浮动窗口关闭通知消息
#define MSG_RECORDDLG_SHOW     WM_USER + 2042    //文件窗口展示隐藏消息
#define MSG_DOWNLIST_SELECT    (WM_USER + 2053)    //下载list切换消息
#define MSG_DOWNLOADTASK_DEL   (WM_USER + 2054)    //下载任务删除消息
#define MSG_DOWNLOADTASK_FAIL  (WM_USER + 2055)    //下载任务失败消息
#define MSG_DOWNLOADTASK_SUC   (WM_USER + 2056)    //下载任务成功消息
#define MSG_DOWNLOADTASK_REDOWN (WM_USER + 2095)   //下载任务重新下载消息
#define MSG_DOWNLOADFILE_PLAY  (WM_USER + 2096)	 // 播放下载成功的文件.
#define MSG_PLAYBACK_NEXT_FILE (WM_USER + 2097)	 // 连续文件播放,播放下一个文件.
#define MSG_BWCONTROL_STOPPLAY (WM_USER + 2098)	 // 带宽控制消息,停止回放.
#define MSG_GROUPCFG_CHANGE (WM_USER + 2099)		 // 回放分组自动刷新消息.
#define MSG_BWCONTROL_RECONNECT WM_USER + 2100	 // 带宽重连消息.
#define MSG_PLAYBACK_PLAYING (WM_USER + 2101)		 // 回放控制消息-正在回放中.
#define MSG_PLAYBACK_PLAY_SUCCESS (WM_USER + 2102) // 回放控制消息-回放成功.
#define MSG_PLAYBACK_PLAY_FAIL (WM_USER + 2103)    // 回放控制消息-回放失败.
#define MSG_PLAYBACK_BW_LIMIT (WM_USER + 2104)	 // 回放控制消息-带宽控制连接上限.
#define MSG_PLAYBACK_STOPPLAY (WM_USER + 2105)	 // 回放控制消息-停止回放.
#define MSG_PLAYBACK_FIND_FAIL (WM_USER + 2106)	 // 回放控制消息-查找文件失败.
#define MSG_PLAYBACK_FIND_SUCCESS (WM_USER + 2107) // 回放控制消息-查找文件成功.
#define MSG_FILELIST_CHANGE (WM_USER + 2108)		 // 文件列表切换显示消息.
#define MSG_PLAYBACK_LOCATEPLAY_SUCCESS (WM_USER + 2109)  // 回放控制消息-设置回放进度成功.
#define MSG_PLAYBACK_LOCATEPLAY_FAIL (WM_USER + 2110)     // 回放控制消息-设置回放进度失败.
#define MSG_PLAYBACK_BWRECONNECT_STOPPLAY (WM_USER + 2111) // 回放控制消息-带宽控制服务器请求停止回放.
#define MSG_MOUSEEVENT_MINITOOLBAR_LEAVE (WM_USER + 2112) // 鼠标离开工具栏消息.
#define MSG_MOUSEEVENT_MINITOOLBAR_MOVE (WM_USER + 2113)  // 鼠标进入工具栏消息.
#define MSG_MINITOOLBAR_EXPAND_CLOSE (WM_USER + 2114)		// 扩展工具栏关闭消息.
#define MSG_MINITOOLBAR_OPENSOUND WM_USER + 2115		// 关闭其他窗口的音频.
#define MSG_MINITOOLBAR_DOWNLOAD (WM_USER + 2116)			// 按时间下载.
#define MSG_UPDATE_SYNBTN_STATE WM_USER + 2117			// 更新同步回放按钮.
#define MSG_CHECK_SPACE_NOTENOUGH (WM_USER + 2118)		// 磁盘空间不足.
#define MSG_PLAYBACK_FIND_NO_FILE (WM_USER + 2119)		// 回放控制消息-查找文件成功,但文件个数为0. 
#define MSG_PLAYBACK_SPEED_CHANGE (WM_USER + 2120)		// 回放速度改变消息.
#define MSG_PLAYBACK_SINGLEFRAME (WM_USER + 2121)			// 单帧回放消息.
#define MSG_UPDATE_REVERSEBTN_STATE WM_USER + 2122		// 更新倒放按钮.
#define MSG_INVALIDATE_WINDOW (WM_USER + 2123)					// 刷新窗口消息.
#define MSG_TOOLBAR_OPERATOR    (WM_USER + 2124)    
#define MSG_PLAYBACK_STATE  (WM_USER + 2025)            //播放状态

//timer
#define	IDT_LOCAL_PLAYBACK_POS_TIMER    WM_USER + 3004	 //本地回放进度计时器ID
#define	IDT_LOCAL_PLAYBACK_TIME_TIMER   WM_USER + 3005	 //本地回放时间计时器ID
#define IDT_SYNPLAY_TIMER               WM_USER + 3006   //同步回放定时器
#define IDT_VOLUMNWNDCLOSE_TIMER        WM_USER + 3007   //音量窗口自动关闭定时器
#define IDT_CAPTUREDLGCLOSE_TIME        WM_USER + 3008   //抓图展示窗口自动关闭定时器


//string
//录像类型
//[海康]//录象文件类型0xff－全部，0－定时录像,1-移动侦测 ，2－报警触发，
//3-报警|移动侦测 4-报警&移动侦测 5-命令触发 6-手动录像
#define		STR_PBL_RECORD_TYPE				     _T("录像类型:")
#define		STR_PBL_RECORD_TYPE_ALL				 _T("全部")
#define		STR_PBL_RECORD_TYPE_TIMING			 _T("定时录像")
#define		STR_PBL_RECORD_TYPE_MOVE			 _T("移动侦测")
#define		STR_PBL_RECORD_TYPE_ALARM_TRIGER	 _T("报警触发")
#define		STR_PBL_RECORD_TYPE_ALARM_OR_MOVE	 _T("报警|移动侦测")
#define		STR_PBL_RECORD_TYPE_ALARM_AND_MOVE	 _T("报警&移动侦测")
#define		STR_PBL_RECORD_TYPE_CMD_TRIGER		 _T("命令触发")
#define		STR_PBL_RECORD_TYPE_MANUAL_RECORD	 _T("手动录像")
#define		STR_PBL_RECORD_TYPE_BACKUP			 _T("备份下载录像")
#define		STR_PBL_RECORD_TYPE_EVENT			 _T("触发下载录像")


/***************ENUM***************************************************/

// 皮肤类型.
typedef enum _EnumSkinType
{
	SKIN_RED = 0,		// 红色.
	SKIN_BLUE = 1,		// 蓝色.
}EnumSkinType;

// 画面分割模式.
typedef enum _PLAY_LAYOUT_WND
{
	LAYOUT_WND_1 = 1,                     //一画面分屏
	LAYOUT_WND_4 = 4,                     //四画面分屏
	LAYOUT_WND_9 = 9,                     //九画面分屏
	LAYOUT_WND_16= 16,                    //十六画面分屏
}PLAY_LAYOUT_WND;

//发送消息目标ID
enum COMMON_SERVER_ID
{
	DB_CMS = 1,            //数据中心
	CLIENT_SELF,           //客户端自身
};

//请求消息类型
typedef enum
{
	ALM_REQ_MSG_WAITING_RSP = 0, //等待同步返回
	ALM_REQ_MSG_ASYS_RSP,        //异步返回
	ALM_REQ_MSG_NO_RSP           //不需要返回
}MSG_REQ_TYPE;

enum MAIN_TAB_ITEM				 //TAB页ID
{
	TAB_ITEM_PLAYBACK = 0,       //回放分页
	TAB_ITEM_DOWNLOAD,           //下载页
	//TAB_ITEM_LABLE,            //录像标签
	TAB_ITEM_SUBSEARCH,          //分段检索
	TAB_ITEM_LOCALPLAYBACK,      //本地回放
	TAB_ITEM_CONFIG,             //配置
};

enum DOWNLOAD_TAB_ITEM
{
	TAB_ITEM_DOWNLOAD_BY_FILE = 0,		// 按文件下载.
	TAB_ITEM_DOWNLOAD_BY_TIME,			// 按时间下载.
};

enum TREENODE_TYPE				 //树节点类型
{
	NODE_CTRLCELL		= 1,		  //控制单元
	NODE_REGION			= 2,		  //区域
	NODE_CAMERA			= 3,          //监控点
	NODE_GROUPROOT		= 4,		  //分组控制单元
	NODE_GROUP			= 5,          //分组
};

enum TREENODE_IMAGE
{
	TREEIMAGE_CTRLCELL  = 0,          //控制单元
	TREEIMAGE_REGION,                 //区域
	TREEIMAGE_CAMERA,                 //监控点
	TREEIMAGE_CAMERASEL,              //当前选中的监控点
};

enum PLAYCONTROL_ID
{
	PLAYBACK_WND_ID = WM_USER + 1001,            //回放窗口ID
	PLAYBACK_TITLE_ID = WM_USER + 1101,          //回放标题窗口ID
	PLAYBACK_VEDIO_ID,                           //回放视频窗口ID
	PLAYBACK_MINITOOL_ID,						 // 回放工具窗口ID.
	PLAYBACK_ID,                                 //回放ID
	PLAYBACK_CONTROL_ID,                         //控制窗口ID
	PLAYBACK_TIMEBAR,                            //进度时间控件

	BTN_PLAY_ID = WM_USER + 1110,                //播放
	BTN_PAUSE_ID,                                //暂停
	BTN_STOP_ID,                                 //停止
	BTN_SLOW_ID,                                 //慢放
	BTN_NOMORL_ID,                               //正常速率
	BTN_FAST_ID,                                 //快进
	BTN_SAVEFILE_ID,                             //保存
	BTN_VOICE_ID,                                //音频
	BTN_VOLUMESHOW_ID,                           //音量控件显示控制
	BTN_CAPTURE_ID,                              //截图
	BTN_DOWNLOAD_ID,                             //下载
	BTN_CONFIG_ID,                               //配置

	STT_VOLUMN_ID,                               //音量

};

enum DOWNLOAD_ID
{
	DOWNLOAD_WND_ID = WM_USER + 1201,            //下载窗口ID
	TASK_LIST_ID,                                //任务列表ID
	DATA_START_ID,                               //开始时间ID
	DATA_STOP_ID,                                //结束时间ID
	BTN_ADD_TASK,                                //添加任务ID
	BTN_CLEAR_ID,                                //清空任务ID
	BTN_DEL_ID,                                  //删除任务ID
	BTN_STARTDOWNLOAD_ID,                        //开始下载ID
	BTN_STOPDOWNLOAD_ID,                         //停止下载ID
	BTN_EXIT_ID,                                 //退出

};

enum TOOLBAR_CTRL_ID
{
	TOOL_GROUPCFG_ID = WM_USER + 1300,           //分组配置
	TOOL_PLAY_ID = WM_USER + 1301,               //播放
	TOOL_PAUSE_ID,                               //暂停
	TOOL_STOP_ID,                                //停止
	TOOL_STOPALL_ID,                             //全部停止
	TOOL_CAPTURE_ID,                             //抓图
	TOOL_SAVEDATA_ID,                            //保存数据
	TOOL_FRAME_ID,                               //单帧
	TOOL_SOUND_ID,                               //音频
	TOOL_DOWNLOAD_ID,                            //时间下载
	TOOL_VOLUMN_ID,                              //音量展开按钮
	TOOL_FULLSCREEN_ID,                          //全屏

	TOOL_SLIDER_SPEED,                           //速度进度条
	TOOL_SLIDER_POS,                             //本地回放进度条
	TOOL_SLIDER_VOLUMN,                          //音频音量条
	TOOL_CHECKBOX_VOICE,                         //静音控制

	TOOL_PRE_ID,                                 //前一个
	TOOL_NEXT_ID,                                //下一个

	TOOL_TIMESTA_ID,                             //本地回放时间显示

	TOOL_EXPAND_ID,								 // 扩展按钮.
	
	TOOL_MINI_SLOW_ID,							 // 减速.
	TOOL_MINI_FAST_ID,							 // 加速.
};

enum WND_ID
{
	WND_PLAYVIEW_ID = WM_USER + 1401,            //视频窗口
	WND_TOOLBAR_ID,                              //回放工具栏
	WND_PLAYBARTIMECTRL_ID,                      //录像条
};

enum CONTROL_MSG
{
	MSG_PLAY = 1,                                //播放
	MSG_PAUSE,                                   //暂停
	MSG_STOP,                                    //停止
	MSG_SPEED,                                   //速度
	MSG_SAVE,                                    //保存
	MSG_VOICE,                                   //声音
	MSG_VOLUME,                                  //音量
	MSG_CAPTURE,                                 //截图
	MSG_DOWNLOAD,                                //下载
	MSG_CONFIG,                                  //配置
};

enum CTRL_CUSTOMSIZE
{
	CTRL_BIGBTN_HEIGHT			= 50,										//自定义按钮大小
	CTRL_EDIT_NORMAL_WIDTH		= 25,										//编辑框
	CTRL_TREE_NORMAL_WIDTH		= 245,										//树控件
	CTRL_TREE_NODE_HEIGHT		= 20,
	CTRL_TREE_SMALL_WIDTH		= 200,		
	CTRL_SPACE_LITTLE			= 2,										//控件间距
	CTRL_SPACE_NORMAL			= 5,
	CTRL_BORD_LITTLE			= 3,										//细边框
	CTRL_PREVIEW_TOOLBAR_HEIGHT	= 39										//预览工具条高度
};

enum IE_SAVE_FILETYPE
{
	SAVE_FILETYPE_PIC = 0,						// 图片.
	SAVE_FILETYPE_REC = 1,						// 录像.
};

typedef enum _BWControlType
{
	BWCONTROL_NOCONTROL = 0,				// 没有控制.
	BWCONTROL_CROWDING,						// 高级别挤占.
	BWCONTROL_RECONNECT,					// 断线重连.
	SHOW_ERROR_MSG,							// 错误信息.
}BWControlType;

// 显示比例.
typedef enum _ShowProportion
{
	SHOW_PROPORTION_ORIGINAL = 0,					// 最大化.
	SHOW_PROPORTION_4_3,							// 4:3.
	SHOW_PROPORTION_16_9,							// 16:9.
}ShowProportion;

/***************STRUCT*************************************************/

//回放参数
typedef struct _stPlayParam
{
	int nChannel;					   // 通道号.
	char szStreamID[32];               //流ID,CVR时有效
	ENUM_PLAY_TYPE emPlayType;         //回放方式
	ENUM_RECORD_TYPE emRecordType;     //录像类型
	PB_TIME startTime;                 //开始时间
	PB_TIME stopTime;                  //结束时间
	BOOL bByCardNum;				   // 是否按卡号查询.
    CString strCardNum;				   // 卡号.
	_stPlayParam()
	{
		nChannel = 0;
		szStreamID[0] = '\0'; 
		emPlayType = PB_TYPE_DVR;
		emRecordType = RECORD_TYPE_ALL;
		bByCardNum = FALSE;
		strCardNum = _T("");
	}
}PLAY_PARAM;

// 下载类型.
typedef struct _stDownloadType
{
	DOWNLOAD_FILE_INFO downloadFileParam;
	DOWNLOAD_TIME_INFO downloadTimeParam;
}DownloadType;

//下载数据信息
typedef struct _stDownloadInfo
{
	int nCameraID;         //监控点ID
	int nChannel;			// 通道号.
	char szCameraName[255];     //监控点名称
	ENUM_PLAY_TYPE emDownType;  //下载方式
	BOOL bByTime;            //是否按时间下载
	PB_LOGININFO stLoginInfo;	// 登录信息.
	DWORD dwRev;			 // 预留字段.
	DownloadType unDownloadParam;	// 下载类型信息.
	UINT nPos;               //下载进度
	BOOL bDownload;          //是否已下载
	_stDownloadInfo()
	{
		nCameraID = -1;
        nChannel = -1;
		dwRev = 0;
		szCameraName[0] = '\0';
		emDownType = PB_TYPE_DVR;
		bByTime = FALSE;
		bDownload = FALSE;
		nPos = 0;
	}
	BOOL operator == (_stDownloadInfo & st)
	{
		if (nCameraID != st.nCameraID || emDownType != st.emDownType || bByTime != st.bByTime  ||  dwRev != st.dwRev)
		{
			return FALSE;
		}
		if (bByTime)
		{
			if (unDownloadParam.downloadTimeParam.lChannel != st.unDownloadParam.downloadTimeParam.lChannel || 
					unDownloadParam.downloadTimeParam.stStartTime != st.unDownloadParam.downloadTimeParam.stStartTime || 
						unDownloadParam.downloadTimeParam.stStopTime != st.unDownloadParam.downloadTimeParam.stStopTime)
			{
				return FALSE;
			}
		}
		else
		{
			if (strcmp(unDownloadParam.downloadFileParam.stFileInfo.szFileName, st.unDownloadParam.downloadFileParam.stFileInfo.szFileName) != 0)
			{
				return FALSE;
			}
		}

		return TRUE;
	}
}DOWNLOAD_INFO;

//回放标签信息
typedef struct _stPlaybackLabelInfo
{
	int nID;
	int nCameraID;                     //监控点ID
	CString strCameraName;             //监控点名称
	CString strFileName;               //文件名称
	CString strStartTime;              //起始时间
	CString strStopTime;               //结束时间
	ENUM_PLAY_TYPE emPlayType;         //数据源
	CString strLabelInfo;              //标签信息
	CString strLabelTime;              //标签时间
	CString strLabelUser;              //标签操作用户
}PLAYBACK_LABELINFO;

//分组资源详细信息表
typedef struct _stGroupResDetail
{
	int nGroupID;
	int nCameraID;
	CString	strCamName;
	CString strRegionName;
}GROUP_RES;

// 任务动态参数.
typedef struct _TaskDynData
{
	LPVOID pObject;			// 参数指针.
}TaskDynData, *LPTaskDynData;

// 倒放时间范围结构体.
typedef struct _ReverseTimeData
{
	PB_TIME tStart[MAX_PLAYWND_NUM];
	PB_TIME tStop[MAX_PLAYWND_NUM];
}ReverseTimeData, *LPReverseTimeData;
// 检测磁盘空间线程信息.
typedef struct _ThreadDataCheckSpace
{
	CString strFilePath;				// 文件路径.
	LPVOID pParam;						// 对话框指针.
}ThreadDataCheckSpace, *LPThreadDataCheckSpace;

// 框架消息.
typedef struct _FrameMsgData
{
	int nCmdID;							// 消息类型.
	std::string sMsg;					// 消息缓冲区.
}FrameMsgData, *LPFrameMsgData;

// 回放下一个文件结构体信息.
typedef struct _OrderPlayData
{
	int nCameraID;						// 监控点ID.
	CString strCamName;					// 监控点名称.
	int nChannel;						// 通道号. 
	char szStreamID[32];                // 流ID
	PB_FILEINFO stFileInfo;				// 文件信息. 
	PB_LOGININFO stLoginInfo;			// 登录信息. 
	int nWndID;							// 窗口号.
	DWORD dwRight;						// 回放权限.
	DWORD dwRev;						// 保留信息.
}OrderPlayData, *LPOrderPlayData;