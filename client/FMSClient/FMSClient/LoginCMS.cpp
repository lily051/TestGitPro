#include "StdAfx.h"
#include "LoginCMS.h"
#include "FMSClient.h"
#include "AppDef.h"
//#include "FBrowserMgr.h"
#include "utils/sha256.h"
#include "utils/Encrypt.h"
#include "MultiLang/lClient.h"
#include <verify/ClientVerify.pb.h>
#include <type/ModuleType.pb.h>
#include <Server/ReqServerData.pb.h>
#include "ChangePasswordDlg.h"

int StrToBin(TCHAR* inWord, BYTE* OutBin, int source_len)
{
	int t;
	int t2;
	int count = 0;
	BYTE temBin[2];
	temBin[0] = 1;
	temBin[1] = 1;

	if (source_len < 1)
		return 0;

	for(t = 0 ;t < source_len; t ++)
	{  
		t2 = inWord[t];
		if( t2 > 127 )
		{
			temBin[0] =  t2 >> 8 ;/// 256;
			temBin[1] =  t2;
			OutBin[count] = temBin[0];
			count += 1;
			OutBin[count] = temBin[1];
			count += 1;
		}
		else
		{
			OutBin[count] = t2;
			count += 1;
		}
	}
	return count;
}

CLoginCMS::CLoginCMS(void)
{
}

CLoginCMS::~CLoginCMS(void)
{
}

// ��¼ƽ̨
BOOL CLoginCMS::DoLogin(LIB_LOGIN_USER_INFO& loginUserInfo)
{
    // ���ӷ�����
    SERVER_INFO stServerInfo;
    swprintf_s(stServerInfo.szServerIp, 16, loginUserInfo.szServerIP);
    stServerInfo.nServerPort = loginUserInfo.nServerPort;
    if (!CNetComm::SnglPtr()->ConnectCms(stServerInfo))
    {
        _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_CONNECT_FAIL);
        return FALSE;
    }

//     if (LOGIN_TYPE_FINGER == loginUserInfo.nLoginType)
//     {
//         if (!GetLoginInfoByCardNo(loginUserInfo))
//         {
//             _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_ERROR_FP_GETUSER_ERROR);
//             return FALSE;
//         }
//     }
    theApp.m_nLoginType = loginUserInfo.nLoginType;

    // ��¼
    cms_8120::ReqClientLogin reqClientLogin;
    reqClientLogin.set_cmd(cms_8120::CMD_DT_CLIENT_LOGIN);
    reqClientLogin.set_moudle_type(cms_8120::MT_CLIENT_CS);
    string szUser = fcT2U(loginUserInfo.szUserName);
    string szLocalIp = fcT2U(loginUserInfo.szLocalIP);
    reqClientLogin.set_user_name(szUser);
    reqClientLogin.set_ip(szLocalIp);
    reqClientLogin.set_mac("");
    reqClientLogin.set_dc_ip(fcT2U(stServerInfo.szServerIp));

    // ����MD5����(�ж�32λ����ʱ����Ϊ�Ѿ����ܣ����ٽ��м���);
    // MD5���ܺ󳤶ȶ���32λ
    //if (32 != _tcslen(loginUserInfo.szPassword))
    //{
    //    string strMd5 = CalculateMD5((LPCWSTR)loginUserInfo.szPassword);
    //    if (!strMd5.empty())
    //    {
    //        _tcscpy_s(loginUserInfo.szPassword, MAX_PASSWORD_LEN, fcA2T(strMd5.c_str()));
    //    }
    //    else
    //    {
    //        // ����ǰ���Ͽ����ӣ��´ε�¼ʱ����ֱ�ӿ�ʼ
    //        FMSCLIENT_ERROR("user: %s, CalculateMD5 failed", szUser.c_str());
    //        return FALSE;
    //    }
    //}

    // �˻���¼ʹ��SHA256����, SHA256���ܺ�Ϊ64λ
//     if (64 != _tcslen(loginUserInfo.szPassword) && 
//         LOGIN_TYPE_ACCOUNT == loginUserInfo.nLoginType)
    {
        char strPwdSHA[64] = {0};
        if (NULL == SHA256(fcT2A(loginUserInfo.szPassword),
            _tcslen(loginUserInfo.szPassword), strPwdSHA))
        {
            FMSCLIENT_ERROR("user: %s, SHA256 failed", szUser.c_str());
            return FALSE;
        }

        strlwr(strPwdSHA);
        reqClientLogin.set_user_pwd(fcA2U(strPwdSHA));
    }
//     else
//     {
//         reqClientLogin.set_user_pwd(fcT2U(loginUserInfo.szPassword));
//     }

    CString strAppVer;
    GetVerValue(_T("FileVersion"), strAppVer);
    reqClientLogin.set_module_version(fcT2U(strAppVer));  // ģ��汾��Ϣ

    // �����¼����
    theApp.m_pbmsgReqLogin = reqClientLogin;

	//����ƽ̨��Ϣ
	theApp.m_strServerIp = stServerInfo.szServerIp;
    theApp.m_nServerPort = stServerInfo.nServerPort;

    // ��ʼ��¼ƽ̨
    cms_8120::RspClientLogin rspClientLogin;
    stServerInfo.nServerID = CMS_SERVER;
    string strReq = reqClientLogin.SerializeAsString();
    string strRsp;
    unsigned int nCmdRsp = cms_8120::CMD_DT_CLIENT_LOGIN;
    int nRet = CNetComm::SnglPtr()->TransmitCompntMsgEx(
        strReq
        , cms_8120::CMD_DT_CLIENT_LOGIN
        , REQ_MSG_WAITING_RSP
        , stServerInfo
        , REQ_MSG_WAITING_RSP
        , nCmdRsp
        , strRsp);

    if (CNetComm::TRANSMIT_SUCC != nRet)
    {
        _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_CONNECT_FAIL);
        return FALSE;
    }

    // ������¼����ֵ
    if (!rspClientLogin.ParseFromString(strRsp))
    {
        _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_CONNECT_FAIL);
        return FALSE;
    }

    // ����IP��ַ
    theApp.m_strAppIp = loginUserInfo.szLocalIP;
    // ���淵����Ϣ
    theApp.m_pbmsgRspLogin = rspClientLogin;
	// ������Ϣ
    std::string strDbg = rspClientLogin.DebugString();

	//�����¼�û�id
	theApp.m_nUserId = rspClientLogin.user_id();
	//theApp.m_strAppIp = UTF8_A2MFCSTRW(rspClientLogin.client_ip());

	//��־�ϴ��Ƿ�����
	int nLog = rspClientLogin.logreport();
	//���䷽ʽ
	int nMethod = rspClientLogin.logreportmethod();

    //���ذ汾��
    CString strLocalVer = theApp.m_strFileDescription.Mid(theApp.m_strFileDescription.ReverseFind('\-') + 1);
    int nLocalVer = My_atoi(strLocalVer);
   
    //����CS�ͻ��˰汾��
    CString strVersionNo = UTF8_A2MFCSTRW(rspClientLogin.version_no());
    CString strServerVer = strVersionNo.Mid(strVersionNo.ReverseFind('\-') + 1);
    int nServerVer = My_atoi(strServerVer);
    if (nServerVer > nLocalVer) //���İ汾�ϸߣ���Ҫ����
    {
        //�����ļ����ص�ַ
        CString strUpdateUrl = UTF8_A2MFCSTRW(rspClientLogin.version_url());
        //�Ƿ�ǿ������
        CString strForceUpdate = UTF8_A2MFCSTRW(rspClientLogin.isforceupdate());
        int nForceUpdate = My_atoi(strForceUpdate);
        if (nForceUpdate == 1)
        {
            FMessageBox::Show(_T("�밲װ���¿ͻ��˺����µ�¼"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
            //��ie�����
            HINSTANCE hinst = ShellExecute(NULL, _T("open"), _T("iexplore.exe"), strUpdateUrl, NULL, SW_SHOWMAXIMIZED);
            ExitProcess(EXIT_SUCCESS);
            return FALSE;
        }
        else
        {
           if (IDYES == FMessageBox::Show(_T("�ͻ����и��£��Ƿ�������"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION))
           {
               //��ie�����
               HINSTANCE hinst = ShellExecute(NULL, _T("open"), _T("iexplore.exe"), strUpdateUrl, NULL, SW_SHOWMAXIMIZED);
               ExitProcess(EXIT_SUCCESS);
               return FALSE;
           }           
        }
    }

    // �������ص���Ϣ
    if (rspClientLogin.result() != 0)
    {
		if (rspClientLogin.is_inital_pwd() == 1) //1-��ʼ���룻0-���ǳ�ʼ����
		{
			FMessageBox::Show(_T("���ε�¼�����޸���������µ�¼��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);

			CString strUrl = theApp.GernateUrl();
			if (strUrl.IsEmpty())
			{
				FMessageBox::Show(_T("�������Ĳ�����"), _T("����"), MB_OK | MB_ICONERROR);
				return FALSE;
			}

			OpenUrl(strUrl);
			CString strErrorInfo = _T("�����µ�¼");
			_tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, strErrorInfo);
			return FALSE;
// 			CChangePasswordDlg dlg;
// 			if (IDOK == dlg.DoModal())
// 			{
// 				CString strErrorInfo = _T("�����µ�¼");
// 				_tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, strErrorInfo);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				CString strErrorInfo = _T("�޸�����ʧ��");
// 				_tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, strErrorInfo);
// 				return FALSE;
// 			}
		}
		string strResultInfo = rspClientLogin.result_info();
		CString strErrorInfo = UTF8_A2MFCSTRW(strResultInfo);
		_tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, strErrorInfo);
		return FALSE;
    }

    // ���뱣��ʱ����
    if (rspClientLogin.user_pwd_check().is_pwdcheck_enabled() && 
        rspClientLogin.user_pwd_check().remaining_days() <= 0)
    {
        _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_RESULT_PWDEXPIRE);
        return FALSE;
    }

    // �������뱣��ʣ������
    //theApp.m_pbMsgPwdCheck.set_remaining_days(rspClientLogin.user_pwd_check().remaining_days());

    // ��ȡ����ǽ���ƿͻ�����������
//     int nTvCompntId = MENUID_TVMS;
//     for (int i = 0; i < rspClientLogin.ui_cs_right_size(); ++i)
//     {
//         int nRightClient = rspClientLogin.ui_cs_right(i);
//         if (cms_8100::RIGHT_UI_CS_VIDEO_UPLOAD == nRightClient)
//         {
//             nTvCompntId = MENUID_TVWALL;
//         }
//         else if (cms_8100::RIGHT_UI_CS_TVMS == nRightClient)
//         {
//             nTvCompntId = MENUID_TVMS;
//         }
//     }

    // production_version 0.0.0.2 ˵���ǻ���20130818�����ݷ���
    // production_version 0.0.0.1 ˵���ǻ���20130608�����ݷ��񣬸ð汾
    // ��is_omni_operatorΪ1ʱui_cs_right_size����Ȩ�ޣ������⴦��
    // ��ȡȨ��
//     theApp.m_vecWndPermissions.clear();
//     if (rspClientLogin.is_omni_operator() == 1 //1��ʾϵͳĬ�Ͻ�ɫ����ز���Ա��ϵͳ����Ա
//         && rspClientLogin.production_version() == "0.0.0.1")
//     {
//         theApp.m_vecWndPermissions.push_back(MENUID_PREVIEW);
//         theApp.m_vecWndPermissions.push_back(nTvCompntId);
//         theApp.m_vecWndPermissions.push_back(MENUID_PLAYBACK);
//         theApp.m_vecWndPermissions.push_back(MENUID_ALARMCENTER);
//         theApp.m_vecWndPermissions.push_back(MENUID_EMAP);
//         theApp.m_vecWndPermissions.push_back(MENUID_STATUSCONTROL);
//         theApp.m_vecWndPermissions.push_back(MENUID_INM);
//         theApp.m_vecWndPermissions.push_back(MENUID_LOG);
//         theApp.m_vecWndPermissions.push_back(MENUID_COFFER);
//         theApp.m_vecWndPermissions.push_back(MENUID_VIEWWALL);
//         theApp.m_vecWndPermissions.push_back(MENUID_VIEWTALK);
//     }
//     else // 0731 ֮��İ汾�������ﴦ��
//     {
//         for (int i = 0; i < rspClientLogin.ui_cs_right_size(); i++)
//         {
//             int nRightClient = rspClientLogin.ui_cs_right(i);
// 
//             // ����ǽ���
//             if (cms_8100::RIGHT_UI_CS_VIDEO_UPLOAD == nRightClient ||
//                 cms_8100::RIGHT_UI_CS_TVMS == nRightClient)
//             {
//                 theApp.m_vecWndPermissions.push_back(nTvCompntId);
//                 continue;
//             }
// 
//             //���˯
//             if (cms_8100::RIGHT_OP_AVOID_DOZING == nRightClient)
//             {
//                 theApp.m_bAvoidDozing = TRUE;
//                 continue;
//             }
// 
//             int nCompntId = theApp.CompntIdFromProtoDef(nRightClient);
//             if (nCompntId != 0)
//             {
//                 theApp.m_vecWndPermissions.push_back(nCompntId);
//             }
//             else
//             {
//                 CLIENT_DEBUG("unrecognised protodef right-module:%d", nRightClient);
//             }
//         }
// 
//         // ���FBrowser���
//         for (int id = MENUID_FBROWSER_MIN; id < MENUID_FBROWSER_MAX; id++)
//         {
//             if (FRAMECOM == FBrowser_GetTypeById(id))
//             {
//                 theApp.m_vecWndPermissions.push_back(id);
//             }
//         }
// 
//         // ������Ƶ��ǽ����Ϊ2�����û�з��أ�
//         SortPermissions(theApp.m_vecWndPermissions);
//     }
//     theApp.m_vecWndPermissions.push_back(MENUID_CONFIG);

    // ����һЩ�Զ�����Ϣ��ƽ̨���⡢��������
    CString strIniFile = theApp.GetAppPath() + _T("\\") + CLIENT_CONFIG_FILENAME;

    // ����
    if (!rspClientLogin.custom_title().empty())
    {
        theApp.m_sCustomTitle = UTF8_A2MFCSTRW(rspClientLogin.custom_title());
        BOOL bRet = WritePrivateProfileString(_T("FMSClient"),
            _T("CustomTitle"), theApp.m_sCustomTitle, strIniFile);
        if (!bRet)
        {
            DWORD dwError = GetLastError();
            FMSCLIENT_ERROR("Update Custom Title Config failed[%u]", dwError);
        }
    }

    // ��Ȩ
    if (rspClientLogin.has_copyright())
    {
        theApp.m_sCopyright = UTF8_A2MFCSTRW(rspClientLogin.copyright());
        if (theApp.m_sCopyright.IsEmpty())
        {
            theApp.m_sCopyright = APP_COPYRIGHT;
        }
        BOOL bRet = WritePrivateProfileString(_T("FMSClient"),
            _T("Copyright"), theApp.m_sCopyright, strIniFile);
        if (!bRet)
        {
            DWORD dwError = GetLastError();
            FMSCLIENT_ERROR("Update Copyright Config failed[%u]", dwError);
        }
    }

    // ����ƽ̨��������
    theApp.m_nLanguage = rspClientLogin.language();

    CString strLangeType;
    strLangeType.Format(_T("%d"), theApp.m_nLanguage);
    WritePrivateProfileString(_T("FMSClient"),
        _T("Language"), strLangeType, strIniFile);

    // ��������ð棬��ô�ڱ�������ʾ��׺
    theApp.m_sMainTitle = theApp.m_sCustomTitle;
    if (rspClientLogin.authorization() == 2)
    {
        // ���ð�
        CString strPath;
        strPath = theApp.GetAppPath() + _T("\\TryVersion.dll");
        BOOL bTry = ::GetPrivateProfileInt(_T("VEREXCHANGE"), _T("TryFlag"), 1, strPath);
        if (bTry)
        {
            theApp.m_sMainTitle.Append(TITLEBAR_TITLE_EXT);
        }
    }

// 	CString strCapturePath = theApp.m_strAppPath + _T("\\RecordFiles\\");
// 	WritePrivateProfileString(_T("LocalConfig"), _T("CapPath"), strCapturePath, strIniFile);

    // Ƥ����ɫ,����Ƥ����ɫֵ�뱾�����ò�ͬʱ�����¼���Ƥ����Դ
    // �����±������á�
//     if (theApp.m_nSkinType != rspClientLogin.skin_color())
//     {
//         if (-1 == rspClientLogin.skin_color())
//         {
//             CLIENT_WARN("RspClientLogin no skin_color");
//             theApp.m_nSkinType = 0;
//         }
//         else
//         {
//             theApp.m_nSkinType = rspClientLogin.skin_color();
//         }
// 
//         if (!theApp.SetSkinScheme(theApp.m_nSkinType))
//         {
//             FMessageBox::Show(LOGIN_LOADSKIN_FAIL, MSGBOX_ERROR, MB_ICONERROR | MB_OK);
//             return FALSE;
//         }
// 
//         // ����Ƥ������
//         CString strSkinScheme;
//         strSkinScheme.Format(_T("%d"), theApp.m_nSkinType);
//         BOOL bRet = WritePrivateProfileString(_T("CentralClient"), _T("SkinScheme"), (LPCTSTR)strSkinScheme, strIniFile);
//         if (!bRet)
//         {
//             DWORD dwError = GetLastError();
//             FMSCLIENT_ERROR("Update Skin Color Config failed[%u]", dwError);
//         }
//     }

    // ��ȡWEB�������Ϣ
    //GetWebInfo();

    // �Ƿ�Ϊ��ʼ���룬��ʾ�û�
//     if (rspClientLogin.has_is_inital_pwd() && rspClientLogin.is_inital_pwd())
//     {
//         if(IDOK == FMessageBox::Show(PWD_CHANGE_TIP, MSGBOX_TIPS, MB_ICONQUESTION | MB_OK))
//         {
//            // CString strUrl = theApp.GernateUrl();
// 			 CString strUrl = _T("");
//             if (strUrl.IsEmpty())
//             {
//                 FMessageBox::Show(ERROR_WEB_NOEXSIST, MSGBOX_ERROR, MB_ICONERROR | MB_OK);
// 
//                 _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_RESULT_PWDCHANGE);
//                 return FALSE;
//             }
//             CString strCmd = strUrl + _T("&type=4");
//             OpenUrl(strCmd);
// 
//             _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_RESULT_PWDCHANGE);
//             return FALSE;
//         }
//         else
//         {
//             _tcscpy_s(loginUserInfo.szErrorMsg, MAX_ERROR_LEN, LOGIN_RESULT_PWDCHANGE);
//             return FALSE;
//         }
//     }

    // ���汾��IP��ַ����tvmcʹ��
    CString strTvmcIni = theApp.GetAppPath() + _T("\\tvmc\\temp.ini");
    WritePrivateProfileString(_T("tvmc"), _T("local_ip"), theApp.m_strAppIp, strTvmcIni);

	theApp.m_strSalesmanName = UTF8_A2MFCSTRW(rspClientLogin.strname());
	theApp.m_strSalesmanCode = UTF8_A2MFCSTRW(rspClientLogin.user_no());
	theApp.m_strRegionName = UTF8_A2MFCSTRW(rspClientLogin.net());

// 	CString cmdParm;
// 	cmdParm.Format(_T("FMSClientName=FMSClient UploadClientName=FSvcUpload CheckURL=http://%s:80/download/UploadClient/UpdateConfig.ini Silence=1"), theApp.m_strServerIp);
// 	ShellExecute(NULL,_T("open"),_T("Updater.exe"), (LPCWSTR)cmdParm,NULL,SW_SHOWNORMAL);

    return TRUE;
}

// BOOL CLoginCMS::DoLogin(const CString &strUserName, 
//                         const CString &strPassword, 
//                         const CString &strServerIP,
//                         const CString &strLocalIP, 
//                         CString &strErrorMsg, 
//                         const int nServerPort)
// {
//     LIB_LOGIN_USER_INFO loginUserInfo;
//     _tcscpy_s(loginUserInfo.szServerIP, MAX_IP_LEN, strServerIP);
//     _tcscpy_s(loginUserInfo.szLocalIP, MAX_IP_LEN, strLocalIP);
//     _tcscpy_s(loginUserInfo.szUserName, MAX_USERNAME_LEN, strUserName);
//     _tcscpy_s(loginUserInfo.szPassword, MAX_PASSWORD_LEN, strPassword);
//     loginUserInfo.nServerPort = nServerPort;
// 
//     return DoLogin(loginUserInfo);
// }

/*
void CLoginCMS::SortPermissions(std::vector<int>& vecPermission)
{
    std::vector<int> vecTemp;

    // preview
    for (size_t i = 0; i < vecPermission.size(); ++i)
    {
        if (MENUID_PREVIEW == vecPermission[i])
        {
            vecTemp.push_back(MENUID_PREVIEW);
            break;
        }
    }

    // rvwall
    for (size_t i = 0; i < vecPermission.size(); ++i)
    {
        if (MENUID_TVWALL == vecPermission[i] ||
            MENUID_TVMS == vecPermission[i])
        {
            vecTemp.push_back(vecPermission[i]);
            break;
        }
    }

    // other
    for (size_t i = 0; i < vecPermission.size(); ++i)
    {
        if (MENUID_PREVIEW != vecPermission[i] &&
            MENUID_TVWALL != vecPermission[i] &&
            MENUID_TVMS != vecPermission[i])
        {
            vecTemp.push_back(vecPermission[i]);
        }
    }

    vecPermission = vecTemp;
}
*/

/*
void CLoginCMS::GetWebInfo()
{
    using namespace cms_8100;
    // ������Ϣ
    ReqServerData reqServerData;
    reqServerData.set_cmd(CMD_REQ_SERVER_DATA);
    reqServerData.set_data_type(CMD_DT_SERVER_LIST);
    reqServerData.set_moudle_type(MT_CLIENT_LOG);
    reqServerData.set_sub_type(41234);  // ��ȡweb������Ϣ�ƶ�������

    // ��������Ϣ
    SERVER_INFO stServerInfo;
    stServerInfo.nServerID = CMS_SERVER;

    // ����
    unsigned int uCmdRsp = CMD_DT_SERVER_LIST;
    std::string strReq, strRsp;
    strReq = reqServerData.SerializeAsString();
    int nRet = CNetComm::SnglPtr()->TransmitCompntMsgEx(
        strReq
        , cms_8100::CMD_REQ_SERVER_DATA
        , REQ_MSG_WAITING_RSP
        , stServerInfo
        , REQ_MSG_WAITING_RSP
        , uCmdRsp
        , strRsp);

    if (CNetComm::TRANSMIT_SUCC != nRet)
    {
        FMSCLIENT_ERROR("Get Web Ip Fail");
        return ;
    }

    // ������¼����ֵ
    ServerList serverList;
    if (!serverList.ParseFromString(strRsp))
    {
        FMSCLIENT_ERROR("Parse Rsp(Web Ip) Fail");
        return ;
    }

    for (int i = 0; i < serverList.server_size(); i++)
    {
        const Server& server = serverList.server(i);

        theApp.m_strWebIp = fcU2T(server.ip().c_str());
        theApp.m_nWebPort = server.ctrl_port();
    }
}
*/

/*
BOOL CLoginCMS::GetLoginInfoByCardNo(LIB_LOGIN_USER_INFO& loginUserInfo)
{
    cms_8100::UserList userList;  // �û���Ϣ�б�

    // ��������������Ϣ
    cms_8100::ReqServerData reqMsg;
    reqMsg.set_cmd(cms_8100::CMD_REQ_SERVER_DATA);
    reqMsg.set_data_type(cms_8100::CMD_DT_USER_LIST);
    reqMsg.set_moudle_type(cms_8100::MT_CLIENT);
    reqMsg.set_server_ip(fcT2U(loginUserInfo.szLocalIP));
    reqMsg.set_dc_ip(fcT2U(loginUserInfo.szServerIP));
    reqMsg.set_login_name(fcT2U(loginUserInfo.szCardNo));
    reqMsg.set_sub_type(1);
    reqMsg.set_task_id(1); // 1-ʹ�ÿ��Ż�ȡ�˺���Ϣ

    SERVER_INFO serverInfo;
    _tcscpy_s(serverInfo.szServerIp, 16, loginUserInfo.szServerIP);
    serverInfo.nServerPort = loginUserInfo.nServerPort;
    serverInfo.nServerID = CMS_SERVER;

    // �������󣬽��շ��ص���Ϣ
    string sReq = reqMsg.SerializeAsString();
    string sRsp;
    unsigned int nCmdRsp = cms_8100::CMD_DT_USER_LIST;
    int nRet = CNetComm::SnglPtr()->TransmitCompntMsgEx(
        sReq
        , cms_8100::CMD_REQ_SERVER_DATA
        , REQ_MSG_WAITING_RSP
        , serverInfo
        , REQ_MSG_WAITING_RSP
        , nCmdRsp
        , sRsp);
    if (CNetComm::TRANSMIT_SUCC != nRet)
    {
        FMSCLIENT_ERROR("TransmitCompntMsgEx [%d] failed", nRet);
        return FALSE;
    }

    // �������ص����ݣ���ȡ��¼���û���Ϣ
    if (!userList.ParseFromString(sRsp))
    {
        FMSCLIENT_ERROR("TransmitCompntMsgEx ParseFromString failed");
        return FALSE;
    }

    if (userList.user_size() < 1)
    {
        FMSCLIENT_ERROR("There is not User Info");
        return FALSE;
    }

    cms_8100::User pbMsgUserInfo = userList.user(0);

    _tcscpy_s(loginUserInfo.szUserName, MAX_USERNAME_LEN, fcU2T(pbMsgUserInfo.login_name().c_str()));
    _tcscpy_s(loginUserInfo.szPassword, MAX_PASSWORD_LEN, fcU2T(pbMsgUserInfo.login_pwd().c_str()));
    return TRUE;
}
*/