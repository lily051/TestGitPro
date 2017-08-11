
// SetNTPDlg.h : ͷ�ļ�
//

#pragma once


// CSetNTPDlg �Ի���
class CSetNTPDlg : public CDialog
{
// ����
public:
	CSetNTPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SETNTP_DIALOG };

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
	afx_msg void OnBnClickedBtnStart();
	DECLARE_MESSAGE_MAP()
public:
	void GetData();
	BOOL ConnectDataBase();
	void PushQueue();
	void PopQueue();
	BSTR SetDevNTPConfig(DVRStruct & funParam);
	void PrintInfo();
public:
	//MYSQL
	MYSQL m_MySql;
	MYSQL_RES *m_Result; 
	MYSQL_ROW m_Sql_row; 
	//���ݿ�
	CString m_csIP;
	int     m_iPort;
	CString m_csUser;
	CString m_csPwd;
	CString m_csAction;
    //Ntp������
	CString m_csNtpIp;
	int     m_iNtpPort;
	int     m_iTime;

	char m_cDataBaseIp[CHAR_LENGTH];
	char m_cDataBaseUser[CHAR_LENGTH];
	char m_cDataBasePwd[CHAR_LENGTH];
	char m_csDataBaseAction[CHAR_LENGTH];
	char m_cNtpIp[CHAR_LENGTH];

	//DVR����
	queue<DVRStruct> m_DvrList;
	queue<DVRStruct> m_FaildList;
	int m_iTotalNumber;
	int m_iSeccessful;
	
	CString m_strDataBaseIp;
};
