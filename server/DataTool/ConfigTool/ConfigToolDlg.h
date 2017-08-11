
// ConfigToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "SkinDropdown.h"
#include <vector>
#include "afxcmn.h"
#include "MyIPAddressCtrl.h"
#include "IntEdit.h"
#include <string>

using namespace std;

#define FILE_BUFF	1024*8

// CConfigToolDlg 对话框
class CConfigToolDlg : public CDialog
{
// 构造
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFIGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	//读取配置文件
	void ReadConfig();
	//保存配置文件
	void SaveConfig();	
    
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
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnExit();
private:
    TCHAR m_szCfgPath[MAX_PATH];

private:
	CEdit m_editLoginName;
	CEdit m_editLoginPwd;
	CIntEdit m_editDataCenterPort;
    CComboBox m_cbDBMSType;
    CIntEdit m_edtListenPort;
	CIntEdit m_edtWebPort;
    std::vector<CString> m_vecLocalIP;
	string m_pwd;		
	bool	m_isCip;		//	是否密文
	std::string	m_strWebPort;	
	char m_xmlPath[FILE_BUFF];			//	server.xml路径
	char m_batPath[FILE_BUFF];			//	ServiceModuleConfig.dat路径
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	char* get_file(char *cBuff);
	char* getBatCfg(char *cBuff);
	std::string load_displayName_xml();
protected:
    virtual void OnCancel();
    
public:
    afx_msg void OnCbnSelchangeComboDbms();
    void IPHistoryDropdown(void);
    CMyIPAddressCtrl m_ipctrlDB;
    CMyIPAddressCtrl m_ipctrlLocal;
    CSkinDropdown m_dropdown;

private:
	typedef enum
	{
		MYSQL_DB=0,
		SQLSERVER_DB=1,
	};
};
