/**	@file StringConverter.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief      字符编码转换类
*	@author		liangjungao
*	@date		2012-3-27
*	@note		
*/
#pragma once
#include <string>
//#include <wtypes.h>

#if (defined _UNICODE) || (defined UNICODE)
#define _tstring wstring
#else
#define _tstring string
#endif

//CStringConverter-->CSC
class CSC 
{
public:
    //ANSI->Unicode(wchar、UTF16）
    static std::wstring Ansi2Unicode(const std::string& szAnsi) 
	{
        int len = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), NULL, 0);
        wchar_t* szUnicode = new wchar_t[len+1];
        MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), szUnicode, len);
        szUnicode[len] = 0;
        std::wstring rs = szUnicode;
        delete[] szUnicode;
        return rs;
    }

    //Unicode(wchar、UTF16)->ANSI
    static std::string Unicode2Ansi(const std::wstring& szUnicode) 
	{
        int len = WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
        char* szAnsi = new char[len + 1];
        WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), szAnsi, len, NULL, NULL);
        szAnsi[len] = 0;
        std::string rs = szAnsi;
        delete[] szAnsi;
        return rs;
    }
    
    //UTF8->Unicode(wchar、UTF16)
    static std::wstring Utf82Unicode(const std::string& szUtf8) 
	{
        int len = MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), NULL, 0);
        wchar_t* szUnicode = new wchar_t[len+1];
        MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), szUnicode, len);
        szUnicode[len] = 0;
        std::wstring rs = szUnicode;
        delete[] szUnicode;
        return rs;
    }
    
    //Unicode(wchar、UTF16)->UTF8
    static std::string Unicode2Utf8(const std::wstring& szUnicode) 
	{
        int len = WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
        char* szUtf8 = new char[len + 1];
        WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), szUtf8, len, NULL, NULL);
        szUtf8[len] = 0;
        std::string rs = szUtf8;
        delete[] szUtf8;
        return rs;
    }
    
    //ANSI->UTF8
    static std::string Ansi2Utf8(const std::string& szAnsi) 
	{
        return Unicode2Utf8(Ansi2Unicode(szAnsi));
    }

    //UTF8->ANSI
    static std::string Utf82Ansi(const std::string& szUtf8) 
	{
        return Unicode2Ansi(Utf82Unicode(szUtf8));
    }

    //下面是扩展的方法
    //
	static std::_tstring Ansi2Tstring(const std::string& szAnsi)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Ansi2Unicode(szAnsi);
#else
		return szAnsi;
#endif
	}

    //
	static std::_tstring Unicode2Tstring(const std::wstring& szUnicode)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return szUnicode;
#else
		return Unicode2Ansi(szUnicode);
#endif
	}

    //
	static std::string Tstring2Ansi(const std::_tstring& _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Unicode2Ansi(_tstr);
#else
		return _tstr;
#endif
	}

    //
	static std::wstring Tstring2Unicode(const std::_tstring& _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return _tstr;
#else
		return Ansi2Unicode(_tstr);
#endif
	}

    //
    static std::string Tstring2UTF8(const std::_tstring& _tstr)
    {
#if (defined _UNICODE) || (defined UNICODE)
        return Unicode2Utf8(_tstr);
#else
        return Ansi2Utf8(_tstr);
#endif
    }

    //
    static std::_tstring  UTF82Tstring(const std::string& str)
    {
#if (defined _UNICODE) || (defined UNICODE)
        return Utf82Unicode(str);
#else
        return Utf82Ansi(str);
#endif
    }
};
