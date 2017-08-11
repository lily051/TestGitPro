
// HlogDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HlogDemo.h"
#include "HlogDemoDlg.h"
#include "hlogConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//BLOCK_LOG_OBJECT(MYLOG_OBJECT_NAME);
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


// CHlogDemoDlg 对话框




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


// CHlogDemoDlg 消息处理程序

BOOL CHlogDemoDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHlogDemoDlg::OnPaint()
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
HCURSOR CHlogDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHlogDemoDlg::OnBnClickedBtnWritelog()
{
    //1、不需要初始化，直接使用对应的函数方法;
    //2、只支持log4cxx.properties一个配置文件;
    //3、不能用多进程调用相同的配置文件；不同进程输出日志的目的地不要相同;----会引起切换不了文件的问题
    //4、在静态库中使用 MFC,环境中使用，不然还是会被MFC环境检测出内存泄露;
    //BLOCK_LOG_OBJECT(MYLOG_OBJECT_NAME);
    MYLOG_ERROR("error test %d", ::GetTickCount());
    MYLOG_WARN("warn  test %d", ::GetTickCount());
    MYLOG_INFO("warn  test %d", ::GetTickCount());
    MYLOG_DEBUG("debug test %d", ::GetTickCount());
    MYLOG_TRACE("warn  test %d", ::GetTickCount());

    //中文
    MYLOG_INFO("warn  test %s", "支持中文");

    //hlog_format(HLOG_LEVEL level, const char* module, const char* format, ...)
    //CString
    CString sMsg;
    sMsg = _T("支持中文，_T");
    MYLOG_INFO("warn  test %s", sMsg);//乱码，不正确的写法
    CStringA sMsgA(sMsg);
    MYLOG_INFO("warn  test %s", sMsgA);//正确

    //std::string
    std::string ss = "海康威视";
    MYLOG_INFO("warn  test %s", ss);//不正确,会输出null
    MYLOG_INFO("warn  test %s", ss.c_str());//正确
}

void CHlogDemoDlg::OnBnClickedBtnNew()
{
    //使用类型安全的日志输出
    MYLOG_EX_INFO("warn  test {0}", 8100);
    MYLOG_EX_INFO("warn  test {0},{1}", 8100,"中文");

    CString sMsg;
    sMsg = _T("支持中文");
    CStringA sMsgA(sMsg);
    std::string ss = "海康威视";
    MYLOG_EX_INFO("数字{0},{1}");//没问题，不崩溃
    MYLOG_EX_INFO("数字{0},{1}", 8100,sMsgA,ss.c_str());//参数多一个,没问题，不崩溃
    MYLOG_EX_INFO("数字{0},{1}", 8100,ss);//参数多一个,没问题，不崩溃
    MYLOG_EX_INFO("数字{0},{1},{2}", 8100,sMsgA);//参数少一个,没问题，不崩溃
    MYLOG_EX_INFO("数字{0},{1},{2}", 8100,sMsgA,ss.c_str());//合理的用法
}
