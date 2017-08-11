
// GetInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetInfo.h"
#include "GetInfoDlg.h"
#include "HCNetSDK.h"
#include "DecodeCardSdk.h"
#include "DataType.h"
#include "plaympeg4.h"
#include <string>
using namespace std;
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


// CGetInfoDlg 对话框




CGetInfoDlg::CGetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GETINFO, &CGetInfoDlg::OnBnClickedBtnGetinfo)
END_MESSAGE_MAP()


// CGetInfoDlg 消息处理程序

BOOL CGetInfoDlg::OnInitDialog()
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

	SetDlgItemText(IDC_EDIT1,L"127.0.0.1");
	SetDlgItemText(IDC_EDIT2,L"8000");
	SetDlgItemText(IDC_EDIT3,L"admin");
	SetDlgItemText(IDC_EDIT4,L"hik12345+");

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGetInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetInfoDlg::OnPaint()
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
HCURSOR CGetInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetInfoDlg::OnBnClickedBtnGetinfo()
{
	// TODO: 在此添加控件通知处理程序代码

	//---------------------------------------
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo;// = {0};
	memset(&struLoginInfo,0,sizeof(NET_DVR_USER_LOGIN_INFO));

	CString csip,csport,csuser,cspsd;

	GetDlgItemText(IDC_EDIT1,csip);
	GetDlgItemText(IDC_EDIT2,csport);
	GetDlgItemText(IDC_EDIT3,csuser);
	GetDlgItemText(IDC_EDIT4,cspsd);
   
    int port = _ttoi(csport);
	//strcpy_s(struLoginInfo.sDeviceAddress, "10.16.37.105"); //设备IP地址

    
	USES_CONVERSION;

	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy_s(struLoginInfo.sDeviceAddress, T2A(csip)); //设备IP地址
	struLoginInfo.wPort = port; //设备服务端口
	strcpy_s(struLoginInfo.sUserName, T2A(csuser)); //设备登录用户名
	strcpy_s(struLoginInfo.sPassword, T2A(cspsd)); //设备登录密码

	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
	//注册设备
	LONG lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	
	if (lUserID == -1)
	{
		AfxMessageBox(L"登录失败");
		return ;
	}

	CString strInfo;
	NET_DVR_HDCFG struHDCfg;
	memset(&struHDCfg,0,sizeof(NET_DVR_HDCFG));
	DWORD BytesReturned = 0;
	if(!NET_DVR_GetDVRConfig(lUserID
		,NET_DVR_GET_HDCFG
		,0xFFFFFFFF
		,&struHDCfg,sizeof(NET_DVR_HDCFG)
		,&BytesReturned))
	{
		LONG dwErrno = NET_DVR_GetLastError();
		std::string szLastErr = NET_DVR_GetErrorMsg(&dwErrno);

        CString str;
	    //String 转CString
		int len = MultiByteToWideChar(CP_ACP, NULL, szLastErr.c_str(), szLastErr.size(), NULL, 0); 
		wchar_t* szUnicode = new wchar_t[len+1]; 
		MultiByteToWideChar(CP_ACP, NULL, szLastErr.c_str(), szLastErr.size(), szUnicode, len); 
		szUnicode[len] = 0; 
		std::wstring rs = szUnicode; 
        str = szUnicode;
		strInfo.Format(_T("获取磁盘信息失败，错误原因(%s)!"),str);
		MessageBox(strInfo, _T("错误"), MB_ICONSTOP);
		NET_DVR_Logout_V30(lUserID);
		return;
	}
	else
	{
		CString count,dwHDNo,dwCapacity,dwFreeSpace,dwHdStatus,dwStorageType,byRecycling;
		CString byHDAttr,byHDType,byDiskDriver,dwHdGroup,dwPictureCapacity,dwFreePictureSpace;
		CString strHDInfo;
        
		strHDInfo.Format(_T("硬盘总数 %d 个！ \r\n"),struHDCfg.dwHDCount);
		for (int i = 0; i < struHDCfg.dwHDCount; i++)
		{
			dwHDNo.Format(_T("硬盘号%d; "),struHDCfg.struHDInfo[i].dwHDNo);
			dwCapacity.Format(_T("总容量%dM; "),struHDCfg.struHDInfo[i].dwCapacity);
			dwFreeSpace.Format(_T("剩余容量%dM; "),struHDCfg.struHDInfo[i].dwFreeSpace);
			dwHdStatus.Format(_T("硬盘状态%d; \r\n"),struHDCfg.struHDInfo[i].dwHdStatus);
			strHDInfo+=dwHDNo+dwCapacity+dwFreeSpace+dwHdStatus;
		}

		
		//byHDAttr.Format(_T("0-普通, 1-冗余 2-只读 3-存档（CVR专用）4-不可读写  %d; "),struHDCfg.struHDInfo[0].byHDAttr);;       /*0-普通, 1-冗余; 2-只读 3-存档（CVR专用），4-不可读写*/
		//byHDType.Format(_T("0-本地硬盘,1-ESATA硬盘,2-NAS硬盘,3-iSCSI硬盘 4-Array虚拟磁盘,5-SD卡  %d; "),struHDCfg.struHDInfo[0].byHDType);;		  /*0-本地硬盘,1-ESATA硬盘,2-NAS硬盘,3-iSCSI硬盘 4-Array虚拟磁盘,5-SD卡*/
		//byDiskDriver.Format(_T("值 代表其ASCII字符 %d; "),struHDCfg.struHDInfo[0].byDiskDriver);;   // 值 代表其ASCII字符 
		////byRes1.Format(_T("属于哪个盘组%d"),struHDCfg.struHDInfo[0].byRes1);;
		//dwHdGroup.Format(_T("属于哪个盘组  %d; "),struHDCfg.struHDInfo[0].dwHdGroup);;      /*属于哪个盘组 1-MAX_HD_GROUP*/
		//byRecycling.Format(_T("是否循环利用 0不循环利用1循环利用  %d; "),struHDCfg.struHDInfo[0].byRecycling);;   // 是否循环利用 0：不循环利用，1：循环利用
		////BYTE  byRes2[3];
		//dwStorageType.Format(_T("按位表示 0-不支持 非0-支持  %d; "),struHDCfg.struHDInfo[0].dwStorageType);    //按位表示 0-不支持 非0-支持

		////dwHdGroup.Format(_T("属于哪个盘组%d"),struHDCfg.struHDInfo[0].dwHdGroup);

		//dwPictureCapacity.Format(_T("硬盘图片容量 %d; "),struHDCfg.struHDInfo[0].dwPictureCapacity);
		//dwFreePictureSpace.Format(_T("剩余图片空间 %d; "),struHDCfg.struHDInfo[0].dwFreePictureSpace);

		MessageBox(strHDInfo, _T("硬盘状态正常"), MB_ICONINFORMATION);
		NET_DVR_Logout_V30(lUserID);
		return;
		//MessageBox(L"OK", _T("错误"), MB_ICONSTOP);
	}
	if (struHDCfg.dwHDCount == 0)
	{
		strInfo.Format(_T("打开录像失败，该设备没有硬盘存储录像!"));
		MessageBox(strInfo, _T("错误"), MB_ICONSTOP);
		//return void;
	}
	NET_DVR_Logout_V30(lUserID);
	
}
