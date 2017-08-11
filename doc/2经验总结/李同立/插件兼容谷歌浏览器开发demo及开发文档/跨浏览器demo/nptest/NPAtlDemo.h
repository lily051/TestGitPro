/**@file NPAtlDemo.h
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief 查询中心控件接口封装类头文件
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
#pragma once
#include "pluginbase.h"

//#include "..\..\public\GlobalFun.h"       //如果不需要CSinkObj类，SinkObj.h的引用可改用GlobalFun.h
#include "SinkObj.h"
#include "atldemo.tlh"


#define _ATL_DLL_IMPL
using namespace ATL;

#pragma comment(lib, "atl.lib")

class CPlugin;

/** @class NPAtlDemo [NPAtlDemo.h]
 *  @brief 封装了查询中心控件的所有接口，供CPlugin类调用。
 *
 *  
 */
class NPAtlDemo
{
public:
    NPAtlDemo(void);
    NPAtlDemo(CPlugin *pPlugin);
    ~NPAtlDemo(void);

    BOOL CreateControl(HWND hParent, int nWidth, int nHeight);
    void DestoryControl();
    void SetWindow(NPWindow* Window)
    {
        m_Window = Window;
    }
    void MoveWindow();

private:
    ATL::CAxWindow m_wnd;
    NPWindow* m_Window;

    CSinkObj *m_pSVIIEvents;            //回调函数处理类
    CPlugin    *m_pPlugin;

    CComPtr<IIAtlDemoCtrl> m_pIDemo; //查询中心控件COM指针

public:
    //控件接口
    long Test( _bstr_t strXml, long nData )
    {
        if (NULL != m_pIDemo)
        {
            return m_pIDemo->Test(strXml, nData);
        }
        return 0;
    }
};
