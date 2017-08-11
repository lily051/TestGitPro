// PlaybackMain.cpp : ʵ���ļ�
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlaybackMain.h"
#include "Markup.h"
#include "../xmldef.h"

// CPlaybackMain �Ի���

/**   @fn          BwRegistMsgCBFunc
 *    @brief   	   ����ע��ص�����.
 *    @param[in]   bRet:�Ƿ�ע��ɹ�.
 *    @param[in]   pParam:�û��Զ�������.
 *    @return      
 */
void CALLBACK BwRegistMsgCBFunc(BOOL bRet, LPVOID pParam)
{
	CPlaybackMain* pDlg = (CPlaybackMain*)pParam;
	if (NULL == pDlg)
	{
		return;
	}

	PLAYBACKDLL_INFO(_T("BwRegistMsgCBFunc StopPlayCam"));
	if (bRet)
	{
		pDlg->StopPlayCam();
	}
}

IMPLEMENT_DYNAMIC(CPlaybackMain, CDialog)

CPlaybackMain::CPlaybackMain(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackMain::IDD, pParent)
{
	m_pDlgPlayVedio = NULL;
	m_pDownload = NULL;
	m_pDlgLocalPlayback = NULL;

	m_bMove = FALSE;
	m_emShowDlg = TAB_ITEM_PLAYBACK;
	m_pfnMenuCB = NULL;
	m_pUserData = NULL;
	m_pWorkLogUserData = NULL;
	m_pfnPlayBackCB = NULL;
	m_pPlaybackUserData = NULL;
	m_bIsLocalPlayback = FALSE;
}

CPlaybackMain::~CPlaybackMain()
{
	m_pDlgPlayVedio = NULL;
	m_pDownload = NULL;
	m_pDlgLocalPlayback = NULL;
	m_pPlaybackUserData = NULL;
	m_pUserData = NULL;
	m_pWorkLogUserData = NULL;
}

void CPlaybackMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PLAY, m_tabPlay);
}


BEGIN_MESSAGE_MAP(CPlaybackMain, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PLAY, &CPlaybackMain::OnTcnSelchangeTabPlay)
	ON_MESSAGE(MSG_TREENODE_DBCLICK, &CPlaybackMain::OnMsgCameraPlay)
	ON_MESSAGE(MSG_TREENODE_DRAG, &CPlaybackMain::OnMsgCameraDrag)
	ON_MESSAGE(MSG_RECORDFILE_DOWNLOAD, &CPlaybackMain::OnMsgFileDownload)
	ON_MESSAGE(MSG_RECORD_DOWNLOAD_NOTIFY, &CPlaybackMain::OnMsgDownloadNotify)
	ON_MESSAGE(MSG_DOWNLOADFILE_PLAY, &CPlaybackMain::OnDownloadFilePlay)
	ON_MESSAGE(MSG_BWCONTROL_STOPPLAY, &CPlaybackMain::OnMsgBWControlStopPlay)
	ON_MESSAGE(MSG_GROUPCFG_CHANGE, &CPlaybackMain::OnUpdateGroup)
	ON_MESSAGE(MSG_MENU_CALLBACK, &CPlaybackMain::OnMenuCallBack)
	ON_MESSAGE(MSG_PLAYBACK_FIND_SUCCESS, &CPlaybackMain::OnMsgFindSuccess)
    ON_MESSAGE(MSG_FILELIST_CHANGE, &CPlaybackMain::OnFileListChange)
END_MESSAGE_MAP()


// CPlaybackMain ��Ϣ�������

BOOL CPlaybackMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitTab();

	CreateChildWnd();

	//���ش���/���������� ָ�봫��طŴ�����
	if (m_pDlgPlayVedio && m_pDlgPlayVedio->m_pToolBarWnd)
	{
		m_pDlgPlayVedio->SetDownloadDlgPoint(m_pDownload);
	}

	m_bMove = TRUE;

	// ע�������Ʒ�����.
	if (theApp.m_bBwMode)
	{
		PLAYBACKDLL_INFO(_T("BwMode, Now registing"));
		PostMessage(MSG_BWCONTROL_RECONNECT, 0, 0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlaybackMain::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pDlgPlayVedio)
	{
		m_pDlgPlayVedio->DestroyWindow();
		delete m_pDlgPlayVedio;
		m_pDlgPlayVedio = NULL;
	}

	if (m_pDownload)
	{
		m_pDownload->DestroyWindow();
		delete m_pDownload;
		m_pDownload = NULL;
	}

// 	if (m_pDlgLabelMgr)
// 	{
// 		m_pDlgLabelMgr->DestroyWindow();
// 		delete m_pDlgLabelMgr;
// 		m_pDlgLabelMgr = NULL;
// 	}

// 	if (m_pDlgTreeParam)
// 	{
// 		m_pDlgTreeParam->DestroyWindow();
// 		delete m_pDlgTreeParam;
// 		m_pDlgTreeParam = NULL;
// 	}

// 	if (m_pDlgSubSerach)
// 	{
// 		m_pDlgSubSerach->DestroyWindow();
// 		delete m_pDlgSubSerach;
// 		m_pDlgSubSerach = NULL;
// 	}

// 	if (m_pDlgPlaybackCfg)
// 	{
// 		m_pDlgPlaybackCfg->DestroyWindow();
// 		delete m_pDlgPlaybackCfg;
// 		m_pDlgPlaybackCfg = NULL;
// 	}

 	if (m_pDlgLocalPlayback)
 	{
 		m_pDlgLocalPlayback->DestroyWindow();
 		delete m_pDlgLocalPlayback;
 		m_pDlgLocalPlayback = NULL;
 	}

}

void CPlaybackMain::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}
}

//�Ӵ��ڴ���
void CPlaybackMain::CreateChildWnd()
{
	if (theApp.HasLocalPlayback())
	{
		m_tabPlay.ShowWindow(SW_HIDE);
		if (m_pDlgLocalPlayback == NULL)
		{
			m_pDlgLocalPlayback = new CLocalPlayback;
			m_pDlgLocalPlayback->Create(CLocalPlayback::IDD, this);
			m_pDlgLocalPlayback->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (m_pDlgPlayVedio == NULL)
		{
			m_pDlgPlayVedio = new CPlayVedioDlg(this);
			m_pDlgPlayVedio->Create(CPlayVedioDlg::IDD, this);
			m_pDlgPlayVedio->ShowWindow(SW_SHOW);
		}

		if (m_pDownload == NULL)
		{
			m_pDownload = new CDownloadDlg(this);
			m_pDownload->Create(CDownloadDlg::IDD, this);
			m_pDownload->ShowWindow(SW_HIDE);
		}

		if (m_pDlgLocalPlayback == NULL)
		{
			m_pDlgLocalPlayback = new CLocalPlayback;
			m_pDlgLocalPlayback->Create(CLocalPlayback::IDD, this);
			m_pDlgLocalPlayback->ShowWindow(SW_HIDE);
		}
	}
}

void CPlaybackMain::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcTab(rcClient);
	rcTab.bottom = rcTab.top + 25;
	m_tabPlay.MoveWindow(rcTab);

	//����
	CRect rcDlgFull(rcClient);
	if (theApp.HasDownloadModule())
	{
		rcDlgFull.top = rcTab.bottom + 2;
	}
	CRect rcDlg(rcDlgFull);
	rcDlg.left = rcClient.left;
	
	if (m_pDlgPlayVedio)
	{
		m_pDlgPlayVedio->MoveWindow(rcDlg);
	}

	if (m_pDownload)
	{
		m_pDownload->MoveWindow(rcDlgFull);
	}

// 	if (m_pDlgLabelMgr)
// 	{
// 		m_pDlgLabelMgr->MoveWindow(rcDlg);
// 	}

// 	if (m_pDlgSubSerach)
// 	{
// 		m_pDlgSubSerach->MoveWindow(rcDlg);
// 	}

// 	if (m_pDlgPlaybackCfg)
// 	{
// 		m_pDlgPlaybackCfg->MoveWindow(rcDlg);
// 	}

 	if (m_pDlgLocalPlayback)
 	{
 	    m_pDlgLocalPlayback->MoveWindow(rcDlgFull);
 	}

}

BOOL CPlaybackMain::InitTab()
{
	int nTabRight = 0;
	if (!theApp.GetTabRight(nTabRight))
	{
		return FALSE;
	}

	PLAYBACKDLL_INFO(_T("nTabRIght=%d\n"), nTabRight);
	if (nTabRight == 255)
	{
	    m_tabPlay.InsertItem(TAB_ITEM_PLAYBACK, _T("Զ�̻ط�"));
		m_tabPlay.InsertItem(TAB_ITEM_DOWNLOAD, _T("¼������"));
		m_tabPlay.InsertItem(TAB_ITEM_LOCALPLAYBACK, _T("���ػط�"));
	}
	else if (nTabRight == 1)
	{
		m_tabPlay.InsertItem(TAB_ITEM_PLAYBACK, _T("Զ�̻ط�"));
		m_tabPlay.InsertItem(TAB_ITEM_DOWNLOAD, _T("¼������"));
	}

	if (!theApp.HasDownloadModule())
	{
		m_tabPlay.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

void CPlaybackMain::OnTcnSelchangeTabPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = m_tabPlay.GetCurSel();

	ChangeTab(nItem);

	m_emShowDlg = (MAIN_TAB_ITEM)nItem;

	*pResult = 0;
}

//�����ڼ�ص�ȷ��/˫����Ϣ
LRESULT CPlaybackMain::OnMsgCameraPlay(WPARAM wParam, LPARAM lParam)
{
#if 0
	int nNodeID = (int)wParam;
	int nItemType = (int)lParam;

	switch (m_emShowDlg)
	{
	case TAB_ITEM_PLAYBACK:
		{
			if (NODE_CAMERA == nItemType)
			{
				int nWndID = -1;
				if (m_pPlayback)
				{
					nWndID = m_pPlayback->GetSelectedWnd();
				}
				if (nWndID == -1)
				{
					return 0;
				}
				PlaybackCam(nNodeID, nWndID);
			}
			if (NODE_GROUP == nItemType)
			{
				if (m_pPlayback->m_pDlgPlayVedio && m_pDlgTreeParam)
				{
					m_pPlayback->m_pDlgPlayVedio->PlayGroup(nNodeID, m_pDlgTreeParam->m_stPlayParam);
				}
			}
		}
		break;
// 	case TAB_ITEM_DOWNLOAD:
// 		{
// 			if (NODE_CAMERA == nItemType)
// 			{
// 				if (m_pDownload)
// 				{
// 					m_pDownload->AddTimeDownloadInfo(nNodeID, m_pDlgTreeParam->m_stPlayParam);
// 				}
// 			}
// 		}
// 		break;
// 	case TAB_ITEM_LABLE:
// 		{
// 
// 		}
// 		break;
	case TAB_ITEM_SUBSEARCH:
		{
			if (NODE_CAMERA == nItemType)
			{
				if (m_pDlgSubSerach)
				{
					m_pDlgSubSerach->SubPlayback(nNodeID, m_pDlgTreeParam->m_stPlayParam);
				}
			}
		}
		break;
	default:
		break;
	}
#endif
	return 0;
}

//�����ڼ�ص���ק��Ϣ
LRESULT CPlaybackMain::OnMsgCameraDrag(WPARAM wParam, LPARAM lParam)
{
#if 0
	int nNodeID = (int)wParam;
	int nItemType = (int)lParam;

	//��ǰ����Զ�̻طŴ��ڲ���Ч
	if (TAB_ITEM_PLAYBACK == m_emShowDlg)
	{
		if (NODE_CAMERA == nItemType)
		{
			int nWndID = -1;
			if (m_pPlayback)
			{
				nWndID = m_pPlayback->GetCursorWnd();
			}
			if (nWndID == -1)
			{
				return 0;
			}

			PlaybackCam(nNodeID, nWndID);
		}
	}
#endif
	return 0;
}

//�ļ�������Ϣ
LRESULT CPlaybackMain::OnMsgFileDownload(WPARAM wParam, LPARAM lParam)
{
	/*PB_FILEINFO stFileInfo;
	if (m_pDlgPlayVedio && m_pDlgPlayVedio->m_pDlgRecordFile)
	{
		stFileInfo = m_pDlgPlayVedio->m_pDlgRecordFile->m_vecFileInfo[lParam];
	}

	if (m_pDownload)
	{
		m_pDownload->AddFileDownloadInfo((int)wParam, stFileInfo);
	}*/

	return 0;
}

//�¼�֪ͨ������Ϣ
LRESULT CPlaybackMain::OnMsgDownloadNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_pDownload)
	{
		m_pDownload->NotifyStartDownload();
	}

	return 0;
}

LRESULT CPlaybackMain::OnDownloadFilePlay(WPARAM wParam, LPARAM lParam)
{
	//m_tabPlay.SetCurSel(TAB_ITEM_LOCALPLAYBACK);
	//ChangeTab(TAB_ITEM_LOCALPLAYBACK);

	// �������ص��ļ�.
// 	if (m_pDlgLocalPlayback != NULL)
// 	{
// 		CString* pRecv = (CString*)lParam;
// 		m_pDlgLocalPlayback->PlaySpecifiedFile(*pRecv);
// 		delete pRecv;
// 		pRecv = NULL;
// 	}
	return 0;
}

/*******************************************************************************
*    Function:      ChangeTab
*    Description:   �ı���ʾ�ı�ǩҳ.
*    Input:         nItem:��ǩҳID.
*    Output:        
*    Return:	    
*******************************************************************************/
void CPlaybackMain::ChangeTab(int nItem)
{
	switch (nItem)
	{
	case TAB_ITEM_PLAYBACK:
		{
			if (m_pDlgPlayVedio)
			{
				m_pDlgPlayVedio->ShowWindow(SW_SHOW);
			}
			if (m_pDownload)
			{
				m_pDownload->ShowWindow(SW_HIDE);
			}
// 			if (m_pDlgLabelMgr)
// 			{
// 				m_pDlgLabelMgr->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgSubSerach)
// 			{
// 				m_pDlgSubSerach->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgPlaybackCfg)
// 			{
// 				m_pDlgPlaybackCfg->ShowWindow(SW_HIDE);
// 			}
 			if (m_pDlgLocalPlayback)
 			{
 				m_pDlgLocalPlayback->ShowWindow(SW_HIDE);
 			}

// 			if (m_pDlgTreeParam)
// 			{
// 				m_pDlgTreeParam->ShowWindow(SW_SHOW);
// 			}
		}
		break;
	case TAB_ITEM_DOWNLOAD:
		{
			if (m_pDlgPlayVedio)
			{
				m_pDlgPlayVedio->ShowWindow(SW_HIDE);
			}
			if (m_pDownload)
			{
				m_pDownload->ShowWindow(SW_SHOW);
			}
// 			if (m_pDlgLabelMgr)
// 			{
// 				m_pDlgLabelMgr->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgSubSerach)
// 			{
// 				m_pDlgSubSerach->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgPlaybackCfg)
// 			{
// 				m_pDlgPlaybackCfg->ShowWindow(SW_HIDE);
// 			}
 			if (m_pDlgLocalPlayback)
 			{
 				m_pDlgLocalPlayback->ShowWindow(SW_HIDE);
 			}

// 			if (m_pDlgTreeParam)
// 			{
// 				m_pDlgTreeParam->ShowWindow(SW_HIDE);
// 			}
		}
		break;
		// 	case TAB_ITEM_LABLE:
		// 		{
		// 			if (m_pPlayback)
		// 			{
		// 				m_pPlayback->ShowWindow(SW_HIDE);
		// 			}
		// 			if (m_pDownload)
		// 			{
		// 				m_pDownload->ShowWindow(SW_HIDE);
		// 			}
		// 			if (m_pDlgLabelMgr)
		// 			{
		// 				m_pDlgLabelMgr->ShowWindow(SW_SHOW);
		// 			}
		// 			if (m_pDlgSubSerach)
		// 			{
		// 				m_pDlgSubSerach->ShowWindow(SW_HIDE);
		// 			}
		// 			if (m_pDlgPlaybackCfg)
		// 			{
		// 				m_pDlgPlaybackCfg->ShowWindow(SW_HIDE);
		// 			}
		// 		}
		// 		break;
	case TAB_ITEM_SUBSEARCH:
		{
			if (m_pDlgPlayVedio)
			{
				m_pDlgPlayVedio->ShowWindow(SW_HIDE);
			}
			if (m_pDownload)
			{
				m_pDownload->ShowWindow(SW_HIDE);
			}
// 			if (m_pDlgLabelMgr)
// 			{
// 				m_pDlgLabelMgr->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgSubSerach)
// 			{
// 				m_pDlgSubSerach->ShowWindow(SW_SHOW);
// 			}
// 			if (m_pDlgPlaybackCfg)
// 			{
// 				m_pDlgPlaybackCfg->ShowWindow(SW_HIDE);
// 			}
 			if (m_pDlgLocalPlayback)
 			{
 				m_pDlgLocalPlayback->ShowWindow(SW_SHOW);
 			}

// 			if (m_pDlgTreeParam)
// 			{
// 				m_pDlgTreeParam->ShowWindow(SW_SHOW);
// 			}
		}
		break;
	case TAB_ITEM_LOCALPLAYBACK:
		{
			if (m_pDlgPlayVedio)
			{
				m_pDlgPlayVedio->ShowWindow(SW_HIDE);
			}
			if (m_pDownload)
			{
				m_pDownload->ShowWindow(SW_HIDE);
			}
// 			if (m_pDlgLabelMgr)
// 			{
// 				m_pDlgLabelMgr->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgSubSerach)
// 			{
// 				m_pDlgSubSerach->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgPlaybackCfg)
// 			{
// 				m_pDlgPlaybackCfg->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgLocalPlayback)
// 			{
// 				m_pDlgLocalPlayback->ShowWindow(SW_SHOW);
// 			}
//
// 			if (m_pDlgTreeParam)
// 			{
// 				m_pDlgTreeParam->ShowWindow(SW_HIDE);
// 			}
		}
		break;
	case TAB_ITEM_CONFIG:
		{
			if (m_pDlgPlayVedio)
			{
				m_pDlgPlayVedio->ShowWindow(SW_HIDE);
			}
			if (m_pDownload)
			{
				m_pDownload->ShowWindow(SW_HIDE);
			}
// 			if (m_pDlgLabelMgr)
// 			{
// 				m_pDlgLabelMgr->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgSubSerach)
// 			{
// 				m_pDlgSubSerach->ShowWindow(SW_HIDE);
// 			}
// 			if (m_pDlgPlaybackCfg)
// 			{
// 				m_pDlgPlaybackCfg->ShowWindow(SW_SHOW);
// 			}
// 			if (m_pDlgLocalPlayback)
// 			{
// 				m_pDlgLocalPlayback->ShowWindow(SW_HIDE);
// 			}
		}
		break;
	default:
		return;
	}
}

/** @fn			PlaybackCam
*	@brief		��ص�ط�.
*	@param		pParam:�طŲ���.
*	@param
*	@return		TRUE:�ɹ�,FALSE:ʧ��.
*/
BOOL CPlaybackMain::PlaybackCam(LPPBSDLoginParam pLoginParam, LPPBSDPlayParam pPlayParam, int nWnd, DWORD dwRight)
{
	m_bIsLocalPlayback = FALSE;
	m_tabPlay.SetCurSel(0);
	if (m_pDlgPlayVedio)
	{
		m_pDlgPlayVedio->ShowWindow(SW_SHOW);
	}
	if (m_pDownload)
	{
		m_pDownload->ShowWindow(SW_HIDE);
	}
	if (m_pDlgLocalPlayback)
	{
		m_pDlgLocalPlayback->ShowWindow(SW_HIDE);
	}
	if (NULL == m_pDlgPlayVedio ||  NULL == pLoginParam  ||  NULL == pPlayParam)
	{
		return FALSE;
	}

	// ��¼����.
	PB_LOGININFO stLoginInfo;
	stLoginInfo.emPlayType = (ENUM_PLAY_TYPE)pLoginParam->emPlayType;
	stLoginInfo.emDevType = (ENUM_DEVICE_TYPE)theApp.m_manDevice.GetDeviceType(pLoginParam->nManType, pLoginParam->nDevType);
	memcpy_s(&stLoginInfo.unLoginType, sizeof(stLoginInfo.unLoginType), &(pLoginParam->unLoginType), sizeof(pLoginParam->unLoginType));

    if (!theApp.InitSDK(stLoginInfo.emDevType, stLoginInfo.emPlayType))
    {
        PLAYBACKDLL_ERROR(_T("InitSDK failed, devtype:%d, playtype:%d"),pLoginParam->nDevType, pLoginParam->emPlayType);
        return FALSE;
    }

	// �طŷ�ʽ.
	int nType = pPlayParam->nType;
	int nCameraID = -1;
	int nChannel = -1;
	CString strCamName = _T("");
	// ��ʱ��ط�.
	if (0 == nType)
	{
		nCameraID = pPlayParam->unPlayParam.stTimePram.nCamID;
		nChannel = pPlayParam->unPlayParam.stTimePram.nChannel;
		TCHAR szCamName[128];
		MByteToWChar(pPlayParam->unPlayParam.stTimePram.szCamName, szCamName, 128);
		strCamName.Format(_T("%s"), szCamName);
		// �طŲ���.
		PLAY_PARAM stParam;
		stParam.nChannel = nChannel;
		stParam.emPlayType = (ENUM_PLAY_TYPE)pLoginParam->emPlayType;
		stParam.emRecordType = (ENUM_RECORD_TYPE)pPlayParam->unPlayParam.stTimePram.emFileType;
		memcpy_s(&stParam.startTime, sizeof(PB_TIME), &(pPlayParam->unPlayParam.stTimePram.startTime), sizeof(PBSDTime));
		memcpy_s(&stParam.stopTime, sizeof(PB_TIME), &(pPlayParam->unPlayParam.stTimePram.stopTime), sizeof(PBSDTime));
		memcpy_s(&stParam.szStreamID, 32, &(pPlayParam->unPlayParam.stTimePram.szStreamID), 32);
		if (PBSD_TYPE_CLOUD == (ENUM_PLAY_TYPE)pLoginParam->emPlayType)
		{
			strcpy_s(stLoginInfo.unLoginType.stDvrLoginInfo.szIndex, 32, pLoginParam->unLoginType.stDvrLoginInfo.szIndex);
		}

		// �ط�¼��.
		if (!m_pDlgPlayVedio->Playback(nCameraID, 
			strCamName,
			stParam, 
			stLoginInfo, 
			nWnd,
			dwRight,
			0))
		{
			return FALSE;
		}
	}
	// ���ļ��ط�.
	else if (1 == nType)
	{
		nCameraID = pPlayParam->unPlayParam.stFileParam.nCamID;
		TCHAR szCamName[128];
		MByteToWChar(pPlayParam->unPlayParam.stFileParam.szCamName, szCamName, 128);
		strCamName.Format(_T("%s"), szCamName);
		nChannel = pPlayParam->unPlayParam.stFileParam.nChannel;
		// �ļ�����.
		PB_FILEINFO stFileInfo;
		memcpy_s(stFileInfo.szFileName, sizeof(stFileInfo.szFileName), 
			pPlayParam->unPlayParam.stFileParam.stFileData.szFileName, sizeof(pPlayParam->unPlayParam.stFileParam.stFileData.szFileName));
		stFileInfo.dwFileSize = pPlayParam->unPlayParam.stFileParam.stFileData.dwFileSize;
		stFileInfo.nFileType = pPlayParam->unPlayParam.stFileParam.stFileData.nFileType;
		memcpy_s(&(stFileInfo.stStartTime), sizeof(PB_TIME), 
			&(pPlayParam->unPlayParam.stFileParam.stFileData.stStartTime), sizeof(PBSDTime));
		memcpy_s(&(stFileInfo.stEndTime), sizeof(PB_TIME),
			&(pPlayParam->unPlayParam.stFileParam.stFileData.stEndTime), sizeof(PBSDTime));
		stFileInfo.emPlayType = (ENUM_PLAY_TYPE)pLoginParam->emPlayType;
		stFileInfo.emDevType = (ENUM_DEVICE_TYPE)pLoginParam->nDevType;
        memcpy_s(stFileInfo.szPlayUrl, sizeof(stFileInfo.szPlayUrl), 
            pPlayParam->unPlayParam.stFileParam.stFileData.szPlayUrl, sizeof(pPlayParam->unPlayParam.stFileParam.stFileData.szPlayUrl));
		if (PBSD_TYPE_CLOUD == (ENUM_PLAY_TYPE)pLoginParam->emPlayType)
		{
			strcpy_s(stLoginInfo.unLoginType.stDvrLoginInfo.szIndex, 32, pLoginParam->unLoginType.stDvrLoginInfo.szIndex);
		}

		// �ط�¼��.
		if (!m_pDlgPlayVedio->PlaybackFile(nCameraID, 
			strCamName,
			nChannel, 
			pPlayParam->unPlayParam.stFileParam.szStreamID,
			stFileInfo, 
			stLoginInfo, 
			nWnd, 
			dwRight,
			0))
		{
			return FALSE;
		}

		std::vector<PB_FILEINFO> vecFileInfo;
		vecFileInfo.push_back(stFileInfo);

		if (m_pDlgPlayVedio)
		{
			m_pDlgPlayVedio->BuildRecordGraph(vecFileInfo);
		}

	}	

	return TRUE;
}

BOOL CPlaybackMain::LocalPlayback(const int nVideoType, const CString csFilePath)
{
	m_tabPlay.SetCurSel(2);
	if (m_pDlgPlayVedio)
	{
		m_pDlgPlayVedio->ShowWindow(SW_HIDE);
	}
	if (m_pDownload)
	{
		m_pDownload->ShowWindow(SW_HIDE);
	}
	if (m_pDlgLocalPlayback)
	{
		m_pDlgLocalPlayback->ShowWindow(SW_SHOW);
		if (!m_pDlgLocalPlayback->LocalPlayback(nVideoType, csFilePath))
		{
			return FALSE;
		}
		m_bIsLocalPlayback = TRUE;
	}
	return TRUE;
}

/**   @fn          StopPlayCam
 *    @brief   	   ֹͣ�طż�ص�.
 *    @param[in]   nResID:��ص�ID.
 *    @param[in]   
 *    @return      
 */
void CPlaybackMain::StopPlayCam(int nResID)
{
	if (m_pDlgPlayVedio != NULL)
	{
		m_pDlgPlayVedio->StopPlayback(nResID);
	}
}

LRESULT CPlaybackMain::OnMsgBWControlStopPlay(WPARAM wParam, LPARAM lParam)
{
	int nCamID = (int)lParam;

	StopPlayCam(nCamID);

	return 0;
}

LRESULT CPlaybackMain::OnUpdateGroup(WPARAM wParam, LPARAM lParam)
{
// 	if (m_pDlgTreeParam != NULL)
// 	{
// 		m_pDlgTreeParam->UdpatePlaybackGroup();
// 	}
	return 0;
}

/**   @fn          StopPlayByWndNo
 *    @brief   	   ָֹͣ���ĻطŴ���.
 *    @param[in]   nWndNo:���ں�.
 *    @param[in]   
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
void CPlaybackMain::StopPlayByWndNo(int nWndNo)
{
	if (m_bIsLocalPlayback)
	{
		if (m_pDlgLocalPlayback)
		{
			m_pDlgLocalPlayback->StopPlayback();
			m_bIsLocalPlayback = FALSE;
		}
	}

	if (NULL == m_pDlgPlayVedio)
	{
		return;
	}
	else
	{
		m_pDlgPlayVedio->StopPlaybackByWndNo(nWndNo);
	}
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   ��ȡ�ط�״̬��Ϣ.
 *    @param[in]   nWndNo:���ں�.
 *    @param[out]  stData:״̬��Ϣ.
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackMain::GetPlaybackInfo(int nWndNo, PlaybackStatData& stData)
{
	if (NULL == m_pDlgPlayVedio  ||  NULL == m_pDownload)
	{
		return FALSE;
	}

	// �Ȼ�ȡ��ص���Ϣ.
	if (!m_pDlgPlayVedio->GetPlaybackInfo(nWndNo, stData))
	{
		return FALSE;
	}

	// ��ȡ������Ϣ.
	if (!m_pDownload->GetPlaybackInfo(stData))
	{
		return FALSE;
	}

	return TRUE;
}

/**   @fn          StopAll
 *    @brief   	   ֹͣȫ������.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackMain::StopAll()
{
	if (NULL == m_pDlgPlayVedio)
	{
		return;
	}

	m_pDlgPlayVedio->StopAll();
}

/**   @fn          BuildFileXML
 *    @brief   	   ����XML����.
 *    @param[in]   vecFileList:�ļ��б�.
 *    @param[out]  strXML:XML�ַ���. 
 *    @return      TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPlaybackMain::BuildFileXML(int nWnd, const std::vector<PB_FILEINFO>& vecFileList, CString& strXML)
{
	if (vecFileList.empty())
	{
		return TRUE;
	}

	CMarkup xml;
	xml.SetDoc(XML_HEADER_CHN);

	xml.AddElem(XML_RESPONSE);
	xml.IntoElem();
	// ���ں�.
	xml.AddElem(XML_WNDNO, nWnd);
	// �ļ�����.
	int nFileNum = vecFileList.size();
	xml.AddElem(XML_FILENUM, nFileNum);

	for (std::vector<PB_FILEINFO>::const_iterator ite = vecFileList.begin();
		ite != vecFileList.end();
		ite++)
	{
		CString strTemp;
		xml.AddElem(XML_FILE);
		// �ļ���.
		TCHAR szFileName[128];
		MByteToWChar((*ite).szFileName, szFileName, sizeof(szFileName));
		strTemp.Format(_T("%s"), szFileName);
		xml.AddChildElem(XML_FILENAME, strTemp);
		// �ļ���С.
		xml.AddChildElem(XML_FILESIZE, (*ite).dwFileSize);
		// �ļ�����.
		xml.AddChildElem(XML_FILETYPE, (*ite).nFileType);
		// ��ʼʱ��.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
			(*ite).stStartTime.dwYear,
			(*ite).stStartTime.dwMonth,
			(*ite).stStartTime.dwDay,
			(*ite).stStartTime.dwHour,
			(*ite).stStartTime.dwMinute,
			(*ite).stStartTime.dwSecond);
		xml.AddChildElem(XML_STARTTIME, strTemp);
		// ����ʱ��.
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			(*ite).stEndTime.dwYear,
			(*ite).stEndTime.dwMonth,
			(*ite).stEndTime.dwDay,
			(*ite).stEndTime.dwHour,
			(*ite).stEndTime.dwMinute,
			(*ite).stEndTime.dwSecond);
		xml.AddChildElem(XML_STOPTIME, strTemp);
		// �ط�����.
		xml.AddChildElem(XML_PLAYTYPE, (*ite).emPlayType);
		// �豸����.
		xml.AddChildElem(XML_DVRTYPE, (*ite).emDevType);
        //�ط�url
        TCHAR szPlayUrl[255];
        MByteToWChar((*ite).szPlayUrl, szPlayUrl, sizeof(szPlayUrl));
        strTemp.Format(_T("%s"), szPlayUrl);
        xml.AddChildElem(XML_PLAYURL, strTemp);
	}
	
	strXML = xml.GetDoc();


	return TRUE;
}

/**   @fn          SetMenuCB
 *    @brief   	   ���ûص�����.
 *    @param[in]   pfnMenuCB:�ص�����.
 *    @param[in]   
 *    @return      
 */
void CPlaybackMain::SetMenuCB(PBSD_MenuCB pfnMenuCB, LPVOID pUserData)
{
	m_pfnMenuCB = pfnMenuCB;
	m_pUserData = pUserData;
}

LRESULT CPlaybackMain::OnMenuCallBack(WPARAM wParam, LPARAM lParam)
{
	int nMenuID = (int)wParam;
	//LPMenuCBData pData = (LPMenuCBData)lParam; 
    //TODO:�޸�ָ��Ϊ����Ϊ��coverity�ܱ����ȥ
    MenuCBData pData = (MenuCBData&)lParam; 
	if (m_pfnMenuCB != NULL)
	{
		OutputDebugString(_T("@z OnMenuCallBack"));
		//m_pfnMenuCB(nMenuID, *pData, m_pUserData);
        m_pfnMenuCB(nMenuID, pData, m_pUserData);
	}

	if (pData.pInfo != NULL)
	{
		delete pData.pInfo;
		pData.pInfo = NULL;
	}
// 	if (pData != NULL)
// 	{
// 		delete pData;
// 		pData = NULL;
// 	}

	return 0;
}

/**   @fn          SetWorkLogCallBack
 *    @brief   	   ���ûص�����.
 *    @param[in]   pfnWorkLogCB:�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CPlaybackMain::SetWorkLogCallBack(PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData)
{
	m_pWorkLogUserData = pUserData;
}

/**   @fn          SetPlaybackCB
 *    @brief   	   ���ûص�����.
 *    @param[in]   PBSD_PlaybackCB:�ص�����.
 *    @param[in]   pUserData:�û��Զ�������.
 *    @return      
 */
void CPlaybackMain::SetPlaybackCB(PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData)
{
	m_pfnPlayBackCB = pfnPlaybackCB;
	m_pPlaybackUserData = pUserData;
}

// �����ļ��ɹ�.
LRESULT CPlaybackMain::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	int nWndID = (int)wParam;
	
	// strXML.
	//<?xml version=\"1.0\" encoding=\"UTF-8\"?>
	//	<Response>
	//	<FileNum>100</FileNum>				// �ļ���.
	//		<File>
	//			<FileName>123</FileName>		// �ļ���.
	//			<FileSize>11111</FileSize>		// �ļ���С.
	//			<FileType>0</FileType>			// �ļ�����.
	//			<StartTime>2012-12-12 12:12:12</StartTime>		// ��ʼʱ��.
	//			<StopTime>2012-12-12 12:12:12</StopTime>		// ����ʱ��.
	//			<PlayType>0</PlayType>				// �洢��ʽ 0-DVR 1-CVR 2-NVR 3-���� 4-��������.
	//			<DvrType>0</DvrType>				// DVR����,����CMD����.
	//		</File>
	//		...
	//</Response>

	if (NULL == m_pfnPlayBackCB)
	{
		return 0;
	}

	// �����ļ�.
	std::vector<PB_FILEINFO> vecFileList = m_pDlgPlayVedio->GetFileInfo(nWndID);

	CString strXML;
	// ����XML����.
	if (!BuildFileXML(nWndID, vecFileList, strXML))
	{
		return FALSE;
	}

	m_pfnPlayBackCB(nWndID, strXML, m_pPlaybackUserData);

	return 0;
}

/**   @fn          OnFileListChange
 *    @brief   	   �ļ��б��л���ʾ.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlaybackMain::OnFileListChange(WPARAM wParam, LPARAM lParam)
{
    if (NULL == m_pfnPlayBackCB)
    {
        return 0;
    }

    // �����ļ�.
    std::vector<PB_FILEINFO> vecFileList = m_pDlgPlayVedio->GetFileInfo(m_pDlgPlayVedio->GetSelectedWnd());
    
    CString strXML;
    // ����XML����.
    if(!BuildFileXML(m_pDlgPlayVedio->GetSelectedWnd(), vecFileList, strXML))
    {
        return 0;
    }

    m_pfnPlayBackCB(m_pDlgPlayVedio->GetSelectedWnd(), strXML, m_pPlaybackUserData);
	return 0;
}

/**   @fn          StopPlayCam
 *    @brief   	   ֹͣ�طż�ص�.
 *    @param[in]   nResID:��ص�ID.
 *    @param[in]   
 *    @return      
 */
void CPlaybackMain::StopPlayCam()
{
	if (m_pDlgPlayVedio != NULL)
	{
		m_pDlgPlayVedio->StopPlayback();
	}
    return;
}

/**   @fn          Invalidate
 *    @brief   	   ˢ�´���.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlaybackMain::Invalidate()
{
	if (m_pDlgPlayVedio != NULL)
	{
		m_pDlgPlayVedio->Invalidate();
	}
}