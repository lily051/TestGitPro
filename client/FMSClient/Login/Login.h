// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LOGIN_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LOGIN_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
    int nLoginType;                     // ��¼���ͣ�0-�˺ŵ�¼��1-ָ�Ƶ�¼
    TCHAR szCardNo[ACS_CARD_NO_LEN];    // ָ�ƶ�Ӧ�Ŀ���
    TCHAR szUserName[MAX_USERNAME_LEN]; // ��¼�û���
    TCHAR szPassword[MAX_PASSWORD_LEN]; // ��¼����
    TCHAR szServerIP[MAX_IP_LEN];       // ��¼����IP
    TCHAR szLocalIP[MAX_IP_LEN];        // ����IP��ַ
    TCHAR szErrorMsg[MAX_ERROR_LEN];    // ������Ϣ
    int   nServerPort;                  // ��¼����˿�

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
* @brief	��¼��ť�����¼������ϲ���д���
* @param	loginUserInfo �û�����ĵ�¼����
* @param	pUser �û��Զ��������ShowLoginDlg�����ǵĴ������
* @return	��¼�ɹ�����TRUE����¼ʧ�ܷ���FALSE
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
typedef BOOL (CALLBACK * _LoginEvent)(LIB_LOGIN_USER_INFO& loginUserInfo, void *pUser);

/**
* @brief	��ʾ��¼�Ի���
* @param	pfunLoginEvent �ص��������û������¼�����ϵĵ�¼��ť�󣬻��Զ�����
* @param	pUser �û��Զ���������ص������лᴫ��
* @param	nSkin ��ʾ��Ƥ������0-��ڣ�1-����
* @param	nUseAutoLogin �Ƿ�ʹ�����õ��Զ���¼
* @param	nLanguage ��ʾ���������� 0-���ģ� 1-Ӣ��
* @return	DoModel() �ķ���ֵ
* @author	jddd
* @version	v0.1
* @date		2015/07/30
* @warning	
*/
LOGIN_API INT_PTR ShowLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
                               int nUseAutoLogin, int nSkin, int nLanguage);
