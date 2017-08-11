#ifndef __GUARD_SIMPLEHTTP_H_
#define __GUARD_SIMPLEHTTP_H_

#include "./stdafx.h"
#include <string>
#include <vector>
#include "GenDef.h"

class CSimpleHttp
{
public:

	static bool AddCenterAlarmLog(const center_info_t &centerInfo, alarm_msg_t &stAlarm);

    static bool PostMsg(const std::string &strHost,
        HPR_UINT16 nPort,
        const std::string &strKey,
        const std::string &strContents,
        std::string &strResult);

    static bool GetMsg(const std::string &strHost,
        HPR_UINT16 nPort,
        const std::string &strKey,
        std::string &strResult);

    static std::vector<std::string> Split(const std::string& str,
        const std::string& pattern);
public:
    CSimpleHttp(void);
	~CSimpleHttp(void);

	bool Connect(const std::string &strHost, HPR_UINT16 nPort = 80);

	void Close();

    bool PostMsg(const std::string &strKey,
        const std::string &strContents,
        std::string &strResult);

    bool GetMsg(const std::string &strKey,
        std::string &strResult);

    const std::string &GetErrorMsg(void) const;

	static std::_tstring strTime(__time32_t const t);

private:
    size_t GetContentLength(const std::string &strHead);
    
    /**	@fn	    IsOK200Msg
     *	@brief	看看消息是否是 200 OK
     *	@param  [in]strHead, http 消息头
     *	@param  [out]strResult, 如果不是200消息则返回具体状态
     *	@return	bool
     */
    bool Is200OKMsg(const std::string &strHead, std::string &strResult);

    bool SendAndRecv(const std::string &strHead,
        const std::string &strContents,
        std::string &strResult);

private:
    HPR_SOCK_T  m_client;
    std::string m_strHost;
    u_short m_nPort;
    std::string m_errMsg;
};

#endif
