
// HlogDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HlogDemo.h"
#include "HlogDemoDlg.h"
#include "hlogConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//BLOCK_LOG_OBJECT(MYLOG_OBJECT_NAME);
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


// CHlogDemoDlg �Ի���




CHlogDemoDlg::CHlogDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHlogDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHlogDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHlogDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_WRITELOG, &CHlogDemoDlg::OnBnClickedBtnWritelog)
    ON_BN_CLICKED(IDC_BTN_NEW, &CHlogDemoDlg::OnBnClickedBtnNew)
END_MESSAGE_MAP()


// CHlogDemoDlg ��Ϣ�������

BOOL CHlogDemoDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHlogDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHlogDemoDlg::OnPaint()
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
HCURSOR CHlogDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHlogDemoDlg::OnBnClickedBtnWritelog()
{
    //1������Ҫ��ʼ����ֱ��ʹ�ö�Ӧ�ĺ�������;
    //2��ֻ֧��log4cxx.propertiesһ�������ļ�;
    //3�������ö���̵�����ͬ�������ļ�����ͬ���������־��Ŀ�ĵز�Ҫ��ͬ;----�������л������ļ�������
    //4���ھ�̬����ʹ�� MFC,������ʹ�ã���Ȼ���ǻᱻMFC���������ڴ�й¶;
    //BLOCK_LOG_OBJECT(MYLOG_OBJECT_NAME);
    MYLOG_ERROR("error test %d", ::GetTickCount());
    MYLOG_WARN("warn  test %d", ::GetTickCount());
    MYLOG_INFO("warn  test %d", ::GetTickCount());
    MYLOG_DEBUG("debug test %d", ::GetTickCount());
    MYLOG_TRACE("warn  test %d", ::GetTickCount());

    //����
    MYLOG_INFO("warn  test %s", "֧������");

    //hlog_format(HLOG_LEVEL level, const char* module, const char* format, ...)
    //CString
    CString sMsg;
    sMsg = _T("֧�����ģ�_T");
    MYLOG_INFO("warn  test %s", sMsg);//���룬����ȷ��д��
    CStringA sMsgA(sMsg);
    MYLOG_INFO("warn  test %s", sMsgA);//��ȷ

    //std::string
    std::string ss = "��������";
    MYLOG_INFO("warn  test %s", ss);//����ȷ,�����null
    MYLOG_INFO("warn  test %s", ss.c_str());//��ȷ
}

void CHlogDemoDlg::OnBnClickedBtnNew()
{
    //ʹ�����Ͱ�ȫ����־���
    MYLOG_EX_INFO("warn  test {0}", 8100);
    MYLOG_EX_INFO("warn  test {0},{1}", 8100,"����");

    CString sMsg;
    sMsg = _T("֧������");
    CStringA sMsgA(sMsg);
    std::string ss = "��������";
    MYLOG_EX_INFO("����{0},{1}");//û���⣬������
    MYLOG_EX_INFO("����{0},{1}", 8100,sMsgA,ss.c_str());//������һ��,û���⣬������
    MYLOG_EX_INFO("����{0},{1}", 8100,ss);//������һ��,û���⣬������
    MYLOG_EX_INFO("����{0},{1},{2}", 8100,sMsgA);//������һ��,û���⣬������
    MYLOG_EX_INFO("����{0},{1},{2}", 8100,sMsgA,ss.c_str());//������÷�
}
