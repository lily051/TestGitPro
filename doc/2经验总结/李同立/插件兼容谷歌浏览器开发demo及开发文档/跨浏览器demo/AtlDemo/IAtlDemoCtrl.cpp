// IAtlDemoCtrl.cpp : Implementation of CIAtlDemoCtrl

#include "stdafx.h"
#include "IAtlDemoCtrl.h"
//#include <strsafe.h>
#include <sstream>
#include <ShlObj.h>
#include <COMUTIL.H>

#pragma comment(lib, "comsuppw.lib")

/**@fn   <BSTRToStdString>
*@brief  <��BSTRתΪstd::string>
*@param  <src: ��ת�����ַ���>
*@return <std::string: ����ת��������ַ��������ַ�����ʾʧ��>
*@note   <��װ��BSTRToStdString����ʹ�á������ڴ�й¶>
*/
std::string BSTRToStdString(BSTR src)
{
    std::string dst;
    if (src != NULL)
    {
        char* pText = _com_util::ConvertBSTRToString(src);
        if (pText != NULL)
        {
            dst = pText;
            delete[] pText;
        }
    }
    return dst;
}

// CIAtlDemoCtrl


STDMETHODIMP CIAtlDemoCtrl::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_IIAtlDemoCtrl
    };

    for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}


STDMETHODIMP CIAtlDemoCtrl::Test(BSTR strXml, LONG nData, LONG* lReturn)
{
    // TODO: Add your implementation code here
    if(strXml == NULL)
    {
        *lReturn = -1;
        return S_FALSE;
    }

    std::string szXml = BSTRToStdString(strXml);
    OutputPrintf("%s", szXml.c_str());
    *lReturn = nData;
    Fire_MsgNotify(INIT_FINISH, _com_util::ConvertStringToBSTR("��ʼ�����"));
    return S_OK;
}
