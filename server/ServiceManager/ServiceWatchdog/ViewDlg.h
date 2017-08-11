/**
*   @file ViewDlg.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 显示即时消息的对话框
*   
*   @author mazhentong
*   @date   2012/08/14
*
*   @note 历史记录：create
*	
*   @note 显示即时消息的对话框，其中包含一个CTabCtrl, N个CSkinListCtrl（N是实际的启动过的服务器总数）
*         其中还有一个CTabCtrl的选择响应函数
*
*   @warning
*/
#pragma once

#include "./Lang/MultiLanRes.h"
#include "./Lang/resource.h"

#include "MsgQueue.h"
#include "GenDef.h"
//#include "include/snmpManager.h"
#include "SkinTabCtrl.h"
#include <map>
#include "./SynchroQueue.h"
//#include "./MySqlDBLayer.h"
#include "WorkThreadBase.h"
#include "AlarmProcessorBase.h"

// CViewDlg 对话框

class CViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIEWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_viewImageList;
    CSkinTabCtrl m_viewTab;     ///< Tab控件对应的变量
    int m_listWidth;            ///< ListCtrl的Width

    //CMsgQueue m_msgQueue;       ///< 消息队列
	CSynchroQueue <dummy_msg_t> m_msgQueue;	  ///< 消息队列

    SOCKET m_socketServer;      ///< UDP接收消息的socket
    SOCKADDR_IN m_addrServer;   ///< bind的socket地址

    SOCKADDR_IN m_addrClient;   ///< clientsocket

    HANDLE m_handleRecvThread;  ///< 接收消息的线程的句柄

    BOOL m_bExitFlag;           ///< 线程退出标志

    std::list<COL_WIDTH_INFO> m_listColWidthInfo;   ///< 即时消息列表列宽信息链表

	CAlarmProcessorBase *m_pProcessor;
    CAlarmProcessorBase *m_pProcessorCol;

public:
    BOOL InitTabControl(void);      // 初始化Tab
    BOOL InitListControl(void);     // 初始化ListCtrl

    BOOL InitSocketServer(void);    // 初始化Socket
    BOOL InitRecvThread(void);      // 初始化ThreadHandle

    std::list<SERVICE_INFO> m_listServiceInfo; // 保存TAB，List，Service的对应关系

    //BOOL m_isChgingService;     ///< 服务配置是否正在更改
    static DWORD WINAPI RecvThreadFun(LPVOID lpParam); // 接收消息函数执行体

    afx_msg void OnTcnSelchangeTabViewcontrol(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    // 显示消息
    BOOL CreateShowMsg(const CString &csMsg, const CString &csMsgResult);

    /* 根据ModuleType显示服务消息 */
    BOOL ShowInstantMsg(int nModuleType);
    /* 根据服务名显示服务消息 */
    BOOL ShowInstantMsg(const CString& svcName);

    /* 通过nModuleType获取记录的列宽 */
    int GetColWidth(DWORD nModuleType, int nCol = 0);
    /* 通过ServiceName获取记录的列宽 */
    //int GetColWidth(std::string strSrvName, int nCol = 0);

    /* 获取保存的列表列宽信息 */
    void GetColWidthInfo(void);
    /* 保存服务列表的列宽 */
    void SaveColWidthInfo(void);

    //static HPR_VOID CALLBACK s_SnmpManagerMsgFunc(PPDUINFO ppdu, HPR_VOIDPTR context);
    //HPR_VOID SnmpManagerMsgFunc(PPDUINFO ppdu);
    //int ProcManagerTrap(PPDUINFO ppdu);

    std::map<int, int> m_mapSrvType;
    void InitSrvType();	
};
