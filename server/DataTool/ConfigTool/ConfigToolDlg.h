
// ConfigToolDlg.h : ͷ�ļ�
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

// CConfigToolDlg �Ի���
class CConfigToolDlg : public CDialog
{
// ����
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONFIGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	//��ȡ�����ļ�
	void ReadConfig();
	//���������ļ�
	void SaveConfig();	
    
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
	bool	m_isCip;		//	�Ƿ�����
	std::string	m_strWebPort;	
	char m_xmlPath[FILE_BUFF];			//	server.xml·��
	char m_batPath[FILE_BUFF];			//	ServiceModuleConfig.dat·��
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
