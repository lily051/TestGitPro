#include "stdafx.h"
#include "SimpleHttp.h"
#include <sstream>
#include <assert.h>
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
: m_client(HPR_INVALID_SOCKET)
, m_nPort(80)
{
}
CSimpleHttp::~CSimpleHttp()
{
	Close();
}

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

void CSimpleHttp::Close()
{
	if (m_client != HPR_INVALID_SOCKET)
	{
		HPR_ShutDown(m_client, HPR_SHUT_RDWR);
		HPR_CloseSocket(m_client);
		m_client = HPR_INVALID_SOCKET;
	}
}

bool CSimpleHttp::PostMsg(
    const std::string &strKey,
    const std::string &strContents,
    std::string &strResult)
{
    assert(HPR_INVALID_SOCKET != m_client);
    if (HPR_INVALID_SOCKET == m_client)
    {
        strResult = "not connect";
        return false;
    }
    assert(!strKey.empty());
    assert(!strContents.empty());
    if (strKey.empty() || strContents.empty())
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
bool CSimpleHttp::GetMsg(
    const std::string &strKey,
    std::string &strResult)
{
    assert(HPR_INVALID_SOCKET != m_client);
    if (HPR_INVALID_SOCKET == m_client)
    {
        strResult = "not connect";
        return false;
    }
    assert(!strKey.empty());
    if (strKey.empty())
    {
        strResult = "invalid param!";
        return false;
    }

    stringstream ss;

    ss << "GET " << strKey << " HTTP/1.1\r\n"
        << "Accept: */*\r\n"
        << "Accept-Encoding: gzip, deflate\r\n"
        << "User-Agent: Mozilla/4.0\r\n"
        << "Host: " << m_strHost + ":" << m_nPort << "\r\n"
        << "Connection: Close\r\n"
        << "\r\n";
    string strHead = ss.str();

    return SendAndRecv(strHead, "", strResult);
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
            assert(0 != total_to_recv);
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
const std::string &CSimpleHttp::GetErrorMsg(void) const
{
    return m_errMsg;
}
bool CSimpleHttp::PostMsg(
    const std::string &strHost,
    HPR_UINT16 nPort,
    const std::string &strKey,
    const std::string &strContents,
    std::string &strResult)
{
    CSimpleHttp http;
    if (!http.Connect(strHost, nPort))
    {
        strResult = http.GetErrorMsg();
        return false;
    }

    bool bRet = http.PostMsg(strKey, strContents, strResult);

    http.Close();

    return bRet;
}

bool CSimpleHttp::GetMsg(
    const std::string &strHost,
    HPR_UINT16 nPort,
    const std::string &strKey,
    std::string &strResult)
{
    CSimpleHttp http;
    if (!http.Connect(strHost, nPort))
    {
        strResult = http.GetErrorMsg();
        return false;
    }

    bool bRet = http.GetMsg(strKey, strResult);

    http.Close();

    return bRet;
}

std::vector<std::string> CSimpleHttp::Split( const std::string& str,
                                            const std::string& pattern )
{
    std::string strTemp = str+pattern;//扩展字符串以方便操作
    std::string::size_type pos;
    std::vector<std::string> result;
    std::string::size_type size=strTemp.size();
    for(std::string::size_type i=0; i<size; i++)
    {
        pos=strTemp.find(pattern,i);
        if(pos<size)
        {
            std::string s=strTemp.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

_tstring CSimpleHttp::strTime(__time32_t const t)
{
	struct tm _tm;
	if (0 != _localtime32_s(&_tm, &t))
	{
		return _T("");
	}

	TCHAR szTime[32] = {0};
	_stprintf_s(szTime, _countof(szTime), _T("%4d-%02d-%02d %2d:%02d:%02d"),
		_tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
		_tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	return szTime;    
}

//bool CSimpleHttp::AddRecordInfo(const center_info_t &centerInfo,
//							   const fileToTrans_t &fileToTrans,
//							   const fileTransing_t &fileTransing,
//							   const cloud_info_t &cloudInfo)
//{
//	std::string strResult;
//	std::string strWebAddr ="/dataCenter!saveRecord";
//	_tstring beginTime = strTime(fileToTrans.fileBegintime);
//	_tstring endTime = strTime(fileToTrans.fileEndtime);
//	char szContents[1024]={0};
//	sprintf_s(szContents, 
//		"input.nproId=%s&record.recordId=%s&record.regionId=%d&record.ntype=%d&\
//		record.mediaType=%d&record.calltype=%d&record.callNo=%s&record.recordUserNo=%s&\
//		record.stateType=%d&record.startTime=%s&record.endTime=%s",
//		fcT2U(fileToTrans.strLinkOrderID.c_str()),
//		fcT2U(fileTransing.channelID.c_str()),
//		fileToTrans.regionID,
//		fileToTrans.mediaFormat,
//		fileToTrans.mediaType,
//		fileToTrans.callType,
//		fcT2U(fileToTrans.callNo.c_str()),
//		fcT2U(fileToTrans.strUserNo.c_str()),
//		fileToTrans.linkType,
//		fcT2U(beginTime.c_str()), 
//		fcT2U(endTime.c_str()));
//	std::string strContents = szContents;
//	VIEWMSGLOG_INFO("AddRecordInfo:%s", strContents.c_str());
//	if(!CSimpleHttp::PostMsg(fcT2A(centerInfo.ip.c_str()),
//		centerInfo.port,
//		strWebAddr,
//		strContents,
//		strResult))
//	{
//		return false;
//	}
//	else
//	{
//		VIEWMSGLOG_INFO("AddRecordInfo:%s", strResult.c_str());
//		CMarkup xml;
//		xml.SetDoc(fcA2T(strResult.c_str()));
//		xml.ResetMainPos();
//
//		xml.FindChildElem(fcA2T("success"));
//		std::_tstring strsuccess = xml.GetChildData().c_str();
//		if (strcmp(fcT2A(strsuccess.c_str()), "false") != 0)
//		{
//			VIEWMSGLOG_ERROR("AddRecordInfo failed");
//			return false;
//		}
//		return true;
//	}
//}

bool CSimpleHttp::AddCenterAlarmLog(const center_info_t &centerInfo,
							  const alarm_msg_t &stAlarm)
{
	std::string strResult;
	std::string strWebAddr ="/dataCenter!saveAlarmInfo";
	char szContents[1024]={0};
	sprintf_s(szContents, 
		"ip=%s&port=%d&alarmlog.checkType=%d&\
		alarmlog.checkChan=%d&alarmlog.contactObjId=%d&\
		alarmlog.alarmType=%d&alarmlog.alarmTime=%s&\
		alarmlog.alarmRegionId=%d&alarmlog.comments=%s&\
		alarmlog.status=%d&alarmlog.strGuid=%s",
		fcT2U(stAlarm.strIP.c_str()),
		stAlarm.nport,
		stAlarm.nParentKey,
		stAlarm.nChannel,
		stAlarm.nRev,
		stAlarm.nKey,
		fcT2U(stAlarm.strOccureTime.c_str()),
		0,
		fcT2U(stAlarm.strInfo.c_str()),
		stAlarm.nState,
		fcT2U(stAlarm.strGUID.c_str()));
	std::string strContents = szContents;
	VIEWMSGLOG_INFO("AddRecordInfo:%s", strContents.c_str());
	if(!CSimpleHttp::PostMsg(fcT2A(centerInfo.ip.c_str()),
		centerInfo.port,
		strWebAddr,
		strContents,
		strResult))
	{
		return false;
	}
	else
	{
		VIEWMSGLOG_INFO("AddRecordInfo:%s", strResult.c_str());
		CMarkup xml;
		xml.SetDoc(fcA2T(strResult.c_str()));
		xml.ResetMainPos();

		xml.FindChildElem(fcA2T("success"));
		std::_tstring strsuccess = xml.GetChildData().c_str();
		if (strcmp(fcT2A(strsuccess.c_str()), "false") != 0)
		{
			VIEWMSGLOG_ERROR("AddRecordInfo failed");
			return false;
		}
		return true;
	}
}
