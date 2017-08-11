#include "stdafx.h"
#include "SimpleHttp.h"
using namespace std;

namespace {
char const HTTP_LINE_TAIL[] = "\r\n";
size_t const HTTP_LINE_TAIL_LEN = ::strlen(HTTP_LINE_TAIL);
char const HEAD_MSG_TAIL[] = "\r\n\r\n";
size_t const HEAD_MSG_TAIL_LEN = strlen(HEAD_MSG_TAIL);
char const SUCC_FLAG[] = "succ";
char const FAIL_FLAG[] = "fail";
size_t const MIN_CONTENT_LEN = ::strlen(SUCC_FLAG);
char const CONTENT_LENGTH[] = "Content-Length:";
size_t const CONTENT_LENGTH_LEN = ::strlen(CONTENT_LENGTH);
}

CSimpleHttp::CSimpleHttp(void)
{
}
CSimpleHttp::~CSimpleHttp()
{
    try
    {
    }
    catch (...)
    {
    
    }
}
//
const std::string &CSimpleHttp::GetErrorMsg(void) const
{
    return m_errMsg;
}
//
void CSimpleHttp::Close()
{
    if (m_client != HPR_INVALID_SOCKET)
    {
        HPR_ShutDown(m_client, HPR_SHUT_RDWR);
        HPR_CloseSocket(m_client);
        m_client = HPR_INVALID_SOCKET;
    }
}
//
bool CSimpleHttp::Connect(const std::string &strHost, HPR_UINT16 nPort/* = 80*/)
{
    bool bRet = false;
    do 
    {
        Close();

        m_client = HPR_CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (HPR_INVALID_SOCKET == m_client)
        {
            stringstream ss;
            ss << "HPR_CreateSocket failed, errno:" << HPR_GetLastError();
            m_errMsg = ss.str();
            break;
        }

        HPR_ADDR_T addr = {0};
        if (HPR_MakeAddrByString(HPR_AF_INET, strHost.c_str(), nPort, &addr) != 0)
        {
            stringstream ss;
            ss << "HPR_MakeAddrByString " << strHost.c_str() << ":" << nPort
                << " failed, errno:" << HPR_GetLastError();
            m_errMsg = ss.str();
            break;
        }

        if (HPR_ConnectWithTimeOut(m_client, &addr, 5000) != 0)
        {
            stringstream ss;
            ss << "connect " << strHost.c_str() << ":" << nPort
                << " failed, errno:" << HPR_GetLastError();
            m_errMsg = ss.str();
            break;
        }

        bRet = true;
    } while (0);

    if (!bRet)
    {
        Close();
    }

    m_strHost = strHost;
    m_nPort = nPort;

    return bRet;
}
//
bool CSimpleHttp::GetLocalIp(std::string &strIp)
{
    char local[255] = {0};
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (ph == NULL)
    {
        return false;
    }
    // 这里仅获取第一个ip
    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
    std::string localIP;
    localIP.assign(inet_ntoa(addr));

    strIp = localIP.c_str();

    return true;
}
//
bool CSimpleHttp::PostMsg(
                          const std::string &strHost,
                          HPR_UINT16 nPort,
                          const std::string &strKey,
                          const std::string &strContents,
                          std::string &strResult)
{
    CSimpleHttp http;
    std::string strIP = strHost;
    if (strIP == "127.0.0.1")
    {
        GetLocalIp(strIP);
    }
    if (!http.Connect(strIP, nPort))
    {
        strResult = http.GetErrorMsg();
        return false;
    }

    bool bRet = http.PostMsg(strKey, strContents, strResult);

    http.Close();

    return bRet;
}
bool CSimpleHttp::PostMsg(
    const std::string &strKey,
    const std::string &strContents,
    std::string &strResult)
{
    //assert(HPR_INVALID_SOCKET != m_client);
    if (HPR_INVALID_SOCKET == m_client)
    {
        strResult = "not connect";
        return false;
    }
    //assert(!strKey.empty());
    //assert(!strContents.empty());
    if (strKey.empty() /*|| strContents.empty()*/)
    {
        strResult = "invalid param!";
        return false;
    }

    stringstream ss;
    ss << "POST " << strKey << " HTTP/1.1\r\n"
        << "Accept: */*\r\n"
        << "Content-Type: application/x-www-form-urlencoded\r\n"
        << "User-Agent: Mozilla/4.0\r\n"
        << "Host: " << m_strHost + ":" << m_nPort << "\r\n"
        << "Content-Length: " << strContents.size() << "\r\n"
        << "Connection: Close\r\n"
        << "\r\n";
    string strHead = ss.str();

    return SendAndRecv(strHead, strContents, strResult);
}
//
bool CSimpleHttp::SendAndRecv(
                              const std::string &strHead,
                              const std::string &strContents,
                              std::string &strResult)
{
    HPR_INT32 const headSize = (HPR_INT32)strHead.size();
    if(headSize != HPR_Send(m_client, (HPR_VOIDPTR)strHead.c_str(), headSize))
    {
        strResult = "send head failed";
        return false;
    }

    if (!strContents.empty())
    {
        size_t nOffset = 0;
        size_t dataSize = strContents.size();
        char *pData = (char*)strContents.data();
        while (dataSize > nOffset)
        {
            HPR_INT32 nSend = HPR_Send(m_client, pData + nOffset, dataSize - nOffset);
            if (nSend == -1)
            {
                strResult = "send data failed";
                return false;
            }
            nOffset += nSend;
        }
    }

    string strResponseHead;
    string strRecv;
    size_t total_to_recv = 0;
    string::size_type contentPos = 0;
    for (;;)
    {
        char szTemp[1024] = {0};
        const HPR_INT32 nRecv = HPR_Recv(m_client, szTemp, sizeof(szTemp));
        if (nRecv <= 0)
        {
            break;
        }

        strRecv.append(szTemp, nRecv);

        // get head
        if (strResponseHead.empty()) 
        {
            string::size_type nPos = strRecv.find(HEAD_MSG_TAIL);
            if (string::npos != nPos)
            {
                contentPos = nPos + HEAD_MSG_TAIL_LEN;
                strResponseHead = strRecv.substr(0, contentPos);
                size_t const contentLen = GetContentLength(strResponseHead);
                total_to_recv = contentPos + contentLen;
                if (strRecv.size() >= total_to_recv)
                {
                    break;
                }
            }
        }
        else
        {
            //assert(0 != total_to_recv);
            if (strRecv.size() >= total_to_recv)
            {
                break;
            }
        }
    }

    if (strResponseHead.empty())
    {
        strResult = "failed to recv response";
        return false;
    }
    if (!Is200OKMsg(strResponseHead, strResult))
    {
        return false;
    }

    string strResponseContent = strRecv.substr(contentPos);
    if (strResponseContent.size() < MIN_CONTENT_LEN)
    {
        strResult = "bad 200 msg, too small";
        return false;
    }

    strResult = strResponseContent.substr(MIN_CONTENT_LEN);

    //if (0 != _strnicmp(SUCC_FLAG, strResponseContent.c_str(), MIN_CONTENT_LEN))
    //{
    //    // 错误原因在报文里，不用指定了
    //    return false;
    //}

    return true;
}
size_t CSimpleHttp::GetContentLength(const std::string &strHead)
{
    string::size_type pos1 = strHead.find(CONTENT_LENGTH);
    if (string::npos == pos1)
    {
        return 0;
    }
    pos1 += CONTENT_LENGTH_LEN;

    string::size_type pos2 = strHead.find(HTTP_LINE_TAIL, pos1);
    if (string::npos == pos2)
    {
        return 0;
    }

    return size_t(::atoi(strHead.data() + pos1));
}
bool CSimpleHttp::Is200OKMsg(
                             const std::string &strHead,
                             std::string &strResult)
{
    string::size_type const pos = strHead.find(HTTP_LINE_TAIL);
    if (string::npos == pos)
    {
        strResult = "bad head, no line";
        return false;
    }

    string const headLine = strHead.substr(0, pos);
    if (string::npos != headLine.find("200"))
    {
        return true;
    }
    else
    {
        strResult = headLine;
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////
bool CSimpleHttp::XmlAnalysis(std::string &strResult)
{
    //解析出字符串ture就行
    std::string strObject;
    std::string strStartFlag = "<success>";
    std::string strEndFlag = "</success>";
    size_t StartFlagLen = strlen(strStartFlag.c_str());
    size_t EndFlagLen = strlen(strEndFlag.c_str());
    const char *pStart = NULL, *pEnd = NULL;
    pStart = strstr(strResult.c_str(), strStartFlag.c_str()); 
    pEnd = strstr(strResult.c_str(), strEndFlag.c_str());
    strObject.assign(pStart+StartFlagLen,pEnd);//获取截取的字符串
    if (0 != strcmp(strObject.c_str(),"true"))
    {
        TPLOG_INFO("XML-Ture:web返回信息为:%s",strResult.c_str());
        return false;
    }

    return true;
}