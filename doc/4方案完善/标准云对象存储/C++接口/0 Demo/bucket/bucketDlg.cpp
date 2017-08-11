
// bucketDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bucket.h"
#include "bucketDlg.h"
#include <time.h>
#include <sys/stat.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CbucketDlg 对话框




CbucketDlg::CbucketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbucketDlg::IDD, pParent)
	, m_strBucketName(_T("ivms_8000_bucket5"))
	, m_strObjectKey(_T(""))
	, m_strCloudIp(_T("10.33.34.20"))
	, m_strCloudPort(6201)
	, m_strAccessKey(_T("7kA404e117n3Z32Do91vNlro7u4gvxeSS0k7g0U729x8970r2gCwEIV9FXA8xwC"))
	, m_strSecretKey(_T("dS6YJC8V0e486DU3728Xv3cUNe7A67dxC5G0a016Z73j8s62338c0263N2WEPK2"))
	, m_strBucketName2(_T("ivms_8000_bucket5"))
	, m_nBucketSize(100)
	, m_HcsClient(NULL)
	, m_nBucketLife(-1)
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
	DDX_Text(pDX, IDC_EDIT_BUCKET_SIZE2, m_nBucketLife);
	DDX_Control(pDX, IDC_LIST1, m_listBucketQuery);
	DDX_Control(pDX, IDC_LIST2, m_listObjectQuery);
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
	ON_BN_CLICKED(IDC_BTN_CREATE_BUCKET2, &CbucketDlg::OnBnClickedBtnSetBucketCapacity)
	ON_BN_CLICKED(IDC_BTN_DELETE_BUCKET2, &CbucketDlg::OnBnClickedBtnSetBucketLife)
	ON_BN_CLICKED(IDC_BTN_DELETE_BUCKET3, &CbucketDlg::OnBnClickedBtnQueryAllBucket)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT4, &CbucketDlg::OnBnClickedBtnQueryAllObject)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT2, &CbucketDlg::OnBnClickedBtnNormalUploadObject)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT3, &CbucketDlg::OnBnClickedBtnNormalDownloadObject)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALLOBJECT2, &CbucketDlg::OnBnClickedBtnStreamUploadobject)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALLOBJECT3, &CbucketDlg::OnBnClickedBtnStreamDownloadobject)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT5, &CbucketDlg::OnBnClickedBtnSelectFile)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT6, &CbucketDlg::OnBnClickedBtnNormalUploadObjectFromBuf)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CbucketDlg::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BTN_DELETE_OBJECT7, &CbucketDlg::OnBnClickedBtnSmallFileNormalUploadObjectFromBuf)
END_MESSAGE_MAP()

bool GetLocalIP(char* ip)
{
	//1.初始化wsa
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return false;
	}
	//2.获取主机名
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return false;
	}
	//3.获取主机ip
	HOSTENT* host=gethostbyname(hostname);
	if (host==NULL)
	{
		return false;
	}
	//4.转化为char*并拷贝返回
	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
	return true;
}

int DownloadStreamdataCallBack(const char* buff, int bufflen, long long offset, int error, void* p_user)
{
	CbucketDlg *pDlg = (CbucketDlg *)p_user;
	FILE* fp = NULL;
	if (NULL != buff && bufflen > 0 && NULL != pDlg->m_file_handle)
	{
		fwrite(buff, 1, bufflen, pDlg->m_file_handle);
	}

	if (NULL == buff || 0 >= bufflen)
	{
		pDlg->m_bDownloadSuc = true;
		return -1;
	}
	return 0;

}


// CbucketDlg 消息处理程序

BOOL CbucketDlg::OnInitDialog()
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
	UpdateData(TRUE);

	m_listBucketQuery.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_AUTOSIZECOLUMNS|LVS_EDITLABELS);
	m_listBucketQuery.InsertColumn(0,_T("名称"),LVCFMT_CENTER,150);
	m_listBucketQuery.InsertColumn(1,_T("大小(G)"),LVCFMT_CENTER,60);
	m_listBucketQuery.InsertColumn(2,_T("生命周期(天)"),LVCFMT_CENTER,100);

	m_listObjectQuery.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_AUTOSIZECOLUMNS|LVS_EDITLABELS);
	m_listObjectQuery.InsertColumn(0,_T("名称"),LVCFMT_CENTER,250);
	m_listObjectQuery.InsertColumn(1,_T("类型"),LVCFMT_CENTER,100);
	m_listObjectQuery.InsertColumn(2,_T("大小(字节)"),LVCFMT_CENTER,100);
	m_listObjectQuery.InsertColumn(3,_T("创建时间"),LVCFMT_CENTER,250);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbucketDlg::OnPaint()
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
HCURSOR CbucketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbucketDlg::OnBnClickedBtnCreateBucket()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName.IsEmpty() || m_nBucketSize <= 1)
	{
		MessageBox(_T("参数非法"));
		return;
	}
	int result = CreateBucket(m_HcsClient, m_strBucketName.GetBuffer(0), "LOCAL-1",m_nBucketSize);

	if (0 == result)
	{
		MessageBox(_T("创建成功"));
	}
	else if (13003 == result)
	{
		MessageBox(_T("bucket已经存在"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("创建失败,错误码(%d)", result);
		MessageBox(strMsg);
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CbucketDlg::OnBnClickedBtnDeleteBucket()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	int result = DeleteBucket(m_HcsClient, m_strBucketName.GetBuffer(0));

	if (0 == result)
	{
		MessageBox(_T("删除成功"));
	}
	else if (13016 == result)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("删除失败,请删除该bucket下所有object，错误码(%d)", result);
		MessageBox(strMsg);
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CbucketDlg::OnBnClickedBtnDeleteObject()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("bucket为空"));
		return;
	}
	if (m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("object key为空，双击列表项选择要删除的object"));
		return;
	}
	int result = DeleteObjectByName(m_HcsClient, m_strBucketName2.GetBuffer(0), m_strObjectKey.GetBuffer(0), NULL);

	if (0 == result)
	{
		MessageBox(_T("删除成功"));
	}
	else if (13016 == result)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("删除失败,错误码(%d)", result);
		MessageBox(strMsg);
	}
}

void CbucketDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
		MessageBox(_T("参数非法"));
		return;
	}
	CString strEndPoint;
	strEndPoint.Format("%s:%d",m_strCloudIp, m_strCloudPort);
	m_HcsClient = ClientInitialize(m_strAccessKey.GetBuffer(0), m_strSecretKey.GetBuffer(0), strEndPoint.GetBuffer(0));
	//调用初始化信息函数
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统初始化失败"));
		return;
	}
	MessageBox(_T("对象存储系统初始化成功"));
	// TODO: 在此添加控件通知处理程序代码
}

void CbucketDlg::OnBnClickedBtnDeleteAllobject()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
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
				strMsg.Format("Object(%s)删除失败,错误码(%d)", list->summaries[i]->key, result);
				MessageBox(strMsg);
				return;
			}
		}
		FreeHCSObjectList(m_HcsClient, &list);
		MessageBox(_T("删除成功"));
		return;
	}
	else if (13016 == ret)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		FreeHCSObjectList(m_HcsClient, &list);
		CString strMsg;
		strMsg.Format("ListObjects失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return;
	}
}

void CbucketDlg::OnBnClickedBtnSetBucketCapacity()
{

	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName.IsEmpty() || m_nBucketSize <= 1)
	{
		MessageBox(_T("参数非法"));
		return;
	}
	int result = SetBucketSize(m_HcsClient, m_strBucketName.GetBuffer(0), m_nBucketSize);

	if (0 == result)
	{
		MessageBox(_T("设置成功"));
	}
	else if (13016 == result)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("设置失败,错误码(%d)", result);
		MessageBox(strMsg);
	}
}

void CbucketDlg::OnBnClickedBtnSetBucketLife()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName.IsEmpty() || m_nBucketLife <= -2)
	{
		MessageBox(_T("参数非法"));
		return;
	}
	int result = SetBucketLifecycle(m_HcsClient, m_strBucketName.GetBuffer(0), m_nBucketLife);

	if (0 == result)
	{
		MessageBox(_T("设置成功"));
	}
	else if (13016 == result)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("设置失败,错误码(%d)", result);
		MessageBox(strMsg);
	}
}

void CbucketDlg::OnBnClickedBtnQueryAllBucket()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	int buckets_num = 0;
	HCSBucketInfo *buckets_list = NULL;
	int result = ListBuckets(m_HcsClient, &buckets_num, &buckets_list);

	if (0 == result)
	{

		if (buckets_num == 0)
		{
			MessageBox(_T("没有Bucket"));
			FreeHCSBucketInfo(m_HcsClient, &buckets_num, &buckets_list);
			return;
		}
        int m_ItemIndexInfo = 0;
		m_listBucketQuery.DeleteAllItems();
		CString csName;
        CString csCapacity;
		CString csLife;
		for (int i = 0; i < buckets_num; i++)
		{
			csName.Format(_T("%s"),buckets_list[i].bucket_name);
			csCapacity.Format(_T("%d"),int(buckets_list[i].bucket_size));
			csLife.Format(_T("%d"),int(buckets_list[i].life_cycle));
			m_listBucketQuery.InsertItem(i,csName);
			m_listBucketQuery.SetItemText(i,1,csCapacity);
			m_listBucketQuery.SetItemText(i,2,csLife);
		}
		FreeHCSBucketInfo(m_HcsClient, &buckets_num, &buckets_list);

		MessageBox(_T("查询成功"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("查询失败,错误码(%d)", result);
		MessageBox(strMsg);
	}

}

void CbucketDlg::OnBnClickedBtnQueryAllObject()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
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
		m_listObjectQuery.DeleteAllItems();
		CString csName;
		CString csFileType;
		CString csCapacity;
		CString csCreateTime;
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

			csName.Format(_T("%s"),list->summaries[i]->key);
			csFileType.Format(_T("%s"),list->summaries[i]->storage_class);
			csCapacity.Format(_T("%d"),list->summaries[i]->size);
			csCreateTime.Format(_T("%s"),list->summaries[i]->create_time);
			m_listObjectQuery.InsertItem(i,csName);
			m_listObjectQuery.SetItemText(i,1,csFileType);
			m_listObjectQuery.SetItemText(i,2,csCapacity);
			m_listObjectQuery.SetItemText(i,3,csCreateTime);
		}
		FreeHCSObjectList(m_HcsClient, &list);
		MessageBox(_T("查询成功"));
		return;
	}
	else if (13016 == ret)
	{
		MessageBox(_T("bucket不存在"));
	}
	else
	{
		FreeHCSObjectList(m_HcsClient, &list);
		CString strMsg;
		strMsg.Format("查询失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return;
	}
}

void CbucketDlg::OnBnClickedBtnNormalUploadObject()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	if (m_csPathName.IsEmpty())
	{
		MessageBox(_T("文件路径为空"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long fileOffset = 0;
	FILE * fileHandle = NULL;
	long long readLength = 0;
	GetLocalIP(ip);
	struct tm *tm;//需要定义tm结构体，存放时间信息
	time_t now = time(NULL);//注意调用方式
	tm = localtime( static_cast<const time_t*>(&now) );//需要传递time_t地址
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //对象存储Key，用来唯一标识文件
	FILE * file_handle = NULL;
	
	file_handle = fopen((char*)m_csPathName.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("打开文件失败"));
		return;
	}
	int result = PutObjectFromFile(m_HcsClient, m_strBucketName2.GetBuffer(0), objectKey, file_handle, NULL, "STANDARD", NULL);

	if (0 == result)
	{
		MessageBox(_T("上传成功"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("上传失败,错误码(%d)", result);
		MessageBox(strMsg);
	}
	fclose(file_handle);

}

void CbucketDlg::OnBnClickedBtnNormalDownloadObject()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty() || m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	//从objectlist 中获取选择的object信息
	char tmp_file_name[64] = {0};
	sprintf(tmp_file_name, "./test_download_%lld", time(NULL));

	FILE * file_handle = NULL;
	file_handle = fopen(tmp_file_name, "wb+");
	if (file_handle == NULL)
	{
		MessageBox(_T("打开文件失败"));
		return;
	}

	HCSGetObjectRequest *request = new HCSGetObjectRequest;
	request->bucket_name = m_strBucketName2.GetBuffer(0);
	request->key = m_strObjectKey.GetBuffer(0);
    request->version_id = NULL;
	request->start_pos = 0;
	request->end_pos = 0xffffffff;

	int set_result = GetObjectToFile(m_HcsClient, request, (void*)file_handle);
	if (0 == set_result)
	{
		MessageBox(_T("下载成功"));
	}
	else
	{
		MessageBox(_T("下载失败"));
	}

	fclose(file_handle);
	delete request;
	request = NULL;
}

void CbucketDlg::OnBnClickedBtnStreamUploadobject()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	if (m_csPathName.IsEmpty())
	{
		MessageBox(_T("文件路径为空"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};

	int  ret = -1;
	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	FILE * fileHandle = NULL;
	long fileOffset = 0;
	long long readLength = 0;
	char streamHandle[64]={0};
	GetLocalIP(ip);
	struct tm *tm;//需要定义tm结构体，存放时间信息
	time_t now = time(NULL);//注意调用方式
	tm = localtime( static_cast<const time_t*>(&now) );//需要传递time_t地址
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //对象存储Key，用来唯一标识文件

	//查询该objectKey曾经是否上传过文件并获得上传大小
	ret = ListObjectSummary(m_HcsClient, m_strBucketName2.GetBuffer(0), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);
		CString strMsg;
		strMsg.Format("上传失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return ;
	}
	if (listCount == 1)
	{
		fileOffset = pSummary->size;
	}
	else if (listCount > 1)
	{
		FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);
		MessageBox(_T("上传失败,该object已经存在"));
		return ;
	}
	else
	{
	}
	FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);

	ret = -1;
	ret = OpenStream(m_HcsClient,m_strBucketName2.GetBuffer(0),objectKey,streamHandle);
	if(0 != ret)
	{
		CString strMsg;
		strMsg.Format("上传失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return;
	}

	fileHandle = fopen(m_csPathName.GetBuffer(0), "rb+");
	if (fileHandle == NULL)
	{
		CString strMsg;
		strMsg.Format("上传失败,错误码(%d)", GetLastError());
		MessageBox(strMsg);
		return ;
	}

	if (fileOffset > 0)
	{
		ret = -1;
		ret = fseek(fileHandle, fileOffset, 0);
		if (0 != ret)
		{
			CString strMsg;
			strMsg.Format("上传失败,错误码(%d)", ret);
			MessageBox(strMsg);
			fclose(fileHandle);
			CloseStream(m_HcsClient,m_strBucketName2.GetBuffer(0),objectKey,streamHandle);
			return ;
		}
	}

	while(true)
	{
		readLength = 0;
		memset(fileData,0,sizeof(fileData));
		readLength = fread(fileData,1,WRITE_LENGTH,fileHandle);
		if(readLength != 0)
		{
			ret = -1;
			ret = UploadStreamData(m_HcsClient,m_strBucketName2.GetBuffer(0),objectKey,streamHandle,fileData,readLength);
			if(0 != ret)
			{
				CString strMsg;
				strMsg.Format("上传失败,错误码(%d)", ret);
				MessageBox(strMsg);
				fclose(fileHandle);
				CloseStream(m_HcsClient,m_strBucketName2.GetBuffer(0),objectKey,streamHandle);
				return ;
			}

		}
		else
		{
			break;
		}
	}
	fclose(fileHandle);
	CloseStream(m_HcsClient,m_strBucketName2.GetBuffer(0),objectKey,streamHandle);
	MessageBox(_T("写入文件成功"));
}

void CbucketDlg::OnBnClickedBtnStreamDownloadobject()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	if (m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("ObjectKey 为空"));
		return;
	}
	//从objectlist 中获取选择的object信息
	char tmp_file_name[64] = {0};
	sprintf(tmp_file_name, "./test_stream_download_%lld", time(NULL));

	m_file_handle = NULL;
	m_file_handle = fopen(tmp_file_name, "wb+");
	if (m_file_handle == NULL)
	{
		MessageBox(_T("打开文件失败"));
		return;
	}

	m_bDownloadSuc = false;
	//取文件头
	HCSGetObjectRequest stObjectRequest;
	memset(&stObjectRequest, 0, sizeof(HCSGetObjectRequest));
	HCSGetObjectRequest *request = &stObjectRequest;
	request->bucket_name = m_strBucketName2.GetBuffer(0);
	request->key = (char*)m_strObjectKey.GetBuffer(0);
	request->version_id = NULL;
	request->start_pos = 0;
	request->end_pos = 40;
	char *read_buffer = NULL;
	unsigned int read_len = 0;
	int set_result = GetObjectToBuffer(m_HcsClient, request, (void**)&read_buffer, &read_len);

	if (0 == set_result)
	{
		if (NULL != read_buffer && read_len > 0 && NULL != m_file_handle)
		{
			fwrite(read_buffer, 1, read_len, m_file_handle);
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format("下载失败,错误码(%d)", set_result);
		MessageBox(strMsg);
		FreeBuffer(m_HcsClient,(void**) &read_buffer);
		return;
	}
	FreeBuffer(m_HcsClient, (void**)&read_buffer);
	long lStartTime;
	long lEndTime;
	if (!GetRecordTime(m_HcsClient, m_strBucketName2.GetBuffer(0), m_strObjectKey.GetBuffer(0),lStartTime,lEndTime))
	{
		return;
	}
	
	int ret = DownloadStreamData(m_HcsClient, m_strBucketName2.GetBuffer(0), m_strObjectKey.GetBuffer(0), lStartTime, 
		lEndTime, OUTPUT_VIDEO_SOURCE,DownloadStreamdataCallBack, this);
	if (0 != ret)
	{
		CString strMsg;
		strMsg.Format("下载失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return;
	}
	while(!m_bDownloadSuc)
	{
		//等待下载完成
	}
	fclose(m_file_handle);
	m_file_handle = NULL;
	MessageBox(_T("下载成功"));


}

BOOL CbucketDlg::GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey, long &lStartTime, long &lEndTime)
{
	//time_t iStartTime = GetTimeStamp(strStartTime);
	//time_t iStopTime  = GetTimeStamp(strEndTime);
	HCSStreamTimeSegmentListEx *seg_list = NULL;
	int result = QueryObjectTimesegmentEx(pClient, strBucket.c_str(), strObjectKey.c_str(), 0, 999999999999, &seg_list, 1);
	if (0 != result)
	{
		CString strMsg;
		strMsg.Format("查询文件时间失败,错误码(%d)", result);
		MessageBox(strMsg);
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}

	if (((seg_list) && (seg_list->segment_count == 0)) || (seg_list == NULL))
	{
		MessageBox(_T("没有文件"));
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}
	if (1 != seg_list->segment_count)
	{
		MessageBox(_T("有多个文件"));
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}

	lStartTime = seg_list->segment_list[0].start_time;
	lEndTime = seg_list->segment_list[0].end_time;
	//录像开始时间

	FreeStreamTimeSegmentListEx(pClient, &seg_list);
	return TRUE;
}

void CbucketDlg::OnBnClickedBtnSelectFile()
{
	GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(_T(""));

	//创建一个可以选择多个文件的CFileDialog
	CFileDialog fileDlg(TRUE,NULL,_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT);

	//显示文件对话框，获取文件名集合
	if (fileDlg.DoModal() == IDOK)
	{
		//获取第一个文件的位置
		POSITION pos_file;
		pos_file = fileDlg.GetStartPosition();
		if(pos_file != NULL)
		{
			m_csPathName = fileDlg.GetNextPathName(pos_file);
		}
		else
		{
			CString strMsg;
			strMsg.Format("读取失败");
			MessageBox(strMsg);
			return;
		}
	}
	GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(m_csPathName);

	UpdateData(FALSE);
}

void CbucketDlg::OnBnClickedBtnNormalUploadObjectFromBuf()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	if (m_csPathName.IsEmpty())
	{
		MessageBox(_T("文件路径为空"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};
	
	GetLocalIP(ip);
	struct tm *tm;//需要定义tm结构体，存放时间信息
	time_t now = time(NULL);//注意调用方式
	tm = localtime( static_cast<const time_t*>(&now) );//需要传递time_t地址
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //对象存储Key，用来唯一标识文件
	
	FILE * file_handle = NULL;
	int ret = -1;
	long long readLength = 0;
	file_handle = fopen((char*)m_csPathName.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("打开文件失败"));
		return;
	}

	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long long fileOffset = 0;
	//查询该objectKey曾经是否上传过文件并获得上传大小
	ret = ListObjectSummary(m_HcsClient, m_strBucketName2.GetBuffer(0), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);
		CString strMsg;
		strMsg.Format("查询object失败,错误码(%d)", ret);
		MessageBox(strMsg);
		return ;
	}
	if (listCount == 1)
	{
		fileOffset = pSummary->size;
		MessageBox(_T("附加到object"));
	}
	else if (listCount > 1)
	{
		FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);
		MessageBox(_T("上传失败,该object已经存在"));
		return ;
	}
	else
	{
	}
	FreeHCSObjectSummary(m_HcsClient, &listCount, &pSummary);

	if (fileOffset > 0)
	{
		ret = -1;
		ret = fseek(file_handle, fileOffset, 0);
		if (0 != ret)
		{
			CString strMsg;
			strMsg.Format("文件操作失败,错误码(%d)", ret);
			MessageBox(strMsg);
			fclose(file_handle);
			return ;
		}
	}
	long long position = fileOffset;
    long long next_position = 0;
	while(true)
	{
		readLength = 0;
		memset(fileData,0,sizeof(fileData));
		readLength = fread(fileData,1,WRITE_LENGTH,file_handle);
		if(readLength != 0)
		{
			ret = -1;
			ret = AppendObject(m_HcsClient, m_strBucketName2.GetBuffer(0), objectKey, fileData, readLength, position, &next_position);
			if(0 != ret)
			{
				fclose(file_handle);
				CString strMsg;
				strMsg.Format("写入文件失败,错误码(%d)", ret);
				MessageBox(strMsg);
				return ;
			}
			position += readLength;
		}
		else
		{
			break;
		}
	}
	fclose(file_handle);
	MessageBox(_T("写入文件成功"));
}


void CbucketDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;
	if(nItem>=0 && nItem<m_listObjectQuery.GetItemCount())//判断双击位置是否在有数据的列表项上面
	{
		m_strObjectKey = m_listObjectQuery.GetItemText(nItem, 0);
	}
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CbucketDlg::OnBnClickedBtnSmallFileNormalUploadObjectFromBuf()
{
	UpdateData(TRUE);
	if (NULL == m_HcsClient)
	{
		MessageBox(_T("对象存储系统未初始化"));
		return;
	}
	if (m_strBucketName2.IsEmpty())
	{
		MessageBox(_T("参数非法"));
		return;
	}
	if (m_csPathName.IsEmpty())
	{
		MessageBox(_T("文件路径为空"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};
	
	GetLocalIP(ip);
	struct tm *tm;//需要定义tm结构体，存放时间信息
	time_t now = time(NULL);//注意调用方式
	tm = localtime( static_cast<const time_t*>(&now) );//需要传递time_t地址
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //对象存储Key，用来唯一标识文件
	
	FILE * file_handle = NULL;
	int ret = -1;
	long long readLength = 0;
	file_handle = fopen((char*)m_csPathName.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("打开文件失败"));
		return;
	}
	//获取本地文件大小信息
	struct stat file_info = {0};
	if(stat(m_csPathName.GetBuffer(0), &file_info) != 0)
	{
		MessageBox(_T("查询文件大小失败"));
		return ;
	}
	if(file_info.st_size >= 10 * 1024 * 1024)//不允许上传超过10M的文件
	{
		CString strMsg;
		strMsg.Format("文件太大(%d)", file_info.st_size);
		MessageBox(strMsg);
		return;
	}
	readLength = 0;
	char * pFile = new char[10 * 1024 * 1024];
	memset(pFile,0,10 * 1024 * 1024);
	readLength = fread(pFile,1,file_info.st_size,file_handle);
	if(readLength != 0)
	{
		ret = -1;
		ret = PutObjectFromBuffer(m_HcsClient, m_strBucketName2.GetBuffer(0), objectKey, pFile, readLength, NULL, "STANDARD", NULL);
		if(0 != ret)
		{
			fclose(file_handle);
			CString strMsg;
			strMsg.Format("写入文件失败,错误码(%d)", ret);
			MessageBox(strMsg);
			delete []pFile;
			return ;
		}
	}
	else
	{
		MessageBox(_T("写入文件失败"));
		fclose(file_handle);
		delete []pFile;
		return;
	}

	fclose(file_handle);
	delete []pFile;
	MessageBox(_T("写入文件成功"));
}
