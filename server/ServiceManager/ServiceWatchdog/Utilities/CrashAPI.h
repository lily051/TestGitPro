/********************************************************************
created:	2011/04/04
created:	4:4:2011   14:42
filename: 	CrashAPI.h
author:		lilei
purpose:    The CrashAPI library is responsible for monitor an application
            for crashes(exceptions),handing them when they occur by 
            generating a minidump file. 
            This library is based on the open source project google-breakpad.
            You can get the source code in google code to learn more.
*********************************************************************/
//lint -library
#ifndef __CRASHAPI_API__
#define __CRASHAPI_API__

#define CRASH_API  _declspec(dllexport)

typedef  void (*CrashCallback)();

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************
*   Function:           CrashAPI_Init
*   Description:        Init the Crash API lib.Call this function  as early
                        in the start-up process as possible.
*   Input:              (null)
*   Output:             (null)
*   Return:             returns true on success.
**************************************************************************/
CRASH_API bool CrashAPI_Init();

/**************************************************************************
*   Function:           CrashAPI_Uninit
*   Description:        uninstall the library
*   Input:              (null)
*   Output:             (null)
*   Return:             none
**************************************************************************/
CRASH_API void CrashAPI_Uninit();

/**************************************************************************
*   Function:           CrashAPI_SetDumpPath
*   Description:        set the minidump file path.the file will be generated
                        in the current dictionary if you don't set it.
*   Input:              dump_path the path of the  
*   Output:             (null)
*   Return:             none
**************************************************************************/
CRASH_API void CrashAPI_SetDumpPath(char * dump_path);

/**************************************************************************
*   Function:           CrashAPI_WriteMinidump
*   Description:        writes a minidump immediately.it can be used to 
*                       capture the execution state independently of a crash.
*   Input:              (null)  
*   Output:             (null)
*   Return:             returns true on success.
**************************************************************************/
CRASH_API bool CrashAPI_WriteMinidump();

/**************************************************************************
*   Function:           CrashAPI_SetCallBack
*   Description:        Set the call back function which will be called when 
                        the crash occurs.
*   Input:              (null)  
*   Output:             (null)
*   Return:             none
**************************************************************************/
CRASH_API void CrashAPI_SetCallBack(CrashCallback callback);

#ifdef __cplusplus
}
#endif

#endif