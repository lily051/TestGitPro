// IAtlDemoCtrl.cpp : Implementation of CIAtlDemoCtrl

#include "stdafx.h"
#include "IAtlDemoCtrl.h"
//#include <strsafe.h>
#include <sstream>
#include <ShlObj.h>
#include <COMUTIL.H>

#pragma comment(lib, "comsuppw.lib")

/**@fn   <BSTRToStdString>
*@brief  <将BSTR转为std::string>
*@param  <src: 待转换的字符串>
*@return <std::string: 保存转换结果的字符串，空字符串表示失败>
*@note   <封装了BSTRToStdString，简化使用、避免内存泄露>
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
    Fire_MsgNotify(INIT_FINISH, _com_util::ConvertStringToBSTR("初始化完成"));
    return S_OK;
}
