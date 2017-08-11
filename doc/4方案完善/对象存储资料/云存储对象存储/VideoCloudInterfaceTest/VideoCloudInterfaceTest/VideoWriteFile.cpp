// VideoWriteFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoWriteFile.h"


// CVideoWriteFile �Ի���
IMPLEMENT_DYNAMIC(CVideoWriteFile, CPropertyPage)

CVideoWriteFile::CVideoWriteFile()
	: CPropertyPage(CVideoWriteFile::IDD)
    , m_camerID(_T("10.16.37.105_8000_32"))
    , m_poolID(_T("962270964"))
    , m_repoolID(_T(""))
    , m_nReNum(0)
    , m_nType(4)
    , m_nLogID(-1)
    , m_write_handle(-1)
    , m_ItemIndex(0)
    , m_ItemIndexInfo(0)
    , m_download_error_code(0)
    , m_download_flag(-1)
    , m_writefile_speed(0)
    , m_writefile_error_code(0)
    , m_handlePlayback(-1)
    , m_handleDownLoad(-1)
    , m_nPort(-1)
    , m_bPlayBacking(true)
    , m_hThreadVideoPlayBack(NULL)
{

}

CVideoWriteFile::~CVideoWriteFile()
{
    if (!ary_filename.IsEmpty())
    {
        ary_filename.RemoveAll();
    }
    if (!ary_fileTitle.IsEmpty())
    {
        ary_fileTitle.RemoveAll();
    }
}

void CVideoWriteFile::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_CODER_ID, m_camerID);
    DDX_Text(pDX, IDC_EDIT_VIDEOPOOL_ID, m_poolID);
    DDX_Text(pDX, IDC_EDIT_VIDEOPOOL_ID_RD, m_repoolID);
    DDX_Text(pDX, IDC_EDIT_FILE_TYPE, m_nType);
    DDX_Text(pDX, IDC_EDIT_RD_NUM, m_nReNum);
    DDX_Control(pDX, IDC_LIST, m_listFileInfo);
    DDX_Control(pDX, IDC_STATIC_PLAYBACK, m_PlayShow);
    DDX_Control(pDX, IDC_LIST_FILE_FIND, m_listInfo);
}


BEGIN_MESSAGE_MAP(CVideoWriteFile, CPropertyPage)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, &CVideoWriteFile::OnBnClickedButtonSelectFile)
    ON_BN_CLICKED(IDC_BUTTON_START_WRITE_ASY, &CVideoWriteFile::OnBnClickedButtonStartWriteAsy)
    ON_BN_CLICKED(IDC_BUTTON_START_WRITE_SYN, &CVideoWriteFile::OnBnClickedButtonStartWriteSyn)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CVideoWriteFile::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_DOWNLOAD, &CVideoWriteFile::OnBnClickedButtonVideoDownload)

    ON_BN_CLICKED(IDC_BUTTON_PLAYBACK, &CVideoWriteFile::OnBnClickedButtonPlayback)
    ON_BN_CLICKED(IDC_BUTTON_PALYBACK_STOP, &CVideoWriteFile::OnBnClickedButtonPalybackStop)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_FIND, &CVideoWriteFile::OnBnClickedButtonVideoFind)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_DELETE, &CVideoWriteFile::OnBnClickedButtonVideoDelete)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_READ, &CVideoWriteFile::OnBnClickedButtonVideoRead)
    ON_BN_CLICKED(IDC_BUTTON_INDEX_SEARCH, &CVideoWriteFile::OnBnClickedButtonIndexSearch)
END_MESSAGE_MAP()

BOOL CVideoWriteFile::OnInitDialog()
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

    m_listFileInfo.InsertColumn(0,_T("�ļ�Key"),LVCFMT_CENTER,60);
    m_listFileInfo.InsertColumn(1,_T("Url"),LVCFMT_CENTER,40);
    m_listFileInfo.InsertColumn(2,_T("Result"),LVCFMT_CENTER,60);
    m_listFileInfo.InsertColumn(3,_T("д������"),LVCFMT_CENTER,60);
    //
    m_listInfo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    m_listInfo.InsertColumn(0,_T("�ļ���"),LVCFMT_CENTER,175);
    m_listInfo.InsertColumn(1,_T("��С"),LVCFMT_CENTER,90);
    m_listInfo.InsertColumn(2,_T("��������"),LVCFMT_CENTER,90);
    m_listInfo.InsertColumn(3,_T("֡��"),LVCFMT_CENTER,90);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// CVideoWriteFile ��Ϣ�������

void CVideoWriteFile::OnBnClickedButtonSelectFile()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ary_filename.RemoveAll();
    ary_fileTitle.RemoveAll();
    GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(_T(""));

    CString strFilesPathShow = _T("");
    CString pathName,fileName,fileTitle;
    WCHAR* filters = _T("mp4�ļ�(*.mp4)|*.mp4");
    //����һ������ѡ�����ļ���CFileDialog
    CFileDialog fileDlg(TRUE,NULL,_T("*.mp4"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,filters);
    //�����ļ������趨 100
    fileDlg.m_ofn.nMaxFile = 100 * MAX_PATH;
    WCHAR* ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
    fileDlg.m_ofn.lpstrFile = ch;
    //���ڴ������
    ZeroMemory(fileDlg.m_ofn.lpstrFile,sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
    //��ʾ�ļ��Ի��򣬻�ȡ�ļ�������
    if (fileDlg.DoModal() == IDOK)
    {
        //��ȡ��һ���ļ���λ��
        POSITION pos_file;
        pos_file = fileDlg.GetStartPosition();
        //ѭ������ÿ��·���������������
        while(pos_file != NULL)
        {
            CString strTmp = _T("");
            //�ļ�·�������������
            pathName = fileDlg.GetNextPathName(pos_file);
            ary_filename.Add(pathName);
            //
            strTmp = pathName;
            strTmp += _T(";");
            strFilesPathShow += strTmp;
            //��ȡ�ļ���
            //���ַ����ĺ�����ǰ�������������'\'���������,'\'�ұߵ��ַ�����Ϊ�ļ���
            int length = pathName.GetLength();
            for (int i = 0; i > 0; i--)
            {
                if ('\\' == pathName.GetAt(i))
                {//�жϵ�ǰ�ַ��Ƿ���'\'
                    fileName = pathName.Right(length - i - 1);
                    break;
                }
            }
            //��ȡ�ļ���(��������׺)
            fileTitle = fileName.Left(fileName.GetLength() - 4); //����CString��Left(int count)��ȡCString�д�����������count���ַ� 4��ʾ".wav"�ĸ��ַ�
            ary_fileTitle.Add(fileTitle);//���ļ���(��������׺)��ӵ�������
        }
    }
    GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(strFilesPathShow);
    GetDlgItem(IDC_EDIT_CODER_ID)->GetWindowText(m_camerID);
    GetDlgItem(IDC_EDIT_VIDEOPOOL_ID)->GetWindowText(m_poolID);
    GetDlgItem(IDC_EDIT_VIDEOPOOL_ID_RD)->GetWindowText(m_repoolID);
    CString csType,csReNum;
    GetDlgItem(IDC_EDIT_RD_NUM)->GetWindowText(csReNum);
    GetDlgItem(IDC_EDIT_FILE_TYPE)->GetWindowText(csType);
    m_nReNum = _wtoi(csReNum);
    m_nType = _wtoi(csType);
    UpdateData(FALSE);
    delete [] ch;
}
void CVideoWriteFile::SetLoginID(unsigned long long nLogID)
{
    m_nLogID = nLogID;
}
//�ص�������������Ƶ�ļ�д��Ľ��Ⱥ�״̬��Ϣ
void UpLoadFileSpeedCB(int handle, int file_speed, int error_code, void* user_data)
{
    //�ڴ˺����в��ܵ���д��Ƶ�ļ���ֹͣ�ӿ�
    CVideoWriteFile* pDownload = (CVideoWriteFile*)user_data;
    if (pDownload != NULL)
    {
        pDownload->m_writefile_speed = file_speed;
        pDownload->m_writefile_error_code = error_code;
    }
}
//�첽д���ļ�
void CVideoWriteFile::OnBnClickedButtonStartWriteAsy()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    if (-1 != m_nLogID)
    {
        UpdateData(TRUE);
        HCS_VideoFileWriteInfo file_info;//��Ƶ�ļ�����Ϣ
        memset(&file_info, 0, sizeof(HCS_VideoFileWriteInfo));
        //
        strcpy(file_info.camera_id, W2A(m_camerID));
        //strcpy(file_info.pool_id, W2A(m_poolID));
        strcpy(file_info.rep_pool_id, W2A(m_repoolID));
        file_info.replication = m_nReNum;
        file_info.file_type = m_nType;
        if (VecPoolInfo.empty())
        {
            MessageBox(_T("��Դ��IDΪ��"));
            return;
        }
        strcpy(file_info.pool_id, VecPoolInfo[0].pool_id);
        for (INT_PTR i = 0;i < ary_filename.GetSize(); i++)//ע:�������ÿ��ֻ�ϴ�һ���ļ�
        {
            m_writefile_speed = 0;
            m_writefile_error_code = 0;
            char url[256] = {0};
            unsigned int url_len = sizeof(url);
            std::string FilePathName = "";
            FilePathName = T2A(ary_filename.GetAt(i));//��Ƶ�ļ�·��
            strcpy(file_info.file_path, FilePathName.c_str());
            int ret = HCS_SDK_VideoWriteFromFileStart(m_nLogID, &file_info, UpLoadFileSpeedCB,
                (void*)(this), url, &url_len, &m_write_handle);
            if (ret != HCS_OK)
            {
                MessageBox(_T("�ļ�д��ʧ��"));
                return;
            }
            while(1)
            {
                if (m_writefile_speed != 100 && m_writefile_error_code == 0)
                {
                    continue;
                }
                else 
                {
                    if(m_writefile_speed == 100 && m_writefile_error_code == 0)
                    {
                        MessageBox(_T("�ϴ����"));
                        break;
                    }
                    else
                    {
                        CString m_csErr;
                        m_csErr.Format(_T("�ϴ��ļ����ݳ����쳣����(�������:%d)"),m_writefile_error_code);
                        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
                        break;
                    }
                }
                
            }
            //
            CString ErrorCode;
            ErrorCode.Format(_T("%d"),m_writefile_error_code);
            std::string strUrlKey = url;
            if (std::string::npos != strUrlKey.find("?"))
            {
                strUrlKey = strUrlKey.substr(0,strUrlKey.find("?"));
            }
            else
            {
                strUrlKey = "No UrlKey";
            }
            m_listFileInfo.InsertItem(m_ItemIndex,A2W(strUrlKey.c_str()));
            m_listFileInfo.SetItemText(m_ItemIndex,1,A2W(url));
            m_listFileInfo.SetItemText(m_ItemIndex,2,ErrorCode);
            m_listFileInfo.SetItemText(m_ItemIndex,3,_T("�첽д��"));
            m_ItemIndex++;
            ret = HCS_SDK_VideoWriteFromFileStop(m_nLogID, m_write_handle);
            if(ret != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("ֹͣ����ʧ��(�������:%d)"),ret);
                MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            }
        }
    }
    else
    {
        MessageBox(_T("���ȵ�¼��!"),_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    
}

void CVideoWriteFile::OnBnClickedButtonStartWriteSyn()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USES_CONVERSION;
    if (-1 != m_nLogID)
    {
        UpdateData(TRUE);
        HCS_VideoFileWriteInfo  file_info;//��Ƶ�ļ�����Ϣ
        memset(&file_info, 0, sizeof(HCS_VideoFileWriteInfo));
        //
        strcpy(file_info.camera_id, W2A(m_camerID));
        strcpy(file_info.pool_id, W2A(m_poolID));
        strcpy(file_info.rep_pool_id, W2A(m_repoolID));
        file_info.replication = m_nReNum;
        file_info.file_type = m_nType;
        for (INT_PTR i = 0;i < ary_filename.GetSize(); i++)//ע:�������ÿ��ֻ�ϴ�һ���ļ�
        {
            char url[256] = {0};
            unsigned int url_len = sizeof(url);
            std::string FilePathName = "";
            FilePathName = T2A(ary_filename.GetAt(i));//��Ƶ�ļ�·��
            strcpy(file_info.file_path, FilePathName.c_str());
            int user_data = 0; //�û�����������ص��������õ�ʱ�����˲���
            int ret = HCS_SDK_SyncVideoWriteFromFile(m_nLogID, &file_info, url, &url_len);
            if (HCS_OK != ret)
            {
                MessageBox(_T("ͬ����Ƶ�ļ��洢ʧ��"));
            }
            //
            CString ErrorCode;
            ErrorCode.Format(_T("%d"),ret);
            std::string strUrlKey = url;
            if (std::string::npos != strUrlKey.find("?"))// ���ݶ�ȡkey,�����¼������������ID���������Ƶ�ļ�������ļ�key
            {
                strUrlKey = strUrlKey.substr(0,strUrlKey.find("?"));
            }
            else
            {
                strUrlKey = "No UrlKey";
            }
            m_listFileInfo.InsertItem(m_ItemIndex,A2W(strUrlKey.c_str()));
            m_listFileInfo.SetItemText(m_ItemIndex,1,A2W(url));
            m_listFileInfo.SetItemText(m_ItemIndex,2,ErrorCode);
            m_listFileInfo.SetItemText(m_ItemIndex,3,_T("ͬ��д��"));
            m_ItemIndex++;
        }
    }
    else
    {
        MessageBox(_T("���ȵ�¼��!"),_T("��ʾ"),MB_ICONWARNING);
        return;
    }
}

void CVideoWriteFile::OnBnClickedButtonClear()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_listFileInfo.DeleteAllItems();
    m_listInfo.DeleteAllItems();
    m_ItemIndex = 0;
    m_ItemIndexInfo = 0;
}

//
void DownLoad_Callback(int handle, const char* data, int data_size, int flag, int error_code, void* user_data)
{
    USES_CONVERSION;
    CVideoWriteFile *pDlg =(CVideoWriteFile*)user_data;
    pDlg->m_download_flag = flag;
    pDlg->m_download_error_code = error_code;
    if (data != NULL && data_size > 0)
    {
        //¼�����ݴ���
        FILE *fp = fopen(pDlg->m_FileName, "ab+");
        if (fp == NULL)
        {
            CString ErrMsg;
            ErrMsg.Format(_T("�ļ�%s������"), A2W(pDlg->m_FileName));
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
void CVideoWriteFile::OnBnClickedButtonVideoDownload()
{
    m_download_flag = -1;
    m_download_error_code = 0;
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    CString csUrl = m_listFileInfo.GetItemText(nItem,1);
    CString strResult = m_listFileInfo.GetItemText(nItem,2);
    //
    std::string strFileKey = W2A(csFileKey);
    std::string strUrlTime = "";
    std::string strUrl = W2A(csUrl);
    if (std::string::npos != strUrl.find_first_of("=") && std::string::npos != strUrl.find_first_of("&"))
    {
        strUrlTime = strUrl.substr(strUrl.find_first_of("=") + 1,strUrl.find_first_of("&") - strUrl.find_first_of("=") -1);
    }
    else
    {
        MessageBox(_T("��ȡʱ���ʽ����ȷ"));
        return;
    }
    std::string strStartYear = strUrlTime.substr(0, 4);
    std::string strStartMouth = strUrlTime.substr(4, 2);
    std::string strStartDay = strUrlTime.substr(6, 2);
    std::string strStartHour = strUrlTime.substr(9,2);
    std::string strStartMin = strUrlTime.substr(11,2);
    std::string strStartSec = strUrlTime.substr(13,2);

    std::string strEndYear = strUrlTime.substr(19, 4);
    std::string strEndMouth = strUrlTime.substr(23, 2);
    std::string strEndDay = strUrlTime.substr(25, 2);
    std::string strEndHour = strUrlTime.substr(28,2);
    std::string strEndMin = strUrlTime.substr(30,2);
    std::string strEndSec = strUrlTime.substr(32,2);
    //
    HCS_Time hcs_beg;
    HCS_Time hcs_end;

    hcs_beg.year = atoi(strStartYear.c_str());
    hcs_beg.month = atoi(strStartMouth.c_str());
    hcs_beg.day = atoi(strStartDay.c_str());
    hcs_beg.hour = atoi(strStartHour.c_str());
    hcs_beg.minute = atoi(strStartMin.c_str());
    hcs_beg.second = atoi(strStartSec.c_str());
    hcs_beg.millisecond = 0;

    hcs_end.year = atoi(strEndYear.c_str());
    hcs_end.month = atoi(strEndMouth.c_str());
    hcs_end.day = atoi(strEndDay.c_str());
    hcs_end.hour = atoi(strEndHour.c_str());
    hcs_end.minute = atoi(strEndMin.c_str());
    hcs_end.second = atoi(strEndSec.c_str());
    hcs_end.millisecond = 0;
    //
    memset(m_FileName, 0, MAX_LEN);
    memcpy_s(m_FileName, MAX_LEN, strFileKey.c_str(),strlen(strFileKey.c_str()));
    memcpy_s(m_FileName + strlen(m_FileName),  MAX_LEN, ("_" + strUrlTime).c_str(), strlen(("_" + strUrlTime).c_str()));
    strcat(m_FileName + strlen(m_FileName),".mp4");

    HCS_DataReadInfo read_info;
    memset(&read_info, 0, sizeof(read_info));
    memcpy(&read_info.begin_time, &hcs_beg, sizeof(read_info.begin_time));
    memcpy(&read_info.end_time, &hcs_end, sizeof(read_info.end_time));
    memcpy_s(&read_info.key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    memcpy(&read_info.read_type, "", sizeof(""));
    read_info.data_type = 1;// ��������[0-¼��,1-��Ƶ�ļ�]
    read_info.read_type = 4;
    m_handleDownLoad = -1;
    int ret = HCS_SDK_RecDownloadStart_V20(m_nLogID, &read_info, 0, DownLoad_Callback, (void*)this, &m_handleDownLoad);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("����¼�����س���(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    //
    while(1)
    {
        if (m_download_flag == 0 && m_download_error_code == 0)
        {
            ret = HCS_SDK_RecSearchStop(m_nLogID, m_handleDownLoad);
            if (ret != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("ֹͣ¼������ʧ��(�������:%d)"), ret);
                MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
                return;
            }
            MessageBox(_T("¼���������!"));
            break;
        }
        else if (m_download_flag != 0 && m_download_error_code == 0)
        {
            continue;
        }
        else
        {
            CString m_csErr;
            m_csErr.Format(_T("¼�������쳣(�������:%d)"), m_download_error_code);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            break;
        }
    }
}
unsigned int _stdcall VideoStartPlayBack(LPVOID lpParam)
{
    CVideoWriteFile* pVideoLinkCheck = static_cast<CVideoWriteFile*>(lpParam);
    if (NULL == pVideoLinkCheck)
    {
        return 1;
    }
    pVideoLinkCheck ->StartVideoPlayBack();
    return 0;
}
void CVideoWriteFile::StartVideoPlayBack()
{
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("����ֹͣ����"));
        return;
    }
    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    CString csUrl = m_listFileInfo.GetItemText(nItem,1);
    CString strResult = m_listFileInfo.GetItemText(nItem,2);
    //
    std::string strFileKey = W2A(csFileKey);
    std::string strUrlTime = "";
    std::string strUrl = W2A(csUrl);
    if (std::string::npos != strUrl.find_first_of("=") && std::string::npos != strUrl.find_first_of("&"))
    {
        strUrlTime = strUrl.substr(strUrl.find_first_of("=") + 1,strUrl.find_first_of("&") - strUrl.find_first_of("=") -1);
    }
    else
    {
        MessageBox(_T("��ȡʱ���ʽ����ȷ"));
        return;
    }
    std::string strStartYear = strUrlTime.substr(0, 4);
    std::string strStartMouth = strUrlTime.substr(4, 2);
    std::string strStartDay = strUrlTime.substr(6, 2);
    std::string strStartHour = strUrlTime.substr(9,2);
    std::string strStartMin = strUrlTime.substr(11,2);
    std::string strStartSec = strUrlTime.substr(13,2);

    std::string strEndYear = strUrlTime.substr(19, 4);
    std::string strEndMouth = strUrlTime.substr(23, 2);
    std::string strEndDay = strUrlTime.substr(25, 2);
    std::string strEndHour = strUrlTime.substr(28,2);
    std::string strEndMin = strUrlTime.substr(30,2);
    std::string strEndSec = strUrlTime.substr(32,2);
    //
    HCS_Time hcs_beg;
    HCS_Time hcs_end;

    hcs_beg.year = atoi(strStartYear.c_str());
    hcs_beg.month = atoi(strStartMouth.c_str());
    hcs_beg.day = atoi(strStartDay.c_str());
    hcs_beg.hour = atoi(strStartHour.c_str());
    hcs_beg.minute = atoi(strStartMin.c_str());
    hcs_beg.second = atoi(strStartSec.c_str());
    hcs_beg.millisecond = 0;

    hcs_end.year = atoi(strEndYear.c_str());
    hcs_end.month = atoi(strEndMouth.c_str());
    hcs_end.day = atoi(strEndDay.c_str());
    hcs_end.hour = atoi(strEndHour.c_str());
    hcs_end.minute = atoi(strEndMin.c_str());
    hcs_end.second = atoi(strEndSec.c_str());
    hcs_end.millisecond = 0;

    HCS_DataReadInfo read_info;
    memset(&read_info, 0, sizeof(read_info));
    memcpy(&read_info.begin_time, &hcs_beg, sizeof(read_info.begin_time));
    memcpy(&read_info.end_time, &hcs_end, sizeof(read_info.end_time));
    memcpy_s(&read_info.key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    memcpy(&read_info.read_type, "", sizeof(""));
    read_info.data_type = 1;// ��������[0-¼��,1-��Ƶ�ļ�]
    read_info.read_type = 1;// ��ȡ����[1-�����طţ�2-����I֡�ط�,3-����ط�]
    int ret = HCS_SDK_PlaybackStart_V20(m_nLogID, &read_info, &m_handlePlayback);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("����¼��طų���(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    //��ȡ¼��ͷ
    char buff_head[HEAD_SIZE] = {0};
    memset(buff_head, 0, HEAD_SIZE);
    int buff_head_size = 0;
    HCS_GetHeadInfo Head_info;
    memset(&Head_info, 0, sizeof(Head_info));
    memcpy(&Head_info.begin_time, &hcs_beg, sizeof(Head_info.begin_time));
    memcpy(&Head_info.end_time, &hcs_end, sizeof(Head_info.end_time));
    memcpy_s(&Head_info.key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    Head_info.data_type = 1;// ��������[0-¼��,1-��Ƶ�ļ�]
    ret = HCS_SDK_GetRecHead_V20(m_nLogID, &Head_info, buff_head, HEAD_SIZE, &buff_head_size);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ȡ¼��ط�ͷʧ��(�������:%d)"),ret);
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
    BOOL bRet = PlayM4_OpenStream(m_nPort,(PBYTE)buff_head,buff_head_size,SOURCE_BUF_MAX);
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
    if(NULL == m_PlayShow.GetSafeHwnd() || !PlayM4_Play(m_nPort, m_PlayShow.GetSafeHwnd()))
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
void CVideoWriteFile::OnBnClickedButtonPlayback()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
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
        m_hThreadVideoPlayBack = (HANDLE)_beginthreadex(NULL,0,VideoStartPlayBack,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoPlayBack)
        {
            CString m_csErr;
            m_csErr.Format(_T("����¼�񲥷��߳�ʧ��"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoWriteFile::OnBnClickedButtonPalybackStop()
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
    m_PlayShow.Invalidate(TRUE);
    MessageBox(_T("ֹͣ�طųɹ�"));
}
//
void CVideoWriteFile::OnBnClickedButtonVideoDelete()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("����ֹͣ����"));
        return;
    }
    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    std::string strFileKey = W2A(csFileKey);

    HCS_VideoBaseInfo videoBaseInfo;
    memset(&videoBaseInfo, 0 ,sizeof(HCS_VideoBaseInfo));
    memcpy_s(videoBaseInfo.file_key, HCS_FILE_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    int ret = HCS_SDK_VideoFileDelete(m_nLogID, &videoBaseInfo);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("ɾ����Ƶʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    m_listFileInfo.DeleteItem(nItem);
    if (m_ItemIndex > 0)
    {
        m_ItemIndex--;
    }
    MessageBox(_T("ɾ����Ƶ�ļ��ɹ�"));
}
void CVideoWriteFile::OnBnClickedButtonVideoFind()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (-1 == m_nLogID)
    {
        MessageBox(_T("���ȵ�¼���豸"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("����ֹͣ����"));
        return;
    }
    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    std::string strFileKey = W2A(csFileKey);

    HCS_VideoBaseInfo videoBaseInfo;
    memset(&videoBaseInfo, 0 ,sizeof(HCS_VideoBaseInfo));
    memcpy_s(videoBaseInfo.file_key, HCS_FILE_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    HCS_VideoFileInfo file_info;
    memset(&file_info, 0 ,sizeof(HCS_VideoFileInfo));
    int ret = HCS_SDK_VideoFileInfo(m_nLogID, &videoBaseInfo, &file_info);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("��ѯ��Ƶʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    CString csFileSize,csIndexCount,csFrameRate;
    csFileSize.Format(_T("%d"),file_info.file_size);
    csIndexCount.Format(_T("%d"),file_info.index_count);
    csFrameRate.Format(_T("%d"),file_info.frame_rate);

    m_listInfo.InsertItem(m_ItemIndexInfo,A2W(file_info.file_key));
    m_listInfo.SetItemText(m_ItemIndexInfo,1,csFileSize);
    m_listInfo.SetItemText(m_ItemIndexInfo,2,csIndexCount);
    m_listInfo.SetItemText(m_ItemIndexInfo,3,csFrameRate);
    m_ItemIndexInfo++;
}
void CVideoWriteFile::OnBnClickedButtonVideoRead()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("����ѡ�ж�Ӧ�л�ȡ¼���ļ�Key"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    //��ȡ¼���С
    POSITION pos1 = m_listInfo.GetFirstSelectedItemPosition();
    int nItem1 = -1;
    if (pos1 == NULL)
    {
        MessageBox(_T("����ѡ�ж�Ӧ�л�ȡ¼���С"));
        return;
    }
    else
    {
        while (pos1)
        {
            nItem1 = m_listInfo.GetNextSelectedItem(pos1);
        }
    }
    //
    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    CString csUrl = m_listFileInfo.GetItemText(nItem,1);
    CString strResult = m_listFileInfo.GetItemText(nItem,2);
    CString csFileSize = m_listInfo.GetItemText(nItem1,1);
    //
    std::string strFileKey = W2A(csFileKey);
    std::string strUrlTime = "";
    std::string strUrl = W2A(csUrl);
    if (std::string::npos != strUrl.find_first_of("=") && std::string::npos != strUrl.find_first_of("&"))
    {
        strUrlTime = strUrl.substr(strUrl.find_first_of("=") + 1,strUrl.find_first_of("&") - strUrl.find_first_of("=") -1);
    }
    else
    {
        MessageBox(_T("��ȡʱ���ʽ����ȷ"));
        return;
    }
    std::string strStartYear = strUrlTime.substr(0, 4);
    std::string strStartMouth = strUrlTime.substr(4, 2);
    std::string strStartDay = strUrlTime.substr(6, 2);
    std::string strStartHour = strUrlTime.substr(9,2);
    std::string strStartMin = strUrlTime.substr(11,2);
    std::string strStartSec = strUrlTime.substr(13,2);

    std::string strEndYear = strUrlTime.substr(19, 4);
    std::string strEndMouth = strUrlTime.substr(23, 2);
    std::string strEndDay = strUrlTime.substr(25, 2);
    std::string strEndHour = strUrlTime.substr(28,2);
    std::string strEndMin = strUrlTime.substr(30,2);
    std::string strEndSec = strUrlTime.substr(32,2);
    //
    HCS_Time hcs_beg;
    HCS_Time hcs_end;

    hcs_beg.year = atoi(strStartYear.c_str());
    hcs_beg.month = atoi(strStartMouth.c_str());
    hcs_beg.day = atoi(strStartDay.c_str());
    hcs_beg.hour = atoi(strStartHour.c_str());
    hcs_beg.minute = atoi(strStartMin.c_str());
    hcs_beg.second = atoi(strStartSec.c_str());
    hcs_beg.millisecond = 0;

    hcs_end.year = atoi(strEndYear.c_str());
    hcs_end.month = atoi(strEndMouth.c_str());
    hcs_end.day = atoi(strEndDay.c_str());
    hcs_end.hour = atoi(strEndHour.c_str());
    hcs_end.minute = atoi(strEndMin.c_str());
    hcs_end.second = atoi(strEndSec.c_str());
    hcs_end.millisecond = 0;
    //
    HCS_VideoReadInfo strVideoReadInfo;
    memset(&strVideoReadInfo, 0, sizeof(HCS_VideoReadInfo));
    strVideoReadInfo.read_type = 1;// ��ȡ����[1-�����طţ�2-����I֡�ط�,3-����طţ�4-����]
    memcpy_s(strVideoReadInfo.file_key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    memcpy(&strVideoReadInfo.begin_time, &hcs_beg, sizeof(strVideoReadInfo.begin_time));
    memcpy(&strVideoReadInfo.end_time, &hcs_end, sizeof(strVideoReadInfo.end_time));
    int handle = -1;
    int ret = HCS_SDK_VideoFileReadStart(m_nLogID, &strVideoReadInfo, &handle);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("������Ƶ�ļ���ȡʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    //
    char cFileName[MAX_LEN];
    memset(cFileName, 0, MAX_LEN);
    memcpy_s(cFileName, MAX_LEN, strFileKey.c_str(),strlen(strFileKey.c_str()));
    memcpy_s(cFileName + strlen(cFileName),  MAX_LEN, strUrlTime.c_str(), strlen(strUrlTime.c_str()));
    strcat(cFileName + strlen(cFileName),".mp4");
    int data_size = -1;
    int nFileSize = _ttoi(csFileSize);
    while (data_size != 0)
    {
        char* TcBuff = new char[nFileSize];
        //¼�����ݴ���
        FILE *fp = fopen(cFileName, "ab+");
        if (fp == NULL)
        {
            CString ErrMsg;
            ErrMsg.Format(_T("�ļ�%s������"), A2W(cFileName));
            AfxMessageBox(ErrMsg);
            return;
        }
        ret = HCS_SDK_VideoFileReadData(m_nLogID, handle, TcBuff,sizeof(char) * nFileSize, &data_size);
        if (ret != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("��ȡ��Ƶ�ļ�����ʧ��(�������:%d)"),ret);
            MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
            return;
        }
        fwrite(TcBuff, sizeof(char), data_size, fp);
        fclose(fp);
        delete [] TcBuff;
    }
}

void CVideoWriteFile::OnBnClickedButtonIndexSearch()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }

    USES_CONVERSION;
    CString csFileKey = m_listFileInfo.GetItemText(nItem,0);
    CString csUrl = m_listFileInfo.GetItemText(nItem,1);
    CString strResult = m_listFileInfo.GetItemText(nItem,2);
    //
    std::string strFileKey = W2A(csFileKey);
    std::string strUrlTime = "";
    std::string strUrl = W2A(csUrl);
    if (std::string::npos != strUrl.find_first_of("=") && std::string::npos != strUrl.find_first_of("&"))
    {
        strUrlTime = strUrl.substr(strUrl.find_first_of("=") + 1,strUrl.find_first_of("&") - strUrl.find_first_of("=") -1);
    }
    else
    {
        MessageBox(_T("��ȡʱ���ʽ����ȷ"));
        return;
    }
    std::string strStartYear = strUrlTime.substr(0, 4);
    std::string strStartMouth = strUrlTime.substr(4, 2);
    std::string strStartDay = strUrlTime.substr(6, 2);
    std::string strStartHour = strUrlTime.substr(9,2);
    std::string strStartMin = strUrlTime.substr(11,2);
    std::string strStartSec = strUrlTime.substr(13,2);

    std::string strEndYear = strUrlTime.substr(19, 4);
    std::string strEndMouth = strUrlTime.substr(23, 2);
    std::string strEndDay = strUrlTime.substr(25, 2);
    std::string strEndHour = strUrlTime.substr(28,2);
    std::string strEndMin = strUrlTime.substr(30,2);
    std::string strEndSec = strUrlTime.substr(32,2);
    //
    HCS_Time hcs_beg;
    HCS_Time hcs_end;

    hcs_beg.year = atoi(strStartYear.c_str());
    hcs_beg.month = atoi(strStartMouth.c_str());
    hcs_beg.day = atoi(strStartDay.c_str());
    hcs_beg.hour = atoi(strStartHour.c_str());
    hcs_beg.minute = atoi(strStartMin.c_str());
    hcs_beg.second = atoi(strStartSec.c_str());
    hcs_beg.millisecond = 0;

    hcs_end.year = atoi(strEndYear.c_str());
    hcs_end.month = atoi(strEndMouth.c_str());
    hcs_end.day = atoi(strEndDay.c_str());
    hcs_end.hour = atoi(strEndHour.c_str());
    hcs_end.minute = atoi(strEndMin.c_str());
    hcs_end.second = atoi(strEndSec.c_str());
    hcs_end.millisecond = 0;

    HCS_VideoReadInfo strVideoReadInfo;
    memset(&strVideoReadInfo, 0, sizeof(strVideoReadInfo));
    memcpy(&strVideoReadInfo.begin_time, &hcs_beg, sizeof(strVideoReadInfo.begin_time));
    memcpy(&strVideoReadInfo.end_time, &hcs_end, sizeof(strVideoReadInfo.end_time));
    memcpy_s(&strVideoReadInfo.file_key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    strVideoReadInfo.read_type = 1;// ��ȡ����[1-�����طţ�2-����I֡�ط�,3-����ط�]
    int handle = -1;
    int ret = HCS_SDK_VideoFileIndexStart(m_nLogID, &strVideoReadInfo, &handle);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("������Ƶ�ļ���������ʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
    //
    int status = -1;
    if (!vecFileIndexData.empty())
    {
        vecFileIndexData.clear();
    }
    HCS_FileIndexData strFileIndexData;
    while (1)
    {
        memset(&strFileIndexData, 0, sizeof(HCS_FileIndexData));
        ret = HCS_SDK_VideoFileIndexNext(m_nLogID, handle, &strFileIndexData, &status);
        if (1 == status)
        {
            vecFileIndexData.push_back(strFileIndexData);
        }
        else if (2 == status)
        {
            Sleep(200);
            continue;
        }
        else if (3 == status)
        {
            CString m_Msg;
            m_Msg.Format(_T("��ѯ���ļ�����,���鵽%��������"),vecFileIndexData.size());
            MessageBox(m_Msg);
            break;
        }
        else
        {
            MessageBox(_T("��ѯ�쳣"));
            break;
        }
    }
    ret = HCS_SDK_VideoFileIndexStop(m_nLogID, handle);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("ֹͣ��������ʧ��(�������:%d)"),ret);
        MessageBox(m_csErr,_T("��ʾ"),MB_ICONWARNING);
        return;
    }
}
