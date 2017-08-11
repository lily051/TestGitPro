#pragma once
/**	@file    PlaybackSdkDef.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   PlaybackSDK数据定义
*
*	@author	 yudan
*	@date	 2012-06-01 18:59
*
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012-06-01 18:59 by yudan
*/
//lint -library
namespace PlaybackSdk
{
/*************DEFINE*********************************************/
#define MAX_RECORD_COUNT 4000           //文件查找上限

/*************ENUM***********************************************/
	// 回放类型标志位,,从500开始,用于区分NVR、PROX、BDP等非DVR回放取流与DVR取流，动态创建取流对象时用,即设备类型号不能大于500.
	#define PLAYTYPE_FLAG 500

	typedef enum _emDeviceType
	{
		PB_DEVICE_HIKVISION          = 0    //海康威视
	}ENUM_DEVICE_TYPE;

	typedef enum _emPbPlayType
	{
		PB_TYPE_DVR = 0,
		PB_TYPE_CVR,
		PB_TYPE_CLOUD,
        PB_TYPE_KMS                       
	}ENUM_PLAY_TYPE;

	//录像类型
	typedef enum _emEnumRecordType
	{
		RECORD_TYPE_ALL = 0xFF,            //全部
		RECORD_TYPE_TIMING = 0,            //定时录像 
		RECORD_TYPE_MOVE	= 1,           //移动侦测
		RECORD_TYPE_ALARM_TRIGER = 2,      //报警触发
		RECORD_TYPE_ALARM_OR_MOVE = 3,     //报警|移动侦测	
		RECORD_TYPE_ALARM_AND_MOVE = 4,    //报警&移动侦测
		RECORD_TYPE_CMD_TRIGER	= 5,       //命令触发
		RECORD_TYPE_MANUAL_RECORD = 6,     //手动录像

		RECORD_TYPE_BACKUP = 7,			   // 备份录像,备份下载服务器使用.
		RECORD_TYPE_EVENT = 8,			   // 触发下载录像,备份下载服务器使用.

		//智能侦测录像类型
		RECORD_TYPE_SMART_RECORD = 9,				//智能侦测
		RECORD_TYPE_VCA_TRAVERSE_PLANE = 10,               //越警侦测
		RECORD_TYPE_FIELD_DETECTION = 11,                  //区域入侵侦测 
		RECORD_TYPE_AUDIO_INPUT_ALARM	= 12,                //音频输入异常
		RECORD_TYPE_SOUND_INTENSITY_ALARM = 13,            //声强突变侦测
		RECORD_TYPE_FACE_DETECTION = 14,                   //人脸侦测
		RECORD_TYPE_VIRTUAL_FOCUS_ALARM = 15,              //虚焦侦测
		RECORD_TYPE_SCENE_CHANGE_ALARM	= 16,            //场景变更侦测
	}ENUM_RECORD_TYPE;

    //智能侦测类型
    typedef enum _emEnumEventType
    {
		EVENT_TYPE_ALL	= 0xffff,						//全部
        EVENT_TYPE_VCA_TRAVERSE_PLANE = 1,               //越警侦测
        EVENT_TYPE_FIELD_DETECTION = 2,                  //区域入侵侦测 
        EVENT_TYPE_AUDIO_INPUT_ALARM	= 3,                //音频输入异常
        EVENT_TYPE_SOUND_INTENSITY_ALARM = 4,            //声强突变侦测
        EVENT_TYPE_FACE_DETECTION = 5,                   //人脸侦测
        EVENT_TYPE_VIRTUAL_FOCUS_ALARM = 6,              //虚焦侦测
        EVENT_TYPE_SCENE_CHANGE_ALARM	= 7,            //场景变更侦测
    }ENUM_EVENT_TYPE;

	//图片类型
	typedef enum _emPicType
	{
		PIC_TYPE_JPG = 1,                  //JPG
		PIC_TYPE_BMP = 2,                  //BMP
	}ENUM_PIC_TYPE;

	//回放控制码定义
	typedef enum _emControlType
	{
		PB_CONTROL_TYPE_NOREMAL = 0,             //正常速度
		PB_CONTROL_TYPE_FAST2	= 1,             //2倍速(快进)	
		PB_CONTROL_TYPE_FAST4	= 2,             //4倍速(快进)	
		PB_CONTROL_TYPE_FAST8	= 3,             //8倍速(快进)	
		PB_CONTROL_TYPE_FAST16	= 4,             //16倍速(快进)
		PB_CONTROL_TYPE_SLOW2   = -1,            //1/2速(慢进)
		PB_CONTROL_TYPE_SLOW4   = -2,            //1/4速(慢进)
		PB_CONTROL_TYPE_SLOW8   = -3,            //1/8速(慢进)
		PB_CONTROL_TYPE_SLOW16  = -4,            //1/16速(慢进)

		PB_CONTROL_TYPE_PAUSE	= 11,            //暂停回放
		PB_CONTROL_TYPE_RESTART = 12,            //恢复回放
		PB_CONTROL_TYPE_OPEN_AUDIO	= 13,        //打开声音
		PB_CONTROL_TYPE_CLOSE_AUDIO	= 14,        //关闭声音
		PB_CONTROL_TYPE_SET_VOLUME	= 15,        //调节音量输出参数是0~100
		PB_CONTROL_TYPE_SINGLE_FRAME = 16,		 // 单帧.
		PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE = 17,// 停止单帧
		PB_CONTROL_TYPE_PASSIVE_DEC_RESETBUFFER = 18,  //清理缓冲区
	}ENUM_CONTROL_TYPE;
	
/*************FUNCTION*********************************************/
	typedef int (CALLBACK* PbStreamDataCB)(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser);

	/**************************************************************************
	*   Description:    画图回调函数
	*   Param:          long nPort 播放端口
	*					HDC hDC 表面设备上下文
	*					LONG nUser 用户数据
	**************************************************************************/
	typedef void (CALLBACK *VideoDrawFunction)(LONG nPort, HDC hDC, LONG nUser);

    /**************************************************************************
	*   Description:    关键帧数据获取回调函数
	*   Param:          nMsgType 消息类型，0--成功，其他失败
	*					pBuffer 图片数据
	*					dwBufSize 数据大小
                        tmTimeStamp 图片时间戳
                        dwUser 用户数据
	**************************************************************************/
    typedef int (CALLBACK* PbKeyFrameDataCB)(int nMsgType, BYTE* pBuffer, DWORD dwBufSize, time_t tmTimeStamp, DWORD dwUser);

/*************STRUCT*********************************************/

	//时间结构体
	typedef struct _stPbTime
	{
		DWORD	dwYear;			           //年
		DWORD	dwMonth;		           //月
		DWORD	dwDay;			           //日
		DWORD	dwHour;			           //时
		DWORD	dwMinute;		           //分
		DWORD	dwSecond;		           //秒
 		_stPbTime()
 		{
 			dwYear = 2012;
 			dwMonth = 1;
 			dwDay = 1;
 			dwHour = 0;
 			dwMinute = 0;
 			dwSecond = 0;
 		}
		BOOL operator == (_stPbTime & st)  //如果相等返回TRUE
		{
			if (dwYear != st.dwYear || dwMonth != st.dwMonth || dwDay != st.dwDay ||
					  dwHour != st.dwHour || dwMinute != st.dwMinute || dwSecond != st.dwSecond)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		BOOL operator != (_stPbTime & st)  //如果不相等返回TRUE
		{
			if (dwYear != st.dwYear || dwMonth != st.dwMonth || dwDay != st.dwDay ||
				dwHour != st.dwHour || dwMinute != st.dwMinute || dwSecond != st.dwSecond)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		BOOL operator < (_stPbTime& st)	
		{
			CTime ctNow((int)dwYear, (int)dwMonth, (int)dwDay, (int)dwHour, (int)dwMinute, (int)dwSecond);
			CTime ctCom((int)st.dwYear, (int)st.dwMonth, (int)st.dwDay, (int)st.dwHour, (int)st.dwMinute, (int)st.dwSecond);
			if (ctNow < ctCom)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		BOOL operator > (_stPbTime& st)	
		{
			CTime ctNow((int)dwYear, (int)dwMonth, (int)dwDay, (int)dwHour, (int)dwMinute, (int)dwSecond);
			CTime ctCom((int)st.dwYear, (int)st.dwMonth, (int)st.dwDay, (int)st.dwHour, (int)st.dwMinute, (int)st.dwSecond);
			if (ctNow > ctCom)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		static BOOL PBTIME2Ctime(const _stPbTime &stTime, CTime &tm)
		{
			if (stTime.dwYear <= 0 || stTime.dwMonth <= 0 || stTime.dwDay <= 0)
			{
				return FALSE;
			}
			tm = CTime(stTime.dwYear, stTime.dwMonth, stTime.dwDay, stTime.dwHour, stTime.dwMinute, stTime.dwSecond);
			return TRUE;
		}
		static BOOL Ctime2PBTIME(const CTime &tm, _stPbTime &stTime)
		{
			if (tm.GetYear() < 1970 || tm.GetMonth() <= 0 || tm.GetDay() <= 0 )
			{
				return FALSE;
			}

			stTime.dwYear = tm.GetYear();
			stTime.dwMonth = tm.GetMonth();
			stTime.dwDay = tm.GetDay();
			stTime.dwHour = tm.GetHour();
			stTime.dwMinute = tm.GetMinute();
			stTime.dwSecond = tm.GetSecond();
			return TRUE;
		}

	}PB_TIME,*LPPB_TIME;

	

	
	//dvr/cvr/云存储登录信息
	typedef struct _stPbDvrLoginInfo
	{
		char szIP[16];                     //ip
		int nPort;                         //端口
		int nDataPort;                     //数据端口
		char szUserName[32];               //用户名
		char szPassword[32];               //密码
		char szIndex[32];                   //云存储唯一标示
		char szRev[160];                    //预留
 		_stPbDvrLoginInfo()
 		{
 			memset(szIP, 0, sizeof(szIP));
 			nPort = 8000;
			nDataPort = 3720;
 			memset(szUserName, 0, sizeof(szUserName));
 			memset(szPassword, 0, sizeof(szPassword));
			memset(szIndex, 0, sizeof(szIndex));
 			memset(szRev, 0, sizeof(szRev));
 		}
	}PB_DVRLOGININFO;

	// 登录类型信息
	typedef struct _stLoginTypeInfo
	{
		PB_DVRLOGININFO stDvrLoginInfo;
	}LoginTypeInfo;

	//登录信息
	typedef struct _stPbLoginInfo
	{
		ENUM_PLAY_TYPE emPlayType;         //回放类型
		ENUM_DEVICE_TYPE emDevType;       //设备类型
		LoginTypeInfo unLoginType;
		_stPbLoginInfo()
		{
			emDevType = PB_DEVICE_HIKVISION;
			emPlayType = PB_TYPE_DVR;
		}
	}PB_LOGININFO;

	//文件查找参数信息
	typedef struct _stFindFileParam
	{
		LONG lChannel;                      //通道
		char szStreamID[32];                //流ID,只对CVR有效
		PB_TIME stStartTime;                //查找起始时间
		PB_TIME stStopTime;                 //查找结束时间
		ENUM_RECORD_TYPE emRecordType;      //查找文件类型
		DWORD dwUseCardNo;					// 是否按卡号查询.
		BYTE sCardNumber[32];				// 卡号.
        BOOL bByEvent;                      // 是否按事件查询
        ENUM_EVENT_TYPE emEventType;        //查找事件类型
		_stFindFileParam()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			emRecordType = RECORD_TYPE_ALL;
			dwUseCardNo = 0;
			memset(sCardNumber, 0, sizeof(sCardNumber));
            bByEvent = FALSE;
            emEventType = EVENT_TYPE_VCA_TRAVERSE_PLANE;
		}
	}FINDFILE_PARAM;

	typedef struct _stConvertPlayParam
	{
		BYTE byResolution;		//分辨率
		DWORD dwVideoBitrate;   //视频码率
		DWORD dwVideoFrameRate; //帧率

		_stConvertPlayParam()
		{
			byResolution = 0;
			dwVideoBitrate = 0;
			dwVideoFrameRate = 0;	
		}
	}CONVERTPLAY_PARAM;

	///<点坐标结构
	typedef struct tagPB_POINT
	{	
		float fX;								///<X轴坐标, 0.001~1
		float fY;								///<Y轴坐标, 0.001~1
	}PB_POINT, *LPPB_POINT;

	///<矩形坐标结构
	typedef struct tagPB_RECT
	{	
		float fLEFT;								///<X轴坐标, 0.001~1
		float fTOP;								///<Y轴坐标, 0.001~1
		float fWidth;
		float fHeight;
	}PB_RECT, *LPPB_RECT;

	//智能检索参数信息
	typedef struct _stSmartSearchParam
	{
		LONG lChannel;				//通道
		PB_TIME stStartTime;        //查找起始时间
		PB_TIME stStopTime;         //查找结束时间
		int	nRecordType;			//智能类型，0-移动侦测，1-越界侦测，2-区域入侵
		DWORD dwCurArea;			//当前区域
		DWORD dwSensitivity;		//灵敏度.
		DWORD dwCrossDirection;		//穿越方向--------越界侦测
		DWORD dwPreTime;			//报警提前时间
		DWORD dwDelayTime;			//报警延时时间
		DWORD dwIntrusionRate;		//占比：区域内所有未报警目标尺寸目标占区域面积的比重，归一化为1~100---区域入侵 
		DWORD dwIntrusionDuration;	//行为事件触发时间阈值: 1~100秒，建议5秒，判断有效报警的时间
										//(如果是ATM设备，时间为1~1200秒)---区域入侵 
		DWORD dwPointNum;   //区域入侵有效点数
		PB_POINT struPos[10];  //多边形边界点，最大值为10 
		BYTE byMotionScope[64][96];  //移动侦测区域

		_stSmartSearchParam()
		{
			nRecordType = 0;
			lChannel = 0;
			dwCurArea = 0;
			dwSensitivity = 1;
			dwCrossDirection = 0;
			dwPreTime = 0;
			dwDelayTime = 0;
			dwIntrusionRate = 0;
			dwIntrusionDuration = 0;
			dwPointNum = 0;
		}
	}SMARTSEARCH_PARAM;

	//时间回放信息
	typedef struct _stPlaybackTimeInfo
	{
		LONG lChannel;                      //通道
		char szStreamID[32];                //流ID,只对CVR有效
		PB_TIME stStartTime;                //开始回放时间
		PB_TIME stStopTime;                 //结束回放时间
		HWND hWnd;                          //播放窗口句柄
		ENUM_RECORD_TYPE emRecordType;      //查找文件类型
        BOOL bByEvent;                      // 是否按事件查询
        ENUM_EVENT_TYPE emEventType;        //查找事件类型
		//转码回放-------------------------------
		BOOL bConvertPlay;      //是否转码回放
		CONVERTPLAY_PARAM stConvertParam;  //转码回放参数 
		VideoDrawFunction pDrawFunc;		//回放画图函数
		LONG nUser;                         //用户数据
        char szPlayUrl[255];                //回放url
		LONG lBwControl;                     //带宽控制
		_stPlaybackTimeInfo()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			hWnd = NULL;
			emRecordType = RECORD_TYPE_ALL;
			bConvertPlay = FALSE;
			pDrawFunc = NULL;
			nUser = 0;
            bByEvent = FALSE;
            emEventType = EVENT_TYPE_VCA_TRAVERSE_PLANE;
            szPlayUrl[0] = '\0';
			lBwControl = 0;
		}
	}PLAYBACK_TIME_INFO;

	//文件信息
	typedef struct _stFileInfo
	{
		char szFileName[128];               //文件名
		DWORD dwFileSize;                   //文件大小
		int nFileType;                      //文件类型
		PB_TIME stStartTime;                //起始时间
		PB_TIME stEndTime;                  //结束时间
		ENUM_PLAY_TYPE emPlayType;          //文件存储源
		ENUM_DEVICE_TYPE emDevType;         //设备类型
        char szPlayUrl[255];                //回放url
		char szStreamID[32];                //流ID
		char szRev[32];                     //预留
 		_stFileInfo()
 		{
 			szFileName[0] = '\0';
 			dwFileSize = 0;
 			nFileType = 0;
            szPlayUrl[0] = '\0';
			szStreamID[0] = '\0';
 			szRev[0] = '\0';
 		}
	}PB_FILEINFO, *LPB_FILEINFO;

	//文件回放参数
	typedef struct _stPlaybackFileInfo
	{
		LONG lChannel;                      //通道号
		char szStreamID[32];                //流ID,只对CVR有效
		PB_FILEINFO stFileInfo;             //文件信息
		HWND hWnd;							//播放窗口
		VideoDrawFunction pDrawFunc;		//回放画图函数
		LONG nUser;                         //用户数据
		LONG lBwControl;                     //带宽控制

		//转码回放-------------------------------
		BOOL bConvertPlay;      //是否转码回放
		CONVERTPLAY_PARAM stConvertParam;  //转码回放参数 
		_stPlaybackFileInfo()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			hWnd = NULL;
			bConvertPlay = FALSE;
			pDrawFunc = NULL;
			nUser = 0;
			lBwControl = 0;
		}
	}PLAYBACK_FILE_INFO;

	//文件下载参数
	typedef struct _stDownloadFileInfo
	{
		LONG lChannel;                      //通道号
		char szStreamID[32];                //流ID,只对CVR有效
		PB_FILEINFO stFileInfo;             //文件信息
		char szDownloadFile[255];           //下载地址
 		_stDownloadFileInfo()
 		{
 			lChannel = 0;
			szStreamID[0] = '\0'; 
 			memset(szDownloadFile, 0, sizeof(szDownloadFile));
 		}
	}DOWNLOAD_FILE_INFO;

	//时间下载参数
	typedef struct _stDownloadTimeInfo
	{
		LONG lChannel;                      //通道号
		char szStreamID[32];                //文件名
		char szFileName[128];               //文件名
		char szFilePath[512];               //文件路径
		DWORD dwFileSize;                   //文件大小
		PB_TIME stStartTime;                //开始时间
		PB_TIME stStopTime;                 //结束时间
		char szDownloadFile[255];           //下载地址
		char szPlayUrl[255];                //回放url
		int nBwControl;                     //带宽控制
 		_stDownloadTimeInfo()
 		{
 			lChannel = 0;
			szStreamID[0] = '\0';
			dwFileSize = 0;
 			memset(szDownloadFile, 0, sizeof(szDownloadFile));
			memset(szFileName, 0, sizeof(szFileName));
			memset(szFilePath, 0, sizeof(szFilePath));
			szPlayUrl[0] = '\0';
			nBwControl = 0;
 		}
	}DOWNLOAD_TIME_INFO;

	//抓图参数
	typedef struct _stCaptureParam 
	{
		char sSavePic[255];                 //保存图片的路径和名称
		ENUM_PIC_TYPE emPicType;            //图片类型
		_stCaptureParam()
		{
			memset(sSavePic, 0, sizeof(sSavePic));
			emPicType = PIC_TYPE_JPG;
		}
	}CAPTURE_PARAM, *LPCAPTURE_PARAM;

	//回放保存数据参数
	typedef struct _stSaveDataParam
	{
		char sSaveName[128];                //保存数据的路径和名称
		_stSaveDataParam()
		{
			memset(sSaveName, 0, sizeof(sSaveName));
		}
	}SAVE_DATA_PARAM, *LPSAVE_DATA_PARAM;

	//设备能力集结构定义----------------------------------------------------
	#define XML_ABILITY_OUT_LEN	3*1024*1024

	//>>>>>>>>>>>>>>>>>>>>>>>>>>云存储回放相关>>>>>>>>>>>>>>>>>>>>>>>>>>
	// 最大文件个数.
	#define MAX_FILE_NUM 3000
	static size_t const SEG_HEAD_LEN = 1024;
	typedef struct _seg_head
	{
		INT32 cbSize;                                    // 此结构大小
		INT32 meidaType;                                 // 录音 or 录像？
		INT32 mediaFormat;                               // hikmp4 or dahuamp4 or wav or mp3
		INT32 segno;                                     // 是原始数据的第n个片段
		INT32 begintime;                                 // 原始数据的开始时间，这段数据可能是原始数据的一个片段
		INT32 endtime;                                   // 原始数据的结束时间
		UINT32 headSize;                                 // 媒体头大小
		BYTE headData[SEG_HEAD_LEN - 7 * sizeof(INT32)]; // 媒体头数据
	}seg_head_t;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<云存储回放相关<<<<<<<<<<<<<<<<<<<<<<<<<<
}
