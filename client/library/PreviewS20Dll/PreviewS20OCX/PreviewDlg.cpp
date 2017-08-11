//lint -library
//lint -e628
#include "stdafx.h"
#include "PreviewS20OCX.h"
#include "PreviewDlg.h"
#include "PreviewS20OCXCtrl.h"

void PvEventFunc(PV_EVENT_TYPE pvEvent, void* lpData)
{
	switch (pvEvent)
	{
	case PV_RECORD_START:
		{
			g_pCtrl->NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
		}
		break;
	case PV_RECORD_STOP:
		{
			g_pCtrl->NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
		}
		break;
	}
}

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent)
	, m_hPreview(NULL)
{

}

CPreviewDlg::CPreviewDlg(PREV_INIT &stPrevInit, CWnd* pParent /*=NULL*/)
: CDialog(CPreviewDlg::IDD, pParent)
, m_hPreview(NULL)
{
	m_stPrevInit = stPrevInit;
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CPreviewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bRet = FALSE;
	CString strSavePath;
	bRet = PV_Init(strSavePath, (SKIN_STYLE)m_stPrevInit.lSkinType);
	if (!bRet)
	{
		AfxMessageBox(_T("预览控件初始化失败,请重新加载控件！"));
		return -2;
	}

	PREVIEW_PARAM stPreviewParam;
	stPreviewParam.nOperRight = m_stPrevInit.lPrevRights;
	stPreviewParam.nHoverToolBarRight = m_stPrevInit.lHoverToolBarRight;
	stPreviewParam.nToolBarRight = m_stPrevInit.lToolBarRight;
	stPreviewParam.nUserLevel = m_stPrevInit.lUserLevel;
	stPreviewParam.nPreviewModel = m_stPrevInit.lPreviewMode;
	stPreviewParam.lBtnRights = m_stPrevInit.lBtnRights;
	m_hPreview = PV_Create(this->GetSafeHwnd(), stPreviewParam);
	if (m_hPreview == NULL)
	{
		AfxMessageBox(_T("预览控件创建窗体失败，请重新加载控件！"));
		return -3;
	}

	// 设置录像结束回调函数
	PV_SetPvEventFunc(m_hPreview, PvEventFunc);

	return 0;
}

void CPreviewDlg::OnDestroy()
{
	if (m_hPreview != NULL)
	{
		PV_Destroy(m_hPreview);
		PV_UnInit();
		m_hPreview = NULL;
	}

	CDialog::OnDestroy();
}

void CPreviewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetClientRect(&rcWnd);

	if (m_hPreview != NULL)
	{
		PV_MoveWindow(m_hPreview, rcWnd);
	}
}

//开始预览
BOOL CPreviewDlg::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
	if (m_hPreview != NULL)
	{
		return PV_StartPreview(m_hPreview, struPreviewInfo);
	}
	return FALSE;
}

//停止预览
BOOL CPreviewDlg::StopPreview()
{
	if (m_hPreview != NULL)
	{
		return PV_StopPreview(m_hPreview);
	}
	return FALSE;
}

//开始录像
BOOL CPreviewDlg::StartRecord(LONG lWndNum)
{
	if (m_hPreview != NULL)
	{
		return PV_StartRecord(m_hPreview);
	}
	return FALSE;
}

//停止录像
BOOL CPreviewDlg::StopRecord(LONG lWndNum, RECOED_FILE_INFO& recoedFile)
{
	if (m_hPreview != NULL)
	{
		if (PV_StopRecord(m_hPreview))
		{
			return PV_GetRecordFile(m_hPreview, recoedFile);
		}
	}
	return FALSE;
}

//设置OSD信息
BOOL CPreviewDlg::SetShowString( const CString& strBussInfo )
{
	if (m_hPreview != NULL)
	{
		return PV_SetShowString(m_hPreview, strBussInfo);
	}
	return FALSE;
}

//设置配置信息
BOOL CPreviewDlg::SetConfigData( const CString& strCfgInfo )
{
	if (m_hPreview != NULL)
	{
		return PV_SetCfgData(m_hPreview, strCfgInfo);
	}
	return FALSE;
}

//设置可信任站点
BOOL CPreviewDlg::SetTrustedSite( const CString& strTrustedSite )
{
	HKEY hkResult;
	int rcRang = RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges"), &hkResult);
	RegCloseKey(hkResult);
	hkResult = NULL;

	if (rcRang != ERROR_SUCCESS)
	{
		CString strMsg;
		strMsg.Format(_T("RegCreateKey Ranges falid!"));
		OutputDebugString(strMsg);
		return FALSE;
	}

	int rcRangFMS = RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\RangeFMS"), &hkResult);
	if (rcRangFMS != ERROR_SUCCESS)
	{
		CString strMsg;
		strMsg.Format(_T("RegCreateKey RangeFMS falid!"));
		OutputDebugString(strMsg);
		return FALSE;
	}
	//必须要先创建DWORD值，再创建字符串值，否则不能在Internet选项中信任站点列表中显示
	wchar_t *wsUrl = strTrustedSite.AllocSysString();
	DWORD dwType = 2;	//2--受信任站点  4--受限制站点
	if (ERROR_SUCCESS != RegSetValueEx(hkResult, _T("http"), NULL, REG_DWORD, (BYTE *)&dwType, sizeof(DWORD)))
	{
		CString strMsg;
		strMsg.Format(_T("RegSetValueEx http falid!"));
		OutputDebugString(strMsg);
        SysFreeString((BSTR)wsUrl);// 用完释放
		return FALSE;
	}

	if (RegSetValueEx(hkResult, _T(":Range"), NULL, REG_SZ, (LPBYTE)wsUrl, (DWORD) (lstrlen(wsUrl)+1)*sizeof(TCHAR)))
	{
		CString strMsg;
		strMsg.Format(_T("RegSetValueEx Range falid!"));
		OutputDebugString(strMsg);
        SysFreeString((BSTR)wsUrl);// 用完释放
		return FALSE;
	}
	RegCloseKey(hkResult);
    SysFreeString((BSTR)wsUrl);// 用完释放
	hkResult = NULL;

	return TRUE;
}

//获取所有USB监控点信息
BOOL CPreviewDlg::GetAllUSBCameraInfo( CString& strCameraInfo )
{
	return PV_GetAllUSBCameraInfo(strCameraInfo);
}
//lint +e628
