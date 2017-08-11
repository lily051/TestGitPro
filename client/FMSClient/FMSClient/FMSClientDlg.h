
// FMSClientDlg.h : ͷ�ļ�
//

#pragma once

#include <set>
//#include "NavibarDlg.h"
#include "BaseLayer.h"
#include "BottomDlg.h"
#include "FUi/FDialog.h"
#include "BusinessManageDlg.h"
#include "BusinessListDlg.h"
#include "ConfigDlg.h"

// CFMSClientDlg �Ի���
class CFMSClientDlg : public CFDialog
{
// ����
public:
	CFMSClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CFMSClientDlg();
// �Ի�������
	enum { IDD = IDD_FMSCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void InitMainWnd(void);
	void LoadSkin(void);  //����������Ƥ��
	static DWORD WINAPI WorkStateThread(LPVOID pParam);

private:
	//std::tr1::shared_ptr<CNavibarDlg> m_pNavibarDlg;
	std::tr1::shared_ptr<CBaseLayer> m_pBaseLayer;
	//std::tr1::shared_ptr<CBottomDlg> m_lpBottomDlg;
	//std::tr1::shared_ptr<CViewTalkDlg> m_pViewTalkDlg;
public:
	void InitChildWnd(void);
	void MoveChildWnd(void);
	
protected:
	CBusinessManageDlg* m_lpBusinessManageDlg;
	CBusinessListDlg* m_lpBusinessListDlg;
	CConfigDlg* m_lpConfigDlg;
	CBottomDlg* m_lpBottomDlg;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnCancel();
	//�˵������
	void DoMenuCmd(int nCmd);
	void BeforeExit();

	CFNcButton * m_pBtnBusinessMgr;
	CFNcButton * m_pBtnBusinessList;
	CFNcButton * m_pBtnBusinessCfg;
	CFNcButton * m_pBtnMenu;

	/* WM_ICON_NOTIFY ��Ϣ��Ӧ */
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	NOTIFYICONDATA m_nid;       ///< ϵͳ����ͼ��
	/* ���ϵͳ����ͼ�� */
	void AddNotifyIcon(void);
	/* ɾ��ϵͳ����ͼ�� */
	void RemoveNotifyIcon(void);
public:
	afx_msg void OnIconnotifyShowWindow();
	afx_msg void OnIconnotifyExit();
	BOOL GetAutoLogin();
	HANDLE m_hThread;
	HANDLE m_hEvent;
	void InitData(void);
	void UnitData(void);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnNetDisconnect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString GernateUserComment();
	LRESULT OnMsgRebootApp(WPARAM wParam, LPARAM lParam);
};
