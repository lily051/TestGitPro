
// TimeConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimeConverter.h"
#include "TimeConverterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimeConverterDlg �Ի���




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


// CTimeConverterDlg ��Ϣ�������

BOOL CTimeConverterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    UpdateData(TRUE);
    m_Date = _T("1970-01-01 08:00:00");
    UpdateData(FALSE);
    AfxMessageBox(_T("ʹ��˵��:\n������һ������������Ӧ�����ݺ�,�����ť��TAB����ת��"));
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTimeConverterDlg::OnPaint()
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
HCURSOR CTimeConverterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CTimeConverterDlg::OnEnKillfocusDateEdit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������2016-7-28 9:53:0
    UpdateData(TRUE);
    CString str = _T("%d-%d-%d %d:%d:%d");
    int Y = -1, M = -1, D = -1, h = -1, m = -1, s = -1;
    _stscanf(m_Date.GetString(), str, &Y, &M, &D, &h, &m, &s);
    if(Y==-1 || M==-1 || D==-1 ||h==-1 || m==-1 || s==-1)
    {
        AfxMessageBox(_T("����ʱ���ʽ����"));
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CTime timeToConvert = CTime(m_TimeStamp);
    CString str = timeToConvert.Format("%Y-%m-%d %H:%M:%S");
    if (str.GetLength()>0)
    {
        m_Date = str;
    }
    else
    {
        AfxMessageBox(_T("����ʱ���ֵ����"));
        m_Date = _T("1970-01-01 08:00:00");
        m_TimeStamp = 0;
    }
    UpdateData(FALSE);
}
