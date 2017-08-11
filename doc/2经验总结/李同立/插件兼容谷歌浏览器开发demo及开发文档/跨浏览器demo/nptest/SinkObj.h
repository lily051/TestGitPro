/**@file SinkObj.h
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief 预览控件处理事件处理类头文件
*
* @author   lichanghua
* @date      2015/4/25
*
* @note 下面的note和warning为可选项目
* @note 这里填写本文件的详细功能描述和注解
* @note 历史记录：
* @note V1.0.0  创建 2015/4/25
*
* @warning 这里填写本文件相关的警告信息
*/
#pragma once

//lint -save -e1510

#include "..\..\public\GlobalFun.h"
#include "resource.h"

class CPlugin;

#define IDC_SINKOBJ    0x102

//继承自ATL所自带响应Event的类IDispEventImpl
class CSinkObj : public IDispEventImpl<IDC_SINKOBJ,CSinkObj>  
{
public:
    CSinkObj();
    virtual ~CSinkObj();

    //注：SINK_ENTRY(IDC_SINKOBJ,1,MsgNotify)中的"1"跟回调函数MsgNotify在控件的函数ID要保持一致 //add by lch 2016-07-29
    /*
    ATL控件的 *.ild文件中id(1)
    dispinterface _IIAtlDemoCtrlEvents
    {
    properties:
    methods:
    [id(1), helpstring("method MsgNotify")] HRESULT MsgNotify([in] LONG lMsgId, [in] BSTR szDetail);
    };
    
    ActiveX控件的 *.ild文件中id(2)
    dispinterface _DSunScanEvents
    {
    properties:
    methods:
    [id(2)] void callbackEvent(BSTR paraXML);
    };
    或
    *Ctrl.h文件中2L
    enum {
    dispidxx=1L,
    eventidxxEvent = 2L
    };
    */
BEGIN_SINK_MAP(CSinkObj)
    SINK_ENTRY(IDC_SINKOBJ,1,MsgNotify)
END_SINK_MAP()

    //回调函数
    HRESULT __stdcall MsgNotify( long lMsgId, _bstr_t szDetail);

    HRESULT CallJS(long lMsgId, const char* szDetail);

    CPlugin    *m_pPlugin;  //用于调用CPlugin类的回调函数
    char       *m_pszDetailMsg;
};
