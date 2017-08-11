
// VideoCloudInterfaceTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoCloudInterfaceTestDlg.h"
#include "hcs_sdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CVideoCloudInterfaceTestDlg �Ի���




CVideoCloudInterfaceTestDlg::CVideoCloudInterfaceTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoCloudInterfaceTestDlg::IDD, pParent)
    , m_nDevPort(6021)
    , m_csUser(_T("root"))
    , m_csPWD(_T("6b74M23cd87D"))
    , m_bLogin(false)
    , m_nLogID(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CVideoCloudInterfaceTestDlg::~CVideoCloudInterfaceTestDlg()
{
    if (-1 != m_nLogID)
    {
        HCS_SDK_Logout(m_nLogID);
    }
    HCS_SDK_Deinit();
}
void CVideoCloudInterfaceTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS, m_ctrlDevIp);
    DDX_Text(pDX, IDC_EDIT_PORT, m_nDevPort);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_csUser);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPWD);
    DDX_Control(pDX, IDC_TAB, m_ctrlTab);
}

BEGIN_MESSAGE_MAP(CVideoCloudInterfaceTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CVideoCloudInterfaceTestDlg::OnTcnSelchangeTabPlayback)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CVideoCloudInterfaceTestDlg::OnBnClickedButtonLogin)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_GET_VERSION, &CVideoCloudInterfaceTestDlg::OnBnClickedButtonGetVersion)
END_MESSAGE_MAP()

void CVideoCloudInterfaceTestDlg::OnTcnSelchangeTabPlayback(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int CurSel;
    CurSel = m_ctrlTab.GetCurSel();
    switch (CurSel)
    {
    case 0:
        m_PageWrite.ShowWindow(SW_SHOW);
        m_PageQueryAndDownload.ShowWindow(SW_HIDE);
        m_PageActiveCloud.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_PageWrite.ShowWindow(SW_HIDE);
        m_PageQueryAndDownload.ShowWindow(SW_SHOW);
        m_PageActiveCloud.ShowWindow(SW_HIDE);
        break;
    case 2:
        m_PageWrite.ShowWindow(SW_HIDE);
        m_PageQueryAndDownload.ShowWindow(SW_HIDE);
        m_PageActiveCloud.ShowWindow(SW_SHOW);
        break;
    default:
        break;
    }
    *pResult = 0;
}
// CVideoCloudInterfaceTestDlg ��Ϣ�������

BOOL CVideoCloudInterfaceTestDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_ctrlDevIp.SetAddress(10,10,48,231);
    //��ʼ������ҳ
    m_ctrlTab.InsertItem(0,_T("�ļ�д��"));
    m_PageWrite.Create(IDD_DIALOG_WRITE_FILE,&m_ctrlTab);
    m_ctrlTab.InsertItem(1,_T("¼���ѯ"));
    m_PageQueryAndDownload.Create(IDD_DIALOG_VIDEO_QUERY,&m_ctrlTab);
    m_ctrlTab.InsertItem(3,_T("�����Ʋ���"));
    m_PageActiveCloud.Create(IDD_DIALOG_ACTIVE_CLOUD,&m_ctrlTab);

    CRect rc;
    m_ctrlTab.GetWindowRect(&rc);
    m_PageWrite.MoveWindow(1,22,rc.Width()-2,rc.Height()-22);
    m_PageQueryAndDownload.MoveWindow(1,22,rc.Width()-2,rc.Height()-22);
    m_PageActiveCloud.MoveWindow(1,22,rc.Width()-2,rc.Height()-22);
    m_PageWrite.ShowWindow(SW_SHOW);
    m_ctrlTab.SetCurSel(0);
    //
    int ret = HCS_SDK_Init();
    if (HCS_OK != ret)
    {
        MessageBox(_T("HCS_SDK_Init is fail..."));
    }
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVideoCloudInterfaceTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoCloudInterfaceTestDlg::OnPaint()
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
HCURSOR CVideoCloudInterfaceTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVideoCloudInterfaceTestDlg::OnBnClickedButtonLogin()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    if (!m_bLogin)
    {
        UpdateData(TRUE);
        CString DeviceIp;
        BYTE nField0,nField1,nField2,nField3;
        m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
        DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
        int ret = HCS_SDK_Login(W2A(DeviceIp),(unsigned int)m_nDevPort,W2A(m_csUser),W2A(m_csPWD),&m_nLogID);
        if (HCS_OK != ret)
        {
            MessageBox(_T("��¼ʧ��"));
            return;
        }
        else
        {
            m_PageWrite.SetLoginID(m_nLogID);
            m_PageActiveCloud.SetLoginID(m_nLogID);
            m_PageQueryAndDownload.SetLoginID(m_nLogID);
            m_bLogin = true;
            GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("�ǳ�"));
        }
    }
    else
    {
        //ע���ж��Ƿ���¼��طš����ص��ڽ���-����ֹͣ------���

        //
        int retOut = HCS_SDK_Logout(m_nLogID);
        if (HCS_OK != retOut)
        {
            MessageBox(_T("�ǳ�ʧ��"));
            return;
        }
        else
        {
            m_bLogin = false;
            m_nLogID = -1;
            m_PageWrite.SetLoginID(-1);
            m_PageActiveCloud.SetLoginID(-1);
            m_PageQueryAndDownload.SetLoginID(-1);
            GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("��¼"));
        }
    }
}

void CVideoCloudInterfaceTestDlg::OnBnClickedButtonGetVersion()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    CString DeviceIp;
    BYTE nField0,nField1,nField2,nField3;
    m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
    DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
    HCS_Version strucVersion;
    memset(&strucVersion,0,sizeof(HCS_Version));

    int ret = HCS_SDK_GetVersion(W2A(DeviceIp),(unsigned int)m_nDevPort,&strucVersion);
    if (HCS_OK != ret)
    {
        MessageBox(_T("��ȡ�汾ʧ��"));
        return;
    }
    CString m_version;
    m_version.Format(_T("�汾��Ϣ:%d.%d.%d"),strucVersion.major_version,strucVersion.sub_version,strucVersion.minor_version);
    MessageBox(m_version,_T("��ʾ"),MB_ICONWARNING);
    //��ȡ��ԴID
    if (m_bLogin)
    {
        int total_cout = 0;
        int handle = 0;
        int ret = HCS_SDK_GetAllPoolInfoStart(m_nLogID, &total_cout, &handle);
        if (ret != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("��ȡ�汾ʧ��(�������:%d)"), ret);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        }
        else
        {
            int status;
            HCS_PoolInfo info;
            int next_count = total_cout;
            while(next_count > 0)
            {
                status = 0;
                memset(&info, 0, sizeof(info));
                int rets = HCS_SDK_GetAllPoolInfoNext(m_nLogID,handle,&info,&status);
                if (rets != HCS_OK)
                {
                    CString m_csErr;
                    m_csErr.Format(_T("��ȡ��һ����Դ����Ϣʧ��(�������:%d)"), ret);
                    MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
                    break;
                }
                else
                {
                    if (1 == status)
                    {
                        next_count--;
                        VecPoolInfo.push_back(info);
                        continue;
                    }
                    else if (2 == status)
                    {
                        Sleep(200);
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            m_PageWrite.GetPoolIDInfo(VecPoolInfo);
            HCS_SDK_GetAllPoolInfoStop(m_nLogID,handle);
        }
    }
}
