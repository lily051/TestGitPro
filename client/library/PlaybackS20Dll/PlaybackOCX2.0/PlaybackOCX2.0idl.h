

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Nov 30 16:00:07 2016
 */
/* Compiler settings for .\PlaybackOCX20.idl:
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


#ifndef __PlaybackOCX22E0idl_h__
#define __PlaybackOCX22E0idl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DPlaybackOCX30_FWD_DEFINED__
#define ___DPlaybackOCX30_FWD_DEFINED__
typedef interface _DPlaybackOCX30 _DPlaybackOCX30;
#endif 	/* ___DPlaybackOCX30_FWD_DEFINED__ */


#ifndef ___DPlaybackOCX30Events_FWD_DEFINED__
#define ___DPlaybackOCX30Events_FWD_DEFINED__
typedef interface _DPlaybackOCX30Events _DPlaybackOCX30Events;
#endif 	/* ___DPlaybackOCX30Events_FWD_DEFINED__ */


#ifndef __PlaybackOCX30_FWD_DEFINED__
#define __PlaybackOCX30_FWD_DEFINED__

#ifdef __cplusplus
typedef class PlaybackOCX30 PlaybackOCX30;
#else
typedef struct PlaybackOCX30 PlaybackOCX30;
#endif /* __cplusplus */

#endif 	/* __PlaybackOCX30_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PlaybackOCX30Lib_LIBRARY_DEFINED__
#define __PlaybackOCX30Lib_LIBRARY_DEFINED__

/* library PlaybackOCX30Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_PlaybackOCX30Lib;

#ifndef ___DPlaybackOCX30_DISPINTERFACE_DEFINED__
#define ___DPlaybackOCX30_DISPINTERFACE_DEFINED__

/* dispinterface _DPlaybackOCX30 */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPlaybackOCX30;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FAB0C0D8-95B8-4A2C-BA94-69584AC7EC53")
    _DPlaybackOCX30 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPlaybackOCX30Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPlaybackOCX30 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPlaybackOCX30 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPlaybackOCX30 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPlaybackOCX30 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPlaybackOCX30 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPlaybackOCX30 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPlaybackOCX30 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPlaybackOCX30Vtbl;

    interface _DPlaybackOCX30
    {
        CONST_VTBL struct _DPlaybackOCX30Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPlaybackOCX30_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPlaybackOCX30_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPlaybackOCX30_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPlaybackOCX30_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPlaybackOCX30_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPlaybackOCX30_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPlaybackOCX30_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPlaybackOCX30_DISPINTERFACE_DEFINED__ */


#ifndef ___DPlaybackOCX30Events_DISPINTERFACE_DEFINED__
#define ___DPlaybackOCX30Events_DISPINTERFACE_DEFINED__

/* dispinterface _DPlaybackOCX30Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPlaybackOCX30Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("10CC88DC-D85C-402B-840E-8B86118704D6")
    _DPlaybackOCX30Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPlaybackOCX30EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPlaybackOCX30Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPlaybackOCX30Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPlaybackOCX30Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPlaybackOCX30Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPlaybackOCX30Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPlaybackOCX30Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPlaybackOCX30Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPlaybackOCX30EventsVtbl;

    interface _DPlaybackOCX30Events
    {
        CONST_VTBL struct _DPlaybackOCX30EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPlaybackOCX30Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPlaybackOCX30Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPlaybackOCX30Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPlaybackOCX30Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPlaybackOCX30Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPlaybackOCX30Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPlaybackOCX30Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPlaybackOCX30Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PlaybackOCX30;

#ifdef __cplusplus

class DECLSPEC_UUID("07EBE387-935C-45DA-A121-AE19F18CEF44")
PlaybackOCX30;
#endif
#endif /* __PlaybackOCX30Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


