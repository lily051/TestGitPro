
// DBSimulateToolForTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "fcConfigParser.h"
#include "HPR_FileSystem.h"
#include "HPR_Error.h"
#include "MySqlDBLayer.h"

//------yh------
#define CONFIG_FILE            _T("DBParam.ini")
#define INI_SEC_AUTH           _T("DBParam")
#define KEY_DB_IP              _T("DBIp")
#define KEY_DB_PORT            _T("DBPort")
#define KEY_DB_USERNAME        _T("DBUsername")
#define KEY_DB_PASSWORD        _T("DBPassword")
#define KEY_DB_SCHEMA          _T("DBSchema")
#define KEY_DB_IP_REF              _T("DBIpRef")
#define KEY_DB_PORT_REF            _T("DBPortRef")
#define KEY_DB_USERNAME_REF        _T("DBUsernameRef")
#define KEY_DB_PASSWORD_REF        _T("DBPasswordRef")
#define KEY_DB_SCHEMA_REF          _T("DBSchemaRef")


#define DEFAULT_DB_IP          _T("127.0.0.1")
#define DEFAULT_DB_PORT        _T("3306")
#define DEFAULT_DB_USERNAME    _T("root")
#define DEFAULT_DB_PASSWORD    _T("888")
#define DEFAULT_DB_SCHEMA      _T("ivms8120v2")
#define DEFAULT_DB_IP_REF          _T("127.0.0.1")
#define DEFAULT_DB_PORT_REF        _T("3306")
#define DEFAULT_DB_USERNAME_REF    _T("root")
#define DEFAULT_DB_PASSWORD_REF    _T("888")
#define DEFAULT_DB_SCHEMA_REF      _T("ivms8120v2")
namespace my{
    TCHAR const INI_CFG_FILE_NAME[] = _T("DBParam.ini");
    TCHAR const INI_SEC_DB_PARAM[] = _T("DBParam");
    TCHAR const INI_SEC_DB_IP[] = _T("DBIp");
    TCHAR const INI_SEC_DB_PORT[] = _T("DBPort");
    TCHAR const INI_SEC_DB_USERNAME[] = _T("DBUsername");
    TCHAR const INI_SEC_DB_PASSWORD[] = _T("DBPassword");
    TCHAR const INI_SEC_DB_SCHEMA[] = _T("DBSchema");
    TCHAR const DEFAULT_SEC_DB_IP[] = _T("127.0.0.1");
    TCHAR const DEFAULT_SEC_DB_PORT[] = _T("3306");
    TCHAR const DEFAULT_SEC_DB_USERNAME[] = _T("root");
    TCHAR const DEFAULT_SEC_DB_PASSWORD[] = _T("888");
    TCHAR const DEFAULT_SEC_DB_SCHEMA[] = _T("ivms8120v2");
    //
    TCHAR const INI_SEC_DB_IP_REF[] = _T("DBIpRef");
    TCHAR const INI_SEC_DB_PORT_REF[] = _T("DBPortRef");
    TCHAR const INI_SEC_DB_USERNAME_REF[] = _T("DBUsernameRef");
    TCHAR const INI_SEC_DB_PASSWORD_REF[] = _T("DBPasswordRef");
    TCHAR const INI_SEC_DB_SCHEMA_REF[] = _T("DBSchemaRef");
    TCHAR const DEFAULT_SEC_DB_IP_REF[] = _T("127.0.0.1");
    TCHAR const DEFAULT_SEC_DB_PORT_REF[] = _T("3306");
    TCHAR const DEFAULT_SEC_DB_USERNAME_REF[] = _T("root");
    TCHAR const DEFAULT_SEC_DB_PASSWORD_REF[] = _T("888");
    TCHAR const DEFAULT_SEC_DB_SCHEMA_REF[] = _T("ivms8120v2");
}
#define TIMER 1
// CDBSimulateToolForTestDlg 对话框
class CDBSimulateToolForTestDlg : public CDialog
{
// 构造
public:
	CDBSimulateToolForTestDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CDBSimulateToolForTestDlg();

// 对话框数据
	enum { IDD = IDD_DBSIMULATETOOLFORTEST_DIALOG };

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
private:
    CIPAddressCtrl m_ctrlDBIp;
    CEdit m_editDBPort;
    CEdit m_editDBUserName;
    CEdit m_editDBPassWord;
    CEdit m_editDBSchema;

    CString m_strDBIP;
    CString m_strDBPort;
    CString m_strDBUser;
    CString m_strDBPassword;
    CString m_strDBSchema;

    CMySqlDBLayer m_mySQL;
    volatile bool m_bLogin;

    volatile bool m_bRadioRegion; 
    volatile bool m_bRadioProduct;
    volatile bool m_bRadioUsers;
    volatile bool m_bRadioDeviceChannel;
    volatile bool m_bRadioDevice;
    volatile bool m_bRadioProsaveRecord;
    volatile bool m_bRadioClientinput;
    volatile bool m_bRadioAlarmlog;
    volatile bool m_bRadioSelectAll;
public:
    void InitConfig();
    void SaveParamLogin();
    void BuildDefaultFile();

    void StartWork();
    bool isNum(std::string &str);
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedRadioRegion();
    afx_msg void OnBnClickedRadioProduct();
    afx_msg void OnBnClickedRadioUser();
    afx_msg void OnBnClickedRadioDeviceChannel();
    afx_msg void OnBnClickedRadioDevice();
    afx_msg void OnBnClickedRadioRecord();
    afx_msg void OnBnClickedRadioClientinput();
    afx_msg void OnBnClickedRadioAlarmlog();
    afx_msg void OnBnClickedRadioSelectAll();
    afx_msg void OnBnClickedButtonDbCheck();
    afx_msg void OnBnClickedButtonExpand();
private:
    CIPAddressCtrl m_ctrlDBIpRef;
    CEdit m_editDBPortRef;
    CEdit m_editDBUserNameRef;
    CEdit m_editDBPassWordRef;
    CEdit m_editDBSchemaRef;

    CString m_strDBIPRef;
    CString m_strDBPortRef;
    CString m_strDBUserRef;
    CString m_strDBPasswordRef;
    CString m_strDBSchemaRef;

    CMySqlDBLayer m_mySQLRef;
    volatile bool m_bLoginRef;

    volatile bool m_bExistReferData;
    std::vector<region_info_t> vecRegionInfo;
    std::vector<product_info_t>vecProductInfo;
    std::vector<users_info_t> vecUsersInfo;
    std::vector<devicechannel_info_t> vecDevicechannelInfo;
    std::vector<device_info_t> vecDeviceInfo;
    std::vector<prosaverecord_info_t> vecProsaverecordInfo;
    std::vector<clientinput_info_t> vecClientinputInfo;
    std::vector<alarmlog_info_t> vecAlarmlogInfo;

    HANDLE m_lhandleSimulate;
    volatile bool m_bStopHandleSimulate;
    volatile bool m_bStartSimulateData;
    volatile bool m_bEndSimulateData;
    volatile bool m_bDot;
public:
    afx_msg void OnBnClickedButtonLoginRef();
    afx_msg void OnBnClickedButtonImPort();
    afx_msg void OnBnClickedButtonSimulateDBData();
    afx_msg void OnTimer(UINT nIDEvent);
    //
    void LoadRegion(std::vector<region_info_t> &vecRegionInfo);
    void LoadProduct(std::vector<product_info_t> &vecProductInfo);
    void LoadUsers(std::vector<users_info_t> &vecUsersInfo);
    void LoadDevicechannel(std::vector<devicechannel_info_t> &vecDevicechannelInfo);
    void LoadDevice(std::vector<device_info_t> &vecDeviceInfo);
    void LoadProsaverecord(std::vector<prosaverecord_info_t> &vecProsaverecordInfo);
    void LoadClientinput(std::vector<clientinput_info_t> &vecClientinputInfo);
    void LoadAlarmlog(std::vector<alarmlog_info_t> &vecAlarmlogInfo);

private:
    CString m_nSelectOneSetNum;
};
