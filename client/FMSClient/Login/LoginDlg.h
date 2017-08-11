#pragma once
#include "resource.h"
#include "Login.h"
#include "FingerMgr.h"
#include <vector>
#include "afxwin.h"

enum _e_login_show_type
{
    LOGIN_SHOW_TYPE_ACCOUNT_ONLY = 0,     // 用户名登录
    LOGIN_SHOW_TYPE_ACCOUNT =      1,     // 双登录(用户名)
    LOGIN_SHOW_TYPE_FINGER  =      2,     // 双登录(指纹)
};

// CLoginDlg 对话框

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
        int nUseAutoLogin, int nSkin, int nLanguage, 
        CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnCancel();

public:
    STU_FP_INFO m_stuFpInfo;            // 指纹机信息
    char m_strCardNo[ACS_CARD_NO_LEN];  // 指纹对应的卡号

    // 指纹登录
    void FingerLogin(char* strCardNo);

    // 背景图
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    // 鼠标移动窗口
    afx_msg LRESULT OnNcHitTest(CPoint point);
    // 拦截一些消息进行自定义处理
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    //< 按钮消息响应
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

    // 登录线程调用函数
    void LoginCenter();
private:
    //< 程序路径
    CString m_strAppPath;
    //< 皮肤类型
    int m_nSkin;
    //< 语言类型
    int m_nLanguage;
    //< 上层调用时是否自动登录
    int m_nUseAutoLogin;
     //< 背景图片
    Image* m_pImgBkg;

	//图标
	HICON m_hIcon;

    CFingerMgr m_fingerMgr;  // 指纹机管理

    //< Title, subTitle
    CString m_strTitle, m_strSubTitle;
    CFont m_ftTitle, m_ftCopyright;
    //< copyright
    CString m_strCopyRight;

    // 是否显示服务配置
    BOOL m_bShowServer;
    // 是否记住密码
    BOOL m_bRemmberPwd;
    // 是否自动登录 
    BOOL m_bAutoLogin;
	// 标记密码框是否需要清空
	BOOL m_bPassWordReset;

    // 本地记录的用户名
    typedef struct _login_user_info 
    {
        int nIdx;               //< 序号
        CString strUsername;    //< 用户名
        CString strPassword;    //< 密码
        CString strServerIP;    //< 服务IP
        int nServerPort;        //< 服务端口
        CString strLocalIP;     //< 本机IP

        _login_user_info()
        {
            nIdx = 0;
            nServerPort = 8849;
        }
    }LOGIN_USER_INFO;
    typedef std::map<CString, LOGIN_USER_INFO>  MAP_LOGIN_USER;
    MAP_LOGIN_USER m_mapLoginUser;

    CString m_strLastUser;              //< 上一次登录使用的用户

    HANDLE m_hLoginThread;
    LOGIN_USER_INFO m_loginUserInfo;    // 当前的登录信息
    int m_nLoginType;                   // 登录界面类型
    int m_nFingerImgId;                 // 指纹图片ID
    CRect m_rcFingerImg;    // 指纹图片区域

    _LoginEvent m_pLoginEvent;
    void* m_pUser;
private:
    // 获取绝对路径
    void GetAppPath();
    // 设置控件
    void InitCtrl(void);
    // 根据多语言设置界面文字
    void InitText(void);
    // 移动控件位置
    void MoveCtrl(void);
    // 加载皮肤
    void LoadSkin(void);
    // 显示登录界面，bShow:TRUE登录界面，FALSE服务界面
    // nType:0-用户名登录，1-双登录(用户名)，2-双登录(指纹)
    void ShowLogin(BOOL bShow, int nType = LOGIN_SHOW_TYPE_ACCOUNT_ONLY);
    // 加载本地配置
    void LoadConfig();
    // 是否指纹登录,是则获取指纹设备信息
    BOOL GetFingerPrint(STU_FP_INFO& stuFpInfo);
    // 从本地文件中加载显示信息：标题、子标题、版权信息
    void LoadText();

    // 校验输入是否合法
    BOOL CheckInputs(LOGIN_USER_INFO& stLoginInfo);
    // 登录中心
    BOOL LoginToServer(LOGIN_USER_INFO& stLoginInfo);
    // 
    void SaveLoginInfo();

	//保存理财上传客户端配置
	void SaveUploadClientConfig();

	void StartUploadClient();

	BOOL RunCmd(const CString& cmd, const CString& DefaultDir);

    // 显示错误提示
    void ShowErrorMsg(const CString& strMsg);

    // 登录时设置控件不可用
    void DisableCtrl(BOOL bDisable);

    // 清除登录历史
    BOOL ClearLoginHistory();

    // 显示用户登录信息
    void ShowLoginInfo(const CString& strUsername);

    // 用户登录信息的转换
    void LoginUserInfoTrans(LIB_LOGIN_USER_INFO& loginUserInfo, LOGIN_USER_INFO& stLoginUserInfo);

    // 服务配置检查
    BOOL ServerConfigCheck(const LOGIN_USER_INFO& logininfo);

    // 读取服务配置
    BOOL LoadServerConfig(LOGIN_USER_INFO& logininfo);

    // 刷新指纹图片
    void FlashFingerImg(BOOL bDrawImg);

    // 获取自动登录信息
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
