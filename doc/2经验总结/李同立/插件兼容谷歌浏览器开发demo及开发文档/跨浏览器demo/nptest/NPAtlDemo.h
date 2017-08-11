/**@file NPAtlDemo.h
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief ��ѯ���Ŀؼ��ӿڷ�װ��ͷ�ļ�
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
#pragma once
#include "pluginbase.h"

//#include "..\..\public\GlobalFun.h"       //�������ҪCSinkObj�࣬SinkObj.h�����ÿɸ���GlobalFun.h
#include "SinkObj.h"
#include "atldemo.tlh"


#define _ATL_DLL_IMPL
using namespace ATL;

#pragma comment(lib, "atl.lib")

class CPlugin;

/** @class NPAtlDemo [NPAtlDemo.h]
 *  @brief ��װ�˲�ѯ���Ŀؼ������нӿڣ���CPlugin����á�
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

    CSinkObj *m_pSVIIEvents;            //�ص�����������
    CPlugin    *m_pPlugin;

    CComPtr<IIAtlDemoCtrl> m_pIDemo; //��ѯ���Ŀؼ�COMָ��

public:
    //�ؼ��ӿ�
    long Test( _bstr_t strXml, long nData )
    {
        if (NULL != m_pIDemo)
        {
            return m_pIDemo->Test(strXml, nData);
        }
        return 0;
    }
};
