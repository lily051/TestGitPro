#include "stdafx.h"
#include "Udd.h"
#include "HCNetSDK.h"
#include <HPR_Hpr.h>
#include <HPR_Socket.h>
#include <HPR_Error.h>
#include <string>
#include <sstream>
//
class CSimpleHttp
{
public:
    CSimpleHttp();
    ~CSimpleHttp();
    //
    const std::string &GetErrorMsg(void) const;
    void Close();
    bool Connect(const std::string &strHost, HPR_UINT16 nPort = 80);
    //
    static bool GetLocalIp(std::string &strIp);
    static bool PostMsg(const std::string &strHost,
        HPR_UINT16 nPort,
        const std::string &strKey,
        const std::string &strContents,
        std::string &strResult);
    bool PostMsg(const std::string &strKey,
        const std::string &strContents,
        std::string &strResult);
    bool SendAndRecv(const std::string &strHead,
        const std::string &strContents,
        std::string &strResult);
    size_t GetContentLength(const std::string &strHead);
    bool Is200OKMsg(const std::string &strHead, std::string &strResult);
    //×Ô¶¨Òå
    static bool XmlAnalysis(std::string &strResult);
private:
    HPR_SOCK_T  m_client;
    std::string m_strHost;
    u_short m_nPort;
    std::string m_errMsg;
    static size_t const MAX_SQL_LEN = 4 * 1024;
};
