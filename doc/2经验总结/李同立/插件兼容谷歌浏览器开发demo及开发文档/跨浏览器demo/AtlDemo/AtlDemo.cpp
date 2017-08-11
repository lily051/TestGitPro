// AtlDemo.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "AtlDemo_i.h"
#include "dllmain.h"
#include   <comcat.h> 
#include   <Objsafe.h>

//安全机制处理
const   GUID   CDECL   CLSID_SafeItem  =
{0x8F55A11E,0x35B9,0x4B5F,0x98,0xBD,0x53,0x50,0x2C,0x15,0x90,0xDD};//这个需要根据你的控件的classid来修改
const   CATID   CATID_SafeForScripting1 =
{0x7dd95801, 0x9882, 0x11cf, {0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};   
const   CATID   CATID_SafeForInitializing1 =
{0x7dd95802, 0x9882, 0x11cf, {0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};   

HRESULT   CreateComponentCategory(CATID catid, WCHAR* catDescription)   
{
    ICatRegister* pcr = NULL;
    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;

    //   Make   sure   the   HKCR\Component   Categories\{..catid...}   
    //   key   is   registered.   
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409;   //   english

    //   Make   sure   the   provided   description   is   not   too   long.   
    //   Only   copy   the   first   127   characters   if   it   is.   
    int len = wcslen(catDescription);   
    if (len > 127)
        len = 127;   
    wcsncpy(catinfo.szDescription,  catDescription,   len);
    //   Make   sure   the   description   is   null   terminated.   
    catinfo.szDescription[len] = '\0';   

    hr = pcr->RegisterCategories(1, &catinfo); 
    pcr->Release();

    return hr;
}   

// 注册组件种类   
HRESULT   RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)   
{
    //   Register   your   component   categories   information.   
    ICatRegister* pcr = NULL;
    HRESULT hr = S_OK;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
        //   Register   this   category   as   being   "implemented"   by   the   class.
        CATID rgcatid[1];
        rgcatid[0] = catid;
        hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}

// 卸载组件种类   
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)   
{
    ICatRegister* pcr = NULL;
    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
        //   Unregister   this   category   as   being   "implemented"   by   the   class.   
        CATID rgcatid[1];
        rgcatid[0] = catid;
        hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
    }

    if (pcr != NULL)
        pcr->Release();

    return hr;
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();

    //   标记控件初始化安全
    //   创建初始化安全组件种类
    hr = CreateComponentCategory(CATID_SafeForInitializing1,   L"Controls   safely   initializable   from   persistent   data!");
    if (FAILED(hr))
        return hr;
    //   注册初始化安全
    hr = RegisterCLSIDInCategory(CLSID_SafeItem,   CATID_SafeForInitializing1);
    if (FAILED(hr))
        return hr;

    //   标记控件脚本安全
    //   创建脚本安全组件种类
    hr = CreateComponentCategory(CATID_SafeForScripting1,   L"Controls   safely   scriptable!");
    if (FAILED(hr))
        return hr;
    //   注册脚本安全组件种类
    hr = RegisterCLSIDInCategory(CLSID_SafeItem,   CATID_SafeForScripting1);
    if (FAILED(hr))
        return hr;

	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();

    //   删除控件初始化安全入口.   
    hr = UnRegisterCLSIDInCategory(CLSID_SafeItem,   CATID_SafeForInitializing1);
    if (FAILED(hr))
        return hr;
    //   删除控件脚本安全入口
    hr = UnRegisterCLSIDInCategory(CLSID_SafeItem,   CATID_SafeForScripting1);
    if (FAILED(hr))
        return hr;

	return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.	
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = L"user";

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}


