
// TimeConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimeConverter.h"
#include "TimeConverterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimeConverterDlg 对话框




CTimeConverterDlg::CTimeConverterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTimeConverterDlg::IDD, pParent)
, m_Date(_T(""))
, m_TimeStamp(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimeConverterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_DATE_EDIT, m_Date);
    DDX_Text(pDX, IDC_TIMESTAMP_EDIT, m_TimeStamp);
}

BEGIN_MESSAGE_MAP(CTimeConverterDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_EN_KILLFOCUS(IDC_DATE_EDIT, &CTimeConverterDlg::OnEnKillfocusDateEdit)
    ON_EN_KILLFOCUS(IDC_TIMESTAMP_EDIT, &CTimeConverterDlg::OnEnKillfocusTimestampEdit)
END_MESSAGE_MAP()


// CTimeConverterDlg 消息处理程序

BOOL CTimeConverterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    UpdateData(TRUE);
    m_Date = _T("1970-01-01 08:00:00");
    UpdateData(FALSE);
    AfxMessageBox(_T("使用说明:\n在任意一个输入框输入对应的内容后,点击按钮或按TAB进行转换"));
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimeConverterDlg::OnPaint()
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
HCURSOR CTimeConverterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CTimeConverterDlg::OnEnKillfocusDateEdit()
{
    // TODO: 在此添加控件通知处理程序代码2016-7-28 9:53:0
    UpdateData(TRUE);
    CString str = _T("%d-%d-%d %d:%d:%d");
    int Y = -1, M = -1, D = -1, h = -1, m = -1, s = -1;
    _stscanf(m_Date.GetString(), str, &Y, &M, &D, &h, &m, &s);
    if(Y==-1 || M==-1 || D==-1 ||h==-1 || m==-1 || s==-1)
    {
        AfxMessageBox(_T("输入时间格式有误"));
        m_Date = _T("1970-01-01 08:00:00");
        m_TimeStamp = 0;
    }
    else 
    {
        CTime timeToConvert = CTime(Y, M, D, h, m, s);
        CTime timeStart = CTime(0);
        CTimeSpan timespan = timeToConvert - timeStart;
        ULONGLONG ullTime = timespan.GetTotalSeconds();
        m_TimeStamp = ullTime;
    }
    UpdateData(FALSE);
}

void CTimeConverterDlg::OnEnKillfocusTimestampEdit()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CTime timeToConvert = CTime(m_TimeStamp);
    CString str = timeToConvert.Format("%Y-%m-%d %H:%M:%S");
    if (str.GetLength()>0)
    {
        m_Date = str;
    }
    else
    {
        AfxMessageBox(_T("输入时间戳值过大"));
        m_Date = _T("1970-01-01 08:00:00");
        m_TimeStamp = 0;
    }
    UpdateData(FALSE);
}
