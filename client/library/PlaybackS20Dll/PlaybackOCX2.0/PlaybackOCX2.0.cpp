// PlaybackOCX3.0.cpp : CPlaybackOCX30App �� DLL ע���ʵ�֡�

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



// CPlaybackOCX30App::InitInstance - DLL ��ʼ��

BOOL CPlaybackOCX30App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CPlaybackOCX30App::ExitInstance - DLL ��ֹ

int CPlaybackOCX30App::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
