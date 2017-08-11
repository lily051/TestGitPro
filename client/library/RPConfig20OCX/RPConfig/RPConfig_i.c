

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, LIBID_RPConfigLib,0x96768222,0xFEB3,0x4CE4,0x9B,0x8E,0x51,0x5B,0xB1,0xDF,0x1D,0x3E);


MIDL_DEFINE_GUID(IID, DIID__DRPConfig,0x21AE9E06,0xDAE1,0x43B8,0x96,0xC9,0x22,0x27,0xAE,0x34,0x2D,0x60);


MIDL_DEFINE_GUID(IID, DIID__DRPConfigEvents,0x0428F28E,0x5E9D,0x4DA2,0xA9,0x1B,0x09,0x23,0x41,0x89,0x78,0x68);


MIDL_DEFINE_GUID(CLSID, CLSID_RPConfig,0xA4660BF5,0x7356,0x4A3B,0x8A,0x34,0x58,0x59,0xED,0x6C,0x4F,0x2F);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



