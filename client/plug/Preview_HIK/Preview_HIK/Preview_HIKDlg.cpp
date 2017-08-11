
// Preview_HIKDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Preview_HIK.h"
#include "Preview_HIKDlg.h"
#include "DlgHelp.h"
#include "DlgSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDM_SETTING 0x9010
#define IDM_HELP    0x9011

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


// CPreview_HIKDlg �Ի���




CPreview_HIKDlg::CPreview_HIKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreview_HIKDlg::IDD, pParent)
    , m_lLoginID(-1)
    , m_lPlayHandle(-1)
    , m_pPicDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPreview_HIKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPreview_HIKDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CPreview_HIKDlg ��Ϣ�������

BOOL CPreview_HIKDlg::OnInitDialog()
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
            pSysMenu->AppendMenu(MF_STRING, IDM_SETTING, _T("����(S)"));
            pSysMenu->AppendMenu(MF_STRING, IDM_HELP, _T("����(H)"));
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    m_bkBrush.CreateSolidBrush(RGB(0,0,0));//�Ի��򱳾�ɫ

    USES_CONVERSION;
    memset(&m_devInfo,0,sizeof(NET_DVR_DEVICEINFO_V30));
    m_lLoginID = NET_DVR_Login_V30(T2A(theApp.m_cmdParam.sIP),theApp.m_cmdParam.nPort,
        T2A(theApp.m_cmdParam.sUser),T2A(theApp.m_cmdParam.sPWD),&m_devInfo);
    if(-1 == m_lLoginID)
    {
        DWORD err=NET_DVR_GetLastError();
        CString csErr;
        csErr.Format(_T("�豸��¼ʧ�ܣ��������%d"),err);
        MessageBox(csErr);
    }
    else
    {
        StartPlay(theApp.m_cmdParam.nChannel);
    }

    if (NULL == m_pPicDlg)
    {
        m_pPicDlg = new CDlgPic();
        if (NULL != m_pPicDlg)
        {
            m_pPicDlg->Create(CDlgPic::IDD, this);
            SetPicDlgShowPos();
        }
    }

    CString szTemp;
    szTemp.Format(_T("Preview_HIK/%s/Camera_%d"),theApp.m_cmdParam.sIP,theApp.m_cmdParam.nChannel);
    this->SetWindowText(szTemp);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPreview_HIKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
    else if (IDM_SETTING == nID)
    {
        CDlgSetting dlgSetting;
        dlgSetting.DoModal();
    }
    else if (IDM_HELP == nID)
    {
        CDlgHelp dlgHelp;
        dlgHelp.DoModal();
    }
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPreview_HIKDlg::OnPaint()
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
HCURSOR CPreview_HIKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPreview_HIKDlg::OnCancel()
{
    StopPlay();
    if (-1 != m_lLoginID)
    {
        NET_DVR_Logout_V30(m_lLoginID);
    }
    CDialog::OnCancel();
}

HBRUSH CPreview_HIKDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    if(nCtlColor == CTLCOLOR_DLG)   // �ж��Ƿ��ǶԻ���
    {
        return   m_bkBrush;
    }
    return hbr;
}

BOOL CPreview_HIKDlg::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            int nKeydown = (int)pMsg->wParam;
            if ('S'== nKeydown || 's' == nKeydown)
            {
                CDlgSetting dlgSetting;
                dlgSetting.DoModal();
            }
            else if ('H'== nKeydown || 'h' == nKeydown)
            {
                CDlgHelp dlgHelp;
                dlgHelp.DoModal();
            }
            else if ('P'== nKeydown || 'p' == nKeydown)
            {
                CapturePicture();
            }
            else if (VK_RETURN == pMsg->wParam)
            {
                return TRUE;
            }
            else if (VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
            else if (VK_SPACE == pMsg->wParam)
            {
                if (!this->IsZoomed())
                {
                    ShowWindow(SW_MAXIMIZE);
                }
                else
                {
                    ShowWindow(SW_RESTORE);
                }
                this->SetFocus();//���¾۽��������û��ٴε�����ڣ��Ϳ��Խ����´ε���Ϣ
                return TRUE;
            }
        }
    default:
        break;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CPreview_HIKDlg::StartPlay(int nChannel)
{
    NET_DVR_CLIENTINFO ClientInfo;
    ClientInfo.hPlayWnd = this->m_hWnd;
    ClientInfo.lChannel = nChannel;
    ClientInfo.lLinkMode = 0;
    ClientInfo.sMultiCastIP = NULL;
    m_lPlayHandle = NET_DVR_RealPlay_V30(m_lLoginID,&ClientInfo,NULL,NULL,TRUE);
    NET_DVR_RigisterDrawFun(m_lPlayHandle,NULL,(DWORD)this);
    if(-1 == m_lPlayHandle)
    {
        DWORD err=NET_DVR_GetLastError();
        CString csErr;
        csErr.Format(_T("���ų����������%d"),err);
        MessageBox(csErr);
    }
}

void CPreview_HIKDlg::StopPlay()
{
    if (-1 != m_lPlayHandle)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle=-1;
    }
}

void CPreview_HIKDlg::CapturePicture()
{
    if (-1 != m_lPlayHandle)
    {
        NET_DVR_JPEGPARA jpegPara;
        jpegPara.wPicSize = 0xff;
        jpegPara.wPicQuality = 0;
        CTime now = CTime::GetCurrentTime();
        CString strPicName;
        strPicName.Format(_T("%s%s_%2d_%04d%02d%02d%02d%02d%02d.jpg"),
            theApp.m_strPicPath,theApp.m_cmdParam.sIP,theApp.m_cmdParam.nChannel,
            now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),now.GetMinute(),now.GetSecond());
        USES_CONVERSION;
        if (NET_DVR_CaptureJPEGPicture(m_lLoginID,theApp.m_cmdParam.nChannel,&jpegPara,T2A(strPicName)))
        {
            if (m_pPicDlg)
            {
                m_pPicDlg->SetPic(strPicName);
                m_pPicDlg->ShowWindow(SW_SHOW);
                this->SetFocus();
            }
        }
    }
}
void CPreview_HIKDlg::OnDestroy()
{
    CDialog::OnDestroy();

    if (m_pPicDlg)
    {
        m_pPicDlg->DestroyWindow();
        delete m_pPicDlg;
        m_pPicDlg = NULL;
    }
}

void CPreview_HIKDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_pPicDlg)
    {
        SetPicDlgShowPos();
        Invalidate();
    }
}
void CPreview_HIKDlg::SetPicDlgShowPos()
{
    int  cx  =  GetSystemMetrics(SM_CXSCREEN);  //�����Ļ�ķֱ���
    int  cy  =  GetSystemMetrics(SM_CYSCREEN);  
    CRect rcWnd;
    m_pPicDlg->GetWindowRect(rcWnd);
    m_pPicDlg->SetWindowPos(NULL,cx-rcWnd.Width()-4, cy-rcWnd.Height()-44, 0, 0, SWP_NOSIZE);
}

