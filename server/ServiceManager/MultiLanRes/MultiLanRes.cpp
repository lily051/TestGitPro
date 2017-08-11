/**	@file MultiLanRes.cpp
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief the implement file of MultiLanRes.h
*
*	@author		jiangtianjing
*	@date		2004/02/14
*
*	@note V1.1 updated by guotianwen, add notes.
*
*	@warning only support string resource.
*/

#include "stdafx.h"
#include "MultiLanRes.h"
#include <string>
#include <tchar.h>

#define MAX_LANG_SUPPORT 256

// global data
// 模块句柄
HANDLE  g_hModule = NULL;

// 支持的语种数目
int     g_iLangCount = 0;

// 语言标准ID
LANGID  g_iAllLangID[MAX_LANG_SUPPORT + 1] = {0};

// function
static void DetectLanguage(void);

//
BOOL CALLBACK EnumLangProc(HANDLE /*hModule*/, LPCTSTR /*lpszType*/, LPCTSTR /*lpszName*/, WORD wIDLanguage, LONG_PTR /*lParam*/)
{
    g_iAllLangID[g_iLangCount++] = wIDLanguage;

    //char lanString[128] = {0};
    //sprintf(lanString, "\t\tLanguage  %u\n",(USHORT)wIDLanguage);
    //OutputDebugString(lanString);

    return TRUE;
}

// dll entry
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID /*lpReserved*/
					 )
{
    if (!g_hModule)
    {
        g_hModule = hModule;

        DetectLanguage();
    }
    
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

/**	@fn	    void DetectLanguage()
*	@brief	detect support language.
*	@return	void.
*/
void DetectLanguage(void)
{
    FillMemory(g_iAllLangID, sizeof(g_iAllLangID)/sizeof(g_iAllLangID[0]), 0xFFFF);
    
    //  6 - RT_STRING
    LPCTSTR szType = (LPCTSTR)(LPVOID)((WORD)RT_STRING);
    // 
    LPCTSTR szName = (LPCTSTR)7; //

	EnumResourceLanguages((HMODULE)g_hModule, szType, szName, (ENUMRESLANGPROC)EnumLangProc, NULL);
    
    LANGID uiLangID = GetUserDefaultUILanguage();

    g_iAllLangID[MAX_LANG_SUPPORT] = uiLangID;
    
    int iIndex = 0;

    while (g_iAllLangID[iIndex] != uiLangID)
    {
        ++iIndex;
    }

    g_iAllLangID[MAX_LANG_SUPPORT] = 0xFFFF;

    if (iIndex == MAX_LANG_SUPPORT) // not found the language in the resource.
    {
        uiLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US); // en-us
    }
    
    LCID dwLocale = MAKELCID(uiLangID, SORT_DEFAULT);

    SetThreadLocale(dwLocale);  
}

/**	@fn	    const MULTILANRES_API void MutliLanRes_LoadString(UINT uID, LPTSTR szBuffer, int nBufferMax)
*	@brief	loads a string resource from the language dll, copies the string into a buffer,
            and appends a terminating NULL character.
*	@param  uID: the integer identifier of the string to be loaded.
*	@param  szBuffer: the buffer alloced by app to receive the string.
*   @param  iBufferMax: the size of the buffer in chars == sizeof(szBuffer)/szBuffer[0],(Use Multi-Byte Character Set).
*	@return	If the function fails, the "-?-" is copied into the szBuffer.
*/
MULTILANRES_API void MultiLanRes_LoadString(UINT uID, LPTSTR szBuffer, int nBufferMax)
{
    int nLoad = ::LoadString((HMODULE)g_hModule, uID, szBuffer, nBufferMax);
	
	if (0 == nLoad)
    {
        strncpy(szBuffer, _T("-?-"), nBufferMax); // // load fail, copy "-?-" into buffer
    }
	else if (nLoad >= nBufferMax)
	{
		szBuffer[nBufferMax - 1] = 0;
	}
}

/**	@fn	    const MULTILANRES_API void MultiLanRes_LoadStringW(UINT uID, LPWSTR szBuffer, int nBufferMax)
*	@brief	loads a string resource from the language dll, copies the string into a buffer,
and appends a terminating NULL character.
*	@param  uID: the integer identifier of the string to be loaded.
*	@param  szBuffer: the buffer alloced by app to receive the string.
*   @param  iBufferMax: the size of the buffer in chars == sizeof(szBuffer)/szBuffer[0],(Use Multi-Byte Character Set).
*	@return	If the function fails, the "-?-" is copied into the szBuffer with wide byte.
*/
MULTILANRES_API void MultiLanRes_LoadStringW(UINT uID, LPWSTR szBuffer, int nBufferMax)
{
    TCHAR srcBuff[MAX_PATH] = {0};
    int nLoad = ::LoadString((HMODULE)g_hModule, uID, srcBuff, _countof(srcBuff));
    if (0 == nLoad)
    {
        strncpy_s(srcBuff, _T("-?-"), nBufferMax); // // load fail, copy "-?-" into buffer
        return;
    }
    else if (nLoad >= nBufferMax)
    {
        szBuffer[nBufferMax - 1] = 0;
    }

    // modify by gtw，transfer mutilbyte to widechar
    int nLen = ( (int)strlen(srcBuff) ) + 1;
    int nwLen = MultiByteToWideChar(CP_ACP, 0, srcBuff, nLen, NULL, 0);

    MultiByteToWideChar(CP_ACP, 0, srcBuff, nLen, szBuffer, nwLen);
}


/**	@fn	    const MULTILANRES_API void MutliLanRes_GetSupportedLanguageCount()
*	@brief	get the language count which resource is supported.
*	@return	the count that resource support.
*/
MULTILANRES_API int MultiLanRes_GetSupportedLanguageCount(void)
{
    return g_iLangCount;
}

/**	@fn	    const char * MutliLanRes_GetSupportedLanguage(int iIndex)
*	@brief	according to parameter iIndex, get the language.
*	@param  iIndex: the index of language.
*	@param  szBuffer: the buffer alloced by app to receive the string.
*	@return	the language description.
*/
MULTILANRES_API const char * MultiLanRes_GetSupportedLanguage(int iIndex)
{
    if (iIndex < 0 || iIndex > g_iLangCount)
    {
        return NULL;
    } 
    
    /*
    Table of Language Identifiers
    The following are language identifiers. They are composed of a primary language identifier and a sublanguage identifier. 
    The following identifiers were composed using the MAKELANGID macro. 

    Identifier Language */

    LANGID uiLangID = g_iAllLangID[iIndex];

    switch (uiLangID)
    {
    case 0x0000:
        return "Language Neutral";
    case 0x007f:
        return "The language for the invariant locale (LOCALE_INVARIANT). See MAKELCID. ";
    case 0x0400:
        return "Process or User Default Language";
    case 0x0800: 
        return "System Default Language"; // 2048
    case 0x0436: 
        return "Afrikaans ";
    case 0x041c: 
        return "Albanian ";
    case 0x0401: 
        return "Arabic (Saudi Arabia) ";
    case 0x0801: 
        return "Arabic (Iraq) ";
    case 0x0c01: 
        return "Arabic (Egypt) ";
    case 0x1001: 
        return "Arabic (Libya) ";
    case 0x1401: 
        return "Arabic (Algeria) ";
    case 0x1801: 
        return "Arabic (Morocco) ";
    case 0x1c01: 
        return "Arabic (Tunisia) ";
    case 0x2001: 
        return "Arabic (Oman) ";
    case 0x2401:
        return "Arabic (Yemen) ";
    case 0x2801: 
        return "Arabic (Syria)"; 
    case 0x2c01: 
        return "Arabic (Jordan)"; 
    case 0x3001: 
        return "Arabic (Lebanon) ";
    case 0x3401: 
        return "Arabic (Kuwait) ";
    case 0x3801: 
        return "Arabic (U.A.E.) ";
    case 0x3c01:
        return "Arabic (Bahrain) ";
    case 0x4001:
        return "Arabic (Qatar) ";
    case 0x042b: 
        return "Windows 2000/XP: Armenian. This is Unicode only."; 
    case 0x042c:
        return "Azeri (Latin)"; 
    case 0x082c:
        return "Azeri (Cyrillic) ";
    case 0x042d:
        return "Basque"; 
    case 0x0423: 
        return "Belarusian ";
    case 0x0402: 
        return "Bulgarian";
    case 0x0455: 
        return "Burmese ";
    case 0x0403: 
        return "Catalan ";
    case 0x0404:
        return "Chinese (Taiwan)"; // 1028
    case 0x0804: 
        return "Chinese (PRC)"; 
    case 0x0c04: 
        return "Chinese (Hong Kong SAR, PRC)";
    case 0x1004: 
        return "Chinese (Singapore) ";
    case 0x1404: 
        return "Windows 98/Me, Windows 2000/XP: Chinese (Macao SAR) ";
    case 0x041a:
        return "Croatian ";
    case 0x0405:
        return "Czech ";
    case 0x0406: 
        return "Danish ";
    case 0x0465:
        return "Windows XP: Divehi. This is Unicode only. ";
    case 0x0413:
        return "Dutch (Netherlands) ";
    case 0x0813:
        return "Dutch (Belgium)";
    case 0x0409: 
        return "English (United States)";
    case 0x0809:
        return "English (United Kingdom) ";
    case 0x0c09:
        return "English (Australian) ";
    case 0x1009: 
        return "English (Canadian) ";
    case 0x1409: 
        return "English (New Zealand)"; 
    case 0x1809: 
        return "English (Ireland) ";
    case 0x1c09: 
        return "English (South Africa) ";
    case 0x2009: 
        return "English (Jamaica) ";
    case 0x2409: 
        return "English (Caribbean) ";
    case 0x2809:
        return "English (Belize) ";
    case 0x2c09:
        return "English (Trinidad) ";
    case 0x3009: 
        return "Windows 98/Me, Windows 2000/XP: English (Zimbabwe) ";
    case 0x3409: 
        return "Windows 98/Me, Windows 2000/XP: English (Philippines)"; 
    case 0x0425: 
        return "Estonian ";
    case 0x0438:
        return "Faeroese ";
    case 0x0429:
        return "Farsi ";
    case 0x040b:
        return "Finnish ";
    case 0x040c:
        return "French (Standard) ";
    case 0x080c :
        return "French (Belgian) ";
    case 0x0c0c:
        return "French (Canadian) ";
    case 0x100c :
        return "French (Switzerland)"; 
    case 0x140c :
        return "French (Luxembourg) ";
    case 0x180c :
        return "Windows 98/Me, Windows 2000/XP: French (Monaco) ";
    case 0x0456:
        return "Windows XP: Galician ";
    case 0x0437:
        return "Windows 2000/XP: Georgian. This is Unicode only. ";
    case 0x0407:
        return "German (Standard) ";
    case 0x0807:
        return "German (Switzerland) ";
    case 0x0c07:
        return "German (Austria) ";
    case 0x1007:
        return "German (Luxembourg) ";
    case 0x1407:
        return "German (Liechtenstein) ";
    case 0x0408:
        return "Greek ";
    case 0x0447:
        return "Windows XP: Gujarati. This is Unicode only.";
    case 0x040d:
        return "Hebrew ";
    case 0x0439:
        return "Windows 2000/XP: Hindi. This is Unicode only.";
    case 0x040e:
        return "Hungarian";
    case 0x040f: 
        return "Icelandic ";
    case 0x0421: 
        return "Indonesian ";
    case 0x0410: 
        return "Italian (Standard) ";
    case 0x0810: 
        return "Italian (Switzerland)";
    case 0x0411: 
        return "Japanese ";
    case 0x044b :
        return "Windows XP: Kannada. This is Unicode only. ";
    case 0x0457: 
        return "Windows 2000/XP: Konkani. This is Unicode only. ";
    case 0x0412: 
        return "Korean ";
    case 0x0812: 
        return "Windows 95, Windows NT 4.0 only: Korean (Johab) ";
    case 0x0440: 
        return "Windows XP: Kyrgyz. ";
    case 0x0426: 
        return "Latvian ";
    case 0x0427: 
        return "Lithuanian"; 
    case 0x0827: 
        return "Windows 98 only: Lithuanian (Classic) ";
    case 0x042f:
        return "FYRO Macedonian ";
    case 0x043e:
        return "Malay (Malaysian)"; 
    case 0x083e:
        return "Malay (Brunei Darussalam) ";
    case 0x044e:
        return "Windows 2000/XP: Marathi. This is Unicode only. ";
    case 0x0450:
        return "Windows XP: Mongolian ";
    case 0x0414:
        return "Norwegian (Bokmal) ";
    case 0x0814:
        return "Norwegian (Nynorsk) ";
    case 0x0415:
        return "Polish ";
    case 0x0416: 
        return "Portuguese (Brazil) ";
    case 0x0816:
        return "Portuguese (Portugal) ";
    case 0x0446:
        return "Windows XP: Punjabi. This is Unicode only. ";
    case 0x0418: 
        return "Romanian ";
    case 0x0419: 
        return "Russian ";
    case 0x044f:
        return "Windows 2000/XP: Sanskrit. This is Unicode only."; 
    case 0x0c1a: 
        return "Serbian (Cyrillic) ";
    case 0x081a: 
        return "Serbian (Latin) ";
    case 0x041b: 
        return "Slovak ";
    case 0x0424: 
        return "Slovenian"; 
    case 0x040a:
        return "Spanish (Spain, Traditional Sort) ";
    case 0x080a:
        return "Spanish (Mexican) ";
    case 0x0c0a: 
        return "Spanish (Spain, Modern Sort) ";
    case 0x100a: 
        return "Spanish (Guatemala) ";
    case 0x140a:
        return "Spanish (Costa Rica) ";
    case 0x180a: 
        return "Spanish (Panama) ";
    case 0x1c0a:
        return "Spanish (Dominican Republic) ";
    case 0x200a:
        return "Spanish (Venezuela) ";
    case 0x240a:
        return "Spanish (Colombia) ";
    case 0x280a: 
        return "Spanish (Peru) ";
    case 0x2c0a:
        return "Spanish (Argentina) ";
    case 0x300a: 
        return "Spanish (Ecuador) ";
    case 0x340a: 
        return "Spanish (Chile) ";
    case 0x380a: 
        return "Spanish (Uruguay) ";
    case 0x3c0a: 
        return "Spanish (Paraguay) ";
    case 0x400a: 
        return "Spanish (Bolivia) ";
    case 0x440a: 
        return "Spanish (El Salvador)"; 
    case 0x480a: 
        return "Spanish (Honduras) ";
    case 0x4c0a: 
        return "Spanish (Nicaragua) ";
    case 0x500a: 
        return "Spanish (Puerto Rico) ";
    case 0x0430: 
        return "Sutu ";
    case 0x0441: 
        return "Swahili (Kenya) ";
    case 0x041d: 
        return "Swedish ";
    case 0x081d: 
        return "Swedish (Finland) ";
    case 0x045a:
        return "Windows XP: Syriac. This is Unicode only. ";
    case 0x0449 :
        return "Windows 2000/XP: Tamil. This is Unicode only. ";
    case 0x0444:
        return "Tatar (Tatarstan) ";
    case 0x044a:
        return "Windows XP: Telugu. This is Unicode only. ";
    case 0x041e:
        return "Thai"; 
    case 0x041f: 
        return "Turkish ";
    case 0x0422: 
        return "Ukrainian ";
    case 0x0420: 
        return "Windows 98/Me, Windows 2000/XP: Urdu (Pakistan) ";
    case 0x0820:
        return "Urdu (India) ";
    case 0x0443:
        return "Uzbek (Latin) ";
    case 0x0843:
        return "Uzbek (Cyrillic) ";
    case 0x042a: 
        return "Windows 98/Me, Windows NT 4.0 and later: Vietnamese";
    default:
        return "-?-";
    }


/*The following three combinations of usPrimaryLanguage and usSubLanguage have special meaning. 

Primary language identifier Sublanguage identifier Meaning 
LANG_NEUTRAL SUBLANG_NEUTRAL Language neutral  
LANG_NEUTRAL SUBLANG_DEFAULT User default language  
LANG_NEUTRAL SUBLANG_SYS_DEFAULT System default language  

--------------------------------------------------------------------------------
*/
}
