

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Dec 16 15:13:56 2016
 */
/* Compiler settings for .\PreviewS20OCX.idl:
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


#ifndef __PreviewS20OCXidl_h__
#define __PreviewS20OCXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DPreviewS20OCX_FWD_DEFINED__
#define ___DPreviewS20OCX_FWD_DEFINED__
typedef interface _DPreviewS20OCX _DPreviewS20OCX;
#endif 	/* ___DPreviewS20OCX_FWD_DEFINED__ */


#ifndef ___DPreviewS20OCXEvents_FWD_DEFINED__
#define ___DPreviewS20OCXEvents_FWD_DEFINED__
typedef interface _DPreviewS20OCXEvents _DPreviewS20OCXEvents;
#endif 	/* ___DPreviewS20OCXEvents_FWD_DEFINED__ */


#ifndef __PreviewS20OCX_FWD_DEFINED__
#define __PreviewS20OCX_FWD_DEFINED__

#ifdef __cplusplus
typedef class PreviewS20OCX PreviewS20OCX;
#else
typedef struct PreviewS20OCX PreviewS20OCX;
#endif /* __cplusplus */

#endif 	/* __PreviewS20OCX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PreviewS20OCXLib_LIBRARY_DEFINED__
#define __PreviewS20OCXLib_LIBRARY_DEFINED__

/* library PreviewS20OCXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_PreviewS20OCXLib;

#ifndef ___DPreviewS20OCX_DISPINTERFACE_DEFINED__
#define ___DPreviewS20OCX_DISPINTERFACE_DEFINED__

/* dispinterface _DPreviewS20OCX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPreviewS20OCX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A25F1DD7-D47C-466C-A075-5EE2CCC534AE")
    _DPreviewS20OCX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPreviewS20OCXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPreviewS20OCX * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPreviewS20OCX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPreviewS20OCX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPreviewS20OCX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPreviewS20OCX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPreviewS20OCX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPreviewS20OCX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPreviewS20OCXVtbl;

    interface _DPreviewS20OCX
    {
        CONST_VTBL struct _DPreviewS20OCXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPreviewS20OCX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPreviewS20OCX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPreviewS20OCX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPreviewS20OCX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPreviewS20OCX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPreviewS20OCX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPreviewS20OCX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPreviewS20OCX_DISPINTERFACE_DEFINED__ */


#ifndef ___DPreviewS20OCXEvents_DISPINTERFACE_DEFINED__
#define ___DPreviewS20OCXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DPreviewS20OCXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPreviewS20OCXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("46292E59-6C27-4A45-9B1A-2E96B8E10659")
    _DPreviewS20OCXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPreviewS20OCXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPreviewS20OCXEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPreviewS20OCXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPreviewS20OCXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPreviewS20OCXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPreviewS20OCXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPreviewS20OCXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPreviewS20OCXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPreviewS20OCXEventsVtbl;

    interface _DPreviewS20OCXEvents
    {
        CONST_VTBL struct _DPreviewS20OCXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPreviewS20OCXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPreviewS20OCXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPreviewS20OCXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPreviewS20OCXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPreviewS20OCXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPreviewS20OCXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPreviewS20OCXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPreviewS20OCXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PreviewS20OCX;

#ifdef __cplusplus

class DECLSPEC_UUID("04655FF8-9979-4FFA-8F77-C3A539EA7370")
PreviewS20OCX;
#endif
#endif /* __PreviewS20OCXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


