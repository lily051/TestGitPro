
// FMSClientDlg.h : 头文件
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

// CFMSClientDlg 对话框
class CFMSClientDlg : public CFDialog
{
// 构造
public:
	CFMSClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CFMSClientDlg();
// 对话框数据
	enum { IDD = IDD_FMSCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void InitMainWnd(void);
	void LoadSkin(void);  //加载主界面皮肤
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
	//菜单项操作
	void DoMenuCmd(int nCmd);
	void BeforeExit();

	CFNcButton * m_pBtnBusinessMgr;
	CFNcButton * m_pBtnBusinessList;
	CFNcButton * m_pBtnBusinessCfg;
	CFNcButton * m_pBtnMenu;

	/* WM_ICON_NOTIFY 消息响应 */
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	NOTIFYICONDATA m_nid;       ///< 系统托盘图标
	/* 添加系统托盘图标 */
	void AddNotifyIcon(void);
	/* 删除系统托盘图标 */
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
