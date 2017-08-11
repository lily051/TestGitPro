
// AE_USBDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AE_USBDemo.h"
#include "AE_USBDemoDlg.h"


#define T_AUDIO16	101
#define T_AUDIO8	100
#define T_UYVY		1
#define T_YV12		3
#define T_RGB32		7

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define RECV_PIC_MAX			2 * 1024 * 1024

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAE_USBDemoDlg 对话框

//保存码流
void StreamRec(char *buf, int len)
{
    int nWriteNum = 0;
    FILE * fp = fopen("./Hik_Rec.mp4", "a+b");
    if (!fp)
    {
        return;
    }

    nWriteNum = fwrite(buf, sizeof(char), len, fp);
    if (nWriteNum != len)
    {
        fclose(fp);
        return;
    }
    fclose(fp);
}

void CAE_USBDemoDlg::OnDevStatus(USB_HANDLE pHandle, int nStatus, void* pUser)
{
    CString str;
    str.Format(_T("\r\n*****%d*****\r\n"), nStatus);
    OutputDebugString(str);
}

void CAE_USBDemoDlg::OnRecvRealPlay(STREAM_HANDLE hRealPlayHandle, int nDataType, char* pBuffer, int nRetLen, void* pUser)
{
    CString str;
    str.Format(_T("\r\n*****%d*****\r\n"), nDataType);
    OutputDebugString(str);
    
    CAE_USBDemoDlg * pWnd = (CAE_USBDemoDlg *)pUser;

    pWnd->InputData((PBYTE)pBuffer, nRetLen, nDataType);
   
}

BOOL CAE_USBDemoDlg::InputData(PBYTE pBuffer,DWORD dwSize,long lFrameType)
{
    //保存码流
    if (m_bRecord)
    {
        StreamRec((char*)pBuffer, (int)dwSize);
    }

    //如果是头数据
    if (0 == lFrameType)
    {
        //生成解码类型
       
        return StartPlay(PT_HIK, pBuffer, dwSize, _T(""));
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

    if (FALSE == GPAPI_InputData(PT_HIK,m_lPort,pBuffer,dwSize))
    {
        return FALSE;
    }
    return TRUE;

}


void CALLBACK DecCallBackFun(long nPort,
                             char * pBuf,
                             long nSize,
                             HIK_FRAME_INFO * pFrameInfo, 
                             long nUser, 
                             long nReserved2)
{
    CAE_USBDemoDlg *pWnd = (CAE_USBDemoDlg *)nUser;
    if( (pWnd != NULL) && (!pWnd->DealDecodeData(nPort,pBuf,nSize,pFrameInfo)))
    {
        CString strTemp;
        strTemp.Format(_T(" --- DealDecodeData --- 错误,数据类型:%d\n"), pFrameInfo->nType);
        OutputDebugString(strTemp);
    }
}

//流画面回调，用于在画面上画东西
void CALLBACK CAE_USBDemoDlg::DrawFunMsgCB(long nPort,HDC hDC,long lUser)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CAE_USBDemoDlg * pWnd = (CAE_USBDemoDlg *)UlongToPtr(lUser);


//     CRect rcText;					//提示文字矩形
//     int nTextHeight = 20;			//字体高度
//     pWnd->GetClientRect(&rcText);
//     rcText.left = rcText.right - 210; 
//     rcText.bottom = nTextHeight;
// 
//     //生成cdc
//     CDC dc ;
//     dc.Attach(hDC);
// 
//     //本地录像提示
//     if (pWnd->IsRecord())
//     {
//         CString strRecordTip;
//         // 		COleDateTime dtRecordCurrenttime = COleDateTime::GetCurrentTime();
//         // 		COleDateTimeSpan timeSpan = dtRecordCurrenttime - pWnd->m_dtRecordTime;
//         CTime tmNow = CTime::GetCurrentTime();  //COLEDateTime调用异常，使用CTime::GetCurrentTime
//         CTimeSpan timeSpan = tmNow - pWnd->m_dtRecordTime;
//         strRecordTip = timeSpan.Format(_T("●录像时长 %H:%M:%S"));
// 
//         pWnd->DrawMyText(&dc,strRecordTip,rcText);
// 
//         rcText.OffsetRect(0,nTextHeight);
//     }
// 
//     if (pWnd->IsSound())
//     {
//         pWnd->DrawMyText(&dc,_T("          ●音频开启"),rcText);
//         rcText.OffsetRect(0,nTextHeight);
//     }
// 
//     dc.Detach();
}


CAE_USBDemoDlg::CAE_USBDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAE_USBDemoDlg::IDD, pParent)
    ,m_hDevice(NULL)
    ,m_hLoginHandle(NULL)
    ,m_nStreamHandle(-1)
    ,m_lPort(-1)
    ,m_bRecord(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAE_USBDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DEVLIST, m_comboDevList);
    DDX_Control(pDX, IDC_PREVIEW_WND, m_prewiewWnd);
    DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
    DDX_Control(pDX, IDC_EDIT_PWD, m_editPwd);
}

BEGIN_MESSAGE_MAP(CAE_USBDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CAE_USBDemoDlg::OnBnClickedBtnLogin)
    ON_BN_CLICKED(IDC_BTN_START_PREVIEW, &CAE_USBDemoDlg::OnBnClickedBtnStartPreview)
    ON_BN_CLICKED(IDC_BTN_LOGOUT, &CAE_USBDemoDlg::OnBnClickedBtnLogout)
    ON_BN_CLICKED(IDC_BTN_STOP_PREVIEW, &CAE_USBDemoDlg::OnBnClickedBtnStopPreview)
    ON_BN_CLICKED(IDC_BTN_START_RECORD, &CAE_USBDemoDlg::OnBnClickedBtnStartRecord)
    ON_BN_CLICKED(IDC_BTN_STOP_RECORD, &CAE_USBDemoDlg::OnBnClickedBtnStopRecord)
    ON_BN_CLICKED(IDC_BTN_CAPTURE, &CAE_USBDemoDlg::OnBnClickedBtnCapture)
END_MESSAGE_MAP()


// CAE_USBDemoDlg 消息处理程序

BOOL CAE_USBDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    m_editUser.SetWindowText(_T("admin"));
    m_editPwd.SetWindowText(_T("1234567890"));
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);

	//初始化SDK
    AEUSB_Initialize();

    //获取所有设备列表
    char szDevName[512];
    usb_simple_device* usbDev;
    int nCount = AEUSB_GetUsbDevList(usbDev);
    for (int i = 0; i < nCount; i++)
    {
        if (NULL != usbDev->pDev)
        {
            strncpy_s(szDevName, 512, usbDev->szFileName, _TRUNCATE);
            CString strName(szDevName);
            m_comboDevList.AddString(strName);

            m_hDevice = usbDev->pDev;
        }
    }
    m_comboDevList.SetCurSel(0);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAE_USBDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAE_USBDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAE_USBDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAE_USBDemoDlg::OnDestroy()
{
    CDialog::OnDestroy();

    if (INVALID_STREAM_HANDLE != m_nStreamHandle)
    {
        AEUSB_StopRealPlay(m_hLoginHandle, m_nStreamHandle);
        m_nStreamHandle = INVALID_STREAM_HANDLE;
    }

    StopPlay();

    if (NULL != m_hLoginHandle)
    {
        AEUSB_LogOut(m_hLoginHandle);
        m_hLoginHandle = NULL;
    }
    
    //反初始化
    AEUSB_Finalize();
}

//登录设备
void CAE_USBDemoDlg::OnBnClickedBtnLogin()
{
    // TODO: 在此添加控件通知处理程序代码
    USES_CONVERSION;

    UpdateData(TRUE);

    if (NULL == m_hDevice)
    {
        AfxMessageBox(_T("请先选择设备！"));
        return;
    }
    CString strUser;
    CString strPwd;
    m_editUser.GetWindowText(strUser);
    m_editPwd.GetWindowText(strPwd);

    char * cUser = T2A(strUser);
    char * cPwd = T2A(strPwd);
    m_hLoginHandle = AEUSB_LogIn(m_hDevice, cUser, strlen(cUser), cPwd, strlen(cPwd));
    if (NULL == m_hLoginHandle)
    {
        AfxMessageBox(_T("登录失败！"));
        return;
    }
    GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);

    AEUSB_SetDevStatusCallBack(m_hLoginHandle, OnDevStatus, (void*)this);
}

//登出设备
void CAE_USBDemoDlg::OnBnClickedBtnLogout()
{
    // TODO: 在此添加控件通知处理程序代码
    if (NULL != m_hLoginHandle)
    {
        AEUSB_LogOut(m_hLoginHandle);
        m_hLoginHandle = NULL;
        GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
    }
}

void CAE_USBDemoDlg::OnBnClickedBtnStartPreview()
{
    // TODO: 在此添加控件通知处理程序代码
    DevRealPlay realPlay = {0};
    realPlay.nChannel = 1;
    realPlay.nType = 0;
    m_nStreamHandle = AEUSB_StartRealPlay(m_hLoginHandle, &realPlay, OnRecvRealPlay, this);
    if (INVALID_STREAM_HANDLE == m_nStreamHandle)
    {
        AfxMessageBox(_T("开始预览失败！"));
        return;
    }
}

//停止预览
void CAE_USBDemoDlg::OnBnClickedBtnStopPreview()
{
    // TODO: 在此添加控件通知处理程序代码
    if (INVALID_STREAM_HANDLE == m_nStreamHandle)
    {
        return;
    }
    AEUSB_StopRealPlay(m_hLoginHandle, m_nStreamHandle);
    m_nStreamHandle = INVALID_STREAM_HANDLE;

    StopPlay();
}

BOOL CAE_USBDemoDlg::StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize, CString strCamName /*= _T("")*/) 
{
    CString strMsg;

    //GetPort
    LONG nPort = -1;
    if (!GPAPI_GetPort(playerType,nPort, m_prewiewWnd.GetSafeHwnd()))
    {
        strMsg.Format(_T("@preview %s GPAPI_GetPort unsuccessfully"),strCamName);
        OutputDebugString(strMsg);

        return FALSE;
    }

    //保存port
    m_lPort = nPort;

    //设置解码回调
    if(!GPAPI_SetDecCallBack(PT_HIK, m_lPort, DecCallBackFun, NULL, 0, (LONG)this))
    {
       AfxMessageBox(_T("GPAPI_SetDecCallBack failed!"));
       return FALSE;
    }

  
    char * cCamName = "AE_USBCamera";
    if (-1 == GPAPI_StartPlay(PT_HIK, m_lPort, m_prewiewWnd.GetSafeHwnd(), pBuffer, dwBufferSize, cCamName, DrawFunMsgCB, (long)this))
    {
        StopPlay();
        return FALSE;
    }


    return TRUE;
}

BOOL CAE_USBDemoDlg::DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo)
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
        {
//             AUDIOFRAME audioFrame;
//             audioFrame.nFrameRate = pFrame->nFrameRate;
//             audioFrame.nStamp = pFrame->nStamp;
//             audioFrame.nType = 8;
//             audioFrame.nSize = nSize;
//             char *bufTemp = new char[nSize];
//             memcpy_s(bufTemp,nSize,pBuf,nSize);
//             audioFrame.pBuf = bufTemp;
//             if(NULL != m_pVideoWav)
//             {
//                 m_pVideoWav->AddAudioFrame(audioFrame);
//             }
        }
        break;
    case T_AUDIO16:  //采样率 16khz，单声道，每个采样点16位
        {
//             AUDIOFRAME audioFrame;
//             audioFrame.nFrameRate = pFrame->nFrameRate;
//             audioFrame.nStamp = pFrame->nStamp;
//             audioFrame.nType = 16;
//             audioFrame.nSize = nSize;
//             char *bufTemp = new char[nSize];
//             memcpy_s(bufTemp,nSize,pBuf,nSize);
//             audioFrame.pBuf = bufTemp;
//             if(NULL != m_pVideoWav)
//             {
//                 m_pVideoWav->AddAudioFrame(audioFrame);
//             }
        }
        break;
    case T_UYVY:
        break;
    case T_YV12:    //hik
        break;
    case T_RGB32:
        break;
    default:
        break;
    }
    return TRUE;
}


/*******************************************************************************
*    Function:      StopPlay
*    Description:   停止播放
*    Input:         
*    Output:       
*    Return:	     
*******************************************************************************/
void CAE_USBDemoDlg::StopPlay()
{
   
    if (-1 != m_lPort)
    {
         GPAPI_StopSoundShare(PT_HIK, m_lPort);
        GPAPI_StopPlay(PT_HIK,m_lPort);
        GPAPI_FreePort(PT_HIK,m_lPort);

        //数据恢复初始
        m_lPort = -1;
    }

}
void CAE_USBDemoDlg::OnBnClickedBtnStartRecord()
{
    // TODO: 在此添加控件通知处理程序代码
    m_bRecord = TRUE;
}

void CAE_USBDemoDlg::OnBnClickedBtnStopRecord()
{
    // TODO: 在此添加控件通知处理程序代码
    m_bRecord = FALSE;
}

void CAE_USBDemoDlg::OnBnClickedBtnCapture()
{
    // TODO: 在此添加控件通知处理程序代码
    if (NULL != m_hLoginHandle)
    {
        int nLenOut = 0;
        char* pBuf = new char[RECV_PIC_MAX];
        memset(pBuf, 0, RECV_PIC_MAX);

        DevCapPicture capPic;
        memset(&capPic, 0, sizeof(DevCapPicture));
        capPic.nChannel = 0;
        capPic.pPicBuff = pBuf;
        capPic.nInBufLen = RECV_PIC_MAX;
        capPic.nOutBufLen = &nLenOut;

        int nRet = AEUSB_CapPicture(m_hLoginHandle, &capPic);
        if (nRet != USB_SDK_OK)
        {
            AfxMessageBox(_T("抓图失败！"));
            return;
        }


        if (pBuf != NULL && nLenOut > 0)
        {
            int nWriteNum = 0;
            FILE * fp = fopen("./cap.jpg", "wb");
            if (!fp)
            {
                return;
            }

            nWriteNum = fwrite(capPic.pPicBuff, sizeof(char), *(capPic.nOutBufLen), fp);
            if (nWriteNum != *(capPic.nOutBufLen))
            {
                fclose(fp);
                return;
            }
            fclose(fp);
            AfxMessageBox(_T("抓图成功！"));
        }
      
    }

}
