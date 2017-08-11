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
		AfxMessageBox(_T("Ԥ���ؼ���ʼ��ʧ��,�����¼��ؿؼ���"));
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
		AfxMessageBox(_T("Ԥ���ؼ���������ʧ�ܣ������¼��ؿؼ���"));
		return -3;
	}

	// ����¼������ص�����
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

//��ʼԤ��
BOOL CPreviewDlg::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
	if (m_hPreview != NULL)
	{
		return PV_StartPreview(m_hPreview, struPreviewInfo);
	}
	return FALSE;
}

//ֹͣԤ��
BOOL CPreviewDlg::StopPreview()
{
	if (m_hPreview != NULL)
	{
		return PV_StopPreview(m_hPreview);
	}
	return FALSE;
}

//��ʼ¼��
BOOL CPreviewDlg::StartRecord(LONG lWndNum)
{
	if (m_hPreview != NULL)
	{
		return PV_StartRecord(m_hPreview);
	}
	return FALSE;
}

//ֹͣ¼��
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

//����OSD��Ϣ
BOOL CPreviewDlg::SetShowString( const CString& strBussInfo )
{
	if (m_hPreview != NULL)
	{
		return PV_SetShowString(m_hPreview, strBussInfo);
	}
	return FALSE;
}

//����������Ϣ
BOOL CPreviewDlg::SetConfigData( const CString& strCfgInfo )
{
	if (m_hPreview != NULL)
	{
		return PV_SetCfgData(m_hPreview, strCfgInfo);
	}
	return FALSE;
}

//���ÿ�����վ��
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
	//����Ҫ�ȴ���DWORDֵ���ٴ����ַ���ֵ����������Internetѡ��������վ���б�����ʾ
	wchar_t *wsUrl = strTrustedSite.AllocSysString();
	DWORD dwType = 2;	//2--������վ��  4--������վ��
	if (ERROR_SUCCESS != RegSetValueEx(hkResult, _T("http"), NULL, REG_DWORD, (BYTE *)&dwType, sizeof(DWORD)))
	{
		CString strMsg;
		strMsg.Format(_T("RegSetValueEx http falid!"));
		OutputDebugString(strMsg);
        SysFreeString((BSTR)wsUrl);// �����ͷ�
		return FALSE;
	}

	if (RegSetValueEx(hkResult, _T(":Range"), NULL, REG_SZ, (LPBYTE)wsUrl, (DWORD) (lstrlen(wsUrl)+1)*sizeof(TCHAR)))
	{
		CString strMsg;
		strMsg.Format(_T("RegSetValueEx Range falid!"));
		OutputDebugString(strMsg);
        SysFreeString((BSTR)wsUrl);// �����ͷ�
		return FALSE;
	}
	RegCloseKey(hkResult);
    SysFreeString((BSTR)wsUrl);// �����ͷ�
	hkResult = NULL;

	return TRUE;
}

//��ȡ����USB��ص���Ϣ
BOOL CPreviewDlg::GetAllUSBCameraInfo( CString& strCameraInfo )
{
	return PV_GetAllUSBCameraInfo(strCameraInfo);
}
//lint +e628
