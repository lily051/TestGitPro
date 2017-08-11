// PlaybackOCX3.0Ctrl.cpp : CPlaybackOCX30Ctrl ActiveX �ؼ����ʵ�֡�
//lint -library
#include "stdafx.h"
#include "PlaybackOCX2.0.h"
#include "PlaybackOCX2.0Ctrl.h"
#include "PlaybackOCX2.0PropPage.h"
#include "utils.h"
#include "Markup.h"
#include "xmldef.h"

//OCX��ȫ����
#include "comcat.h" 
#include "objsafe.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ȫ��ָ��.
CPlaybackOCX30Ctrl* g_pCtrl = NULL;
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

IMPLEMENT_DYNCREATE(CPlaybackOCX30Ctrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CPlaybackOCX30Ctrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CPlaybackOCX30Ctrl, COleControl)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lLayout", dispidlLayout, m_lLayout, OnlLayoutChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lSkinType", dispidlSkinType, m_lSkinType, OnlSkinTypeChanged, VT_I4)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "PB_Playback", dispidPB_Playback, PB_Playback, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "PB_LocalPlayback", dispidPB_LocalPlayback, PB_LocalPlayback, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "PB_StopPlayback", dispidPB_StopPlayback, PB_StopPlayback, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "PB_StopAll", dispidPB_StopAll, PB_StopAll, VT_I4, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lpstrPath", dispidlpstrPath, m_lpstrPath, OnlpstrPathChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lPicType", dispidlPicType, m_lPicType, OnlPicTypeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lModuleStyle", dispidlModuleStyle, m_lModuleStyle, OnlModuleStyleChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CPlaybackOCX30Ctrl, "lBtnRights", dispidlpstrBtnRights, m_lBtnRights, OnlBtnRights, VT_BSTR)
	DISP_FUNCTION_ID(CPlaybackOCX30Ctrl, "PB_GetVersion", dispidPB_GetVersion, PB_GetVersion, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CPlaybackOCX30Ctrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CPlaybackOCX30Ctrl, 1)
	PROPPAGEID(CPlaybackOCX30PropPage::guid)
END_PROPPAGEIDS(CPlaybackOCX30Ctrl)



// ��ʼ���๤���� guid

#ifdef _FOR8120
	IMPLEMENT_OLECREATE_EX(CPlaybackOCX30Ctrl, "PLAYBACKOCX30.PlaybackOCX30Ctrl.1",
	0xc0400eb9, 0xb305, 0x447d, 0x80, 0xb7, 0x5c, 0xd4, 0xdf, 0x01, 0x0a, 0xef)
#elif _FOR8190
IMPLEMENT_OLECREATE_EX(CPlaybackOCX30Ctrl, "PLAYBACKOCX30.PlaybackOCX30ClientCtrl.1",
	0x875a909c, 0x61c0, 0x4d82, 0x9d, 0x09, 0x97, 0x34, 0xf4, 0xb2, 0x55, 0x3a)
#elif _FORCLIENT
	IMPLEMENT_OLECREATE_EX(CPlaybackOCX30Ctrl, "PLAYBACKOCX30.PlaybackOCX30ClientCtrl.1",
	0xf79eb02d, 0x83b1, 0x47c6, 0x9e, 0xe2, 0xdf, 0xc8, 0xce, 0xdf, 0x8c, 0x47)
#else
	IMPLEMENT_OLECREATE_EX(CPlaybackOCX30Ctrl, "PLAYBACKOCX30.PlaybackOCX30Ctrl.1",
	0x7ebe387, 0x935c, 0x45da, 0xa1, 0x21, 0xae, 0x19, 0xf1, 0x8c, 0xef, 0x44)
#endif


// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CPlaybackOCX30Ctrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

#ifndef _FORCLIENT
const IID BASED_CODE IID_DPlaybackOCX30 =
		{ 0xFAB0C0D8, 0x95B8, 0x4A2C, { 0xBA, 0x94, 0x69, 0x58, 0x4A, 0xC7, 0xEC, 0x53 } };
const IID BASED_CODE IID_DPlaybackOCX30Events =
		{ 0x10CC88DC, 0xD85C, 0x402B, { 0x84, 0xE, 0x8B, 0x86, 0x11, 0x87, 0x4, 0xD6 } };
#else
const IID BASED_CODE IID_DPlaybackOCX30 =
		{ 0x13D42446, 0xE430, 0x4E93, { 0xA0, 0xEF, 0x6, 0x59, 0x7E, 0x62, 0x5, 0xEE } };
const IID BASED_CODE IID_DPlaybackOCX30Events =
		{ 0x24950B1F, 0x2FA5, 0x435B, { 0xA2, 0xA0, 0xE1, 0xB6, 0x69, 0x7C, 0xC0, 0xD3 } };
#endif



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwPlaybackOCX30OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPlaybackOCX30Ctrl, IDS_PLAYBACKOCX30, _dwPlaybackOCX30OleMisc)



// CPlaybackOCX30Ctrl::CPlaybackOCX30CtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CPlaybackOCX30Ctrl ��ϵͳע�����

BOOL CPlaybackOCX30Ctrl::CPlaybackOCX30CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

// 	if (bRegister)
// 		return AfxOleRegisterControlClass(
// 			AfxGetInstanceHandle(),
// 			m_clsid,
// 			m_lpszProgID,
// 			IDS_PLAYBACKOCX30,
// 			IDB_PLAYBACKOCX30,
// 			afxRegApartmentThreading,
// 			_dwPlaybackOCX30OleMisc,
// 			_tlid,
// 			_wVerMajor,
// 			_wVerMinor);
// 	else
// 		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);

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
			IDS_PLAYBACKOCX30,
			IDB_PLAYBACKOCX30,
			afxRegInsertable | afxRegApartmentThreading, 
			_dwPlaybackOCX30OleMisc, 
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



// CPlaybackOCX30Ctrl::CPlaybackOCX30Ctrl - ���캯��

CPlaybackOCX30Ctrl::CPlaybackOCX30Ctrl()
{
	InitializeIIDs(&IID_DPlaybackOCX30, &IID_DPlaybackOCX30Events);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_pPlaybackMainWnd = NULL;
	m_lLayout = 0;
	m_lSkinType = 0;
	m_lpstrPath = _T("");
	m_lpstrPicPath = _T("");
	m_lPicType = 0;
	m_lModuleStyle = 0;
	m_lpstrBwInfo = _T("");
	m_lpstrIP = _T("");
	m_lUserLevel = 0;
	m_lPort = 0;
	m_lBtnRights = _T("");
}



// CPlaybackOCX30Ctrl::~CPlaybackOCX30Ctrl - ��������

CPlaybackOCX30Ctrl::~CPlaybackOCX30Ctrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->DestroyWindow();
		delete m_pPlaybackMainWnd;
		m_pPlaybackMainWnd = NULL;
	}
}



// CPlaybackOCX30Ctrl::OnDraw - ��ͼ����

void CPlaybackOCX30Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	/*pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);*/
}



// CPlaybackOCX30Ctrl::DoPropExchange - �־���֧��

void CPlaybackOCX30Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
	PX_Long(pPX, _T("lLayout"), m_lLayout);
	PX_Long(pPX, _T("lSkinType"), m_lSkinType);
	PX_String(pPX, _T("lpstrPath"), m_lpstrPath);
	PX_String(pPX, _T("lpstrPicPath"), m_lpstrPicPath);
	PX_Long(pPX, _T("lPicType"), m_lPicType);
	PX_Long(pPX, _T("lModuleStyle"), m_lModuleStyle);
	PX_String(pPX, _T("lBtnRights"), m_lBtnRights);
	PX_String(pPX, _T("lpstrIP"), m_lpstrIP);
	PX_Long(pPX, _T("lUserLevel"), m_lUserLevel);
	PX_Long(pPX, _T("lPort"), m_lPort);
}	



// CPlaybackOCX30Ctrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CPlaybackOCX30Ctrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CPlaybackOCX30Ctrl::AboutBox - ���û���ʾ�����ڡ���

void CPlaybackOCX30Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_PLAYBACKOCX30);
	dlgAbout.DoModal();
}



// CPlaybackOCX30Ctrl ��Ϣ�������

int CPlaybackOCX30Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

    CheckCurrentDirectory();

	// TODO:  �ڴ������ר�õĴ�������
	g_pCtrl = this;


	// ����·��.
	/*CString strConfigFile;
	AppFun_GetAppPath(strConfigFile);
	strConfigFile.AppendFormat(_T("%s"), _T("localdata\\PlaybackParamOCX.ini"));

	char * szConfigFile;
	int nLength;
	CP936_W2A(strConfigFile, strConfigFile.GetLength()+1, szConfigFile, nLength);
	MakeSureDirectoryPathExists(szConfigFile);
	CodeSet_Free(szConfigFile);

	WritePrivateProfileString(_T("PLAYBACK"), _T("strDownloadPath"), m_lpstrPath, strConfigFile);
	WritePrivateProfileString(_T("PLAYBACK"), _T("strPicPath"), m_lpstrPath, strConfigFile);
	CString strTemp;
	strTemp.Format(_T("%ld"), m_lPicType);
	WritePrivateProfileString(_T("PLAYBACK"), _T("nPicType"), strTemp, strConfigFile);*/

	OutputDebugString(_T("@z  CPlaybackOCX30Ctrl::OnCreate"));
	// ��ʼ��.
	if (!PBSD_Init((PBSDSkinType)m_lSkinType, m_lModuleStyle, m_lBtnRights))
	{
		OutputDebugString(_T("@z PBSD_Init error"));
		return 0;
	}

	PLAYBACK_CONFIG stConfig;
	char * szPath = NULL;
	int nLength = 0;
	CP936_W2A(m_lpstrPath, m_lpstrPath.GetLength()+1, szPath, nLength);
	_snprintf_s(stConfig.stPBParam.szDownloadPath, sizeof(stConfig.stPBParam.szDownloadPath),
		_TRUNCATE, szPath);
	CodeSet_Free(szPath);
	szPath = NULL;
	CP936_W2A(m_lpstrPath, m_lpstrPath.GetLength()+1, szPath, nLength);
	_snprintf_s(stConfig.stPBParam.szPicPath, sizeof(stConfig.stPBParam.szPicPath),
		_TRUNCATE, szPath);
	CodeSet_Free(szPath);
	szPath = NULL;

	stConfig.stPBParam.nPicType = m_lPicType;

	PDSD_SetPlayBackConfig(stConfig);

	
	if (NULL == m_pPlaybackMainWnd)
	{
		m_pPlaybackMainWnd = new CPlaybackMainWnd((PBSDLayout)m_lLayout, (PBSDSkinType)m_lSkinType, (DWORD)m_lModuleStyle, m_lpstrIP, m_lPort, m_lUserLevel);
		m_pPlaybackMainWnd->Create(NULL, NULL, WS_VISIBLE, CRect(0,0,0,0), this, IDD_PLAYBACKMAINWND, NULL);
		m_pPlaybackMainWnd->ShowWindow(SW_SHOW);
	}

	return 0;
}

void CPlaybackOCX30Ctrl::OnDestroy()
{
	COleControl::OnDestroy();
    CheckCurrentDirectory();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->DestroyWindow();
		delete m_pPlaybackMainWnd;
		m_pPlaybackMainWnd = NULL;
	}

	PBSD_UnInit();
	OutputDebugString(_T("@z  CPlaybackOCX30Ctrl::OnDestroy"));
}

void CPlaybackOCX30Ctrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);
    CheckCurrentDirectory();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->MoveWindow(0, 0, cx, cy, TRUE);
	}
}

void CPlaybackOCX30Ctrl::OnlLayoutChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CPlaybackOCX30Ctrl::OnlSkinTypeChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

LONG CPlaybackOCX30Ctrl::PB_Playback(LPCTSTR lpstrXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();
	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		return m_pPlaybackMainWnd->Playback(lpstrXML);
	}

	return -1;
}

LONG CPlaybackOCX30Ctrl::PB_LocalPlayback(LPCTSTR lpstrXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CheckCurrentDirectory();
	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		return m_pPlaybackMainWnd->LocalPlayback(lpstrXML);
	}

	return -1;
}

BSTR CPlaybackOCX30Ctrl::PB_GetRecordFile(LPCTSTR lpstrXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	CString strResult = _T("");

	// TODO: �ڴ���ӵ��ȴ���������
	return strResult.AllocSysString();
}

LONG CPlaybackOCX30Ctrl::PB_StopPlayback(LONG lWndNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->StopPlayback(lWndNo);
	}
	return -1;
}

LONG CPlaybackOCX30Ctrl::PB_StopAll(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->StopAll();
	}
	return -1;
}

LONG CPlaybackOCX30Ctrl::PB_SetParam(LPCTSTR lpstrParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->SetParameter(lpstrParam);
	}
	
	return -1;
}

void CPlaybackOCX30Ctrl::OnlpstrPathChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CPlaybackOCX30Ctrl::OnlPicTypeChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CPlaybackOCX30Ctrl::OnlModuleStyleChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CPlaybackOCX30Ctrl::OnlBtnRights(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CPlaybackOCX30Ctrl::OnlpstrPicPathChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

BSTR CPlaybackOCX30Ctrl::PB_GetPlaybackInfo(LONG lWndNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	if (m_pPlaybackMainWnd != NULL)
	{
		strResult = m_pPlaybackMainWnd->GetPlaybackInfo(lWndNo);
	}
	
	return strResult.AllocSysString();
}

void CPlaybackOCX30Ctrl::OnlpstrBwInfoChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

LONG CPlaybackOCX30Ctrl::PB_EnableWindow(LONG lEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	// TODO: �ڴ���ӵ��ȴ���������
	CString strMsg;
	strMsg.Format(_T("@z PB_EnableWindow:%ld"), lEnable);
	OutputDebugString(strMsg);

	if (m_pPlaybackMainWnd != NULL)
	{
		m_pPlaybackMainWnd->EnableWindow(lEnable);
	}

	return -1;
}

/**   @fn          GetFileVersion
 *    @brief   	   ��ȡ�ļ��汾��.
 *    @param[in]   strFileName:�ļ�·����strVersion:�ļ��汾��.
 *    @param[in]   
 *    @return      
 */
void CPlaybackOCX30Ctrl::GetFileVersion(const CString& strFileName, CString& strVersion)   
{      
	int iVerInfoSize;   
	TCHAR* pBuf = NULL;     
	VS_FIXEDFILEINFO* pVsInfo = NULL;   
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO); 

	iVerInfoSize = GetFileVersionInfoSize(strFileName,NULL); 
	if(iVerInfoSize != 0)   
	{      
		pBuf = new TCHAR[iVerInfoSize];   //�������ַ����� -- �����ͷ�
        if(NULL == pBuf)
        {
            return;
        }
		if(GetFileVersionInfo(strFileName, 0, iVerInfoSize, pBuf))
		{
			if(VerQueryValue(pBuf,_T("\\"),(void**)&pVsInfo, &iFileInfoSize))
			{
				strVersion.Format(_T("%d.%d.%d.%d"), 
					HIWORD(pVsInfo->dwFileVersionMS), 
					LOWORD(pVsInfo->dwFileVersionMS), 
					HIWORD(pVsInfo->dwFileVersionLS), 
					LOWORD(pVsInfo->dwFileVersionLS));   
			}
		}

		delete []pBuf;
		pBuf = NULL;
	}       
}

BSTR CPlaybackOCX30Ctrl::PB_GetVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CheckCurrentDirectory();

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	CString strPath;
	AppFun_GetAppPath(strPath);

	CString strMsg;
	strMsg.Format(_T("@z FilePath:%s"), (LPCTSTR)strPath);
	OutputDebugString(strMsg);

	strPath += _T("PlaybackOCX2.0.ocx");
	GetFileVersion(strPath, strResult);

	return strResult.AllocSysString();
}

BOOL CPlaybackOCX30Ctrl::IsInvokeAllowed(DISPID /*dispid*/)
{
	// TODO: �ڴ����ר�ô����/����û���

	return TRUE;
}

CString g_strPath = _T("");
void CPlaybackOCX30Ctrl::CheckCurrentDirectory()
{
    CString strAppPath;
    AppFun_GetAppPath(strAppPath);
    CString strInfo;
    strInfo.Format(_T("@previewocx current directory: %s\n"), (LPCTSTR)strAppPath);
    OutputDebugString(strInfo);
    if (g_strPath != strAppPath)
    {
        BOOL bRet = FALSE;
        HINSTANCE hInst = LoadLibrary(_T("Kernel32.dll")); 
        if(hInst) 
        {            
            typedef BOOL (WINAPI *pSetDllDirectory)(LPCTSTR);          
            pSetDllDirectory pfun = NULL;   

#ifdef UNICODE
            pfun = (pSetDllDirectory)GetProcAddress(hInst, "SetDllDirectoryW");
#else
            pfun = (pSetDllDirectory)GetProcAddress(hInst, "SetDllDirectoryA");
#endif // !UNICODE
            if (pfun)
            {
                bRet = pfun(strAppPath);
            }
            else
            {
                bRet = SetCurrentDirectory(strAppPath);
            }
            if (bRet)
            {
                g_strPath = strAppPath;
            }
            FreeLibrary(hInst); 
        }
    }
}