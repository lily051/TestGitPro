
// TransFileToCVRDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <time.h>
#include <winsock2.h>
#include <Windows.h>
#include "TransFileToCVR.h"
#include "TransFileToCVRDlg.h"


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


// CTransFileToCVRDlg 对话框




CTransFileToCVRDlg::CTransFileToCVRDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTransFileToCVRDlg::IDD, pParent)
, m_bLogin(false)
, m_bLoading(false)
, m_bLoadingReverse(false)
, m_nLoginID(BN_INVALID_HANDLE)
, m_nStartRecordHandle(BN_INVALID_HANDLE)
, m_nPlayHandleLoad(BN_INVALID_HANDLE)
, m_SendHandle(INVALID_HANDLE_VALUE)
, m_nCountList(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransFileToCVRDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS_CVR, m_strIPAddressCtrl);
    DDX_Control(pDX, IDC_DATE_START, m_tDateStart);
    DDX_Control(pDX, IDC_TIME_START, m_tTimeStart);
    DDX_Control(pDX, IDC_DATE_STOP, m_tDataEnd);
    DDX_Control(pDX, IDC_TIME_STOP, m_tTimeEnd);
    DDX_Control(pDX, IDC_LIST_FIND_FILE, m_listFindFile);
}

BEGIN_MESSAGE_MAP(CTransFileToCVRDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CTransFileToCVRDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, &CTransFileToCVRDlg::OnBnClickedButtonSelectFile)
    ON_BN_CLICKED(IDC_BUTTON_TRANS_TO_CVR, &CTransFileToCVRDlg::OnBnClickedButtonTransToCvr)
    ON_BN_CLICKED(IDC_BUTTON_FIND_FILE, &CTransFileToCVRDlg::OnBnClickedButtonFindFile)
    ON_BN_CLICKED(IDC_BUTTON_DOWNLOADE, &CTransFileToCVRDlg::OnBnClickedButtonDownloade)
    ON_BN_CLICKED(IDC_BUTTON_RECORD_DEL, &CTransFileToCVRDlg::OnBnClickedButtonRecordDel)
    ON_BN_CLICKED(IDC_BUTTON_STOP_LOAD, &CTransFileToCVRDlg::OnBnClickedButtonStopLoad)
    ON_BN_CLICKED(IDC_BUTTON_DOWNLOADE_RE, &CTransFileToCVRDlg::OnBnClickedButtonDownloadeReverse)
    ON_BN_CLICKED(IDC_BUTTON_STOP_LOAD_RE, &CTransFileToCVRDlg::OnBnClickedButtonStopLoadRe)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_FIND_FILE, &CTransFileToCVRDlg::OnNMRClickListFindFile)

    ON_COMMAND(IDM_GET_FILE_SIZE,&CTransFileToCVRDlg::GetSelectFileSize)
    ON_COMMAND(IDM_GET_PLAY_MP4,&CTransFileToCVRDlg::StartPlay)
END_MESSAGE_MAP()


// CTransFileToCVRDlg 消息处理程序

BOOL CTransFileToCVRDlg::OnInitDialog()
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
    WSADATA WsaData;
    if(WSAStartup(MAKEWORD(2,0),&WsaData))
    {
        MessageBox(_T("初始化网络系统失败"));
    }
    m_strIPAddressCtrl.SetWindowText(_T("10.16.53.232"));
    //
    GetDlgItem(IDC_EDIT_FIND_ID)->SetWindowText(_T("10.16.36.5_2"));
    CTime timeDataStart(2000,1,1,0,0,0);
    CDateTimeCtrl* pCtrlTimeStart = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_START);
    ASSERT(pCtrlTimeStart != NULL);
    VERIFY(pCtrlTimeStart->SetTime(&timeDataStart));
    CTime timeDataStop(2000,1,1,23,59,59); 
    CDateTimeCtrl* pCtrlTimeStop = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_STOP);
    ASSERT(pCtrlTimeStop != NULL);
    VERIFY(pCtrlTimeStop->SetTime(&timeDataStop));
    //
    m_listFindFile.InsertColumn(0,_T("编码器ID"),LVCFMT_LEFT,200);
    m_listFindFile.InsertColumn(1,_T("开始时间"),LVCFMT_CENTER,200);
    m_listFindFile.InsertColumn(2,_T("结束时间"),LVCFMT_CENTER,200);
    m_listFindFile.InsertColumn(3,_T("录像类型"),LVCFMT_CENTER,200);
    m_listFindFile.InsertColumn(4,_T("描述"),LVCFMT_LEFT,200);
    m_listFindFile.SetExtendedStyle(m_listFindFile.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTransFileToCVRDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTransFileToCVRDlg::OnPaint()
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
HCURSOR CTransFileToCVRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CALLBACK MsgError(unsigned int playhandle,DWORD user,int ErrorCode)
{
    DWORD m_ErrorCode = GetLastError();
    char msg[200]={0};
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,0,m_ErrorCode,0,msg,200,NULL);
    OutputDebugStringA(msg);

}
void CTransFileToCVRDlg::OnBnClickedButtonLogin()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLogin)
    {
        USES_CONVERSION;
        UpdateData(TRUE);
        BYTE nField0,nField1,nField2,nField3;
        m_strIPAddressCtrl.GetAddress(nField0,nField1,nField2,nField3);
        CvrDeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
        m_nLoginID = BnNVR_Login(T2A(CvrDeviceIp.GetBuffer(CvrDeviceIp.GetLength())),
            8008,"admin","123", MsgError, NULL);
        if (BN_INVALID_HANDLE == m_nLoginID)
        {
            CString csErr;
            csErr.Format(_T("登录失败,错误码:%d"),BnNVR_GetLastError());
            MessageBox(csErr,_T("提示!"));
            return;
        }
        else
        {
            m_bLogin = true;
            GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登出"));
        }
    }
    else
    {
        int nRet = BnNVR_Logout(m_nLoginID);
        if (0 == nRet)
        {
            m_bLogin = false;
            m_nLoginID = BN_INVALID_HANDLE;
            GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText(_T("登录"));
        }
        else
        {
            CString csErr;
            csErr.Format(_T("登出失败,错误码:%d"),BnNVR_GetLastError());
            MessageBox(csErr,_T("提示!"));
        }
    }

}

void CTransFileToCVRDlg::OnBnClickedButtonSelectFile()
{
    // TODO: 在此添加控件通知处理程序代码
    ary_filename.RemoveAll();
    ary_fileTitle.RemoveAll();
    GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(_T(""));

    CString strFilesPathShow = _T("");
    CString pathName,fileName,fileTitle;
    WCHAR* filters = _T("wav文件(*.wav)|*.wav");
    //创建一个可以选择多个文件的CFileDialog
    CFileDialog fileDlg(TRUE,NULL,_T("*.wav"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,filters);
    //最多打开文件个数设定 100
    fileDlg.m_ofn.nMaxFile = 100 * MAX_PATH;
    WCHAR* ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
    fileDlg.m_ofn.lpstrFile = ch;
    //对内存块清零
    ZeroMemory(fileDlg.m_ofn.lpstrFile,sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
    //显示文件对话框，获取文件名集合
    if (fileDlg.DoModal() == IDOK)
    {
        //获取第一个文件的位置
        POSITION pos_file;
        pos_file = fileDlg.GetStartPosition();
        //循环读出每个路径并存放在数组中
        while(pos_file != NULL)
        {
            CString strTmp = _T("");
            //文件路径存放在数组中
            pathName = fileDlg.GetNextPathName(pos_file);
            ary_filename.Add(pathName);
            //
            strTmp = pathName;
            strTmp += _T(";");
            strFilesPathShow += strTmp;
            //获取文件名
            //从字符串的后面往前遍历，如果遇到'\'则结束遍历,'\'右边的字符串则为文件名
            int length = pathName.GetLength();
            for (int i = 0; i > 0; i--)
            {
                if ('\\' == pathName.GetAt(i))
                {//判断当前字符是否是'\'
                    fileName = pathName.Right(length - i - 1);
                    break;
                }
            }
            //获取文件名(不包含后缀)
            fileTitle = fileName.Left(fileName.GetLength() - 4); //采用CString的Left(int count)截取CString中从左往右数的count个字符 4表示".wav"四个字符
            ary_fileTitle.Add(fileTitle);//将文件名(不包含后缀)添加到数组中
        }
    }
    GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(strFilesPathShow);
    UpdateData(FALSE);
    delete [] ch;
}
//
DWORD WINAPI SendFileToCVR(LPVOID LPARAM)
{
    CTransFileToCVRDlg* pThis = (CTransFileToCVRDlg*)LPARAM;
    pThis->StartSendFileToCVR(NULL);

    return NULL;
}
DWORD WINAPI CTransFileToCVRDlg::StartSendFileToCVR(LPVOID LPARAM)
{
    USES_CONVERSION;
    for (int i = 0;i < ary_filename.GetSize(); i++)
    {
        string FileName = T2A(ary_filename.GetAt(i));
        FILE* fp = fopen(FileName.c_str(),"rb");
        if (fp == NULL)
        {
            CString ErrMsg;
            ErrMsg.Format(_T("打开文件%s失败"),ary_filename.GetAt(i));
            MessageBox(ErrMsg);
            return 0;
        }
        //获取头文件
        char header[40];
        int lTitle = fread(header,sizeof(byte),40,fp);
        if (lTitle != 40)
        {
            MessageBox(_T("读文件头失败!"));
            return 0;
        }
        string strDescript = "description about the record";
        string strCamerID = "10.16.36.5_2";
        m_nStartRecordHandle = BnNVR_StartRecord(m_nLoginID,strCamerID.c_str(),strCamerID.c_str(),
        0, 0, header, 40,strDescript.c_str(),strDescript.length());
        if (BN_INVALID_HANDLE == m_nStartRecordHandle)
        {
            CString ErrorMsg;
            ErrorMsg.Format(_T("编码器 %s,启动录像失败,%u"),_T("10.16.36.5_2"),BnNVR_GetLastError());
            AfxMessageBox(ErrorMsg);
            return 0;
        }
        //实现文件上传
        long Len = 0,Len2 = 40;
        char* pFile = NULL;
        pFile = new char[BUFFSIZE];
        if (pFile == NULL)
        {
            MessageBox(_T("分配缓冲区失败!"));
            return 0;
        }
        fseek(fp, Len2, SEEK_SET);
        time_t tTime = time(NULL);
        while((Len = fread(pFile, sizeof(byte), BUFFSIZE - 10,fp)) > 0)
        {
            tTime = time(NULL);
            int nRet = BnNVR_Record(m_nStartRecordHandle, pFile, Len, tTime, tTime + 2);
            if (nRet < 0)
            {
                CString ErrMsg;
                ErrMsg.Format(_T("发送数据%s,发生失败"),FileName.c_str());
                MessageBox(ErrMsg);
                continue;
            }
            CString strDes;
            strDes.Format(_T("===fld == 发送缓冲区的长度为 %d"),nRet);
            OutputDebugString(strDes);
            Sleep(2000);
            memset(pFile, 0, BUFFSIZE);
        }
        fclose(fp);
        delete[] pFile;
        pFile = NULL;
    }
    BnNVR_StopRecord(m_nStartRecordHandle);
    CString strMsg;
    strMsg.Format(_T("全部文件(%d个)上传完成!"),ary_filename.GetSize());
    MessageBox(strMsg);
    return 1;
}
void CTransFileToCVRDlg::OnBnClickedButtonTransToCvr()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLogin)
    {
        MessageBox(_T("请先登录CVR!"));
        return;
    }
    if (ary_filename.IsEmpty())
    {
        MessageBox(_T("请先选择需要上传的文件!"));
        return;
    }
    //
    m_SendHandle = CreateThread(NULL, 0, SendFileToCVR, this, 0, NULL);
    if (INVALID_HANDLE_VALUE == m_SendHandle)
    {
        MessageBox(_T("创建上传文件线程失败!"));
    }
}
//
void CALLBACK GetResult(unsigned int logInID,const char *CamerID,unsigned int beginTime,unsigned int endTime,const char *Alarmtype,const char*desc,int desclen,NVR_DWORD duser)
{
    if (0 == logInID)
    {
        return;
    }
    CTransFileToCVRDlg* pThis = (CTransFileToCVRDlg*)duser;
    tm timePara;
    char StartTimeBuf[64] = {0};
    char EndTimeBuf[64] = {0};

    time_t beginTime_t = beginTime;
    time_t endTime_t = endTime;
    timePara = *localtime(&beginTime_t);
    strftime(StartTimeBuf, 64, "%Y-%m-%d %H:%M:%S", &timePara);
    timePara = *localtime(&endTime_t);
    strftime(EndTimeBuf, 64, "%Y-%m-%d %H:%M:%S", &timePara);
    OutputDebugString((LPCWSTR)StartTimeBuf);
    OutputDebugString((LPCWSTR)EndTimeBuf);
    
    CString strCamerID(CamerID);
    CString strStartTimeBuf(StartTimeBuf);
    CString strEndTimeBuf(EndTimeBuf);
    CString strAlarmtype(Alarmtype);
    CString strDesc(desc);

    pThis->m_listFindFile.InsertItem(pThis->m_nCountList,strCamerID);
    pThis->m_listFindFile.SetItemText(pThis->m_nCountList,1,strStartTimeBuf);
    pThis->m_listFindFile.SetItemText(pThis->m_nCountList,2,strEndTimeBuf);
    pThis->m_listFindFile.SetItemText(pThis->m_nCountList,3,strAlarmtype);
    OutputDebugString((LPCWSTR)desc);
    char mesg[100];
    sprintf(mesg,"%d",desclen);
    OutputDebugString((LPCWSTR)mesg);
    pThis->m_listFindFile.SetItemText(pThis->m_nCountList,4,strDesc);
    pThis->m_nCountList++;

}
void CTransFileToCVRDlg::OnBnClickedButtonFindFile()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLogin)
    {
        MessageBox(_T("请先登录CVR!"));
        return;
    }
    UpdateData(TRUE);
    CTime dataStart,dataEnd;
    CTime timeStart,timeEnd;
    m_tDateStart.GetTime(dataStart);
    m_tDataEnd.GetTime(dataEnd);
    m_tTimeStart.GetTime(timeStart);
    m_tTimeEnd.GetTime(timeEnd);

    struct tm startTm,endTm;
    memset(&startTm,0,sizeof(struct tm));
    memset(&endTm,0,sizeof(struct tm));
    startTm.tm_year = dataStart.GetYear() - 1900;
    startTm.tm_mon = dataStart.GetMonth() - 1;
    startTm.tm_mday = dataStart.GetDay();
    startTm.tm_hour = timeStart.GetHour();
    startTm.tm_min = timeStart.GetMinute();
    startTm.tm_sec = timeStart.GetSecond();
    startTm.tm_isdst = 0;

    endTm.tm_year = dataEnd.GetYear() - 1900;
    endTm.tm_mon = dataEnd.GetMonth() - 1;
    endTm.tm_mday = dataEnd.GetDay();
    endTm.tm_hour = timeEnd.GetHour();
    endTm.tm_min = timeEnd.GetMinute();
    endTm.tm_sec = timeEnd.GetSecond();
    endTm.tm_isdst = 0;

    time_t startTimeSecond = mktime(&startTm);
    time_t EndTimeSecond = mktime(&endTm);
    //char s[80];
    //strftime(s,80,"%Y-%m-%d %H:%M:%S", &startTm);
    if (difftime(startTimeSecond, EndTimeSecond) >= 0)
    {
        MessageBox(_T("请调整你的查询时间，保证开始时间小于结束时间!"));
        return;
    }
    //
    USES_CONVERSION;
    m_nCountList = 0;
    m_listFindFile.DeleteAllItems();
    CString strCarmeID = _T("");
    CString strRecordType = _T("");
    //GetDlgItem(IDC_EDIT_FIND_ID)->GetWindowText(strCarmeID);
    
    int nRet = BnNVR_SearchRecord(m_nLoginID, W2A(strCarmeID), W2A(strRecordType), startTimeSecond,
        EndTimeSecond, GetResult, (NVR_DWORD)this);
    if (-1 == nRet)
    {
        CString ErrMsg;
        ErrMsg.Format(_T("查询录像失败(errcode=%u)"),BnNVR_GetLastError());
        MessageBox(ErrMsg);
    }
    else
    {
        MessageBox(_T("查询录像成功!"));
    }
}
//
void CALLBACK GetData(unsigned int playhandle, BYTE *data, DWORD datalen, NVR_DWORD user)
{
    USES_CONVERSION;
    CTransFileToCVRDlg *pDlg =(CTransFileToCVRDlg*)user;
    TRACE("datalen is %d\r\n",datalen);
    if(data == NULL || datalen == 0)
    {
        pDlg->m_bLoading = false;
        pDlg->m_bLoadingReverse = false;
        AfxMessageBox(_T("下载完毕\r\n"));
        return;
    }
    FILE *fp = fopen(pDlg->m_FileName, "ab+");
    if (fp == NULL)
    {
        CString ErrMsg;
        ErrMsg.Format(_T("文件%s不存在"), A2W(pDlg->m_FileName));
        AfxMessageBox(ErrMsg);
        return;
    }
    fwrite(data, sizeof(byte), datalen, fp);
    fclose(fp);
}
void CTransFileToCVRDlg::OnBnClickedButtonDownloade()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos = m_listFindFile.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中一行"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFindFile.GetNextSelectedItem(pos);
        }
    }
    if (m_bLoading)
    {
        MessageBox(_T("正在下载中，请先停止!"));
        return;
    }
    if (m_bLoadingReverse)
    {
        MessageBox(_T("正在倒序下载中，请先停止!"));
        return;
    }
    USES_CONVERSION;
    CString strCarmeID = m_listFindFile.GetItemText(nItem,0);
    CString strStartTime = m_listFindFile.GetItemText(nItem,1);
    CString strEndTime = m_listFindFile.GetItemText(nItem,2);
    int nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec;
    int nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec;
    sscanf(W2A(strStartTime),"%d-%d-%d %d:%d:%d",
        &nStartYear, &nStartMonth, &nStartDate, &nStartHour, &nStartMin, &nStartSec);
    sscanf(W2A(strEndTime),"%d-%d-%d %d:%d:%d",
        &nEndYear, &nEndMonth, &nEndDate, &nEndHour, &nEndMin, &nEndSec);
    CTime tTstart(nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec);
    CTime tTend(nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec);
    tm tTM;
    time_t startTime = mktime(tTstart.GetLocalTm(&tTM));
    time_t endTime = mktime(tTend.GetLocalTm(&tTM));
    //
    memset(m_FileName, 0, MAX_LEN);
    memcpy(m_FileName, W2A(strCarmeID),strCarmeID.GetLength());
    char nowTime[50];
    time_t myTime;
    myTime = time(NULL);
    memset(nowTime, 0, 50);
    sprintf(nowTime, "_%u", myTime);
    memcpy(m_FileName + strlen(m_FileName), nowTime, strlen(nowTime));
    strcat(m_FileName + strlen(m_FileName),".wav");
    //
    m_nPlayHandleLoad = BnNVR_GetStream(m_nLoginID, W2A(strCarmeID), 0, startTime, endTime,
        GetData, (NVR_DWORD)this);
    if (m_nPlayHandleLoad == BN_INVALID_HANDLE)
    {
        CString ErrMsg;
        ErrMsg.Format(_T("启动下载失败,%u"),BnNVR_GetLastError());
        MessageBox(ErrMsg);
    }
    else
    {
        m_nPlayHandleLoad = BN_INVALID_HANDLE;
        m_bLoading = true;//正在下载
        MessageBox(_T("启动下载成功"));
    }
    return;
}

void CTransFileToCVRDlg::OnBnClickedButtonRecordDel()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos = m_listFindFile.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中一行"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFindFile.GetNextSelectedItem(pos);
        }
    }
    if (m_bLoading)
    {
        MessageBox(_T("正在下载中，请先停止!"));
        return;
    }
    if (m_bLoadingReverse)
    {
        MessageBox(_T("正在倒序下载中，请先停止!"));
        return;
    }
    USES_CONVERSION;
    CString strCarmeID = m_listFindFile.GetItemText(nItem,0);
    CString strStartTime = m_listFindFile.GetItemText(nItem,1);
    CString strEndTime = m_listFindFile.GetItemText(nItem,2);
    int nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec;
    int nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec;
    sscanf(W2A(strStartTime),"%d-%d-%d %d:%d:%d",
        &nStartYear, &nStartMonth, &nStartDate, &nStartHour, &nStartMin, &nStartSec);
    sscanf(W2A(strEndTime),"%d-%d-%d %d:%d:%d",
        &nEndYear, &nEndMonth, &nEndDate, &nEndHour, &nEndMin, &nEndSec);
    CTime tTstart(nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec);
    CTime tTend(nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec);
    tm tTM;
    time_t startTime = mktime(tTstart.GetLocalTm(&tTM));
    time_t endTime = mktime(tTend.GetLocalTm(&tTM));

    int nRet = BnNVR_DeleteRecord(m_nLoginID,W2A(strCarmeID),startTime, endTime,"");
    if (0 != nRet)
    {
        DWORD m_ErrorCode=BnNVR_GetLastError();
        CString MsgErr;
        MsgErr.Format(_T("删除录像失败,错误码:%d"),m_ErrorCode);
        MessageBox(MsgErr);
    }
    else
    {
        m_listFindFile.DeleteItem(nItem);
        MessageBox(_T("删除录像成功!"));
    }
}

void CTransFileToCVRDlg::OnBnClickedButtonStopLoad()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLoading)
    {
        MessageBox(_T("没有下载中!"));
        return;
    }
    int nResult = BnNVR_StopGetStream(m_nPlayHandleLoad);
    if (0 != nResult)
    {
        DWORD m_ErrorCode = BnNVR_GetLastError();
        CString MsgErr;
        MsgErr.Format(_T("停止下载失败,错误码:%d"),m_ErrorCode);
        MessageBox(MsgErr);
    }
    else
    {
        m_nPlayHandleLoad = BN_INVALID_HANDLE;
        m_bLoading = false;
        MessageBox(_T("停止下载成功!"));
    }
}

void CTransFileToCVRDlg::OnBnClickedButtonDownloadeReverse()
{
    // TODO: 在此添加控件通知处理程序代码
    if (BN_INVALID_HANDLE == m_nLoginID)
    {
        MessageBox(_T("请先登录设备!"));
        return;
    }
    POSITION pos = m_listFindFile.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中一行"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFindFile.GetNextSelectedItem(pos);
        }
    }
    if (m_bLoading)
    {
        MessageBox(_T("正在下载中，请先停止!"));
        return;
    }
    if (m_bLoadingReverse)
    {
        MessageBox(_T("正在倒序下载中，请先停止!"));
        return;
    }
    USES_CONVERSION;
    CString strCarmeID = m_listFindFile.GetItemText(nItem,0);
    CString strStartTime = m_listFindFile.GetItemText(nItem,1);
    CString strEndTime = m_listFindFile.GetItemText(nItem,2);
    int nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec;
    int nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec;
    sscanf(W2A(strStartTime),"%d-%d-%d %d:%d:%d",
        &nStartYear, &nStartMonth, &nStartDate, &nStartHour, &nStartMin, &nStartSec);
    sscanf(W2A(strEndTime),"%d-%d-%d %d:%d:%d",
        &nEndYear, &nEndMonth, &nEndDate, &nEndHour, &nEndMin, &nEndSec);
    CTime tTstart(nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec);
    CTime tTend(nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec);
    tm tTM;
    time_t startTime = mktime(tTstart.GetLocalTm(&tTM));
    time_t endTime = mktime(tTend.GetLocalTm(&tTM));
    //
    memset(m_FileName, 0, MAX_LEN);
    memcpy(m_FileName, W2A(strCarmeID),strCarmeID.GetLength());
    char nowTime[50];
    time_t myTime;
    myTime = time(NULL);
    memset(nowTime, 0, 50);
    sprintf(nowTime, "_%u", myTime);
    memcpy(m_FileName + strlen(m_FileName), nowTime, strlen(nowTime));
    strcat(m_FileName + strlen(m_FileName),".wav");
    //
    m_nPlayHandleLoad = BnNVR_ReverseGetStrem(m_nLoginID, W2A(strCarmeID), 0, startTime, endTime,
        GetData, (NVR_DWORD)this);
    if (m_nPlayHandleLoad == BN_INVALID_HANDLE)
    {
        CString ErrMsg;
        ErrMsg.Format(_T("启动倒序下载失败,%u"),BnNVR_GetLastError());
        MessageBox(ErrMsg);
    }
    else
    {
        m_nPlayHandleLoad = BN_INVALID_HANDLE;
        m_bLoadingReverse = true;
        MessageBox(_T("启动倒序下载成功"));
    }
    return;
}

void CTransFileToCVRDlg::OnBnClickedButtonStopLoadRe()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_bLoadingReverse)
    {
        MessageBox(_T("没有倒序下载中!"));
        return;
    }
    int nResult = BnNVR_ReverseStopGetStream(m_nPlayHandleLoad);
    if (0 != nResult)
    {
        DWORD m_ErrorCode=BnNVR_GetLastError();
        CString MsgErr;
        MsgErr.Format(_T("停止倒序下载失败!"));
        MessageBox(MsgErr);
    }
    else
    {
        MessageBox(_T("停止倒序下载成功!"));
    }
}

void CTransFileToCVRDlg::OnNMRClickListFindFile(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu,MF_STRING,IDM_GET_FILE_SIZE,_T("获取此文件的大小"));
    AppendMenu(hMenu,MF_STRING,IDM_GET_PLAY_MP4,_T("播放此文件"));

    HBITMAP hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
    ASSERT(SetMenuItemBitmaps(hMenu,IDM_GET_FILE_SIZE, MF_BYCOMMAND,hBmp,NULL));
    HBITMAP hBmpPlay = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2));
    ASSERT(SetMenuItemBitmaps(hMenu,IDM_GET_PLAY_MP4, MF_BYCOMMAND,hBmpPlay,NULL));

    POINT point; 
    ::GetCursorPos(&point); 
    ::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, this->m_hWnd, NULL); 
    *pResult = 0;

    //CPoint ptCur;
    //GetCursorPos(&ptCur);
    //if (popMenu.CreatePopupMenu())
    //{
    //    popMenu.AppendMenu(MF_STRING,IDM_GET_FILE_SIZE,_T("获取此文件的大小"));
    //    //CBitmap bitmap;
    //    //bitmap.LoadBitmap(IDB_BITMAP1);
    //    HBITMAP hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));

    //    //CMenu *pSubMenu = new CMenu;
    //    //pSubMenu = popMenu.GetSubMenu(0);
    //    //ASSERT(pSubMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmap,&bitmap));
    //    ASSERT(SetMenuItemBitmaps(popMenu.m_hMenu,IDM_GET_FILE_SIZE, MF_BYCOMMAND,hBmp,NULL));
    //    popMenu.TrackPopupMenu(TPM_LEFTALIGN,ptCur.x,ptCur.y,this);
    //    //delete pSubMenu;
    //    ClientToScreen(&ptCur);
    //}
    //*pResult = 0;
}
//
void CTransFileToCVRDlg::GetSelectFileSize()
{
    POSITION pos = m_listFindFile.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中一行"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFindFile.GetNextSelectedItem(pos);
        }
    }
    USES_CONVERSION;
    CString strCarmeID = m_listFindFile.GetItemText(nItem,0);
    CString strStartTime = m_listFindFile.GetItemText(nItem,1);
    CString strEndTime = m_listFindFile.GetItemText(nItem,2);
    int nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec;
    int nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec;
    sscanf(W2A(strStartTime),"%d-%d-%d %d:%d:%d",
        &nStartYear, &nStartMonth, &nStartDate, &nStartHour, &nStartMin, &nStartSec);
    sscanf(W2A(strEndTime),"%d-%d-%d %d:%d:%d",
        &nEndYear, &nEndMonth, &nEndDate, &nEndHour, &nEndMin, &nEndSec);
    CTime tTstart(nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec);
    CTime tTend(nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec);
    tm tTM;
    time_t startTime = mktime(tTstart.GetLocalTm(&tTM));
    time_t endTime = mktime(tTend.GetLocalTm(&tTM));

    unsigned int DataSizeHigh = 0;
    unsigned int DataSizeLow = 0;

    int nRet = BnNVR_GetRecordSize(m_nLoginID,W2A(strCarmeID),startTime, endTime,&DataSizeHigh,&DataSizeLow);
    if (-1 == nRet)
    {
        DWORD m_ErrorCode=BnNVR_GetLastError();
        CString MsgErr;
        MsgErr.Format(_T("得到录像的大小失败,%u"),m_ErrorCode);
        MessageBox(MsgErr);
    }
    else
    {
        _int64 m_totalsize = ((_int64)DataSizeHigh<<32) + DataSizeLow;
        char ErrorMsg[50]={0};
        memset(ErrorMsg,0,50);
        _i64toa(m_totalsize,ErrorMsg,10);
        char ErrorMsg2[50]={0};
        memset(ErrorMsg2,0,50);
        _i64toa(m_totalsize/1024,ErrorMsg2,10);
        char meg[150]={0};
        CString MsgErr;
        MsgErr.Format(_T("得到录像的大小为%s Byte = %s KB"),A2W(ErrorMsg), A2W(ErrorMsg2));
        MessageBox(MsgErr);
    }
}
//
void CTransFileToCVRDlg::StartPlay()
{
    POSITION pos = m_listFindFile.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中一行"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFindFile.GetNextSelectedItem(pos);
        }
    }
    USES_CONVERSION;
    CString strCarmeID = m_listFindFile.GetItemText(nItem,0);
    CString strStartTime = m_listFindFile.GetItemText(nItem,1);
    CString strEndTime = m_listFindFile.GetItemText(nItem,2);
    int nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec;
    int nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec;
    sscanf(W2A(strStartTime),"%d-%d-%d %d:%d:%d",
        &nStartYear, &nStartMonth, &nStartDate, &nStartHour, &nStartMin, &nStartSec);
    sscanf(W2A(strEndTime),"%d-%d-%d %d:%d:%d",
        &nEndYear, &nEndMonth, &nEndDate, &nEndHour, &nEndMin, &nEndSec);
    CTime tTstart(nStartYear, nStartMonth, nStartDate, nStartHour, nStartMin, nStartSec);
    CTime tTend(nEndYear, nEndMonth, nEndDate, nEndHour, nEndMin, nEndSec);
    tm tTM;
    time_t startTime = mktime(tTstart.GetLocalTm(&tTM));
    time_t endTime = mktime(tTend.GetLocalTm(&tTM));

    //
    memset(&m_StructPlayInfo,0,sizeof(Play_Info));
    m_StructPlayInfo.m_nLoginID = m_nLoginID;
    sprintf(m_StructPlayInfo.CamerID,"%s",W2A(strCarmeID));
    m_StructPlayInfo.startTime = startTime;
    m_StructPlayInfo.endTime = endTime;

    INT_PTR nResponse = dlg.DoModal();

    if (IDCANCEL == nResponse)
    {
        return;
    }
}