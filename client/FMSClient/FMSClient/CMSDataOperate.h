#pragma once
#include "AppDef.h"
#include "csclient//Region.pb.h"
#include "csclient/ParamDictionary.pb.h"
#include "csclient/ProductType.pb.h"
#include "csclient/Product.pb.h"
#include "csclient/CsClientRecord.pb.h"
#include "csclient/HistoryClientInput.pb.h"
#include "csclient/CsSysconfig.pb.h"
#include "verify/MsgClientLogout.pb.h"
#include "csclient/WorkState.pb.h"
#include "server/RspServerData.pb.h"

using namespace cms_8120;


enum 
{
// 	SEND_MSG = LOG_TYPE_SEND_MSG,
// 	PREVIEW_TALK = LOG_TYPE_TALK,
// 	PREVIEW_CAMERA = LOG_TYPE_PREVIEW,
// 	PLAYBACK_CAMERA = LOG_TYPE_PLAYBACK,
// 	PLAYBACK_GIS = LOG_TYPE_PLAYBACK_GIS,
// 	OPEN_OID = LOG_TYPE_OPEN_OID,
// 	CLOSE_OID = LOG_TYPE_CLOSE_OID,
// 	OPEN_ELECTRIC = LOG_TYPE_OPEN_ELECTRIC,
// 	CLOSE_ELECTRIC = LOG_TYPE_CLOSE_ELECTRIC
};

class CCMSMgr
{
	SINGLETON_DECLARE(CCMSMgr) 

public:
	CCMSMgr();
	~CCMSMgr();
	
private:
	SERVER_INFO m_serverInfo;
// 	ReqClientLogin m_reqMsg;
// 	RspClientLogin m_rspMsg;
	CString m_strClientIP;
	CString m_strRealTimeGisWeb;
	CString m_strPlaybackGisWeb;
    CString  strEquatorRadius;
    CString strPolarRadius;
	int m_nLimitPreview;
public:
	//void ResolveLoginRspDataEx();
	SERVER_INFO GetServerInfo();
	CString GetClientIP();

protected:
	BOOL MsgSendRecv(google::protobuf::Message& msgReq, HPR_UINT32 nCmdReq, int reqMsgType, SERVER_INFO &stServerInfo, 
		google::protobuf::Message& msgRsq, HPR_UINT32 nCmdRsp);
	
public:
	BOOL ChangePassword(cms_8120::RspClientLogin& rspClientLogin, CString strNowPassword, CString strNewPassword);
	BOOL GetRegion(int nParentID, cms_8120::Region& region);
	BOOL GetRegionList(int nParentID, cms_8120::RegionList & regionList);
	BOOL GetIdTypeList(cms_8120::ParamDictionaryList & IdTypeList);
	BOOL GetProductTypeList(cms_8120::ProductTypeList& productTypeList);
	BOOL GetProductList(cms_8120::ProductList& productList, int nParentId);
	BOOL GetProductInfoByCode(cms_8120::ProductInfo& productInfo, CString strCode);
	BOOL SubmitBusiness(cms_8120::CSClientInput clientInput, CArray<cms_8120::CSRecords>& recordsArray, cms_8120::RspCsClientRecord& rspMsg);
	BOOL UpdateBusiness(cms_8120::CSClientInput clientInput, CArray<cms_8120::CSRecords>& recordsArray, cms_8120::RspCsClientRecord& rspMsg);
	BOOL GetWordTemplate(cms_8120::ProductInfo& productInfo, int nProductId);
	BOOL GetProductInfo(cms_8120::ProductInfo& productInfo, int nProductId);
	BOOL GetHistoryList(cms_8120::RspCHisClientInput& rspHisClientInput, cms_8120::HisClientInput clientInput, CString strStartTime, CString strEndTime);
	//获取业务记录（业务单）列表
	BOOL GetBusinessList(cms_8120::RspCsClientRecordsInfo& rspBusinessInfo, CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName, int nRecordTab);
	//获取异常录像列表
	BOOL GetErrorRecList(cms_8120::RspErrRecordsInfo& rspErrorRecInfo, CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName, int nRecordTab);
	BOOL DeleteErrorRecord(cms_8120::RspCsClientRecord& rspErrorRecord, CUIntArray& recIdArray);
	BOOL ReUploadRecord(cms_8120::RspCsClientRecord& rspErrorRecord, CUIntArray& recIdArray);
	BOOL GetSystemConfig(cms_8120::RspSysconfig& rspSystemCfg);
	BOOL WriteWorkState(int nFirst, std::string strip, int nPort, cms_8120::RspClientInfo & rspClientInfo);
	BOOL ClientLogout(void);
};