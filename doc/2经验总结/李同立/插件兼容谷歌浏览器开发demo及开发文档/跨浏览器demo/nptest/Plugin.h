/**@file Plugin.h
* @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
* @brief npapi框架类头文件
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
#include "NPAtlDemo.h"

class CPlugin : public nsPluginInstanceBase
{
private:
    NPAtlDemo *m_npDemo;//查询中心控件对象指针

    NPP     m_pNPInstance;
    NPBool  m_bInitialized;
    HWND    m_hWnd;                 //插件窗口句柄
    NPObject* m_pScriptableObject;  //脚本对象

public:
    CPlugin(NPP pNPInstance);
    ~CPlugin(void);

    NPBool init(NPWindow* pNPWindow);
    void shut();
    NPBool isInitialized();
    NPError SetWindow(NPWindow* Window);
    //获取脚本对象
    NPObject* GetScriptableObject();

    //回调函数处理
    void CallJavaScript(LPCTSTR funname, long lParm1, LPCTSTR szParm3);

public:
    //控件的接口
    long Test( _bstr_t strXml, long nData )
    {
        if (NULL != m_npDemo)
        {
            return m_npDemo->Test(strXml, nData);
        }
        return 0;
    }
};

class CPluginScriptObject : public NPObject
{
public:
    CPluginScriptObject(NPP);
    ~CPluginScriptObject(void);//lint !e1510

    void deallocate();
    void invalidate();
    bool hasMethod(NPIdentifier methodName);
    bool invokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
    void test();
    //NPAPI接口调用
    bool invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);

    bool hasProperty(NPIdentifier propertyName);
    bool getProperty(NPIdentifier propertyName, NPVariant *result);
    bool setProperty(NPIdentifier name, const NPVariant *value);
    bool removeProperty(NPIdentifier name);
    bool enumerate(NPIdentifier **identifier, uint32_t *count);
    bool construct(const NPVariant *args, uint32_t argCount, NPVariant *result);

public:
    static NPObject* _allocate(NPP npp, NPClass* aClass);
    static void _deallocate(NPObject *npobj);
    static void _invalidate(NPObject *npobj);
    static bool _hasMethod(NPObject* obj, NPIdentifier methodName);
    static bool _invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
    static bool _invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
    static bool _hasProperty(NPObject *obj, NPIdentifier propertyName);
    static bool _getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result);
    static bool _setProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
    static bool _removeProperty(NPObject *npobj, NPIdentifier name);
    static bool _enumerate(NPObject *npobj, NPIdentifier **identifier, uint32_t *count);
    static bool _construct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

private:
    NPP npp;
};

#ifndef __object_class
#define __object_class
static NPClass objectClass = //lint -e528
{
    NP_CLASS_STRUCT_VERSION,
    CPluginScriptObject::_allocate,
    CPluginScriptObject::_deallocate,
    CPluginScriptObject::_invalidate,
    CPluginScriptObject::_hasMethod,
    CPluginScriptObject::_invoke,
    CPluginScriptObject::_invokeDefault,
    CPluginScriptObject::_hasProperty,
    CPluginScriptObject::_getProperty,
    CPluginScriptObject::_setProperty,
    CPluginScriptObject::_removeProperty,
    CPluginScriptObject::_enumerate,
    CPluginScriptObject::_construct
};
#endif
