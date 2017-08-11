
// AE_USBDemoDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAE_USBDemoDlg �Ի���

//��������
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
    //��������
    if (m_bRecord)
    {
        StreamRec((char*)pBuffer, (int)dwSize);
    }

    //�����ͷ����
    if (0 == lFrameType)
    {
        //���ɽ�������
       
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
        strTemp.Format(_T(" --- DealDecodeData --- ����,��������:%d\n"), pFrameInfo->nType);
        OutputDebugString(strTemp);
    }
}

//������ص��������ڻ����ϻ�����
void CALLBACK CAE_USBDemoDlg::DrawFunMsgCB(long nPort,HDC hDC,long lUser)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CAE_USBDemoDlg * pWnd = (CAE_USBDemoDlg *)UlongToPtr(lUser);


//     CRect rcText;					//��ʾ���־���
//     int nTextHeight = 20;			//����߶�
//     pWnd->GetClientRect(&rcText);
//     rcText.left = rcText.right - 210; 
//     rcText.bottom = nTextHeight;
// 
//     //����cdc
//     CDC dc ;
//     dc.Attach(hDC);
// 
//     //����¼����ʾ
//     if (pWnd->IsRecord())
//     {
//         CString strRecordTip;
//         // 		COleDateTime dtRecordCurrenttime = COleDateTime::GetCurrentTime();
//         // 		COleDateTimeSpan timeSpan = dtRecordCurrenttime - pWnd->m_dtRecordTime;
//         CTime tmNow = CTime::GetCurrentTime();  //COLEDateTime�����쳣��ʹ��CTime::GetCurrentTime
//         CTimeSpan timeSpan = tmNow - pWnd->m_dtRecordTime;
//         strRecordTip = timeSpan.Format(_T("��¼��ʱ�� %H:%M:%S"));
// 
//         pWnd->DrawMyText(&dc,strRecordTip,rcText);
// 
//         rcText.OffsetRect(0,nTextHeight);
//     }
// 
//     if (pWnd->IsSound())
//     {
//         pWnd->DrawMyText(&dc,_T("          ����Ƶ����"),rcText);
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


// CAE_USBDemoDlg ��Ϣ�������

BOOL CAE_USBDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    m_editUser.SetWindowText(_T("admin"));
    m_editPwd.SetWindowText(_T("1234567890"));
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);

	//��ʼ��SDK
    AEUSB_Initialize();

    //��ȡ�����豸�б�
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



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAE_USBDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
    
    //����ʼ��
    AEUSB_Finalize();
}

//��¼�豸
void CAE_USBDemoDlg::OnBnClickedBtnLogin()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;

    UpdateData(TRUE);

    if (NULL == m_hDevice)
    {
        AfxMessageBox(_T("����ѡ���豸��"));
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
        AfxMessageBox(_T("��¼ʧ�ܣ�"));
        return;
    }
    GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);

    AEUSB_SetDevStatusCallBack(m_hLoginHandle, OnDevStatus, (void*)this);
}

//�ǳ��豸
void CAE_USBDemoDlg::OnBnClickedBtnLogout()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    DevRealPlay realPlay = {0};
    realPlay.nChannel = 1;
    realPlay.nType = 0;
    m_nStreamHandle = AEUSB_StartRealPlay(m_hLoginHandle, &realPlay, OnRecvRealPlay, this);
    if (INVALID_STREAM_HANDLE == m_nStreamHandle)
    {
        AfxMessageBox(_T("��ʼԤ��ʧ�ܣ�"));
        return;
    }
}

//ֹͣԤ��
void CAE_USBDemoDlg::OnBnClickedBtnStopPreview()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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

    //����port
    m_lPort = nPort;

    //���ý���ص�
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
    case T_AUDIO16:  //������ 16khz����������ÿ��������16λ
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
*    Description:   ֹͣ����
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

        //���ݻָ���ʼ
        m_lPort = -1;
    }

}
void CAE_USBDemoDlg::OnBnClickedBtnStartRecord()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_bRecord = TRUE;
}

void CAE_USBDemoDlg::OnBnClickedBtnStopRecord()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_bRecord = FALSE;
}

void CAE_USBDemoDlg::OnBnClickedBtnCapture()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
            AfxMessageBox(_T("ץͼʧ�ܣ�"));
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
            AfxMessageBox(_T("ץͼ�ɹ���"));
        }
      
    }

}
