//lint -library
#include "StdAfx.h"
#include "PreviewS20API.h"
#include "PreviewMainDlg.h"
#include "PreviewS20Dll.h"
#include "InitOper.h"
#include <sstream>

// 初始化标志位,只初始化、释放一次,避免内存映射问题多次初始化和释放造成软件崩溃.
static int g_nInitCount = 0;

CInitOper *g_pInitOper = NULL;
CPreviewMainDlg *g_pPreviewMainDlg = NULL;
ULONG_PTR m_gdiplusToken;
CString g_strInstancePath;              // DLL程序路径
CPreviewMainDlg *g_pMainDlg[MAX_DLG_ARRAY] = {0};
BOOL LoadSkinRes(int nSkinType)
{
    theApp.m_nUIStyle = nSkinType;
    CString strModulePath;
    if (theApp.m_hResInstance == NULL)
    {
        BOOL bRet = FALSE;
        DWORD dwError = 0;
        HINSTANCE hInstance = NULL;

        switch (nSkinType)
        {
        case PV_SKIN_STYLE_RED:
            {
                strModulePath = theApp.m_strModulePath + _T("PVSkin_Red.dll");
                hInstance = LoadLibraryEx(strModulePath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
                dwError = GetLastError();
                break;
            }
        case PV_SKIN_STYLE_BLUE:
            {
                strModulePath = theApp.m_strModulePath + _T("PVSkin_Blue.dll");
                hInstance = LoadLibraryEx(strModulePath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
                dwError = GetLastError();
                break;
            }
        default:
            break;
        }

        if (hInstance == NULL)
        {
            CString strError;
            strError.Format(_T("...Load SkinRes[%d] failed: %u..."), nSkinType, dwError);
            PV_ERROR("%s", CStringW2A(strError));
            OutputDebugString(strError);
            strError.Format(_T("...Load Path: %s..."), strModulePath);
            PV_ERROR("%s", CStringW2A(strError));
            OutputDebugString(strError);
            return FALSE;
        }

        theApp.m_hResInstance = hInstance;
    }
    else
    {
        PV_TRACE("...SkinRes has loaded...");
        OutputDebugString(_T("...SkinRes has loaded..."));
    }

    return TRUE;
}

/*
 *  @fn         Checklegality
 *  @brief      检查预览窗口句柄合法性
 *  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
 *  @param[OUT] nIndex: 合法：预览窗口句柄所在句柄池索引，非法：-1
 *  @return     TRUE: 成功 FALSE: 失败
 */
BOOL Checklegality(HANDLE hPreview, int &nIndex)
{
    nIndex = -1;
    if (hPreview == NULL)
    {
        return FALSE;
    }
    for (int i = 0; i < MAX_DLG_ARRAY; i++)
    {
        if (g_pMainDlg[i] != NULL && hPreview == g_pMainDlg[i])
        {
            nIndex = i;
            break;
        }
    }
    if (nIndex == -1)
    {
        return FALSE;
    }
    return TRUE;
}

/*
*  @fn         PV_Init
*  @brief      初始化dll
*  @param[IN]  emSkinStyle: 皮肤风格
*  @return     TRUE: 成功 FALSE: 失败
*/
PVS20_API BOOL PV_Init(const CString& strSavePath, SKIN_STYLE emSkinStyle/* = PV_SKIN_STYLE_BLUE*/)
{
	PV_INFO("PV_Init start");
	g_strInstancePath = AppFun_GetAppPath();

    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (g_nInitCount <= 0)
    {
        GdiplusStartupInput gdiplusStartupInput;
        Status st = GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
        ASSERT(st == Gdiplus::Ok);

        if (!LoadSkinRes(emSkinStyle))
        {
			PV_ERROR("PV_Init LoadSkinRes falid!");
            return FALSE;
        }

        if (g_pInitOper == NULL)
        {
            g_pInitOper = new CInitOper;

            if (g_pInitOper == NULL)
            {
                return FALSE;
            }

            if (!g_pInitOper->Init())
            {
				PV_ERROR("PV_Init Init falid!");
                return FALSE;
            }
        }
    }
    g_nInitCount++;
	PV_INFO("PV_Init end");
    return TRUE;
}

/*
*  @fn         PV_UnInit
*  @brief      反初始化dll
*  @return     void
*/
PVS20_API void PV_UnInit(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	PV_INFO("PV_UnInit start");

    g_nInitCount--;
    if (g_nInitCount > 0)
    {
        return;
    }
    if (g_pInitOper != NULL && g_pInitOper->IsInited())
    {
        g_pInitOper->UnInit();

        delete g_pInitOper;
        g_pInitOper = NULL;
    }
    GdiplusShutdown(m_gdiplusToken);

	PV_INFO("PV_UnInit end");
}

/*
*  @fn         PV_Create
*  @brief      创建预览窗口
*  @param[IN]  hParentWnd: 指定预览窗口的父窗口
*  @param[IN]  stPreviewParam: 预览参数
*  @return     成功：返回预览窗口句柄，失败：返回NULL
*/
PVS20_API HANDLE PV_Create(HWND hParentWnd, const PREVIEW_PARAM &stPreviewParam)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	PV_INFO("PV_Create start");

    if (hParentWnd == NULL)
    {
        return NULL;
    }

    if (g_pInitOper == NULL)
    {
        return NULL;
    }

    if (!g_pInitOper->IsInited())
    {
        return NULL;
    }
    theApp.m_stPreviewParam = stPreviewParam;
    theApp.m_pParentWnd = (CWnd*)CWnd::FromHandle(hParentWnd);
	theApp.m_nPreviewModel = stPreviewParam.nPreviewModel;
	theApp.m_nBtnRights = stPreviewParam.lBtnRights;
    g_pPreviewMainDlg = ::new(nothrow) CPreviewMainDlg;
    if (g_pPreviewMainDlg == NULL)
    {
        return NULL;
    }
    g_pPreviewMainDlg->Create(CPreviewMainDlg::IDD, theApp.m_pParentWnd);
    g_pPreviewMainDlg->ShowWindow(SW_SHOW);
    g_pPreviewMainDlg->SetPreviewLayoutMode(stPreviewParam.nDefaultLayout);

	PV_INFO("PV_Create end");

    return g_pPreviewMainDlg;
}

/*
*  @fn         PV_Destroy
*  @brief      销毁预览窗口
*  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
*  @return     TRUE: 成功 FALSE: 失败
*/
PVS20_API BOOL PV_Destroy(HANDLE hPrevew)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	PV_INFO("PV_Destroy start");

    if (hPrevew == NULL)
    {

        return FALSE;
    }

    if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
    {

        return FALSE;
    }

    if (g_pPreviewMainDlg == NULL)
    {

        return FALSE;
    }
    g_pPreviewMainDlg->DestroyWindow();
    ::delete g_pPreviewMainDlg;
    g_pPreviewMainDlg = NULL;

	PV_INFO("PV_Destroy end");

    return TRUE;
}

/*
*  @fn         PV_MoveWindow
*  @brief      设置预览窗口位置
*  @param[IN]  hPrevew: PV_Create成功返回的预览窗口句柄
*  @param[IN]  rcWnd: 窗口位置
*  @return     TRUE: 成功 FALSE: 失败
*/
PVS20_API BOOL PV_MoveWindow(HANDLE hPrevew, CRect rcWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	PV_INFO("PV_MoveWindow start");

    if (hPrevew == NULL)
    {
        return FALSE;
    }

    if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
    {
        return FALSE;
    }

    if (g_pPreviewMainDlg == NULL)
    {
        return FALSE;
    }

    g_pPreviewMainDlg->MoveWindow(rcWnd);

	PV_INFO("PV_MoveWindow start");

    return TRUE;
}

//开始预览
PVS20_API BOOL PV_StartPreview(HANDLE hPrevew, const PREVIEW_CAMINFO &struPreviewInfo)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (hPrevew == NULL)
    {
        return FALSE;
    }

    if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
    {
        return FALSE;
    }

    if (g_pPreviewMainDlg == NULL)
    {
        return FALSE;
    }

    return g_pPreviewMainDlg->StartPreview(struPreviewInfo);
}

//停止预览
PVS20_API BOOL PV_StopPreview(HANDLE hPrevew)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (hPrevew == NULL)
    {
        return FALSE;
    }

    if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
    {
        return FALSE;
    }

    if (g_pPreviewMainDlg == NULL)
    {
        return FALSE;
    }

    return g_pPreviewMainDlg->StopPreview();
}

//开始录像
PVS20_API BOOL PV_StartRecord(HANDLE hPrevew)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (hPrevew == NULL)
	{
		return FALSE;
	}

	if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
	{
		return FALSE;
	}

	if (g_pPreviewMainDlg == NULL)
	{
		return FALSE;
	}

	return g_pPreviewMainDlg->StartRecord();
}

//停止录像
PVS20_API BOOL PV_StopRecord(HANDLE hPrevew)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (hPrevew == NULL)
	{
		return FALSE;
	}

	if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
	{
		return FALSE;
	}

	if (g_pPreviewMainDlg == NULL)
	{
		return FALSE;
	}

	return g_pPreviewMainDlg->StopRecord();
}

PVS20_API BOOL PV_SetPvEventFunc(HANDLE hPrevew, PV_EVENT_FUNC Func)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return  g_pPreviewMainDlg->SetEventFunc(Func);	
}

PVS20_API BOOL PV_GetRecordFile(HANDLE hPrevew, RECOED_FILE_INFO& recordFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return  g_pPreviewMainDlg->GetRecordFile(recordFile);	
}

//设置抓图和录像配置信息
PVS20_API BOOL PV_SetCfgData(HANDLE hPrevew, const CString &strCfgInfo)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    SetCurrentDirectory(g_strInstancePath);

    if (hPrevew == NULL)
    {
        return FALSE;
    }

    if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
    {
        return FALSE;
    }

	CMarkup xml;
	if (!xml.SetDoc(strCfgInfo))
	{
		return FALSE;
	}
	if (!xml.FindElem(_T("CfgData")))
	{
		return FALSE;
	}
	xml.IntoElem();
	if (xml.FindElem(_T("PicPath")))
	{
		CString csPath = xml.GetData();
		if (0 == csPath.GetLength())
		{
			csPath = _T(".\\RecordFiles\\PV_Capture\\");
		}
		wcscpy_s(theApp.m_stLocalCfg.stSnapInfo.szCappicPath, _countof(theApp.m_stLocalCfg.stSnapInfo.szCappicPath), csPath);
	}

	if (xml.FindElem(_T("RecPath")))
	{
		CString csPath = xml.GetData();
		if (0 == csPath.GetLength())
		{
			csPath = _T(".\\RecordFiles\\PV_Record\\");
		}
		wcscpy_s(theApp.m_stLocalCfg.stRecordInfo.szRecordPath, _countof(theApp.m_stLocalCfg.stRecordInfo.szRecordPath), csPath);
	}

	if (xml.FindElem(_T("MaxRecSize")))
	{
		theApp.m_stLocalCfg.stRecordInfo.nRecordFileSize = _wtoi(xml.GetData());
	}

	if (xml.FindElem(_T("PicType")))
	{
		theApp.m_stLocalCfg.stSnapInfo.nCappicMode = _wtoi(xml.GetData());
	}

	return TRUE;
}

//设置OSD信息
PVS20_API BOOL PV_SetShowString( HANDLE hPrevew, const CString &strBussInfo )
{
	//strBussInfo格式如下
	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//<OSDNum>4</OSDNum>
	//<BussInfo>
	//  <Node>案件号:10215410</Node>
	//  <Node>业务流水号：123123</Node>
	//  <Node>业务类型：结算类</Node>
	//  <Node>客户姓名：张三</Node>
	//  <Node>客户证件号：9527</Node>
	//  <Node>录单区域杭州支行</Node>
	//  <Node>业务员工号：001</Node>
	//  <Node>业务时间：2015-07-24 19:25:36</Node>
	//</BussInfo>
	if (hPrevew == NULL)
	{
		return FALSE;
	}

	if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
	{
		return FALSE;
	}

	if (g_pPreviewMainDlg == NULL)
	{
		return FALSE;
	}

	return g_pPreviewMainDlg->SetShowString(strBussInfo);
}

/*
*  @fn         PV_GetAllUSBCameraInfo
*  @brief      获取所有USB监控点信息
*  @return     strCameraInfo: USB监控点信息
*/
PVS20_API BOOL PV_GetAllUSBCameraInfo(CString& strCameraInfo)
{
	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//	<param>
	//	<Camera>
	//	  <CamIndex>001</CamIndex>
	//	  <CamName>摄像头2</CamName>
	//	</Camera>
	//	<MicroPhone>
	//	  <MicIndex>002</MicIndex>
	//	  <MicName>摄像头2</MicName>
	//	</MicroPhone>
	//	...
	//	</param>
	void* pCameraDS = NULL;
	std::list<CAMERA_INFO_ST> listCamInfo;
	std::list<MICOR_INFO_ST> listMicInfo;
	std::string strCameraInfo2;
    TCHAR friendlyName[512] = {0};  //设备名称
    DWORD retBufSize;
    int nDevNum = 0;	//设备数量
    BOOL bRet = FALSE;
    LONG nPort = -1;
    theApp.m_bUSBExit = FALSE;

    //USBCamera_InitSDK();

    //bRet = USBCamera_SetFilter(USBCAMERA_ALL);  //目前SDK版本存在问题，本接口返回值不做判断

    //bRet = USBCamera_GetPort(&nPort);
    //if (!bRet)
    //{
    //    PV_ERROR("USBCamera_GetPort Failed![Error:%d]", USBCamera_GetLastError());
    //    USBCamera_UninitSDK();
    //    return FALSE;
    //}
    //获取麦克风信息
    bRet = USBCamera_EnumAudioDevice(&nDevNum);
    if (!bRet)
    {
        PV_ERROR("USBCamera_EnumAudioDevice Failed![Error:%d]", USBCamera_GetLastError());
        //USBCamera_FreePort(nPort);
        //USBCamera_UninitSDK();
        return FALSE;
    }
    for (int i = 0; i < nDevNum; i++)
    {
        HANDLE hDevice = NULL;
        bRet = USBCamera_GetAudioDevice(i, &hDevice);
        if (!bRet)
        {
            PV_ERROR("USBCamera_GetAudioDevice Failed![Error:%d]", USBCamera_GetLastError());
            //USBCamera_FreePort(nPort);
            //USBCamera_UninitSDK();
            return FALSE;
        }
        bRet = USBCamera_GetDeviceFriendlyName(hDevice, friendlyName, sizeof(friendlyName), &retBufSize);
        if (bRet)
        {
            MICOR_INFO_ST microInfo;
            microInfo.uiIndex = i;
            DWORD dwLen = WideCharToMultiByte(CP_OEMCP, NULL, friendlyName, -1, NULL, 0, NULL, FALSE);
            char * psText;
            psText = new char[dwLen];
            WideCharToMultiByte(CP_OEMCP, NULL, friendlyName, -1, psText, dwLen, NULL, FALSE);
            microInfo.strName = psText;
            delete [] psText;

            listMicInfo.push_back(microInfo);
            if (!USBCamera_ReleaseDevice(hDevice))
            {
				PV_ERROR("USBCamera_ReleaseDevice Failed![Error:%d]", USBCamera_GetLastError());
            }
        }
        else
        {
            PV_ERROR("USBCamera_GetDeviceFriendlyName Failed![Error:%d]", USBCamera_GetLastError());
			if (!USBCamera_ReleaseDevice(hDevice))
			{
				PV_ERROR("USBCamera_ReleaseDevice Failed![Error:%d]", USBCamera_GetLastError());
			}
            //USBCamera_FreePort(nPort);
            //USBCamera_UninitSDK();
            return FALSE;
        }
    }
    //获取摄像头信息
    bRet = USBCamera_EnumVideoDevice(&nDevNum);
    if (!bRet)
    {
        theApp.m_bUSBExit = TRUE;
        PV_ERROR("USBCamera_EnumVideoDevice Failed![Error:%d]", USBCamera_GetLastError());
        //USBCamera_FreePort(nPort);
        //USBCamera_UninitSDK();
        return FALSE;
    }
    for (int i = 0; i < nDevNum; i++)
    {
        HANDLE hDevice = NULL;
        bRet = USBCamera_GetVideoDevice(i, &hDevice);
        if (!bRet)
        {
            PV_ERROR("USBCamera_GetVideoDevice Failed![Error:%d]", USBCamera_GetLastError());
            //USBCamera_FreePort(nPort);
            //USBCamera_ReleaseDevice(hDevice);
           // USBCamera_UninitSDK();
            return FALSE;
        }
        bRet = USBCamera_GetDeviceFriendlyName(hDevice, friendlyName, sizeof(friendlyName), &retBufSize);
        if (bRet)
        {
            CAMERA_INFO_ST camInfo;
            camInfo.uiIndex = i;
            DWORD dwLen = WideCharToMultiByte(CP_OEMCP, NULL, friendlyName, -1, NULL, 0, NULL, FALSE);
            char *psText;
            psText = new char[dwLen];
            WideCharToMultiByte(CP_OEMCP, NULL, friendlyName, -1, psText, dwLen, NULL, FALSE);
            camInfo.strName = psText;
            delete [] psText;
            listCamInfo.push_back(camInfo);
			if (!USBCamera_ReleaseDevice(hDevice))
			{
				PV_ERROR("USBCamera_ReleaseDevice Failed![Error:%d]", USBCamera_GetLastError());
			}
        }
        else
        {
            PV_ERROR("USBCamera_GetDeviceFriendlyName Failed![Error:%d]", USBCamera_GetLastError());
			if (!USBCamera_ReleaseDevice(hDevice))
			{
				PV_ERROR("USBCamera_ReleaseDevice Failed![Error:%d]", USBCamera_GetLastError());
			}
            //USBCamera_FreePort(nPort);
            //USBCamera_UninitSDK();
            return FALSE;
        }
    }

    //USBCamera_UninitSDK();
/*
	if (!CameraDSAPI_Init(&pCameraDS)<0)
	{
		PV_ERROR("CameraDSAPI_Init falid!");
		return FALSE;
	}
	if (!CameraDSAPI_GetAllCameraInfo(listCamInfo, listMicInfo)<0)
	{
		PV_ERROR("CameraDSAPI_GetAllCameraInfo falid!");
		return FALSE;
	}
	if (!CameraDSAPI_Fini(&pCameraDS)<0)
	{
		PV_ERROR("CameraDSAPI_Fini falid!");
		return FALSE;
	}
*/
	strCameraInfo2 = "";
	strCameraInfo2 += "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param>";
	for (std::list<CAMERA_INFO_ST>::iterator itCam = listCamInfo.begin(); itCam != listCamInfo.end(); itCam++)
	{
		CAMERA_INFO_ST stCamInfo = *itCam;
		std::string strParamBegin = "<Camera>";

		std::string strIndex;
		std::string strTemp;
		std::stringstream ss;
		ss<<stCamInfo.uiIndex;
		ss>>strTemp;

		strIndex = "<CamIndex>" + strTemp + "</CamIndex>";
		strParamBegin += strIndex;

		std::string strCamName;
		strCamName = "<CamName>" + stCamInfo.strName + "</CamName>";
		strParamBegin += strCamName;

		strParamBegin += "</Camera>";
		strCameraInfo2 += strParamBegin;
	}
	for (std::list<MICOR_INFO_ST>::iterator itMic = listMicInfo.begin(); itMic != listMicInfo.end(); itMic++)
	{

		MICOR_INFO_ST stMicInfo = *itMic;
		std::string strParamBegin = "<MicroPhone>";

		std::string strIndex;
		std::string strTemp;
		std::stringstream ss;
		ss<<stMicInfo.uiIndex;
		ss>>strTemp;

		strIndex = "<MicIndex>" + strTemp + "</MicIndex>";
		strParamBegin += strIndex;

		std::string strMicName;
		strMicName = "<MicName>" + stMicInfo.strName + "</MicName>";
		strParamBegin += strMicName;

		strParamBegin += "</MicroPhone>";
		strCameraInfo2 += strParamBegin;
	}
	strCameraInfo2 += "</Param>";
	strCameraInfo = strCameraInfo2.c_str();
	return TRUE;
}

PVS20_API BOOL PV_OpenSound( HANDLE hPrevew)
{
	if (hPrevew == NULL)
	{
		return FALSE;
	}

	if (g_pInitOper == NULL || g_pInitOper->IsInited() == FALSE)
	{
		return FALSE;
	}

	if (g_pPreviewMainDlg == NULL)
	{
		return FALSE;
	}

	return g_pPreviewMainDlg->OpenSound();
}
