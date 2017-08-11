/**	@file vag_client_define.h
*   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*   @brief 设备接入网关客户端结构和宏定义 
*
*	@author zhangqingwen@hikvision.com
*	@date 2013年04月22日
*
*	@note 历史记录：V1.0.0
*	@note 
*
*	@warning
*/

#ifndef _VAG_CLIENT_DEFINE_EX_H_
#define _VAG_CLIENT_DEFINE_EX_H_

enum MSG_TYPE
{
	E_RTSP_MSG,
	E_CONN_MSG
};

enum CONN_MSG_CODE
{
	CONN_HEARTBEAT_TIMEOUT,
	CONN_EXCEPTION_CLOSE,
	CONN_SESSION_KICKOUT,
	CONN_SESSION_REDIRECT
};

struct CONN_MSG
{
	CONN_MSG_CODE eCode;
	const char* pszMsg;
	int iMsgLen;
};

enum RTSP_MSG_CODE
{
	RTSP_OPTION_REQ,
	RTSP_OPTION_RSP,
	RTSP_DESCRIBE_REQ,
	RTSP_DESCRIBE_RSP,
	RTSP_SETUP_REQ,
	RTSP_SETUP_RSP,
	RTSP_PLAY_REQ,
	RTSP_PLAY_RSP,
	RTSP_PAUSE_REQ,
	RTSP_PAUSE_RSP,
	RTSP_TEARDOWN_REQ,
	RTSP_TEARDOWN_RSP,
	RTSP_GET_PARAMETER_REQ,
	RTSP_GET_PARAMETER_RSP,
	RTSP_SET_PARAMETER_REQ,
	RTSP_SET_PARAMETER_RSP,
	RTSP_ANNOUNCE_REQ,
	RTSP_ANNOUNCE_RSP,
	RTSP_REDIRECT_REQ,
	RTSP_REDIRECT_RSP,
	RTSP_RECORD_REQ,
	RTSP_RECORD_RSP,
	RTSP_PTZ_REQ,
	RTSP_PTZ_RSP,
	RTSP_GETVIDEOPARAM_REQ,
	RTSP_GETVIDEOPARAM_RSP,
	RTSP_SETVIDEOPARAM_REQ,
	RTSP_SETVIDEOPARAM_RSP
};

struct RTSP_MSG
{
	RTSP_MSG_CODE eCode;
	const char* pszMsg;
	int iMsgLen;
};

#endif
