
// DBSimulateToolForTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBSimulateToolForTest.h"
#include "DBSimulateToolForTestDlg.h"

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


// CDBSimulateToolForTestDlg 对话框


CDBSimulateToolForTestDlg::CDBSimulateToolForTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBSimulateToolForTestDlg::IDD, pParent)
    , m_strDBIP(_T("127.0.0.1"))
    , m_strDBPort(_T("3306"))
    , m_strDBUser(_T("root"))
    , m_strDBPassword(_T("888"))
    , m_strDBSchema(_T("ivms8120v2"))
    , m_strDBIPRef(_T("127.0.0.1"))
    , m_strDBPortRef(_T("3306"))
    , m_strDBUserRef(_T("root"))
    , m_strDBPasswordRef(_T("888"))
    , m_strDBSchemaRef(_T("ivms8120v2"))
    , m_bLogin(false)
    , m_bLoginRef(false)
    , m_bRadioRegion(false)
    , m_bRadioProduct(false)
    , m_bRadioUsers(false)
    , m_bRadioDeviceChannel(false)
    , m_bRadioDevice(false)
    , m_bRadioProsaveRecord(false)
    , m_bRadioClientinput(false)
    , m_bRadioAlarmlog(false)
    , m_bRadioSelectAll(false)
    , m_bExistReferData(false)
    , m_bStopHandleSimulate(false)
    , m_bStartSimulateData(false)
    , m_bEndSimulateData(true)
    , m_bDot(false)
    , m_lhandleSimulate(NULL)
    , m_nSelectOneSetNum(_T("1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CDBSimulateToolForTestDlg::~CDBSimulateToolForTestDlg()
{
    try
    {
        m_bStopHandleSimulate = true;
        m_bStartSimulateData = false;
        if(NULL != m_lhandleSimulate)
        {
            ::CloseHandle(m_lhandleSimulate);
            m_lhandleSimulate = NULL;
        }
        if (!vecRegionInfo.empty())
        {
            vecRegionInfo.clear();
        }
        if (!vecProductInfo.empty())
        {
            vecProductInfo.clear();
        }
        if (!vecUsersInfo.empty())
        {
            vecUsersInfo.clear();
        }
        if (!vecDevicechannelInfo.empty())
        {
            vecDevicechannelInfo.clear();
        }
        if (!vecDeviceInfo.empty())
        {
            vecDeviceInfo.clear();
        }
        if (!vecProsaverecordInfo.empty())
        {
            vecProsaverecordInfo.clear();
        }
        if (!vecClientinputInfo.empty())
        {
            vecClientinputInfo.clear();
        }
        if (!vecAlarmlogInfo.empty())
        {
            vecAlarmlogInfo.clear();
        }
        m_mySQLRef.CloseConnect();
    }
    catch (...)
    {

    }
}
void CDBSimulateToolForTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS_HOST, m_ctrlDBIp);
    DDX_Control(pDX, IDC_EDIT_USER_PORT, m_editDBPort);
    DDX_Control(pDX, IDC_EDIT_USER_NAME, m_editDBUserName);
    DDX_Control(pDX, IDC_EDIT_USER_PSW, m_editDBPassWord);
    DDX_Control(pDX, IDC_EDIT_DB_NAME, m_editDBSchema);
    DDX_Control(pDX, IDC_IPADDRESS_HOST_REF, m_ctrlDBIpRef);
    DDX_Control(pDX, IDC_EDIT_USER_PORT_REF, m_editDBPortRef);
    DDX_Control(pDX, IDC_EDIT_USER_NAME_REF, m_editDBUserNameRef);
    DDX_Control(pDX, IDC_EDIT_USER_PSW_REF, m_editDBPassWordRef);
    DDX_Control(pDX, IDC_EDIT_DB_NAME_REF, m_editDBSchemaRef);
    DDX_Text(pDX, IDC_EDIT_SELECT_ONE, m_nSelectOneSetNum);
}

BEGIN_MESSAGE_MAP(CDBSimulateToolForTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CDBSimulateToolForTestDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_RADIO_REGION, &CDBSimulateToolForTestDlg::OnBnClickedRadioRegion)
    ON_BN_CLICKED(IDC_RADIO_PRODUCT, &CDBSimulateToolForTestDlg::OnBnClickedRadioProduct)
    ON_BN_CLICKED(IDC_RADIO_USER, &CDBSimulateToolForTestDlg::OnBnClickedRadioUser)
    ON_BN_CLICKED(IDC_RADIO_DEVICE_CHANNEL, &CDBSimulateToolForTestDlg::OnBnClickedRadioDeviceChannel)
    ON_BN_CLICKED(IDC_RADIO_DEVICE, &CDBSimulateToolForTestDlg::OnBnClickedRadioDevice)
    ON_BN_CLICKED(IDC_RADIO_RECORD, &CDBSimulateToolForTestDlg::OnBnClickedRadioRecord)
    ON_BN_CLICKED(IDC_RADIO_CLIENTINPUT, &CDBSimulateToolForTestDlg::OnBnClickedRadioClientinput)
    ON_BN_CLICKED(IDC_RADIO_ALARMLOG, &CDBSimulateToolForTestDlg::OnBnClickedRadioAlarmlog)
    ON_BN_CLICKED(IDC_RADIO_SELECT_ALL, &CDBSimulateToolForTestDlg::OnBnClickedRadioSelectAll)
    ON_BN_CLICKED(IDC_BUTTON_DB_CHECK, &CDBSimulateToolForTestDlg::OnBnClickedButtonDbCheck)
    ON_BN_CLICKED(IDC_BUTTON_EXPAND, &CDBSimulateToolForTestDlg::OnBnClickedButtonExpand)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN_REF, &CDBSimulateToolForTestDlg::OnBnClickedButtonLoginRef)
    ON_BN_CLICKED(IDC_BUTTON_EX_IN_PORT, &CDBSimulateToolForTestDlg::OnBnClickedButtonImPort)
    ON_BN_CLICKED(IDC_BUTTON_SIMULATE_DB, &CDBSimulateToolForTestDlg::OnBnClickedButtonSimulateDBData)
    ON_WM_TIMER()
END_MESSAGE_MAP()
//检测输入模拟条数是否合法
bool CDBSimulateToolForTestDlg::isNum(std::string &str)
{
    std::stringstream sin(str);
    double d;
    char c;
    if(!(sin >> d))
        return false;
    if (sin >> c)
        return false;
    return true;
}
// CDBSimulateToolForTestDlg 消息处理程序
unsigned int _stdcall StartSimulateData(LPVOID lpParam)
{
    CDBSimulateToolForTestDlg* pThis = static_cast<CDBSimulateToolForTestDlg*>(lpParam);
    if (NULL == pThis)
    {
        return 1;
    }
    pThis ->StartWork();
    return 0;
}
void CDBSimulateToolForTestDlg::StartWork()
{
    USES_CONVERSION;
    UpdateData(TRUE);
    while (!m_bStopHandleSimulate)
    {
        time_t tTstart = time(NULL);
        int nNumberTotal = 0;
        if (m_bStartSimulateData)
        {
            GetDlgItemText(IDC_EDIT_SELECT_ONE,m_nSelectOneSetNum);
            std::string strSelectOneSetNum = W2A(m_nSelectOneSetNum);
            if (!isNum(strSelectOneSetNum) || _ttoi(m_nSelectOneSetNum) < 1 || _ttoi(m_nSelectOneSetNum) > 10000)
            {
                KillTimer(TIMER);
                GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                if (_ttoi(m_nSelectOneSetNum) > 10000)
                {
                    MessageBox(_T("设置模拟条数不支持大于10000(K)!"));
                }
                else
                {
                    MessageBox(_T("输入模拟条数-参数不合法!"));
                }
            }
            else
            {
                std::string strDesc = "";
                //
                if (m_bRadioRegion)
                {
                    if (/*m_mySQL.GetRegionNumber() < 1000 && */m_mySQL.GetRegionNumber() > 0)
                    {
                        vecRegionInfo.clear();
                        m_mySQL.GetRegionInfo(vecRegionInfo);
                        //全选:默认参数 单选：设置参数
                        int nRegionNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nRegionNumber = 1000;
                        }
                        else
                        {
                            nRegionNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateRegion = m_mySQL.StartSimulateRegion(vecRegionInfo,nRegionNumber);
                        if (!bSimulateRegion)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateRegion模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "Region,";
                            nNumberTotal += nRegionNumber;
                            //MessageBox(_T("模拟插入数据成功!"));
                        }
                    }
                    //else if(m_mySQL.GetRegionNumber() >= 1000)
                    //{
                    //    GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                    //    MessageBox(_T("当前模拟Region已有参考数据基数大于1000,不允许再模拟数据,请清空表重新模拟!"));
                    //}
                    else
                    {
                        MessageBox(_T("当前Region没有参考模拟数据!"));
                    }
                }
                //
                if (m_bRadioDevice)
                {
                    if (/*m_mySQL.GetDeviceNumber() < 1000 &&*/ m_mySQL.GetDeviceNumber() > 0)
                    {
                        vecDeviceInfo.clear();
                        m_mySQL.GetDeviceInfo(vecDeviceInfo);
                        //
                        int nDeviceNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nDeviceNumber = 1000;
                        }
                        else
                        {
                            nDeviceNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateDevice = m_mySQL.StartSimulateDevice(vecDeviceInfo,nDeviceNumber);
                        if (!bSimulateDevice)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateDevice模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "Device,";
                            nNumberTotal += nDeviceNumber;
                        }
                    }
                    //else if(m_mySQL.GetDeviceNumber() >= 1000)
                    //{
                    //    GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                    //    MessageBox(_T("当前模拟Device参考数据基数大于1000,不允许再模拟数据,请清空表重新模拟!"));
                    //}
                    else
                    {
                        MessageBox(_T("当前Device没有参考模拟数据!"));
                    }
                }
                //
                if (m_bRadioDeviceChannel)
                {
                    if (/*m_mySQL.GetDeviceChannelNumber() < 1000 &&*/ m_mySQL.GetDeviceChannelNumber() > 0)
                    {
                        vecDevicechannelInfo.clear();
                        m_mySQL.GetDevicechannelInfo(vecDevicechannelInfo);
                        //
                        int nDevicechannelNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nDevicechannelNumber = 10000;
                        }
                        else
                        {
                            nDevicechannelNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateDevicechannel = m_mySQL.StartSimulateDevicechannel(vecDevicechannelInfo,nDevicechannelNumber);
                        if (!bSimulateDevicechannel)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("DeviceChannel模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "DeviceChannel,";
                            nNumberTotal += nDevicechannelNumber;
                        }

                    }
                    /*else if(m_mySQL.GetDeviceChannelNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟DeviceChannel已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前DeviceChannel没有模拟参考数据!"));
                    }
                }
                //
                if (m_bRadioUsers)
                {
                    if (/*m_mySQL.GetUsersNumber() < 1000 &&*/ m_mySQL.GetUsersNumber() > 0)
                    {
                        vecUsersInfo.clear();
                        m_mySQL.GetUsersInfo(vecUsersInfo);
                        //
                        int nUsersNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nUsersNumber = 10000;
                        }
                        else
                        {
                            nUsersNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateUsers = m_mySQL.StartSimulateUsers(vecUsersInfo,nUsersNumber);
                        if (!bSimulateUsers)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateUsers模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "Users,";
                            nNumberTotal += nUsersNumber;
                        }
                    }
                    /*else if(m_mySQL.GetUsersNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟Users已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前Users没有模拟参考数据!"));
                    }
                }
                //
                if (m_bRadioProduct)
                {
                    if (/*m_mySQL.GetProductNumber() < 1000 &&*/ m_mySQL.GetProductNumber() > 0)
                    {
                        vecProductInfo.clear();
                        m_mySQL.GetProductInfo(vecProductInfo);
                        //
                        int nProductNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nProductNumber = 10000;
                        }
                        else
                        {
                            nProductNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateProduct = m_mySQL.StartSimulateProduct(vecProductInfo,nProductNumber);
                        if (!bSimulateProduct)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateProduct模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "Product,";
                            nNumberTotal += nProductNumber;
                        }
                    }
                    /*else if(m_mySQL.GetProductNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟Product已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前Product没有模拟参考数据!"));
                    }
                }
                //
                if (m_bRadioClientinput)
                {
                    if (/*m_mySQL.GetClientinputNumber() < 1000 &&*/ m_mySQL.GetClientinputNumber() > 0)
                    {
                        vecClientinputInfo.clear();
                        m_mySQL.GetClientinputInfo(vecClientinputInfo);
                        //
                        int nClientinputNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nClientinputNumber = 10000 * 300;
                        }
                        else
                        {
                            nClientinputNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateClientinput = m_mySQL.StartSimulateClientinput(vecClientinputInfo,nClientinputNumber);
                        if (!bSimulateClientinput)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateClientinput模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "ClientInput,";
                            nNumberTotal += nClientinputNumber;
                        }
                    }
                    /*else if(m_mySQL.GetClientinputNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟Clientinput已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前ClientInput没有模拟参考数据!"));
                    }
                }
                //
                if (m_bRadioAlarmlog)
                {
                    if (/*m_mySQL.GetAlarmlogNumber() < 1000 &&*/ m_mySQL.GetAlarmlogNumber() > 0)
                    {
                        vecAlarmlogInfo.clear();
                        m_mySQL.GetAlarmlogInfo(vecAlarmlogInfo);
                        //
                        int nAlarmlogNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nAlarmlogNumber = 10000 * 500;
                        }
                        else
                        {
                            nAlarmlogNumber = _ttoi(m_nSelectOneSetNum) * 1000;
                        }
                        bool bSimulateAlarmlog = m_mySQL.StartSimulateAlarmlog(vecAlarmlogInfo,nAlarmlogNumber);
                        if (!bSimulateAlarmlog)
                        {
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateAlarmlog模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "AlarmLog,";
                            nNumberTotal += nAlarmlogNumber;
                        }
                    }
                    /*else if(m_mySQL.GetAlarmlogNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟Alarmlog已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前Alarmlog没有模拟参考数据!"));
                    }
                }
                //
                if (m_bRadioProsaveRecord)
                {
                    if (/*m_mySQL.GetProsaveRecordNumber() < 1000 && */m_mySQL.GetProsaveRecordNumber() > 0)
                    {
                        vecProsaverecordInfo.clear();
                        m_mySQL.GetProsaverecordInfo(vecProsaverecordInfo);
                        //
                        int nProsaveRecordNumber = 0;
                        if (m_bRadioSelectAll)
                        {
                            nProsaveRecordNumber = 10000 * 300;
                        }
                        else
                        {
                            int n = _ttoi(m_nSelectOneSetNum);
                            TPLOG_INFO("n = %d m_nSelectOneSetNum:%s",n,fcT2A(m_nSelectOneSetNum));
                            nProsaveRecordNumber = n * 1000;
                            TPLOG_INFO("nProsaveRecordNumber = %d",nProsaveRecordNumber);
                        }
                        //m_mySQL.BeginTransaction();
                        bool bSimulateProsaveRecord = m_mySQL.StartSimulateProsaveRecord(vecProsaverecordInfo,nProsaveRecordNumber);
                        if (!bSimulateProsaveRecord)
                        {
                            //m_mySQL.RollBackTransaction();
                            GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                            MessageBox(_T("StartSimulateProsaveRecord模拟插入数据失败!"));
                        }
                        else
                        {
                            strDesc += "ProsaveRecord,";
                            nNumberTotal += nProsaveRecordNumber;
                        }
                        //m_mySQL.CommitTransaction();

                    }
                    /*else if(m_mySQL.GetProsaveRecordNumber() >= 1000)
                    {
                        GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                        MessageBox(_T("当前模拟ProsaveRecord已有参考数据基数大于1000,不允许再模拟数据,请清理表部分参考数据重新模拟!"));
                    }*/
                    else
                    {
                        MessageBox(_T("当前ProsaveRecord没有模拟参考数据!"));
                    }
                }
                KillTimer(TIMER);
                time_t tTstop = time(NULL);
                TPLOG_INFO("模拟结束了,导入%d条数据共耗时%d秒!",nNumberTotal,tTstop - tTstart);
                GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
                GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
                //
                CString MsgSimulate;
                MsgSimulate.Format(_T("所选模拟数据库表中，(%s)表(()表示没有表模拟成功)模拟数据成功!"),fcA2T(strDesc.c_str()));
                MessageBox(MsgSimulate);
            }
            m_bStartSimulateData = false;
            m_bEndSimulateData = true;
        }
    }
}
BOOL CDBSimulateToolForTestDlg::OnInitDialog()
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
    SetWindowText(_T("性能测试数据模拟工具"));
    BuildDefaultFile();
    InitConfig();

    GetDlgItem(IDC_BUTTON_EXPAND)->EnableWindow(FALSE);
    GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
    OnBnClickedButtonExpand();

    //创建模拟数据线程
    if (NULL == m_lhandleSimulate)
    {
        m_lhandleSimulate = (HANDLE)_beginthreadex(NULL,0,StartSimulateData,(LPVOID)this,0,NULL);
        if(NULL == m_lhandleSimulate)
        {
            CString m_csErr;
            m_csErr.Format(_T("创建模拟数据线程失败,错误码:%d"),GetLastError());
            MessageBox(m_csErr);
        }
    }
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDBSimulateToolForTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDBSimulateToolForTestDlg::OnPaint()
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
HCURSOR CDBSimulateToolForTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//////////////////////////////////////////////////////////////////////////
void CDBSimulateToolForTestDlg::BuildDefaultFile()
{
    CConfigParser myConfig(CONFIG_FILE);
    assert(!myConfig.IsExist());
    if (myConfig.IsExist())
    {
        return;
    }
    myConfig.SetSection(INI_SEC_AUTH);
    myConfig.SetStringValue(KEY_DB_IP, DEFAULT_DB_IP);
    myConfig.SetStringValue(KEY_DB_PORT, DEFAULT_DB_PORT);
    myConfig.SetStringValue(KEY_DB_USERNAME, DEFAULT_DB_USERNAME);
    myConfig.SetStringValue(KEY_DB_PASSWORD, DEFAULT_DB_PASSWORD);
    myConfig.SetStringValue(KEY_DB_SCHEMA, DEFAULT_DB_SCHEMA);

    myConfig.SetStringValue(KEY_DB_IP_REF, DEFAULT_DB_IP_REF);
    myConfig.SetStringValue(KEY_DB_PORT_REF, DEFAULT_DB_PORT_REF);
    myConfig.SetStringValue(KEY_DB_USERNAME_REF, DEFAULT_DB_USERNAME_REF);
    myConfig.SetStringValue(KEY_DB_PASSWORD_REF, DEFAULT_DB_PASSWORD_REF);
    myConfig.SetStringValue(KEY_DB_SCHEMA_REF, DEFAULT_DB_SCHEMA_REF);

}
void CDBSimulateToolForTestDlg::InitConfig()
{
    std::_tstring m_appDir;
    std::_tstring m_strCfgFile;
    //
    char szAppPath[MAX_PATH] = {0};
    if (HPR_OK != HPR_GetCurExePath(szAppPath, _countof(szAppPath)))
    {
        TPLOG_ERROR("get app path failed(%d)", HPR_GetLastError());
        return ;
    }
    char *pEnd = NULL;
    if ((pEnd = ::strrchr(szAppPath, '\\')) != NULL
        || (pEnd = ::strrchr(szAppPath, '/')) != NULL)
    {
        if (pEnd - szAppPath + 1 < (int)(_countof(szAppPath)))
        {
            *(pEnd + 1) = 0;
        }
        else
        {
            return;
        }
    }
    else
    {
        TPLOG_ERROR("failed to get splash for: %s", szAppPath);
        return;
    }
    TPLOG_INFO("current dir is : %s", szAppPath);
    if (FALSE == ::SetCurrentDirectoryA(szAppPath))
    {
        TPLOG_ERROR("set current dir failed(%d)", ::GetLastError());
        return;
    }
    m_appDir = fcA2T(szAppPath);

    //本地配置文件路径
    m_strCfgFile = m_appDir + my::INI_CFG_FILE_NAME;

    CConfigParser myini(m_strCfgFile.c_str());
    myini.SetSection(my::INI_SEC_DB_PARAM);
    m_strDBIP = myini.GetStringValue(my::INI_SEC_DB_IP, my::DEFAULT_SEC_DB_IP);
    m_strDBPort = myini.GetStringValue(my::INI_SEC_DB_PORT, my::DEFAULT_SEC_DB_PORT);
    m_strDBUser = myini.GetStringValue(my::INI_SEC_DB_USERNAME, my::DEFAULT_SEC_DB_USERNAME);
    m_strDBPassword = myini.GetStringValue(my::INI_SEC_DB_PASSWORD, my::DEFAULT_SEC_DB_PASSWORD);
    m_strDBSchema = myini.GetStringValue(my::INI_SEC_DB_SCHEMA, my::DEFAULT_SEC_DB_SCHEMA);

    m_strDBIPRef = myini.GetStringValue(my::INI_SEC_DB_IP_REF, my::DEFAULT_SEC_DB_IP_REF);
    m_strDBPortRef = myini.GetStringValue(my::INI_SEC_DB_PORT_REF, my::DEFAULT_SEC_DB_PORT_REF);
    m_strDBUserRef = myini.GetStringValue(my::INI_SEC_DB_USERNAME_REF, my::DEFAULT_SEC_DB_USERNAME_REF);
    m_strDBPasswordRef = myini.GetStringValue(my::INI_SEC_DB_PASSWORD_REF, my::DEFAULT_SEC_DB_PASSWORD_REF);
    m_strDBSchemaRef = myini.GetStringValue(my::INI_SEC_DB_SCHEMA_REF, my::DEFAULT_SEC_DB_SCHEMA_REF);
    //
    m_ctrlDBIp.SetWindowText(m_strDBIP);
    m_editDBPort.SetWindowText(m_strDBPort);
    m_editDBUserName.SetWindowText(m_strDBUser);
    m_editDBPassWord.SetWindowText(m_strDBPassword);
    m_editDBSchema.SetWindowText(m_strDBSchema);

    m_ctrlDBIpRef.SetWindowText(m_strDBIPRef);
    m_editDBPortRef.SetWindowText(m_strDBPortRef);
    m_editDBUserNameRef.SetWindowText(m_strDBUserRef);
    m_editDBPassWordRef.SetWindowText(m_strDBPasswordRef);
    m_editDBSchemaRef.SetWindowText(m_strDBSchemaRef);
    UpdateData();
}
//参数保存
void CDBSimulateToolForTestDlg::SaveParamLogin()
{
    //检查保存信息合法性-IP
    CString sDBIP = _T("");
    m_ctrlDBIp.GetWindowText(sDBIP);
    sDBIP.Trim();
    if (sDBIP.IsEmpty())
    {
        MessageBox(_T("设备IP不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ctrlDBIp.SetFocus();
        return;
    }
    CString sDBIPRef = _T("");
    m_ctrlDBIpRef.GetWindowText(sDBIPRef);
    sDBIPRef.Trim();
    if (sDBIPRef.IsEmpty())
    {
        MessageBox(_T("设备IP不能为空!"),_T("警告"),MB_ICONWARNING|MB_OK);
        m_ctrlDBIpRef.SetFocus();
        return;
    }
    //获取参数保存
    CString sDBPort = _T("");
    CString sDBUserName = _T("");
    CString sDBPassWord = _T("");
    CString sDBSchema = _T("");
    m_editDBPort.GetWindowText(sDBPort);
    sDBPort.Trim();
    m_editDBUserName.GetWindowText(sDBUserName);
    sDBUserName.Trim();
    m_editDBPassWord.GetWindowText(sDBPassWord);
    sDBPassWord.Trim();
    m_editDBSchema.GetWindowText(sDBSchema);
    sDBSchema.Trim();
    CString sDBPortRef = _T("");
    CString sDBUserNameRef = _T("");
    CString sDBPassWordRef = _T("");
    CString sDBSchemaRef = _T("");
    m_editDBPortRef.GetWindowText(sDBPortRef);
    sDBPortRef.Trim();
    m_editDBUserNameRef.GetWindowText(sDBUserNameRef);
    sDBUserNameRef.Trim();
    m_editDBPassWordRef.GetWindowText(sDBPassWordRef);
    sDBPassWordRef.Trim();
    m_editDBSchemaRef.GetWindowText(sDBSchemaRef);
    sDBSchemaRef.Trim();

    CConfigParser myConfig(CONFIG_FILE);
    myConfig.SetSection(INI_SEC_AUTH);
    myConfig.SetStringValue(KEY_DB_IP, sDBIP);
    myConfig.SetStringValue(KEY_DB_PORT, sDBPort);
    myConfig.SetStringValue(KEY_DB_USERNAME, sDBUserName);
    myConfig.SetStringValue(KEY_DB_PASSWORD, sDBPassWord);
    myConfig.SetStringValue(KEY_DB_SCHEMA, sDBSchema);

    myConfig.SetStringValue(KEY_DB_IP_REF, sDBIPRef);
    myConfig.SetStringValue(KEY_DB_PORT_REF, sDBPortRef);
    myConfig.SetStringValue(KEY_DB_USERNAME_REF, sDBUserNameRef);
    myConfig.SetStringValue(KEY_DB_PASSWORD_REF, sDBPassWordRef);
    myConfig.SetStringValue(KEY_DB_SCHEMA_REF, sDBSchemaRef);

}
//////////////////////////////////////////////////////////////////////////
void CDBSimulateToolForTestDlg::OnBnClickedButtonLogin()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (!m_bLogin)
    {
        CString sDBIP = _T("");
        CString sDBPort = _T("");
        CString sDBUserName = _T("");
        CString sDBPassWord = _T("");
        CString sDBSchema = _T("");
        m_ctrlDBIp.GetWindowText(sDBIP);
        sDBIP.Trim();
        m_editDBPort.GetWindowText(sDBPort);
        sDBPort.Trim();
        m_editDBUserName.GetWindowText(sDBUserName);
        sDBUserName.Trim();
        m_editDBPassWord.GetWindowText(sDBPassWord);
        sDBPassWord.Trim();
        m_editDBSchema.GetWindowText(sDBSchema);
        sDBSchema.Trim();
        db_conn_info_t connInfo;
        connInfo.host = sDBIP.GetBuffer();
        connInfo.port = sDBPort.GetBuffer();
        connInfo.username = sDBUserName.GetBuffer();
        connInfo.password = sDBPassWord.GetBuffer();
        connInfo.dbschema = sDBSchema.GetBuffer();
        //
        if(!m_mySQL.GetConnect(connInfo))
        {
            AfxMessageBox(_T("登录数据库失败！请检查数据库连接！\n"));
            return;
        }
        SaveParamLogin();
        m_bLogin = true;
        GetDlgItem(IDC_IPADDRESS_HOST)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_PORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_PSW)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_DB_NAME)->EnableWindow(FALSE);

        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("数据库登出"));
    }
    else
    {
        if(!m_mySQL.CloseConnect())
        {
            AfxMessageBox(_T("登出数据库失败！\n"));
            return;
        }

        SaveParamLogin();
        m_bLogin = false;
        GetDlgItem(IDC_IPADDRESS_HOST)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_PORT)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_PSW)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_DB_NAME)->EnableWindow(TRUE);
        //
        ((CButton*)GetDlgItem(IDC_RADIO_SELECT_ALL))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_REGION))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_PRODUCT))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_USER))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE_CHANNEL))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_RECORD))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_CLIENTINPUT))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_ALARMLOG))->SetCheck(FALSE);

        m_bRadioSelectAll = false;
        m_bRadioRegion = false;
        m_bRadioProduct = false;
        m_bRadioUsers = false;
        m_bRadioDeviceChannel = false;
        m_bRadioDevice = false;
        m_bRadioProsaveRecord = false;
        m_bRadioClientinput = false;
        m_bRadioAlarmlog = false;
        GetDlgItem(IDC_BUTTON_EXPAND)->EnableWindow(FALSE);
        //
        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("数据库登录"));
    }
}




void CDBSimulateToolForTestDlg::OnBnClickedRadioRegion()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioRegion)
    {
        m_bRadioRegion = true;
        ((CButton*)GetDlgItem(IDC_RADIO_REGION))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioRegion = false;
        ((CButton*)GetDlgItem(IDC_RADIO_REGION))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioProduct()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioProduct)
    {
        m_bRadioProduct = true;
        ((CButton*)GetDlgItem(IDC_RADIO_PRODUCT))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioProduct = false;
        ((CButton*)GetDlgItem(IDC_RADIO_PRODUCT))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioUser()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioUsers)
    {
        m_bRadioUsers = true;
        ((CButton*)GetDlgItem(IDC_RADIO_USER))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioUsers = false;
        ((CButton*)GetDlgItem(IDC_RADIO_USER))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioDeviceChannel()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioDeviceChannel)
    {
        m_bRadioDeviceChannel = true;
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE_CHANNEL))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioDeviceChannel = false;
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE_CHANNEL))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioDevice()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioDevice)
    {
        m_bRadioDevice = true;
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioDevice = false;
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioRecord()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioProsaveRecord)
    {
        m_bRadioProsaveRecord = true;
        ((CButton*)GetDlgItem(IDC_RADIO_RECORD))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioProsaveRecord = false;
        ((CButton*)GetDlgItem(IDC_RADIO_RECORD))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioClientinput()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioClientinput)
    {
        m_bRadioClientinput = true;
        ((CButton*)GetDlgItem(IDC_RADIO_CLIENTINPUT))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioClientinput = false;
        ((CButton*)GetDlgItem(IDC_RADIO_CLIENTINPUT))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioAlarmlog()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioAlarmlog)
    {
        m_bRadioAlarmlog = true;
        ((CButton*)GetDlgItem(IDC_RADIO_ALARMLOG))->SetCheck(TRUE);
    }
    else
    {
        m_bRadioAlarmlog = false;
        ((CButton*)GetDlgItem(IDC_RADIO_ALARMLOG))->SetCheck(FALSE);
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedRadioSelectAll()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bRadioSelectAll)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_SELECT_ALL))->SetWindowText(_T("取消全选"));
        ((CButton*)GetDlgItem(IDC_RADIO_REGION))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_PRODUCT))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_USER))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE_CHANNEL))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_RECORD))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_CLIENTINPUT))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_ALARMLOG))->SetCheck(TRUE);
        GetDlgItem(IDC_EDIT_SELECT_ONE)->EnableWindow(FALSE);

        m_bRadioSelectAll = true;
        m_bRadioRegion = true;
        m_bRadioProduct = true;
        m_bRadioUsers = true;
        m_bRadioDeviceChannel = true;
        m_bRadioDevice = true;
        m_bRadioProsaveRecord = true;
        m_bRadioClientinput = true;
        m_bRadioAlarmlog = true;
    }
    else
    {
        if (m_bStartSimulateData)
        {
            MessageBox(_T("正在进行模拟数据,请结束后再取消全选"),_T("提示"),MB_ICONWARNING);
            return;
        }
        ((CButton*)GetDlgItem(IDC_RADIO_SELECT_ALL))->SetWindowText(_T("全选"));
        ((CButton*)GetDlgItem(IDC_RADIO_SELECT_ALL))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_REGION))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_PRODUCT))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_USER))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE_CHANNEL))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_DEVICE))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_RECORD))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_CLIENTINPUT))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_ALARMLOG))->SetCheck(FALSE);
        GetDlgItem(IDC_EDIT_SELECT_ONE)->EnableWindow(TRUE);

        m_bRadioSelectAll = false;
        m_bRadioRegion = false;
        m_bRadioProduct = false;
        m_bRadioUsers = false;
        m_bRadioDeviceChannel = false;
        m_bRadioDevice = false;
        m_bRadioProsaveRecord = false;
        m_bRadioClientinput = false;
        m_bRadioAlarmlog = false;
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedButtonDbCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLogin)
    {
        MessageBox(_T("请先连接数据库!"));
        return;
    }
    if (!m_bRadioRegion && !m_bRadioProduct && !m_bRadioUsers && !m_bRadioDeviceChannel 
        && !m_bRadioDevice && !m_bRadioProsaveRecord && !m_bRadioClientinput && !m_bRadioAlarmlog)
    {
        MessageBox(_T("没有选中任何表，请选择需要检查的表!"),_T("提示"),MB_ICONINFORMATION);
        return;
    }
    std::string strDesc = "";
    std::string strDescEx = "";
    bool bException = false;
    if (m_bRadioRegion)
    {
        int nRegionNumber = m_mySQL.GetRegionNumber();
        if (0 == nRegionNumber)
        {
            strDesc += "Region,";
        }
        else if (nRegionNumber < 0)
        {
            strDescEx += "使用Region异常,";
            bException = true;
        }
        else
        {
            if (!vecRegionInfo.empty())
            {
                vecRegionInfo.clear();
            }
            bool bRet = m_mySQL.GetRegionInfo(vecRegionInfo);
            if (!bRet)
            {
                strDescEx += "获取Region异常,";
                bException = true;
            }
        }
    }
    if (m_bRadioProduct)
    {
        int nProductNumber = m_mySQL.GetProductNumber();
        if (0 == nProductNumber)
        {
            strDesc += "Product,";
        }
        else if (nProductNumber < 0)
        {
            strDescEx += "使用Product异常,";
            bException = true;
        }
        else
        {
            if (!vecProductInfo.empty())
            {
                vecProductInfo.clear();
            }
           bool bRet = m_mySQL.GetProductInfo(vecProductInfo);
           if (!bRet)
           {
               strDescEx += "获取Product异常,";
               bException = true;
           }
        }
    }
    if (m_bRadioUsers)
    {
        int nUsersNumber = m_mySQL.GetUsersNumber();
        if (0 == nUsersNumber)
        {
            strDesc += "Users,";
        }
        else if (nUsersNumber < 0)
        {
            strDescEx += "使用Users异常,";
            bException = true;
        }
        else
        {
            if (!vecUsersInfo.empty())
            {
                vecUsersInfo.clear();
            }
            bool bRet = m_mySQL.GetUsersInfo(vecUsersInfo);
            if (!bRet)
            {
                strDescEx += "获取Users异常,";
                bException = true;
            }
        }
    }
    if (m_bRadioDeviceChannel)
    {
        int nDeviceChannelNumber = m_mySQL.GetDeviceChannelNumber();
        if (0 == nDeviceChannelNumber)
        {
            strDesc += "DeviceChannel,";
        }
        else if (nDeviceChannelNumber < 0)
        {
            strDescEx += "使用DeviceChannel异常,";
            bException = true;
        }
        else
        {
            if (!vecDevicechannelInfo.empty())
            {
                vecDevicechannelInfo.clear();
            }
           bool bRet = m_mySQL.GetDevicechannelInfo(vecDevicechannelInfo);
           if (!bRet)
           {
               strDescEx += "获取DeviceChannel异常,";
               bException = true;
           }
        }
    }
    if (m_bRadioDevice)
    {
        int nDeviceNumber = m_mySQL.GetDeviceNumber();
        if (0 == nDeviceNumber)
        {
            strDesc += "Device,";
        }
        else if (nDeviceNumber < 0)
        {
            strDescEx += "使用Device异常,";
            bException = true;
        }
        else
        {
            if (!vecDeviceInfo.empty())
            {
                vecDeviceInfo.clear();
            }
            bool bRet = m_mySQL.GetDeviceInfo(vecDeviceInfo);
            if (!bRet)
            {
                strDescEx += "获取Device异常,";
                bException = true;
            }
        }
    }
    if (m_bRadioProsaveRecord)
    {
        int nProsaveRecordNumber = m_mySQL.GetProsaveRecordNumber();
        if (0 == nProsaveRecordNumber)
        {
            strDesc += "ProsaveRecord,";
        }
        else if (nProsaveRecordNumber < 0)
        {
            strDescEx += "使用ProsaveRecord异常,";
            bException = true;
        }
        else
        {
            if (!vecProsaverecordInfo.empty())
            {
                vecProsaverecordInfo.clear();
            }
           bool bRet = m_mySQL.GetProsaverecordInfo(vecProsaverecordInfo);
           if (!bRet)
           {
               strDescEx += "获取ProsaveRecord异常,";
               bException = true;
           }
        }
    }
    if (m_bRadioClientinput)
    {
        int nClientinputNumber = m_mySQL.GetClientinputNumber();
        if (0 == nClientinputNumber)
        {
            strDesc += "ClientInput,";
        }
        else if (nClientinputNumber < 0)
        {
            strDescEx += "使用ClientInput异常,";
            bException = true;
        }
        else
        {
            if (!vecClientinputInfo.empty())
            {
                vecClientinputInfo.clear();
            }
           bool bRet = m_mySQL.GetClientinputInfo(vecClientinputInfo);
           if (!bRet)
           {
               strDescEx += "获取ClientInput异常,";
               bException = true;
           }
        }
    }
    if (m_bRadioAlarmlog)
    {
        int nAlarmlogNumber = m_mySQL.GetAlarmlogNumber();
        if (0 == nAlarmlogNumber)
        {
            strDesc += "AlarmLog";
        }
        else if (nAlarmlogNumber < 0)
        {
            strDescEx += "使用AlarmLog异常,";
            bException = true;
        }
        else
        {
            if (!vecAlarmlogInfo.empty())
            {
                vecAlarmlogInfo.clear();
            }
           bool bRet = m_mySQL.GetAlarmlogInfo(vecAlarmlogInfo);
           if (!bRet)
           {
               strDescEx += "获取AlarmLog异常,";
               bException = true;
           }
        }
    }
    //
    if (bException)
    {
        CString MsgException;
        MsgException.Format(_T("所选模拟数据库表中，%s,请检查使用的数据库表是否异常!"),fcA2T(strDescEx.c_str()));
        MessageBox(MsgException,_T("提示"),MB_ICONWARNING);
        return;
    }
    if ("" != strDesc)
    {
        CString MsgCheck;
        MsgCheck.Format(_T("所选模拟数据库表中参考数据情况:%s表没有模拟参考数据,需要从参考数据库导出参考数据或者本机中心配置添加参考数据!"),fcA2T(strDesc.c_str()));
        MessageBox(MsgCheck,_T("提示"),MB_ICONWARNING);
        GetDlgItem(IDC_BUTTON_EXPAND)->EnableWindow(TRUE);
    }
    else
    {
        m_bExistReferData = true;
        MessageBox(_T("所选表存在参考数据!"));
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedButtonExpand()
{
    // TODO: 在此添加控件通知处理程序代码
    CString str;
    if (GetDlgItemText(IDC_BUTTON_EXPAND,str),str == _T("导参考数据(收缩)<<"))
    {
        SetDlgItemText(IDC_BUTTON_EXPAND,_T("导参考数据(扩展)>>"));
    }
    else
    {
        SetDlgItemText(IDC_BUTTON_EXPAND,_T("导参考数据(收缩)<<"));
    }
    static CRect rectLarge;
    static CRect rectSmall;
    if (rectLarge.IsRectNull())
    {
        CRect rectSeparator;
        GetWindowRect(&rectLarge);
        GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);
        rectSmall.left = rectLarge.left;
        rectSmall.top = rectLarge.top;
        rectSmall.right = rectLarge.right;
        rectSmall.bottom = rectSeparator.bottom;
    }
    if (str == _T("导参考数据(收缩)<<"))
    {
        SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE
            | SWP_NOZORDER);
    }
    else
    {
        SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),SWP_NOMOVE
            | SWP_NOZORDER);
    }
}
//////////////////////////////////////////////////////////////////////////
void CDBSimulateToolForTestDlg::OnBnClickedButtonLoginRef()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (!m_bLoginRef)
    {
        CString sDBIPRef = _T("");
        CString sDBPortRef = _T("");
        CString sDBUserNameRef = _T("");
        CString sDBPassWordRef = _T("");
        CString sDBSchemaRef = _T("");
        m_ctrlDBIpRef.GetWindowText(sDBIPRef);
        sDBIPRef.Trim();
        m_editDBPortRef.GetWindowText(sDBPortRef);
        sDBPortRef.Trim();
        m_editDBUserNameRef.GetWindowText(sDBUserNameRef);
        sDBUserNameRef.Trim();
        m_editDBPassWordRef.GetWindowText(sDBPassWordRef);
        sDBPassWordRef.Trim();
        m_editDBSchemaRef.GetWindowText(sDBSchemaRef);
        sDBSchemaRef.Trim();
        db_conn_info_t connInfo;
        connInfo.host = sDBIPRef.GetBuffer();
        connInfo.port = sDBPortRef.GetBuffer();
        connInfo.username = sDBUserNameRef.GetBuffer();
        connInfo.password = sDBPassWordRef.GetBuffer();
        connInfo.dbschema = sDBSchemaRef.GetBuffer();
        //
        if(!m_mySQLRef.GetConnect(connInfo))
        {
            AfxMessageBox(_T("登录参考数据库失败！请检查数据库连接！\n"));
            return;
        }
        SaveParamLogin();
        m_bLoginRef = true;
        GetDlgItem(IDC_IPADDRESS_HOST_REF)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_PORT_REF)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_NAME_REF)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_PSW_REF)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_DB_NAME_REF)->EnableWindow(FALSE);

        GetDlgItem(IDC_BUTTON_LOGIN_REF)->SetWindowText(_T("参考数据库登出"));
    }
    else
    {
        if(!m_mySQLRef.CloseConnect())
        {
            AfxMessageBox(_T("登出数据库失败！\n"));
            return;
        }

        SaveParamLogin();
        m_bLoginRef = false;
        GetDlgItem(IDC_IPADDRESS_HOST_REF)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_PORT_REF)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_NAME_REF)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_PSW_REF)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_DB_NAME_REF)->EnableWindow(TRUE);

        GetDlgItem(IDC_BUTTON_LOGIN_REF)->SetWindowText(_T("参考数据库登录"));
    }
}

void CDBSimulateToolForTestDlg::OnBnClickedButtonImPort()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLoginRef)
    {
        MessageBox(_T("请先登录参考数据库!"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if (!m_bLogin)
    {
        MessageBox(_T("请先登录模拟数据库!"),_T("提示"),MB_ICONWARNING);
        return;
    }
    if (m_bRadioRegion)
    {
        if (m_mySQL.GetRegionNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Region已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetRegionNumber() > 0 && m_mySQL.GetRegionNumber() < 200)
        {
            int result = MessageBox(TEXT("Region表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadRegion(vecRegionInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetRegionNumber())
        {
            if (!vecRegionInfo.empty())
            {
                vecRegionInfo.clear();
            }
            LoadRegion(vecRegionInfo);
        }
    }
    //
    if (m_bRadioProduct)
    {
        if (m_mySQL.GetProductNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Product已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetProductNumber() > 0 && m_mySQL.GetProductNumber() < 200)
        {
            int result = MessageBox(TEXT("Product表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadProduct(vecProductInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetProductNumber())
        {
            if (!vecProductInfo.empty())
            {
                vecProductInfo.clear();
            }
            LoadProduct(vecProductInfo);
        }
    }
    //
    if (m_bRadioUsers)
    {
        if (m_mySQL.GetUsersNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Users已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetUsersNumber() > 0 && m_mySQL.GetUsersNumber() < 200)
        {
            int result = MessageBox(TEXT("Users表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadUsers(vecUsersInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetUsersNumber())
        {
            if (!vecUsersInfo.empty())
            {
                vecUsersInfo.clear();
            }
            LoadUsers(vecUsersInfo);
        }
    }

    if (m_bRadioDeviceChannel)
    {
        if (m_mySQL.GetDeviceChannelNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表DeviceChannel已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetDeviceChannelNumber() > 0 && m_mySQL.GetDeviceChannelNumber() < 200)
        {
            int result = MessageBox(TEXT("DeviceChannel表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadDevicechannel(vecDevicechannelInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetDeviceChannelNumber())
        {
            if (!vecDevicechannelInfo.empty())
            {
                vecDevicechannelInfo.clear();
            }
            LoadDevicechannel(vecDevicechannelInfo);
        }
    }
    if (m_bRadioDevice)
    {
        if (m_mySQL.GetDeviceNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Device已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetDeviceNumber() > 0 && m_mySQL.GetDeviceNumber() < 0)
        {
            int result = MessageBox(TEXT("Device表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadDevice(vecDeviceInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetDeviceNumber())
        {
            if (!vecDeviceInfo.empty())
            {
                vecDeviceInfo.clear();
            }
            LoadDevice(vecDeviceInfo);
        }
    }
    //
    if (m_bRadioProsaveRecord)
    {
        if (m_mySQL.GetProsaveRecordNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表ProsaveRecord已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetProsaveRecordNumber() > 0 && m_mySQL.GetProsaveRecordNumber() < 200)
        {
            int result = MessageBox(TEXT("ProsaveRecord表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadProsaverecord(vecProsaverecordInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetProsaveRecordNumber())
        {
            if (!vecProsaverecordInfo.empty())
            {
                vecProsaverecordInfo.clear();
            }
            LoadProsaverecord(vecProsaverecordInfo);
        }
    }
    //
    if (m_bRadioClientinput)
    {
        if (m_mySQL.GetClientinputNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Clientinput已有的参考数据大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetClientinputNumber() > 0 && m_mySQL.GetProsaveRecordNumber() < 200)
        {
            int result = MessageBox(TEXT("Clientinput表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadClientinput(vecClientinputInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetClientinputNumber())
        {
            if (!vecClientinputInfo.empty())
            {
                vecClientinputInfo.clear();
            }
            LoadClientinput(vecClientinputInfo);
        }
    }
    //
    if (m_bRadioAlarmlog)
    {
        if (m_mySQL.GetAlarmlogNumber() >= 200)
        {
            MessageBox(_T("目前模拟数据库表Alarmlog已有的参考数据已大于200条,不能再导入模拟数据!"));
        }
        else if (m_mySQL.GetAlarmlogNumber() > 0 && m_mySQL.GetAlarmlogNumber() < 200)
        {
            int result = MessageBox(TEXT("Alarmlog表已有数据仍导入参考数据吗?"),TEXT("提示"),MB_ICONINFORMATION|MB_YESNO);
            switch(result)
            {
            case IDYES:
                LoadAlarmlog(vecAlarmlogInfo);
                break;
            case IDNO:
                break;
            default:
                break;
            }
        }
        else if (0 == m_mySQL.GetAlarmlogNumber())
        {
            if (!vecAlarmlogInfo.empty())
            {
                vecAlarmlogInfo.clear();
            }
            LoadAlarmlog(vecAlarmlogInfo);
        }
    }
    //
    if (!m_bRadioRegion && !m_bRadioProduct && !m_bRadioUsers && !m_bRadioDeviceChannel 
        && !m_bRadioDevice && !m_bRadioProsaveRecord && !m_bRadioClientinput && !m_bRadioAlarmlog)
    {
        MessageBox(_T("请选择模拟表!"));
        return;
    }
    m_bExistReferData = true;
    MessageBox(_T("导入工作完成,请退出参考数据库!"));

}

void CDBSimulateToolForTestDlg::OnBnClickedButtonSimulateDBData()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_bLoginRef)
    {
        MessageBox(_T("请退出所连接参考数据库!"));
        return;
    }
    if (!m_bLogin)
    {
        MessageBox(_T("请先登陆模拟数据库!"));
        return;
    }
    if (!m_bExistReferData)
    {
        MessageBox(_T("请先确认所选模拟数据库表是否有参考数据!"));
        return;
    }
    m_bStartSimulateData = false;
    if (!m_bStartSimulateData)
    {
        if (m_bEndSimulateData)
        {
            m_bEndSimulateData = false;
            m_bStartSimulateData = true;
            GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(TRUE);
            GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("正在模拟数据中..."));
            CDBSimulateToolForTestDlg::SetTimer(TIMER,1000, NULL);
        }
        //else
        //{
        //    GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
        //    GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
        //    GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
        //    GetDlgItem(IDC_BUTTON_SIMULATE_DB)->SetWindowText(_T("开始模拟数据"));
        //    m_bStartSimulateData = false;
        //    m_bEndSimulateData = true;
        //}
    }
    //else
    //{
    //    MessageBox(_T("正在进行模拟数据,请在结束后再开始!"));
    //    return;
    //}

}
void CDBSimulateToolForTestDlg::OnTimer(UINT nIDEvent)
{
    UpdateData(TRUE);
    if (m_bStartSimulateData && !m_bEndSimulateData)
    {
        GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(TRUE);
        if (m_bDot)
        {
            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
            m_bDot = false;
        }
        else
        {
            GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(TRUE);
            m_bDot = true;
        }
        GetDlgItem(IDC_STATIC_RUNING)->Invalidate(TRUE);
    }
    else if(!m_bStartSimulateData && m_bEndSimulateData)
    {
        GetDlgItem(IDC_STATIC_DOT1)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_DOT2)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_RUNING)->ShowWindow(FALSE);
    }
    CDialog::OnTimer(nIDEvent);
}
//导数据库
void CDBSimulateToolForTestDlg::LoadRegion(std::vector<region_info_t> &vecRegionInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecRegionInfo.size()) > 0)
    {
        nIndexVecID = vecRegionInfo[nIndexVecID - 1].ID;
        vecRegionInfo.clear();
    }
    bool bRet = m_mySQLRef.GetRegionInfo(vecRegionInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertRegionInfo(vecRegionInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Region表失败!"));
            TPLOG_ERROR("InsertRegionInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadProduct(std::vector<product_info_t> &vecProductInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecProductInfo.size()) > 0)
    {
        nIndexVecID = vecProductInfo[nIndexVecID - 1].ID;
        vecProductInfo.clear();
    }
    bool bRet = m_mySQLRef.GetProductInfo(vecProductInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertProductInfo(vecProductInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Product表失败!"));
            TPLOG_ERROR("InsertProductInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadUsers(std::vector<users_info_t> &vecUsersInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecUsersInfo.size()) > 0)
    {
        nIndexVecID = vecUsersInfo[nIndexVecID - 1].ID;
        vecUsersInfo.clear();
    }
    bool bRet = m_mySQLRef.GetUsersInfo(vecUsersInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertUsersInfo(vecUsersInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Users表失败!"));
            TPLOG_ERROR("InsertUsersInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadDevicechannel(std::vector<devicechannel_info_t> &vecDevicechannelInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecDevicechannelInfo.size()) > 0)
    {
        nIndexVecID = vecDevicechannelInfo[nIndexVecID - 1].ID;
        vecDevicechannelInfo.clear();
    }
    bool bRet = m_mySQLRef.GetDevicechannelInfo(vecDevicechannelInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertDevicechannelInfo(vecDevicechannelInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Devicechannel表失败!"));
            TPLOG_ERROR("InsertDevicechannelInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadDevice(std::vector<device_info_t> &vecDeviceInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecDeviceInfo.size()) > 0)
    {
        nIndexVecID = vecDeviceInfo[nIndexVecID - 1].ID;
        vecDeviceInfo.clear();
    }
    bool bRet = m_mySQLRef.GetDeviceInfo(vecDeviceInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertDeviceInfo(vecDeviceInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Device表失败!"));
            TPLOG_ERROR("InsertDeviceInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadProsaverecord(std::vector<prosaverecord_info_t> &vecProsaverecordInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecProsaverecordInfo.size()) > 0 && (nIndexVecID = vecProsaverecordInfo.size()) < 200)
    {
        nIndexVecID = vecProsaverecordInfo[nIndexVecID - 1].ID;
        vecProsaverecordInfo.clear();
    }
    bool bRet = m_mySQLRef.GetProsaverecordInfo(vecProsaverecordInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertProsaverecordInfo(vecProsaverecordInfo,nIndexVecID);
        if (!bRetInsert)
        {
            TPLOG_ERROR("InsertProsaverecordInfo size=%d，nIndexVecID=%d",vecProsaverecordInfo.size(),nIndexVecID);
            MessageBox(_T("导入Prosaverecord表失败!"));
            TPLOG_ERROR("InsertProsaverecordInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadClientinput(std::vector<clientinput_info_t> &vecClientinputInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecClientinputInfo.size()) > 0)
    {
        nIndexVecID = vecClientinputInfo[nIndexVecID - 1].ID;
        vecClientinputInfo.clear();
    }
    bool bRet = m_mySQLRef.GetClientinputInfo(vecClientinputInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertClientinputInfo(vecClientinputInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Clientinput表失败!"));
            TPLOG_ERROR("InsertClientinputInfo is failed..");
            return;
        }
    }
}
void CDBSimulateToolForTestDlg::LoadAlarmlog(std::vector<alarmlog_info_t> &vecAlarmlogInfo)
{
    int nIndexVecID = 0;
    if ((nIndexVecID = vecAlarmlogInfo.size()) > 0)
    {
        nIndexVecID = vecAlarmlogInfo[nIndexVecID - 1].ID;
        vecAlarmlogInfo.clear();
    }
    bool bRet = m_mySQLRef.GetAlarmlogInfo(vecAlarmlogInfo);
    if (bRet)
    {
        bool bRetInsert = m_mySQL.InsertAlarmlogInfo(vecAlarmlogInfo,nIndexVecID);
        if (!bRetInsert)
        {
            MessageBox(_T("导入Alarmlog表失败!"));
            TPLOG_ERROR("InsertAlarmlogInfo is failed..");
            return;
        }
    }
}