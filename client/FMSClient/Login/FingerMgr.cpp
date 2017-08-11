#include "StdAfx.h"
#include "HCNetSDK.h"
#include "FingerMgr.h"
#include "MultiLang/lClient.h"

// 读取配置文件判断指纹设备是否支持
BOOL IsFpDeviceSupport(int id)
{
    TCHAR szFileName[MAX_PATH] = {0};
    if (0 == GetModuleFileName(NULL, szFileName, MAX_PATH - 1))
    {
        return FALSE;
    }
    CString strAppPath = szFileName;

    szFileName[strAppPath.ReverseFind(_T('\\')) + 1] = _T('\0');

    strAppPath = szFileName;
    CString strPath = strAppPath + _T("\\Config\\FingerPrint.xml");
    std::string strPathA = fcT2A(strPath.GetBuffer());

    TiXmlDocument doc;
    if (!doc.LoadFile(strPathA))
    {
        return FALSE;
    }

    TiXmlElement *pRootNode = doc.FirstChildElement();	// root
    if (!pRootNode)
    {
        return FALSE;
    }

    TiXmlElement* pDevice = pRootNode->FirstChildElement("device");
    while (pDevice)
    {
        if (id == atoi(pDevice->Attribute("id")))
        {
            return TRUE;
        }

        // 下一个节点
        pDevice = pDevice->NextSiblingElement();
    }

    return FALSE;
}

unsigned __stdcall LoginFpThread(void * pParam)
{
    int nRet = FP_LOGIN_UNKOWN_FAILDE;
    CFingerMgr* pFingerLogin = (CFingerMgr*)pParam;
    if (NULL == pFingerLogin)
    {
        //pFingerLogin->m_pLoginCallBack(nRet, pFingerLogin->m_pLoginUser);
        return 1;
    }

    STU_FP_INFO stuFpInfo;
    pFingerLogin->GetFingerInfo(stuFpInfo);

    NET_DVR_USER_LOGIN_INFO loginInfo = {0};
    strcpy_s(loginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, stuFpInfo.strIp.c_str());
    loginInfo.wPort = (WORD)atoi(stuFpInfo.strPort.c_str());
    strcpy_s(loginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, stuFpInfo.strUser.c_str());
    strcpy_s(loginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, stuFpInfo.strPwd.c_str());

    NET_DVR_DEVICEINFO_V40 deviceInfo = {0};
    pFingerLogin->m_lFingerLogin= NET_DVR_Login_V40(&loginInfo, &deviceInfo);
    if (-1 == pFingerLogin->m_lFingerLogin)
    {
        nRet = FP_LOGIN_FAILED;
    }
    else
    {
        nRet = FP_LOGIN_SUCCESS;

        if (!IsFpDeviceSupport((int)deviceInfo.struDeviceV30.wDevType))
        {
            NET_DVR_Logout(pFingerLogin->m_lFingerLogin);
            pFingerLogin->m_lFingerLogin = -1;
            nRet = FP_LOGIN_UNSUPPORT;
        }
    }

    pFingerLogin->m_pLoginCallBack(nRet, pFingerLogin->m_pLoginUser);
    return 0;
}

BOOL CALLBACK msgCallBack(LONG lCommand, NET_DVR_ALARMER* pAlarmer,
                          char* pAlarmInfo, DWORD dwBufLen, void* pUser)
{
    CFingerMgr* pMgr = (CFingerMgr*)pUser;
    if (NULL== pMgr)
    {
        return FALSE;
    }

    switch(lCommand)
    {       
    case COMM_ALARM_ACS:
        {
            NET_DVR_ACS_ALARM_INFO acsAlarm = {0};
            memcpy_s(&acsAlarm, sizeof(NET_DVR_ACS_ALARM_INFO), pAlarmInfo, sizeof(NET_DVR_ACS_ALARM_INFO));

            SYSTEMTIME fingerTime;
            fingerTime.wYear = acsAlarm.struTime.dwYear;
            fingerTime.wMonth = acsAlarm.struTime.dwMonth;
            fingerTime.wDay = acsAlarm.struTime.dwDay;
            fingerTime.wHour = acsAlarm.struTime.dwHour;
            fingerTime.wMinute = acsAlarm.struTime.dwMinute;
            fingerTime.wSecond = acsAlarm.struTime.dwSecond;

            CTime ctFinger(fingerTime);

            // 比布防时间早的刷指纹事件，忽略
            if (pMgr->m_ctAlarm > ctFinger)
            {
                break;
            }

            // 刷指纹事件
            if (MAJOR_EVENT == acsAlarm.dwMajor)
            {
                switch (acsAlarm.dwMinor)
                {
                case MINOR_FINGERPRINT_COMPARE_PASS:  // 刷指纹通过事件
                    {
                        // 回调卡号
                        pMgr->m_pAlarmCallBack((char*)acsAlarm.struAcsEventInfo.byCardNo, pMgr->m_pAlarmUser);
                    }
                    break;
                case MINOR_FINGERPRINT_INEXISTENCE:  // 指纹不存在
                    {
                        // 回调NULL
                        pMgr->m_pAlarmCallBack(NULL, pMgr->m_pAlarmUser);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    return TRUE;
}

CFingerMgr::CFingerMgr()
: m_pLoginCallBack(NULL)
, m_pAlarmCallBack(NULL)
, m_lFingerLogin(-1)
, m_lFingerAlarm(-1)
, m_pLoginUser(NULL)
, m_pAlarmUser(NULL)
{
}

CFingerMgr::~CFingerMgr(void)
{
}

void CFingerMgr::Init()
{
    NET_DVR_Init();
}

void CFingerMgr::UnInit()
{
    if (-1 != m_lFingerAlarm)
    {
        NET_DVR_CloseAlarmChan_V30(m_lFingerAlarm);
        m_lFingerAlarm = -1;
    }

    if (-1 != m_lFingerLogin)
    {
        NET_DVR_Logout(m_lFingerLogin);
        m_lFingerLogin = -1;
    }

    NET_DVR_Cleanup();
}

BOOL CFingerMgr::LoginFinger(const STU_FP_INFO& stuFpInfo, FmLoginCallBack loginCallBack, void* pUser)
{
    if (-1 != m_lFingerLogin)
    {
        return FALSE;
    }

    m_stuFpInfo = stuFpInfo;
    m_pLoginCallBack = loginCallBack;
    m_pLoginUser = pUser;

    unsigned threadID = 0;
    HANDLE hHeartLogin = (HANDLE)_beginthreadex(NULL, 0, LoginFpThread, this, 0, &threadID);
    if (NULL == hHeartLogin)
    {
        return FALSE;
    }

    return TRUE;
}

void CFingerMgr::GetFingerInfo(STU_FP_INFO& stuFpInfo)
{
    stuFpInfo = m_stuFpInfo;
}

BOOL CFingerMgr::StartAlarm(CString& strError, FmAlarmCallBack listenCallBack, void* pUser)
{
    if (-1 != m_lFingerAlarm)
    {
        return FALSE;
    }

    m_pAlarmCallBack = listenCallBack;
    m_pAlarmUser = pUser;

    //校时
    CTime ctCurTime = CTime::GetCurrentTime();
    NET_DVR_TIME CurTime;
    CurTime.dwYear = ctCurTime.GetYear();
    CurTime.dwMonth = ctCurTime.GetMonth();
    CurTime.dwDay = ctCurTime.GetDay();
    CurTime.dwHour = ctCurTime.GetHour();
    CurTime.dwMinute = ctCurTime.GetMinute();
    CurTime.dwSecond = ctCurTime.GetSecond();

    m_ctAlarm = ctCurTime;

    if (!NET_DVR_SetDVRConfig(m_lFingerLogin, NET_DVR_SET_TIMECFG, 0, &CurTime, sizeof(NET_DVR_TIME)))
    {
        DWORD dwError = NET_DVR_GetLastError();
        strError.Format(_T("%s : %d"), LOGIN_ERROR_FP_TIME_ERROR, dwError);
        return FALSE;
    }

    NET_DVR_SETUPALARM_PARAM setupParam = {0};
    setupParam.dwSize = sizeof(NET_DVR_SETUPALARM_PARAM);
    setupParam.byAlarmInfoType = 1;
    setupParam.byRetAlarmTypeV40 = 1; //支持可变长 NET_DVR_ALARMINFO_V40
    setupParam.byRetVQDAlarmType = 1;
    setupParam.byRetDevInfoVersion = 1;
    setupParam.byFaceAlarmDetection = 1;

    m_lFingerAlarm = NET_DVR_SetupAlarmChan_V41(m_lFingerLogin, &setupParam);
    if (-1 == m_lFingerAlarm)
    {
        strError = LOGIN_ERROR_FP_ALARM_FAILED;
        return FALSE;
    }

    if (!NET_DVR_SetDVRMessageCallBack_V31(msgCallBack, this))
    {
        strError = LOGIN_ERROR_FP_ALARM_FAILED;
        return FALSE;
    }

    return TRUE;
}

BOOL CFingerMgr::CloseAlarm()
{
    int nRet = FALSE;

    if (-1 != m_lFingerAlarm)
    {
        nRet = NET_DVR_CloseAlarmChan_V30(m_lFingerAlarm);
        m_lFingerAlarm = -1;
    }

    return nRet;
}

BOOL CFingerMgr::Logout()
{
    int nRet = FALSE;
    if (-1 != m_lFingerLogin)
    {
        nRet = NET_DVR_Logout(m_lFingerLogin);
        m_lFingerLogin = -1;
    }

    return nRet;
}
