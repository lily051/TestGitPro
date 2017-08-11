// PreviewS20OCX.cpp : CPreviewS20OCXApp 和 DLL 注册的实现。
//lint -library
#include "stdafx.h"
#include "PreviewS20OCX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPreviewS20OCXApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xDE56021, 0xBC11, 0x4491, { 0xBA, 0x24, 0xEA, 0xA, 0xDA, 0xBE, 0xD3, 0x34 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CPreviewS20OCXApp::InitInstance - DLL 初始化

BOOL CPreviewS20OCXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CPreviewS20OCXApp::ExitInstance - DLL 终止

int CPreviewS20OCXApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
