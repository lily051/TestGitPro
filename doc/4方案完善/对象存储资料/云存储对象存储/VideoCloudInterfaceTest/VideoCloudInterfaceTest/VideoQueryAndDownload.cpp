// VideoQueryAndDownload.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoQueryAndDownload.h"


// CVideoQueryAndDownload �Ի���

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
    m_listVideoQuery.InsertColumn(0,_T("��ص�ID"),LVCFMT_CENTER,85);
    m_listVideoQuery.InsertColumn(1,_T("��ʼʱ��"),LVCFMT_CENTER,85);
    m_listVideoQuery.InsertColumn(2,_T("����ʱ��"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(3,_T("��������"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(4,_T("¼������"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(5,_T("����״̬"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(6,_T("ת������"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(7,_T("¼���С"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(8,_T("��Դ��"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(9,_T("�洢������IP���˿�"),LVCFMT_CENTER,60);
    m_listVideoQuery.InsertColumn(10,_T("����״̬"),LVCFMT_CENTER,60);
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// CVideoQueryAndDownload ��Ϣ�������

void CVideoQueryAndDownload::OnBnClickedButtonGethead()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ��һ��"));
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
        MessageBox(_T("��ǰδ��ѯ��¼��"));
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
        m_csErr.Format(_T("��ȡ�ļ�ͷ���ݳ���(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        CString m_Msg;
        m_Msg.Format(_T("��ȡ���ID:%s ��:%d¼��ͷ���ݣ��ҳ���:%d"),A2W(strCameraID.c_str()),nItem,head_size);
        MessageBox(m_Msg);
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonVideoQuery()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
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
        MessageBox(_T("��ѯ������¼�����ͷ�0��1��2"));
        return;
    }
    int count_total = 0;
    int handle = 0;
    int ret = HCS_SDK_RecSearchStart(m_nLogID,strCameraID.c_str(),
        begin_time,end_time,strRecType.c_str(),HCS_SRLOCK_TYPE_All,&count_total,&handle);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("����¼�����ʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
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
                m_csErr.Format(_T("��ȡ��һ��¼�����Ϣʧ��(�������:%d)"), ret);
                MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
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
        m_Msg.Format(_T("¼�񹲼���%d¼���"), count_total);
        MessageBox(m_Msg);
        ret= HCS_SDK_RecSearchStop(m_nLogID,handle);
        if (ret != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("ֹͣ¼�����ʧ��(�������:%d)"), ret);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
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
        //¼�����ݴ���
        FILE *fp = fopen(pDlg->m_FileName, "ab+");
        if (fp == NULL)
        {
            DWORD errCode = GetLastError();
            CString ErrMsg;
            ErrMsg.Format(_T("�ļ�%s������(������:%d)"), A2W(pDlg->m_FileName),errCode);
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
            //���������������
            //AfxMessageBox(_T("�������\r\n"));
            return;
        }
        else
        {
            //���س����쳣����
            //AfxMessageBox(_T("�����쳣\r\n"));
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
                m_csErr.Format(_T("ֹͣ¼������ʧ��(�������:%d)"), ret);
                MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            }
            MessageBox(_T("¼���������!"));
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
            m_csErr.Format(_T("¼�������쳣(�������:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            break;
        }
    }
    m_bDownLoading = false;
}
void CVideoQueryAndDownload::OnBnClickedButtonDownload()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
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
        MessageBox(_T("����ѡ��һ��"));
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
        MessageBox(_T("�������������ļ�����ȴ��������!"));
        return;
    }
    m_nFlag = -1;
    m_nErrorCode = 0;
    //
    std::vector<HCS_RecordInfo>::const_iterator iterRecord = VecRecordInfo.begin();
    if (iterRecord + nItem == VecRecordInfo.end())
    {
        MessageBox(_T("ѡ������¼���г�����Χ!"));
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
        m_csErr.Format(_T("����¼������ʧ��(�������:%d)"), ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    //
    m_bDownLoading = true;
    if(NULL != m_hThreadVideoDownLoad)
    {
        ::CloseHandle(m_hThreadVideoDownLoad);
        m_hThreadVideoDownLoad = NULL;
    }
    //����¼�����ؽ����߳�
    if(NULL == m_hThreadVideoDownLoad)
    {
        m_hThreadVideoDownLoad = (HANDLE)_beginthreadex(NULL,0,VideoDownLoad,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoDownLoad)
        {
            CString m_csErr;
            m_csErr.Format(_T("����¼�񲥷��߳�ʧ��"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonDelVideo()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ��һ��"));
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
            m_csErr.Format(_T("ɾ��¼��ʧ��(�������:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            return;
        }
        VecRecordInfo.erase(iterRecord + nItem);
        m_listVideoQuery.DeleteItem(nItem);
        MessageBox(_T("ɾ��¼��ɹ�!"));
    }
    else
    {
        MessageBox(_T("ѡ�е��г�����ѯ����ķ�Χ"));
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonQueryClear()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (!VecRecordInfo.empty())
    {
        VecRecordInfo.clear();
    }
    m_listVideoQuery.DeleteAllItems();
}

void CVideoQueryAndDownload::OnBnClickedButtonLockVideo()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ��һ��"));
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
            m_csErr.Format(_T("����¼��ʧ��(�������:%d)"), m_nErrorCode);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            return;
        }
        MessageBox(_T("����¼��ɹ�!"));
    }
    else
    {
        MessageBox(_T("ѡ�е��г�����ѯ����ķ�Χ�����޷�����"));
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
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ��һ��"));
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
        MessageBox(_T("������������ȴ�������Ϻ�ط�!"));
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
        //�Ƿ��л��ط�
        if (m_bIsChangePlayMode)
        {
            m_bIsChangePlayMode = false;
            CString csExplain;
            if (0 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 2;
                m_nPlayBackModCout = m_nPlayBackModCout + 1;
                csExplain = _T("����I֡�ط�");
            }
            else if (1 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 3;
                m_nPlayBackModCout = m_nPlayBackModCout + 1;
                csExplain = _T("����ط�");
            }
            else if (2 == m_nPlayBackModCout)
            {
                m_nPlayBackMod = 1;
                m_nPlayBackModCout = 0;
                csExplain = _T("˳��ط�");
            }
            else
            {
                MessageBox(_T("�ط��л�ʧ��-m_nPlayBackModCout>3"));
                return;
            }
            nRet = HCS_SDK_PlaybackChangeMode(m_nLogID, m_handlePlayback,HCS_PlaybackMode(m_nPlayBackMod));
            if (nRet != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("�ط��л�ʧ��(�������:%d)"),nRet);
                MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
                return;
            }
            CString m_csMsg;
            m_csMsg.Format(_T("�ط��л���%s�ɹ�!"),csExplain);
            MessageBox(m_csMsg);
        }

        //��ȡ¼��ͷ
        char buffVideHead[MAX_HEAD_LEN] = {0};
        memset(buffVideHead, 0, sizeof(buffVideHead));
        int head_size = 0;
        int ret = HCS_SDK_GetRecHead(m_nLogID, RecordInfo.camera_id, VecRecordInfo[nItem].begin_time, VecRecordInfo[nItem].end_time, buffVideHead,sizeof(buffVideHead), &head_size);
        if (nRet != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("��ȡ¼��ط�ͷʧ��(�������:%d)"),nRet);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            return;
        }
        //
        m_nPort = -1;
        if (!PlayM4_GetPort(&m_nPort))
        {
            MessageBox(_T("��ȡͨ����ʧ��!"));
            return;
        }
        //�������Ƹ����ſⲥ��
        if (!PlayM4_GetPort(&m_nPort))
        {
            MessageBox(_T("��ȡͨ����ʧ��!"));
            return;
        }
        //������ģʽ���� 
        if (!PlayM4_SetStreamOpenMode(m_nPort,STREAME_FILE))
        {
            MessageBox(_T("������ģʽ����ʧ��!"));
            return;
        }
        //����ģʽ
        BOOL bRet = PlayM4_OpenStream(m_nPort,(PBYTE)buffVideHead,head_size,SOURCE_BUF_MAX);
        if (!bRet)
        {
            MessageBox(_T("����ģʽʧ��!"));
            return;
        }
        //���ý���ص�
        //PlayM4_SetDecCallBackExMend(m_nPort,DecCBFun,NULL,0,NULL);
        if (!PlayM4_SetDisplayBuf(m_nPort,MAX_DIS_FRAMES))
        {
            MessageBox(_T("���ò��Ż�������󻺳�֡��ʧ��!"));
        }
        if(NULL == m_PlayBackShow.GetSafeHwnd() || !PlayM4_Play(m_nPort, m_PlayBackShow.GetSafeHwnd()))
        {
            DWORD m_ErrorCode = PlayM4_GetLastError(m_nPort);
            CString m_csErr;
            m_csErr.Format(_T("����ʧ��(�������:%d)"),m_ErrorCode);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
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
                        //���������������ظ���������
                        if (nError == PLAYM4_BUF_OVER)
                        {
                            Sleep(200);
                            continue;
                        }
                        else
                        {
                            //CString m_csErr;
                            //m_csErr.Format(_T("����������ʧ��(�������:%d)"),nError);
                            //MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
                        }
                    }
                    //������ɹ����������ȡ�������뵽���ſ⻺��
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listVideoQuery.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ��һ��"));
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
        MessageBox(_T("����ֹͣ����"));
        return;
    }
    //����¼�񲥷��߳�
    if(NULL == m_hThreadVideoPlayBack)
    {
        m_hThreadVideoPlayBack = (HANDLE)_beginthreadex(NULL,0,VideoPlayBack,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoPlayBack)
        {
            CString m_csErr;
            m_csErr.Format(_T("����¼�񲥷��߳�ʧ��"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoQueryAndDownload::OnBnClickedButtonStopRecordPlayback()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("û�е�¼�豸"));
        return;
    }
    if (-1 == m_handlePlayback)
    {
        MessageBox(_T("δ���ţ�����ֹͣ"));
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
        m_csErr.Format(_T("ֹͣ����ʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
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
    MessageBox(_T("ֹͣ�طųɹ�"));
}

void CVideoQueryAndDownload::OnBnClickedButtonRecordPlaybackMod()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //if (m_bPlayBacking || -1 != m_handlePlayback)
    //{
    //    MessageBox(_T("���ȿ����ط�"));
    //    return;
    //}
    //CString csExplain;
    //if (0 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 2;
    //    m_nPlayBackModCout = m_nPlayBackModCout + 1;
    //    csExplain = _T("����I֡�ط�");
    //}
    //else if (1 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 3;
    //    m_nPlayBackModCout = m_nPlayBackModCout + 1;
    //    csExplain = _T("����ط�");
    //}
    //else if (2 == m_nPlayBackModCout)
    //{
    //    m_nPlayBackMod = 1;
    //    m_nPlayBackModCout = 0;
    //    csExplain = _T("˳��ط�");
    //}
    //else
    //{
    //    MessageBox(_T("�ط��л�ʧ��-m_nPlayBackModCout>3"));
    //    return;
    //}
    //int nRet = HCS_SDK_PlaybackChangeMode(m_nLogID, m_handlePlayback,HCS_PlaybackMode(m_nPlayBackMod));
    //if (nRet != HCS_OK)
    //{
    //    CString m_csErr;
    //    m_csErr.Format(_T("�ط��л�ʧ��(�������:%d)"),nRet);
    //    MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
    //    return;
    //}
    //CString m_csMsg;
    //m_csMsg.Format(_T("�ط��л���%s�ɹ�!"),csExplain);
    //MessageBox(m_csMsg);
    m_bIsChangePlayMode = true;
    OnBnClickedButtonStopRecordPlayback();
    Sleep(200);
    OnBnClickedButtonRecordPlayback();
}
