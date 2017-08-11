// public.h : public DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#include "resource.h"		// 主符号

# if defined(PUBLIC_EXPORTS)
#    define PUBLIC extern  __declspec(dllexport)
#  else
#    define PUBLIC extern  __declspec(dllimport)
#  endif

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

/** @ fn     CString2Char
 *  @ brief  CString转str
 *  @ param  temp-[out] 要输出的字符串
 *  @ param  str-[in] 要转换的CString
 *  @ return 字符串指针
 */
PUBLIC void CString2Char(const CString & str,char *temp);

/** @ fn     Char2CString
 *  @ brief  char转CString
 *  @ param  str-[in] 要转换的CString
 *  @ param  temp-[out] 要输出的CString
 *  @ return CString
 */
PUBLIC void Char2CString(char *temp,CString &str);

/** @ fn     Add
 *  @ brief  两个int相加
 *  @ param  a-[in] 整数a
 *  @ param  b-[in] 整数b
 *  @ return 返回两个数的和
 */
PUBLIC int Add(const int &a,const int &b);
/** @ fn     TCHAR2Char
 *  @ brief  TCHAR类型转CHAR
 *  @ param  tchar-[in] 要转换的TCHAR
 *  @ param  _char-[in/out] 输入并输出的CHAR指针
 *  @ return 返回两个数的和
 */
PUBLIC void TCHAR2Char(const TCHAR * tchar, char * _char) ;
/** @ fn     Char2TCHAR
 *  @ brief  CHAR类型转TCHAR
 *  @ param  _char-[in] 要转换的CHAR
 *  @ param  tchar-[in/out] 输入并输出的TCHAR指针
 *  @ return 返回两个数的和
 */
PUBLIC void Char2TCHAR (const char * _char, TCHAR * tchar);

/** @ fn     SetConfig
 *  @ brief  创建配置文件
 *  @ param  hModule-[in] 模块句柄
 *  @ param  configname-[in] 配置文件名称
 *  @ return 返回两个数的和
 */
PUBLIC CString SetConfig(HMODULE hModule,CString configname=TEXT("Config.ini"));
/** @ fn     GetConfig
 *  @ brief  创建配置文件
 *  @ param  hModule-[in] 模块句柄
 *  @ param  configname-[in] 配置文件名称
 *  @ return 返回两个数的和
 */
PUBLIC CString GetConfig( HMODULE hModule,CString configname = TEXT("Config.ini"));

/** @ fn     SoftDog
 *  @ brief  密码加密
 *  @ param  password-[in] 要加密的密码
 *  @ param  res_password-[out] 加密后的密码
 *  @ return 加密后的密码
 */
PUBLIC void SoftDog(CString const  password,TCHAR *res_password);

/** @ fn     SoftDog
 *  @ brief  密码解密
 *  @ param  res_password-[in] 要解密的密码
 *  @ param  password-[out] 解密后的密码
 *  @ return 加密后的密码
 */
PUBLIC void SoftToDog(TCHAR *res_password,TCHAR * password);
/** @ fn     THCAR2char
 *  @ brief  TCHAR转为Char
 *  @ param  tchStr-[in] 要转换的TCHAR
 *  @ param  temp-[in/out] 转换后的TCHAR

 */
PUBLIC void THCAR2char(TCHAR* tchStr,char *temp) ;

/** @ fn     char2THCAR
 *  @ brief  Char转为TCHAR
 *  @ param  tchStr-[in] 要转换的TCHAR
 *  @ param  temp-[in/out] 转换后的TCHAR

 */
PUBLIC void Char2THCAR(char *_char,TCHAR* temp); 