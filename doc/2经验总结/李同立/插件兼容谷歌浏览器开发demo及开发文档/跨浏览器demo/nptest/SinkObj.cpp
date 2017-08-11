/**@file SinkObj.cpp
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief 预览控件处理事件处理类实现文件
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
#include "SinkObj.h"
#include "Plugin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DETAIL_MSG_LEN  (1024*1024)

//////////////////////////////////////////////////////////////////////
CSinkObj::CSinkObj()
{
    m_pPlugin = NULL;
    m_pszDetailMsg = NULL;
}

CSinkObj::~CSinkObj()
{
    m_pPlugin = NULL;
    if (m_pszDetailMsg != NULL)
    {
        delete[] m_pszDetailMsg;
        m_pszDetailMsg = NULL;
    }
}

//add by lch 2016-07-29
//捕获特殊场景NP插件回调异常的情况
HRESULT CSinkObj::CallJS(long lMsgId, const char* szDetail)
{
    __try
    {
        if (m_pPlugin != NULL)
        {
            OutputPrintf("CSinkObj CallJS111: %s,%d,%d,%s,%d", "MsgNotify", lMsgId, szDetail);
            m_pPlugin->CallJavaScript("MsgNotify", lMsgId, szDetail);
            OutputPrintf("CSinkObj CallJS222: %s,%d,%d,%s,%d", "MsgNotify", lMsgId, szDetail);
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        OutputPrintf("CSinkObj CallJS exception, err = %x",GetExceptionCode());
    }

    return S_OK;
}


HRESULT __stdcall CSinkObj::MsgNotify( long lMsgId, _bstr_t szDetail )
{
    try
    {
        if (NULL == m_pszDetailMsg)
        {
            m_pszDetailMsg = new (std::nothrow) char[DETAIL_MSG_LEN];
            if (NULL == m_pszDetailMsg)
            {
                return S_FALSE;
            }
        }

        char szCode[128] = {0};
        memset(m_pszDetailMsg, 0, DETAIL_MSG_LEN);
        wsprintf(m_pszDetailMsg, "%S", szDetail);//lint !e437

        return CallJS(lMsgId, m_pszDetailMsg);
    }
    catch(...)
    {
        OutputPrintf("CSinkObj MsgNotify exception");
        //MessageBox(NULL, "exception", "exception", MB_OK);
    }

    return S_OK;
}
