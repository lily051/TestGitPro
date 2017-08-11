// PlayMp4Cvr.cpp : 实现文件
//

#include "stdafx.h"
#include "TransFileToCVR.h"
#include "PlayMp4Cvr.h"
#include "TransFileToCVRDlg.h"

// PlayMp4Cvr 对话框

IMPLEMENT_DYNAMIC(PlayMp4Cvr, CDialog)

PlayMp4Cvr::PlayMp4Cvr(CWnd* pParent /*=NULL*/)
	: CDialog(PlayMp4Cvr::IDD, pParent)
    , PlayHandle(0)
{
    //memset(&m_StructPlayInfo,0,sizeof(Play_Info));
    m_Event = CreateEvent(NULL,TRUE,FALSE,NULL);
    if(m_Event == INVALID_HANDLE_VALUE)
    {
        MessageBox(_T("create Event fail"));
        exit(0);
    }
    
}

PlayMp4Cvr::~PlayMp4Cvr()
{
}

void PlayMp4Cvr::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PLAY, m_PlayShow);
}


BEGIN_MESSAGE_MAP(PlayMp4Cvr, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_START, &PlayMp4Cvr::OnBnClickedButtonPlayStart)
END_MESSAGE_MAP()

BOOL PlayMp4Cvr::OnInitDialog()
{
    CDialog::OnInitDialog();

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
    //
    PlayM4_InitDDrawDevice();

    return TRUE;
}
// PlayMp4Cvr 消息处理程序
void CALLBACK HK_FILEPLAY(unsigned int playhandle,BYTE *data,DWORD datalen,NVR_DWORD user)
{
	PlayMp4Cvr *dlg = (PlayMp4Cvr*)user;
	
	if(!dlg)
	{
		MessageBox(NULL,_T("用户参数不正确!"),_T("提示"),MB_ICONEXCLAMATION);
		return ;
	}
    
    int lPort = 0;

	if(datalen <= 0)
	{
		MessageBox(NULL,_T("视频流传输结束!"),_T("提示"),MB_OK);
		return;
	}

    if(datalen == 40)
	{   
        /*if(!PlayM4_SetSourceBufCallBack(lPort, 10000, NULL, NULL, NULL))
		{
		    TRACE("SetSourceBufCallBack failed\n");
		}*/
		PlayM4_SetStreamOpenMode(lPort,STREAME_FILE);
		PlayM4_OpenStream(lPort,(PBYTE)data,datalen,POOLSIZE);

		if(!PlayM4_SetDisplayBuf(lPort,MAX_DIS_FRAMES))
		{

		}
		if(!PlayM4_Play(lPort,dlg->m_PlayShow.GetSafeHwnd()))
		{
            DWORD m_ErrorCode = PlayM4_GetLastError(lPort);
			CString MsgErr;
            MsgErr.Format(_T("PlayM4_Play is failde(err=%d)"),m_ErrorCode);
			MessageBox(NULL,MsgErr,_T("提示"),MB_ICONEXCLAMATION);
			return ;
		}
		return ;
	}
	
	if(datalen > 0)
	{
         BOOL isPut=FALSE;
         isPut=PlayM4_InputData(lPort,(PBYTE)data,datalen);
		 while(!isPut&&WaitForSingleObject(dlg->m_Event,50) == WAIT_TIMEOUT)
		 {
            isPut=PlayM4_InputData(lPort,(PBYTE)data,datalen);
			TRACE("wait");
			//Sleep(50);
		 }
        
	}

	return ;
	
}
void PlayMp4Cvr::OnBnClickedButtonPlayStart()
{
    // TODO: 在此添加控件通知处理程序代码
    CTransFileToCVRDlg* pDlg = (CTransFileToCVRDlg*) GetParent();
    PlayM4_SetDisplayRegion(0,0,NULL,GetDlgItem(IDC_STATIC_PLAY)->m_hWnd,TRUE);

    PlayHandle = BnNVR_GetStream(pDlg->m_StructPlayInfo.m_nLoginID,pDlg->m_StructPlayInfo.CamerID,0,
        pDlg->m_StructPlayInfo.startTime,pDlg->m_StructPlayInfo.endTime,HK_FILEPLAY,(NVR_DWORD)this);

    if (-1 == PlayHandle)
    {
        DWORD m_ErrorCode=BnNVR_GetLastError();
        CString MsgErr;
        MsgErr.Format(_T("启动播放失败 is failde(err=%d)"),m_ErrorCode);
        MessageBox(MsgErr);
    }
    else
    {
        PlayHandle = 0;
        ResetEvent(m_Event);
    }
}
