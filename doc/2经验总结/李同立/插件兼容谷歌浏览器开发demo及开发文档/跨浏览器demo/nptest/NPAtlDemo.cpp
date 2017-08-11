/**@file NPAtlDemo.cpp
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief 查询中心控件接口封装类实现文件
*
* @author   lichanghua
* @date      2015/7/27
*
* @note 下面的note和warning为可选项目
* @note 这里填写本文件的详细功能描述和注解
* @note 历史记录：
* @note V1.0.0  创建 2015/7/27
*
* @warning 这里填写本文件相关的警告信息
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
 *  @brief  ATL容器窗口加载ATL COM
 *  @param  HWND hParent 父句柄
            int nWidth 宽度
            int nHeight 高度
 *  @return 成功：TRUE 失败：FALSE
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

        //CSinkObj用来接收控件回调事件，如果没有回调事件，AtlGetObjectSourceInterface会出现异常，请删除跟CSinkObj有关的以下代码
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

        //为事件源建立连接
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
 *  @brief  插件窗口大小改变时移动ATL COM位置
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