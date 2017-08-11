

#pragma once
#define SUPPORT_PCNVRCB

//#define SUPPORT_DAHUAVOD

//#define SUPPORT_VTDU

//#define SUPPORT_AXISVOD

//#define SUPPORT_SONYVOD

//#define SUPPORT_CASCADE

//#define SUPPORT_DB33

//#define SUPPORT_NETPERSEC

#define SUPPORT_ASYNC_ACCEPT

#ifdef SUPPORT_PCNVRCB
typedef struct _STR_PCNVR_
{
	HPR_VOIDPTR pCB;
	HPR_VOIDPTR pthis;
}strPcnvr;
#endif