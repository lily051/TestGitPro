#pragma once

#include "../Login/Login.h"

class CLoginCMS
{
public:
    CLoginCMS(void);
    ~CLoginCMS(void);

    // 登录平台
    BOOL DoLogin(LIB_LOGIN_USER_INFO& loginUserInfo);
//     BOOL DoLogin(const CString &strUserName, const CString &strPassword, 
//         const CString &strServerIP, const CString &strLocalIP, 
//         CString &strErrorMsg, const int nServerPort);
    // 组件根据权限排序
    //void SortPermissions(std::vector<int>& vecPermission);
    // 获取WEB服务的信息
    //void GetWebInfo();
    // 通过卡号获取账号信息
   // BOOL GetLoginInfoByCardNo(LIB_LOGIN_USER_INFO& loginUserInfo);
};
