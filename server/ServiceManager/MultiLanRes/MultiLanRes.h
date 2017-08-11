/**	@file MultiLanRes.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief manage resource of multi-language software.
*
*	@author		jiangtianjing
*	@date		2004/02/14
*
*	@note V1.1 updated by guotianwen, add notes.
*
*	@warning only support string resource.
*/

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RECORDSRVRES_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RECORDSRVRES_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MultiLanRes_EXPORTS
#define MULTILANRES_API __declspec(dllexport)
#else
#define MULTILANRES_API __declspec(dllimport)
#endif

/**	@fn	    const MULTILANRES_API void MutliLanRes_GetSupportedLanguageCount()
*	@brief	get the language count which resource is supported.
*	@return	the count that resource support.
*/
MULTILANRES_API int MultiLanRes_GetSupportedLanguageCount(void);

/**	@fn	    const char * MutliLanRes_GetSupportedLanguage(int iIndex)
*	@brief	according to parameter iIndex, get the language.
*	@param  iIndex: the index of language.
*	@param  szBuffer: the buffer alloced by app to receive the string.
*	@return	the language description.
*/
MULTILANRES_API const char * MultiLanRes_GetSupportedLanguage(int iIndex);

/**	@fn	    const MULTILANRES_API void MutliLanRes_LoadString(UINT uID, LPTSTR szBuffer, int nBufferMax)
*	@brief	loads a string resource from the language dll, copies the string into a buffer,
            and appends a terminating NULL character.
*	@param  uID: the integer identifier of the string to be loaded.
*	@param  szBuffer: the buffer alloced by app to receive the string.
*   @param  iBufferMax: the size of the buffer in chars == sizeof(szBuffer)/szBuffer[0],(Use Multi-Byte Character Set).
*	@return	If the function fails, the "-?-" is copied into the szBuffer.
*/
MULTILANRES_API void MultiLanRes_LoadString(UINT uID, LPTSTR szBuffer, int nBufferMax);

MULTILANRES_API void MultiLanRes_LoadStringW(UINT uID, LPWSTR szBuffer, int nBufferMax);
