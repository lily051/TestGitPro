// public.h : public DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "resource.h"		// ������

# if defined(PUBLIC_EXPORTS)
#    define PUBLIC extern  __declspec(dllexport)
#  else
#    define PUBLIC extern  __declspec(dllimport)
#  endif

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

/** @ fn     CString2Char
 *  @ brief  CStringתstr
 *  @ param  temp-[out] Ҫ������ַ���
 *  @ param  str-[in] Ҫת����CString
 *  @ return �ַ���ָ��
 */
PUBLIC void CString2Char(const CString & str,char *temp);

/** @ fn     Char2CString
 *  @ brief  charתCString
 *  @ param  str-[in] Ҫת����CString
 *  @ param  temp-[out] Ҫ�����CString
 *  @ return CString
 */
PUBLIC void Char2CString(char *temp,CString &str);

/** @ fn     Add
 *  @ brief  ����int���
 *  @ param  a-[in] ����a
 *  @ param  b-[in] ����b
 *  @ return �����������ĺ�
 */
PUBLIC int Add(const int &a,const int &b);
/** @ fn     TCHAR2Char
 *  @ brief  TCHAR����תCHAR
 *  @ param  tchar-[in] Ҫת����TCHAR
 *  @ param  _char-[in/out] ���벢�����CHARָ��
 *  @ return �����������ĺ�
 */
PUBLIC void TCHAR2Char(const TCHAR * tchar, char * _char) ;
/** @ fn     Char2TCHAR
 *  @ brief  CHAR����תTCHAR
 *  @ param  _char-[in] Ҫת����CHAR
 *  @ param  tchar-[in/out] ���벢�����TCHARָ��
 *  @ return �����������ĺ�
 */
PUBLIC void Char2TCHAR (const char * _char, TCHAR * tchar);

/** @ fn     SetConfig
 *  @ brief  ���������ļ�
 *  @ param  hModule-[in] ģ����
 *  @ param  configname-[in] �����ļ�����
 *  @ return �����������ĺ�
 */
PUBLIC CString SetConfig(HMODULE hModule,CString configname=TEXT("Config.ini"));
/** @ fn     GetConfig
 *  @ brief  ���������ļ�
 *  @ param  hModule-[in] ģ����
 *  @ param  configname-[in] �����ļ�����
 *  @ return �����������ĺ�
 */
PUBLIC CString GetConfig( HMODULE hModule,CString configname = TEXT("Config.ini"));

/** @ fn     SoftDog
 *  @ brief  �������
 *  @ param  password-[in] Ҫ���ܵ�����
 *  @ param  res_password-[out] ���ܺ������
 *  @ return ���ܺ������
 */
PUBLIC void SoftDog(CString const  password,TCHAR *res_password);

/** @ fn     SoftDog
 *  @ brief  �������
 *  @ param  res_password-[in] Ҫ���ܵ�����
 *  @ param  password-[out] ���ܺ������
 *  @ return ���ܺ������
 */
PUBLIC void SoftToDog(TCHAR *res_password,TCHAR * password);
/** @ fn     THCAR2char
 *  @ brief  TCHARתΪChar
 *  @ param  tchStr-[in] Ҫת����TCHAR
 *  @ param  temp-[in/out] ת�����TCHAR

 */
PUBLIC void THCAR2char(TCHAR* tchStr,char *temp) ;

/** @ fn     char2THCAR
 *  @ brief  CharתΪTCHAR
 *  @ param  tchStr-[in] Ҫת����TCHAR
 *  @ param  temp-[in/out] ת�����TCHAR

 */
PUBLIC void Char2THCAR(char *_char,TCHAR* temp); 