// RPConfig.cpp : CRPConfigApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "RPConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRPConfigApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x96768222, 0xFEB3, 0x4CE4, { 0x9B, 0x8E, 0x51, 0x5B, 0xB1, 0xDF, 0x1D, 0x3E } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CRPConfigApp::InitInstance - DLL ��ʼ��

BOOL CRPConfigApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
        AfxEnableControlContainer();
	}

	return bInit;
}



// CRPConfigApp::ExitInstance - DLL ��ֹ

int CRPConfigApp::ExitInstance()
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
