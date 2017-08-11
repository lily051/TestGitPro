// VideoWriteFile.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoCloudInterfaceTest.h"
#include "VideoWriteFile.h"


// CVideoWriteFile 对话框
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

    m_listFileInfo.InsertColumn(0,_T("文件Key"),LVCFMT_CENTER,60);
    m_listFileInfo.InsertColumn(1,_T("Url"),LVCFMT_CENTER,40);
    m_listFileInfo.InsertColumn(2,_T("Result"),LVCFMT_CENTER,60);
    m_listFileInfo.InsertColumn(3,_T("写入类型"),LVCFMT_CENTER,60);
    //
    m_listInfo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    m_listInfo.InsertColumn(0,_T("文件名"),LVCFMT_CENTER,175);
    m_listInfo.InsertColumn(1,_T("大小"),LVCFMT_CENTER,90);
    m_listInfo.InsertColumn(2,_T("索引个数"),LVCFMT_CENTER,90);
    m_listInfo.InsertColumn(3,_T("帧率"),LVCFMT_CENTER,90);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// CVideoWriteFile 消息处理程序

void CVideoWriteFile::OnBnClickedButtonSelectFile()
{
    // TODO: 在此添加控件通知处理程序代码
    ary_filename.RemoveAll();
    ary_fileTitle.RemoveAll();
    GetDlgItem(IDC_EDIT_SELECT_FILE)->SetWindowText(_T(""));

    CString strFilesPathShow = _T("");
    CString pathName,fileName,fileTitle;
    WCHAR* filters = _T("mp4文件(*.mp4)|*.mp4");
    //创建一个可以选择多个文件的CFileDialog
    CFileDialog fileDlg(TRUE,NULL,_T("*.mp4"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,filters);
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
//回调函数，返回视频文件写入的进度和状态信息
void UpLoadFileSpeedCB(int handle, int file_speed, int error_code, void* user_data)
{
    //在此函数中不能调用写视频文件的停止接口
    CVideoWriteFile* pDownload = (CVideoWriteFile*)user_data;
    if (pDownload != NULL)
    {
        pDownload->m_writefile_speed = file_speed;
        pDownload->m_writefile_error_code = error_code;
    }
}
//异步写入文件
void CVideoWriteFile::OnBnClickedButtonStartWriteAsy()
{
    // TODO: 在此添加控件通知处理程序代码
    USES_CONVERSION;
    if (-1 != m_nLogID)
    {
        UpdateData(TRUE);
        HCS_VideoFileWriteInfo file_info;//视频文件的信息
        memset(&file_info, 0, sizeof(HCS_VideoFileWriteInfo));
        //
        strcpy(file_info.camera_id, W2A(m_camerID));
        //strcpy(file_info.pool_id, W2A(m_poolID));
        strcpy(file_info.rep_pool_id, W2A(m_repoolID));
        file_info.replication = m_nReNum;
        file_info.file_type = m_nType;
        if (VecPoolInfo.empty())
        {
            MessageBox(_T("资源池ID为空"));
            return;
        }
        strcpy(file_info.pool_id, VecPoolInfo[0].pool_id);
        for (INT_PTR i = 0;i < ary_filename.GetSize(); i++)//注:方便起见每次只上传一个文件
        {
            m_writefile_speed = 0;
            m_writefile_error_code = 0;
            char url[256] = {0};
            unsigned int url_len = sizeof(url);
            std::string FilePathName = "";
            FilePathName = T2A(ary_filename.GetAt(i));//视频文件路径
            strcpy(file_info.file_path, FilePathName.c_str());
            int ret = HCS_SDK_VideoWriteFromFileStart(m_nLogID, &file_info, UpLoadFileSpeedCB,
                (void*)(this), url, &url_len, &m_write_handle);
            if (ret != HCS_OK)
            {
                MessageBox(_T("文件写入失败"));
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
                        MessageBox(_T("上传完成"));
                        break;
                    }
                    else
                    {
                        CString m_csErr;
                        m_csErr.Format(_T("上传文件数据出错，异常结束(错误代码:%d)"),m_writefile_error_code);
                        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
            m_listFileInfo.SetItemText(m_ItemIndex,3,_T("异步写入"));
            m_ItemIndex++;
            ret = HCS_SDK_VideoWriteFromFileStop(m_nLogID, m_write_handle);
            if(ret != HCS_OK)
            {
                CString m_csErr;
                m_csErr.Format(_T("停止下载失败(错误代码:%d)"),ret);
                MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            }
        }
    }
    else
    {
        MessageBox(_T("请先登录云!"),_T("提示"),MB_ICONWARNING);
        return;
    }
    
}

void CVideoWriteFile::OnBnClickedButtonStartWriteSyn()
{
    // TODO: 在此添加控件通知处理程序代码
    USES_CONVERSION;
    if (-1 != m_nLogID)
    {
        UpdateData(TRUE);
        HCS_VideoFileWriteInfo  file_info;//视频文件的信息
        memset(&file_info, 0, sizeof(HCS_VideoFileWriteInfo));
        //
        strcpy(file_info.camera_id, W2A(m_camerID));
        strcpy(file_info.pool_id, W2A(m_poolID));
        strcpy(file_info.rep_pool_id, W2A(m_repoolID));
        file_info.replication = m_nReNum;
        file_info.file_type = m_nType;
        for (INT_PTR i = 0;i < ary_filename.GetSize(); i++)//注:方便起见每次只上传一个文件
        {
            char url[256] = {0};
            unsigned int url_len = sizeof(url);
            std::string FilePathName = "";
            FilePathName = T2A(ary_filename.GetAt(i));//视频文件路径
            strcpy(file_info.file_path, FilePathName.c_str());
            int user_data = 0; //用户传入参数，回调函数调用的时候会带此参数
            int ret = HCS_SDK_SyncVideoWriteFromFile(m_nLogID, &file_info, url, &url_len);
            if (HCS_OK != ret)
            {
                MessageBox(_T("同步视频文件存储失败"));
            }
            //
            CString ErrorCode;
            ErrorCode.Format(_T("%d"),ret);
            std::string strUrlKey = url;
            if (std::string::npos != strUrlKey.find("?"))// 数据读取key,如果是录像则填充编码器ID，如果是视频文件则填充文件key
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
            m_listFileInfo.SetItemText(m_ItemIndex,3,_T("同步写入"));
            m_ItemIndex++;
        }
    }
    else
    {
        MessageBox(_T("请先登录云!"),_T("提示"),MB_ICONWARNING);
        return;
    }
}

void CVideoWriteFile::OnBnClickedButtonClear()
{
    // TODO: 在此添加控件通知处理程序代码
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
        //录像数据处理
        FILE *fp = fopen(pDlg->m_FileName, "ab+");
        if (fp == NULL)
        {
            CString ErrMsg;
            ErrMsg.Format(_T("文件%s不存在"), A2W(pDlg->m_FileName));
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
void CVideoWriteFile::OnBnClickedButtonVideoDownload()
{
    m_download_flag = -1;
    m_download_error_code = 0;
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
        MessageBox(_T("提取时间格式不正确"));
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
    read_info.data_type = 1;// 数据类型[0-录像,1-视频文件]
    read_info.read_type = 4;
    m_handleDownLoad = -1;
    int ret = HCS_SDK_RecDownloadStart_V20(m_nLogID, &read_info, 0, DownLoad_Callback, (void*)this, &m_handleDownLoad);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启录像下载出错(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
                m_csErr.Format(_T("停止录像下载失败(错误代码:%d)"), ret);
                MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
                return;
            }
            MessageBox(_T("录像下载完毕!"));
            break;
        }
        else if (m_download_flag != 0 && m_download_error_code == 0)
        {
            continue;
        }
        else
        {
            CString m_csErr;
            m_csErr.Format(_T("录像下载异常(错误代码:%d)"), m_download_error_code);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
        MessageBox(_T("请先选中一行"));
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
        MessageBox(_T("请先停止播放"));
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
        MessageBox(_T("提取时间格式不正确"));
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
    read_info.data_type = 1;// 数据类型[0-录像,1-视频文件]
    read_info.read_type = 1;// 读取类型[1-正常回放，2-正常I帧回放,3-倒序回放]
    int ret = HCS_SDK_PlaybackStart_V20(m_nLogID, &read_info, &m_handlePlayback);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启录像回放出错(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    //获取录像头
    char buff_head[HEAD_SIZE] = {0};
    memset(buff_head, 0, HEAD_SIZE);
    int buff_head_size = 0;
    HCS_GetHeadInfo Head_info;
    memset(&Head_info, 0, sizeof(Head_info));
    memcpy(&Head_info.begin_time, &hcs_beg, sizeof(Head_info.begin_time));
    memcpy(&Head_info.end_time, &hcs_end, sizeof(Head_info.end_time));
    memcpy_s(&Head_info.key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    Head_info.data_type = 1;// 数据类型[0-录像,1-视频文件]
    ret = HCS_SDK_GetRecHead_V20(m_nLogID, &Head_info, buff_head, HEAD_SIZE, &buff_head_size);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("获取录像回放头失败(错误代码:%d)"),ret);
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
    BOOL bRet = PlayM4_OpenStream(m_nPort,(PBYTE)buff_head,buff_head_size,SOURCE_BUF_MAX);
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
    if(NULL == m_PlayShow.GetSafeHwnd() || !PlayM4_Play(m_nPort, m_PlayShow.GetSafeHwnd()))
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
void CVideoWriteFile::OnBnClickedButtonPlayback()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
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
        m_hThreadVideoPlayBack = (HANDLE)_beginthreadex(NULL,0,VideoStartPlayBack,(LPVOID)this,0,NULL);
        if(NULL == m_hThreadVideoPlayBack)
        {
            CString m_csErr;
            m_csErr.Format(_T("创建录像播放线程失败"));
            MessageBox(m_csErr);
        }
    }
}

void CVideoWriteFile::OnBnClickedButtonPalybackStop()
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
    m_PlayShow.Invalidate(TRUE);
    MessageBox(_T("停止回放成功"));
}
//
void CVideoWriteFile::OnBnClickedButtonVideoDelete()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("请先停止播放"));
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
        m_csErr.Format(_T("删除视频失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
    m_listFileInfo.DeleteItem(nItem);
    if (m_ItemIndex > 0)
    {
        m_ItemIndex--;
    }
    MessageBox(_T("删除视频文件成功"));
}
void CVideoWriteFile::OnBnClickedButtonVideoFind()
{
    // TODO: 在此添加控件通知处理程序代码
    if (-1 == m_nLogID)
    {
        MessageBox(_T("请先登录云设备"));
        return;
    }
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    if (-1 != m_handlePlayback)
    {
        MessageBox(_T("请先停止播放"));
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
        m_csErr.Format(_T("查询视频失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
    int nItem = -1;
    if (pos == NULL)
    {
        MessageBox(_T("请先选中对应行获取录像文件Key"));
        return;
    }
    else
    {
        while (pos)
        {
            nItem = m_listFileInfo.GetNextSelectedItem(pos);
        }
    }
    //获取录像大小
    POSITION pos1 = m_listInfo.GetFirstSelectedItemPosition();
    int nItem1 = -1;
    if (pos1 == NULL)
    {
        MessageBox(_T("请先选中对应行获取录像大小"));
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
        MessageBox(_T("提取时间格式不正确"));
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
    strVideoReadInfo.read_type = 1;// 读取类型[1-正常回放，2-正常I帧回放,3-倒序回放，4-下载]
    memcpy_s(strVideoReadInfo.file_key, HCS_MAX_VIDEO_KEY_LEN, strFileKey.c_str(), strlen(strFileKey.c_str()));
    memcpy(&strVideoReadInfo.begin_time, &hcs_beg, sizeof(strVideoReadInfo.begin_time));
    memcpy(&strVideoReadInfo.end_time, &hcs_end, sizeof(strVideoReadInfo.end_time));
    int handle = -1;
    int ret = HCS_SDK_VideoFileReadStart(m_nLogID, &strVideoReadInfo, &handle);
    if (ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启视频文件读取失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
        //录像数据处理
        FILE *fp = fopen(cFileName, "ab+");
        if (fp == NULL)
        {
            CString ErrMsg;
            ErrMsg.Format(_T("文件%s不存在"), A2W(cFileName));
            AfxMessageBox(ErrMsg);
            return;
        }
        ret = HCS_SDK_VideoFileReadData(m_nLogID, handle, TcBuff,sizeof(char) * nFileSize, &data_size);
        if (ret != HCS_OK)
        {
            CString m_csErr;
            m_csErr.Format(_T("读取视频文件数据失败(错误代码:%d)"),ret);
            MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
            return;
        }
        fwrite(TcBuff, sizeof(char), data_size, fp);
        fclose(fp);
        delete [] TcBuff;
    }
}

void CVideoWriteFile::OnBnClickedButtonIndexSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos = m_listFileInfo.GetFirstSelectedItemPosition();
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
        MessageBox(_T("提取时间格式不正确"));
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
    strVideoReadInfo.read_type = 1;// 读取类型[1-正常回放，2-正常I帧回放,3-倒序回放]
    int handle = -1;
    int ret = HCS_SDK_VideoFileIndexStart(m_nLogID, &strVideoReadInfo, &handle);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("开启视频文件索引检索失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
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
            m_Msg.Format(_T("查询此文件结束,共查到%个块索引"),vecFileIndexData.size());
            MessageBox(m_Msg);
            break;
        }
        else
        {
            MessageBox(_T("查询异常"));
            break;
        }
    }
    ret = HCS_SDK_VideoFileIndexStop(m_nLogID, handle);
    if(ret != HCS_OK)
    {
        CString m_csErr;
        m_csErr.Format(_T("停止索引检索失败(错误代码:%d)"),ret);
        MessageBox(m_csErr,_T("提示"),MB_ICONWARNING);
        return;
    }
}
