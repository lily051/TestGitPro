// VideoQueryAndDownload.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoQueryAndDownload.h"


// CVideoQueryAndDownload 对话框

IMPLEMENT_DYNAMIC(CVideoQueryAndDownload, CPropertyPage)

CVideoQueryAndDownload::CVideoQueryAndDownload()
	: CPropertyPage(CVideoQueryAndDownload::IDD)
    , m_camerID(_T("10.16.36.211_8006_9"))
    , m_VideoTypeQuery(_T(""))
    , m_VideoLockType(_T("0"))
    , m_nLogID(-1)
    , m_VideoTypeDown(_T(""))
    , m_VideoOffsetDown(_T("0"))
    , m_nFlag(-1)
    , m_nErrorCode(0)
    , m_ItemIndexInfo(0)
    , m_csLockTime(_T("0"))
    , m_handlePlayback(-1)
    , m_handleDownLoad(-1)
    , m_hThreadVideoPlayBack(NULL)
    , m_hThreadVideoDownLoad(NULL)
    , m_nPlayBackMod(HCS_PBMODE_NORMAL)
    , m_nPort(-1)
    , m_bPlayBacking(true)
    , m_bDownLoading(false)
    , m_nPlayBackModCout(0)
    , m_bIsChangePlayMode(false)
{
}

CVideoQueryAndDownload::~CVideoQueryAndDownload()
{
    if (!VecRecordInfo.empty())
    {
        VecRecordInfo.clear();
    }
    if(NULL != m_hThreadVideoPlayBack)
    {
        ::CloseHandle(m_hThreadVideoPlayBack);
        m_hThreadVideoPlayBack = NULL;
    }
    if(NULL != m_hThreadVideoDownLoad)
    {
        ::CloseHandle(m_hThreadVideoDownLoad);
        m_hThreadVideoDownLoad = NULL;
    }
}

void CVideoQueryAndDownload::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DATE_START, m_DateStart);
    DDX_Control(pDX, IDC_DATE_STOP, m_DateEnd);
    DDX_Control(pDX, IDC_TIME_START, m_TimeStart);
    DDX_Control(pDX, IDC_TIME_STOP, m_TimeEnd);
    DDX_Text(pDX, IDC_EDIT_CODE_ID, m_camerID);
    DDX_Text(pDX, IDC_EDIT_VIDEOTYPE_QUERY, m_VideoTypeQuery);
    DDX_Text(pDX, IDC_EDIT_VIDEOTYPE_LOCK, m_VideoLockType);
    DDX_Text(pDX, IDC_EDIT_VIDEOTYPE_DOWN, m_VideoTypeDown);
    DDX_Text(pDX, IDC_EDIT_OFFSET, m_VideoOffsetDown);
    DDX_Control(pDX, IDC_LIST_QUERY, m_listVideoQuery);
    DDX_Text(pDX, IDC_EDIT_LOCK_TIME, m_csLockTime);
    DDX_Control(pDX, IDC_STATIC_FRAM_SHOW, m_PlayBackShow);
}


BEGIN_MESSAGE_MAP(CVideoQueryAndDownload, CPropertyPage)
    ON_BN_CLICKED(IDC_BUTTON_GETHEAD, &CVideoQueryAndDownload::OnBnClickedButtonGethead)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_QUERY, &CVideoQueryAndDownload::OnBnClickedButtonVideoQuery)
    ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CVideoQueryAndDownload::OnBnClickedButtonDownload)
    ON_BN_CLICKED(IDC_BUTTON_DEL_VIDEO, &CVideoQueryAndDownload::OnBnClickedButtonDelVideo)
    ON_BN_CLICKED(IDC_BUTTON_QUERY_CLEAR, &CVideoQueryAndDownload::OnBnClickedButtonQueryClear)
    ON_BN_CLICKED(IDC_BUTTON_LOCK_VIDEO, &CVideoQueryAndDownload::OnBnClickedButtonLockVideo)
    ON_BN_CLICKED(IDC_BUTTON_RECORD_PLAYBACK, &CVideoQueryAndDownload::OnBnClickedButtonRecordPlayback)
    ON_BN_CLICKED(IDC_BUTTON_STOP_RECORD_PLAYBACK, &CVideoQueryAndDownload::OnBnClickedButtonStopRecordPlayback)
    ON_BN_CLICKED(IDC_BUTTON_RECORD_PLAYBACK_MOD, &CVideoQueryAndDownload::OnBnClickedButtonRecordPlaybackMod)
END_MESSAGE_MAP()

BOOL CVideoQueryAndDownload::OnInitDialog()
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
    CTime timeDataStart(2000,1,1,0,0,0); 
    CDateTimeCtrl* pCtrlTimeStart = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_START);
    ASSERT(pCtrlTimeStart != NULL);
    VERIFY(pCtrlTimeStart->SetTime(&timeDataStart));
    CTime timeDataStop(2000,1,1,23,59,59); 
    CDateTimeCtrl* pCtrlTimeStop = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_STOP);
    ASSERT(pCtrlTimeStop != NULL);
    VERIFY(pCtrlTimeStop->SetTime(&timeDataStop));
    //
    m_listVideoQuery.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    m_listVideoQuery.InsertColumn(0,_T("监控点ID"),LVCFMT_CENTER,85);
    m_listVideoQuery.InsertColumn(1,_T("开始时间"),LVCFMT_CENTER,85);
    m_listVideoQuery.InsertColumn(2,_T("结束时间"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(3,_T("数据类型"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(4,_T("录像类型"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(5,_T("锁定状态"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(6,_T("转码类型"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(7,_T("录像大小"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(8,_T("资源池"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(9,_T("存储服务器IP及端口"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(10,_T("可用状态"),LVCFMT_CENTER,60);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// CVideoQueryAndDownload 消息处理程序

void CVideoQueryAndDownload::OnBnClickedButtonGethead()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    if (VecRecordInfo.empty())
    {
        MessageBox(_T("当前未查询到录像"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_camerID);
    //
    char buffVideHead[MAX_HEAD_LEN] = {0};
    memset(buffVideHead, 0, sizeof(buffVideHead));
    int head_size = 0;
    int ret = HCS_SDK_GetRecHead(m_nLogID, strCameraID.c_str(), VecRecordInfo[nItem].begin_time, VecRecordInfo[nItem].end_time, buffVideHead,sizeof(buffVideHead), &head_size);
    if (HCS_OK != ret)
    {
        CString m_csErr;
        m_csErr.Format(_T("获取文件头数据出错(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    else
    {
        CString m_Msg;
        m_Msg.Format(_T("获取监控ID:%s 第:%d录像头数据，且长度:%d"),A2W(strCameraID.c_str()),nItem,head_size);
        MessageBox(m_Msg);
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonVideoQuery()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_camerID);
    std::string strRecType = W2A(m_VideoTypeQuery);
    std::string strRecordType  = W2A(m_VideoTypeQuery);
    

    CTime StartDay,StartTime;
    CTime StopDay,StopTime;
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);

    HCS_Time begin_time;
    HCS_Time end_time;
    memset(&begin_time, 0, sizeof(HCS_Time));
    memset(&end_time, 0, sizeof(HCS_Time));
    begin_time.year = (unsigned int)(StartDay.GetYear());
    begin_time.month = (unsigned int)(StartDay.GetMonth());
    begin_time.day = (unsigned int)(StartDay.GetDay());
    begin_time.hour = (unsigned int)(StartTime.GetHour());
    begin_time.minute = (unsigned int)(StartTime.GetMinute());
    begin_time.second = (unsigned int)(StartTime.GetSecond());
    begin_time.millisecond = 0;

    end_time.year = (unsigned int)(StopDay.GetYear());
    end_time.month = (unsigned int)(StopDay.GetMonth());
    end_time.day = (unsigned int)(StopDay.GetDay());
    end_time.hour = (unsigned int)(StopTime.GetHour());
    end_time.minute = (unsigned int)(StopTime.GetMinute());
    end_time.second = (unsigned int)(StopTime.GetSecond());
    end_time.millisecond = 0;
    if (0 != _ttoi(m_VideoLockType) && 1 != _ttoi(m_VideoLockType) && 2 != _ttoi(m_VideoLockType))
    {
        MessageBox(_T("查询锁定的录像类型非0、1、2"));
        return;
    }
    int count_total = 0;
    int handle = 0;
    int ret = HCS_SDK_RecSearchStart(m_nLogID,strCameraID.c_str(),
        begin_time,end_time,strRecType.c_str(),HCS_SRLOCK_TYPE_All,&count_total,&handle);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启录像检索失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
    }
    else
    {
        m_listVideoQuery.DeleteAllItems();
        VecRecordInfo.clear();
        m_ItemIndexInfo = 0;
        int status;
        HCS_RecordInfo info;
        int next_count = count_total;
        while(next_count > 0)
        {
            status = 0;
            memset(&info, 0, sizeof(info));
            int rets = HCS_SDK_RecSearchNext(m_nLogID,handle,&info,&status);
            if (rets != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("获取下一条录像段信息失败(错误代码:%d)"), ret);
                MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
                break;
            }
            else
            {
                if (1 == status)
                {
                    next_count--;
                    VecRecordInfo.push_back(info);
                    //
                    CString csBeginTime,csEndTime,csType,csRecordType,csLock,csTransType,csSize,csStatus;
                    csBeginTime.Format(_T("%4d-%2d-%2d %2d:%2d:%2d"),info.begin_time.year,info.begin_time.month,info.begin_time.day,
                        info.begin_time.hour,info.begin_time.minute,info.begin_time.second);
                    csEndTime.Format(_T("%4d-%2d-%2d %2d:%2d:%2d"),info.end_time.year,info.end_time.month,info.end_time.day,
                        info.end_time.hour,info.end_time.minute,info.end_time.second);
                    csType.Format(_T("%d"),int(info.type));
                    csRecordType.Format(_T("%d"),info.record_type);
                    csSize.Format(_T("%d"),info.size);
                    csLock.Format(_T("%d"),int(info.lock));
                    csStatus.Format(_T("%d"),int(info.status));
                    csTransType.Format(_T("%d"),int(info.trans_type));
                    m_listVideoQuery.InsertItem(m_ItemIndexInfo,A2W(info.camera_id));
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,1,csBeginTime);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,2,csEndTime);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,3,csType);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,4,csRecordType);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,5,csLock);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,6,csTransType);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,7,csSize);
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,8,A2W(info.pool_id));
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,9,A2W(info.vs_ip));
                    m_listVideoQuery.SetItemText(m_ItemIndexInfo,10,csStatus);
                    m_ItemIndexInfo++;
                    continue;
                }
                else if (2 == status)
                {
                    Sleep(200);
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        CString m_Msg;
        m_Msg.Format(_T("录像共检索%d录像段"), count_total);
        MessageBox(m_Msg);
        ret= HCS_SDK_RecSearchStop(m_nLogID,handle);
        if (ret != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("停止录像检索失败(错误代码:%d)"), ret);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        }
    }
}
void DownloadCb(int handle, const char* data, int data_size, int flag, int error_code, void* user_data)
{
    USES_CONVERSION;
    CVideoQueryAndDownload *pDlg =(CVideoQueryAndDownload*)user_data;
    pDlg->m_nFlag = flag;
    pDlg->m_nErrorCode = error_code;
    if (data != NULL && data_size > 0)
    {
        //录像数据处理
        FILE *fp = fopen(pDlg->m_FileName, "ab+");
        if (fp == NULL)
        {
            DWORD errCode = GetLastError();
            CString ErrMsg;
            ErrMsg.Format(_T("文件%s不存在(错误码:%d)"), A2W(pDlg->m_FileName),errCode);
            AfxMessageBox(ErrMsg);
            return;
        }
        fwrite(data, sizeof(char), data_size, fp);
        fclose(fp);
    }
    else
    {
        if (flag == 0 && error_code == 0)
        {
            //数据正常下载完毕
            //AfxMessageBox(_T("下载完毕\r\n"));
            return;
        }
        else
        {
            //下载出错，异常结束
            //AfxMessageBox(_T("下载异常\r\n"));
            return;
        }
    }
}
//
unsigned int _stdcall VideoDownLoad(LPVOID lpParam)
{
    CVideoQueryAndDownload* pVideoLinkCheck = static_cast<CVideoQueryAndDownload*>(lpParam);
    if (NULL == pVideoLinkCheck)
    {
        return 1;
    }
    pVideoLinkCheck ->VideoDownLoadDeal();
    return 0;
}
void CVideoQueryAndDownload::VideoDownLoadDeal()
{
    while(m_bDownLoading)
    {
        if (m_nFlag == 0 && m_nErrorCode == 0)
        {
            int ret = HCS_SDK_RecSearchStop(m_nLogID, m_handleDownLoad);
            if (ret != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("停止录像下载失败(错误代码:%d)"), ret);
                MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            }
            MessageBox(_T("录像下载完毕!"));
            break;
        }
        else if (m_nFlag != 0 && m_nErrorCode == 0)
        {
            Sleep(200);
            continue;
        }
        else
        {
            CString m_csErr;
            m_csErr.Format(_T("录像下载异常(错误代码:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            break;
        }
    }
    m_bDownLoading = false;
}
void CVideoQueryAndDownload::OnBnClickedButtonDownload()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_camerID);
    unsigned long long lVideoOffsetDown  = _ttoi(m_VideoOffsetDown);

    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    if (m_bDownLoading)
    {
        MessageBox(_T("正在下载其他文件，请等待下载完毕!"));
        return;
    }
    m_nFlag = -1;
    m_nErrorCode = 0;
    //
    std::vector<HCS_RecordInfo>::const_iterator iterRecord = VecRecordInfo.begin();
    if (iterRecord + nItem == VecRecordInfo.end())
    {
        MessageBox(_T("选中下载录像行超出范围!"));
        return;
    }
    HCS_RecordInfo RecordInfo = *(iterRecord + nItem);
    char buffRecordType[MAX_LEN];
    sprintf_s(buffRecordType,MAX_LEN,"%d",RecordInfo.record_type);

    memset(m_FileName, 0, MAX_LEN);
    memcpy_s(m_FileName, MAX_LEN, RecordInfo.camera_id,strlen(RecordInfo.camera_id));
    char cTime[50];
    sprintf(cTime, "_%4d%02d%02d%02d%02d%02d_%4d%02d%02d%02d%02d%02d", RecordInfo.begin_time.year,RecordInfo.begin_time.month,
        RecordInfo.begin_time.day,RecordInfo.begin_time.hour,RecordInfo.begin_time.minute,RecordInfo.begin_time.second
        ,RecordInfo.end_time.year,RecordInfo.end_time.month,RecordInfo.end_time.day,RecordInfo.end_time.hour,RecordInfo.end_time.minute,RecordInfo.end_time.second);
    memcpy(m_FileName + strlen(m_FileName), cTime, strlen(cTime));
    strcat(m_FileName + strlen(m_FileName),".mp4");

    int userdata = 0;
    int ret = HCS_SDK_RecDownloadStart(m_nLogID,RecordInfo.camera_id, RecordInfo.begin_time, 
        RecordInfo.end_time, buffRecordType, lVideoOffsetDown, DownloadCb, (void*)this, &m_handleDownLoad);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启录像下载失败(错误代码:%d)"), ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    //
    m_bDownLoading = true;
    if(NULL != m_hThreadVideoDownLoad)
    {
        ::CloseHandle(m_hThreadVideoDownLoad);
        m_hThreadVideoDownLoad = NULL;
    }
    //创建录像下载结束线程
    if(NULL == m_hThreadVideoDownLoad)
    {
        m_hThreadVideoDownLoad = (HANDLE)_beginthreadex(NULL,0,VideoDownLoad,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoDownLoad)
        {
            CString m_csErr;
            m_csErr.Format(_T("创建录像播放线程失败"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonDelVideo()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    std::vector<HCS_RecordInfo>::const_iterator iterRecord = VecRecordInfo.begin();
    if (iterRecord + nItem != VecRecordInfo.end())
    {
        HCS_RecordInfo RecordInfo = *(iterRecord + nItem);
        char buffRecordType[MAX_LEN];
        sprintf_s(buffRecordType,MAX_LEN,"%d",RecordInfo.record_type);
        int nRet = HCS_SDK_RecDelete(m_nLogID, RecordInfo.camera_id,buffRecordType,RecordInfo.begin_time,RecordInfo.end_time);
        if (nRet != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("删除录像失败(错误代码:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        VecRecordInfo.erase(iterRecord + nItem);
        m_listVideoQuery.DeleteItem(nItem);
        MessageBox(_T("删除录像成功!"));
    }
    else
    {
        MessageBox(_T("选中的行超出查询结果的范围"));
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonQueryClear()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!VecRecordInfo.empty())
    {
        VecRecordInfo.clear();
    }
    m_listVideoQuery.DeleteAllItems();
}

void CVideoQueryAndDownload::OnBnClickedButtonLockVideo()
{
    // TODO: 在此添加控件通知处理程序代码
    USES_CONVERSION;
    UpdateData(TRUE);
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    std::vector<HCS_RecordInfo>::const_iterator iterRecord = VecRecordInfo.begin();
    if (iterRecord + nItem != VecRecordInfo.end())
    {
        HCS_RecordInfo RecordInfo = *(iterRecord + nItem);
        char buffRecordType[MAX_LEN];
        sprintf_s(buffRecordType,MAX_LEN,"%d",RecordInfo.record_type);
        int nRet = HCS_SDK_RecLock(m_nLogID, RecordInfo.camera_id,buffRecordType,RecordInfo.begin_time,RecordInfo.end_time,_ttoi(m_csLockTime));
        if (nRet != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("锁定录像失败(错误代码:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        MessageBox(_T("锁定录像成功!"));
    }
    else
    {
        MessageBox(_T("选中的行超出查询结果的范围，致无法锁定"));
    }
}
//
unsigned int _stdcall VideoPlayBack(LPVOID lpParam)
{
    CVideoQueryAndDownload* pVideoLinkCheck = static_cast<CVideoQueryAndDownload*>(lpParam);
    if (NULL == pVideoLinkCheck)
    {
        return 1;
    }
    pVideoLinkCheck ->StartVideoPlayBack();
    return 0;
}
void CVideoQueryAndDownload::StartVideoPlayBack()
{
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    if (m_bDownLoading)
    {
        MessageBox(_T("正在下载中请等待下载完毕后回放!"));
        return;
    }
    std::vector<HCS_RecordInfo>::const_iterator iterRecord = VecRecordInfo.begin();
    if (iterRecord + nItem != VecRecordInfo.end())
    {
        HCS_RecordInfo RecordInfo = *(iterRecord + nItem);
        char buffRecordType[MAX_LEN];
        sprintf_s(buffRecordType,MAX_LEN,"%d",RecordInfo.record_type);
        int nRet = HCS_SDK_PlaybackStart(m_nLogID, RecordInfo.camera_id, RecordInfo.begin_time,
            RecordInfo.end_time, HCS_PlaybackMode(m_nPlayBackMod), buffRecordType, &m_handlePlayback);
        //是否切换回放
        if (m_bIsChangePlayMode)
        {
            m_bIsChangePlayMode = false;
            CString csExplain;
            if (0 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 2;
                m_nPlayBackModCout = m_nPlayBackModCout + 1;
                csExplain = _T("正常I帧回放");
            }
            else if (1 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 3;
                m_nPlayBackModCout = m_nPlayBackModCout + 1;
                csExplain = _T("倒序回放");
            }
            else if (2 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 1;
                m_nPlayBackModCout = 0;
                csExplain = _T("顺序回放");
            }
            else
            {
                MessageBox(_T("回放切换失败-m_nPlayBackModCout>3"));
                return;
            }
            nRet = HCS_SDK_PlaybackChangeMode(m_nLogID, m_handlePlayback,HCS_PlaybackMode(m_nPlayBackMod));
            if (nRet != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("回放切换失败(错误代码:%d)"),nRet);
                MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
                return;
            }
            CString m_csMsg;
            m_csMsg.Format(_T("回放切换成%s成功!"),csExplain);
            MessageBox(m_csMsg);
        }

        //获取录像头
        char buffVideHead[MAX_HEAD_LEN] = {0};
        memset(buffVideHead, 0, sizeof(buffVideHead));
        int head_size = 0;
        int ret = HCS_SDK_GetRecHead(m_nLogID, RecordInfo.camera_id, VecRecordInfo[nItem].begin_time, VecRecordInfo[nItem].end_time, buffVideHead,sizeof(buffVideHead), &head_size);
        if (nRet != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("获取录像回放头失败(错误代码:%d)"),nRet);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        //
        m_nPort = -1;
        if (!PlayM4_GetPort(&m_nPort))
        {
            MessageBox(_T("获取通道号失败!"));
            return;
        }
        //将数据推给播放库播放
        if (!PlayM4_GetPort(&m_nPort))
        {
            MessageBox(_T("获取通道号失败!"));
            return;
        }
        //设置流模式类型 
        if (!PlayM4_SetStreamOpenMode(m_nPort,STREAME_FILE))
        {
            MessageBox(_T("设置流模式类型失败!"));
            return;
        }
        //打开流模式
        BOOL bRet = PlayM4_OpenStream(m_nPort,(PBYTE)buffVideHead,head_size,SOURCE_BUF_MAX);
        if (!bRet)
        {
            MessageBox(_T("打开流模式失败!"));
            return;
        }
        //设置解码回调
        //PlayM4_SetDecCallBackExMend(m_nPort,DecCBFun,NULL,0,NULL);
        if (!PlayM4_SetDisplayBuf(m_nPort,MAX_DIS_FRAMES))
        {
            MessageBox(_T("设置播放缓冲区最大缓冲帧数失败!"));
        }
        if(NULL == m_PlayBackShow.GetSafeHwnd() || !PlayM4_Play(m_nPort, m_PlayBackShow.GetSafeHwnd()))
        {
            DWORD m_ErrorCode = PlayM4_GetLastError(m_nPort);
            CString m_csErr;
            m_csErr.Format(_T("播放失败(错误代码:%d)"),m_ErrorCode);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        char buffer[MAX_PLAYDATA_LEN] = {0};
        m_bPlayBacking = true;
        while(m_bPlayBacking)
        {
            memset(buffer,0,MAX_PLAYDATA_LEN);
            int data_len = 0;
            ret = HCS_SDK_PlaybackData(m_nLogID, m_handlePlayback, buffer, MAX_PLAYDATA_LEN, &data_len);
            if(ret == HCS_OK && data_len != 0)
            {
                while (1)
                {
                    BOOL bFlag = PlayM4_InputData(m_nPort,(PBYTE)buffer,data_len);
                    if (bFlag == FALSE)
                    {
                        DWORD nError = PlayM4_GetLastError(m_nPort);
                        //若缓冲区满，则重复送入数据
                        if (nError == PLAYM4_BUF_OVER)
                        {
                            Sleep(200);
                            continue;
                        }
                        else
                        {
                            //CString m_csErr;
                            //m_csErr.Format(_T("输入流数据失败(错误代码:%d)"),nError);
                            //MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
                        }
                    }
                    //若送入成功，则继续读取数据送入到播放库缓冲
                    break; 
                }
            }
        }
        while(0 != PlayM4_GetSourceBufferRemain(m_nPort) && m_bPlayBacking)
        {
            continue;
        }
    }
}
void CVideoQueryAndDownload::OnBnClickedButtonRecordPlayback()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
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
            nItem = m_listVideoQuery.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("请先停止播放"));
        return;
    }
    //创建录像播放线程
    if(NULL == m_hThreadVideoPlayBack)
    {
        m_hThreadVideoPlayBack = (HANDLE)_beginthreadex(NULL,0,VideoPlayBack,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoPlayBack)
        {
            CString m_csErr;
            m_csErr.Format(_T("创建录像播放线程失败"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonStopRecordPlayback()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("没有登录设备"));
        return;
    }
    if (-1 == m_handlePlayback)
    {
        MessageBox(_T("未播放，无需停止"));
        return;
    }
    m_bPlayBacking = false;
    if (0 != PlayM4_GetSourceBufferRemain(m_nPort))
    {
        PlayM4_ResetSourceBuffer(m_nPort);
    }

    PlayM4_Stop(m_nPort);
    PlayM4_CloseStream(m_nPort);
    PlayM4_FreePort(m_nPort);
    m_nPort = -1;
    //
    int ret = HCS_SDK_PlaybackStop(m_nLogID, m_handlePlayback);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("停止播放失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    //
    m_handlePlayback = -1;
    if(NULL != m_hThreadVideoPlayBack)
    {
        ::CloseHandle(m_hThreadVideoPlayBack);
        m_hThreadVideoPlayBack = NULL;
    }
    m_PlayBackShow.Invalidate(TRUE);
    MessageBox(_T("停止回放成功"));
}

void CVideoQueryAndDownload::OnBnClickedButtonRecordPlaybackMod()
{
    // TODO: 在此添加控件通知处理程序代码
    //if (m_bPlayBacking || -1 != m_handlePlayback)
    //{
    //    MessageBox(_T("请先开启回放"));
    //    return;
    //}
    //CString csExplain;
    //if (0 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 2;
    //    m_nPlayBackModCout = m_nPlayBackModCout + 1;
    //    csExplain = _T("正常I帧回放");
    //}
    //else if (1 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 3;
    //    m_nPlayBackModCout = m_nPlayBackModCout + 1;
    //    csExplain = _T("倒序回放");
    //}
    //else if (2 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 1;
    //    m_nPlayBackModCout = 0;
    //    csExplain = _T("顺序回放");
    //}
    //else
    //{
    //    MessageBox(_T("回放切换失败-m_nPlayBackModCout>3"));
    //    return;
    //}
    //int nRet = HCS_SDK_PlaybackChangeMode(m_nLogID, m_handlePlayback,HCS_PlaybackMode(m_nPlayBackMod));
    //if (nRet != HCS_OK)
    //{
    //    CString m_csErr;
    //    m_csErr.Format(_T("回放切换失败(错误代码:%d)"),nRet);
    //    MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
    //    return;
    //}
    //CString m_csMsg;
    //m_csMsg.Format(_T("回放切换成%s成功!"),csExplain);
    //MessageBox(m_csMsg);
    m_bIsChangePlayMode = true;
    OnBnClickedButtonStopRecordPlayback();
    Sleep(200);
    OnBnClickedButtonRecordPlayback();
}
