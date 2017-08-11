/**	@file    PreviewDef.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   预览Dll全局数据定义
 *
 *	@author	 Liu Ronghua
 *	@date	 2013/01/04
 *	@note   
 *	@note    历史记录：
 *	@note    V1.0  create at 2013/01/04
 */

#ifndef _PREVIEW_DEF_
#define _PREVIEW_DEF_

//////////////////////////////////////////////////////////////////////////
// define

#define WM_WRITE_WORKLOG            1024

/*
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<Preview>
    <CamID></CamID>         // 监控点ID
    <LogType></LogType>     // 操作日志类型
    <Result></Result>       // 操作结果
    <Remark></Remark>       // 备注
</Preview>
*/

#define PV_XML_HEADER           _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
#define PV_XML_PREVIEW          _T("Preview")
#define PV_XML_CAMID            _T("CamID")
#define PV_XML_CAMNAME          _T("CamName")
#define PV_XML_DEVTYPE          _T("DevType")
#define PV_XML_REGIONID         _T("RegionID")
#define PV_XML_DEVIP            _T("DevIP")
#define PV_XML_DEVPORT          _T("DevPort")
#define PV_XML_USER             _T("User")
#define PV_XML_PWD              _T("Pwd")
#define PV_XML_CHANNEL          _T("Channel")
#define PV_XML_REALPLAYTYPE     _T("RealPlayType")
#define PV_XML_VIDEOFORMAT      _T("VideoFormat")
#define PV_XML_CAMTYPE          _T("CamType")
#define PV_XML_CAMRIGHT         _T("CamRight")
#define PV_XML_STREAMIP         _T("StreamIP")
#define PV_XML_STREAMPORT       _T("StreamPort")
#define PV_XML_STREAMSRVID      _T("StreamSrvID")
#define PV_XML_DEVICEID         _T("DeviceID")
#define PV_XML_PROTOCOL         _T("Protocol")
#define PV_XML_PREVIEWTYPE      _T("PreviewType")
#define PV_XML_LOGTYPE          _T("LogType")
#define PV_XML_RESULT           _T("Result")
#define PV_XML_REMARK           _T("Remark")
#define PV_XML_USERID           _T("UserID")
#define PV_XML_COLLECTORIP      _T("CollectorIP")
#define PV_XML_PHONEPORT        _T("PhonePort")
#define PV_XML_VEDIOPORT        _T("VedioPort")
#define PV_XML_PCHANNEL         _T("PChannel")
#define PV_XML_TIMEOUT          _T("TimeOut")
#define PV_XML_DELAYTIME        _T("DelayTime")
#define PV_XML_PHONEENABLE      _T("PhoneEnable")
#define PV_XML_MAXRECORDTIME    _T("MaxRecordTime")
#define PV_XML_CAMINDEX         _T("CamIndex")
#define PV_XML_MICINDEX         _T("MicIndex")
#define PV_XML_MICNAME          _T("MicName")

//////////////////////////////////////////////////////////////////////////
// Struct

typedef struct _tag_PREV_INIT
{
    LONG lPreviewMode;
    LONG lPrevRights;
    LONG lCapType;
    LONG lSkinType;
    LONG lMaxRecSize;
    LONG lHoverToolBarRight;
    LONG lToolBarRight;
    LONG lUserLevel;
	LONG lBtnRights;
    _tag_PREV_INIT()
    {
        lPreviewMode = 0;
        lSkinType = 1;
        lPrevRights = 3;
        lCapType = 0;
        lMaxRecSize = 1;
        lHoverToolBarRight = 31;
        lToolBarRight = 15;
        lUserLevel = 0;
		lBtnRights = 0;
    }
}PREV_INIT;

/*
<Preview>
    <CamID>1</CamID>
    <CamName>Test</CamName>
    <DevType>0</DevType>
    <RegionID>2</RegionID>
    <DevIP>10.64.60.101</DevIP>
    <DevPort>8000</DevPort>
    <User>admin</User>
    <Pwd>12345</Pwd>
    <Channel>1</Channel>
    <RealPlayType>0</RealPlayType>
    <VideoFormat>0</VideoFormat>
    <CamType>0</CamType>
    <CamRight></CamRight>
    <StreamIP>10.64.56.212</StreamIP>
    <StreamPort>27070</StreamPort>
    <Protocol></Protocol>
    <PreviewType></PreviewType>
	<UserID>1</UserID>
	<CollectorIP>10.64.56.212</CollectorIP>
	<PhonePort>9527</PhonePort>
	<VedioPort>9527</VedioPort>
	<PChannel>0</PChannel>
</Preview>
*/

#endif