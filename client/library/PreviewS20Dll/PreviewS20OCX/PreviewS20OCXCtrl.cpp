// PreviewS20OCXCtrl.cpp : CPreviewS20OCXCtrl ActiveX �ؼ����ʵ�֡�
//lint -library
//lint -e628
#include "stdafx.h"
#include "PreviewS20OCX.h"
#include "PreviewS20OCXCtrl.h"
#include "PreviewS20OCXPropPage.h"
#include "base64.h"

//OCX��ȫ����
#include "comcat.h" 
#include "objsafe.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_FINGERPRINT_FIREDATA (WM_USER + 0x1001) // ����ָ����Ϣ��Ϣ
#define USB_COM 1001
#define DEVICE_ERROR_CODE_OK 1

// ȫ��ָ��.
CPreviewS20OCXCtrl * g_pCtrl = NULL;
////////////////////////////////OCX ��ȫ���� begin//////////////////////////////////////////
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
////////////////////////////////OCX ��ȫ���� end//////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPreviewS20OCXCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CPreviewS20OCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_FINGERPRINT_FIREDATA, OnFireFingerPrint)
END_MESSAGE_MAP()



// ����ӳ��

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



// �¼�ӳ��

BEGIN_EVENT_MAP(CPreviewS20OCXCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CPreviewS20OCXCtrl, 1)
	PROPPAGEID(CPreviewS20OCXPropPage::guid)
END_PROPPAGEIDS(CPreviewS20OCXCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CPreviewS20OCXCtrl, "PREVIEWS20OCX.PreviewS20OCXCtrl.1",
	0x4655ff8, 0x9979, 0x4ffa, 0x8f, 0x77, 0xc3, 0xa5, 0x39, 0xea, 0x73, 0x70)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CPreviewS20OCXCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID BASED_CODE IID_DPreviewS20OCX =
		{ 0xA25F1DD7, 0xD47C, 0x466C, { 0xA0, 0x75, 0x5E, 0xE2, 0xCC, 0xC5, 0x34, 0xAE } };
const IID BASED_CODE IID_DPreviewS20OCXEvents =
		{ 0x46292E59, 0x6C27, 0x4A45, { 0x9B, 0x1A, 0x2E, 0x96, 0xB8, 0xE1, 0x6, 0x59 } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwPreviewS20OCXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPreviewS20OCXCtrl, IDS_PREVIEWS20OCX, _dwPreviewS20OCXOleMisc)



// CPreviewS20OCXCtrl::CPreviewS20OCXCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CPreviewS20OCXCtrl ��ϵͳע�����

BOOL CPreviewS20OCXCtrl::CPreviewS20OCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��
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

//�����߳�
DWORD WINAPI _ReadCardThreadFun(void* pParam)
{
	CPreviewS20OCXCtrl* pInstance = (CPreviewS20OCXCtrl*) pParam;
	if (pInstance != NULL)
	{
		pInstance->ReadCard();
	}
	return 0;
}

// CPreviewS20OCXCtrl::CPreviewS20OCXCtrl - ���캯��

CPreviewS20OCXCtrl::CPreviewS20OCXCtrl()
	: m_bManualStop(FALSE)
{
	InitializeIIDs(&IID_DPreviewS20OCX, &IID_DPreviewS20OCXEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_pPreviewDlg = NULL;
	m_lPreviewMode = 0;
	m_lSkinType = 0;
	m_lBtnRights = 0;
	m_hReadCardThread = NULL;
	m_bInitSucc = FALSE;
    m_bStopThread = false;
    m_strIdCardInfo = _T("");
}



// CPreviewS20OCXCtrl::~CPreviewS20OCXCtrl - ��������

CPreviewS20OCXCtrl::~CPreviewS20OCXCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CPreviewS20OCXCtrl::OnDraw - ��ͼ����

void CPreviewS20OCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
}



// CPreviewS20OCXCtrl::DoPropExchange - �־���֧��

void CPreviewS20OCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
	PX_Long(pPX, _T("lLayoutMode"), m_lPreviewMode);
	PX_Long(pPX, _T("lSkinType"), m_lSkinType);
	PX_Long(pPX, _T("lBtnRights"), m_lBtnRights);
}



// CPreviewS20OCXCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CPreviewS20OCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CPreviewS20OCXCtrl::AboutBox - ���û���ʾ�����ڡ���

void CPreviewS20OCXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_PREVIEWS20OCX);
	dlgAbout.DoModal();
}



// CPreviewS20OCXCtrl ��Ϣ�������
int CPreviewS20OCXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	int nStart = 0;
	int nStop = 0;
	nStart = GetTickCount();

	// TODO:  �ڴ������ר�õĴ�������
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
	// TODO: �ڴ˴������Ϣ����������
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

//��ʼԤ��
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

//ֹͣԤ��
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

//��ʼ¼��
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

//ֹͣ¼��
BSTR CPreviewS20OCXCtrl::StopRecord(LONG lWndNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strXml;
	if (m_pPreviewDlg != NULL)
	{
		// �豸Ϊ�ֶ�ֹͣ¼��ȥ������Ҫ���¼�����
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
					//base64����
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

//����OSD��Ϣ
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

//����������Ϣ
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

//���ÿ�����վ��
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
*   @brief      ��ȡ����USB��ص���Ϣ
*   @param  
*   @return     XML��ʽ�ļ���Ϣ 
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
*	@brief	��ʼ�����������ӷ������������������߳�
*	@param  Input:	��
*           Output: ��
*           InOut:  ��
*	@return	��ʼ�������0 �ɹ���-1 ���ӷ�����ʧ�� -2 ���������߳�ʧ��
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
*	@brief	����ʼ��������ֹͣ�����̲߳��Ͽ��豸����
*	@param  Input:	��
*           Output: ��
*           InOut:  ��
*	@return	����ʼ�������0 �ɹ�
*/
LONG CPreviewS20OCXCtrl::UnInitializeReader()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Stop();

	m_bInitSucc = FALSE;

	return 0;
}

/**	@fn	FireCardSerialNo
*	@brief	�������֤��Ϣ
*	@param  Input:	lpName  ����
*	@param  Input:	lpIDCardNo  ���֤��
*	@param  Input:	lpBirthday  ��������
*	@param  Input:	lpSex  �Ա�
*	@param  Input:	lpNation  ����
*	@param  Input:	lpAddress  סַ
*	@param  Input:	lpBeginDate  ��Ч�ڿ�ʼ����
*	@param  Input:	lpEndDate  ��Ч�ڽ�������
*	@param  Input:	lpGovernment  ǩ�����
*           Output: ��
*           InOut:  ��
*	@return	
*/
void CPreviewS20OCXCtrl::FireIDCardInfo( LPCTSTR lpName, LPCTSTR lpIDCardNo, LPCTSTR lpBirthday, LPCTSTR lpSex, LPCTSTR lpNation, LPCTSTR lpAddress, LPCTSTR lpBeginDate, LPCTSTR lpEndDate, LPCTSTR lpGovernment )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	FireEvent(dispidFireIDCardInfo, EVENT_PARAM(VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR), lpName, lpIDCardNo, lpBirthday, lpSex, lpNation, lpAddress, lpBeginDate, lpEndDate, lpGovernment);
}

/**	@fn	Start
*	@brief	���������߳�
*	@param  Input:	��
*           Output: ��
*           InOut:  ��
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
			//�رն�����
			CloseComm();
			return false;
		}
	}

	return true;
}

/**	@fn	Stop
*	@brief	ֹͣ�����̣߳��Ͽ��豸����
*	@param  Input:	��
*           Output: ��
*           InOut:  ��
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
		//�رն����߳�
		m_bStopThread = true;
		if (m_hReadCardThread)
		{
			WaitForSingleObject(m_hReadCardThread, INFINITE);
			CloseHandle(m_hReadCardThread);
			m_hReadCardThread = NULL;
		}

		//�رն�����
		CloseComm();
	}
	catch (...)
	{

	}
}

/**	@fn	ReadCard
*	@brief  ����ִ�к���
*	@param  Input:	��
*           Output: ��
*           InOut:  ��
*	@return	
*/
void CPreviewS20OCXCtrl::ReadCard()
{
	unsigned long lCardNo = 0;
	while (!m_bStopThread)
	{
		// _Mode: IDLE = 0, ALL = 1��
		// ��ΪIDLEʱ������dc_halt�ӿ�ʹ�ö��������ٽ���ͬһ���ſ�Ƭ���������������ˢ����
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
				//�����Ϣ�������֤��
				if (nLen >= 192)
				{
					CString strTemp;

					//����
					char chName[32] = {0};
					memcpy_s(chName, 32, Msg, 31);
					strTemp.Format(L"%s", A2T(chName));
					m_strIdCardInfo += strTemp + L"|";
					//�Ա�
					char chSex[4] = {0};
					memcpy_s(chSex, 4, Msg+31, 3);
					strTemp.Format(L"%s", A2T(chSex));
					m_strIdCardInfo += strTemp + L"|";
					//����
					char chNation[11] = {0};
					memcpy_s(chNation, 11, Msg+31+3, 10);
					strTemp.Format(L"%s", A2T(chNation));
					m_strIdCardInfo += strTemp + L"|";
					//��������
					char chBirthday[10] = {0};
					memcpy_s(chBirthday, 10, Msg+31+3+10, 9);
					strTemp.Format(L"%s", A2T(chBirthday));
					m_strIdCardInfo += strTemp + L"|";
					//סַ
					char chAddress[72] = {0};
					memcpy_s(chAddress, 72, Msg+31+3+10+9, 71);
					strTemp.Format(L"%s", A2T(chAddress));
					m_strIdCardInfo += strTemp + L"|";
					//���֤��
					char chCardNo[20] = {0};
					memcpy_s(chCardNo, 20, Msg+31+3+10+9+71, 19);
					strTemp.Format(L"%s", A2T(chCardNo));
					m_strIdCardInfo += strTemp + L"|";
					//ǩ������
					char chGovernment[32] = {0};
					memcpy_s(chGovernment, 32, Msg+31+3+10+9+71+19, 31);
					strTemp.Format(L"%s", A2T(chGovernment));
					m_strIdCardInfo += strTemp + L"|";
					//��Ч����ʼ����
					char chBeginData[10] = {0};
					memcpy_s(chBeginData, 10, Msg+31+3+10+9+71+19+31, 9);
					strTemp.Format(L"%s", A2T(chBeginData));
					m_strIdCardInfo += strTemp + L"|";
					//��Ч�ڽ�������
					char chEndData[10] = {0};
					memcpy_s(chEndData, 10, Msg+31+3+10+9+71+19+31+9, 9);
					strTemp.Format(L"%s", A2T(chEndData));
					m_strIdCardInfo += strTemp;

					// ָ�Ʋɼ����֮�󣬻�ɼ�����ʱ�����ռ�����ָ����Ϣ�׳�  ****�������new��coverity�޷���⵽�����ڵ��ͷ�****
					PostMessage(WM_FINGERPRINT_FIREDATA, reinterpret_cast<WPARAM>(&m_strIdCardInfo));
				}
			}
		}
		Sleep(500);
	}
}

//���֤��Ϣ��Ϣ������
LRESULT CPreviewS20OCXCtrl::OnFireFingerPrint( WPARAM wParam, LPARAM lParam )
{
	CString* pString = reinterpret_cast<CString*>(wParam);
	if (pString != NULL)
	{
		CString strAllInfo = *pString;

		CString strName = strAllInfo.Mid(0, 31);//����
		CString strSex = strAllInfo.Mid(31, 3);//�Ա�
		CString strNation = strAllInfo.Mid(31+3, 10);//����
		CString strBirthday = strAllInfo.Mid(31+3+10, 9);//��������
		CString strAddress = strAllInfo.Mid(31+3+10+9, 71);//סַ
		CString strIDNo = strAllInfo.Mid(31+3+10+9+71, 19);//���֤��
		CString strGoverment = strAllInfo.Mid(31+3+10+9+71+19, 31);//ǩ������
		CString strBeginData = strAllInfo.Mid(31+3+10+9+71+19+31, 9);//��Ч�ڿ�ʼ����
		CString strEndData = strAllInfo.Mid(31+3+10+9+71+19+31+9, 9);//��Ч�ڽ�������

		USES_CONVERSION;
		CString strTmp;
		char sep[] = "|";
		char *token = NULL;
		token = strtok(T2A(strAllInfo.GetBuffer(strAllInfo.GetLength())), sep);
		//����
		if (token != NULL)
		{
			strName = A2T(token);
			token = strtok(NULL, sep);
		}
		//�Ա�
		if (token != NULL)
		{
			strSex = A2T(token);
			token = strtok(NULL, sep);
		}
		//����
		if (token != NULL)
		{
			strNation = A2T(token);
			token = strtok(NULL, sep);
		}
		//��������
		if (token != NULL)
		{
			strBirthday = A2T(token);
			token = strtok(NULL, sep);
		}
		//סַ
		if (token != NULL)
		{
			strAddress = A2T(token);
			token = strtok(NULL, sep);
		}
		//���֤��
		if (token != NULL)
		{
			strIDNo = A2T(token);
			token = strtok(NULL, sep);
		}
		//ǩ������
		if (token != NULL)
		{
			strGoverment = A2T(token);
			token = strtok(NULL, sep);
		}
		//��Ч�ڿ�ʼ����
		if (token != NULL)
		{
			strBeginData = A2T(token);
			token = strtok(NULL, sep);
		}
		//��Ч�ڽ�������
		if (token != NULL)
		{
			strEndData = A2T(token);
			token = strtok(NULL, sep);
		}

		//�������֤��Ϣ
		FireIDCardInfo(strName, strIDNo, strBirthday, strSex, strNation, strAddress, strBeginData, strEndData, strGoverment);

		// ������ɺ��ͷ��ڴ�
		delete pString;
	}

	return 0;
}
//lint +e628
