#pragma once
#include <string>
#include <wtypes.h>

#if (defined _UNICODE) || (defined UNICODE)
#define _tstring wstring
#else
#define _tstring string
#endif

class CSCT
{/*lint !e39*/
public:
    static std::wstring Ansi2Unicode(std::string szAnsi) 
	{
        //calc block size to be returned
        int len = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), NULL, 0);
        //malloc and fill the returned block
        wchar_t* szUnicode = new wchar_t[len+1];
        MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), szUnicode, len);
        szUnicode[len] = 0;
        std::wstring rs = szUnicode;
        delete[] szUnicode;
      
        return rs;
    }

	static LPCWSTR Ansi2LPCTSTR(std::string szAnsi) 
	{
		//calc block size to be returned
		int len = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), NULL, 0);
		//malloc and fill the returned block
		wchar_t* szUnicode = new wchar_t[len+1];
		MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), szUnicode, len);
		szUnicode[len] = 0;
		return szUnicode;
	}

    static std::string Unicode2Ansi(std::wstring szUnicode) 
	{
        //calc block size to be returned
        int len = WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
        //malloc and fill the returned block
        char* szAnsi = new char[len + 1];
        WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), szAnsi, len, NULL, NULL);
        szAnsi[len] = 0;
        std::string rs = szAnsi;
        delete[] szAnsi;
        
        return rs;
    }

//     static std::string UnicodeCS2Ansi(CString &strUnicode) 
//     {
//         return CSCT::Unicode2Ansi(strUnicode.GetBuffer(strUnicode.GetLength()));
//     }
    
    static std::wstring Utf82Unicode(std::string szUtf8) 
	{
        //calc block size to be returned
        int len = MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), NULL, 0);
        //malloc and fill the returned block
        wchar_t* szUnicode = new wchar_t[len+1];
        MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), szUnicode, len);
        szUnicode[len] = 0;
        std::wstring rs = szUnicode;
        delete[] szUnicode;
        
        return rs;
    }

    static std::string Unicode2Utf8(std::wstring szUnicode) 
	{
        //calc block size to be returned
        int len = WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
        //malloc and fill the returned block
        char* szUtf8 = new char[len + 1];
        WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), szUtf8, len, NULL, NULL);
        szUtf8[len] = 0;
        std::string rs = szUtf8;
        delete[] szUtf8;
        
        return rs;
    }
    
    static std::string Ansi2Utf8(std::string szAnsi) 
	{
        return Unicode2Utf8(Ansi2Unicode(szAnsi));
    }

    static std::string Utf82Ansi(std::string szUtf8) 
	{
        return Unicode2Ansi(Utf82Unicode(szUtf8));
    }

	static std::_tstring Ansi2Tstring(std::string szAnsi)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Ansi2Unicode(szAnsi);
#else
		return szAnsi;
#endif
	}

	static std::_tstring Unicode2Tstring(std::wstring szUnicode)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return szUnicode;
#else
		return Unicode2Ansi(szUnicode);
#endif
	}

	static std::string Tstring2Ansi(std::_tstring _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Unicode2Ansi(_tstr);
#else
		return _tstr;
#endif
	}

	static std::wstring Tstring2Unicode(std::_tstring _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return _tstr;
#else
		return Ansi2Unicode(_tstr);
#endif
	}
};
