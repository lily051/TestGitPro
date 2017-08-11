/**   @file TaskPlaybackByFile.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	按文件进行回放任务类.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/01/18
 *
 *    @note		
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "TaskBase.h"

typedef struct _TaskDataByFile
{
	HANDLE hDlg;				// 对话框句柄.
	int nCamID;					// 监控点ID.
	DWORD dwRev;				// 预留字段.
	PB_LOGININFO stLoginInfo;	// 登录信息.
	PLAYBACK_FILE_INFO stPlaybackParam;	// 回放信息.
	BOOL bReversePlay;					// 是否是倒放.
}TaskDataByFile, *LPTaskDataByFile;


class CTaskPlaybackByFile : public CTaskBase
{
public:
	CTaskPlaybackByFile(const TaskDataByFile& stDataByFile);
	~CTaskPlaybackByFile(void);

public:
	// 执行任务.
	virtual void DoTask(TaskMsgCB pfnTaskMsgCB, LPVOID pUserData, LPTaskDynData pData);

private:
	// 回放.
	HANDLE Playback();

private:
	// 按文件回放参数.
	TaskDataByFile m_stByFile;
};
