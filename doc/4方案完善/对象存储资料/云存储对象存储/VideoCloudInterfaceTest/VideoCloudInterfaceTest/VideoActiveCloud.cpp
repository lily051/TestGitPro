// VideoActiveCloud.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoActiveCloud.h"


// CVideoActiveCloud �Ի���

IMPLEMENT_DYNAMIC(CVideoActiveCloud, CPropertyPage)

CVideoActiveCloud::CVideoActiveCloud()
	: CPropertyPage(CVideoActiveCloud::IDD)
    , m_csUser(_T("test"))
    , m_csPort(_T("8006"))
    , m_csPassWord(_T("1qaz2wsx"))
    , m_csCodeID(_T("10.16.36.211_8006_9"))
    , m_csCodeName(_T("Embedded Net DVR_9"))
    , m_csDevName(_T("Embedded Net DVR"))
    , m_csStreamType(_T("0"))
    , m_csChanNum(_T("10"))
    , m_csPoolID(_T("962270964"))
    , m_nLogID(-1)
    , m_taskID(-1)
    , m_ItemIndexInfo(0)
{

}

CVideoActiveCloud::~CVideoActiveCloud()
{
}

void CVideoActiveCloud::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS_PlAN, m_ctrlDevIp);
    DDX_Text(pDX, IDC_EDIT_USERNAME_PLAN, m_csUser);
    DDX_Text(pDX, IDC_EDIT_PORT_PLAN, m_csPort);
    DDX_Text(pDX, IDC_EDIT_PASSWORD_PLAN, m_csPassWord);
    DDX_Text(pDX, IDC_EDIT_CODEID_PLAN, m_csCodeID);
    DDX_Text(pDX, IDC_EDIT_CODENAME_PLAN, m_csCodeName);
    DDX_Text(pDX, IDC_EDIT_STREAMTYPE_PLAN, m_csStreamType);
    DDX_Text(pDX, IDC_EDIT_CHANNUM_PLAN, m_csChanNum);
    DDX_Text(pDX, IDC_EDIT_POOLID_PLAN, m_csPoolID);
    DDX_Control(pDX, IDC_DATE_START_PLAN, m_DateStart);
    DDX_Control(pDX, IDC_TIME_START_PLAN, m_TimeStart);
    DDX_Control(pDX, IDC_DATE_END_PLAN, m_DateEnd);
    DDX_Control(pDX, IDC_TIME_END_PLAN, m_TimeEnd);
    DDX_Text(pDX, IDC_EDIT_DEVICE_NAME, m_csDevName);
    DDX_Control(pDX, IDC_LIST_VIDEO_PLAN, m_ListVideoPlan);
}


BEGIN_MESSAGE_MAP(CVideoActiveCloud, CPropertyPage)
    ON_BN_CLICKED(IDC_BUTTON_START_RECORD, &CVideoActiveCloud::OnBnClickedButtonStartRecord)
    ON_BN_CLICKED(IDC_BUTTON_SET_VIDEOPLAN, &CVideoActiveCloud::OnBnClickedButtonSetVideoplan)
    ON_BN_CLICKED(IDC_BUTTON_GET_VIDEOSTATUS, &CVideoActiveCloud::OnBnClickedButtonGetVideostatus)
    ON_BN_CLICKED(IDC_BUTTON_STOP_RECORD, &CVideoActiveCloud::OnBnClickedButtonStopRecord)
    ON_BN_CLICKED(IDC_BUTTON_DEL_VIDEOPLAN, &CVideoActiveCloud::OnBnClickedButtonDelVideoplan)
    ON_BN_CLICKED(IDC_BUTTON_QURY_VIDEOPLAN, &CVideoActiveCloud::OnBnClickedButtonQuryVideoplan)
    ON_BN_CLICKED(IDC_BUTTON_SET_HISPLAN, &CVideoActiveCloud::OnBnClickedButtonSetHisplan)
    ON_BN_CLICKED(IDC_BUTTON_DEL_HISPLAN, &CVideoActiveCloud::OnBnClickedButtonDelHisplan)
    ON_BN_CLICKED(IDC_BUTTON_GET_HISPLAN, &CVideoActiveCloud::OnBnClickedButtonGetHisplan)
    ON_BN_CLICKED(IDC_BUTTON_CURPLAN_QUERY, &CVideoActiveCloud::OnBnClickedButtonCurplanQuery)
    ON_BN_CLICKED(IDC_BUTTON_HISPLAN_QUERY, &CVideoActiveCloud::OnBnClickedButtonHisplanQuery)
    ON_BN_CLICKED(IDC_BUTTON_EVENT_RECORD, &CVideoActiveCloud::OnBnClickedButtonEventRecord)
END_MESSAGE_MAP()

BOOL CVideoActiveCloud::OnInitDialog()
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
    m_ctrlDevIp.SetAddress(10,16,36,211);
    CTime timeDataStart(2000,1,1,0,0,0); 
    CDateTimeCtrl* pCtrlTimeStart = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_START_PLAN);
    ASSERT(pCtrlTimeStart != NULL);
    VERIFY(pCtrlTimeStart->SetTime(&timeDataStart));
    CTime timeDataStop(2000,1,1,23,59,59); 
    CDateTimeCtrl* pCtrlTimeStop = (CDateTimeCtrl*)GetDlgItem(IDC_TIME_END_PLAN);
    ASSERT(pCtrlTimeStop != NULL);
    VERIFY(pCtrlTimeStop->SetTime(&timeDataStop));
    //
    m_ListVideoPlan.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
    m_ListVideoPlan.InsertColumn(0,_T("��ص�ID"),LVCFMT_CENTER,85);
    m_ListVideoPlan.InsertColumn(1,_T("��ص�����"),LVCFMT_CENTER,85);
    m_ListVideoPlan.InsertColumn(2,_T("�豸IP"),LVCFMT_CENTER,60);
    m_ListVideoPlan.InsertColumn(3,_T("VAG_IP"),LVCFMT_CENTER,60);
    m_ListVideoPlan.InsertColumn(4,_T("��ý��IP"),LVCFMT_CENTER,60);
    m_ListVideoPlan.InsertColumn(5,_T("¼���ID"),LVCFMT_CENTER,60);
    m_ListVideoPlan.InsertColumn(6,_T("¼��״̬"),LVCFMT_CENTER,60);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// CVideoActiveCloud ��Ϣ�������

void CVideoActiveCloud::OnBnClickedButtonStartRecord()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    CString DeviceIp;
    BYTE nField0,nField1,nField2,nField3;
    m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
    DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
    std::string strDeviceIp = W2A(DeviceIp);
    std::string strUserName = W2A(m_csUser);
    std::string strPassword = W2A(m_csPassWord);
    std::string strDevName = W2A(m_csDevName);
    std::string strCameraID = W2A(m_csCodeID);
    std::string strCameraName = W2A(m_csCodeName);
    std::string strPoolID = W2A(m_csPoolID);

    //
    CTime StartDay,StartTime;
    CTime StopDay,StopTime;
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);
    //����¼����Ϣ
    HCS_HisRecInfo struRecordInfo;
    memset(&struRecordInfo, 0, sizeof(HCS_HisRecInfo));

    memcpy_s(struRecordInfo.camera_id, HCS_CAMERA_ID_LEN,strCameraID.c_str(), strlen(strCameraID.c_str()));
    struRecordInfo.rec_type = 0;     //¼������,0��ʾ��ʱ¼���������Ͳ�������

    struRecordInfo.detail.enable = 1;//0-��ʹ�ã���ʹ��¼��ƻ�ģ���е���Ϣ,1-ʹ��
    struRecordInfo.detail.replication = 0;
    memcpy_s(struRecordInfo.detail.user_name, HCS_USERNAME_LEN,strUserName.c_str(), strlen(strUserName.c_str()));
    memcpy_s(struRecordInfo.detail.password, HCS_PASSWORD_LEN,strPassword.c_str(), strlen(strPassword.c_str()));
    memcpy_s(struRecordInfo.detail.pool_id, HCS_POOL_ID_LEN,strPoolID.c_str(), strlen(strPoolID.c_str()));
    //¼��ʼʱ��
    struRecordInfo.begin_time.year = StartDay.GetYear();
    struRecordInfo.begin_time.month = StartDay.GetMonth();
    struRecordInfo.begin_time.day = StartDay.GetDay();
    struRecordInfo.begin_time.hour = StartTime.GetHour();
    struRecordInfo.begin_time.minute = StartTime.GetMinute();
    struRecordInfo.begin_time.second = StartTime.GetSecond();
    //¼�����ʱ��

    struRecordInfo.end_time.year = StopDay.GetYear();
    struRecordInfo.end_time.month = StopDay.GetMonth();
    struRecordInfo.end_time.day = StopDay.GetDay();
    struRecordInfo.end_time.hour = StopTime.GetHour();
    struRecordInfo.end_time.minute = StopTime.GetMinute();
    struRecordInfo.end_time.second = StopTime.GetSecond();
    //�·�¼���ֶ���¼����
    char szUrl[1024] = {0};
    int nRet = HCS_SDK_HistoryRecord(m_nLogID, &struRecordInfo, sizeof(szUrl), szUrl, &m_taskID);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("�ֶ�¼��ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        CString m_csShow;
        m_csShow.Format(_T("�ֶ�¼��ɹ�(szUrl:%s��m_taskID:%d)"),A2W(szUrl),m_taskID);
        MessageBox(m_csShow);
    }
}

void CVideoActiveCloud::OnBnClickedButtonSetVideoplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    HCS_RecordPlan struRecPlan;
    memset(&struRecPlan, 0, sizeof(HCS_RecordPlan));

    CString DeviceIp;
    BYTE nField0,nField1,nField2,nField3;
    m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
    DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
    std::string strDeviceIp = W2A(DeviceIp);
    std::string strUserName = W2A(m_csUser);
    std::string strPassword = W2A(m_csPassWord);
    std::string strDevName = W2A(m_csDevName);
    std::string strCameraID = W2A(m_csCodeID);
    std::string strCameraName = W2A(m_csCodeName);
    std::string strPoolID = W2A(m_csPoolID);

    //
    CTime StartDay,StartTime;
    CTime StopDay,StopTime;
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);
    //-1
    struRecPlan.stream_mode = _ttoi(m_csStreamType);
    memcpy_s(struRecPlan.camera_id, HCS_CAMERA_ID_LEN, strCameraID.c_str(), strlen(strCameraID.c_str()));//������ID
    memcpy_s(struRecPlan.camera_name, HCS_CAMERA_ID_LEN, strCameraName.c_str(), strlen(strCameraName.c_str()));//����������
    memcpy_s(struRecPlan.pool_id, HCS_POOL_ID_LEN, strPoolID.c_str(), strlen(strPoolID.c_str()));
    //-2 ȡ����Ϣ
    //ȡ���豸��Ϣ
    memcpy_s(struRecPlan.stream_info.stream_dev.device.name, HCS_CAMERA_NAME_LEN,
        strDevName.c_str(), strlen(strDevName.c_str()));
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.ip, HCS_DEVICE_IP_LEN, 
        strDeviceIp.c_str(), strlen(strDeviceIp.c_str()));
    struRecPlan.stream_info.stream_dev.device.endpoint.port = _ttoi(m_csPort);
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.username, HCS_USERNAME_LEN, 
        strUserName.c_str(), strlen(strUserName.c_str()));
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.password, HCS_PASSWORD_LEN, 
        strPassword.c_str(), strlen(strPassword.c_str()));
    struRecPlan.stream_info.stream_dev.channel = _ttoi(m_csChanNum);//DVR�豸ͨ����
    struRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.port = 554;//
    memcpy(struRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.ip, "127.0.0.1", HCS_DEVICE_IP_LEN);
    struRecPlan.stream_info.stream_dev.device.protocol = 0;//hik
    // ¼��ƻ�ִ��ʱ����Ϣϸ��
    struRecPlan.detail.record = 1;
    struRecPlan.detail.stream_type = 0;
    for (int i = 0; i < HCS_DAYS_PER_WEEK; i++)
    {
        struRecPlan.detail.record_allday[i] = (int)0;  //�Ƿ�ȫ��¼��
        CTime ctRecordStart((int)2016, (int)1, (int)1, (int)(StartTime.GetHour()), (int)(StartTime.GetMinute()), (int)0);
        CTime ctRecordEnd((int)2016, (int)1, (int)1, (int)(StopTime.GetHour()), (int)(StopTime.GetMinute()), (int)0);
        for (int i = 0; i < HCS_DAYS_PER_WEEK; i++)
        {
            struRecPlan.detail.record_allday[i] = (int)0;  //�Ƿ�ȫ��¼��
            if (ctRecordStart > ctRecordEnd)  //��0�����
            {
                //0�㵽����ʱ��-��һ��ʱ��
                struRecPlan.detail.record_time[i][0].start_hour = (int)0;
                struRecPlan.detail.record_time[i][0].stop_hour = (int)(StopTime.GetHour());
                struRecPlan.detail.record_time[i][0].start_min = (int)0;
                struRecPlan.detail.record_time[i][0].stop_min = (int)(StopTime.GetMinute());
                ////��ʼʱ�䵽23:59-�ڶ���ʱ��
                //struRecPlan.detail.record_time[i][1].start_hour = (int)(StartTime.GetHour());
                //struRecPlan.detail.record_time[i][1].stop_hour = (int)23;
                //struRecPlan.detail.record_time[i][1].start_min = (int)(StartTime.GetMinute());
                //struRecPlan.detail.record_time[i][1].stop_min = (int)59;
                for (int j = 2; j < HCS_PLAN_SEGMENT_COUNT; j++)
                {
                    struRecPlan.detail.record_time[i][j].start_hour = (int)0;
                    struRecPlan.detail.record_time[i][j].stop_hour = (int)0;
                    struRecPlan.detail.record_time[i][j].start_min = (int)0;
                    struRecPlan.detail.record_time[i][j].stop_min = (int)0;
                }
            }
            else  //����0�����
            {
                struRecPlan.detail.record_time[i][0].start_hour = (int)(StartTime.GetHour());
                struRecPlan.detail.record_time[i][0].stop_hour = (int)(StopTime.GetHour());
                struRecPlan.detail.record_time[i][0].start_min = (int)(StartTime.GetMinute());
                struRecPlan.detail.record_time[i][0].stop_min = (int)(StopTime.GetMinute());
                for (int j = 1; j < HCS_PLAN_SEGMENT_COUNT; j++)
                {
                    struRecPlan.detail.record_time[i][j].start_hour = (int)0;
                    struRecPlan.detail.record_time[i][j].stop_hour = (int)0;
                    struRecPlan.detail.record_time[i][j].start_min = (int)0;
                    struRecPlan.detail.record_time[i][j].stop_min = (int)0;
                }
            }
        }
    }
    struRecPlan.detail.pre_record = 0;
    struRecPlan.detail.replication = 0;

    int nRet = HCS_SDK_SetRecPlan(m_nLogID, &struRecPlan);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("����¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("����¼��ƻ��ɹ�"));
    }
}

void CVideoActiveCloud::OnBnClickedButtonDelVideoplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_csCodeID);
    int nRet = HCS_SDK_DelRecPlan(m_nLogID, strCameraID.c_str());
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("ɾ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("ɾ��¼��ƻ��ɹ�"));
    }
}
void CVideoActiveCloud::OnBnClickedButtonQuryVideoplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_csCodeID);

    int status = 0;//¼��ƻ��·�״̬[0-δ�·���1-���·�]
    HCS_RecordPlan strRecordPlan;
    memset(&strRecordPlan, 0, sizeof(HCS_RecordPlan));
    int nRet =  HCS_SDK_GetRecPlan(m_nLogID, strCameraID.c_str(), &strRecordPlan, &status);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ȡ��������Ӧ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("��ȡ��������Ӧ��¼��ƻ��ɹ�"));
    }
}
void CVideoActiveCloud::OnBnClickedButtonGetVideostatus()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    if (-1 == m_taskID)
    {
        MessageBox(_T("�����·���¼����"));
        return;
    }
    HCS_TaskInfo strTaskInfo;
    memset(&strTaskInfo, 0, sizeof(HCS_TaskInfo));
    int nRet =  HCS_SDK_HistoryRecordStatus(m_nLogID, m_taskID, &strTaskInfo);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ȡ�ֶ���¼״̬ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        CString m_Msg;
        m_Msg.Format(_T("��ȡ�ֶ���¼״̬-����ID:%d,��������:%d,����״̬:%d,�������:%d"),
            strTaskInfo.task_id,strTaskInfo.task_type,strTaskInfo.task_status,strTaskInfo.task_speed);
        MessageBox(m_Msg);
    }
}

void CVideoActiveCloud::OnBnClickedButtonStopRecord()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    if (-1 == m_taskID)
    {
        MessageBox(_T("�����·���¼����"));
        return;
    }
    int nRet =  HCS_SDK_HistoryRecordStop(m_nLogID, m_taskID);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("ֹͣ�ֶ���¼ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        m_taskID = -1;
        MessageBox(_T("ֹͣ�ֶ���¼�ɹ�"));
    }
}

void CVideoActiveCloud::OnBnClickedButtonSetHisplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    CString DeviceIp;
    BYTE nField0,nField1,nField2,nField3;
    m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
    DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
    std::string strDeviceIp = W2A(DeviceIp);
    std::string strUserName = W2A(m_csUser);
    std::string strPassword = W2A(m_csPassWord);
    std::string strDevName = W2A(m_csDevName);
    std::string strCameraID = W2A(m_csCodeID);
    std::string strCameraName = W2A(m_csCodeName);
    std::string strPoolID = W2A(m_csPoolID);

    //
    CTime StartDay,StartTime;
    CTime StopDay,StopTime;
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);

    HCS_HisRecPlan strHisRecPlan;
    memset(&strHisRecPlan, 0, sizeof(HCS_HisRecPlan));
    memcpy_s(strHisRecPlan.camera_id, HCS_CAMERA_ID_LEN, strCameraID.c_str(), strlen(strCameraID.c_str()));
    memcpy_s(strHisRecPlan.camera_name, HCS_CAMERA_NAME_LEN, strCameraName.c_str(), strlen(strCameraName.c_str()));
    strHisRecPlan.stream_mode = _ttoi(m_csStreamType);
    memcpy_s(strHisRecPlan.pool_id, HCS_POOL_ID_LEN, strPoolID.c_str(), strlen(strPoolID.c_str()));
    strHisRecPlan.replication = 0;
    //
    memcpy_s(strHisRecPlan.stream_info.stream_dev.device.endpoint.ip,HCS_DEVICE_IP_LEN,strDeviceIp.c_str(),strlen(strDeviceIp.c_str()));
    strHisRecPlan.stream_info.stream_dev.device.endpoint.port = _ttoi(m_csPort);
    memcpy_s(strHisRecPlan.stream_info.stream_dev.device.endpoint.username, HCS_USERNAME_LEN, 
        strUserName.c_str(), strlen(strUserName.c_str()));
    memcpy_s(strHisRecPlan.stream_info.stream_dev.device.endpoint.password, HCS_PASSWORD_LEN, 
        strPassword.c_str(), strlen(strPassword.c_str()));
    strHisRecPlan.stream_info.stream_dev.channel = _ttoi(m_csChanNum);//DVR�豸ͨ����
    //strHisRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.port = 554;
    //
    for (int i = 0; i < HCS_DAYS_PER_WEEK; i++)
    {
        strHisRecPlan.detail.history_stream_allday[i] = 0;
        strHisRecPlan.detail.replenish_allday[i] = 0;
        for (int j = 0; j < HCS_PLAN_SEGMENT_COUNT; j++)
        {
            strHisRecPlan.detail.history_stream_time[i][j].start_hour = StartTime.GetHour();
            strHisRecPlan.detail.history_stream_time[i][j].start_min = StartTime.GetMinute();
            strHisRecPlan.detail.history_stream_time[i][j].stop_hour = StopTime.GetHour();
            strHisRecPlan.detail.history_stream_time[i][j].stop_min = StopTime.GetMinute();
            //
            strHisRecPlan.detail.replenish_time[i][j].start_hour = StartTime.GetHour();
            strHisRecPlan.detail.replenish_time[i][j].start_min = StartTime.GetMinute();
            strHisRecPlan.detail.replenish_time[i][j].stop_hour = StopTime.GetHour();
            strHisRecPlan.detail.replenish_time[i][j].stop_min = StopTime.GetMinute();
        }
    }
    strHisRecPlan.detail.date.year = 0;
    strHisRecPlan.detail.date.month = 0;
    strHisRecPlan.detail.date.day = 0;
    int nRet = HCS_SDK_SetHisRecPlan(m_nLogID, &strHisRecPlan);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("������ʷ���洢�ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("����¼��ƻ��ɹ�"));
    }

}

void CVideoActiveCloud::OnBnClickedButtonDelHisplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_csCodeID);

    int nRet =  HCS_SDK_DelHisRecPlan(m_nLogID, strCameraID.c_str());
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("ɾ����ʷ���洢�ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("ɾ����ʷ���洢�ƻ��ɹ�"));
    }
}

void CVideoActiveCloud::OnBnClickedButtonGetHisplan()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    UpdateData(TRUE);
    std::string strCameraID = W2A(m_csCodeID);

    int status = -1;
    HCS_HisRecPlan strHisRecPlan;
    memset(&strHisRecPlan, 0, sizeof(HCS_HisRecPlan));
    int nRet =  HCS_SDK_GetHisRecPlan(m_nLogID, strCameraID.c_str(), &strHisRecPlan, &status);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ȡ��������Ӧ����ʷ���洢�ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("��ȡ��������Ӧ����ʷ���洢�ƻ��ɹ�"));
    }

}

void CVideoActiveCloud::OnBnClickedButtonCurplanQuery()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    m_ItemIndexInfo = 0;
    m_ListVideoPlan.DeleteAllItems();
    int nCount = 0;
    int hHandle = 0;
    int nRet = HCS_SDK_GetAllRecPlanStart(m_nLogID, &nCount, &hHandle);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ʼ��ȡȫ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    int nAlloc = -1;
    int nStatus = -1;
    HCS_RecordPlan_V20 strRecordPlan_V20;
    while(1)
    {
        nAlloc = -1;
        nStatus = -1;
        memset(&strRecordPlan_V20, 0, sizeof(HCS_RecordPlan_V20));
        nRet = HCS_SDK_GetAllRecPlanNext_V20(m_nLogID, hHandle, &strRecordPlan_V20, &nAlloc, &nStatus);
        if (1 == nStatus)
        {
            CString csCameraId,csCameraName,csDeviceIp,csVagIp,csVtduIp,csPoolId,csAlloc;
            csCameraId = A2W(strRecordPlan_V20.camera_id);
            csCameraName = A2W(strRecordPlan_V20.camera_name);
            csDeviceIp = A2W(strRecordPlan_V20.stream_info.stream_dev.device.endpoint.ip);
            csVagIp = A2W(strRecordPlan_V20.stream_info.stream_dev.svc_vag.endpoint.ip);
            csVtduIp = A2W(strRecordPlan_V20.stream_info.stream_dev.svc_vtdu.endpoint.ip);
            csPoolId = A2W(strRecordPlan_V20.pool_id);
            csAlloc.Format(_T("%d"),nAlloc);
            m_ListVideoPlan.InsertItem(m_ItemIndexInfo,csCameraId);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,1,csCameraName);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,2,csDeviceIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,3,csVagIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,4,csVtduIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,5,csPoolId);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,6,csAlloc);
            m_ItemIndexInfo++;
        }
        else if (2 == nStatus)
        {
            Sleep(200);
            continue;
        }
        else if (3 == nStatus)
        {
            MessageBox(_T("��ѯ¼��ƻ�����"));
            break;
        }
        else
        {
            MessageBox(_T("��ѯ¼��ƻ��쳣"));
            break;
        }
    }
    nRet = HCS_SDK_GetAllRecPlanStop(m_nLogID, hHandle);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("ֹͣ��ȡȫ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
}
void CVideoActiveCloud::OnBnClickedButtonHisplanQuery()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼��"));
        return;
    }
    USES_CONVERSION;
    m_ItemIndexInfo = 0;
    m_ListVideoPlan.DeleteAllItems();
    int nCount = 0;
    int hHandle = 0;
    int nRet = HCS_SDK_GetAllHisRecPlanStart(m_nLogID, &nCount, &hHandle);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ʼ��ȡȫ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    int nAlloc = -1;
    int nStatus = -1;
    HCS_HisRecPlan strHisRecPlan;
    while(1)
    {
        nAlloc = -1;
        nStatus = -1;
        memset(&strHisRecPlan, 0, sizeof(HCS_HisRecPlan));
        nRet = HCS_SDK_GetAllHisRecPlanNext(m_nLogID, hHandle, &strHisRecPlan, &nAlloc, &nStatus);
        if (1 == nStatus)
        {
            CString csCameraId,csCameraName,csDeviceIp,csVagIp,csVtduIp,csPoolId,csAlloc;
            csCameraId = A2W(strHisRecPlan.camera_id);
            csCameraName = A2W(strHisRecPlan.camera_name);
            csDeviceIp = A2W(strHisRecPlan.stream_info.stream_dev.device.endpoint.ip);
            csVagIp = A2W(strHisRecPlan.stream_info.stream_dev.svc_vag.endpoint.ip);
            csVtduIp = A2W(strHisRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.ip);
            csPoolId = A2W(strHisRecPlan.pool_id);
            csAlloc.Format(_T("%d"),nAlloc);
            m_ListVideoPlan.InsertItem(m_ItemIndexInfo,csCameraId);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,1,csCameraName);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,2,csDeviceIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,3,csVagIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,4,csVtduIp);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,5,csPoolId);
            m_ListVideoPlan.SetItemText(m_ItemIndexInfo,6,csAlloc);
            m_ItemIndexInfo++;
        }
        else if (2 == nStatus)
        {
            Sleep(200);
            continue;
        }
        else if (3 == nStatus)
        {
            MessageBox(_T("��ѯ¼��ƻ�����"));
            break;
        }
        else
        {
            MessageBox(_T("��ѯ¼��ƻ��쳣"));
            break;
        }
    }
    nRet = HCS_SDK_GetAllRecPlanStop(m_nLogID, hHandle);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("ֹͣ��ȡȫ��¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
}

void CVideoActiveCloud::OnBnClickedButtonEventRecord()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    UpdateData(TRUE);
    HCS_RecordPlan struRecPlan;
    memset(&struRecPlan, 0, sizeof(HCS_RecordPlan));

    CString DeviceIp;
    BYTE nField0,nField1,nField2,nField3;
    m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
    DeviceIp.Format(_T("%d.%d.%d.%d"),nField0,nField1,nField2,nField3);
    std::string strDeviceIp = W2A(DeviceIp);
    std::string strUserName = W2A(m_csUser);
    std::string strPassword = W2A(m_csPassWord);
    std::string strDevName = W2A(m_csDevName);
    std::string strCameraID = W2A(m_csCodeID);
    std::string strCameraName = W2A(m_csCodeName);
    std::string strPoolID = W2A(m_csPoolID);

    //
    CTime StartDay,StartTime;
    CTime StopDay,StopTime;
    m_DateStart.GetTime(StartDay);
    m_TimeStart.GetTime(StartTime);
    m_DateEnd.GetTime(StopDay);
    m_TimeEnd.GetTime(StopTime);
    //-1
    struRecPlan.stream_mode = _ttoi(m_csStreamType);
    memcpy_s(struRecPlan.camera_id, HCS_CAMERA_ID_LEN, strCameraID.c_str(), strlen(strCameraID.c_str()));//������ID
    memcpy_s(struRecPlan.camera_name, HCS_CAMERA_ID_LEN, strCameraName.c_str(), strlen(strCameraName.c_str()));//����������
    memcpy_s(struRecPlan.pool_id, HCS_POOL_ID_LEN, strPoolID.c_str(), strlen(strPoolID.c_str()));
    //-2 ȡ����Ϣ
    //ȡ���豸��Ϣ
    memcpy_s(struRecPlan.stream_info.stream_dev.device.name, HCS_CAMERA_NAME_LEN,
        strDevName.c_str(), strlen(strDevName.c_str()));
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.ip, HCS_DEVICE_IP_LEN, 
        strDeviceIp.c_str(), strlen(strDeviceIp.c_str()));
    struRecPlan.stream_info.stream_dev.device.endpoint.port = _ttoi(m_csPort);
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.username, HCS_USERNAME_LEN, 
        strUserName.c_str(), strlen(strUserName.c_str()));
    memcpy_s(struRecPlan.stream_info.stream_dev.device.endpoint.password, HCS_PASSWORD_LEN, 
        strPassword.c_str(), strlen(strPassword.c_str()));
    struRecPlan.stream_info.stream_dev.channel = _ttoi(m_csChanNum);//DVR�豸ͨ����
    struRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.port = 554;//
    memcpy(struRecPlan.stream_info.stream_dev.svc_vtdu.endpoint.ip, "127.0.0.1", HCS_DEVICE_IP_LEN);
    struRecPlan.stream_info.stream_dev.device.protocol = 0;//hik
    // ¼��ƻ�ִ��ʱ����Ϣϸ��
    struRecPlan.detail.record = 1;
    struRecPlan.detail.stream_type = 0;
    for (int i = 0; i < HCS_DAYS_PER_WEEK; i++)
    {
        struRecPlan.detail.record_allday[i] = (int)0;  //�Ƿ�ȫ��¼��
        for (int j = 1; j < HCS_PLAN_SEGMENT_COUNT; j++)
        {
            struRecPlan.detail.record_time[i][j].start_hour = (int)0;
            struRecPlan.detail.record_time[i][j].stop_hour = (int)0;
            struRecPlan.detail.record_time[i][j].start_min = (int)0;
            struRecPlan.detail.record_time[i][j].stop_min = (int)0;
        }
    }
    struRecPlan.detail.pre_record = 0;
    struRecPlan.detail.replication = 0;

    int nRet = HCS_SDK_SetRecPlan(m_nLogID, &struRecPlan);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("����¼��ƻ�ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    nRet = HCS_SDK_EventRecord(m_nLogID, strCameraID.c_str(), 0, 10);
    if (HCS_OK != nRet)
    {
        CString m_csErr;
        m_csErr.Format(_T("�����¼�¼��ʧ��(�������:%d)"),nRet);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    else
    {
        MessageBox(_T("�����¼�¼��ɹ�"));
    }

}
