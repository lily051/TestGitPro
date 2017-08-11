#pragma once
#include "resource.h"
#include "Login.h"
#include "FingerMgr.h"
#include <vector>
#include "afxwin.h"

enum _e_login_show_type
{
    LOGIN_SHOW_TYPE_ACCOUNT_ONLY = 0,     // �û�����¼
    LOGIN_SHOW_TYPE_ACCOUNT =      1,     // ˫��¼(�û���)
    LOGIN_SHOW_TYPE_FINGER  =      2,     // ˫��¼(ָ��)
};

// CLoginDlg �Ի���

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
        int nUseAutoLogin, int nSkin, int nLanguage, 
        CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnCancel();

public:
    STU_FP_INFO m_stuFpInfo;            // ָ�ƻ���Ϣ
    char m_strCardNo[ACS_CARD_NO_LEN];  // ָ�ƶ�Ӧ�Ŀ���

    // ָ�Ƶ�¼
    void FingerLogin(char* strCardNo);

    // ����ͼ
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    // ����ƶ�����
    afx_msg LRESULT OnNcHitTest(CPoint point);
    // ����һЩ��Ϣ�����Զ��崦��
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    //< ��ť��Ϣ��Ӧ
    afx_msg void OnBnClickedBtnConfig();
    afx_msg void OnBnClickedBtnMin();
    afx_msg void OnBnClickedBtnClose();
    afx_msg void OnBnClickedCheckRemember();
    afx_msg void OnBnClickedCheckAutologin();
    afx_msg void OnBnClickedBtnLogin();
    afx_msg void OnBnClickedButtonSave();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedButtonUserlogin();
    afx_msg void OnBnClickedButtonFplogin();
    afx_msg void OnCbnEditupdateComboUsername();
    afx_msg void OnCbnSelchangeComboUsername();
    afx_msg void OnCbnSelchangeComboLocalip();
    afx_msg void OnEnSetfocusEditPassword();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    afx_msg LRESULT OnMsgLoginCenter(WPARAM, LPARAM);
    afx_msg LRESULT OnMsgPreLogin(WPARAM, LPARAM);
    afx_msg LRESULT OnMsgAftLogin(WPARAM, LPARAM);
    afx_msg LRESULT OnMsgFpLoginResult(WPARAM, LPARAM);
    afx_msg LRESULT OnMsgFingerLogin(WPARAM, LPARAM);

    // ��¼�̵߳��ú���
    void LoginCenter();
private:
    //< ����·��
    CString m_strAppPath;
    //< Ƥ������
    int m_nSkin;
    //< ��������
    int m_nLanguage;
    //< �ϲ����ʱ�Ƿ��Զ���¼
    int m_nUseAutoLogin;
     //< ����ͼƬ
    Image* m_pImgBkg;

	//ͼ��
	HICON m_hIcon;

    CFingerMgr m_fingerMgr;  // ָ�ƻ�����

    //< Title, subTitle
    CString m_strTitle, m_strSubTitle;
    CFont m_ftTitle, m_ftCopyright;
    //< copyright
    CString m_strCopyRight;

    // �Ƿ���ʾ��������
    BOOL m_bShowServer;
    // �Ƿ��ס����
    BOOL m_bRemmberPwd;
    // �Ƿ��Զ���¼ 
    BOOL m_bAutoLogin;
	// ���������Ƿ���Ҫ���
	BOOL m_bPassWordReset;

    // ���ؼ�¼���û���
    typedef struct _login_user_info 
    {
        int nIdx;               //< ���
        CString strUsername;    //< �û���
        CString strPassword;    //< ����
        CString strServerIP;    //< ����IP
        int nServerPort;        //< ����˿�
        CString strLocalIP;     //< ����IP

        _login_user_info()
        {
            nIdx = 0;
            nServerPort = 8849;
        }
    }LOGIN_USER_INFO;
    typedef std::map<CString, LOGIN_USER_INFO>  MAP_LOGIN_USER;
    MAP_LOGIN_USER m_mapLoginUser;

    CString m_strLastUser;              //< ��һ�ε�¼ʹ�õ��û�

    HANDLE m_hLoginThread;
    LOGIN_USER_INFO m_loginUserInfo;    // ��ǰ�ĵ�¼��Ϣ
    int m_nLoginType;                   // ��¼��������
    int m_nFingerImgId;                 // ָ��ͼƬID
    CRect m_rcFingerImg;    // ָ��ͼƬ����

    _LoginEvent m_pLoginEvent;
    void* m_pUser;
private:
    // ��ȡ����·��
    void GetAppPath();
    // ���ÿؼ�
    void InitCtrl(void);
    // ���ݶ��������ý�������
    void InitText(void);
    // �ƶ��ؼ�λ��
    void MoveCtrl(void);
    // ����Ƥ��
    void LoadSkin(void);
    // ��ʾ��¼���棬bShow:TRUE��¼���棬FALSE�������
    // nType:0-�û�����¼��1-˫��¼(�û���)��2-˫��¼(ָ��)
    void ShowLogin(BOOL bShow, int nType = LOGIN_SHOW_TYPE_ACCOUNT_ONLY);
    // ���ر�������
    void LoadConfig();
    // �Ƿ�ָ�Ƶ�¼,�����ȡָ���豸��Ϣ
    BOOL GetFingerPrint(STU_FP_INFO& stuFpInfo);
    // �ӱ����ļ��м�����ʾ��Ϣ�����⡢�ӱ��⡢��Ȩ��Ϣ
    void LoadText();

    // У�������Ƿ�Ϸ�
    BOOL CheckInputs(LOGIN_USER_INFO& stLoginInfo);
    // ��¼����
    BOOL LoginToServer(LOGIN_USER_INFO& stLoginInfo);
    // 
    void SaveLoginInfo();

	//��������ϴ��ͻ�������
	void SaveUploadClientConfig();

	void StartUploadClient();

	BOOL RunCmd(const CString& cmd, const CString& DefaultDir);

    // ��ʾ������ʾ
    void ShowErrorMsg(const CString& strMsg);

    // ��¼ʱ���ÿؼ�������
    void DisableCtrl(BOOL bDisable);

    // �����¼��ʷ
    BOOL ClearLoginHistory();

    // ��ʾ�û���¼��Ϣ
    void ShowLoginInfo(const CString& strUsername);

    // �û���¼��Ϣ��ת��
    void LoginUserInfoTrans(LIB_LOGIN_USER_INFO& loginUserInfo, LOGIN_USER_INFO& stLoginUserInfo);

    // �������ü��
    BOOL ServerConfigCheck(const LOGIN_USER_INFO& logininfo);

    // ��ȡ��������
    BOOL LoadServerConfig(LOGIN_USER_INFO& logininfo);

    // ˢ��ָ��ͼƬ
    void FlashFingerImg(BOOL bDrawImg);

    // ��ȡ�Զ���¼��Ϣ
    BOOL GetAutoLogin();

	std::string Encrypt(const char *szString);
	std::string Decrypt(const char *szString);
public:
    CFButton m_btnUserLogin;
    CFButton m_btnFpLogin;
    CFButton m_btnConfig;
    CFButton m_btnMin;
    CFButton m_btnClose;
    CFButton m_btnSave;
    CFButton m_btnCancel;
    CFButton m_btnLogin;
    CFButton m_btnErrorTip;
    CFStatic m_stWelcome;
    CFStatic m_stCopyright;
    CFStatic m_stFinger;
    CFEdit m_edPassword;
    CFComboBox m_cbUsername;
    CFButton m_ckbRemember;
    CFButton m_ckbAutologin;
    CFEdit m_edServerIP;
    CFEdit m_edServerPort;
    CFComboBox m_cbLocalIP;
    CFProgress m_pgcLogin;
    Image* m_pImgFinger;

    afx_msg void OnDestroy();
	CFButton m_btnLocalLogin;
	afx_msg void OnBnClickedButtonLocal();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
};
