// UploadService.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

#include <time.h>
#include "UploadService.h"
#include <fclib/fcNetComm.h>
#include <fclib/fcTchar.h>
#include <HPR_Hpr.h>
//#include "SQLiteHelper.h"
#include "ServiceFactory.h"
#include "Config.h"
#include "Msger.h"
#include "type/ModuleType.pb.h"
#include "../util/StringConverter.h"

static pTransMsgBack g_pMsgBack = NULL;
static HANDLE g_hServiceThread = NULL;
static HANDLE g_hEvent = NULL;

static const int UploadSvc_ERROR_BASIC = -1;
//TCHAR g_szTransportDB[] = _T("transport.db");

/**	@fn	    InitWorkPath
*	@brief	���ù���·��
*	@param  [in] ��
*	@param  [out] ��
*	@return	
*/
void InitWorkPath()
{
    TCHAR path[256] = {0};
    GetModuleFileName(NULL, path, 256);
    for (int i = (int)_tcslen(path); i >= 0; --i)
    {
        if (path[i] == _T('\\'))
        {
            path[i]=0;
            break;
        }
    }
    SetCurrentDirectory(path);
}

DWORD WINAPI ServiceThread(LPVOID /*lpPrame*/)
{
    ULSERVER_LOG_INFO("ServiceThread..........IN");

    int nStartResult = HPR_ERROR;
    bool bWaitOK = false;

    do 
    {
        //��������
        nStartResult = CServiceFactory::GetSingletonPtr()->Start();

        if (HPR_OK == nStartResult)
        {
            break;
        }

        bWaitOK = ::WaitForSingleObject(g_hEvent, 5000) == WAIT_OBJECT_0;
        if (bWaitOK)
        {
            ULSERVER_LOG_INFO("Get Exit Signal......");
            break;
        }
    } while (HPR_OK != nStartResult);

    ULSERVER_LOG_INFO("ServiceThread............OUT");
    return 0;
}

// This is an example of an exported function.
UPLOADSERVICE_API int WINAPI UploadSvc_Init(void)
{
    //���ù���·��
    InitWorkPath();
    //��ʼ��
    CServiceFactory::GetSingletonPtr()->Init();
    return UploadSvc_NO_ERROR;  
}

UPLOADSERVICE_API int WINAPI UploadSvc_Fini(void)
{
    if (g_hServiceThread)
    {
        UploadSvc_Stop();
    }

    //�ͷ�
    CServiceFactory::GetSingletonPtr()->Fini();
    CServiceFactory::FreeSingleton();
    return UploadSvc_NO_ERROR;
}

UPLOADSERVICE_API int WINAPI UploadSvc_Start(void)
{
    if (!g_hServiceThread)
    {
        g_hEvent = ::CreateEvent(NULL,FALSE,FALSE,NULL);
        if (NULL == g_hEvent)
        {
            return -1;
        }
        g_hServiceThread = ::CreateThread(NULL, 0, ServiceThread, NULL, 0, NULL);
    }    

    ULSERVER_LOG_INFO("UploadSvc_Start........IN");
    Msger_NotifyMsg(ALL_USER,_T("�ϴ���������"));
    return UploadSvc_NO_ERROR;
}

UPLOADSERVICE_API int WINAPI UploadSvc_Stop(void)
{
    if (g_hServiceThread)
    {
        ::SetEvent(g_hEvent);
        ::WaitForSingleObject(g_hServiceThread, INFINITE);
        ::CloseHandle(g_hServiceThread);
        g_hServiceThread = NULL;

        if (NULL != g_hEvent)
        {
            ::CloseHandle(g_hEvent);
            g_hEvent = NULL;
        }

        CServiceFactory::GetSingletonPtr()->Stop();
    }
    ULSERVER_LOG_INFO("UploadSvc_Start........OUT");
    Msger_NotifyMsg(ALL_USER,_T("�ϴ�����ֹͣ"));
    return UploadSvc_NO_ERROR;
}

UPLOADSERVICE_API DWORD WINAPI UploadSvc_GetVersion(void)
{
    return UploadSvc_NO_ERROR;
}

UPLOADSERVICE_API int WINAPI UploadSvc_GetLastError(void)
{
    return UploadSvc_NO_ERROR;
}

UPLOADSERVICE_API int WINAPI UploadSvc_SetMsgCallback(const pTransMsgBack msgfun)
{
    if (NULL == msgfun)
    {
        return -1;
    }
    g_pMsgBack = msgfun;
    return UploadSvc_NO_ERROR;
}

/** @fn     _gGetCurTimeStr
* @brief ��ȡ��ǰʱ����ַ���
* @param  [in] ��
* @param  [out] ��
* @return ʱ���ַ����� ��ʽ��%Y%m%d %H:%M:%S--
*/
std::string _gGetCurTimeStr()
{ 
    time_t t = time(NULL); 
    char buff[128]; 
    strftime(buff, sizeof(buff), "%Y%m%d %H:%M:%S--", localtime(&t));
    return std::string(buff); 
}

//ע�����ΪUTF-8����
void _gShowMsgStr(const std::string  & sMsg_)
{
    if (NULL == g_pMsgBack)
    {
        ULSERVER_LOG_INFO("g_pMsgBack == NULL");
        return;
    }

    BASIC_MESSAGE_EX msgEx;
    //��Ҫ���ݾ���ķ����������޸�����Ĳ���
    msgEx.dwSvcType = cms_8120::MT_SERVER_TRANSPORT;
    msgEx.dwMsgType = MSG_TYPE_NOTIFY;
    std::string sMsg = CSCT::Ansi2Utf8(_gGetCurTimeStr())+ sMsg_;
    strcpy_s(msgEx.chContent, SVCMSG_CONTENT_LEN, sMsg.c_str());
    strcpy_s(msgEx.chResult, SVCMSG_RESULT_LEN, "");
    g_pMsgBack(&msgEx);
}

//void main()
//{
//    UploadSvc_Init();
//    UploadSvc_Start();
//    while(1)
//    {
//        Sleep(1000);
//        if(0)
//        {
//            UploadSvc_Stop();
//        }
//
//    }
//}
