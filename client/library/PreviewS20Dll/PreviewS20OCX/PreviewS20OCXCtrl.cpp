// PreviewS20OCXCtrl.cpp : CPreviewS20OCXCtrl ActiveX 控件类的实现。
//lint -library
//lint -e628
#include "stdafx.h"
#include "PreviewS20OCX.h"
#include "PreviewS20OCXCtrl.h"
#include "PreviewS20OCXPropPage.h"
#include "base64.h"

//OCX安全下载
#include "comcat.h" 
#include "objsafe.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_FINGERPRINT_FIREDATA (WM_USER + 0x1001) // 发送指纹信息消息
#define USB_COM 1001
#define DEVICE_ERROR_CODE_OK 1

// 全局指针.
CPreviewS20OCXCtrl * g_pCtrl = NULL;
////////////////////////////////OCX 安全下载 begin//////////////////////////////////////////
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription) 
{ 

	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (FAILED(hr)) 
	{
		return hr; 
	}
	CATEGORYINFO catinfo; 
	catinfo.catid = catid; 
	catinfo.lcid = 0x0409 ; // english 
	int len = wcslen(catDescription); 
	if (len>127) 
		len = 127; 
	wcsncpy(catinfo.szDescription, catDescription, len); 
	// Make sure the description is null terminated 
	catinfo.szDescription[len] = '\0'; 
	hr = pcr->RegisterCategories(1, &catinfo); 
	pcr->Release(); 
	return hr; 
} 

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid) 
{ 
	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (SUCCEEDED(hr)) 
	{ 
		CATID rgcatid[1] ; 
		rgcatid[0] = catid; 
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid); 
	} 
	if (pcr != NULL) 
		pcr->Release(); 
	return hr; 
} 

HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid) 
{ 
	ICatRegister* pcr = NULL ; 
	HRESULT hr = S_OK ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (SUCCEEDED(hr)) 
	{ 
		// Unregister this category as being "implemented" by 
		// the class. 
		CATID rgcatid[1] ; 
		rgcatid[0] = catid; 
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid); 
	} 
	if (pcr != NULL) 
	{
		pcr->Release(); 
	}
	return hr; 
}
////////////////////////////////OCX 安全下载 end//////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPreviewS20OCXCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CPreviewS20OCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_FINGERPRINT_FIREDATA, OnFireFingerPrint)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CPreviewS20OCXCtrl, COleControl)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StartPreview", dispidStartPreview, StartPreview, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StopPreview", dispidStopPreview, StopPreview, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StartRecord", dispidStartRecord, StartRecord, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StopRecord", dispidStopRecord, StopRecord, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "GetAllUsbCameraInfo", dispidGetAllUsbCameraInfo, GetAllUSBCameraInfo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "SetShowString", dispidSetShowString, SetShowString, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "SetConfigData", dispidSetConfigData, SetConfigData, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "SetTrustedSite", dispidSetTrustedSite, SetTrustedSite, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "RecordFile", dispidRecordFile, RecordFile, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "FireIDCardInfo", dispidFireIDCardInfo, FireIDCardInfo, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CPreviewS20OCXCtrl, "lLayoutMode", dispidlLayoutMode, m_lPreviewMode, OnlLayoutModeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPreviewS20OCXCtrl, "lSkinType", dispidlSkinType, m_lSkinType, OnlSkinTypeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPreviewS20OCXCtrl, "lBtnRights", dispidlBtnRights, m_lBtnRights, OnlBtnRightsChanged, VT_I4)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CPreviewS20OCXCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CPreviewS20OCXCtrl, 1)
	PROPPAGEID(CPreviewS20OCXPropPage::guid)
END_PROPPAGEIDS(CPreviewS20OCXCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CPreviewS20OCXCtrl, "PREVIEWS20OCX.PreviewS20OCXCtrl.1",
	0x4655ff8, 0x9979, 0x4ffa, 0x8f, 0x77, 0xc3, 0xa5, 0x39, 0xea, 0x73, 0x70)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CPreviewS20OCXCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_DPreviewS20OCX =
		{ 0xA25F1DD7, 0xD47C, 0x466C, { 0xA0, 0x75, 0x5E, 0xE2, 0xCC, 0xC5, 0x34, 0xAE } };
const IID BASED_CODE IID_DPreviewS20OCXEvents =
		{ 0x46292E59, 0x6C27, 0x4A45, { 0x9B, 0x1A, 0x2E, 0x96, 0xB8, 0xE1, 0x6, 0x59 } };



// 控件类型信息

static const DWORD BASED_CODE _dwPreviewS20OCXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPreviewS20OCXCtrl, IDS_PREVIEWS20OCX, _dwPreviewS20OCXOleMisc)



// CPreviewS20OCXCtrl::CPreviewS20OCXCtrlFactory::UpdateRegistry -
// 添加或移除 CPreviewS20OCXCtrl 的系统注册表项

BOOL CPreviewS20OCXCtrl::CPreviewS20OCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。
	if (bRegister) 
	{ 
		HRESULT hr = S_OK ; 
		// register as safe for scripting 
		hr = CreateComponentCategory(CATID_SafeForScripting, 
			L"Controls that are safely scriptable"); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		// register as safe for initializing 
		hr = CreateComponentCategory(CATID_SafeForInitializing, 
			L"Controls safely initializable from persistent data"); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		hr = RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		return AfxOleRegisterControlClass( 
			AfxGetInstanceHandle(), 
			m_clsid, 
			m_lpszProgID, 
			IDS_PREVIEWS20OCX,
			IDB_PREVIEWS20OCX,
			afxRegInsertable | afxRegApartmentThreading, 
			_dwPreviewS20OCXOleMisc, 
			_tlid, 
			_wVerMajor, 
			_wVerMinor); 
	} 
	else 
	{ 
		HRESULT hr = S_OK ; 
		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		hr = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing); 
		if (FAILED(hr)) 
		{
			return FALSE; 
		}
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID); 
	}
}

//读卡线程
DWORD WINAPI _ReadCardThreadFun(void* pParam)
{
	CPreviewS20OCXCtrl* pInstance = (CPreviewS20OCXCtrl*) pParam;
	if (pInstance != NULL)
	{
		pInstance->ReadCard();
	}
	return 0;
}

// CPreviewS20OCXCtrl::CPreviewS20OCXCtrl - 构造函数

CPreviewS20OCXCtrl::CPreviewS20OCXCtrl()
	: m_bManualStop(FALSE)
{
	InitializeIIDs(&IID_DPreviewS20OCX, &IID_DPreviewS20OCXEvents);
	// TODO: 在此初始化控件的实例数据。
	m_pPreviewDlg = NULL;
	m_lPreviewMode = 0;
	m_lSkinType = 0;
	m_lBtnRights = 0;
	m_hReadCardThread = NULL;
	m_bInitSucc = FALSE;
    m_bStopThread = false;
    m_strIdCardInfo = _T("");
}



// CPreviewS20OCXCtrl::~CPreviewS20OCXCtrl - 析构函数

CPreviewS20OCXCtrl::~CPreviewS20OCXCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CPreviewS20OCXCtrl::OnDraw - 绘图函数

void CPreviewS20OCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
}



// CPreviewS20OCXCtrl::DoPropExchange - 持久性支持

void CPreviewS20OCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
	PX_Long(pPX, _T("lLayoutMode"), m_lPreviewMode);
	PX_Long(pPX, _T("lSkinType"), m_lSkinType);
	PX_Long(pPX, _T("lBtnRights"), m_lBtnRights);
}



// CPreviewS20OCXCtrl::OnResetState - 将控件重置为默认状态

void CPreviewS20OCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CPreviewS20OCXCtrl::AboutBox - 向用户显示“关于”框

void CPreviewS20OCXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_PREVIEWS20OCX);
	dlgAbout.DoModal();
}



// CPreviewS20OCXCtrl 消息处理程序
int CPreviewS20OCXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	int nStart = 0;
	int nStop = 0;
	nStart = GetTickCount();

	// TODO:  在此添加您专用的创建代码
	g_pCtrl = this;

	if (m_pPreviewDlg == NULL)
	{
		PREV_INIT stPrevInit;
		stPrevInit.lPreviewMode = m_lPreviewMode;
		stPrevInit.lSkinType = m_lSkinType;
		stPrevInit.lBtnRights = m_lBtnRights;
		m_pPreviewDlg = new CPreviewDlg(stPrevInit, this);
		m_pPreviewDlg->Create(CPreviewDlg::IDD, this);
		m_pPreviewDlg->ShowWindow(SW_SHOW);
	}

	nStop = GetTickCount();
	CString strTime;
	strTime.Format(_T("CPreviewOCX30Ctrl::OnCreate Time=%d"), nStop-nStart);
	OutputDebugString(strTime);

	InitializeReader(_T(""));

	return 0;
}

void CPreviewS20OCXCtrl::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码
	int nStart = 0;
	int nStop = 0;
	nStart = GetTickCount();

	if (m_pPreviewDlg != NULL)
	{
		m_pPreviewDlg->DestroyWindow();
		delete m_pPreviewDlg;
		m_pPreviewDlg = NULL;
	}

    UnInitializeReader();
	COleControl::OnDestroy();

	nStop = GetTickCount();
	CString strTime;
	strTime.Format(_T("CPreviewOCX30Ctrl::OnDestroy Time=%d"), nStop-nStart);
	OutputDebugString(strTime);
}

void CPreviewS20OCXCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	if (m_pPreviewDlg != NULL)
	{
		m_pPreviewDlg->MoveWindow(rcClient);
	}
}

//开始预览
LONG CPreviewS20OCXCtrl::StartPreview(LPCTSTR strXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::StartPreview xml=%s"), strXML);
	OutputDebugString(strMsg);

	if (m_pPreviewDlg != NULL)
	{
		PREVIEW_CAMINFO struPrevCamInfo;
		if (!ParseXml4Preview(struPrevCamInfo, strXML))
		{
			return 0;
		}
		if (!m_pPreviewDlg->StartPreview(struPrevCamInfo))
		{
			return 0;
		}
		return 1;
	}
	strMsg.Format(_T("CPreviewS20OCXCtrl::StartPreview falid!"));
	OutputDebugString(strMsg);
	return -1;
}

//停止预览
LONG CPreviewS20OCXCtrl::StopPreview(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		if (!m_pPreviewDlg->StopPreview())
		{
			return 0;
		}
		return 1;
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::StopPreview falid!"));
	OutputDebugString(strMsg);
	return -1;
}

//开始录像
LONG CPreviewS20OCXCtrl::StartRecord(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		if (!m_pPreviewDlg->StartRecord(lWndNum))
		{
			return 0;
		}
		return 1;
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::StartRecord falid!"));
	OutputDebugString(strMsg);
	return -1;
}

//停止录像
BSTR CPreviewS20OCXCtrl::StopRecord(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strXml;
	if (m_pPreviewDlg != NULL)
	{
		// 设备为手动停止录像，去除不必要的事件提醒
		m_bManualStop = FALSE;
		RECOED_FILE_INFO recoedFile;
		if (m_pPreviewDlg->StopRecord(lWndNum, recoedFile))
		{
			//SerialRecordFile4Xml(recoedFile, strXml);
			return strXml.AllocSysString();
		}
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::StopRecord falid!"));
	OutputDebugString(strMsg);
	return strXml.AllocSysString();
}

BOOL CPreviewS20OCXCtrl::ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml)
{
	USES_CONVERSION;
	CMarkup xml;
	if (!xml.SetDoc(strXml))
	{
		return FALSE;
	}

	if (!xml.FindElem(PV_XML_PREVIEW))
	{
		return FALSE;
	}
	xml.IntoElem();

	if (xml.FindElem(PV_XML_CAMTYPE))
	{
		int nCamType = _wtoi(xml.GetData());
		switch(nCamType)
		{
		case PV_CAMTPYE_DVR:
		case PV_CAMTPYE_NVR:
			{
				struPrevCamInfo.nCamType = nCamType;
				if (xml.FindElem(PV_XML_CAMID))
				{
					struPrevCamInfo.nCamID = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_CAMNAME))
				{
					struPrevCamInfo.strCamName = xml.GetData();
				}

				if (xml.FindElem(PV_XML_DEVTYPE))
				{
					struPrevCamInfo.nDevType = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_REGIONID))
				{
					struPrevCamInfo.nRegionID = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_DEVIP))
				{
					struPrevCamInfo.strDevIP = xml.GetData();
				}

				if (xml.FindElem(PV_XML_DEVPORT))
				{
					struPrevCamInfo.nDevPort = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_USER))
				{
					struPrevCamInfo.strUser = xml.GetData();
				}

				if (xml.FindElem(PV_XML_PWD))
				{
					//struPrevCamInfo.strPwd = xml.GetData();
					const char* pDevPWDIn = T2A(xml.GetData().GetBuffer()); 
					int nLenIn = strlen(pDevPWDIn);
					unsigned char devPWDOut[512] = {0};
					int nLenOut = sizeof(devPWDOut);
					CString strErr;
					//base64解密
					if (CBase64::Decrypt((const unsigned char*)pDevPWDIn, nLenIn, devPWDOut, &nLenOut, strErr))
					{
						struPrevCamInfo.strPwd = devPWDOut;
					}
				}

				if (xml.FindElem(PV_XML_CHANNEL))
				{
					struPrevCamInfo.nChannel = _wtoi(xml.GetData());
					if (struPrevCamInfo.nChannel < 0)
					{
						return FALSE;
					}
				}

				if (xml.FindElem(PV_XML_REALPLAYTYPE))
				{
					struPrevCamInfo.nRealPlayType = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_VIDEOFORMAT))
				{
					struPrevCamInfo.nVideoFormat = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_CAMRIGHT))
				{
					struPrevCamInfo.nCamRight = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_PROTOCOL))
				{
					struPrevCamInfo.nProtocol = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_USERID))
				{
					struPrevCamInfo.nUserID = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_COLLECTORIP))
				{
					struPrevCamInfo.strCollectorIP = xml.GetData();
				}

				if (xml.FindElem(PV_XML_PHONEPORT))
				{
					struPrevCamInfo.nPhonePort = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_PCHANNEL))
				{
					struPrevCamInfo.nPChannel = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_TIMEOUT))
				{
					struPrevCamInfo.nTimeout = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_DELAYTIME))
				{
					struPrevCamInfo.nDelayTime = _wtoi(xml.GetData());
				}
			}
			break;
		case PV_CAMTPYE_USB:
			{
				struPrevCamInfo.nCamType = nCamType;
				if (xml.FindElem(PV_XML_CAMINDEX))
				{
					struPrevCamInfo.lCamIndex = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_CAMNAME))
				{
					struPrevCamInfo.strCamName = xml.GetData();
				}

				if (xml.FindElem(PV_XML_MICINDEX))
				{
					struPrevCamInfo.lMicIndex = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_MICNAME))
				{
					struPrevCamInfo.strMicName = xml.GetData();
				}
				if (xml.FindElem(PV_XML_TIMEOUT))
				{
					struPrevCamInfo.nTimeout = _wtoi(xml.GetData());
				}

				if (xml.FindElem(PV_XML_DELAYTIME))
				{
					struPrevCamInfo.nDelayTime = _wtoi(xml.GetData());
				}
			}
			break;
		default: 
			break;
		}
	}
	return TRUE;
}

void CPreviewS20OCXCtrl::SerialRecordFile4Xml(const RECOED_FILE_INFO& recoedFile, CString& strXml)
{
	/*
		<?xml version=\"1.0\" encoding=\"UTF-8\"?>
		<Param>
		<DevIP></DevIP>
		<DevPort></DevPort>
		<User></User>
		<Pwd></Pwd>
		<Channel>0</Channel>			
		<FileName>123</FileName>		
		<FileSize>11111</FileSize>		
		<FileType>0</FileType>			
		<StartTime>2012-12-12 12:12:12</StartTime>		
		<StopTime>2012-12-12 12:12:12</StopTime>		
		<RecordStatus>1</RecordStatus>		
		</Param>
	*/
	strXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	
	CString strParamBegin = _T("<Param>");
	strXml += strParamBegin;

	CString strIP;
	strIP.Format(_T("<DevIP>%s</DevIP>"), recoedFile.strDevIP);
	strXml += strIP;
	
	CString strPort;
	strPort.Format(_T("<DevPort>%d</DevPort>"), recoedFile.nDevPort);
	strXml += strPort;

	CString strUser;
	strUser.Format(_T("<User>%s</User>"), recoedFile.strUser);
	strXml += strUser;
	
	CString strPwd;
	strPwd.Format(_T("<Pwd>%s</Pwd>"), recoedFile.strPwd);
	strXml += strPwd;

	CString strChannel;
	strChannel.Format(_T("<Channel>%d</Channel>"), recoedFile.nChannel);
	strXml += strChannel;

	CString strFileName;
	strFileName.Format(_T("<FileName>%s</FileName>"), recoedFile.strFileName);
	strXml += strFileName;

	CString strFileSize;
	strFileSize.Format(_T("<FileSize>%ld</FileSize>"), recoedFile.lFileSize);
	strXml += strFileSize;

	CString strFileType;
	strFileType.Format(_T("<FileType>%ld</FileType>"), recoedFile.lFileType);
	strXml += strFileType;

	CString strStartTime;
	strStartTime.Format(_T("<StartTime>%ld</StartTime>"), recoedFile.lStartTime);
	strXml += strStartTime;

	CString strStopTime;
	strStopTime.Format(_T("<StopTime>%ld</StopTime>"), recoedFile.lEndTime);
	strXml += strStopTime;

	CString strRecordStatus;
	strRecordStatus.Format(_T("<RecordStatus>%ld</RecordStatus>"), recoedFile.nRecordStatus);
	strXml += strRecordStatus;

	CString strPicPath;
	strPicPath.Format(_T("<PicPath>%s</PicPath>"), recoedFile.csPicPath);
	strXml += strPicPath;

	CString strParamEnd = _T("</Param>");
	strXml += strParamEnd;
}

void CPreviewS20OCXCtrl::NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile)
{
	CString strXml;
	SerialRecordFile4Xml(recoedFile, strXml);
	RecordFile(strXml);
}

void CPreviewS20OCXCtrl::OnlLayoutModeChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}

void CPreviewS20OCXCtrl::OnlSkinTypeChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}

void CPreviewS20OCXCtrl::OnlBtnRightsChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}

//设置OSD信息
LONG CPreviewS20OCXCtrl::SetShowString(LPCTSTR strXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		if (m_pPreviewDlg->SetShowString(strXML))
		{
			return 1;
		}
		return 0;
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::SetShowString falid!"));
	OutputDebugString(strMsg);
	return -1;
}

//设置配置信息
LONG CPreviewS20OCXCtrl::SetConfigData( LPCTSTR strXML )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		if (m_pPreviewDlg->SetConfigData(strXML))
		{
			return 1;
		}
		return 0;
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::SetConfigData falid!"));
	OutputDebugString(strMsg);
	return -1;
}

//设置可信任站点
LONG CPreviewS20OCXCtrl::SetTrustedSite( LPCTSTR strXML )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		if (m_pPreviewDlg->SetTrustedSite(strXML))
		{
			return 1;
		}
		return 0;
	}
	CString strMsg;
	strMsg.Format(_T("CPreviewS20OCXCtrl::SetTrustedSite falid!"));
	OutputDebugString(strMsg);
	return -1;
}

/** @fn         GetAllUSBCameraInfo
*   @brief      获取所有USB监控点信息
*   @param  
*   @return     XML格式文件信息 
*/
BSTR CPreviewS20OCXCtrl::GetAllUSBCameraInfo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strCameraInfo;
	if (m_pPreviewDlg != NULL)
	{
		m_pPreviewDlg->GetAllUSBCameraInfo(strCameraInfo);
	}
	return strCameraInfo.AllocSysString();
}

/**	@fn	InitializeCardReader
*	@brief	初始化函数，连接发卡器，并开启读卡线程
*	@param  Input:	无
*           Output: 无
*           InOut:  无
*	@return	初始化结果，0 成功，-1 连接发卡器失败 -2 开启读卡线程失败
*/
LONG CPreviewS20OCXCtrl::InitializeReader(LPCTSTR lpFilePath)
{
	if (m_bInitSucc)
	{
		return 0;
	}

	int iRet = InitComm(USB_COM);
	if (DEVICE_ERROR_CODE_OK != iRet)
	{
		return -1;
	}

	if (!Start())
	{
		return -2;
	}

	m_strFilePath = lpFilePath;
	m_bInitSucc = TRUE;
	return 0;
}

/**	@fn	UnInitializeCardReader
*	@brief	反初始化函数，停止读卡线程并断开设备连接
*	@param  Input:	无
*           Output: 无
*           InOut:  无
*	@return	反初始化结果，0 成功
*/
LONG CPreviewS20OCXCtrl::UnInitializeReader()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Stop();

	m_bInitSucc = FALSE;

	return 0;
}

/**	@fn	FireCardSerialNo
*	@brief	抛送身份证信息
*	@param  Input:	lpName  姓名
*	@param  Input:	lpIDCardNo  身份证号
*	@param  Input:	lpBirthday  出生日期
*	@param  Input:	lpSex  性别
*	@param  Input:	lpNation  民族
*	@param  Input:	lpAddress  住址
*	@param  Input:	lpBeginDate  有效期开始日期
*	@param  Input:	lpEndDate  有效期结束日期
*	@param  Input:	lpGovernment  签发相关
*           Output: 无
*           InOut:  无
*	@return	
*/
void CPreviewS20OCXCtrl::FireIDCardInfo( LPCTSTR lpName, LPCTSTR lpIDCardNo, LPCTSTR lpBirthday, LPCTSTR lpSex, LPCTSTR lpNation, LPCTSTR lpAddress, LPCTSTR lpBeginDate, LPCTSTR lpEndDate, LPCTSTR lpGovernment )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	FireEvent(dispidFireIDCardInfo, EVENT_PARAM(VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR), lpName, lpIDCardNo, lpBirthday, lpSex, lpNation, lpAddress, lpBeginDate, lpEndDate, lpGovernment);
}

/**	@fn	Start
*	@brief	开启读卡线程
*	@param  Input:	无
*           Output: 无
*           InOut:  无
*	@return	
*/
bool CPreviewS20OCXCtrl::Start()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_bStopThread = false;
	if (m_hReadCardThread == NULL)
	{
		m_hReadCardThread = CreateThread(NULL, NULL, &_ReadCardThreadFun, this, NULL, NULL);
		if (m_hReadCardThread == NULL)
		{
			//关闭读卡器
			CloseComm();
			return false;
		}
	}

	return true;
}

/**	@fn	Stop
*	@brief	停止读卡线程，断开设备连接
*	@param  Input:	无
*           Output: 无
*           InOut:  无
*	@return	
*/
void CPreviewS20OCXCtrl::Stop()
{
	try
	{
		if (!m_bInitSucc)
		{
			return;
		}
		//关闭读卡线程
		m_bStopThread = true;
		if (m_hReadCardThread)
		{
			WaitForSingleObject(m_hReadCardThread, INFINITE);
			CloseHandle(m_hReadCardThread);
			m_hReadCardThread = NULL;
		}

		//关闭读卡器
		CloseComm();
	}
	catch (...)
	{

	}
}

/**	@fn	ReadCard
*	@brief  读卡执行函数
*	@param  Input:	无
*           Output: 无
*           InOut:  无
*	@return	
*/
void CPreviewS20OCXCtrl::ReadCard()
{
	unsigned long lCardNo = 0;
	while (!m_bStopThread)
	{
		// _Mode: IDLE = 0, ALL = 1，
		// 当为IDLE时，采用dc_halt接口使得读卡器不再接受同一卡号卡片（除非拿起后重新刷卡）
		if (DEVICE_ERROR_CODE_OK == Authenticate())
		{
			char Msg[200] = {0};
			int nLen = 0;
			USES_CONVERSION;
			int nEet= 0;
			if (m_strFilePath.IsEmpty())
			{
				nEet = ReadBaseMsg ((byte*)Msg, &nLen);
			}
			else
			{
				nEet = ReadBaseMsgPhoto ((byte*)Msg, &nLen, T2A(m_strFilePath));
			}

			if (DEVICE_ERROR_CODE_OK == nEet)
			{
				//输出信息包含身份证号
				if (nLen >= 192)
				{
					CString strTemp;

					//姓名
					char chName[32] = {0};
					memcpy_s(chName, 32, Msg, 31);
					strTemp.Format(L"%s", A2T(chName));
					m_strIdCardInfo += strTemp + L"|";
					//性别
					char chSex[4] = {0};
					memcpy_s(chSex, 4, Msg+31, 3);
					strTemp.Format(L"%s", A2T(chSex));
					m_strIdCardInfo += strTemp + L"|";
					//民族
					char chNation[11] = {0};
					memcpy_s(chNation, 11, Msg+31+3, 10);
					strTemp.Format(L"%s", A2T(chNation));
					m_strIdCardInfo += strTemp + L"|";
					//出生日期
					char chBirthday[10] = {0};
					memcpy_s(chBirthday, 10, Msg+31+3+10, 9);
					strTemp.Format(L"%s", A2T(chBirthday));
					m_strIdCardInfo += strTemp + L"|";
					//住址
					char chAddress[72] = {0};
					memcpy_s(chAddress, 72, Msg+31+3+10+9, 71);
					strTemp.Format(L"%s", A2T(chAddress));
					m_strIdCardInfo += strTemp + L"|";
					//身份证号
					char chCardNo[20] = {0};
					memcpy_s(chCardNo, 20, Msg+31+3+10+9+71, 19);
					strTemp.Format(L"%s", A2T(chCardNo));
					m_strIdCardInfo += strTemp + L"|";
					//签发机关
					char chGovernment[32] = {0};
					memcpy_s(chGovernment, 32, Msg+31+3+10+9+71+19, 31);
					strTemp.Format(L"%s", A2T(chGovernment));
					m_strIdCardInfo += strTemp + L"|";
					//有效期起始日期
					char chBeginData[10] = {0};
					memcpy_s(chBeginData, 10, Msg+31+3+10+9+71+19+31, 9);
					strTemp.Format(L"%s", A2T(chBeginData));
					m_strIdCardInfo += strTemp + L"|";
					//有效期结束日期
					char chEndData[10] = {0};
					memcpy_s(chEndData, 10, Msg+31+3+10+9+71+19+31+9, 9);
					strTemp.Format(L"%s", A2T(chEndData));
					m_strIdCardInfo += strTemp;

					// 指纹采集完成之后，或采集出错时，将收集到的指纹信息抛出  ****这里避免new，coverity无法检测到函数内的释放****
					PostMessage(WM_FINGERPRINT_FIREDATA, reinterpret_cast<WPARAM>(&m_strIdCardInfo));
				}
			}
		}
		Sleep(500);
	}
}

//身份证信息消息处理函数
LRESULT CPreviewS20OCXCtrl::OnFireFingerPrint( WPARAM wParam, LPARAM lParam )
{
	CString* pString = reinterpret_cast<CString*>(wParam);
	if (pString != NULL)
	{
		CString strAllInfo = *pString;

		CString strName = strAllInfo.Mid(0, 31);//姓名
		CString strSex = strAllInfo.Mid(31, 3);//性别
		CString strNation = strAllInfo.Mid(31+3, 10);//民族
		CString strBirthday = strAllInfo.Mid(31+3+10, 9);//出生日期
		CString strAddress = strAllInfo.Mid(31+3+10+9, 71);//住址
		CString strIDNo = strAllInfo.Mid(31+3+10+9+71, 19);//身份证号
		CString strGoverment = strAllInfo.Mid(31+3+10+9+71+19, 31);//签发机关
		CString strBeginData = strAllInfo.Mid(31+3+10+9+71+19+31, 9);//有效期开始日期
		CString strEndData = strAllInfo.Mid(31+3+10+9+71+19+31+9, 9);//有效期结束日期

		USES_CONVERSION;
		CString strTmp;
		char sep[] = "|";
		char *token = NULL;
		token = strtok(T2A(strAllInfo.GetBuffer(strAllInfo.GetLength())), sep);
		//姓名
		if (token != NULL)
		{
			strName = A2T(token);
			token = strtok(NULL, sep);
		}
		//性别
		if (token != NULL)
		{
			strSex = A2T(token);
			token = strtok(NULL, sep);
		}
		//民族
		if (token != NULL)
		{
			strNation = A2T(token);
			token = strtok(NULL, sep);
		}
		//出生日期
		if (token != NULL)
		{
			strBirthday = A2T(token);
			token = strtok(NULL, sep);
		}
		//住址
		if (token != NULL)
		{
			strAddress = A2T(token);
			token = strtok(NULL, sep);
		}
		//身份证号
		if (token != NULL)
		{
			strIDNo = A2T(token);
			token = strtok(NULL, sep);
		}
		//签发机关
		if (token != NULL)
		{
			strGoverment = A2T(token);
			token = strtok(NULL, sep);
		}
		//有效期开始日期
		if (token != NULL)
		{
			strBeginData = A2T(token);
			token = strtok(NULL, sep);
		}
		//有效期结束日期
		if (token != NULL)
		{
			strEndData = A2T(token);
			token = strtok(NULL, sep);
		}

		//上抛身份证信息
		FireIDCardInfo(strName, strIDNo, strBirthday, strSex, strNation, strAddress, strBeginData, strEndData, strGoverment);

		// 传递完成后，释放内存
		delete pString;
	}

	return 0;
}
//lint +e628
