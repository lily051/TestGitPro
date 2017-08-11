#pragma once

#include <windows.h>
#include <string>           //string

#include <comutil.h>
#include <atlbase.h>
#include <atlctl.h>         //_bstr_t

#include "pluginbase.h"     //NPVariant

#pragma comment(lib, "comsuppw.lib")


//屏蔽sprintf、strcpy、_vsnprintf不安全的警告
//#pragma warning(disable: 4996)

std::string BSTRToStdString(BSTR src);

//Utf8与GBK的编码转换
std::string ConvertUtf8ToGBK(const std::string& strUtf8);
std::string ConvertGBKToUtf8(const std::string& strGBK);
std::string ConvertGBKToUtf8(const _bstr_t &bstr);

void* NPN_MemAllocEx(const std::string& pstr);//lint -e101 e-132

std::string NPVariant_TO_GBK(const NPVariant args);
long NPVariant_TO_long(const NPVariant args);
unsigned long NPVariant_TO_Ulong(const NPVariant args);
bool NPVariant_TO_bool(const NPVariant args);

#define NPVARIANT_TO_GBK    NPVariant_TO_GBK
#define NPVARIANT_TO_LONG   NPVariant_TO_long
#define NPVARIANT_TO_ULONG   NPVariant_TO_Ulong
#define NPVARIANT_TO_BOOL   NPVariant_TO_bool


//打日志到Debugview
void OutputPrintf(const char* szContent,...);

