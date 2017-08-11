

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Jul 02 12:57:04 2016
 */
/* Compiler settings for .\AtlDemo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AtlDemo_i_h__
#define __AtlDemo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IIAtlDemoCtrl_FWD_DEFINED__
#define __IIAtlDemoCtrl_FWD_DEFINED__
typedef interface IIAtlDemoCtrl IIAtlDemoCtrl;
#endif 	/* __IIAtlDemoCtrl_FWD_DEFINED__ */


#ifndef ___IIAtlDemoCtrlEvents_FWD_DEFINED__
#define ___IIAtlDemoCtrlEvents_FWD_DEFINED__
typedef interface _IIAtlDemoCtrlEvents _IIAtlDemoCtrlEvents;
#endif 	/* ___IIAtlDemoCtrlEvents_FWD_DEFINED__ */


#ifndef __IAtlDemoCtrl_FWD_DEFINED__
#define __IAtlDemoCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class IAtlDemoCtrl IAtlDemoCtrl;
#else
typedef struct IAtlDemoCtrl IAtlDemoCtrl;
#endif /* __cplusplus */

#endif 	/* __IAtlDemoCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IIAtlDemoCtrl_INTERFACE_DEFINED__
#define __IIAtlDemoCtrl_INTERFACE_DEFINED__

/* interface IIAtlDemoCtrl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IIAtlDemoCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("127943FC-60A0-4480-82F2-CA78C9C03874")
    IIAtlDemoCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test( 
            /* [in] */ BSTR strXml,
            /* [in] */ LONG nData,
            /* [retval][out] */ LONG *lReturn) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIAtlDemoCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIAtlDemoCtrl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIAtlDemoCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIAtlDemoCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIAtlDemoCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIAtlDemoCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIAtlDemoCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIAtlDemoCtrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Test )( 
            IIAtlDemoCtrl * This,
            /* [in] */ BSTR strXml,
            /* [in] */ LONG nData,
            /* [retval][out] */ LONG *lReturn);
        
        END_INTERFACE
    } IIAtlDemoCtrlVtbl;

    interface IIAtlDemoCtrl
    {
        CONST_VTBL struct IIAtlDemoCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIAtlDemoCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIAtlDemoCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIAtlDemoCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIAtlDemoCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIAtlDemoCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIAtlDemoCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIAtlDemoCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IIAtlDemoCtrl_Test(This,strXml,nData,lReturn)	\
    ( (This)->lpVtbl -> Test(This,strXml,nData,lReturn) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIAtlDemoCtrl_INTERFACE_DEFINED__ */



#ifndef __AtlDemoLib_LIBRARY_DEFINED__
#define __AtlDemoLib_LIBRARY_DEFINED__

/* library AtlDemoLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AtlDemoLib;

#ifndef ___IIAtlDemoCtrlEvents_DISPINTERFACE_DEFINED__
#define ___IIAtlDemoCtrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IIAtlDemoCtrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IIAtlDemoCtrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("ECB057BA-98E9-494B-B11B-70B2AC8214EB")
    _IIAtlDemoCtrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IIAtlDemoCtrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IIAtlDemoCtrlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IIAtlDemoCtrlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IIAtlDemoCtrlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IIAtlDemoCtrlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IIAtlDemoCtrlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IIAtlDemoCtrlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IIAtlDemoCtrlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IIAtlDemoCtrlEventsVtbl;

    interface _IIAtlDemoCtrlEvents
    {
        CONST_VTBL struct _IIAtlDemoCtrlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IIAtlDemoCtrlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IIAtlDemoCtrlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IIAtlDemoCtrlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IIAtlDemoCtrlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IIAtlDemoCtrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IIAtlDemoCtrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IIAtlDemoCtrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IIAtlDemoCtrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IAtlDemoCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("8F55A11E-35B9-4B5F-98BD-53502C1590DD")
IAtlDemoCtrl;
#endif
#endif /* __AtlDemoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


