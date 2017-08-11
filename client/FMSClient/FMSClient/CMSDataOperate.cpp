#include "stdafx.h"
#include "FMSClient.h"
#include "type/cmd.pb.h"
#include "CMSDataOperate.h"
#include "utils/sha256.h"

SINGLETON_IMPLEMENT(CCMSMgr)

CCMSMgr::CCMSMgr()
:m_nLimitPreview(0)
{

}

CCMSMgr::~CCMSMgr()
{

}

CString CCMSMgr::GetClientIP()
{
	m_strClientIP = theApp.m_strAppIp;
	return m_strClientIP;
}

BOOL CCMSMgr::MsgSendRecv(google::protobuf::Message& msgReq, HPR_UINT32 nCmdReq, int reqMsgType, SERVER_INFO &stServerInfo, 
						  google::protobuf::Message& msgRsq, HPR_UINT32 nCmdRsp)
{
	string sRsp;
	int nRet = CNetComm::SnglPtr()->TransmitCompntMsgEx(msgReq.SerializeAsString(), nCmdReq, reqMsgType, stServerInfo, 
		REQ_MSG_WAITING_RSP, nCmdRsp, sRsp);

	if (CNetComm::TRANSMIT_SUCC != nRet || !msgRsq.ParseFromString(sRsp))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCMSMgr::ChangePassword(cms_8120::RspClientLogin& rspClientLogin, CString strNowPassword, CString strNewPassword)
{
	cms_8120::ReqChangePassword msgReq;
	TCHAR szPassword[128] = {0};
	
	//原密码
	_tcscpy_s(szPassword, 120, strNowPassword);
	if (64 != strNowPassword.GetLength())
	{
		char strPwdSHA[64] = {0};
		if (NULL == SHA256(fcT2A(szPassword),
			strNowPassword.GetLength(), strPwdSHA))
		{
			FMSCLIENT_ERROR("user: %s, SHA256 failed", fcT2A(theApp.m_strSalesmanName));
			return FALSE;
		}
		strlwr(strPwdSHA);
		msgReq.set_oldpwd(strPwdSHA);
	}
	else
	{
		msgReq.set_oldpwd(UTF8_MFCSTRW2A(strNowPassword));
	}

	//新密码
	_tcscpy_s(szPassword, 120, strNewPassword);
	// 账户登录使用SHA256加密, SHA256加密后为64位
	if (64 != strNewPassword.GetLength())
	{
		char strPwdSHA[64] = {0};
		if (NULL == SHA256(fcT2A(szPassword),
			strNewPassword.GetLength(), strPwdSHA))
		{
			FMSCLIENT_ERROR("user: %s, SHA256 failed", fcT2A(theApp.m_strSalesmanName));
			return FALSE;
		}
		strlwr(strPwdSHA);
		msgReq.set_newpwd(strPwdSHA);
	}
	else
	{
		msgReq.set_newpwd(UTF8_MFCSTRW2A(strNewPassword));
	}

	msgReq.set_cmd(CMD_DT_CHANGE_PASSWORD);
	msgReq.set_userid(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_CHANGE_PASSWORD, REQ_MSG_WAITING_RSP, serverInfo, rspClientLogin, CMD_DT_CHANGE_PASSWORD);
	if (!bRet)
	{
		FMSCLIENT_ERROR("ChangePassword Failed!");
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSMgr::GetRegion(int nParentID, cms_8120::Region& region)
{
	return TRUE;
}

BOOL CCMSMgr::GetRegionList(int nParentID, cms_8120::RegionList & regionList)
{
	cms_8120::ReqRegion msgReq;
	msgReq.set_cmd(CMD_DT_REGION_LIST);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_REGION_LIST, REQ_MSG_WAITING_RSP, serverInfo, regionList, CMD_DT_REGION_LIST);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetRegionList Failed!");
		return FALSE;
	}
	return TRUE;
}

//获取证件类型列表
BOOL CCMSMgr::GetIdTypeList(cms_8120::ParamDictionaryList & IdTypeList)
{
	cms_8120::ReqParamDictionary msgReq;
	msgReq.set_cmd(CMD_DT_PARAMDICTIONARY_LIST);
	msgReq.CheckInitialized();
	
	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PARAMDICTIONARY_LIST, REQ_MSG_WAITING_RSP, serverInfo, IdTypeList, CMD_DT_PARAMDICTIONARY_LIST);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetIdTypeList Failed!");
		return FALSE;
	}

	return TRUE;
}

//获取产品类型列表
BOOL CCMSMgr::GetProductTypeList(cms_8120::ProductTypeList& productTypeList)
{
	cms_8120::ReqProductType msgReq;
	msgReq.set_cmd(CMD_DT_PRODUCTTYPE_LIST);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PRODUCTTYPE_LIST, REQ_MSG_WAITING_RSP, serverInfo, productTypeList, CMD_DT_PRODUCTTYPE_LIST);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetProductTypeList Failed!");
		return FALSE;
	}

	return TRUE;
}

//获取产品列表
BOOL CCMSMgr::GetProductList(cms_8120::ProductList& productList, int nParentId)
{
	cms_8120::ReqProduct msgReq;
	msgReq.set_cmd(CMD_DT_PRODUCT_LIST);
	if (0 == nParentId)
	{
		msgReq.set_pid(-1);	//获取全部产品列表
	}
	else
	{
		msgReq.set_pid(nParentId);
	}
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PRODUCT_LIST, REQ_MSG_WAITING_RSP, serverInfo, productList, CMD_DT_PRODUCT_LIST);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetProductList Failed!");
		return FALSE;
	}

	return TRUE;
}

//根据产品编码获取产品信息
BOOL CCMSMgr::GetProductInfoByCode(cms_8120::ProductInfo& productInfo, CString strCode)
{
	cms_8120::ReqProduct msgReq;
	msgReq.set_cmd(CMD_DT_PRODUCT_INFO);
	msgReq.set_strcode(UTF8_MFCSTRW2A(strCode));
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.GetCachedSize();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PRODUCT_INFO, REQ_MSG_WAITING_RSP, serverInfo, productInfo, CMD_DT_PRODUCT_INFO);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetProductInfo Failed!");
		return FALSE;
	}

	return TRUE;
}

//提交业务记录
BOOL CCMSMgr::SubmitBusiness(cms_8120::CSClientInput clientInput, CArray<cms_8120::CSRecords>& recordsArray, cms_8120::RspCsClientRecord &rspMsg)
{
	cms_8120::ReqCsClientRecord msgReq;
	msgReq.set_cmd(CMD_WDT_CLIENT_RECORD);
	cms_8120::CSClientInput * input = msgReq.mutable_clientinput();
	input->CopyFrom(clientInput);

	for (int i = 0; i < recordsArray.GetSize(); i++)
	{
		*msgReq.add_records() = recordsArray.GetAt(i);
	}
	msgReq.CheckInitialized();	

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_WDT_CLIENT_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspMsg, CMD_WDT_CLIENT_RECORD);
	if (!bRet)
	{
		FMSCLIENT_ERROR("SubmitBusiness Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspMsg.result_info())));
		OutputDebugString(UTF8_A2MFCSTRW(rspMsg.result_info()));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMSMgr::UpdateBusiness(cms_8120::CSClientInput clientInput, CArray<cms_8120::CSRecords>& recordsArray, cms_8120::RspCsClientRecord& rspMsg)
{
	cms_8120::ReqCsClientRecord msgReq;
	msgReq.set_cmd(CMD_MDT_CLIENT_RECORD);
	cms_8120::CSClientInput * input = msgReq.mutable_clientinput();
	input->CopyFrom(clientInput);

	for (int i = 0; i < recordsArray.GetSize(); i++)
	{
		*msgReq.add_records() = recordsArray.GetAt(i);
	}
	msgReq.CheckInitialized();	

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_MDT_CLIENT_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspMsg, CMD_MDT_CLIENT_RECORD);
	if (!bRet)
	{
		FMSCLIENT_ERROR("UpdateBusiness Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspMsg.result_info())));
		OutputDebugString(UTF8_A2MFCSTRW(rspMsg.result_info()));
		return FALSE;
	}

	return TRUE;
}

//获取产品话术
BOOL CCMSMgr::GetWordTemplate(cms_8120::ProductInfo& productInfo, int nProductId)
{
	cms_8120::ReqProduct msgReq;
	msgReq.set_cmd(CMD_DT_PRODUCT_WORD_TEMPLATE);
	msgReq.set_id(nProductId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PRODUCT_WORD_TEMPLATE, REQ_MSG_WAITING_RSP, serverInfo, productInfo, CMD_DT_PRODUCT_WORD_TEMPLATE);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetWordTemplate Failed!%s", fcT2A(UTF8_A2MFCSTRW(productInfo.result_info())));
		return FALSE;
	}
	return TRUE;
}

//获取产品详细信息
BOOL CCMSMgr::GetProductInfo(cms_8120::ProductInfo& productInfo, int nProductId)
{
	cms_8120::ReqProduct msgReq;
	msgReq.set_cmd(CMD_DT_PRODUCT_INFO);
	msgReq.set_id(nProductId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_PRODUCT_INFO, REQ_MSG_WAITING_RSP, serverInfo, productInfo, CMD_DT_PRODUCT_INFO);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetProductInfo Failed!%s", fcT2A(UTF8_A2MFCSTRW(productInfo.result_info())));
		return FALSE;
	}
	return TRUE;
}

//获取历史业务单
BOOL CCMSMgr::GetHistoryList(cms_8120::RspCHisClientInput& rspHisClientInput, cms_8120::HisClientInput clientInput, CString strStartTime, CString strEndTime)
{
	cms_8120::ReqHisClientInput msgReq;
	msgReq.set_cmd(CMD_DT_HIS_CLIENTINPUT);
	msgReq.set_startdate(UTF8_MFCSTRW2A(strStartTime));
	msgReq.set_enddate(UTF8_MFCSTRW2A(strEndTime));
	msgReq.set_custname(clientInput.name());
	msgReq.set_nproid(clientInput.nproid());
	msgReq.set_inputuser(clientInput.inputuser());
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_HIS_CLIENTINPUT, REQ_MSG_WAITING_RSP, serverInfo, rspHisClientInput, CMD_DT_HIS_CLIENTINPUT);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetHistoryList Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspHisClientInput.result_info())));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSMgr::GetBusinessList(cms_8120::RspCsClientRecordsInfo &rspBusinessInfo, CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName, int nRecordTab)
{
	cms_8120::ReqRecordsInfo msgReq;
	msgReq.set_cmd(CMD_DT_CLIENT_RECORD);
	msgReq.set_recordtab(nRecordTab);  //保留字段
	msgReq.set_startdate(UTF8_MFCSTRW2A(strStartTime));
	msgReq.set_enddate(UTF8_MFCSTRW2A(strEndTime));
	if (_T("") != strProductName)
	{
		msgReq.set_productname(UTF8_MFCSTRW2A(strProductName));
	}
	if (_T("") != strConsumerName)
	{
		msgReq.set_custname(UTF8_MFCSTRW2A(strConsumerName));
	}	
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_CLIENT_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspBusinessInfo, CMD_DT_CLIENT_RECORD);
	if (!bRet)
	{
		CString str = UTF8_A2MFCSTRW(rspBusinessInfo.result_info());
		FMSCLIENT_ERROR("GetBusinessList Failed!%s", fcT2A(str));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSMgr::GetErrorRecList(cms_8120::RspErrRecordsInfo& rspErrorRecInfo, CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName, int nRecordTab)
{
	cms_8120::ReqRecordsInfo msgReq;
	msgReq.set_cmd(CMD_DT_ERROR_RECORD);
	msgReq.set_startdate(UTF8_MFCSTRW2A(strStartTime));
	msgReq.set_enddate(UTF8_MFCSTRW2A(strEndTime));
	if (_T("") != strProductName)
	{
		msgReq.set_productname(UTF8_MFCSTRW2A(strProductName));
	}
	if (_T("") != strConsumerName)
	{
		msgReq.set_custname(UTF8_MFCSTRW2A(strConsumerName));
	}	
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_ERROR_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspErrorRecInfo, CMD_DT_ERROR_RECORD);
	if (!bRet)
	{
		CString str = UTF8_A2MFCSTRW(rspErrorRecInfo.result_info());
		FMSCLIENT_ERROR("GetErrorRecList Failed!%s", fcT2A(str));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSMgr::DeleteErrorRecord(cms_8120::RspCsClientRecord& rspErrorRecord, CUIntArray& recIdArray)
{
	cms_8120::ReqErrorRecord msgReq;
	msgReq.set_cmd(CMD_DDT_ERROR_RECORD);
	for (int i = 0; i < recIdArray.GetSize(); i++)
	{
		msgReq.add_id(recIdArray.GetAt(i));
	}
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DDT_ERROR_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspErrorRecord, CMD_DDT_ERROR_RECORD);
	if (!bRet)
	{
		FMSCLIENT_ERROR("DeleteErrorRecord Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspErrorRecord.result_info())));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMSMgr::ReUploadRecord(cms_8120::RspCsClientRecord& rspErrorRecord, CUIntArray& recIdArray)
{
	cms_8120::ReqErrorRecord msgReq;
	msgReq.set_cmd(CMD_MDT_ERROR_RECORD);
	for (int i = 0; i < recIdArray.GetSize(); i++)
	{
		msgReq.add_id(recIdArray.GetAt(i));
	}
	msgReq.set_inputuser(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_MDT_ERROR_RECORD, REQ_MSG_WAITING_RSP, serverInfo, rspErrorRecord, CMD_MDT_ERROR_RECORD);
	if (!bRet)
	{
		FMSCLIENT_ERROR("ReUploadRecord Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspErrorRecord.result_info())));
		return FALSE;
	}

	return TRUE;
}

//获取系统配置信息
BOOL CCMSMgr::GetSystemConfig(cms_8120::RspSysconfig& rspSystemCfg)
{
	cms_8120::ReqSysconfig msgReq;
	msgReq.set_cmd(CMD_DT_SYSTEM_CONFIG);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_DT_SYSTEM_CONFIG, REQ_MSG_WAITING_RSP, serverInfo, rspSystemCfg, CMD_DT_SYSTEM_CONFIG);
	if (!bRet)
	{
		FMSCLIENT_ERROR("GetSystemConfig Failed!%s", fcT2A(UTF8_A2MFCSTRW(rspSystemCfg.result_info())));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSMgr::WriteWorkState(int nFirst, std::string strip, int nPort, cms_8120::RspClientInfo & rspClientInfo)
{
// 	cms_8120::WWorkState wState;
// 	wState.set_data_type(CMD_WRITE_WORK_STATE);
// 	wState.set_module_type(MT_CLIENT_CS);
// 	wState.set_user_or_server_id(theApp.m_nUserId);
// 	wState.set_login_ip(strip);
// 	wState.set_login_port(nPort);

	cms_8120::ReqClientInfo msgReq;
	msgReq.set_cmd(CMD_WRITE_WORK_STATE);
	msgReq.set_userid(theApp.m_nUserId);
	msgReq.CheckInitialized();


	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_WRITE_WORK_STATE, REQ_MSG_WAITING_RSP, serverInfo, rspClientInfo, CMD_WRITE_WORK_STATE);
	if (!bRet)
	{
		FMSCLIENT_ERROR("WriteWorkState Failed!%d", rspClientInfo.result());
		return FALSE;
	}

	return TRUE;
}

BOOL CCMSMgr::ClientLogout()
{
// 	cms_8120::MsgClientLogout msgLogout;
// 	msgLogout.set_cmd(CMD_MSG_CLIENT_LOGOUT);
// 	msgLogout.set_moudle_type(cms_8120::MT_CLIENT_CS);
// 	msgLogout.set_user_name(UTF8_MFCSTRW2A(theApp.m_strSalesmanName));
// 	msgLogout.set_ip(UTF8_MFCSTRW2A(theApp.m_strAppIp));
// 	msgLogout.set_dc_ip(UTF8_MFCSTRW2A(theApp.m_strServerIp));
// 	msgLogout.CheckInitialized();

	cms_8120::ReqClientInfo msgReq;
	msgReq.set_cmd(CMD_MSG_CLIENT_LOGOUT);
	msgReq.set_userid(theApp.m_nUserId);
	msgReq.CheckInitialized();

	SERVER_INFO serverInfo;
	serverInfo.nServerID = CMS_SERVER;
	swprintf_s(serverInfo.szServerIp, 16, theApp.m_strServerIp);
	serverInfo.nServerPort = theApp.m_nServerPort;

	cms_8120::RspClientInfo rsp;

	BOOL bRet = FALSE;
	bRet = MsgSendRecv(msgReq, CMD_MSG_CLIENT_LOGOUT, REQ_MSG_NOT_NEED_RSP, serverInfo, rsp, CMD_MSG_CLIENT_LOGOUT);
	if (!bRet)
	{
		FMSCLIENT_ERROR("ClientLogout Failed!%d", bRet);
		return FALSE;
	}
	return TRUE;
}

// void CCMSMgr::ResolveLoginRspDataEx()
// {
// 	CString strDataEx = UTF8_A2MFCSTRW(m_rspMsg.data_ex());
// 	CMarkup dataExXml;
// 	dataExXml.SetDoc(strDataEx);
// 	if (dataExXml.FindElem(_T("DataEx")))
// 	{	
// 		if (dataExXml.FindChildElem(_T("ClientIP")))
// 		{
// 			m_strClientIP = dataExXml.GetChildData();
// 		}
// 		if (dataExXml.FindChildElem(_T("RealTimeGisWeb")))
// 		{
// 			m_strRealTimeGisWeb = dataExXml.GetChildData();
// 		}
// 		if (dataExXml.FindChildElem(_T("PlaybackGisWeb")))
// 		{
// 			m_strPlaybackGisWeb = dataExXml.GetChildData();
// 		}	
//         if (dataExXml.FindChildElem(_T("EquatorRadius")))
//         {
//            CVehicleData::EQUATE_RADIUS = _wtof(dataExXml.GetChildData().GetBuffer());;
//         }	
//         if (dataExXml.FindChildElem(_T("PolarRadius")))
//         {
//             CVehicleData::POLAR_RADIUS = _wtof(dataExXml.GetChildData().GetBuffer());
//         }
// 		if (dataExXml.FindChildElem(_T("LimitPreview")))
// 		{
// 			CString strLimitPre = dataExXml.GetChildData();
// 			m_nLimitPreview = _ttoi(strLimitPre);
// 		}
// 	}
// }

SERVER_INFO CCMSMgr::GetServerInfo()
{
	return m_serverInfo;
}

