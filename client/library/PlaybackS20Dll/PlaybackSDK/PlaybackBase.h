#pragma once
/**	@file    PlaybackBase.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   PlaybackSDK基类
*
*	@author	 yudan
*	@date	 2012-06-01 16:34
*
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012-06-01 16:34 by yudan
*/
#include "PlaybackSdkDef.h"
using namespace PlaybackSdk;

#define OSDTIME_GET_YEAR(time)  (((time)>>26) + 2000)
#define OSDTIME_GET_MONTH(time) (((time)>>22) & 15)
#define OSDTIME_GET_DAY(time)   (((time)>>17) & 31)
#define OSDTIME_GET_HOUR(time)  (((time)>>12) & 31)
#define OSDTIME_GET_MIN(time)   (((time)>>6) & 63)
#define OSDTIME_GET_SEC(time)   (((time)>>0) & 63)

enum _STREAM_TYPE_DATA
{
    PB_STREAM_HEADER = 0,              //头数据
    PB_STREAM_DATA,                    //视频数据
    PB_STREAM_END,                     //结束数据
};

// 回放剪辑缓冲区大小,设置为比播放缓冲区稍微大.
#define MAX_SAVEBUF_SIZE 1024 * 1124
// 多保存的回放剪辑数据大小.
#define MAX_PRE_SAVEBUF_SIZE 1024 * 100

class CPlaybackBase
{
public:
	CPlaybackBase();
	virtual ~CPlaybackBase();

protected:
	LONG m_lPlaybackID;         //回放句柄
	LONG m_lDownloadID;         //下载句柄
public:
	LONG m_lChannel;            //通道

protected:
	// 文件类型转换.
	virtual int ChangeFileType(int nFileType);
	void PBTimeToCTime(const PB_TIME& stPBTime, CTime& odtTime);

public:
	/** @fn Login
	*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
	*   @param[in] stLoginInfo: 登录设备信息
	*   @param NULL
	*   @return 类操作句柄, -1标识失败
	*/
	virtual BOOL Login(PB_LOGININFO& stLoginInfo) = 0;

	/** @fn Logout
	*   @brief 登出设备,销毁句柄
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void Logout() = 0;

	/** @fn FindFile
	*   @brief 查找文件
	*   @param[in] stFindFileParam: 查找文件参数
	*   @param[in] lpFileList: 文件链表
	*   @param[in] nFileCount: 文件数量
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount) = 0;

	/** @fn SmartSearch
	*   @brief 智能检索
	*   @param[in] stFindFileParam: 查找文件参数
	*   @param[in] lpFileList: 文件链表
	*   @param[in] nFileCount: 文件数量
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	/** @fn PlaybackTime
	*   @brief 时间回放
	*   @param[in] stPbTimeInfo: 时间回放参数
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo) = 0;

	/** @fn PlaybackFile
	*   @brief 文件回放
	*   @param[in] stPbFileInfo: 文件回放参数
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo) = 0;

	/** @fn LocatePlayTime
	*   @brief 定位绝对时间
	*   @param[in]  stStartTime: 定位时间
	*   @param[in]  stStopTime:停止时间.
	*   @param[out] NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime) = 0;

	/** @fn StopPlayback
	*   @brief 停止回放
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopPlayback() = 0;

	/** @fn PlaybackControl
	*   @brief 回放控制
	*   @param[in] emControlType: 控制类型
	*   @param[in] lValueIn: 输入参数
	*   @param[out] lValueOut: 输出参数
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut) = 0;

	/** @fn GetOsdTime
	*   @brief 获取osd时间
	*   @param[in]  NULL
	*   @param[out] lOsdTime: osd时间，32time_t数值
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL GetOsdTime(LONG & lOsdTime) = 0;

	/** @fn GetPlayPos
	*   @brief 获取回放进度
	*   @param[in]  NULL
	*   @param[out] lPos: 进度
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL GetPlayPos(LONG & lPos) = 0;

	/** @fn PlaybackCapture
	*   @brief 回放抓图
	*   @param[in] szPicFile: 图片路径
	*   @param[in] emPicType: 图片类型
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL PlaybackCapture(char * szPicFile, ENUM_PIC_TYPE emPicType) = 0;

	/** @fn PB_SaveData
	*   @brief 录像剪辑
	*   @param[in] szSaveFile: 文件存储地址
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL SaveData(char * szSaveFile) = 0;

	/** @fn PB_StopSave
	*   @brief 停止剪辑
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopSave() = 0;

	/** @fn PB_DownloadFile
	*   @brief 文件下载
	*   @param[in] stDownloadParam: 下载参数
	*   @param NULL
	*   @return 下载句柄，NULL表示下载失败
	*/
	virtual BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam) = 0;

	/** @fn PB_DownloadTime
	*   @brief 时间下载
	*   @param[in] stDownloadParam: 下载信息
	*   @param NULL
	*   @return 下载句柄，NULL表示下载失败
	*/
	virtual BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam) = 0;

	/** @fn PB_StopDownload
	*   @brief 停止下载
	*   @param[in] hDownload: 下载句柄
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopDownload() = 0;

	/** @fn PB_GetDownloadPos
	*   @brief 获取下载进度
	*   @param[out] nPos: 进度
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL GetDownloadPos(int & nPos) = 0;

	/**  @fn          IsCardNumAllowed
	*    @brief   	  是否支持按卡号查询.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-支持,FALSE-不支持.
	*/
	virtual BOOL IsCardNumAllowed();

    /**  @fn          IsEventAllowed
	*    @brief   	  是否支持按事件查询.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-支持,FALSE-不支持.
	*/
    virtual BOOL IsEventAllowed();

	/**  @fn          GetStreamData
	*    @brief   	  获取回放流数据.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-支持,FALSE-不支持.
	*/
	virtual BOOL StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser);

	/**   @fn          StopGetStreamData
	*    @brief   	   停止获取回放流数据.
	*    @param[in]    hPlayback:回放句柄.
	*    @return     
	*/
	virtual void StopGetStreamData();

	/**   @fn          GetCompressCfgAbility
	*    @brief   	   获取压缩参数能力集.
	*    @param[out]   stCompresscfgAbility:能力集结构体.
	*    @return     
	*/
	virtual BOOL GetCompressCfgAbility( char* pOutbuf );

	/**   @fn          GetVideoFormat
	*    @brief   	   获取视频制式.
	*    @param[out]   nVideoFormat:视频制式.
	*    @return     
	*/
	virtual BOOL GetVideoFormat(int& nVideoFormat);

	/**   @fn          EnableZoomOut
	*    @brief   	   开启回放解码放大.
	*    @param[in]   rcShow: 显示区域
	*    @param[in]   nRegionNum: 显示区域序号，为0时忽略hWnd，bEnable参数
	*    @param[in]   hWnd: 目标窗口
	*    @param[in]   bEnable: 打开关闭显示区域.
	*    @return     
	*/
	virtual BOOL EnableZoomOut(RECT rcShow, DWORD nRegionNum = 0, HWND hWnd = NULL, BOOL bEnable = FALSE);

	/**   @fn          GetPictureSize
	*    @brief   	   获取解码图像大小.
	*    @param[out]   lWidth: 宽
	*    @param[out]   lHeight: 高.
	*    @return     
	*/
	virtual BOOL GetPictureSize( LONG* lWidth, LONG* lHeight);

    /**   @fn          GetKeyFrameData
	*    @brief   	   获取关键帧数据.
    *    @param[in]    stFrameTime: 关键帧时间
	*    @param[in]    pFunGetKeyFrame: 关键帧数据回调函数
	*    @param[in]    dwUser: 用户数据.
	*    @return     
	*/
	virtual BOOL GetKeyFrameData(const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser );

    /**   @fn          StopGetKeyFrameData
	*    @brief   	   停止获取关键帧数据.
	*    @return     
	*/
	virtual BOOL StopGetKeyFrameData();
};
