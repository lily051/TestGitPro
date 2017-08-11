#pragma once

enum _e_fp_login_result
{
    FP_LOGIN_UNKOWN_FAILDE = -1,   // δ֪����
    FP_LOGIN_SUCCESS       = 0,    // ��¼�ɹ�
    FP_LOGIN_FAILED        = 1,    // ��¼ʧ��
    FP_LOGIN_UNSUPPORT     = 2,    // �豸��֧��
};

// ָ��������Ϣ
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

// ��¼ָ�ƻ�����ص�������
typedef void (*FmLoginCallBack)(int nResult, void* pUser);

// ָ�ƻ���������ص�������
typedef void (*FmAlarmCallBack)(char* strCardNo, void* pUser);

class CFingerMgr
{
public:
    CFingerMgr(void);
    ~CFingerMgr(void);

    LONG m_lFingerLogin;                // ��¼ID
    LONG m_lFingerAlarm;                // ����ID
    FmLoginCallBack m_pLoginCallBack;
    FmAlarmCallBack m_pAlarmCallBack;
    void* m_pLoginUser;
    void* m_pAlarmUser;
    CTime m_ctAlarm;        // ����ʱ��

    void Init();
    void UnInit();

    void GetFingerInfo(STU_FP_INFO& stuFpInfo);

    // ��¼ָ�ƻ�
    BOOL LoginFinger(const STU_FP_INFO& stuFpInfo, FmLoginCallBack loginCallBack, void* pUser);

    // ָ�ƻ�����
    BOOL Logout();

    // ָ�ƻ�����
    BOOL StartAlarm(CString& strError, FmAlarmCallBack listenCallBack, void* pUser);

    // ָ�ƻ�����
    BOOL CloseAlarm();

private:
    STU_FP_INFO m_stuFpInfo;
};
