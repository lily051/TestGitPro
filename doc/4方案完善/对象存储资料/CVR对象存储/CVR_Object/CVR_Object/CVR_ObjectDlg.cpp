// CVR_ObjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CVR_Object.h"
#include "CVR_ObjectDlg.h"
#include "HCNetSDK.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;
#define  WM_PHY_DISK_INFO_FINISH 1001
#define WM_PROC_PHY_DISK_INFO_SHOW		WM_USER + 1

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


// CCVR_ObjectDlg 对话框




CCVR_ObjectDlg::CCVR_ObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCVR_ObjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	lLoginID = 0;
}

void CCVR_ObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctrlDevIp);
	DDX_Control(pDX, IDC_EDIT1, m_editnum);
	DDX_Control(pDX, IDC_EDIT2, m_editcode);
	DDX_Control(pDX, IDC_EDIT3, m_editport);
	DDX_Control(pDX, IDC_EDIT4, m_editfile);
	DDX_Control(pDX, IDC_BUTTON1, m_upload);
	DDX_Control(pDX, IDC_EDIT5, m_editurl);
	DDX_Control(pDX, IDC_EDIT6, m_showfile);
	DDX_Control(pDX, IDC_BUTTON7, m_URL);
	DDX_Control(pDX, IDC_BUTTON3, m_btnDownLoad);
	DDX_Control(pDX, IDC_COMBO1, m_comb);
	DDX_Control(pDX, IDC_EDIT7, m_editInfo);
	DDX_Control(pDX, IDC_COMBO2, m_combID);
}

BEGIN_MESSAGE_MAP(CCVR_ObjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCVR_ObjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCVR_ObjectDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CCVR_ObjectDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CCVR_ObjectDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CCVR_ObjectDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CCVR_ObjectDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CCVR_ObjectDlg::OnBnClickedButton8)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCVR_ObjectDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON9, &CCVR_ObjectDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CCVR_ObjectDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CCVR_ObjectDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CCVR_ObjectDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CCVR_ObjectDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


// CCVR_ObjectDlg 消息处理程序

BOOL CCVR_ObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ctrlDevIp.SetAddress(10,16,53,232);
	m_editnum.SetWindowText("admin");
	m_editcode.SetWindowText("hik12345+");
	m_editport.SetWindowText("8010");
	m_editcode.SetPasswordChar('*');
	m_strFileOut = "E:\\test\\";
	m_URL.EnableWindow(FALSE);
	m_comb.InsertString(0,_T("低速"));
	m_comb.InsertString(1,_T("中低速"));
	m_comb.InsertString(2,_T("中速"));
	m_comb.InsertString(3,_T("中高速"));
	m_comb.InsertString(4,_T("高速"));
	m_comb.InsertString(5,_T("全速"));
	m_comb.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCVR_ObjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCVR_ObjectDlg::OnPaint()
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
HCURSOR CCVR_ObjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCVR_ObjectDlg::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽ESC关闭窗体/
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) 
	{
		return TRUE;
	}
	//屏蔽回车关闭窗体,但会导致回车在窗体上失效.
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) 
	{
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}

}


//上传
void CCVR_ObjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strFileName;   
	m_editfile.GetWindowText(m_strFileName);
	if (m_strFileName == "")
	{
		AfxMessageBox(_T("请选择文件!"));
		return;
	}
	struct stat file_info = {0};
	if(stat(m_strFileName.GetBuffer(0), &file_info) != 0)
	{
		AfxMessageBox(_T("get local file length fail"));
		return;
	}
	m_upload.EnableWindow(FALSE);
	NET_DVR_UPLOAD_RECORD_INFO struRecordInfo = {0};
	memset(&m_struDataInfo, 0, sizeof(m_struDataInfo));
	m_struDataInfo.dwSize = sizeof(m_struDataInfo);
	m_struDataInfo.byFileType = 0;
	int poolID = m_combID.GetCurSel();
	m_struDataInfo.byStoragePoolID = poolID;
	m_struDataInfo.dwFileSize = file_info.st_size;
	strncpy(m_struDataInfo.szFileName, m_strFileName, sizeof(m_struDataInfo.szFileName));
	m_lUploadHandle = NET_DVR_UploadFile_V40(lLoginID, UPLOAD_DATA_FILE, &m_struDataInfo, sizeof(m_struDataInfo), m_strFileName.GetBuffer(0), NULL, 0);
	SetTimer(1,5000,NULL);
	if (m_lUploadHandle<0)
	{
		AfxMessageBox(_T("error"));
		m_upload.EnableWindow(TRUE);
	}
}
//下载
void CCVR_ObjectDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString url;
	m_editurl.GetWindowText(url);
	if (url.IsEmpty())
	{
		AfxMessageBox(_T("没有下载文件"));
		return;
	}
	m_btnDownLoad.EnableWindow(FALSE);
	memset(&m_struFileDownLoad, 0, sizeof(m_struFileDownLoad));
	m_struFileDownLoad.dwSize = sizeof(m_struFileDownLoad);
	strcpy((char*)m_struFileDownLoad.sUrl,url.GetBuffer(url.GetLength()));
	m_struFileDownLoad.fnDownloadDataCB = NULL;
	m_struFileDownLoad.pUserData = NULL;
	CString s_name;
	srand((unsigned)time(NULL));
	int i = rand();
	s_name.Format(_T("%d"),i);
	m_strFileOut = m_strFileOut + s_name + ".mp4";
	m_download = NET_DVR_StartDownload(lLoginID,NET_SDK_DOWNLOAD_DATA_FILE,&m_struFileDownLoad,sizeof(m_struFileDownLoad),m_strFileOut);
	SetTimer(2,5000,NULL);
}


//登录
void CCVR_ObjectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	NET_DVR_Init(); //初始化sdk
	CString DeviceIp;
	BYTE nField[4]; 
	m_ctrlDevIp.GetAddress(nField[0],nField[1],nField[2],nField[3]);
	DeviceIp.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);

	CString	m_nDevPort;//端口
	CString	m_csUser;//用户
	CString	m_csPWD; //密码
	m_editport.GetWindowText(m_nDevPort);
	m_editnum.GetWindowText(m_csUser);
	m_editcode.GetWindowText(m_csPWD);
	int port =  8010;
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));

	lLoginID = NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength())
		,port,m_csUser.GetBuffer(m_csUser.GetLength()),m_csPWD.GetBuffer(m_csPWD.GetLength()),&DeviceInfoTmp);
	if(lLoginID == -1)
	{
		CString error;
		error.Format(_T("%d"), NET_DVR_GetLastError()); 
		AfxMessageBox(error);
		MessageBox(_T("Login to Device failed!\n"));
		return;
	}
	else
	{
		AfxMessageBox(_T("登录成功!"));
	}
}


//定时器
void CCVR_ObjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		{
			DWORD dwProgress = 0;
			int state = NET_DVR_GetUploadState(m_lUploadHandle, &dwProgress);

			if (state == 1)
			{
				//上传成功将按钮复原,关闭定时器,获取URL并显示
				CString strUrl;
				memset(&m_struFileRet, 0, sizeof(m_struFileRet));
				NET_DVR_GetUploadResult(m_lUploadHandle,&m_struFileRet,sizeof(m_struFileRet));
				strUrl.Format(_T("%s"), m_struFileRet.sUrl);
				m_editurl.SetWindowText(strUrl);
				AfxMessageBox(_T("上传成功!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 2)
			{
				//AfxMessageBox(_T("正在上传!"));
			}
			else if (state == 3)
			{
				AfxMessageBox(_T("上传失败!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 4)
			{
				AfxMessageBox(_T("网络断开,状态未知!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 6)
			{
				AfxMessageBox(_T("硬盘错误!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 7)
			{
				AfxMessageBox(_T("无审讯文件存放盘!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 8)
			{
				AfxMessageBox(_T("容量不足!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 9)
			{
				AfxMessageBox(_T("设备资源不足!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 10)
			{
				AfxMessageBox(_T("文件个数超过最大值!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
			else if (state == 15)
			{
				AfxMessageBox(_T("文件类型错误!"));
				m_upload.EnableWindow(TRUE);
				KillTimer(1);
			}
		}
		break;
	case 2:
		{
			DWORD dwProgress = 0;
			int state = NET_DVR_GetDownloadState(m_download,&dwProgress);
			if (state == 1)
			{
				AfxMessageBox(_T("下载成功!"));
				m_btnDownLoad.EnableWindow(TRUE);
				KillTimer(2);
			}
			else if (state == 2)
			{
				//AfxMessageBox(_T("正在下载!"));
			}
			else if (state == 3)
			{
				AfxMessageBox(_T("下载失败!"));
				m_btnDownLoad.EnableWindow(TRUE);
				KillTimer(2);
			}
			else if (state == 4)
			{
				AfxMessageBox(_T("网络断开,状态未知!"));
				m_btnDownLoad.EnableWindow(TRUE);
				KillTimer(2);
			}
		}
		break;
	default:
		break;
	}

	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}


//浏览文件
void CCVR_ObjectDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strFileName;   
	CFileDialog dlg(TRUE,NULL,"*.*");   
	if(dlg.DoModal()==IDOK)
	{
		//当你找到文件并确定打开时   
		m_strFileName = dlg.GetPathName();
		//AfxMessageBox(m_strFileName);
	}
	m_editfile.SetWindowText(m_strFileName);
}


//退出
void CCVR_ObjectDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

//文件路径
void CCVR_ObjectDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	char szSelected[MAX_PATH]; //用来存放文件夹路径
	BROWSEINFO bi;
	LPITEMIDLIST pidl; 
	bi.hwndOwner = this->m_hWnd; 
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = szSelected; 
	bi.lpszTitle = "选择输出文件路径"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; 
	bi.lParam = NULL; 
	bi.iImage = NULL; 
	if((pidl = SHBrowseForFolder(&bi)) != NULL) 
	{ 
		if(SUCCEEDED(SHGetPathFromIDList(pidl, szSelected))) //得到文件夹的全路径，不要的话，只得本文件夹名
		{ 
			m_strFileOut = szSelected; 
		} 
	} 
	int lenth = m_strFileOut.GetLength();
	if (m_strFileOut[--lenth] != '\\')
	{
		m_strFileOut = m_strFileOut + '\\';
	}
	m_showfile.SetWindowText(m_strFileOut);

}

//速率设置
void CCVR_ObjectDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	/*NET_DVR_XML_CONFIG_INPUT    struInput = {0};
	NET_DVR_XML_CONFIG_OUTPUT   struOuput = {0};
	struInput.dwSize = sizeof(struInput);
	struInput.lpRequestUrl = "GET /ISAPI/ContentMgmt/CloudStorageServer/pool/count";
	struInput.dwRequestUrlLen = strlen("GET /ISAPI/ContentMgmt/CloudStorageServer/pool/count");
	DWORD dwOutputLen = 1024 * 1024;
	char *pOutBuf = new char[dwOutputLen];
	memset(pOutBuf, 0, dwOutputLen);
	struOuput.lpOutBuffer = pOutBuf;
	struOuput.dwOutBufferSize = dwOutputLen;
	struOuput.dwSize = sizeof(struOuput);
	NET_DVR_STDXMLConfig(lLoginID,&struInput,&struOuput);*/

	NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
	char szReqUrl[] = "PUT /ISAPI/ContentMgmt/FileData/TransferSpeedConfiguration\r\n";
	struInputParam.dwSize = sizeof(struInputParam);
	struInputParam.lpRequestUrl = szReqUrl;
	struInputParam.dwRequestUrlLen = strlen(szReqUrl);

	char szOutBuff[1024] = {0};
	char szStatusBuff[1024] = {0};
	NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
	struOutputParam.dwSize = sizeof(struOutputParam);
	struOutputParam.lpOutBuffer = szOutBuff;
	struOutputParam.dwOutBufferSize = sizeof(szOutBuff);
	struOutputParam.lpStatusBuffer = szStatusBuff;
	struOutputParam.dwStatusSize = sizeof(szStatusBuff);
	std::string strXMLTransferSpeedConfiguration = "<TransferSpeedConfiguration version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">";
	int i = m_comb.GetCurSel();    //获取速率
	string strSpeed = "full";
	switch(i)
	{
	case 0:
		{
			strSpeed = "low";
		}
		break;
	case 1:
		{
			strSpeed = "mediumLow";
		}
		break;
	case 2:
		{
			strSpeed = "medium";
		}
		break;
	case 3:
		{
			strSpeed = "mediumHigh";
		}
		break;
	case 4:
		{
			strSpeed = "high";
		}
		break;
	case 5:
		{
			strSpeed = "full";
		}
		break;
	default:break;
	}

	char szUpload[200] = {0};
	sprintf(szUpload, "<upload>%s</upload>", strSpeed.c_str());
	strXMLTransferSpeedConfiguration += szUpload;

	char szDownload[200] = {0};
	sprintf(szDownload, "<download>%s</download>", strSpeed.c_str());
	strXMLTransferSpeedConfiguration += szDownload;

	strXMLTransferSpeedConfiguration += "</TransferSpeedConfiguration>";

	struInputParam.lpInBuffer = (void*)(strXMLTransferSpeedConfiguration.c_str());
	struInputParam.dwInBufferSize = strlen(strXMLTransferSpeedConfiguration.c_str());

	if (!NET_DVR_STDXMLConfig(lLoginID, &struInputParam, &struOutputParam))
	{
		CString error;    //输出错误码
		error.Format(_T("%d"), NET_DVR_GetLastError()); 
		AfxMessageBox(error);
		return;
	}
	CString otpt;
	otpt.Format(_T("%s"),(char*)struOutputParam.lpStatusBuffer);
	AfxMessageBox(otpt);
}

void CCVR_ObjectDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


//锁定
void CCVR_ObjectDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
	char szReqUrl[] = "PUT /ISAPI/ContentMgmt/FileData/LockConfiguration\r\n";
	struInputParam.dwSize = sizeof(struInputParam);
	struInputParam.lpRequestUrl = szReqUrl;
	struInputParam.dwRequestUrlLen = strlen(szReqUrl);

	char szOutBuff[1024] = {0};
	char szStatusBuff[1024] = {0};
	NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
	struOutputParam.dwSize = sizeof(struOutputParam);
	struOutputParam.lpOutBuffer = szOutBuff;
	struOutputParam.dwOutBufferSize = sizeof(szOutBuff);
	struOutputParam.lpStatusBuffer = szStatusBuff;
	struOutputParam.dwStatusSize = sizeof(szStatusBuff);
	std::string strLockConfiguration  = "<LockConfiguration version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">";

	strLockConfiguration += "<LockList size=1>";
	strLockConfiguration += "<LockInfo>";

	strLockConfiguration += "<id>0</id>";

	CString str_URL;
	char szurl[200] = {0};
	m_editurl.GetWindowText(str_URL);
	if (str_URL.IsEmpty())
	{
		return;
	}
	
	sprintf(szurl, "<url>%s</url>", str_URL.GetBuffer(0));
	strLockConfiguration += szurl;

	strLockConfiguration += "<operateType>lock</operateType>";

	strLockConfiguration += "<lockTime>0xffffffff</lockTime>";

	strLockConfiguration += "</LockInfo>";
	strLockConfiguration += "</LockList>";

	strLockConfiguration += "</LockConfiguration>";

	struInputParam.lpInBuffer = (void*)(strLockConfiguration.c_str());
	struInputParam.dwInBufferSize = strlen(strLockConfiguration.c_str());

	if (!NET_DVR_STDXMLConfig(lLoginID, &struInputParam, &struOutputParam))
	{
		CString error;    //输出错误码
		error.Format(_T("%d"), NET_DVR_GetLastError()); 
		AfxMessageBox(error);
		return;
	}
	CString otpt;
	otpt.Format(_T("%s"),(char*)struOutputParam.lpStatusBuffer);
	m_editInfo.SetWindowText(otpt);
}

//获取
void CCVR_ObjectDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
	char szReqUrl[] = "POST /ISAPI/ContentMgmt/FileData/SearchFile\r\n";
	struInputParam.dwSize = sizeof(struInputParam);
	struInputParam.lpRequestUrl = szReqUrl;
	struInputParam.dwRequestUrlLen = strlen(szReqUrl);

	char szOutBuff[1024] = {0};
	char szStatusBuff[1024] = {0};
	NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
	struOutputParam.dwSize = sizeof(struOutputParam);
	struOutputParam.lpOutBuffer = szOutBuff;
	struOutputParam.dwOutBufferSize = sizeof(szOutBuff);
	struOutputParam.lpStatusBuffer = szStatusBuff;
	struOutputParam.dwStatusSize = sizeof(szStatusBuff);
	std::string strSearchFileResult  = "<SearchFileCondition version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">";

	strSearchFileResult += "<SearchList size=1>";
	strSearchFileResult += "<SearchInfo>";

	CString str_URL;
	char szurl[200] = {0};
	m_editurl.GetWindowText(str_URL);
	if (str_URL.IsEmpty())
	{
		return;
	}

	sprintf(szurl, "<url>%s</url>", (char*)str_URL.GetBuffer(0));
	strSearchFileResult += szurl;

	strSearchFileResult += "</SearchInfo>";

	strSearchFileResult += "</SearchList>";
	strSearchFileResult += "</SearchFileCondition>";

	struInputParam.lpInBuffer = (void*)(strSearchFileResult.c_str());
	struInputParam.dwInBufferSize = strlen(strSearchFileResult.c_str());

	if (!NET_DVR_STDXMLConfig(lLoginID, &struInputParam, &struOutputParam))
	{
		CString error;    //输出错误码
		error.Format(_T("%d"), NET_DVR_GetLastError()); 
		AfxMessageBox(error);
		return;
	}
	CString otpt;
	otpt.Format(_T("%s"),(char*)struOutputParam.lpOutBuffer);
	m_editInfo.SetWindowText(otpt);
}


//解锁
void CCVR_ObjectDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
	char szReqUrl[] = "PUT /ISAPI/ContentMgmt/FileData/LockConfiguration\r\n";
	struInputParam.dwSize = sizeof(struInputParam);
	struInputParam.lpRequestUrl = szReqUrl;
	struInputParam.dwRequestUrlLen = strlen(szReqUrl);

	char szOutBuff[1024] = {0};
	char szStatusBuff[1024] = {0};
	NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
	struOutputParam.dwSize = sizeof(struOutputParam);
	struOutputParam.lpOutBuffer = szOutBuff;
	struOutputParam.dwOutBufferSize = sizeof(szOutBuff);
	struOutputParam.lpStatusBuffer = szStatusBuff;
	struOutputParam.dwStatusSize = sizeof(szStatusBuff);
	std::string strunLockConfiguration  = "<LockConfiguration version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">";
	//int i = m_comb.GetCurSel();    //获取速率

	strunLockConfiguration += "<LockList size=1>";
	strunLockConfiguration += "<LockInfo>";

	strunLockConfiguration += "<id>0</id>";

	CString str_URL;
	char szurl[200] = {0};
	m_editurl.GetWindowText(str_URL);
	if (str_URL.IsEmpty())
	{
		return;
	}

	sprintf(szurl, "<url>%s</url>", str_URL.GetBuffer(0));
	strunLockConfiguration += szurl;

	strunLockConfiguration += "<operateType>unlock</operateType>";

	strunLockConfiguration += "<lockTime>0xffffffff</lockTime>";

	strunLockConfiguration += "</LockInfo>";
	strunLockConfiguration += "</LockList>";

	strunLockConfiguration += "</LockConfiguration>";

	struInputParam.lpInBuffer = (void*)(strunLockConfiguration.c_str());
	struInputParam.dwInBufferSize = strlen(strunLockConfiguration.c_str());

	if (!NET_DVR_STDXMLConfig(lLoginID, &struInputParam, &struOutputParam))
	{
		CString error;    //输出错误码
		error.Format(_T("%d"), NET_DVR_GetLastError()); 
		AfxMessageBox(error);
		return;
	}
	CString otpt;
	otpt.Format(_T("%s"),(char*)struOutputParam.lpStatusBuffer);
	AfxMessageBox(otpt);
	m_editInfo.SetWindowText(otpt);
}

//文件查看
void CCVR_ObjectDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strFileName;   //文件名   包括路径  
	CString path;
	path = m_strFileOut + "*.*";
	CFileDialog dlg(TRUE,NULL,path);   
	if(dlg.DoModal()==IDOK)
	{ 
		m_strFileName = dlg.GetPathName();
	}
}

void CALLBACK g_PhyDiskInfoCallback(DWORD dwDataType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
	CCVR_ObjectDlg *pThis = static_cast<CCVR_ObjectDlg*>(pUserData);

	HWND hWnd = pThis->GetSafeHwnd();
	if (NULL == hWnd)
	{
		return;
	}
	if (dwDataType == NET_SDK_CALLBACK_TYPE_DATA)
	{
		NET_DVR_PHY_DISK_INFO struPhyDiskInfo = {0};


		memcpy(&struPhyDiskInfo,lpBuffer,dwBufLen);
		char *pSzTemp = new char[dwBufLen];
		if (pSzTemp == NULL)
		{
			return;
		}
		memset(pSzTemp,0, dwBufLen);

		memcpy(pSzTemp,&struPhyDiskInfo,dwBufLen);
		::PostMessage(hWnd,WM_PROC_PHY_DISK_INFO_SHOW, (WPARAM)pSzTemp,dwBufLen);
		pThis->m_disk.push_back(struPhyDiskInfo);
		int poolID = struPhyDiskInfo.wPhySlot;
		int filetype = struPhyDiskInfo.byFunctionType;
		CString str;
		switch(filetype)
		{
		case 0:
			{
				str = "视频";
			}
			break;
		case 1:
			{
				str = "文件";
			}
			break;
		case 2:
			{
				str = "图片";
			}
			break;
		case 3:
			{
				str = "存档卷";
			}
			break;
		default:break;
		}
		pThis->m_combID.InsertString(poolID,str);
		pThis->m_combID.SetCurSel(0);
	}
}

//磁盘信息
void CCVR_ObjectDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	NET_DVR_StartRemoteConfig(lLoginID,NET_DVR_GET_PHY_DISK_INFO,NULL,0,g_PhyDiskInfoCallback,this);
}
