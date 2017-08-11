// PreviewCtrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RecTool.h"
#include "RecToolDlg.h"
#include "PreviewCtrlDlg.h"

// const TCHAR* CLIENT_CONFIG_FILENAME = _T("FMSClient.ini");
// const TCHAR* MODULE_CONFIG_PATH  = _T("Config\\ModuleConfig.xml");      // ģ�������ļ�λ��
// const TCHAR* FUI_RES_PASSWORD = _T("Hik_Financial_8100");               // Fui��Դ����

void PvEventFunc(PV_EVENT_TYPE pvEvent, void* lpData)
{
	switch (pvEvent)
	{
	case PV_RECORD_START:
		{
			//theApp.NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
			if (NULL != g_pRecMgrDlg)
			{
				g_pRecMgrDlg->NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
			}
		}
		break;
	case PV_RECORD_STOP:
		{
			//theApp.NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
			g_pRecMgrDlg->NotifyRecordFileInfo(*(RECOED_FILE_INFO*)lpData);
		}
		break;
	}
}

// CPreviewCtrlDlg �Ի���

IMPLEMENT_DYNAMIC(CPreviewCtrlDlg, CDialog)

CPreviewCtrlDlg::CPreviewCtrlDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPreviewCtrlDlg::IDD, pParent)
,m_hPreview(NULL)
{
	PV_Init(_T("C:"));

}

CPreviewCtrlDlg::~CPreviewCtrlDlg()
{
	//PV_UnInit();
}

void CPreviewCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewCtrlDlg, CDialog)

	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPreviewCtrlDlg ��Ϣ�������

BOOL CPreviewCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();

	MoveCtrl();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPreviewCtrlDlg::InitCtrl(void)
{
	PREVIEW_PARAM stPVParam;
	stPVParam.nHoverToolBarRight = PV_HOVER_TOOLBAR_STOP | PV_HOVER_TOOLBAR_AUDIO | PV_HOVER_TOOLBAR_RECORD | PV_HOVER_TOOLBAR_CAPTURE; 
	stPVParam.nDefaultLayout = 0;
	stPVParam.nToolBarRight = PV_TOOLBAR_FULLSCREEN | PV_HOVER_TOOLBAR_STOP;
	m_hPreview = PV_Create(this->GetSafeHwnd(), stPVParam);
	if (m_hPreview == NULL)
	{
		return;
	}	

	//�����ļ�·��
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(_T("FMSClient.ini"));

	TCHAR szCapPath[MAX_PATH] = {0};
	GetPrivateProfileString(_T("LocalConfig"), _T("CapPath"), theApp.m_strAppCapPath, szCapPath, MAX_PATH, strConfigPath);

	CString strRecPath = theApp.m_strAppCapPath + _T("PV_Capture\\");
	CString strPicPath = theApp.m_strAppCapPath + _T("PV_Record\\");
	CString strCfgData;
	strCfgData.Format(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
						 <CfgData>\
						 <PicPath>%s</PicPath>\
						 <RecPath>%s</RecPath>\
						 <MaxRecSize>8</MaxRecSize>\
						 <PicType>1</PicType>\
						 </CfgData>"), strRecPath, strPicPath);
	PV_SetCfgData(m_hPreview, strCfgData);

	CString USBCamInfo;
	PV_GetAllUSBCameraInfo(USBCamInfo);

	// ����¼������ص�����
	PV_SetPvEventFunc(m_hPreview, PvEventFunc);
}

void CPreviewCtrlDlg::MoveCtrl(void)
{
	CRect rcRect(0, 0, 468, 318);
	CRect rcClient(rcRect);
	MoveWindow(rcClient);
	PV_MoveWindow(m_hPreview, rcClient);
}

void CPreviewCtrlDlg::UnInitCtrl(void)
{
	if (m_hPreview != NULL)
	{
		if (PV_Destroy(m_hPreview))
		{
			m_hPreview = NULL;
		}
	}
}

void CPreviewCtrlDlg::OnDestroy()
{
	CDialog::OnDestroy();
	//FMSCLIENT_INFO("Before CPreviewCtrlDlg::OnDestroy");

	UnInitCtrl();

	PV_UnInit();

	//FMSCLIENT_INFO("After CPreviewCtrlDlg::OnDestroy");
}

BOOL CPreviewCtrlDlg::StartPreview(PREVIEW_CAMINFO& stCamInfo)
{
	if (NULL == m_hPreview)
	{
		return FALSE;
	}

	//�����ļ�·��
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(_T("FMSClient.ini"));

	BOOL bRet = PV_StartPreview(m_hPreview, stCamInfo);
	if (bRet)
	{
		// 		//Ԥ���ɹ���Ĭ�ϴ�����
		// 		int nPreviewAudio = GetPrivateProfileInt(_T("LocalConfig"), _T("PreviewAudio"), 0, strConfigPath);
		// 		if (1 == nPreviewAudio)
		// 		{
		// 			Sleep(3000);	//���������ӳ٣���ֹδ����ʱ���������ӿ�ʧ������
		// 			PV_OpenSound(m_hPreview);
		// 		}
	}

	return TRUE;
}

BOOL CPreviewCtrlDlg::StartSound(void)
{
	//�����ļ�·��
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(_T("FMSClient.ini"));

	//Ԥ���ɹ����Ƿ�Ĭ�ϴ�����
	int nPreviewAudio = GetPrivateProfileInt(_T("LocalConfig"), _T("PreviewAudio"), 0, strConfigPath);
	if (1 == nPreviewAudio)
	{
		//Sleep(2000);	//���������ӳ٣���ֹδ����ʱ���������ӿ�ʧ������
		return PV_OpenSound(m_hPreview);
	}
	else
	{
		return TRUE;
	}

	return FALSE;
}
