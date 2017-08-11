/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HPR_Config.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! \file HPR_Config.h
 */

#ifndef __OS_CONFIG_H__
#define __OS_CONFIG_H__

#define  HPR_OS_NOT_SUPPORT "OS Not Implement Yet."

#if (defined(WIN32) || defined(_WIN32_WCE))

	//MSVC Version (vs98/vs2003/vs2005/vs2008/vs2010)
	#if (_MSC_VER >= 1600)		//vs2010
		#pragma warning(disable: 4996)
		#define MSVC_2010
	#elif (_MSC_VER >= 1500)	//vs2008
		#pragma warning(disable: 4996)
		#define MSVC_2008
	#elif (_MSC_VER >= 1400)	//vs2005
		#define MSVC_2005
	#elif (_MSC_VER >= 1310)	//vs2003
		#define _WIN32_WINNT 0x0500
		#define MSVC_2003		
	#else						//vs98
		#define _WIN32_WINNT 0x0500
		#pragma warning(disable: 4786)
		#define MSVC_98
	#endif

	#pragma warning(disable:4100)
	#pragma warning(disable:4127)

	#ifndef CALLBACK
	#define CALLBACK __stdcall
	#endif

	#include <winsock2.h>	
	#include <ws2tcpip.h>
	#include <windows.h>
	#include <stdio.h>
	#include <stdlib.h>
    #include <Mswsock.h>
	
	#define OS_WINDOWS	1
	#define HPR_EXPORTS 1
	#define HPR_INFINITE INFINITE

	#if defined(HPR_EXPORTS)
		#define HPR_DECLARE extern "C" __declspec(dllexport)
		#define HPR_DECLARE_CLASS __declspec(dllexport)
	#else
		#define HPR_DECLARE extern "C" __declspec(dllimport)
		#define HPR_DECLARE_CLASS __declspec(dllimport)
	#endif

#elif (defined(__linux__) || defined(__APPLE__))
	#include <unistd.h>	
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <netdb.h>
	#include <errno.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <uuid/uuid.h>

	#define OS_POSIX	1
	#define HPR_INFINITE 0xFFFFFFFF
	#define WAIT_TIMEOUT	258

	#if defined (__APPLE__)//Æ»¹û
	#include <sys/stat.h>
	#include <sys/sockio.h>
	#include <net/if.h>
	#endif

	#define HPR_DECLARE extern "C"
	#define HPR_DECLARE_CLASS 

	#ifndef CALLBACK
		#define CALLBACK
	#else
		#undef CALLBACK
		#define CALLBACK
	#endif
#else
	#error OS Not Implement Yet.
#endif	// end #if (defined(WIN32) || defined(_WIN32_WCE))

//·µ»ØÖµ
#define HPR_OK			0
#define HPR_ERROR		-1
#define HPR_NOSUPPORT	-2

#define OS_SUPPORT_IPV6 1

#endif	// end #ifndef __OS_CONFIG_H__
