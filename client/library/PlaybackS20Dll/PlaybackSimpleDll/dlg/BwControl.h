/**   @file BwControl.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	带宽控制类.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/03/05
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

// 重连带带宽控制服务器的时间间隔，10秒.
#define BWSERVER_RECONNECT_TIMESPAN 10 * 1000

// 带宽注册回调函数.
typedef void (CALLBACK * BwRegistMsgCB)(BOOL bRet, LPVOID pParam);

// 带宽注册线程消息.
class CBwControl;
typedef struct _RegisThreadData
{
	CBwControl* pControl;		// 当前指针.
	BwRegistMsgCB pfnMsgCB;		// 带宽控制回调函数.
	LPVOID pUserData;			// 用户自定义数据.

	_RegisThreadData()
	{
		pControl = NULL;
		pfnMsgCB = NULL;
		pUserData = NULL;
	}
}RegisThreadData, *LPRegisThreadData;


class CBwControl
{
public:
	CBwControl(void);
	~CBwControl(void);

private:
	// 带宽控制服务器注册线程句柄.
	HANDLE m_hRegThread;
	// 带宽控制服务器注册线程停止信号量.
	HANDLE m_hStopReg;
	// 带宽注册线程参数.
	RegisThreadData m_stData;

public:
	// 注册带宽控制服务器.
	BOOL RegistBwServer(BwRegistMsgCB pfnBwFunc, LPVOID pUserData);
	// 注册带宽控制服务器线程函数.
	static DWORD WINAPI ThreadFuncRegBwServer(LPVOID pParam);
	// 停止注册带宽控制服务器.
	void StopRegistBwServer();
	// 请求带宽控制.
	BOOL ReqBwControl(int nCameraID, BOOL bReq);
	// 设置是否注册完成.
	void SetRegisted();
};
