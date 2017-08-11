// PlaybackMainWnd.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "PlaybackOCX2.0.h"
#include "PlaybackMainWnd.h"
#include "xmldef.h"
#include "base64.h"

// CPlaybackMainWnd


IMPLEMENT_DYNAMIC(CPlaybackMainWnd, CWnd)

CPlaybackMainWnd::CPlaybackMainWnd(PBSDLayout emLayout, PBSDSkinType emSkinType, DWORD dwStyle, LPCTSTR lpstrIP, LONG LPort, LONG lUserLevel)
: m_emLayout(emLayout)
, m_emSkinType(emSkinType)
, m_dwStyle(dwStyle)
, m_hDlg(NULL)
, m_lpstrIP(lpstrIP)
, m_lPort(LPort)
, m_lUserLevel(lUserLevel)
, m_lBwControl(0)
{

}

CPlaybackMainWnd::~CPlaybackMainWnd()
{
	m_hDlg = NULL;
}


BEGIN_MESSAGE_MAP(CPlaybackMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPlaybackMainWnd 消息处理程序



int CPlaybackMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码 
 	if (NULL == this->GetSafeHwnd())
 	{
 		OutputDebugString(_T("@z safehwnd is null"));
 		return 0;
 	}

 	m_hDlg = PBSD_Create(this->GetSafeHwnd(), m_emLayout);
 	if (m_hDlg != NULL)
 	{
 		PBSD_ShowWindow(m_hDlg, SW_SHOW);
		PBSD_SetIEProtected();
 	}

	return 0;
}

void CPlaybackMainWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_hDlg != NULL)
	{
		PBSD_Destroy(m_hDlg);
	}
}

void CPlaybackMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
 	if (m_hDlg != NULL)
 	{
 		CRect rcClient(0, 0, cx, cy);
 		PBSD_MoveWindow(m_hDlg, &rcClient);
 	}
}


/**  @fn           Playback
*    @brief   	   开始回放.
*    @param[in]    lpstrXML:xml参数报文.
*    @param[in]   
*    @return       0-成功,非0-失败.
*/
LONG CPlaybackMainWnd::Playback(LPCTSTR lpstrXML)
{
	int nWndNo = 0;
	PBSDLoginParam stLoginInfo;
	PBSDPlayParam stPlayParam;
	DWORD dwRight = 0;
	if (!ParsePlaybackXML(lpstrXML, nWndNo, dwRight, stLoginInfo, stPlayParam))
	{
		return 0;
	}

	if (!PBSD_PlaybackByWndNo(m_hDlg, nWndNo, &stLoginInfo, &stPlayParam, dwRight))
	{
		return 0;
	}

	return 1;
}

LONG CPlaybackMainWnd::LocalPlayback(LPCTSTR lpstrXML)
{
	CMarkup xml;
	if (!xml.SetDoc(lpstrXML))
	{
		return 0;
	}

	if (!xml.FindElem(XML_PARAM))
	{
		return 0;
	}
	// 录像类型.
	xml.ResetChildPos();
	if (!xml.FindChildElem(XML_VIDEOTYPE))
	{
		return 0;
	}
	int nVideoType = _wtoi(xml.GetChildData());
	// 文件路径.
	xml.ResetChildPos();
	if (!xml.FindChildElem(XML_FILEPATH))
	{
		return 0;
	}
	CString csFilePath = xml.GetChildData();

	if (!PBSD_LocalPlayback(m_hDlg, nVideoType, csFilePath))
	{
		return 0;
	}

	return 1;
}

/**   @fn          ParsePlaybackXML
 *    @brief   	   解析回放参数字符串.	
 *    @param[in]   strXML:回放参数字符串.
 *    @param[out]  dwWndNo:窗口号.
 *    @param[out]  dwRight:权限.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlaybackMainWnd::ParsePlaybackXML(CString strXML, int& nWndNo, DWORD& dwRight, PBSDLoginParam& rLoginInfo, PBSDPlayParam& rPlayParam)
{
	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//	<Param>
	//	<RecordType>0</RecordType>			
	//	<SearchType>0</SearchType>			
	//	<WndNo>0</WndNo>					
	//	<PlayType>0</PlayType>				
	//	<DvrType>0</DvrType>				
	//  <Right>0xFF</Right>	 
	//	<LoginInfo>
    //  <NetID>1</NetID>
	//	<IP>10.64.56.31</IP> 
	//	<Port>8000</Port>
	//	<DataPort>3720</DataPort> 
	//	<User>admin</User>
	//	<Password>12345</Password> 
	//	<ProxIP>10.64.56.31</ProxIP>
	//	<ProxPort>8000</ProxPort> 
    //  <VrmIP>10.64.56.31</VrmIP>
    //  <VrmPort>6300</VrmPort>
    //  <VodPort>6304</VodPort>
    //  <VagIP>10.64.56.31</VagIP>
    //  <VagPort>7302</VagPort>
    //  <CamIndex>10000001</CamIndex>
    //  <VagDefaultIP></VagDefaultIP>
    //  <Rev>1000001</Rev>
	//	</LoginInfo>
	//	<PlayInfo>
	//	// 二选一.
	//	// 按时间.
	//	<CamID>0</CamID>					
	//  <CamName>9612</CamName> 
	//	<Channel>0</Channel>			
	//	<StartTime>2012-12-12 12:12:12</StartTime>		
	//	<StopTime>2012-12-12 12:12:12</StopTime>		
	//	<ByCard>0</ByCard> 	
	//	<CardNum>100000001</CardNum>  

	//	// 按文件.
	//	<CamID>0</CamID>					
	//  <CamName>9612</CamName>
	//	<Channel>0</Channel>			
	//	<FileName>123</FileName>		
	//	<FileSize>11111</FileSize>		
	//	<FileType>0</FileType>			
	//	<StartTime>2012-12-12 12:12:12</StartTime>		
	//	<StopTime>2012-12-12 12:12:12</StopTime>		
	//	<PlayType>0</PlayType>				
	//	<DvrType>0</DvrType>				
	//	</PlayInfo>
	//</Param>
	//OutputDebugString(strXML);

	USES_CONVERSION;

	CMarkup xml;
	if (!xml.SetDoc(strXML))
	{
		OutputDebugString(_T("@z ParsePlaybackXML setdoc error"));
		return FALSE;
	}

	if (!xml.FindElem(XML_PARAM))
	{
        OutputDebugString(_T("@z xml.FindElem(XML_PARAM) error"));
		return FALSE;
	}

	// 录像类型.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_RECORDTYPE))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_RECORDTYPE) error"));
		return FALSE;
	}
	int nFileType = _wtoi(xml.GetChildData());

	// 回放类型.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_SEARCHTYPE))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_SEARCHTYPE) error"));
		return FALSE;
	}
	rPlayParam.nType = _wtoi(xml.GetChildData());

	// 窗口号.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_WNDNO))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_WNDNO) error"));
		return FALSE;
	}
	nWndNo = _wtoi(xml.GetChildData());

	// 播放类型.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_PLAYTYPE))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_PLAYTYPE) error"));
		return FALSE;
	}
	rLoginInfo.emPlayType = (PBSDPlayType)_wtoi(xml.GetChildData());

	// 厂商类型.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_MANTYPE))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_MANTYPE) error"));
		return FALSE;
	}
	rLoginInfo.nManType = _wtoi(xml.GetChildData());

	// 设备类型.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_DVRTYPE))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_DVRTYPE) error"));
		return FALSE;
	}
	rLoginInfo.nDevType = _wtoi(xml.GetChildData());

	// 权限信息.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_RIGHT))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_RIGHT) error"));
		return FALSE;
	}
	dwRight = _wtoi(xml.GetChildData());

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>登录信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// 登录信息.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_LOGININFO))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_LOGININFO) error"));
		return FALSE;
	}
	xml.IntoElem();

	CString strTemp;
	// 直连DVR/CVR/云存储
	if (PBSD_TYPE_DVR == rLoginInfo.emPlayType    
		|| PBSD_TYPE_CVR == rLoginInfo.emPlayType
		|| PBSD_TYPE_CLOUD == rLoginInfo.emPlayType
		|| PBSD_TYPE_KMS == rLoginInfo.emPlayType)
	{
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_IP))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_IP) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
// 		if (strTemp == _T("null"))  //简单检查下IP地址是否为空
// 		{
// 			return FALSE;
// 		}
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szIP, 16);

        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_PORT))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_PORT) error"));
			return FALSE;
		}
		rLoginInfo.unLoginType.stDvrLoginInfo.nPort = _wtoi(xml.GetChildData());

        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_USER))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_USER) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szUserName, 32);

        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_PASSWORD))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_PASSWORD) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szPassword, 32);
		const char* pDevPWDIn = T2A(strTemp); 
		int nLenIn = strlen(pDevPWDIn);
		unsigned char devPWDOut[512] = {0};
		int nLenOut = sizeof(devPWDOut);
		CString strErr;
		//base64解密
		if (CBase64::Decrypt((const unsigned char*)pDevPWDIn, nLenIn, devPWDOut, &nLenOut, strErr))
		{
			memcpy(rLoginInfo.unLoginType.stDvrLoginInfo.szPassword, devPWDOut, 32);
			//rLoginInfo.unLoginType.stDvrLoginInfo.szPassword = devPWDOut;
		}

		if (PBSD_TYPE_CLOUD == rLoginInfo.emPlayType)
		{
			xml.ResetChildPos();
			if (!xml.FindChildElem(XML_INDEX))
			{
				OutputDebugString(_T("@z xml.FindChildElem(XML_INDEX) error"));
				return FALSE;
			}
			strTemp = xml.GetChildData();
			WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szIndex, 32);
		}

        xml.ResetChildPos();
		if (xml.FindChildElem(XML_REV))
		{
			strTemp = xml.GetChildData();
			if (strTemp.IsEmpty())
			{
				return FALSE;
			}
			WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szRev, 160);
		}
	}
	else if (PBSD_TYPE_OBJECTCLOUD == rLoginInfo.emPlayType)
	{
		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_IP))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_IP) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		// 		if (strTemp == _T("null"))  //简单检查下IP地址是否为空
		// 		{
		// 			return FALSE;
		// 		}
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szIP, 16);

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_PORT))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_PORT) error"));
			return FALSE;
		}
		rLoginInfo.unLoginType.stDvrLoginInfo.nPort = _wtoi(xml.GetChildData());

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_ACCESSKEY))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_ACCESSKEY) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szAccessKey, 255);
        //

        const char* pAccessPWDIn = T2A(strTemp); 
        int nAccessPWDLenIn = strlen(pAccessPWDIn);
        unsigned char AccessPWDOut[512] = {0};
        int nAccessPWDLenOut = sizeof(AccessPWDOut);
        CString strAccessPWDErr;
        //base64解密
        if (CBase64::Decrypt((const unsigned char*)pAccessPWDIn, nAccessPWDLenIn, AccessPWDOut, &nAccessPWDLenOut, strAccessPWDErr))
        {
            memcpy(rLoginInfo.unLoginType.stDvrLoginInfo.szAccessKey, AccessPWDOut, 255);
            //rLoginInfo.unLoginType.stDvrLoginInfo.szPassword = devPWDOut;
        }

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_SECRETKEY))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_SECRETKEY) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szSecretKey, 255);
        //
       
		const char* pSecretPWDIn = T2A(strTemp); 
		int nSecretPWDLenIn = strlen(pSecretPWDIn);
		unsigned char SecretPWDOut[512] = {0};
		int nSecretPWDLenOut = sizeof(SecretPWDOut);
		CString strSecretPWDErr;
		//base64解密
		if (CBase64::Decrypt((const unsigned char*)pSecretPWDIn, nSecretPWDLenIn, SecretPWDOut, &nSecretPWDLenOut, strSecretPWDErr))
		{
			memcpy(rLoginInfo.unLoginType.stDvrLoginInfo.szSecretKey, SecretPWDOut, 255);
			//rLoginInfo.unLoginType.stDvrLoginInfo.szPassword = devPWDOut;
		}

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_BUCKET))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_BUCKET) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szBucket, 255);

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_OBJECTKEY))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_OBJECTKEY) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szObjectKey, 255);

		xml.ResetChildPos();
		if (xml.FindChildElem(XML_REV))
		{
			strTemp = xml.GetChildData();
			if (strTemp.IsEmpty())
			{
				return FALSE;
			}
			WCharToMByte(strTemp, rLoginInfo.unLoginType.stDvrLoginInfo.szRev, 160);
		}
	}
	xml.OutOfElem();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<登录信息<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>回放信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// 回放信息.
    xml.ResetChildPos();
	if (!xml.FindChildElem(XML_PLAYINFO))
	{
        OutputDebugString(_T("@z xml.FindChildElem(XML_PLAYINFO) error"));
		return FALSE;
	}
	xml.IntoElem();

	// 按时间回放.
	if (0 == rPlayParam.nType)
	{
		// 文件类型.
		rPlayParam.unPlayParam.stTimePram.emFileType = (PBSDRecordType)nFileType;
		// 监控点ID.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CAMID))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CAMID) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stTimePram.nCamID = _wtoi(xml.GetChildData());

		// 监控点名称.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CAMNAME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CAMNAME) error"));
			return FALSE;
		}
		CString strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rPlayParam.unPlayParam.stTimePram.szCamName, 128);

		if (PBSD_TYPE_CVR == rLoginInfo.emPlayType)
		{
			xml.ResetChildPos();
			if (!xml.FindChildElem(XML_STREAMID))
			{
				OutputDebugString(_T("@z xml.FindChildElem(XML_STREAMID) error"));
				return FALSE;
			}
			strTemp = xml.GetChildData();
			WCharToMByte(strTemp, rPlayParam.unPlayParam.stTimePram.szStreamID, 32);
		}

		// 通道号.如果为CVR回放，通道号为对应的CVR通道编号
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CHANNEL))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CHANNEL) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stTimePram.nChannel = _wtoi(xml.GetChildData());

		// 开始时间.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_STARTTIME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_STARTTIME) error"));
			return FALSE;
		}
		COleDateTime oleTime;

		if ((xml.GetChildData()).GetLength() == 1)
		{
			return FALSE;
		}
		oleTime.ParseDateTime(xml.GetChildData());
		rPlayParam.unPlayParam.stTimePram.startTime.dwYear = oleTime.GetYear();
		rPlayParam.unPlayParam.stTimePram.startTime.dwMonth = oleTime.GetMonth();
		rPlayParam.unPlayParam.stTimePram.startTime.dwDay = oleTime.GetDay();
		rPlayParam.unPlayParam.stTimePram.startTime.dwHour = oleTime.GetHour();
		rPlayParam.unPlayParam.stTimePram.startTime.dwMinute = oleTime.GetMinute();
		rPlayParam.unPlayParam.stTimePram.startTime.dwSecond = oleTime.GetSecond();

		// 结束时间.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_STOPTIME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_STOPTIME) error"));
			return FALSE;
		}

		if ((xml.GetChildData()).GetLength() == 1)
		{
			return FALSE;
		}

		oleTime.ParseDateTime(xml.GetChildData());
		rPlayParam.unPlayParam.stTimePram.stopTime.dwYear = oleTime.GetYear();
		rPlayParam.unPlayParam.stTimePram.stopTime.dwMonth = oleTime.GetMonth();
		rPlayParam.unPlayParam.stTimePram.stopTime.dwDay = oleTime.GetDay();
		rPlayParam.unPlayParam.stTimePram.stopTime.dwHour = oleTime.GetHour();
		rPlayParam.unPlayParam.stTimePram.stopTime.dwMinute = oleTime.GetMinute();
		rPlayParam.unPlayParam.stTimePram.stopTime.dwSecond = oleTime.GetSecond();

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_REV))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_REV) error"));
			return FALSE;
		}
		strTemp, rPlayParam.unPlayParam.stTimePram.dwRev = _wtoi(xml.GetChildData());
	}
	// 按文件回放.
	else if (1 == rPlayParam.nType)
	{
		// 监控点ID.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CAMID))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CAMID) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stFileParam.nCamID = _wtoi(xml.GetChildData());

		// 监控点名称.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CAMNAME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CAMNAME) error"));
			return FALSE;
		}
		CString strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rPlayParam.unPlayParam.stFileParam.szCamName, 128);

		if (PBSD_TYPE_CVR == rLoginInfo.emPlayType)
		{
			xml.ResetChildPos();
			if (!xml.FindChildElem(XML_STREAMID))
			{
				OutputDebugString(_T("@z xml.FindChildElem(XML_STREAMID) error"));
				return FALSE;
			}
			strTemp = xml.GetChildData();
			WCharToMByte(strTemp, rPlayParam.unPlayParam.stTimePram.szStreamID, 32);
		}

		// 通道号.CVR复用该字段
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_CHANNEL))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_CHANNEL) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stFileParam.nChannel = _wtoi(xml.GetChildData());

		// 文件名.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_FILENAME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_FILENAME) error"));
			return FALSE;
		}
		strTemp = xml.GetChildData();
		WCharToMByte(strTemp, rPlayParam.unPlayParam.stFileParam.stFileData.szFileName, 128);

		// 文件大小.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_FILESIZE))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_FILESIZE) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stFileParam.stFileData.dwFileSize = _wtoi(xml.GetChildData());

		// 文件类型.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_FILETYPE))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_FILETYPE) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stFileParam.stFileData.nFileType = _wtoi(xml.GetChildData());

		// 开始时间.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_STARTTIME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_STARTTIME) error"));
			return FALSE;
		}
		CString strDataTime;
		if ((xml.GetChildData()).GetLength() == 1)
		{
			return FALSE;
		}
		COleDateTime oleTime;
		oleTime.ParseDateTime(xml.GetChildData());
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwYear = oleTime.GetYear();
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwMonth = oleTime.GetMonth();
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwDay = oleTime.GetDay();
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwHour = oleTime.GetHour();
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwMinute = oleTime.GetMinute();
		rPlayParam.unPlayParam.stFileParam.stFileData.stStartTime.dwSecond = oleTime.GetSecond();

		// 结束时间.
        xml.ResetChildPos();
		if (!xml.FindChildElem(XML_STOPTIME))
		{
            OutputDebugString(_T("@z xml.FindChildElem(XML_STOPTIME) error"));
			return FALSE;
		}

		if ((xml.GetChildData()).GetLength() == 1)
		{
			return FALSE;
		}

		oleTime.ParseDateTime(xml.GetChildData());
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwYear = oleTime.GetYear();
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwMonth = oleTime.GetMonth();
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwDay = oleTime.GetDay();
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwHour = oleTime.GetHour();
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwMinute = oleTime.GetMinute();
		rPlayParam.unPlayParam.stFileParam.stFileData.stEndTime.dwSecond = oleTime.GetSecond();
		rPlayParam.unPlayParam.stFileParam.stFileData.emPlayType = rLoginInfo.emPlayType;
		rPlayParam.unPlayParam.stFileParam.stFileData.nDevType = rLoginInfo.nDevType;

		xml.ResetChildPos();
		if (!xml.FindChildElem(XML_REV))
		{
			OutputDebugString(_T("@z xml.FindChildElem(XML_REV) error"));
			return FALSE;
		}
		rPlayParam.unPlayParam.stFileParam.dwRev = _wtol(xml.GetChildData());
	}
	xml.ResetChildPos();
	if (!xml.FindChildElem(XML_LBWCONTROL))
	{
		OutputDebugString(_T("@z xml.FindChildElem(XML_LBWCONTROL) error"));
		return FALSE;
	}
	rPlayParam.lBwControl = _wtol(xml.GetChildData());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<回放信息<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	return TRUE;
}

/**   @fn          StopPlayback
 *    @brief   	   停止回放.	
 *    @param[in]   lWndNo:窗口号.
 *    @param[in]   
 *    @return      0-成功,非0-失败.
 */
LONG CPlaybackMainWnd::StopPlayback(LONG lWndNo)
{
	if (!PBSD_StopPlayByWndNo(m_hDlg, (int)lWndNo))
	{
		return 0;
	}

	return 1;
}

/**   @fn          StopAll
 *    @brief   	   停止全部.	
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LONG CPlaybackMainWnd::StopAll()
{
	PBSD_StopAll(m_hDlg);

	return 0;
}

/**   @fn          SetParameter
 *    @brief   	   设置参数.
 *    @param[in]   lpstrXML:参数报文.
 *    @param[in]   
 *    @return      
 */
LONG CPlaybackMainWnd::SetParameter(LPCTSTR lpstrXML)
{
	PLAYBACK_PARAM stParam;
	CString strPicPath;
	CString strDownloadPath;
	
	if (!ParseParamXML(lpstrXML, strPicPath, strDownloadPath, stParam.nPicType, stParam.nShowWndTitle))
	{
		return 0;
	}

	WCharToMByte(strPicPath, stParam.szPicPath, MAX_PATH);
	WCharToMByte(strDownloadPath, stParam.szDownloadPath, MAX_PATH);
	if (!PBSD_SetParameter(stParam))
	{
		return 0;
	}

	return 1;
}

/**   @fn          ParseParamXML
 *    @brief   	   解析参数配置XML.	
 *    @param[in]   lpstrXML:文件报文.
 *    @param[in]   
 *    @return      
 */
BOOL CPlaybackMainWnd::ParseParamXML(LPCTSTR lpstrXML, CString& strPicPath, CString& strDownloadPath, int& nPicType, int& nShowTitile)
{
	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//	<Param>
	//	<PicPath>c:\lala\</PicPath>			// 图片保存路径.
	//	<DownloadPath>c:\lala\</DownloadPath>	// 下载路径.
	//	<PicType>0</PicType>		// 图片类型. 0-BMP,1-JPG.
	//	<ShowTitle>1</ShowTitle>	// 显示标题栏.0-不显示,1-显示.
	//</Param>


	CMarkup xml;
	if (!xml.SetDoc(lpstrXML))
	{
		return FALSE;
	}

	if (!xml.FindElem(XML_PARAM))
	{
		return FALSE;
	}
	xml.IntoElem();

	if (!xml.FindElem(XML_PIC_PATH))
	{
		return FALSE;
	}
	strPicPath = xml.GetData();

	if (!xml.FindElem(XML_DOWNLOAD_PATH))
	{
		return FALSE;
	}
	strDownloadPath = xml.GetData();

	if (!xml.FindElem(XML_PIC_TYPE))
	{
		return FALSE;
	}
	nPicType = _wtoi(xml.GetData());

	if (!xml.FindElem(XML_SHOWTITLE))
	{
		return FALSE;
	}
	nShowTitile = _wtoi(xml.GetData());

	return TRUE;
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   获取回放状态信息.
 *    @param[in]   lWndNo:窗口号.
 *    @param[in]   
 *    @return      状态信息报文.
 */
CString CPlaybackMainWnd::GetPlaybackInfo(LONG lWndNo)
{
	PlaybackStatData stData;
	if (!PBSD_GetPlaybackInfo(m_hDlg, lWndNo, stData))
	{
		return _T("");
	}

	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//<Param>
	//	<IsPlaying>0</IsPlaying>				// 是否正在回放中,0-否,1-是.
	//	<IsSaving>0</IsSaving>					// 是否正在剪辑中,0-否,1-是.
	//	<IsDownloading>0</IsDownloading>		// 是否正在下载中,0-否,1-是.
	//	<CamID>123</CamID>						// 监控点ID.
	//	<Rev>999<//Rev>							// 保留字段.
	//</Param>

	CString strTemp;
	CMarkup xml;
	xml.SetDoc(XML_HEADER_CHN);
	xml.AddElem(XML_PARAM);
	xml.IntoElem();

	// 是否正在回放中.
	xml.AddElem(XML_ISPLAYING, stData.bIsPlaying);
	// 是否正在剪辑中.
	xml.AddElem(XML_ISSAVING, stData.bIsSaving);
	// 是否正在下载中.
	xml.AddElem(XML_ISDOWNLOADING, stData.bIsDownloading);
	// 监控点ID.
	xml.AddElem(XML_CAMID, stData.nCamID);
	// 保留字段.
	strTemp.Format(_T("%ld"), stData.dwRev);
	xml.AddElem(XML_REV, strTemp);

	strTemp = xml.GetDoc();
	return strTemp;
}