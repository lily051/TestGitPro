
// ArchivePlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ArchivePlayer.h"
#include "ArchivePlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxdb.h>
#include <assert.h>


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//std::vector<BusinessInfo> BusinessInfolist;
std::map<int,BusinessInfo> BusinessInfolist;
std::vector<RecordInfo> RecordInfoList;
std::vector<DevicInfo> g_Devlist;
std::vector<filtrate> g_filter;
std::vector<Iteminfo> Itemlist;


int  CVRcompareTime(const NET_DVR_TIME &src,const NET_DVR_TIME &dst)
{
    CTime time1(src.dwYear,src.dwMonth,src.dwDay,src.dwHour,src.dwMinute,src.dwSecond);
    CTime time2(dst.dwYear,dst.dwMonth,dst.dwDay,dst.dwHour,dst.dwMinute,dst.dwSecond);



    CTimeSpan timeSpan = time2 - time1; 
    LONGLONG nTSeconds = timeSpan.GetTotalSeconds();
    ARCHIVEPLAY_LOG_INFO("one:%d-%d-%d %d:%d:%d  %d", src.dwYear,src.dwMonth,src.dwDay,src.dwHour,src.dwMinute,src.dwSecond,nTSeconds);
    ARCHIVEPLAY_LOG_INFO("two:%d-%d-%d %d:%d:%d  %d", dst.dwYear,dst.dwMonth,dst.dwDay,dst.dwHour,dst.dwMinute,dst.dwSecond,nTSeconds);
    return (int)nTSeconds;
}

int  CloudcompareTime(const HCS_Time &src,const HCS_Time &dst)
{
    CTime time1(src.year,src.month,src.day,src.hour,src.minute,src.second);
    CTime time2(dst.year,dst.month,dst.day,dst.hour,dst.minute,dst.second);
    CTimeSpan timeSpan = time2 - time1; 
    LONGLONG nTSeconds = timeSpan.GetTotalSeconds();
    return (int)nTSeconds;
}

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


// CArchivePlayerDlg 对话框




CArchivePlayerDlg::CArchivePlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArchivePlayerDlg::IDD, pParent)
{

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);   
    m_bdlg = NULL;
    m_CurSelTab = 0;
    m_beLogin   = FALSE;    
    m_beSearchDev     = FALSE;
    pDBinter = NULL;
    m_beLoginMysql  = FALSE;
    m_beOldVersions = FALSE;

}

void CArchivePlayerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB, m_tab);
    DDX_Control(pDX, IDC_DATETIMEPICKER_BDATE, m_ctrlDateStart);
    DDX_Control(pDX, IDC_DATETIMEPICKER_EDATE, m_ctrlDateEnd);
    DDX_Control(pDX, IDC_DATETIMEPICKER_BTIME, m_ctrlTimeStart);
    DDX_Control(pDX, IDC_DATETIMEPICKER_ETIME, m_ctrlTimeEnd);
    DDX_Control(pDX, IDC_EDIT_PORT, m_sqlport);
    DDX_Control(pDX, IDC_EDIT_NAME, m_sqlname);
    DDX_Control(pDX, IDC_EDIT_USER, m_sqluser);
    DDX_Control(pDX, IDC_EDIT_PSD, m_sqlpassword);
    DDX_Control(pDX, IDC_IPADDRESS1, m_sqlip);
    DDX_Control(pDX, IDC_EDIT_DIFF, m_edit_diff);
    DDX_Control(pDX, IDC_COMBO_CONDITION1, m_comCondition1);
    DDX_Control(pDX, IDC_COMBO_CONDITION2, m_comCondition2);
    DDX_Control(pDX, IDC_COMBO_CONDITION3, m_comCondition3);
    DDX_Control(pDX, IDC_COMBO_CONDITION4, m_comCondition4);
    DDX_Control(pDX, IDC_EDIT__CONDITION4, m_editCondition3);
    DDX_Control(pDX, IDC_EDIT_CONDITION3, m_editCondition4);
    DDX_Control(pDX, IDC_EDIT_CONDITION2, m_editCondition2);
    DDX_Control(pDX, IDC_EDIT_CONDITION1, m_editCondition1);
    DDX_Control(pDX, IDC_COMBO_VERSION, m_cbVersion);
}

BEGIN_MESSAGE_MAP(CArchivePlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CArchivePlayerDlg::OnTcnSelchangeTab)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CArchivePlayerDlg::OnBnClickedButtonSearch)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_DEV, &CArchivePlayerDlg::OnBnClickedButtonDev)
    ON_BN_CLICKED(IDC_BUTTON1, &CArchivePlayerDlg::OnBnClickedButton1)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON2, &CArchivePlayerDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON_NTP_TIMING, &CArchivePlayerDlg::OnBnClickedButtonNtpTiming)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CArchivePlayerDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_SELECT, &CArchivePlayerDlg::OnBnClickedButtonSelect)
END_MESSAGE_MAP()


void CArchivePlayerDlg::InitComBox(CComboBox &pCom,std::vector<Iteminfo> &plist)
{
    pCom.ResetContent();
    pCom.AddString(_T(""));
    for (std::size_t i = 0;i<plist.size();++i)
    {
        if (plist.at(i).type != -1)
        {       
            if ( StrCmpW(plist.at(i).listname.c_str(),_T("开始时间")) == 0)
            {
                continue;
            }
            if ( StrCmpW(plist.at(i).listname.c_str(),_T("结束时间")) == 0)
            {
                continue;
            }
            pCom.AddString(plist.at(i).listname.c_str());
        }
        
    }

}

void CArchivePlayerDlg::InitPage()
{
    m_tab.InsertItem(0, _T("视频检查"));
    m_tab.InsertItem(1, _T("设备检查"));

    m_bdlg = new BusinessDialg;
    //创建两个对话框
    if ( NULL == m_bdlg )
    {
        ARCHIVEPLAY_LOG_ERROR("业务单对话框创建失败");
       return;
    }

    m_bdlg->Create(IDD_DIALOG_BUSINESS, &m_tab);
    m_ddlg.Create(IDD_DIALOG_DEVINFO, &m_tab);
    //设定在Tab内显示的范围
    CRect rc;
    m_tab.GetClientRect(rc);
    rc.top += 20;
    rc.bottom -= 0;
    rc.left += 0;
    rc.right -= 0;
    m_bdlg->MoveWindow(&rc);
    m_ddlg.MoveWindow(&rc);
    
    //把对话框对象指针保存起来
    pDialog[0] = m_bdlg;
    pDialog[1] = &m_ddlg;
    //显示初始页面
    pDialog[0]->ShowWindow(SW_SHOW);
    pDialog[1]->ShowWindow(SW_HIDE);


}


BOOL CArchivePlayerDlg::OnInitDialog()
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
   

    InterlockedExchange(&m_beSearchRecord,FALSE);
    InitPage();

    m_sqlip.SetAddress(10,16,38,102);
    m_sqlport.SetWindowTextW(_T("3306"));
    m_sqluser.SetWindowTextW(_T("root"));
    m_sqlpassword.SetWindowTextW(_T("888"));
   // m_sqlname.SetWindowTextW(_T("ivms8000fms"));
    m_sqlname.SetWindowTextW(_T("ivms8120v2"));
    m_edit_diff.SetWindowText(_T("5"));
    


  
    

    CTimeSpan span = CTimeSpan(-7,0,0,0);//iDays间隔天数，可以正负
    CTime time,timeOver;
    time = CTime::GetCurrentTime();
    timeOver = time+span;//获得计算后日期
    m_ctrlDateStart.SetTime(&timeOver);



    m_cbVersion.AddString(_T("v2.0"));
    m_cbVersion.AddString(_T("v2.1"));
    m_cbVersion.SetCurSel(0);


    //初始化加解密库
   int ret =  Hak1((char*)103,"DATA",NULL);
   if(ret != 0)
   {
       ARCHIVEPLAY_LOG_ERROR("加解密库初始化失败");
   }
    
   if( TRUE !=NET_DVR_Init())
   {
       ret = NET_DVR_GetLastError();
       ARCHIVEPLAY_LOG_ERROR("errorCode:%d,errMsg:%s",ret,NET_DVR_GetErrorMsg((LONG*)&ret));
   }
    ret = HCS_SDK_Init();
    if(HCS_OK != ret)
    {
        ARCHIVEPLAY_LOG_ERROR("errorCode:%d,errMsg:%s",ret,NET_DVR_GetErrorMsg((LONG*)&ret));

    }
    
    GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);	
    GetDlgItem(IDC_BUTTON_DEV)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CArchivePlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CArchivePlayerDlg::OnPaint()
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
HCURSOR CArchivePlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CArchivePlayerDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
    //得到新的页面索引
    m_CurSelTab =  m_tab.GetCurSel();
    //把新的页面显示出来
    pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

   /* if (m_CurSelTab == 1)
    {
        m_ddlg.m_ntpdlg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_ddlg.m_ntpdlg.ShowWindow(SW_HIDE);
    }*/
    *pResult = 0;
}


/*************************************************
函数名:    	GetStartStopTime
函数描述:	获取开始与结束时间
输入参数:  
输出参数:   			
返回值:		
**************************************************/
void CArchivePlayerDlg::GetStartStopTime(NET_DVR_TIME &StartDayTime, NET_DVR_TIME &StopDayTime)
{
    CTime StartDay,StartTime;
	CTime StopDay,StopTime;
	
	m_ctrlDateStart.GetTime(StartDay);
	m_ctrlTimeStart.GetTime(StartTime);
	m_ctrlDateEnd.GetTime(StopDay);
	m_ctrlTimeEnd.GetTime(StopTime);
		
	StartDayTime.dwYear = StartDay.GetYear();
	StartDayTime.dwMonth = StartDay.GetMonth();
    StartDayTime.dwDay = StartDay.GetDay();
    StartDayTime.dwHour = StartTime.GetHour();
	StartDayTime.dwMinute = StartTime.GetMinute();
	StartDayTime.dwSecond = StartTime.GetSecond();
	
	StopDayTime.dwYear = StopDay.GetYear();
	StopDayTime.dwMonth = StopDay.GetMonth();
    StopDayTime.dwDay = StopDay.GetDay();
    StopDayTime.dwHour = StopTime.GetHour();
	StopDayTime.dwMinute = StopTime.GetMinute();
	StopDayTime.dwSecond = StopTime.GetSecond();
/*  char fName[100]={0};
    sprintf(fName,"start_%d-%02d-%02d %02d:%02d:%02d",StartDayTime.dwYear,StartDayTime.dwMonth,StartDayTime.dwDay,StartDayTime.dwHour,StartDayTime.dwMinute,StartDayTime.dwSecond);
    OutputDebugStringW(CString(fName));
   
    sprintf(fName,"end_%d-%02d-%02d %02d:%02d:%02d",StopDayTime.dwYear,StopDayTime.dwMonth,StopDayTime.dwDay,StopDayTime.dwHour,StopDayTime.dwMinute,StopDayTime.dwSecond);
    OutputDebugStringW(CString(fName));*/

}








void changeString2Date(const char* str,NET_DVR_TIME &time)
{    
    assert(str != NULL);
    CString strDate;
    COleDateTime ole_time;
    coding code;
    strDate = code.AnsiToUnicode(str);
    ole_time.ParseDateTime(strDate);

    SYSTEMTIME sys_time;
    ole_time.GetAsSystemTime(sys_time);
    
    time.dwYear   = sys_time.wYear;
    time.dwMonth  = sys_time.wMonth;
    time.dwDay    = sys_time.wDay;
    time.dwHour   = sys_time.wHour;
    time.dwMinute = sys_time.wMinute;
    time.dwSecond = sys_time.wSecond;
}


BOOL  CArchivePlayerDlg::CameraFileSearch(RecordInfo &record)
{
    BOOL ret = FALSE;
    DevicInfo &pDev = record.camera;
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
    strcpy_s((char *)struLoginInfo.sDeviceAddress,sizeof(struLoginInfo.sDeviceAddress),pDev.ip); //设备 IP
    strcpy_s((char *)struLoginInfo.sUserName,sizeof(struLoginInfo.sUserName),pDev.user); //设备登录用户
    strcpy_s((char *)struLoginInfo.sPassword,sizeof(struLoginInfo.sPassword),pDev.password); //设备登录密码
    struLoginInfo.wPort = (WORD)pDev.port;
    struLoginInfo.bUseAsynLogin = 0; //同步登录，登录接口返回成功
    LONG lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if(lLoginID == -1)
    { 
        int ret = NET_DVR_GetLastError();
         char * pErr = NET_DVR_GetErrorMsg((LONG*)&ret);
        sprintf_s(record.errMsg,sizeof(record.errMsg),"CAMERA-Login:%s",pErr);
        ARCHIVEPLAY_LOG_ERROR("CAMERA-Login:%s", pErr);
        return FALSE;
    }
   // LONG lLoginID = NET_DVR_Login_V30(pDev.ip,pDev.port,pDev.user,pDev.password,&DeviceInfoTmp);
    ARCHIVEPLAY_LOG_INFO("CAMERA-Login 登录成功 ip:%s",pDev.ip);
    // //录像类型，0xff－全部；0－定时录像；1－移动侦测；2－报警触发；3-报警|移动侦测；4－报警&移动侦测；5－命令触发；6－手动录像
    /* NET_DVR_PLAYCOND  FileCond = {0};
    FileCond.dwChannel = record.channel;

    changeString2Date(record.startTime,FileCond.struStartTime);
    changeString2Date(record.endTime,FileCond.struStopTime);

   LONG hFindHandle = NET_DVR_GetFileByTime_V40(lLoginID,"./test.mp4",&FileCond);
    if(-1 == hFindHandle)
    {
        DWORD ret = NET_DVR_GetLastError();
        char * pErr = NET_DVR_GetErrorMsg((LONG*)&ret);
        sprintf_s(record.errMsg,sizeof(record.errMsg),"CAMERA-Find:%s",pErr); 
        ARCHIVEPLAY_LOG_ERROR("CAMERA-Find:%s", pErr);
        return FALSE;
    }

    //关闭查找，释放句柄
    NET_DVR_StopGetFile(hFindHandle);
    NET_DVR_Logout(lLoginID);*/
    NET_DVR_FILECOND_V40 FileCond = {0};
    FileCond.lChannel = record.channel;
    changeString2Date(record.startTime,FileCond.struStartTime);
    changeString2Date(record.endTime,FileCond.struStopTime);
    FileCond.dwIsLocked = 0xff;

    LONG hFindHandle = NET_DVR_FindFile_V40(lLoginID, &FileCond);

    if (hFindHandle < 0)
    {
        DWORD ret = NET_DVR_GetLastError();
        char *pErr = NET_DVR_GetErrorMsg((LONG*)&ret);
        sprintf_s(record.errMsg,sizeof(record.errMsg),"CAMERA-Find:%s",pErr); 
        ARCHIVEPLAY_LOG_ERROR("CAMERA-Find:%s", pErr);
        return FALSE;
    }
    NET_DVR_FindClose_V30(hFindHandle);
    NET_DVR_Logout(lLoginID);
    return TRUE;
}


BOOL  CArchivePlayerDlg::SearchInCVR(RecordInfo &record)
{
    DevicInfo &pDev = record.store;
    BOOL ret = FALSE;
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
    strcpy_s((char *)struLoginInfo.sDeviceAddress,sizeof(struLoginInfo.sDeviceAddress),pDev.ip); //设备 IP
    strcpy_s((char *)struLoginInfo.sUserName,sizeof(struLoginInfo.sUserName),pDev.user); //设备登录用户
    strcpy_s((char *)struLoginInfo.sPassword,sizeof(struLoginInfo.sPassword),pDev.password); //设备登录密码
    struLoginInfo.wPort = (WORD)pDev.port;
    struLoginInfo.bUseAsynLogin = 0; //同步登录，登录接口返回成功
    LONG lLoginID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if( -1 == lLoginID )
    {
        ret = NET_DVR_GetLastError();
        char* perr = NET_DVR_GetErrorMsg((LONG*)&ret);
        TRACE("CVR_Login error, ErrMsg:%s", perr);
        sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR_Login error, ErrMsg:%s", perr);
        ARCHIVEPLAY_LOG_ERROR("CVR_Login error, ErrMsg:%s", perr);
        return FALSE;
    }

    ARCHIVEPLAY_LOG_INFO("CVR_Login CVR登录成功.ip:%s)",pDev.ip);
    NET_DVR_SEARCH_EVENT_PARAM stFileCond;
    memset(&stFileCond, 0, sizeof(NET_DVR_SEARCH_EVENT_PARAM));
    stFileCond.wMajorType = (WORD)EVENT_STREAM_INFO;
    stFileCond.wMinorType = (WORD)EVENT_STREAM_ID;
    changeString2Date(record.startTime,stFileCond.struStartTime);
    changeString2Date(record.endTime,stFileCond.struEndTime);


    stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
    strncpy_s((char*)stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID,sizeof(stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID),record.streamNo , STREAM_ID_LEN - 1);
    stFileCond.uSeniorParam.struStreamIDParam.struIDInfo.byID[STREAM_ID_LEN - 1] = NULL;  //防止没有结尾
    stFileCond.uSeniorParam.struStreamIDParam.byBackupVolumeNum = 0;

    LONG lFindHandle = NET_DVR_FindFileByEvent(lLoginID, &stFileCond);
    if (-1 == lFindHandle)
    {
        DWORD dwErrCode = NET_DVR_GetLastError();
        TRACE("CVR---NET_DVR_FindFileByEvent失败. lLoginID:%ld, errCode=%lu, ErrMsg:%s", lLoginID, dwErrCode, NET_DVR_GetErrorMsg());
        sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR-Find:%s", NET_DVR_GetErrorMsg());
        ARCHIVEPLAY_LOG_ERROR("CVR---NET_DVR_FindFileByEvent失败. lLoginID:%ld, errCode=%lu, ErrMsg:%s", lLoginID, dwErrCode, NET_DVR_GetErrorMsg());
        NET_DVR_Logout(lLoginID);
        return FALSE;
    }

    BOOL bFind = TRUE;
    BOOL bReturn = TRUE;
    NET_DVR_SEARCH_EVENT_RET stFileData  = {0};
    NET_DVR_SEARCH_EVENT_RET tmpFileData = {0};
    int filecnt = 0;
    while (bFind)
    {
       
        LONG lFileHandle = NET_DVR_FindNextEvent(lFindHandle, &tmpFileData);
        switch (lFileHandle)
        {
        case NET_DVR_FILE_SUCCESS:  //获取文件成功
            { 
                
                if(filecnt == 0)
                {
                    memcpy(&stFileData,&tmpFileData,sizeof(NET_DVR_SEARCH_EVENT_RET));
                }
                else
                {
                    memcpy(&stFileData.struEndTime,&tmpFileData.struEndTime,sizeof(NET_DVR_TIME));
                }
                filecnt++;
                
            }
            break;
        case NET_DVR_FILE_NOFIND:   //没有查到文件
            {
                bFind = FALSE;
                TRACE("Hik---NET_DVR_FILE_NOFIND.");
                sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR-Find:没有找到文件");
                ARCHIVEPLAY_LOG_ERROR("CVR-Find:没有找到文件");

            }
            break;
        case NET_DVR_ISFINDING:     //正在查找
            {
                Sleep(5);
            }
            break;
        case NET_DVR_NOMOREFILE:    //查找文件时没有更多的文件
            {
                int  startdiff = CVRcompareTime(stFileCond.struStartTime,stFileData.struStartTime);// 实际-要求<= interval
                int  enddiff   = CVRcompareTime(stFileData.struEndTime,stFileCond.struEndTime);// 要求-实际<=interval
                //开始时间,实际要比理论值小


                if (  startdiff <= m_interval)
                {
                    //结束时间,实际要比理论值大
                    if (enddiff <= m_interval)
                    {
                        ret = TRUE;
                        bFind = FALSE;
                        break;
                    }
                }

                ret = FALSE;
                sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR-Find:文件不完整,开始时间差%ds,结束时间差:%ds,误差:%d s",startdiff,enddiff,m_interval);
                ARCHIVEPLAY_LOG_ERROR("CVR-Find:文件不完整");
                bFind = FALSE;
            }
            break;
        case NET_DVR_FILE_EXCEPTION:     //查找文件异常
            {
                bFind = FALSE;
                bReturn = FALSE;
                DWORD dwErrCode = NET_DVR_GetLastError();
                TRACE("Hik---NET_DVR_FILE_EXCEPTION. lLogin:%ld, errCode=%ld, ErrMsg:%s", lLoginID, dwErrCode,NET_DVR_GetErrorMsg());
                sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR-Find:%s", NET_DVR_GetErrorMsg());
                ARCHIVEPLAY_LOG_ERROR("CVR-Find:%s", NET_DVR_GetErrorMsg());
            }
            break;
        default:                    //查找失败
            {
                ARCHIVEPLAY_LOG_INFO("CVR_Find 文件查到失败");
                sprintf_s(record.errMsg,sizeof(record.errMsg),"CVR_Find 文件查到失败");
                bFind = FALSE;
                bReturn = FALSE;
            }
            break;
        }
    }

    //关闭查找
   
    if (FALSE == NET_DVR_FindClose_V30(lFindHandle))
    {
        ret = NET_DVR_GetLastError();
        char* perr = NET_DVR_GetErrorMsg((LONG*)&ret);
        TRACE("NET_DVR_FindClose_V30 error:%d, ErrMsg:%s",NET_DVR_GetLastError(), perr);
        ARCHIVEPLAY_LOG_ERROR("NET_DVR_FindClose_V30 error:%d, ErrMsg:%s",NET_DVR_GetLastError(), perr);
    }
    if (FALSE ==  NET_DVR_Logout(lLoginID))
    {
        ret = NET_DVR_GetLastError();
        char* perr = NET_DVR_GetErrorMsg((LONG*)&ret);
        TRACE("CVR_LoginOut error:%d, ErrMsg:%s",NET_DVR_GetLastError(), perr);
        ARCHIVEPLAY_LOG_ERROR("CVR_LoginOut error:%d, ErrMsg:%s",NET_DVR_GetLastError(), perr);
    }
   
    
    return ret;
}



BOOL CArchivePlayerDlg::SearchInCLoud(RecordInfo &record)
{
    DevicInfo &pDev = record.store;
    BOOL ret  = FALSE;
  
    NET_DVR_TIME net_startTime = {0};
    NET_DVR_TIME net_endTime = {0};
    changeString2Date(record.startTime,net_startTime);
    changeString2Date(record.endTime,net_endTime);


    HCS_Time beginTime;
    beginTime.year   = net_startTime.dwYear;
    beginTime.month  = net_startTime.dwMonth;
    beginTime.day    = net_startTime.dwDay;
    beginTime.hour   = net_startTime.dwHour;
    beginTime.minute = net_startTime.dwMinute;
    beginTime.second = net_startTime.dwSecond;
    beginTime.millisecond = 0;

    HCS_Time endTime;
    endTime.year   = net_endTime.dwYear;
    endTime.month  = net_endTime.dwMonth;
    endTime.day    = net_endTime.dwDay;
    endTime.hour   = net_endTime.dwHour;
    endTime.minute = net_endTime.dwMinute;
    endTime.second = net_endTime.dwSecond;
    endTime.millisecond = 0;
    //登录云存储
    unsigned long long login_id;
    if(HCS_OK != HCS_SDK_Login(pDev.ip,pDev.port,
        pDev.user,pDev.password,&login_id))
    {
        TRACE("登录云存储失败\n");
        sprintf_s(record.errMsg,sizeof(record.errMsg),"登录云存储失败");
        ARCHIVEPLAY_LOG_ERROR("登录云存储失败\n");
        return FALSE;
    }
    ARCHIVEPLAY_LOG_INFO("CLoud_Login 云存储登录成功.ip:%s)",pDev.ip);
    int count = 0;
    int handle = 0;
    ret = HCS_SDK_RecSearchStart(
        login_id,
        record.streamNo,
        beginTime,endTime,"",
        HCS_SRLOCK_TYPE_All,
        &count,&handle);

    HCS_RecordInfo info = {0};
    HCS_RecordInfo tmpinfo = {0};
    int filecnt = 0;
    if (ret == HCS_OK && count > 0)
    {  
        for(int i = 0;i < count ;i++)
        {
            int status = 0 ;
            ret = HCS_SDK_RecSearchNext(login_id, handle,&tmpinfo,&status);
            if (ret == HCS_OK)
            {//1-获取信息成功 2-正在查询请等待 3-没有更多的信息，查询结束


                switch (status)
                {
                case 1:  //获取信息成功
                    { 
                        if(filecnt == 0)
                        {
                            memcpy(&info,&tmpinfo,sizeof(HCS_RecordInfo));
                        }
                        else
                        {
                            memcpy(&info.end_time,&tmpinfo.end_time,sizeof(HCS_Time));
                        }
                        filecnt++;
                        
                    }
                    break;
                case 2:   //正在查询请等待
                    {

                        Sleep(5);
                    }
                    break;
                case 3:     //没有更多的信息
                    {

                        int startdiff = CloudcompareTime(info.begin_time,beginTime);
                        int enddiff   = CloudcompareTime(endTime,info.end_time) ;
                        if ( startdiff <= m_interval)
                        {
                            //开始时间,实际要比理论值大
                            if (enddiff <= m_interval)
                            {
                                ret = TRUE;
                                break;
                            }
                        }

                        ret = FALSE;
                        sprintf_s(record.errMsg,sizeof(record.errMsg),"云存储-Find:文件不完整,开始时间差%ds,结束时间差:%ds,误差:%d s",startdiff,enddiff,m_interval);
                        ARCHIVEPLAY_LOG_INFO("CLoud_Find 文件不完整)");
                    }
                    break;
                default:    //查找失败
                    {
                        TRACE("获取录像信息失败\n");
                        sprintf_s(record.errMsg,sizeof(record.errMsg),"CLoud_Find 获取录像信息失败.");
                        ARCHIVEPLAY_LOG_ERROR("CLoud_Find 获取录像信息失败\n");
                        ret = FALSE;
                    }
                    break;
                }

            } 
            else
            {
                //AfxMessageBox(_T("获取录像信息失败..."));
                TRACE("获取录像信息失败\n");
                sprintf_s(record.errMsg,sizeof(record.errMsg),"获取录像信息失败");
                ARCHIVEPLAY_LOG_ERROR("获取录像信息失败\n");
                ret = FALSE;
                HCS_SDK_Logout(login_id);
                return FALSE;
            }
        }

    }
    else if (ret == HCS_OK && count == 0)
    {
        //AfxMessageBox(_T("没有找到录像..."));
         TRACE("云存储没有找到录像\n");
        sprintf_s(record.errMsg,sizeof(record.errMsg),"云存储没有找到录像");
        ARCHIVEPLAY_LOG_INFO("CLoud_Find 云存储没有找到录像)");
        HCS_SDK_Logout(login_id);
        return FALSE;
    }
    else
    {
        //AfxMessageBox(_T("开启录像检索失败..."));
        sprintf_s(record.errMsg,sizeof(record.errMsg),"开启录像检索失败");
        TRACE("开启录像检索失败\n");
        ARCHIVEPLAY_LOG_INFO("CLoud_Find 开启录像检索失败)");
        HCS_SDK_Logout(login_id);
        return FALSE;
    }

    HCS_SDK_Logout(login_id);
    return ret;
}

BOOL  CArchivePlayerDlg::RemoteFileSearch(RecordInfo &record)
{
    BOOL ret = -1;
    DevicInfo &pDev = record.store;

    if(TRUE == m_beOldVersions  &&DEVICE_TYPE_CVR2_0 == pDev.type)//cvr
    {
         ret = SearchInCVR(record);
    }

    if(pDev.type == DEVICE_TYPE_CVR)//cvr
    {
        ret = SearchInCVR(record);
    }
    else if(pDev.type == DEVICE_TYPE_CLOUD)//云存储
    {
        ret = SearchInCLoud(record);
    }
    
    return ret;
}






void CArchivePlayerDlg::ShowInList(const int items,RecordInfo& pRinfo)
{  
    
    std::map<int,BusinessInfo>::iterator itb = BusinessInfolist.find(pRinfo.Businessid);  
    BusinessInfo pBinfo = {0};
    if(itb != BusinessInfolist.end())  
    {  
        pBinfo = itb->second;
    } 
    else
    {
        pBinfo.status = -1;
        return;
    }
    CString ss = _T("");     
    ss.Format(_T("%d"),items);
    int cnt = 0;
    m_bdlg->m_listdata.SetItemText(items,cnt++,ss);
    ss.Empty();

  
    

    coding number;
    m_bdlg->m_listdata.SetItemText(items, cnt++,number.AnsiToUnicode(pBinfo.number));

 /*   if (pBinfo.status == 0)
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,BUSINESS_TYPE_SIGNED_1);
    }
    else if (pBinfo.status == 1)
    {
        m_bdlg->m_listdata.SetItemText(items,cnt++,BUSINESS_TYPE_COMPLAIN_2);
    } 
    else
    {
        m_bdlg->m_listdata.SetItemText(items,cnt++,_T("未关联"));
    }*/
    if (FALSE == m_beOldVersions)
    {  
        coding regionName;
        m_bdlg->m_listdata.SetItemText(items, cnt++,regionName.AnsiToUnicode(pBinfo.regionName));

        coding productName;
        m_bdlg->m_listdata.SetItemText(items, cnt++,productName.AnsiToUnicode(pBinfo.productName));

        coding productTypeName;
        m_bdlg->m_listdata.SetItemText(items, cnt++,productTypeName.AnsiToUnicode(pBinfo.productTypeName));
    }
     
  

    coding proName;
    m_bdlg->m_listdata.SetItemText(items, cnt++,proName.AnsiToUnicode(pBinfo.proName));

    coding inputTime;
    m_bdlg->m_listdata.SetItemText(items, cnt++,inputTime.AnsiToUnicode(pBinfo.inputTime));

    coding recordname;
    m_bdlg->m_listdata.SetItemText(items, cnt++,recordname.AnsiToUnicode(pRinfo.name));

 /*   if (pRinfo.type == 1)
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,RECORD_TYPE_NORMAL_1);
    }
    else
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,RECORD_TYPE_RECORD_2);
    }*/

    if(-1 != pBinfo.status  )//临时录像 ,状态码和历史不一致
    {
        if (pRinfo.status == 0)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,RECORD_STAUS_UPLOAD_0);
        }
        else if (pRinfo.status == 1)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,RECORD_STAUS_UPLOADED_1);
        }
        else if(pRinfo.status == 3)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,RECORD_STAUS_UPLOADING_3);
        }
        else
        {
            ss.Format(_T("%d"),pRinfo.status);
            m_bdlg->m_listdata.SetItemText(items,cnt++,ss);
            ss.Empty();
        }   
    }
    else
    {
        /*
        0-开始记录，1-结束记录（未关联），2-已关联（未上传），3-上传中，4-已上传，8-异常结束，9-上传失败，100-已删除
        */
        if (pRinfo.status == 0)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("开始记录"));
        }
        else if (pRinfo.status == 1)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("未关联"));
        }
        else if(pRinfo.status == 2)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("未上传"));
        }
        else if(pRinfo.status == 3)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("未上传"));
        }
        else if(pRinfo.status == 4)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("已上传"));
        }
        else if(pRinfo.status == 8)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("异常结束"));
        }
        else if(pRinfo.status == 9)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("上传失败"));
        }
        else if(pRinfo.status == 100)
        {
            m_bdlg->m_listdata.SetItemText(items, cnt++,_T("已删除"));
        }
        else
        {
            ss.Format(_T("%d"),pRinfo.status);
            m_bdlg->m_listdata.SetItemText(items,cnt++,ss);
            ss.Empty();
        }   
    }
   

    coding beginTime;
    m_bdlg->m_listdata.SetItemText(items, cnt++,beginTime.AnsiToUnicode(pRinfo.startTime));

    coding endTime;
    m_bdlg->m_listdata.SetItemText(items, cnt++,endTime.AnsiToUnicode(pRinfo.endTime));

    coding cip;
    m_bdlg->m_listdata.SetItemText(items, cnt++,cip.AnsiToUnicode(pRinfo.store.ip));

    coding cstreamno;
    m_bdlg->m_listdata.SetItemText(items, cnt++,cstreamno.AnsiToUnicode(pRinfo.streamNo));



    

    if (pRinfo.bCameraExist == TRUE)
    {      
        m_bdlg->m_listdata.SetItemText(items, cnt++,_T("正常"));

    }
    else
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,_T("异常"));
    }


    if( pRinfo.bStoreExist == TRUE)
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,_T("正常"));
    }
    else
    {
        m_bdlg->m_listdata.SetItemText(items, cnt++,_T("异常"));
    }

    if (pRinfo.status == RECORD_STAUS_UNUPLOAD)//未上传只查前端  已经上传查后端
    {
        m_bdlg->m_listdata.SetItemText(items, cnt-1,_T("未查询"));
    }
    coding errMgs;
    m_bdlg->m_listdata.SetItemText(items, cnt++,errMgs.AnsiToUnicode(pRinfo.errMsg));
}


void CArchivePlayerDlg::SearchRecord()
{

    for(std::size_t i = 0;i<RecordInfoList.size();++i)
    {

        if (RecordInfoList.at(i).status == RECORD_STAUS_UNUPLOAD)//未上传只查前端  已经上传查后端
        {
            clock_t starttime = clock();
            //RecordInfoList.at(i).bStoreExist = TRUE;
            RecordInfoList.at(i).bCameraExist =  CameraFileSearch(RecordInfoList.at(i));
            RecordInfoList.at(i).bStoreExist = RecordInfoList.at(i).bCameraExist;  
            clock_t endtime = clock();
            ARCHIVEPLAY_LOG_INFO("CameraFileSearch time :%d\n",(endtime - starttime));
        }
        else if (RecordInfoList.at(i).status == RECORD_STAUS_UPLOADED)//
        { 
            clock_t starttime = clock();
            RecordInfoList.at(i).bCameraExist = TRUE;
            RecordInfoList.at(i).bStoreExist = RemoteFileSearch(RecordInfoList.at(i));
            clock_t endtime = clock();
            ARCHIVEPLAY_LOG_INFO("RemoteFileSearch time :%d\n",(endtime - starttime));
            if (RecordInfoList.at(i).bStoreExist == FALSE)//后端上传失败的,查一下前端
            {

                 RecordInfoList.at(i).bCameraExist =  CameraFileSearch(RecordInfoList.at(i));

            }
           
        }
        else
        {

        }      
        m_bdlg->m_listdata.InsertItem(i,_T(""));


        ShowInList(i,RecordInfoList.at(i));   
        Sleep(100);
    }
    Invalidate(TRUE);
}


unsigned int __stdcall  SearchThread(void* pParam)
{
    CArchivePlayerDlg *pDlg = (CArchivePlayerDlg*)pParam;


    NET_DVR_TIME StartDayTime ={0};
    NET_DVR_TIME StopDayTime ={0};
    pDlg->GetStartStopTime(StartDayTime,StopDayTime);

    clock_t starttime2 = clock();
    pDlg->pDBinter->getRecordInfo(StartDayTime,StopDayTime);
    ARCHIVEPLAY_LOG_INFO("完成数据视频搜索,共%d条",BusinessInfolist.size());
   

    clock_t endtime2 = clock();
    TRACE("consume time :%d\n",(endtime2 - starttime2));
    ARCHIVEPLAY_LOG_INFO("getRecordInfo time :%d\n",(endtime2 - starttime2));
    clock_t starttime = clock();
  
    ARCHIVEPLAY_LOG_INFO("数据库信息获取完毕");

    //设备上搜索录像
    pDlg->SearchRecord();
    clock_t endtime = clock();
    
    TRACE("consume time :%d\n",(endtime - starttime));
    ARCHIVEPLAY_LOG_INFO("SearchRecord time:%d\n",(endtime - starttime));
    if (BusinessInfolist.empty())
    {
        AfxMessageBox(_T("搜索业务单无结果"));
  
    }
    InterlockedExchange(&pDlg->m_beSearchRecord,FALSE);
    return 0;
}


void CArchivePlayerDlg::getCondition( CComboBox &pCom,CEdit &pEdit,std::vector<filtrate>& pfilter)
{
    CString csConditiontmp,csdatatmp;
    pCom.GetWindowText(csConditiontmp);
    pEdit.GetWindowText(csdatatmp);
    
    if(!csConditiontmp.IsEmpty() && !csdatatmp.IsEmpty())
    {
        for (std::size_t i=0;i<Itemlist.size();++i)
        {
            if (StrCmpW(csConditiontmp.GetBuffer(),Itemlist.at(i).listname.c_str()) == 0)
            {

                if (csConditiontmp.CompareNoCase(_T("录像状态")) == 0)
                {
                    if(csdatatmp.CompareNoCase(RECORD_STAUS_UPLOAD_0) == 0)
                    {
                        csdatatmp  = _T("0");
                    }  
                    else if (csdatatmp.CompareNoCase(RECORD_STAUS_UPLOADED_1) == 0)
                    {
                          csdatatmp  = _T("1");
                    }
                    else if(csdatatmp.CompareNoCase(RECORD_STAUS_UPLOADING_3) == 0)
                    {
                         csdatatmp  = _T("3");
                    }
                    else
                    {
                          csdatatmp  = _T("-1");
                    }   
                }
              
                coding cConditiontmp,cdatatmp;
                filtrate tmp;
                tmp.type = Itemlist.at(i).type;
                tmp.condition  = cConditiontmp.UnicodeToAnsi(Itemlist.at(i).Dbname.c_str());
                tmp.data       = cdatatmp.UnicodeToAnsi(csdatatmp);
                pfilter.push_back(tmp);
            }
        }
       
    }
    
}

void CArchivePlayerDlg::OnBnClickedButtonSearch()
{
    if(TRUE == m_beSearchRecord )    
    {
        AfxMessageBox(_T("正在查询,请不要重复点击"));
        return;
    }
    m_bdlg->m_listdata.DeleteAllItems();
    
    g_filter.clear();
    getCondition( m_comCondition1,m_editCondition1,g_filter);
    getCondition( m_comCondition2,m_editCondition2,g_filter);
    getCondition( m_comCondition3,m_editCondition3,g_filter);
    getCondition( m_comCondition4,m_editCondition4,g_filter);


    CString csInterval;
    m_edit_diff.GetWindowText(csInterval);
    m_interval = _wtoi(csInterval.GetBuffer());

        
    InterlockedExchange(&m_beSearchRecord,TRUE);

    m_tab.SetCurSel(0);
    pDialog[0]->ShowWindow(SW_SHOW);
    pDialog[1]->ShowWindow(SW_HIDE);
    m_CurSelTab = m_tab.GetCurSel();
  
    m_serachThreadHandle = (HANDLE)_beginthreadex(NULL, 0, SearchThread, this, 0,NULL);
   
  

}

void CArchivePlayerDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值 

    DWORD ret = WaitForSingleObject(m_serachThreadHandle,5000);
    switch(ret)
    {
    case WAIT_OBJECT_0:break;
    case WAIT_TIMEOUT:
        {
          TerminateThread(m_serachThreadHandle,0);
        }break;
    default:break;

    }
  
    Hak2(NULL,NULL,NULL);
    NET_DVR_Cleanup();
    HCS_SDK_Deinit();
  
    if ( NULL != pDBinter)
    { 
        delete pDBinter;
        pDBinter = NULL;
    }
    delete m_bdlg;
    m_bdlg = NULL;
  
    CDialog::OnClose();  

}

/*************************************************
函数名:    	getDeviceResoureCfg
函数描述:	获取设备的信息
输入参数:   
输出参数:   			
返回值:		
**************************************************/

void CArchivePlayerDlg::getDeviceResoureCfg(DevicInfo &pDev)
{
    NET_DVR_IPPARACFG_V40 IpAccessCfg;
    memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	
    DWORD  dwReturned;
    BOOL bIPRet = NET_DVR_GetDVRConfig(pDev.iLoginId,NET_DVR_GET_IPPARACFG_V40,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG_V40),&dwReturned);

    int i;
    
    if(!bIPRet)   //不支持ip接入,9000以下设备不支持禁用模拟通道
    {
        
        for(i=0; i<MAX_ANALOG_CHANNUM; i++)
        {
            CHANNEL_INFO chInfo;
            if (i < pDev.iDeviceChanNum)
            {
                sprintf_s(chInfo.chChanName,"camera%d",i+pDev.iStartChan);
                chInfo.iChanIndex=i+pDev.iStartChan;  //通道号
                chInfo.bEnable = TRUE;

            }
            else
            {
                chInfo.iChanIndex = -1;
                chInfo.bEnable = FALSE;
                sprintf_s(chInfo.chChanName, "");
            }
            pDev.chanList.push_back(chInfo);
        }

    }
    else        //支持IP接入，9000设备
    {
        for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //模拟通道
        {
            CHANNEL_INFO chInfo;
            if (i < pDev.iDeviceChanNum)
            {
                sprintf_s(chInfo.chChanName,"camera%d",i+pDev.iStartChan);
                chInfo.iChanIndex = i + pDev.iStartChan;
                if(IpAccessCfg.byAnalogChanEnable[i])
                {
                    chInfo.bEnable = TRUE;
                }
                else
                {
                    chInfo.bEnable = FALSE;
                }

            }
            else//clear the state of other channel
            {
                chInfo.iChanIndex = -1;
                chInfo.bEnable = FALSE;
                sprintf_s(chInfo.chChanName, "");	
            }	
            pDev.chanList.push_back(chInfo);
        }

        //数字通道
        for(i=0; i<MAX_IP_CHANNEL; i++)
        {
            CHANNEL_INFO chInfo;
            if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ip通道在线
            {
               
                chInfo.bEnable = TRUE;
                chInfo.iChanIndex = i+IpAccessCfg.dwStartDChan;
                sprintf_s(chInfo.chChanName,"IP Camera %d",i+1);

            }
            else
            {
               chInfo.bEnable = FALSE;
               chInfo.iChanIndex = -1;
            }
            pDev.chanList.push_back(chInfo);
        }


    }
}


/*************************************************
函数名:    	getNTPinfo()
函数描述:	获取设备的信息
输入参数:   
输出参数:   			
返回值:		
**************************************************/
void CArchivePlayerDlg::getNTPinfo(DevicInfo &pDev)
{
    NET_DVR_NTPPARA ntpInfo = {0};
    DWORD dwReturned = 0;
    DWORD ret = 0;
    ret = NET_DVR_GetDVRConfig(pDev.iLoginId,NET_DVR_GET_NTPCFG,0xFFFFFFFF,&ntpInfo,sizeof(NET_DVR_NTPPARA), &dwReturned);
    if( TRUE != ret)
    {
        ret = NET_DVR_GetLastError();
        TRACE("校时失败:%s\n",NET_DVR_GetErrorMsg((LONG*)&ret));
        pDev.errorno = ret;
    }
    else
    {
        pDev.ntpInfo = ntpInfo;
        //memcpy( &pDev.ntpInfo,&ntpInfo,sizeof(NET_DVR_NTPPARA));

    }
}

/*************************************************
函数名:    	getRecordPlan()
函数描述:	获取录像计划
输入参数:   
输出参数:   			
返回值:		
**************************************************/
void CArchivePlayerDlg::getRecordPlan(DevicInfo &pDev)
{
    NET_DVR_RECORD_V40 RecordPlan = {0};
    DWORD dwReturned = 0;
    for (std::size_t j = 0;j<pDev.chanList.size();++j)
    {
        CHANNEL_INFO &pChannel = pDev.chanList.at(j);
        if(TRUE == pChannel.bEnable )
        {
            if( TRUE != NET_DVR_GetDVRConfig(pDev.iLoginId,NET_DVR_GET_RECORDCFG_V40,pChannel.iChanIndex,&RecordPlan,sizeof(NET_DVR_RECORD_V40), &dwReturned))
            {

                DWORD ret = NET_DVR_GetLastError();
                TRACE("校时失败:%s\n",NET_DVR_GetErrorMsg((LONG*)&ret));
                pDev.errorno = ret;
            }
            else
            {
                memcpy( &pChannel.recordPlan,&RecordPlan,sizeof(NET_DVR_RECORD_V40));
            }

        }
  
    }
   

}

/*************************************************
函数名:    	getInfoFromDevice
函数描述:	获取设备的信息
输入参数:   
输出参数:   			
返回值:		
**************************************************/
void CArchivePlayerDlg::getInfoFromDevice(DevicInfo &pDev)
{
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};

    strcpy_s((char *)struLoginInfo.sDeviceAddress,sizeof(struLoginInfo.sDeviceAddress),pDev.ip); //设备 IP
    strcpy_s((char *)struLoginInfo.sUserName,sizeof(struLoginInfo.sUserName),pDev.user); //设备登录用户
    strcpy_s((char *)struLoginInfo.sPassword,sizeof(struLoginInfo.sPassword),pDev.password); //设备登录密码
    struLoginInfo.wPort = (WORD)pDev.port;
    struLoginInfo.bUseAsynLogin = 0; //同步登录，登录接口返回成功
    pDev.iLoginId = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if(pDev.iLoginId == -1)
    {
        LONG ret = NET_DVR_GetLastError();
        TRACE("Login err %s\n",NET_DVR_GetErrorMsg(&ret));
        ARCHIVEPLAY_LOG_ERROR("Device_Login err:%s",NET_DVR_GetErrorMsg(&ret));
        return;
    }
    ARCHIVEPLAY_LOG_INFO("Device_Login:登录成功.ip:%s ",pDev.ip);

    pDev.iDeviceChanNum = struDeviceInfo.struDeviceV30.byChanNum;
    pDev.iIPChanNum = struDeviceInfo.struDeviceV30.byIPChanNum;
    pDev.iStartChan  = struDeviceInfo.struDeviceV30.byStartChan;
    pDev.iIPStartChan  = struDeviceInfo.struDeviceV30.byStartDChan;
    getDeviceResoureCfg(pDev);
    getNTPinfo(pDev);
    getRecordPlan(pDev);
    NET_DVR_Logout(pDev.iLoginId);
}




void CArchivePlayerDlg::OnBnClickedButtonDev()
{
    // TODO: 在此添加控件通知处理程序代码

    m_tab.SetCurSel(1);
    pDialog[0]->ShowWindow(SW_HIDE);
    pDialog[1]->ShowWindow(SW_SHOW);
   // m_ddlg.m_ntpdlg.ShowWindow(SW_SHOW);
    m_CurSelTab = m_tab.GetCurSel();

    ARCHIVEPLAY_LOG_INFO("开始从数据库获取设备信息");
   // getDeviceInfoFromDB();
    if (NULL != pDBinter)
    {
        pDBinter->getDeviceInfo();
    }
    
    ARCHIVEPLAY_LOG_INFO("从数据库获取%d台设备信息 ",g_Devlist.size());
    if (g_Devlist.empty())
    {
        TRACE("找不到设备\n");
        AfxMessageBox(_T("找不到设备信息"));
        return;
    }

    for (std::size_t i = 0;i<g_Devlist.size();++i)
    {
        getInfoFromDevice(g_Devlist.at(i));
    }

  
    m_ddlg.showChannelList(); 
    m_ddlg.showDevList();
    //CreateDeviceTree();
}


void CArchivePlayerDlg::save2File(CListCtrl & plist)
{
    if ( plist.GetItemCount()<= 0 )
    {
        AfxMessageBox(_T("列表中没有记录需要保存！"));
        return;
    }
    CString strCurTime;
    CString strfilename = _T("下载文件列表-");
    CTime t = CTime::GetCurrentTime();
    strCurTime = t.Format(_T("%Y%m%d%H%M%S"));
    strfilename += strCurTime;
    //打开另存为对话框 ，需要包含 #include <Afxdlgs.h>

    CFileDialog dlg( FALSE, 
        _T("xls"), 
        strfilename, 
        OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
        _T("Excel 文件(*.xls)|*.xls||"));
    dlg.m_ofn.lpstrTitle = _T("文件列表另存为");

    if (dlg.DoModal() != IDOK)
        return;
    CString strFilePath;
    //获得文件路径名
    strFilePath = dlg.GetPathName();
    //判断文件是否已经存在，存在则删除重建
    DWORD dwRe = GetFileAttributes(strFilePath);
    if ( dwRe != (DWORD)-1 )
    {
        DeleteFile(strFilePath);
    }

    CDatabase database;//数据库库需要包含头文件 #include <afxdb.h>
    CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)"); // Excel驱动
    CString sSql,strInsert;

    TRY
    {
        // 创建进行存取的字符串
        sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),sDriver, strFilePath, strFilePath);

        // 创建数据库 (既Excel表格文件)
        if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
        {
            //获得列别框总列数
            int iColumnNum,iRowCount;
            LVCOLUMN lvCol;
            CString strColName; //用于保存列标题名称
            int i,j; //列、行循环参数

            iColumnNum = plist.GetHeaderCtrl()->GetItemCount();   //不包含最后一列 文件大小
            iRowCount = plist.GetItemCount();

            sSql = " CREATE TABLE DSO_DX ( ";
            strInsert = " INSERT INTO DSO_DX ( " ;
            //获得列标题名称
            lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
            lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
            lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
            //以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
            for( i=0 ; i< iColumnNum ; i++ )
            {
                if ( !(plist.GetColumn(i,&lvCol)) )
                    return;
                if ( i<iColumnNum-1 )
                {
                    sSql = sSql + lvCol.pszText + _T(" TEXT , ");
                    strInsert = strInsert + lvCol.pszText + _T(" , ");
                }
                else
                {
                    sSql = sSql + lvCol.pszText + _T(" TEXT ) ");
                    strInsert = strInsert + lvCol.pszText + _T(" )  VALUES ( ");
                }
            }
            //创建Excel表格文件
            database.ExecuteSQL(sSql);

            //循环提取记录并插入到EXCEL中
            sSql = strInsert;
            //char chTemp[33];
            TCHAR chTemp[256];
            for ( j=0 ; j<iRowCount ; j++ )
            {
                memset(chTemp,0,sizeof(chTemp));
                for ( i=0 ; i<iColumnNum ; i++ )
                {
                    plist.GetItemText(j,i,chTemp,sizeof(chTemp));
                    if ( i < (iColumnNum-1) )
                    {
                        sSql = sSql + _T("'") + chTemp + _T("' , ");
                    }
                    else
                    {
                        sSql = sSql + _T("'") + chTemp + _T("' ) ");
                    }
                }
                //将记录插入到表格中
                database.ExecuteSQL(sSql);
                sSql = strInsert; 
            }
        }      

        // 关闭Excel表格文件
        database.Close();

        AfxMessageBox(_T("保存查询结果为Excel文件成功！"));
    }
    CATCH_ALL(e)
    {
        //错误类型很多，根据需要进行报错。
        
        AfxMessageBox(_T("Excel文件保存失败。"));
    }
    END_CATCH_ALL;

}
void CArchivePlayerDlg::OnBnClickedButton1()
{
    
   save2File(m_bdlg->m_listdata);

}

void CArchivePlayerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
  /*  GetClientRect(&m_rect); 

    if(m_tab.m_hWnd) 
    { 
        m_tab.SetWindowPos(NULL, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height()-100, SWP_DRAWFRAME);  
    }
    */
    // TODO: 在此处添加消息处理程序代码
}

void CArchivePlayerDlg::OnBnClickedButton2()
{
    save2File(m_ddlg.m_listDev);
    save2File(m_ddlg.m_listchannel);
}


BOOL CArchivePlayerDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) 
    {  
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 
        {  

            return TRUE;  
        }  
    }  
    return CDialog::PreTranslateMessage(pMsg);
}


/*************************************************
函数名:    	setNPTTiming
函数描述:	设置NTP校时
输入参数:   
输出参数:   			
返回值:		
**************************************************/
BOOL CArchivePlayerDlg::setNPTTiming(DevicInfo &pDev,NET_DVR_NTPPARA &ntpinfo)
{
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};

    strcpy_s((char *)struLoginInfo.sDeviceAddress,sizeof(struLoginInfo.sDeviceAddress),pDev.ip); //设备 IP
    strcpy_s((char *)struLoginInfo.sUserName,sizeof(struLoginInfo.sUserName),pDev.user); //设备登录用户
    strcpy_s((char *)struLoginInfo.sPassword,sizeof(struLoginInfo.sPassword),pDev.password); //设备登录密码
    struLoginInfo.wPort = (WORD)pDev.port;
    struLoginInfo.bUseAsynLogin = 0; //同步登录，登录接口返回成功
    pDev.iLoginId = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if(pDev.iLoginId == -1)
    {
        LONG ret = NET_DVR_GetLastError();
        char *pErr =NET_DVR_GetErrorMsg(&ret);
        TRACE("Login err %s\n",pErr);
        ARCHIVEPLAY_LOG_ERROR("set_NTP err:%s",pErr);
        return FALSE;
    }

    if(!NET_DVR_SetDVRConfig(pDev.iLoginId,NET_DVR_SET_NTPCFG,0xFFFFFFFF,&ntpinfo,sizeof(NET_DVR_NTPPARA)))
    {	
        //pDev.beOpenNTP = FALSE;
        LONG ret = NET_DVR_GetLastError();
        char *pErr =NET_DVR_GetErrorMsg(&ret);
        ARCHIVEPLAY_LOG_ERROR("set_NTP err:%s",pErr);
        pDev.errMsg = pErr;
        return FALSE;
    }
    else
    {
        //pDev.beOpenNTP = FALSE;
    }
   
    NET_DVR_Logout(pDev.iLoginId);
    return TRUE;
}


void CArchivePlayerDlg::OnBnClickedButtonNtpTiming()
{
    NTPTimingdlg myDlg;  
    int nRet = myDlg.DoModal();  
    NET_DVR_NTPPARA ntpinfo = {0};

    switch(nRet)
    {
    case IDOK:
          {
            coding ip,port,interver;
            ntpinfo.byEnableNTP = TRUE;
            ntpinfo.wInterval = (WORD)atoi(interver.UnicodeToAnsi(myDlg.m_Interval.GetBuffer()));
            ntpinfo.wNtpPort = (WORD)atoi(port.UnicodeToAnsi(myDlg.m_DevPort.GetBuffer()));
            char *pIp = ip.UnicodeToAnsi(myDlg.m_DevIp.GetBuffer());
            
            sprintf_s((char *)ntpinfo.sNTPServer,sizeof(ntpinfo.sNTPServer),"%s",pIp);
            int beOpencnt = 0;
            int bedocnt = 0;
            for(std::size_t i = 0;i<g_Devlist.size();++i)
            {
                if (g_Devlist.at(i).beOpenNTP == TRUE)
                {
                    bedocnt++;
                    if (TRUE ==  setNPTTiming(g_Devlist.at(i),ntpinfo))
                    {
                        beOpencnt++;
                    }
                  /*  else
                    {
                        coding coderr;
                        m_ddlg.m_listDev.SetItemText(i,ERR_ITEM_INDEX,coderr.AnsiToUnicode( g_Devlist.at(i).errMsg.c_str()));
                    }*/

                    
                                    
                }
            }
            
            CString ss = _T("");

            ss.Format(_T("共设置%d个设备,成功配置%d个"),bedocnt,beOpencnt);
            AfxMessageBox(ss);
            m_ddlg.showDevList();
          }break;
    case IDCANCEL:break;
    case IDCLOSE:break;
    default:break;
    }


    // TODO: 在此添加控件通知处理程序代码
 

}


void onErrMsgCallback(const int code ,const char* msg,void* userdata)
{
     CArchivePlayerDlg *pDlg = (CArchivePlayerDlg*)userdata;
    // OutputDebugString(msg);

    if(0 == code)
    {
        pDlg->m_beLogin = TRUE;
       
        AfxMessageBox(_T("数据库登陆成功")); 
        pDlg->GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登出"));
        pDlg->GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);	
        pDlg->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);	
        pDlg-> GetDlgItem(IDC_BUTTON_DEV)->EnableWindow(TRUE);
    }
    else
    {   
        pDlg->m_beLogin = FALSE;
        coding err;
        
        AfxMessageBox(err.AnsiToUnicode(msg)); 
        pDlg->GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登录"));
        pDlg->GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
        pDlg->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);	
        pDlg->GetDlgItem(IDC_BUTTON_DEV)->EnableWindow(FALSE);

    }
}

void CArchivePlayerDlg::OnBnClickedButtonLogin()
{
    // TODO: 在此添加控件通知处理程序代码



    if (NULL != pDBinter)
    {
       delete pDBinter;
       pDBinter = NULL;
    }
    if(TRUE == m_beLogin)
    {
        m_beLogin = FALSE;
        GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登录"));
        GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);	
        GetDlgItem(IDC_BUTTON_DEV)->EnableWindow(FALSE);
        return;
    }
    

    CString csDevIp,csDevPort,csUser,csPWD,csDBname,csVersion;
    BYTE nField0,nField1,nField2,nField3; 
    m_sqlip.GetAddress(nField0,nField1,nField2,nField3);
    csDevIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);

    m_sqlport.GetWindowTextW(csDevPort);
    m_sqluser.GetWindowTextW(csUser);
    m_sqlpassword.GetWindowTextW(csPWD);
    m_sqlname.GetWindowText(csDBname);
    m_cbVersion.GetWindowTextW(csVersion);

    if (csVersion.Find(_T("v2.0"))>= 0)
    {
        m_beOldVersions = TRUE;
        if (NULL == pDBinter)
        {
            pDBinter = new operateDBV20;       

        }

//        initItemV20();
    }
    else
    {
        m_beOldVersions = FALSE;
        if (NULL == pDBinter)
        {
            pDBinter = new operateDBV21;       
        }
//        initItemV21();
    }

    coding host,user,password,databasename,port;

    mysqlInfo info;
    info.Ip       = host.UnicodeToAnsi(csDevIp.GetBuffer());
    info.Port     = port.UnicodeToAnsi(csDevPort.GetBuffer());
    info.Username = user.UnicodeToAnsi(csUser.GetBuffer());
    info.Password = password.UnicodeToAnsi(csPWD.GetBuffer());
    info.pErrCallback = onErrMsgCallback;
    info.pData = this;
   
    pDBinter->setDBInfo(info,Itemlist);
    int itemweight =  m_bdlg->getWeight()/Itemlist.size()+1;
   
    int nCols = m_bdlg->m_listdata.GetHeaderCtrl()->GetItemCount();
    m_bdlg->m_listdata.DeleteAllItems();
    for (int j = 0;j < nCols;j++)
    {
        m_bdlg->m_listdata.DeleteColumn(0);
    }


    m_bdlg->m_listdata.InsertColumn(0,_T("序号"),LVCFMT_LEFT ,itemweight);
    for (std::size_t i =0;i<Itemlist.size();++i)
    {

        m_bdlg->m_listdata.InsertColumn(i+1, Itemlist.at(i).listname.c_str(),LVCFMT_LEFT ,itemweight);

        //m_bdlg->m_listdata.SetRedraw(FALSE);//防止重绘
    }

    m_ddlg.m_listDev.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
    m_ddlg.initDevList();

    m_ddlg.m_listchannel.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
    m_ddlg.initChannalList();
    InitComBox(m_comCondition1,Itemlist);
    InitComBox(m_comCondition2,Itemlist);
    InitComBox(m_comCondition3,Itemlist);
    InitComBox(m_comCondition4,Itemlist);
    GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("正在登录"));
    GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);	
}


void CArchivePlayerDlg::OnBnClickedButtonSelect()
{
    static BOOL bCheckFlag = FALSE;
    if (TRUE == bCheckFlag)
    {
        for (int i = 0; i<m_ddlg.m_listDev.GetItemCount(); ++i)  
        {  

            m_ddlg.m_listDev.SetCheck(i, FALSE);  
        }  
        bCheckFlag = FALSE;
        GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("全部选择"));
    }
    else
    {
        for (int i = 0; i<m_ddlg.m_listDev.GetItemCount(); ++i)  
        {  

            m_ddlg.m_listDev.SetCheck(i, TRUE);  
        }  
        bCheckFlag = TRUE;
        GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T(" 全部取消"));
    }

   
    // TODO: 在此添加控件通知处理程序代码
}
