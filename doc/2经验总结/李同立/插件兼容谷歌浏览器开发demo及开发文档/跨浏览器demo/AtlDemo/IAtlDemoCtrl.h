// IAtlDemoCtrl.h : Declaration of the CIAtlDemoCtrl

#pragma once
#include "resource.h"       // main symbols

#include <atlctl.h>
#include <comutil.h>
#include <wchar.h>
#include "AtlDemo_i.h"
#include "_IIAtlDemoCtrlEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define INIT_FINISH     1001

// CIAtlDemoCtrl

class ATL_NO_VTABLE CIAtlDemoCtrl :
	public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CIAtlDemoCtrl, &CLSID_IAtlDemoCtrl>,
    public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CIAtlDemoCtrl>,
    public CProxy_IIAtlDemoCtrlEvents<CIAtlDemoCtrl>,
    public IObjectWithSiteImpl<CIAtlDemoCtrl>,
    public IProvideClassInfo2Impl<&CLSID_IAtlDemoCtrl, &__uuidof(_IIAtlDemoCtrlEvents), &LIBID_AtlDemoLib>,
    public IObjectSafetyImpl<CIAtlDemoCtrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER| INTERFACESAFE_FOR_UNTRUSTED_DATA>,			//safe
    public IDispatchImpl<IIAtlDemoCtrl, &IID_IIAtlDemoCtrl, &LIBID_AtlDemoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CIAtlDemoCtrl()
	{
        OutputDebugString("初始化完成-ATL");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IATLDEMOCTRL)


BEGIN_COM_MAP(CIAtlDemoCtrl)
	COM_INTERFACE_ENTRY(IIAtlDemoCtrl)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IObjectSafety)			//safe
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CIAtlDemoCtrl)
	CONNECTION_POINT_ENTRY(__uuidof(_IIAtlDemoCtrlEvents))
END_CONNECTION_POINT_MAP()


    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

    STDMETHOD(Test)(BSTR strXml, LONG nData, LONG* lReturn);
};

OBJECT_ENTRY_AUTO(__uuidof(IAtlDemoCtrl), CIAtlDemoCtrl)
