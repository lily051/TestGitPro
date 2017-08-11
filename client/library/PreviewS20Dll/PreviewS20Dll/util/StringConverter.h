#pragma once
#include <string>
#include <wtypes.h>
/*lint -e39*/
#if (defined _UNICODE) || (defined UNICODE)
#define _tstring wstring
#else
#define _tstring string
#endif

class CStringConverter 
{
public:
    static std::wstring Ansi2Unicode(const std::string& szAnsi) 
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
    static std::string Unicode2Ansi(const std::wstring& szUnicode) 
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
    
    static std::wstring Utf82Unicode(const std::string& szUtf8) 
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

    static std::string Unicode2Utf8(const std::wstring& szUnicode) 
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
    
    static std::string Ansi2Utf8(const std::string& szAnsi) 
	{
        return Unicode2Utf8(Ansi2Unicode(szAnsi));
    }

    static std::string Utf82Ansi(const std::string& szUtf8) 
	{
        return Unicode2Ansi(Utf82Unicode(szUtf8));
    }

	static std::_tstring Ansi2Tstring(const std::string& szAnsi)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Ansi2Unicode(szAnsi);
#else
		return szAnsi;
#endif
	}

	static std::_tstring Unicode2Tstring(const std::wstring& szUnicode)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return szUnicode;
#else
		return Unicode2Ansi(szUnicode);
#endif
	}

	static std::string Tstring2Ansi(const std::_tstring& _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Unicode2Ansi(_tstr);
#else
		return _tstr;
#endif
	}

	static std::wstring Tstring2Unicode(const std::_tstring& _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return _tstr;
#else
		return Ansi2Unicode(_tstr);
#endif
	}

	static std::string Tstring2Utf8(const std::_tstring& _tstr)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Unicode2Utf8(_tstr);
#else
		return Unicode2Utf8(Ansi2Unicode(_tstr));
#endif
	}
	static std::_tstring Utf82Tstring(const std::string& szUtf8)
	{
#if (defined _UNICODE) || (defined UNICODE)
		return Utf82Unicode(szUtf8);
#else
		return Unicode2Ansi(Utf82Unicode(szUtf8));
#endif
	}
/*lint -e438*/
    static std::_tstring StdTstringFormat(const TCHAR *fmt,... ) 
    {
        std::_tstring strResult;
        if (NULL != fmt)
        {
            va_list marker = NULL;            
            va_start(marker, fmt);                            //初始化变量参数 
#if (defined _UNICODE) || (defined UNICODE)
            size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度

            wchar_t* pBuffer = new(std::nothrow) wchar_t[nLength];

            _vsnwprintf_s(pBuffer, nLength, nLength, fmt, marker); 
#else
            size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度

            char* pBuffer = new(std::nothrow) char[nLength];

            _vsnprintf_s(pBuffer, nLength, nLength, fmt, marker);
#endif
            strResult = pBuffer;
            delete [] pBuffer;
            va_end(marker);                                    //重置变量参数
        }
        return strResult; 
    }
	static std::string StdstringFormat(const char *fmt,... ) 
	{ 
		std::string strResult;

		if (NULL != fmt)
		{
			va_list marker = NULL;            
			va_start(marker, fmt);                            //初始化变量参数 

			int nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度

			char* pBuffer = new(std::nothrow) char[nLength];

			_vsnprintf_s(pBuffer, nLength, nLength, fmt, marker);

			strResult = pBuffer;

			delete [] pBuffer;
			va_end(marker);                                    //重置变量参数
		}
		return strResult; 
	}
	//字符串格式化函数
	static std::wstring StdwstringFormat(const wchar_t *fmt,... ) 
	{ 
		std::wstring strResult;
		if (NULL != fmt)
		{
			va_list marker = NULL;            
			va_start(marker, fmt);                            //初始化变量参数

			size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度
			
			wchar_t* pBuffer = new(std::nothrow) wchar_t[nLength];

			_vsnwprintf_s(pBuffer, nLength, nLength, fmt, marker); 

			strResult = pBuffer;

			delete [] pBuffer;
			va_end(marker);                                    //重置变量参数
			marker = NULL;
		}

		return strResult; 
	}
    /*lint -e438*/
};

/*lint -e39*/