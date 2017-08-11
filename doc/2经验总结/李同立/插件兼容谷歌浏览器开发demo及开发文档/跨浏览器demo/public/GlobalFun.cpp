#include "GlobalFun.h"

#pragma warning (disable : 4996)

/**@fn   <BSTRToStdString>
*@brief  <将BSTR转为std::string>
*@param  <src: 待转换的字符串>
*@return <std::string: 保存转换结果的字符串，空字符串表示失败>
*@note   <封装了BSTRToStdString，简化使用、避免内存泄露>
*/
std::string BSTRToStdString(BSTR src)
{
    std::string dst;
    if (src != NULL)
    {
        char* pText = _com_util::ConvertBSTRToString(src);
        if (pText != NULL)
        {
            dst = pText;
            delete[] pText;
        }
    }

    return dst;
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8) 
{
    std::string strGBK;
    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL,0);
    wchar_t* wszGBK = new (std::nothrow)wchar_t[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
    char *szGBK=new (std::nothrow)char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

    strGBK = szGBK;
    delete[] szGBK;
    delete[] wszGBK;

    return strGBK;
}

std::string ConvertGBKToUtf8(const std::string& strGBK) 
{
    std::string strUtf8;
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, NULL,0);
    wchar_t* wszUtf8 = new (std::nothrow)wchar_t[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, wszUtf8, len);

    len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 
    char *szUtf8=new (std::nothrow)char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);

    strUtf8 = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;

    return strUtf8;
}

std::string ConvertGBKToUtf8(const _bstr_t &bstr) 
{
    std::string strUtf8;
    std::string strGBK = BSTRToStdString((BSTR)bstr);
    strUtf8 = ConvertGBKToUtf8(strGBK);

    return strUtf8;
}

void* NPN_MemAllocEx(const std::string& pstr)
{
    int size = pstr.length() + 1;
    void * rv = NPN_MemAlloc( size );
    strcpy_s( (char*) rv, size, pstr.c_str() );

    return rv;
}

std::string NPVariant_TO_GBK(const NPVariant args)
{
    std::string strGBK = "";
    std::string strUtf8 = "";
    if (args.type == NPVariantType_String)
    {
        strUtf8 = std::string(args.value.stringValue.UTF8Characters);
    }
    else
    {
        return strGBK;
    }

    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL,0);
    wchar_t* wszGBK = new (std::nothrow)wchar_t[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
    char *szGBK=new (std::nothrow)char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

    strGBK = szGBK;
    delete[] szGBK;
    delete[] wszGBK;

    return strGBK;
}

long NPVariant_TO_long(const NPVariant args)
{
    //传入的int参数 Fireforx下需要从int32取，chrome下需要从double下取，如果是字符串，则转成数字
    long lData = -1;
    if (args.type == NPVariantType_Int32)
    {
        lData = (long)args.value.intValue;
    }
    if (args.type == NPVariantType_Double)
    {
        lData = (long)args.value.doubleValue;
    }
    else if (args.type == NPVariantType_String)
    {
        NPString s(args.value.stringValue);
        lData = atol(s.UTF8Characters);
    }

    return lData;
}

unsigned long NPVariant_TO_Ulong(const NPVariant args)
{
    unsigned long lData = 0;
    if (args.type == NPVariantType_Int32)
    {
        lData = (unsigned long)args.value.intValue;
    }
    if (args.type == NPVariantType_Double)
    {
        lData = (unsigned long)args.value.doubleValue;
    }
    else if (args.type == NPVariantType_String)
    {
        NPString s(args.value.stringValue);
        lData = (unsigned long)atol(s.UTF8Characters);
    }

    return lData;
}

bool NPVariant_TO_bool(const NPVariant args)
{
    bool bData = 0;
    //传入的int参数 Fireforx下需要从int32取，chrome下需要从double下取，如果是字符串，则转成数字
    if (args.type == NPVariantType_Int32)
    {
        bData = args.value.intValue ? 1 : 0;
    }
    else if (args.type == NPVariantType_Double)
    {
        bData = args.value.doubleValue ? 1 : 0;
    }
    else if (args.type == NPVariantType_String)
    {
        NPString s(args.value.stringValue);
        bData = atoi(s.UTF8Characters) ? 1 : 0;
    }

    return bData;
}

void OutputPrintf(const char* szContent,...)
{
    va_list args;//lint !e830
    va_start(args, szContent);
    int nLen = _vsnprintf(NULL, 0, szContent, args);
    va_end(args);

    if(nLen > 0)
    {
        char* pbuf = new(std::nothrow)char[nLen+1];
        if(pbuf != NULL)
        {
            memset(pbuf,0,nLen + 1);
            va_start(args, szContent);
            int retLen = _vsnprintf_s(pbuf, nLen + 1, _TRUNCATE, szContent, args);
            va_end(args);
            if(retLen > 0)
            {
                pbuf[nLen] = 0;
                OutputDebugString(pbuf);
            }
            delete []pbuf;
        }
    }
}//lint !e438

