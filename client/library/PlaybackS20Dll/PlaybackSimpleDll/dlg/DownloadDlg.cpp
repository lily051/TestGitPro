// DownloadDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadDlg.h"


// CDownloadDlg 对话框

IMPLEMENT_DYNAMIC(CDownloadDlg, CDialog)

CDownloadDlg::CDownloadDlg(HANDLE hDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
	m_bMove = FALSE;

	m_lDownloadSeq = 0;

	m_pDownloadTitleDlg = NULL;
	m_pDownloadingDlg = NULL;
	m_pDownloadSucDlg = NULL;
	m_pDownloadFailedDlg = NULL;
	m_hDlg = hDlg;
}

CDownloadDlg::~CDownloadDlg()
{
	m_pDownloadTitleDlg = NULL;
	m_pDownloadingDlg = NULL;
	m_pDownloadSucDlg = NULL;
	m_pDownloadFailedDlg = NULL;
	m_hDlg = NULL;
}

void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(MSG_DOWNLIST_SELECT, OnMsgListSelect)
	ON_MESSAGE(MSG_DOWNLOADTASK_DEL, OnMsgDelete)
	ON_MESSAGE(MSG_DOWNLOADTASK_REDOWN, OnMsgRedown)
	ON_MESSAGE(MSG_DOWNLOADTASK_FAIL, OnMsgFailed)
	ON_MESSAGE(MSG_DOWNLOADTASK_SUC, OnMsgSuccess)
	ON_MESSAGE(MSG_RECORD_DOWNLOAD_NOTIFY, OnMsgNotifyDown)
	ON_MESSAGE(MSG_DOWNLOADFILE_PLAY, OnDownloadFilePlay)
END_MESSAGE_MAP()


// CDownloadDlg 消息处理程序

BOOL CDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDownloadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pDownloadTitleDlg != NULL)
	{
		m_pDownloadTitleDlg->DestroyWindow();
		delete m_pDownloadTitleDlg;
		m_pDownloadTitleDlg = NULL;
	}

	if (m_pDownloadingDlg != NULL)
	{
		m_pDownloadingDlg->DestroyWindow();
		delete m_pDownloadingDlg;
		m_pDownloadingDlg = NULL;
	}

	if (m_pDownloadFailedDlg != NULL)
	{
		m_pDownloadFailedDlg->DestroyWindow();
		delete m_pDownloadFailedDlg;
		m_pDownloadFailedDlg = NULL;
	}

	if (m_pDownloadSucDlg != NULL)
	{
		m_pDownloadSucDlg->DestroyWindow();
		delete m_pDownloadSucDlg;
		m_pDownloadSucDlg = NULL;
	}

}

void CDownloadDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}
}

//创建子控件
void CDownloadDlg::InitCtrl()
{
	if (m_pDownloadTitleDlg == NULL)
	{
		m_pDownloadTitleDlg = new CDownloadTitleDlg;
		m_pDownloadTitleDlg->Create(CDownloadTitleDlg::IDD, this);
		m_pDownloadTitleDlg->ShowWindow(SW_SHOW);
	}

	if (m_pDownloadingDlg == NULL)
	{
		m_pDownloadingDlg = new CDownloadingDlg(m_hDlg);
		m_pDownloadingDlg->Create(CDownloadingDlg::IDD, this);
		m_pDownloadingDlg->ShowWindow(SW_SHOW);
	}

	if (m_pDownloadSucDlg == NULL)
	{
		m_pDownloadSucDlg = new CDownSuccessDlg;
		m_pDownloadSucDlg->Create(CDownSuccessDlg::IDD, this);
		m_pDownloadSucDlg->ShowWindow(SW_HIDE);
	}

	if (m_pDownloadFailedDlg == NULL)
	{
		m_pDownloadFailedDlg = new CDownFailedDlg;
		m_pDownloadFailedDlg->Create(CDownFailedDlg::IDD, this);
		m_pDownloadFailedDlg->ShowWindow(SW_HIDE);
	}

}

//移动控件
void CDownloadDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcTitleDlg(rcClient);
	rcTitleDlg.DeflateRect(2, 2, 0, 1);
	rcTitleDlg.right = rcTitleDlg.left + DOWNLOADTITLE_WIDTH;
	if (m_pDownloadTitleDlg)
	{
		m_pDownloadTitleDlg->MoveWindow(rcTitleDlg);
	}

	CRect rcListDlg(rcClient);
	rcListDlg.DeflateRect(0, 2, 2, 1);
	rcListDlg.left = rcTitleDlg.right + 2;
	if (m_pDownloadingDlg)
	{
		m_pDownloadingDlg->MoveWindow(rcListDlg);
	}
	if (m_pDownloadSucDlg)
	{
		m_pDownloadSucDlg->MoveWindow(rcListDlg);
	}
	if (m_pDownloadFailedDlg)
	{
		m_pDownloadFailedDlg->MoveWindow(rcListDlg);
	}

}

void CDownloadDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, UI_WND_BKGND);

}


//添加下载信息
BOOL CDownloadDlg::AddTimeDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PLAY_PARAM& stPlayParam, CString& strErrorMsg)
{
	DOWNLOAD_INFO stDownloadInfo;
	stDownloadInfo.nCameraID = nCameraID;
	stDownloadInfo.dwRev = dwRev;
	stDownloadInfo.emDownType = stPlayParam.emPlayType;
	stDownloadInfo.bByTime = TRUE;
	memcpy_s(&stDownloadInfo.stLoginInfo, sizeof(PB_LOGININFO), &stLoginInfo, sizeof(PB_LOGININFO));
	stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime = stPlayParam.startTime;
	stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime = stPlayParam.stopTime;

	////获取监控点信息
	//PlaybackInfo pbPlaybackInfo;
	//if (!theApp.GetPlaybackInfo(nCameraID, pbPlaybackInfo))
	//{
	//	CString strMsg;
	//	strMsg.Format(_T("获取id为%d的监控点回放信息失败."), nCameraID);
	//	OutputDebugString(strMsg);
	//	return;
	//}
    stDownloadInfo.unDownloadParam.downloadTimeParam.lChannel = nChannel;
	memcpy_s(&stDownloadInfo.unDownloadParam.downloadTimeParam.szStreamID, 32, &(stPlayParam.szStreamID), 32);
    INT64 nTotalSize = 0;
    nTotalSize = CalTotalSizeInTime(stDownloadInfo, stLoginInfo);
    if (nTotalSize >= (double)4*1024*1024*1024)
    {
        strErrorMsg = _T("[录像大小超过4G,请缩短时间片段]");
        return FALSE;
    }
    else
    {
        stDownloadInfo.unDownloadParam.downloadTimeParam.dwFileSize = (DWORD)nTotalSize;
    }

	// 下载类型名称转换.
	CString strDownType = theApp.m_manDevice.GetPBTypeNameFromPBType(stDownloadInfo.emDownType);

	CStringA strStartDate;
	CStringA strStartTime, strStopTime;
	strStartDate.Format("%04d%02d%02d", stPlayParam.startTime.dwYear, stPlayParam.startTime.dwMonth, stPlayParam.startTime.dwDay);
	strStartTime.Format("%02d%02d%02d", stPlayParam.startTime.dwHour, stPlayParam.startTime.dwMinute, stPlayParam.startTime.dwSecond);
	strStopTime.Format("%02d%02d%02d", stPlayParam.stopTime.dwHour, stPlayParam.stopTime.dwMinute, stPlayParam.stopTime.dwSecond);
	CStringA strDownloadFile;
	CStringW strCamera = strCamName;
	char * szCamera;
	int nLength;
	CP936_W2A(strCamera.GetBuffer(), strCamera.GetLength()+1, szCamera, nLength);
	sprintf_s(stDownloadInfo.szCameraName, 255, szCamera);
	strDownloadFile.Format("%sDownload\\%s\\%s_%s%s-%s", 
		theApp.m_stPlaybackParam.szDownloadPath,
		strStartDate, 
		CStringW2A(strDownType),
		szCamera, 
		strStartTime, 
		strStopTime);
	CodeSet_Free(szCamera);
	
	if (strDownloadFile.GetLength() > 251)
	{
		strDownloadFile = strDownloadFile.Left(251);
	}
	strDownloadFile += ".mp4";
	sprintf_s(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile, 255, strDownloadFile.GetBuffer());

	//判断是否在list中
	{
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownloadInfo.begin();
		while (itor != m_mapDownloadInfo.end())
		{
			if (stDownloadInfo == itor->second)
			{
				strErrorMsg = _T("[下载信息已在下载列表中，不可重复加入]");
				return FALSE;
			}
			itor++;
		}
	}

	//获取本地相应文件大小
	{
		CFile cfile;  
		CFileStatus rStatus;
		wchar_t* wszSavePath = NULL;
		int nLeng = 0;
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile,
			strlen(stDownloadInfo.unDownloadParam.downloadTimeParam.szDownloadFile)+1, 
			wszSavePath, nLeng);
		if(CFile::GetStatus(wszSavePath,rStatus))
		{
			CodeSet_Free(wszSavePath);
			if (MsgBoxApi_Show(_T("要下载的文件已经在本地磁盘，是否覆盖"), MSGBOX_TYPE_YESNO) == IDNO)
			{
				return TRUE;
			}
		}
	}

	//加入
	m_lDownloadSeq++;
	if (m_pDownloadingDlg && m_pDownloadingDlg->AddDownloadInfo(m_lDownloadSeq, stDownloadInfo))
	{
		m_mapDownloadInfo[m_lDownloadSeq] = stDownloadInfo;
		if (m_pDownloadTitleDlg)
		{
			(m_pDownloadTitleDlg->m_nDowningCount)++;
			m_pDownloadTitleDlg->SetStaText();
		}
	}

	return TRUE;
}

//增加文件下载信息
BOOL CDownloadDlg::AddFileDownloadInfo(int nCameraID, DWORD dwRev, const CString& strCamName, int nChannel, PB_LOGININFO& stLoginInfo, PB_FILEINFO& stFileInfo)
{
	DOWNLOAD_INFO stDownloadInfo;
	stDownloadInfo.nCameraID = nCameraID;
	stDownloadInfo.dwRev = dwRev;
	stDownloadInfo.emDownType = stFileInfo.emPlayType;
	stDownloadInfo.bByTime = FALSE;
	memcpy_s(&stDownloadInfo.stLoginInfo, sizeof(PB_LOGININFO), &stLoginInfo, sizeof(PB_LOGININFO));
	stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo = stFileInfo;
    stDownloadInfo.nChannel = nChannel;
	/*PlaybackInfo pbPlaybackInfo;
	if (!theApp.GetPlaybackInfo(nCameraID, pbPlaybackInfo))
	{
		CString strMsg;
		strMsg.Format(_T("获取id为%d的监控点回放信息失败."), nCameraID);
		OutputDebugString(strMsg);
		return;
	}*/

	stDownloadInfo.unDownloadParam.downloadFileParam.lChannel = nChannel;
	memcpy_s(&stDownloadInfo.unDownloadParam.downloadFileParam.szStreamID, 32, &(stFileInfo.szStreamID), 32);
	CStringA strStartDate;
	strStartDate.Format("%04d%02d%02d", stFileInfo.stStartTime.dwYear, stFileInfo.stStartTime.dwMonth, stFileInfo.stStartTime.dwDay);
	CStringA strStartTime;
	strStartTime.Format("%02d%02d%02d", stFileInfo.stStartTime.dwHour, stFileInfo.stStartTime.dwMinute, stFileInfo.stStartTime.dwSecond);
	CStringA strDownloadFile;
	CStringW strCamera = strCamName;
	char * szCamera;
	int nLeng;
	CP936_W2A(strCamera.GetBuffer(), strCamera.GetLength()+1, szCamera, nLeng);
	sprintf_s(stDownloadInfo.szCameraName, 255, szCamera);
	CStringA strFileName;
	strFileName.Format("%s", stFileInfo.szFileName);
	CheckFileName(strFileName);
	strDownloadFile.Format("%sDownload\\%s\\%s_%s", 
		theApp.m_stPlaybackParam.szDownloadPath,
		strStartDate, 
		strFileName, 
		strStartTime);
	CodeSet_Free(szCamera);
	if (strDownloadFile.GetLength() > 240)
	{
		strDownloadFile = strDownloadFile.Left(240);
	}
	// 文件名与设备类型组合.
	strDownloadFile += ".mp4";
	sprintf_s(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile, 255, strDownloadFile.GetBuffer());

	//判断是否在list中
	{
		std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownloadInfo.begin();
		while (itor != m_mapDownloadInfo.end())
		{
			if (stDownloadInfo == itor->second)
			{
				MsgBoxApi_Show(_T("下载信息已在下载列表中，不可重复加入"), MSGBOX_TYPE_WARNING);
				return FALSE;
			}
			itor++;
		}
	}

	//获取本地相应文件大小
	{
		CFile cfile;  
		CFileStatus rStatus;
		wchar_t* wszSavePath = NULL;
		int nLeng2 = 0;
		CP936_A2W(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile,
			strlen(stDownloadInfo.unDownloadParam.downloadFileParam.szDownloadFile)+1, 
			wszSavePath, nLeng2);
		if (CFile::GetStatus(wszSavePath,rStatus))
		{
			CodeSet_Free(wszSavePath);
			if (MsgBoxApi_Show(_T("要下载的文件已经在本地磁盘，是否覆盖"), MSGBOX_TYPE_YESNO) == IDNO)
			{
				return FALSE;
			}
		}
	}
	 if (stLoginInfo.emPlayType == PBSD_TYPE_KMS)
	 {
		 strcpy_s(stDownloadInfo.unDownloadParam.downloadFileParam.stFileInfo.szPlayUrl, 255, stLoginInfo.unLoginType.stDvrLoginInfo.szRev);
	 }
	//加入
	m_lDownloadSeq++;
	if (m_pDownloadingDlg && m_pDownloadingDlg->AddDownloadInfo(m_lDownloadSeq, stDownloadInfo))
	{
		m_mapDownloadInfo[m_lDownloadSeq] = stDownloadInfo;
		if (m_pDownloadTitleDlg)
		{
			(m_pDownloadTitleDlg->m_nDowningCount)++;
			m_pDownloadTitleDlg->SetStaText();
		}
	}

	return TRUE;
}

//事件通知下载
BOOL CDownloadDlg::NotifyStartDownload()
{
	if (m_pDownloadingDlg)
	{
		return m_pDownloadingDlg->NotifyStartDownload();
	}

	return FALSE;
}

//接收来自static的点击消息
LRESULT CDownloadDlg::OnMsgListSelect(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_STATIC_DOWNLOADING:
		{
			if (m_pDownloadingDlg)
			{
				m_pDownloadingDlg->ShowWindow(SW_SHOW);
			}
			if (m_pDownloadFailedDlg)
			{
				m_pDownloadFailedDlg->ShowWindow(SW_HIDE);
			}
			if (m_pDownloadSucDlg)
			{
				m_pDownloadSucDlg->ShowWindow(SW_HIDE);
			}
		}
		break;
	case IDC_STATIC_DOWNFAILED:
		{
			if (m_pDownloadingDlg)
			{
				m_pDownloadingDlg->ShowWindow(SW_HIDE);
			}
			if (m_pDownloadFailedDlg)
			{
				m_pDownloadFailedDlg->ShowWindow(SW_SHOW);
			}
			if (m_pDownloadSucDlg)
			{
				m_pDownloadSucDlg->ShowWindow(SW_HIDE);
			}
		}
		break;
	case IDC_STATIC_DOWNSUCCESS:
		{
			if (m_pDownloadingDlg)
			{
				m_pDownloadingDlg->ShowWindow(SW_HIDE);
			}
			if (m_pDownloadFailedDlg)
			{
				m_pDownloadFailedDlg->ShowWindow(SW_HIDE);
			}
			if (m_pDownloadSucDlg)
			{
				m_pDownloadSucDlg->ShowWindow(SW_SHOW);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

//删除消息
LRESULT CDownloadDlg::OnMsgDelete(WPARAM wParam, LPARAM lParam)
{
	m_mapDownloadInfo.erase(wParam);

	switch (lParam)
	{
	case IDD_DIALOG_DOWNLOADING:
		{
			if (m_pDownloadTitleDlg)
			{
				(m_pDownloadTitleDlg->m_nDowningCount)--;
				m_pDownloadTitleDlg->SetStaText();
			}
		}
		break;
	case IDD_DIALOG_DOWNSUCCESS:
		{
			if (m_pDownloadTitleDlg)
			{
				(m_pDownloadTitleDlg->m_nDownSuccessCount)--;
				m_pDownloadTitleDlg->SetStaText();
			}
		}
		break;
	case IDD_DIALOG_DOWNFAILED:
		{
			if (m_pDownloadTitleDlg)
			{
				(m_pDownloadTitleDlg->m_nDownFailedCount)--;
				m_pDownloadTitleDlg->SetStaText();
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

//重新下载消息
LRESULT CDownloadDlg::OnMsgRedown(WPARAM wParam, LPARAM lParam)
{
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownloadInfo.find((LONG)wParam);
	if (itor == m_mapDownloadInfo.end())
	{
		return 0;
	}

	if (m_pDownloadingDlg)
	{
		m_pDownloadingDlg->AddDownloadInfo(itor->first, itor->second);
	}

	//刷新界面下载数显示
	if (m_pDownloadTitleDlg)
	{
		(m_pDownloadTitleDlg->m_nDownFailedCount)--;
		(m_pDownloadTitleDlg->m_nDowningCount)++;
		m_pDownloadTitleDlg->SetStaText();
	}

	return 0;
}

//下载通知消息
LRESULT CDownloadDlg::OnMsgNotifyDown(WPARAM wParam, LPARAM lParam)
{
	NotifyStartDownload();

	return 0;
}

//下载完成消息
LRESULT CDownloadDlg::OnMsgSuccess(WPARAM wParam, LPARAM lParam)
{
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownloadInfo.find((LONG)wParam);
	if (itor == m_mapDownloadInfo.end())
	{
		return 0;
	}

	if (m_pDownloadSucDlg)
	{
		m_pDownloadSucDlg->AddDownloadInfo(itor->first, itor->second);
	}

	//刷新界面下载数显示
	if (m_pDownloadTitleDlg)
	{
		(m_pDownloadTitleDlg->m_nDownSuccessCount)++;
		(m_pDownloadTitleDlg->m_nDowningCount)--;
		m_pDownloadTitleDlg->SetStaText();
	}

	return 0;
}

//下载失败
LRESULT CDownloadDlg::OnMsgFailed(WPARAM wParam, LPARAM lParam)
{
	std::map<LONG, DOWNLOAD_INFO>::iterator itor = m_mapDownloadInfo.find((LONG)wParam);
	DWORD dwError = (DWORD)lParam;
	if (itor == m_mapDownloadInfo.end())
	{
		return 0;
	}

	if (PB_CREATEFILE_FILE == PB_GetLastError())
	{
		MsgBoxApi_Show(_T("下载失败,请使用管理员身份运行IE,或降低权限等级"), MSGBOX_TYPE_ERROR);
	}
	
	if (PB_SEND_TO_DEV_FAILD == PB_GetLastError())
	{
		MsgBoxApi_Show(_T("任务下载失败，请检查本地及设备网络连接"), MSGBOX_TYPE_ERROR);
	}
	if (m_pDownloadFailedDlg)
	{
		m_pDownloadFailedDlg->AddDownloadInfo(itor->first, itor->second, dwError);
	}

	//刷新界面下载数显示
	if (m_pDownloadTitleDlg)
	{
		(m_pDownloadTitleDlg->m_nDownFailedCount)++;
		(m_pDownloadTitleDlg->m_nDowningCount)--;
		m_pDownloadTitleDlg->SetStaText();
	}

	return 0;
}

LRESULT CDownloadDlg::OnDownloadFilePlay(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_DOWNLOADFILE_PLAY, wParam, lParam);
	return 0;
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   获取下载信息.
 *    @param[in]   stData:下载信息.
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CDownloadDlg::GetPlaybackInfo(PlaybackStatData& stData)
{
	if (NULL == m_pDownloadingDlg)
	{
		return FALSE;
	}

	return m_pDownloadingDlg->GetPlaybackInfo(stData);
}

/**   @fn          CheckFileName
 *    @brief   	   检查文件名.
 *    @param[in]   strFileName:文件名.
 *    @param[in]   
 *    @return      
 */
void CDownloadDlg::CheckFileName(CStringA& strFileName)
{
	strFileName.Replace('/', '_');
	strFileName.Replace('\\', '_');
	strFileName.Replace('%', '_');
	strFileName.Replace(':', '_');
	strFileName.Replace('?', '_');
	strFileName.Replace('<', '_');
	strFileName.Replace('>', '_');
	strFileName.Replace('*', '_');
	strFileName.Replace('\"', '_');

	strFileName.Replace(".mp4", "");
	strFileName.Replace(".avi", "");
}

//lint -e524
INT64 CDownloadDlg::CalTotalSizeInTime( DOWNLOAD_INFO &stDownloadInfo, PB_LOGININFO& stLoginInfo )
{
    FINDFILE_PARAM stFindFileParam;
    CTime tmStartTime, tmStopTime;
    if (!PB_TIME::PBTIME2Ctime(stDownloadInfo.unDownloadParam.downloadTimeParam.stStartTime, tmStartTime) ||
        !PB_TIME::PBTIME2Ctime(stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime, tmStopTime))
    {
        return 0;
    }

    CTime tmAdjustTime(tmStartTime.GetTime() - 3600);
    PB_TIME::Ctime2PBTIME(tmAdjustTime, stFindFileParam.stStartTime);
    stFindFileParam.stStopTime = stDownloadInfo.unDownloadParam.downloadTimeParam.stStopTime;
    stFindFileParam.lChannel = stDownloadInfo.unDownloadParam.downloadTimeParam.lChannel;
    LPB_FILEINFO lpFileList = new PB_FILEINFO[MAX_RECORD_COUNT];
    int nFileCount  = 0;
    PB_FindFile(stLoginInfo, stFindFileParam, lpFileList, nFileCount);
    CTime tmTempEnd, tmTempBegin;
    INT64  nTotalSize = 0;
    for(int i = 0; i < nFileCount; i++)
    {
        PB_TIME::PBTIME2Ctime(lpFileList[i].stStartTime, tmTempBegin);
        PB_TIME::PBTIME2Ctime(lpFileList[i].stEndTime, tmTempEnd);

        if (tmStartTime >= tmTempEnd)
        {
            continue;
        }
        if (tmStopTime <= tmTempBegin)
        {
            continue;
        }
        if ((tmStartTime < tmTempEnd && tmStartTime > tmTempBegin) && (tmStopTime < tmTempEnd && tmStopTime > tmTempBegin ))
        {
            nTotalSize += ((double)(tmStopTime.GetTime() - tmStartTime.GetTime()) / (tmTempEnd.GetTime()- tmTempBegin.GetTime())) * lpFileList[i].dwFileSize;
        }
        else
        {
            if (tmStartTime < tmTempEnd && tmStartTime > tmTempBegin)
            {
                nTotalSize += ((double)(tmTempEnd.GetTime() - tmStartTime.GetTime()) / (tmTempEnd.GetTime()- tmTempBegin.GetTime())) * lpFileList[i].dwFileSize;
            }
            else if (tmStopTime < tmTempEnd && tmStopTime > tmTempBegin)
            {
                nTotalSize += ((double)(tmStopTime.GetTime() - tmTempBegin.GetTime()) / (tmTempEnd.GetTime()- tmTempBegin.GetTime())) * lpFileList[i].dwFileSize;
            }
            else
            {
                nTotalSize += lpFileList[i].dwFileSize;
            }
        }
    }
    delete []lpFileList;

    return nTotalSize;
}
//lint +e524