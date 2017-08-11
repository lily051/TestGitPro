/**
*   @file MainDialog.cpp
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*   
*   @author 
*   @date   
*
*   @note
*	@history add ��ʱ��Ϣ��ӦDialog
*            date 2012/08/14
*   @warning
*/

// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceWatchdog.h"
#include "MainDialog.h"
#include "AboutDlg.h"
#include "Guard.h"
#include "Folder.h"

#include "ServiceWatchdogConfig.h"
#include <string>

#include "Iphlpapi.h"
#pragma comment(lib, "iphlpapi.lib")

//#include "HPPFunction.h"
#include "hppServer.h"
#include "XMLFunction.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainDialog dialog

IMPLEMENT_DYNAMIC(CMainDialog, CSkinDialog)

CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CMainDialog::IDD, pParent)
    , m_bWindowShowing(true)
    , m_nDlgFlag(SVC_LIST_DLG)
    //, m_hppDCClientHandle(NULL)
    //, m_hppLocalHandle(NULL)
    //, m_hppLocalSerHanlde(NULL)
    , m_hEventGetInfo(NULL)
    , m_hThreadGetInfo(NULL)
    , m_hCtrlMsgThread(NULL)
    , m_hCtrlMsgEvent(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    memset(&m_nid, 0, sizeof(NOTIFYICONDATA));

    InitializeCriticalSectionAndSpinCount(&m_hppDCClientHandleLock, 4 * 1000);
    InitializeCriticalSectionAndSpinCount(&m_hppLocalSevHanldeLock, 4 * 1000);

    memset(&m_preIdleTime, 0, sizeof(FILETIME));
    memset(&m_preKernelTime, 0, sizeof(FILETIME));
    memset(&m_preUserTime, 0, sizeof(FILETIME));
    memset(&m_curIdleTime, 0, sizeof(FILETIME));
    memset(&m_curKernelTime, 0, sizeof(FILETIME));
    memset(&m_curUserTime, 0, sizeof(FILETIME));

    if (XML_GetSkinType() == 0) // blue
    {
        m_bIsRedBlackSkin = FALSE;
    }
    else //red_black
    {
        m_bIsRedBlackSkin = TRUE;
    }
    // ��ʼ��HPP
    GetHPPServer()->InitHpp();
}

CMainDialog::~CMainDialog() 
{
    DeleteCriticalSection(&m_hppDCClientHandleLock);
    DeleteCriticalSection(&m_hppLocalSevHanldeLock);
    // OnDestroy ���Ѿ��ر���Щ���
    m_hEventGetInfo = NULL;
    m_hThreadGetInfo = NULL;

    //m_hppDCClientHandle = NULL;
    //m_hppLocalHandle = NULL;
    //m_hppLocalSerHanlde = NULL;

    m_hCtrlMsgThread = NULL;
    m_hCtrlMsgEvent = NULL;

    m_hIcon = NULL;
    try
    {
        // ����ʼ��HPP
        GetHPPServer()->UninitHpp();
        GetHPPServer()->free();
    }
    catch (...)
    {
    }
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MINIMIZE, m_buttonMin);
    DDX_Control(pDX, IDC_EXIT, m_buttonExit);
    DDX_Control(pDX, IDC_APP_STAT, m_appStat);
    DDX_Control(pDX, IDC_LIST_SERVICE, m_listService);
    DDX_Control(pDX, IDC_BUTTON_INSTANT_SHOW_INFO, m_buttonViewIstantMessage);
    DDX_Control(pDX, IDC_BUTTON_SERVICE_LIST, m_buttonServiceList);
    DDX_Control(pDX, IDC_LOGO, m_appLogo);
    DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnCfg);
}

BEGIN_MESSAGE_MAP(CMainDialog, CSkinDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_SERVICE_CONTROL_COMMAND_ERROR, &CMainDialog::OnServiceControlCommandError)
    ON_MESSAGE(WM_SERVICE_STATUS_CHANGED, &CMainDialog::OnServiceStatusChanged)
    ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CMainDialog::OnTaskBarCreated)
    ON_MESSAGE(WM_ICON_NOTIFY, &CMainDialog::OnNotifyIcon)
    ON_MESSAGE(WM_USER_CLOSE, &CMainDialog::OnUserClose)
    ON_BN_CLICKED(IDC_MINIMIZE, &CMainDialog::OnBnClickedMinimize)
    ON_BN_CLICKED(IDC_EXIT, &CMainDialog::OnBnClickedExit)
    ON_COMMAND(ID_SERVICECONTROL_ALL_STOP, &CMainDialog::OnBnClickedButtonStopAllService)
    ON_COMMAND(ID_SERVICECONTROL_ALL_START, &CMainDialog::OnBnClickedButtonRestartAllService)
    ON_COMMAND(ID_ICONNOTIFY_SHOW_WINDOW, &CMainDialog::OnIconnotifyShowWindow)
    ON_COMMAND(ID_ICONNOTIFY_HIDE_WINDOW, &CMainDialog::OnIconnotifyHideWindow)
    ON_COMMAND(ID_ICONNOTIFY_EXIT, &CMainDialog::OnIconnotifyExit)
    ON_UPDATE_COMMAND_UI(ID_ICONNOTIFY_SHOW_WINDOW, &CMainDialog::OnUpdateIconnotifyShowWindow)
    ON_UPDATE_COMMAND_UI(ID_ICONNOTIFY_HIDE_WINDOW, &CMainDialog::OnUpdateIconnotifyHideWindow)
    ON_UPDATE_COMMAND_UI(ID_ICONNOTIFY_EXIT, &CMainDialog::OnUpdateIconnotifyExit)
    ON_WM_INITMENUPOPUP()
    ON_BN_CLICKED(IDC_BUTTON_INSTANT_SHOW_INFO, &CMainDialog::OnBnClickedButtonInstantShowInfo)
    ON_BN_CLICKED(IDC_BUTTON_SERVICE_LIST, &CMainDialog::OnBnClickedButtonServiceList)
    ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CMainDialog::OnBnClickedButtonConfig)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVICE, &CMainDialog::OnNMRClickListService)
    ON_COMMAND(ID_SERVICECONTROL_START, &CMainDialog::MenuStartService)
    ON_COMMAND(ID_SERVICECONTROL_STOP, &CMainDialog::MenuStopService)
    ON_COMMAND(ID_SERVICECONTROL_RESTART, &CMainDialog::MenuRestartService)
    ON_COMMAND(ID_SERVICECONTROL_ALL_INSTALL, &CMainDialog::MenuInstallALLService)
    ON_COMMAND(ID_SERVICECONTROL_INSTALL, &CMainDialog::MenuInstallService)
    ON_COMMAND(ID_SERVICECONTROL_UNINSTALL, &CMainDialog::MenuUninstallService)
    ON_COMMAND(ID_SERVICECONTROL_ALL_UNINSTALL, &CMainDialog::MenuUninstallAllService)
    ON_COMMAND(ID_SERVICECONTROL_CONFIG, &CMainDialog::MenuOpenConfig)
    ON_COMMAND(ID_SERVICECONTROL_RUN_DIR, &CMainDialog::MenuOpenRunDir)
    ON_COMMAND(ID_SERVICECONTROL_MESSAGE, &CMainDialog::MenuInstantMsg)
END_MESSAGE_MAP()


// CMainDialog message handlers

BOOL CMainDialog::OnInitDialog()
{
    CSkinDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
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

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    {// ��ʼ�������
        WSADATA wsaData;
        WORD sockVersion = MAKEWORD(2, 2);
        ::WSAStartup(sockVersion, &wsaData);
    }
    // ����Ƥ��
    m_bExclude = TRUE;
    LoadWatchBitMap(m_bIsRedBlackSkin);

    TCHAR szServerNameBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_SERVICE_NAME, szServerNameBuff, _countof(szServerNameBuff));
    SetWindowText(szServerNameBuff);

    // ���ϵͳ���̡�
    AddNotifyIcon();

    // ��ʼ�������ϵ����а�ť
    InitButton();

    // �����б����ʽ��
    SetListControlServicesHeader();

    // ��ʼ��m_lstServiceController
    InitServiceController();

    //
    SetForegroundWindow();

    // ��ʼ��������Ϣdlg
    InitAddDlg();

    // ��ϵͳ��ʱ��Ϣ����ʾ
    // �������ķ����IP��ַ�Ͷ˿ڼ�
    // ���ص�IP��ַ�Ͷ˿�
    //ShowDCAndLocal();

    //// ��ʼ����ȡ��Ϣ���߳�
    //if (FALSE == InitGetThread())
    //{
    //    SVCMANAGERLOG_ERROR(_T("Create thread of get info from ServerSys and ServerProc failed."));
    //}

    if (FALSE == InitCtrlMsgThread())
    {
        SVCMANAGERLOG_ERROR(_T("InitCtrlMsgThread return FALSE."));
    }

    // �������ؼ���
    do 
    {
        CGuard guard(&m_hppLocalSevHanldeLock);
        BOOL bRet = GetHPPServer()->StartListen(6679);

        {
            CString csTmp;
            csTmp.Format(_T("��ʼ�����ؼ�������%s"), (bRet ? _T("�ɹ�"):_T("ʧ��")));
            CreateSysShowMsg(csTmp, _T(""));
        }
    } while (0);

    {// ��ʾ��������ҳ ((a||b)&&c)
        if (((1 == theApp.m_nAlarmProcessor) || (2 == theApp.m_nAlarmProcessor) )
            && !XML_CheckCfgPara())
        {
            OnBnClickedButtonConfig();
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CSkinDialog::OnSysCommand(nID, lParam);
    }
}

void CMainDialog::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CSkinDialog::OnPaint();
    }
}

HCURSOR CMainDialog::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

///* ��ʱ�� */
//void CMainDialog::OnTimer(UINT_PTR nIDEvent)
//{
//    switch (nIDEvent)
//    {
//    case LOCAL_SERVER_RECONN:
//        {
//            if (NULL == m_hppLocalSerHanlde)
//            {
//                CGuard guard(&m_hppLocalSevHanldeLock);
//                OutputDebugString(_T("------LOCAL_SERVER_RECONN-------------\n"));
//                BOOL bRet = InitServerHppHandle(m_hppLocalHandle, m_hppLocalSerHanlde, theApp.m_nListenPort);
//                CString csTmp;
//                csTmp.Format(_T("���ؼ���������������%s"), (bRet ? _T("�ɹ�"):_T("ʧ��")));
//                CreateSysShowMsg(csTmp, _T(""));
//            }
//        }
//    	break;
//    default:
//        break;
//    }
//
//    CSkinDialog::OnTimer(nIDEvent);
//}

/* �����ڵĹرհ�ť��Ϣ��Ӧ */
void CMainDialog::OnClose()
{
    HideMainWindow();
}

/* ��Ӧ�����ڵ� WM_DESTROY ��Ϣ */
void CMainDialog::OnDestroy()
{
    CDialog::OnDestroy();

    if (theApp.m_nListenPort > 0 && theApp.m_nListenPort < 65535)
    {
        // �رն�ʱ��
        KillTimer(LOCAL_SERVER_RECONN);
    }

    // �˳���ȡ��Ϣ�߳�
    CloseGetThread();

    // �˳�������Ϣ�����߳�
    CloseCtrlMsgThread();

    // �ͷŷ����б���Դ
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin(); itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            bool rtn = pSevCtrol->Shutdown();

            CString csTmp;
            csTmp.Format("ServiceWatchdog>> Service [%s] Controller Shutdown %s .\n", pSevCtrol->m_svcName.c_str(), rtn ? "succeeded" : "failed");
            OutputDebugString(csTmp);

            delete pSevCtrol;
            //pSevCtrol = NULL;
        }
    }
    m_lstServiceController.clear();

    // ��������ͼ�ꡣ
    RemoveNotifyIcon();

    {// �ر�HPPЭ��ջ
        GetHPPServer()->StopListen();
    }
    //{// �ر�HPPЭ��ջ
    //    CGuard guard(&m_hppDCClientHandleLock);
    //    if (g_pCMSConnSession != NULL)
    //    {
    //        g_pCMSConnSession->StopService();
    //    }

    //    CloseHPPHandle(m_hppDCClientHandle);
    //}
    //{// �رձ��ؼ�������HPPЭ��ջ
    //    CGuard guard(&m_hppLocalSevHanldeLock);
    //    CloseHPPHandle(m_hppLocalHandle, m_hppLocalSerHanlde);
    //}

    WSACleanup();
}

/* ���ΰ����ĵ����� */
void CMainDialog::WinHelp(DWORD_PTR dwData, UINT nCmd)
{
    if (HELP_CONTEXT == nCmd)
    {
        return;
    }

    CDialog::WinHelp(dwData, nCmd);
}

/* ����F1��ESC��Enter�� */
BOOL CMainDialog::PreTranslateMessage(MSG* pMsg)
{
    if ( (WM_KEYDOWN == pMsg->message) &&
        ((VK_F1 == pMsg->wParam) || (VK_RETURN == pMsg->wParam) || (VK_ESCAPE == pMsg->wParam)))
    {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

/** @fn       LRESULT CMainDialog::OnServiceControlCommandError(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_SERVICE_CONTROL_COMMAND_ERROR ��Ϣ��Ӧ
 *  @param    (WPARAM) wParam [IN] : Command Error ����
 *            (LPARAM) lParam [IN] : ServiceController ʵ��
 *  @return:  LRESULT��������
 */
LRESULT CMainDialog::OnServiceControlCommandError(WPARAM wParam, LPARAM lParam)
{
    DWORD error = (DWORD)wParam;
    ServiceController* SvcControl = (ServiceController*)lParam;

    if (SvcControl)
    {
        DWORD status = SvcControl->getServiceStatus();

        int itemIdex = (int)SvcControl->m_userData;
        if (itemIdex >= 0)
        {
            // ����״̬
            LV_ITEM lvitem = {0};
            lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
            lvitem.iItem = itemIdex;
            lvitem.iSubItem = 1;

            lvitem.iImage = GetListIconIndexBySvcStatus(status);
            lvitem.pszText = FormatServiceStatus(status);

            BOOL bRet = m_listService.SetItem(&lvitem);
            CString csTmp;
            csTmp.Format("ServiceWatchdog>> Update Service [%s] Status [%s] In ListControl ", SvcControl->m_svcName.c_str(), FormatServiceStatus(status));
            if (TRUE == bRet)
            {
                csTmp.AppendFormat(_T("succeeded.\n"));
            }
            else
            {
                csTmp.AppendFormat(_T("failed.\n"));
            }
            OutputDebugString(csTmp);
        }
        else
        {
            // ���������б�����ʾ��
            int nIndex = m_listService.GetItemCount();

            LV_ITEM lvitemAdd = {0};
            lvitemAdd.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
            lvitemAdd.iItem = nIndex;
            lvitemAdd.iSubItem = 0;
            lvitemAdd.iImage = -1;
            lvitemAdd.pszText = (char*)(SvcControl->m_svcDisplayName.c_str());

            lvitemAdd.lParam = (LPARAM)SvcControl;

            BOOL bRet = FALSE;
            int nItemIdex = -1;
            if ((nItemIdex = m_listService.InsertItem(&lvitemAdd)) != -1)
            {
                LV_ITEM lvitem = {0};
                lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
                lvitem.iItem = nIndex;
                lvitem.iSubItem = 1;

                lvitem.iImage = GetListIconIndexBySvcStatus(status);
                lvitem.pszText = FormatServiceStatus(status);

                bRet = m_listService.SetItem(&lvitem);
            }

            // �洢�������ݡ�
            SvcControl->m_userData = (void*)nItemIdex;

            TRACE("ServiceWatchdog>> Insert Service [%s] Status [%s] Into ListControl %s .\n", SvcControl->m_svcName.c_str(), FormatServiceStatus(status), bRet ? "succeeded" : "failed");
        }

        if (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST == error)
        {
            TRACE("ServiceWatchdog>> Service [%s] Does Not Exist .\n", SvcControl->m_svcName.c_str());
        }
        else
        {
            // ˢ�Է���״̬��
            //controller->QuerySvcStatus();

            TRACE("ServiceWatchdog>> Service [%s] Command Error [0x%08x] .\n", SvcControl->m_svcName.c_str(), error);
        }
    }

    return 0;
}

/** @fn       LRESULT CMainDialog::OnServiceStatusChanged(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_SERVICE_STATUS_CHANGED ��Ϣ��Ӧ
 *  @param    (WPARAM) wParam [IN] : Service Status
 *            (LPARAM) lParam [IN] : ServiceControllerʵ��
 *  @return:  LRESULT��������
 */
LRESULT CMainDialog::OnServiceStatusChanged(WPARAM wParam, LPARAM lParam)
{
    DWORD status = (DWORD)wParam;
    ServiceController* SvcControl = (ServiceController*)lParam;

    if (SvcControl != NULL)
    {
        int itemIdex = (int)SvcControl->m_userData;
        if (itemIdex >= 0)
        {
            {// ������ʾ��
                if (SvcControl->m_bDispNameChnge)
                {
                    LV_ITEM lvitemUpdate = {0};
                    lvitemUpdate.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
                    lvitemUpdate.iItem = itemIdex;
                    lvitemUpdate.iSubItem = 0;
                    lvitemUpdate.iImage = -1;
                    lvitemUpdate.pszText = (char*)(SvcControl->m_svcDisplayName.c_str());

                    lvitemUpdate.lParam = (LPARAM)SvcControl;
                    if (TRUE == m_listService.SetItem(&lvitemUpdate))
                    {
                        SvcControl->m_bDispNameChnge = false;
                    }
                }  
            }

            // ����״̬
            LV_ITEM lvitem = {0};
            lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
            lvitem.iItem = itemIdex;
            lvitem.iSubItem = 1;

            lvitem.iImage = GetListIconIndexBySvcStatus(status);
            lvitem.pszText = FormatServiceStatus(status);

            BOOL bRet = m_listService.SetItem(&lvitem);

            CString csTmp;
            csTmp.Format("ServiceWatchdog>> Update Service [%s] Status [%s] In ListControl %s .\n", SvcControl->m_svcName.c_str(), FormatServiceStatus(status), (bRet ? "succeeded" : "failed"));
            OutputDebugString(csTmp);
        }
        else
        {
            // ���������б�����ʾ
            int nIndex = m_listService.GetItemCount();

            LV_ITEM lvitemAdd = {0};
            lvitemAdd.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
            lvitemAdd.iItem = nIndex;
            lvitemAdd.iSubItem = 0;
            lvitemAdd.iImage = -1;
            lvitemAdd.pszText = (char*)(SvcControl->m_svcDisplayName.c_str());

            lvitemAdd.lParam = (LPARAM)SvcControl;

            BOOL rtn = FALSE;
            int nItemIdex = -1;
            if ((nItemIdex = m_listService.InsertItem(&lvitemAdd)) != -1)
            {
                LV_ITEM lvitem = {0};
                lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
                lvitem.iItem = nIndex;
                lvitem.iSubItem = 1;

                lvitem.iImage = GetListIconIndexBySvcStatus(status);
                lvitem.pszText = FormatServiceStatus(status);

                rtn = m_listService.SetItem(&lvitem);

                SvcControl->m_bDispNameChnge = false;
            }

            // �洢��������
            SvcControl->m_userData = (void*)nItemIdex;

            TRACE("ServiceWatchdog>> Insert Service [%s] Status [%s] Into ListControl %s .\n", SvcControl->m_svcName.c_str(), FormatServiceStatus(status), rtn ? "succeeded" : "failed");
        }
    }

    return 0;
}

/** @fn       LRESULT CMainDialog::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_TASKBARCREATED ��Ϣ��Ӧ
 *  @param
 *  @return:  LRESULT��������
 */
LRESULT CMainDialog::OnTaskBarCreated(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    RemoveNotifyIcon();
    AddNotifyIcon();

    return NULL;
}

/** @fn       LRESULT CMainDialog::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
 *  @brief    WM_ICON_NOTIFY ��Ϣ��Ӧ
 *  @param    (WPARAM) wParam [IN] : 
 *            (LPARAM) lParam [IN] : ����
 *  @return:  LRESULT��������
 */
LRESULT CMainDialog::OnNotifyIcon(WPARAM /*wParam*/, LPARAM lParam)
{
    UINT uMouseMsg = (UINT)lParam;

    switch (uMouseMsg)
    {
    case WM_LBUTTONDBLCLK:
        {
            ShowMainWindow();
        }
        break;
    case WM_RBUTTONUP:
        {
            CPoint point;
            GetCursorPos(&point);
            SetForegroundWindow();

            CMenu popMenu;
            popMenu.LoadMenu(IDR_MENU_ICON_NOTIFY);
            CMenu *pSubMenu = (CMenu *)popMenu.GetSubMenu(0);

            pSubMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

            popMenu.DestroyMenu();
        }
        break;

    default:
        break;
    }

    return 0;
}

/** @fn       LRESULT CMainDialog::OnUserClose(WPARAM wParam, LPARAM lParam)
 *  @brief    �������������˳���Ϣ��Ӧ
 *  @param
 *  @return:  LRESULT��������
 */
LRESULT CMainDialog::OnUserClose(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    CDialog::OnCancel();

    return 0;
}

/**
*  @fn void CMainDialog::SetListControlServicesHeader
*  @brief   ���� Service List ��ʽ
*  @param   void
*  @return  void
*/
void CMainDialog::SetListControlServicesHeader(void)
{
    // �����
    CRect lcRect;
    m_listService.GetClientRect(&lcRect);

    TCHAR szServerBuff[MAX_PATH] = {0};
    TCHAR szStatusBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_SERVER, szServerBuff, _countof(szServerBuff));
    MultiLanRes_LoadString(IDS_STATUS, szStatusBuff, _countof(szStatusBuff));

    TCHAR titles[2][32] = {0};
    _tcscpy_s(titles[0], szServerBuff);
    _tcscpy_s(titles[1], szStatusBuff);

    int colWidth[2] = { lcRect.Width() - 118, 100 };

    LV_COLUMN column;
    for(int i = 0; i < 2; i++)
    {
        column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
        column.fmt = LVCFMT_LEFT;
        column.pszText = titles[i];
        column.iSubItem = i;
        column.iOrder = i;
        column.cx = colWidth[i];

        m_listService.InsertColumn(i, &column);
    }

    m_listService.SetItemHeight(26);

    CBitmap icon;
    icon.LoadBitmap(IDB_BITMAP_ICON_START);
    m_listImageList.Create(26, 26, ILC_COLOR32 | ILC_MASK, 2, 1);     
    m_listImageList.Add(&icon, RGB(255, 255, 255));
    m_listService.SetImageList(&m_listImageList, LVSIL_SMALL);
}

/** @fn       void CMainDialog::AddNotifyIcon(void)
 *  @brief    ���ϵͳ����ͼ��
 *  @param    void
 *  @return:  void
 */
void CMainDialog::AddNotifyIcon(void)
{
    // ��NOTIFYICONDATA�ṹ��������ʼ��
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd   = m_hWnd;
    m_nid.hIcon  = m_hIcon;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;

    m_nid.uCallbackMessage = WM_ICON_NOTIFY;

    //CString appName(_T("Service Manager Tray"));

    TCHAR szServerNameBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_SERVICE_NAME, szServerNameBuff, _countof(szServerNameBuff));

    lstrcpyn(m_nid.szTip, szServerNameBuff, ARRAYSIZE(m_nid.szTip));

    m_nid.uTimeout = 10;

    lstrcpyn(m_nid.szInfoTitle, szServerNameBuff, ARRAYSIZE(m_nid.szInfoTitle));

    lstrcpyn(m_nid.szInfo, szServerNameBuff, ARRAYSIZE(m_nid.szInfo));

    Shell_NotifyIcon(NIM_ADD, &m_nid);
}

/** @fn       void CMainDialog::RemoveNotifyIcon(void)
 *  @brief    ɾ��ϵͳ����ͼ��
 *  @param    void
 *  @return:  void
 */
void CMainDialog::RemoveNotifyIcon(void)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

/** @fn       void CMainDialog::HideMainWindow(void)
 *  @brief    ����������
 *  @param    void
 *  @return:  void
 */
void CMainDialog::HideMainWindow(void)
{
    ShowWindow(SW_HIDE);
    m_bWindowShowing = false;
}

/** @fn       void CMainDialog::ShowMainWindow(void)
 *  @brief    ��ʾ������
 *  @param    void
 *  @return:  void
 */
void CMainDialog::ShowMainWindow(void)
{
    ShowWindow(SW_RESTORE);
    SetForegroundWindow();

    m_bWindowShowing = true;
}

/** @fn       BOOL CMainDialog::InitServiceController(void)
 *  @brief    ��ʼ���������List
 *  @param    void
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:�����ɹ�
 */
BOOL CMainDialog::InitServiceController(void)
{
    std::list<ServiceConfig>& listAllService = ServiceWatchdogConfig::Instance().m_lstAllService;
//lint -e429
    // ���ط����б�
    for (std::list<ServiceConfig>::iterator itor = listAllService.begin();
        itor != listAllService.end(); itor++)
    {
        ServiceController* pSevCtrol = new ServiceController();
        if (pSevCtrol != NULL)
        {
            pSevCtrol->m_svcName = itor->m_svcName.c_str();
            pSevCtrol->m_svcDisplayName = itor->m_svcDisplayName.c_str();
            pSevCtrol->m_svcShortName = itor->m_svcShortName.c_str();
            pSevCtrol->m_moduleType = itor->m_nModuleType;
            pSevCtrol->setPollInterval(itor->m_pollInterval);
            pSevCtrol->m_svcPath = itor->m_svcPath.c_str();
            pSevCtrol->m_svcStartCmd = itor->m_svcStartCmd.c_str();
            pSevCtrol->m_svcStopCmd = itor->m_svcStopCmd.c_str();
            pSevCtrol->m_svcInstallCmd = itor->m_svcInstallCmd.c_str();
            pSevCtrol->m_svcUninstallCmd = itor->m_svcUninstallCmd.c_str();
            pSevCtrol->m_svcConfigToolName = itor->m_svcConfigToolName.c_str();
            pSevCtrol->m_svcStartTime = itor->m_svcStartTime.c_str();
            pSevCtrol->m_svcStopTime = itor->m_svcStopTime.c_str();
            pSevCtrol->m_svcStartTimeOut = itor->m_svcStartTimeOut;
            pSevCtrol->m_svcStopTimeOut = itor->m_svcStopTimeOut;

            pSevCtrol->m_userData = (void*)(-1); // �÷������б�ؼ�����ʾ��λ�á�

            bool rtn = pSevCtrol->Run();

            m_lstServiceController.push_back(pSevCtrol);

            if (rtn)
            {
                SERVICE_INFO srvInfo;
                srvInfo.strSrvName = itor->m_svcDisplayName.c_str();
                srvInfo.nModuleType = itor->m_nModuleType;
                m_viewDlg.m_listServiceInfo.push_back(srvInfo);
            }

            // �����������ķ���·��
            if (MT_SERVICE_DC == itor->m_nModuleType)
            {
                theApp.m_strDcPath = itor->m_svcPath.c_str();
            }

            TRACE("ServiceWatchdog>> Service [%s] Controller Run %s .\n", itor->m_svcName.c_str(), rtn ? "succeeded" : "failed");
        }
    }
//lint +e429
    return TRUE;
}

/** @fn       void CMainDialog::OnBnClickedMinimize()
 *  @brief    ��С����ť��Ӧ
 *  @param
 *  @return:  void
 */
void CMainDialog::OnBnClickedMinimize()
{
    ShowWindow(SW_MINIMIZE);
}

/** @fn       void CMainDialog::OnBnClickedExit()
 *  @brief    �˳���ť��Ӧ
 *  @param
 *  @return:  void
 */
void CMainDialog::OnBnClickedExit()
{
    OnClose();
}

/** @fn       void CMainDialog::OnBnClickedButtonStopAllService()
 *  @brief    ֹͣ���з���
 *  @param
 *  @return:  void
 */
void CMainDialog::OnBnClickedButtonStopAllService()
{
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin(); itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            if (pSevCtrol->getServiceExists())
            {
                bool rtn = pSevCtrol->StopSvc();

                CString csTmp;
                csTmp.Format(_T("ServiceWatchdog>> Service [%s] Controller Stop %s .\n"), pSevCtrol->m_svcName.c_str(), rtn ? _T("succeeded") : _T("failed"));
                OutputDebugString(csTmp);
            }
        }
    }
}

/** @fn       void CMainDialog::OnBnClickedButtonRestartAllService()
 *  @brief    �������з���
 *  @param
 *  @return:  void
 */
void CMainDialog::OnBnClickedButtonRestartAllService()
{
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin(); itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            if (pSevCtrol->getServiceExists())
            {
                bool rtn = pSevCtrol->RestartSvc();

                CString csTmp;
                csTmp.Format("ServiceWatchdog>> Service [%s] Controller Restart %s .\n", pSevCtrol->m_svcName.c_str(), rtn ? "succeeded" : "failed");
                OutputDebugString(csTmp);
            }
        }
    }
}

/** @fn       void CMainDialog::OnIconnotifyShowWindow()
 *  @brief    ��ʾ���ڲ˵�����Ӧ
 *  @param
 *  @return:  void
 */
void CMainDialog::OnIconnotifyShowWindow()
{
    ShowMainWindow();
}

/** @fn       void CMainDialog::OnIconnotifyHideWindow()
 *  @brief    ���ش��ڲ˵�����Ӧ
 *  @param
 *  @return:  void
 */
void CMainDialog::OnIconnotifyHideWindow()
{
    HideMainWindow();
}

/** @fn       void CMainDialog::OnIconnotifyExit()
 *  @brief    �˳��˵�����Ӧ
 *  @param
 *  @return:  void
 */
void CMainDialog::OnIconnotifyExit()
{
    CDialog::OnCancel();
}

/** @fn       void CMainDialog::OnUpdateIconnotifyShowWindow(CCmdUI *pCmdUI)
 *  @brief    �������̲˵��е���ʾ���ڲ˵���״̬
 *  @param
 *  @return:  void
 */
void CMainDialog::OnUpdateIconnotifyShowWindow(CCmdUI *pCmdUI)
{
    TCHAR szTextBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_SHOW_WINDOW, szTextBuff, _countof(szTextBuff));

    if (pCmdUI != NULL)
    {
        pCmdUI->SetText(szTextBuff);

        pCmdUI->Enable(m_bWindowShowing ? FALSE : TRUE);
    } 
}

/** @fn       void CMainDialog::OnUpdateIconnotifyHideWindow(CCmdUI *pCmdUI)
 *  @brief    �������̲˵��е����ش��ڲ˵���״̬
 *  @param
 *  @return:  void
 */
void CMainDialog::OnUpdateIconnotifyHideWindow(CCmdUI *pCmdUI)
{
    TCHAR szTextBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_HIDE_WINDOW, szTextBuff, _countof(szTextBuff));

    if (pCmdUI != NULL)
    {
        pCmdUI->SetText(szTextBuff);
        pCmdUI->Enable(m_bWindowShowing ? TRUE : FALSE);
    }
}

/** @fn       void CMainDialog::OnUpdateIconnotifyExit(CCmdUI *pCmdUI)
 *  @brief    �������̲˵��е��˳��˵���״̬
 *  @param
 *  @return:  void
 */
void CMainDialog::OnUpdateIconnotifyExit(CCmdUI *pCmdUI)
{
    TCHAR szTextBuff[MAX_PATH] = {0};
    MultiLanRes_LoadString(IDS_EXIT, szTextBuff, _countof(szTextBuff));
    if (pCmdUI != NULL)
    {
        pCmdUI->SetText(szTextBuff);
    }
}

/** @fn       void CMainDialog::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
 *  @brief    ��ʼ���˵��ĵ���
 *  @param
 *  @return:  void
 */
void CMainDialog::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    // TODO: Add your message handler code here
    ASSERT(pPopupMenu != NULL);
    // Check the enabled state of various menu items.

    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
        // Child windows don't have menus--need to go to the top!
        if (pParent != NULL &&
            (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
            int nIndexMax = ::GetMenuItemCount(hParentMenu);
            for (int index = 0; index < nIndexMax; index++)
            {
                if (::GetSubMenu(hParentMenu, index) == pPopupMenu->m_hMenu)
                {
                    // When popup is found, m_pParentMenu is containing menu.
                    state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                    break;
                }
            }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
        state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
            continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        if (state.m_nID == (UINT)-1)
        {
            // Possibly a popup menu, route to first item of that popup.
            state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
            if (state.m_pSubMenu == NULL ||
                (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
                state.m_nID == (UINT)-1)
            {
                continue;       // First item of popup can't be routed to.
            }
            state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
            // Normal menu item.
            // Auto enable/disable if frame window has m_bAutoMenuEnable
            // set and command is _not_ a system command.
            state.m_pSubMenu = NULL;
            state.DoUpdate(this, FALSE);
        }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
            state.m_nIndex -= (state.m_nIndexMax - nCount);
            while (state.m_nIndex < nCount &&
                pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
            {
                state.m_nIndex++;
            }
        }
        state.m_nIndexMax = nCount;
    }
}

/** @fn     void CMainDialog::OnBnClickButtonInstantShowInfo(void)
 *  @brief  ��ʱ��Ϣ��ť����Ӧ����ʾ��ʱ��Ϣ
 *  @param  void
 *  @return void
 */
void CMainDialog::OnBnClickedButtonInstantShowInfo(void)
{
    if(m_nDlgFlag != VIEW_MSG_DLG)
    {
        if (SVC_CFG_DLG == m_nDlgFlag)
        {
            if (TRUE == m_svcCfgDlg.CheckIsChanged())
            {
                int nRet = MessageBox(_T("���������Ѹı䣬�����µ�����?"), _T("����"), MB_OKCANCEL);
                if (IDOK == nRet)
                {
                    return;
                }
            }
        }

        //��ʾ��Ϣ�б�
        m_viewDlg.ShowWindow(SW_SHOW);
        
        m_listService.ShowWindow(SW_HIDE);
        m_svcCfgDlg.ShowWindow(SW_HIDE);

        m_nDlgFlag = VIEW_MSG_DLG;
    
        Invalidate(TRUE);
        UpdateWindow();
    }
}

/** @fn       void CMainDialog::OnBnClickedButtonServiceList(void)
 *  @brief    �������б�ť����Ӧ,��ʾ�������б�
 *  @param    void
 *  @return:  void
 */
void CMainDialog::OnBnClickedButtonServiceList(void)
{
    if (m_nDlgFlag != SVC_LIST_DLG)
    {
        if (SVC_CFG_DLG == m_nDlgFlag)
        {
            if (TRUE == m_svcCfgDlg.CheckIsChanged())
            {
                int nRet = MessageBox(_T("���������Ѹı䣬�����µ�����?"), _T("����"), MB_OKCANCEL);
                if (IDOK == nRet)
                {
                    return;
                }
            }
        }

        // ��ʾ�����б�
        m_listService.ShowWindow(SW_SHOW);

        m_viewDlg.ShowWindow(SW_HIDE);
        m_svcCfgDlg.ShowWindow(SW_HIDE);

        m_nDlgFlag = SVC_LIST_DLG;

        Invalidate(TRUE);
        UpdateWindow();
    }
}

/** @fn       void CMainDialog::OnBnClickedButtonConfig(void)
 *  @brief    �������ð�ť��Ӧ
 *  @param    void
 *  @return:  void
 */
void CMainDialog::OnBnClickedButtonConfig(void)
{
    if(m_nDlgFlag != SVC_CFG_DLG)
    {
        //��������ҳ
        m_svcCfgDlg.InitDlg(theApp.m_nAlarmProcessor);
        m_svcCfgDlg.ShowWindow(SW_SHOW);
        m_viewDlg.ShowWindow(SW_HIDE);
        m_listService.ShowWindow(SW_HIDE);

        m_nDlgFlag = SVC_CFG_DLG;

        Invalidate(TRUE);
        UpdateWindow();
    }
}

/** @fn       void CMainDialog::OnNMRClickListService(NMHDR *pNMHDR, LRESULT *pResult)
 *  @brief    �������б�ѡ������һ���Ӧ������һ���˵���
 *  @param
 *  @return:  void
 */
void CMainDialog::OnNMRClickListService(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int iItem = pNMItemActivate->iItem;

    CMenu cMenu;
    if (NULL == cMenu.LoadMenu(IDR_MENU_SERVICE))
    {
        return;
    }

    LV_ITEM lvitem = {0};
    lvitem.iItem = iItem;
    lvitem.iSubItem = 0;
    lvitem.mask = LVIF_TEXT | LVIF_PARAM;

    if (m_listService.GetItem(&lvitem))
    {
        ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;

        if (pSevCtrol != NULL)
        {
            DWORD status = pSevCtrol->getServiceStatus();

            // ��ʾ��ͬ��״̬
            switch (status)
            {
            case SERVICE_STOPPED:
                {
                    cMenu.GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
                break;
            case SERVICE_PAUSED:
                {
                    cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
                break;
            case SERVICE_RUNNING:
                {
                    cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
                break;
            case SERVICE_START_PENDING:
            case SERVICE_CONTINUE_PENDING:
            case SERVICE_STOP_PENDING:
            case SERVICE_PAUSE_PENDING:
                {
                    cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
                break;
            default:
                {
                    cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                    cMenu.GetSubMenu(0)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
                }
                break;
            }
            /* start bat */
            if (pSevCtrol->m_svcStartCmd.empty())
            {
                cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
            /* stop bat */
            if (pSevCtrol->m_svcStopCmd.empty())
            {
                cMenu.GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
            /* Install bat */
            if (pSevCtrol->m_svcInstallCmd.empty())
            {
                cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
            /* Uninstall bat */
            if (pSevCtrol->m_svcUninstallCmd.empty())
            {
                cMenu.GetSubMenu(0)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
            /* ���ù��߲����� */
            if (pSevCtrol->m_svcConfigToolName.empty())
            {
                cMenu.GetSubMenu(0)->EnableMenuItem(7, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
        }
        else    // if (pSevCtrol != NULL) ... else ...
        {
            cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            cMenu.GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            cMenu.GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            cMenu.GetSubMenu(0)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            //cMenu.GetSubMenu(0)->EnableMenuItem(7, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            //cMenu.GetSubMenu(0)->EnableMenuItem(8, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        }
    }
    else    // if (m_listService.GetItem(&lvitem)) ... else ...
    {
        cMenu.GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(7, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        //cMenu.GetSubMenu(0)->EnableMenuItem(8, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        //cMenu.GetSubMenu(0)->EnableMenuItem(15, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
    }

    /* �ж��Ƿ����еķ��񶼲����� */
    if (TRUE == IsAllServiceNotExist())
    {
        cMenu.GetSubMenu(0)->EnableMenuItem(10, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(11, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
        cMenu.GetSubMenu(0)->EnableMenuItem(13, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
    }

    /* �ж��Ƿ����еķ����� */
    if (TRUE == IsAllServiceExist())
    {
        cMenu.GetSubMenu(0)->EnableMenuItem(12, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
    }

    CPoint pt(0,0);
    GetCursorPos(&pt);
    cMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

    *pResult = 0;
}

/** @fn       BOOL CMainDialog::InitButton(void)
 *  @brief    ��ʼ��Button��ť
 *  @param    void
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::InitButton(void)
{
    TCHAR szViewInstantMessage[MAX_PATH] = {0};
    TCHAR szServiceListMessage[MAX_PATH] = {0};
    TCHAR szServiceCfgMessage[MAX_PATH] = {0};

    MultiLanRes_LoadString(IDS_VIEW, szViewInstantMessage, _countof(szViewInstantMessage));
    MultiLanRes_LoadString(IDS_SERVICE_LIST,szServiceListMessage, _countof(szServiceListMessage) );
    MultiLanRes_LoadString(IDS_SERVICE_CFG, szServiceCfgMessage, _countof(szServiceCfgMessage));

    m_buttonViewIstantMessage.SetWindowText(szViewInstantMessage);
    m_buttonServiceList.SetWindowText(szServiceListMessage);
    m_btnCfg.SetWindowText(szServiceCfgMessage);
    return TRUE;
}

/** @fn       BOOL CMainDialog::LoadWatchBitMap(BOOL bIsRedAndBlack)
 *  @brief    ���ؿ��ſڵ�Ƥ��
 *  @param    (BOOL) bIsRedAndBlack [IN] : TRUE:���Ƥ�� ; FALSE:ǳ��Ƥ��
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::LoadWatchBitMap(BOOL bIsRedAndBlack)
{
    if (TRUE == bIsRedAndBlack) // ���
    {
        LoadSkin(IDB_BITMAP_RED_BLACK_MAIN_MODIFY);

        m_buttonMin.Load(IDB_BITMAP_RED_BLACK_MINIMIZE);
        m_buttonExit.Load(IDB_BITMAP_RED_BLACK_EXIT);

        m_buttonViewIstantMessage.Load(IDB_BITMAP_RED_BLACK_MESSIGE, TRUE);
        m_buttonServiceList.Load(IDB_BITMAP_RED_BLACK_SERVER_LIST, TRUE);
        m_btnCfg.Load(IDB_BITMAP_RED_BLACK_SERVER_CFG, TRUE);

        m_appStat.SetFlags(TRUE);

        m_buttonServiceList.SetFlags(TRUE);
        m_buttonViewIstantMessage.SetFlags(TRUE);
        m_btnCfg.SetFlags(TRUE);
    }
    else    // ����
    {
        LoadSkin(IDB_BITMAP_MAIN_MODIFY);

        m_buttonMin.Load(IDB_BITMAP_BLUE_MINIMIZE, TRUE);
        m_buttonExit.Load(IDB_BITMAP_BLUE_EXIT, TRUE);

        m_buttonViewIstantMessage.Load(IDB_BITMAP_BLUE_MESSIGE, FALSE);
        m_buttonServiceList.Load(IDB_BITMAP_BLUE_SERVER_LIST, FALSE);
        m_btnCfg.Load(IDB_BITMAP_BLUE_SERVER_CFG, FALSE);

        m_appStat.SetFlags(FALSE);

        m_buttonServiceList.SetFlags(FALSE);
        m_buttonViewIstantMessage.SetFlags(FALSE);
        m_btnCfg.SetFlags(FALSE);
    }

    return TRUE;
}

/** @fn     BOOL CMainDialog::InitAddDlg(void)
 *  @brief  ��ʼ��m_viewDlg��m_serviceCfgDlg
 *  @param  void
 *  @return BOOL��TRUE:�ɹ� ; FALSE:ʧ��
 */
BOOL CMainDialog::InitAddDlg(void)
{
    CRect serRect;
    m_listService.GetClientRect(&serRect);
    CRect winRect;
    ::GetWindowRect(m_listService.GetSafeHwnd(), winRect);
    ScreenToClient(winRect);

//     CRect rcClient;
//     GetClientRect(&rcClient);
//     CRect rcRect;
//     rcRect.right =  rcClient.Width() - 11;
//     rcRect.bottom = serRect.Height() + 42;
//     rcRect.top = rcClient.Height() - serRect.Height() - 53;
//     rcRect.left = rcClient.Width() - serRect.Width() - 14;
//     m_viewDlg.MoveWindow(&rcRect);

    m_viewDlg.Create(IDD_DIALOG_VIEWINFO, this);
    m_viewDlg.MoveWindow(winRect);
//    m_viewDlg.MoveWindow(227, 36, 523, 453);
    m_viewDlg.ShowWindow(SW_HIDE);

    m_svcCfgDlg.Create(IDD_DIALOG_CONFIG, this);
    m_svcCfgDlg.MoveWindow(winRect);
    m_viewDlg.ShowWindow(SW_HIDE);

    return TRUE;
}

///** @fn       BOOL CMainDialog::ReconDcServer(void)
// *  @brief    ���³�ʼ�����������ķ���֮�������
// *  @param    void
// *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
// */
//BOOL CMainDialog::ReconDcServer(void)
//{
//    CString csTmp;
//    // ��ʾ�������ķ���IP��ַ�Ͷ˿ں�
//    csTmp.Format(_T("�������ĵ�IP��ַ�Ͷ˿ں�(IP:PROT):%s:%d"), (LPCTSTR)theApp.m_strDCServerIP, theApp.m_nDCPort);
//    CreateSysShowMsg(csTmp, _T(""));
//
//    //// ����������������
//    //BOOL bRet = ReConnDCServer(m_hppDCClientHandle, theApp.m_strDCServerIP, theApp.m_nDCPort);
//
//    csTmp.Format(_T("���������������ķ���%s"), (bRet ? _T("�ɹ�"):_T("ʧ��")) );
//    CreateSysShowMsg(csTmp, _T(""));
//
//    return bRet;
//}

///** @fn       BOOL CMainDialog::ReinitLocalServer(void)
// *  @brief    ���³�ʼ�����ؼ���
// *  @param    void
// *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
// */
//BOOL CMainDialog::ReinitLocalServer(void)
//{
//    CString csTmp;
//    // ��ʾ����IP�Ͷ˿ں�
//    csTmp.Format(_T("���ص�IP��ַ�Ͷ˿ں�(IP:PORT):%s:%d"), (LPCTSTR)theApp.m_hostIP, theApp.m_nListenPort);
//    CreateSysShowMsg(csTmp, _T(""));
//
//    CGuard guard(&m_hppLocalSevHanldeLock);
//    // ���³�ʼ�����ؼ�������
//    BOOL bRet = ReStartLocalServer(m_hppLocalHandle, m_hppLocalSerHanlde, theApp.m_nListenPort);
//
//    csTmp.Format(_T("���³�ʼ�����ؼ�������%s"), (bRet ? _T("�ɹ�"):_T("ʧ��")));
//    CreateSysShowMsg(csTmp, _T(""));
//
//    return bRet;
//}

/** @fn       void CMainDialog::MenuStartService(void)
*  @brief    �˵�����������ѡ�����Ӧ����������
*  @param    void
*  @return:  void
 */
void CMainDialog::MenuStartService(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuStartService SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcStartCmd.c_str());

                pSevCtrol->SetCommandStop(false);
                //��������
                RunCmd(runCmd, runDir);

                pSevCtrol->StartSvcStatus();
            }
        }
    }
}

/** @fn       void CMainDialog::MenuStopService(void)
*  @brief    �˵���ֹͣ����ѡ�����Ӧ��ֹͣ����
*  @param    void
*  @return:  void
 */
void CMainDialog::MenuStopService(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuStopService SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcStopCmd.c_str());

                pSevCtrol->SetCommandStop();
                //��������
                RunCmd(runCmd, runDir);

                pSevCtrol->StopSvcStatus();
            }
        }
    }
}

/** @fn       void CMainDialog::MenuRestartService(void)
 *  @brief    �˵�����������ѡ�����Ӧ����������
 *  @param    void
 *  @return:  void
 */
void CMainDialog::MenuRestartService(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();

    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
            if (pSevCtrol != NULL)
            {
                pSevCtrol->RestartSvc();
            }
        }
    }
}

/** @fn       void CMainDialog::MenuInstallALLService(void)
 *  @brief    �˵�����ȫ����װ��Ӧ����װ���в����õķ���
 *  @param    viod
 *  @return:  void
 */
void CMainDialog::MenuInstallALLService(void)
{
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            // ��װ���� 
            if ( (!pSevCtrol->m_svcInstallCmd.empty()) && (!pSevCtrol->getServiceExists()))
            {
                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcInstallCmd.c_str());

                // ��������
                RunCmd(runCmd, runDir);

                pSevCtrol->SetCommandStop(false);
            }  
        }
    }
}

/** @fn       void CMainDialog::MenuInstallService(void)
*  @brief    �˵��װ����ѡ�����Ӧ����װ����
*  @param    void
*  @return:  void
 */
void CMainDialog::MenuInstallService(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;

            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuInstallService SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcInstallCmd.c_str());

                //��������
                RunCmd(runCmd, runDir);

                pSevCtrol->SetCommandStop(false);
            }
        }
    }
}

/** @fn       void CMainDialog::MenuUninstallService(void)
*  @brief    �˵���ж�ط���ѡ�����Ӧ��ж�ط���
*  @param    void
 *  @return:  void
 */
void CMainDialog::MenuUninstallService(void)
{
    int nRet = MessageBox(_T("ȷ��ж�ط���?"), _T("����"), MB_OKCANCEL);
    if (nRet != IDOK)
    {
        return;
    }

    POSITION pos = m_listService.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuUninstallService SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcUninstallCmd.c_str());


                // 
                pSevCtrol->SetCommandStop();

                //��������
                RunCmd(runCmd, runDir);
            }
        }
    }
}

/** @fn       void CMainDialog::MenuAllUninstallService(void)
 *  @brief    �˵�����һ��ж����Ӧ��ж�����з���
 *  @param    viod
 *  @return:  void
 */
void CMainDialog::MenuUninstallAllService(void)
{
    int nRet = MessageBox(_T("ȷ��ж�����з���?"), _T("����"), MB_OKCANCEL);
    if (nRet != IDOK)
    {
        return;
    }

    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            // ж�ط���
            if ( (!pSevCtrol->m_svcUninstallCmd.empty()) && (pSevCtrol->getServiceExists()))
            {
                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcUninstallCmd.c_str());

                //
                pSevCtrol->SetCommandStop();
                // ��������
                RunCmd(runCmd, runDir);                
            } 
        }
    }
}

/** @fn       void CMainDialog::MenuOpenConfig(void)
 *  @brief    �˵��������ѡ�����Ӧ�������ù���
 *  @param    void
 *  @return:  void
 */
void CMainDialog::MenuOpenConfig(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();

    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuOpenConfig SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                CString runCmd;
                runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcConfigToolName.c_str());

                //��������
                RunCmd(runCmd, runDir);
                return;
            }
        }
    }

//     {// open Service Manager Config
//         CString runDir;
//         runDir.Format(_T("%s"), CFolder::GetExecutionRoot().GetBuffer());
//         CString runCmd;
//         runCmd.Format(_T("%s%s"), runDir.GetBuffer(0), _T("ServiceManagerCfgTool.exe"));
// 
//         if (CFolder::FileExists(runCmd))
//         {
//             // ��������
//             RunCmd(runCmd, runDir);
//         }
//         else
//         {
//             AfxMessageBox(_T("���ù��߲�����"));
//         }
//     }
}

/** @fn       void CMainDialog::MenuOpenRunDir(void)
 *  @brief    �˵���������ļ�ѡ�����Ӧ���������ļ�
 *  @param    void
 *  @return:  void
 */
void CMainDialog::MenuOpenRunDir(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();

    if (pos != NULL)
    {
        int iItem = m_listService.GetNextSelectedItem(pos);

        LV_ITEM lvitem = {0};
        lvitem.iItem = iItem;
        lvitem.iSubItem = 0;
        lvitem.mask = LVIF_TEXT | LVIF_PARAM;

        if (m_listService.GetItem(&lvitem))
        {
            ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;

            if (pSevCtrol != NULL)
            {
                if (pSevCtrol->m_svcPath.empty())
                {
                    //MessageBox(_T("���·��δ����"), _T("Error"));
                    CString csErrMsg;
                    csErrMsg.Format(_T("MenuOpenRunDir SvcName[%s] relative path not configured.\n"), pSevCtrol->m_svcName.c_str());
                    OutputDebugString(csErrMsg);
                    return;
                }

                CString runDir;
                runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());

                //��������
                RunCmd(runDir, runDir);
                return;
            }
        }
    }

    {// open Service Manager Dir
        CString runDir;
        runDir.Format(_T("%s"), CFolder::GetExecutionRoot().GetBuffer());

        //��������
        RunCmd(runDir, runDir);
    }
}

/** @fn       void CMainDialog::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    ��������
 *  @param    (CString) cmd [IN] : ShellExecute������
 *            (CString) DefaultDir [IN] : Ĭ�ϵ�����Ŀ¼
 *  @return:  void
 */
BOOL CMainDialog::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // ��־
    SVCMANAGERLOG_INFO(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);

    // ִ������
    HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, NULL, DefaultDir, SW_SHOWNORMAL);

    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        MessageBox(_T("�ڴ治��"), _T("Error"));
        break;
    case ERROR_FILE_NOT_FOUND:
        MessageBox(_T("�ļ�����·��������"), _T("Error"));
        break;
    case ERROR_PATH_NOT_FOUND:
        MessageBox(_T("�ļ�����·��������"), _T("Error"));
        break;
    case ERROR_BAD_FORMAT:
        MessageBox(_T("ERROR_BAD_FORMAT"), _T("Error"));
        break;
    case SE_ERR_ASSOCINCOMPLETE:
        MessageBox(_T("SE_ERR_ASSOCINCOMPLETE"), _T("Error"));
        break;
    case SE_ERR_ACCESSDENIED:
        MessageBox(_T("SE_ERR_ACCESSDENIED"), _T("Error"));
        break;
    case SE_ERR_DDEBUSY:
        MessageBox(_T("SE_ERR_DDEBUSY"), _T("Error"));
        break;
    case SE_ERR_DDEFAIL:
        MessageBox(_T("SE_ERR_DDEFAIL"), _T("Error"));
        break;
    case SE_ERR_DDETIMEOUT:
        MessageBox(_T("SE_ERR_DDETIMEOUT"), _T("Error"));
        break;
    case SE_ERR_DLLNOTFOUND:
        MessageBox(_T("SE_ERR_DLLNOTFOUND"), _T("Error"));
        break;
    case SE_ERR_NOASSOC:
        MessageBox(_T("SE_ERR_NOASSOC"), _T("Error"));
        break;
    case SE_ERR_OOM:
        MessageBox(_T("SE_ERR_OOM"), _T("Error"));
        break;
    case SE_ERR_SHARE:
        MessageBox(_T("SE_ERR_SHARE"), _T("Error"));
        break;
    default:
        bRet = TRUE;
        break;
    }

    OutputDebugString(_T("---------------------> RunCmd end.\n"));
    return bRet;
}

/** @fn       void CMainDialog::OnMuenInstantMsg(void)
 *  @brief    ��ʱ��Ϣ�˵������Ϣ��Ӧ
 *  @param    void
 *  @return:  void
 */
void CMainDialog::MenuInstantMsg(void)
{
    POSITION pos = m_listService.GetFirstSelectedItemPosition();
    if (NULL == pos)
    {
        BOOL bRet = m_viewDlg.ShowInstantMsg(SYS_INFO);
        if (TRUE == bRet)
        {
            OnBnClickedButtonInstantShowInfo();
        }
        return;
    }

    int iItem = m_listService.GetNextSelectedItem(pos);

    LV_ITEM lvitem = {0};
    lvitem.iItem = iItem;
    lvitem.iSubItem = 0;
    lvitem.mask = LVIF_TEXT | LVIF_PARAM;

    if (m_listService.GetItem(&lvitem))
    {
        ServiceController* pSevCtrol = (ServiceController*)lvitem.lParam;
        if (pSevCtrol != NULL)
        {
            BOOL bRet = m_viewDlg.ShowInstantMsg(pSevCtrol->m_moduleType);
            if (TRUE == bRet)
            {
                OnBnClickedButtonInstantShowInfo();
            }
            else
            {
                MessageBox(_T("��ת��ʱ��Ϣʧ�ܣ���鿴��ʱ��Ϣ�б��Ƿ����"));
            }
        }
    }
}

/** @fn       void CMainDialog::ShowCtrlMsg(const RECV_CMD_MSG& struRecvCmdMsg)
 *  @brief    ��ʾ������Ϣ
 *  @param    (RECV_CMD_MSG) struRecvCmdMsg [IN] : ������Ϣ
 *  @return:  void
 */
void CMainDialog::ShowCtrlMsg(const RECV_CMD_MSG& struRecvCmdMsg)
{
    // ������Ϣ
    CString csMsg;
    csMsg.Format(_T("%s "), struRecvCmdMsg.remoteIP.c_str());

    if (!struRecvCmdMsg.svcName.empty())
    {
        csMsg.AppendFormat(_T("SvrName:%s "), struRecvCmdMsg.svcName.c_str());
    }

    if (struRecvCmdMsg.nModuleType != 0)
    {
        csMsg.AppendFormat(_T("ModuleType:%d "), struRecvCmdMsg.nModuleType);
    }

    // ������Ϣ�������
    switch (struRecvCmdMsg.nCtrlType)
    {
    case cms_8120::SERVER_STOP:     // ֹͣ����
        {
            csMsg.AppendFormat(_T("cmd:Stop"));
        }
        break;
    case cms_8120::SERVER_START:    // ��ʼ����
        {
            csMsg.AppendFormat(_T("cmd:Start"));
        }
        break;
    case cms_8120::SERVER_INSTALL:  // ��װ����
        {
            csMsg.AppendFormat(_T("cmd:Install"));
        }
        break;
    case cms_8120::SERVER_UNINSTALL:    // ж�ط���
        {
            csMsg.AppendFormat(_T("cmd:Uninstall"));
        }
        break;
    case cms_8120::COMPUTER_RESTART:    // ��������
        {
            csMsg.Format(_T("%s cmd:Restart computer"), struRecvCmdMsg.remoteIP.c_str());
        }
        break;
    case cms_8120::SERVER_RESTART:  // ��������
        {
            csMsg.AppendFormat(_T("cmd:Restart Service"));
        }
        break;
    case cms_8120::SERVER_CMD:      // cmd����
        {
            csMsg.AppendFormat(_T("cmd:Shell cmd"));
        }
        break;
    default:    // δ֪������
        {
            csMsg.AppendFormat(_T("cmd:unknown"));
        }
        break;
    }

    // ��ʾ��Ϣ
    CreateSysShowMsg(csMsg, _T(""));
}

/** @fn       BOOL CMainDialog::OnServiceWatchDogControl(CString svcName, HPR_INT32 controlType)
 *  @brief    ����������Ӧ
 *  @param    (const CString &) svcName [IN] : ������
 *            (HPR_INT32) controlType [IN] : ��������
 *  @return:  BOOL��TRUE:��Ӧ�ɹ� ; FALSE:��Ӧʧ��
 */
BOOL CMainDialog::OnServiceWatchDogControl(const CString& svcName, HPR_INT32 controlType)
{
    for (std::list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (svcName.Compare(pSevCtrol->m_svcName.c_str()) != 0)
        {
            continue;
        }

        // �ҵ���Ӧ������Ӧ��������
        switch (controlType)
        {
        case cms_8120::SERVER_STOP:     // ֹͣ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[SERVER_STOP]"), (LPCTSTR)svcName);
                pSevCtrol->StopSvc();
            }
            break;
        case cms_8120::SERVER_START:    // ��ʼ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[SERVER_START]"), (LPCTSTR)svcName);
                pSevCtrol->StartSvc();
            }
            break;
        case cms_8120::SERVER_RESTART:  // ��������
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[SERVER_RESTART]"), (LPCTSTR)svcName);
                pSevCtrol->RestartSvc();
            }
            break;
        case cms_8120::SERVER_INSTALL:  // ��װ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[SERVER_INSTALL]"), (LPCTSTR)svcName);
                DWORD svcStatus = pSevCtrol->getServiceStatus();
                if (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST == svcStatus)
                {
                    CString runDir;
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                    CString runCmd;
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcInstallCmd.c_str());

                    // ��������
                    RunCmd(runCmd, runDir);
                }
            }
            break;
        case cms_8120::SERVER_UNINSTALL:    // ж�ط���
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[SERVER_UNINSTALL]"), (LPCTSTR)svcName);
                DWORD svcStatus = pSevCtrol->getServiceStatus();
                if (svcStatus != CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST)
                {
                    CString runDir;
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                    CString runCmd;
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcUninstallCmd.c_str());

                    // ��������
                    RunCmd(runCmd, runDir);
                }
            }
            break;
        default:
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl svcName[%s] cmd[UNKNOWN]"), (LPCTSTR)svcName);
            }
            break;
        }
        break;
    }
    return TRUE;
}

/** @fn       BOOL CMainDialog::OnServiceWatchDogControl(HPR_INT32 moduleType, HPR_INT32 controlType)
 *  @brief    ����������Ӧ
 *  @param    (HPR_INT32) moduleType [IN] : �����ģ������
 *            (HPR_INT32) controlType [IN] : ������������
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::OnServiceWatchDogControl(HPR_INT32 moduleType, HPR_INT32 controlType)
{
    for (std::list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (moduleType != pSevCtrol->m_moduleType)
        {
            continue;
        }

        // �ҵ���Ӧ������Ӧ��������
        switch (controlType)
        {
        case cms_8120::SERVER_STOP:     // ֹͣ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[SERVER_STOP]"), moduleType);
                pSevCtrol->StopSvc();
            }
            break;
        case cms_8120::SERVER_START:    // ��ʼ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[SERVER_START]"), moduleType);
                pSevCtrol->StartSvc();
            }
            break;
        case cms_8120::SERVER_RESTART:  // ��������
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[SERVER_RESTART]"), moduleType);
                pSevCtrol->RestartSvc();
            }
            break;
        case cms_8120::SERVER_INSTALL:  // ��װ����
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[SERVER_INSTALL]"), moduleType);
                DWORD svcStatus = pSevCtrol->getServiceStatus();
                if (CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST == svcStatus)
                {
                    CString runDir;
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                    CString runCmd;
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcInstallCmd.c_str());

                    //��������
                    RunCmd(runCmd, runDir);
                }
            }
            break;
        case cms_8120::SERVER_UNINSTALL:    // ж�ط���
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[SERVER_UNINSTALL]"), moduleType);
                DWORD svcStatus = pSevCtrol->getServiceStatus();
                if (svcStatus != CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST)
                {
                    CString runDir;
                    runDir.Format(_T("%s%s\\"), CFolder::GetExecutionRoot().GetBuffer(), pSevCtrol->m_svcPath.c_str());
                    CString runCmd;
                    runCmd.Format(_T("%s%s"), runDir.GetBuffer(), pSevCtrol->m_svcUninstallCmd.c_str());

                    //��������
                    RunCmd(runCmd, runDir);
                }
            }
            break;
        default:
            {
                SVCMANAGERLOG_INFO(_T("OnServiceWatchDogControl moduleType[%d] cmd[UNKNOWN]"), moduleType);
            }
            break;
        }
        break;
    }
    return TRUE;
}

/** @fn       BOOL CMainDialog::OnServiceWatchDogControl(void)
 *  @brief    ����������������
 *  @param    void
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::OnServiceWatchDogControl(void)
{
    for (std::list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        pSevCtrol->StopSvc();
    }

    RestartComputer();

    CDialog::OnCancel();
    return TRUE;
}

/** @fn       BOOL CMainDialog::CreateSysShowMsg(const CString &csMsg, const CString &csMsgResult)
 *  @brief    ����ϵͳ��Ϣ
 *  @param    (const CString &) csMsg [IN] : ϵͳ��Ϣ
 *            (const CString &) csMsgResult [IN] : ��Ϣ���
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::CreateSysShowMsg(const CString &csMsg, const CString &csMsgResult)
{
    if (csMsg.GetLength() <= 0)
    {
        return FALSE;
    }
   
    OutputDebugString(_T("---> CreateSysShowMsg:"));
    OutputDebugString(csMsg);
    OutputDebugString(_T("\n"));

    // ��ʾ������ʾ��Ϣ
    BOOL bRet = m_viewDlg.CreateShowMsg(csMsg, csMsgResult);

    return bRet;
}

///** @fn       BOOL CMainDialog::InitGetThread(void)
// *  @brief    ��ʼ����ȡ��Ϣ���߳�
// *  @param    void
// *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
// */
//BOOL CMainDialog::InitGetThread(void)
//{
//    if (m_hEventGetInfo != NULL)
//    {
//        OutputDebugString(_T("InitThread failed. ERROR:m_hEventGetInfo is not NULL.\n"));
//        return FALSE;
//    }
//    
//    // ��ȡ��Ϣ�̵߳Ŀ����¼�
//    m_hEventGetInfo = CreateEvent(NULL, FALSE, FALSE, NULL);
//    if (NULL == m_hEventGetInfo)
//    {
//        OutputDebugString(_T("InitThread failed. ERROR:CreateEvent return NULL.\n"));
//        return FALSE;
//    }
//
//    // ��ȡ��Ϣ���̴߳���
//    m_hThreadGetInfo = CreateThread(NULL, 0, ThreadForGetInfo, this, 0, NULL);
//    if (NULL == m_hThreadGetInfo)
//    {
//        OutputDebugString(_T("InitThread failed. ERROR:CreateThread return NULL.\n"));
//        CloseHandle(m_hEventGetInfo);
//        m_hEventGetInfo = NULL;
//        return FALSE;
//    }
//
//    return TRUE;
//}

///** @fn       DWORD CMainDialog::ThreadForGetInfo(LPVOID lParam)
// *  @brief    ��ȡ��������Ϣ�����������Ϣ���߳�
// *  @param
// *  @return:  DWORD
// */
//DWORD WINAPI CMainDialog::ThreadForGetInfo(LPVOID lParam)
//{
//    CMainDialog* pMainDlg = (CMainDialog*) lParam;
//    if (NULL == pMainDlg)
//    {
//        return DWORD(1);
//    }
//
//    // ���һ��ͬ��ʱ��
//    time_t lastSyncTime = time(NULL) - cms_8100::SYNC_TIME_INTERVAL;
//
//    // ������ϵͳ��Ϣ
//    SERVER_SYS_INFO SerSysInfo;
//    // ��������Ϣ
//    SerSysInfo.CpuInfo = pMainDlg->GetCpuInfo();
//    // ����ϵͳ�汾
//    SerSysInfo.OsInfo = pMainDlg->GetOSInfo();
//    // ������
//    pMainDlg->GetHostName(SerSysInfo.HostName, SerSysInfo.SN);
//    // ����ϵͳλ��
//    pMainDlg->GetOSType(SerSysInfo.OsType);
//    // ����ʱ��
//    SerSysInfo.startTime = pMainDlg->GetSysStartTime();
//    // Service Manager������ʱ��
//    SerSysInfo.csServiceManagerStartTime = pMainDlg->GetServiceManagerStartTime();
//
//    // ���������Ϣ
//    std::vector<SERVER_PROCESS_INFO> vecSerProcInfo;
//    while(TRUE)
//    {
//        // ������Ϣ
//        pMainDlg->GetNetWorkInfo(SerSysInfo.NetWorkInfo);
//        // ������Ϣ
//        pMainDlg->GetDiskInfo(SerSysInfo.DiskInfo);
//        // �ڴ���Ϣ
//        pMainDlg->GetMemInfo(SerSysInfo.TotalMem, SerSysInfo.UsingMem);
//        // CPU��Ϣ
//        SerSysInfo.CpuSysWorking = pMainDlg->GetCpu();
//        // ��������Ϣ
//        
//        // ��ȡ���������Ϣ
//        vecSerProcInfo.clear();
//        for (std::list<ServiceController*>::iterator itor = pMainDlg->m_lstServiceController.begin();
//            itor != pMainDlg->m_lstServiceController.end(); itor++)
//        {
//            ServiceController* pSevCtrol = *itor;
//            if (pSevCtrol != NULL)
//            {
//                SERVER_PROCESS_INFO serProcInfo;
//
//                // ��ȡ������̵���Ϣ
//                pSevCtrol->GetServerProcInfo(serProcInfo);
//
//                vecSerProcInfo.push_back(serProcInfo);
//            }
//        }
//
//        // ����/�����������ķ���
//        if ( (NULL == pMainDlg->m_hppDCClientHandle) || (NULL == g_pCMSConnSession))
//        {
//            if ( (strcmp(theApp.m_strDCServerIP, _T("")) != 0)
//                && (theApp.m_nDCPort > 0) && (theApp.m_nDCPort <= 65535))
//            {
//                CGuard guard(&pMainDlg->m_hppDCClientHandleLock);
//                CString csTmp;
//                if (NULL == pMainDlg->m_hppDCClientHandle)
//                {
//                    csTmp.Format(_T("�����������ķ���"));
//                }
//                else
//                {
//                    csTmp.Format(_T("�����������ķ���"));
//                }
//
//                BOOL bRet = InitCMSClientHppHandle(pMainDlg->m_hppDCClientHandle, theApp.m_strDCServerIP, theApp.m_nDCPort);
//
//                csTmp.AppendFormat(_T("%s"), (bRet ? _T("�ɹ�"):_T("ʧ��")) );
//                pMainDlg->CreateSysShowMsg(csTmp, _T(""));
//            }
//        }
//
//        if (g_pCMSConnSession != NULL)
//        {
//            CGuard guard(&pMainDlg->m_hppDCClientHandleLock);
//            // Уʱ
//            if (TRUE == theApp.m_bCorrTimeFlag)
//            {
//                time_t curTime = time(NULL);
//                if (difftime(curTime, lastSyncTime) >= cms_8100::SYNC_TIME_INTERVAL)
//                {
//                    pMainDlg->SendReqServerData();
//                    lastSyncTime = curTime;
//                }
//            }
//
//            // ����ȡ����Ϣת����XML����
//            pMainDlg->SendInfoToCMS(SerSysInfo, vecSerProcInfo);
//        }
//        else
//        {
//            OutputDebugString(_T("NULL == g_pCMSConnSession.\n"));
//            //SVCMANAGERLOG_ERROR(_T("NULL == g_pCMSConnSession."));
//        }
//
//        // wait 8s and exit control
//        if (WAIT_OBJECT_0 == WaitForSingleObject(pMainDlg->m_hEventGetInfo, 8 * 1000))
//        {
//            break;
//        }
//    }
//
//    return DWORD(0);
//}
//
/** @fn       void CMainDialog::CloseGetThread(void)
 *  @brief    �رջ�ȡ��Ϣ���߳�
 *  @param    void
 *  @return:  void
 */
void CMainDialog::CloseGetThread(void)
{
    // �˳���ȡ��Ϣ�߳�
    if (m_hEventGetInfo != NULL)
    {
        if (m_hThreadGetInfo != NULL)
        {
            SetEvent(m_hEventGetInfo);

            WaitForSingleObject(m_hThreadGetInfo, INFINITE);

            // �ر��߳�
            CloseHandle(m_hThreadGetInfo);
            m_hThreadGetInfo = NULL;
        }

        // �رտ����¼�
        CloseHandle(m_hEventGetInfo);
        m_hEventGetInfo = NULL;
    }
    else if (m_hThreadGetInfo != NULL)
    {
        // �ر��߳�
        CloseHandle(m_hThreadGetInfo);
        m_hThreadGetInfo = NULL;
    }
}

/** @fn       BOOL CMainDialog::InitCtrlMsgThread(void)
 *  @brief    ��ʼ��������Ϣ������߳�
 *  @param    void
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::InitCtrlMsgThread(void)
{
    if (m_hCtrlMsgEvent != NULL)
    {
        SVCMANAGERLOG_ERROR(_T("InitCtrlMsgThread failed. m_hCtrlMsgEvent != NULL."));
        return FALSE;
    }

    // �����¼��Ĵ���
    m_hCtrlMsgEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (NULL == m_hCtrlMsgEvent)
    {
        SVCMANAGERLOG_ERROR(_T("InitCtrlMsgThread failed. After CreateEvent. m_hCtrlMsgEvent == NULL."));
        return FALSE;
    }

    // ��ȡ��Ϣ���̴߳���
    m_hCtrlMsgThread = CreateThread(NULL, 0, ThreadForCtrlMsg, this, 0, NULL);
    if (NULL == m_hCtrlMsgThread)
    {
        SVCMANAGERLOG_ERROR(_T("InitCtrlMsgThread failed. After CreateEvent. m_hCtrlMsgThread == NULL."));
        CloseHandle(m_hCtrlMsgEvent);
        m_hCtrlMsgEvent = NULL;
        return FALSE;
    }

    return TRUE;
}

/** @fn       DWORD WINAPI CMainDialog::ThreadForCtrlMsg(LPVOID lParam)
 *  @brief    ������Ϣ�����̵߳�ִ�к���
 *  @param
 *  @return:  DWORD WINAPI
 */
DWORD WINAPI CMainDialog::ThreadForCtrlMsg(LPVOID lParam)
{
    CMainDialog* pMainDlg = static_cast<CMainDialog*> (lParam);
    if (NULL == pMainDlg)
    {
        SVCMANAGERLOG_ERROR(_T("CMainDialog::ThreadForCltrMsg failed. pMainDlg is NULL."));
        return DWORD(1);
    }

    while(TRUE)
    {
        RECV_CMD_MSG struRecvCmdMsg;
        //������Ϣ����
        while (theApp.m_ctrlMsgQueue.Remove(struRecvCmdMsg))
        {
            // ��ʾ������Ϣ
            pMainDlg->ShowCtrlMsg(struRecvCmdMsg);

            // ִ������
            if ((cms_8120::COMPUTER_RESTART == struRecvCmdMsg.nCtrlType))  // ��������
            {
                SVCMANAGERLOG_INFO(_T("ThreadForCtrlMsg SvcName[%s] ModuleType[%d] CtrlType[%d]"), struRecvCmdMsg.svcName.c_str(),
                    struRecvCmdMsg.nModuleType, struRecvCmdMsg.nCtrlType);

                pMainDlg->OnServiceWatchDogControl();
            }
            else if ((cms_8120::SERVER_CMD == struRecvCmdMsg.nCtrlType))    // �����cmd����
            {
                SVCMANAGERLOG_INFO(_T("ThreadForCtrlMsg SvcName[%s] shellCmd[%s] ShellDir[%d] CtrlType[%d]"), struRecvCmdMsg.svcName.c_str(),
                    struRecvCmdMsg.shellCmd.c_str(), struRecvCmdMsg.shellDir.c_str(), struRecvCmdMsg.nCtrlType);

                CString cmd = struRecvCmdMsg.shellCmd.c_str();
                CString DefaultDir = struRecvCmdMsg.shellDir.c_str();
                ShellExecute(NULL, _T("open"), cmd, NULL, DefaultDir, SW_HIDE);
            }
            else if ((!struRecvCmdMsg.svcName.empty())) // ������
            {
                SVCMANAGERLOG_INFO(_T("ThreadForCtrlMsg SvcName[%s] ModuleType[%d] CtrlType[%d]"), struRecvCmdMsg.svcName.c_str(),
                    struRecvCmdMsg.nModuleType, struRecvCmdMsg.nCtrlType);

                CString ServerName;
                ServerName.Format("%s", struRecvCmdMsg.svcName.c_str());
                ServerName.Trim();

                pMainDlg->OnServiceWatchDogControl(ServerName, struRecvCmdMsg.nCtrlType);
            }
            else if (struRecvCmdMsg.nModuleType != 0)   // ��������
            {
                SVCMANAGERLOG_INFO(_T("ThreadForCtrlMsg SvcName[%s] ModuleType[%d] CtrlType[%d]"), struRecvCmdMsg.svcName.c_str(),
                    struRecvCmdMsg.nModuleType, struRecvCmdMsg.nCtrlType);

                pMainDlg->OnServiceWatchDogControl(struRecvCmdMsg.nModuleType, struRecvCmdMsg.nCtrlType);
            }
        }

        // �߳��˳������¼�
        if (WAIT_OBJECT_0 == WaitForSingleObject(pMainDlg->m_hCtrlMsgEvent, 100))
        {
            break;
        }
    }

    return DWORD(0);
}

/** @fn       void CMainDialog::CloseCtrlMsgThread(void)
 *  @brief    �رտ�����Ϣ������߳�
 *  @param    void
 *  @return:  void
 */
void CMainDialog::CloseCtrlMsgThread(void)
{
    // �˳���ȡ��Ϣ�߳�
    if (m_hCtrlMsgEvent != NULL)
    {
        if (m_hCtrlMsgThread != NULL)
        {
            SetEvent(m_hCtrlMsgEvent);

            WaitForSingleObject(m_hCtrlMsgThread, INFINITE);

            // �ر��߳�
            CloseHandle(m_hCtrlMsgThread);
            m_hCtrlMsgThread = NULL;
        }

        // �رտ����¼�
        CloseHandle(m_hCtrlMsgEvent);
        m_hCtrlMsgEvent = NULL;
    }
    else if (m_hCtrlMsgThread != NULL)
    {
        // �ر��߳�
        CloseHandle(m_hCtrlMsgThread);
        m_hCtrlMsgThread = NULL;
    }
}

/** @fn       BOOL CMainDialog::InitFileTime(void)
 *  @brief    ��ʼ������ϵͳCPU��FILETIME
 *  @param    void
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::InitFileTime(void)
{
    memset(&m_preIdleTime, 0, sizeof(FILETIME));
    memset(&m_preKernelTime, 0, sizeof(FILETIME));
    memset(&m_preUserTime, 0, sizeof(FILETIME));
    memset(&m_curIdleTime, 0, sizeof(FILETIME));
    memset(&m_curKernelTime, 0, sizeof(FILETIME));
    memset(&m_curUserTime, 0, sizeof(FILETIME));

    return TRUE;
}

/** @fn       int CMainDialog::GetCpu(void)
 *  @brief    ��ȡϵͳ��CPUռ����
 *  @param    void
 *  @return:  int��ϵͳCPUռ���ʣ�%��
 */
int CMainDialog::GetCpu(void)
{
    int cpu = 0;
    BOOL bRet = FALSE;
    if ( (0 == m_preKernelTime.dwHighDateTime) && (0 == m_preKernelTime.dwLowDateTime))
    {
        bRet = GetSystemTimes(&m_preIdleTime, &m_preKernelTime, &m_preUserTime);
        if (FALSE == bRet)
        {
            InitFileTime();
        }
    }
    else
    {
        // ��ȡ��ǰϵͳʱ��
        bRet = GetSystemTimes(&m_curIdleTime, &m_curKernelTime, &m_curUserTime);

        if (TRUE == bRet)
        {
            int idleTime = (int) (GetFileTimeInterval(m_preIdleTime, m_curIdleTime) / 10000);
            int kernelTime = (int) (GetFileTimeInterval( m_preKernelTime, m_curKernelTime) / 10000);
            int userTime = (int) (GetFileTimeInterval(m_preUserTime, m_curUserTime)/ 10000);

            // cpu������
            int cpuidle = 100 * (idleTime)/(kernelTime+userTime);

            // cpuʹ����
            cpu = 100 - cpuidle;

            //int cpu = (kernelTime + userTime - idleTime)/((kernelTime+userTime)/100);

            m_preIdleTime = m_curIdleTime;
            m_preKernelTime = m_curKernelTime;
            m_preUserTime = m_curUserTime;
        }
    }
    return cpu;
}

/** @fn       BOOL CMainDialog::GetDiskInfo(std::vector<DISK_INFO> &vecDiskInfo)
 *  @brief    ��ȡϵͳ�Ĵ�����Ϣ
 *  @param    (std::vector<DISK_INFO> &) vecDiskInfo [OUT] : ������Ϣ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::GetDiskInfo(std::vector<DISK_INFO> &vecDiskInfo)
{
    // ��ռ�¼
    vecDiskInfo.clear();

    ULARGE_INTEGER FreeAvailable,TotalNum,TotalFreeNum;

    DWORD dwLogical = GetLogicalDriveStrings(0, NULL);
    LPTSTR pAllDrivers;
#ifdef UNICODE
    pAllDrivers = new wchar_t[dwLogical];
#else
    pAllDrivers = new char[dwLogical];
#endif
    ::GetLogicalDriveStrings(dwLogical, pAllDrivers);

    LPTSTR pDriver = pAllDrivers;
    char chTempDriver[27] = {0};

    int  iDriverNum = 0;
    while(pDriver[0] != 0 && (iDriverNum < 27))
    {
        chTempDriver[iDriverNum++] = (char)*pDriver;
        pDriver = _tcschr(pDriver,0)+1;          //��λ����һ���̷�
    }

    if (pAllDrivers != NULL)
    {
        delete [] pAllDrivers;
    }

    char diskName[4];
    diskName[0] = 'A';
    diskName[1] = ':';
    diskName[2] = '\\';
    diskName[3] = 0;

    if (iDriverNum > 0)
    {
        for (int i = 0; i < iDriverNum; i++)
        {
            diskName[0] = chTempDriver[i];
            BOOL bRet = GetDiskFreeSpaceEx(diskName, &FreeAvailable,&TotalNum,&TotalFreeNum);

            if (TRUE == bRet)
            {
                DISK_INFO diskInfo;
                diskInfo.disk = diskName[0];
                diskInfo.totalNum = (int)((TotalNum.QuadPart)/(1024*1024*1024));
                int AvailNum = (int)((FreeAvailable.QuadPart)/(1024*1024*1024));
                diskInfo.UsingNum = diskInfo.totalNum - AvailNum;
                vecDiskInfo.push_back(diskInfo);
            }
        }
    }
    else // �������д��̻�ȡ������Ϣ
    {
        for (int i = 2; i < 27; i++)
        {
            // ��������� A&B
            diskName[0] = (char)('A' + i);
            BOOL bRet = GetDiskFreeSpaceEx(diskName, &FreeAvailable,&TotalNum,&TotalFreeNum);

            if (TRUE == bRet)
            {
                DISK_INFO diskInfo;
                diskInfo.disk = diskName[0];
                diskInfo.totalNum = (int)((TotalNum.QuadPart)/(1024*1024*1024));
                int AvailNum = (int)((FreeAvailable.QuadPart)/(1024*1024*1024));
                diskInfo.UsingNum = diskInfo.totalNum - AvailNum;
                vecDiskInfo.push_back(diskInfo);
            }
        }
    }

    return TRUE;
}

/** @fn       BOOL CMainDialog::GetMemInfo(int &TotalMem, int &UsingMem)
 *  @brief    ��ȡϵͳ�ڴ�
 *  @param    (int &) TotalMem [OUT] : ϵͳ���ڴ棨M��
 *            (int &) UsingMem [OUT] : ϵͳ��ʹ���ڴ棨M��
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::GetMemInfo(int &TotalMem, int &UsingMem)
{
    MEMORYSTATUSEX memStatus;
    memset(&memStatus, 0, sizeof(MEMORYSTATUSEX));
    memStatus.dwLength = sizeof (memStatus);

    // ��ȡϵͳ�ڴ�
    BOOL bRet = GlobalMemoryStatusEx(&memStatus);
    if (TRUE == bRet)
    {
        TotalMem = (int) (memStatus.ullTotalPhys/(1024 * 1024));
        int availPhys = (int)(memStatus.ullAvailPhys/(1024 * 1024));
        UsingMem = TotalMem - availPhys;
    }
    else
    {
        TotalMem = 0;
        UsingMem = 0;
    }

    return TRUE;
}

/** @fn       CString CMainDialog::GetCpuInfo(void)
 *  @brief    ��ȡCPU����Ϣ
 *  @param    void
 *  @return:  CString��CPU����Ϣ
 */
CString CMainDialog::GetCpuInfo(void)
{
    CString cpuInfo;

    HKEY hKey = NULL;
    // ��ѯע���
    long lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);

    if(lResult == ERROR_SUCCESS)
    {
        char chData[128];
        DWORD dwSize;
        dwSize = sizeof(chData);
        RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)chData, &dwSize);
        cpuInfo.Format(_T("%s"), chData);
    }
    else
    {
        cpuInfo.Format(_T("Unknown"));
    }

    if (hKey != NULL)
    {
        RegCloseKey(hKey);
    }

    cpuInfo.Replace("  ", "");

    return cpuInfo;
}

/** @fn       CString CMainDialog::GetOSInfo(void)
 *  @brief    ��ȡϵͳ�汾��Ϣ
 *  @param    void
 *  @return:  CString������ϵͳ�汾��Ϣ
 */
CString CMainDialog::GetOSInfo(void)
{
    // ����ϵͳ��Ϣ
    CString OsInfo;

    // ����ϵͳ�汾��Ϣ
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    // ��ȡϵͳ�汾��Ϣ
    BOOL bRet = FALSE;
    bRet = GetVersionEx((OSVERSIONINFO *) &osvi);
    if (FALSE == bRet)
    {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (FALSE == GetVersionEx((OSVERSIONINFO*) &osvi))
        {
            return OsInfo;
        }
    }

    switch(osvi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_NT:     //Win7/XP/08/2000/2003
        {
            // Test for the specific product family.
            if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
            {
                if( GetSystemMetrics(SM_SERVERR2) )
                {
                    OsInfo = "Windows Server 2003 R2 ";
                    break;
                }
                else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
                {
                    OsInfo = "Windows Storage Server 2003 ";
                    break;
                }
//                 else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
//                 {
//                     OsInfo = "Windows Home Server ";
//                 }
//                 else if( osvi.wProductType == VER_NT_WORKSTATION &&
//                     si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
//                 {
//                     OsInfo = "Windows XP Professional x64 Edition ";
//                 }
                else
                {
                    OsInfo = "Windows Server 2003 ";
                    break;
                } 
            }
            if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            {
                OsInfo = "Microsoft Windows XP ";
                break;
            }
            if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            {
                OsInfo = "Microsoft Windows 2000 ";
                break;
            }
            if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
            {
                if (osvi.wProductType != VER_NT_WORKSTATION)
                {
                    OsInfo = "Microsoft Windows Server 2008 ";
                    break;
                }// else "Windows Vista"
            }
            if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 )
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                {
                    OsInfo = "Microsoft Windows 7 ";
                    break;
                } // else "Windows Server 2008 R2"
            }

//             if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 )
//             {
//                 if (osvi.wProductType == VER_NT_WORKSTATION)
//                 {
//                     OsInfo = "Microsoft Windows 8 ";
//                     break;
//                 } // else "Windows Server 2012"
//             }

            OsInfo = "unsupported system";
        }
        break;
    default:
        {
            OsInfo = "unsupported system";
        }
        break;
    }

    if (strcmp(OsInfo, "unsupported system") != 0)
    {
        OsInfo.AppendFormat("Version %d.%d.%d, %s",
            osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, osvi.szCSDVersion);
    }

    return OsInfo;
}

/** @fn       void CMainDialog::GetHostName(CString &HostName, CString &SN)
 *  @brief    ��ȡ������
 *  @param    void
 *  @return:  void
 */
void CMainDialog::GetHostName(CString &HostName, CString &SN)
{
    char szHostName[50];
    gethostname(szHostName, 50);

    HostName.Format(_T("%s"), szHostName);

    hostent* pHostent = NULL;

    pHostent = gethostbyname(szHostName);

   if (pHostent != NULL)
   {
       SOCKADDR_IN lv_sa;
       lv_sa.sin_family = AF_INET;
       lv_sa.sin_port = htons(GET_HOST_NAME_PORT);
       memcpy(&lv_sa.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
       SN.Format(_T("%s"), inet_ntoa(lv_sa.sin_addr));
   }
}

/** @fn       CString CMainDialog::GetSysStartTime(void)
 *  @brief    ��ȡ����������ʱ��
 *  @param    void
 *  @return:  CString������������ʱ��(YYYY-MM-DD HH:MM:SS)
 */
CString CMainDialog::GetSysStartTime(void)
{
    CString SysStartTime;

    long long time_last;  
    time_last = time(NULL);     // ������
    DWORD dwTickCount = GetTickCount();
    
    time_last -= dwTickCount/1000;
    CTime ctSysStartTime(time_last);
    SysStartTime = ctSysStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));

    return SysStartTime;
}

/** @fn       CString CMainDialog::GetServiceManagerStartTime(void)
 *  @brief    ��ȡService Manager������ʱ��
 *  @param    void
 *  @return:  CString��Service Manager������ʱ��(YYYY-MM-DD HH:MM:SS)
 */
CString CMainDialog::GetServiceManagerStartTime(void)
{
    return theApp.m_startTime.Format(_T("%Y-%m-%d %H:%M:%S"));
}

/** @fn       BOOL CMainDialog::GetOSType(int &OsType)
 *  @brief    ��ȡϵͳ��λ��
 *  @param    (int &) OsType [IN] : ����ϵͳ��λ��(Ϊ0��δ֪)
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::GetOSType(int &OsType)
{
    OsType = 0;

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // == PROCESSOR_ARCHITECTURE_AMD64
    if (PROCESSOR_ARCHITECTURE_INTEL == si.wProcessorArchitecture)
    {
        OsType = 32;
    }
    else if ((PROCESSOR_ARCHITECTURE_AMD64 == si.wProcessorArchitecture)
        || (PROCESSOR_ARCHITECTURE_IA64 == si.wProcessorArchitecture))
    {
        OsType = 64;
    }

    return TRUE;
}

/** @fn       BOOL CMainDialog::GetNetWorkInfo(std::vector<NET_WORK_INFO> &vecNetWorkInfo)
 *  @brief    ��ȡϵͳ��������Ϣ
 *  @param    (std::vector<NET_WORK_INFO> &) vecNetWorkInfo [OUT] : ������Ϣ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL CMainDialog::GetNetWorkInfo(std::vector<NET_WORK_INFO> &vecNetWorkInfo)
{
    // ���
    vecNetWorkInfo.clear();

    IP_ADAPTER_INFO * pAdptInfo = NULL;
    IP_ADAPTER_INFO * pNextAd = NULL; 
    ULONG ulLen = 0;
    DWORD dwRet = 0;

    dwRet = GetAdaptersInfo(pAdptInfo, &ulLen);
    if (ERROR_BUFFER_OVERFLOW == dwRet) 
    {
        pAdptInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T) ulLen);
        dwRet = GetAdaptersInfo(pAdptInfo, &ulLen);  
    }

    if(ERROR_SUCCESS == dwRet) 
    {
        pNextAd = pAdptInfo;
        while(pNextAd != NULL) 
        {
            NET_WORK_INFO NetWorkInfo;

            strcpy_s(NetWorkInfo.IpType, 5, "ipv4");

            IP_ADDR_STRING * pNext = NULL;
            //IP��ַ����������
            pNext = &(pNextAd->IpAddressList);
            while (pNext != NULL)
            {
                IP_ADD_INFO struIpAdd;
                struIpAdd.IpAdd.Format(_T("%s"), pNext->IpAddress.String);
                struIpAdd.csMask.Format(_T("%s"), pNext->IpMask.String);
                if ( (struIpAdd.IpAdd.Compare(_T("0.0.0.0")) != 0) && (struIpAdd.IpAdd.Compare(_T("127.0.0.1")) != 0) 
                    && (struIpAdd.IpAdd.Compare(_T("")) != 0) )
                {
                    NetWorkInfo.vecIPAddAndSubNet.push_back(struIpAdd);
                }

                pNext = pNext->Next;
            }
            // Gateway
            IP_ADDR_STRING *pNextGate = &(pNextAd->GatewayList);
            while (pNextGate != NULL)
            {
                CString csGateway;
                csGateway.Format(_T("%s"), pNextGate->IpAddress.String);
                if (csGateway.Compare(_T("")) != 0)
                {
                    NetWorkInfo.vecGateway.push_back(csGateway);
                }
                pNextGate = pNextGate->Next;
            }

            if (NetWorkInfo.vecIPAddAndSubNet.size() > 0)
            {
                vecNetWorkInfo.push_back(NetWorkInfo);
            }

            pNextAd = pNextAd->Next;
        }
    }

    if (pAdptInfo != NULL)
    {
        HeapFree(GetProcessHeap(), 0, pAdptInfo);
    }

    return TRUE;
}

///** @fn       BOOL CMainDialog::SendInfoToCMS(const SERVER_SYS_INFO& SerSysInfo, const std::vector<SERVER_PROCESS_INFO>& SerProcInfo)
// *  @brief    ��������ϵͳ��Ϣ�ͷ��������Ϣת����XML���͸�CMS
// *  @param    (const SERVER_SYS_INFO&) SerSysInfo [IN] : ������ϵͳ��Ϣ
// *            (const std::vector<SERVER_PROCESS_INFO>&) SerProcInfo [IN] : ���������Ϣ
// *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
// */
//BOOL CMainDialog::SendInfoToCMS(const SERVER_SYS_INFO& SerSysInfo, const std::vector<SERVER_PROCESS_INFO>& SerProcInfo)
//{
//    CString csSerSysInfo;
//    XML_GetServerSysInfo(csSerSysInfo, SerSysInfo);
//    CString csSerProcInfo;
//    XML_GetServerProcInfo(csSerProcInfo, SerProcInfo);
//
//    {// show Info
//        CString csInfo;
//        csInfo.Format(_T("---@Info---------SerSysInfo-------------------\n"));
//        csInfo.AppendFormat(_T("%s"), (LPCTSTR)csSerSysInfo);
//        csInfo.AppendFormat(_T("---@Info---------SerProcInfo------------------\n"));
//        csInfo.AppendFormat(_T("%s"), (LPCTSTR)csSerProcInfo);
//        csInfo.AppendFormat(_T("----------------------------------------------\n"));
//        OutputDebugString(csInfo);
//    }
//
//    if (NULL == g_pCMSConnSession)
//    {
//        OutputDebugString(_T("g_pCMSConnSession == NULL.\n"));
//        return FALSE;
//    }
//    
//    PBWrap<cms_8100::ServerMonitorInfoList>* p_pbWrap_SerMonInfoList = 
//        PBWrapFactory<cms_8100::ServerMonitorInfoList>::CreatePBWrap();
//
//    if (NULL == p_pbWrap_SerMonInfoList)
//    {
//        OutputDebugString(_T("NULL == p_pbWrap_SerMonInfoList.\n"));
//        return FALSE;
//    }
//
//    p_pbWrap_SerMonInfoList->SetCommandId(cms_8100::CMD_DT_SERVER_MON_INFO_LIST);
//    p_pbWrap_SerMonInfoList->SetInnerSequence(HPP_GetUnRepeatSeq());
//    p_pbWrap_SerMonInfoList->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NOT_NEED_RSP);
//    p_pbWrap_SerMonInfoList->SetWrapDstId(g_pCMSConnSession->GetSessionId());
//
//    cms_8100::ServerMonitorInfoList &pbWrap_SerMonInfoList = p_pbWrap_SerMonInfoList->GetMsg();
//    pbWrap_SerMonInfoList.set_data_type(cms_8100::CMD_DT_SERVER_MON_INFO_LIST);
//
//    cms_8100::ServerMonitorInfo* p_pbWrap_SerMonInfo = pbWrap_SerMonInfoList.add_info();
//    p_pbWrap_SerMonInfo->set_svr_dev_sn(SerSysInfo.SN);
//    p_pbWrap_SerMonInfo->set_svr_info(csSerSysInfo);
//    p_pbWrap_SerMonInfo->set_svr_proc_info(csSerProcInfo);
//    p_pbWrap_SerMonInfo->set_watchdog_ip(theApp.m_hostIP);
//    p_pbWrap_SerMonInfo->set_watchdog_port(theApp.m_nListenPort);
//
//    if (HPR_OK != ConnSession::SendMessage_r(p_pbWrap_SerMonInfoList, g_pCMSConnSession->GetSessionId()))
//    {
//        SVCMANAGERLOG_ERROR(_T("Send info to CMS failed."));
//        PBWrapFactory<cms_8100::ServerMonitorInfoList>::DestroyPBWrap(p_pbWrap_SerMonInfoList);
//    }
//    else
//    {
//        OutputDebugString(_T("Send info to CMS success.\n"));
//    }
//
//    return TRUE;
//}

///** @fn       BOOL CMainDialog::SendReqServerData(void)
// *  @brief    ����Уʱ����
// *  @param    void
// *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
// */
//BOOL CMainDialog::SendReqServerData(void)
//{
//    OutputDebugString(_T("SendReqServerData...\n"));
//
//    //�����������ݿ�ʱ�����Ϣ
//    PBWrap<cms_8100::ReqServerData>* p_msg_wrap = PBWrapFactory<cms_8100::ReqServerData>::CreatePBWrap();
//    if (p_msg_wrap == NULL)
//    {
//        SVCMANAGERLOG_ERROR("CreatePBWrap fail ...");
//        return FALSE;
//    }
//
//    //����HPPЭ��ͷ
//    p_msg_wrap->SetCommandId(cms_8100::CMD_REQ_SERVER_DATA);
//    p_msg_wrap->SetInnerSequence(HPP_GetUnRepeatSeq());
//    p_msg_wrap->SetMessageType(HPP_PACK_HEAD::REQ_MSG_NEED_RSP);//��Ҫ��Ӧ
//    p_msg_wrap->SetWrapDstId(g_pCMSConnSession->GetSessionId());
//    //������Ϣ����
//    cms_8100::ReqServerData& r_msg = p_msg_wrap->GetMsg();
//    r_msg.set_cmd(cms_8100::CMD_REQ_SERVER_DATA);
//    r_msg.set_data_type(cms_8100::CMD_DT_DB_INFO);
//    r_msg.set_moudle_type(0);
//    r_msg.set_server_ip(/*CSC::Ansi2Utf8(CConfigFile::GetSingletonPtr()->m_sLocalIP)*/theApp.m_hostIP);
//    r_msg.CheckInitialized();
//
//    if ((NULL == g_pCMSConnSession) || (HPR_OK != ConnSession::SendMessage_r(p_msg_wrap, g_pCMSConnSession->GetSessionId())))
//    {
//        SVCMANAGERLOG_ERROR("SendMessage_r fail ...");
//        PBWrapFactory<cms_8100::ReqServerData>::DestroyPBWrap(p_msg_wrap);
//        return FALSE;
//    }
//
//    return TRUE;
//}

/** @fn       BOOL CMainDialog::RestartComputer(void)
 *  @brief    ��������
 *  @param    void
 *  @return:  BOOL��TRUE:ִ�гɹ� ; FALSE:ִ��ʧ��
 */
BOOL CMainDialog::RestartComputer(void)
{
    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = TRUE;

    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));

    CString csCmd;
    csCmd.Format(_T("shutdown /r /f /t %d"), theApp.m_nRestartInterval);

    BOOL bRet = ::CreateProcess(NULL, (LPSTR)(LPCTSTR)csCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (FALSE == bRet)
    {
        SVCMANAGERLOG_ERROR(_T("RestartComputer failed."));
        return FALSE;
    }

    return TRUE;
}

///** @fn       void CMainDialog::ShowDCAndLocal(void)
// *  @brief    ��ʾDC�ͱ���IP�Ͷ˿ں�
// *  @param    void
// *  @return:  void
// */
//void CMainDialog::ShowDCAndLocal(void)
//{
//    CString csTmp;
//    CString csTmpResult;
//    // ��ʾ�������ĵ�IP��ַ�Ͷ˿ں�
//    csTmp.Format(_T("�������ĵ�IP��ַ�Ͷ˿ں�(IP:PROT):%s:%d"), (LPCTSTR)theApp.m_strDCServerIP, theApp.m_nDCPort);
//    CreateSysShowMsg(csTmp, csTmpResult);
//    // ��ʾ����IP�Ͷ˿ں�
//    csTmp.Format(_T("���ص�IP��ַ�Ͷ˿ں�(IP:PORT):%s:%d"), (LPCTSTR)theApp.m_hostIP, theApp.m_nListenPort);
//    CreateSysShowMsg(csTmp, csTmpResult);
//}

/** @fn       BOOL CMainDialog::IsAllServiceNotExist(void)
 *  @brief    �ж��Ƿ����еķ��񶼲�����
 *  @param    void
 *  @return:  BOOL��TRUE:���з��񶼲����� ; FALSE:�������еķ��񶼲�����
 */
BOOL CMainDialog::IsAllServiceNotExist(void)
{
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            // �жϷ����Ƿ����
            if (pSevCtrol->getServiceExists())
            {
                return FALSE;
            }   
        }    
    }
    return TRUE;
}

/** @fn       BOOL CMainDialog::IsAllServiceExist(void)
 *  @brief    �ж��Ƿ����еķ��񶼴���
 *  @param    void
 *  @return:  BOOL��TRUE:���з��񶼴��� ; FALSE:�������еķ��񶼴���
 */
BOOL CMainDialog::IsAllServiceExist(void)
{
    for (list<ServiceController*>::iterator itor = m_lstServiceController.begin();
        itor != m_lstServiceController.end(); itor++)
    {
        ServiceController* pSevCtrol = *itor;
        if (pSevCtrol != NULL)
        {
            // �жϷ����Ƿ����
            if (!pSevCtrol->getServiceExists())
            {
                return FALSE;
            }   
        }
    }
    return TRUE;
}
