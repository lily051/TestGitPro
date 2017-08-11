/**
*   @file ViewDlg.cpp
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 显示即时消息对话框的实现文件
*   
*   @author mazhentong
*   @date   2012/08/14
*
*   @note 历史记录：create
*	
*   @warning
*/

#include "stdafx.h"

#include "ServiceWatchdog.h"
#include "ViewDlg.h"
//#include <MSG/ServiceWatchDogControl.pb.h>
#include "fclib/fcTchar.h"
#include "XMLFunction.h"
#include "Markup.h"
#include "Folder.h"
#include "./SimpleHttp.h"
#include "./utils.h"
//#include "CollectorProcessor.h"
//#include "CentorProcessor.h"

//#pragma comment(lib, "lib/snmpManager.lib")
using namespace std;


//namespace {
//	TCHAR const INI_CFG_FILE_NAME[] = _T("config.ini");
//	TCHAR const INI_SEC_COLLECTORDBINFO[] = _T("collectorDB");
//	TCHAR const INI_KEY_ODBHOST[] = _T("odbHost");
//	TCHAR const DEFAULT_ODBHOST[] = _T("127.0.0.1");
//	TCHAR const INI_KEY_ODBPORT[] = _T("odbPort");
//	TCHAR const DEFAULT_ODBPORT[] = _T("3306");
//	TCHAR const INI_KEY_ODBUSER[] = _T("odbUser");
//	TCHAR const DEFAULT_ODBUSER[] = _T("root");
//	TCHAR const INI_KEY_ODBPASSWORD[] = _T("odbPassword");
//	TCHAR const DEFAULT_ODBPASSWORD[] = _T("888");
//	TCHAR const INI_KEY_ODBSCHEMA[] = _T("odbSchema");
//	TCHAR const DEFAULT_ODBSCHEMA[] = _T("vpcdb");
//}

// CViewDlg 对话框

IMPLEMENT_DYNAMIC(CViewDlg, CDialog)

//
CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CViewDlg::IDD, pParent)
, m_bExitFlag(FALSE)
, m_listWidth(100)
, m_pProcessor(NULL)
, m_pProcessorCol(NULL)
{
    m_socketServer = INVALID_SOCKET;
    m_handleRecvThread = NULL;

    memset(&m_addrClient, 0, sizeof(SOCKADDR_IN));
    memset(&m_addrServer, 0, sizeof(SOCKADDR_IN));
}

CViewDlg::~CViewDlg()
{
    if (m_handleRecvThread != NULL)
    {
        CloseHandle(m_handleRecvThread);
        m_handleRecvThread = NULL;
    }
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
 //   DDX_Control(pDX, IDC_LIST_VIEW_INSTANT_INFO, m_viewList);
    DDX_Control(pDX, IDC_TAB_VIEWCONTROL, m_viewTab);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VIEWCONTROL, &CViewDlg::OnTcnSelchangeTabViewcontrol)
    ON_WM_DESTROY()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CViewDlg 消息处理程序

BOOL CViewDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    //初始化m_viewTab
    InitTabControl();

    //初始化m_viewList
    InitListControl();

    //初始化m_socketServer
    BOOL bRet = InitSocketServer();
    if (FALSE == bRet)
    {
        CreateShowMsg(_T("初始化接收theService消息的Socket失败"), _T(""));
    }

    //初始化m_handleRecvThread
    InitRecvThread();
    
    {
        InitSrvType();
    }


//     if (0 == theApp.m_nAlarmProcessor)
//     {
//         m_pProcessorCol = new CCollectorProcessor();
//     }
//     else if (1 == theApp.m_nAlarmProcessor)
//     {
//         m_pProcessor = new CCentorProcessor();
//     }
//     else if (2 == theApp.m_nAlarmProcessor)
//     {
//         m_pProcessorCol = new CCollectorProcessor();
//         m_pProcessor = new CCentorProcessor();
//     }
// 
//     if (NULL != m_pProcessor)
//     {
//         m_pProcessor->InitProcessor();
//     }
// 
//     if (NULL != m_pProcessorCol)
//     {
//         m_pProcessorCol->InitProcessor();
//     }

    //设置显示消息的定时器
    SetTimer(VIEWMESSAGE_TIMER, 50, NULL);

    return TRUE;
}

void CViewDlg::OnDestroy()
{
    CDialog::OnDestroy();

    {// 释放资源

        m_bExitFlag = TRUE;

        if (m_socketServer != NULL)     //关闭socket
        {
            closesocket(m_socketServer);
        }

        if (m_handleRecvThread != NULL) //退出线程，关闭句柄
        {
            WaitForSingleObject(m_handleRecvThread, INFINITE);
            CloseHandle(m_handleRecvThread);
            m_handleRecvThread = NULL;
        }

        if (NULL != m_pProcessor)
        {
            m_pProcessor->FiniProcessor();
            delete m_pProcessor;
            m_pProcessor = NULL;
        }

        if (NULL != m_pProcessorCol)
        {
            m_pProcessorCol->FiniProcessor();
            delete m_pProcessorCol;
            m_pProcessorCol = NULL;
        }


        //SnmpManager_StopService();
        //SnmpManager_FiniLib();
    }

    {
        // 销毁显示消息的定时器
        KillTimer(VIEWMESSAGE_TIMER);

        // 获取保存的列表列宽信息
        SaveColWidthInfo();
    }

    {// 即时消息及列表资源的释放
        for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
            itrSrv != m_listServiceInfo.end(); itrSrv++)
        {
            if (itrSrv->pListCtrl != NULL)
            {
                delete itrSrv->pListCtrl;
            }
        }

        //m_msgQueue.RemoveAll();

        while (m_msgQueue.size() > 0)
        {
            dummy_msg_t dummyMsg = {0};
            if (FALSE == m_msgQueue.Pop(dummyMsg))
            {
                break;
            }

            if (NULL == dummyMsg.lpMsg)
            {
                continue;
            }

            ::free(dummyMsg.lpMsg);
        }
    }

    //WSACleanup();
}


void CViewDlg::OnOK()
{
    //CDialog::OnOK();
}

void CViewDlg::OnCancel()
{
//    CDialog::OnCancel();
}

/* 屏蔽Esc、Enter键、F1键 */
BOOL CViewDlg::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam 
                || VK_F1 == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    default:
        break;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/** @fn       void CViewDlg::OnTimer(UINT_PTR nIDEvent)
 *  @brief    定时器
 *  @param    (UINT_PTR) nIDEvent [IN] : 定时事件的ID
 *  @return:   void
 */
void CViewDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
        case VIEWMESSAGE_TIMER:
            {
                //BASIC_MESSAGE *pMsgElem = NULL;
                dummy_msg_t dummyMsg = {0};
                while (m_msgQueue.Pop(dummyMsg))
                {
                    if (NULL == dummyMsg.lpMsg)
                    {
                        continue;
                    }

                    BASIC_MESSAGE * pBaseMsg = (BASIC_MESSAGE *)dummyMsg.lpMsg;

                    for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
                        itrSrv != m_listServiceInfo.end(); itrSrv++)
                    {
                        //添加消息到对应服务器的ListCtrl
                        if (HPR_Ntohl(pBaseMsg->dwSvcType) != itrSrv->nModuleType	///< 8120相关模块进行了字节序转换
                            && pBaseMsg->dwSvcType != itrSrv->nModuleType)          ///< FSvcDC模块没有进行字节序转换
                        {
                            continue;
                        }

                        // 消息列表不存在
                        if (itrSrv->pListCtrl == NULL)
                        {
                            OutputDebugString(_T("pListCtrl is NULL!\n"));
                            break;
                        }

                        int nItem = itrSrv->pListCtrl->GetItemCount();

                        if (MT_SERVICE_DC == itrSrv->nModuleType)
                        {
                            pBASIC_MESSAGE_EX pMsg2Para = (pBASIC_MESSAGE_EX)dummyMsg.lpMsg;
                            itrSrv->pListCtrl->InsertItem(nItem, fcU2T(pMsg2Para->chContent));
                        }
                        else if (MSG_TYPE_NOTIFY == HPR_Ntohl(pBaseMsg->dwMsgType) && MT_SERVICE_DC != itrSrv->nModuleType)
                        {
                            pNOTIFY_MSG pMsgNotify = (pNOTIFY_MSG)dummyMsg.lpMsg;
                            itrSrv->pListCtrl->InsertItem(nItem, fcU2T(pMsgNotify->szNotifyInfo));
                        }
                        else if (MSG_TYPE_ALARM == HPR_Ntohl(pBaseMsg->dwMsgType))
                        {
                            pALARM_MSG pMsgAlarm = (pALARM_MSG)dummyMsg.lpMsg;
                            if(ALARM_NORMAL != pMsgAlarm->dwStatus)
                            {
                                itrSrv->pListCtrl->InsertItem(nItem, fcU2T(pMsgAlarm->szAlarmInfo));
                            }

                            if(SYS_INFO == pMsgAlarm->dwSvcType)
                            {
                                //现在只有中心不在线会走这里。
                                if(NULL != m_pProcessorCol)
                                {
                                    m_pProcessorCol->AddAlarmMsg(pMsgAlarm);
                                }
                            }
                        }

                        //显示的消息行数超过MsgMaxCount，删除最早的min{50, MsgMaxCount/10}条
                        if (nItem > theApp.m_nMsgMaxCount)
                        {
                            int nDelNums = theApp.m_nMsgMaxCount/10;
                            if (nDelNums > 50)
                            {
                                nDelNums = 50;
                            }

                            for (int i = 0; i < nDelNums; i++)
                            {
                                itrSrv->pListCtrl->DeleteItem(0);  
                            }

                            itrSrv->pListCtrl->UpdateData(TRUE);
                            nItem = itrSrv->pListCtrl->GetItemCount();
                        }
                        itrSrv->pListCtrl->EnsureVisible(nItem, FALSE);
                        break;      //跳出for循环
                    }/*end of for(...)*/

                    ::free(dummyMsg.lpMsg);
                    dummyMsg.lpMsg = NULL;
                }
            }
            break;
        default:
            break;
    }/*end of switch*/

    CDialog::OnTimer(nIDEvent);
}

/** @fn       BOOL CViewDlg::InitTabControl(void)
 *  @brief    初始化m_viewTab
 *  @param    void
 *  @return:  BOOL：TRUE:成功 ; FALSE:失败
 */
BOOL CViewDlg::InitTabControl(void)
{
    CRect iviewDlg;
    GetClientRect(&iviewDlg);
    iviewDlg.bottom += 33;
    m_viewTab.MoveWindow(&iviewDlg);

    int tabIdx = 0;

    {// 系统消息
        SERVICE_INFO SyssvcInfo;
        SyssvcInfo.nModuleType = SYS_INFO;
        SyssvcInfo.strSrvName = "服务管理中心";
        m_listServiceInfo.push_front(SyssvcInfo);
    }

    for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
        itrSrv != m_listServiceInfo.end(); itrSrv++)
    {
        if (itrSrv->pListCtrl != NULL)
        {
            itrSrv->nTabIdx = tabIdx;
            m_viewTab.InsertItem(tabIdx++, itrSrv->strSrvName.c_str());
            continue;
        }

        itrSrv->pListCtrl = new CSkinListCtrl2;  //CListCtrl;
        itrSrv->nTabIdx = tabIdx;
        m_viewTab.InsertItem(tabIdx++, itrSrv->strSrvName.c_str());
    }

    m_viewTab.ShowWindow(SW_SHOW);

    return TRUE;
}

/** @fn       BOOL CViewDlg::InitListControl(void)
 *  @brief    初始化m_viewList
 *  @param    void
 *  @return:  BOOL：TRUE:成功 ; FALSE:失败
 */
BOOL CViewDlg::InitListControl(void)
{
    // 获取列宽信息
    GetColWidthInfo();

    CRect ctTable;
    m_viewTab.GetClientRect(&ctTable);

    TCHAR szMessageBuff[MAX_PATH] = {0};
    //TCHAR szResultBuff[MAX_PATH] = {0};

    MultiLanRes_LoadString(IDS_MsgInfo, szMessageBuff, _countof(szMessageBuff));
    //MultiLanRes_LoadString(IDS_MsgResult, szResultBuff, _countof(szResultBuff));

    //risk...
    TCHAR titles[2][32] = {0};

    _tcscpy_s(titles[0], szMessageBuff);
    //_tcscpy_s(titles[1], szResultBuff);

    m_listWidth = ctTable.Width()/*iviewDlg.Width()*/;

    int colWidth[2] = { m_listWidth - 30/*170 */, 0/*132*/ };

    int nItemHeight = 24;

//     m_viewImageList.Create(2, nItemHeight, ILC_COLOR32 | ILC_MASK, 2, 1);
//     m_viewImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_IMG));
//     m_viewImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

    m_viewImageList.Create(1, nItemHeight, ILC_COLOR32|ILC_MASK, 1, 1);
    CBitmap cBit;   // 析构删除资源
    cBit.LoadBitmap(IDB_BITMAP_HEADER);
    m_viewImageList.Add(&cBit, RGB(255, 255, 255));
    
    int icolumnNum = 1;
 
    for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
        itrSrv != m_listServiceInfo.end(); itrSrv++)
    {
        if (itrSrv->pListCtrl != NULL)
        {
            itrSrv->pListCtrl->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS,// | WS_BORDER
                CRect(0, 0, 100, 100), FromHandle(m_viewTab.m_hWnd), USER_LIST_ID_BASE + itrSrv->nTabIdx);  //FromHandle(m_viewTab.m_hWnd)// | LVS_OWNERDRAWFIXED

            //setImageList
            itrSrv->pListCtrl->SetImageList(&m_viewImageList, LVSIL_SMALL);
            

//              //setStyle
//             ListView_SetExtendedListViewStyle(itrSrv->pListCtrl->m_hWnd, itrSrv->pListCtrl->GetStyle() | LVS_EX_GRIDLINES);
//             itrSrv->pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);

            // InsertColumn
            for(int i = 0; i < icolumnNum; i++)
            {
                int nColWidth = GetColWidth(itrSrv->nModuleType, i);
                if (nColWidth <= 0)
                {
                    nColWidth = colWidth[i];
                }
                itrSrv->pListCtrl->InsertColumn(i, titles[i], LVCFMT_CENTER, nColWidth);
            }

            //setStyle
            itrSrv->pListCtrl->SetHeaderStyle();
            itrSrv->pListCtrl->SetItemHeight(nItemHeight);

            // moveWindow
            itrSrv->pListCtrl->MoveWindow(CRect(0, 27, ctTable.Width()/*iviewDlg.Width()*/ - 3, ctTable.Height() - 3));

            // showhide
            itrSrv->pListCtrl->ShowWindow(SW_HIDE);
        } /*end of if (itrSrv->second.pListCtrl != NULL)*/
    }/*end of for*/

    // 把第一个tab的list ShowWindow
    std::list<SERVICE_INFO>::iterator itrSrvFrist = m_listServiceInfo.begin();
    if (itrSrvFrist != m_listServiceInfo.end() )
    {
        m_viewTab.SetCurSel(itrSrvFrist->nTabIdx);

        if (itrSrvFrist->pListCtrl != NULL)
        {
            itrSrvFrist->pListCtrl->ShowWindow(SW_SHOW);
        }
    }

    return TRUE;
}

/** @fn       BOOL CViewDlg::InitSocketServer(void)
 *  @brief    初始化m_socketServer
 *  @param    void
 *  @return:  BOOL：TRUE:成功 ; FALSE:失败
 */
BOOL CViewDlg::InitSocketServer(void)
{
//     //加载套接字库
//     WORD wVersionRequested;
//     WSADATA wsaData;
// 
//     int iRet;
//     wVersionRequested = MAKEWORD(1, 1);
//     iRet = WSAStartup( wVersionRequested, &wsaData);
//     if (iRet != NO_ERROR)
//     {
//         CString errMsg;
//         errMsg.Format(_T("WSAStartup error:%d\n"), iRet);
//         OutputDebugString(errMsg);
//         return FALSE;
//     }

    int nRecvSocketPort;
    if (FALSE == XML_GetRecvSocketPort(nRecvSocketPort))
    {
        VIEWMSGLOG_ERROR(_T("XML_GetRecvSocketPort failed."));
        return FALSE;
    }

    {// 日志
        VIEWMSGLOG_INFO(_T("XML_GetRecvSocketPort return: %d."), nRecvSocketPort);
    }

    m_socketServer = socket(AF_INET, SOCK_DGRAM, 0);

    if (INVALID_SOCKET == m_socketServer)
    {
        OutputDebugString(_T("server socket create failed\n"));
        return FALSE;
    }

    m_addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    m_addrServer.sin_family = AF_INET;
    m_addrServer.sin_port = htons((u_short)nRecvSocketPort);

    //bind
    int iRet = bind(m_socketServer, (SOCKADDR*) &m_addrServer, sizeof(SOCKADDR) );

    if (SOCKET_ERROR == iRet)
    {
        OutputDebugString(_T("server socket bind failed\n"));

        //关闭socket
        closesocket(m_socketServer);
        m_socketServer = INVALID_SOCKET;
        return FALSE;
    }

    {// 在系统消息中显示
        CString csTmp;
        csTmp.Format(_T("初始化接收theService消息的Socket成功, 端口号:%d"), nRecvSocketPort);
        CreateShowMsg(csTmp, _T(""));
    }
    return TRUE;
}

/** @fn       DWORD WINAPI CViewDlg::RecvThreadFun(LPVOID lpParam)
 *  @brief    接收信息的线程函数
 *  @param
 *  @return:  DWORD WINAPI
 */
DWORD WINAPI CViewDlg::RecvThreadFun(LPVOID lpParam)
{
    //判断lpParam是否可以强制转换成CViewDlg
    //
    CViewDlg * pViewDlg = static_cast<CViewDlg *> (lpParam);
    if (NULL == pViewDlg)
    {
        return DWORD(1);
    }

    if (pViewDlg->m_socketServer != INVALID_SOCKET)
    {
        int len = sizeof(SOCKADDR);
        int recvLen = -1;
        int bufLen = 2048;

        char * msgBuffer = new(nothrow) char[bufLen];

        if (NULL == msgBuffer)
        {
            CString errMsg;
            errMsg.Format("RecvThreadFun msgBuffer new failed \n");
            TRACE(errMsg);
            return ((DWORD)1);
        }

        while(FALSE == pViewDlg->m_bExitFlag) //m_bExitFlag
        {
            //
            recvLen = recvfrom(pViewDlg->m_socketServer, msgBuffer, bufLen, 0, (SOCKADDR *) &pViewDlg->m_addrClient, &len);
            if (recvLen <= 0)
            {
                CString errMsg;
                errMsg.Format(_T("recvfrom error. RecvLength:%d\n"), recvLen);
                TRACE(errMsg);
            }
            else
            {
                dummy_msg_t dummyMsg = {0};
                dummyMsg.cbMsgSize = recvLen;
                dummyMsg.lpMsg = ::malloc(recvLen);
                if (NULL == dummyMsg.lpMsg)
                {
                    ::OutputDebugString(_T("Out of memory\n"));
                    VIEWMSGLOG_INFO("new operation failed!");
                    delete [] msgBuffer;
                    return ((DWORD)1);
                }

                ::memcpy(dummyMsg.lpMsg, msgBuffer, recvLen);

                pViewDlg->m_msgQueue.Push(dummyMsg);

                //else if(MESSAGE_TYPE_SVC_CTRL == baseMsg->dwMsgType)
                //{
                //    BASIC_MESSAGE_EX * baseMsgEx = (BASIC_MESSAGE_EX *) msgBuffer;
                //    // 消息处理
                //    RECV_CMD_MSG struRecvCmdMsg;
                //    if (SVC_CTRL_STOP_SERVICE == baseMsgEx->dwSvcType)
                //    {
                //        struRecvCmdMsg.nCtrlType = cms_8100::SERVER_STOP;
                //    }
                //    else
                //    {
                //        struRecvCmdMsg.nCtrlType = cms_8100::SERVER_START;
                //    }

                //    struRecvCmdMsg.nModuleType = 0;
                //    struRecvCmdMsg.svcName = baseMsgEx->chContent;

                //    // 将控制信息添加到控制信息队列中
                //    BOOL bRet = theApp.m_ctrlMsgQueue.Append(struRecvCmdMsg);
                //    if (FALSE == bRet)
                //    {
                //        SVCMANAGERLOG_ERROR(_T("CViewDlg::RecvThreadFun theApp.m_ctrlMsgQueue.Append(struRecvCmdMsg) return FALSE."));
                //    }               
                //}
            }
        }

        delete [] msgBuffer;
    }

    return DWORD(0);
}


/** @fn       BOOL CViewDlg::InitRecvThread(void)
 *  @brief    初始化m_handleRecvThread
 *  @param    void
 *  @return:  BOOL：TRUE:成功 ; FALSE:失败
 */
BOOL CViewDlg::InitRecvThread(void)
{
    m_handleRecvThread = CreateThread(NULL, 0, RecvThreadFun, (LPVOID)this, 0, NULL);

    if (NULL == m_handleRecvThread)
    {
        OutputDebugString(_T("create thread failed\n"));
        return FALSE;
    }
    return TRUE;
}

/** @fn       void CViewDlg::OnTcnSelchangeTabViewcontrol(NMHDR *pNMHDR, LRESULT *pResult)
 *  @brief    tab选择的响应
 *  @param
 *  @return:  void
 */
void CViewDlg::OnTcnSelchangeTabViewcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
    int nClickIdx = m_viewTab.GetCurSel();

    for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
        itrSrv != m_listServiceInfo.end(); itrSrv++)
    {
        if (itrSrv->nTabIdx == nClickIdx)
        {   //显示选中的tab对应的ListCtrl
            if (itrSrv->pListCtrl != NULL)
            {
                itrSrv->pListCtrl->ShowWindow(SW_SHOW);
            }
            else 
            {
                MessageBox("ListCtrl NULL");
            }/*end of if (itrSrv->second.pListCtrl != NULL) else*/
        }/*end of if (itrSrv->second.nTabIdx == nClickIdx)*/
        else  //隐藏其他tab对应的ListCtrl
        {
            if (itrSrv->pListCtrl != NULL)
            {
                itrSrv->pListCtrl->ShowWindow(SW_HIDE);
            }
            else 
            {
                MessageBox("ListCtrl NULL");
            }/*end of if (itrSrv->second.pListCtrl != NULL) else*/
        }
    }

    *pResult = 0;
}

/** @fn       BOOL CViewDlg::CreateShowMsg(const CString &csMsg, const CString &csMsgResult)
 *  @brief    显示消息
 *  @param    (const CString &) csMsg [IN] : 消息内容
 *            (const CString &) csMsgResult [IN] : 消息结果
 *            (int) nSvcType [IN] : 服务类型
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CViewDlg::CreateShowMsg(const CString &csMsg, const CString &csMsgResult)
{
    return FALSE;

    //if (strcmp(csMsg, _T("")) == 0)
    //   {
    //       return FALSE;
    //   }

    //   CTime ctCurTime = CTime::GetCurrentTime();
    //   CString csTmp = ctCurTime.Format(_T("%Y%m%d %H:%M:%S--"));
    //   // 添加时间
    //   csTmp = csTmp + csMsg;

    //   NOTIFY_MSG MsgElem;
    //   MsgElem.dwSvcType = SYS_INFO;
    //   MsgElem.dwMsgType = MSG_TYPE_NOTIFY;

    //   /* 消息长度 */
    //   int nContentLength = csTmp.GetLength() + 1;
    //   /* 复制消息 */
    //   if (nContentLength > 0)
    //   {
    //       strcpy_s(MsgElem.szNotifyInfo, nContentLength, (LPSTR)(LPCTSTR)csTmp);
    //   }

    //dummy_msg_t dummyMsg = {0};
    //dummyMsg.cbMsgSize = sizeof(NOTIFY_MSG) + nContentLength;
    //dummyMsg.lpMsg = ::malloc(dummyMsg.cbMsgSize);
    //if (NULL == dummyMsg.lpMsg)
    //{
    //	::OutputDebugString(_T("Out of memory\n"));
    //	return FALSE;
    //}

    //::memcpy(dummyMsg.lpMsg, (LPSTR)(LPCTSTR)csTmp, dummyMsg.cbMsgSize);

    //m_msgQueue.Push(dummyMsg);

    //   return TRUE;
}

/** @fn       BOOL CViewDlg::ShowInstantMsg(int nModuleType)
 *  @brief    根据ModuleType显示服务消息
 *  @param    (int) nModuleType [IN] : 服务的模块类型
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CViewDlg::ShowInstantMsg(int nModuleType)
{
    BOOL bRet = FALSE;
    for (std::list<SERVICE_INFO>::iterator itor = m_listServiceInfo.begin();
        itor != m_listServiceInfo.end(); itor++)
    {
        if (nModuleType != (int)itor->nModuleType)
        {
            if (itor->pListCtrl != NULL)
            {
                itor->pListCtrl->ShowWindow(SW_HIDE);
            }
            continue;
        }

        /* 显示ModuleType对应的即时消息和Tab */
        if ( (itor->pListCtrl != NULL) && (itor->nTabIdx >= 0) )
        {
            m_viewTab.SetCurSel(itor->nTabIdx);
            itor->pListCtrl->ShowWindow(SW_SHOW);
            bRet = TRUE;
        }
    }

    return bRet;
}

/** @fn       BOOL CViewDlg::ShowInstantMsg(const CString& svcName)
 *  @brief    根据服务名显示服务消息
 *  @param    (const CString&) svcName [IN] : 服务名
 *  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
 */
BOOL CViewDlg::ShowInstantMsg(const CString &svcName)
{
    BOOL bRet = FALSE;

    for (std::list<SERVICE_INFO>::iterator itor = m_listServiceInfo.begin();
        itor != m_listServiceInfo.end(); itor++)
    {
        if (strcmp(itor->strSrvName.c_str(), (LPCTSTR)svcName) != 0)
        {
            if (itor->pListCtrl != NULL)
            {
                itor->pListCtrl->ShowWindow(SW_HIDE);
            }
            continue;
        }

        /* 显示ModuleType对应的即时消息和Tab */
        if ( (itor->pListCtrl != NULL) && (itor->nTabIdx >= 0) )
        {
            m_viewTab.SetCurSel(itor->nTabIdx);
            itor->pListCtrl->ShowWindow(SW_SHOW);
            bRet = TRUE;
        }
    }

    return bRet;
}

/** @fn       int CViewDlg::GetColWidth(DWORD nModuleType, int nCol  = 0 )
 *  @brief    通过nModuleType获取记录的列宽
 *  @param    (DWORD) nModuleType [IN] : 服务的模块类型
 *            (int) nCol [IN] : 列标（从0开始）
 *  @return:  int：获取到的列宽
 */
int CViewDlg::GetColWidth(DWORD nModuleType, int nCol /* = 0 */)
{
    if ( (nCol < 0 ) || (nCol > (MAX_COL_NUM-1))
        || (m_listColWidthInfo.size() <= 0))
    {
        return 0;
    }

    /* 获取列宽 */
    for (std::list<COL_WIDTH_INFO>::iterator itor = m_listColWidthInfo.begin(); itor != m_listColWidthInfo.end(); itor++)
    {
        if (nModuleType == itor->nModuleType)
        {
            return itor->szColWidth[nCol];
        }
    }

    return 0;
}

/** @fn       void CViewDlg::GetColWidthInfo(void)
 *  @brief    获取即时消息列表的列宽信息
 *  @param    void
 *  @return:  void
 */
void CViewDlg::GetColWidthInfo(void)
{
    m_listColWidthInfo.clear();
    CMarkup xmlInfo;

    xmlInfo.SetDoc(CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + COL_WIDTH_INFO_FILE));

    if (xmlInfo.FindElem(XML_LIST_COL_INFO) && xmlInfo.IntoElem())
    {

        while (xmlInfo.FindElem(XML_COL_INFO))
        {
            COL_WIDTH_INFO colWithInfo;
            colWithInfo.nModuleType = atoi(xmlInfo.GetAttrib(XML_MODULE_TYPE));
            colWithInfo.szColWidth[0] = atoi(xmlInfo.GetAttrib(XML_COL_ONE));
            colWithInfo.szColWidth[1] = atoi(xmlInfo.GetAttrib(XML_COL_TWO));

            m_listColWidthInfo.push_back(colWithInfo);
        }
    }
}

/** @fn       void CViewDlg::SaveColWidthInfo(void)
 *  @brief    保存列宽信息
 *  @param    void
 *  @return:  void
 */
void CViewDlg::SaveColWidthInfo(void)
{
    CMarkup xmlInfo;
    xmlInfo.SetDoc(XML_HEAD);

    xmlInfo.AddElem(XML_LIST_COL_INFO);
    xmlInfo.IntoElem();

    /* 现有 */
    for (std::list<SERVICE_INFO>::iterator itrSrv = m_listServiceInfo.begin();
        itrSrv != m_listServiceInfo.end(); itrSrv++)
    {
        if (NULL == itrSrv->pListCtrl)
        {
            continue;
        }

        for (std::list<COL_WIDTH_INFO>::iterator itorCol = m_listColWidthInfo.begin(); itorCol != m_listColWidthInfo.end();)
        {
            if (itrSrv->nModuleType == itorCol->nModuleType)
            {
                itorCol = m_listColWidthInfo.erase(itorCol);
            }
            else
            {
                itorCol++;
            }
        }

        xmlInfo.AddElem(XML_COL_INFO);
        xmlInfo.AddAttrib(XML_MODULE_TYPE, itrSrv->nModuleType);
        xmlInfo.AddAttrib(XML_COL_ONE, itrSrv->pListCtrl->GetColumnWidth(0));
//         xmlInfo.AddAttrib(XML_COL_TWO, itrSrv->pListCtrl->GetColumnWidth(1));
//         xmlInfo.AddAttrib(XML_COL_TWO, itrSrv->pListCtrl->GetColumnWidth(2));
    }

    /* 历史记录 */
    for (std::list<COL_WIDTH_INFO>::iterator itorCol = m_listColWidthInfo.begin(); itorCol != m_listColWidthInfo.end(); itorCol++)
    {
        xmlInfo.AddElem(XML_COL_INFO);
        xmlInfo.AddAttrib(XML_MODULE_TYPE, itorCol->nModuleType);
        xmlInfo.AddAttrib(XML_COL_ONE, itorCol->szColWidth[0]);
    }

    CString csFilePath = CFolder::GetExecutionRoot() + COL_WIDTH_INFO_FILE;
    CFolder::SaveToUTF8File(xmlInfo.GetDoc(), csFilePath);
}

////回调函数声明和定义
//HPR_VOID CALLBACK CViewDlg::s_SnmpManagerMsgFunc(PPDUINFO ppdu, HPR_VOIDPTR context)
//{
//    CViewDlg *_this = (CViewDlg *)context;
//    if (_this != NULL)
//    {
//        _this->SnmpManagerMsgFunc(ppdu);
//    }
//}

//HPR_VOID CViewDlg::SnmpManagerMsgFunc(PPDUINFO ppdu)
//{
//    /* 屏蔽消息 */
//    return;
///*
//    //int ret = HPR_OK;
//    switch (ppdu->type)
//    {
//    case sNMP_PDU_RESPONSE:
//        //ret = ProcManagerGet(ppdu);
//        break;
//    case sNMP_PDU_TRAP:
//       // ret = ProcManagerTrap(ppdu);
//       break;
//    case sNMP_PDU_SET:
//    case sNMP_PDU_V1TRAP:
//    case sNMP_PDU_GETBULK:
//    case sNMP_PDU_INFORM:
//    case sNMP_PDU_REPORT:
//    case sNMP_PDU_GET:
//    case sNMP_PDU_GETNEXT:
//    default:
//        break;
//    }
//*/
//}

//int CViewDlg::ProcManagerTrap(PPDUINFO ppdu)
//{  
//    VIEWMSGLOG_INFO("receive trap mssage");
//
//    int  ret              = HPR_OK;
//    int  level            = 0;
//    int  svrtype          = 0;
//    char trapinfo[2048]   = {0};
//    PVBVALUE pvalue       = NULL;
//    bool is3rddev      = false;
//    string index = "";
//    string bntrapinfo;
//
//    if ((ppdu->vbcount <= 0) || (ppdu->objvaluearray == NULL))
//    {
//        VIEWMSGLOG_ERROR("process trap failed, array is empty");
//        return HPR_ERROR;
//    }
//
//    for (int i = 0; i < ppdu->vbcount; i++)
//    {
//        pvalue = (PVBVALUE)(&(ppdu->objvaluearray[i].value));
//        if (HPR_Strcasecmp(ppdu->stroidarray[i], hikTrapLevel) == 0)
//        {
//            level = pvalue->uNumber;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], hikServerType) == 0)
//        {
//            // 10:vrm
//            svrtype = pvalue->uNumber;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], hikServerIndexCode) == 0)
//        {
//            index= (char*)pvalue->string.ptr;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], hikTrapContent) == 0)
//        {
//            strncpy(trapinfo, (char*)pvalue->string.ptr, pvalue->string.len);
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], GetBnOidString(hikTrapRaidState)) == 0)
//        {
//            bntrapinfo += GetTrapdesc(hikTrapRaidState);
//            is3rddev = true;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], GetBnOidString(hikTrapChannelState)) == 0)
//        {
//            bntrapinfo += GetTrapdesc(hikTrapChannelState);
//            is3rddev = true;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], GetBnOidString(hikTrapCPUAlarm)) == 0)
//        {
//            bntrapinfo += GetTrapdesc(hikTrapCPUAlarm);
//            is3rddev = true;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], GetBnOidString(hikTrapFanStatus)) == 0)
//        {
//            bntrapinfo += GetTrapdesc(hikTrapFanStatus);
//            is3rddev = true;
//        }
//        else if (HPR_Strcasecmp(ppdu->stroidarray[i], GetBnOidString(hikTrapPowerStatus)) == 0)
//        {
//            bntrapinfo += GetTrapdesc(hikTrapPowerStatus);
//           is3rddev = true;
//        }
//    }
//
//    HPR_ADDR_T  remoteaddr;
//    SnmpManager_GetRemoteAddr(&remoteaddr);
//
//    int srvTypeV3 = SYS_INFO;
//    std::map<int, int>::iterator itr = m_mapSrvType.find(svrtype);
//    if (m_mapSrvType.end() == itr)
//    {
//        srvTypeV3 = SYS_INFO;
//    }
//    else
//    {
//        srvTypeV3 = m_mapSrvType[svrtype];
//    }
//    BASIC_MESSAGE_EX MsgElem;
//    MsgElem.dwSvcType = srvTypeV3;
//    MsgElem.dwMsgType = MESSAGE_TYPE_2PARAM;
//    strcpy_s(MsgElem.chContent, SVCMSG_CONTENT_LEN, trapinfo);
//    m_msgQueue.Append(&MsgElem);
//
//    CString csTmp;
//    csTmp.Format(_T("level:%d, is3rddev:%d\n"), level, is3rddev);
//    OutputDebugString(csTmp);
//
//    return ret;
//}


void CViewDlg::InitSrvType()
{
    char szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    (_tcsrchr(szPath, _T('\\')))[1] = 0;

    CString strXml = CString(szPath) + "dev.xml";
    CMarkup xml;
    xml.Load(strXml);

    xml.FindElem("content");
    xml.IntoElem();

    while (xml.FindElem("srv"))
    {
        int srvType = atoi(xml.GetAttrib("svrtype"));
        int srvTypeV3 = atoi(xml.GetAttrib("srvtypev3"));

        m_mapSrvType.insert(make_pair(srvType, srvTypeV3));
    }
}

