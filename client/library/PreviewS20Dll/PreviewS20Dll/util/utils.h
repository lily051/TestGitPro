/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.h
*   Description:    实用函数
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/
#pragma once
#include "TCharExpand.h"
#include <process.h>
#pragma comment(lib, "version.lib")


Image * LoadImageFromResource(UINT uID);
Bitmap * LoadBitmapFromResource(UINT uID);
HANDLE Create_File(TCHAR *csFileName);
void SetRegion(CWnd * pWnd, CBitmap & bitmap);


//ljg 2010/03/22  位图
HBITMAP ResourceLoad_Bmp(UINT nID);
int GetFontSize(int nPixelSize);
int GetFontSize0();
CString GetFontName();
int GetFontCharset();

//创建文件夹和文件
HANDLE CreateMyFile(const CString &strFileName);
/** @fn         SaveBinaryFile
 *  @brief      保存数据到指定文件
 *  @param[IN]  pData: 缓冲数据
 *  @param[IN]  nDataSize: 数据大小
 *  @param[IN]  strFilePath: 文件名
 *  @return     TRUE: 成功，FALSE: 失败
 */
BOOL SaveBinaryFile(PBYTE pData, DWORD nDataSize, const CString &strFilePath);
/** @fn         SuperMakeDir
 *  @brief      检查指定目录是否存在，如果不存在则创建整个strFilePath所表示的整个目
 *  @param[IN]  strFilePath: 文件的绝对路径
 *  @return     TRUE: 成功，FALSE: 失败
 */
BOOL SuperMakeDir(PCSTR strFilePath);
BOOL CreateFileDir(TCHAR *csFilePath);
/** @fn         CheckDiskSpace
 *  @brief      检查指定磁盘是否有可用空间
 *  @param[IN]  strDisk: 指定盘符
 *  @param[IN]  dwFreeSpace: 可用空间下限(MB为单位)
 *  @return     TRUE: 有，FALSE: 无
 */
BOOL CheckDiskSpace(const CString &strDisk, DWORD dwFreeSpace);

// dsx unicode utf8 cp936 字符编码和解码相关的函数---begin
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,    				 LPBOOL lpUsedDefaultChar,DWORD* pdwError);
bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
std::wstring CP936_A2W(const std::string& strA);
std::string CP936_W2A(const std::wstring& strW);
CStringA CStringW2A(CStringW strText);
std::string UTF8_MFCSTRW2A(const CStringW& strW);
CStringW UTF8_A2MFCSTRW(const std::string& strA);
void CodeSet_Free(void* p);
int My_atoi(CString strNum);

HWND MyCreateToolTip(int toolID, HWND hDlg,  TCHAR* pText);