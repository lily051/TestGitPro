/**	@file ConnSessionInfo.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 连接会话信息
*	@author		liangjungao
*	@date		2012-4-26 16:01:53
*	@note 
*/
#include "StdAfx.h"
#include "ConnSessionInfo.h"

CConnSessionInfo::CConnSessionInfo(void)
:m_pConnSession(NULL)
,m_nPort(0)
,m_StartConnectTime(0)
{
}

CConnSessionInfo::~CConnSessionInfo(void)
{
    m_pConnSession = NULL;
}
