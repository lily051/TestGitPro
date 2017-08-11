// PlaybackSimpleDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//
//lint -library
#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include <Iepmapi.h>
#include "PlaybackMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CPlaybackSimpleDllApp

typedef HRESULT (WINAPI *IsProtectedModeProcess)(                     
	__out BOOL     *pbResult                         
	); 
typedef HRESULT (WINAPI *ShowSaveFileDialog)(                         
	__in        HWND     hwnd,                       
	__in        LPCWSTR  lpwstrInitialFileName,      
	__in_opt    LPCWSTR  lpwstrInitialDir,           
	__in_opt    LPCWSTR  lpwstrFilter,               
	__in_opt    LPCWSTR  lpwstrDefExt,               
	__in        DWORD    dwFilterIndex,              
	__in        DWORD    dwFlags,                    
	__deref_out LPWSTR   *lppwstrDestinationFilePath,
	__out       HANDLE   *phState                    
	);  
typedef HRESULT (WINAPI *GetWriteableFolderPath)(                     
	__in        REFGUID clsidFolderID,               
	__deref_out LPWSTR   *lppwstrPath                
	);
typedef HRESULT (WINAPI *SaveFile)(                                   
								   __in HANDLE    hState,                           
								   __in LPCWSTR   lpwstrSourceFile                  
								   );    
typedef HRESULT (WINAPI *CancelSaveFile)(
	__in HANDLE    hState
	);

// ��ȡDLL����.
IsProtectedModeProcess IE_IsProtected = NULL;
ShowSaveFileDialog IE_ShowDialog = NULL;
GetWriteableFolderPath IE_GetPath = NULL;
SaveFile IE_SaveFile = NULL;
CancelSaveFile IE_Cancel = NULL;

BEGIN_MESSAGE_MAP(CPlaybackSimpleDllApp, CWinApp)
END_MESSAGE_MAP()


// CPlaybackSimpleDllApp ����

CPlaybackSimpleDllApp::CPlaybackSimpleDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_strModulePath = _T("");
	m_hResInstance = NULL;
	m_hOldInstance = NULL;
	m_hIEFrame = NULL;
	m_nSkinType = 0;
	m_dwStyle = 0;
	m_bIsIEProtected = FALSE;
	m_bBwControl = FALSE;
	m_strLocalIPAddr = _T("");
	m_nBWUserID = 0;
	m_bBwMode = FALSE;
	m_pClientVoid = NULL;
	m_pPlaybackMain = NULL;
	m_strModulePath = _T("");
	m_bInitProx = TRUE;
	m_bInitNvr = TRUE;
	m_bInitBdp = TRUE;
	m_bInitFinAudio = TRUE;
	m_lBwControl = 0;
    m_emLayoutStyle = LAYOUT_WND_4;
    ZeroMemory(&m_csSDK, sizeof(CRITICAL_SECTION));
}


// Ψһ��һ�� CPlaybackSimpleDllApp ����

CPlaybackSimpleDllApp theApp;

// CPlaybackSimpleDllApp ��ʼ��

BOOL CPlaybackSimpleDllApp::InitInstance()
{
	CWinApp::InitInstance();

    InitializeCriticalSection(&m_csSDK);

	// �����ļ�.
	m_hIEFrame = LoadLibrary(L"ieframe.dll");
	if (NULL == m_hIEFrame)
	{
		PLAYBACKDLL_ERROR(_T("fail to load ieframe.dll"));
	}
	else
	{
		IE_IsProtected= (IsProtectedModeProcess)::GetProcAddress(m_hIEFrame, "IEIsProtectedModeProcess");
		IE_ShowDialog = (ShowSaveFileDialog)::GetProcAddress(m_hIEFrame, "IEShowSaveFileDialog");
		IE_GetPath = (GetWriteableFolderPath)::GetProcAddress(m_hIEFrame, "IEGetWriteableFolderPath");
		IE_SaveFile = (SaveFile)::GetProcAddress(m_hIEFrame, "IESaveFile");
		IE_Cancel = (CancelSaveFile)::GetProcAddress(m_hIEFrame, "IECancelSaveFile");
	}
    
	return TRUE;
}

//��ʼ��������Ϣ
void CPlaybackSimpleDllApp::InitConfigInfo()
{
	// ��ȡ����IP��ַ.
	AppFun_GetLocalIPAddr(m_strLocalIPAddr);

	PLAYBACKDLL_INFO(_T("Local ip addr:%s"), m_strLocalIPAddr);

	//��ȡ��ǰ��������·��
	CString strConfigFile;
	AppFun_GetAppPath(strConfigFile);
	m_strModulePath = strConfigFile;
	strConfigFile.AppendFormat(_T("%s"), _T("localdata\\PlaybackParam.ini"));

	if (GetFileAttributes(strConfigFile) == 0xFFFFFFFF)
	{
		//�ļ�������
		_snprintf_s(m_stPlaybackParam.szDownloadPath, sizeof(m_stPlaybackParam.szDownloadPath),
			_TRUNCATE, "C:\\iVMS8120\\PB_Record\\");
		_snprintf_s(m_stPlaybackParam.szPicPath, sizeof(m_stPlaybackParam.szPicPath), 
			_TRUNCATE, "C:\\iVMS8120\\PB_Capture\\");
		m_stPlaybackParam.nPicType = 0;
		m_stPlaybackParam.nShowFileWnd = 0;
		m_stPlaybackParam.nShowWndTitle = 1;

		char * szConfigFile;
		int nLength;
		CP936_W2A(strConfigFile, strConfigFile.GetLength()+1, szConfigFile, nLength);
		MakeSureDirectoryPathExists(szConfigFile);
		CodeSet_Free(szConfigFile);

		WritePrivateProfileString(_T("PLAYBACK"), _T("strDownloadPath"), _T("C:\\iVMS8120\\PB_Record\\"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("strPicPath"), _T("C:\\iVMS8120\\PB_Capture\\"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("nPicType"), _T("0"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("nShowFileWnd"), _T("0"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("nShowTitleWnd"), _T("1"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("InitNvr"), _T("1"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("InitFinAudio"), _T("1"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("InitBdp"), _T("1"), strConfigFile);
		WritePrivateProfileString(_T("PLAYBACK"), _T("InitProxy"), _T("1"), strConfigFile);
	}
	else
	{
		//��ȡ�ļ���Ϣ
		TCHAR szDownloadPath[MAX_PATH];
		TCHAR szPicPath[MAX_PATH];
		int nPicType, nShowFileWnd, nShowWndTitle;
		GetPrivateProfileString(_T("PLAYBACK"), _T("strDownloadPath"), _T("C:\\iVMS8120\\PB_Record\\"), szDownloadPath, 255, strConfigFile);
		GetPrivateProfileString(_T("PLAYBACK"), _T("strPicPath"), _T("C:\\iVMS8120\\PB_Capture\\"), szPicPath, 255, strConfigFile);
		nPicType = GetPrivateProfileInt(_T("PLAYBACK"), _T("nPicType"), 0, strConfigFile);
		nShowFileWnd = GetPrivateProfileInt(_T("PLAYBACK"), _T("nShowFileWnd"), 0, strConfigFile);
		nShowWndTitle = GetPrivateProfileInt(_T("PLAYBACK"), _T("nShowWndTitle"), 1, strConfigFile);
		m_bInitNvr = GetPrivateProfileInt(_T("PLAYBACK"), _T("InitNvr"), 1, strConfigFile);
		m_bInitProx = GetPrivateProfileInt(_T("PLAYBACK"), _T("InitProxy"), 1, strConfigFile);
		m_bInitBdp = GetPrivateProfileInt(_T("PLAYBACK"), _T("InitBdp"), 1, strConfigFile);
		m_bInitFinAudio = GetPrivateProfileInt(_T("PLAYBACK"), _T("InitFinAudio"), 1, strConfigFile);

		WCharToMByte(szDownloadPath, m_stPlaybackParam.szDownloadPath, MAX_PATH);
		WCharToMByte(szPicPath, m_stPlaybackParam.szPicPath, MAX_PATH);
		m_stPlaybackParam.nPicType = nPicType;
		m_stPlaybackParam.nShowFileWnd = nShowFileWnd;
		m_stPlaybackParam.nShowWndTitle = nShowWndTitle;
	}

}

//��ȡ��ص�����Ȩ��
BOOL CPlaybackSimpleDllApp::IsCamDownloadRight(DWORD dwRight)
{
	if (dwRight & PBSD_RIGHT_DOWNLOAD)
	{
		return TRUE;
	}

	return FALSE;
}

// ��ȡ��ص�ץͼȨ��.
BOOL CPlaybackSimpleDllApp::IsCamCaptureRight(DWORD dwRight)
{
	if (dwRight & PBSD_RIGHT_CAPTURE)
	{
		return TRUE;
	}

	return FALSE;
}

// ��ȡ��ص����Ȩ��.
BOOL CPlaybackSimpleDllApp::IsCamSaveRight(DWORD dwRight)
{
	if (dwRight & PBSD_RIGHT_SAVE)
	{
		return TRUE;
	}

	return FALSE;
}

//��ȡ����ڲ�Ȩ��
BOOL CPlaybackSimpleDllApp::GetTabRight(int& nTabRight)
{
	nTabRight = (int)m_dwStyle;
	return TRUE;
}
int CPlaybackSimpleDllApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
    DeleteCriticalSection(&m_csSDK);
	// �ͷ���Դ.
	if(m_hResInstance != NULL)		
	{ 
		::FreeLibrary(m_hResInstance); 
	}
	// ��ԭ��Դ.
	if(m_hOldInstance != NULL)				
	{ 
		AfxSetResourceHandle(m_hOldInstance);  
	}

	return CWinApp::ExitInstance();
}

// �Ƿ�������ģ��.
BOOL CPlaybackSimpleDllApp::HasDownloadModule()
{
	if (m_dwStyle & PBSD_STYLE_DOWNLOAD)
	{
		return TRUE;
	}

	return FALSE;
}

// �Ƿ񱾵ػط�.
BOOL CPlaybackSimpleDllApp::HasLocalPlayback()
{
	if (m_dwStyle == PBSD_STYLE_LOCALPLAYBACK)
	{
		return TRUE;
	}

	return FALSE;
}


/**   @fn          GetIESafePath
 *    @brief   	   ��ȡIE�ͼ���·��.
 *    @param[in]   strFileName:�ļ���.
 *    @param[out]  strTempPath:��ʱ·��. 
 *    @param[out]  strSelectPath:�Ի���·��.
 *    @param[out]  hState:�ļ�״̬���. 
 *    @param[in]   emFileType:�ļ�����.
 *    @param[in]   emPicType:ͼƬ����,emFileTypeΪͼƬʱ��Ч.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackSimpleDllApp::GetIESafePath(const CString& strFileName, CString& strTempPath, CString& strSelectPath, HANDLE& hState, IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType)
{
	if (NULL == m_hIEFrame)
	{
		PLAYBACKDLL_ERROR(_T("m_hIEFrame is null"));
		return FALSE;
	}	

	if (NULL == IE_IsProtected  
		||  NULL == IE_ShowDialog  
		||  NULL == IE_GetPath)
	{
		PLAYBACKDLL_ERROR(_T("ie pfn is null"));
		return FALSE;
	}

	// ����Ƿ���IE����ģʽ��.
	HRESULT hr; 
	BOOL bProtectedMode = FALSE; 
	hr = IE_IsProtected ( &bProtectedMode );
	if (FAILED(hr))
	{
		PLAYBACKDLL_ERROR(_T("failed to exec IE_IsProtected"));
		return FALSE;
	}

	// IEδ����.
	if (!bProtectedMode)
	{
		PLAYBACKDLL_ERROR(_T("ie is not in protected mode"));
		return FALSE;
	}

	// ��ʾ�ļ��Ի���.
	LPWSTR pwszSelectedFilename = NULL; 
	const DWORD dwSaveFlags = 
		OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | 
		OFN_OVERWRITEPROMPT; 

	// �����û�ѡ���ȡ�ļ�·��. 
	CString strFilter = _T("");
	CString strExt = _T("");
	GetFileFilter(emFileType, emPicType, strFilter, strExt);
	hr = IE_ShowDialog ( 
		NULL, strFileName, NULL, 
		strFilter, 
		strExt, 1, dwSaveFlags, &pwszSelectedFilename, 
		&hState ); 
	
	if ( S_OK != hr ) 
	{
		PLAYBACKDLL_ERROR(_T("failed to exec IE_ShowDialog"));
		return FALSE;
	}

	LPWSTR pwszCacheDir = NULL; 
	TCHAR szTempFile[MAX_PATH] = {0}; 
	TCHAR szSelectFile[MAX_PATH] = {0};

	// ��ȡIE����Ŀ¼.
	hr = IE_GetPath ( FOLDERID_InternetCache, &pwszCacheDir ); 

	if (FAILED(hr)) 
	{
		PLAYBACKDLL_ERROR(_T("failed to exec IE_GetPath"));
		return FALSE;
	}

	// ת��·��.
	GetTempFileName ( CW2CT(pwszCacheDir), _T("bob"), 0, szTempFile ); 
	CoTaskMemFree ( pwszCacheDir );
	GetTempFileName ( CW2CT(pwszSelectedFilename), _T("bob"), 0, szSelectFile ); 
	CoTaskMemFree ( pwszSelectedFilename ); 

	strTempPath.Format(_T("%s"), szTempFile);
	strSelectPath.Format(_T("%s"), szSelectFile);
	PLAYBACKDLL_INFO(_T("IE tmeppath:%s, selectpath:%s"), strTempPath, strSelectPath);
	return TRUE;
}

/**   @fn          SaveIEFile
 *    @brief   	   ����IE�ļ�.
 *    @param[in]   strPath:�ļ�·��.
 *    @param[in]   hState: �ļ�״̬���.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackSimpleDllApp::SaveIEFile(const CString& strPath, const HANDLE& hState)
{
	if (NULL == m_hIEFrame)
	{
		PLAYBACKDLL_ERROR(_T("m_hIEFrame is null"));
		return FALSE;
	}

	if (NULL == IE_SaveFile  ||  NULL == IE_Cancel)
	{
		PLAYBACKDLL_ERROR(_T("SaveIEFile ie pfn is null"));
		return FALSE;
	}

	HRESULT hr = hr = IE_SaveFile(hState, strPath);
	if (FAILED(hr))
	{
		PLAYBACKDLL_ERROR(_T("failed to exec savefile!"));
	}
	DeleteFile(strPath);

	return TRUE;
}

/**   @fn          CancelIEFile
 *    @brief   	   ȡ������IE�ļ�.
 *    @param[in]   hState:�ļ�״̬���.
 *    @param[in]   
 *    @return      
 */
BOOL CPlaybackSimpleDllApp::CancelIEFile(const HANDLE& hState)
{
	if (NULL == m_hIEFrame)
	{
		PLAYBACKDLL_ERROR(_T("m_hIEFrame is null"));
		return FALSE;
	}

	if (NULL == IE_Cancel)
	{
		PLAYBACKDLL_ERROR(_T("CancelIEFile ie pfn is null"));
		return FALSE;
	}

	IE_Cancel(hState);

	return TRUE;
}

/**   @fn          GetFileFilter
 *    @brief   	   ��ȡ�ļ������ַ���.
 *    @param[in]   emFileType:�ļ�����.
 *    @param[in]   emPicType:ͼƬ����.
 *    @param[out]  strFileter:������.
 *    @param[out]  strExt:��չ��.
 *    @return      
 */
void CPlaybackSimpleDllApp::GetFileFilter(IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType, CString& strFilter, CString& strExt)
{
	if (SAVE_FILETYPE_PIC == emFileType)
	{
		if (PIC_TYPE_BMP == emPicType)
		{
			strFilter = _T("BMP|*.bmp|");
			strExt = _T("bmp");
		}
		else
		{
			strFilter = _T("JPEG|*.jpeg|");
			strExt = _T("jpeg");
		}
	}
	else
	{
		strFilter = _T("MP4|*.mp4|");
		strExt = _T("mp4");
	}
}

BOOL CPlaybackSimpleDllApp::InitSDK(ENUM_DEVICE_TYPE emDevType, ENUM_PLAY_TYPE emPlayType)
{
    CGuard guard(&m_csSDK);
    if (emPlayType == PB_TYPE_DVR)
    {
        if(m_setSDKInit.find((int)emDevType) != m_setSDKInit.end())
        {
            return TRUE;
        }
        else
        {
            if (PB_InitSDK(emDevType))
            {
                m_setSDKInit.insert((int)emDevType);
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        switch(emPlayType)
        {
        case PB_TYPE_CVR:
        case PB_TYPE_CLOUD:
		case PB_TYPE_KMS:
		case PB_TYPE_OBJECTCLOUD:
            {
                if (m_setSDKInit.find((int)emPlayType+FLAG_PLAYTYPE) != m_setSDKInit.end())
                {
                    return TRUE;
                }
                else
                {
                    if (PB_InitSDK(PB_DEVICE_HIKVISION, emPlayType))
                    {
                        m_setSDKInit.insert((int)emPlayType + FLAG_PLAYTYPE);
                    }
                    else
                    {
                        return FALSE;
                    }
                }
            }
            break;
        default:
            return FALSE;
        }
    }
    return TRUE;
}

void CPlaybackSimpleDllApp::UnInitSDK()
{
    CGuard guard(&m_csSDK);
    for (std::set<int>::iterator itor = m_setSDKInit.begin();
        itor != m_setSDKInit.end(); ++itor)
    {
        if (*itor >= FLAG_PLAYTYPE)
        {
            ENUM_PLAY_TYPE pbType = (ENUM_PLAY_TYPE)(*itor - FLAG_PLAYTYPE);
            PB_ReleaseSDK(PB_DEVICE_HIKVISION, pbType);
        }
        else
        {
            PB_ReleaseSDK((ENUM_DEVICE_TYPE)*itor);
        }
    }
    m_setSDKInit.clear();
}