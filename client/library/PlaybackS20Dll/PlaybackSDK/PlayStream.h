#pragma once
/**	@file    PlayStream.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   解码层
*
*	@author	 yudan
*	@date	 2012-06-02 13:51
*
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012-06-02 13:51 by yudan
*/
#include "./PlaybackBase.h"
#include "GeneralPlayerAPI.h"


#define PLAYBACK_DETECT_TIME 10
#define DOWNLOAD_DETECT_TIME 10

class CPlayStream : public CPlaybackBase
{
public:
	CPlayStream();
	virtual ~CPlayStream();

protected:
	PLAYERTYPE m_emPlayer;           //播放器类型
	HWND m_hPlayHwnd;                //播放窗口句柄
	BYTE m_byteHeadBuf[1024];        //头文件数据保存
	BYTE m_byteSaveBuf[MAX_SAVEBUF_SIZE];	// 回放剪辑缓冲区,循环写入流数据.
	BOOL m_bIsReWrite;				 // 缓冲区是否满了，并从头开始写入了.
	DWORD m_dwCurBufPointer;				// 剪辑缓冲区当前指针.
	int m_nHeadLen;                  //头数据大小
	HANDLE m_hSaveFile;              //文件句柄
	BOOL m_bSaveFile;				 // 是否要保存文件
	HANDLE m_hDownloadFile;          //下载文件句柄
	DWORD m_dwTotleSize;             //文件总大小
	DWORD m_dwCurDownSize;           //当前下载的大小
	PLAYBACK_TIME_INFO m_stPlaybackTimeInfo;  //当前回放的时间参数
	CRITICAL_SECTION m_csSaveBufLock;// 回放剪辑缓冲锁.
	BOOL m_bOpenSound;				 // 打开音频.
	PbStreamDataCB m_pfnStreamCB;	 // 取流回调函数
	DWORD m_dwStreamUserData;		 // 取流回调用户自定义数据.
	BOOL m_bStreamCB;				 // 是否要开始取流回调.
	BOOL m_bGetHead;				 // 是否获取头数据.
	VIDEODRAWFUNC m_pDrawFunc;		 // 视频画图函数
	LONG m_nDrawUserData;			// 用户数据

public:
    BOOL m_bPlaying;                 //是否解码中
	LONG m_lPort;                    //解码端口
	BOOL m_bDownload;                //是否下载
	BOOL m_bPause;                   //是否暂停回放在
	LONG m_lPlaybackLastTime;        //回放最后一次来流时间
	LONG m_lDownloadLastTime;        //下载最后一次来流时间
	int m_nPlaybackPos;              //回放进度
	int m_nDownloadPos;              //下载进度
	BOOL m_bPlaybackThread;          //回放检测线程
	HANDLE m_hPlaybackThread;        //回放检测线程句柄
	BOOL m_bDownloadThread;          //下载检测线程
	HANDLE m_hDownloadThread;        //下载检测线程句柄

	BOOL m_bReversePlay;			// 是否是倒放.
	BOOL m_bStopPlay;		
	BOOL m_bBlock;					// 是否阻塞数据回调.
	CString m_strSaveFilePath;      //剪辑文件路径

    CRITICAL_SECTION m_csStreamCB;  //取流回调函数锁
	ENUM_CONTROL_TYPE m_emPlaySpeed; //回放速度
	DWORD m_dwBuffSize;             //播放库缓冲区大小

protected:
	// 设备类型和播放类型转换.
	PLAYERTYPE GetPlayTypeFromDeviceType(int nType);

public:
	// 设置头数据缓冲.
	void SetHeadData(PBYTE pBuffer, DWORD dwBufSize);
	// 获取头数据.
	void GetHeadData(PbStreamDataCB pfnStreamData, DWORD dwUserData);
	// 设置缓冲区数据.
	void SetBufferData(PBYTE pBuffer, DWORD dwBufSize);
	// 获取缓冲区数据.
	void GetBufferData(PbStreamDataCB pfnStreamData, DWORD dwUserData);
	// 获取取流回调函数.
	PbStreamDataCB GetStreamDataCB();
	// 获取取流回调用户自定义数据.
	DWORD GetStreamUserData();
	// 向上层推流.
	void PushStreamData(int nDataType, BYTE * pBuffer, DWORD dwBufSize);
	// 获取回放类型.
	PLAYERTYPE GetPlayType();
	// 获取保存文件句柄.
	HANDLE GetSaveFileHandle();

public:
	/** @fn pFunPlayback
	*   @brief 取流回调函数
	*   @param[in] hPlayData: 回放句柄
	*   @param[in] nDataType: 流类型
	*   @param[in] pBuffer:   流数据
	*   @param[in] dwBufferSize: 数据大小
	*   @return NULL
	*/
	BOOL pFunPlayback(HANDLE hPlayData, int nDataType, BYTE * pBuffer, DWORD dwBufferSize);

	/** @fn pFunDownload
	*   @brief 取流回调函数
	*   @param[in] hPlayData: 回放句柄
	*   @param[in] nDataType: 流类型
	*   @param[in] pBuffer:   流数据
	*   @param[in] dwBufferSize: 数据大小
	*   @return NULL
	*/
	void pFunDownload(HANDLE hPlayData, int nDataType, BYTE * pBuffer, int dwBufferSize);

	/** @fn PlaybackStreamDetect
	*   @brief 回放流检测线程
	*   @param[in] lpvoid: 线程参数
	*   @param NULL
	*   @return NULL
	*/
	static void PlaybackStreamDetect(LPVOID lpvoid);

	/** @fn DownloadStreamDetect
	*   @brief 下载流检测线程
	*   @param[in] lpvoid: 线程参数
	*   @param NULL
	*   @return NULL
	*/
	static void DownloadStreamDetect(LPVOID lpvoid);

	/** @fn StartStreamThread
	*   @brief 开启线程
	*   @param[in] bPlayback: TRUE回放线程 FALSE下载线程
	*   @param NULL
	*   @return BOOL成功 FALSE失败
	*/
	BOOL StartStreamThread(BOOL bPlayback);
	BOOL StartCheckPlayStream(BOOL bPlayback);

	/** @fn StopStreamThread
	*   @brief 停止流检测线程
	*   @param[in] bPlayback: TRUE回放线程 FALSE下载线程
	*   @param NULL
	*   @return NULL
	*/
	void StopStreamThread(BOOL bPlayback);
	void StopCheckPlayStream(BOOL bPlayback);

	/** @fn Login
	*   @brief 登录设备，获取类句柄，单个回放或下载操作即在此类上进行
	*   @param[in] stLoginInfo: 登录设备信息
	*   @param NULL
	*   @return TRUE成功 FALSE失败
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
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)= 0;

	/** @fn StopPlayback
	*   @brief 停止回放
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopPlayback();

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

	/** @fn LocatePlayTime
	*   @brief 定位绝对时间
	*   @param[in]  stStartTime: 定位时间
	*   @param[in]  stStopTime:停止时间.
	*   @param[out] NULL
	*   @return TRUE成功 FALSE失败
	*/
	BOOL LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime);

	/** @fn StopPlay
	*   @brief 停止解码
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	void StopPlay();

	/** @fn PlaybackControl
	*   @brief 回放控制
	*   @param[in] emControlType: 控制类型
	*   @param[in] lValueIn: 输入参数
	*   @param[out] lValueOut: 输出参数
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

	/** @fn GetOsdTime
	*   @brief 获取osd时间
	*   @param[in]  NULL
	*   @param[out] lOsdTime: osd时间，32time_t数值
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL GetOsdTime(LONG & lOsdTime);

	/** @fn GetPlayPos
	*   @brief 获取回放进度
	*   @param[in]  NULL
	*   @param[out] lPos: 进度
	*   @return TRUE成功 FALSE失败
	*/
	virtual BOOL GetPlayPos(LONG & lPos);

	/** @fn PlaybackCapture
	*   @brief 回放抓图
	*   @param[in] szPicFile: 图片路径
	*   @param[in] emPicType: 图片类型
	*   @return TRUE成功 FALSE失败
	*/
	BOOL PlaybackCapture(char * szPicFile, ENUM_PIC_TYPE emPicType);

	/** @fn PB_SaveData
	*   @brief 录像剪辑
	*   @param[in] szSaveFile: 文件存储地址
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	BOOL SaveData(char * szSaveFile);

	/** @fn PB_StopSave
	*   @brief 停止剪辑
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	void StopSave();

	/** @fn PB_GetDownloadPos
	*   @brief 获取下载进度
	*   @param[out] nPos: 进度
	*   @param NULL
	*   @return TRUE成功 FALSE失败
	*/
	virtual	BOOL GetDownloadPos(int & nPos);

public:
	// 检测回放流.
	static DWORD WINAPI ThreadFuncCheckPlayStream(LPVOID pParam);

	// 获取检测流特殊停止标志.
	virtual DWORD GetSpecialStopFlag();

	// 获取取流回放数据.
	virtual BOOL StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser);

	// 停止获取取流回放数据.
	virtual void StopGetStreamData();

	//获取解码图像的大小
	virtual BOOL GetPictureSize( LONG* lWidth, LONG* lHeight);
};

extern int CALLBACK OnSaveData(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser);