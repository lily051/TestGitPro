/**
*   @file MainDialog.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*   
*   @author 
*   @date   
*
*   @note
*   @history��add ��ʱ��Ϣ��ʾ�Ի���
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

    /* ���ΰ����ĵ����� */
    virtual void WinHelp(DWORD_PTR dwData, UINT nCmd /* = HELP_CONTEXT */);
    
    /* ����F1��ESC��Enter�� */
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    /* WM_SERVICE_CONTROL_COMMAND_ERROR ��Ϣ��Ӧ */
    afx_msg LRESULT OnServiceControlCommandError(WPARAM wParam, LPARAM lParam);
    /* WM_SERVICE_STATUS_CHANGED ��Ϣ��Ӧ */
    afx_msg LRESULT OnServiceStatusChanged(WPARAM wParam, LPARAM lParam);
    /* WM_TASKBARCREATED ��Ϣ��Ӧ */
    afx_msg LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
    /* WM_ICON_NOTIFY ��Ϣ��Ӧ */
    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);

    /* �������������˳���Ϣ��Ӧ */
    afx_msg LRESULT OnUserClose(WPARAM wParam, LPARAM lParam);

    /* ���� Service List ��ʽ */
    void SetListControlServicesHeader(void);
    
    /* ���ϵͳ����ͼ�� */
    void AddNotifyIcon(void);
    /* ɾ��ϵͳ����ͼ�� */
    void RemoveNotifyIcon(void);

    /* ���������� */
    void HideMainWindow(void);
    /* ��ʾ������ */
    void ShowMainWindow(void);

    /* ��ʼ��m_lstServiceController */
    BOOL InitServiceController(void);

public:
    /* ��С����ť��Ӧ */
    afx_msg void OnBnClickedMinimize();
    /* �˳���Ӧ */
    afx_msg void OnBnClickedExit();
    /* ֹͣ���з�����Ӧ */
    afx_msg void OnBnClickedButtonStopAllService();
    /* �������з�����Ӧ */
    afx_msg void OnBnClickedButtonRestartAllService();
    /* ��ʾ���� */
    afx_msg void OnIconnotifyShowWindow();
    /* ���ش��� */
    afx_msg void OnIconnotifyHideWindow();
    /* �˳� */
    afx_msg void OnIconnotifyExit();
    /* �������̲˵��е���ʾ���ڲ˵���״̬ */
    afx_msg void OnUpdateIconnotifyShowWindow(CCmdUI *pCmdUI);
    /* �������̲˵��е����ش��ڲ˵���״̬ */
    afx_msg void OnUpdateIconnotifyHideWindow(CCmdUI *pCmdUI);
    /* �������̲˵��е��˳��˵���״̬ */
    afx_msg void OnUpdateIconnotifyExit(CCmdUI *pCmdUI);
    /* ��ʼ���˵��ĵ��� */
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

    /* ��ʱ��Ϣ��ť��Ӧ */
    afx_msg void OnBnClickedButtonInstantShowInfo(void);
    /* �����б�ť��Ӧ */
    afx_msg void OnBnClickedButtonServiceList(void);
    /* �������ð�ť��Ӧ */
    afx_msg void OnBnClickedButtonConfig(void);

    /* �������б���ѡ������һ���Ӧ */
    afx_msg void OnNMRClickListService(NMHDR *pNMHDR, LRESULT *pResult);

protected:
    std::list<ServiceController*> m_lstServiceController;    ///< �������ļ��ж�ȡ�� ServiceController �б�
    bool m_bWindowShowing;      ///< ������������ʾ/����״̬
    NOTIFYICONDATA m_nid;       ///< ϵͳ����ͼ��
    CImageList m_listImageList; ///< �б�ͼƬ
    HICON m_hIcon;              ///< ����ͼƬ

public:
    CSkinButton m_buttonMin;    ///< ��С����ť
    CSkinButton m_buttonExit;   ///< �˳���ť
    CAppStat m_appStat;         ///< ״̬

    CSkinListCtrl m_listService;    ///< �����б�

    CSkinButton m_buttonViewIstantMessage;  ///< ��ʱ��Ϣ��ť
    CSkinButton m_buttonServiceList;        ///< �����б�ť
    CSkinButton m_btnCfg;                   ///< �������ð�ť

    CViewDlg m_viewDlg;                     ///< ��ʱ��Ϣ����
    CSvcCfg m_svcCfgDlg;                    ///< �������ý���

    DLG_FLAGS m_nDlgFlag;                   ///< ������ʾ��־λ��0:�����б� ; 1:��ʱ��Ϣ ; 2:��������

    //HPP_HANDLE m_hppDCClientHandle;         ///< ���������ķ������ӵ�HPPЭ��ջ

    //HPP_HANDLE m_hppLocalHandle;            ///< ���ؼ��������HPPЭ��ջ
    //HPP_SERVER_HANDLE m_hppLocalSerHanlde;  ///< ���ؼ�������

    CRITICAL_SECTION m_hppDCClientHandleLock;   ///< ���������ķ������ӵ�Lock
    CRITICAL_SECTION m_hppLocalSevHanldeLock;   ///< ����HPP�����Lock

private:
     CAppLogo m_appLogo;         ///< LOGO
     BOOL m_bIsRedBlackSkin;     ///< Ƥ��ѡ���־

     HANDLE m_hEventGetInfo;     ///< ��ȡ��Ϣ�Ŀ����¼�
     HANDLE m_hThreadGetInfo;    ///< ��ȡ��Ϣ���߳̾��

     FILETIME m_preIdleTime;     ///< ǰһ�εĿ���ʱ��
     FILETIME m_preKernelTime;   ///< ǰһ�ε��ں�ʱ��
     FILETIME m_preUserTime;     ///< ǰһ�ε��û�ʱ��
     FILETIME m_curIdleTime;     ///< ��ǰ�Ŀ���ʱ��
     FILETIME m_curKernelTime;   ///< ��ǰ���ں�ʱ��
     FILETIME m_curUserTime;     ///< ��ǰ���û�ʱ��

     HANDLE m_hCtrlMsgThread;    ///< ������Ϣ������߳̾��
     HANDLE m_hCtrlMsgEvent;     ///< ������Ϣ����Ŀ����¼�  
public:
    /* ��ʼ��button */
    BOOL InitButton(void);

    /* ���ؿ��Ź���Ƥ�� */
    BOOL LoadWatchBitMap(BOOL bIsRedAndBlack);

    /* ��ʼ����ʱ��Ϣ����ͷ������ý��� */
    BOOL InitAddDlg(void);

    ///* ���³�ʼ�����������ķ���֮������� */
    //BOOL ReconDcServer(void);
    ///* ���³�ʼ�����ؼ��� */
    //BOOL ReinitLocalServer(void);

    /* �˵��п�ʼ������Ӧ */
    void MenuStartService(void);
    /* �˵���ֹͣ������Ӧ */
    void MenuStopService(void);
    /* �˵�������������Ӧ */
    void MenuRestartService(void);
    /* �˵���ȫ����װ������Ӧ */
    void MenuInstallALLService(void);
    /* �˵��а�װ������Ӧ */
    void MenuInstallService(void);
    /* �˵���ж�ط�����Ӧ */
    void MenuUninstallService(void);
    /* �˵���һ��ж����Ӧ */
    void MenuUninstallAllService(void);
    /* �˵��д���������Ӧ */
    void MenuOpenConfig(void);
    /* ������Ŀ¼ */
    void MenuOpenRunDir(void);
    /* �������� */
    BOOL RunCmd(const CString& cmd, const CString& DefaultDir);
    /* �˵��еļ�ʱ��Ϣ��Ӧ */
    void MenuInstantMsg(void);

    /* ��ʾ������Ϣ */
    void ShowCtrlMsg(const RECV_CMD_MSG& struRecvCmdMsg);
    /* ����Ѳ��Ŀ������� */
    BOOL OnServiceWatchDogControl(const CString& svcName, HPR_INT32 controlType);
    /* �������� */
    BOOL OnServiceWatchDogControl(HPR_INT32 moduleType, HPR_INT32 controlType);
    /* ���������������� */
    BOOL OnServiceWatchDogControl(void);

    /* ����ϵͳ��ʾ��Ϣ */
    BOOL CreateSysShowMsg(const CString &csMsg, const CString &csMsgResult);

    ///* ��ʼ����ȡ��Ϣ���߳� */
    //BOOL InitGetThread(void);
    ///* ��ȡ��Ϣ���߳� */
    //static DWORD WINAPI ThreadForGetInfo(LPVOID lParam);
    /* �رջ�ȡ��Ϣ���߳� */
    void CloseGetThread(void);

    /* ��ʼ��������Ϣ�Ĵ����߳� */
    BOOL InitCtrlMsgThread(void);
    /* ������Ϣ�Ĵ����߳� */
    static DWORD WINAPI ThreadForCtrlMsg(LPVOID lParam);
    /* �رտ�����Ϣ�Ĵ����߳� */
    void CloseCtrlMsgThread(void);

    /* ��ʼ������ϵͳCPU��FILETIME */
    BOOL InitFileTime(void);

    /* ��ȡCPU */
    int GetCpu(void);

    /* ��ȡϵͳ������Ϣ */
    BOOL GetDiskInfo(std::vector<DISK_INFO>& diskInfo);

    /* ��ȡϵͳ�ڴ���Ϣ */
    BOOL GetMemInfo(int& TotalMem, int& UsingMem);

    /* ��ȡ��������Ϣ */
    CString GetCpuInfo(void);

    /* ��ȡ����ϵͳ��Ϣ */
    CString GetOSInfo(void);

    /* ��ȡ������ */
    void GetHostName(CString& HostName, CString& SN);

    /* ��ȡ����ʱ�� */
    CString GetSysStartTime(void);

    // ��ȡService Manager������ʱ�� */
    CString GetServiceManagerStartTime(void);

    /* ��ȡ����ϵͳλ�� */
    BOOL GetOSType(int& OsType);

    /* ��ȡ������Ϣ */
    BOOL GetNetWorkInfo(std::vector<NET_WORK_INFO>& vecNetWorkInfo);

    ///* ʹ��HPP������Ϣ��CMS */
    //BOOL SendInfoToCMS(const SERVER_SYS_INFO& SerSysInfo, const std::vector<SERVER_PROCESS_INFO>& SerProcInfo);

    ///* ����Уʱ���� */
    //BOOL SendReqServerData(void);

    /* �������� */
    BOOL RestartComputer(void);

    ///* ��ʾDC�ͱ��ص�IP��PORT */
    //void ShowDCAndLocal(void);

    /* �ж��Ƿ������з��񶼲����� */
    BOOL IsAllServiceNotExist(void);
    /* �ж��Ƿ������з��񶼿��� */
    BOOL IsAllServiceExist(void);

protected:

    /** @fn       char* CMainDialog::FormatServiceStatus(DWORD svcStatus)
     *  @brief    ��ʽ�� Service Status Ϊ�ַ���
     *  @param    (DWORD) svcStatus [IN] : Service Status
     *  @return:  char* : �� svcStatus ��Ӧ���ַ���
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
     *  @brief    ���� Service Status ��ȡ ListCtrl ͼ��
     *  @param    (DWORD) svcStatus [IN] : Service Status
     *  @return:  int��ListCtrlͼ������
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
