
// bucketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "bucket.h"
#include "bucketDlg.h"


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


// CbucketDlg �Ի���




CbucketDlg::CbucketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbucketDlg::IDD, pParent)
	, m_strBucketName(_T("ivms_8000_bucket"))
	, m_strObjectKey(_T(""))
	, m_strCloudIp(_T("10.33.34.20"))
	, m_strCloudPort(6201)
	, m_strAccessKey(_T("62dz356jj5YQ11X5364pm681slTd92xm6nE931oqG99NX7T0b0q812t8t49j0Ag"))
	, m_strSecretKey(_T("9p828SN200FnH028wj7h5k922G5uO0i469u2eT0919C3Y1kO1vM7Mv65D6qaJ19"))
	, m_strBucketName2(_T("ivms_8000_bucket"))
	, m_nBucketSize(100)
	, m_HcsClient(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbucketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BUCKET, m_strBucketName);
	DDX_Text(pDX, IDC_EDIT_OBJECT_KEY, m_strObjectKey);
	DDX_Text(pDX, IDC_EDIT_CLOUD_IP, m_strCloudIp);
	DDX_Text(pDX, IDC_EDIT_CLOUD_PORT, m_strCloudPort);
	DDX_Text(pDX, IDC_EDIT_CLOUD_ACCESSKEY, m_strAccessKey);
	DDX_Text(pDX, IDC_EDIT_CLOUD_SECRETKEY, m_strSecretKey);
	DDX_Text(pDX, IDC_EDIT_BUCKET2, m_strBucketName2);
	DDX_Text(pDX, IDC_EDIT_BUCKET_SIZE, m_nBucketSize);
}

BEGIN_MESSAGE_MAP(CbucketDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CREATE_BUCKET, &CbucketDlg::OnBnClickedBtnCreateBucket)
	ON_BN_CLICKED(IDC_BTN_DELETE_BUCKET, &CbucketDlg::OnBnClickedBtnDeleteBucket)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT, &CbucketDlg::OnBnClickedBtnDeleteObject)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_INITIAL, &CbucketDlg::OnBnClickedBtnInitial)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALLOBJECT, &CbucketDlg::OnBnClickedBtnDeleteAllobject)
END_MESSAGE_MAP()


// CbucketDlg ��Ϣ�������

BOOL CbucketDlg::OnInitDialog()
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
	UpdateData(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CbucketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CbucketDlg::OnPaint()
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
HCURSOR CbucketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbucketDlg::OnBnClickedBtnCreateBucket()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	if (m_strBucketName.IsEmpty() || m_nBucketSize <= 1)
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	int result = CreateBucket(m_HcsClient, m_strBucketName.GetBuffer(0), "LOCAL-1",m_nBucketSize);

	if (0 == result)
	{
		MessageBox(_T("�����ɹ�"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("����ʧ��,������(%d)", result);
		MessageBox(strMsg);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CbucketDlg::OnBnClickedBtnDeleteBucket()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	if (m_strBucketName.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	int result = DeleteBucket(m_HcsClient, m_strBucketName.GetBuffer(0));

	if (0 == result)
	{
		MessageBox(_T("ɾ���ɹ�"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("ɾ��ʧ��,��ɾ����bucket������object��������(%d)", result);
		MessageBox(strMsg);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CbucketDlg::OnBnClickedBtnDeleteObject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	if (m_strBucketName2.IsEmpty() || m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	int result = DeleteObjectByName(m_HcsClient, m_strBucketName2.GetBuffer(0), m_strObjectKey.GetBuffer(0), NULL);

	if (0 == result)
	{
		MessageBox(_T("ɾ���ɹ�"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("ɾ��ʧ��,������(%d)", result);
		MessageBox(strMsg);
	}
}

void CbucketDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

   if (NULL != m_HcsClient)
   {
	  ClientFinalize(m_HcsClient);
   }
	CDialog::OnClose();
}

void CbucketDlg::OnBnClickedBtnInitial()
{
	UpdateData(TRUE);
	if (m_strCloudIp.IsEmpty() || m_strCloudPort != 6201 || m_strAccessKey.IsEmpty() || m_strSecretKey.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	CString strEndPoint;
	strEndPoint.Format("%s:%d",m_strCloudIp, m_strCloudPort);
	m_HcsClient = ClientInitialize(m_strAccessKey.GetBuffer(0), m_strSecretKey.GetBuffer(0), strEndPoint.GetBuffer(0));
	//���ó�ʼ����Ϣ����
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("����洢ϵͳ��ʼ��ʧ��"));
		return;
	}
	MessageBox(_T("����洢ϵͳ��ʼ���ɹ�"));
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CbucketDlg::OnBnClickedBtnDeleteAllobject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	HCSListObjectRequest list_params;
	HCSObjectList *list = NULL;

	list_params.prefix = NULL;
	list_params.max_keys = NULL;
	list_params.marker = NULL;
	list_params.delimiter = NULL;

	int ret = ListObjects(m_HcsClient, m_strBucketName2.GetBuffer(0), &list_params, &list);
	if (0 == ret && NULL != list)
	{
		for (int i = 0; i < list->summaries_count; i++)
		{
			if ((NULL == list->summaries) || (NULL == list->summaries[i]))
			{
				//DemoError("list object summaries(%p) or sumaries[%d] is null", list->summaries, i);
				continue;
			}
			if (NULL == list->summaries[i]->key || NULL == list->summaries[i]->version_id || NULL == list->summaries[i]->storage_class || NULL == list->summaries[i]->create_time || NULL == list->summaries[i]->user)
			{
				//DemoError("list object key or version or storageclass or create time is null");
				continue;
			}

			int result = DeleteObjectByName(m_HcsClient, m_strBucketName2.GetBuffer(0), list->summaries[i]->key, NULL);
			if (0 != result)
			{
				FreeHCSObjectList(m_HcsClient, &list);
				CString strMsg;
				strMsg.Format("Object(%s)ɾ��ʧ��,������(%d)", list->summaries[i]->key, result);
				MessageBox(strMsg);
				return;
			}
		}
		FreeHCSObjectList(m_HcsClient, &list);
		MessageBox(_T("ɾ���ɹ�"));
		return;
	}
	else
	{
		FreeHCSObjectList(m_HcsClient, &list);
		CString strMsg;
		strMsg.Format("ListObjectsʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return;
	}
}
