/**@file Plugin.cpp
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief npapi框架类实现文件
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
#include "Plugin.h"
#include <windowsx.h>  

#include <string>

//////////////4个function////////////////////////////////////////////////////////////
NPError NS_PluginInitialize()
{
    return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{

}

nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
    if(NULL == aCreateDataStruct)
    {
        return NULL;
    }
    CPlugin * plugin = new(std::nothrow) CPlugin(aCreateDataStruct->instance);
    return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
    if(aPlugin != NULL)
    {
        delete (CPlugin *)aPlugin;
    }
}

////// CPlugin /////////
CPlugin::CPlugin(NPP pNPInstance) : nsPluginInstanceBase(),
m_pNPInstance(pNPInstance)
,m_pScriptableObject(NULL)
,m_bInitialized(FALSE)
,m_hWnd(NULL)
{
    m_npDemo = NULL;
}

CPlugin::~CPlugin()
{
    try
    {
        if (m_npDemo)
        {
            m_npDemo->DestoryControl();
            delete m_npDemo;
            m_npDemo = NULL;
        }
    }
    catch(...)
    {

    }
}

/** @fn     init
 *  @brief  插件初始化
 *  @param  NPWindow* pNPWindow 插件窗口
 *  @return 成功：TRUE 失败：FALSE
 *  @note   在插件窗口创建并加载查询中心控件
 */
NPBool CPlugin::init(NPWindow* pNPWindow)
{
    OutputPrintf("CPlugin::init window:[%ld]", pNPWindow);
    if (NULL == pNPWindow)
    {
        return FALSE;
    }

    m_hWnd = (HWND)pNPWindow->window;
    m_npDemo = new(std::nothrow) NPAtlDemo(this);
    if (NULL == m_npDemo || NULL == m_hWnd)
    {
        return FALSE;
    }

    m_npDemo->SetWindow(pNPWindow);
    if (!m_npDemo->CreateControl(m_hWnd, pNPWindow->width, pNPWindow->height))
    {
        return FALSE;
    }

    OutputPrintf("CPlugin::init ok");
    m_bInitialized = TRUE;
    //Sleep(1000);

    return TRUE;
}

void CPlugin::shut() 
{
    m_hWnd = NULL;  
    m_bInitialized = FALSE;
}

NPBool CPlugin::isInitialized()
{
    return m_bInitialized;
}

NPError CPlugin::SetWindow(NPWindow* Window)  
{
    if (m_npDemo != NULL)
    {
        OutputPrintf("CPlugin::SetWindow window:[%ld]", Window);
        m_npDemo->SetWindow(Window);
        m_npDemo->MoveWindow();
    }
    return NPERR_NO_ERROR;  
}  

/**
 *  @Function:              CPlugin::GetScriptableObject
 *  @Description:           获取脚本对象
 *  @param input:           null
 *  @param output:          null
 *  @return value:          NPObject指针
 *  @note:                  <Func>
**/
NPObject* CPlugin::GetScriptableObject()
{
    if (NULL == m_pScriptableObject)
    {
        m_pScriptableObject = NPN_CreateObject(m_pNPInstance, &objectClass);
    }

    if (m_pScriptableObject)
    {
        NPN_RetainObject(m_pScriptableObject);
    }

    return m_pScriptableObject;
}

//lint -save -e522
//插件的回调函数触发到js页面
/** @fn     CallJavaScript
 *  @brief  np插件向页面回调事件
 *  @param  LPCTSTR funname 回调函数名称    其他为5个参数
 *  @return void
 */
void CPlugin::CallJavaScript(LPCTSTR funname, long lParm1, LPCTSTR szParm3)
{
    NPVariant result;  

    NPObject *sWindowObj;

    NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &sWindowObj);

    NPVariant windowVar;
    NPIdentifier winID = NPN_GetStringIdentifier("window");

    NPN_GetProperty(m_pNPInstance, sWindowObj, winID, &windowVar);

    NPObject  *windowFun = NPVARIANT_TO_OBJECT(windowVar);

    NPIdentifier funcID = NPN_GetStringIdentifier(funname);

    NPVariant relements[2];

    std::string firstr = ConvertGBKToUtf8(std::string(szParm3));
    char* szFir = (char*)NPN_MemAllocEx(firstr);    //分配内存

    USES_CONVERSION;//lint -e830
    INT32_TO_NPVARIANT(lParm1, relements[0]);
    STRINGZ_TO_NPVARIANT(szFir, relements[1]);

    NPN_Invoke(m_pNPInstance, windowFun, funcID, relements, 2, &result);

    NPN_ReleaseObject(sWindowObj);
    NPN_ReleaseVariantValue(&windowVar);
    NPN_ReleaseVariantValue(&result);
}//lint !e438 !e550

////////////////////CPluginObject////////////////////////
CPluginScriptObject::CPluginScriptObject(NPP nppobj)
{
    this->npp = nppobj;
}

CPluginScriptObject::~CPluginScriptObject(void)
{
    this->npp = NULL;
}

void CPluginScriptObject::deallocate()
{

}

void CPluginScriptObject::invalidate()
{

}

/** @fn     hasMethod
 *  @brief  判断是否有该方法
 *  @param  NPIdentifier methodName 函数名称
 *  @return 存在返回true，不存在返回false
 *..@note   与method相关的函数为hasMethod和Invoke，如果要为插件创建一个method，那么必须在hasMethod中为该函数返回true，method的实现代码应在Invoke实现或者由Invoke调用
 */
bool CPluginScriptObject::hasMethod(NPIdentifier methodName)
{
    char *pFunc = NPN_UTF8FromIdentifier(methodName);
    OutputPrintf("hasMethod:%s", pFunc);
    if (strcmp(pFunc, "Test") == 0)
    {
        return true;
    }
    //else if (strcmp(pFunc, "ClearResult") == 0)
    //{
    //    return true;
    //}
    return false;
}

bool CPluginScriptObject::invokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return true;
}

/**
 *  @Function:              CPluginScriptObject::invoke
 *  @Description:           NPAPI接口调用
 *  @param input:           methodName：        方法名
 *                            args:                接口参数列表
 *                            argCount:            接口参数个数
 *  @param output:          result:                接口返回值
 *  @return value:          bool:                true-成功，false-失败
 *  @note:                  <member Func>
**/
bool CPluginScriptObject::invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    //OutputPrintf("invokeinvokeinvoke");
    if (NULL == npp)
    {
        return false;
    }
    CPlugin* pPlugin = (CPlugin*)npp->pdata;
    if (NULL == pPlugin)
    {
//        OutputPrintf("pPlugin is NULL!!! [test]");
        return false;
    }

    char *pFunc = NPN_UTF8FromIdentifier(methodName);
    OutputPrintf("invoke:%s", pFunc);
    long nRet;

    if (strcmp(pFunc, "Test") == 0)
    {
        if(args != NULL && argCount >= 2)
        {
            std::string firstr = NPVARIANT_TO_GBK(args[0]);
            _bstr_t szXml = _com_util::ConvertStringToBSTR(firstr.c_str());
            long nData = NPVARIANT_TO_LONG(args[1]);

            nRet = pPlugin->Test( szXml, nData );
            INT32_TO_NPVARIANT(nRet, *result);
            return true;
        }
        OutputPrintf("%s function argument error", pFunc);
        return false;
    }
    //else if (strcmp(pFunc, "ClearResult") == 0)
    //{
    //    nRet = pPlugin->ClearResult();
    //    INT32_TO_NPVARIANT(nRet, *result);
    //    return true;
    //}

    return false;
}

bool CPluginScriptObject::hasProperty(NPIdentifier propertyName)
{
    return false;
}

bool CPluginScriptObject::getProperty(NPIdentifier propertyName, NPVariant *result)
{
    return false;
}

bool CPluginScriptObject::setProperty(NPIdentifier name,const NPVariant *value)
{
    return false;
}

bool CPluginScriptObject::removeProperty(NPIdentifier name)
{
    return true;
}

bool CPluginScriptObject::enumerate(NPIdentifier **identifier,uint32_t *count)
{
    return false;
}

bool CPluginScriptObject::construct(const NPVariant *args,uint32_t argCount, NPVariant *result)
{
    return true;
}

/////////////////////////

NPObject* CPluginScriptObject::_allocate(NPP nppobj,NPClass* aClass)
{
    return new(std::nothrow) CPluginScriptObject(nppobj);
}

void CPluginScriptObject::_deallocate(NPObject *npobj)
{
    ((CPluginScriptObject*)npobj)->deallocate();
    if(npobj)
    {
        delete npobj;
    }
}

void CPluginScriptObject::_invalidate(NPObject *npobj)
{
    ((CPluginScriptObject*)npobj)->invalidate();
}

bool CPluginScriptObject::_hasMethod(NPObject* obj, NPIdentifier methodName)
{
    return ((CPluginScriptObject*)obj)->hasMethod(methodName);
}

bool CPluginScriptObject::_invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return ((CPluginScriptObject*)obj)->invokeDefault(args,argCount,result);
}

bool CPluginScriptObject::_invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return ((CPluginScriptObject*)obj)->invoke(methodName,args,argCount,result);
}

bool CPluginScriptObject::_hasProperty(NPObject *obj, NPIdentifier propertyName)
{
    return ((CPluginScriptObject*)obj)->hasProperty(propertyName);
}

bool CPluginScriptObject::_getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
    return ((CPluginScriptObject*)obj)->getProperty(propertyName,result);
}

bool CPluginScriptObject::_setProperty(NPObject *npobj, NPIdentifier name,const NPVariant *value)
{
    return ((CPluginScriptObject*)npobj)->setProperty(name,value);
}

bool CPluginScriptObject::_removeProperty(NPObject *npobj, NPIdentifier name)
{
    return ((CPluginScriptObject*)npobj)->removeProperty(name);
}

bool CPluginScriptObject::_enumerate(NPObject *npobj, NPIdentifier **identifier,uint32_t *count)
{
    return ((CPluginScriptObject*)npobj)->enumerate(identifier,count);
}

bool CPluginScriptObject::_construct(NPObject *npobj, const NPVariant *args,uint32_t argCount, NPVariant *result)
{
    return ((CPluginScriptObject*)npobj)->construct(args,argCount,result);
}
