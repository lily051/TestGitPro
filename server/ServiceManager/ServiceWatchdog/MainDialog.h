/**
*   @file MainDialog.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*   
*   @author 
*   @date   
*
*   @note
*   @history：add 即时消息显示对话框
*             date 2012/08/14
*   @warning
*/

#pragma once


#include "UserMsgDef.h"
#include "ServiceController.h"
#include "skinbutton.h"
#include "appstat.h"
#include "SkinDialog.h"
#include "SkinListCtrl.h"
#include "ViewDlg.h"
#include "SvcCfg.h"

#include "./Utilities/AppLogo.h"

#include <list>
//#include "afxwin.h"
using namespace std;

// CMainDialog dialog
class CMainDialog : public CSkinDialog
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAIN };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    //afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();
    afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

    /* 屏蔽帮助文档弹出 */
    virtual void WinHelp(DWORD_PTR dwData, UINT nCmd /* = HELP_CONTEXT */);
    
    /* 屏蔽F1、ESC和Enter键 */
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    /* WM_SERVICE_CONTROL_COMMAND_ERROR 消息响应 */
    afx_msg LRESULT OnServiceControlCommandError(WPARAM wParam, LPARAM lParam);
    /* WM_SERVICE_STATUS_CHANGED 消息响应 */
    afx_msg LRESULT OnServiceStatusChanged(WPARAM wParam, LPARAM lParam);
    /* WM_TASKBARCREATED 消息响应 */
    afx_msg LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
    /* WM_ICON_NOTIFY 消息响应 */
    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);

    /* 其他程序发来的退出消息响应 */
    afx_msg LRESULT OnUserClose(WPARAM wParam, LPARAM lParam);

    /* 设置 Service List 样式 */
    void SetListControlServicesHeader(void);
    
    /* 添加系统托盘图标 */
    void AddNotifyIcon(void);
    /* 删除系统托盘图标 */
    void RemoveNotifyIcon(void);

    /* 隐藏主窗口 */
    void HideMainWindow(void);
    /* 显示主窗口 */
    void ShowMainWindow(void);

    /* 初始化m_lstServiceController */
    BOOL InitServiceController(void);

public:
    /* 最小化按钮响应 */
    afx_msg void OnBnClickedMinimize();
    /* 退出响应 */
    afx_msg void OnBnClickedExit();
    /* 停止所有服务响应 */
    afx_msg void OnBnClickedButtonStopAllService();
    /* 重启所有服务响应 */
    afx_msg void OnBnClickedButtonRestartAllService();
    /* 显示窗口 */
    afx_msg void OnIconnotifyShowWindow();
    /* 隐藏窗口 */
    afx_msg void OnIconnotifyHideWindow();
    /* 退出 */
    afx_msg void OnIconnotifyExit();
    /* 更新托盘菜单中的显示窗口菜单项状态 */
    afx_msg void OnUpdateIconnotifyShowWindow(CCmdUI *pCmdUI);
    /* 更新托盘菜单中的隐藏窗口菜单项状态 */
    afx_msg void OnUpdateIconnotifyHideWindow(CCmdUI *pCmdUI);
    /* 更新托盘菜单中的退出菜单项状态 */
    afx_msg void OnUpdateIconnotifyExit(CCmdUI *pCmdUI);
    /* 初始化菜单的弹出 */
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

    /* 即时消息按钮响应 */
    afx_msg void OnBnClickedButtonInstantShowInfo(void);
    /* 服务列表按钮响应 */
    afx_msg void OnBnClickedButtonServiceList(void);
    /* 服务配置按钮响应 */
    afx_msg void OnBnClickedButtonConfig(void);

    /* 服务器列表中选中项的右击响应 */
    afx_msg void OnNMRClickListService(NMHDR *pNMHDR, LRESULT *pResult);

protected:
    std::list<ServiceController*> m_lstServiceController;    ///< 从配置文件中读取的 ServiceController 列表
    bool m_bWindowShowing;      ///< 跟踪主窗口显示/隐藏状态
    NOTIFYICONDATA m_nid;       ///< 系统托盘图标
    CImageList m_listImageList; ///< 列表图片
    HICON m_hIcon;              ///< 托盘图片

public:
    CSkinButton m_buttonMin;    ///< 最小化按钮
    CSkinButton m_buttonExit;   ///< 退出按钮
    CAppStat m_appStat;         ///< 状态

    CSkinListCtrl m_listService;    ///< 服务列表

    CSkinButton m_buttonViewIstantMessage;  ///< 即时消息按钮
    CSkinButton m_buttonServiceList;        ///< 服务列表按钮
    CSkinButton m_btnCfg;                   ///< 服务配置按钮

    CViewDlg m_viewDlg;                     ///< 即时消息界面
    CSvcCfg m_svcCfgDlg;                    ///< 服务配置界面

    DLG_FLAGS m_nDlgFlag;                   ///< 界面显示标志位：0:服务列表 ; 1:即时消息 ; 2:服务配置

    //HPP_HANDLE m_hppDCClientHandle;         ///< 与数据中心服务连接的HPP协议栈

    //HPP_HANDLE m_hppLocalHandle;            ///< 本地监听服务的HPP协议栈
    //HPP_SERVER_HANDLE m_hppLocalSerHanlde;  ///< 本地监听服务

    CRITICAL_SECTION m_hppDCClientHandleLock;   ///< 与数据中心服务连接的Lock
    CRITICAL_SECTION m_hppLocalSevHanldeLock;   ///< 本地HPP服务的Lock

private:
     CAppLogo m_appLogo;         ///< LOGO
     BOOL m_bIsRedBlackSkin;     ///< 皮肤选择标志

     HANDLE m_hEventGetInfo;     ///< 获取信息的控制事件
     HANDLE m_hThreadGetInfo;    ///< 获取信息的线程句柄

     FILETIME m_preIdleTime;     ///< 前一次的空闲时间
     FILETIME m_preKernelTime;   ///< 前一次的内核时间
     FILETIME m_preUserTime;     ///< 前一次的用户时间
     FILETIME m_curIdleTime;     ///< 当前的空闲时间
     FILETIME m_curKernelTime;   ///< 当前的内核时间
     FILETIME m_curUserTime;     ///< 当前的用户时间

     HANDLE m_hCtrlMsgThread;    ///< 控制信息处理的线程句柄
     HANDLE m_hCtrlMsgEvent;     ///< 控制信息处理的控制事件  
public:
    /* 初始化button */
    BOOL InitButton(void);

    /* 加载看门狗的皮肤 */
    BOOL LoadWatchBitMap(BOOL bIsRedAndBlack);

    /* 初始化即时消息界面和服务配置界面 */
    BOOL InitAddDlg(void);

    ///* 重新初始化与数据中心服务之间的连接 */
    //BOOL ReconDcServer(void);
    ///* 重新初始化本地监听 */
    //BOOL ReinitLocalServer(void);

    /* 菜单中开始服务响应 */
    void MenuStartService(void);
    /* 菜单中停止服务响应 */
    void MenuStopService(void);
    /* 菜单中重启服务响应 */
    void MenuRestartService(void);
    /* 菜单中全部安装服务响应 */
    void MenuInstallALLService(void);
    /* 菜单中安装服务响应 */
    void MenuInstallService(void);
    /* 菜单中卸载服务响应 */
    void MenuUninstallService(void);
    /* 菜单中一键卸载响应 */
    void MenuUninstallAllService(void);
    /* 菜单中打开配置项响应 */
    void MenuOpenConfig(void);
    /* 打开运行目录 */
    void MenuOpenRunDir(void);
    /* 运行命令 */
    BOOL RunCmd(const CString& cmd, const CString& DefaultDir);
    /* 菜单中的即时消息响应 */
    void MenuInstantMsg(void);

    /* 显示控制信息 */
    void ShowCtrlMsg(const RECV_CMD_MSG& struRecvCmdMsg);
    /* 智能巡检的控制请求 */
    BOOL OnServiceWatchDogControl(const CString& svcName, HPR_INT32 controlType);
    /* 控制请求 */
    BOOL OnServiceWatchDogControl(HPR_INT32 moduleType, HPR_INT32 controlType);
    /* 重启机器控制请求 */
    BOOL OnServiceWatchDogControl(void);

    /* 创建系统显示消息 */
    BOOL CreateSysShowMsg(const CString &csMsg, const CString &csMsgResult);

    ///* 初始化获取信息的线程 */
    //BOOL InitGetThread(void);
    ///* 获取信息的线程 */
    //static DWORD WINAPI ThreadForGetInfo(LPVOID lParam);
    /* 关闭获取信息的线程 */
    void CloseGetThread(void);

    /* 初始化控制信息的处理线程 */
    BOOL InitCtrlMsgThread(void);
    /* 控制信息的处理线程 */
    static DWORD WINAPI ThreadForCtrlMsg(LPVOID lParam);
    /* 关闭控制信息的处理线程 */
    void CloseCtrlMsgThread(void);

    /* 初始化计算系统CPU的FILETIME */
    BOOL InitFileTime(void);

    /* 获取CPU */
    int GetCpu(void);

    /* 获取系统磁盘信息 */
    BOOL GetDiskInfo(std::vector<DISK_INFO>& diskInfo);

    /* 获取系统内存信息 */
    BOOL GetMemInfo(int& TotalMem, int& UsingMem);

    /* 获取处理器信息 */
    CString GetCpuInfo(void);

    /* 获取操作系统信息 */
    CString GetOSInfo(void);

    /* 获取主机名 */
    void GetHostName(CString& HostName, CString& SN);

    /* 获取开机时间 */
    CString GetSysStartTime(void);

    // 获取Service Manager的启动时间 */
    CString GetServiceManagerStartTime(void);

    /* 获取操作系统位数 */
    BOOL GetOSType(int& OsType);

    /* 获取网卡信息 */
    BOOL GetNetWorkInfo(std::vector<NET_WORK_INFO>& vecNetWorkInfo);

    ///* 使用HPP发送信息到CMS */
    //BOOL SendInfoToCMS(const SERVER_SYS_INFO& SerSysInfo, const std::vector<SERVER_PROCESS_INFO>& SerProcInfo);

    ///* 发送校时请求 */
    //BOOL SendReqServerData(void);

    /* 重启电脑 */
    BOOL RestartComputer(void);

    ///* 显示DC和本地的IP和PORT */
    //void ShowDCAndLocal(void);

    /* 判断是否是所有服务都不可用 */
    BOOL IsAllServiceNotExist(void);
    /* 判断是否是所有服务都可用 */
    BOOL IsAllServiceExist(void);

protected:

    /** @fn       char* CMainDialog::FormatServiceStatus(DWORD svcStatus)
     *  @brief    格式化 Service Status 为字符串
     *  @param    (DWORD) svcStatus [IN] : Service Status
     *  @return:  char* : 与 svcStatus 对应的字符串
     */
    inline char* FormatServiceStatus(DWORD svcStatus)
    {
        static TCHAR szTextBuff[MAX_PATH] = {0};

        switch (svcStatus)
        {
        case SERVICE_STOPPED:
            MultiLanRes_LoadString(IDS_STOPPED, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_START_PENDING:
            MultiLanRes_LoadString(IDS_STARTING, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_STOP_PENDING:
            MultiLanRes_LoadString(IDS_STOPPING, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_RUNNING:
            MultiLanRes_LoadString(IDS_RUNNING, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_CONTINUE_PENDING:
            MultiLanRes_LoadString(IDS_RESUMING, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_PAUSE_PENDING:
            MultiLanRes_LoadString(IDS_PAUSING, szTextBuff, _countof(szTextBuff));
            break;
        case SERVICE_PAUSED:
            MultiLanRes_LoadString(IDS_PAUSED, szTextBuff, _countof(szTextBuff));
            break;
        case CONTROL_COMMAND_ERROR_SERVICE_DOES_NOT_EXIST:
            MultiLanRes_LoadString(IDS_UNUSABLE, szTextBuff, _countof(szTextBuff));
            break;
        default:
            MultiLanRes_LoadString(IDS_UNKNOWN, szTextBuff, _countof(szTextBuff));
            break;
        }

        return szTextBuff;
    }

    /** @fn       int CMainDialog::GetListIconIndexBySvcStatus(DWORD svcStatus)
     *  @brief    根据 Service Status 获取 ListCtrl 图标
     *  @param    (DWORD) svcStatus [IN] : Service Status
     *  @return:  int：ListCtrl图标索引
     */
    inline int GetListIconIndexBySvcStatus(DWORD svcStatus)
    {
        switch (svcStatus)
        {
        case SERVICE_STOPPED:
        case SERVICE_STOP_PENDING:
        case SERVICE_PAUSED:
        case SERVICE_PAUSE_PENDING:
            {
                return 1;
            }
        case SERVICE_RUNNING:
        case SERVICE_START_PENDING:
        case SERVICE_CONTINUE_PENDING:
            {
                return 0;
            }
        default:
            {
                return 1;
            }
        }
    }

};
