#pragma once

#include "../Login/Login.h"

class CLoginCMS
{
public:
    CLoginCMS(void);
    ~CLoginCMS(void);

    // ��¼ƽ̨
    BOOL DoLogin(LIB_LOGIN_USER_INFO& loginUserInfo);
//     BOOL DoLogin(const CString &strUserName, const CString &strPassword, 
//         const CString &strServerIP, const CString &strLocalIP, 
//         CString &strErrorMsg, const int nServerPort);
    // �������Ȩ������
    //void SortPermissions(std::vector<int>& vecPermission);
    // ��ȡWEB�������Ϣ
    //void GetWebInfo();
    // ͨ�����Ż�ȡ�˺���Ϣ
   // BOOL GetLoginInfoByCardNo(LIB_LOGIN_USER_INFO& loginUserInfo);
};
