#pragma once

enum _e_fp_login_result
{
    FP_LOGIN_UNKOWN_FAILDE = -1,   // 未知错误
    FP_LOGIN_SUCCESS       = 0,    // 登录成功
    FP_LOGIN_FAILED        = 1,    // 登录失败
    FP_LOGIN_UNSUPPORT     = 2,    // 设备不支持
};

// 指纹配置信息
typedef struct _s_fp_info
{
    int nUse;
    std::string strIp;
    std::string strPort;
    std::string strUser;
    std::string strPwd;
    _s_fp_info()
    {
        nUse = 0;
    }
}STU_FP_INFO;

// 登录指纹机结果回调函数，
typedef void (*FmLoginCallBack)(int nResult, void* pUser);

// 指纹机布防结果回调函数，
typedef void (*FmAlarmCallBack)(char* strCardNo, void* pUser);

class CFingerMgr
{
public:
    CFingerMgr(void);
    ~CFingerMgr(void);

    LONG m_lFingerLogin;                // 登录ID
    LONG m_lFingerAlarm;                // 布防ID
    FmLoginCallBack m_pLoginCallBack;
    FmAlarmCallBack m_pAlarmCallBack;
    void* m_pLoginUser;
    void* m_pAlarmUser;
    CTime m_ctAlarm;        // 布防时间

    void Init();
    void UnInit();

    void GetFingerInfo(STU_FP_INFO& stuFpInfo);

    // 登录指纹机
    BOOL LoginFinger(const STU_FP_INFO& stuFpInfo, FmLoginCallBack loginCallBack, void* pUser);

    // 指纹机撤防
    BOOL Logout();

    // 指纹机布防
    BOOL StartAlarm(CString& strError, FmAlarmCallBack listenCallBack, void* pUser);

    // 指纹机撤防
    BOOL CloseAlarm();

private:
    STU_FP_INFO m_stuFpInfo;
};
