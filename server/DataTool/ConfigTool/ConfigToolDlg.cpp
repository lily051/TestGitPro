
// ConfigToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"
#include "./utils/myutils.h"
#include <string>
#include "./include/hak.h"
#include "./tinyxml/tinyxml.h"
#include "./tinyxml/tinystr.h"
#include "Tchar.h"
#include <iostream>
#include <fstream>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"Iphlpapi.lib")

void AppFun_GetAllLocalIPAddr(std::vector<CString> & vecIP)   
{       
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    pIPAddrTable = (MIB_IPADDRTABLE*) GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));
    if ( pIPAddrTable ) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            GlobalFree( pIPAddrTable );
            pIPAddrTable = (MIB_IPADDRTABLE *) GlobalAlloc (GPTR, dwSize);
        }
    }
    else
    {
        OutputDebugString(_T("@ljg-Memory allocation failed."));
    }

    if ( pIPAddrTable ) 
    {
        // Make a second call to GetIpAddrTable to get the
        // actual data we want
        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == NO_ERROR ) 
        { 
            for (unsigned int i=0;i<dwSize/sizeof(MIB_IPADDRTABLE);++i)
            {
                if (pIPAddrTable->table[i].dwAddr != 0)
                {
                    in_addr in;
                    in.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                    CString tempIP;
                    tempIP = ::inet_ntoa(in);
                    vecIP.push_back(tempIP);
                }
            }
        }
        else 
        {
            OutputDebugString(_T("@ljg-Call to GetIpAddrTable failed."));
        }
    }

    if (pIPAddrTable)
    {
        GlobalFree( pIPAddrTable );
    }
}


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


// CConfigToolDlg 对话框




CConfigToolDlg::CConfigToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    ZeroMemory(m_szCfgPath, sizeof(m_szCfgPath));
	m_isCip = true;
}

void CConfigToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_DATA_PORT, m_editDataCenterPort);
    DDX_Control(pDX, IDC_EDIT_LOGIN_NAME, m_editLoginName);
    DDX_Control(pDX, IDC_EDIT_LOGIN_PWD, m_editLoginPwd);
    DDX_Control(pDX, IDC_COMBO_DBMS, m_cbDBMSType);
    DDX_Control(pDX, IDC_EDIT_LISTEN_PORT, m_edtListenPort);
	DDX_Control(pDX, IDC_EDIT_LISTEN_PORT2, m_edtWebPort);
    DDX_Control(pDX, IDC_IPADDRESS_DB, m_ipctrlDB);
    DDX_Control(pDX, IDC_IPADDRESS_LOCAL, m_ipctrlLocal);
    DDX_Control(pDX, IDC_DROPDOWN, m_dropdown);
}

BEGIN_MESSAGE_MAP(CConfigToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SAVE, &CConfigToolDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CConfigToolDlg::OnBnClickedBtnExit)
    ON_CBN_SELCHANGE(IDC_COMBO_DBMS, &CConfigToolDlg::OnCbnSelchangeComboDbms)
END_MESSAGE_MAP()


// CConfigToolDlg 消息处理程序

BOOL CConfigToolDlg::OnInitDialog()
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
    m_editDataCenterPort.SetLimitText(6);
    m_edtListenPort.SetLimitText(6);
    m_editLoginName.SetLimitText(10);
    m_editLoginPwd.SetLimitText(16);

    AppFun_GetAllLocalIPAddr(m_vecLocalIP);

    // 设置配置文件路径
    WCHAR szPath[MAX_PATH] = {0};
    GetCurrentDirectoryW(MAX_PATH, szPath);
	PathCombine(m_szCfgPath, szPath, _T("\config.properties"));

	Hak1((char*)103, "DATA", NULL);
	ReadConfig();

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

char* CConfigToolDlg::get_file(char *cBuff)
{
	//	获取文件路径
	GetCurrentDirectoryA(MAX_PATH, m_xmlPath);
	strcat(m_xmlPath, "\\conf\\server.xml");

	//	读取文件内容
	locale::global(locale(""));//将全局区域设为操作系统默认区域
	ifstream file (m_xmlPath, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		int nsize = file.tellg();
		file.seekg (0, ios::beg);
		file.read (cBuff, nsize);
		file.close();
	}
	file.close();
	locale::global(locale("C"));//还原全局区域设定
	return cBuff;
}

char* CConfigToolDlg::getBatCfg(char *cBuff)
{
	//	获取文件路径
	GetCurrentDirectoryA(MAX_PATH, m_batPath);
	strcat(m_batPath, "\\ServiceModuleConfig.dat");

	//	读取文件内容
	locale::global(locale(""));//将全局区域设为操作系统默认区域
	ifstream file (m_batPath, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		int nsize = file.tellg();
		file.seekg (0, ios::beg);
		file.read (cBuff, nsize);
		file.close();
	}
	file.close();
	locale::global(locale("C"));//还原全局区域设定

	return cBuff;
}

std::string CConfigToolDlg::load_displayName_xml()
{
	char cBuff[FILE_BUFF] = {0};
	char *buff = get_file(cBuff);
	
	//	获取并解析XML
	TiXmlDocument xml;
	xml.Parse(buff);
	TiXmlHandle docHandle(&xml);
	TiXmlElement* pRtsp = docHandle.FirstChild("Server").FirstChild("Service").FirstChild("Connector").ToElement();
	std::string strWebPort = "";
	if (pRtsp && pRtsp->Attribute("port"))
	{
		strWebPort = pRtsp->Attribute("port");
	}

	return strWebPort;
}

void CConfigToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConfigToolDlg::OnPaint()
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
HCURSOR CConfigToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int is_valid_ip(const char *ip_str) 
{
    char temp[31];
    int a,b,c,d; 
    if(sscanf(ip_str, "%d.%d.%d.%d ",&a,&b,&c,&d)==4
        &&   a>=0   &&   a<=255
        &&   b>=0   &&   b<=255
        &&   c>=0   &&   c<=255
        &&   d>=0   &&   d<=255)
    {    
        sprintf(temp, "%d.%d.%d.%d",a,b,c,d);    //把格式化的数据写入字符串temp     
        if(strcmp(temp,ip_str)==0)     
        {    
            return 1;     
        }     
        else    
        {    
            return 0;
        }    
    }    
    else     
    {    
        return 0; 
    }
}


void CConfigToolDlg::OnBnClickedBtnSave()
{
    //检查参数的合法性
    DWORD dwDBAddress;
    int nRet = m_ipctrlDB.GetAddress(dwDBAddress);
    if (4 != nRet) 
    {
        MessageBox(_T("数据库IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlDB.SetFocus();
        return;
    }
    if (0 == dwDBAddress)
    {
        MessageBox(_T("数据库IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlDB.SetFocus();
        return;
    }

    DWORD dwLocalAddress;
    nRet = m_ipctrlLocal.GetAddress(dwLocalAddress);
    if (4 != nRet) 
    {
        MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlLocal.SetFocus();
        return;
    }
    if (0 == dwLocalAddress)
    {
        MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlLocal.SetFocus();
        return;
    }

    CString sDBIP,sDBPort,sLocalIP,sDBUser,sListenPort,sDBPwd;
    m_ipctrlDB.GetWindowText(sDBIP);
    sDBIP.Trim();
    m_editDataCenterPort.GetWindowText(sDBPort);
    sDBPort.Trim();
    m_ipctrlLocal.GetWindowText(sLocalIP);
    sLocalIP.Trim();
    m_editLoginName.GetWindowText(sDBUser);
    sDBUser.Trim();
    m_editLoginPwd.GetWindowText(sDBPwd);
    sDBPwd.Trim();
    m_edtListenPort.GetWindowText(sListenPort);
    sListenPort.Trim();

    if (!is_valid_ip(CStringA(sDBIP)))
    {
        MessageBox(_T("数据库IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlLocal.SetFocus();
        return;
    }

    if (sDBPort.IsEmpty())
    {
        MessageBox(_T("数据库端口不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_editDataCenterPort.SetFocus();
        m_editDataCenterPort.SetSel(0, -1);
        return;
    }

    int nDBPort = _tstoi(sDBPort);
    if (nDBPort<=1024 || nDBPort >=65535)
    {
        MessageBox(_T("数据库端口范围为1024~65535!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_editDataCenterPort.SetFocus();
        m_editDataCenterPort.SetSel(0, -1);
        return;
    }

    if (sDBUser.IsEmpty())
    {
        MessageBox(_T("数据库登录名不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_editLoginName.SetFocus();
        m_editLoginName.SetSel(0, -1);
        return;
    }

    if (sDBPwd.IsEmpty())
    {
        MessageBox(_T("数据库登录密码不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_editLoginPwd.SetFocus();
        m_editLoginPwd.SetSel(0, -1);
        return;
    }

    if (!is_valid_ip(CStringA(sLocalIP)))
    {
        MessageBox(_T("本机IP地址不合法!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ipctrlLocal.SetFocus();
        return;
    }

    int nListenPort = _tstoi(sListenPort);
    if (nListenPort<=1 || nListenPort >=65535)
    {
        MessageBox(_T("监听端口范围为1~65535!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_edtListenPort.SetFocus();
        m_edtListenPort.SetSel(0, -1);
        return;
    }

	SaveConfig();
    MessageBox(_T("保存成功!"),_T("提示"),MB_ICONINFORMATION|MB_OK);

	Hak2(NULL, NULL, NULL);
}

void CConfigToolDlg::OnBnClickedBtnExit()
{
	Hak2(NULL, NULL, NULL);
	CConfigToolDlg::OnCancel();
}

//读取配置文件
void CConfigToolDlg::ReadConfig()
{
    CString strLocalIP;
    if (m_vecLocalIP.size() > 0)
    {
        strLocalIP = m_vecLocalIP[0];
    }
    
    // 读取配置
    // Database
	//	数据库类型
	int nDBMS = -1;
	TCHAR szDbUrl[250] = {0};
	GetPrivateProfileString(_T("database"), _T("hibernate.connection.url"), _T("127.0.0.1"), szDbUrl, 250, m_szCfgPath);
	TCHAR szDbUrlTmp[250] = {0};
	wcscpy(szDbUrlTmp, szDbUrl);
	const TCHAR aTypeFlag[]= _T("mysql");
	TCHAR* szDbType = wcstok((TCHAR*)szDbUrlTmp, aTypeFlag); 
	if(szDbType != NULL)
	{
		nDBMS = MYSQL_DB;
	}
	else
	{
		nDBMS = SQLSERVER_DB;
	}
	//	读取IP、Port
	const TCHAR split[]= _T("//");
	TCHAR* szTmp = wcstok((TCHAR*)szDbUrl, split); 
	if(szTmp != NULL) 
	{
		szTmp = wcstok(NULL, split); 
	}
	int nDatabasePort = -1;
	const TCHAR szFlag[]= _T(":");
	TCHAR *szDatabaseIp = wcstok((TCHAR*)szTmp, szFlag); 
	TCHAR *szPort = NULL;
	if(szDatabaseIp != NULL) 
	{
		szPort = wcstok(NULL, split); 
		nDatabasePort = _wtoi(szPort);
	}
	//	读取账号、密码
    TCHAR szLoginName[20] = {0};
    GetPrivateProfileString(_T("database"), _T("hibernate.connection.username"), _T("dml_mgr"), szLoginName, 20, m_szCfgPath);

	WCHAR szCipherInIni[20] = {0};
	GetPrivateProfileStringW(L"database", L"hibernate.connection.password", L"", szCipherInIni, 20, m_szCfgPath);

	wstring wsDbPwd = L"888";
	wstring wsCipher = szCipherInIni;
	string sCipher;
	if (!wsCipher.empty())
	{
		//	密文配置
		string srPwd = myutils::CP936_W2A(wsCipher);
		sCipher = myutils::DecipherStr(srPwd);
		m_pwd=sCipher;
	}

    // CMService
    TCHAR szListenIP[50] = {0};
    GetPrivateProfileString(_T("local-info"), _T("local.ip"), strLocalIP, szListenIP, 50, m_szCfgPath);        
	TCHAR szListenPort[20] = {0};
	GetPrivateProfileString(_T("cms-port"), _T("cms.port"), _T("root"), szListenPort, 20, m_szCfgPath);
    // Tunings
    int nHppThreadNum = GetPrivateProfileInt(_T("Tunings"), _T("HppThreadNum"), 5, m_szCfgPath);
    int nHppContextTimeout = GetPrivateProfileInt(_T("Tunings"), _T("HppContextTimeout"), 10, m_szCfgPath);
    int nWorkThreadNum = GetPrivateProfileInt(_T("Tunings"), _T("WorkThreadNum"), 5, m_szCfgPath);
    int nDBConnNum = GetPrivateProfileInt(_T("Tunings"), _T("DBConnNum"), 10, m_szCfgPath);

	//	读取文本配置server.xml，获取web端口
	m_strWebPort = load_displayName_xml();
	if(0 == m_strWebPort.length())
	{
		MessageBox(_T("读取文件./conf/server.xml失败"),_T("错误"),MB_ICONWARNING|MB_OK);
		return;
	}

    CString strTemp;
    m_cbDBMSType.SetCurSel(nDBMS);
    m_ipctrlDB.SetWindowText(szDatabaseIp);
    strTemp.Format(_T("%d"), nDatabasePort);
    m_editDataCenterPort.SetWindowText(strTemp);
    m_editLoginName.SetWindowText(szLoginName);
	wstring tt= myutils::CP936_A2W(sCipher);
    m_editLoginPwd.SetWindowText(tt.c_str());        
    m_ipctrlLocal.SetWindowText(szListenIP);
  //  strTemp.Format(_T("%d"), nPort);
    m_edtListenPort.SetWindowText(szListenPort);
	wstring wStrWebPort = myutils::CP936_A2W(m_strWebPort);
	m_edtWebPort.SetWindowText(wStrWebPort.c_str());
}

/*lint -e429*/ 
//保存配置文件
void CConfigToolDlg::SaveConfig()
{
	//	DB IP
	CString cStrIp;
	m_ipctrlDB.GetWindowText(cStrIp);
	wstring wStrIp = cStrIp.GetString();
	//	DB Port
	CString cStrPort;
	m_editDataCenterPort.GetWindowText(cStrPort);
	wstring wStrPort = cStrPort.GetString();
	//	DB 登录名
	CString cStrName;
	m_editLoginName.GetWindowText(cStrName);
	//	DB 密码
	CString cStrPwd;
	m_editLoginPwd.GetWindowText(cStrPwd);
	wstring wsStr = myutils::CP936_A2W(m_pwd);

    string cipher = myutils::CP936_W2A(cStrPwd.GetString());
    string sCipher = myutils::CipherStr(cipher);
	string sSorc = myutils::DecipherStr(sCipher);
    //	string转cstring
    cStrPwd=sCipher.c_str();
	
	//	本地IP
	CString cStrLocalIp;
	m_ipctrlLocal.GetWindowText(cStrLocalIp);
	//	本地Port
	CString cStrLocalPort;
	m_edtListenPort.GetWindowText(cStrLocalPort);
	//	web port
	{
		//	获取工具配置的web端口
		CString cStrWebPort;
		m_edtWebPort.GetWindowText(cStrWebPort);

		std::string strNewPort = myutils::CP936_W2A(cStrWebPort.GetString()).c_str();
		const char *cNewPortTmp = strNewPort.c_str();
		int nNewPortLen = strlen(cNewPortTmp);

		//	老的web端口
		const char *cPortTmp = m_strWebPort.c_str();
		int nOldPortLen = strlen(cPortTmp);
		if(strcmp(cNewPortTmp, cPortTmp) != 0)		//	新的web端口写入到server.xml和ServiceModuleConfig.dat
		{
			/*	写入到server.xml中	*/
			//	读取XML，获取文件内容
			char file_Buff[FILE_BUFF] = {0};
			char *cBuff = NULL;
			cBuff = get_file(file_Buff);
			if(cBuff == NULL)
			{
				MessageBox(_T("读取./conf/server.xml文件出错"),_T("错误"),MB_ICONWARNING|MB_OK);
				return;
			}
			//	XML新长度
			int nXmlLen = strlen(cBuff)-nOldPortLen+nNewPortLen;
			//	组成新的字符串
			char cOldPort[64] = {0};
			sprintf(cOldPort, "\"%s\"", cPortTmp);

			char *cPortPosition = strstr(cBuff, cOldPort);
			if(NULL == cPortPosition)
			{
				return;
			}
			int nPosiont = strlen(cBuff) - strlen(cPortPosition);

			
			char cTemp[FILE_BUFF] = {0};
			strncpy(cTemp, cBuff, nPosiont+1);
			strcat_s(cTemp, cNewPortTmp);
			strcat_s(cTemp, cBuff+nPosiont+1+nOldPortLen);
			cBuff[nXmlLen-1] = '\0';
			//	写入文件
			ofstream on (m_xmlPath, ios::in|ios::binary|ios::ate);
			on.seekp(0, ios::beg);
			on.write(cTemp, FILE_BUFF);
			on.close();

			/*	写入到ServiceModuleConfig.dat中	*/
			//	读取XML，获取文件内容
			char bat_buff[FILE_BUFF] = {0};
			char *cBatBuff = NULL;
			cBatBuff = getBatCfg(bat_buff);
			if(cBatBuff == NULL)
			{
				MessageBox(_T("读取./ServiceModuleConfig.dat文件出错"),_T("错误"),MB_ICONWARNING|MB_OK);
				return;
			}
			//	XML新长度
			int nBatLen = strlen(cBatBuff)-nOldPortLen+nNewPortLen;

			char *cBatPortPosition = strstr(cBatBuff, cPortTmp);
			if(NULL == cBatPortPosition)
			{
				return;
			}
			int nBatPortPosition = strlen(cBatBuff) - strlen(cBatPortPosition);

			char cBatTemp[FILE_BUFF] = {0};
			strncpy(cBatTemp, cBatBuff, nBatPortPosition);

			strcat_s(cBatTemp, cNewPortTmp);
			strcat_s(cBatTemp, cBatBuff+nBatPortPosition+nOldPortLen);
			cBatTemp[nXmlLen-1] = '\0';
			//	写入文件
			ofstream of (m_batPath, ios::in|ios::binary|ios::ate);
			of.seekp(0, ios::beg);
			of.write(cBatTemp, FILE_BUFF);
			of.close();
		}
	}	
	//	连接参数
	int nDBMS = m_cbDBMSType.GetCurSel();
	wstring wStrConnectUrl;
	wstring wStrMysqlUrlTmp = _T("jdbc:mysql://%s:%s/ivms8000fms?useUnicode=true&characterEncoding=utf8&useOldAliasMetadataBehavior=true");
	wstring wStrSqlserverUrlTmp = _T("jdbc:sqlserver://%s:%s;DatabaseName=ivms8000fms");
	if(MYSQL_DB == nDBMS)
	{
		myutils::StdWStrFormat(wStrConnectUrl, wStrMysqlUrlTmp.c_str(), wStrIp.c_str(), wStrPort.c_str());
	}
	else
	{
		myutils::StdWStrFormat(wStrConnectUrl, wStrSqlserverUrlTmp.c_str(), wStrIp.c_str(), wStrPort.c_str());
	}
	//	根据数据库类型写入配置数据
	if(MYSQL_DB == nDBMS)
	{
		WritePrivateProfileStringW(_T("local-info"), _T("local.ip"), cStrLocalIp, m_szCfgPath);
		WritePrivateProfileStringW(_T("cms-port"), _T("cms.port"), cStrLocalPort, m_szCfgPath);
	//	WritePrivateProfileStringW(_T("web"), _T("web.port"), cStrWebPort, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.driver_class"), _T("com.mysql.jdbc.Driver"), m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.dialect"), _T("org.hibernate.dialect.MySQL5InnoDBDialect"), m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.username"), cStrName, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.password"), cStrPwd, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.url"), wStrConnectUrl.c_str(), m_szCfgPath);
	}
	else
	{
		WritePrivateProfileStringW(_T("local-info"), _T("local.ip"), cStrLocalIp, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.driver_class"), _T("com.microsoft.sqlserver.jdbc.SQLServerDriver"), m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.dialect"), _T("org.hibernate.dialect.SQLServerDialect"), m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.username"), cStrName, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.password"), cStrPwd, m_szCfgPath);
		WritePrivateProfileStringW(_T("database"), _T("hibernate.connection.url"), wStrConnectUrl.c_str(), m_szCfgPath);
	}
}
/*lint +e429*/

BOOL CConfigToolDlg::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            if (VK_F4 == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CConfigToolDlg::OnCancel()
{
    CDialog::OnCancel();
}

void CConfigToolDlg::OnCbnSelchangeComboDbms()
{
    // TODO: 在此添加控件通知处理程序代码
    int nCurSel = m_cbDBMSType.GetCurSel();
    if (0 == nCurSel)   // MySQL
    {
        m_editDataCenterPort.SetWindowText(_T("3306"));
        m_editLoginName.SetWindowText(_T("dml_mgr"));
    }else if (1 == nCurSel) // SQL Server
    {
        m_editDataCenterPort.SetWindowText(_T("1433"));
        m_editLoginName.SetWindowText(_T("sa"));
    }
}

void CConfigToolDlg::IPHistoryDropdown(void)
{
    int ipCount = m_vecLocalIP.size();
    if (ipCount <= 0)
    {
        return;
    }

    CMenu IPListMenu;
    IPListMenu.CreatePopupMenu();	// create menu

    for (int i = 0; i < ipCount; ++i)
    {
        if (!m_vecLocalIP[i].IsEmpty())
        {
            IPListMenu.AppendMenu(MF_STRING | MF_ENABLED, i+1, m_vecLocalIP[i]);
        }
    }

    POINT curPos;
    GetCursorPos(&curPos);
    SetForegroundWindow();

    // show the menu
    int nSel = IPListMenu.TrackPopupMenu(
        TPM_LEFTBUTTON | TPM_RIGHTALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
        curPos.x, curPos.y+10, this);

    if (nSel > 0)
    {
        // fill the IP address control with the selection in the menu
        m_ipctrlLocal.SetWindowText(m_vecLocalIP[nSel-1]);
        ::PostMessage(m_hWnd, 0, 0, 0);
    }    

    // destroy pop up menu
    IPListMenu.DestroyMenu();
}