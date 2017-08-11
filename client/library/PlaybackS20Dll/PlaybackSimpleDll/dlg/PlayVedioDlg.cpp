// PlayVedioDlg.cpp : 实现文件
//
//lint -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "PlayVedioDlg.h"
#include "DownloadTaskAdd.h"

// CPlayVedioDlg 对话框
void CALLBACK pMsgOsdTime(LONG lOsdTime, UINT unWndID, LPVOID lpvoid)
{
	CPlayVedioDlg* pInstance = (CPlayVedioDlg*)lpvoid;
	if (pInstance == NULL || lOsdTime <= 0)
	{
		return;
	}

	if (pInstance->m_pPlayBarTimeCtrl)
	{
		pInstance->m_pPlayBarTimeCtrl->PlayBar_SetTimeByElem(unWndID, lOsdTime);
	}

	

}

IMPLEMENT_DYNAMIC(CPlayVedioDlg, CDialog)

CPlayVedioDlg::CPlayVedioDlg(HANDLE hDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CPlayVedioDlg::IDD, pParent)
	, m_hDlg(hDlg)
{
	m_pPlayViewWnd = NULL;
	m_pToolBarWnd = NULL;
	m_pPlayBarTimeCtrl = NULL;
	m_pFullScreenWnd = NULL;
	m_pVolumnSetWnd = NULL;
	m_pDownloadDlg = NULL;
	m_bFullScreen = FALSE;

	m_bMove = FALSE;

	m_tMax = 0;
	m_tMin = 0;
	m_pParentWnd = NULL;
}

CPlayVedioDlg::~CPlayVedioDlg()
{
	m_pDownloadDlg = NULL;
	m_pPlayViewWnd = NULL;
	m_pToolBarWnd = NULL;
	m_pPlayBarTimeCtrl = NULL;
	m_pFullScreenWnd = NULL;
	m_pVolumnSetWnd = NULL;
	m_hDlg = NULL;
	m_pParentWnd = NULL;
}

void CPlayVedioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayVedioDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_TOOLBAR_BTN_CLICK, &CPlayVedioDlg::OnMsgToolButtonClick)
	ON_MESSAGE(WM_PB_SINGLE_TIMECHG, &CPlayVedioDlg::OnMsgTimeBarSetPos)
	ON_MESSAGE(MSG_TOOLBAR_UPDATE, &CPlayVedioDlg::OnMsgToolBarUpdate)
	ON_MESSAGE(MSG_FLOATWND_CLOST, &CPlayVedioDlg::OnMsgVolumnWndClose)
	ON_MESSAGE(MSG_PLAYBACK_NEXT_FILE, &CPlayVedioDlg::OnMsgPlayNextFile)
	ON_MESSAGE(MSG_GROUPCFG_CHANGE, &CPlayVedioDlg::OnMsgUpdateGroup)
	ON_MESSAGE(MSG_MENU_CALLBACK, OnMenuCallBack)
	ON_MESSAGE(MSG_PLAYBACK_FIND_SUCCESS, &CPlayVedioDlg::OnMsgFindSuccess)
	ON_MESSAGE(MSG_PLAYBACK_FIND_FAIL, &CPlayVedioDlg::OnMsgFindFail)
	ON_MESSAGE(MSG_FILELIST_CHANGE, &CPlayVedioDlg::OnFileListChange)
	ON_MESSAGE(MSG_MINITOOLBAR_DOWNLOAD, &CPlayVedioDlg::OnMsgMiniToolbarDownload)
	ON_MESSAGE(MSG_PLAYBACK_STATE, &CPlayVedioDlg::OnMsgPlayBackState)
END_MESSAGE_MAP()


// CPlayVedioDlg 消息处理程序

BOOL CPlayVedioDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CTime ctCur = CTime::GetCurrentTime();
	CTime ctStart = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),0,0,0);    
	CTime ctEnd = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),23,59,59);  

	m_tMin = (time_t)ctStart.GetTime();
	m_tMax = (time_t)ctEnd.GetTime();

	//创建子窗口
	CreateChildWnd();

	m_pParentWnd = GetParent();

	m_bMove = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPlayVedioDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_bFullScreen)
	{
		SetParent(m_pParentWnd);
	}

	if (m_pPlayViewWnd)
	{
		m_pPlayViewWnd->DestroyWindow();
		delete m_pPlayViewWnd;
		m_pPlayViewWnd = NULL;
	}

	if (m_pToolBarWnd)
	{
		m_pToolBarWnd->DestroyWindow();
		delete m_pToolBarWnd;
		m_pToolBarWnd = NULL;
	}

	if (m_pPlayBarTimeCtrl)
	{
		m_pPlayBarTimeCtrl->DestroyWindow();
		delete m_pPlayBarTimeCtrl;
		m_pPlayBarTimeCtrl = NULL;
	}

	if (m_pFullScreenWnd)
	{
		m_pFullScreenWnd->DestroyWindow();
		delete m_pFullScreenWnd;
		m_pFullScreenWnd = NULL;
	}

	if (m_pVolumnSetWnd)
	{
		m_pVolumnSetWnd->DestroyWindow();
		delete m_pVolumnSetWnd;
		m_pVolumnSetWnd = NULL;
	}

}

void CPlayVedioDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CPlayVedioDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_bMove)
	{
		MoveCtrl();
	}
}

//创建子窗口
void CPlayVedioDlg::CreateChildWnd()
{
	if (m_pPlayViewWnd == NULL)
	{
		m_pPlayViewWnd = new CPlayWndView(m_hDlg);
		if (m_pPlayViewWnd != NULL)
		{
			m_pPlayViewWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, WND_PLAYVIEW_ID);
			m_pPlayViewWnd->SetOsdCallback(pMsgOsdTime, this);
			m_pPlayViewWnd->ShowWindow(SW_SHOW);
		}
	}

	if (m_pToolBarWnd == NULL)
	{

		m_pToolBarWnd = new CPlayToolBarWnd();
		if (m_pToolBarWnd != NULL)
		{
			m_pToolBarWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, WND_TOOLBAR_ID);
		}
	}

	if (m_pPlayBarTimeCtrl == NULL)
	{
		m_pPlayBarTimeCtrl = new CPlayBarTimeCtrl_For9000;
		if (m_pPlayBarTimeCtrl != NULL)
		{
			m_pPlayBarTimeCtrl->PlayBar_Create(CRect(0,0,0,0), this, WND_PLAYBARTIMECTRL_ID, 1);
			m_pPlayBarTimeCtrl->PlayBar_SetSynState(FALSE);
		}
		switch (theApp.m_emLayoutStyle)
		{
		case LAYOUT_WND_1:
		case LAYOUT_WND_4:
			m_pPlayBarTimeCtrl->PlayBar_SetCycleElem((int)theApp.m_emLayoutStyle, 1);
			break;

		case LAYOUT_WND_9:
			m_pPlayBarTimeCtrl->PlayBar_SetCycleElem(3, 3);
			break;

		case LAYOUT_WND_16:
			m_pPlayBarTimeCtrl->PlayBar_SetCycleElem(4, 4);
			break;

		default:
			m_pPlayBarTimeCtrl->PlayBar_SetCycleElem((int)theApp.m_emLayoutStyle, 1);
		}
		
		InitBarTimeCtrl_For9000();
	}

	//创建全屏窗口载体
	if (NULL == m_pFullScreenWnd)
	{
		m_pFullScreenWnd = new CWnd;
	}
	if (NULL != m_pFullScreenWnd)
	{
		m_pFullScreenWnd->CreateEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW, AfxRegisterWndClass(NULL), 
			NULL,WS_POPUP|WS_VISIBLE,CRect(0,0,0,0), NULL, 0);

		CWnd* pScreenWnd = GetDesktopWindow();
		CRect rc;
		pScreenWnd->GetClientRect(&rc);
		m_pFullScreenWnd->MoveWindow(&rc);
		m_pFullScreenWnd->ShowWindow(SW_HIDE);
	}

}

//窗口移动
void CPlayVedioDlg::MoveCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nTimeBarHeight = 0;
	switch (theApp.m_emLayoutStyle)
	{
	case LAYOUT_WND_1:
		nTimeBarHeight = 58;
		break;

	case LAYOUT_WND_4:
	case LAYOUT_WND_16:
		nTimeBarHeight = 118;
		break;

	case LAYOUT_WND_9:
		nTimeBarHeight = 98;
		break;

	default:
		nTimeBarHeight = 58;
		break;
	}
	int nToolBarHeight = 39;

	//视频
	CRect rcVedio(rcClient);
	rcVedio.bottom = rcClient.bottom - nToolBarHeight - nTimeBarHeight;
	if (m_pPlayViewWnd)
	{
		m_pPlayViewWnd->MoveWindow(rcVedio);
	}

	//工具栏
	CRect rcToolBar(rcClient);
	rcToolBar.top = rcVedio.bottom;
	rcToolBar.bottom = rcToolBar.top + nToolBarHeight;
	if (m_pToolBarWnd)
	{
		m_pToolBarWnd->MoveWindow(rcToolBar);
	}

	//录像条
	CRect rcTimeBar(rcClient);
	rcTimeBar.top = rcToolBar.bottom;
	rcTimeBar.bottom = rcTimeBar.top + nTimeBarHeight;
	rcTimeBar.DeflateRect(-1, 0, -1, 0);
	if (m_pPlayBarTimeCtrl)
	{
		m_pPlayBarTimeCtrl->MoveWindow(rcTimeBar);
	}

}

//初始化TimeBar
void CPlayVedioDlg::InitBarTimeCtrl_For9000()
{
	if (m_pPlayBarTimeCtrl == NULL)
	{
		return;
	}

	PLAYBAR_ELEMENT_INFO PbElementInfo;
	int i = 0;
	for(i = 0; i < (int)theApp.m_emLayoutStyle; i++)
	{
		PbElementInfo.iElementID = i;
		wsprintf(PbElementInfo.czElementName, _T("%s%02d"), _T("窗口"), i+1);
		m_pPlayBarTimeCtrl->PlayBar_AddElement(&PbElementInfo);
	}

	m_pPlayBarTimeCtrl->PlayBar_SetCursor(IDC_CURSOR_HANLD);
	m_pPlayBarTimeCtrl->PlayBar_SetDragCursor((unsigned char)IDC_CURSOR_POINT);

	CTime ctCur = CTime::GetCurrentTime();
	CTime ctStart = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),0,0,0);    

	time_t ctSpanStart = (time_t)ctStart.GetTime();
	CTime ctEnd = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),23,59,59);     
	time_t ctSpanEnd = (time_t)ctEnd.GetTime();

	m_pPlayBarTimeCtrl->PlayBar_InitTimeSpan(ctSpanStart,ctSpanEnd);

	m_pPlayBarTimeCtrl->PlayBar_AddfileType(0,RGB(68,84,172));//定时录像
	m_pPlayBarTimeCtrl->PlayBar_AddfileType(1,RGB(255,0,0));//报警录像
	m_pPlayBarTimeCtrl->PlayBar_AddfileType(2,RGB(255,255,0));//手动录像
	m_pPlayBarTimeCtrl->PlayBar_InitScroll();	// 滚动条.
}

//监控点回放
BOOL CPlayVedioDlg::Playback(
							 int nCameraID, 
							 const CString& strCamName, 
							 const PLAY_PARAM& stPlayParam, 
							 PB_LOGININFO& stLoginInfo, 
							 int nWndID, 
							 DWORD dwRight,
							 DWORD dwRev)
{
	// 清空录像彩条.
	Clear((-1 == nWndID) ? GetSelectedWnd() : nWndID);
	PLAYBACK_TIME_INFO stPlaybackParam;
	FINDFILE_PARAM stFindFileParam;
	CString strCameraName;
	
	//获取登录信息
	int nChannel = stPlayParam.nChannel;
	strCameraName = strCamName;

	m_stSpanStart = stPlayParam.startTime;
	m_stSpanStop = stPlayParam.stopTime;
	stPlaybackParam.lBwControl = theApp.m_lBwControl;
	stPlaybackParam.lChannel = nChannel;
	stFindFileParam.lChannel = nChannel;
	stPlaybackParam.stStartTime = stPlayParam.startTime;
	stPlaybackParam.stStopTime = stPlayParam.stopTime;
	stPlaybackParam.emRecordType = stPlayParam.emRecordType;
	strcpy_s(stPlaybackParam.szStreamID, 32, stPlayParam.szStreamID);
	stFindFileParam.stStartTime = stPlayParam.startTime;
	stFindFileParam.stStopTime = stPlayParam.stopTime;
	stFindFileParam.emRecordType = stPlayParam.emRecordType;
	stFindFileParam.dwUseCardNo = (DWORD)stPlayParam.bByCardNum;
	strcpy_s(stFindFileParam.szStreamID, 32, stPlayParam.szStreamID);
	char szCardNum[32] = {0};
	WCharToMByte(stPlayParam.strCardNum, szCardNum, 32);
	memcpy_s(stFindFileParam.sCardNumber, sizeof(stFindFileParam.sCardNumber), szCardNum, sizeof(szCardNum));

	strcpy_s(stPlaybackParam.szPlayUrl, 255, stLoginInfo.unLoginType.stDvrLoginInfo.szRev);

	if (m_pPlayViewWnd)
	{
		if (!m_pPlayViewWnd->PlaybackTime(stLoginInfo, stPlaybackParam, stFindFileParam, strCameraName, nCameraID, nWndID, dwRight, dwRev))
		{
			PLAYBACKDLL_ERROR(_T("PlaybackTime error"));
			return FALSE;
		}
	}

	return TRUE;
}

/**   @fn          StopPlayback
 *    @brief   	   停止回放监控点.
 *    @param[in]   nCameraID:监控点ID.
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlayVedioDlg::StopPlayback(int nCameraID)
{
	if (NULL == m_pPlayViewWnd)
	{
		return FALSE;
	}
	m_pPlayViewWnd->StopPlayCam(nCameraID);

	return TRUE;
}

/**   @fn          StopPlayback
 *    @brief   	   停止回放监控点.
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CPlayVedioDlg::StopPlayback()
{
	if (NULL == m_pPlayViewWnd)
	{
		return FALSE;
	}
	m_pPlayViewWnd->StopPlayCam();

	return TRUE;
}


/**   @fn          StopPlaybackByWndNo
 *    @brief   	   停止回放指定窗口.
 *    @param[in]   nWndNo:窗口号.
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
void CPlayVedioDlg::StopPlaybackByWndNo(int nWndNo)
{
	if (NULL == m_pPlayViewWnd)
	{
		return;
	}
	m_pPlayViewWnd->StopPlay(nWndNo); 
}

/**   @fn          GetPlaybackInfo
 *    @brief   	   获取回放状态信息.
 *    @param[in]   nWndNo:窗口号.
 *    @param[out]  stData:状态信息.
 *    @return      
 */
BOOL CPlayVedioDlg::GetPlaybackInfo(int nWndNo, PlaybackStatData& stData)
{
	if (NULL == m_pPlayViewWnd)
	{
		return FALSE;
	}

	return m_pPlayViewWnd->GetPlaybackInfo(nWndNo, stData);
}

/**   @fn          StopAll
 *    @brief   	   停止全部回放窗口.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayVedioDlg::StopAll()
{
	m_pPlayViewWnd->StopAll();
}

//回放文件
BOOL CPlayVedioDlg::PlaybackFile(
								 int nCameraID, 
								 const CString& strCamName, 
								 int nChannel,
								 const char* pStreamID,
								 const PB_FILEINFO& stFileInfo, 
								 const PB_LOGININFO& stLoginInfo, 
								 int nWndID, 
								 DWORD dwRight,
								 DWORD dwRev)
{
	// 清空录像彩条.
	Clear((-1 == nWndID) ? GetSelectedWnd() : nWndID);
	PLAYBACK_FILE_INFO stPlaybackParam;
	CString strCameraName;

	//停止回放
	/*if (m_pPlayViewWnd)
	{
		m_pPlayViewWnd->StopPlay(nWndID);
	}*/

	//获取登录信息
	/*std::string strCamName;
	if (!theApp.GetLoginInfo(nCameraID, stFileInfo.emPlayType, stLoginInfo, nChannel, strCamName))
	{
		return FALSE;
	}*/
	strCameraName = strCamName;

	if (PB_TYPE_CVR == stLoginInfo.emPlayType  ||  PB_TYPE_CLOUD == stLoginInfo.emPlayType)
	{ 
		stPlaybackParam.lChannel = nCameraID;
	}
	else
	{
	    stPlaybackParam.lChannel = nChannel;
	}

	m_stSpanStart = stFileInfo.stStartTime;
	m_stSpanStop = stFileInfo.stEndTime;
	stPlaybackParam.stFileInfo = stFileInfo;
	strcpy_s(stPlaybackParam.stFileInfo.szPlayUrl, 255, stLoginInfo.unLoginType.stDvrLoginInfo.szRev);
	stPlaybackParam.lBwControl = theApp.m_lBwControl;

	if (m_pPlayViewWnd)
	{
		if (!m_pPlayViewWnd->PlaybackFile(stLoginInfo, stPlaybackParam, strCameraName, nCameraID, nWndID, dwRight, dwRev))
		{
			PLAYBACKDLL_ERROR(_T("PlaybackFile error"));
			return FALSE;
		}
	}

	return TRUE;
}

//建立录像彩条
void CPlayVedioDlg::BuildRecordGraph(const std::vector<PB_FILEINFO>& vecFileInfo, UINT unElementID)
{
	if (m_pPlayBarTimeCtrl == NULL)
	{
		return;
	}

	if (unElementID == -1)
	{
		if (NULL != m_pPlayViewWnd)
		{
			unElementID = m_pPlayViewWnd->m_unSelectWnd;
		}
	}

	m_pPlayBarTimeCtrl->PlayBar_ClearSection(unElementID);

	if (vecFileInfo.empty())
	{
		return;
	}

	//找最小的时间和最大的时间
	CTime ctMin(2050,1,1,0,0,0);
	CTime ctMax(2000,1,1,0,0,0);
	for (int i = 0; i < (int)(theApp.m_emLayoutStyle); i++)
	{
		std::vector<PB_FILEINFO>& vecFileInfoTmp = GetFileInfo(i);

		if (vecFileInfoTmp.empty())
		{
			continue;
		}
		CTime ctTempStart(vecFileInfoTmp[0].stStartTime.dwYear, 
			vecFileInfoTmp[0].stStartTime.dwMonth,
			vecFileInfoTmp[0].stStartTime.dwDay,
			vecFileInfoTmp[0].stStartTime.dwHour,
			vecFileInfoTmp[0].stStartTime.dwMinute,
			vecFileInfoTmp[0].stStartTime.dwSecond);
		CTime ctSpanStart(m_stSpanStart.dwYear, 
			m_stSpanStart.dwMonth,
			m_stSpanStart.dwDay,
			m_stSpanStart.dwHour,
			m_stSpanStart.dwMinute,
			m_stSpanStart.dwSecond);
		if (ctTempStart < ctSpanStart)
		{
			vecFileInfoTmp[0].stStartTime.dwYear = m_stSpanStart.dwYear;
			vecFileInfoTmp[0].stStartTime.dwMonth = m_stSpanStart.dwMonth;
			vecFileInfoTmp[0].stStartTime.dwDay = m_stSpanStart.dwDay;
			vecFileInfoTmp[0].stStartTime.dwHour = m_stSpanStart.dwHour;
			vecFileInfoTmp[0].stStartTime.dwMinute = m_stSpanStart.dwMinute;
			vecFileInfoTmp[0].stStartTime.dwSecond = m_stSpanStart.dwSecond;
		}

		int nLastFile = vecFileInfoTmp.size()-1;
		CTime ctTempStop(vecFileInfoTmp[nLastFile].stEndTime.dwYear,
			vecFileInfoTmp[nLastFile].stEndTime.dwMonth,
			vecFileInfoTmp[nLastFile].stEndTime.dwDay,
			vecFileInfoTmp[nLastFile].stEndTime.dwHour,
			vecFileInfoTmp[nLastFile].stEndTime.dwMinute,
			vecFileInfoTmp[nLastFile].stEndTime.dwSecond);
		CTime ctSpanStop(m_stSpanStop.dwYear, 
			m_stSpanStop.dwMonth,
			m_stSpanStop.dwDay,
			m_stSpanStop.dwHour,
			m_stSpanStop.dwMinute,
			m_stSpanStop.dwSecond);
		if (ctTempStop > ctSpanStop)
		{
			vecFileInfoTmp[nLastFile].stEndTime.dwYear = m_stSpanStop.dwYear;
			vecFileInfoTmp[nLastFile].stEndTime.dwMonth = m_stSpanStop.dwMonth;
			vecFileInfoTmp[nLastFile].stEndTime.dwDay = m_stSpanStop.dwDay;
			vecFileInfoTmp[nLastFile].stEndTime.dwHour = m_stSpanStop.dwHour;
			vecFileInfoTmp[nLastFile].stEndTime.dwMinute = m_stSpanStop.dwMinute;
			vecFileInfoTmp[nLastFile].stEndTime.dwSecond = m_stSpanStop.dwSecond;
		}

		if (ctMin > ctSpanStart)
		{
			ctMin = ctSpanStart;
		}

		if (ctMax < ctSpanStop)
		{
			ctMax = ctSpanStop;
		}

		m_tMin = ctMin.GetTime();
		m_tMax = ctMax.GetTime();
	}

	//m_pPlayBarTimeCtrl->PlayBar_ClearList();
	m_pPlayBarTimeCtrl->PlayBar_InitTimeSpan(m_tMin, m_tMax);

	CTime ctElemStart(vecFileInfo[0].stStartTime.dwYear, 
		vecFileInfo[0].stStartTime.dwMonth,
		vecFileInfo[0].stStartTime.dwDay,
		vecFileInfo[0].stStartTime.dwHour,
		vecFileInfo[0].stStartTime.dwMinute,
		vecFileInfo[0].stStartTime.dwSecond);
	int nLastFile = vecFileInfo.size()-1;
	CTime ctElemStop(vecFileInfo[nLastFile].stEndTime.dwYear,
		vecFileInfo[nLastFile].stEndTime.dwMonth,
		vecFileInfo[nLastFile].stEndTime.dwDay,
		vecFileInfo[nLastFile].stEndTime.dwHour,
		vecFileInfo[nLastFile].stEndTime.dwMinute,
		vecFileInfo[nLastFile].stEndTime.dwSecond);
	m_pPlayBarTimeCtrl->PlayBar_InitElemTimeSpan(unElementID, ctElemStart.GetTime(), ctElemStop.GetTime());

	SECTIONINFO_FOR9000 sectionInfo;
	SECTIONINFO_FOR9000 * pCurLocation;

	{//赋值第一个元素
		PB_FILEINFO fileData = vecFileInfo[0];
		sectionInfo.iDataType = GetGraphRecordType(fileData.nFileType);
		sectionInfo.iElementID = unElementID;
		CTime ctTempStart(fileData.stStartTime.dwYear, fileData.stStartTime.dwMonth,
			fileData.stStartTime.dwDay, fileData.stStartTime.dwHour,
			fileData.stStartTime.dwMinute,fileData.stStartTime.dwSecond);
		CTime ctTempStop(fileData.stEndTime.dwYear, fileData.stEndTime.dwMonth,
			fileData.stEndTime.dwDay, fileData.stEndTime.dwHour,
			fileData.stEndTime.dwMinute,fileData.stEndTime.dwSecond);
		sectionInfo.tTimeStart = (time_t)ctTempStart.GetTime();
		sectionInfo.tTimeEnd = (time_t)ctTempStop.GetTime();
		sectionInfo.pNext = NULL;
		pCurLocation = &sectionInfo;
	}

	SECTIONINFO_FOR9000 * pNext = NULL;
	CTimeSpan minTimeSpan;
	for (int iFile=1;iFile<(int)vecFileInfo.size();iFile++)
	{
		pNext = new SECTIONINFO_FOR9000;
		PB_FILEINFO fileData = vecFileInfo[iFile];
		pNext->iDataType = GetGraphRecordType(fileData.nFileType);
		pNext->iElementID = unElementID;
		CTime ctTempStart(fileData.stStartTime.dwYear, fileData.stStartTime.dwMonth,
			fileData.stStartTime.dwDay, fileData.stStartTime.dwHour,
			fileData.stStartTime.dwMinute,fileData.stStartTime.dwSecond);
		CTime ctTempStop(fileData.stEndTime.dwYear, fileData.stEndTime.dwMonth,
			fileData.stEndTime.dwDay, fileData.stEndTime.dwHour,
			fileData.stEndTime.dwMinute,fileData.stEndTime.dwSecond);
		pNext->tTimeStart = (time_t)ctTempStart.GetTime();
		pNext->tTimeEnd = (time_t)ctTempStop.GetTime();
		pCurLocation->pNext = pNext;
		//移动到下一个节点
		pCurLocation = pCurLocation->pNext;
		// 确定最合适的时间轴刻度
		CTimeSpan timeSpan = ctTempStop - ctTempStart;
		if (minTimeSpan.GetTotalSeconds() == 0)
		{
			minTimeSpan = timeSpan;
		}
	}
	if (minTimeSpan.GetTotalMinutes() <= 5)
	{
		m_pPlayBarTimeCtrl->PlayBar_SetSpantype(SPAN_HALFHOUR);
	}
	else if (minTimeSpan.GetTotalMinutes() > 5 && minTimeSpan.GetTotalMinutes() <= 30)
	{
		m_pPlayBarTimeCtrl->PlayBar_SetSpantype(SPAN_TWOHOUR);
	}
	else if (minTimeSpan.GetTotalMinutes() > 30 && minTimeSpan.GetTotalMinutes() <= 60)
	{
		m_pPlayBarTimeCtrl->PlayBar_SetSpantype(SPAN_SIXHOUR);
	}
	else 
	{
		m_pPlayBarTimeCtrl->PlayBar_SetSpantype(SPAN_ONEDAY);
	}
	pNext = NULL;
	m_pPlayBarTimeCtrl->PlayBar_AddSection(&sectionInfo, unElementID);
	while(sectionInfo.pNext)
	{
		SECTIONINFO_FOR9000 * pSecNext = sectionInfo.pNext;
		sectionInfo.pNext = pSecNext->pNext;
		delete pSecNext;
	}
}

//录像类型转换
int CPlayVedioDlg::GetGraphRecordType(int nRecordType)
{
	//0定时录像，1报警录像,2手动录像
	int nRetType;
	switch(nRecordType)
	{
	case (int)RECORD_TYPE_ALL:
	case (int)RECORD_TYPE_TIMING:
	case (int)RECORD_TYPE_BACKUP:
	case (int)RECORD_TYPE_EVENT:
		{
			nRetType = 0;
		}
		break;
	case (int)RECORD_TYPE_MOVE:
	case (int)RECORD_TYPE_ALARM_TRIGER:
	case (int)RECORD_TYPE_ALARM_OR_MOVE:
	case (int)RECORD_TYPE_ALARM_AND_MOVE:
	case (int)RECORD_TYPE_CMD_TRIGER:
		{
			nRetType = 1;
		}
		break;
	case (int)RECORD_TYPE_MANUAL_RECORD:
		{
			nRetType = 2;
		}
		break;
	default: 
		nRetType = 0;
	}

	return nRetType;
}

////获取登录信息
//BOOL CPlayVedioDlg::GetLoginInfo(int nDevID, int nCameraID, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO & stLoginInfo)
//{
//	//获取设备信息
//	Device deviceInfo;
//	if (!theApp.GetDeviceInfo(nDevID, deviceInfo))
//	{
//		return FALSE;
//	}
//
//	PB_DVRLOGININFO stDvrLoginInfo;
//	CStringW strIp = UTF8_A2MFCSTRW(deviceInfo.ip());
//	CStringW strUser = UTF8_A2MFCSTRW(deviceInfo.login_name());
//	CStringW strPsw = UTF8_A2MFCSTRW(deviceInfo.login_pwd());
//	char* szIp = NULL;
//	char* szUser = NULL;
//	char* szPsw = NULL;
//	int nLength;
//	CP936_W2A(strIp.GetBuffer(), strIp.GetLength()+1, szIp, nLength);
//	CP936_W2A(strUser.GetBuffer(), strUser.GetLength()+1, szUser, nLength);
//	CP936_W2A(strPsw.GetBuffer(), strPsw.GetLength()+1, szPsw, nLength);
//	sprintf_s(stDvrLoginInfo.szIP,16, szIp);
//	sprintf_s(stDvrLoginInfo.szUserName, 32, szUser);
//	sprintf_s(stDvrLoginInfo.szPassword, 32, szPsw);
//	stDvrLoginInfo.nPort = deviceInfo.port();
//	stLoginInfo.emDevType = (ENUM_DEVICE_TYPE)deviceInfo.man_type();
//	stLoginInfo.emPlayType = emPlayType;
//	stLoginInfo.unLoginType.stDvrLoginInfo = stDvrLoginInfo;
//	CodeSet_Free(szIp);
//	CodeSet_Free(szUser);
//	CodeSet_Free(szPsw);
//
//	return TRUE;
//}

//工具栏按钮点击响应消息
LRESULT CPlayVedioDlg::OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam)
{
	switch ((TOOLBAR_CTRL_ID)wParam)
	{
	case TOOL_PLAY_ID://播放 
	case TOOL_STOP_ID://停止
	case TOOL_STOPALL_ID://停止全部  
	case TOOL_FRAME_ID://单帧播放 
	case TOOL_CAPTURE_ID://抓图 
	case TOOL_SAVEDATA_ID://保存 
	case TOOL_CHECKBOX_VOICE://音频 
	case TOOL_SLIDER_SPEED://速度
	case TOOL_SLIDER_VOLUMN://音量
		{
			if (m_pPlayViewWnd)
			{
				m_pPlayViewWnd->OnButtonClick(wParam, lParam);
			}
		}
		break;   
	case TOOL_FULLSCREEN_ID://全屏
		{
			WndFullScreenSet();
		}
		break;
	case TOOL_SOUND_ID:
		{
			if (m_pVolumnSetWnd == NULL)
			{
				m_pVolumnSetWnd = new CVolumnSetWnd;
				m_pVolumnSetWnd->CreateFloatWindow(this);
				CRect rcVoice;
				if (m_pToolBarWnd)
				{
					m_pToolBarWnd->m_btnSound.GetWindowRect(&rcVoice);
					m_pVolumnSetWnd->MoveWindow(rcVoice.left-60, rcVoice.top-34, 110, 28);
					BOOL bVoice;
					int nVolumn;
					if (m_pPlayViewWnd->GetCurSelWndVolumn(bVoice, nVolumn))
					{
						m_pVolumnSetWnd->SetInitPos(nVolumn);
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

//录像彩条的定位消息
LRESULT CPlayVedioDlg::OnMsgTimeBarSetPos(WPARAM wParam, LPARAM lParam)
{
	LPRecordBarDbClickData pData = (LPRecordBarDbClickData)lParam;
	if (NULL == pData)
	{
		return 0;
	}
	//time_t tTime = (time_t)lParam;
	time_t tCur = pData->tCur;
	time_t tStart = pData->tStart;
	time_t tStop = pData->tStop;
	CTime timeSel(tCur);
	CTime timeStart(tStart);
	CTime timeEnd(tStop);
	int nElementID = (int)wParam;

	PB_TIME stSelTime;
	stSelTime.dwYear = timeSel.GetYear();
	stSelTime.dwMonth = timeSel.GetMonth();
	stSelTime.dwDay = timeSel.GetDay();
	stSelTime.dwHour = timeSel.GetHour();
	stSelTime.dwMinute = timeSel.GetMinute();
	stSelTime.dwSecond = timeSel.GetSecond();

	PB_TIME stStartTime;
	stStartTime.dwYear = timeStart.GetYear();
	stStartTime.dwMonth = timeStart.GetMonth();
	stStartTime.dwDay = timeStart.GetDay();
	stStartTime.dwHour = timeStart.GetHour();
	stStartTime.dwMinute = timeStart.GetMinute();
	stStartTime.dwSecond = timeStart.GetSecond();

	PB_TIME stEndTime;
	stEndTime.dwYear = timeEnd.GetYear();
	stEndTime.dwMonth = timeEnd.GetMonth();
	stEndTime.dwDay = timeEnd.GetDay();
	stEndTime.dwHour = timeEnd.GetHour();
	stEndTime.dwMinute = timeEnd.GetMinute();
	stEndTime.dwSecond = timeEnd.GetSecond();

	if (m_pPlayViewWnd)
	{
		m_pPlayViewWnd->SetPlayTime(stSelTime, stStartTime, stEndTime, nElementID);
	}

	SAFEDELETE(pData);

	return 0;
}

//工具栏按钮状态刷新消息
LRESULT CPlayVedioDlg::OnMsgToolBarUpdate(WPARAM wParam, LPARAM lParam)
{
	if (m_pToolBarWnd)
	{
		m_pToolBarWnd->UpdateBtnState((LPVOID)wParam);
	}

	return 0;
}

// 文件列表切换.
LRESULT CPlayVedioDlg::OnFileListChange(WPARAM wParam, LPARAM lParam)
{
	// 向发窗口发送消息,响应文件列表的文件切换显示.
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_FILELIST_CHANGE, wParam, lParam);
	return 0;
}

//全屏切换消息
void CPlayVedioDlg::WndFullScreenSet()
{
	m_bFullScreen = !m_bFullScreen;
	if (m_pFullScreenWnd == NULL)
	{
		return;
	}

	if (m_bFullScreen)
	{
		//保存全屏前的位置
		GetWindowRect(&m_rcWndPos);
		GetParent()->ScreenToClient(&m_rcWndPos);

		//获取当前处于哪个屏幕
		HMONITOR hMonitor = MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hMonitor, &mi);
		//设置全屏
	    SetParent(m_pFullScreenWnd);
		m_pToolBarWnd->UpdateButtonTips();
		m_pFullScreenWnd->MoveWindow(&(mi.rcMonitor));
		CRect rcFullScreen;
		m_pFullScreenWnd->GetClientRect(&rcFullScreen);
		MoveWindow(rcFullScreen);
		m_pFullScreenWnd->ShowWindow(SW_SHOW);
		ShowWindow(SW_SHOW);
	}
	else
	{
		SetParent(m_pParentWnd);
		m_pToolBarWnd->UpdateButtonTips();
		m_pFullScreenWnd->ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
		MoveWindow(&m_rcWndPos);
	}

	//更新全屏按钮
	if(m_bFullScreen)
	{
		m_pToolBarWnd->SetFullScreenOn(TRUE);
	}
	else
	{
		m_pToolBarWnd->SetFullScreenOn(FALSE);
	}
}

//分组回放
BOOL CPlayVedioDlg::PlayGroup(int nGroupID, const PLAY_PARAM& stPlaybackParam)
{
#if 0
	//获取分组信息
	PlaybackGroupList listGroup;
	CString strLoginUser;
	strLoginUser.Format(_T("%s"), theApp.m_stDataInfo.szLoginUser);

	if (!theApp.GetPlayGroupList(listGroup, UTF8_MFCSTRW2A(strLoginUser)))
	{
		MsgBoxApi_Show(_T("获取回放分组列表失败"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}

	BOOL bFind = FALSE;
	PlaybackGroup pbGroup;
	for (int i=0; i<listGroup.playback_group_size(); i++)
	{
		pbGroup = listGroup.playback_group(i);
		if (pbGroup.id() == nGroupID)
		{
			bFind = TRUE;
			break;
		}
	}
	if (!bFind)
	{
		MsgBoxApi_Show(_T("未找到回放分组"), MSGBOX_TYPE_WARNING);
		return FALSE;
	}

	//回放
	if (m_pPlayViewWnd)
	{
		int nLayout = pbGroup.split_mode();
		m_pPlayViewWnd->SetWndLayout((PLAY_LAYOUT_WND)nLayout);
		//m_pPlayViewWnd->StopAll();
		// 清理所有.
		Clear(TRUE);
		PlaybackInfo pbPlaybackInfo;
		for (int i=0; i<pbGroup.playback_info_size(); i++)
		{
			pbPlaybackInfo = pbGroup.playback_info(i);
			Playback(pbPlaybackInfo.id(), stPlaybackParam, i);
 
			//录像条
			std::vector<PB_FILEINFO> vecFileList;
			if (FindFile(pbPlaybackInfo.id(), stPlaybackParam, vecFileList))
			{
				BuildRecordGraph(vecFileList, i);
			}
		}
	}
#endif
	return TRUE;
}

//音量浮动窗口关闭消息
LRESULT CPlayVedioDlg::OnMsgVolumnWndClose(WPARAM wParam, LPARAM lParam)
{
	if (m_pVolumnSetWnd)
	{
		m_pVolumnSetWnd->DestroyWindow();
		delete m_pVolumnSetWnd;
		m_pVolumnSetWnd = NULL;
	}

	return 0;
}

/** @fn			GetCursorWnd
*	@brief		获取鼠标所在窗口.
*	@param
*	@param
*	@return		窗口号.
*/
int CPlayVedioDlg::GetCursorWnd()
{
	if (NULL == m_pPlayViewWnd)
	{
		return -1;
	}

	return m_pPlayViewWnd->GetCursorWnd();
}

/** @fn			GetSelectedWnd
*	@brief		获取选中的窗口.
*	@param
*	@param
*	@return		窗口号.
*/
int CPlayVedioDlg::GetSelectedWnd()
{
	if (NULL == m_pPlayViewWnd)
	{
		return 0;
	}

	return m_pPlayViewWnd->GetCurSelectedWnd();
}

LRESULT CPlayVedioDlg::OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam)
{
	// 回放参数.
	LPOrderPlayData pData = (LPOrderPlayData)wParam;
	// 文件序号.
	int nIndex = (int)lParam;

	if (m_pPlayViewWnd != NULL)
	{
		PLAYBACKDLL_INFO(_T("Playback next file."));

		// 在原来的窗口播放下一个文件.
		if (PlaybackFile(
			pData->nCameraID, 
			pData->strCamName, 
			pData->nChannel,
			pData->szStreamID,
			pData->stFileInfo, 
			pData->stLoginInfo, 
			pData->nWndID, 
			pData->dwRight,
			pData->dwRev))
		{
			m_pPlayViewWnd->SetCurPlayFileIndex(pData->nWndID, nIndex);
		}

		std::vector<PB_FILEINFO> vecFileInfo;
		vecFileInfo.push_back(pData->stFileInfo);

		// 重绘彩条.
		BuildRecordGraph(vecFileInfo, pData->nWndID);
	}

	SAFEDELETE(pData);

	return 0;
}

LRESULT CPlayVedioDlg::OnMsgUpdateGroup(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_GROUPCFG_CHANGE, wParam, lParam);
	return 0;
}

LRESULT CPlayVedioDlg::OnMenuCallBack(WPARAM wParam, LPARAM lParam)
{
	// 如果是全屏状态，则先退出全屏状态.
	if (m_bFullScreen)
	{
		WndFullScreenSet();
	}
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_MENU_CALLBACK, wParam, lParam);
	return 0;
}

// 查找文件成功.
LRESULT CPlayVedioDlg::OnMsgFindSuccess(WPARAM wParam, LPARAM lParam)
{
	int nWndID = (int)wParam;
	if (m_pPlayViewWnd != NULL)
	{
		// 绘制录像彩条.
		BuildRecordGraph(m_pPlayViewWnd->GetFileInfo(nWndID), nWndID);
	}
	::PostMessage(GetParent()->GetSafeHwnd(), MSG_PLAYBACK_FIND_SUCCESS, wParam, lParam);
	return 0;
}

// 查找文件失败.
LRESULT CPlayVedioDlg::OnMsgFindFail(WPARAM wParam, LPARAM lParam)
{
	int nWndID = (int)wParam;
	// 清空录像彩条.
	Clear(nWndID);
	return 0;
}

/**   @fn          GetFileInfo
 *    @brief   	   获取文件列表的引用.
 *    @param[in]   nWndID:文件窗口ID.
 *    @param[in]   
 *    @return      文件列表引用.
 */
std::vector<PB_FILEINFO>& CPlayVedioDlg::GetFileInfo(int nWndID)
{
	return m_pPlayViewWnd->GetFileInfo(nWndID);
}

/**   @fn          GetFileInfo
 *    @brief   	   获取单个文件信息.
 *    @param[in]   nIndex:文件索引.
 *    @param[in]   nWnd:窗口ID.
 *    @param[out]  stFileFInfo:文件信息.
 *    @return      文件信息引用.
 */
BOOL CPlayVedioDlg::GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo)
{
	return m_pPlayViewWnd->GetFileInfo(nWndID, nIndex, stFileInfo);
}

/**   @fn          GetCurSelectedWnd
 *    @brief   	   获取光标所在的窗口.
 *    @param[in]   
 *    @param[in]   
 *    @return      窗口号.
 */
int CPlayVedioDlg::GetCurSelectedWnd()
{
	if (NULL == m_pPlayViewWnd)
	{
		return -1;
	}

	return m_pPlayViewWnd->GetCurSelectedWnd();
}

/**   @fn          Clear
 *    @brief   	   清空录像彩条.
 *    @param[in]   nWndID:窗口号.
 *    @param[in]   
 *    @return      
 */
void CPlayVedioDlg::Clear(int nWndID)
{
	m_pPlayBarTimeCtrl->PlayBar_ClearSection(nWndID);
	m_pPlayBarTimeCtrl->PlayBar_InitTimeSpan(m_tMin, m_tMax);
}

//传入下载窗口指针
void CPlayVedioDlg::SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg)
{
	m_pDownloadDlg = pDownloadDlg;
}
/**   @fn          OnMsgMiniToolbarDownload
 *    @brief   	   按时间下载.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
LRESULT CPlayVedioDlg::OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam)
{
	int nWndID = (int)wParam;
	std::vector<PB_FILEINFO> vecFileList = m_pPlayViewWnd->GetFileInfo(nWndID);
	PB_LOGININFO stLoginInfo = m_pPlayViewWnd->GetLoginInfo(nWndID);
	BOOL bShowDownTimeTab = TRUE;
	//以下类型不支持按时间下载
	if (PB_TYPE_CLOUD == stLoginInfo.emPlayType
		|| PB_TYPE_CVR == stLoginInfo.emPlayType
		|| PB_TYPE_KMS == stLoginInfo.emPlayType)
	{
		bShowDownTimeTab = FALSE;
	}
	CDownloadTaskAdd dlg(&vecFileList,NULL,bShowDownTimeTab);
	dlg.SetDownloadDlgPoint(m_pDownloadDlg);
	dlg.SetInitInfo(
		m_pPlayViewWnd->GetCameraID(nWndID), 
		m_pPlayViewWnd->GetRevInfo(nWndID),
		m_pPlayViewWnd->GetCamName(nWndID),
		m_pPlayViewWnd->GetChannel(nWndID), 
		m_pPlayViewWnd->GetRight(nWndID), 
		m_pPlayViewWnd->GetPlayType(nWndID),
		&stLoginInfo);
	dlg.DoModal();

	return 0;
}

LRESULT CPlayVedioDlg::OnMsgPlayBackState(WPARAM wParam, LPARAM lParam)
{
	if (theApp.m_emLayoutStyle == LAYOUT_WND_1)
	{
		BOOL bState = (BOOL)wParam;
		if (m_pToolBarWnd)
		{
			m_pToolBarWnd->UpdatePlayState(bState);
		}
		
	}
	return 0;
}

/**   @fn          Invalidate
 *    @brief   	   刷新窗口.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CPlayVedioDlg::Invalidate()
{
	if (NULL == m_pPlayViewWnd  ||  NULL == m_pPlayBarTimeCtrl  ||  NULL == m_pToolBarWnd)
	{
		return;
	}

	for(int i = 0; i < (int)theApp.m_emLayoutStyle; i++)
	{
		m_pPlayViewWnd->Invalidate(i);
		m_pPlayBarTimeCtrl->PlayBar_ClearSection(i);
	}

	CTime ctCur = CTime::GetCurrentTime();
	CTime ctStart = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),0,0,0); 
	time_t ctSpanStart = (time_t)ctStart.GetTime();
	CTime ctEnd = CTime(ctCur.GetYear(),ctCur.GetMonth(),ctCur.GetDay(),23,59,59);     
	time_t ctSpanEnd = (time_t)ctEnd.GetTime();

	m_pPlayBarTimeCtrl->PlayBar_InitTimeSpan(ctSpanStart,ctSpanEnd);

	m_pToolBarWnd->Invalidate();
}