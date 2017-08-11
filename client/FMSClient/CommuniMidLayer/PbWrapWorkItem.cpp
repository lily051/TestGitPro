#include "stdafx.h"
#include "PbWrapWorkItem.h"
#include "NetComm.h"
#include <hpp_hpp.h>

//////////////////////////////////////////////////////////////////////////
// FiPbWrapWorkItem
FiPbWrapWorkItem::FiPbWrapWorkItem(void)
	: m_nSeq(0)
	, m_nSrcID(0)
	, m_nCommand(0)
	, m_nMark(0)
{

}

FiPbWrapWorkItem::~FiPbWrapWorkItem(void)
{

}

BOOL FiPbWrapWorkItem::UnderTake(void)
{
    if (m_sMsgSrc.empty())
    {
        CLIENT_WARN("No data for PbWrapWorkItem to undertake");
        return FALSE;
    }

    std::ostringstream stream;
    stream << "FiPbWrapWorkItem(" 
        << m_nSrcID << ","
        << m_nSeq << ","
        << m_nCommand
        << ")";
    string sInfo = stream.str();
    COMM_TRACE("%s", sInfo.c_str());

    // 34009 = CMD_REQ_FORCE_OFFLINE
    // ǿ��������Ϣ��Ĭ�ϲ�����
    if (34009 == m_nCommand)
    {
        m_nMark = HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP;
    }

    switch (m_nMark)
    {
    case HPP_PACK_HEAD::REQ_MSG_NEED_RSP:   //��Ϣ��Ҫ����
        {
            CLIENT_ERROR("REQ_MSG_NEED_RSP not implemented!!!");
            DebugBreak();
        }
        break;
    case HPP_PACK_HEAD::RSP_MSG_NOT_FINISH: //��Ϣû�з�����ɣ��ݲ�����
        {
            COMM_ERROR("RSP_MSG_NOT_FINISH message not supported!");
            DebugBreak();
        }
        break;
    case HPP_PACK_HEAD::RSP_MSG_FINISH: //������Ϣ
        {
            CNetComm::SnglPtr()->RecievingResponseEx(m_sMsgSrc, m_nSrcID, m_nSeq, m_nCommand);
        }
        break;
    default:
        {
            CNetComm::SnglPtr()->AddSessionId(m_nCommand, m_nSrcID);
            CNetComm::SnglPtr()->RecievingTransmissionEx(m_sMsgSrc, m_nCommand);
        }
        break;
    }

    return TRUE;
}
