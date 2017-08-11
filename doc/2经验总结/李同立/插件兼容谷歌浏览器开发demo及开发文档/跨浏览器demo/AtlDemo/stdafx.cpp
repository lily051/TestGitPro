// stdafx.cpp : source file that includes just the standard includes
// AtlDemo.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


#pragma warning(disable:4996)
#pragma warning(disable:4995)
void OutputPrintf(const char* szContent,...)
{
    va_list args;
    va_start(args, szContent);
    int nLen = _vsnprintf(NULL, 0, szContent, args);
    va_end(args);

    if(nLen > 0)
    {
        char* pbuf = new(std::nothrow)char[nLen+1];
        if(pbuf != NULL)
        {
            memset(pbuf,0,nLen + 1);
            va_list args;//lint !e578
            va_start(args, szContent);
            int retLen = _vsnprintf(pbuf, nLen + 1, szContent, args);
            va_end(args);
            if(retLen > 0)
            {
                char szMsg[30];
                memset(szMsg, 0, 30); 
                SYSTEMTIME sys; 
                GetLocalTime( &sys ); 
                sprintf(szMsg, "[%02d:%02d:%02d:%03d]    ", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
                std::string s(szMsg);
                s.append(pbuf);
                OutputDebugString(s.c_str());
                //pbuf[nLen] = 0;
                //OutputDebugString(pbuf);
                //char szMsg[30];
                //memset(szMsg, 0, 30); 
                //SYSTEMTIME sys; 
                //GetLocalTime( &sys ); 
                //sprintf(szMsg, "[%02d:%02d:%02d:%03d]    ", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
                //std::string s(szMsg);
                //s.append(pbuf);
                //OutputDebugString(s.c_str());
            }
            delete []pbuf;
        }//lint !e438
    }
}//lint !e438