// Cloud_ObjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cloud_Object.h"
#include "Cloud_ObjectDlg.h"
#include "hcs_object_api.h"
#include <time.h>
#include <sys/stat.h>

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


// CCloud_ObjectDlg �Ի���




CCloud_ObjectDlg::CCloud_ObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCloud_ObjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCloud_ObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_ip);
	DDX_Control(pDX, IDC_EDIT2, m_edit_port);
	DDX_Control(pDX, IDC_EDIT3, m_edit_access);
	DDX_Control(pDX, IDC_EDIT4, m_edit_secret);
	DDX_Control(pDX, IDC_LIST1, m_list_show);
	DDX_Control(pDX, IDC_EDIT5, m_edit_bucketname);
	DDX_Control(pDX, IDC_EDIT6, m_edit_objectkey);
	DDX_Control(pDX, IDC_EDIT7, m_edit_filepath);
}

BEGIN_MESSAGE_MAP(CCloud_ObjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCloud_ObjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCloud_ObjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &CCloud_ObjectDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON3, &CCloud_ObjectDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCloud_ObjectDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CCloud_ObjectDlg::OnBnClickedButton6)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCloud_ObjectDlg::OnNMDblclkList1)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCloud_ObjectDlg::OnNMDblclkList1)
ON_BN_CLICKED(IDC_BUTTON5, &CCloud_ObjectDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON11, &CCloud_ObjectDlg::OnBnClickedButton11)
ON_BN_CLICKED(IDC_BUTTON9, &CCloud_ObjectDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON10, &CCloud_ObjectDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON8, &CCloud_ObjectDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON13, &CCloud_ObjectDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


// CCloud_ObjectDlg ��Ϣ�������

BOOL CCloud_ObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	client = NULL;
	CRect rect;
	m_edit_ip.SetWindowText(_T("10.33.34.20"));
	m_edit_port.SetWindowText(_T("6201"));
	m_edit_access.SetWindowText(_T("7kA404e117n3Z32Do91vNlro7u4gvxeSS0k7g0U729x8970r2gCwEIV9FXA8xwC"));
	m_edit_secret.SetWindowText(_T("dS6YJC8V0e486DU3728Xv3cUNe7A67dxC5G0a016Z73j8s62338c0263N2WEPK2"));

	m_list_show.GetClientRect(&rect);
	m_list_show.SetExtendedStyle(m_list_show.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_list_show.InsertColumn(0,_T("����"),LVCFMT_CENTER,rect.Width()/4,0);
	m_list_show.InsertColumn(1,_T("����"),LVCFMT_CENTER,rect.Width()/4,1);
	m_list_show.InsertColumn(2,_T("��С(�ֽ�)"),LVCFMT_CENTER,rect.Width()/4,2);
	m_list_show.InsertColumn(3,_T("����ʱ��"),LVCFMT_CENTER,rect.Width()/4,3);

	m_edit_bucketname.SetWindowText(_T("ivms_8000_bucket5"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCloud_ObjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCloud_ObjectDlg::OnPaint()
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
HCURSOR CCloud_ObjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ʼ��
void CCloud_ObjectDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstr_ip;
	CString cstr_port;
	CString cstr_access;
	CString cstr_secret;
	m_edit_ip.GetWindowText(cstr_ip);
	m_edit_port.GetWindowText(cstr_port);
	m_edit_access.GetWindowText(cstr_access);
	m_edit_secret.GetWindowText(cstr_secret);
	if (cstr_ip.IsEmpty() || cstr_port.IsEmpty() || cstr_access.IsEmpty() || cstr_secret.IsEmpty())
	{
		AfxMessageBox(_T("��������ȷ!"));
		return;
	}
	CString strEndPoint;
	strEndPoint.Format("%s:%s",cstr_ip, cstr_port);
	client =  ClientInitialize(cstr_access.GetBuffer(0), cstr_secret.GetBuffer(0), strEndPoint.GetBuffer(0));
	if (NULL == client)
	{
		AfxMessageBox(_T("��ʼ��ʧ��!"));
	}
	else
	{
		AfxMessageBox(_T("��ʼ���ɹ�!"));
	}
}

//���ѡ���ļ�
void CCloud_ObjectDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString m_strFileName;   //�ļ�������·�� 
	CFileDialog dlg(TRUE,NULL,"*.*");   
	if(dlg.DoModal()==IDOK)
	{ 
		m_strFileName = dlg.GetPathName();
	}
	m_edit_filepath.SetWindowText(m_strFileName);
}

//�˳�
void CCloud_ObjectDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (client != NULL)
	{
		ClientFinalize(client);
	}
	exit(0);
}

//��ȡ����IP,����Ψһ��ʶobjectkey
bool GetLocalIP(char* ip)
{
	//1.��ʼ��wsa
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return false;
	}
	//2.��ȡ������
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return false;
	}
	//3.��ȡ����ip
	HOSTENT* host=gethostbyname(hostname);
	if (host==NULL)
	{
		return false;
	}
	//4.ת��Ϊchar*����������
	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
	return true;
}
//��ʽ�ϴ�
void CCloud_ObjectDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString name;
	m_edit_bucketname.GetWindowText(name);
	CString path;
	m_edit_filepath.GetWindowText(path);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	if (name.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	if (path.IsEmpty())
	{
		MessageBox(_T("�ļ�·��Ϊ��"));
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
	struct tm *tm;//��Ҫ����tm�ṹ�壬���ʱ����Ϣ
	time_t now = time(NULL);//ע����÷�ʽ
	tm = localtime( static_cast<const time_t*>(&now) );//��Ҫ����time_t��ַ
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //����洢Key������Ψһ��ʶ�ļ�

	//��ѯ��objectKey�����Ƿ��ϴ����ļ�������ϴ���С
	ret = ListObjectSummary(client, name.GetBuffer(0), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		FreeHCSObjectSummary(client, &listCount, &pSummary);
		CString strMsg;
		strMsg.Format("�ϴ�ʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return ;
	}
	if (listCount == 1)
	{
		fileOffset = pSummary->size;
	}
	else if (listCount > 1)
	{
		FreeHCSObjectSummary(client, &listCount, &pSummary);
		MessageBox(_T("�ϴ�ʧ��,��object�Ѿ�����"));
		return ;
	}
	else
	{
	}
	FreeHCSObjectSummary(client, &listCount, &pSummary);

	ret = -1;
	ret = OpenStream(client,name.GetBuffer(0),objectKey,streamHandle);
	if(0 != ret)
	{
		CString strMsg;
		strMsg.Format("�ϴ�ʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return;
	}

	fileHandle = fopen(path.GetBuffer(0), "rb+");
	if (fileHandle == NULL)
	{
		CString strMsg;
		strMsg.Format("�ϴ�ʧ��,������(%d)", GetLastError());
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
			strMsg.Format("�ϴ�ʧ��,������(%d)", ret);
			MessageBox(strMsg);
			fclose(fileHandle);
			CloseStream(client,name.GetBuffer(0),objectKey,streamHandle);
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
			ret = UploadStreamData(client,name.GetBuffer(0),objectKey,streamHandle,fileData,readLength);
			if(0 != ret)
			{
				CString strMsg;
				strMsg.Format("�ϴ�ʧ��,������(%d)", ret);
				MessageBox(strMsg);
				fclose(fileHandle);
				CloseStream(client,name.GetBuffer(0),objectKey,streamHandle);
				return ;
			}

		}
		else
		{
			break;
		}
	}
	fclose(fileHandle);
	CloseStream(client,name.GetBuffer(0),objectKey,streamHandle);
	MessageBox(_T("д���ļ��ɹ�"));
}


BOOL CCloud_ObjectDlg::GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey, long &lStartTime, long &lEndTime)
{
	//time_t iStartTime = GetTimeStamp(strStartTime);
	//time_t iStopTime  = GetTimeStamp(strEndTime);
	HCSStreamTimeSegmentListEx *seg_list = NULL;
	int result = QueryObjectTimesegmentEx(pClient, strBucket.c_str(), strObjectKey.c_str(), 0, 999999999999, &seg_list, 1);
	if (0 != result)
	{
		CString strMsg;
		strMsg.Format("��ѯ�ļ�ʱ��ʧ��,������(%d)", result);
		MessageBox(strMsg);
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}

	if (((seg_list) && (seg_list->segment_count == 0)) || (seg_list == NULL))
	{
		MessageBox(_T("û���ļ�"));
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}
	if (1 != seg_list->segment_count)
	{
		MessageBox(_T("�ж���ļ�"));
		FreeStreamTimeSegmentListEx(pClient, &seg_list);
		return FALSE;
	}

	lStartTime = seg_list->segment_list[0].start_time;
	lEndTime = seg_list->segment_list[0].end_time;
	//¼��ʼʱ��

	FreeStreamTimeSegmentListEx(pClient, &seg_list);
	return TRUE;
}


int DownloadStreamdataCallBack(const char* buff, int bufflen, long long offset, int error, void* p_user)
{
	CCloud_ObjectDlg *pDlg = (CCloud_ObjectDlg *)p_user;
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


//��ʽ����
void CCloud_ObjectDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	if (m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("ObjectKey Ϊ��"));
		return;
	}
	//��objectlist �л�ȡѡ���object��Ϣ
	char tmp_file_name[64] = {0};
	sprintf(tmp_file_name, "./test_stream_download_%lld", time(NULL));

	m_file_handle = NULL;
	m_file_handle = fopen(tmp_file_name, "wb+");
	if (m_file_handle == NULL)
	{
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}

	m_bDownloadSuc = false;
	//ȡ�ļ�ͷ
	HCSGetObjectRequest stObjectRequest;
	memset(&stObjectRequest, 0, sizeof(HCSGetObjectRequest));
	HCSGetObjectRequest *request = &stObjectRequest;
	request->bucket_name = name.GetBuffer(0);
	request->key = (char*)m_strObjectKey.GetBuffer(0);
	request->version_id = NULL;
	request->start_pos = 0;
	request->end_pos = 40;
	char *read_buffer = NULL;
	unsigned int read_len = 0;
	int set_result = GetObjectToBuffer(client, request, (void**)&read_buffer, &read_len);

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
		strMsg.Format("����ʧ��,������(%d)", set_result);
		MessageBox(strMsg);
		FreeBuffer(client,(void**) &read_buffer);
		return;
	}
	FreeBuffer(client, (void**)&read_buffer);
	long lStartTime;
	long lEndTime;
	if (!GetRecordTime(client, name.GetBuffer(0), m_strObjectKey.GetBuffer(0),lStartTime,lEndTime))
	{
		return;
	}

	int ret = DownloadStreamData(client, name.GetBuffer(0), m_strObjectKey.GetBuffer(0), lStartTime, 
		lEndTime, OUTPUT_VIDEO_SOURCE,DownloadStreamdataCallBack, this);
	if (0 != ret)
	{
		CString strMsg;
		strMsg.Format("����ʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return;
	}
	while(!m_bDownloadSuc)
	{
		//�ȴ��������
	}
	fclose(m_file_handle);
	m_file_handle = NULL;
	MessageBox(_T("���سɹ�"));



}


//��ѯobject
void CCloud_ObjectDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
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

	int ret = ListObjects(client, name.GetBuffer(0), &list_params, &list);
	if (0 == ret && NULL != list)
	{
		m_list_show.DeleteAllItems();
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
			m_list_show.InsertItem(i,csName);
			m_list_show.SetItemText(i,1,csFileType);
			m_list_show.SetItemText(i,2,csCapacity);
			m_list_show.SetItemText(i,3,csCreateTime);
		}
		FreeHCSObjectList(client, &list);
		MessageBox(_T("��ѯ�ɹ�"));
		return;
	}
	else if (13016 == ret)
	{
		MessageBox(_T("bucket������"));
	}
	else
	{
		FreeHCSObjectList(client, &list);
		CString strMsg;
		strMsg.Format("��ѯʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return;
	}
}


//˫����objectkey��ʾ��edit��
void CCloud_ObjectDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;
	if(nItem>=0 && nItem<m_list_show.GetItemCount())//�ж�˫��λ���Ƿ��������ݵ��б�������
	{
		m_strObjectKey = m_list_show.GetItemText(nItem, 0);
	}
	m_edit_objectkey.SetWindowText(m_strObjectKey);
	UpdateData(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


//ɾ��object
void CCloud_ObjectDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("bucketΪ��"));
		return;
	}
	if (m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("object keyΪ�գ�˫���б���ѡ��Ҫɾ����object"));
		return;
	}
	int result = DeleteObjectByName(client, name.GetBuffer(0), m_strObjectKey.GetBuffer(0), NULL);

	if (0 == result)
	{
		MessageBox(_T("ɾ���ɹ�"));
	}
	else if (13016 == result)
	{
		MessageBox(_T("bucket������"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("ɾ��ʧ��,������(%d)", result);
		MessageBox(strMsg);
	}
}


//С�ļ���ͨ�ϴ�(�ڴ�)
void CCloud_ObjectDlg::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	if (name.IsEmpty())
	{
		MessageBox(_T("�ļ�·��Ϊ��"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};

	GetLocalIP(ip);
	struct tm *tm;//��Ҫ����tm�ṹ�壬���ʱ����Ϣ
	time_t now = time(NULL);//ע����÷�ʽ
	tm = localtime( static_cast<const time_t*>(&now) );//��Ҫ����time_t��ַ
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //����洢Key������Ψһ��ʶ�ļ�

	FILE * file_handle = NULL;
	int ret = -1;
	long long readLength = 0;
	CString path;
	m_edit_filepath.GetWindowText(path);
	file_handle = fopen((char*)path.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}
	//��ȡ�����ļ���С��Ϣ
	struct stat file_info = {0};
	if(stat(path.GetBuffer(0), &file_info) != 0)
	{
		MessageBox(_T("��ѯ�ļ���Сʧ��"));
		return ;
	}
	if(file_info.st_size >= 10 * 1024 * 1024)//�������ϴ�����10M���ļ�
	{
		CString strMsg;
		strMsg.Format("�ļ�̫��(%d)", file_info.st_size);
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
		ret = PutObjectFromBuffer(client, name.GetBuffer(0), objectKey, pFile, readLength, NULL, "STANDARD", NULL);
		if(0 != ret)
		{
			fclose(file_handle);
			CString strMsg;
			strMsg.Format("д���ļ�ʧ��,������(%d)", ret);
			MessageBox(strMsg);
			delete []pFile;
			return ;
		}
	}
	else
	{
		MessageBox(_T("д���ļ�ʧ��"));
		fclose(file_handle);
		delete []pFile;
		return;
	}

	fclose(file_handle);
	delete []pFile;
	MessageBox(_T("д���ļ��ɹ�"));
}

//��ͨ�ϴ�(�ļ�)
void CCloud_ObjectDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	CString path;
	m_edit_filepath.GetWindowText(path);
	if (path.IsEmpty())
	{
		MessageBox(_T("�ļ�·��Ϊ��"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};
	FILE * fileHandle = NULL;

	//����һ��Ψһ��ʶ��objectkey
	GetLocalIP(ip);
	struct tm *tm;//��Ҫ����tm�ṹ�壬���ʱ����Ϣ
	time_t now = time(NULL);//ע����÷�ʽ
	tm = localtime( static_cast<const time_t*>(&now) );//��Ҫ����time_t��ַ
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //����洢Key������Ψһ��ʶ�ļ�

	FILE * file_handle = NULL;
	file_handle = fopen((char*)path.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}
	//����
	int result = PutObjectFromFile(client, name.GetBuffer(0), objectKey, file_handle, NULL, "STANDARD", NULL);

	if (0 == result)
	{
		MessageBox(_T("�ϴ��ɹ�"));
	}
	else
	{
		CString strMsg;
		strMsg.Format("�ϴ�ʧ��,������(%d)", result);
		MessageBox(strMsg);
	}
	fclose(file_handle);
}

//��ͨ�ϴ�(�ڴ�)
void CCloud_ObjectDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("�����Ƿ�"));
		return;
	}
	CString path;
	m_edit_filepath.GetWindowText(path);
	if (path.IsEmpty())
	{
		MessageBox(_T("�ļ�·��Ϊ��"));
		return;
	}
	char ip[100] = {0};
	char szlocaltime[100]={0};
	char objectKey[60]={0};

	GetLocalIP(ip);
	struct tm *tm;//��Ҫ����tm�ṹ�壬���ʱ����Ϣ
	time_t now = time(NULL);//ע����÷�ʽ
	tm = localtime( static_cast<const time_t*>(&now) );//��Ҫ����time_t��ַ
	sprintf(szlocaltime, "%02d:%02d:%02d_%02d/%02d/%4d", tm->tm_hour, tm->tm_min,tm->tm_sec, tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
	sprintf(objectKey, "%s_%s_%d", ip, szlocaltime, rand()); //����洢Key������Ψһ��ʶ�ļ�

	FILE * file_handle = NULL;
	int ret = -1;
	long long readLength = 0;
	file_handle = fopen((char*)path.GetBuffer(0), "rb");
	if (file_handle == NULL)
	{
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}

	int listCount = 0;
	HCSObjectSummary * pSummary = NULL;
	long long fileOffset = 0;
	//��ѯ��objectKey�����Ƿ��ϴ����ļ�������ϴ���С
	ret = ListObjectSummary(client, name.GetBuffer(0), objectKey, &listCount, &pSummary);
	if (0 != ret)
	{
		FreeHCSObjectSummary(client, &listCount, &pSummary);
		CString strMsg;
		strMsg.Format("��ѯobjectʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return ;
	}
	if (listCount == 1)
	{
		fileOffset = pSummary->size;
		MessageBox(_T("���ӵ�object"));
	}
	else if (listCount > 1)
	{
		FreeHCSObjectSummary(client, &listCount, &pSummary);
		MessageBox(_T("�ϴ�ʧ��,��object�Ѿ�����"));
		return ;
	}
	else
	{
	}
	FreeHCSObjectSummary(client, &listCount, &pSummary);

	if (fileOffset > 0)
	{
		ret = -1;
		ret = fseek(file_handle, fileOffset, 0);
		if (0 != ret)
		{
			CString strMsg;
			strMsg.Format("�ļ�����ʧ��,������(%d)", ret);
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
			ret = AppendObject(client, name.GetBuffer(0), objectKey, fileData, readLength, position, &next_position);
			if(0 != ret)
			{
				fclose(file_handle);
				CString strMsg;
				strMsg.Format("д���ļ�ʧ��,������(%d)", ret);
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
	MessageBox(_T("д���ļ��ɹ�"));
}

//��ͨ����
void CCloud_ObjectDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox(_T("bucketΪ��"));
		return;
	}
	if (m_strObjectKey.IsEmpty())
	{
		MessageBox(_T("object keyΪ�գ���˫���б���ѡ��Ҫ���ص��ļ�"));
		return;
	}
	//��objectlist �л�ȡѡ���object��Ϣ
	char tmp_file_name[64] = {0};
	sprintf(tmp_file_name, "./test_download_%lld", time(NULL));

	FILE * file_handle = NULL;
	file_handle = fopen(tmp_file_name, "wb+");
	if (file_handle == NULL)
	{
		MessageBox(_T("���ļ�ʧ��"));
		return;
	}

	HCSGetObjectRequest *request = new HCSGetObjectRequest;
	request->bucket_name = name.GetBuffer(0);
	request->key = m_strObjectKey.GetBuffer(0);
	request->version_id = NULL;
	request->start_pos = 0;
	request->end_pos = 0xffffffff;

	int set_result = GetObjectToFile(client, request, (void*)file_handle);
	if (0 == set_result)
	{
		MessageBox(_T("���سɹ�"));
	}
	else
	{
		MessageBox(_T("����ʧ��"));
	}

	fclose(file_handle);
	delete request;
	request = NULL;
}

//ȫ��ɾ��
void CCloud_ObjectDlg::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (NULL == client)
	{
		MessageBox(_T("����洢ϵͳδ��ʼ��"));
		return;
	}
	CString name;
	m_edit_bucketname.GetWindowText(name);
	if (name.IsEmpty())
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

	int ret = ListObjects(client, name.GetBuffer(0), &list_params, &list);
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

			int result = DeleteObjectByName(client, name.GetBuffer(0), list->summaries[i]->key, NULL);
			if (0 != result)
			{
				FreeHCSObjectList(client, &list);
				CString strMsg;
				strMsg.Format("Object(%s)ɾ��ʧ��,������(%d)", list->summaries[i]->key, result);
				MessageBox(strMsg);
				return;
			}
		}
		FreeHCSObjectList(client, &list);
		MessageBox(_T("ɾ���ɹ�"));
		return;
	}
	else if (13016 == ret)
	{
		MessageBox(_T("bucket������"));
	}
	else
	{
		FreeHCSObjectList(client, &list);
		CString strMsg;
		strMsg.Format("ListObjectsʧ��,������(%d)", ret);
		MessageBox(strMsg);
		return;
	}
}
