// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LOGIN_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LOGIN_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef LOGIN_EXPORTS
#define LOGIN_API EXTERN_C __declspec(dllexport)
#else
#define LOGIN_API EXTERN_C __declspec(dllimport)
#endif

#ifndef ___LOGIN_HHH___
#define ___LOGIN_HHH___

#include "HCNetSDK.h"

#define MAX_USERNAME_LEN    51
#define MAX_PASSWORD_LEN    512
#define MAX_IP_LEN          16
#define MAX_ERROR_LEN       32

enum _e_login_type
{
    LOGIN_TYPE_ACCOUNT = 0,
    LOGIN_TYPE_FINGER  = 1,
};

typedef struct _lib_login_user_info
{
    int nLoginType;                     // 登录类型：0-账号登录，1-指纹登录
    TCHAR szCardNo[ACS_CARD_NO_LEN];    // 指纹对应的卡号
    TCHAR szUserName[MAX_USERNAME_LEN]; // 登录用户名
    TCHAR szPassword[MAX_PASSWORD_LEN]; // 登录密码
    TCHAR szServerIP[MAX_IP_LEN];       // 登录服务IP
    TCHAR szLocalIP[MAX_IP_LEN];        // 本机IP地址
    TCHAR szErrorMsg[MAX_ERROR_LEN];    // 错误信息
    int   nServerPort;                  // 登录服务端口

    _lib_login_user_info()
    {
        nLoginType = LOGIN_TYPE_ACCOUNT;
        szCardNo[0] = _T('\0');
        szUserName[0] = _T('\0');
        szPassword[0] = _T('\0');
        szServerIP[0] = _T('\0');
        szLocalIP[0]  = _T('\0');
        szErrorMsg[0] = _T('\0');
        nServerPort = 8849;
    }
}LIB_LOGIN_USER_INFO;

#endif


/**
* @brief	登录按钮单击事件，由上层进行处理
* @param	loginUserInfo 用户输入的登录参数
* @param	pUser 用户自定义参数，ShowLoginDlg调用是的传入参数
* @return	登录成功返回TRUE，登录失败返回FALSE
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
typedef BOOL (CALLBACK * _LoginEvent)(LIB_LOGIN_USER_INFO& loginUserInfo, void *pUser);

/**
* @brief	显示登录对话框
* @param	pfunLoginEvent 回调函数，用户点击登录界面上的登录按钮后，会自动调用
* @param	pUser 用户自定义参数，回调函数中会传出
* @param	nSkin 显示的皮肤类型0-红黑，1-蓝白
* @param	nUseAutoLogin 是否使用配置的自动登录
* @param	nLanguage 显示的语言类型 0-中文， 1-英文
* @return	DoModel() 的返回值
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
LOGIN_API INT_PTR ShowLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
                               int nUseAutoLogin, int nSkin, int nLanguage);
