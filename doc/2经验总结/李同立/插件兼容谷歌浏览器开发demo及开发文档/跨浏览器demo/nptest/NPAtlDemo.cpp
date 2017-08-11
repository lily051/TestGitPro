/**@file NPAtlDemo.cpp
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief ��ѯ���Ŀؼ��ӿڷ�װ��ʵ���ļ�
*
* @author   lichanghua
* @date      2015/7/27
*
* @note �����note��warningΪ��ѡ��Ŀ
* @note ������д���ļ�����ϸ����������ע��
* @note ��ʷ��¼��
* @note V1.0.0  ���� 2015/7/27
*
* @warning ������д���ļ���صľ�����Ϣ
*/
#include "NPAtlDemo.h"
/////////////////////////////////////////////////////////////////////////////
//
CComModule _Module;
#define IDC_MYCTL 0x1024

NPAtlDemo::NPAtlDemo(CPlugin *pPlugin)
{
    m_Window = NULL;
    m_pPlugin = pPlugin;
    m_pSVIIEvents = NULL;
}

NPAtlDemo::NPAtlDemo(void)
{
    m_Window = NULL;
    m_pPlugin = NULL;
    m_pSVIIEvents = NULL;
}

NPAtlDemo::~NPAtlDemo(void)
{
    try
    {
        m_pPlugin = NULL;
        m_Window = NULL;
        if (m_pSVIIEvents)    
        {
            if (m_pSVIIEvents->m_dwEventCookie != 0xFEFEFEFE)
            {
                m_pSVIIEvents->DispEventUnadvise(m_pIDemo, &m_pSVIIEvents->m_iid);
            }
            delete m_pSVIIEvents;

            m_pSVIIEvents = NULL;
        }
    }
    catch(...)
    {

    }
}

/** @fn     CreateControl
 *  @brief  ATL�������ڼ���ATL COM
 *  @param  HWND hParent �����
            int nWidth ���
            int nHeight �߶�
 *  @return �ɹ���TRUE ʧ�ܣ�FALSE
 */
BOOL NPAtlDemo::CreateControl(HWND hParent, int nWidth, int nHeight)
{
    CoInitialize(NULL);
 //   _Module.Init(NULL,NULL);
    AtlAxWinInit();

    RECT rc = {0, 0, nWidth, nHeight};
    char szMsg[255] = {0};

    try
    {
        HWND hWnd = m_wnd.Create(hParent, rc, "AtlDemo.IAtlDemoCtrl.1", WS_CHILD | WS_VISIBLE, 0, IDC_MYCTL);
        if (NULL == hWnd)
        {
            return FALSE;
        }
        HRESULT hr;
        hr = m_wnd.QueryControl(__uuidof(IIAtlDemoCtrl),(void**)&m_pIDemo);

        if (FAILED(hr) || NULL == m_pIDemo)
        {
            CoUninitialize();
            return FALSE;
        }
        m_wnd.SetRedraw();
        m_wnd.ShowWindow(SW_SHOW);

        //CSinkObj�������տؼ��ص��¼������û�лص��¼���AtlGetObjectSourceInterface������쳣����ɾ����CSinkObj�йص����´���
        m_pSVIIEvents = new(std::nothrow) CSinkObj();
        if (NULL == m_pSVIIEvents)
        {
            OutputPrintf("Fail to Create CSinkObj");
            ::MessageBox(NULL, "Error", "Fail to Create CSinkObj",MB_OK);
            CoUninitialize();
            return FALSE;
        }
        hr = AtlGetObjectSourceInterface(m_pIDemo, &m_pSVIIEvents->m_libid, 
            &m_pSVIIEvents->m_iid, &m_pSVIIEvents->m_wMajorVerNum, &m_pSVIIEvents->m_wMinorVerNum);
        if (FAILED(hr))
        {
            sprintf_s(szMsg, strlen(szMsg), "CreateControl AtlGetObjectSourceInterface Fail: %d", GetLastError() );
            OutputPrintf(szMsg);
            ::MessageBox(NULL, "Error", szMsg,MB_OK);
            CoUninitialize();
            return FALSE;
        }

        m_pSVIIEvents->m_pPlugin = m_pPlugin;

        //Ϊ�¼�Դ��������
        hr = m_pSVIIEvents->DispEventAdvise(m_pIDemo,&m_pSVIIEvents->m_iid);
        if (FAILED(hr))
        {
            sprintf_s(szMsg, strlen(szMsg), "CreateControl DispEventAdvise Fail: %d", GetLastError() );
            OutputPrintf(szMsg);
            ::MessageBox(NULL, "Error", szMsg,MB_OK);
            CoUninitialize();
            return FALSE;
        }
    }
    catch( ... )
    {
        sprintf_s(szMsg, strlen(szMsg), "CreateControl:%lu", GetLastError() );
        OutputPrintf(szMsg);
        ::MessageBox(NULL, "Exception", szMsg,MB_OK);
        CoUninitialize();
        return FALSE;
    }
    
    CoUninitialize();
    return TRUE;
}

/** @fn     MoveWindow
 *  @brief  ������ڴ�С�ı�ʱ�ƶ�ATL COMλ��
 *  @param  void
 *  @return void
 */
void NPAtlDemo::MoveWindow()
{
    if (m_Window != NULL)
    {
        m_wnd.SetRedraw();

        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = m_Window->width;
        rc.bottom = m_Window->height;
        //m_wnd.MoveWindow(0, 0, m_Window->width, m_Window->height);
        m_wnd.SetWindowPos((HWND)m_Window->window, &rc, SWP_SHOWWINDOW|SWP_NOZORDER); 
        m_wnd.FlashWindow(TRUE);
        m_wnd.ShowWindow(SW_SHOW);
        OutputPrintf("NPAtlDemo MoveWindow:(%d,%d)", m_Window->width, m_Window->height);
    }
}

void NPAtlDemo::DestoryControl()
{
    try
    {
        m_wnd.DestroyWindow();
//      _Module.Term();
    }
    catch(...)
    {

    }
}