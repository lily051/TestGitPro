// PreviewS20OCXCtrl.cpp : CPreviewS20OCXCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "PreviewS20OCX.h"
#include "PreviewS20OCXCtrl.h"
#include "PreviewS20OCXPropPage.h"

//OCX安全下载
#include "comcat.h" 
#include "objsafe.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CPreviewS20OCXCtrl, COleControl)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StartPreview", dispidStartPreview, StartPreview, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StopPreview", dispidStopPreview, StopPreview, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StartRecord", dispidStartRecord, StartRecord, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StopRecord", dispidStopRecord, StopRecord, VT_I4, VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CPreviewS20OCXCtrl, "lLayoutMode", dispidlLayoutMode, m_lLayoutMode, OnlLayoutModeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPreviewS20OCXCtrl, "lSkinType", dispidlSkinType, m_lSkinType, OnlSkinTypeChanged, VT_I4)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "OnRecordFileInfo", dispidOnRecordFileInfo, OnRecordFileInfo, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CPreviewS20OCXCtrl, "StopRecordByWndNumXML", dispidStopRecordByWndNumXML, StopRecord, VT_BSTR, VTS_I4)
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

	//if (bRegister)
	//	return AfxOleRegisterControlClass(
	//		AfxGetInstanceHandle(),
	//		m_clsid,
	//		m_lpszProgID,
	//		IDS_PREVIEWS20OCX,
	//		IDB_PREVIEWS20OCX,
	//		afxRegApartmentThreading,
	//		_dwPreviewS20OCXOleMisc,
	//		_tlid,
	//		_wVerMajor,
	//		_wVerMinor);
	//else
	//	return AfxOleUnregisterClass(m_clsid, m_lpszProgID);

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



// CPreviewS20OCXCtrl::CPreviewS20OCXCtrl - 构造函数

CPreviewS20OCXCtrl::CPreviewS20OCXCtrl()
	: m_bManualStop(FALSE)
{
	InitializeIIDs(&IID_DPreviewS20OCX, &IID_DPreviewS20OCXEvents);
	// TODO: 在此初始化控件的实例数据。
	m_pPreviewDlg = NULL;
	m_lLayoutMode = 0;
	m_lSkinType = 0;
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
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CPreviewS20OCXCtrl::DoPropExchange - 持久性支持

void CPreviewS20OCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
	PX_Long(pPX, _T("lLayoutMode"), m_lLayoutMode);
	PX_Long(pPX, _T("lSkinType"), m_lSkinType);
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
		stPrevInit.lLayoutMode = m_lLayoutMode;
		//stPrevInit.lPrevRights = m_lPreviewRights;
		//stPrevInit.lCapType = m_lCapType;
		stPrevInit.lSkinType = m_lSkinType;
		//stPrevInit.lMaxRecSize = m_lMaxRecSize;
		//stPrevInit.strSavePath = m_strSavePath;
		//stPrevInit.lHoverToolBarRight = m_lHoverToolBarRight;
		//stPrevInit.lToolBarRight = m_lToolBarRight;
		//stPrevInit.lUserLevel = m_lUserLevel;
		m_pPreviewDlg = new CPreviewDlg(stPrevInit, this);
		m_pPreviewDlg->Create(CPreviewDlg::IDD, this);
		m_pPreviewDlg->ShowWindow(SW_SHOW);
	}

	nStop = GetTickCount();
	CString strTime;
	strTime.Format(_T("<<<<<<<<<< CPreviewOCX30Ctrl::OnCreate Time=%d >>>>>>>>>>"), nStop-nStart);
	OutputDebugString(strTime);

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

	COleControl::OnDestroy();

	nStop = GetTickCount();
	CString strTime;
	strTime.Format(_T("<<<<<<<<<< CPreviewOCX30Ctrl::OnDestroy Time=%d >>>>>>>>>>"), nStop-nStart);
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

LONG CPreviewS20OCXCtrl::StartPreview(LPCTSTR strXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMsg;
	strMsg.Format(_T("@PreviewS20OCX -- %s"), strXML);
	OutputDebugString(strMsg);

	if (m_pPreviewDlg != NULL)
	{
		PREVIEW_CAMINFO struPrevCamInfo;
		ParseXml4Preview(struPrevCamInfo, strXML);

		return m_pPreviewDlg->StartPreview(struPrevCamInfo);
	}

	return 0;
}

LONG CPreviewS20OCXCtrl::StopPreview(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		return m_pPreviewDlg->StopPreview();
	}
	return 0;
}

LONG CPreviewS20OCXCtrl::StartRecord(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		return m_pPreviewDlg->StartRecord(lWndNum);
	}
	return 0;
}

/** @fn         StopRecordByWndNumXML
*   @brief      停止录像并返回文件信息
*   @param[IN]  lWndNum:窗口编号
*   @return     XML格式文件信息 
*/
BSTR CPreviewS20OCXCtrl::StopRecord(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strXml;
	if (m_pPreviewDlg != NULL)
	{
		// 设备为手动停止录像，去除不必要的事件提醒
		m_bManualStop = TRUE;

		RECOED_FILE_INFO recoedFile;
		if (m_pPreviewDlg->StopRecord(lWndNum, recoedFile))
		{
			SerialRecordFile4Xml(recoedFile, strXml);
			return strXml.AllocSysString();
		}
	}
	return strXml.AllocSysString();
}

void CPreviewS20OCXCtrl::ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml)
{
	CMarkup xml;
	if (!xml.SetDoc(strXml))
	{
		return;
	}

	if (!xml.FindElem(PV_XML_PREVIEW))
	{
		return;
	}
	xml.IntoElem();

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
		struPrevCamInfo.strPwd = xml.GetData();
	}

	if (xml.FindElem(PV_XML_CHANNEL))
	{
		struPrevCamInfo.nChannel = _wtoi(xml.GetData());
	}

	if (xml.FindElem(PV_XML_REALPLAYTYPE))
	{
		struPrevCamInfo.nRealPlayType = _wtoi(xml.GetData());
	}

	if (xml.FindElem(PV_XML_VIDEOFORMAT))
	{
		struPrevCamInfo.nVideoFormat = _wtoi(xml.GetData());
	}

	if (xml.FindElem(PV_XML_CAMTYPE))
	{
		struPrevCamInfo.nCamType = _wtoi(xml.GetData());
	}

	if (xml.FindElem(PV_XML_CAMRIGHT))
	{
		struPrevCamInfo.nCamRight = _wtoi(xml.GetData());
	}

	// 协议类型
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

void CPreviewS20OCXCtrl::SerialRecordFile4Xml(const RECOED_FILE_INFO &recoedFile, CString& strXml)
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
	strFileSize.Format(_T("<FileSize>%d</FileSize>"), recoedFile.lFileSize);
	strXml += strFileSize;

	CString strFileType;
	strFileType.Format(_T("<FileType>%d</FileType>"), recoedFile.lFileType);
	strXml += strFileType;

	CString strStartTime;
	strStartTime.Format(_T("<StartTime>%d</StartTime>"), recoedFile.lStartTime);
	strXml += strStartTime;

	CString strStopTime;
	strStopTime.Format(_T("<StopTime>%d</StopTime>"), recoedFile.lEndTime);
	strXml += strStopTime;

	CString strRecordStatus;
	strRecordStatus.Format(_T("<RecordStatus>%d</RecordStatus>"), recoedFile.nRecordStatus);
	strXml += strRecordStatus;

	CString strParamEnd = _T("</Param>");
	strXml += strParamEnd;
}

void CPreviewS20OCXCtrl::NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile)
{
	CString strXml;
	SerialRecordFile4Xml(recoedFile, strXml);
	OnRecordFileInfo(strXml);
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

BOOL CPreviewS20OCXCtrl::IsInvokeAllowed(DISPID /*dispid*/)
{
	return TRUE;
}

LONG CPreviewS20OCXCtrl::StopAllPreview(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_pPreviewDlg != NULL)
	{
		return m_pPreviewDlg->StopPreview();
	}
	return 0;
}
