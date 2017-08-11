// PlaybackOCX3.0.cpp : CPlaybackOCX30App 和 DLL 注册的实现。

#include "stdafx.h"
#include "PlaybackOCX2.0.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPlaybackOCX30App theApp;

#ifndef _FORCLIENT
const GUID CDECL BASED_CODE _tlid =
		{ 0x92F662D8, 0xC422, 0x4A20, { 0x9A, 0xA8, 0xC9, 0x1E, 0x97, 0xB8, 0xB9, 0x6F } };
#else
const GUID CDECL BASED_CODE _tlid =
		{ 0x22F1202D, 0xA9F2, 0x4ADE, { 0xBB, 0x59, 0xF, 0xEF, 0x5A, 0x58, 0x89, 0xFF } };
#endif

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CPlaybackOCX30App::InitInstance - DLL 初始化

BOOL CPlaybackOCX30App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CPlaybackOCX30App::ExitInstance - DLL 终止

int CPlaybackOCX30App::ExitInstance()
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
