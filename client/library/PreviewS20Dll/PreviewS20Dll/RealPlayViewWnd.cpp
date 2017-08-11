/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			RealPlayViewWnd.cpp
Description:  		预览窗口
Remark:				
Modification History:      
<version >    <time>       <author>    <desc>
1.0          2012-4-10      nobody      创建
*****************************************************/
//lint -library
#include "stdafx.h"
#include "RealPlayViewWnd.h"
#include "PreviewS20Dll.h"
#include "ui/BCMenu.h"
#include "PlayView.h"
#include "PlayviewToolbar.h"

bool g_bFirst = true;

//frame type
#define T_AUDIO16	101
#define T_AUDIO8	100
#define T_UYVY		1
#define T_YV12		3
#define T_RGB32		7

#define		TIMER_HIDE_TOOLBAR			1
#define		TIMER_MAX_RECORDTIME		2
#define		TIMER_CHECK_STREAM		    3    //流检测
#define		TIMER_OPEN_SOUND		    4    //等待2s开启音频

//CCaptureTipDlg *CRealPlayViewWnd::m_pCapDlg;

void CALLBACK PhoneStreamDataCallback(HANDLE hLogin, HANDLE userdata, 
									  long datatype, void *pBuffer, DWORD dwSize)
{
	if (userdata)
	{
		CRealPlayViewWnd *pRealplayDlg = reinterpret_cast<CRealPlayViewWnd*>(userdata);
		pRealplayDlg->StreamDataProc(datatype, pBuffer, dwSize);
	}
}

void CALLBACK PhoneErrorCallback(HANDLE hLogin, HANDLE userdata, int nError)
{
	if (userdata)
	{
		CRealPlayViewWnd *pRealplayDlg = reinterpret_cast<CRealPlayViewWnd*>(userdata);
		if (hLogin != pRealplayDlg->m_hPhoneLogin)
		{
			return;
		}
		pRealplayDlg->PhoneErrorProc(nError);
	}
}

IMPLEMENT_DYNAMIC(CRealPlayViewWnd, CCustomDrawBlackWnd)
CRealPlayViewWnd::CRealPlayViewWnd()
: m_pStreamHead(NULL)
, m_hPreviewhandle(NULL)
, m_pTextFont(NULL)
, m_dwHeadSize(0)
, m_bIsSounding(FALSE)
, m_bIsMouseHover(FALSE)
, m_bIsRecording(FALSE)
, m_bIsOsdShowing(FALSE)
, m_bIsOsdShow4Row(FALSE)
, m_hRecordFile(NULL)
, m_lPort(-1)
, m_nCamID(-1)
, m_nDeviceType(0)
, m_playerType(PT_HIK)
, m_uiWndIndex(-1)
, m_pWndToolBar(NULL)
, m_bFullScreen(FALSE)
, m_bIsSteaming(FALSE)
, m_bFirstCleanup(TRUE)
, m_hPhoneLogin(NULL)
, m_pVideoWav(NULL)
, m_pAudioWav(NULL)
, m_hPlayViewWnd(NULL)
, m_bIsOpenPSClient(FALSE)
, m_bIsRecordNormal(TRUE)
, m_pvFunc(NULL)
, m_lUserID(-1)
, m_nCurStreamType(0)
, m_nOsdInfoShowNum(4)
, m_pCameraDS(NULL)
, m_bIsPreviewing(FALSE)
, m_dwRecFileSize(0)
, m_bFirstShow(FALSE)
, m_bIsShowCaptureInfo(TRUE)
, m_lLastStreamTime(0)
, m_lUSBPort(0)
, m_nCamType(0)
, m_pCapDlg(NULL)
, m_ttStart(0)
, m_ttEnd(0) 
, m_nRecordTimeOut(0)
, m_nRecordTimeOutOld(0)
, m_nDelayTime(-1)
, m_bProIsSound(FALSE)
{
	m_rcClient = CRect(0,0,0,0);

	//设置字体 用于提示信息
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfUnderline = FALSE;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, STR_DRAWTEXT_FONT);
	lf.lfHeight = 160; 
	m_FontBig.CreatePointFontIndirect(&lf);
	lf.lfHeight = 90;
	m_FontSmall.CreatePointFontIndirect(&lf);
	m_tmOld = time(NULL);
    ZeroMemory(&m_stDevInfo, sizeof(NET_DVR_DEVICEINFO_V30));
    ZeroMemory(&m_stShowString, sizeof(m_stShowString));

}

CRealPlayViewWnd::~CRealPlayViewWnd()
{
	//预览句柄置为kong，damn pclint
	m_hPreviewhandle = NULL;
    m_hPlayViewWnd = NULL;
}


BEGIN_MESSAGE_MAP(CRealPlayViewWnd, CCustomDrawBlackWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
    ON_MESSAGE(WM_PV_REALWND_TOOLBAR_CLK, CRealPlayViewWnd::OnToolBarBtnClk)
	ON_MESSAGE(WM_PV_REALWND_NET_EXCEPTION, CRealPlayViewWnd::OnNetException)
END_MESSAGE_MAP()



void CALLBACK DecCallBackFun(long nPort,
                             char * pBuf,
                             long nSize,
                             HIK_FRAME_INFO * pFrameInfo, 
                             long nUser, 
                             long nReserved2)
{
    CRealPlayViewWnd *pWnd = (CRealPlayViewWnd *)nUser;
    if( (pWnd != NULL) && (!pWnd->DealDecodeData(nPort,pBuf,nSize,pFrameInfo)))
    {
        CString strTemp;
        strTemp.Format(_T(" --- DealDecodeData --- 错误,数据类型:%d\n"), pFrameInfo->nType);
        OutputDebugString(strTemp);
    }
}

//流画面回调，用于在画面上画东西
void CALLBACK CRealPlayViewWnd::DrawFunMsgCB(long nPort,HDC hDC,long lUser)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRealPlayViewWnd * pWnd = (CRealPlayViewWnd *)UlongToPtr(lUser);

	
	CRect rcText;					//提示文字矩形
	int nTextHeight = 20;			//字体高度
	pWnd->GetClientRect(&rcText);
	rcText.left = rcText.right - 210; 
	rcText.bottom = nTextHeight;
	
	//生成cdc
	CDC dc ;
	dc.Attach(hDC);

	//本地录像提示
	if (pWnd->IsRecord())
	{
		CString strRecordTip;
// 		COleDateTime dtRecordCurrenttime = COleDateTime::GetCurrentTime();
// 		COleDateTimeSpan timeSpan = dtRecordCurrenttime - pWnd->m_dtRecordTime;
        CTime tmNow = CTime::GetCurrentTime();  //COLEDateTime调用异常，使用CTime::GetCurrentTime
        CTimeSpan timeSpan = tmNow - pWnd->m_dtRecordTime;
		strRecordTip = timeSpan.Format(_T("●录像时长 %H:%M:%S"));

		pWnd->DrawMyText(&dc,strRecordTip,rcText);

		rcText.OffsetRect(0,nTextHeight);
	}

	if (pWnd->IsSound())
	{
		pWnd->DrawMyText(&dc,_T("          ●音频开启"),rcText);
		rcText.OffsetRect(0,nTextHeight);
	}

	dc.Detach();
}

/*************************************************
Function:    	DrawMyText
Description:	流画面上显示提示信息
Input:		    pDC 刘画面dc
				csDrawText 提示信息文本
				rcDraw 提示信息所在区域
Output:      	
Return:			是否窗口创建成功
*************************************************/
void CRealPlayViewWnd::DrawMyText(CDC * pDC,CString csDrawText,CRect rcDraw)
{
	if (NULL == pDC || csDrawText.IsEmpty())
	{
		return;
	}

	CFont* pOldFont;
	pOldFont = pDC->SelectObject(m_pTextFont);

	pDC->SetTextColor(RGB(0xff,0,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(csDrawText,csDrawText.GetLength(),&rcDraw,DT_LEFT|DT_VCENTER|DT_WORDBREAK );
	pDC->SelectObject(pOldFont);
}

//onsize 你们都理解是什么意思吧
void CRealPlayViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CCustomDrawBlackWnd::OnSize(nType, cx, cy);

	m_rcClient.right = cx;
	m_rcClient.bottom = cy;

	if (cx< 300)
	{
		m_pTextFont = &m_FontSmall;
	}
	else
	{
		m_pTextFont = &m_FontBig;
	}

    // 视频窗口工具条
	CRect rcToolBar(m_rcClient);
	rcToolBar.top = rcToolBar.bottom - 28;
	if (m_pWndToolBar)
	{
		m_pWndToolBar->MoveWindow(rcToolBar);
	}
}

//右键窗口
void CRealPlayViewWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	//设置窗口焦点
	CPlayView *pParrentWnd;
	pParrentWnd = (CPlayView *)GetParent();
	pParrentWnd->SetChildWndZoom(m_uiWndIndex);

	if (!IsPreview())
	{
		return ;
	}
    PopupMenu();

	CCustomDrawBlackWnd::OnRButtonDown(nFlags, point);
}

void CRealPlayViewWnd::OnDestroy()
{
    StopPreview();

    KillTimer(TIMER_HIDE_TOOLBAR);
	KillTimer(TIMER_CHECK_STREAM);

	if (m_pWndToolBar != NULL)
	{
		m_pWndToolBar->DestroyWindow();
		delete m_pWndToolBar;
		m_pWndToolBar = NULL;
	}
    //关闭录像句柄 damn pclint
    if (m_hRecordFile)
    {
        CloseHandle(m_hRecordFile);
        m_hRecordFile = NULL;
    }
    //删除头数据
    if(NULL != m_pStreamHead)
    {
        delete []m_pStreamHead;
        m_pStreamHead = NULL;
    }
}


void CRealPlayViewWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//设置窗口焦点
	CPlayView *pParrentWnd;
	pParrentWnd = (CPlayView *)GetParent();
	pParrentWnd->SetChildWndZoom(m_uiWndIndex);

	if (!IsPreview())
	{
		return;
	}

    if (IsPreview() && !m_strShowMsg.IsEmpty())
    {
        return;
    }
	SetCapture();
	CCustomDrawBlackWnd::OnLButtonDown(nFlags, point);
}

void CRealPlayViewWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!IsPreview())
	{
		return;
	}

	if (!m_bIsMouseHover)
	{
		m_bIsMouseHover = TRUE;

		CPlayView *pParrentWnd;
		pParrentWnd = (CPlayView *)GetParent();
		pParrentWnd->ShowToolBar(m_uiWndIndex);
	}

	CCustomDrawBlackWnd::OnMouseMove(nFlags, point);
}

void CRealPlayViewWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPlayView *pParrentWnd;
	pParrentWnd = (CPlayView *)GetParent();

	if (!IsPreview())
	{
		return;
	}

	if (GetCapture() == this)
	{
		ReleaseCapture();
	}
	ReleaseCapture();

	CCustomDrawBlackWnd::OnLButtonUp(nFlags, point);
}

/*************************************************
Function:    	CreateWnd
Description:	创建窗口
Input:		    REALPLAYWND_TYPE emType,窗口类型
				rect  窗口的位置
				pParent  父窗口指针
				uiID     窗口id
				uiIndex  窗口索引号
Output:      	
Return:			是否窗口创建成功
*************************************************/
BOOL CRealPlayViewWnd::CreateWnd(const RECT & rect, CWnd * pParent,unsigned int uiID,unsigned int uiIndex)
{
	CWnd * pWnd = this;
	
	if (pWnd->Create(NULL, NULL, WS_VISIBLE|WS_CHILD|WS_TABSTOP|WS_CLIPCHILDREN, rect, pParent,uiID))
	{
		m_hPlayViewWnd = GetParent()->GetParent()->GetSafeHwnd();
		//记录窗口信息
		m_uiWndIndex = uiIndex;
		
		Wnd_SetBakeColor(theApp.GetUIColor(UI_COLOR_TYPE_RW));
		return TRUE;
	}
	return FALSE;
}


/*************************************************
Function:    	StartPreview
Description:	开始视频预览
Input:		    Struct_CamreaInfo struCamInfo,监控点信息
                BOOL bWriteLog，是否写日志
Output:      	
Return:			是否预览成功
*************************************************/
BOOL CRealPlayViewWnd::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
	StopPreview();
	//m_bIsPreviewing = TRUE;
	m_nCurStreamType = struPreviewInfo.nRealPlayType;
	m_nRecordTimeOut = struPreviewInfo.nTimeout;
	m_nRecordTimeOutOld = struPreviewInfo.nTimeout;
	m_nDelayTime = struPreviewInfo.nDelayTime;

	BOOL bRet = FALSE;
	int nCamType = struPreviewInfo.nCamType;
	m_nCamType = nCamType;
	PV_INFO("camType:%d", nCamType);
	switch(nCamType)
	{
	case PV_CAMTPYE_DVR:
		{
			bRet = OpenVSClient(struPreviewInfo);
			break;
		}
	case PV_CAMTPYE_NVR:
		{
			bRet = OpenVSClient(struPreviewInfo);
			break;
		}
	case PV_CAMTPYE_USB:
		{
			bRet = USBStartPreview(struPreviewInfo);
			break;
		}
	default:
		break;
	}

	if (bRet)
	{
		if (NULL != m_pWndToolBar)
		{
			m_pWndToolBar->ShowWindow(SW_SHOW);
		}
		::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STOP_PREVIEW,1);
		if (m_nCurStreamType == 0)
		{
			PV_INFO("主码流.");
			::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STREAMTYPE,0);
		}
		else
		{
			PV_INFO("子码流.");
			::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STREAMTYPE,1);
		}
		PV_INFO("预览成功.");
		m_bIsPreviewing = TRUE;
		return TRUE;
	}
	PV_INFO("预览失败.");
	return FALSE;
}

/*************************************************
Function:    	PreparStreamInfo
Description:	根据监控点信息，准备预览数据
Input:			pCameraInfo,监控点信息
Output:      	
Return:			流层预览信息结构
*************************************************/
BOOL CRealPlayViewWnd::PreparStreamInfo(const PREVIEW_CAMINFO &struPreviewInfo, STREAMPARAMS &stStreamParam)
{
    CString strTips;

	//设备信息
	strcpy_s(stStreamParam.device.szName, CStringW2A(struPreviewInfo.strDevIP));
	strcpy_s(stStreamParam.device.szUsername, CStringW2A(struPreviewInfo.strUser));
	strcpy_s(stStreamParam.device.szPassword, CStringW2A(struPreviewInfo.strPwd));
	stStreamParam.device.nPort = struPreviewInfo.nDevPort;

	//设备类型
	switch (struPreviewInfo.nDevType)
	{
	case PV_DEVTPYE_HIKVISON:
		stStreamParam.device.nDeviceType = SL_DEV_HIK;
		break;
	default:
		stStreamParam.device.nDeviceType = struPreviewInfo.nDevType;
		break;
	}

	//其他字段
	stStreamParam.device.nType = 0;									//固定为IP格式段
	stStreamParam.nChannel = struPreviewInfo.nChannel;				//通道号
	stStreamParam.nSubChannel = struPreviewInfo.nRealPlayType;		//主码流 子码流
	stStreamParam.nMode = struPreviewInfo.nProtocol;				//协议类型：0-TCP, 1-UDP
	stStreamParam.hUserData = this;									//回调UserData
	stStreamParam.nType = CONNECT_DEVICE;
	stStreamParam.path.nTransmit = 0;


	strTips.Format(_T("Camera[%s]  Info[%s:%d]..."), struPreviewInfo.strCamName, struPreviewInfo.strDevIP, struPreviewInfo.nDevPort);
	PV_INFO("%s", CStringW2A(strTips));

	return TRUE;
}

/*************************************************
Function:    	PreviewStreamCallback
Description:	视频流数据回调函数
Input:		
Output:      	hStream 开始取流返回的句柄
				hUserData 用户数据
				pStreamDataInfo 流数据
Return:			
*************************************************/
void CALLBACK CRealPlayViewWnd::PreviewStreamCallback(HANDLE hStream, HANDLE hUserData, STREAM_DATA_INFO * pStreamDataInfo)
{
	CRealPlayViewWnd * pWnd = (CRealPlayViewWnd *)hUserData;

	//开始解码
	if (pWnd->m_hPreviewhandle == hStream)
	{
		pWnd->InputData((PBYTE)pStreamDataInfo->pBuff, pStreamDataInfo->nDataLen, pStreamDataInfo->nPacketType);
	}
	
}

/*************************************************
Function:		InputData
Description:	塞入数据  未进行帧分析的数据
Input:			pBuffer 数据指针地址
				dwSize 数据大小
				lFrameType 数据的类型
Output:      	
Return:			
*************************************************/
BOOL CRealPlayViewWnd::InputData(PBYTE pBuffer,DWORD dwSize,long lFrameType)
{
	if (m_bFirstCleanup)
	{
		m_strShowMsg.Empty();
		m_bFirstCleanup = FALSE;
	}
	m_lLastStreamTime = (LONG)CTime::GetCurrentTime().GetTime();

	//保存头数据，录像的时候用到
	int nType = 0;
	if(nType == lFrameType)
	{
// 		if(NULL != m_pStreamHead)
// 		{   这里的判断完全多余了
// 			delete []m_pStreamHead;
// 			m_pStreamHead = NULL;
// 		}

		m_pStreamHead = new BYTE[dwSize];
        if (NULL != m_pStreamHead)
        {
            memcpy(m_pStreamHead,pBuffer,dwSize);
            m_dwHeadSize = dwSize;
        }
	}

	//实时录像数据保存
	if (m_bIsRecording)
	{
		DWORD dwReturn = 0;
		if (NULL != m_hRecordFile)
		{
			//检查是否超过最大大小
			DWORD dwFileSize;
			dwFileSize = GetFileSize(m_hRecordFile, NULL);
           /*   暂时去掉录像文件达到一定容量重新分文件录像
			if (dwFileSize > m_dwRecFileSize)
			{
				PV_INFO("InputData");
				StopRecord();
				StartRecord();
			}
			else    */
			{
				BOOL bWrite = TRUE;
				if (bWrite)
				{
					WriteFile(m_hRecordFile,pBuffer,dwSize,&dwReturn,NULL);
				}
			}	
		}
		else
		{
// 			int iLen = m_stLastRecordFile.strFileName.GetLength();    
// 			TCHAR* szRs = new TCHAR[iLen];
// 			lstrcpy(szRs, m_stLastRecordFile.strFileName.GetBuffer(iLen));    
// 			m_stLastRecordFile.strFileName.ReleaseBuffer();
// 			m_hRecordFile = AppFun_CreateFile(szRs);
// 			DWORD dwErr = GetLastError();
//             delete []szRs;  
//             szRs = NULL;
			int iLen = m_stLastRecordFile.strFileName.GetLength();    
			TCHAR szRs[MAX_PATH] = {0};
            _tcsncpy_s(szRs, MAX_PATH, m_stLastRecordFile.strFileName.GetBuffer(iLen), _TRUNCATE);
			//lstrcpy(szRs, m_stLastRecordFile.strFileName.GetBuffer(iLen));    
			m_stLastRecordFile.strFileName.ReleaseBuffer();
			m_hRecordFile = AppFun_CreateFile(szRs);
			DWORD dwErr = GetLastError();
		
			if (NULL != m_hRecordFile)
			{
				// 写头
				WriteFile(m_hRecordFile, m_pStreamHead, m_dwHeadSize, &dwReturn,NULL);
				// 写数据
				WriteFile(m_hRecordFile,pBuffer,dwSize,&dwReturn,NULL);
			}
		}
	}

	//如果是头数据
	if (nType == lFrameType)
	{
		//生成解码类型
		m_playerType = GetPlayerType(m_nDeviceType);
		m_bIsSounding = FALSE;	// 重新打开声音
		return StartPlay(m_playerType, pBuffer, dwSize, m_strCamName);
	}
	if (0 == lFrameType || dwSize == 40)
	{
		CString str;
		str.Format(_T("**********lFrameType = %d, dwSize = %d**********\r\n"), lFrameType, dwSize);
		OutputDebugString(str);
	}

	if (-1 == m_lPort)
	{
		return FALSE;
	}

	if (FALSE == GPAPI_InputData(m_playerType,m_lPort,pBuffer,dwSize))
	{
		CString strMsg = _T("");
		strMsg.Format(_T("@preview %s GPAPI_InputData unsuccessfully, error: %d"), 
			(LPCWSTR)m_strCamName, GPAPI_GetLastError(m_playerType, m_lPort));
		OutputDebugString(strMsg);
		return FALSE;
	}
	return TRUE;
}

/*************************************************
Function:    	ChangeDevType2PlayType
Description:	根据DB的设备类型，转化为通用播放库需要的类型
Input:			int nDevType,设备类型(数据库标记)
Output:      	
Return:			播放需要的类型(通用播放库标记)
*************************************************/
PLAYERTYPE CRealPlayViewWnd::GetPlayerType(int nDevType)
{
	PLAYERTYPE playerType;
	switch(nDevType)
	{
	case PV_DEVTPYE_HIKVISON:
		playerType = PT_HIK;;
		break;
	default:
		playerType = PT_HIK;
		break;
	}
	return playerType;
}


/*******************************************************************************
*    Function:      StartPlay
*    Description:   开始播放
*    Input:         playerType,解码类型
                    PBYTE pBuffer, 数据
                    DWORD dwBufferSize, 数据大小 
                    CString strCamName，通道名称
                        
*    Output:        (NULL)
*    Return:	    return void  
*******************************************************************************/
BOOL CRealPlayViewWnd::StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize, CString strCamName /*= _T("")*/) 
{
	CString strMsg;

	//GetPort
	LONG nPort = -1;
	if (!GPAPI_GetPort(playerType,nPort,GetSafeHwnd()))
	{
		strMsg.Format(_T("@preview %s GPAPI_GetPort unsuccessfully"),strCamName);
		OutputDebugString(strMsg);
		PV_ERROR("GPAPI_GetPort falid, playtype = %d, port = %d, errorcode = %d", playerType, nPort, GPAPI_GetLastError(playerType, nPort));
		
		return FALSE;
	}

	//保存port
	m_lPort = nPort;

	if (NULL != m_pVideoWav)
	{
		m_pVideoWav->OpenScope();
		m_pVideoWav->SetZoomNumber(5);
	}

    //设置解码回调
    if(!GPAPI_SetDecCallBack(m_playerType, m_lPort, DecCallBackFun, NULL, 0, (LONG)this))
    {
		PV_ERROR("GPAPI_SetDecCallBack falid, playtype = %d, port = %d, errorcode = %d", playerType, nPort, GPAPI_GetLastError(playerType, nPort));
    }

	char *cCamName = NULL;
	int nLength;
	CP936_W2A(strCamName, strCamName.GetLength() + 1, cCamName, nLength);

	if (-1 == GPAPI_StartPlay(m_playerType,m_lPort,GetSafeHwnd(), pBuffer,dwBufferSize,cCamName,DrawFunMsgCB,(long)this))
    {
        StopPlay();
		CodeSet_Free(cCamName);
		PV_ERROR("GPAPI_StartPlay falid, playtype = %d, port = %d, errorcode = %d", playerType, nPort, GPAPI_GetLastError(playerType, nPort));
        return FALSE;
    }

	CodeSet_Free(cCamName);

	return TRUE;
}

/*************************************************
Function:    	StopPreview
Description:	停止视频预览
Input:		    
Output:      	
Return:			是否停止预览成功
*************************************************/
BOOL CRealPlayViewWnd::StopPreview()
{
	PV_INFO("停止预览.");
	BOOL bRet = FALSE;
	m_lLastStreamTime = 0; 
	if (m_bIsPreviewing)
	{
		int nCamType = m_nCamType;
		switch(nCamType)
		{
		case PV_CAMTPYE_DVR:
			{
				ClosePSCLient();
				bRet = CloseVSClient();
				break;
			}
		case PV_CAMTPYE_NVR:
			{
				ClosePSCLient();
				bRet = CloseVSClient();
				break;
			}
		case PV_CAMTPYE_USB:
			{
				PV_INFO("Before USBStopPreview：%d", m_stCamInfo.lCamIndex);
				bRet = USBStopPreview(m_stCamInfo.lCamIndex);
				PV_INFO("After USBStopPreview：%d", m_stCamInfo.lCamIndex);
				break;
			}
		default:
			break;
		}
		if (NULL != m_pWndToolBar)
		{
			m_pWndToolBar->ShowWindow(SW_HIDE);
		}
		m_bIsPreviewing = FALSE;
		m_bIsOsdShowing = FALSE;
	}
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STOP_PREVIEW,0);
	return bRet;
}

/*******************************************************************************
*    Function:      StopPlay
*    Description:   停止播放
*    Input:         
*    Output:       
*    Return:	     
*******************************************************************************/
void CRealPlayViewWnd::StopPlay()
{
	PV_INFO("停止播放.");
	if (-1 != m_lPort)
	{
		CString strMsg;
		strMsg.Format(_T("@preview %d StopPlay"),m_uiWndIndex);
		OutputDebugString(strMsg);

		GPAPI_StopSoundShare(m_playerType, m_lPort);
		GPAPI_StopPlay(m_playerType,m_lPort);
		GPAPI_FreePort(m_playerType,m_lPort);

		//数据恢复初始
		m_lPort = -1;
        
		if (NULL != m_pVideoWav && m_pVideoWav->IsOpen())
		{
			m_pVideoWav->CloseScope();
		}
	}

    m_nCamID = -1;
}

/*************************************************
Function:    	OpenSound
Description:	播放音频
Input:		    
Output:      	
Return:			是否成功
*************************************************/
BOOL CRealPlayViewWnd::OpenSound()
{
	if (!IsPreview())
	{
		return FALSE;
	}

	if (m_bIsSounding)
	{
		return FALSE;
	}

	BOOL bRet = GPAPI_PlaySoundShare(m_playerType, m_lPort);
	if (bRet)  
	{
		m_bIsSounding = TRUE;
	}
	else
	{
        PV_ERROR("GPAPI_PlaySoundShare falid, playtype = %d, port = %d, errorcode = %d, errInfo:%s", m_playerType, m_lPort, GPAPI_GetLastError(m_playerType, m_lPort),
            GPAPI_GetLastErrorString(m_playerType, m_lPort));
	}

    if (NULL != m_pVideoWav)
    {
        m_pVideoWav->OpenScope();
        m_pVideoWav->SetZoomNumber(5);
    }
	return bRet;
}

/*************************************************
Function:    	CloseSound
Description:	停止音频
Input:		    
Output:      	
Return:			是否成功
*************************************************/
BOOL CRealPlayViewWnd::CloseSound()
{
	if (!m_bIsSounding)
	{
		return FALSE;
	}

	BOOL bRet = GPAPI_StopSoundShare(m_playerType,m_lPort);
	if (bRet)
	{
		m_bIsSounding = FALSE;
	}
    else
    {
        PV_ERROR("GPAPI_StopSoundShare falid, playtype = %d, port = %d, errorcode = %d, errInfo:%s", m_playerType, m_lPort, GPAPI_GetLastError(m_playerType, m_lPort),
            GPAPI_GetLastErrorString(m_playerType, m_lPort));
    }

    if (NULL != m_pVideoWav)
    {
        m_pVideoWav->CloseScope();
    }

	return bRet;
}

/*************************************************
Function:    	PopupMenu
Description:	窗口右键菜单构成函数
Input:		    
Output:      	
Return:			
*************************************************/
void CRealPlayViewWnd::PopupMenu(BOOL bJustClose/* = FALSE*/)
{
// 	CPoint point;
// 	GetCursorPos(&point);

    if (theApp.m_stPreviewParam.nHoverToolBarRight == 0)
    {
        return;
    }

	if (m_bIsSounding == FALSE)
	{
		return;
	}

	BCMenu menuPopup;
	menuPopup.CreatePopupMenu();

    if (!bJustClose && 0 == theApp.m_nPreviewModel)
    {
        BOOL bHasMenu = FALSE;
		//支持抓图功能
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_CAPTURE)
		{
			bHasMenu = TRUE;
			menuPopup.AppendMenu(MF_STRING, MENU_ID_CAPTURE, STR_POPUPMENU_SNAPSHOT);
		}

        //支持录像功能
        if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_RECORD)
        {
            bHasMenu = TRUE;
            if(IsRecord())
            {
                menuPopup.AppendMenu(MF_STRING, MENU_ID_RECORD,STR_POPUPMENU_STOPRECORD);
            }
            else
            {
                if (m_bIsRecording)
                {
                    menuPopup.AppendMenu(MF_STRING, MENU_ID_RECORD,STR_POPUPMENU_STOPRECORD);
                } 
                else
                {
                    menuPopup.AppendMenu(MF_STRING ,MENU_ID_RECORD,STR_POPUPMENU_STARTRECORD);
                }
            }
        }
        //支持声音开关操作
        if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_AUDIO)
        {
            bHasMenu = TRUE;
            if (IsSound())
            {
                menuPopup.AppendMenu(MF_STRING, MENU_ID_SOUND, STR_POPUPMENU_CLOSESOUND);
            }
            else
            {
                menuPopup.AppendMenu(MF_STRING, MENU_ID_SOUND, STR_POPUPMENU_OPENSOUND);
            }
        }

		//支持主子码流切换（清晰度切换）
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STREAM_TYPE)
		{
			bHasMenu = TRUE;
			if (m_nCurStreamType == 0)
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STREAMTYPE, STR_POPUPMENU_SUB_STREAM);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_STREAMTYPE, STR_POPUPMENU_MAIN_STREAM);
			}
		}

		if (0 != theApp.m_nBtnRights)
		{
			if (m_nCamType != PV_CAMTPYE_USB)
			{
				//OSD信息
				if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_OSDINFO)
				{
					bHasMenu = TRUE;
					if (IsShowOsd())
					{
						menuPopup.AppendMenu(MF_STRING, MENU_ID_OSD, STR_POPUPMENU_HIDEOSD);
					}
					else
					{
						menuPopup.AppendMenu(MF_STRING, MENU_ID_OSD, STR_POPUPMENU_SHOWOSD);
					}
				}
			}
		}

        if (bHasMenu)
        {
            menuPopup.AppendMenu(MF_SEPARATOR);
        }
    }
	else
	{
		//支持声音开关操作
		if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_AUDIO)
		{
			if (IsSound())
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SOUND, STR_POPUPMENU_CLOSESOUND);
			}
			else
			{
				menuPopup.AppendMenu(MF_STRING, MENU_ID_SOUND, STR_POPUPMENU_OPENSOUND);
			}
		}
	}

    //停止预览
    if (theApp.m_stPreviewParam.nHoverToolBarRight & PV_HOVER_TOOLBAR_STOP)
    {
        menuPopup.AppendMenu(MF_STRING, MENU_ID_STOP_PREVIEW, STR_POPUPMENU_STOPPREVIEW);
    }

    CPoint point;
    GetCursorPos(&point);
 	int nCmd = menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, this);

	ExcuteSelectOption(nCmd);
}

/*************************************************
Function:    	ExcuteSelectOption
Description:	执行菜单选中的选项
Input:		    
Output:      	
Return:			
*************************************************/
void CRealPlayViewWnd::ExcuteSelectOption(int nCmd)
{
    if (!CheckOperateRights(nCmd))
    {
        MessageBox(_T("抱歉，您没有该操作权限！"), _T("提示"), MB_ICONINFORMATION);
        return;
    }
	switch(nCmd)
	{
		case MENU_ID_CAPTURE:       //抓图
			{
				m_bIsShowCaptureInfo = TRUE;
                if(!CapturePic())
                {
                    PV_ERROR("...CapturePic failed...");
                    MsgBoxApi_Show(_T("手动抓图失败，请重新抓图!"), MSGBOX_TYPE_WARNING);
                }
			}
			break;
		case MENU_ID_RECORD:        //录像
			{
				if (m_bIsRecording)
				{
					PV_INFO("ExcuteSelectOption");
					StopRecord();
					m_bIsRecording = FALSE;
					m_bIsRecordNormal = TRUE;	
				}
				else
                {
					StartRecord();
					m_bIsRecording = TRUE;
					m_bIsRecordNormal = TRUE;
				}
			}
			break;
		case MENU_ID_SOUND:         //声音控制
			if (IsSound())
			{
				CloseSound();
				::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_SOUND,0);
			}
			else
			{
				OpenSound();
				::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_SOUND,1);
			}
			break;
		case MENU_ID_STREAMTYPE:    //清晰度切换
			{
                m_bProIsSound = m_bIsSounding;      //前一个状态
				if (m_bIsRecording)
				{
					MsgBoxApi_Show(_T("正在录像中，不支持码流切换."), MSGBOX_TYPE_WARNING);
					return;
				}
			    if (0 == m_nCurStreamType)
			    {
					StreamTypeSwitch(1);
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STREAMTYPE,1);
					m_nCurStreamType = 1;
			    }
				else
				{
					StreamTypeSwitch(0);
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_STREAMTYPE,0);
					m_nCurStreamType = 0;
				}
                if (m_bProIsSound)
                {
                    SetTimer(TIMER_OPEN_SOUND, 1000, NULL);
                }
			}
			break;
		case MENU_ID_OSD:           //OSD信息显示
			{
				if (m_nCamType == PV_CAMTPYE_USB)
				{
					MsgBoxApi_Show(_T("不支持该操作!"), MSGBOX_TYPE_INFORMATION);
					return;
				}
				if (IsShowOsd())
				{
					HideOSD();
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_OSD,0);
					m_bIsOsdShowing = FALSE;
				}
				else
				{
					ShowOSD();
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_OSD,1);
					m_bIsOsdShowing = TRUE;
				}
			}
            break;
		case MENU_ID_STOP_PREVIEW:  //关闭预览
			{
				if (m_bIsRecording)
				{
					MsgBoxApi_Show(_T("录像中，禁止停止预览."), MSGBOX_TYPE_WARNING);
					return;
				}
			    StopPreview();
			}
			break;
		default:
			break;
	}
}

/** @fn         CaptureByMyself
 *  @brief      自己手动抓图
 *  @return     TRUE: 成功，FALSE: 失败
 */
BOOL CRealPlayViewWnd::CapturePic()
{
    LOCAL_CFG struLocalCfg;
    theApp.GetLocalCfg(struLocalCfg);
    int nPicForm = PF_BMP;
    CString strSuffix = _T("bmp");
    switch (struLocalCfg.stSnapInfo.nCappicMode)
    {
	case PREVIEW_SNAPMODE_BMP:
		{
			nPicForm = PF_BMP;
			strSuffix = _T("bmp");
			break;
		}
    case PREVIEW_SNAPMODE_JPG:
        {
            nPicForm = PF_JPEG;
            strSuffix = _T("jpg");
            break;
        }
    default:
        break;
    }

    // 根据类别计算缓冲大小
    DWORD dwRetSize = 0;
    DWORD dwBufferSize = 25 * 1024 * 1024;
    PBYTE pBuff = NULL;
    switch (m_nDeviceType)
    {
    case PV_DEVTPYE_HIKVISON:
        {
            LONG lWidth = 0;
            LONG lHeight = 0;
            if (!GPAPI_GetPictureSize(m_playerType, m_lPort, &lWidth, &lHeight))
            {
				MsgBoxApi_Show(_T("获取图片大小失败!"), MSGBOX_TYPE_ERROR);
                return FALSE;
            }

//             这里的判断完全没必要了。
//             if (lWidth <= 0 || lHeight <= 0)
//             {
//                 dwBufferSize = 25 * 1024 * 1024;
//             }

            // 播放库给的算法
            if (nPicForm == PF_BMP)
            {
                dwBufferSize = 14 + 40 + lWidth * lHeight * 4;
            } 
            else
            {
                dwBufferSize = lWidth * lHeight * 3 / 2;
            }
        }
        break;
    default:
    	break;
    }
    pBuff = new BYTE[dwBufferSize];     //定义了字符数组 -- 数组释放

    // 开始抓图
    BOOL bRet = GPAPI_CaptureByUserEx(m_playerType, m_lPort, pBuff, dwBufferSize, dwRetSize, (PICTUREFORMAT)nPicForm);
    if (!bRet)
    {
        if (pBuff != NULL)
        {
            delete []pBuff;   //释放数组
            pBuff = NULL;
        }
		MsgBoxApi_Show(_T("预览抓图失败!"), MSGBOX_TYPE_ERROR);
        return FALSE;
    }

    CString strFile;
    SYSTEMTIME timeNow;
    ::GetLocalTime(&timeNow);

 	strFile.Format(_T("%s图片%4u%02u%02u%02u%02u%02u.%s"), struLocalCfg.stSnapInfo.szCappicPath,
		 timeNow.wYear, timeNow.wMonth,timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond, strSuffix);


    // 确保目录的存在
    SuperMakeDir(CStringW2A(strFile).GetBuffer());

    // 保存图片数据
    if (!SaveBinaryFile(pBuff, dwRetSize, strFile.GetBuffer()))
    {
        if (pBuff != NULL)
        {
            delete []pBuff;
            pBuff = NULL;
        }
        PV_ERROR("...[%s]Save Picture Data failed...", CStringW2A(m_stCamInfo.strCamName));
		MsgBoxApi_Show(_T("保存抓图失败，请以管理员身份运行IE，或更改抓图路径!"), MSGBOX_TYPE_WARNING);
        return FALSE;
    }
	m_stLastRecordFile.csPicPath = strFile;

	if (m_bIsShowCaptureInfo)
	{
		// 显示图片数据
		CString strPath(struLocalCfg.stSnapInfo.szCappicPath);
		m_pCapDlg = new CCaptureTipDlg(strPath);
		m_pCapDlg->Create(CCaptureTipDlg::IDD, NULL);
		m_pCapDlg->LoadPic(pBuff, dwRetSize);
		m_pCapDlg->ShowWindow(SW_SHOW);
		delete pBuff;
		pBuff = NULL;
	}

    return TRUE;
}

/*************************************************
Function:    	StartRecord
Description:	开始录像
Input:		    
Output:      	
Return:			TRUE表示操作成功
*************************************************/
BOOL CRealPlayViewWnd::StartRecord()
{
// 	//防止过于频繁操作
// 	time_t tmNow = time(NULL);
// 	if ((tmNow - m_tmOld) < 1)
// 	{
// 		MsgBoxApi_Show(_T("操作过于频繁[1]!"), MSGBOX_TYPE_WARNING);
// 		Sleep(1000);
// 	}
// 	m_tmOld = tmNow;	
	PV_INFO("Start Record!");

	if (!m_bIsPreviewing)
	{
		return TRUE;
	}
	m_bIsRecordNormal = TRUE;
	m_bIsShowCaptureInfo = FALSE;
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_RECORD,1);

	if (m_nCamType == PV_CAMTPYE_USB)
	{
		USBStartRecord();
		return TRUE;
	}

	CString strErrMsg;
	//判断是否在预览
	if (!IsPreview())
	{
		strErrMsg.Format(_T("开始录像失败，监控点未处于预览状态。"));
		SetShowMsg(strErrMsg);
		PV_ERROR("IsPreview() failed!");
		return FALSE;
	}

	if (m_lUserID == -1)
	{
		m_lUserID = NET_DVR_Login_V30(
			const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strDevIP).c_str()), 
			m_stCamInfo.nDevPort, 
			const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strUser).c_str()), 
			const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strPwd).c_str()), 
			&m_stDevInfo);

		if (m_lUserID == -1)
		{
			PV_ERROR("NET_DVR_Login_V30 failed: %d", NET_DVR_GetLastError());
			return FALSE;
		}
	}
	//获取设备磁盘信息
	CString strInfo;
	NET_DVR_HDCFG struHDCfg;
	memset(&struHDCfg,0,sizeof(NET_DVR_HDCFG));
	DWORD BytesReturned = 0;
	BOOL bRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDCFG, 0xFFFFFFFF, &struHDCfg,sizeof(NET_DVR_HDCFG), &BytesReturned);
	if(!bRet)
	{
		LONG dwErrno = NET_DVR_GetLastError();
		std::string szLastErr = NET_DVR_GetErrorMsg(&dwErrno);
		strInfo.Format(_T("获取磁盘信息失败，错误原因(%s)!"), CStringConverter::Ansi2Tstring(szLastErr).c_str());
		if (dwErrno == 23)   //错误为设备不支持
		{
			;  //不处理：兼容DS-8016HF等老设备
			PV_ERROR("获取磁盘信息失败，错误原因(%d)", NET_DVR_GetLastError());
		}
		else
		{
			NET_DVR_Logout_V30(m_lUserID);
			PV_ERROR("获取磁盘信息失败，错误原因(%d)", NET_DVR_GetLastError());
			return FALSE;
		}
	}
	else
	{
		//新设备从未插入过硬盘时会提示无硬盘，无法录像；设备已经插入过硬盘，后来拔掉硬盘，无法给出无硬盘提示。因为dwHDCount的值不会为0！！！
		if (struHDCfg.dwHDCount == 0)
		{
			strInfo.Format(_T("设备没有硬盘存储录像!"));
			PV_ERROR("...[%s]设备没有硬盘存储录像...", CStringW2A(m_stCamInfo.strCamName));
			MsgBoxApi_Show(_T("开始录像失败，设备没有硬盘存储录像!"), MSGBOX_TYPE_ERROR);
			StopRecord();
			return FALSE;
		}
	}

	m_bIsRecording = TRUE;
	SetTimer(TIMER_MAX_RECORDTIME, 500, NULL);
	// 录像类型：0- 手动，1- 报警，2- 回传，3- 信号，4- 移动，5- 遮挡 
	if (!NET_DVR_StopDVRRecord(m_lUserID, m_stCamInfo.nChannel + m_stDevInfo.byStartChan))
	{
		PV_ERROR("NET_DVR_StopDVRRecord failed %d", NET_DVR_GetLastError());
	}
	//开始录像时抓图
	if (!CapturePic())
	{
		PV_ERROR("CapturePic failed !");
	}
	m_dtRecordStartTime = COleDateTime::GetCurrentTime();
	m_dtRecordStartCurTime = CTime::GetCurrentTime();
	//开始录像
	if (!NET_DVR_StartDVRRecord(m_lUserID, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, 0))
	{
		LONG dwErrno = NET_DVR_GetLastError();
		strErrMsg.Format(_T("打开录像失败，错误原因(%d)!"), dwErrno);
		MsgBoxApi_Show(strErrMsg.GetBuffer(), MSGBOX_TYPE_ERROR);

		PV_ERROR("NET_DVR_StartDVRRecord failed, errmsg : %d", dwErrno);
		return FALSE;
	}
	m_dtRecordTime = CTime::GetCurrentTime();
	
	// 获取设备当前时间
	DWORD dwErr = 0;
	if (!GetDvrTimeStamp(m_lUserID, m_ttStart, dwErr))
	{
		m_ttStart = time(NULL);
		PV_ERROR("get dvr time failed, errcode : %ld", dwErr);
	}
	
	// 开始录像是发送录像信息,不填写结束时间,并标记为异常录像，待停止后标记为正常录像
	m_stLastRecordFile.lStartTime = m_ttStart;
	m_stLastRecordFile.lEndTime = 0;
	m_stLastRecordFile.lFileSize = 0;
	m_stLastRecordFile.lFileType = 6;
	m_stLastRecordFile.strDevIP = m_stCamInfo.strDevIP;
	m_stLastRecordFile.nDevPort = m_stCamInfo.nDevPort;
	m_stLastRecordFile.strUser = m_stCamInfo.strUser;
	m_stLastRecordFile.strPwd = m_stCamInfo.strPwd;
	m_stLastRecordFile.nChannel = m_stCamInfo.nChannel;
	m_stLastRecordFile.nRecordStatus = 0;	// 正在采集
	m_stLastRecordFile.strFileName = _T("");
	

	if (m_pvFunc != NULL)
		m_pvFunc(PV_RECORD_START, (void*)&m_stLastRecordFile);

	return TRUE;
}

/*************************************************
Function:    	StopRecord
Description:	停止录像
Input:		    
Output:      	
Return:			TRUE表示操作成功
*************************************************/
BOOL CRealPlayViewWnd::StopRecord()
{
// 	//防止过于频繁操作
// 	time_t tmNow = time(NULL);
// 	if ((tmNow - m_tmOld) < 1)
// 	{
// 		MsgBoxApi_Show(_T("操作过于频繁[2]!"), MSGBOX_TYPE_WARNING);
// 		Sleep(1000);
// 	}
// 	m_tmOld = tmNow;	

	if (!m_bIsRecording)  //录像异常停止
	{
		return FALSE;
	}

	m_bIsShowCaptureInfo = FALSE;
	KillTimer(TIMER_MAX_RECORDTIME);
	//KillTimer(TIMER_CHECK_STREAM);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_RECORD,0);
	if (m_bIsRecording && m_nCamType == PV_CAMTPYE_USB)
	{
		USBStopRecord();
		return TRUE;
	}

	//停止录像时抓图
	if (!CapturePic())
	{
		PV_ERROR("CapturePic failed !");
	}

	if (m_bIsRecordNormal)
	{
		//停止录像并查询录像文件信息/
		if (!NET_DVR_StopDVRRecord(m_lUserID, m_stCamInfo.nChannel + m_stDevInfo.byStartChan))
		{
			PV_ERROR("NET_DVR_StopDVRRecord failed %d", NET_DVR_GetLastError());
		}

		//刷新录像索引
		NET_DVR_UpdateRecordIndex(m_lUserID, m_stCamInfo.nChannel);

		// 获取设备当前时间
		DWORD dwErr = 0;
		if (!GetDvrTimeStamp(m_lUserID, m_ttEnd, dwErr))
		{
			PV_ERROR("get dvr time failed, errcode : %ld", dwErr);
			m_ttEnd = time(NULL);
		}
	}
	else
	{
		PV_ERROR("Record is Not Normal!");
		m_ttEnd = time(NULL);
	}
	
	m_stLastRecordFile.lStartTime = m_ttStart;
	m_stLastRecordFile.lEndTime = m_ttEnd;
	m_stLastRecordFile.lFileType = 6;
	m_stLastRecordFile.lFileSize = 0;
	m_stLastRecordFile.strDevIP = m_stCamInfo.strDevIP;
	m_stLastRecordFile.nDevPort = m_stCamInfo.nDevPort;
	m_stLastRecordFile.strUser = m_stCamInfo.strUser;
	m_stLastRecordFile.strPwd = m_stCamInfo.strPwd;
	m_stLastRecordFile.nChannel = m_stCamInfo.nChannel;
	if (m_bIsRecordNormal)
	{
		m_stLastRecordFile.nRecordStatus = 1;	// 正常状态
	}
	else
	{
		m_stLastRecordFile.nRecordStatus = 8;	// 异常状态
	}

	if (m_pvFunc != NULL)
		m_pvFunc(PV_RECORD_STOP, (void*)&m_stLastRecordFile);

	m_nRecordTimeOut = m_nRecordTimeOutOld;
	m_bIsRecording = FALSE;
	
	return TRUE;
}

/************************************************
Function:			SetEventFunc
Description:		设置事件回调函数
Input:				Func 回调函数
Return:				void
*************************************************/
BOOL CRealPlayViewWnd::SetEventFunc(PV_EVENT_FUNC Func)
{
	m_pvFunc = Func;
	return TRUE;
}

/************************************************
Function:			GetRecordFile
Description:		
Input:				recoedFile 最近一次录制文件信息
Return:				TRUE 成功，FALSE 失败
*************************************************/
BOOL CRealPlayViewWnd::GetRecordFile(RECOED_FILE_INFO& recoedFile)
{
	int nCamType = m_nCamType;
	switch(nCamType)
	{
	case PV_CAMTPYE_DVR:
	case PV_CAMTPYE_NVR:
		{
			recoedFile.strDevIP = m_stLastRecordFile.strDevIP;
			recoedFile.nDevPort = m_stLastRecordFile.nDevPort;
			recoedFile.strUser = m_stLastRecordFile.strUser;
			recoedFile.strPwd = m_stLastRecordFile.strPwd;
			recoedFile.nChannel = m_stLastRecordFile.nChannel;
		}
		break;
	case PV_CAMTPYE_USB:
		{
			recoedFile.strFileName = m_stLastRecordFile.strFileName;
			recoedFile.lFileSize = m_stLastRecordFile.lFileSize;
		}
		break;
	default:
		break;
	}
	recoedFile.lFileType = m_stLastRecordFile.lFileType;
	recoedFile.lStartTime = m_stLastRecordFile.lStartTime;
	recoedFile.lEndTime = m_stLastRecordFile.lEndTime;
	recoedFile.nRecordStatus = m_stLastRecordFile.nRecordStatus;	
	
	return TRUE;
}

BOOL CRealPlayViewWnd::StopRecordByCaminfo(const PREVIEW_CAMINFO &struPreviewInfo)
{
	NET_DVR_DEVICEINFO_V30 deviceInfo;			// 登陆设备信息
	LONG lUserID;								// 登陆设备UserID

	lUserID = NET_DVR_Login_V30(
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)struPreviewInfo.strDevIP).c_str()), 
		m_stCamInfo.nDevPort, 
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)struPreviewInfo.strUser).c_str()), 
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)struPreviewInfo.strPwd).c_str()), 
		&deviceInfo);

	if (lUserID == -1)
	{
		PV_ERROR("NET_DVR_Login_V30 failed %d", NET_DVR_GetLastError());
		return FALSE;
	}

	if (!NET_DVR_StopDVRRecord(lUserID, struPreviewInfo.nChannel + deviceInfo.byStartChan))
	{
		PV_ERROR("NET_DVR_StopDVRRecord failed %d", NET_DVR_GetLastError());
	}
	PV_INFO("Stop  DVR record(UserID:%d, Channel:%d)", lUserID, struPreviewInfo.nChannel + deviceInfo.byStartChan);

	NET_DVR_Logout_V30(lUserID);
	return TRUE;
}

/************************************************
Function:			SetShowMsg  	
Description:		设置要显示的取流消息
Input:				nMsgID 消息ID
Output:      	
Return:			
*************************************************/
void CRealPlayViewWnd::SetShowMsg(int nMsgID)
{
	switch(nMsgID)
	{
	case E_CONNECT_FAIL:
        {
            PV_ERROR("---------------------------------E_CONNECT_FAIL---------------------------------");
            m_strShowMsg = m_strCamName + STR_PREVIEW_CONNECT_FAIL;
            m_bIsSteaming = FALSE;
            m_bFirstCleanup = FALSE;
        }
        break;
	case E_NETWORK_EXCEPTION:
        {
            PV_ERROR("---------------------------------E_NETWORK_EXCEPTION---------------------------------");
            m_strShowMsg = m_strCamName + STR_PREVIEW_CONNECT_FAIL;
            m_bIsSteaming = FALSE;
            m_bFirstCleanup = FALSE;
			
			// 跨线程调用使用消息，不能直接进行窗口相关操作
			PostMessage(WM_PV_REALWND_NET_EXCEPTION);
        }
        break;
	case E_RECONNECT_FAIL:
        {
            PV_ERROR("---------------------------------E_RECONNECT_FAIL---------------------------------");
            m_strShowMsg = m_strCamName + STR_PREVIEW_CONNECT_FAIL;
            m_bIsSteaming = FALSE;
            m_bFirstCleanup = FALSE;
		}
		break;
	case E_RECONNECT_OVER:
        {
            PV_ERROR("---------------------------------E_RECONNECT_OVER---------------------------------");
            m_strShowMsg = m_strCamName + STR_PREVIEW_RECONNECT_OVER;
            m_bIsSteaming = FALSE;
            m_bFirstCleanup = FALSE;
		}
		break;
	case E_CONNECT_OK:
        {
            PV_ERROR("---------------------------------E_CONNECT_OK---------------------------------");
            m_bIsSteaming = TRUE;
            m_bFirstCleanup = TRUE;
		}
		break;
    case E_RECONNECT_OK:
        {
            PV_ERROR("---------------------------------E_RECONNECT_OK---------------------------------");
            m_bIsSteaming = TRUE;
            m_bFirstCleanup = TRUE;
        }
        break;
	default:
		break;
	}

	Invalidate(TRUE);
}

void CRealPlayViewWnd::SetShowMsg(const CString &strShowMsg)
{
    m_strShowMsg = strShowMsg;
    Invalidate(TRUE);
}

void CRealPlayViewWnd::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CFont* pOldFont;
	pOldFont = dc.SelectObject(m_pTextFont);

	CPoint centerPoint = rcClient.CenterPoint();
	rcClient.top = centerPoint.y-20;
	dc.SetTextColor(theApp.GetUIColor(UI_COLOR_TYPE_RW_BORDER));
	dc.SetBkMode(TRANSPARENT);
	if (!m_strShowMsg.IsEmpty())
	{
		dc.DrawText(m_strShowMsg,-1,&rcClient,DT_CENTER|DT_VCENTER|DT_WORDBREAK );
	}
	dc.SelectObject(pOldFont);
}

void CRealPlayViewWnd::ShowToolBar(BOOL bShow)
{
	if (bShow)
	{
		m_bIsMouseHover = TRUE;
        if (m_pWndToolBar != NULL)
        {
            m_pWndToolBar->ShowWindow(SW_SHOW);
        }
	}
	else
	{
		m_bIsMouseHover = FALSE;
        if (m_pWndToolBar != NULL)
        {
            m_pWndToolBar->ShowWindow(SW_HIDE);
        }
	}
}

void CRealPlayViewWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_HIDE_TOOLBAR:
		{
            HideToolBar();
		}
		break;
	case TIMER_MAX_RECORDTIME:
		{

			COleDateTime dtRecordCurrentTime = COleDateTime::GetCurrentTime();
			COleDateTimeSpan timespan = dtRecordCurrentTime - m_dtRecordStartTime;
			if((!m_bFirstShow && timespan.GetTotalMinutes() >= m_nRecordTimeOut))    //录像超时
			{
				m_bFirstShow = TRUE;
				if (IDYES == MsgBoxApi_Show(_T("结束录像？"), MSGBOX_TYPE_YESNO, 1))
				{
					PV_INFO("TIMER_MAX_RECORDTIME");
					StopRecord();
					m_bFirstShow = FALSE;
					PV_INFO("结束录像");
				}
				else
				{
					m_nRecordTimeOut += m_nDelayTime;
					m_bFirstShow = FALSE;
				}
			}
		}
		break;
	case TIMER_CHECK_STREAM:
		{
			if (m_lLastStreamTime == 0)
			{
				m_lLastStreamTime = (LONG)CTime::GetCurrentTime().GetTime();
			}
			LONG lCurTime = (LONG)CTime::GetCurrentTime().GetTime();
			if (m_lLastStreamTime + 2 < lCurTime)
			{
				if (m_bIsRecording)
				{
					PV_INFO("TIMER_CHECK_STREAM-RECORD");
					StopRecord();
					StopPreview();
					m_strShowMsg = _T("未检测到视频流.");
				}
				else
				{
					PV_INFO("TIMER_CHECK_STREAM-PREVIEW");
					StopPreview();
					m_strShowMsg = _T("未检测到视频流.");
				}
				m_lLastStreamTime = 0;
			}
		}
		break;
        case TIMER_OPEN_SOUND:
            {
                if(m_bProIsSound)
                {
                    if (OpenSound())
                    {
                        KillTimer(TIMER_OPEN_SOUND);
                    }
                    ::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_SOUND,1);
                }
            }
    default:
        break;
	}

	CCustomDrawBlackWnd::OnTimer(nIDEvent);
}

LRESULT CRealPlayViewWnd::OnToolBarBtnClk(WPARAM wParam, LPARAM lParam)
{
    int nCmd = (int)wParam;
    ExcuteSelectOption(nCmd);
    return 0;
}

LRESULT CRealPlayViewWnd::OnNetException(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsRecording)
	{	
		m_bIsRecordNormal = FALSE;
		::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_PV_TOOLBAR_BTN_CLK_REF, PT_CMD_STOP_RECORD, 0);
	}	
	return 0;
}

void CRealPlayViewWnd::HideToolBar()
{
	CString strMsg;

	if (!IsPreview())
	{
		return;
	}

	//如果鼠标还在窗口内
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_rcClient.PtInRect(pt))
	{
		return;
	}

	//如果工具条已经隐藏
	if (m_pWndToolBar != NULL && !m_pWndToolBar->IsWindowVisible())
	{
		return;
	}

	m_bIsMouseHover = FALSE;
    if (m_pWndToolBar != NULL)
    {
        m_pWndToolBar->ShowWindow(SW_HIDE);
    }
}

CString CRealPlayViewWnd::GetChanName()
{
    return m_strCamName;
}

//预览重连消息回调
void CALLBACK CRealPlayViewWnd::PreviewMsgNotifyCallback(HANDLE hStream, HANDLE hUserData, int nMsgID)
{
	CRealPlayViewWnd *pWnd = (CRealPlayViewWnd *)hUserData;

	//ASSERT(pWnd);

	if(pWnd == NULL)
	{
		return;
	}

	pWnd->SetShowMsg(nMsgID);
}

/** @fn         CheckOperateRights
 *  @brief      根据菜单项检查是否有操作权限
 *  @param[IN]  nMenuID: 焦点播放窗口
 *  @return     TRUE: 有，FALSE: 无
 */
BOOL CRealPlayViewWnd::CheckOperateRights(int nMenuID)
{
    int nOperType = 0;
    switch (nMenuID)
    {
    case MENU_ID_CAPTURE:       //抓图
        {
            nOperType = PV_RIGHTTYPE_CAPTURE;
            break;
        }
    case MENU_ID_RECORD:        //录像
        {
            nOperType = PV_RIGHTTYPE_RECORD;
            break;
        }
    default:
        return TRUE;
    }

    return theApp.GetOperateRight(nOperType);
}

/** @fn         SetFullScreenStatus
 *  @brief      设置全屏状态
 *  @param[IN]  bFullScreen: 是否全屏
 *  @return     void
 */
void CRealPlayViewWnd::SetFullScreenStatus(BOOL bFullScreen/* = FALSE*/)
{
    m_bFullScreen = bFullScreen;
}

BOOL CRealPlayViewWnd::DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo)
{
    if ((nSize <= 0) || (nPort < 0))
    {
        return FALSE;
    }
    HIK_FRAME_INFO *pFrame = (HIK_FRAME_INFO *)pFrameInfo;
    if (pFrame == NULL)
    {
        return FALSE;
    }
    switch(pFrame->nType)
    {
    case T_AUDIO8:
        PV_INFO("T_AUDIO8");
		{
			AUDIOFRAME audioFrame;
			audioFrame.nFrameRate = pFrame->nFrameRate;
			audioFrame.nStamp = pFrame->nStamp;
			audioFrame.nType = 8;
			audioFrame.nSize = nSize;
			char *bufTemp = new char[nSize];
			memcpy_s(bufTemp,nSize,pBuf,nSize);
			audioFrame.pBuf = bufTemp;
			if(NULL != m_pVideoWav)
			{
				m_pVideoWav->AddAudioFrame(audioFrame);
			}
		}
        break;
    case T_AUDIO16:  //采样率 16khz，单声道，每个采样点16位
        PV_INFO("T_AUDIO16");
        {
            AUDIOFRAME audioFrame;
            audioFrame.nFrameRate = pFrame->nFrameRate;
            audioFrame.nStamp = pFrame->nStamp;
            audioFrame.nType = 16;
            audioFrame.nSize = nSize;
            char *bufTemp = new char[nSize];
            memcpy_s(bufTemp,nSize,pBuf,nSize);
            audioFrame.pBuf = bufTemp;
            if(NULL != m_pVideoWav)
            {
                m_pVideoWav->AddAudioFrame(audioFrame);
            }
        }
        break;
    case T_UYVY:
        PV_INFO("T_UYVY");
        break;
    case T_YV12:    //hik
        //PV_INFO("T_YV12");
        break;
    case T_RGB32:
        PV_INFO("T_RGB32");
        break;
    default:
        break;
    }
    return TRUE;
}

void CRealPlayViewWnd::StreamDataProc(long datatype, void *pBuffer, DWORD dwSize)
{
	switch(datatype)
	{
	case PHONE_DT_STREAM_START:
		PV_INFO("EVC_DT_STREAM_START");
		if (NULL != m_pAudioWav)
		{
			m_pAudioWav->OpenScope();
			m_pAudioWav->SetZoomNumber(5);
		}
		break;
	case PHONE_DT_STREAM_STOP:
		{
			PV_INFO("EVC_DT_STREAM_STOP");
			if (NULL != m_pAudioWav && m_pAudioWav->IsOpen())
			{
				m_pAudioWav->CloseScope();
			}
		}
		break;
	case PHONE_DT_STREAM_DATA:
		{
			const AUDIOFRAME* pAudioFrame = reinterpret_cast<const AUDIOFRAME*>(pBuffer);
			if (NULL != m_pAudioWav && m_pAudioWav->IsOpen())
			{
				m_pAudioWav->AddAudioFrame(*pAudioFrame);
			}
		}
		break;
	default:
		break;
	}
}

void CRealPlayViewWnd::PhoneErrorProc(int nErrortype)
{
	switch(nErrortype)
	{
	case PHONE_ERROR_CONNECTION:
		{
			PV_ERROR("PHONE_ERROR_CONNECTION, StopPreview ... ");
			ClosePSCLient();
			CString strTips = _T("");
			strTips.Format(_T("录音服务连接失败,请检查录音服务并重新连接"));
			SetShowMsg(strTips);
		}
		break;
	default:
		break;
	}
}

// 录像错误消息回调处理
void CRealPlayViewWnd::VideoErrorProc(int nErrortype)
{
	switch(nErrortype)
	{
	case EVC_ERROR_CONNECTION:
		{
			if (IsRecord())
			{
				HWND hWnd = m_hPlayViewWnd;
				::PostMessage(hWnd, WM_PV_TOOLBAR_BTN_CLK, IDC_BTN_PT_COMMERROR, 0);
			}

			PV_ERROR("EVC_ERROR_CONNECTION, StopPreview ... ");
			CloseVSClient();

			CString strTips = _T("");
			strTips.Format(_T("录像服务连接失败,请检查录像服务并重新连接！"));
			SetShowMsg(strTips);
		}
		break;
	case EVC_ERROR_RESERVE_SAPCE_FULL:
		{
			MsgBoxApi_Show(_T("采集器磁盘空间满！"), MSGBOX_TYPE_WARNING, 0);
		}
		break;
	case EVC_ERROR_FILE_OPERATION_FAIL:
		{
			MsgBoxApi_Show(_T("开始录像失败，采集器录像异常！"), MSGBOX_TYPE_WARNING, 0);
		}
		break;
	default:
		break;
	}
}

void CRealPlayViewWnd::SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav)
{
	m_pVideoWav = NULL;//pVideoWav;
	m_pAudioWav = NULL;//pAudioWav;
}

BOOL CRealPlayViewWnd::OpenPSCLient(const PREVIEW_CAMINFO &struPreviewInfo)
{
	ASSERT(NULL == m_hPhoneLogin);
	CString strTips;
	m_hPhoneLogin = PSClient_Login(
		CStringConverter::Tstring2Ansi((LPCTSTR)struPreviewInfo.strCollectorIP).c_str(), 
		struPreviewInfo.nPhonePort, struPreviewInfo.nUserID, 
		struPreviewInfo.nPChannel, this, PhoneStreamDataCallback, PhoneErrorCallback);
	if (m_hPhoneLogin == NULL)
	{
		strTips.Format(_T("录音服务登录失败！"));
		m_strShowMsg = strTips;
		Invalidate();
		PV_ERROR("VSClient_Login failed, strConnectorIP:%s, nPhonePort: %d", 
			 struPreviewInfo.strCollectorIP, struPreviewInfo.nPhonePort);
		return FALSE;
	}
	m_bIsOpenPSClient = TRUE;

	return TRUE;
}

void CRealPlayViewWnd::ClosePSCLient()
{
	if (FALSE == m_bIsOpenPSClient)
	{
		return;
	}

	if (m_hPhoneLogin != NULL)
	{
		PSClient_Logout(m_hPhoneLogin);
	}
	m_hPhoneLogin = NULL;

	if (NULL != m_pAudioWav && m_pAudioWav->IsOpen())
	{
		m_pAudioWav->CloseScope();
	}

	m_bIsOpenPSClient = FALSE;
}


BOOL CRealPlayViewWnd::OpenVSClient(const PREVIEW_CAMINFO &struPreviewInfo)
{
	m_strShowMsg = struPreviewInfo.strCamName + _T("正在连接...");
	m_stCamInfo = struPreviewInfo;
	
	Invalidate();

	STREAMPARAMS streamParam;
	PreparStreamInfo(struPreviewInfo, streamParam);

	PV_INFO("start stream IP:%s, Port:%d, User:%s, Pwd:%s, DevType:%d, streamIP:%s, streamPort:%d", 
		streamParam.device.szName, streamParam.device.nPort,
		streamParam.device.szUsername, streamParam.device.szPassword,
		streamParam.device.nDeviceType, streamParam.path.szSrvIp[0],
		streamParam.path.nPort[0]);

	SetTimer(TIMER_CHECK_STREAM, 500, NULL);
	m_hPreviewhandle = SLStartStream(&streamParam, (SLStreamCallback)PreviewStreamCallback, PreviewMsgNotifyCallback);
	if (!m_hPreviewhandle)
	{
		PV_ERROR("SLStartStream Failed!");
		return FALSE;
	}

	m_lUserID = NET_DVR_Login_V30(
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strDevIP).c_str()), 
		m_stCamInfo.nDevPort, 
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strUser).c_str()), 
		const_cast<char*>(CStringConverter::Tstring2Ansi((LPCTSTR)m_stCamInfo.strPwd).c_str()), 
		&m_stDevInfo);

	if (m_lUserID == -1)
	{
		CString strErrMsg;
		strErrMsg.Format(_T("录像登陆设备失败!"));
		PV_ERROR("NET_DVR_Login_V30 failed %d", NET_DVR_GetLastError());
	}

	//刷新OSD信息
	SetOsdState();

	//记录当前监控点信息
	m_nCamID = struPreviewInfo.nCamID;
	m_strCamName = struPreviewInfo.strCamName;
	m_nDeviceType = struPreviewInfo.nDevType;

	return TRUE;
}

// 关闭录像通信
BOOL CRealPlayViewWnd::CloseVSClient()
{
	if(m_hPreviewhandle == NULL)
	{
		// 重置一些数据
		m_nCamID = -1;
		m_playerType = PT_HIK;
		m_stCamInfo.Clear();
		m_strCamName.Empty();
		m_strShowMsg.Empty();
		m_bFullScreen = FALSE;
		m_bIsSteaming = FALSE;
		m_bFirstCleanup = TRUE;
		Invalidate(TRUE);
		return TRUE;
	}

	//停止取流 放在最前
	SLStopStream(m_hPreviewhandle);
	m_hPreviewhandle = NULL;
	

	//关闭音频
	if(m_bIsSounding)
	{
		CloseSound();
	}

	//关闭录像
	if (m_bIsRecording)
	{
		PV_INFO("CloseVSClient");
		StopRecord();
	}
	
	// 断开连接
	NET_DVR_Logout_V30(m_lUserID);
	m_lUserID = -1;

	//停止解码
	StopPlay();

	//回复一些数据初值
	m_nCamID = -1;
	m_playerType = PT_HIK;

	m_stCamInfo.Clear();
	m_strCamName.Empty();
	m_strShowMsg.Empty();
	m_bFullScreen = FALSE;
	m_bIsSteaming = FALSE;
	m_bFirstCleanup = TRUE;
	Invalidate(TRUE);

	return TRUE;
}

// 主子码流切换
void CRealPlayViewWnd::StreamTypeSwitch(const int nStreamType)
{
	PREVIEW_CAMINFO  struPreviewInfo = m_stCamInfo;
	int nCamType = struPreviewInfo.nCamType;
	switch(nCamType)
	{
	case PV_CAMTPYE_DVR:
		{
			struPreviewInfo.nRealPlayType = nStreamType;
			break;
		}
	case PV_CAMTPYE_NVR:
		{
			struPreviewInfo.nRealPlayType = nStreamType;
			break;
		}
	case PV_CAMTPYE_USB:
		{
			if (nStreamType == 0)
			{
				struPreviewInfo.uiWidth = 1280;
				struPreviewInfo.uiHeight = 720;
			}
			else
			{
				struPreviewInfo.uiWidth = 640;
				struPreviewInfo.uiHeight = 480;
			}
			break;
		}
	default: 
		break;
	}
	StartPreview(struPreviewInfo);
}

void CRealPlayViewWnd::SetOsdState()
{
	DWORD dwReturned = 0;
	DWORD dwErr = 0;
	memset(&m_stShowString, 0, sizeof(m_stShowString));
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30), &dwReturned))
	{
		dwErr = NET_DVR_GetLastError();
		PV_ERROR("NET_DVR_GetDVRConfig failed %d", dwErr);
		return ;
	}
	int nCount = 0;
	for (int iIndex = 0; iIndex < MAX_STRINGNUM_V30; iIndex++)
	{
		if (m_stShowString.struStringInfo[iIndex].wShowString == (WORD)TRUE)
		{
			nCount ++;
		}
	}
	if (nCount > 0)
	{
		::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_OSD,1);
		m_bIsOsdShowing = TRUE;
	}
	else
	{
		::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_PV_REALPLAY_STOPPREVIEW,MENU_ID_OSD,0);
		m_bIsOsdShowing = FALSE;
	}
	m_nOsdInfoShowNum = nCount;
}

void CRealPlayViewWnd::ShowOSD()
{
	for (int iIndex = 0; iIndex < m_nOsdInfoShowNum; iIndex++)
	{
		m_stShowString.struStringInfo[iIndex].wShowString = (WORD)TRUE;
	}
	DWORD dwErr;
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
		dwErr = NET_DVR_GetLastError();
		PV_ERROR("NET_DVR_SetDVRConfig failed %d", dwErr);
		if (m_nOsdInfoShowNum == 8)
		{
			m_nOsdInfoShowNum = 4;
			for (int iIndex = m_nOsdInfoShowNum; iIndex < 8; iIndex++)
			{
				m_stShowString.struStringInfo[iIndex].wShowString = (WORD)FALSE;
			}
			if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
			{
				dwErr = NET_DVR_GetLastError();
				PV_ERROR("NET_DVR_SetDVRConfig failed %d", dwErr);
			}
		}
	}
}

void CRealPlayViewWnd::HideOSD()
{
	for (int iIndex = 0; iIndex < m_nOsdInfoShowNum; iIndex++)
	{
		m_stShowString.struStringInfo[iIndex].wShowString = (WORD)FALSE;
	}
	DWORD dwErr;
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
		dwErr = NET_DVR_GetLastError();
		PV_ERROR("NET_DVR_SetDVRConfig failed %d", dwErr);
	}
}

BOOL CRealPlayViewWnd::SetOSDInfo(const CString &strOsdInfo)
{
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

	DWORD dwReturned = 0;
	DWORD dwErr = 0;
	memset(&m_stShowString, 0, sizeof(m_stShowString));
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30), &dwReturned))
	{
		dwErr = NET_DVR_GetLastError();
		PV_ERROR("NET_DVR_GetDVRConfig failed %d", dwErr);
		return FALSE;
	}

	CMarkup xml;
	m_strOsdInfo = strOsdInfo;
	if (!xml.SetDoc(strOsdInfo))
	{
		return FALSE;
	}
	if (!xml.FindElem(_T("BussInfo")))
	{
		return FALSE;
	}
	xml.IntoElem();
	if (!xml.FindElem(_T("OSDNum")))
	{
		m_nOsdInfoShowNum = 4;
	}
	else
	{
		m_nOsdInfoShowNum = _wtoi(xml.GetData());
	}
	int iIndex = 0;
	int iLeftY = 0;
	for (int iIndex = 0; iIndex < MAX_STRINGNUM_V30; iIndex++)
	{
		if (xml.FindElem(_T("Node")))
		{
			m_stShowString.struStringInfo[iIndex].wShowString = (WORD)TRUE;
			memset(&m_stShowString.struStringInfo[iIndex].sString, 0, sizeof(m_stShowString.struStringInfo[iIndex].sString));
			strncpy_s(m_stShowString.struStringInfo[iIndex].sString, 
				sizeof(m_stShowString.struStringInfo[iIndex].sString)/sizeof(char), 
				CStringConverter::Tstring2Ansi((LPCTSTR)xml.GetData()).c_str(), _TRUNCATE);
			WORD wStringSize = sizeof(m_stShowString.struStringInfo[iIndex].sString)/sizeof(char);
			// 以下操作是为了保证长于44字符拷贝时最后一汉字（2字符）不被中间截断
			// 首先检测sString[42]是否为中文（不检测sString[43]是因为拷贝时最后一个字符会被'\0'占用）
			// &0X80判断一个字符是否为汉字字符
			if (m_stShowString.struStringInfo[iIndex].sString[wStringSize-2]&0X80)	
			{
				int nCount = 0;
				for (int i = 0; i < wStringSize; i++)
				{
					if (m_stShowString.struStringInfo[iIndex].sString[i]&0X80)
					{
						nCount++;
					}
				}

				//中文字符为单数，索性去除最后一位
				if (nCount%2)
				{
					m_stShowString.struStringInfo[iIndex].sString[wStringSize-2] = '\0';
				}
			}
			m_stShowString.struStringInfo[iIndex].wStringSize = (WORD)strlen(m_stShowString.struStringInfo[iIndex].sString);
			
			m_stShowString.struStringInfo[iIndex].wShowStringTopLeftX = (WORD)0;
			iLeftY = (iIndex+2)*35;
			m_stShowString.struStringInfo[iIndex].wShowStringTopLeftY = (WORD)iLeftY;	
		}
		else
		{
			memset(&m_stShowString.struStringInfo[iIndex], 0, sizeof(m_stShowString.struStringInfo[iIndex]));
		}
	}

	if (m_nOsdInfoShowNum == 4)
	{
		for (int iIndex = m_nOsdInfoShowNum; iIndex < 8; iIndex++)
		{
			m_stShowString.struStringInfo[iIndex].wShowString = (WORD)FALSE;
		}
	}
	else
	{
		for (int iIndex = 0; iIndex < m_nOsdInfoShowNum; iIndex++)
		{
			m_stShowString.struStringInfo[iIndex].wShowString = (WORD)TRUE;
		}
	}

	if (!m_bIsOsdShowing)
	{
		return TRUE;
	}

	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
		dwErr = NET_DVR_GetLastError();
		PV_ERROR("NET_DVR_SetDVRConfig failed %d", dwErr);
		if (m_nOsdInfoShowNum == 8)
		{
			m_nOsdInfoShowNum = 4;
			for (int iIndex = m_nOsdInfoShowNum; iIndex < 8; iIndex++)
			{
				m_stShowString.struStringInfo[iIndex].wShowString = (WORD)FALSE;
			}
			if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SHOWSTRING_V30, m_stCamInfo.nChannel + m_stDevInfo.byStartChan, &m_stShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
			{
				dwErr = NET_DVR_GetLastError();
				PV_ERROR("NET_DVR_SetDVRConfig failed %d", dwErr);
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CRealPlayViewWnd::USBStartPreview( const PREVIEW_CAMINFO& stUSBPreInfo)
{
	HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
	//获取绝对路径
	TCHAR szDir[MAX_PATH] = {0};
	GetModuleFileName(hModule, szDir, MAX_PATH);
	PathRemoveFileSpec(szDir);
	SetCurrentDirectory(szDir);
	m_strShowMsg = _T("正在连接...");
	m_nRecordTimeOut = stUSBPreInfo.nTimeout;
	m_nRecordTimeOutOld = stUSBPreInfo.nTimeout;
	m_nDelayTime = stUSBPreInfo.nDelayTime;
	m_strCamName = stUSBPreInfo.strCamName;
    
    g_bFirst = true;

    Invalidate();
    int iRet = 0;
    BOOL bRet = FALSE;
    
    //GetPort
    LONG nPort = -1;
    if (!GPAPI_GetPort(PT_HIK,nPort,GetSafeHwnd()))
    {
        PV_ERROR("GPAPI_GetPort falid!");
        return FALSE;
    }

    //保存port
    m_lPort = nPort;

   /* bRet = USBCamera_GetPort(&m_lUSBPort);
    if (!bRet)
    {
        PV_ERROR("USBCamera_GetPort Failed![Error:%d]", USBCamera_GetLastError());
        USBCamera_UninitSDK();
        return FALSE;
    }*/

	/////////重新枚举摄像机信息，解决中途拔掉摄像机的奔溃问题//////////////
	int nDevNum = 0;
	//获取摄像头信息
	bRet = USBCamera_EnumVideoDevice(&nDevNum);
	if (!bRet)
	{   
		PV_ERROR("USBCamera_EnumVideoDevice Failed![Error:%d]", USBCamera_GetLastError());
		//return FALSE;
	}

	//获取麦克风信息
	bRet = USBCamera_EnumAudioDevice(&nDevNum);
	if (!bRet)
	{
		PV_ERROR("USBCamera_EnumAudioDevice Failed![Error:%d]", USBCamera_GetLastError());
		//return FALSE;
	}

    HANDLE vDevice = NULL;
    HANDLE aDevice = NULL;
    if (!USBCamera_GetVideoDevice(stUSBPreInfo.lCamIndex, &vDevice))
    {
		PV_ERROR("USBCamera_GetVideoDevice Failed![Error:%d]", USBCamera_GetLastError());
    }
	if (!USBCamera_GetAudioDevice(stUSBPreInfo.lMicIndex, &aDevice))
	{
		PV_ERROR("USBCamera_GetAudioDevice Failed![Error:%d]", USBCamera_GetLastError());
	}
	if (NULL == vDevice)
	{
		PV_ERROR("USBCamera_GetVideoDevice Failed![Error:%d]", USBCamera_GetLastError());
        m_strShowMsg = _T("预览失败");
        Invalidate();
		//USBCamera_FreePort(m_lUSBPort);
		// USBCamera_UninitSDK();
		return FALSE;
	}
    if (NULL == aDevice)
    {
		PV_ERROR("USBCamera_GetAudioDevice Failed![Error:%d]", USBCamera_GetLastError());
        //USBCamera_FreePort(m_lUSBPort);
       // USBCamera_UninitSDK();
        return FALSE;
    }
    //选择待预览设备
    bRet = USBCamera_ChooseDevice(theApp.m_lUSBPort, vDevice, aDevice);
    if (!bRet)
    {
        PV_ERROR("USBCamera_ChooseDevice Failed![Error:%d]", USBCamera_GetLastError());
       // USBCamera_FreePort(m_lUSBPort);
        //USBCamera_UninitSDK();
        return FALSE;
    }
    if (stUSBPreInfo.uiWidth == 640 && stUSBPreInfo.uiHeight == 480)
    {
        if (!USBCamera_SetBitRate(theApp.m_lUSBPort, 512))
        {
			 PV_ERROR("USBCamera_SetBitRate Failed![Error:%d]", USBCamera_GetLastError());
        }
    }
    else
    {
        //设置H264码率
		if (!USBCamera_SetBitRate(theApp.m_lUSBPort, 1024))
		{
			PV_ERROR("USBCamera_SetBitRate Failed![Error:%d]", USBCamera_GetLastError());
		}
    }

    //设置帧率
	if (!USBCamera_SetFrameRate(theApp.m_lUSBPort, 15))
	{
		PV_ERROR("USBCamera_SetFrameRate Failed![Error:%d]", USBCamera_GetLastError());
	}

    //设置分辨率
	if (!USBCamera_SetResolution(theApp.m_lUSBPort, stUSBPreInfo.uiWidth, stUSBPreInfo.uiHeight))
	{
		PV_ERROR("USBCamera_SetResolution Failed![Error:%d]", USBCamera_GetLastError());
	}

    //设置码流回调
    DEC_CALLBACK_INFO struDecCbInfo = {0};
    struDecCbInfo.dwVideoFormat = USBCAMERA_PS_H264_PCM;
    struDecCbInfo.dwAudioFormat = 0;
    struDecCbInfo.lPort = theApp.m_lUSBPort;
    struDecCbInfo.pUser = this;
    struDecCbInfo.fnDevCb = DecCallBack;
	if (!USBCamera_SetDecCallBack(theApp.m_lUSBPort, struDecCbInfo))
	{
		PV_ERROR("USBCamera_SetDecCallBack Failed![Error:%d]", USBCamera_GetLastError());
	}

    //设置语音码率等信息
    WAVEFORMATEX wf;
	if (!USBCamera_GetAudioFormat(theApp.m_lUSBPort, &wf))
	{
		PV_ERROR("USBCamera_GetAudioFormat Failed![Error:%d]", USBCamera_GetLastError());
	}
    wf.nChannels = 1;   // 单通道
    wf.nSamplesPerSec = 8000;   // 采样率
    wf.wBitsPerSample = 16;     //只支持16位，8位则没有声音
    wf.nBlockAlign = 2;
    wf.nAvgBytesPerSec =  wf.nSamplesPerSec * wf.nBlockAlign;
	if (!USBCamera_SetAudioFormat(theApp.m_lUSBPort, wf))
	{
		PV_ERROR("USBCamera_SetAudioFormat Failed![Error:%d]", USBCamera_GetLastError());
	}

    //设置预览参数
    int nPreviewType = USBCAMERA_PS_H264;
    BOOL bPreviewAudio = FALSE;
	if (!USBCamera_SetPreview(theApp.m_lUSBPort, nPreviewType, bPreviewAudio, NULL))
	{
		PV_ERROR("USBCamera_SetPreview Failed![Error:%d]", USBCamera_GetLastError());
	}

    //启动预览
    bRet = USBCamera_Start(theApp.m_lUSBPort);
    if (!bRet)
    {
        PV_ERROR("USBCamera_Start Failed![Error:%d]", USBCamera_GetLastError());
        //USBCamera_FreePort(m_lUSBPort);
        //USBCamera_UninitSDK();
		return FALSE;
    }

    SetTimer(TIMER_CHECK_STREAM, 5000, NULL);
	m_stCamInfo = stUSBPreInfo;
	m_bIsPreviewing = TRUE;
	return TRUE;
}

BOOL CRealPlayViewWnd::USBStopPreview( LONG lCameraIndex)
{
    //关闭音频
    if(m_bIsSounding)
    {
        CloseSound();
    }
	StopPlay();
	int nret = 0;
    USBCamera_Stop(theApp.m_lUSBPort);
    USBCamera_FreeDevice(theApp.m_lUSBPort);
    //USBCamera_FreePort(m_lUSBPort);
    //USBCamera_UninitSDK();
	m_bIsPreviewing = FALSE;
	m_nCamID = -1;
	m_playerType = PT_HIK;
	m_stCamInfo.Clear();
	m_strCamName.Empty();
	m_strShowMsg.Empty();
	m_bFullScreen = FALSE;
	m_bIsSteaming = FALSE;
	m_bFirstCleanup = TRUE;
	Invalidate(TRUE);
	return TRUE;
}

BOOL CRealPlayViewWnd::USBStartRecord()
{
	PV_INFO("USBStartRecord begin!");
	m_bIsRecording = TRUE;
	SetTimer(TIMER_MAX_RECORDTIME, 500, NULL);
	CString strErrMsg;
	if (!m_bIsPreviewing)
	{
		strErrMsg.Format(_T("开始录像失败：录像取流失败！"));
		SetShowMsg(strErrMsg);
		return FALSE;
	}

	//开始录像时抓图
	if (!CapturePic())
	{
		PV_ERROR("CapturePic failed !");
	}
	PV_INFO("USBStartRecord CapturePic!");
	m_dtRecordStartTime = COleDateTime::GetCurrentTime();
	m_bIsRecordNormal = TRUE;
	m_dtRecordTime = CTime::GetCurrentTime();
	m_ttStart = time(NULL);

	LOCAL_CFG struLocalCfg;
	theApp.GetLocalCfg(struLocalCfg);
	//0: 无限制，1：64M，2：128M，4：256M，8：512M
	int nRecSize = struLocalCfg.stRecordInfo.nRecordFileSize;
	switch(nRecSize)
	{
    case 0:
        m_dwRecFileSize = 0xFFFFFFFF;
        break;
	case 1:
		m_dwRecFileSize = 64*1024*1024;
		break;
	case 2:
		m_dwRecFileSize = 128*1024*1024;
		break;
	case 4:
		m_dwRecFileSize = 256*1024*1024;
		break;
	case 8:
		m_dwRecFileSize = 512*1024*1024;
		break;
	default:
		m_dwRecFileSize = 128*1024*1024;
		break;
	}
	// 确保目录的存在
	SuperMakeDir(CStringW2A(struLocalCfg.stRecordInfo.szRecordPath).GetBuffer());
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
// 	m_stLastRecordFile.strFileName.Format(_T("%s%s_%04u%02u%02u%02u%02u%02u.mp4"), struLocalCfg.stRecordInfo.szRecordPath, 
// 		m_strCamName, 
// 		timeNow.wYear,
// 		timeNow.wMonth,
// 		timeNow.wDay,
// 		timeNow.wHour,
// 		timeNow.wMinute,
// 		timeNow.wSecond);
	m_stLastRecordFile.strFileName.Format(_T("%s录像%04u%02u%02u%02u%02u%02u.mp4"), struLocalCfg.stRecordInfo.szRecordPath, 
		timeNow.wYear,
		timeNow.wMonth,
		timeNow.wDay,
		timeNow.wHour,
		timeNow.wMinute,
		timeNow.wSecond);

	m_stLastRecordFile.strFileName.Replace(_T("\\"), _T("\\\\"));
	m_stLastRecordFile.lStartTime = m_ttStart;
	m_stLastRecordFile.lEndTime = 0;
	m_stLastRecordFile.lFileSize = 0;
	m_stLastRecordFile.nRecordStatus = 0;	// 正在采集

	PV_INFO("USBStartRecord PV_RECORD_START!");
	if (m_pvFunc != NULL)
		m_pvFunc(PV_RECORD_START, (void*)&m_stLastRecordFile);

	PV_INFO("USBStartRecord end!");
	return TRUE;
}

BOOL CRealPlayViewWnd::USBStopRecord()
{
	if (m_bIsRecording == FALSE)
		return TRUE;

	m_ttEnd = time(NULL);
	CString csFileOldPath = m_stLastRecordFile.strFileName;
	CString csFilePath = m_stLastRecordFile.strFileName;
	int nPos = csFilePath.ReverseFind('.');
	csFilePath = csFilePath.Left(nPos);
	PV_INFO("filename:%s", CStringW2A(csFilePath));
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
// 	m_stLastRecordFile.strFileName.Format(_T("%s-%04u%02u%02u%02u%02u%02u.mp4"), 
// 		csFilePath, 
// 		timeNow.wYear,
// 		timeNow.wMonth,
// 		timeNow.wDay,
// 		timeNow.wHour,
// 		timeNow.wMinute,
// 		timeNow.wSecond);
	m_stLastRecordFile.strFileName.Format(_T("%s.mp4"), csFilePath);
	m_stLastRecordFile.lStartTime = m_ttStart;
	m_stLastRecordFile.lEndTime = m_ttEnd;
	m_stLastRecordFile.lFileType = 6;
	if (m_bIsRecordNormal)
	{
		m_stLastRecordFile.nRecordStatus = 1;	// 正常状态
	}
	else
	{
		m_stLastRecordFile.nRecordStatus = 8;	// 异常状态
	}
	if (NULL != m_hRecordFile)
	{
		m_stLastRecordFile.lFileSize = (long)GetFileSize(m_hRecordFile, NULL);
		CloseHandle(m_hRecordFile);
		m_hRecordFile = NULL;
		m_bIsRecording = FALSE;
	}

	//停止录像时抓图
	if (!CapturePic())
	{
		PV_ERROR("CapturePic failed !");
	}

	if (m_pvFunc != NULL)
		m_pvFunc(PV_RECORD_STOP, (void*)&m_stLastRecordFile);

	// 恢复状态标记
	m_bIsRecordNormal = TRUE;
	// 重命名录像文件
	CString csFileNewPath = m_stLastRecordFile.strFileName;
	PV_INFO("Before rename");
	//CFile::Rename(csFileOldPath, csFileNewPath);
	PV_INFO("After rename");

	return TRUE;
}

void CALLBACK CRealPlayViewWnd::USBPreviewStreamCallback(OUTPUTDATA_INFO *pDataInfo, unsigned long pUser)
{
	CRealPlayViewWnd * pWnd = (CRealPlayViewWnd *)pUser;
	//开始解码
	pWnd->InputData((PBYTE)pDataInfo->pData, pDataInfo->dwDataLen, pDataInfo->dwDataType);
}

//USB摄像头SDK取流回调函数
void CALLBACK CRealPlayViewWnd::DecCallBack (LONG nPort, unsigned char *pBuf, long nSize, FRAME_INFO *pFrameInfo, void* pUser)
{
    CRealPlayViewWnd * pWnd = (CRealPlayViewWnd *)pUser;
    if (NULL == pWnd)
    {
        return;
    }
    //目前USB SDK码流回调没有区分回调类型是视频头还是数据，统一为12；
    //在接收到码流后需要做一下转换：第一次为数据头，将类型改为0，以后为数据体，将类型改为1.
    int nType = 0;
    if (g_bFirst)
    {
        nType = pFrameInfo->nType - 13;
        g_bFirst = false;
    }
    else
    {
        nType = pFrameInfo->nType - 12;
    }
    pWnd->InputData((PBYTE)pBuf, nSize, nType);
}


/** @fn     GetDvrTimeStamp
*  @brief  获取指定dvr当前时间戳
*  @param  [in]lUserId, dvr登陆id
*  @param  [in]stDvrInfo, dvr 信息
*  @param  [out]iDvrTimeStamp, dvr当前时间戳
*  @return 正常 TRUE
           异常 非 FALSE
*/
BOOL CRealPlayViewWnd::GetDvrTimeStamp(long lUserId, time_t &iDvrTimeStamp, DWORD& dwError)
{
	DWORD        dwReturned = 0;
    NET_DVR_TIME stDvrTime;
    memset(&stDvrTime, 0, sizeof(NET_DVR_TIME));
    //获取dvr时间
    if (!NET_DVR_GetDVRConfig(lUserId, NET_DVR_GET_TIMECFG, 0,
        &stDvrTime, sizeof(NET_DVR_TIME), &dwReturned))
    {
        dwError = NET_DVR_GetLastError();
        return FALSE;
    }

    struct tm    stTime = {0};
    //dvr时间转换为时间戳
    stTime.tm_year = stDvrTime.dwYear - 1900; //struct tm 的year为距离1900多少年
    stTime.tm_mon  = stDvrTime.dwMonth - 1; //struct tm月份从0开始 所以减一
    stTime.tm_mday = stDvrTime.dwDay;
    stTime.tm_hour = stDvrTime.dwHour;
    stTime.tm_min  = stDvrTime.dwMinute;
    stTime.tm_sec  = stDvrTime.dwSecond;
    iDvrTimeStamp = mktime(&stTime);

    return TRUE;
}