/**
*   @file ViewDlg.h
*   @note HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief ��ʾ��ʱ��Ϣ�ĶԻ���
*   
*   @author mazhentong
*   @date   2012/08/14
*
*   @note ��ʷ��¼��create
*	
*   @note ��ʾ��ʱ��Ϣ�ĶԻ������а���һ��CTabCtrl, N��CSkinListCtrl��N��ʵ�ʵ��������ķ�����������
*         ���л���һ��CTabCtrl��ѡ����Ӧ����
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

// CViewDlg �Ի���

class CViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_VIEWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_viewImageList;
    CSkinTabCtrl m_viewTab;     ///< Tab�ؼ���Ӧ�ı���
    int m_listWidth;            ///< ListCtrl��Width

    //CMsgQueue m_msgQueue;       ///< ��Ϣ����
	CSynchroQueue <dummy_msg_t> m_msgQueue;	  ///< ��Ϣ����

    SOCKET m_socketServer;      ///< UDP������Ϣ��socket
    SOCKADDR_IN m_addrServer;   ///< bind��socket��ַ

    SOCKADDR_IN m_addrClient;   ///< clientsocket

    HANDLE m_handleRecvThread;  ///< ������Ϣ���̵߳ľ��

    BOOL m_bExitFlag;           ///< �߳��˳���־

    std::list<COL_WIDTH_INFO> m_listColWidthInfo;   ///< ��ʱ��Ϣ�б��п���Ϣ����

	CAlarmProcessorBase *m_pProcessor;
    CAlarmProcessorBase *m_pProcessorCol;

public:
    BOOL InitTabControl(void);      // ��ʼ��Tab
    BOOL InitListControl(void);     // ��ʼ��ListCtrl

    BOOL InitSocketServer(void);    // ��ʼ��Socket
    BOOL InitRecvThread(void);      // ��ʼ��ThreadHandle

    std::list<SERVICE_INFO> m_listServiceInfo; // ����TAB��List��Service�Ķ�Ӧ��ϵ

    //BOOL m_isChgingService;     ///< ���������Ƿ����ڸ���
    static DWORD WINAPI RecvThreadFun(LPVOID lpParam); // ������Ϣ����ִ����

    afx_msg void OnTcnSelchangeTabViewcontrol(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    // ��ʾ��Ϣ
    BOOL CreateShowMsg(const CString &csMsg, const CString &csMsgResult);

    /* ����ModuleType��ʾ������Ϣ */
    BOOL ShowInstantMsg(int nModuleType);
    /* ���ݷ�������ʾ������Ϣ */
    BOOL ShowInstantMsg(const CString& svcName);

    /* ͨ��nModuleType��ȡ��¼���п� */
    int GetColWidth(DWORD nModuleType, int nCol = 0);
    /* ͨ��ServiceName��ȡ��¼���п� */
    //int GetColWidth(std::string strSrvName, int nCol = 0);

    /* ��ȡ������б��п���Ϣ */
    void GetColWidthInfo(void);
    /* ��������б���п� */
    void SaveColWidthInfo(void);

    //static HPR_VOID CALLBACK s_SnmpManagerMsgFunc(PPDUINFO ppdu, HPR_VOIDPTR context);
    //HPR_VOID SnmpManagerMsgFunc(PPDUINFO ppdu);
    //int ProcManagerTrap(PPDUINFO ppdu);

    std::map<int, int> m_mapSrvType;
    void InitSrvType();	
};
