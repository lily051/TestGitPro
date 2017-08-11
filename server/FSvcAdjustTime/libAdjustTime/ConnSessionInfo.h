/**	@file ConnSessionInfo.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 连接会话信息
*	@author		liangjungao
*	@date		2012-4-26 16:01:53
*	@note 
*/
#pragma once
#include <string>
#include <hpp_hpp.h>//<HPP协议栈库头文件
#include <time.h>

class CConnSessionInfo
{
public:
    std::string m_sIP;
    int m_nPort;
    ConnSession* m_pConnSession;
    time_t m_StartConnectTime;//发起连接的时间
public:
    CConnSessionInfo(void);
    virtual ~CConnSessionInfo(void);
};
