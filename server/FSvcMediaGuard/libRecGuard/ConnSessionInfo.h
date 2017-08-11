/**	@file ConnSessionInfo.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief ���ӻỰ��Ϣ
*	@author		liangjungao
*	@date		2012-4-26 16:01:53
*	@note 
*/
#pragma once
#include <string>
#include <hpp_hpp.h>//<HPPЭ��ջ��ͷ�ļ�
#include <time.h>

class CConnSessionInfo
{
public:
    std::string m_sIP;
    int m_nPort;
    ConnSession* m_pConnSession;
    time_t m_StartConnectTime;//�������ӵ�ʱ��
public:
    CConnSessionInfo(void);
    virtual ~CConnSessionInfo(void);
};
