// ATPRemoteCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CATPRemoteCtrlDlg dialog

CATPRemoteCtrlDlg::CATPRemoteCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATPRemoteCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATPRemoteCtrlDlg)
	m_nTarget = 0;
	m_szResult = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_bDllConnected = FALSE;
	//m_bSoapConnected = FALSE;
	m_pSynapiProxy = new CfnXwebApiBindingProxy(SOAP_IO_DEFAULT|SOAP_C_UTFSTRING|SOAP_IO_KEEPALIVE);
	m_nServerIndex = -1;
}

void CATPRemoteCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATPRemoteCtrlDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER_LIST, m_cbServerList);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	DDX_Radio(pDX, IDC_RADIO_MODE, m_nTarget);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_szResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPRemoteCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CATPRemoteCtrlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_BN_CLICKED(IDC_RADIO_MODE, OnRadioMode)
	ON_BN_CLICKED(IDC_RADIO_MODE2, OnRadioMode2)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER_LIST, OnSelchangeComboServerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATPRemoteCtrlDlg message handlers

BOOL CATPRemoteCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TC_ITEM TCItem;
    TCItem.mask = TCIF_TEXT;
    TCItem.pszText = "Init";
    m_tabMain.InsertItem(0, &TCItem);
	SubDlg(CSubDlgInit).Create(CSubDlgInit::IDD, this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgInit)] = SubDlg(CSubDlgInit).GetSafeHwnd();
    TCItem.pszText = "Cfg";
    m_tabMain.InsertItem(1, &TCItem);
	SubDlg(CSubDlgCfg).Create(CSubDlgCfg::IDD,this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgCfg)] = SubDlg(CSubDlgCfg).GetSafeHwnd();
    TCItem.pszText = "Status";
    m_tabMain.InsertItem(2, &TCItem);
	SubDlg(CSubDlgStatus).Create(CSubDlgStatus::IDD,this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgStatus)] = SubDlg(CSubDlgStatus).GetSafeHwnd();
    TCItem.pszText = "Operation";
    m_tabMain.InsertItem(3, &TCItem);
	SubDlg(CSubDlgOp).Create(CSubDlgOp::IDD,this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgOp)] = SubDlg(CSubDlgOp).GetSafeHwnd();
    TCItem.pszText = "Query";
    m_tabMain.InsertItem(4, &TCItem);
	SubDlg(CSubDlgQuery).Create(CSubDlgQuery::IDD,this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgQuery)] = SubDlg(CSubDlgQuery).GetSafeHwnd();
    TCItem.pszText = "DVD";
    m_tabMain.InsertItem(5, &TCItem);
	SubDlg(CSubDlgDvd).Create(CSubDlgDvd::IDD,this);
	m_hWndViews[ENUM_SUBDLG(CSubDlgDvd)] = SubDlg(CSubDlgDvd).GetSafeHwnd();

	POINT pt1,pt2;
	CRect rcBaseFram;
	m_tabMain.GetWindowRect(&rcBaseFram);
	pt1.x=rcBaseFram.left;
	pt1.y=rcBaseFram.top;
	pt2.x=rcBaseFram.right;
	pt2.y=rcBaseFram.bottom;
    ::ScreenToClient(m_hWnd,&pt1);
    ::ScreenToClient(m_hWnd,&pt2);
	rcBaseFram.left=pt1.x;
	rcBaseFram.top=pt1.y;
	rcBaseFram.right=pt2.x;
	rcBaseFram.bottom=pt2.y;

	int i;
	for (i=0; i<VIEW_MAX; i++)
	{
		::MoveWindow(m_hWndViews[i], rcBaseFram.left + 2, rcBaseFram.top + 22,
			rcBaseFram.Width() - 4, rcBaseFram.Height() - 24, 0);
	}
	::ShowWindow(m_hWndViews[ENUM_SUBDLG(CSubDlgInit)], SW_SHOW);

	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CATPRemoteCtrlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CATPRemoteCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CATPRemoteCtrlDlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int nCurSel = m_tabMain.GetCurSel();
	if (m_nServerIndex<0)
	{
		MessageBox("Set Server IP first");
		((CTabCtrl*)GetDlgItem(IDC_TAB_MAIN))->SetCurSel(0);
		return;
	}
    for (int i = 0; i < VIEW_MAX; i++)
    {
		if (i != nCurSel)
		{
			::ShowWindow(m_hWndViews[i], SW_HIDE);
		}
    }
	
    ::ShowWindow(m_hWndViews[nCurSel], SW_SHOW);
	
    *pResult = 0;
}

void CATPRemoteCtrlDlg::OnRadioMode() 
{
	UpdateData(TRUE);
    SubDlg(CSubDlgInit).OnTargetChanged(m_nTarget);
    SubDlg(CSubDlgCfg).OnTargetChanged(m_nTarget);
    SubDlg(CSubDlgStatus).OnTargetChanged(m_nTarget);
    SubDlg(CSubDlgOp).OnTargetChanged(m_nTarget);
	SubDlg(CSubDlgQuery).OnTargetChanged(m_nTarget);
    SubDlg(CSubDlgDvd).OnTargetChanged(m_nTarget);
}

void CATPRemoteCtrlDlg::OnRadioMode2() 
{
	OnRadioMode();
}

//DEL PSYATP_EVENT CATPRemoteCtrlDlg::GetAtpEvent(int event)
//DEL {
//DEL 	int cnt=0;
//DEL 	static SYATP_EVENT se;
//DEL 	while (cnt++<50)
//DEL 	{
//DEL 		if (GetEvent(&se)==0)
//DEL 		{
//DEL 			if (se.wEventCode==event)
//DEL 				return &se;
//DEL 		}
//DEL 		Sleep(100);
//DEL 	}
//DEL 	return NULL;
//DEL }

void CATPRemoteCtrlDlg::OnTimer(UINT nIDEvent) 
{
	CString tmpMsg, appendMsg;
	SYATP_EVENT se;
	CHAR szPath[300];
	CHAR buffer[2048];
	se.pcRemoteAddr=szPath; memset(szPath,0,sizeof(szPath));
	se.pvBuffer=buffer;
	se.dwBufferLength=sizeof(buffer);
	int i;
	PRecEventInfo pCh;
	while ((i=GetEvent(&se))>=0)
	{
		//TRACE("%d, event=%d\n",i,se.wEventCode);
		appendMsg.Format("rcv from %s: ", se.pcRemoteAddr);
		switch(se.wEventCode)
		{
		case E_GetHardwareInfo:
			if (se.nReference==0)
			{
				PHARDWARE_INFO pInfo;
				pInfo = (PHARDWARE_INFO)se.pvBuffer;
				tmpMsg.Format("TotalChannel=%d, Type=%s, IP=%s, Gateway=%s, "
					"Macaddress=%s, Mask=%s, DNS=%s, DevID=%s, "
					"WebVersion=%s, RecSvrVersion=%s, SqlVersion=%s, CpldVersion=%s, "
					"kernelVersion=%s",
					pInfo->iTotalChannels,pInfo->szBoardType,
					pInfo->szIpAddress,pInfo->szGateway,
					pInfo->szMacaddress,pInfo->szMask,
					pInfo->szDns,pInfo->szDevId,
					pInfo->szWebVersion,pInfo->szRecSvrVersion,
					pInfo->szSqlVersion,pInfo->szCpldVersion,
					pInfo->szKernelVersion
					);
			}
			else
			{
				tmpMsg.Format("GetHardwareInfo return=%d",se.nReference);
			}
			break;
		case E_GetChannelStatus:
			if (se.nReference==0)
			{
				PTotalChStatus pReturn;
				pReturn = (PTotalChStatus)se.pvBuffer;
				tmpMsg.Format("Total=%d,Start=%d,status=%s",
					pReturn->cTotalCh,pReturn->iStartChannel,
					pReturn->szChStatus);
			}
			else
			{
				tmpMsg.Format("GetChannelStatus return=%d",se.nReference);
			}
			break;
		case E_GetDiskFreeSpace:
			if (se.nReference==0)
			{
				PDiskSpace pReturn;
				pReturn = (PDiskSpace)se.pvBuffer;
				tmpMsg.Format("Disk1:%s(type=%s),total/free=%d/%d\r\n"
					"Disk2:%s(type=%s),total/free=%d/%d\r\n"
					"Disk3:%s(type=%s),total/free=%d/%d",
					pReturn->szDiskName, pReturn->szDiskType, 
					pReturn->iDiskTotalSpace, pReturn->iDiskFreeSpace,
					pReturn->szDiskName1,pReturn->szDiskType1, 
					pReturn->iDiskTotalSpace1, pReturn->iDiskFreeSpace1,
					pReturn->szDiskName2,pReturn->szDiskType2, 
					pReturn->iDiskTotalSpace2, pReturn->iDiskFreeSpace2
					);
			}
			else
			{
				tmpMsg.Format("GetDiskFreeSpace return=%d",se.nReference);
			}
		    break;
		case E_StartMonitor:
			tmpMsg.Format("StartMonitor return=%d",se.nReference);
		    break;
		case E_StopMonitor:
			tmpMsg.Format("StopMonitor return=%d",se.nReference);
			break;
		case E_RebootDevice:
			tmpMsg.Format("RebootDevice return=%d",se.nReference);
			break;
		case E_SetDateTime:
			tmpMsg.Format("SetDateTime return=%d",se.nReference);
		    break;
		case E_GetGlobalCfg:
			if (se.nReference==0)
			{
				PGlobalCfg pReturn;
				pReturn = (PGlobalCfg)se.pvBuffer;
				SubDlg(CSubDlgCfg).m_nCompress=pReturn->iCompress;
				SubDlg(CSubDlgCfg).m_nFileFormat=pReturn->iFileFormat;
				SubDlg(CSubDlgCfg).m_nStartCh = pReturn->iStartChannel;
				SubDlg(CSubDlgCfg).m_nTotalCh = pReturn->iTotalChannels;
				SubDlg(CSubDlgCfg).m_szRecPath = pReturn->szRecPath;
				SubDlg(CSubDlgCfg).m_szRebootTime = pReturn->szDevRebootTime;
				SubDlg(CSubDlgCfg).m_bAutoReboot = pReturn->cSetReboot;
				SubDlg(CSubDlgCfg).m_nRebootHour = pReturn->iRebootHour;
				SubDlg(CSubDlgCfg).m_nRebootMinute = pReturn->iRebootMin;
				SubDlg(CSubDlgCfg).UpdateData(FALSE);
			}
			else
			{
				tmpMsg.Format("GetGlobalCfg return=%d",se.nReference);
			}
		    break;
		case E_SetGlobalCfg:
			tmpMsg.Format("SetGlobalCfg return=%d",se.nReference);
			break;
		case E_GetChannelCfg:
			if (se.nReference==0)
			{
				PChannelCfg pReturn;
				pReturn = (PChannelCfg)se.pvBuffer;
				SubDlg(CSubDlgCfg).m_nCh=pReturn->iChID;
				SubDlg(CSubDlgCfg).m_nRecType=pReturn->iRecType;
				SubDlg(CSubDlgCfg).m_nRecDir = pReturn->iRecDir;
				SubDlg(CSubDlgCfg).m_nMaxRecLen = pReturn->iMaxReclen;
				SubDlg(CSubDlgCfg).m_nRcvDTMFTime = pReturn->iRecvDtmfTime;
				SubDlg(CSubDlgCfg).m_nRecInt = pReturn->iLongRecInterval;
				SubDlg(CSubDlgCfg).m_szBindPhone = pReturn->szBindPhoneNumber;
				SubDlg(CSubDlgCfg).m_szStartDtmf = pReturn->szStartDtmf;
				SubDlg(CSubDlgCfg).m_szStopDtmf = pReturn->szStopDtmf;
				SubDlg(CSubDlgCfg).UpdateData(FALSE);
			}
			else
			{
				tmpMsg.Format("GetChannelCfg return=%d",se.nReference);
			}
			break;
		case E_SetChannelCfg:
			tmpMsg.Format("SetChannelCfg return=%d",se.nReference);
			break;
		case E_SetNetworkCfg:
			tmpMsg.Format("SetNetworkCfg return=%d",se.nReference);
			break;
		case E_GetLineVol:
			if (se.nReference==0)
			{
				PTotalChVol pReturn;
				pReturn = (PTotalChVol)se.pvBuffer;
				tmpMsg.Format("Total=%d,Start=%d,status=%s",
					pReturn->iTotalCh,pReturn->iStartCh,
					pReturn->szChVol);
			}
			else
			{
				tmpMsg.Format("GetLineVol return=%d",se.nReference);
			}
			break;
		case E_DelFile:
			tmpMsg.Format("DelFile return=%d",se.nReference);
			break;
		case E_DelFiles:
			tmpMsg.Format("DelFiles return=%d",se.nReference);
			break;
		case E_StopDelFile:
			tmpMsg.Format("StopDelFile return=%d",se.nReference);
			break;
		case E_EnableDebugMsg:
			// TODO:
			tmpMsg.Format("EnableDebugMsg return=%d",se.nReference);
			break;
		case E_StartUploadFile:
			tmpMsg.Format("StartUploadFile return=%d",se.nReference);
			break;
		case E_SetAdminPassword:
			tmpMsg.Format("SetAdminPassword return=%d",se.nReference);
			break;
		case E_SetNetTimeout:
			tmpMsg.Format("SetNetTimeout return=%d",se.nReference);
			break;
		case E_Connect:
			tmpMsg.Format("Connect return=%d",se.nReference);
			break;
		case E_Disconnect:
			tmpMsg.Format("Disconnect return=%d",se.nReference);
			break;
		case E_CaculateData:
			if (se.nReference==0)
			{
				PCalculateResult pReturn = (PCalculateResult)se.pvBuffer;
				tmpMsg.Format("TotalCalls=%d,TotalSize=%d(MB)/%d(kB), Enough space in DVD=%d",
					pReturn->iTotalCalls, pReturn->iTotalSizeM,pReturn->iTotalSIzeKB,pReturn->cSizeAvailable);
			}
			else
			{
				tmpMsg.Format("CaculateData return=%d",se.nReference);
			}
			break;
		case E_WriteDVD:
			tmpMsg.Format("WriteDVD return=%d",se.nReference);
			break;
		case E_CheckMediaAvailable:
			tmpMsg.Format("CheckMediaAvailable return=%d",se.nReference);
			break;
		case E_CheckMediaConnected:
			tmpMsg.Format("CheckMediaConnected return=%d",se.nReference);
			break;
		case E_CALL_STANDBY:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) standby",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_PICKUPED:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) pickuped",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_RINGING:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) ringing",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_TALKING:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) talking",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_PENDING:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) pending",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_OFFLINE:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) offline",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_CALL_UNAVAILABLE:
			pCh = (PRecEventInfo)se.pvBuffer;
			tmpMsg.Format("%s: ch%d(%s) unavailable",pCh->szEventTime, pCh->nCh, pCh->szIPAddr);
			break;
		case E_LOW_DISK_SPACE:
			{
				PDiskFullEvent pReturn;
				pReturn = (PDiskFullEvent)se.pvBuffer;
				tmpMsg.Format("(%s)Disk%s: DiskFreeSpace=%d, DiskSpaceMin=%d",
					pReturn->szEventTime,pReturn->szDiskIndex,pReturn->dwDiskFreeSpace,pReturn->dwDiskSpaceMin);
			}
			break;
		case E_GETCHANNELRUNNINGSTATUS:
			if (se.nReference==0)
			{
				PChRunStatus pReturn;
				pReturn = (PChRunStatus)se.pvBuffer;
				tmpMsg.Format("ch%d: status=%d, RecDirection=%d, RecTime=%d, DTMF=%s, "
					"RingTime=%s, RingCnt=%d, Caller=%s, Callee=%s",
					pReturn->cChID,pReturn->cChStatus,pReturn->cRecDir,pReturn->dwRecLen,
					pReturn->szDtmf,pReturn->szRingTime,pReturn->cRingCnt,pReturn->szCallerID,pReturn->szCalleeID
					);
			}
			else
			{
				tmpMsg.Format("GetChannelRunningStatus return=%d",se.nReference);
			}
			break;
		case E_GETDEVICETIME:
			if (se.nReference==0)
			{
				PDevTime pReturn;
				pReturn = (PDevTime)se.pvBuffer;
				tmpMsg.Format("DeviceTime=%s",pReturn->szDevTime);
			}
			else
			{
				tmpMsg.Format("E_GETDEVICETIME return=%d",se.nReference);
			}
			break;
		case E_GETDEVICEID:
			if (se.nReference==0)
			{
				tmpMsg.Format("DeviceID=%s",(char*)se.pvBuffer);
			}
			else
			{
				tmpMsg.Format("E_GETDEVICEID return=%d",se.nReference);
			}
			break;
		case E_LONGCALLALERT:
			if (se.nReference==0)
			{
				PRecEventInfo pReturn;
				pReturn = (PRecEventInfo)se.pvBuffer;
				tmpMsg.Format("(%s)ch%d calltime ALERT! Event time=%s", pReturn->szIPAddr, pReturn->nCh, pReturn->szEventTime);
			}
			else
			{
				tmpMsg.Format("CallerID event return=%d",se.nReference);
			}
			break;
		case E_OUTCALL_DTMF:
			if (se.nReference==0)
			{
				PDTMFEvent pReturn;
				pReturn = (PDTMFEvent)se.pvBuffer;
				tmpMsg.Format("(%s)ch%d DTMF=%c", pReturn->szIPAddr, pReturn->nCh, pReturn->cDtmf);
			}
			else
			{
				tmpMsg.Format("DTMF event return=%d",se.nReference);
			}
			break;
		case E_INCALL_CALLERID:
			if (se.nReference==0)
			{
				PCallerIDEvent pReturn;
				pReturn = (PCallerIDEvent)se.pvBuffer;
				tmpMsg.Format("(%s)ch%d rcv CallerID=%s", pReturn->szIPAddr, pReturn->nCh, pReturn->szCallerID);
			}
			else
			{
				tmpMsg.Format("INCALL CallerID event return=%d",se.nReference);
			}
			break;
		case E_MISSEDCALL:
			if (se.nReference==0)
			{
				PMissedCallEvent pReturn;
				pReturn = (PMissedCallEvent)se.pvBuffer;
				tmpMsg.Format("(%s)ch%d call missed, CallerID=%s", pReturn->szIPAddr, pReturn->nCh, pReturn->szCallerID);
			}
			else
			{
				tmpMsg.Format("Missed Call event return=%d",se.nReference);
			}
			break;
		case E_ONRECORDING:
			if (se.nReference==0)
			{
				POnRecordingEvent pReturn;
				pReturn = (POnRecordingEvent)se.pvBuffer;
				tmpMsg.Format("(%s)ch%d recording. BindPhone=%s, Event time=%s", pReturn->szIPAddr, pReturn->nCh,
					pReturn->szBindPhoneNum, pReturn->szEventTime);
			}
			else
			{
				tmpMsg.Format("Recording event return=%d",se.nReference);
			}
			break;
		case E_SETWEBPORT:
			if (se.nReference==0)
			{
				tmpMsg.Format("SetWebPort success");
				ChangeWebPort(m_dlgCSubDlgCfg.m_uPort);
			}
			else
			{
				tmpMsg.Format("E_SETWEBPORT return=%d",se.nReference);
			}
			break;
		case E_GETWEBPORT:
			if (se.nReference==0)
			{
				int *pn=(int*)se.pvBuffer;
				tmpMsg.Format("GetWebPort=%d.", *pn);
			}
			else
			{
				tmpMsg.Format("E_GETWEBPORT return=%d",se.nReference);
			}
			break;
		default:
			tmpMsg.Format("Unkown Event=%d", se.wEventCode);
		    break;
		}
		if (tmpMsg.GetLength()>50)
			appendMsg += "\r\n";

		appendMsg += tmpMsg;
		AppendResult(appendMsg);
	}

	CDialog::OnTimer(nIDEvent);
}

void CATPRemoteCtrlDlg::AppendResult(CString &msg)
{
	int u16size = (msg.GetLength()+1)*sizeof(wchar_t);
	wchar_t * pU16=(wchar_t*)malloc(u16size);
	int u16resultsize = MultiByteToWideChar(CP_UTF8, 0, msg, -1, pU16, msg.GetLength()+1);
	char * pMBCS = (char*)malloc(u16resultsize*2);
	WideCharToMultiByte(CP_ACP, 0, pU16, -1, pMBCS, u16resultsize*2,NULL,NULL);
	
	
	CString tmp;
	tmp.Format("%s\r\n%s",pMBCS, m_szResult);
	m_szResult = tmp;
	free(pU16);free(pMBCS);
	UpdateData(FALSE);
}

void CATPRemoteCtrlDlg::OnSelchangeComboServerList() 
{
	char tmp[10];
	m_nServerIndex = m_cbServerList.GetCurSel();
	strcpy(g_szDefaultServer, m_arrServer[m_nServerIndex]);
	strcpy(g_szDefaultEndPoint, m_arrEndPoint[m_nServerIndex]);
	if (m_dlgCSubDlgInit.m_szServerIP!=g_szDefaultServer)
	{
		m_dlgCSubDlgInit.m_szServerIP = g_szDefaultServer;
		m_dlgCSubDlgInit.GetDlgItem(IDC_EDIT_SERVER_IP)->SetWindowText(g_szDefaultServer);
	}
	if (m_dlgCSubDlgInit.m_uWebPort != m_arrPort[m_nServerIndex])
	{
		m_dlgCSubDlgInit.m_uWebPort = m_arrPort[m_nServerIndex];
		m_dlgCSubDlgInit.GetDlgItem(IDC_EDIT_WEBPORT)->SetWindowText(itoa(m_dlgCSubDlgInit.m_uWebPort,tmp, 10));
	}
}

void CATPRemoteCtrlDlg::ChangeWebPort(WORD uPort)
{
	if (m_nServerIndex<0) return;
	char str[100];
	sprintf(str, "%s:%d", g_szDefaultServer, uPort);

	m_arrPort[m_nServerIndex]=uPort;
	sprintf(g_szDefaultEndPoint, "http://%s/server.php", str);
	m_arrEndPoint[m_nServerIndex]=g_szDefaultEndPoint;
	
	m_cbServerList.DeleteString(m_nServerIndex);
	m_cbServerList.InsertString(m_nServerIndex, str);
	m_cbServerList.SetCurSel(m_nServerIndex);
	CString tmp;
	tmp.Format("Server Index=%d, new Port=%d, EndPoint=%s", m_nServerIndex+1, uPort, g_szDefaultEndPoint);
	AppendResult(tmp);

}
