// PreviewS20OCX.cpp : CPreviewS20OCXApp �� DLL ע���ʵ�֡�
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


// CPreviewS20OCXApp::InitInstance - DLL ��ʼ��

BOOL CPreviewS20OCXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CPreviewS20OCXApp::ExitInstance - DLL ��ֹ

int CPreviewS20OCXApp::ExitInstance()
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
