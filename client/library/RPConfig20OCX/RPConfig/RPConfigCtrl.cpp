// RPConfigCtrl.cpp : CRPConfigCtrl ActiveX 控件类的实现。
//lint -library
#include "stdafx.h"
#include "RPConfig.h"
#include "RPConfigCtrl.h"
#include "RPConfigPropPage.h"
#include "Markup/Markup.h"
#include <atlconv.h>
#include <HCNetSDK.h>
#include <vector>
#include "hcs_sdk.h"
#include "base64.h"

#pragma comment (lib, "Version.lib")

//ocx 安全下载
#include "comcat.h" 
#include "objsafe.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USE_NEW_INTEL  TRUE

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


IMPLEMENT_DYNCREATE(CRPConfigCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CRPConfigCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
    ON_WM_CREATE()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CRPConfigCtrl, COleControl)
	DISP_FUNCTION_ID(CRPConfigCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRPConfigCtrl, "RP_InitLib", dispidRP_InitLib, RP_InitLib, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CRPConfigCtrl, "RP_UninitLib", dispidRP_UninitLib, RP_UninitLib, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CRPConfigCtrl, "RP_Function1", dispidRP_Function1, RP_Function1, VT_BSTR, VTS_BSTR)
    DISP_FUNCTION_ID(CRPConfigCtrl, "RP_GetVersion", dispidRP_GetVersion, RP_GetVersion, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CRPConfigCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CRPConfigCtrl, 1)
	PROPPAGEID(CRPConfigPropPage::guid)
END_PROPPAGEIDS(CRPConfigCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CRPConfigCtrl, "RPCONFIG.RPConfigCtrl.1",
	0xa4660bf5, 0x7356, 0x4a3b, 0x8a, 0x34, 0x58, 0x59, 0xed, 0x6c, 0x4f, 0x2f)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CRPConfigCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_DRPConfig =
		{ 0x21AE9E06, 0xDAE1, 0x43B8, { 0x96, 0xC9, 0x22, 0x27, 0xAE, 0x34, 0x2D, 0x60 } };
const IID BASED_CODE IID_DRPConfigEvents =
		{ 0x428F28E, 0x5E9D, 0x4DA2, { 0xA9, 0x1B, 0x9, 0x23, 0x41, 0x89, 0x78, 0x68 } };



// 控件类型信息

static const DWORD BASED_CODE _dwRPConfigOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRPConfigCtrl, IDS_RPCONFIG, _dwRPConfigOleMisc)



// CRPConfigCtrl::CRPConfigCtrlFactory::UpdateRegistry -
// 添加或移除 CRPConfigCtrl 的系统注册表项

BOOL CRPConfigCtrl::CRPConfigCtrlFactory::UpdateRegistry(BOOL bRegister)
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
	//		IDS_RPCONFIG,
	//		IDB_RPCONFIG,
	//		afxRegApartmentThreading,
	//		_dwRPConfigOleMisc,
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
            IDS_RPCONFIG,
            IDB_RPCONFIG,
            afxRegInsertable | afxRegApartmentThreading, 
            _dwRPConfigOleMisc, 
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



// CRPConfigCtrl::CRPConfigCtrl - 构造函数

CRPConfigCtrl::CRPConfigCtrl()
: m_bInit(FALSE)
{
	InitializeIIDs(&IID_DRPConfig, &IID_DRPConfigEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CRPConfigCtrl::~CRPConfigCtrl - 析构函数

CRPConfigCtrl::~CRPConfigCtrl()
{
	// TODO: 在此清理控件的实例数据。
	RP_UninitLib();
}



// CRPConfigCtrl::OnDraw - 绘图函数

void CRPConfigCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;
}



// CRPConfigCtrl::DoPropExchange - 持久性支持

void CRPConfigCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CRPConfigCtrl::OnResetState - 将控件重置为默认状态

void CRPConfigCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CRPConfigCtrl::AboutBox - 向用户显示“关于”框

void CRPConfigCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_RPCONFIG);
	dlgAbout.DoModal();
}



// CRPConfigCtrl 消息处理程序

/**	@fn	    RP_InitLib
*	@brief	初始化库
*	@param  [in] xmlParam参数报文
*	@return	0成功，-1失败
*/
LONG CRPConfigCtrl::RP_InitLib()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_bInit)
    {
        return -1;
    }

    // 初始化
    if (!NET_DVR_Init())
    {
		RPC_ERROR(_T("NET_DVR_Init falid, error = %d"), NET_DVR_GetLastError());
    }
	int nRet = HCS_SDK_Init();
	if (HCS_OK != nRet)
	{
		RPC_ERROR(_T("HCS_SDK_Init falid, error = %d"), nRet);
	}
    m_bInit = TRUE;

    return 0;
}

/**	@fn	    RP_UninitLib
*	@brief	反初始化库
*	@param  [in] 
*	@return	0成功，-1失败
*/
LONG CRPConfigCtrl::RP_UninitLib(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (!m_bInit)
    {
		RPC_ERROR(_T("RP_UninitLib falid!"));
        return -1;
    }

	//释放配置库资源
	int nRet = HCS_SDK_Deinit();
	if (HCS_OK != nRet)
	{
		RPC_ERROR(_T("HCS_SDK_Deinit falid, error = %d"), nRet);
	}
    //释放SDK资源
	if (!NET_DVR_Cleanup())
	{
		RPC_ERROR(_T("NET_DVR_Cleanup falid, error = %d"), NET_DVR_GetLastError());
	}

    m_bInit = FALSE;
    return 0;
}

/**	@fn	    RP_Function1
*	@brief	功能函数1
*	@param  [in] xmlParam参数报文
*	@return	空失败;成功返回结果XML报文
*/
BSTR CRPConfigCtrl::RP_Function1(LPCTSTR xmlParam)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString strInfo;
    strInfo.Format(_T("$RPConfig RP_Function1 %s\n"), xmlParam);
    OutputDebugString(strInfo);
    CString strResult;//返回的结果字符串
    if (!m_bInit)
    {
		RPC_ERROR(_T("RP_Function1 falid!"));
        return strResult.AllocSysString();
    }

    FUN_PARAM funParam;
    if (!ParseFunParam(xmlParam, funParam))
    {
		RPC_ERROR(_T("ParseFunParam falid!"));
        return strResult.AllocSysString();
    }

    switch(funParam.nCode)
    {
    case CODE_GET_DEV_INFO:
        {
            return GetDevInfo(funParam);
        }
    case CODE_GET_CHANNEL_NAME:
        {
            return GetChannelName(funParam);
        }
    case CODE_SHOW_REMOTE_CONFIG:
        {
            //return ShowRemoteConfig(funParam);
        }
    case CODE_SET_NTPCONFIG:
        {
            return SetDevNTPConfig(funParam);
        }
    default:
        break;
    }

    return strResult.AllocSysString();
}

/**	@fn	    ParseInitLibParam
*	@brief	转换初始化参数
*	@param  [in] xmlParam参数报文
*   @param  [out] initLibParam转换后的参数信息
*	@return	TRUE成功，FALSE失败
*/
BOOL CRPConfigCtrl::ParseInitLibParam(LPCTSTR xmlParam, INIT_LIB_PARAM & initLibParam)
{
    CMarkup xmlFile;
    if (!xmlFile.SetDoc(xmlParam))
    {
        return FALSE;
    }
    if (xmlFile.FindChildElem(XML_NODE_LOG_PATH))
    {
        initLibParam.sLogPath = xmlFile.GetChildData();
    }
    return TRUE;
}

/**	@fn	    ParseFunParam
*	@brief	转换函数参数
*	@param  [in] xmlParam参数报文
*   @param  [out] funParam函数参数
*	@return	TRUE成功，FALSE失败
*/
BOOL CRPConfigCtrl::ParseFunParam(LPCTSTR xmlParam, FUN_PARAM & funParam)
{
    USES_CONVERSION;//不要写在循环体呢
    CMarkup xmlFile;
    if (!xmlFile.SetDoc(xmlParam))
    {
        return FALSE;
    }

    if (xmlFile.FindChildElem(XML_NODE_CODE))
    {
        funParam.nCode = atoi(T2A(xmlFile.GetChildData()));
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_TYPE))
    {
        funParam.nDevType = atoi(T2A(xmlFile.GetChildData()));
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_IP))
    {
        funParam.sDevIP = xmlFile.GetChildData();
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_PORT))
    {
        funParam.nDevPort = atoi(T2A(xmlFile.GetChildData()));
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_USER))
    {
        funParam.sDevUser = xmlFile.GetChildData();
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_PWD))
    {
       // funParam.sDevPWD = xmlFile.GetChildData();
		const char* pDevPWDIn = T2A(xmlFile.GetChildData().GetBuffer()); 
		int nLenIn = strlen(pDevPWDIn);
		unsigned char devPWDOut[512] = {0};
		int nLenOut = sizeof(devPWDOut);
		CString strErr;
		//base64解密
		if (CBase64::Decrypt((const unsigned char*)pDevPWDIn, nLenIn, devPWDOut, &nLenOut, strErr))
		{
			funParam.sDevPWD = devPWDOut;
		}
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_DEV_CHANNEL))
    {
        funParam.nChannel = atoi(T2A(xmlFile.GetChildData()));
    }

    if (xmlFile.FindChildElem(XML_NODE_DEV_SHOW_IP_MODE))
    {
        if (1 == atoi(T2A(xmlFile.GetChildData())))
        {
            funParam.bShowIPMode = TRUE;
        }
        else
        {
            funParam.bShowIPMode = FALSE;
        }
    }

    if (xmlFile.FindChildElem(XML_NODE_NTP_SERVER))
    {
        strcpy((char*)funParam.nNtpConfigInfo.sNTPServer, T2A(xmlFile.GetChildData()));
    }
    if (xmlFile.FindChildElem(XML_NODE_NTP_PORT))
    {
        funParam.nNtpConfigInfo.wNtpPort = atoi(T2A(xmlFile.GetChildData()));
    }
    if (xmlFile.FindChildElem(XML_NODE_NTP_INTERVAL))
    {
        funParam.nNtpConfigInfo.wInterval = atoi(T2A(xmlFile.GetChildData()));
    }
    if (xmlFile.FindChildElem(XML_NODE_NTP_ENABLE))
    {
        funParam.nNtpConfigInfo.byEnableNTP = atoi(T2A(xmlFile.GetChildData()));
        //strcpy((char)funParam.nNtpConfigInfo.byEnableNTP, T2A(xmlFile.GetChildData()));
    }

    return TRUE;
}

/**	@fn	    GetDevInfo
*	@brief	获取设备名称
*	@param  [in] funParam参数信息
*	@return	空失败;成功返回结果XML报文
*/
BSTR CRPConfigCtrl::GetDevInfo(const FUN_PARAM & funParam)
{
    CString strResult;//返回的结果字符串
    switch(funParam.nDevType)
    {
    case DEV_TYPE_HIK:
        {
            return HIK_GetDevInfo(funParam);
        }
	case DEV_TYPE_CVR:
		{
			return CVR_GetDevInfo(funParam);
		}
	case DEV_TYPE_CLOUD:
		{
			return CLOUD_GetDevInfo(funParam);
		}
    default:
        break;
    }
    return strResult.AllocSysString();
}

/**	@fn	    GetChannelName
*	@brief	获取通道名称
*	@param  [in] funParam参数信息
*	@return	空失败;成功返回结果XML报文
*/
BSTR CRPConfigCtrl::GetChannelName(const FUN_PARAM & funParam)
{
    CString strResult;//返回的结果字符串
    switch(funParam.nDevType)
    {
    case DEV_TYPE_HIK:
        {
            return HIK_GetChannelName(funParam);
        }
    default:
        break;
    }
    return strResult.AllocSysString();
}

/**	@fn	    HIK_GetDevInfo
*	@brief	获取设备名称[海康设备]
*	@param  [in] funParam参数信息
*	@return	空失败;成功返回结果XML报文
*/
BSTR CRPConfigCtrl::HIK_GetDevInfo(const FUN_PARAM & funParam)
{
    CString strResult;//返回的结果字符串
    USES_CONVERSION;//不要写在循环体呢
    //登入设备
    NET_DVR_DEVICEINFO_V30 devInfoV30;//设备信息
    memset(&devInfoV30, 0, sizeof(NET_DVR_DEVICEINFO_V30));
    LONG lLoginID =  NET_DVR_Login_V30(T2A(funParam.sDevIP), 
        funParam.nDevPort,
        T2A(funParam.sDevUser), 
        T2A(funParam.sDevPWD),&devInfoV30);
    if (lLoginID == -1)
    {
		RPC_ERROR(_T("NET_DVR_Login_V30 falid, error = %d"), NET_DVR_GetLastError());
        CString strMsg;
        strMsg.Format(_T("登录失败。错误码:%ld"),NET_DVR_GetLastError());
        OutputDebugString(strMsg);
		strResult = _T("登录设备失败!");
		return strResult.AllocSysString();
    }

    NET_DVR_DEVICECFG pDeviceCfg;
    DWORD BytesReturned = 0;
    if(!NET_DVR_GetDVRConfig(lLoginID,NET_DVR_GET_DEVICECFG,0,&pDeviceCfg,sizeof(NET_DVR_DEVICECFG),&BytesReturned))
    {
        //登出设备
		RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), NET_DVR_GetLastError());
        NET_DVR_Logout_V30(lLoginID);
        CString strMsg;
        strMsg.Format(_T("获取设备信息失败。错误码:%ld"),NET_DVR_GetLastError());
        OutputDebugString(strMsg);
		strResult = _T("获取设备参数失败!");
		return strResult.AllocSysString();
    }

    NET_DVR_IPPARACFG_V40 sIPAccessCfgV40;
    DWORD dwReturned = 0;
    memset(&sIPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG_V40));
    int nIPChannelStart = 0;
    if (NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_IPPARACFG_V40, 0, &sIPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
    {
        nIPChannelStart = sIPAccessCfgV40.dwStartDChan-1;
    }

    //需要使用比较大的空间，否则当设备返回的名称到达最大值时，将不会有结束符
    char cName[NAME_LEN+1];
    ZeroMemory(cName, NAME_LEN+1);
    memcpy(cName, pDeviceCfg.sDVRName, NAME_LEN);

    RP_DEV_INFO rpDevInfo;
    rpDevInfo.strName = cName;
   
    char sSerialNumber[SERIALNO_LEN+1];
    ZeroMemory(sSerialNumber, SERIALNO_LEN+1);
    memcpy(sSerialNumber, pDeviceCfg.sSerialNumber, SERIALNO_LEN);
    rpDevInfo.strSerialNumber = sSerialNumber;
    rpDevInfo.nChanNum = devInfoV30.byChanNum;
    rpDevInfo.nDiskNum = devInfoV30.byDiskNum;
    rpDevInfo.nStartChanNum = devInfoV30.byStartChan-1;
    rpDevInfo.nIPChannel = devInfoV30.byIPChanNum;
    rpDevInfo.nIPChannelStart = nIPChannelStart;
    
    //登出设备
    NET_DVR_Logout_V30(lLoginID);

    return ParseDevInfoResult(rpDevInfo);
}

/**	@fn	    HIK_GetChannelName
*	@brief	获取通道名称[海康设备]
*	@param  [in] funParam参数信息
*	@return	空失败;成功返回结果XML报文
*/
BSTR CRPConfigCtrl::HIK_GetChannelName(const FUN_PARAM & funParam)
{
    CString strResult;//返回的结果字符串
    USES_CONVERSION;//不要写在循环体呢
    //登入设备
    NET_DVR_DEVICEINFO_V30 devInfoV30;//设备信息
    memset(&devInfoV30, 0, sizeof(NET_DVR_DEVICEINFO_V30));
    LONG lLoginID =  NET_DVR_Login_V30(T2A(funParam.sDevIP), funParam.nDevPort, T2A(funParam.sDevUser),T2A(funParam.sDevPWD),&devInfoV30);
    if (lLoginID == -1)
    {
		RPC_ERROR(_T("NET_DVR_Login_V30 falid, error = %d"), NET_DVR_GetLastError());
        CString strMsg;
        strMsg.Format(_T("登录失败。错误码:%ld"),NET_DVR_GetLastError());
        OutputDebugString(strMsg);
		strResult = _T("登录设备失败!");
		return strResult.AllocSysString();
    }

	//获取设备信息
	NET_DVR_DEVICECFG pDeviceCfg;
	DWORD BytesReturned = 0;
	if(!NET_DVR_GetDVRConfig(lLoginID,NET_DVR_GET_DEVICECFG,0,&pDeviceCfg,sizeof(NET_DVR_DEVICECFG),&BytesReturned))
	{
		//登出设备
		RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), NET_DVR_GetLastError());
		NET_DVR_Logout_V30(lLoginID);
		CString strMsg;
		strMsg.Format(_T("获取设备信息失败。错误码:%ld"),NET_DVR_GetLastError());
		OutputDebugString(strMsg);
		strResult = _T("获取设备参数失败!");
		return strResult.AllocSysString();
	}

	//获取IP通道信息
	NET_DVR_IPPARACFG_V40 sIPAccessCfgV40;
	DWORD dwReturned = 0;
	memset(&sIPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG_V40));
	int nIPChannelStart = 0;
	if (NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_IPPARACFG_V40, 0, &sIPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
	{
		nIPChannelStart = sIPAccessCfgV40.dwStartDChan;
	}

	RP_DEV_INFO rpDevInfo;
	rpDevInfo.nChanNum = devInfoV30.byChanNum;
	rpDevInfo.nStartChanNum = devInfoV30.byStartChan;
	rpDevInfo.nIPChannel = devInfoV30.byIPChanNum;
	rpDevInfo.nIPChannelStart = nIPChannelStart;

	//构建结果报文
	CMarkup   xmlFile;
	xmlFile.SetDoc(XML_HEAD);
	xmlFile.AddElem(XML_NODE_ROOT);
	xmlFile.IntoElem();
	//模拟通道
	for (int i = rpDevInfo.nStartChanNum;i < rpDevInfo.nChanNum+1; i++)
	{
		//获取监控点属性
		NET_DVR_PICCFG_V30 pPicCfg;
		memset(&pPicCfg,0,sizeof(NET_DVR_PICCFG_V30));
		DWORD dwRet = 0;
		if(!NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_PICCFG_V30, i, &pPicCfg,sizeof(NET_DVR_PICCFG_V30), &dwRet))
		{
			RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), NET_DVR_GetLastError());
			continue;
		}
		char cName[NAME_LEN+1];
		ZeroMemory(cName, NAME_LEN+1);
		memcpy(cName, pPicCfg.sChanName, NAME_LEN);
		CString sChannelName;
		sChannelName = cName;
		xmlFile.AddElem(_T("CHANNEL"));
		xmlFile.IntoElem();
		xmlFile.AddElem(_T("CHANNEL_ID"), i-1);
		xmlFile.AddElem(_T("CHANNEL_NAME"), sChannelName);
		xmlFile.OutOfElem();
	}
	//IP通道
	for (int i = rpDevInfo.nIPChannelStart;i < (rpDevInfo.nIPChannel + rpDevInfo.nIPChannelStart + 1); i++)
	{
		//获取监控点属性
		NET_DVR_PICCFG_V30 pPicCfg;
		memset(&pPicCfg,0,sizeof(NET_DVR_PICCFG_V30));
		DWORD dwRet = 0;
		if(!NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_PICCFG_V30, i, &pPicCfg,sizeof(NET_DVR_PICCFG_V30), &dwRet))
		{
			RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), NET_DVR_GetLastError());
			continue;
		}
		char cName[NAME_LEN+1];
		ZeroMemory(cName, NAME_LEN+1);
		memcpy(cName, pPicCfg.sChanName, NAME_LEN);
		CString sChannelName;
		sChannelName = cName;
		xmlFile.AddElem(_T("CHANNEL"));
		xmlFile.IntoElem();
		xmlFile.AddElem(_T("CHANNEL_ID"), i-1);
		xmlFile.AddElem(_T("CHANNEL_NAME"), sChannelName);
		xmlFile.OutOfElem();
	}
    //登出设备
    NET_DVR_Logout_V30(lLoginID);

    xmlFile.OutOfElem();
    strResult = xmlFile.GetDoc();

    return strResult.AllocSysString();
}

BSTR CRPConfigCtrl::CVR_GetDevInfo(const FUN_PARAM & funParam)
{
	CString strResult;
	USES_CONVERSION;

	NET_DVR_DEVICEINFO_V30 devInfoV30;//设备信息
	memset(&devInfoV30, 0, sizeof(NET_DVR_DEVICEINFO_V30));
	LONG lLoginID =  NET_DVR_Login_V30(T2A(funParam.sDevIP), funParam.nDevPort,T2A(funParam.sDevUser), T2A(funParam.sDevPWD), &devInfoV30);
	if (lLoginID < 0)
	{
		RPC_ERROR(_T("NET_DVR_Login_V30 falid, error = %d"), NET_DVR_GetLastError());
		strResult = _T("登录CVR设备失败!");
		return strResult.AllocSysString();
	}
	NET_DVR_HDCFG struHDCfg;
	memset(&struHDCfg,0,sizeof(NET_DVR_HDCFG));
	DWORD BytesReturned = 0;
	BOOL bRet = NET_DVR_GetDVRConfig(lLoginID, NET_DVR_GET_HDCFG, 0xFFFFFFFF, &struHDCfg,sizeof(NET_DVR_HDCFG), &BytesReturned);
	if(!bRet)
	{
		RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), NET_DVR_GetLastError());
		CString strError;
		strError.Format(_T("$RPConfig NET_DVR_GetDVRConfig failed[%lu]\n"), NET_DVR_GetLastError());
		OutputDebugString(strError);
		NET_DVR_Logout(lLoginID);
		lLoginID = -1;
		strResult = _T("获取CVR设备参数失败!");
		return strResult.AllocSysString();
	}

	//构建结果报文
	CMarkup   xmlFile;
	xmlFile.SetDoc(XML_HEAD);
	xmlFile.AddElem(XML_NODE_ROOT);
	xmlFile.IntoElem();
	for (int i = 0;i < struHDCfg.dwHDCount; i++)
	{
		xmlFile.IntoElem();
		xmlFile.AddElem(_T("HDINFO"));
		xmlFile.IntoElem();
		xmlFile.AddElem(_T("HDNO"), struHDCfg.struHDInfo[i].dwHDNo);              //编号
		xmlFile.AddElem(_T("HDCAP"), struHDCfg.struHDInfo[i].dwCapacity);         //容量
		xmlFile.AddElem(_T("HDFREESPACE"), struHDCfg.struHDInfo[i].dwFreeSpace);  //剩余
		xmlFile.AddElem(_T("HDTYPE"), struHDCfg.struHDInfo[i].byHDType);          //类型，0-本地硬盘,1-ESATA硬盘,2-NAS硬盘,3-iSCSI硬盘 4-Array虚拟磁盘
		xmlFile.OutOfElem();
		xmlFile.OutOfElem();
	}
	xmlFile.OutOfElem();
	strResult = xmlFile.GetDoc();
	return strResult.AllocSysString();
}

BSTR CRPConfigCtrl::CLOUD_GetDevInfo(const FUN_PARAM & funParam)
{
	CString strResult;//返回的结果字符串
	CString strMsg;
	USES_CONVERSION;//不要写在循环体
	//登入设备
	unsigned long long login_id = 0;
	int iRet = HCS_SDK_Login(T2A(funParam.sDevIP),funParam.nDevPort,T2A(funParam.sDevUser),T2A(funParam.sDevPWD),&login_id);
	if (iRet != HCS_OK)
	{
		RPC_ERROR(_T("HCS_SDK_Login falid, error = %d"), iRet);
		strResult = _T("登录Cloud失败!");
		return strResult.AllocSysString();
	}
	int nPoolCount = 0;
	int nHandle = 0;
	iRet = HCS_SDK_GetAllPoolInfoStart(login_id, &nPoolCount, &nHandle);
	if (iRet != HCS_OK)
	{
		RPC_ERROR(_T("HCS_SDK_GetAllPoolInfoStart falid, error = %d"), iRet);
		HCS_SDK_Logout(login_id);
		strResult = _T("获取Cloud参数失败!");
		return strResult.AllocSysString();
	}
	//构建结果报文
	CString strInfo;
	CMarkup xmlFile;
	xmlFile.SetDoc(XML_HEAD);
	xmlFile.AddElem(XML_NODE_ROOT);
	xmlFile.IntoElem();
	for (int i = 0; i < nPoolCount; i++)
	{
		HCS_PoolInfo stInfo;
		memset(&stInfo, 0, sizeof(HCS_PoolInfo));
		int nStatus = 0;
		iRet = HCS_SDK_API HCS_SDK_GetAllPoolInfoNext(login_id, nHandle, &stInfo, &nStatus);
		if (iRet != HCS_OK)
		{
			RPC_ERROR(_T("HCS_SDK_GetAllPoolInfoNext falid, error = %d"), iRet);
			strMsg.Format(_T("$RPConfig HCS_SDK_GetAllPoolInfoNext failed[%d]\n"), iRet);
			OutputDebugString(strMsg);
			HCS_SDK_GetAllPoolInfoStop(login_id, nHandle);
			HCS_SDK_Logout(login_id);
			strResult = _T("获取Cloud参数失败!");
			return strResult.AllocSysString();
		}
		xmlFile.AddElem(_T("HDINFO"));
		xmlFile.IntoElem();
		xmlFile.AddElem(_T("HDNO"), A2T(stInfo.pool_id));
		xmlFile.AddElem(_T("HDNAME"), A2T(stInfo.pool_name));
		xmlFile.AddElem(_T("HDSTAT"), stInfo.pool_status);
		xmlFile.AddElem(_T("HDTYPE"), stInfo.pool_type);
		strInfo.Format(_T("%lu"), stInfo.pool_size);
		xmlFile.AddElem(_T("HDCAP"), strInfo);
		strInfo.Format(_T("%lu"), stInfo.free_size);
		xmlFile.AddElem(_T("HDFREESPACE"), strInfo);
		xmlFile.OutOfElem();
	}
	HCS_SDK_GetAllPoolInfoStop(login_id, nHandle);
	HCS_SDK_Logout(login_id);
	xmlFile.OutOfElem();
	strResult = xmlFile.GetDoc();
	return strResult.AllocSysString();
}


BSTR CRPConfigCtrl::ParseDevInfoResult(const RP_DEV_INFO & rpDevInfo)
{
    CString strResult;//返回的结果字符串
    
    CMarkup   xmlFile;
    xmlFile.SetDoc(XML_HEAD);
    xmlFile.AddElem(XML_NODE_ROOT);
    xmlFile.IntoElem();
    xmlFile.AddElem(_T("DEV_NAME"), rpDevInfo.strName);
    xmlFile.AddElem(_T("DEV_DISK"), rpDevInfo.nDiskNum);
    xmlFile.AddElem(_T("DEV_CHANNEL"), rpDevInfo.nChanNum);
    xmlFile.AddElem(_T("DEV_START_CHANNEL"),rpDevInfo.nStartChanNum);
    xmlFile.AddElem(_T("DEV_IPCHANNEL"),rpDevInfo.nIPChannel);
    xmlFile.AddElem(_T("DEV_IPCHANNEL_START"),rpDevInfo.nIPChannelStart);
    xmlFile.OutOfElem();
    strResult = xmlFile.GetDoc();

    return strResult.AllocSysString();
}

void GetFileVersion(const CString& strFileName, CString& strVersion)   
{      
    int iVerInfoSize;   
    TCHAR* pBuf = NULL;     
    VS_FIXEDFILEINFO* pVsInfo = NULL;   
    unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO); 

    iVerInfoSize = GetFileVersionInfoSize(strFileName,NULL); 
    if(iVerInfoSize!= 0)   
    {      
        pBuf = new TCHAR[iVerInfoSize];   
        if(GetFileVersionInfo(strFileName, 0, iVerInfoSize, pBuf))      
        {      
            if(VerQueryValue(pBuf,_T("\\"),(void**)&pVsInfo,&iFileInfoSize))      
            {      
                strVersion.Format(_T("%d.%d.%d.%d"), 
                    HIWORD(pVsInfo->dwFileVersionMS), 
                    LOWORD(pVsInfo->dwFileVersionMS), 
                    HIWORD(pVsInfo->dwFileVersionLS), 
                    LOWORD(pVsInfo->dwFileVersionLS));   
            }      
        }    

        delete pBuf;  
        pBuf = NULL;
    }       
}

void AppFun_GetAppPath(CString &strPathName)
{
    TCHAR szFileName[MAX_PATH];

    if (0 != GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH - 1))
    {
        szFileName[MAX_PATH - 1] = _T( '\0');
    }

    TCHAR * p = _tcsrchr(szFileName, _T('\\')) + 1;

    if (p)
    {
        *p = _T('\0');

        strPathName = szFileName;
    }
}

BSTR CRPConfigCtrl::RP_GetVersion(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strResult;

    CString strPath;
    AppFun_GetAppPath(strPath);
    strPath += _T("RPConfig.ocx");
    CString strVer;
    GetFileVersion(strPath, strVer);
    strResult = strVer;

    return strResult.AllocSysString();
}

BOOL CRPConfigCtrl::IsInvokeAllowed(DISPID /*dispid*/)
{
    // TODO: 在此添加专用代码和/或调用基类

    return TRUE;
}

int CRPConfigCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (COleControl::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    return 0;
}

BSTR CRPConfigCtrl::SetDevNTPConfig(const FUN_PARAM & funParam)
{
    CString strResult;//返回的结果字符串
    USES_CONVERSION;//不要写在循环体呢
    //登入设备
    NET_DVR_DEVICEINFO_V30 devInfoV30;//设备信息
    memset(&devInfoV30, 0, sizeof(NET_DVR_DEVICEINFO_V30));
    LONG lLoginID =  NET_DVR_Login_V30(T2A(funParam.sDevIP), 
        funParam.nDevPort,
        T2A(funParam.sDevUser), 
        T2A(funParam.sDevPWD), &devInfoV30);
    if (-1 == lLoginID)
    {
        int err = NET_DVR_GetLastError();
		RPC_ERROR(_T("NET_DVR_Login_V30 falid, error = %d"), err);
        CString strMsg;
        strMsg.Format(_T("登录失败。错误码:%ld"), err);
        OutputDebugString(strMsg);
		strResult.Format(_T("登录设备失败!"));
		return strResult.AllocSysString();
    }

    NET_DVR_NTPPARA nNtpConfigInfo;
    memset(&nNtpConfigInfo, 0, sizeof(NET_DVR_NTPPARA));
    strcpy((char*)nNtpConfigInfo.sNTPServer, (char*)funParam.nNtpConfigInfo.sNTPServer);
    nNtpConfigInfo.wNtpPort = funParam.nNtpConfigInfo.wNtpPort;
    nNtpConfigInfo.wInterval = funParam.nNtpConfigInfo.wInterval ;
    nNtpConfigInfo.byEnableNTP = funParam.nNtpConfigInfo.byEnableNTP;
    nNtpConfigInfo.cTimeDifferenceH += 8;    //时区+8否则时区改变，不是北京时间
    if(!NET_DVR_SetDVRConfig(lLoginID, NET_DVR_SET_NTPCFG, 0xFFFFFFFF, &nNtpConfigInfo, sizeof(NET_DVR_NTPPARA)))
    {
        //登出设备
        int err = NET_DVR_GetLastError();
        RPC_ERROR(_T("NET_DVR_GetDVRConfig falid, error = %d"), err);
        NET_DVR_Logout_V30(lLoginID);
        CString strMsg;
        strMsg.Format(_T("设置NTP校时失败。错误码:%ld"), err);
        OutputDebugString(strMsg);
        strResult.Format(_T("设置NTP校时失败!"));
        return strResult.AllocSysString();
    }
    RPC_ERROR(_T("设置NTP校时成功"));
    strResult = _T("1");
    //登出设备
    NET_DVR_Logout_V30(lLoginID);

    return strResult.AllocSysString();
}
