

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_PreviewS20OCXLib,0x0DE56021,0xBC11,0x4491,0xBA,0x24,0xEA,0x0A,0xDA,0xBE,0xD3,0x34);


MIDL_DEFINE_GUID(IID, DIID__DPreviewS20OCX,0xA25F1DD7,0xD47C,0x466C,0xA0,0x75,0x5E,0xE2,0xCC,0xC5,0x34,0xAE);


MIDL_DEFINE_GUID(IID, DIID__DPreviewS20OCXEvents,0x46292E59,0x6C27,0x4A45,0x9B,0x1A,0x2E,0x96,0xB8,0xE1,0x06,0x59);


MIDL_DEFINE_GUID(CLSID, CLSID_PreviewS20OCX,0x04655FF8,0x9979,0x4FFA,0x8F,0x77,0xC3,0xA5,0x39,0xEA,0x73,0x70);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



