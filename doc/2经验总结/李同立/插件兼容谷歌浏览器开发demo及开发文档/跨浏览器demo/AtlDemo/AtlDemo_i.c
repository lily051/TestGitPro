

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IIAtlDemoCtrl,0x127943FC,0x60A0,0x4480,0x82,0xF2,0xCA,0x78,0xC9,0xC0,0x38,0x74);


MIDL_DEFINE_GUID(IID, LIBID_AtlDemoLib,0x18AD0E93,0x3DF5,0x40D1,0xBD,0x6C,0x55,0x4F,0x9C,0x83,0x15,0xD5);


MIDL_DEFINE_GUID(IID, DIID__IIAtlDemoCtrlEvents,0xECB057BA,0x98E9,0x494B,0xB1,0x1B,0x70,0xB2,0xAC,0x82,0x14,0xEB);


MIDL_DEFINE_GUID(CLSID, CLSID_IAtlDemoCtrl,0x8F55A11E,0x35B9,0x4B5F,0x98,0xBD,0x53,0x50,0x2C,0x15,0x90,0xDD);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



