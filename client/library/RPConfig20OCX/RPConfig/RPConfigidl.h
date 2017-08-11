

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Nov 02 19:10:34 2016
 */
/* Compiler settings for .\RPConfig.idl:
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


#ifndef __RPConfigidl_h__
#define __RPConfigidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DRPConfig_FWD_DEFINED__
#define ___DRPConfig_FWD_DEFINED__
typedef interface _DRPConfig _DRPConfig;
#endif 	/* ___DRPConfig_FWD_DEFINED__ */


#ifndef ___DRPConfigEvents_FWD_DEFINED__
#define ___DRPConfigEvents_FWD_DEFINED__
typedef interface _DRPConfigEvents _DRPConfigEvents;
#endif 	/* ___DRPConfigEvents_FWD_DEFINED__ */


#ifndef __RPConfig_FWD_DEFINED__
#define __RPConfig_FWD_DEFINED__

#ifdef __cplusplus
typedef class RPConfig RPConfig;
#else
typedef struct RPConfig RPConfig;
#endif /* __cplusplus */

#endif 	/* __RPConfig_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __RPConfigLib_LIBRARY_DEFINED__
#define __RPConfigLib_LIBRARY_DEFINED__

/* library RPConfigLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_RPConfigLib;

#ifndef ___DRPConfig_DISPINTERFACE_DEFINED__
#define ___DRPConfig_DISPINTERFACE_DEFINED__

/* dispinterface _DRPConfig */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DRPConfig;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("21AE9E06-DAE1-43B8-96C9-2227AE342D60")
    _DRPConfig : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DRPConfigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DRPConfig * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DRPConfig * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DRPConfig * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DRPConfig * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DRPConfig * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DRPConfig * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DRPConfig * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DRPConfigVtbl;

    interface _DRPConfig
    {
        CONST_VTBL struct _DRPConfigVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DRPConfig_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DRPConfig_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DRPConfig_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DRPConfig_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DRPConfig_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DRPConfig_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DRPConfig_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DRPConfig_DISPINTERFACE_DEFINED__ */


#ifndef ___DRPConfigEvents_DISPINTERFACE_DEFINED__
#define ___DRPConfigEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DRPConfigEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DRPConfigEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0428F28E-5E9D-4DA2-A91B-092341897868")
    _DRPConfigEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DRPConfigEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DRPConfigEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DRPConfigEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DRPConfigEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DRPConfigEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DRPConfigEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DRPConfigEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DRPConfigEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DRPConfigEventsVtbl;

    interface _DRPConfigEvents
    {
        CONST_VTBL struct _DRPConfigEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DRPConfigEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DRPConfigEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DRPConfigEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DRPConfigEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DRPConfigEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DRPConfigEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DRPConfigEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DRPConfigEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_RPConfig;

#ifdef __cplusplus

class DECLSPEC_UUID("A4660BF5-7356-4A3B-8A34-5859ED6C4F2F")
RPConfig;
#endif
#endif /* __RPConfigLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


